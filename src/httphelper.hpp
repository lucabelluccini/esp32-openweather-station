#ifndef HTTPHELPER_H
#define HTTPHELPER_H

#include <ArduinoJson.h>
#include <HTTPClient.h>
/*
struct SpiRamAllocator
{
    void *allocate(size_t size)
    {
        return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
    }
    void deallocate(void *pointer)
    {
        heap_caps_free(pointer);
    }
};

using SpiRamJsonDocument = BasicJsonDocument<SpiRamAllocator>;
*/

bool httpGetHelper(const String &iUrl, const char *iCaCert, DynamicJsonDocument &ioData, const size_t iBufferSize)
{
    bool error = true;
    WiFiClientSecure client;
    if (iCaCert != NULL)
    {
        client.setCACert(iCaCert);
    }
    HTTPClient http;
    http.setTimeout(5000);
    http.setReuse(false);
#ifdef DEBUG
    USE_SERIAL.print(F("[HTTP] Request to: "));
    USE_SERIAL.println(iUrl.c_str());
#endif
    if (http.begin(client, iUrl))
    {
        const int httpCode = http.GET();
        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK)
            {
#ifdef DEBUG
                USE_SERIAL.println(F("[HTTP] Response: 200 OK"));
                USE_SERIAL.println(F("[JSON] Deserializing..."));
#endif
                DynamicJsonDocument rawJsonDoc(iBufferSize);
                DeserializationError jsonError = deserializeJson(rawJsonDoc, http.getString().c_str());
                if (jsonError)
                {
#ifdef DEBUG
                    USE_SERIAL.print(F("[JSON] Error deserializing error: "));
                    USE_SERIAL.println(jsonError.c_str());
#endif
                }
                else
                {
                    ioData = rawJsonDoc;
#ifdef DEBUG
                    USE_SERIAL.print(F("[JSON] Deserialized: "));
                    serializeJsonPretty(ioData, USE_SERIAL);
                    USE_SERIAL.println();
#endif
                    error = false;
                }
            }
            else
            {
#ifdef DEBUG
                USE_SERIAL.print(F("[HTTP] Response: "));
                USE_SERIAL.println(http.errorToString(httpCode).c_str());
#endif
            }
        }
        else
        {
#ifdef DEBUG
            USE_SERIAL.print(F("[HTTP] Error: "));
            USE_SERIAL.println(http.errorToString(httpCode).c_str());
#endif
        }
        http.end();
    }
    else
    {
#ifdef DEBUG
        USE_SERIAL.println(F("[HTTP] Error: unable to setup an HTTPS connection"));
#endif
    }
    return error;
}

#endif