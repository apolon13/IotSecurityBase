#include "NetworkFactory.h"

std::unique_ptr<Network> NetworkFactory::createNetwork() {
    std::unique_ptr<Network> network;
    WiFi.persistent(false);
    WiFi.setSleep(false);
    if (projectPreferences.networkModeIsSim()) {
        network = createSimNetwork();
    } else {
        network = createWifiNetwork();
    }
    return network;
}

std::unique_ptr<SimNetwork> NetworkFactory::createSimNetwork() {
    WiFi.mode(WIFI_MODE_STA);
    return std::make_unique<SimNetwork>(SimCredentials{projectPreferences.get(ProjectPreferences::APN, "")}, stream);
}

std::unique_ptr<WiFiNetwork> NetworkFactory::createWifiNetwork() {
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.setScanMethod(WIFI_FAST_SCAN);
    return std::make_unique<WiFiNetwork>(WiFiCredentials{
            projectPreferences.get(ProjectPreferences::WifiSsid, ""),
            projectPreferences.get(ProjectPreferences::WifiPassword, "")
    });
}
