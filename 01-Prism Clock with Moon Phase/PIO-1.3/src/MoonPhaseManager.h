#ifndef MOON_PHASE_MANAGER_H
#define MOON_PHASE_MANAGER_H

#include <Arduino.h>
#include <lvgl.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class MoonPhaseManager {
public:
    MoonPhaseManager();
    
    // Initialize with required UI elements
    void begin(lv_obj_t* labelPhase, lv_obj_t* imageMoon);
    
    // Fetch moon phase data using the NASA API
    bool fetchMoonPhaseData(const String& date, const String& time24hr, const String& location, float& moonAge);
    
    // Calculate the moon phase based on the phase value (0.0-1.0)
    String calculateMoonPhase(double moonphase);
    
    // Calculate the index for selecting the appropriate moon image
    int calculateMoonPhaseIndex(float age);
    
    // Set the moon image based on the calculated phase index
    void updateMoonImage(int phaseIndex, const lv_img_dsc_t *ui_imgset_moon_[]);

private:
    // UI elements
    lv_obj_t* _labelPhase;
    lv_obj_t* _imageMoon;
    
    HTTPClient _httpClient;
};

#endif // MOON_PHASE_MANAGER_H