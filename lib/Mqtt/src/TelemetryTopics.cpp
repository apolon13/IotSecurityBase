#include "TelemetryTopics.h"

using namespace std;

void TelemetryTopics::sendTelemetry(SimNetwork &network) {
    Topic regStatus("/security/telemetry/reg-status", pubSubClient);
    Topic sigQuality("/security/telemetry/sig-quality", pubSubClient);
    Topic modemTemperature("/security/telemetry/modem-temperature", pubSubClient);
    Topic chipTemperature("/security/telemetry/chip-temperature", pubSubClient);
    Topic freeHeap("/security/telemetry/free-heap", pubSubClient);
    Topic freePsram("/security/telemetry/free-psram", pubSubClient);

    regStatus.publish(to_string(network.getModem().getRegistrationStatus()));
    sigQuality.publish(to_string(network.getModem().getSignalQuality()));
    modemTemperature.publish(to_string(network.getModem().getTemperature()));
    chipTemperature.publish(to_string(temperatureRead()));
    freeHeap.publish(to_string(ESP.getFreeHeap()));
    freePsram.publish(to_string(ESP.getFreePsram()));
}
