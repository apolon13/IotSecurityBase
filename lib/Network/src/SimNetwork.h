#ifndef DISPLAY_SIMNETWORK_H
#define DISPLAY_SIMNETWORK_H

#include "TinyGsmClient7670.h"
#include "StreamDebugger.h"
#include "Network.h"
#include "ProjectPreferences.h"

class SimNetwork : public Network {
protected:
    ProjectPreferences &projectPreferences;
    TinyGsmSim7670 *modem;
    TinyGsmSim7670::GsmClientSim7670 client;
public:
    SimNetwork(ProjectPreferences &preferences, Stream &stream);

    bool isConnected() override;

    void connect() override;

    Client &getClient() override;

    TinyGsmSim7670 &getModem();

    NetworkType getType() override;
};

#endif
