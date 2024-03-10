#include "Topic.h"

using namespace std;

string Topic::getName() {
    return topicName;
}

void Topic::handleData(const string &payload) {
    for (const auto &func: handlers) {
        func(payload);
    }
}

void Topic::addHandler(const function<void(string payload)> &handler) {
    handlers.push_back(handler);
}

void Topic::refreshHandlers() {
    handlers = {};
}

void Topic::publish(const string &payload) {
    client->publish(topicName, payload);
}

void Topic::withPubSub(PubSubDelegate *c) {
    client = c;
}
