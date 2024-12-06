#include "Store.h"
#include "ui.h"
#include "Screen.h"

#ifndef DISPLAY_GENERALSETTINGSSCREEEN_H
#define DISPLAY_GENERALSETTINGSSCREEEN_H

enum class GeneralSettingsScreenEvent {
    EventOnUpdateSettings
};

class GeneralSettingsScreen : public Screen {
private:
    Store &store;
public:
    explicit GeneralSettingsScreen(Store &p) : store(p) {};

    void loadGeneralSettings(lv_event_t *e);

    void saveGeneralSettings(lv_event_t *e);
};

#endif
