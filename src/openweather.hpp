#ifndef OPENWEATHER_H
#define OPENWEATHER_H

#include "httphelper.hpp"

#ifndef OPENWEATHER_APIKEY
#define OPENWEATHER_APIKEY "MY_OPENWEATHER_APIKEY"
#endif

//const char* weatherUrl = "https://api.openweathermap.org/data/2.5/onecall?lat={lat}&lon={lon}&units=metric&exclude=hourly,daily&appid={appid}";
const char *weatherUrl = "https://api.openweathermap.org/data/2.5/weather?units=metric&appid=";

// openssl s_client -connect api.openweathermap.org:443 -showcerts -verify 5
const char *weatherCaCert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIGvjCCBaagAwIBAgIRAKL7IEo7D+v9u0G4ItYCJYwwDQYJKoZIhvcNAQELBQAw\n"
    "gY8xCzAJBgNVBAYTAkdCMRswGQYDVQQIExJHcmVhdGVyIE1hbmNoZXN0ZXIxEDAO\n"
    "BgNVBAcTB1NhbGZvcmQxGDAWBgNVBAoTD1NlY3RpZ28gTGltaXRlZDE3MDUGA1UE\n"
    "AxMuU2VjdGlnbyBSU0EgRG9tYWluIFZhbGlkYXRpb24gU2VjdXJlIFNlcnZlciBD\n"
    "QTAeFw0yMDAzMTcwMDAwMDBaFw0yMjA2MTkwMDAwMDBaMB8xHTAbBgNVBAMMFCou\n"
    "b3BlbndlYXRoZXJtYXAub3JnMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKC\n"
    "AQEA2DMTq6QbiQ6N/PK6u6dv8J1w5/w/GLm1d7J3daL80/15qRlsxUEpM78/OWmE\n"
    "s60kKSfyOVyxOHrVoXMfEhIxATdYQtRtN2JQEFYDkRauvVgr5eXQO2EJZXBZUb2C\n"
    "0dLFMD2WtrQGl7059kCOBlA/vX2+uTIQwFx/qZyVKkhzgdthtoDQ5jDzx7scDM0U\n"
    "9c/be/aWNPzoJV1HK37luC0nHUyT0zDpXMt82DgoCRix9z9RzDNkyjsPW2qP/pOE\n"
    "RpXk0z49jOFqtUxTtR9HfbKoeQ/RobxD2fG5P1cfunZ2lU3lyl5PeKbmMlSdSlci\n"
    "4OuileGdauTqgU254X7bB/9iTQIDAQABo4IDgjCCA34wHwYDVR0jBBgwFoAUjYxe\n"
    "xFStiuF36Zv5mwXhuAGNYeEwHQYDVR0OBBYEFP2HTXuP9/WVxbHQk4RHPpXCLktU\n"
    "MA4GA1UdDwEB/wQEAwIFoDAMBgNVHRMBAf8EAjAAMB0GA1UdJQQWMBQGCCsGAQUF\n"
    "BwMBBggrBgEFBQcDAjBJBgNVHSAEQjBAMDQGCysGAQQBsjEBAgIHMCUwIwYIKwYB\n"
    "BQUHAgEWF2h0dHBzOi8vc2VjdGlnby5jb20vQ1BTMAgGBmeBDAECATCBhAYIKwYB\n"
    "BQUHAQEEeDB2ME8GCCsGAQUFBzAChkNodHRwOi8vY3J0LnNlY3RpZ28uY29tL1Nl\n"
    "Y3RpZ29SU0FEb21haW5WYWxpZGF0aW9uU2VjdXJlU2VydmVyQ0EuY3J0MCMGCCsG\n"
    "AQUFBzABhhdodHRwOi8vb2NzcC5zZWN0aWdvLmNvbTAzBgNVHREELDAqghQqLm9w\n"
    "ZW53ZWF0aGVybWFwLm9yZ4ISb3BlbndlYXRoZXJtYXAub3JnMIIB9gYKKwYBBAHW\n"
    "eQIEAgSCAeYEggHiAeAAdwBGpVXrdfqRIDC1oolp9PN9ESxBdL79SbiFq/L8cP5t\n"
    "RwAAAXDobGj8AAAEAwBIMEYCIQDuoxRU3qxvOhsXh/vQPwAzBQfmu0b76RYKY27r\n"
    "3IjeuwIhAKhiaG0C9WMqsBNviTNJHl8iUZppSoDbreFWKU3ju715AHYA36Veq2iC\n"
    "Tx9sre64X04+WurNohKkal6OOxLAIERcKnMAAAFw6GxoyAAABAMARzBFAiEAiPLZ\n"
    "oR9BVGbeBKcZWWCWe5khT1jrbwqFFs1qqciHhmUCICNPG3dRIueExiu3HF6tUiNb\n"
    "rlGF/mf9Efr3JkAkqGsZAHUAQcjKsd8iRkoQxqE6CUKHXk4xixsD6+tLx2jwkGKW\n"
    "BvYAAAFw6Gxo7AAABAMARjBEAiAzzodBqseRU0wn7ukh37SvTOjmv8vpayKuZ4AE\n"
    "ut06BAIgArnrQObBVZU87a6ubmSWGHPiEi8cyPYdqZkMVycT3TgAdgBvU3asMfAx\n"
    "GdiZAKRRFf93FRwR2QLBACkGjbIImjfZEwAAAXDobGnaAAAEAwBHMEUCIGo9M7aa\n"
    "TjzbYPbR16+gwPnAGNiZI0ujRTDXRUJsW+D8AiEAgexT/9i23R7/XZfh5sL1Q9E/\n"
    "pE40zy1wXC1O3BHvz2MwDQYJKoZIhvcNAQELBQADggEBANJ4pa0tYp5QOtGy1RxM\n"
    "hcX2WydaU89WwySUB41pxbXBvaRLQyFBzC/COjPyN6zR52irYeBr0uFLLmwkaZfg\n"
    "eavkaExosslVP9g1js4j7wAKR5CdlEJfgw4eTxu8LAx5WUhm66HaMQol2neSyky2\n"
    "XPZt4KvZC9Fk/0x28JpXbMpckpH1/VpWPz3ulQw1/9TgV0+saRpFaKVXoZT5IObo\n"
    "j6cAp85OGBmRNJFypFFZRvy85aPJCP8IIyNoC9MoZIQ2VEuXQMTrIDU14Y46BTDq\n"
    "HaolM6WQZl42iGBzqJcOF2PGzcZ5YUahZW1GMxwB3NCyugR93FMCwtM4Wip6Ja5Q\n"
    "5fs=\n"
    "-----END CERTIFICATE-----\n"
    "-----BEGIN CERTIFICATE-----\n"
    "MIIGEzCCA/ugAwIBAgIQfVtRJrR2uhHbdBYLvFMNpzANBgkqhkiG9w0BAQwFADCB\n"
    "iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n"
    "cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n"
    "BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTgx\n"
    "MTAyMDAwMDAwWhcNMzAxMjMxMjM1OTU5WjCBjzELMAkGA1UEBhMCR0IxGzAZBgNV\n"
    "BAgTEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UEBxMHU2FsZm9yZDEYMBYGA1UE\n"
    "ChMPU2VjdGlnbyBMaW1pdGVkMTcwNQYDVQQDEy5TZWN0aWdvIFJTQSBEb21haW4g\n"
    "VmFsaWRhdGlvbiBTZWN1cmUgU2VydmVyIENBMIIBIjANBgkqhkiG9w0BAQEFAAOC\n"
    "AQ8AMIIBCgKCAQEA1nMz1tc8INAA0hdFuNY+B6I/x0HuMjDJsGz99J/LEpgPLT+N\n"
    "TQEMgg8Xf2Iu6bhIefsWg06t1zIlk7cHv7lQP6lMw0Aq6Tn/2YHKHxYyQdqAJrkj\n"
    "eocgHuP/IJo8lURvh3UGkEC0MpMWCRAIIz7S3YcPb11RFGoKacVPAXJpz9OTTG0E\n"
    "oKMbgn6xmrntxZ7FN3ifmgg0+1YuWMQJDgZkW7w33PGfKGioVrCSo1yfu4iYCBsk\n"
    "Haswha6vsC6eep3BwEIc4gLw6uBK0u+QDrTBQBbwb4VCSmT3pDCg/r8uoydajotY\n"
    "uK3DGReEY+1vVv2Dy2A0xHS+5p3b4eTlygxfFQIDAQABo4IBbjCCAWowHwYDVR0j\n"
    "BBgwFoAUU3m/WqorSs9UgOHYm8Cd8rIDZsswHQYDVR0OBBYEFI2MXsRUrYrhd+mb\n"
    "+ZsF4bgBjWHhMA4GA1UdDwEB/wQEAwIBhjASBgNVHRMBAf8ECDAGAQH/AgEAMB0G\n"
    "A1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAbBgNVHSAEFDASMAYGBFUdIAAw\n"
    "CAYGZ4EMAQIBMFAGA1UdHwRJMEcwRaBDoEGGP2h0dHA6Ly9jcmwudXNlcnRydXN0\n"
    "LmNvbS9VU0VSVHJ1c3RSU0FDZXJ0aWZpY2F0aW9uQXV0aG9yaXR5LmNybDB2Bggr\n"
    "BgEFBQcBAQRqMGgwPwYIKwYBBQUHMAKGM2h0dHA6Ly9jcnQudXNlcnRydXN0LmNv\n"
    "bS9VU0VSVHJ1c3RSU0FBZGRUcnVzdENBLmNydDAlBggrBgEFBQcwAYYZaHR0cDov\n"
    "L29jc3AudXNlcnRydXN0LmNvbTANBgkqhkiG9w0BAQwFAAOCAgEAMr9hvQ5Iw0/H\n"
    "ukdN+Jx4GQHcEx2Ab/zDcLRSmjEzmldS+zGea6TvVKqJjUAXaPgREHzSyrHxVYbH\n"
    "7rM2kYb2OVG/Rr8PoLq0935JxCo2F57kaDl6r5ROVm+yezu/Coa9zcV3HAO4OLGi\n"
    "H19+24rcRki2aArPsrW04jTkZ6k4Zgle0rj8nSg6F0AnwnJOKf0hPHzPE/uWLMUx\n"
    "RP0T7dWbqWlod3zu4f+k+TY4CFM5ooQ0nBnzvg6s1SQ36yOoeNDT5++SR2RiOSLv\n"
    "xvcRviKFxmZEJCaOEDKNyJOuB56DPi/Z+fVGjmO+wea03KbNIaiGCpXZLoUmGv38\n"
    "sbZXQm2V0TP2ORQGgkE49Y9Y3IBbpNV9lXj9p5v//cWoaasm56ekBYdbqbe4oyAL\n"
    "l6lFhd2zi+WJN44pDfwGF/Y4QA5C5BIG+3vzxhFoYt/jmPQT2BVPi7Fp2RBgvGQq\n"
    "6jG35LWjOhSbJuMLe/0CjraZwTiXWTb2qHSihrZe68Zk6s+go/lunrotEbaGmAhY\n"
    "LcmsJWTyXnW0OMGuf1pGg+pRyrbxmRE1a6Vqe8YAsOf4vmSyrcjC8azjUeqkk+B5\n"
    "yOGBQMkKW+ESPMFgKuOXwIlCypTPRpgSabuY0MLTDXJLR27lk8QyKGOHQ+SwMj4K\n"
    "00u/I5sUKUErmgQfky3xxzlIPK1aEn8=\n"
    "-----END CERTIFICATE-----\n"
    "-----BEGIN CERTIFICATE-----\n"
    "MIIFdzCCBF+gAwIBAgIQE+oocFv07O0MNmMJgGFDNjANBgkqhkiG9w0BAQwFADBv\n"
    "MQswCQYDVQQGEwJTRTEUMBIGA1UEChMLQWRkVHJ1c3QgQUIxJjAkBgNVBAsTHUFk\n"
    "ZFRydXN0IEV4dGVybmFsIFRUUCBOZXR3b3JrMSIwIAYDVQQDExlBZGRUcnVzdCBF\n"
    "eHRlcm5hbCBDQSBSb290MB4XDTAwMDUzMDEwNDgzOFoXDTIwMDUzMDEwNDgzOFow\n"
    "gYgxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpOZXcgSmVyc2V5MRQwEgYDVQQHEwtK\n"
    "ZXJzZXkgQ2l0eTEeMBwGA1UEChMVVGhlIFVTRVJUUlVTVCBOZXR3b3JrMS4wLAYD\n"
    "VQQDEyVVU0VSVHJ1c3QgUlNBIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MIICIjAN\n"
    "BgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAgBJlFzYOw9sIs9CsVw127c0n00yt\n"
    "UINh4qogTQktZAnczomfzD2p7PbPwdzx07HWezcoEStH2jnGvDoZtF+mvX2do2NC\n"
    "tnbyqTsrkfjib9DsFiCQCT7i6HTJGLSR1GJk23+jBvGIGGqQIjy8/hPwhxR79uQf\n"
    "jtTkUcYRZ0YIUcuGFFQ/vDP+fmyc/xadGL1RjjWmp2bIcmfbIWax1Jt4A8BQOujM\n"
    "8Ny8nkz+rwWWNR9XWrf/zvk9tyy29lTdyOcSOk2uTIq3XJq0tyA9yn8iNK5+O2hm\n"
    "AUTnAU5GU5szYPeUvlM3kHND8zLDU+/bqv50TmnHa4xgk97Exwzf4TKuzJM7UXiV\n"
    "Z4vuPVb+DNBpDxsP8yUmazNt925H+nND5X4OpWaxKXwyhGNVicQNwZNUMBkTrNN9\n"
    "N6frXTpsNVzbQdcS2qlJC9/YgIoJk2KOtWbPJYjNhLixP6Q5D9kCnusSTJV882sF\n"
    "qV4Wg8y4Z+LoE53MW4LTTLPtW//e5XOsIzstAL81VXQJSdhJWBp/kjbmUZIO8yZ9\n"
    "HE0XvMnsQybQv0FfQKlERPSZ51eHnlAfV1SoPv10Yy+xUGUJ5lhCLkMaTLTwJUdZ\n"
    "+gQek9QmRkpQgbLevni3/GcV4clXhB4PY9bpYrrWX1Uu6lzGKAgEJTm4Diup8kyX\n"
    "HAc/DVL17e8vgg8CAwEAAaOB9DCB8TAfBgNVHSMEGDAWgBStvZh6NLQm9/rEJlTv\n"
    "A73gJMtUGjAdBgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/\n"
    "BAQDAgGGMA8GA1UdEwEB/wQFMAMBAf8wEQYDVR0gBAowCDAGBgRVHSAAMEQGA1Ud\n"
    "HwQ9MDswOaA3oDWGM2h0dHA6Ly9jcmwudXNlcnRydXN0LmNvbS9BZGRUcnVzdEV4\n"
    "dGVybmFsQ0FSb290LmNybDA1BggrBgEFBQcBAQQpMCcwJQYIKwYBBQUHMAGGGWh0\n"
    "dHA6Ly9vY3NwLnVzZXJ0cnVzdC5jb20wDQYJKoZIhvcNAQEMBQADggEBAJNl9jeD\n"
    "lQ9ew4IcH9Z35zyKwKoJ8OkLJvHgwmp1ocd5yblSYMgpEg7wrQPWCcR23+WmgZWn\n"
    "RtqCV6mVksW2jwMibDN3wXsyF24HzloUQToFJBv2FAY7qCUkDrvMKnXduXBBP3zQ\n"
    "YzYhBx9G/2CkkeFnvN4ffhkUyWNnkepnB2u0j4vAbkN9w6GAbLIevFOFfdyQoaS8\n"
    "Le9Gclc1Bb+7RrtubTeZtv8jkpHGbkD4jylW6l/VXxRTrPBPYer3IsynVgviuDQf\n"
    "Jtl7GQVoP7o81DgGotPmjw7jtHFtQELFhLRAlSv0ZaBIefYdgWOWnU914Ph85I6p\n"
    "0fKtirOMxyHNwu8=\n"
    "-----END CERTIFICATE-----\n";

typedef struct WeatherData
{
    bool error = false;
    bool initialized = false;
    long dt = 0; // would be better unsigned long
    long timezone = 0;
    DynamicJsonDocument raw = DynamicJsonDocument(1);
} WeatherData_t;

bool getDataFromWeather(const double latitude, const double longitude, WeatherData_t &ioData, long &ioTimezoneOffset)
{
    // Get it via http://arduinojson.org/assistant/
    static const size_t weatherBufferSize(JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13) + 280);
    ioData.initialized = false;
    ioData.error = false;
    String url = String(weatherUrl) + OPENWEATHER_APIKEY + "&lat=" + String((double)latitude, 3) + "&lon=" + String((double)longitude, 3);
    ioData.error = httpGetHelper(url, weatherCaCert, ioData.raw, weatherBufferSize);
    if (!ioData.error)
    {
#ifdef DEBUG
        USE_SERIAL.println(F("[JSON] Weather Deserialized!"));
#endif
        ioData.dt = (long)ioData.raw["dt"];
        ioData.timezone = (long)ioData.raw["timezone"];
#ifdef DEBUG
        USE_SERIAL.print(F("[JSON] Weather Decoded data: "));
        USE_SERIAL.printf("%ld %ld\n", ioData.dt, ioData.timezone);
#endif
        ioData.error = false;
        ioData.initialized = true;
        ioTimezoneOffset = ioData.timezone;
    }
    else
    {
        ioData.initialized = true;
        ioData.error = true;
#ifdef DEBUG
        USE_SERIAL.print(F("[JSON] Error occurred when getting weather!"));
#endif
    }
    
    return ioData.error;
}

#endif