#include "Dispatcher.h"
#include "WiFiGeneric.h"
#include <PubSubClient.h>
#include <ESP32Ping.h>

#include <utility>


WiFiClient espClient;
PubSubClient pubSub(espClient);

Dispatcher::Dispatcher(ProjectPreferences *p, Logger *l, vector<Topic *> t) : projectPreferences(p), logger(l),
                                                                              topics(std::move(t)) {
    logger->debug("Init dispatcher");
    WiFi.persistent(false);
    WiFi.mode(WIFI_MODE_APSTA);
    pubSub.setCallback([this](char *topic, byte *payload, unsigned int length) {
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

bool Dispatcher::hasNeedReconnectCloud() {
    return needReconnectCloud;
}

bool Dispatcher::cloudIsConnected() {
    // logger->debug(connected ? "pubsub connected" : "pubsub disconnected");
    return pubSub.connected();
}

bool Dispatcher::networkIsConnected() {
    bool connected = WiFi.isConnected();
    //logger->debug(connected ? "network connected" : "network disconnected");
    //logger->debug(WiFi.macAddress().c_str());
    return connected;
}

void Dispatcher::connectToWiFi() {
    if (networkConnectionInProcess) {
        return;
    }
    networkConnectionInProcess = true;
    string ssid = projectPreferences->get(WifiSsid, "");
    string password = projectPreferences->get(WifiPassword, "");
    WiFi.begin(ssid.c_str(), password.c_str());
    WiFi.printDiag(Serial);
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
    string id = projectPreferences->get(MqttEntityId, defaultValue);
    string username = projectPreferences->get(MqttUsername, defaultValue);
    string pass = projectPreferences->get(MqttPassword, defaultValue);
    string mqttServer = projectPreferences->get(MqttIp, defaultValue);
    string mqttPort = projectPreferences->get(MqttPort, defaultValue);
    if (!mqttServer.empty() && !mqttPort.empty()) {
        logger->debug("MQTT connecting ...");
        pubSub.disconnect();
        string debugInfo = "MQTT server - ";
        debugInfo.append((string) mqttServer + ":" + mqttPort + "\n");
        debugInfo.append("MQTT credentials - ");
        debugInfo.append((string) username + " " + pass + "\n");
        debugInfo.append("MQTT id - " + id);
        logger->debug(debugInfo);
        pubSub.setServer(mqttServer.c_str(), stoi(mqttPort));
        if (pubSub.connect(id.c_str(), username.c_str(), pass.c_str())) {
            logger->debug("MQTT connected");
            for (auto topic: topics) {
                pubSub.subscribe(topic->getName().c_str());
            }
        } else {
            IPAddress dns(8, 8, 8, 8);
            logger->debug("MQTT failed, status code = " + to_string(pubSub.state()));
            if (!Ping.ping(dns, 1)) {
                logger->debug("Reconnect to network, ping failed");
                espClient.flush();
                espClient.stop();
                connectToWiFi();
            } else {
                logger->debug("Ethernet available");
            }
        }
    }
    needReconnectCloud = false;
    cloudConnectionInProcess = false;
}

void Dispatcher::reconnectToCloud() {
    needReconnectCloud = true;
}

void Dispatcher::connectToCloud() {
    connectToMqtt();
}

