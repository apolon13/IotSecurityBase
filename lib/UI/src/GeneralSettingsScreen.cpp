#include "GeneralSettingsScreen.h"

using namespace std;

void GeneralSettingsScreen::saveGeneralSettings(lv_event_t *e) {
    string newPin = lv_textarea_get_text(ui_settingsPincodeInput);
    string newConnectionTimeout = lv_textarea_get_text(ui_settingsConnectionTimeoutInput);
    string newSecurityTimeout = lv_textarea_get_text(ui_settingsTimeoutInput);
    string newConnectionAttemptsBeforeRestart = lv_textarea_get_text(ui_settingsMaxAttemptsInput);
    string phone = lv_textarea_get_text(ui_settingsPhoneInput);
    bool networkModeIsWifi = static_cast<bool>(lv_obj_get_state(ui_wiFiNetworkType) & LV_STATE_CHECKED);
    bool networkModeIsSim = static_cast<bool>(lv_obj_get_state(ui_simNetworkType) & LV_STATE_CHECKED);

    projectPreferences.set(ProjectPreferences::SystemPin, newPin);
    projectPreferences.set(ProjectPreferences::ConnectionTimeout, newConnectionTimeout);
    projectPreferences.set(ProjectPreferences::SecurityTimeout, newSecurityTimeout);
    projectPreferences.set(ProjectPreferences::Phone, phone);
    projectPreferences.set(ProjectPreferences::ConnectionAttemptsBeforeRestart, newConnectionAttemptsBeforeRestart);
    projectPreferences.setNetworkMode((networkModeIsWifi || !networkModeIsSim) ? ProjectPreferences::WifiNetworkMode : ProjectPreferences::SimNetworkMode);
    triggerEvent((int)GeneralSettingsScreenEvent::EventOnUpdateSettings);
}

void GeneralSettingsScreen::loadGeneralSettings(lv_event_t *e) {
    lv_textarea_set_text(ui_settingsPincodeInput, projectPreferences.get(ProjectPreferences::SystemPin, "").c_str());
    lv_textarea_set_text(ui_settingsConnectionTimeoutInput, projectPreferences.getConnectionTimeout().c_str());
    lv_textarea_set_text(ui_settingsTimeoutInput, projectPreferences.getSecurityTimeout().c_str());
    lv_textarea_set_text(ui_settingsMaxAttemptsInput, projectPreferences.getConnectionAttemptsBeforeRestart().c_str());
    lv_textarea_set_text(ui_settingsPhoneInput, projectPreferences.get(ProjectPreferences::Phone, "").c_str());

    if (projectPreferences.networkModeIsWifi()) {
        lv_obj_add_state(ui_wiFiNetworkType, LV_STATE_CHECKED);
        lv_obj_clear_state(ui_simNetworkType, LV_STATE_CHECKED);
    }

    if (projectPreferences.networkModeIsSim()) {
        lv_obj_add_state(ui_simNetworkType, LV_STATE_CHECKED);
        lv_obj_clear_state(ui_wiFiNetworkType, LV_STATE_CHECKED);
    }
}