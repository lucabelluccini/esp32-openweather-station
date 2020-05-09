#ifndef IPAPI_H
#define IPAPI_H

#include "httphelper.hpp"

const char *ipapiUrl = "https://ipapi.co/json";

const char *ipapiCaCert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n"
    "RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n"
    "VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n"
    "DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n"
    "ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n"
    "VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n"
    "mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n"
    "IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n"
    "mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n"
    "XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n"
    "dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n"
    "jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n"
    "BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n"
    "DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n"
    "9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n"
    "jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n"
    "Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n"
    "ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n"
    "R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n"
    "-----END CERTIFICATE-----\n"
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDozCCAougAwIBAgIQD/PmFjmqPRoSZfQfizTltjANBgkqhkiG9w0BAQsFADBa\n"
    "MQswCQYDVQQGEwJJRTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJl\n"
    "clRydXN0MSIwIAYDVQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTE1\n"
    "MTAxNDEyMDAwMFoXDTIwMTAwOTEyMDAwMFowbzELMAkGA1UEBhMCVVMxCzAJBgNV\n"
    "BAgTAkNBMRYwFAYDVQQHEw1TYW4gRnJhbmNpc2NvMRkwFwYDVQQKExBDbG91ZEZs\n"
    "YXJlLCBJbmMuMSAwHgYDVQQDExdDbG91ZEZsYXJlIEluYyBFQ0MgQ0EtMjBZMBMG\n"
    "ByqGSM49AgEGCCqGSM49AwEHA0IABNFW9Jy25DGg9aRSz+Oaeob/8oayXsy1WcwR\n"
    "x07dZP1VnGDjoEvZeFT/SFC6ouGhWHWPx2A3RBZNVZns7tQzeiOjggEZMIIBFTAS\n"
    "BgNVHRMBAf8ECDAGAQH/AgEAMA4GA1UdDwEB/wQEAwIBhjA0BggrBgEFBQcBAQQo\n"
    "MCYwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTA6BgNVHR8E\n"
    "MzAxMC+gLaArhilodHRwOi8vY3JsMy5kaWdpY2VydC5jb20vT21uaXJvb3QyMDI1\n"
    "LmNybDA9BgNVHSAENjA0MDIGBFUdIAAwKjAoBggrBgEFBQcCARYcaHR0cHM6Ly93\n"
    "d3cuZGlnaWNlcnQuY29tL0NQUzAdBgNVHQ4EFgQUPnQtH89FdQR+P8Cihz5MQ4NR\n"
    "E8YwHwYDVR0jBBgwFoAU5Z1ZMIJHWMys+ghUNoZ7OrUETfAwDQYJKoZIhvcNAQEL\n"
    "BQADggEBADhfp//8hfJzMuTVo4mZlmCvMsEDs2Xfvh4DyqXthbKPr0uMc48qjKkA\n"
    "DgEkF/fsUoV2yOUcecrDF4dQtgQzNp4qnhgXljISr0PMVxje28fYiCWD5coGJTH9\n"
    "vV1IO1EB3SwUx8FgUemVAdiyM1YOR2aNbM2v+YXZ6xxHR4g06PD6wqtPaU4JWdRX\n"
    "xszByOPmGcFYOFLi4oOF3iI03D+m968kvOBvwKtoLVLHawVXLEIbLUiHAwyQq0hI\n"
    "qSi+NIr7uu30YJkdFXgRqtltU39pKLy3ayB2f6BVA3F59WensKAKF1eyAKmtz/9n\n"
    "jD4m5ackvMJvEOiJxnCl0h+A7Q0/JxM=\n"
    "-----END CERTIFICATE-----\n"
    "-----BEGIN CERTIFICATE-----\n"
    "MIIE3zCCBISgAwIBAgIQC2idH27JvIXV+Bcfh1XWljAKBggqhkjOPQQDAjBvMQsw\n"
    "CQYDVQQGEwJVUzELMAkGA1UECBMCQ0ExFjAUBgNVBAcTDVNhbiBGcmFuY2lzY28x\n"
    "GTAXBgNVBAoTEENsb3VkRmxhcmUsIEluYy4xIDAeBgNVBAMTF0Nsb3VkRmxhcmUg\n"
    "SW5jIEVDQyBDQS0yMB4XDTE5MTExMzAwMDAwMFoXDTIwMTAwOTEyMDAwMFowbTEL\n"
    "MAkGA1UEBhMCVVMxCzAJBgNVBAgTAkNBMRYwFAYDVQQHEw1TYW4gRnJhbmNpc2Nv\n"
    "MRkwFwYDVQQKExBDbG91ZGZsYXJlLCBJbmMuMR4wHAYDVQQDExVzbmkuY2xvdWRm\n"
    "bGFyZXNzbC5jb20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAARC8ROUxLvTAf1l\n"
    "LX+/3qtOrOmgdlKK2wrQmVnPKd1LxqqbD9EweLw1elj9uWbFVWI1AVfYLh/zXcJ/\n"
    "w2SRSkILo4IDAjCCAv4wHwYDVR0jBBgwFoAUPnQtH89FdQR+P8Cihz5MQ4NRE8Yw\n"
    "HQYDVR0OBBYEFDf6MuGqA5qv5b5SkS256AKWX4paMDYGA1UdEQQvMC2CCGlwYXBp\n"
    "LmNvghVzbmkuY2xvdWRmbGFyZXNzbC5jb22CCiouaXBhcGkuY28wDgYDVR0PAQH/\n"
    "BAQDAgeAMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjB5BgNVHR8EcjBw\n"
    "MDagNKAyhjBodHRwOi8vY3JsMy5kaWdpY2VydC5jb20vQ2xvdWRGbGFyZUluY0VD\n"
    "Q0NBMi5jcmwwNqA0oDKGMGh0dHA6Ly9jcmw0LmRpZ2ljZXJ0LmNvbS9DbG91ZEZs\n"
    "YXJlSW5jRUNDQ0EyLmNybDBMBgNVHSAERTBDMDcGCWCGSAGG/WwBATAqMCgGCCsG\n"
    "AQUFBwIBFhxodHRwczovL3d3dy5kaWdpY2VydC5jb20vQ1BTMAgGBmeBDAECAjB2\n"
    "BggrBgEFBQcBAQRqMGgwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0\n"
    "LmNvbTBABggrBgEFBQcwAoY0aHR0cDovL2NhY2VydHMuZGlnaWNlcnQuY29tL0Ns\n"
    "b3VkRmxhcmVJbmNFQ0NDQS0yLmNydDAMBgNVHRMBAf8EAjAAMIIBBAYKKwYBBAHW\n"
    "eQIEAgSB9QSB8gDwAHYApLkJkLQYWBSHuxOizGdwCjw1mAT5G9+443fNDsgN3BAA\n"
    "AAFuZDSEcQAABAMARzBFAiBxESo7FtV1ptNNfVEW9dkbAMcReS3O3CXvgAUnUNi7\n"
    "EQIhALE+/0CBYH6jYUoIORdrNhobki5XsAwdrJHmvjbGupaJAHYAXqdz+d9WwOe1\n"
    "Nkh90EngMnqRmgyEoRIShBh1loFxRVgAAAFuZDSEYAAABAMARzBFAiBPQrPMDWMI\n"
    "X6Cemibg+7KSy0+w+X9iVTLV497rjyrOggIhAJZqKRDT0tGEpdwDpwHju+++ipw0\n"
    "nZTEFIGUDPberxLcMAoGCCqGSM49BAMCA0kAMEYCIQDe1Cp778XSDGUGG2GNYvp/\n"
    "utESI4yqq0bJKJcVa50JvwIhAJbzNTPxgHt+HmtoRmZWGXsk+d8WXyLlSrm2AV5S\n"
    "O9/e\n"
    "-----END CERTIFICATE-----\n";
    
typedef struct IpapiData
{
    bool error = false;
    bool initialized = false;
    double lat = 0.0f;
    double lon = 0.0f;
    String city;
    String country;
    DynamicJsonDocument raw = DynamicJsonDocument(1);
} IpapiData_t;

bool getDataFromIpapi(IpapiData_t &ioData)
{
    // Get it via http://arduinojson.org/assistant/
    static const size_t ipapiBufferSize = JSON_OBJECT_SIZE(25) + 490;
    bool error = true;
    error = httpGetHelper(ipapiUrl, ipapiCaCert, ioData.raw, ipapiBufferSize);
    if (!error)
    {
        ioData.lat = ioData.raw["latitude"];
        ioData.lon = ioData.raw["longitude"];
        ioData.city = String((const char *)ioData.raw["city"]);
        ioData.country = String((const char *)ioData.raw["country_name"]);
#ifdef DEBUG
        USE_SERIAL.print(F("[JSON] Decoded data: "));
        USE_SERIAL.printf("%f %f %s %s\n", ioData.lat, ioData.lon, ioData.city.c_str(), ioData.country.c_str());
#endif
        error = false;
        ioData.initialized = true;
    }
    ioData.error = error;
    return !error;
}

#endif