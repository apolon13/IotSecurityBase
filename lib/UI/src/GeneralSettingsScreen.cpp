#include "GeneralSettingsScreen.h"

void GeneralSettingsScreen::saveGeneralSettings(lv_event_t *e) {
    string newPin = lv_textarea_get_text(ui_settingsPincodeInput);
    projectPreferences->set(ProjectPreferences::SystemPin, newPin);
}

void GeneralSettingsScreen::loadGeneralSettings(lv_event_t *e) {
    string currentPin = projectPreferences->get(ProjectPreferences::SystemPin, "");
    lv_textarea_set_text(ui_settingsPincodeInput, currentPin.c_str());
}