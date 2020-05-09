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
int queueSize = 10;
hw_timer_t *timerLcd = NULL;
hw_timer_t *timerWeather = NULL;

enum class TaskType
{
  lcd,
  weather
};

void consumerTask(void *parameter)
{
  TaskType iReceivedTaskType;
  uint8_t lcdScreen(0);
  String line1, line2;
  for (;;)
  {
    if (xQueueReceive(queue, &iReceivedTaskType, portMAX_DELAY))
    {
      switch (iReceivedTaskType)
      {
      case TaskType::lcd:
      {
#ifdef DEBUG
        USE_SERIAL.println("[consumerTask] TaskType::lcd");
#endif
        line1 = line2 = "                ";
        switch (lcdScreen++)
        {
        case 0:
          if (timeData.initialized)
          {
            readTime(timeData, "%F %R");
          }
          dht11.readDht(dht11Data);
          if (dht11Data.initiated && !dht11Data.error)
          {
            line2 = "In: " + String(dht11Data.temperature, 0) + String((char)CENTIGRADES) + "C" + " " + String(dht11Data.relative_humidity, 0) + "%";
            writeToLcd(timeData.localTime.c_str(), line2.c_str());
          }
          break;
        case 1:
          if (weatherData.initialized && !weatherData.error)
          {
            line1 = "Out: " + String((double)weatherData.raw["main"]["temp"], 0) + String((char)CENTIGRADES) + "C" + " " + String((double)weatherData.raw["main"]["humidity"], 0) + "%";
            line2 = String((const char *)weatherData.raw["name"]) + ", " + String((const char *)weatherData.raw["sys"]["country"]);
            writeToLcd(line1.c_str(), line2.c_str());
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
            line1 = "Pa: " + String((double)weatherData.raw["main"]["pressure"], 0);
            line2 = "Wind: " + String((double)weatherData.raw["wind"]["speed"], 2) + " " + String((double)weatherData.raw["wind"]["deg"], 0);
            writeToLcd(line1.c_str(), line2.c_str());
          }
          break;
        case 4:
          if (weatherData.initialized && !weatherData.error)
          {
            line1 = "Vis: " + String((double)weatherData.raw["visibility"], 0);
            line2 = "Clouds: " + String((double)weatherData.raw["clouds"]["all"], 0);
            writeToLcd(line1.c_str(), line2.c_str());
          }
          break;
        default:
          lcdScreen = 0;
          break;
        }
        break;
      }
      case TaskType::weather:
      {
#ifdef DEBUG
        USE_SERIAL.println("[consumerTask] TaskType::weather");
#endif
        getDataFromWeather(ipapiData.lat, ipapiData.lon, weatherData);
        if (weatherData.initialized && !weatherData.error)
        {
          if (!timeData.tzProvided)
          {
            timeData.gmtOffset_sec = weatherData.timezone;
            timeData.initialized = false;
            timeData.tzProvided = true;
            syncTime(timeData);
          }
        }
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
  timerAlarmWrite(timerWeather, 1000000 * 60, true);
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
        timerAlarmEnable(timerLcd);
        timerAlarmEnable(timerWeather);
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
    resetData();
#ifdef DEBUG
    USE_SERIAL.println(F("[WIFI] Connecting..."));
#endif
    writeToLcd("Connecting Wifi", "Please wait...");
  }
  delay(1000 * 5);
}
