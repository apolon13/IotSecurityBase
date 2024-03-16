#include "Dispatcher.h"
#include "WiFiGeneric.h"
#include <PubSubClient.h>

using namespace std;

Dispatcher::Dispatcher(ProjectPreferences &p, TopicsContainer &t, Network &n, PubSubClient &psc) : projectPreferences(p),
                                                                                topicsContainer(t), network(n), pubSubClient(psc) {
    pubSubClient.setCallback([this](char *topic, uint8_t *payload, unsigned int length) {
        string data;
        for (int i = 0; i < length; i++) {
            data.push_back((char) payload[i]);
        }
        topicsContainer.handleSubscribeData(topic, data);
    });
}

bool Dispatcher::cloudIsConnected() {
    return pubSubClient.connected();
}

bool Dispatcher::networkIsConnected() {
    return network.isConnected();
}

void Dispatcher::connectToNetwork() {
    if (networkConnectionInProcess) {
        return;
    }
    networkConnectionInProcess = true;
    network.connect();
    networkConnectionAttempts++;
    networkConnectionInProcess = false;
}

void Dispatcher::loop() {
    pubSubClient.loop();
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
        //Serial.println("MQTT connecting ...");
        pubSubClient.disconnect();
        string debugInfo = "MQTT server - ";
        debugInfo.append((string) mqttServer + ":" + mqttPort + "\n");
        debugInfo.append("MQTT credentials - ");
        debugInfo.append((string) username + " " + pass + "\n");
        debugInfo.append("MQTT id - " + id);
        //Serial.println(debugInfo.c_str());
        pubSubClient.setServer(mqttServer.c_str(), stoi(mqttPort));
        if (pubSubClient.connect(id.c_str(), username.c_str(), pass.c_str())) {
            //Serial.println("MQTT connected");
            for (const auto &name: topicsContainer.getTopicNames()) {
                pubSubClient.subscribe(name.c_str());
            }
        } else {
            connectToNetwork();
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

