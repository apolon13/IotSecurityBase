#include "WiFiNetwork.h"

WiFiNetwork::WiFiNetwork() {

}

bool WiFiNetwork::isConnected() {
    return WiFi.isConnected();
}

void WiFiNetwork::connect(void * data) {
    auto creds = (WiFiCredentials *) data;
    WiFi.begin(creds->ssid.c_str(), creds->pass.c_str());
}

Client &WiFiNetwork::getClient() {
    return client;
}

NetworkType WiFiNetwork::getType() {
    return NetworkType::WiFi;
}
