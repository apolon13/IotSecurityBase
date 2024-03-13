#include "Network.h"
#include "WiFi.h"
#include "ProjectPreferences.h"

#ifndef DISPLAY_WIFINETWORK_H
#define DISPLAY_WIFINETWORK_H
class WiFiNetwork: public Network {
protected:
    ProjectPreferences &projectPreferences;
    WiFiClient wifiClient;
public:
    WiFiNetwork(ProjectPreferences &preferences);

    bool isConnected() override;

    void connect() override;

    Client &getClient() override;
};
#endif
