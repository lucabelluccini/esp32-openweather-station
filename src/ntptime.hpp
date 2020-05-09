#ifndef NTPTIME_H
#define NTPTIME_H

#include <time.h>

typedef struct TimeData {
    bool initialized = false;
    bool tzProvided = false;
    long gmtOffset_sec = 0L;
    int daylightOffset_sec = 0;
    String localTime;
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
        USE_SERIAL.print(F("[NTP] Waiting for NTP time sync"));
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
        USE_SERIAL.println();
#endif
        ioTimedata.initialized = true;
    }
}


void readTime(TimeData_t& ioTimedata, const char * iFormat)
{
    char timebuffer[17];
    struct tm timeinfo;
    time_t nowSecs = time(nullptr);
    gmtime_r(&nowSecs, &timeinfo);
    getLocalTime(&timeinfo);
    if(strftime(timebuffer, 17, iFormat, &timeinfo) != 0) {
        ioTimedata.localTime = String(timebuffer);
    }
    else {
        ioTimedata.localTime = "Date format error!";
    }
    
#ifdef DEBUG
    USE_SERIAL.print(F("[NTP] Current time (local) considering offset "));
    USE_SERIAL.printf("%ld is %s\n", ioTimedata.gmtOffset_sec, ioTimedata.localTime.c_str());
#endif
}

#endif