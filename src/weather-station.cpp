#include <Arduino.h>
#include <WiFiMulti.h>

#include "ntptime.hpp"
#include "lcd.hpp"
#include "dht11.hpp"
#include "openweather.hpp"
#include "ipapi.hpp"

#ifndef WIFI_SSID
#define WIFI_SSID "MYSSID"
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "MYPASSWORD"
#endif
#ifndef DHT11_PIN
#define DHT11_PIN 4
#endif

// Pinout ESP32Wroom Doit V1 https://github.com/playelek/pinout-doit-32devkitv1

WiFiMulti wifiMulti;

bool isClockSet(false);
WeatherData_t weatherData;
IpapiData_t ipapiData;
Dht11 dht11;
Dht11Data_t dht11Data;
TimeData_t timeData;

void resetData()
{
  ipapiData.initialized = false;
  weatherData.initialized = false;
  timeData.initialized = false;
}

QueueHandle_t queue;
int queueSize = 16;
hw_timer_t *timerLcd = NULL;
hw_timer_t *timerWeather = NULL;
hw_timer_t *timerLocalWeather = NULL;

enum class TaskType
{
  lcd,
  weather,
  localWeather
};

void consumerTask(void *parameter)
{
  TaskType iReceivedTaskType;
  uint8_t lcdScreen(0);
  char lineBuffer1[24];
  char lineBuffer2[24];
  memset(lineBuffer1, 0, sizeof(lineBuffer1));
  memset(lineBuffer2, 0, sizeof(lineBuffer2));
  for (;;)
  {
    if (xQueueReceive(queue, &iReceivedTaskType, portMAX_DELAY))
    {
      switch (iReceivedTaskType)
      {
      case TaskType::lcd:
      {
#ifdef DEBUG
        USE_SERIAL.printf("[consumerTask] TaskType::lcd %u type\n", lcdScreen);
#endif

        switch (lcdScreen++)
        {
        case 0:
          if (timeData.initialized)
          {
            readTime(timeData, "%F %R");
          }
          if (dht11Data.initiated && !dht11Data.error)
          {
            sprintf(lineBuffer2, "In: %2.0fC %2.0f %%", dht11Data.temperature, dht11Data.relative_humidity);
          }
          writeToLcd(timeData.localTime, lineBuffer2);
          break;
        case 1:
          if (weatherData.initialized && !weatherData.error)
          {
            // (byte)CENTIGRADES
            sprintf(lineBuffer1, "Out: %2.0f C %2.0f %%", (double)weatherData.raw["main"]["temp"], (double)weatherData.raw["main"]["humidity"]);
            sprintf(lineBuffer2, "%s, %s", (const char*)weatherData.raw["name"], (const char*)weatherData.raw["sys"]["country"]);
            writeToLcd(lineBuffer1, lineBuffer2);
          }
          break;
        case 2:
          if (weatherData.initialized && !weatherData.error)
          {
            writeToLcd((const char *)weatherData.raw["weather"][0]["main"], (const char *)weatherData.raw["weather"][0]["description"]);
          }
          break;
        case 3:
          if (weatherData.initialized && !weatherData.error)
          {
            sprintf(lineBuffer1, "Pa: %.0f", (double)weatherData.raw["main"]["pressure"]);
            sprintf(lineBuffer2, "Wind: %.2f %.0f", (double)weatherData.raw["wind"]["speed"], (double)weatherData.raw["wind"]["deg"]);
            writeToLcd(lineBuffer1, lineBuffer2);
          }
          break;
        case 4:
          if (weatherData.initialized && !weatherData.error)
          {
            sprintf(lineBuffer1, "Vis: %.0f", (double)weatherData.raw["visibility"]);
            sprintf(lineBuffer2, "Clouds: %.0f", (double)weatherData.raw["clouds"]["all"]);
            writeToLcd(lineBuffer1, lineBuffer2);
          }
          break;
        default:
          
          break;
        }
        if (lcdScreen > 4)
        {
          lcdScreen = 0;
        }
        break;
      }
      case TaskType::weather:
      {
#ifdef DEBUG
        USE_SERIAL.println("[consumerTask] TaskType::weather");
#endif
        getDataFromWeather(ipapiData.lat, ipapiData.lon, weatherData, timeData.gmtOffset_sec);
        break;
      }
      case TaskType::localWeather:
      {
#ifdef DEBUG
        USE_SERIAL.println("[consumerTask] TaskType::localWeather");
#endif
        dht11.readDht(dht11Data);
        break;
      }
      default:
#ifdef DEBUG
        USE_SERIAL.println("[consumerTask] Should never happen!");
#endif
        break;
      }
    }
  }
  vTaskDelete(NULL);
}

void IRAM_ATTR onTimerLcd()
{
  TaskType lcdTT(TaskType::lcd);
  xQueueSendFromISR(queue, &lcdTT, NULL);
}

void IRAM_ATTR onTimerWeather()
{
  TaskType weatherTT(TaskType::weather);
  xQueueSendFromISR(queue, &weatherTT, NULL);
}

void IRAM_ATTR onTimerLocalWeather()
{
  TaskType localWeatherTT(TaskType::localWeather);
  xQueueSendFromISR(queue, &localWeatherTT, NULL);
}

void setup()
{
#ifdef DEBUG
  USE_SERIAL.begin(115200);
#endif
  dht11.setup(DHT11_PIN);
  // Wifi could be handled with events as in https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiClientEvents/WiFiClientEvents.ino
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  setupLcd();
  resetData();

  queue = xQueueCreate(queueSize, sizeof(int));
  if (queue == NULL)
  {
#ifdef DEBUG
    USE_SERIAL.println("[QUEUE] Failed creating queue");
#endif
  }

  xTaskCreate(consumerTask, "consumerTask", 10000, NULL, 1, NULL);
  timerLcd = timerBegin(0, 80, true);
  timerAttachInterrupt(timerLcd, &onTimerLcd, true);
  timerAlarmWrite(timerLcd, 1000000 * 10, true);
  timerWeather = timerBegin(1, 80, true);
  timerAttachInterrupt(timerWeather, &onTimerWeather, true);
  timerAlarmWrite(timerWeather, 1000000 * 60 * 15, true);
  timerLocalWeather = timerBegin(2, 80, true);
  timerAttachInterrupt(timerLocalWeather, &onTimerLocalWeather, true);
  timerAlarmWrite(timerLocalWeather, 1000000 * 60, true);
}

void loop()
{
  if ((wifiMulti.run() == WL_CONNECTED))
  {
    if (!ipapiData.initialized)
    {
      syncTime(timeData);
      writeToLcd("Getting location", "Please wait...");
      getDataFromIpapi(ipapiData);
      if (ipapiData.initialized && !ipapiData.error)
      {
        dht11.readDht(dht11Data);
        getDataFromWeather(ipapiData.lat, ipapiData.lon, weatherData, timeData.gmtOffset_sec);
        timeData.initialized = false;
        syncTime(timeData);
        timerAlarmEnable(timerWeather);
        timerAlarmEnable(timerLocalWeather);
        timerAlarmEnable(timerLcd);
      }
      else
      {
        writeToLcd("No location!", "An error occurred.");
      }
    }
  }
  else
  {
    timerAlarmDisable(timerLcd);
    timerAlarmDisable(timerWeather);
    timerAlarmDisable(timerLocalWeather);
    resetData();
#ifdef DEBUG
    USE_SERIAL.println(F("[WIFI] Connecting..."));
#endif
    writeToLcd("Connecting Wifi", "Please wait...");
  }
  delay(1000 * 5);
}
