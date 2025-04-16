#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

class WiFiManager {
public:
    WiFiManager();
    
    // Initialize WiFi with no credentials (configure separately)
    bool begin();
    
    // Configure primary and fallback WiFi networks
    void setPrimaryWiFi(const char* ssid, const char* password);
    void setFallbackWiFi(const char* fallbackSsid, const char* fallbackPassword);
    
    // Configure AP mode settings
    void setAPSettings(const char* apSsid, const char* apPassword = "");
    
    // Connect to configured networks with timeout
    bool connect(uint32_t timeout = 30000);
    
    // Try to connect to the fallback WiFi
    bool connectToFallback(uint32_t timeout = 30000);
    
    // Start AP mode with configured SSID and password
    bool startAP();
    
    // Check if connected to WiFi
    bool isConnected();
    
    // Get local IP address as string
    String getLocalIP();
    
    // Get AP IP address as string
    String getAPIP();
    
    // Handle WiFi events
    void handleEvents();
    
    // Update - call in loop to handle potential reconnects
    void update();
    
private:
    // Helper method to handle failed reconnection attempts
    void handleFailedReconnect();
    
    const char* _ssid;
    const char* _password;
    const char* _fallbackSsid;
    const char* _fallbackPassword;
    const char* _apSsid;
    const char* _apPassword;
    bool _apMode;
    bool _initialized;
    bool _usingFallback;
    uint32_t _lastReconnectAttempt;
    const uint32_t _reconnectInterval = 30000;  // 30 seconds between reconnection attempts
};

#endif // WIFI_MANAGER_H