#include "MoonPhaseManager.h"

MoonPhaseManager::MoonPhaseManager() 
    : _labelPhase(nullptr), _imageMoon(nullptr) {
}

void MoonPhaseManager::begin(lv_obj_t* labelPhase, lv_obj_t* imageMoon) {
    _labelPhase = labelPhase;
    _imageMoon = imageMoon;
}

String MoonPhaseManager::calculateMoonPhase(double moonAge) {
    String phase;
    
    // NASA API returns moon age in days (0-29.53)
    // Normalize to phase value (0-1)
    double normalizedPhase = moonAge / 29.53;
    
    // Ensure the value stays between 0 and 1
    normalizedPhase = normalizedPhase - floor(normalizedPhase);
    
    if (normalizedPhase < 0.01 || normalizedPhase > 0.99) {
        phase = "New Moon";
    } else if (normalizedPhase < 0.25) {
        phase = "Waxing Crescent";
    } else if (normalizedPhase >= 0.25 && normalizedPhase < 0.26) {
        phase = "First Quarter";
    } else if (normalizedPhase >= 0.26 && normalizedPhase < 0.49) {
        phase = "Waxing Gibbous";
    } else if (normalizedPhase >= 0.49 && normalizedPhase < 0.51) {
        phase = "Full Moon";
    } else if (normalizedPhase >= 0.51 && normalizedPhase < 0.74) {
        phase = "Waning Gibbous";
    } else if (normalizedPhase >= 0.74 && normalizedPhase < 0.76) {
        phase = "Last Quarter";
    } else {
        phase = "Waning Crescent";
    }
    
    return phase;
}

int MoonPhaseManager::calculateMoonPhaseIndex(float age) {
    // Calculate the index of the moon phase image based on the moon age
    // Assuming you have 30 images for the moon phases (0-29)
    int totalPhases = 30;
    // Normalize the age value to be within the range [0, 29]
    int index = (int)(age / 29.53 * totalPhases);
    // Make sure index is within bounds
    if (index >= totalPhases) index = totalPhases - 1;
    if (index < 0) index = 0;
    
    return index;
}

void MoonPhaseManager::updateMoonImage(int phaseIndex, const lv_img_dsc_t *ui_imgset_moon_[]) {
    if (_imageMoon) {
        lv_img_set_src(_imageMoon, (const void *)ui_imgset_moon_[phaseIndex]);
        Serial.print("Updated moon image to phase index: ");
        Serial.println(phaseIndex);
    }
}

bool MoonPhaseManager::fetchMoonPhaseData(const String& date, const String& time24hr, const String& location, float& moonAge) {
    String nasaPayload = "";
    bool success = false;

    Serial.println("Fetching NASA Moon data...");

    // Construct the NASA API URL with the date and 24-hour format time string
    String url = "https://svs.gsfc.nasa.gov/api/dialamoon/" + date + "T" + time24hr;
    Serial.println(url);
    
    _httpClient.begin(url);

    int httpResponseCode = _httpClient.GET();
    if (httpResponseCode > 0) {
        nasaPayload = _httpClient.getString();
        Serial.println("NASA API response:");
        Serial.println(nasaPayload);
        success = true;
    } else {
        Serial.print("Error code from NASA API: ");
        Serial.println(httpResponseCode);
        
        // Display error message on UI suggesting a restart
        if (_labelPhase) {
            lv_label_set_text(_labelPhase, "API Error - Restart Device");
        }
    }
    _httpClient.end();

    if (success) {
        // Parse the NASA response
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, nasaPayload);
        if (error) {
            Serial.println("Failed to parse NASA JSON");
            
            // Display JSON parse error on UI
            if (_labelPhase) {
                lv_label_set_text(_labelPhase, "API Error - Restart Device");
            }
            
            return false;
        }

        float phase = doc["phase"];
        moonAge = doc["age"]; // Store the moon age in the output parameter
        float posangle = doc["posangle"];

        Serial.print("Moon Phase: ");
        Serial.println(phase);
        Serial.print("Moon Age: ");
        Serial.println(moonAge);
        Serial.print("Position Angle: ");
        Serial.println(posangle);

        // Calculate the moon phase name based on moonAge
        String phaseText = calculateMoonPhase(moonAge);
        
        // Update the phase label if it exists
        if (_labelPhase) {
            lv_label_set_text(_labelPhase, phaseText.c_str());
        }
        
        return true;
    }
    
    return false;
}