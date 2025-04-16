#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

#include "lvgl_setup.h"
#include "WS_QMI8658.h"
#include "ui.h"  // LVGL UI initialization (e.g., from SquareLine Studio)

// https://www.youtube.com/nishad2m8
// https://buymeacoffee.com/nishad2m8


// Update direction using the gyroscope
void updateDirectionFromGyro() {
  float accelX = QMI8658_get_A_fx();  // Read X-axis acceleration
  float accelY = QMI8658_get_A_fy();  // Read Y-axis acceleration
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

  // Force full screen redraw after UI setup
  lv_obj_invalidate(lv_scr_act());
  lv_refr_now(NULL);

  // Initialize QMI8658 Sensor
  QMI8658_Init();

  Serial.println("Setup complete");
}


//-----------------------------------------------------
// Arduino Loop: Handle LVGL tasks
//-----------------------------------------------------
void loop() {
  lv_timer_handler(); // Run LVGL's task handler
  delay(5);           // Small delay to allow other background tasks

  // QMI8658_Loop(); // Print accel/gyro values every time
}
