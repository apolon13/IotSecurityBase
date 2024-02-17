#include "SD.h"
#include "Logger.h"

using namespace std;

#ifndef DISPLAY_PROJECTPREFERENCES_H
#define DISPLAY_PROJECTPREFERENCES_H

enum Props {
    MqttIp,
    MqttPort,
    MqttUsername,
    MqttPassword,
    MqttEntityId,
    WifiPassword,
    WifiSsid,
    IotSensor,
    SystemPin,
    SystemState
};

class ProjectPreferences {
private:
    Logger *logger;
    string convertProperty(Props property) {
        switch (property) {
            case MqttIp:
                return "MqttIp";
            case MqttPort:
                return "MqttPort";
            case MqttUsername:
                return "MqttUsername";
            case MqttPassword:
                return "MqttPassword";
            case MqttEntityId:
                return "MqttEntityId";
            case WifiPassword:
                return "WifiPassword";
            case WifiSsid:
                return "WifiSsid";
            case IotSensor:
                return "IotSensor";
            case SystemPin:
                return "SystemPin";
            case SystemState:
                return "SystemState";
        }
        throw std::invalid_argument("Invalid property");
    }
public:
    explicit ProjectPreferences(Logger *l);
    string get(Props property, string defaultValue);
    void set(Props property, string value);
};
#endif //DISPLAY_PROJECTPREFERENCES_H

