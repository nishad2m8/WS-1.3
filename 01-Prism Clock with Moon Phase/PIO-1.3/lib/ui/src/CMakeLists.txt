SET(SOURCES screens/ui_ScreenLoading.c
    screens/ui_Screen_Time.c
    screens/ui_Screen_MoonPhase.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_moon_phase_moon_frame_00004_png.c
    images/ui_img_moon_phase_moon_frame_00003_png.c
    images/ui_img_moon_phase_moon_frame_00000_png.c
    images/ui_img_moon_phase_moon_frame_00001_png.c
    images/ui_img_moon_phase_moon_frame_00002_png.c
    images/ui_img_moon_phase_moon_frame_00005_png.c
    images/ui_img_moon_phase_moon_frame_00006_png.c
    images/ui_img_moon_phase_moon_frame_00007_png.c
    images/ui_img_moon_phase_moon_frame_00008_png.c
    images/ui_img_moon_phase_moon_frame_00009_png.c
    images/ui_img_moon_phase_moon_frame_00010_png.c
    images/ui_img_moon_phase_moon_frame_00011_png.c
    images/ui_img_moon_phase_moon_frame_00012_png.c
    images/ui_img_moon_phase_moon_frame_00013_png.c
    images/ui_img_moon_phase_moon_frame_00014_png.c
    images/ui_img_moon_phase_moon_frame_00015_png.c
    images/ui_img_moon_phase_moon_frame_00016_png.c
    images/ui_img_moon_phase_moon_frame_00017_png.c
    images/ui_img_moon_phase_moon_frame_00018_png.c
    images/ui_img_moon_phase_moon_frame_00019_png.c
    images/ui_img_moon_phase_moon_frame_00020_png.c
    images/ui_img_moon_phase_moon_frame_00021_png.c
    images/ui_img_moon_phase_moon_frame_00022_png.c
    images/ui_img_moon_phase_moon_frame_00023_png.c
    images/ui_img_moon_phase_moon_frame_00024_png.c
    images/ui_img_moon_phase_moon_frame_00025_png.c
    images/ui_img_moon_phase_moon_frame_00026_png.c
    images/ui_img_moon_phase_moon_frame_00027_png.c
    images/ui_img_moon_phase_moon_frame_00028_png.c
    fonts/ui_font_IBM110.c
    fonts/ui_font_IBM170.c
    fonts/ui_font_IBM20.c
    fonts/ui_font_IBM25.c
    fonts/ui_font_IBM45.c
    fonts/ui_font_IBM70.c)

add_library(ui ${SOURCES})
