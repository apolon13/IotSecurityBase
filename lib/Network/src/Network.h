#include "Client.h"
#include "WiFiClient.h"

#ifndef DISPLAY_NETWORK_H
#define DISPLAY_NETWORK_H

class Network {
public:
    virtual bool isConnected() = 0;

    virtual void connect() = 0;

    virtual Client &getClient() = 0;
};

#endif //DISPLAY_NETWORK_H
