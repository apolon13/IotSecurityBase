#include <WiFi.h>
#include "Telemetry.h"

using namespace std;

void Telemetry::send() {
    pubSub.publish(PubSub::ChipTelemetry, to_string(temperatureRead()));
    pubSub.publish(PubSub::HeapTelemetry, to_string(ESP.getFreeHeap()));
    pubSub.publish(PubSub::PSRAMTelemetry, to_string(ESP.getFreePsram()));
    pubSub.publish(PubSub::WiFiTelemetry, to_string(WiFi.channel()));
}
