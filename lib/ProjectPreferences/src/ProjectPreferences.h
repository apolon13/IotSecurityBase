#include "SD.h"
#include "Logger.h"

using namespace std;

#ifndef DISPLAY_PROJECTPREFERENCES_H
#define DISPLAY_PROJECTPREFERENCES_H

#define SYSTEM_STATE_LOCK "1"
#define SYSTEM_STATE_UNLOCK "0"

class ProjectPreferences {
protected:
    unordered_map<int, string> cache;

    string convertProperty(int key) {
        switch (key) {
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
            case IotControl:
                return "IotControl";
            case SystemPin:
                return "SystemPin";
            case SystemState:
                return "SystemState";
            case EditableDetectSensor:
                return "EditableDetectSensor";
            case LastError:
                return "LastError";
            case SecurityTimeout:
                return "SecurityTimeout";
            case ConnectionTimeout:
                return "ConnectionTimeout";
            case ConnectionAttemptsBeforeRestart:
                return "ConnectionAttemptsBeforeRestart";
            default:
                throw std::invalid_argument("Invalid property");
        }
    }

public:
    enum PreferencesKey {
        MqttIp,
        MqttPort,
        MqttUsername,
        MqttPassword,
        MqttEntityId,
        WifiPassword,
        WifiSsid,
        IotSensor,
        IotControl,
        SystemPin,
        SystemState,
        EditableDetectSensor,
        LastError,
        SecurityTimeout,
        ConnectionTimeout,
        ConnectionAttemptsBeforeRestart,
    };

    explicit ProjectPreferences();

    string get(PreferencesKey key, string defaultValue);

    void set(PreferencesKey key, const string &value);

    void lockSystem();

    void unlockSystem();

    bool systemIsLocked();

    string getSecurityTimeout();

    string getConnectionTimeout();

    string getConnectionAttemptsBeforeRestart();
};

#endif

