#include "Network.h"
#include "Store.h"
#include "SimNetwork.h"
#include "WiFiNetwork.h"

#ifndef DISPLAY_NETWORKFACTORY_H
#define DISPLAY_NETWORKFACTORY_H

class NetworkFactory {
public:
    NetworkFactory() = default;

    std::unique_ptr<Network> create(Store &s);

    std::unique_ptr<SimNetwork> createSimNetwork();

    std::unique_ptr<WiFiNetwork> createWifiNetwork();
};

#endif
