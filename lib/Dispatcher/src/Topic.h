#include <string>
#include <vector>
#include <functional>
#include <map>
#include "Arduino.h"

#ifndef DISPLAY_TOPIC_H
#define DISPLAY_TOPIC_H

using namespace std;

class Topic {
private:
    string topicName;
    vector<function<void(string payload)>> handlers;
public:
    explicit Topic(string n) : topicName(n) {};

    string getName();

    void addHandler(function<void(string payload)> handler);

    void refreshHandlers();

    void handleData(string payload);
};

#endif
