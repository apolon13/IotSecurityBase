#include "WiFiConfigurator.h"
#include "WiFi.h"

void WiFiConfigurator::configure(NetworkType type) {
    WiFi.persistent(false);
    WiFi.setSleep(false);
    if (type == NetworkType::WiFi) {
        WiFi.mode(WIFI_MODE_APSTA);
        WiFi.setAutoReconnect(false);
        WiFi.setScanMethod(WIFI_FAST_SCAN);
    }

    if (type == NetworkType::Sim) {
        WiFi.mode(WIFI_MODE_STA);
    }
}
