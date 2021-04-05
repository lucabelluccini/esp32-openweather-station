#ifndef DHT11_H
#define DHT11_H

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

typedef struct Dht11Data {
    bool error = false;
    bool initiated = false;
    float temperature = 0;
    float relative_humidity = 0;
} Dht11Data_t;

class Dht11
{
public:
    void setup(const uint8_t iDhtPin) {
        _dht.reset(new DHT_Unified(iDhtPin, DHT11));
        _dht->begin();
#ifdef DEBUG
        sensor_t sensor;
        _dht->temperature().getSensor(&sensor);
        USE_SERIAL.print(F("[DHT11] Sensor Type: "));
        USE_SERIAL.println(sensor.name);
        USE_SERIAL.print(F("[DHT11] Max Value °C: "));
        USE_SERIAL.println(sensor.max_value);
        USE_SERIAL.print(F("[DHT11] Min Value °C: "));
        USE_SERIAL.println(sensor.min_value);
        USE_SERIAL.print(F("[DHT11] Resolution °C: "));
        USE_SERIAL.println(sensor.resolution);
        _dht->humidity().getSensor(&sensor);
        USE_SERIAL.print(F("[DHT11] Sensor Type: "));
        USE_SERIAL.println(sensor.name);
        USE_SERIAL.print(F("[DHT11] Max Value %: "));
        USE_SERIAL.println(sensor.max_value);
        USE_SERIAL.print(F("[DHT11] Min Value %: "));
        USE_SERIAL.println(sensor.min_value);
        USE_SERIAL.print(F("[DHT11] Resolution %: "));
        USE_SERIAL.println(sensor.resolution);
#endif
    }
    bool readDht(Dht11Data_t& ioData)
    {
        ioData.error = true;
        ioData.initiated = false;
        if (_dht) {
            sensors_event_t event;
            _dht->temperature().getEvent(&event);
            if (isnan(event.temperature))
            {
#ifdef DEBUG
                USE_SERIAL.println(F("[DHT11] Error reading temperature!"));
#endif
            }
            else
            {
#ifdef DEBUG
                USE_SERIAL.print(F("[DHT11] Temperature °C: "));
                USE_SERIAL.println(event.temperature);
#endif
                ioData.temperature = event.temperature;
            }
            _dht->humidity().getEvent(&event);
            if (isnan(event.relative_humidity))
            {
#ifdef DEBUG
                USE_SERIAL.println(F("[DHT11] Error reading humidity!"));
#endif
            }
            else
            {
#ifdef DEBUG
                USE_SERIAL.print(F("[DHT11] Humidity %: "));
                USE_SERIAL.println(event.relative_humidity);
#endif
                ioData.relative_humidity = event.relative_humidity;
            }
            ioData.error = false;
            ioData.initiated = true;
        }
        else
        {
#ifdef DEBUG
            USE_SERIAL.println(F("[DHT11] Sensor not initialized!"));
#endif
            ioData.error = true;
        }
        return ioData.error;
    }

private:
    std::unique_ptr<DHT_Unified> _dht;
};

#endif