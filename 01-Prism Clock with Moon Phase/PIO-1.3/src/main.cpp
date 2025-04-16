#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

// LVGL and display includes
#include "lvgl_setup.h"
#include "WS_QMI8658.h"
#include "ui.h"  // LVGL UI initialization (e.g., from SquareLine Studio)
#include "pixel_refresh.h"

// Add WiFi and time includes
#include "WiFiManager.h"
#include "TimeManager.h"
#include "MoonPhaseManager.h" 
#include "config.h"

// https://www.youtube.com/nishad2m8
// https://buymeacoffee.com/nishad2m8

// Create manager instances
WiFiManager wifiManager;
TimeManager timeManager;

// UI Elements
extern lv_obj_t* ui_Label_HH;    // Hour label
extern lv_obj_t* ui_Label_MM;    // Minute label
extern lv_obj_t* ui_Label_Colon; // Colon label
extern lv_obj_t* ui_Label_Abbr;  // AM/PM label

// Global time tracking variables
int currentHour = 0;
int currentMinute = 0;
bool colonVisible = true;
unsigned long lastColonToggle = 0;
const unsigned long COLON_BLINK_INTERVAL = 500; // 500ms for 1Hz blink

// Create instance of MoonPhaseManager
MoonPhaseManager moonPhaseManager;

// External declarations of UI elements for moon phase
extern lv_obj_t* ui_Label_Phase;
extern lv_obj_t* ui_Image_Moon;

// External declaration of moon images array
extern const lv_img_dsc_t *ui_imgset_moon_frame_[30];

// Global variables to track last moon data update
unsigned long lastMoonDataUpdate = 0;
const unsigned long MOON_DATA_UPDATE_INTERVAL = 6 * 3600000; // Update every 6 hours

// Global variables for screen tracking
extern lv_obj_t* ui_Screen_Time;      // Main time screen
extern lv_obj_t* ui_Screen_MoonPhase; // Moon phase screen
lv_obj_t* currentScreen;              // Track current active screen

// Screen change threshold and debounce
const float ROTATION_THRESHOLD = 2.5;  // 0.5 Adjust based on sensitivity needs
unsigned long lastScreenChange = 0;
const unsigned long SCREEN_CHANGE_DEBOUNCE = 1000; // 1 second debounce

// Function prototypes
int convertTo12Hour(int hour);
bool isTimeAM(int hour);

// Loading screen timer variables
static lv_timer_t* loading_timer = NULL;
const uint32_t LOADING_DURATION = 5000; // 5 seconds in milliseconds

// Convert 24hr time to 12hr format
int convertTo12Hour(int hour) {
    if (hour == 0) return 12;  // 00:00 -> 12:00 AM
    if (hour > 12) return hour - 12;
    return hour;
}

// Determine if time is AM or PM
bool isTimeAM(int hour) {
    return hour < 12;
}

// Function to update time display in 12-hour format
void updateTimeDisplay() {
    // Check if time is set by checking the epoch time
    time_t now = timeManager.getEpochTime();
    if (now < 1577836800) { // 2020-01-01 00:00:00
        Serial.println("DEBUG: Time not set yet");
        return;
    }
    
    // Get current time as tm struct for easier access to components
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    
    // Get time components
    int hour24 = timeinfo.tm_hour;
    int minutes = timeinfo.tm_min;
    int seconds = timeinfo.tm_sec;
    int day = timeinfo.tm_mday;
    int month = timeinfo.tm_mon;  // 0-based (0 = January)
    int year = timeinfo.tm_year + 1900;  // tm_year is years since 1900
    int weekday = timeinfo.tm_wday;  // 0 = Sunday, 1 = Monday, etc.
    
    // Get formatted time string
    String timeStr = timeManager.getFormattedTime("%H:%M:%S");
    Serial.print("DEBUG: Current time (24h): ");
    Serial.println(timeStr);
    
    // Convert to 12-hour format
    int hour12 = convertTo12Hour(hour24);
    bool isAM = isTimeAM(hour24);
    
    Serial.print("DEBUG: 12h format - ");
    Serial.print(hour12);
    Serial.print(":");
    Serial.print(minutes < 10 ? "0" : "");
    Serial.print(minutes);
    Serial.println(isAM ? " AM" : " PM");
    
    // Only update other UI elements if values have changed
    if (hour12 != currentHour || minutes != currentMinute) {
        currentHour = hour12;
        currentMinute = minutes;
        
        // Update hour with leading zero
        char hourStr[3];
        sprintf(hourStr, "%02d", hour12);
        lv_label_set_text(ui_Label_HH, hourStr);
        
        // Update minute
        char minStr[3];
        sprintf(minStr, "%02d", minutes);
        lv_label_set_text(ui_Label_MM, minStr);
        
        // Update AM/PM
        lv_label_set_text(ui_Label_Abbr, isAM ? "AM" : "PM");
        lv_label_set_text(ui_Label_Abbr2, isAM ? "AM" : "PM");
        
        Serial.println("DEBUG: Time display updated");
    }
    
    // 3-letter abbreviations for days of week
    static const char* weekdaysAbbr[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    lv_label_set_text(ui_Label_Day, weekdaysAbbr[weekday]);
    
    // Day of month
    char dayStr[3];
    sprintf(dayStr, "%d", day);
    lv_label_set_text(ui_Label_Date, dayStr);
    
    // 3-letter abbreviations for months
    static const char* monthsAbbr[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", 
                                      "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
    lv_label_set_text(ui_Label_Month, monthsAbbr[month]);
    
    Serial.print("DEBUG: Date updated - ");
    Serial.print(weekdaysAbbr[weekday]);
    Serial.print(", ");
    Serial.print(monthsAbbr[month]);
    Serial.print(" ");
    Serial.println(day);
}

// Function to blink colon every second
void updateColonBlink() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - lastColonToggle >= COLON_BLINK_INTERVAL) {
        lastColonToggle = currentMillis;
        colonVisible = !colonVisible;
        
        // Update the separate colon label
        lv_label_set_text(ui_Label_Colon, colonVisible ? ":" : " ");
        
        // Also update the combined time label with the same colon state
        char timeHHMM[6];
        if (colonVisible) {
            sprintf(timeHHMM, "%02d:%02d", currentHour, currentMinute);
        } else {
            sprintf(timeHHMM, "%02d %02d", currentHour, currentMinute);
        }
        lv_label_set_text(ui_Label_Time, timeHHMM);
    }
}

// Update direction using the gyroscope and cycle between screens
void updateDirectionFromGyro() {
    // Get accelerometer data
    float accelX = QMI8658_get_A_fx();  // Read X-axis acceleration
    float accelY = QMI8658_get_A_fy();  // Read Y-axis acceleration
    
    // Detect significant rotation
    unsigned long currentMillis = millis();
    if (currentMillis - lastScreenChange >= SCREEN_CHANGE_DEBOUNCE) {
        // Check if rotated left (positive X acceleration)
        if (accelX > ROTATION_THRESHOLD) {
            // If we're on Time screen, go to Moon Phase
            if (currentScreen == ui_Screen_Time) {
                Serial.println("DEBUG: Left rotation detected, switching to Moon Phase screen");
                lv_scr_load(ui_Screen_MoonPhase);
                currentScreen = ui_Screen_MoonPhase;
            } 
            // If we're on Moon Phase screen, go to Time
            else if (currentScreen == ui_Screen_MoonPhase) {
                Serial.println("DEBUG: Left rotation detected, switching to Time screen");
                lv_scr_load(ui_Screen_Time);
                currentScreen = ui_Screen_Time;
            }
            lastScreenChange = currentMillis;
        }
        // Check if rotated right (negative X acceleration)
        else if (accelX < -ROTATION_THRESHOLD) {
            // If we're on Time screen, go to Moon Phase
            if (currentScreen == ui_Screen_Time) {
                Serial.println("DEBUG: Right rotation detected, switching to Moon Phase screen");
                lv_scr_load(ui_Screen_MoonPhase);
                currentScreen = ui_Screen_MoonPhase;
            } 
            // If we're on Moon Phase screen, go to Time
            else if (currentScreen == ui_Screen_MoonPhase) {
                Serial.println("DEBUG: Right rotation detected, switching to Time screen");
                lv_scr_load(ui_Screen_Time);
                currentScreen = ui_Screen_Time;
            }
            lastScreenChange = currentMillis;
        }
    }
}

//-----------------------------------------------------
// Moon Phase Management
//-----------------------------------------------------

// Function to get current date in YYYY-MM-DD format from TimeManager
String getCurrentDateFormatted() {
    time_t now = timeManager.getEpochTime();
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    
    char dateBuffer[11]; // YYYY-MM-DD + null terminator
    sprintf(dateBuffer, "%04d-%02d-%02d", 
            timeinfo.tm_year + 1900, 
            timeinfo.tm_mon + 1, 
            timeinfo.tm_mday);
    
    return String(dateBuffer);
}

// Function to get current time in HH:MM format from TimeManager
String getCurrentTime24hr() {
    time_t now = timeManager.getEpochTime();
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    
    char timeBuffer[6]; // HH:MM + null terminator
    sprintf(timeBuffer, "%02d:%02d", 
            timeinfo.tm_hour, 
            timeinfo.tm_min);
    
    return String(timeBuffer);
}

// Update moon phase data
void updateMoonPhaseData() {
    if (!timeManager.isTimeSet()) {
        Serial.println("DEBUG: Time not set, skipping moon phase update");
        return;
    }
    
    String currentDate = getCurrentDateFormatted();
    String currentTime = getCurrentTime24hr();
    String location = "Dubai"; // Default location - change as needed
    
    Serial.println("DEBUG: Updating moon phase data");
    Serial.print("Current date: ");
    Serial.println(currentDate);
    Serial.print("Current time: ");
    Serial.println(currentTime);
    
    // Get NASA moon data
    float moonAge = 0.0;
    bool nasaSuccess = moonPhaseManager.fetchMoonPhaseData(
        currentDate, 
        currentTime, 
        location,
        moonAge
    );
    
    if (nasaSuccess) {
        // Calculate the moon phase index using the actual moon age
        int phaseIndex = moonPhaseManager.calculateMoonPhaseIndex(moonAge);
        
        // Update the moon image
        moonPhaseManager.updateMoonImage(phaseIndex, ui_imgset_moon_frame_);
        
        Serial.println("DEBUG: Moon phase data updated successfully");
    } else {
        Serial.println("DEBUG: Failed to update moon phase data");
    }
}

// Initialize moon phase functionality
void initMoonPhase() {
    // Initialize the MoonPhaseManager with UI elements
    moonPhaseManager.begin(ui_Label_Phase, ui_Image_Moon);
    
    // Force an initial update
    updateMoonPhaseData();
}

// Function to periodically check and update moon phase data
void handleMoonPhaseUpdates() {
    unsigned long currentMillis = millis();
    
    // Check if it's time to update the moon phase data
    if (currentMillis - lastMoonDataUpdate >= MOON_DATA_UPDATE_INTERVAL) {
        lastMoonDataUpdate = currentMillis;
        updateMoonPhaseData();
    }
}

//-----------------------------------------------------
// Loading screen timer callback function
//-----------------------------------------------------

// Callback function for when loading is complete
static void loading_timer_cb(lv_timer_t* timer) {
    Serial.println("DEBUG: Loading complete, switching to Time screen");
    
    // Transition to main time screen
    lv_scr_load(ui_Screen_Time);
    
    // Update current screen tracking
    currentScreen = ui_Screen_Time;
    
    // Delete the timer as it's no longer needed
    lv_timer_del(timer);
}

// Function to add this to your setup() function
void setupLoadingScreen() {
    // Show loading screen
    lv_scr_load(ui_ScreenLoading);
    
    // Create a timer to switch to the main screen after loading completes
    loading_timer = lv_timer_create(loading_timer_cb, LOADING_DURATION, NULL);
    lv_timer_set_repeat_count(loading_timer, 1); // Run only once
}

//-----------------------------------------------------
// Arduino Setup: Initialize Serial, Display, LVGL, and UI
//-----------------------------------------------------
void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting setup...");

    // Initialize TFT display
    tft.begin();

    // Panel-specific settings (adjust as needed)
    tft.setRotation(4);      // Use 4 for Prism version, 2 for NoPrism version
    tft.invertDisplay(1);    // 1 = invert colors, 0 = normal
    tft.fillScreen(TFT_BLACK);

    // Initialize LVGL rendering system
    lvgl_display_setup();

    // Initialize UI components (generated with SquareLine or LVGL editor)
    ui_init();
    
    // Check if UI elements are valid
    if (!ui_Label_HH || !ui_Label_MM || !ui_Label_Colon || !ui_Label_Abbr) {
        Serial.println("ERROR: UI time elements not properly initialized!");
    } else {
        Serial.println("DEBUG: UI time elements initialized");
    }

    // Show loading screen first instead of directly setting the time screen
    lv_scr_load(ui_ScreenLoading);
    
    // Create a timer to switch to the time screen after 5 seconds
    loading_timer = lv_timer_create(loading_timer_cb, LOADING_DURATION, NULL);
    lv_timer_set_repeat_count(loading_timer, 1); // Run only once

    // Force full screen redraw after UI setup
    lv_obj_invalidate(lv_scr_act());
    lv_refr_now(NULL);

    // Initialize QMI8658 Sensor
    QMI8658_Init();
  
    // Initialize WiFi with primary and fallback networks
    Serial.println("Setting up WiFi...");
    wifiManager.begin();
    wifiManager.setPrimaryWiFi(WIFI_SSID, WIFI_PASSWORD);
    wifiManager.setFallbackWiFi(FALLBACK_WIFI_SSID, FALLBACK_WIFI_PASSWORD);
    wifiManager.setAPSettings("WS_Prism", ""); // Optional custom AP name
    
    // Connect to WiFi
    Serial.println("Connecting to WiFi...");
    if (wifiManager.connect()) {
        Serial.println("WiFi connected successfully");
    } else {
        Serial.println("WiFi connection failed or using AP mode");
    }
  
    // Initialize time manager
    Serial.println("Setting up time synchronization...");
    if (timeManager.begin(TIME_ZONE)) {
        Serial.println("Time synchronized with NTP server");
        Serial.print("Current time: ");
        Serial.println(timeManager.getFormattedTime());
        
        // Update time display initially
        updateTimeDisplay();
    } else {
        Serial.println("Failed to synchronize time, will retry later");
    }

    Serial.println("Setup complete");

    initMoonPhase();

    initPixelRefresh(60000); // Set refresh interval to 60 seconds
}

//-----------------------------------------------------
// Arduino Loop: Handle LVGL tasks
//-----------------------------------------------------
void loop() {
    lv_timer_handler(); // Run LVGL's task handler
  
    // Handle WiFi
    wifiManager.update();
  
    // Update time display and blink colon
    updateTimeDisplay();
    updateColonBlink();
  
    // Periodically sync time - once per hour
    static unsigned long lastTimeSync = 0;
    const unsigned long TIME_SYNC_INTERVAL = 3600000; // 1 hour
  
    if (millis() - lastTimeSync >= TIME_SYNC_INTERVAL) {
        lastTimeSync = millis();
        if (wifiManager.isConnected()) {
            Serial.println("DEBUG: Syncing time with NTP server");
            timeManager.syncTime();
        }
    }
  
    // Add a controlled rate for QMI8658 readings and screen rotation checks
    static unsigned long lastQmiRead = 0;
    const unsigned long QMI_READ_INTERVAL = 100; // 100ms interval for responsive rotation detection
    
    if (millis() - lastQmiRead >= QMI_READ_INTERVAL) {
        lastQmiRead = millis();
        QMI8658_Loop(); // Read sensor values
        updateDirectionFromGyro(); // Check for rotation and change screens if needed
    }

    delay(5);           // Small delay to allow other background tasks
    pixelRefreshRoutine(); // Call pixel refresh routine

    handleMoonPhaseUpdates(); // Check and update moon phase data
}