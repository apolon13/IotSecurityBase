#include "Topic.h"

using namespace std;

string Topic::name() {
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
