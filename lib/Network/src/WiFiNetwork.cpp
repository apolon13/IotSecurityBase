#include "WiFiNetwork.h"

WiFiNetwork::WiFiNetwork(ProjectPreferences &preferences) : projectPreferences(preferences) {
    WiFi.persistent(false);
    WiFi.mode(WIFI_MODE_APSTA);
}

bool WiFiNetwork::isConnected() {
    return WiFi.isConnected();
}

void WiFiNetwork::connect() {
    auto ssid = projectPreferences.get(ProjectPreferences::WifiSsid, "");
    auto pass = projectPreferences.get(ProjectPreferences::WifiPassword, "");
    WiFi.begin(ssid.c_str(), pass.c_str());
}

Client &WiFiNetwork::getClient() {
    return wifiClient;
}