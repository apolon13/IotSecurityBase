#include "Client.h"
#include "WiFiClient.h"

#ifndef DISPLAY_NETWORK_H
#define DISPLAY_NETWORK_H

enum class NetworkType {
    Sim,
    WiFi
};

class Network {
public:
    virtual bool isConnected() = 0;

    virtual void connect(void *) = 0;

    virtual Client &getClient() = 0;

    virtual NetworkType getType() = 0;
};

#endif //DISPLAY_NETWORK_H
