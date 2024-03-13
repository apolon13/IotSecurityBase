#include "SimNetwork.h"
#include "WiFi.h"

SimNetwork::SimNetwork(ProjectPreferences &preferences, Stream &stream) : projectPreferences(preferences) {
    WiFi.mode(WIFI_MODE_STA);
    modem = new TinyGsm(stream);
    client.init(modem);
}

bool SimNetwork::isConnected() {
    return modem->isNetworkConnected();
}

void SimNetwork::connect() {
    modem->init();
    while (modem->getRegistrationStatus() == -1) {
        delay(200);
    }
    auto apn = projectPreferences.get(ProjectPreferences::APN, "");
    modem->gprsConnect(apn.c_str(), "", "");
    modem->waitForNetwork();
}

Client &SimNetwork::getClient() {
    return client;
}
