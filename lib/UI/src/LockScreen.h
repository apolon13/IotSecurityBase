#include "Store.h"
#include "ui.h"
#include "Screen.h"

#ifndef DISPLAY_LOCKSCREEN_H
#define DISPLAY_LOCKSCREEN_H

enum class LockScreenEvent {
    EventOnLock,
    EventOnUnlock
};

class LockScreen : public Screen {
private:
    Store &store;
public:
    explicit LockScreen(Store &p) : store(p) {};

    void unlockSystem(lv_event_t *e);

    void lockSystem(lv_event_t *e);

    void goTo(bool lockScreenState = false);
};

#endif
