#include "ProjectPreferences.h"
#include "Logger.h"
#include <vector>
#include <esp_now.h>

using namespace std;

#ifndef DISPLAY_SENSOR_H
#define DISPLAY_SENSOR_H

struct Sensor {
    string name;
    string signal;
    bool isActive;
    int sensivity;

    bool isEqual(Sensor *s) const {
        return s->signal == signal;
    }
};

class IoTRadioSignal {
protected:
    vector<Sensor> currentSensors;
    ProjectPreferences *projectPreferences;
    Logger *logger;
    void loadCurrentSensors();
    vector<string> getSignals();
public:
    IoTRadioSignal(ProjectPreferences *p, int receivePin, Logger *l);
    void startScan();
    void stopScan();
    void forget(string signal);
    bool exist(string signal);
    void save(Sensor *sensor);
    Sensor* getSensorByName(string name);
    vector<Sensor> getCurrentSensors();

    void addReceiver(const uint8_t macAddress[]);
    void addRecvHandler(esp_now_recv_cb_t recvCb);
    void addSendHandler(esp_now_send_cb_t sendCb);
};
#endif //DISPLAY_SENSOR_H
