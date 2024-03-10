#include "WifiSettingsScreen.h"

using namespace std;

void WifiSettingsScreen::loadFiWiSettings(lv_event_t *e) {
    string wifiSsid = projectPreferences.get(ProjectPreferences::WifiSsid, "");
    string wifiPass = projectPreferences.get(ProjectPreferences::WifiPassword, "");
    if (!wifiSsid.empty()) {
        lv_textarea_set_text(ui_wifiSsid, wifiSsid.c_str());
    }
    if (!wifiPass.empty()) {
        lv_textarea_set_text(ui_wifiPassword, wifiPass.c_str());
    }
}

void WifiSettingsScreen::saveWifiSettings(lv_event_t *e) {
    string ssid = lv_textarea_get_text(ui_wifiSsid);
    string pass = lv_textarea_get_text(ui_wifiPassword);
    projectPreferences.set(ProjectPreferences::WifiSsid, ssid);
    projectPreferences.set(ProjectPreferences::WifiPassword, pass);
}