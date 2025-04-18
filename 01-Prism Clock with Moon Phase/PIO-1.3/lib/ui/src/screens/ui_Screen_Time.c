// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "../ui.h"

void ui_Screen_Time_screen_init(void)
{
ui_Screen_Time = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Screen_Time, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_Screen_Time, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Screen_Time, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label_HH = lv_label_create(ui_Screen_Time);
lv_obj_set_width( ui_Label_HH, 240);
lv_obj_set_height( ui_Label_HH, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label_HH, 0 );
lv_obj_set_y( ui_Label_HH, -41 );
lv_obj_set_align( ui_Label_HH, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label_HH,"00");
lv_obj_set_style_text_color(ui_Label_HH, lv_color_hex(0xC4E232), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Label_HH, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_Label_HH, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Label_HH, &ui_font_IBM170, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label_Colon = lv_label_create(ui_Screen_Time);
lv_obj_set_width( ui_Label_Colon, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label_Colon, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label_Colon, -73 );
lv_obj_set_y( ui_Label_Colon, 59 );
lv_obj_set_align( ui_Label_Colon, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label_Colon,":");
lv_obj_set_style_text_color(ui_Label_Colon, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Label_Colon, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Label_Colon, &ui_font_IBM110, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label_MM = lv_label_create(ui_Screen_Time);
lv_obj_set_width( ui_Label_MM, LV_SIZE_CONTENT);  /// 162
lv_obj_set_height( ui_Label_MM, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label_MM, 18 );
lv_obj_set_y( ui_Label_MM, 68 );
lv_obj_set_align( ui_Label_MM, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label_MM,"00");
lv_obj_set_style_text_color(ui_Label_MM, lv_color_hex(0x18D5F1), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Label_MM, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Label_MM, &ui_font_IBM110, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Panel_Abbr = lv_obj_create(ui_Screen_Time);
lv_obj_set_width( ui_Panel_Abbr, 55);
lv_obj_set_height( ui_Panel_Abbr, 36);
lv_obj_set_x( ui_Panel_Abbr, 74 );
lv_obj_set_y( ui_Panel_Abbr, 35 );
lv_obj_set_align( ui_Panel_Abbr, LV_ALIGN_CENTER );
lv_obj_clear_flag( ui_Panel_Abbr, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Panel_Abbr, 15, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_Panel_Abbr, lv_color_hex(0xFF3000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Panel_Abbr, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_Panel_Abbr, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_Panel_Abbr, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_Panel_Abbr, 5, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label_Abbr = lv_label_create(ui_Panel_Abbr);
lv_obj_set_width( ui_Label_Abbr, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label_Abbr, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label_Abbr, 0 );
lv_obj_set_y( ui_Label_Abbr, -1 );
lv_obj_set_align( ui_Label_Abbr, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label_Abbr,"");
lv_obj_set_style_text_font(ui_Label_Abbr, &ui_font_IBM25, LV_PART_MAIN| LV_STATE_DEFAULT);

}
