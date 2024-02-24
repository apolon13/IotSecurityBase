#include "LockScreen.h"
#include "UiMutex.h"

void mutexWrap(std::function<void()> cb) {
    if (UiMutex::take()) {
        cb();
        UiMutex::give();
    }
}

void LockScreen::lockSystem(lv_event_t *e) {
    projectPreferences->lockSystem();
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

void LockScreen::unlockSystem(lv_event_t *e, bool useMutex, bool ignorePin) {
    auto cb = [this, ignorePin]() {
        string currentPin = projectPreferences->get(SystemPin, "");
        string userPin = lv_textarea_get_text(ui_pincode);
        bool isOk = ignorePin;

        if (currentPin.empty() && !userPin.empty()) {
            projectPreferences->set(SystemPin, userPin);
            isOk = true;
        } else if (currentPin == userPin) {
            isOk = true;
        }

        if (isOk) {
            projectPreferences->unlockSystem();
            lv_disp_load_scr(ui_home);
        }

        string emptyValue;
        lv_textarea_set_text(ui_pincode, emptyValue.c_str());
    };

    if (useMutex) {
        mutexWrap(cb);
    } else {
        cb();
    }
}