/**
 * @file ui.c
 * @author TianShuang Ke (dske@listenai.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-20
 * 
 * @copyright Copyright (c) 2021 - 2023  shenzhen listenai co., ltd.
 * 
 * SPDX-License-Identifier: Apache-2.0
 */
#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////

lv_obj_t * ui_ScreenHome;
lv_obj_t * ui_ScreenHome_BodyBase;
lv_obj_t * ui_ScreenHome_Body;

lv_obj_t * ui_AppName;
lv_obj_t *ui_AppIcon;

#define UI_SCREEN_WIDTH     (LV_HOR_RES)
#define UI_SCREEN_HEIGHT    (LV_VER_RES / 10 * 9)
#define UI_STATUSBAR_HEIGHT (LV_VER_RES / 10)

typedef struct {
    const int id;
    const char *name;
    const lv_img_dsc_t *icon;
    uint16_t zoom;
    lv_obj_t *icon_canvas;
    uint16_t icon_width;
    uint16_t icon_height;
} ui_app_t;

#define APP_ID_OCR              (1)
#define APP_ID_AUDIO_PLAYER     (2)
#define APP_ID_DIALOGUE         (3)
#define APP_ID_DICTIONARY       (4)
#define APP_ID_SETTING          (5)

#define APP_ICON_ZOOM(x)    (x*256)

ui_app_t apps[] = {
    {.id = APP_ID_OCR,            .name = "扫描", .icon_width = LV_SIZE_CONTENT, .icon_height = LV_SIZE_CONTENT, .icon = &ui_img__launcher_scan_png,        .zoom = APP_ICON_ZOOM(0),},
    {.id = APP_ID_AUDIO_PLAYER,   .name = "播音", .icon_width = LV_SIZE_CONTENT, .icon_height = LV_SIZE_CONTENT, .icon = &ui_img__launcher_composition_png, .zoom = APP_ICON_ZOOM(0),},
    // {.id = APP_ID_SETTING,        .name = "词典", .icon_width = LV_SIZE_CONTENT, .icon_height = LV_SIZE_CONTENT, .icon = &ui_img__launcher_fav_png,         .zoom = APP_ICON_ZOOM(0),},
    // {.id = APP_ID_DICTIONARY,     .name = "拼读", .icon_width = LV_SIZE_CONTENT, .icon_height = LV_SIZE_CONTENT, .icon = &ui_img__launcher_practice_png,    .zoom = APP_ICON_ZOOM(0),},
    {.id = APP_ID_SETTING,        .name = "设置", .icon_width = LV_SIZE_CONTENT, .icon_height = LV_SIZE_CONTENT, .icon = &ui_img__launcher_setting_png,     .zoom = APP_ICON_ZOOM(0),},
};
lv_obj_t * ui_AppIcon_Wsp;
lv_obj_t * ui_AppName_Wsp;

void ui_event_OpenAppAudioPlayer(lv_event_t * e);
void ui_event_OpenAppSetting(lv_event_t * e);
void ui_event_OpenAppAudioPlayer(lv_event_t * e);
void ui_event_OpenAppWsp(lv_event_t * e);

// 状态栏
void ui_event_StatusBar_BtnBackHome(lv_event_t * e);
lv_obj_t * ui_StatusBar;
lv_obj_t * ui_StatusBar_BtnBackHome;
lv_obj_t * ui_StatusBar_LabDate;
lv_obj_t * ui_StatusBar_LabTime;
lv_obj_t * ui_StatusBar_IconBatteryState;
lv_obj_t * ui_StatusBar_LabBatteryLevel;

// Scan App
lv_obj_t * ui_ScreenApp_ScanOCR;
lv_obj_t * ui_ScreenApp_ScanOCR_Body;
lv_obj_t * ui_ScreenApp_ScanOCR_LabResult;
lv_obj_t * ui_ScreenApp_ScanOCR_ImgOCRTips;
lv_obj_t * ui_ScreenApp_ScanOCR_LabelOCRTips;

// Audio Player App
lv_obj_t * ui_ScreenApp_AudioPlayer;
lv_obj_t * ui_ScreenApp_AudioPlayer_Body;
lv_obj_t * ui_ScreenApp_AudioPlayer_BtnStart;
lv_obj_t * ui_ScreenApp_AudioPlayer_BtnStop;

// Setting App
lv_obj_t * ui_ScreenApp_Setting;
lv_obj_t * ui_ScreenApp_Setting_LabelBacklight;
lv_obj_t * ui_ScreenApp_Setting_LabelVolume;
lv_obj_t * ui_ScreenApp_Setting_SliderBacklight;
lv_obj_t * ui_ScreenApp_Setting_SliderVolume;

// Wsp App
lv_obj_t * ui_ScreenApp_Wsp;
lv_obj_t * ui_ScreenApp_Wsp_Body;
lv_obj_t * ui_ScreenApp_Wsp_BtnStart;
lv_obj_t * ui_ScreenApp_Wsp_BtnStop;
lv_obj_t * ui_ScreenApp_Wsp_Result;

static uint8_t current_screen = UI_SCREEN_NONE;
///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
// #if LV_COLOR_16_SWAP !=0
//     #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
// #endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_OpenApp(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    int app_id = (int)lv_event_get_user_data(e);
    LV_UNUSED(target);
    if(event_code == LV_EVENT_CLICKED) {
        lv_obj_clear_flag(ui_StatusBar_BtnBackHome, LV_OBJ_FLAG_HIDDEN); //显示 BackHome 按钮
        lv_obj_add_flag(ui_StatusBar_LabDate, LV_OBJ_FLAG_HIDDEN);   // 隐藏日期
        switch (app_id)
        {
        case APP_ID_OCR:
            current_screen = UI_SCREEN_OCR;
            lv_obj_set_parent(ui_StatusBar, ui_ScreenApp_ScanOCR);
            lv_obj_set_style_text_font(ui_ScreenApp_ScanOCR_LabResult, &ui_font_PocketDictPenRegular, LV_PART_MAIN | LV_STATE_DEFAULT);
            _ui_screen_change(ui_ScreenApp_ScanOCR, LV_SCR_LOAD_ANIM_FADE_ON, 60, 0);
            break;
        case APP_ID_AUDIO_PLAYER:
            current_screen = UI_SCREEN_AUDIO_PLAY;
            lv_obj_set_parent(ui_StatusBar, ui_ScreenApp_AudioPlayer);
            _ui_screen_change(ui_ScreenApp_AudioPlayer, LV_SCR_LOAD_ANIM_FADE_ON, 60, 0);
            break;
        case APP_ID_SETTING:
            current_screen = UI_SCREEN_SETTING;
            lv_obj_set_parent(ui_StatusBar, ui_ScreenApp_Setting);
            _ui_screen_change(ui_ScreenApp_Setting, LV_SCR_LOAD_ANIM_FADE_ON, 60, 0);
            break;
        case APP_ID_DICTIONARY:
            current_screen = UI_SCREEN_DICTIONARY;
            lv_obj_set_parent(ui_StatusBar, ui_ScreenApp_Wsp);
            _ui_screen_change(ui_ScreenApp_Wsp, LV_SCR_LOAD_ANIM_FADE_ON, 60, 0);
            break;
        case APP_ID_DIALOGUE:
            current_screen = UI_SCREEN_DIALOGUE;
            lv_obj_set_parent(ui_StatusBar, ui_ScreenApp_Setting);
            _ui_screen_change(ui_ScreenApp_Setting, LV_SCR_LOAD_ANIM_FADE_ON, 60, 0);
            break;
        default:
            break;
        }
    }
}

void ui_event_StatusBar_BtnBackHome(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    LV_UNUSED(target);
    if(event_code == LV_EVENT_CLICKED) {
        current_screen = UI_SCREEN_HOME;
        lv_obj_set_parent(ui_StatusBar, ui_ScreenHome);
        lv_obj_add_flag(ui_StatusBar_BtnBackHome, LV_OBJ_FLAG_HIDDEN); // 隐藏 BackHome 按钮
        lv_obj_clear_flag(ui_StatusBar_LabDate, LV_OBJ_FLAG_HIDDEN);   // 显示日期
        _ui_screen_change(ui_ScreenHome, LV_SCR_LOAD_ANIM_FADE_ON, 60, 0);
    }
}

static ui_get_battery_cb_t battery_update_cb = NULL;
void ui_register_battery_update_cb(ui_get_battery_cb_t update_handler)
{
    battery_update_cb = update_handler;
}

static void ui_event_BatteryUpdate(lv_event_t *e)
{
    uint8_t battery_level = 0;
    uint8_t battery_status = 0;
    
    if (ui_StatusBar_LabBatteryLevel == NULL) {
        return;
    }
    if (battery_update_cb) {
        battery_update_cb(&battery_status, &battery_level);
        lv_label_set_text_fmt(ui_StatusBar_LabBatteryLevel, "%d%%", battery_level);
        // TODO: update battery icon according to battery status
    }
}

#define UI_BATTERY_UPDATE_INTERVAL 20
static lv_timer_t * ui_timer_date;
static ui_get_timer_cb_t ui_timer_update_cb = NULL;
void ui_timer_date_cb(lv_timer_t * timer)
{
    static uint8_t time_dot = 1;
    static uint8_t time_cnt = UI_BATTERY_UPDATE_INTERVAL;
    struct tm t;
    if (ui_timer_update_cb) {
        if (!ui_timer_update_cb(&t)) {
            lv_label_set_text_fmt(ui_StatusBar_LabTime, time_dot?"%02d:%02d:%02d":"%02d:%02d %02d", t.tm_hour, t.tm_min, t.tm_sec);
            lv_label_set_text_fmt(ui_StatusBar_LabDate, "%04d/%02d/%02d", t.tm_year+1900, t.tm_mon, t.tm_mday);
            time_dot = !time_dot;
        }
    }

    if(++time_cnt >= UI_BATTERY_UPDATE_INTERVAL) {
        time_cnt = 0;
        ui_event_BatteryUpdate(NULL);
    }
}

void ui_register_time_update_cb(ui_get_timer_cb_t update_handler)
{
    ui_timer_update_cb = update_handler;
}

void ui_timer_date_init(void)
{
    ui_timer_date = lv_timer_create(ui_timer_date_cb, 500, NULL);
}


static uint8_t _battery_level = 0;
int ui_set_battery_level(const uint8_t level)
{
    if (ui_StatusBar_LabBatteryLevel == NULL) {
        return -1;
    }
    _battery_level = level;
    lv_label_set_text_fmt(ui_StatusBar_LabBatteryLevel, "%d%%", _battery_level);
    return 0;
}

static const lv_img_dsc_t* _ui_StatusBar_IconbatteryStateCharging[5] = {
    &ui_img__status_charging_1_png,
    &ui_img__status_charging_2_png,
    &ui_img__status_charging_3_png,
    &ui_img__status_charging_4_png,
    &ui_img__status_charging_5_png,
};

static const lv_img_dsc_t* _ui_StatusBar_IconbatteryState_status_battery[5] = {
    &ui_img__status_battery_1_png,
    &ui_img__status_battery_2_png,
    &ui_img__status_battery_3_png,
    &ui_img__status_battery_4_png,
    &ui_img__status_battery_5_png,
};


int ui_set_battery_state(const uint8_t state)
{
    if (ui_StatusBar_IconBatteryState == NULL) {
        return -1;
    }
    uint8_t level = _battery_level/20;
    if (state == UI_BATTERY_STATE_CHARGING) {
        lv_img_set_src(ui_StatusBar_IconBatteryState, _ui_StatusBar_IconbatteryStateCharging[level]);
    } else {
        lv_img_set_src(ui_StatusBar_IconBatteryState, _ui_StatusBar_IconbatteryState_status_battery[level]);
    }
    return 0;
}

void ui_StatusBar_init(lv_obj_t *parent)
{
    ui_StatusBar = lv_obj_create(parent);
    lv_obj_set_size(ui_StatusBar, UI_SCREEN_WIDTH, UI_STATUSBAR_HEIGHT);
    lv_obj_set_pos(ui_StatusBar, 0, 4);

    lv_obj_set_align(ui_StatusBar, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_StatusBar, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_StatusBar, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_StatusBar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_StatusBar, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_StatusBar_LabBatteryLevel = lv_label_create(ui_StatusBar);
    lv_obj_set_size(ui_StatusBar_LabBatteryLevel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(ui_StatusBar_LabBatteryLevel, 0, 0);

    lv_obj_set_align(ui_StatusBar_LabBatteryLevel, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_StatusBar_LabBatteryLevel, "100%");
    lv_obj_set_style_text_color(ui_StatusBar_LabBatteryLevel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_StatusBar_LabBatteryLevel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_StatusBar_LabBatteryLevel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);


    ui_StatusBar_LabTime = lv_label_create(ui_StatusBar);
    lv_obj_set_size(ui_StatusBar_LabTime, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(ui_StatusBar_LabTime, 0, -10);

    lv_obj_set_align(ui_StatusBar_LabTime, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_StatusBar_LabTime, "10:30:00");
    lv_obj_set_style_text_color(ui_StatusBar_LabTime, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_StatusBar_LabTime, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_StatusBar_LabTime, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_StatusBar_IconBatteryState = lv_img_create(ui_StatusBar);
    lv_img_set_src(ui_StatusBar_IconBatteryState, &ui_img__status_charging_5_png);
    lv_obj_set_size(ui_StatusBar_IconBatteryState, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(ui_StatusBar_IconBatteryState, 10, -5);
    lv_obj_set_align(ui_StatusBar_IconBatteryState, LV_ALIGN_TOP_RIGHT);
    lv_obj_add_flag(ui_StatusBar_IconBatteryState, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_StatusBar_IconBatteryState, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_StatusBar_LabDate = lv_label_create(ui_StatusBar);
    lv_obj_set_size(ui_StatusBar_LabDate, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(ui_StatusBar_LabDate, -4, -10);
    lv_label_set_text(ui_StatusBar_LabDate, "2023/4/20");
    lv_obj_set_style_text_color(ui_StatusBar_LabDate, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_StatusBar_LabDate, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_StatusBar_LabDate, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Home 界面 StatusBar 返回按钮（默认隐藏）
    ui_StatusBar_BtnBackHome = lv_img_create(ui_StatusBar);
    lv_img_set_src(ui_StatusBar_BtnBackHome, &ui_img__angle_left_double_png);
    lv_obj_set_size(ui_StatusBar_BtnBackHome, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(ui_StatusBar_BtnBackHome, -20, -14);
    lv_obj_add_flag(ui_StatusBar_BtnBackHome, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui_StatusBar_BtnBackHome, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_add_event_cb(ui_StatusBar_BtnBackHome, ui_event_StatusBar_BtnBackHome, LV_EVENT_CLICKED, NULL);
}

///////////////////// SCREENS ////////////////////
void ui_Screen_Home_init(void)
{
    ui_ScreenHome = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenHome, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ScreenHome, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenHome, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_StatusBar_init(ui_ScreenHome);

    ui_ScreenHome_BodyBase = lv_obj_create(ui_ScreenHome);
    lv_obj_set_size(ui_ScreenHome_BodyBase, UI_SCREEN_WIDTH, UI_SCREEN_WIDTH);
    lv_obj_set_pos(ui_ScreenHome_BodyBase, 0, 0);
    lv_obj_set_align(ui_ScreenHome_BodyBase, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_add_flag(ui_ScreenHome_BodyBase, LV_OBJ_FLAG_SCROLL_ONE);     /// Flags
    lv_obj_clear_flag(ui_ScreenHome_BodyBase, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_GESTURE_BUBBLE);      /// Flags
    lv_obj_set_scrollbar_mode(ui_ScreenHome_BodyBase, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_ScreenHome_BodyBase, LV_DIR_HOR);
    lv_obj_set_style_bg_color(ui_ScreenHome_BodyBase, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenHome_BodyBase, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_ScreenHome_BodyBase, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ScreenHome_Body = lv_obj_create(ui_ScreenHome_BodyBase);
    lv_obj_set_size(ui_ScreenHome_Body, 720, 140);
    lv_obj_set_pos(ui_ScreenHome_Body, 0, 0);
    lv_obj_set_align(ui_ScreenHome_Body, LV_ALIGN_LEFT_MID);
    lv_obj_clear_flag(ui_ScreenHome_Body, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_scroll_dir(ui_ScreenHome_Body, LV_DIR_HOR);
    lv_obj_set_style_bg_color(ui_ScreenHome_Body, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenHome_Body, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_ScreenHome_Body, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    for (int i=0; i<(sizeof(apps)/sizeof(apps[0])); i++) {
        ui_AppIcon = lv_img_create(ui_ScreenHome_Body);
        lv_img_set_src(ui_AppIcon, apps[i].icon);
        if (apps[i].zoom) {
            lv_img_set_zoom(ui_AppIcon, apps[i].zoom);
        }

        lv_obj_set_x(ui_AppIcon, 40+i*130);
        lv_obj_set_align(ui_AppIcon, LV_ALIGN_LEFT_MID);
        lv_obj_add_flag(ui_AppIcon, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
        lv_obj_clear_flag(ui_AppIcon, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_add_event_cb(ui_AppIcon, ui_event_OpenApp, LV_EVENT_ALL, (void *)apps[i].id);
        lv_obj_set_size(ui_AppIcon, apps[i].icon_width, apps[i].icon_height);

        ui_AppName = lv_label_create(ui_ScreenHome_Body);
        lv_obj_set_size(ui_AppName, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_pos(ui_AppName, 60+i*130, 54);
        lv_obj_set_align(ui_AppName, LV_ALIGN_LEFT_MID);
        lv_label_set_text(ui_AppName, apps[i].name);
        lv_obj_set_style_text_color(ui_AppName, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(ui_AppName, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui_AppName, &ui_font_PocketDictPenRegular, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    ui_timer_date_init();
    current_screen = UI_SCREEN_HOME;
}

int ui_set_ocr_result_text(const char *result)
{
    if (ui_ScreenApp_ScanOCR_LabResult == NULL || ui_ScreenApp_ScanOCR == NULL)
    {
        return -1;
    }
    if (current_screen == UI_SCREEN_NONE) {
        return -1;
    }
    if (current_screen != UI_SCREEN_OCR) {
        current_screen = UI_SCREEN_OCR;
        lv_obj_set_parent(ui_StatusBar, ui_ScreenApp_ScanOCR);
        lv_obj_clear_flag(ui_StatusBar_BtnBackHome, LV_OBJ_FLAG_HIDDEN); //显示 BackHome 按钮
        lv_obj_add_flag(ui_StatusBar_LabDate, LV_OBJ_FLAG_HIDDEN);   // 隐藏日期
        _ui_screen_change(ui_ScreenApp_ScanOCR, LV_SCR_LOAD_ANIM_OVER_LEFT, 0, 0);
        lv_obj_set_style_text_font(ui_ScreenApp_ScanOCR_LabResult, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    
    lv_textarea_set_text(ui_ScreenApp_ScanOCR_LabResult, result);
    return 0;
}

int ui_set_wsp_result_text(const char *result)
{
    if (ui_ScreenApp_Wsp_Result == NULL)
    {
        return -1;
    }
    if (current_screen == UI_SCREEN_NONE) {
        return -1;
    }
    lv_textarea_set_text(ui_ScreenApp_Wsp_Result, result);
    return 0;
}

void ui_Screen_AppOCR_init(void)
{
    ui_ScreenApp_ScanOCR = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenApp_ScanOCR, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ScreenApp_ScanOCR, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenApp_ScanOCR, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ScreenApp_ScanOCR_Body = lv_obj_create(ui_ScreenApp_ScanOCR);
    lv_obj_set_size(ui_ScreenApp_ScanOCR_Body, 454, 140);
    lv_obj_set_pos(ui_ScreenApp_ScanOCR_Body, 2, 1);
    lv_obj_set_align(ui_ScreenApp_ScanOCR_Body, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_add_flag(ui_ScreenApp_ScanOCR_Body, LV_OBJ_FLAG_SCROLL_ONE);     /// Flags
    lv_obj_clear_flag(ui_ScreenApp_ScanOCR_Body, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK |
                      LV_OBJ_FLAG_GESTURE_BUBBLE);      /// Flags
    lv_obj_set_scrollbar_mode(ui_ScreenApp_ScanOCR_Body, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_ScreenApp_ScanOCR_Body, LV_DIR_HOR);
    lv_obj_set_style_bg_color(ui_ScreenApp_ScanOCR_Body, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenApp_ScanOCR_Body, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_ScreenApp_ScanOCR_Body, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ScreenApp_ScanOCR_ImgOCRTips = lv_img_create(ui_ScreenApp_ScanOCR_Body);
    lv_img_set_src(ui_ScreenApp_ScanOCR_ImgOCRTips, &ui_img__guide_scan_png);
    lv_obj_set_size(ui_ScreenApp_ScanOCR_ImgOCRTips, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(ui_ScreenApp_ScanOCR_ImgOCRTips, -11, -3);
    lv_obj_set_align(ui_ScreenApp_ScanOCR_ImgOCRTips, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ScreenApp_ScanOCR_ImgOCRTips, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_ScreenApp_ScanOCR_ImgOCRTips, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // ui_ScreenApp_ScanOCR_LabelOCRTips = lv_label_create(ui_ScreenApp_ScanOCR_Body);
    // lv_obj_set_size(ui_ScreenApp_ScanOCR_LabelOCRTips, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    // lv_obj_set_pos(ui_ScreenApp_ScanOCR_LabelOCRTips, -17, 47);
    // lv_obj_set_align(ui_ScreenApp_ScanOCR_LabelOCRTips, LV_ALIGN_CENTER);
    // lv_label_set_text(ui_ScreenApp_ScanOCR_LabelOCRTips, "扫描中英文字词句");
    // lv_obj_set_style_text_color(ui_ScreenApp_ScanOCR_LabelOCRTips, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_opa(ui_ScreenApp_ScanOCR_LabelOCRTips, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_font(ui_ScreenApp_ScanOCR_LabelOCRTips, &ui_font_PocketDictPenRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ScreenApp_ScanOCR_LabResult = lv_textarea_create(ui_ScreenApp_ScanOCR);
    lv_obj_set_size(ui_ScreenApp_ScanOCR_Body, 445, 138);
    lv_obj_set_pos(ui_ScreenApp_ScanOCR_Body, -2, 20);
    lv_obj_set_align(ui_ScreenApp_ScanOCR_LabResult, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_ScreenApp_ScanOCR_LabResult, "请按压扫描英文字词句...");
    lv_obj_set_style_text_color(ui_ScreenApp_ScanOCR_LabResult, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_ScreenApp_ScanOCR_LabResult, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ScreenApp_ScanOCR_LabResult, &ui_font_PocketDictPenRegular, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ScreenApp_ScanOCR_LabResult, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenApp_ScanOCR_LabResult, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_ScreenApp_ScanOCR_LabResult, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_ScreenApp_ScanOCR_LabResult, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

}

static ui_audio_player_action_cb_t ui_audio_play_action_cb = NULL;
void ui_register_audio_player_action_cb(ui_audio_player_action_cb_t update_handler)
{
    ui_audio_play_action_cb = update_handler;
}

void ui_event_AudioPlayerAction(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        if (obj == ui_ScreenApp_AudioPlayer_BtnStart) {
            if (ui_audio_play_action_cb) {
                ui_audio_play_action_cb(UI_AUDIO_PLAY_ACTION_PLAY);
            }
        }
        else if (obj == ui_ScreenApp_AudioPlayer_BtnStop)
        {
            if (ui_audio_play_action_cb) {
                ui_audio_play_action_cb(UI_AUDIO_PLAY_ACTION_STOP);
            }
        }
    }
}

static ui_wsp_action_cb_t ui_wsp_action_cb = NULL;
void ui_register_wsp_action_cb(ui_wsp_action_cb_t update_handler)
{
    ui_wsp_action_cb = update_handler;
}

void ui_event_WspAction(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        if (obj == ui_ScreenApp_Wsp_BtnStart) {
            if (ui_wsp_action_cb) {
                ui_wsp_action_cb(UI_AUDIO_PLAY_ACTION_PLAY);
            }
        }
        if (obj == ui_ScreenApp_Wsp_BtnStop) {
            if (ui_wsp_action_cb) {
                ui_wsp_action_cb(UI_AUDIO_PLAY_ACTION_STOP);
            }
        }
    }
}

void ui_Screen_AppAudioPlayer_init(void)
{
    ui_ScreenApp_AudioPlayer = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenApp_AudioPlayer, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ScreenApp_AudioPlayer, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenApp_AudioPlayer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ScreenApp_AudioPlayer_Body = lv_obj_create(ui_ScreenApp_AudioPlayer);
    lv_obj_set_size(ui_ScreenApp_AudioPlayer_Body, 454, 140);
    lv_obj_set_pos(ui_ScreenApp_AudioPlayer_Body, 0,0);
    lv_obj_set_align(ui_ScreenApp_AudioPlayer_Body, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_add_flag(ui_ScreenApp_AudioPlayer_Body, LV_OBJ_FLAG_SCROLL_ONE);     /// Flags
    lv_obj_clear_flag(ui_ScreenApp_AudioPlayer_Body, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK |
                      LV_OBJ_FLAG_GESTURE_BUBBLE);      /// Flags
    lv_obj_set_scrollbar_mode(ui_ScreenApp_AudioPlayer_Body, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_ScreenApp_AudioPlayer_Body, LV_DIR_HOR);
    lv_obj_set_style_bg_color(ui_ScreenApp_AudioPlayer_Body, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenApp_AudioPlayer_Body, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_ScreenApp_AudioPlayer_Body, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ScreenApp_AudioPlayer_BtnStart = lv_btn_create(ui_ScreenApp_AudioPlayer);
    lv_obj_set_size(ui_ScreenApp_AudioPlayer_BtnStart, 180, 80);
    lv_obj_set_pos(ui_ScreenApp_AudioPlayer_BtnStart, 30,0);
    lv_obj_set_align(ui_ScreenApp_AudioPlayer_BtnStart, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_ScreenApp_AudioPlayer_BtnStart, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ScreenApp_AudioPlayer_BtnStart, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    
    lv_obj_t * ui_BtnLaelAudioPlayStart = lv_label_create(ui_ScreenApp_AudioPlayer_BtnStart);
    lv_label_set_text(ui_BtnLaelAudioPlayStart, "Play");
    lv_obj_set_style_text_font(ui_BtnLaelAudioPlayStart, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(ui_BtnLaelAudioPlayStart);

    ui_ScreenApp_AudioPlayer_BtnStop = lv_btn_create(ui_ScreenApp_AudioPlayer);
    lv_obj_set_size(ui_ScreenApp_AudioPlayer_BtnStop, 180, 80);
    lv_obj_set_pos(ui_ScreenApp_AudioPlayer_BtnStop, 220,0);
    lv_obj_set_align(ui_ScreenApp_AudioPlayer_BtnStop, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_ScreenApp_AudioPlayer_BtnStop, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ScreenApp_AudioPlayer_BtnStop, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    
    lv_obj_t * ui_BtnLabelAudioPlayStop = lv_label_create(ui_ScreenApp_AudioPlayer_BtnStop);
    lv_label_set_text(ui_BtnLabelAudioPlayStop, "Stop");
    lv_obj_set_style_text_font(ui_BtnLabelAudioPlayStop, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(ui_BtnLabelAudioPlayStop);

    lv_obj_add_event_cb(ui_ScreenApp_AudioPlayer_BtnStart, ui_event_AudioPlayerAction, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_ScreenApp_AudioPlayer_BtnStop, ui_event_AudioPlayerAction, LV_EVENT_CLICKED, NULL);
}

void ui_Screen_AppWsp_init(void)
{
    ui_ScreenApp_Wsp = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenApp_Wsp, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ScreenApp_Wsp, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenApp_Wsp, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_ScreenApp_Wsp_Body = lv_obj_create(ui_ScreenApp_Wsp);
    // lv_obj_set_size(ui_ScreenApp_Wsp_Body, 454, 140);
    // lv_obj_set_pos(ui_ScreenApp_Wsp_Body, 4, 4);
    // lv_obj_set_align(ui_ScreenApp_Wsp_Body, LV_ALIGN_BOTTOM_LEFT);
    // lv_obj_add_flag(ui_ScreenApp_Wsp_Body, LV_OBJ_FLAG_SCROLL_ONE);     /// Flags
    // lv_obj_clear_flag(ui_ScreenApp_Wsp_Body, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK |
    //                   LV_OBJ_FLAG_GESTURE_BUBBLE);      /// Flags
    // lv_obj_set_scrollbar_mode(ui_ScreenApp_Wsp_Body, LV_SCROLLBAR_MODE_OFF);
    // lv_obj_set_scroll_dir(ui_ScreenApp_Wsp_Body, LV_DIR_HOR);
    // lv_obj_set_style_bg_color(ui_ScreenApp_Wsp_Body, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(ui_ScreenApp_Wsp_Body, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_side(ui_ScreenApp_Wsp_Body, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ScreenApp_Wsp_BtnStart = lv_btn_create(ui_ScreenApp_Wsp);
    lv_obj_set_size(ui_ScreenApp_Wsp_BtnStart, 100, 60);
    lv_obj_set_pos(ui_ScreenApp_Wsp_BtnStart, -150, -10);
    lv_obj_set_align(ui_ScreenApp_Wsp_BtnStart, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ScreenApp_Wsp_BtnStart, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ScreenApp_Wsp_BtnStart, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    
    lv_obj_t * ui_BtnLaelWspStart = lv_label_create(ui_ScreenApp_Wsp_BtnStart);
    lv_label_set_text(ui_BtnLaelWspStart, "Wsp Play");
    lv_obj_set_style_text_font(ui_BtnLaelWspStart, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(ui_BtnLaelWspStart);

    ui_ScreenApp_Wsp_BtnStop = lv_btn_create(ui_ScreenApp_Wsp);
    lv_obj_set_size(ui_ScreenApp_Wsp_BtnStop, 100, 60);
    lv_obj_set_pos(ui_ScreenApp_Wsp_BtnStop, -150, 60);
    lv_obj_set_align(ui_ScreenApp_Wsp_BtnStop, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ScreenApp_Wsp_BtnStop, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ScreenApp_Wsp_BtnStop, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    
    lv_obj_t * ui_BtnLabelWspStop = lv_label_create(ui_ScreenApp_Wsp_BtnStop);
    lv_label_set_text(ui_BtnLabelWspStop, "Wsp Stop");
    lv_obj_set_style_text_font(ui_BtnLabelWspStop, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(ui_BtnLabelWspStop);

    ui_ScreenApp_Wsp_Result = lv_textarea_create(ui_ScreenApp_Wsp);
    lv_obj_set_size(ui_ScreenApp_Wsp_Result, 240, 140);
    lv_obj_set_pos(ui_ScreenApp_Wsp_Result, -80, 30);
    lv_obj_set_align(ui_ScreenApp_Wsp_Result, LV_ALIGN_RIGHT_MID);
    lv_textarea_set_placeholder_text(ui_ScreenApp_Wsp_Result, "...");
    lv_obj_set_style_text_color(ui_ScreenApp_Wsp_Result, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_ScreenApp_Wsp_Result, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ScreenApp_Wsp_Result, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ScreenApp_Wsp_Result, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenApp_Wsp_Result, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_ScreenApp_Wsp_Result, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_ScreenApp_Wsp_Result, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_ScreenApp_Wsp_BtnStart, ui_event_WspAction, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_ScreenApp_Wsp_BtnStop, ui_event_WspAction, LV_EVENT_CLICKED, NULL);
}

static const lv_font_t * font_normal;
static ui_set_backlight_cb_t backlight_update_cb = NULL;
static ui_set_volume_cb_t volume_update_cb = NULL;

void ui_register_set_backlight_cb(ui_set_backlight_cb_t update_handler)
{
    backlight_update_cb = update_handler;
}
void ui_register_set_volume_cb(ui_set_volume_cb_t update_handler)
{
    volume_update_cb = update_handler;
}
    
static void ui_event_SliderHandler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_REFR_EXT_DRAW_SIZE) {
        lv_coord_t *s = lv_event_get_param(e);
        *s = LV_MAX(*s, 60);
    } else if(code == LV_EVENT_DRAW_PART_END) {
        lv_obj_draw_part_dsc_t * dsc = lv_event_get_param(e);
        if(dsc->part == LV_PART_KNOB && lv_obj_has_state(obj, LV_STATE_PRESSED)) {
            char buf[8];
            lv_snprintf(buf, sizeof(buf), "%"LV_PRId32, lv_slider_get_value(obj));

            lv_point_t text_size;
            lv_txt_get_size(&text_size, buf, font_normal, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);

            lv_area_t txt_area;
            txt_area.x1 = dsc->draw_area->x1 + lv_area_get_width(dsc->draw_area) / 2 - text_size.x / 2;
            txt_area.x2 = txt_area.x1 + text_size.x;
            txt_area.y2 = dsc->draw_area->y1 - 10;
            txt_area.y1 = txt_area.y2 - text_size.y;

            lv_area_t bg_area;
            bg_area.x1 = txt_area.x1 - LV_DPX(8);
            bg_area.x2 = txt_area.x2 + LV_DPX(8);
            bg_area.y1 = txt_area.y1 - LV_DPX(8);
            bg_area.y2 = txt_area.y2 + LV_DPX(8);

            lv_draw_rect_dsc_t rect_dsc;
            lv_draw_rect_dsc_init(&rect_dsc);
            rect_dsc.bg_color = lv_palette_darken(LV_PALETTE_GREY, 3);
            rect_dsc.radius = LV_DPX(5);
            lv_draw_rect(dsc->draw_ctx, &rect_dsc, &bg_area);

            lv_draw_label_dsc_t label_dsc;
            lv_draw_label_dsc_init(&label_dsc);
            label_dsc.color = lv_color_white();
            label_dsc.font = font_normal;
            lv_draw_label(dsc->draw_ctx, &label_dsc, &txt_area, buf, NULL);
        }
    } else if(code == LV_EVENT_VALUE_CHANGED) {
        if (obj == ui_ScreenApp_Setting_SliderBacklight) {
            if (backlight_update_cb) {
                backlight_update_cb(lv_slider_get_value(obj));
            }
        } else if (obj == ui_ScreenApp_Setting_SliderVolume) {
            if (volume_update_cb) {
                volume_update_cb(lv_slider_get_value(obj));
            }
        }
    }
}


void ui_Screen_AppSetting_init(void)
{
    ui_ScreenApp_Setting = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenApp_Setting, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ScreenApp_Setting, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenApp_Setting, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(ui_ScreenApp_Setting, 454, 180);

    ui_ScreenApp_Setting_LabelBacklight = lv_label_create(ui_ScreenApp_Setting);
    lv_obj_set_size(ui_ScreenApp_Setting_LabelBacklight, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(ui_ScreenApp_Setting_LabelBacklight, -173, -10);
    lv_obj_set_align(ui_ScreenApp_Setting_LabelBacklight, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ScreenApp_Setting_LabelBacklight, "亮度");
    lv_obj_set_style_text_color(ui_ScreenApp_Setting_LabelBacklight, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_ScreenApp_Setting_LabelBacklight, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ScreenApp_Setting_LabelBacklight, &ui_font_PocketDictPenRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ScreenApp_Setting_LabelVolume = lv_label_create(ui_ScreenApp_Setting);
    lv_obj_set_size(ui_ScreenApp_Setting_LabelVolume, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(ui_ScreenApp_Setting_LabelVolume, -175, 44);
    lv_obj_set_align(ui_ScreenApp_Setting_LabelVolume, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ScreenApp_Setting_LabelVolume, "音量");
    lv_obj_set_style_text_color(ui_ScreenApp_Setting_LabelVolume, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_ScreenApp_Setting_LabelVolume, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ScreenApp_Setting_LabelVolume, &ui_font_PocketDictPenRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ScreenApp_Setting_SliderBacklight = lv_slider_create(ui_ScreenApp_Setting);
    lv_obj_set_size(ui_ScreenApp_Setting_SliderBacklight, 280, 16);
    lv_obj_set_pos(ui_ScreenApp_Setting_SliderBacklight, 22, -10);
    lv_obj_set_align(ui_ScreenApp_Setting_SliderBacklight, LV_ALIGN_CENTER);
    lv_slider_set_value(ui_ScreenApp_Setting_SliderBacklight, 100, LV_ANIM_OFF);
    lv_slider_set_range(ui_ScreenApp_Setting_SliderBacklight, UI_SILDER_BACKLIGHT_BRIGHTNESS_MIN_VALUE, UI_SILDER_BACKLIGHT_BRIGHTNESS_MAX_VALUE);
    lv_obj_set_style_bg_color(ui_ScreenApp_Setting_SliderBacklight, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenApp_Setting_SliderBacklight, 255, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_ScreenApp_Setting_SliderVolume = lv_slider_create(ui_ScreenApp_Setting);
    lv_obj_set_size(ui_ScreenApp_Setting_SliderVolume, 280, 16);
    lv_obj_set_pos(ui_ScreenApp_Setting_SliderVolume, 22, 44);
    lv_obj_set_align(ui_ScreenApp_Setting_SliderVolume, LV_ALIGN_CENTER);
    lv_slider_set_value(ui_ScreenApp_Setting_SliderVolume, 8, LV_ANIM_OFF);
    lv_slider_set_range(ui_ScreenApp_Setting_SliderVolume, UI_SILDER_VOLUME_MIN_VALUE, UI_SILDER_VOLUME_MAX_VALUE);
    lv_obj_set_style_bg_color(ui_ScreenApp_Setting_SliderVolume, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenApp_Setting_SliderVolume, 255, LV_PART_KNOB | LV_STATE_DEFAULT);

    font_normal = LV_FONT_DEFAULT;
    lv_obj_add_event_cb(ui_ScreenApp_Setting_SliderBacklight, ui_event_SliderHandler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScreenApp_Setting_SliderVolume, ui_event_SliderHandler, LV_EVENT_ALL, NULL);
}

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    ui_Screen_AppOCR_init();
    ui_Screen_AppAudioPlayer_init();
    ui_Screen_AppSetting_init();
    ui_Screen_AppWsp_init();

    ui_Screen_Home_init();
    lv_disp_load_scr(ui_ScreenHome);
}
