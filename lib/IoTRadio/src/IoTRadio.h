#include <sstream>
#include "ProjectPreferences.h"
#include <esp_now.h>
#include <esp_wifi_types.h>
#include <vector>

#ifndef DISPLAY_IOTRADIO_H
#define DISPLAY_IOTRADIO_H

struct ReceivedSignal {
    String value{};

    void clear() {
        value = "";
    }
};

struct Sensor {
    std::string name;
    std::string signal;
    bool isActive;
    int sensitivity;

    bool isEqual(Sensor *s) const {
        return s->signal == signal;
    }
};

typedef struct {
    bool activateScan;
    bool alarm;
} PeerMessage;

class IoTRadio {
protected:
    std::vector<Sensor> currentSensors;
    ProjectPreferences &projectPreferences;
    static esp_now_recv_cb_t currentRcvCallback;
    static PeerMessage lastMessage;

    std::vector<std::string> getSignals();

    virtual ProjectPreferences::PreferencesKey getPreferencesConfigKey() = 0;

    void loadCurrentSensors();

    bool hasInCurrentSensors(Sensor *s);

    std::string getSensorsConfig();

    std::vector<std::string> getConfigLines();

    std::string buildConfigStringBySensor(Sensor *s);

    std::vector<std::string> parseSensorString(std::string sensorString);

    Sensor buildSensorByConfigString(std::string configString);

    Sensor *getSensorByPredicate(std::function<bool(const Sensor &s)> predicate);

public:

    IoTRadio(ProjectPreferences &p, Logger &l);

    void forget(const std::string &signal);

    bool exist(const std::string &signal);

    void save(Sensor *sensor);

    Sensor *getSensorByName(const std::string &name);

    Sensor *getSensorBySignal(const std::string &signal);

    std::vector<Sensor> getCurrentSensors();

    static void addReceiver(const uint8_t macAddress[]);

    static void addRecvHandler(esp_now_recv_cb_t recvCb);

    static void startScan();

    static void stopScan();

    static void sendMessageToPeer(PeerMessage msg);
};

#endif
