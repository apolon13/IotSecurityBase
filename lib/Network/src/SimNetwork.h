#ifndef DISPLAY_SIMNETWORK_H
#define DISPLAY_SIMNETWORK_H

#include "TinyGsmClient7670.h"
#include "StreamDebugger.h"
#include "Network.h"
#include "Store.h"

typedef struct {
    std::string apn;
} SimCredentials;

class SimNetwork : public Network {
protected:
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
