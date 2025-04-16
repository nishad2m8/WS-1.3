#include "lvgl_setup.h"

// Screen resolution
static const uint16_t screenWidth  = 240;
static const uint16_t screenHeight = 240;

// LVGL draw buffers
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[screenWidth * screenHeight / 2];
static lv_color_t buf2[screenWidth * screenHeight / 2];

// TFT instance (global)
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

// Flush callback
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)color_p, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

// Initialize LVGL 
void lvgl_display_setup() {
    lv_init();

    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screenWidth * screenHeight / 2);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res   = screenWidth;
    disp_drv.ver_res   = screenHeight;
    disp_drv.flush_cb  = my_disp_flush;
    disp_drv.draw_buf  = &draw_buf;
    disp_drv.full_refresh = 1;
    lv_disp_drv_register(&disp_drv);
}
