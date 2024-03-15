#include "SimNetwork.h"
#include "WiFi.h"
//#include "StreamDebugger.h"
//StreamDebugger debug(Serial, Serial1);

SimNetwork::SimNetwork(ProjectPreferences &preferences, Stream &stream) : projectPreferences(preferences) {
    WiFi.mode(WIFI_MODE_STA);
    modem = new TinyGsm(stream);
    client.init(modem);
}

bool SimNetwork::isConnected() {
    return modem->isNetworkConnected();
}

void SimNetwork::connect() {
    auto apn = projectPreferences.get(ProjectPreferences::APN, "");
    modem->gprsConnect(apn.c_str(), "", "");
}

Client &SimNetwork::getClient() {
    return client;
}
