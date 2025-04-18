// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "../ui.h"

void ui_ScreenLoading_screen_init(void)
{
ui_ScreenLoading = lv_obj_create(NULL);
lv_obj_clear_flag( ui_ScreenLoading, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ScreenLoading, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ScreenLoading, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label_info = lv_label_create(ui_ScreenLoading);
lv_obj_set_width( ui_Label_info, 230);
lv_obj_set_height( ui_Label_info, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label_info, 0 );
lv_obj_set_y( ui_Label_info, 67 );
lv_obj_set_align( ui_Label_info, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label_info,"Loading...");
lv_obj_set_style_text_align(ui_Label_info, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Label_info, &ui_font_IBM25, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Image_Loading = lv_img_create(ui_ScreenLoading);
lv_img_set_src(ui_Image_Loading, &ui_img_moon_phase_moon_frame_00004_png);
lv_obj_set_width( ui_Image_Loading, LV_SIZE_CONTENT);  /// 170
lv_obj_set_height( ui_Image_Loading, LV_SIZE_CONTENT);   /// 170
lv_obj_set_x( ui_Image_Loading, 0 );
lv_obj_set_y( ui_Image_Loading, -14 );
lv_obj_set_align( ui_Image_Loading, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Image_Loading, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image_Loading, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Image_Loading,255);

}
