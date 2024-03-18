#include "lvgl.h"
#include "EventableObject.h"

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

public:
    explicit UiControl(int bcklTimeout);

    void render();

    void init();

    void backlightOn();

    void backlightOff(bool triggerEvent = true);

    void toggleBacklight();

    void withoutTouch();

    void simulateTouch();
};

#endif
