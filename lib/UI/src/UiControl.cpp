#include "UiControl.h"
#include "LGFX.h"

static lv_disp_draw_buf_t drawBuf;
static lv_color_t buf[800 * 480 / 10];
static lv_disp_drv_t dispDrv;
UiControl *selfUiControl;

#define TFT_BL 2

#include "touch.h"

void readTouchpad(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    if (touch_has_signal()) {
        if (touch_touched()) {
            data->state = LV_INDEV_STATE_PR;
            data->point.x = touch_last_x;
            data->point.y = touch_last_y;
            selfUiControl->hasTouch();
        } else if (touch_released()) {
            data->state = LV_INDEV_STATE_REL;
            selfUiControl->withoutTouch();
        }
    } else {
        selfUiControl->withoutTouch();
        data->state = LV_INDEV_STATE_REL;
    }
}

void flushDisplay(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
#if (LV_COLOR_16_SWAP != 0)
    lcd.pushImageDMA(area->x1, area->y1, w, h, (lgfx::rgb565_t*) &color_p->full);
#else

    lcd.pushImageDMA(area->x1, area->y1, w, h, (lgfx::rgb565_t*) &color_p->full);
#endif
    lv_disp_flush_ready(disp);
}

void UiControl::backlightOn() {
    if (!backlightIsOn) {
        backlightIsOn = true;
        digitalWrite(TFT_BL, HIGH);
    }
}

void UiControl::backlightOff() {
    if (backlightIsOn) {
        triggerEvent((int) UiControlEvent::EventOnBacklightOff);
        backlightIsOn = false;
        digitalWrite(TFT_BL, LOW);
    }

}

UiControl::UiControl(int bcklTimeout) : backlightTimeout(bcklTimeout) {
    lcd.begin();
    lcd.fillScreen(lcd.color565(255,255,255));
    lcd.setTextSize(2);
    delay(200);
    pinMode(TFT_BL, OUTPUT);
    backlightOn();
    lv_init();
    touch_init();
    uint32_t screenWidth;
    uint32_t screenHeight;
    screenWidth = lcd.width();
    screenHeight = lcd.height();
    lv_disp_draw_buf_init(&drawBuf, buf, nullptr, screenWidth * screenHeight / 10);
    lv_disp_drv_init(&dispDrv);
    dispDrv.hor_res = screenWidth;
    dispDrv.ver_res = screenHeight;
    dispDrv.flush_cb = flushDisplay;
    dispDrv.draw_buf = &drawBuf;
    dispDrv.full_refresh = 1;
    lv_disp_drv_register(&dispDrv);
    static lv_indev_drv_t indevDrv;
    lv_indev_drv_init(&indevDrv);
    indevDrv.type = LV_INDEV_TYPE_POINTER;
    indevDrv.read_cb = readTouchpad;
    lv_indev_drv_register(&indevDrv);
    selfUiControl = this;
}

void UiControl::render() {
    if (UiMutex::take()) {
        toggleBacklight();
        lv_timer_handler();
        UiMutex::give();
    }
}

void UiControl::init() {
    ui_init();
}

void UiControl::withoutTouch() {
    if (timeWithoutTouch < (LONG_MAX - LV_INDEV_DEF_READ_PERIOD)) {
        timeWithoutTouch += LV_INDEV_DEF_READ_PERIOD;
    }
}

void UiControl::hasTouch() {
    timeWithoutTouch = 0;
}

void UiControl::toggleBacklight() {
    if (timeWithoutTouch > backlightTimeout) {
        backlightOff();
    } else {
        backlightOn();
    }
}
