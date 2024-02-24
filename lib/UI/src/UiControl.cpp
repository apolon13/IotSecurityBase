#include "UiControl.h"

static lv_disp_draw_buf_t drawBuf;
static lv_color_t buf[800 * 480 / 10];
static lv_disp_drv_t dispDrv;

#define TFT_BL 2

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
        GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
        40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 0 /* PCLK */,
        45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
        5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
        8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */
);

Arduino_RPi_DPI_RGBPanel *gfx = new Arduino_RPi_DPI_RGBPanel(
        bus,
        800 /* width */, 0 /* hsync_polarity */, 8 /* hsync_front_porch */, 8 /* hsync_pulse_width */,
        43 /* hsync_back_porch */,
        480 /* height */, 0 /* vsync_polarity */, 8 /* vsync_front_porch */, 8 /* vsync_pulse_width */,
        12 /* vsync_back_porch */,
        1 /* pclk_active_neg */, 12000000 /* prefer_speed */, true /* auto_flush */);

#include "touch.h"

void readTouchpad(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    if (touch_has_signal()) {
        if (touch_touched()) {
            data->state = LV_INDEV_STATE_PR;
            data->point.x = touch_last_x;
            data->point.y = touch_last_y;
        } else if (touch_released()) {
            data->state = LV_INDEV_STATE_REL;
        }
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

void flushDisplay(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
#if (LV_COLOR_16_SWAP != 0)
    lcd->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *) &color_p->full, w, h);
#endif
    lv_disp_flush_ready(disp);
}

UiControl::UiControl(ProjectPreferences *p, IoTRadioDetect *i, Dispatcher *d, QueueTask *q, IotRadioControl *ct) {
    gfx->begin();
    gfx->fillScreen(BLACK);
    gfx->setTextSize(2);
    delay(200);
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    lv_init();
    touch_init();
    uint32_t screenWidth;
    uint32_t screenHeight;
    screenWidth = gfx->width();
    screenHeight = gfx->height();
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
    gfx->fillScreen(BLACK);
    eventHandler = new UiEventHandler(p, i, d, q, ct);
}

void UiControl::render() {
    if (UiMutex::take()) {
        lv_timer_handler();
        UiMutex::give();
    }
}

UiEventHandler *UiControl::getEventHandler() {
    return eventHandler;
}

void UiControl::init() {
    ui_init();
}