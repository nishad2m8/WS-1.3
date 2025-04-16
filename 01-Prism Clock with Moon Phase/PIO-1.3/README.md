# Instruction

Update the config.h file with your own WiFi credentials & Time Zone.


```c++
#ifndef CONFIG_H
#define CONFIG_H

// Primary WiFi credentials
const char* WIFI_SSID = "SSID";  // Change this to your WiFi network name
const char* WIFI_PASSWORD = "PASSWORD";  // Change this to your WiFi password

// Fallback WiFi credentials
const char* FALLBACK_WIFI_SSID = "SSID";  // Change this to your fallback network
const char* FALLBACK_WIFI_PASSWORD = "PASSWORD";  // Change to your fallback password

// POSIX timezone string for Arabian Standard Time (UTC+3)
const char* TIME_ZONE = "AST-3";

#endif // CONFIG_H
```

# Time Zone Configuration for NTP
This project uses NTP (Network Time Protocol) to synchronize time over the internet and adjusts the local time based on the configured time zone. The time zone is set using POSIX-style strings, allowing you to handle both standard and daylight saving time automatically.

Example Time Zones:
- `PST8PDT` - Pacific Standard Time with Daylight Saving Time
- `EST5EDT` - Eastern Standard Time with Daylight Saving Time
- `UTC` - Coordinated Universal Time
- `GMT` - Greenwich Mean Time
- `CET-1CEST`- Central European Time with Daylight Saving Time
- `IST-5:30` - Indian Standard Time (UTC+5:30)
- `JST-9` - Japan Standard Time (UTC+9)
- `CST6CDT` - Central Standard Time with Daylight Saving Time (US)
- `MST7MDT` - Mountain Standard Time with Daylight Saving Time (US)
- `PHT-8` - Philippine Time (UTC+8)
- `SGT-8` - Singapore Time (UTC+8)
- `MSK-3` - Moscow Time (UTC+3)
- `GMT+4` - UAE Time (UTC+4)
- `AST-3` - Arabian Standard Time (Bahrain, UTC+3)