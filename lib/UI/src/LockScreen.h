#include "ProjectPreferences.h"
#include "ui.h"
#include "Screen.h"

#ifndef DISPLAY_LOCKSCREEN_H
#define DISPLAY_LOCKSCREEN_H
class LockScreen: public Screen {
private:
    ProjectPreferences *projectPreferences;
public:
    explicit LockScreen(ProjectPreferences *p): projectPreferences(p) {};
    void unlockSystem(lv_event_t *e, bool useMutex = false, bool ignorePin = false);
    void lockSystem(lv_event_t *e);
    void goTo(bool useMutex = false);
};
#endif //DISPLAY_LOCKSCREEN_H
