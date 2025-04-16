#include "TimeManager.h"

TimeManager::TimeManager() 
    : _ntpServer1("pool.ntp.org"), 
      _ntpServer2("time.nist.gov"), 
      _ntpServer3(nullptr),
      _timeZone("UTC0"),
      _initialized(false) {
}

bool TimeManager::begin(const char* timeZone) {
    if (timeZone) {
        _timeZone = timeZone;
    }
    
    // For Arabian Standard Time (UTC+3)
    // Configure time with GMT offset in seconds (3 hours = 10800 seconds)
    configTime(10800, 0, _ntpServer1, _ntpServer2, _ntpServer3);
    
    // Set timezone environment variable
    setenv("TZ", _timeZone, 1);
    tzset();
    
    Serial.print("DEBUG: Timezone set to ");
    Serial.println(_timeZone);
    
    _initialized = true;
    return syncTime();
}

bool TimeManager::syncTime() {
    if (!_initialized) {
        return false;
    }
    
    // Try to sync with NTP server
    configTime(10800, 0, _ntpServer1, _ntpServer2, _ntpServer3);
    
    // Wait up to 5 seconds for time to be set
    int retry = 0;
    const int maxRetries = 10;
    
    while (!isTimeSet() && retry < maxRetries) {
        delay(500);
        retry++;
    }
    
    if (isTimeSet()) {
        Serial.println("DEBUG: Time synchronized successfully");
        time_t now;
        time(&now);
        Serial.print("DEBUG: Epoch time: ");
        Serial.println(now);
    } else {
        Serial.println("DEBUG: Time synchronization failed");
    }
    
    return isTimeSet();
}

String TimeManager::getFormattedTime(const char* format) {
    char timeBuffer[64];
    time_t now;
    struct tm timeinfo;
    
    time(&now);
    localtime_r(&now, &timeinfo);
    
    strftime(timeBuffer, sizeof(timeBuffer), format, &timeinfo);
    return String(timeBuffer);
}

time_t TimeManager::getEpochTime() {
    time_t now;
    time(&now);
    return now;
}

void TimeManager::setNtpServers(const char* server1, const char* server2, const char* server3) {
    _ntpServer1 = server1;
    _ntpServer2 = server2;
    _ntpServer3 = server3;
    
    if (_initialized) {
        configTime(10800, 0, _ntpServer1, _ntpServer2, _ntpServer3);
    }
}

bool TimeManager::setTimeZone(const char* timeZone) {
    if (!timeZone) {
        return false;
    }
    
    _timeZone = timeZone;
    setenv("TZ", _timeZone, 1);
    tzset();
    
    return true;
}

bool TimeManager::isTimeSet() {
    time_t now = time(nullptr);
    return now > 1577836800; // 2020-01-01 00:00:00
}