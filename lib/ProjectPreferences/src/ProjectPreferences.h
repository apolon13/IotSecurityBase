#include "SD.h"
#include "Logger.h"

#ifndef DISPLAY_PROJECTPREFERENCES_H
#define DISPLAY_PROJECTPREFERENCES_H

#define SYSTEM_STATE_LOCK "1"
#define SYSTEM_STATE_UNLOCK "0"

struct Property {
    std::string value;
    size_t position;
    bool exist;
};

class ProjectPreferences {
protected:
    static std::unordered_map<int, std::string> cache;

    std::string convertProperty(int key) {
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

    virtual Property readPreferencesProperty(const std::string &name, std::string defaultValue) = 0;

    virtual void writePreferencesProperty(const std::string &name, const std::string &value) = 0;

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

    std::string get(PreferencesKey key, std::string defaultValue);

    void set(PreferencesKey key, const std::string &value);

    void lockSystem();

    void unlockSystem();

    bool systemIsLocked();

    std::string getSecurityTimeout();

    std::string getConnectionTimeout();

    std::string getConnectionAttemptsBeforeRestart();
};

#endif

