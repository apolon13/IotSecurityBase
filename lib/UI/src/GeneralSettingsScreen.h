#include "ProjectPreferences.h"
#include "ui.h"
#include "Screen.h"

#ifndef DISPLAY_GENERALSETTINGSSCREEEN_H
#define DISPLAY_GENERALSETTINGSSCREEEN_H

enum class GeneralSettingsScreenEvent {
    EventOnUpdateSettings
};

class GeneralSettingsScreen : public Screen {
private:
    ProjectPreferences &projectPreferences;
public:
    explicit GeneralSettingsScreen(ProjectPreferences &p) : projectPreferences(p) {};

    void loadGeneralSettings(lv_event_t *e);

    void saveGeneralSettings(lv_event_t *e);
};

#endif
