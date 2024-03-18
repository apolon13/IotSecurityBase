#include "SimNetwork.h"
#include "WiFi.h"
//#include "StreamDebugger.h"
//StreamDebugger debug(Serial1, Serial);

SimNetwork::SimNetwork(ProjectPreferences &preferences, Stream &stream) : projectPreferences(preferences) {
    modem = new TinyGsmSim7670(stream);
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

TinyGsmSim7670 &SimNetwork::getModem() {
    return *modem;
}

NetworkType SimNetwork::getType() {
    return NetworkType::Sim;
}
