/* ----------------- Expirimental --------
 #include "pixel_refresh.h"

void setup() {
  // ... your setup code ...
  initPixelRefresh(60000); // Set refresh interval to 60 seconds
}

void loop() {
  // ... your loop code ...

  pixelRefreshRoutine(); // Call this function in your loop to refresh the display
} 
---------------------------------------- */

#include <Arduino.h>
#include <lvgl.h>
#include "pixel_refresh.h"

static unsigned long refreshInterval = 60000; // Default to 60 seconds
static unsigned long lastRefreshTime = 0;

void initPixelRefresh(unsigned long interval) {
  refreshInterval = interval;
  lastRefreshTime = millis();
}

void pixelRefreshRoutine() {
  unsigned long currentTime = millis();
  if (currentTime - lastRefreshTime >= refreshInterval) {
    lv_obj_invalidate(lv_scr_act());
    lastRefreshTime = currentTime;
  }
}
