#ifndef NTPTIME_H
#define NTPTIME_H

#include <time.h>

typedef struct TimeData {
    bool initialized = false;
    long gmtOffset_sec = 0L;
    int daylightOffset_sec = 0;
    char localTime[32];
} TimeData_t;

void syncTime(TimeData_t& ioTimedata)
{
    time_t nowSecs = time(nullptr);
    if (ioTimedata.initialized)
    {
#ifdef DEBUG
        USE_SERIAL.println(F("[NTP] Skipping sync"));
#endif
    }
    else
    {
        configTime(ioTimedata.gmtOffset_sec, ioTimedata.daylightOffset_sec, "pool.ntp.org", "time.nist.gov");
#ifdef DEBUG
        USE_SERIAL.print(F("[NTP] Waiting for NTP time sync with GMT Offset:"));
        USE_SERIAL.print(ioTimedata.gmtOffset_sec);
#endif
        while (nowSecs < 8 * 3600 * 2)
        {
            delay(500);
#ifdef DEBUG
            USE_SERIAL.print(F("."));
#endif
            yield();
            nowSecs = time(nullptr);
        }
#ifdef DEBUG
        USE_SERIAL.println("");
        USE_SERIAL.println(F("[NTP] NTP time sync done"));
#endif
        ioTimedata.initialized = true;
    }
}


void readTime(TimeData_t& ioTimedata, const char iFormat[])
{
    struct tm timeinfo;
    time_t nowSecs = time(nullptr);
    gmtime_r(&nowSecs, &timeinfo);
    getLocalTime(&timeinfo);
    memset(ioTimedata.localTime, 0, sizeof(ioTimedata.localTime));
    if(strftime(&ioTimedata.localTime[0], sizeof(ioTimedata.localTime), iFormat, &timeinfo) == 0) {
        #ifdef DEBUG
            USE_SERIAL.print(F("[NTP] Wrong format, will print garbage"));
        #endif
    }
    
#ifdef DEBUG
    USE_SERIAL.print(F("[NTP] Current time (local) considering offset "));
    USE_SERIAL.printf("%ld is %s\n", ioTimedata.gmtOffset_sec, ioTimedata.localTime);
#endif
}

#endif