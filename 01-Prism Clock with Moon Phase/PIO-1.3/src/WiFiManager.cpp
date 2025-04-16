#include "WiFiManager.h"

WiFiManager::WiFiManager() 
    : _ssid(nullptr), 
      _password(nullptr),
      _fallbackSsid(nullptr),
      _fallbackPassword(nullptr),
      _apSsid("WS_Prism"),
      _apPassword(""),
      _apMode(false),
      _initialized(false),
      _usingFallback(false),
      _lastReconnectAttempt(0) {
}

bool WiFiManager::begin() {
    WiFi.mode(WIFI_STA);
    _initialized = true;
    return true;
}

void WiFiManager::setPrimaryWiFi(const char* ssid, const char* password) {
    _ssid = ssid;
    _password = password;
    Serial.println("Primary WiFi credentials set");
}

void WiFiManager::setFallbackWiFi(const char* fallbackSsid, const char* fallbackPassword) {
    _fallbackSsid = fallbackSsid;
    _fallbackPassword = fallbackPassword;
    Serial.println("Fallback WiFi credentials set");
}

void WiFiManager::setAPSettings(const char* apSsid, const char* apPassword) {
    if (apSsid != nullptr) {
        _apSsid = apSsid;
    }
    
    if (apPassword != nullptr) {
        _apPassword = apPassword;
    }
    Serial.println("AP settings configured");
}

bool WiFiManager::connect(uint32_t timeout) {
    if (!_initialized || _ssid == nullptr) {
        Serial.println("ERROR: WiFi not initialized or no primary SSID set");
        return false;
    }
    
    // Disconnect if already connected
    if (WiFi.status() == WL_CONNECTED) {
        WiFi.disconnect();
        delay(100);
    }
    
    Serial.print("Connecting to primary WiFi: ");
    Serial.println(_ssid);
    
    // Start connection attempt
    WiFi.begin(_ssid, _password);
    
    // Wait for connection with timeout
    uint32_t startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < timeout) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected to primary WiFi. IP: ");
        Serial.println(WiFi.localIP().toString());
        _apMode = false;
        _usingFallback = false;
        return true;
    } else {
        Serial.println("Failed to connect to primary WiFi");
        
        // Try fallback WiFi if available
        if (_fallbackSsid != nullptr) {
            if (connectToFallback(timeout)) {
                return true;
            }
        }
        
        // Start AP mode if all connection attempts fail
        if (timeout >= 30000) {  // Only start AP for initial connection failures
            return startAP();
        }
        return false;
    }
}

bool WiFiManager::connectToFallback(uint32_t timeout) {
    if (_fallbackSsid == nullptr) {
        Serial.println("ERROR: No fallback SSID configured");
        return false;
    }
    
    Serial.print("Trying fallback WiFi: ");
    Serial.println(_fallbackSsid);
    
    // Start connection attempt to fallback
    WiFi.begin(_fallbackSsid, _fallbackPassword);
    
    // Wait for connection with timeout
    uint32_t startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < timeout) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected to fallback WiFi. IP: ");
        Serial.println(WiFi.localIP().toString());
        _apMode = false;
        _usingFallback = true;
        return true;
    } else {
        Serial.println("Failed to connect to fallback WiFi");
        return false;
    }
}

bool WiFiManager::startAP() {
    WiFi.disconnect();
    delay(100);
    
    WiFi.mode(WIFI_AP);
    bool result = WiFi.softAP(_apSsid, _apPassword);
    
    if (result) {
        Serial.print("AP started. SSID: ");
        Serial.print(_apSsid);
        Serial.print(", IP: ");
        Serial.println(WiFi.softAPIP().toString());
        _apMode = true;
    } else {
        Serial.println("Failed to start AP");
    }
    
    return result;
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getLocalIP() {
    return WiFi.localIP().toString();
}

String WiFiManager::getAPIP() {
    return WiFi.softAPIP().toString();
}

void WiFiManager::handleEvents() {
    // This method could be expanded to handle WiFi events using WiFiEvent
}

void WiFiManager::handleFailedReconnect() {
    static uint8_t failCount = 0;
    failCount++;
    
    if (failCount >= 6) {  // After about a minute of failures (6 * 10s)
        startAP();
        failCount = 0;
    }
}

void WiFiManager::update() {
    if (!_initialized) return;
    
    // If in AP mode, try to reconnect to the WiFi periodically
    if (_apMode) {
        uint32_t currentTime = millis();
        if (currentTime - _lastReconnectAttempt > _reconnectInterval) {
            _lastReconnectAttempt = currentTime;
            
            // Try to connect in STA mode
            WiFi.mode(WIFI_STA);
            if (connect(5000)) {  // Short timeout for reconnection attempts
                // Successfully reconnected, shut down AP
                _apMode = false;
            } else {
                // Failed to reconnect, ensure AP is running
                WiFi.mode(WIFI_AP);
            }
        }
    } else if (!isConnected()) {
        // We were connected but lost connection, try to reconnect
        uint32_t currentTime = millis();
        if (currentTime - _lastReconnectAttempt > _reconnectInterval) {
            _lastReconnectAttempt = currentTime;
            Serial.println("Connection lost, trying to reconnect...");
            
            // Try primary WiFi first
            if (!_usingFallback) {
                // Try primary connection first
                if (!connect(5000)) {
                    // If primary fails and we have fallback, try that
                    if (_fallbackSsid != nullptr) {
                        if (!connectToFallback(5000)) {
                            handleFailedReconnect();
                        }
                    } else {
                        handleFailedReconnect();
                    }
                }
            } else {
                // We were on fallback, try to stay on it
                if (!connectToFallback(5000)) {
                    // If fallback fails, try primary
                    if (!connect(5000)) {
                        handleFailedReconnect();
                    }
                }
            }
        }
    }
}