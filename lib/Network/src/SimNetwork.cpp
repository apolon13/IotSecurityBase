#include "SimNetwork.h"
#include "WiFi.h"

SimNetwork::SimNetwork(SimCredentials c, Stream &stream) : credentials(c) {
    modem = new TinyGsmSim7670(stream);
    client.init(modem);
}

bool SimNetwork::isConnected() {
    return modem->isNetworkConnected();
}

void SimNetwork::connect() {
    modem->setNetworkMode(38); // LTE only
    modem->setSmsTextMode();
    modem->gprsConnect(credentials.apn.c_str(), "", "");
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
