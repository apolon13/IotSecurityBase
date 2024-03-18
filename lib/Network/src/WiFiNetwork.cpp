#include "WiFiNetwork.h"

WiFiNetwork::WiFiNetwork(ProjectPreferences &preferences) : projectPreferences(preferences) {

}

bool WiFiNetwork::isConnected() {
    return WiFi.isConnected();
}

void WiFiNetwork::connect() {
    auto ssid = projectPreferences.get(ProjectPreferences::WifiSsid, "");
    auto pass = projectPreferences.get(ProjectPreferences::WifiPassword, "");
    WiFi.disconnect(true, true);
    WiFi.begin(ssid.c_str(), pass.c_str(), 1, nullptr, true);
}

Client &WiFiNetwork::getClient() {
    return wifiClient;
}

NetworkType WiFiNetwork::getType() {
    return NetworkType::WiFi;
}
