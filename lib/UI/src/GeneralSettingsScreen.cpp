#include "GeneralSettingsScreen.h"

void GeneralSettingsScreen::saveGeneralSettings(lv_event_t *e) {
    string newPin = lv_textarea_get_text(ui_settingsPincodeInput);
    string newConnectionTimeout = lv_textarea_get_text(ui_settingsConnectionTimeoutInput);
    string newSecurityTimeout = lv_textarea_get_text(ui_settingsTimeoutInput);
    string newConnectionAttemptsBeforeRestart = lv_textarea_get_text(ui_settingsMaxAttemptsInput);

    projectPreferences.set(ProjectPreferences::SystemPin, newPin);
    projectPreferences.set(ProjectPreferences::ConnectionTimeout, newConnectionTimeout);
    projectPreferences.set(ProjectPreferences::SecurityTimeout, newSecurityTimeout);
    projectPreferences.set(ProjectPreferences::ConnectionAttemptsBeforeRestart, newConnectionAttemptsBeforeRestart);
}

void GeneralSettingsScreen::loadGeneralSettings(lv_event_t *e) {
    lv_textarea_set_text(ui_settingsPincodeInput, projectPreferences.get(ProjectPreferences::SystemPin, "").c_str());
    lv_textarea_set_text(ui_settingsConnectionTimeoutInput, projectPreferences.getConnectionTimeout().c_str());
    lv_textarea_set_text(ui_settingsTimeoutInput, projectPreferences.getSecurityTimeout().c_str());
    lv_textarea_set_text(ui_settingsMaxAttemptsInput, projectPreferences.getConnectionAttemptsBeforeRestart().c_str());
}