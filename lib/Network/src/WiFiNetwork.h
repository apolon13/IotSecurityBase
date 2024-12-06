#include "Network.h"
#include "WiFi.h"
#include "ProjectPreferences.h"

#ifndef DISPLAY_WIFINETWORK_H
#define DISPLAY_WIFINETWORK_H

typedef struct {
    std::string ssid;
    std::string pass;
} WiFiCredentials;

class WiFiNetwork: public Network {
protected:
    WiFiCredentials credentials;
    WiFiClient wifiClient;
public:
    explicit WiFiNetwork(WiFiCredentials credentials);

    bool isConnected() override;

    void connect() override;

    Client &getClient() override;

    NetworkType getType() override;
};
#endif
