#include "Topic.h"
#include "set"
#include "Network.h"

#ifndef IOTSECURITYBASE_PSCLIENT_H
#define IOTSECURITYBASE_PSCLIENT_H

typedef struct {
    std::string id;
    std::string pass;
    std::string user;
    std::string server;
    std::string port;
} MqttCredentials;

class PubSub {
private:
    PubSubClient c;
    std::map<int, Topic> &topics;
public:
    enum Topics {
        Alarm,
        Receive,
        Command,
        ChipTelemetry,
        HeapTelemetry,
        PSRAMTelemetry,
        WiFiTelemetry,
    };
    explicit PubSub(std::map<int, Topic> &topics, Network &n);
    PubSubClient client();
    void connect(MqttCredentials cred);
    bool isConnected();
    void listen();
    void publish(Topics topic, std::string data);
    void onRecv(Topics topic, const std::function<void(const std::string &payload)> &onRecv);
};


#endif //IOTSECURITYBASE_PSCLIENT_H
