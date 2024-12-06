#include <map>
#include "SD.h"
#include "Logger.h"

#ifndef DISPLAY_PROJECTPREFERENCES_H
#define DISPLAY_PROJECTPREFERENCES_H

#define ON "1"
#define OFF "0"

struct Property {
    std::string value;
    size_t position;
    bool exist;
};

class FailedInitialization : public std::exception {
public:
    explicit FailedInitialization() = default;
};

class ProjectPreferences {
protected:
    static std::map<int, std::string> cache;

    std::string convertProperty(int key) {
        switch (key) {
            case MqttIp:
                return "MI";
            case MqttPort:
                return "MPRT";
            case MqttUsername:
                return "MU";
            case MqttPassword:
                return "MPASS";
            case MqttEntityId:
                return "MEI";
            case WifiPassword:
                return "WP";
            case WifiSsid:
                return "WS";
            case IotSensor:
                return "IS";
            case IotControl:
                return "IC";
            case SystemPin:
                return "SP";
            case SystemState:
                return "SS";
            case EditableDetectSensor:
                return "EDS";
            case LastError:
                return "LE";
            case SecurityTimeout:
                return "ST";
            case ConnectionTimeout:
                return "CT";
            case ConnectionAttemptsBeforeRestart:
                return "CABR";
            case APN:
                return "APN";
            case WifiNetworkMode:
                return "WNM";
            case SimNetworkMode:
                return "SNM";
            case Phone:
                return "P";
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
        APN,
        WifiNetworkMode,
        SimNetworkMode,
        Phone
    };

    std::string get(PreferencesKey key, std::string defaultValue);

    void set(PreferencesKey key, const std::string &value);

    void lockSystem();

    void unlockSystem();

    bool systemIsLocked();

    std::string getSecurityTimeout();

    std::string getConnectionTimeout();

    std::string getConnectionAttemptsBeforeRestart();

    bool networkModeIsWifi();

    bool networkModeIsSim();

    void setNetworkMode(PreferencesKey mode);
};

#endif

