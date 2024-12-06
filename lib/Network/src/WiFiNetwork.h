#include "Network.h"
#include "WiFi.h"
#include "Store.h"

#ifndef DISPLAY_WIFINETWORK_H
#define DISPLAY_WIFINETWORK_H

typedef struct {
    std::string ssid;
    std::string pass;
} WiFiCredentials;

class WiFiNetwork: public Network {
protected:
    WiFiClient client;
public:
    explicit WiFiNetwork();

    bool isConnected() override;

    void connect(void *) override;

    Client &getClient() override;

    NetworkType getType() override;
};
#endif
