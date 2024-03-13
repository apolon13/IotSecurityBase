#ifndef DISPLAY_SIMNETWORK_H
#define DISPLAY_SIMNETWORK_H

#define TINY_GSM_MODEM_SIM7600

#include "TinyGsmClient.h"
#include "StreamDebugger.h"
#include "Network.h"
#include "ProjectPreferences.h"

class SimNetwork : public Network {
protected:
    ProjectPreferences &projectPreferences;
    TinyGsmSim7600 *modem;
    TinyGsmClient client;
public:
    SimNetwork(ProjectPreferences &preferences, Stream &stream);

    bool isConnected() override;

    void connect() override;

    Client &getClient() override;
};

#endif
