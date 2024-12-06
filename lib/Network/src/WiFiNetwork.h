#include "Network.h"
#include "WiFi.h"
#include "Store.h"

#ifndef DISPLAY_WIFINETWORK_H
#define DISPLAY_WIFINETWORK_H

typedef struct WiFiCredentials {
    std::string ssid;
    std::string pass;
    bool operator!=(WiFiCredentials b) const {
        return ssid != b.ssid || pass != b.pass;
    }
} WiFiCredentials;


class WiFiNetwork: public Network {
protected:
    WiFiCredentials currentCreds;
    WiFiClient client;
public:
    explicit WiFiNetwork();

    bool isConnected() override;

    void connect(void *) override;

    Client &getClient() override;

    NetworkType getType() override;
};
#endif
