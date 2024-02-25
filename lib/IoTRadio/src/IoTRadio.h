#include <sstream>
#include "ProjectPreferences.h"
#include <esp_now.h>
#include <esp_wifi_types.h>
#include <vector>

using namespace std;

#ifndef DISPLAY_IOTRADIO_H
#define DISPLAY_IOTRADIO_H

struct ReceivedSignal {
    String value{};

    void clear() {
        value = "";
    }
};

struct Sensor {
    string name;
    string signal;
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
    vector<Sensor> currentSensors;
    ProjectPreferences *projectPreferences;
    Logger *logger;

    vector<string> getSignals();

    virtual Props getPreferencesConfigKey() = 0;

    void loadCurrentSensors();

    bool hasInCurrentSensors(Sensor *s);

    string getSensorsConfig();

    vector<string> getConfigLines();

    string buildConfigStringBySensor(Sensor *s);

    vector<string> parseSensorString(string sensorString);

    Sensor buildSensorByConfigString(string configString);

    Sensor *getSensorByPredicate(std::function<bool(const Sensor &s)> predicate);

public:
    IoTRadio(ProjectPreferences *p, Logger *l);

    void forget(const string& signal);

    bool exist(const string& signal);

    void save(Sensor *sensor);

    Sensor *getSensorByName(const string &name);

    Sensor *getSensorBySignal(const string &signal);

    vector<Sensor> getCurrentSensors();

    static void addReceiver(const uint8_t macAddress[]);

    static bool receiverExist(const uint8_t macAddress[]);

    static void addRecvHandler(esp_now_recv_cb_t recvCb);

    static void addSendHandler(esp_now_send_cb_t sendCb);

    static void startScan();

    static void stopScan();

    static void sendMessageToPeer(PeerMessage msg);
};

#endif
