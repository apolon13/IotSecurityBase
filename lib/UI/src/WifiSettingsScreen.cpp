#include "WifiSettingsScreen.h"

using namespace std;

void WifiSettingsScreen::loadFiWiSettings(lv_event_t *e) {
    string wifiSsid = store.get(Store::WifiSsid, "");
    string wifiPass = store.get(Store::WifiPassword, "");
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
    store.set(Store::WifiSsid, ssid);
    store.set(Store::WifiPassword, pass);
}