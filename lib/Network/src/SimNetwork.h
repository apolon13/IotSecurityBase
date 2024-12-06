#include "TinyGsmClient7670.h"
#include "StreamDebugger.h"
#include "Network.h"
#include "Store.h"

#ifndef DISPLAY_SIMNETWORK_H
#define DISPLAY_SIMNETWORK_H

typedef struct SimCredentials {
    std::string apn;
    bool operator!=(SimCredentials b) const {
        return apn != b.apn;
    }
} SimCredentials;

class SimNetwork : public Network {
protected:
    SimCredentials currentCreds;
    TinyGsmSim7670 *modem;
    TinyGsmSim7670::GsmClientSim7670 client;
public:
    explicit SimNetwork(Stream &stream);

    bool isConnected() override;

    void connect(void *) override;

    Client &getClient() override;

    TinyGsmSim7670 &getModem();

    NetworkType getType() override;
};

#endif
