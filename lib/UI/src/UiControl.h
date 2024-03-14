#include "lvgl.h"
#include <SPI.h>
#include "map"
#include "LovyanGFX.h"
#include "Wire.h"
#include "TAMC_GT911.h"
#include "ScreenFactory.h"
#include "UiMutex.h"
#include "QueueTask.h"
#include "IotRadioControl.h"

#ifndef DISPLAY_UICONTROL_H
#define DISPLAY_UICONTROL_H

enum class UiControlEvent {
    EventOnBacklightOff
};

class UiControl : public EventableObject {
private:
    long timeWithoutTouch = 0;
    bool backlightIsOn = false;
    int backlightTimeout;
    lv_disp_draw_buf_t drawBuffer;
    lv_color_t *buffer = new lv_color_t[800 * 480 / 10];
    lv_disp_drv_t displayDriver;
    lv_indev_drv_t indevDriver;

public:
    explicit UiControl(int bcklTimeout);

    void render();

    void init();

    void backlightOn();

    void backlightOff();

    void toggleBacklight();

    void withoutTouch();

    void hasTouch();
};

#endif
