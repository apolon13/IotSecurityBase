#include "LockScreen.h"
#include "UiMutex.h"

using namespace std;

void LockScreen::lockSystem(lv_event_t *e) {
    triggerEvent((int) LockScreenEvent::EventOnLock);
}

void LockScreen::goTo(bool useMutex) {
    auto cb = []() {
        lv_disp_load_scr(ui_lockScreen);
    };
    if (useMutex) {
        mutexWrap(cb);
    } else {
        cb();
    }
}

void LockScreen::unlockSystem(lv_event_t *e) {
    string currentPin = projectPreferences.get(ProjectPreferences::SystemPin, "");
    string userPin = lv_textarea_get_text(ui_pincode);
    bool isOk = false;

    if (currentPin.empty() && !userPin.empty()) {
        projectPreferences.set(ProjectPreferences::SystemPin, userPin);
        isOk = true;
    } else if (currentPin == userPin) {
        isOk = true;
    }

    string emptyValue;
    lv_textarea_set_text(ui_pincode, emptyValue.c_str());

    if (isOk) {
        triggerEvent((int) LockScreenEvent::EventOnUnlock);
    }
}