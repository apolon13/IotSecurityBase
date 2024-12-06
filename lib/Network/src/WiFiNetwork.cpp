#include "WiFiNetwork.h"

WiFiNetwork::WiFiNetwork(WiFiCredentials c) : credentials(c) {

}

bool WiFiNetwork::isConnected() {
    return WiFi.isConnected();
}

void WiFiNetwork::connect() {
    WiFi.begin(credentials.ssid.c_str(), credentials.pass.c_str());
}

Client &WiFiNetwork::getClient() {
    return wifiClient;
}

NetworkType WiFiNetwork::getType() {
    return NetworkType::WiFi;
}
