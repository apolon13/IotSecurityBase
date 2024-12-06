#include "PubSub.h"


PubSub::PubSub(std::map<int, Topic> &t, Network &n) : topics(t) {
    c = PubSubClient(n.getClient());
    c.setCallback([this](char *topic, uint8_t *payload, unsigned int length) {
        std::string data;
        for (int i = 0; i < length; i++) {
            data.push_back((char) payload[i]);
        }
        for (auto &[k, v] : topics) {
            if (v.name() == topic) {
                v.handleData(data);
            }
        }
    });

    for (auto [k, v]: topics) {
        c.subscribe(v.name().c_str());
    }
}

PubSubClient PubSub::client() {
    return c;
}

void PubSub::publish(Topics topic, std::string data) {
   c.publish(topics.find(topic)->second.name().c_str(), data.c_str());
}

void PubSub::onRecv(Topics topic, const std::function<void(const std::string &)> &onRecv) {
    topics.find(topic)->second.addHandler(onRecv);
}

void PubSub::connect(MqttCredentials cred) {
    if (!cred.server.empty() && !cred.port.empty()) {
        c.disconnect();
        c.setServer(cred.server.c_str(), stoi(cred.port));
        c.connect(cred.id.c_str(), cred.user.c_str(), cred.pass.c_str());
    }
}

bool PubSub::isConnected() {
    return c.connected();
}

void PubSub::listen() {
    c.loop();
}

