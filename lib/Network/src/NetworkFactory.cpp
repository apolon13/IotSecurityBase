#include "NetworkFactory.h"

std::unique_ptr<Network> NetworkFactory::create(Store &s) {
    std::unique_ptr<Network> network;
    WiFi.persistent(false);
    WiFi.setSleep(false);
    if (s.networkModeIsSim()) {
        network = createSimNetwork();
    } else {
        network = createWifiNetwork();
    }
    return network;
}

std::unique_ptr<SimNetwork> NetworkFactory::createSimNetwork() {
    WiFi.mode(WIFI_MODE_STA);
    Serial1.begin(115200, SERIAL_8N1, GPIO_NUM_18, GPIO_NUM_17);
    return std::make_unique<SimNetwork>(Serial1);
}

std::unique_ptr<WiFiNetwork> NetworkFactory::createWifiNetwork() {
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.setScanMethod(WIFI_FAST_SCAN);
    return std::make_unique<WiFiNetwork>();
}
