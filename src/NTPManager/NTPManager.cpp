#include "NTPManager.h"
#include <WiFi.h>
#include <time.h>
#include <Arduino.h>

NTPManager::NTPManager(const char *ntpServer, long gmtOffset_sec, int daylightOffset_sec)
    : _ntpServer(ntpServer), _gmtOffset_sec(gmtOffset_sec), _daylightOffset_sec(daylightOffset_sec)
{

    configTime(_gmtOffset_sec, _daylightOffset_sec, _ntpServer);
}

NTPManager::~NTPManager()
{
    
}

bool NTPManager::updateLocalTime(uint32_t timeout)
{
    uint32_t start = millis();
    struct tm timeinfo;

    while (!getLocalTime(&timeinfo, timeout))
    {
        if (millis() - start > timeout)
        {
            return false;
        }
        delay(500);
    }
    return true;
}

time_t NTPManager::getCurrentTime()
{
    return time(NULL);
}

bool NTPManager::getLocalTime(struct tm *timeinfo, uint32_t timeout)
{

    uint32_t start = millis();
    time_t now = time(NULL);
    while (now < 1000000000)
    {
        if (millis() - start > timeout)
        {
            return false;
        }
        delay(500);
        now = time(NULL);
    }
    localtime_r(&now, timeinfo);
    return true;
}
