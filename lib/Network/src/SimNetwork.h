#ifndef DISPLAY_SIMNETWORK_H
#define DISPLAY_SIMNETWORK_H

#include "TinyGsmClient7670.h"
#include "StreamDebugger.h"
#include "Network.h"
#include "ProjectPreferences.h"

typedef struct {
    std::string apn;
} SimCredentials;

class SimNetwork : public Network {
protected:
    SimCredentials credentials;
    TinyGsmSim7670 *modem;
    TinyGsmSim7670::GsmClientSim7670 client;
public:
    SimNetwork(SimCredentials credentials, Stream &stream);

    bool isConnected() override;

    void connect() override;

    Client &getClient() override;

    TinyGsmSim7670 &getModem();

    NetworkType getType() override;
};

#endif
