#ifndef LCD_H
#define LCD_H

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

#ifndef LCD_ADDR
#define LCD_ADDR 0x27
#endif

LiquidCrystal_PCF8574 lcd(LCD_ADDR);

// Generated using https://omerk.github.io/lcdchargen/
byte centigrades[8] = {
    0b00100,
    0b01010,
    0b00100,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000};
#define CENTIGRADES 1

#define MAX_BACKLIGHT 255

int brightness = MAX_BACKLIGHT;

void setMaxBacklightLcd()
{
    brightness = MAX_BACKLIGHT;
    lcd.setBacklight(brightness);
}

void fadeBacklightLcd()
{
    if(brightness > 0) {
        brightness -= 128;
    } else {
        brightness = 0;
    }
    lcd.setBacklight(brightness);
}

void setupLcd()
{
    Wire.begin();
    Wire.beginTransmission(LCD_ADDR);
    uint8_t error = Wire.endTransmission();
    if (error == 0)
    {
#ifdef DEBUG
        USE_SERIAL.println(F("[LCD] Found!"));
#endif
        lcd.begin(16, 2);
        lcd.createChar(CENTIGRADES, centigrades);
        setMaxBacklightLcd();
        lcd.home();
        lcd.clear();
    }
    else
    {
#ifdef DEBUG
        USE_SERIAL.println(F("[LCD] Not found!"));
#endif
    }
}

void writeToLcd(const char *line1, const char *line2)
{
    lcd.clear();
    lcd.home();
    if (line1 != NULL)
    {
#ifdef DEBUG
        USE_SERIAL.print("Line 1: ");
        USE_SERIAL.println(line1);
#endif
        lcd.setCursor(0,0);
        lcd.print((const char *)line1);
    }
    if (line2 != NULL)
    {
#ifdef DEBUG
        USE_SERIAL.print("Line 2: ");
        USE_SERIAL.println(line2);
#endif
        lcd.setCursor(0, 1);
        lcd.print((const char *)line2);
    }
    lcd.display();
}

//void writeToLcd()
//{
//  lcd.clear();
//  size_t dateOnlyOffset(krakenData.timestamp.length()-12);
//  lcd.printf("%12s[%c%c]", krakenData.timestamp.substring(dateOnlyOffset).c_str(), (krakenData.error? '!' : ' '), (krakenData.parsed? ' ' : '!'));
//  USE_SERIAL.printf("Timestamp shortened: %s\n", krakenData.timestamp.substring(dateOnlyOffset).c_str());
//  lcd.setCursor(0,1);
//  lcd.printf("%.0f %c%.0f %c%.0f", krakenData.lastValue, (uint8_t)ARROWDOWN, krakenData.lowValue, (uint8_t)ARROWUP, krakenData.highValue);
//}

#endif