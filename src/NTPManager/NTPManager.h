#ifndef NTP_MANAGER_H
#define NTP_MANAGER_H

#include <time.h>

class NTPManager
{
public:

    NTPManager(const char *ntpServer = "de.pool.ntp.org", long gmtOffset_sec = 3600, int daylightOffset_sec = 3600);
    ~NTPManager();
    bool updateLocalTime(uint32_t timeout = 5000);
    time_t getCurrentTime();
    bool getLocalTime(struct tm *timeinfo, uint32_t timeout = 5000);

private:

    const char *_ntpServer;
    long _gmtOffset_sec;
    int _daylightOffset_sec;
};

#endif // NTP_MANAGER_H
