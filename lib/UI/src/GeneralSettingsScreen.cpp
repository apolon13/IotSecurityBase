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

    store.set(Store::SystemPin, newPin);
    store.set(Store::ConnectionTimeout, newConnectionTimeout);
    store.set(Store::SecurityTimeout, newSecurityTimeout);
    store.set(Store::Phone, phone);
    store.set(Store::ConnectionAttemptsBeforeRestart, newConnectionAttemptsBeforeRestart);
    store.setNetworkMode((networkModeIsWifi || !networkModeIsSim) ? Store::WifiNetworkMode : Store::SimNetworkMode);
    triggerEvent((int)GeneralSettingsScreenEvent::EventOnUpdateSettings);
}

void GeneralSettingsScreen::loadGeneralSettings(lv_event_t *e) {
    lv_textarea_set_text(ui_settingsPincodeInput, store.get(Store::SystemPin, "").c_str());
    lv_textarea_set_text(ui_settingsConnectionTimeoutInput, store.getConnectionTimeout().c_str());
    lv_textarea_set_text(ui_settingsTimeoutInput, store.getSecurityTimeout().c_str());
    lv_textarea_set_text(ui_settingsMaxAttemptsInput, store.getConnectionAttemptsBeforeRestart().c_str());
    lv_textarea_set_text(ui_settingsPhoneInput, store.get(Store::Phone, "").c_str());

    if (store.networkModeIsWifi()) {
        lv_obj_add_state(ui_wiFiNetworkType, LV_STATE_CHECKED);
        lv_obj_clear_state(ui_simNetworkType, LV_STATE_CHECKED);
    }

    if (store.networkModeIsSim()) {
        lv_obj_add_state(ui_simNetworkType, LV_STATE_CHECKED);
        lv_obj_clear_state(ui_wiFiNetworkType, LV_STATE_CHECKED);
    }
}