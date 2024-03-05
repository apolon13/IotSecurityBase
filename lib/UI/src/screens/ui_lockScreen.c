// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "../ui.h"

void ui_lockScreen_screen_init(void) {
    ui_lockScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_lockScreen, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
    lv_obj_set_style_pad_left(ui_lockScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_lockScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_lockScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_lockScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_lockScreen, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_lockScreen, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TabView6 = lv_tabview_create(ui_lockScreen, LV_DIR_TOP, 50);
    lv_obj_set_width(ui_TabView6, 800);
    lv_obj_set_height(ui_TabView6, 480);
    lv_obj_set_align(ui_TabView6, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_TabView6, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

    lv_obj_set_style_bg_color(lv_tabview_get_tab_btns(ui_TabView6), lv_color_hex(0x2095F6),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(lv_tabview_get_tab_btns(ui_TabView6), 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label15 = lv_label_create(ui_lockScreen);
    lv_obj_set_width(ui_Label15, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label15, LV_SIZE_CONTENT);   /// 1
    lv_label_set_text(ui_Label15, "IoT Security");
    lv_obj_set_style_text_font(ui_Label15, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label15, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label15, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lockContainer = lv_obj_create(ui_lockScreen);
    lv_obj_remove_style_all(ui_lockContainer);
    lv_obj_set_height(ui_lockContainer, 480);
    lv_obj_set_width(ui_lockContainer, lv_pct(100));
    lv_obj_set_align(ui_lockContainer, LV_ALIGN_TOP_MID);
    lv_obj_set_scrollbar_mode(ui_lockContainer, LV_SCROLLBAR_MODE_ON);
    lv_obj_set_style_pad_left(ui_lockContainer, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_lockContainer, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_lockContainer, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_lockContainer, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel3 = lv_obj_create(ui_lockContainer);
    lv_obj_set_height(ui_Panel3, 395);
    lv_obj_set_width(ui_Panel3, lv_pct(100));
    lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

    ui_pincode = lv_textarea_create(ui_Panel3);
    lv_obj_set_width(ui_pincode, 250);
    lv_obj_set_height(ui_pincode, LV_SIZE_CONTENT);   /// 140
    lv_obj_set_align(ui_pincode, LV_ALIGN_TOP_MID);
    lv_textarea_set_placeholder_text(ui_pincode, "PIN");
    lv_textarea_set_one_line(ui_pincode, true);
    lv_textarea_set_password_mode(ui_pincode, true);


    ui_Button1 = lv_btn_create(ui_Panel3);
    lv_obj_set_width(ui_Button1, 100);
    lv_obj_set_height(ui_Button1, 50);
    lv_obj_set_x(ui_Button1, 0);
    lv_obj_set_y(ui_Button1, -40);
    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);   /// Flags
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
    lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(0xE61616), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label20 = lv_label_create(ui_Button1);
    lv_obj_set_width(ui_Label20, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label20, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align(ui_Label20, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label20, "Unlock");

    ui_Keyboard2 = lv_keyboard_create(ui_lockScreen);
    lv_obj_set_width(ui_Keyboard2, lv_pct(100));
    lv_obj_set_height(ui_Keyboard2, lv_pct(50));
    lv_obj_set_align(ui_Keyboard2, LV_ALIGN_BOTTOM_MID);
    lv_obj_add_flag(ui_Keyboard2, LV_OBJ_FLAG_HIDDEN);   /// Flags

    lv_obj_add_event_cb(ui_pincode, ui_event_pincode, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button1, ui_event_Button1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Keyboard2, ui_event_Keyboard2, LV_EVENT_ALL, NULL);

}
