#include "Dispatcher.h"
#include "WiFiGeneric.h"
#include <PubSubClient.h>
#include <ESP32Ping.h>
#include <utility>
#include "PubSubDelegate.h"

WiFiClient espClient;
PubSubClient pubSub(espClient);

Dispatcher::Dispatcher(const ProjectPreferences& p, vector<Topic *> t) : projectPreferences(p), topics(std::move(t)) {
    WiFi.persistent(false);
    WiFi.mode(WIFI_MODE_APSTA);
    auto pubSubDelegate = new PubSubDelegate(&pubSub);
    for (auto topic: topics) {
        topic->withPubSub(pubSubDelegate);
    }
    pubSub.setCallback([this](char *topic, uint8_t *payload, unsigned int length) {
        string data;
        for (int i = 0; i < length; i++) {
            data.push_back((char)payload[i]);
        }
        for (auto item: topics) {
            if (item->getName() == topic) {
                item->handleData(data);
            }
        }
    });
}

void Dispatcher::connectToNetwork() {
    connectToWiFi();
}

bool Dispatcher::cloudIsConnected() {
    return pubSub.connected();
}

bool Dispatcher::networkIsConnected() {
    bool connected = WiFi.isConnected();
    return connected;
}

void Dispatcher::connectToWiFi() {
    if (networkConnectionInProcess) {
        return;
    }
    networkConnectionInProcess = true;
    string ssid = projectPreferences.get(ProjectPreferences::WifiSsid, "");
    string password = projectPreferences.get(ProjectPreferences::WifiPassword, "");
    WiFi.begin(ssid.c_str(), password.c_str());
    WiFi.printDiag(Serial);
    networkConnectionAttempts++;
    networkConnectionInProcess = false;
}

void Dispatcher::loop() {
    pubSub.loop();
}

void Dispatcher::connectToMqtt() {
    if (cloudConnectionInProcess) {
        return;
    }
    cloudConnectionInProcess = true;
    string defaultValue;
    string id = projectPreferences.get(ProjectPreferences::MqttEntityId, defaultValue);
    string username = projectPreferences.get(ProjectPreferences::MqttUsername, defaultValue);
    string pass = projectPreferences.get(ProjectPreferences::MqttPassword, defaultValue);
    string mqttServer = projectPreferences.get(ProjectPreferences::MqttIp, defaultValue);
    string mqttPort = projectPreferences.get(ProjectPreferences::MqttPort, defaultValue);
    if (!mqttServer.empty() && !mqttPort.empty()) {
        Serial.println("MQTT connecting ...");
        pubSub.disconnect();
        string debugInfo = "MQTT server - ";
        debugInfo.append((string) mqttServer + ":" + mqttPort + "\n");
        debugInfo.append("MQTT credentials - ");
        debugInfo.append((string) username + " " + pass + "\n");
        debugInfo.append("MQTT id - " + id);
        Serial.println(debugInfo.c_str());
        pubSub.setServer(mqttServer.c_str(), stoi(mqttPort));
        if (pubSub.connect(id.c_str(), username.c_str(), pass.c_str())) {
            Serial.println("MQTT connected");
            for (auto topic: topics) {
                pubSub.subscribe(topic->getName().c_str());
            }
        } else {
            IPAddress dns(8, 8, 8, 8);
            Serial.println("MQTT failed");
            if (!Ping.ping(dns, 3)) {
                Serial.println("Reconnect to network, ping failed");
                connectToWiFi();
            } else {
                Serial.println("Ethernet available");
            }
        }
    }
    cloudConnectionInProcess = false;
}

void Dispatcher::connectToCloud() {
    connectToMqtt();
}

int Dispatcher::getNetworkConnectionAttempts() const {
    return networkConnectionAttempts;
}

