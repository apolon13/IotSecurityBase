#include "NetworkFactory.h"

std::unique_ptr<Network> NetworkFactory::createNetwork() {
    if (projectPreferences.networkModeIsSim()) {
        return createSimNetwork();
    } else {
        return createWifiNetwork();
    }
}

std::unique_ptr<SimNetwork> NetworkFactory::createSimNetwork() {
    return std::make_unique<SimNetwork>(projectPreferences, stream);
}

std::unique_ptr<WiFiNetwork> NetworkFactory::createWifiNetwork() {
    return std::make_unique<WiFiNetwork>(projectPreferences);
}
