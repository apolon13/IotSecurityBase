#include "ProjectPreferences.h"
#include "ui.h"
#include "Screen.h"

#ifndef DISPLAY_WIFISETTINGSSCREEN_H
#define DISPLAY_WIFISETTINGSSCREEN_H

class WifiSettingsScreen : public Screen {
private:
    ProjectPreferences& projectPreferences;
public:
    WifiSettingsScreen(ProjectPreferences& p) : projectPreferences(p) {};

    void loadFiWiSettings(lv_event_t *e);

    void saveWifiSettings(lv_event_t *e);
};

#endif
