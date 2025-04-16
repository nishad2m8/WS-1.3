#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <Arduino.h>
#include <time.h>

class TimeManager {
public:
    TimeManager();
    
    // Initialize with timezone
    bool begin(const char* timeZone);
    
    // Synchronize time with NTP servers
    bool syncTime();
    
    // Get current formatted time string
    String getFormattedTime(const char* format = "%Y-%m-%d %H:%M:%S");
    
    // Get current Unix timestamp
    time_t getEpochTime();
    
    // Set NTP servers
    void setNtpServers(const char* server1, const char* server2 = nullptr, const char* server3 = nullptr);
    
    // Set timezone using POSIX format string
    bool setTimeZone(const char* timeZone);
    
    // Check if time is synchronized (moved to public)
    bool isTimeSet();
    
private:
    const char* _ntpServer1;
    const char* _ntpServer2;
    const char* _ntpServer3;
    const char* _timeZone;
    bool _initialized;
};

#endif // TIME_MANAGER_H