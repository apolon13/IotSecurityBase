#include "SimNetwork.h"
#include "WiFiNetwork.h"
#include "NetworkFactory.h"

std::unique_ptr<Network> NetworkFactory::createNetwork() {
    if (!projectPreferences.networkModeIsWifi()) {
        return std::make_unique<SimNetwork>(projectPreferences, Serial1);
    } else {
        return std::make_unique<WiFiNetwork>(projectPreferences);
    }
}
