#ifndef LVGL_SETUP_H
#define LVGL_SETUP_H

#include <lvgl.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft;

void lvgl_display_setup();
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

#endif
