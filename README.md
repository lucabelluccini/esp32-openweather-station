# esp32-openweather-station

This is my first attempt to use [PlatformIO](https://platformio.org/) to develop code for ESP32 with the Arduino framework.

Compared to the Arduino IDE, the big benefits are the total control of the build, the possibility to declare dependencies (and pin a specific version).

Requires:
- ESP32 (in my case a [esp32doit-devkit-v1](https://docs.platformio.org/en/latest/boards/espressif32/esp32doit-devkit-v1.html))
- A DHT11 sensor (with board)
- A 16x2 LCD Display with I2C driver PCF8574
- Subscribe to the free tier of [OpenWeathermap API](https://openweathermap.org/)

To pass the Wifi SSID and password and the Openweather API key, you can edit the `platformio.ini` file (`[common]` section) as those values are set as build flags.

![in-action](https://raw.githubusercontent.com/lucabelluccini/esp32-openweather-station/master/chrome_2020-05-09_18-28-24.png)

- [Demo video](https://www.youtube.com/watch?v=hGhArzygDM0)
