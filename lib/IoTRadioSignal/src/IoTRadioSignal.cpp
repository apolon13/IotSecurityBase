#include <sstream>
#include "IoTRadioSignal.h"
#include <esp_now.h>
#include <esp_wifi_types.h>

using namespace std;

#define SENSORS_DELIMITER '_'
#define ATTRIBUTE_DELIMITER ','

esp_now_peer_info_t Peer;

typedef struct {
    bool activateScan;
    bool alarm;
} PeerMessage;

string getSensorsConfig(ProjectPreferences *p) {
    return p->get(IotSensor, "");
}

string buildConfigStringBySensor(const Sensor *s) {
    return s->name + ATTRIBUTE_DELIMITER +
           s->signal + ATTRIBUTE_DELIMITER +
           to_string(s->isActive) + ATTRIBUTE_DELIMITER +
           to_string(s->sensivity);
}

vector<string> getConfigLines(ProjectPreferences *p) {
    stringstream stream(getSensorsConfig(p));
    vector<string> lines;
    string segment;
    while (getline(stream, segment, SENSORS_DELIMITER)) {
        lines.push_back(segment);
    }
    return lines;
}

vector<string> parseSensorString(string sensorString) {
    vector<string> result;
    while (true) {
        int position = sensorString.find(ATTRIBUTE_DELIMITER);
        string val = sensorString.substr(0, position);
        result.push_back(val);
        sensorString.erase(0, position + 1);
        if (position <= 0) {
            break;
        }
    }
    return result;
}

void addToConfig(string *config, string value) {
    config->append(!config->empty() ? SENSORS_DELIMITER + value : value);
}

Sensor buildSensorByConfigString(string configString) {
    int key = 0;
    Sensor sensor;
    vector<string> parsed = parseSensorString(configString);
    for_each(parsed.begin(), parsed.end(), [&key, &sensor](const string &n) {
        string active("1");
        switch (key) {
            case 0:
                sensor.name = n;
                break;
            case 1:
                sensor.signal = n;
                break;
            case 2:
                sensor.isActive = (n == active);
                break;
            case 3:
                sensor.sensivity = stoi(n);
                break;
        }
        key++;
    });
    return sensor;
}

IoTRadioSignal::IoTRadioSignal(ProjectPreferences *p, int rp, Logger *l) : projectPreferences(p), logger(l) {
    loadCurrentSensors();
}

void IoTRadioSignal::startScan() {
    PeerMessage msg = {true, false};
    esp_now_send(Peer.peer_addr, (uint8_t *) &msg, sizeof(msg));
}

void IoTRadioSignal::stopScan() {
    PeerMessage msg = {false, false};
    esp_now_send(Peer.peer_addr, (uint8_t *) &msg, sizeof(msg));
}

void IoTRadioSignal::forget(string signal) {
    vector<string> signals = getSignals();
    auto iteratorSignals = find(signals.begin(), signals.end(), signal);
    if (iteratorSignals != signals.end()) {
        string newConfig;
        for (const Sensor &sensor: currentSensors) {
            if (sensor.signal != signal) {
                addToConfig(&newConfig, buildConfigStringBySensor(&sensor));
            }
        }
        auto iterator = std::find_if(currentSensors.begin(), currentSensors.end(), [signal](const Sensor &s) {
            return s.signal == signal;
        });
        if (iterator != currentSensors.end()) {
            currentSensors.erase(iterator);
        }
        projectPreferences->set(IotSensor, newConfig);
    }
}

vector<string> IoTRadioSignal::getSignals() {
    vector<string> signals;
    for_each(currentSensors.begin(), currentSensors.end(), [&signals](Sensor &s) {
        signals.push_back(s.signal);
    });
    return signals;
}


void IoTRadioSignal::save(Sensor *sensor) {
    vector<string> signals = getSignals();
    string newConfig;
    string newSensorConfigString = buildConfigStringBySensor(sensor);
    if (find(signals.begin(), signals.end(), sensor->signal) == signals.end()) {
        string currentConfig = getSensorsConfig(projectPreferences);
        addToConfig(&newConfig, currentConfig);
        addToConfig(&newConfig, newSensorConfigString);
    } else {
        for (string line: getConfigLines(projectPreferences)) {
            Sensor inConfig = buildSensorByConfigString(line);
            addToConfig(&newConfig,
                        (inConfig.isEqual(sensor) ? newSensorConfigString : buildConfigStringBySensor(&inConfig)));
        }
    }
    if (!hasInCurrentSensors(sensor)) {
        currentSensors.push_back(*sensor);
    }
    projectPreferences->set(IotSensor, newConfig);
}


void IoTRadioSignal::loadCurrentSensors() {
    currentSensors = {};
    auto config = getConfigLines(projectPreferences);
    for (string line: config) {
        logger->debug("Sensor - " + line);
        Sensor newSensor = buildSensorByConfigString(line);
        currentSensors.push_back(newSensor);
    }
}

vector<Sensor> IoTRadioSignal::getCurrentSensors() {
    return currentSensors;
}

Sensor *IoTRadioSignal::getSensorByName(string name) {
    Sensor s;
    for (Sensor &sensor: currentSensors) {
        if (sensor.name == name) {
            return &sensor;
        }
    }
    return nullptr;
}

void IoTRadioSignal::addRecvHandler(esp_now_recv_cb_t recvCb) {
    esp_now_register_recv_cb(recvCb);
}

void IoTRadioSignal::addReceiver(const uint8_t macAddress[]) {
    if (esp_now_init() != ESP_OK) {
        logger->debug("Error initializing ESP-NOW");
        return;
    }
    logger->debug("Success init ESP-NOW");
    memcpy(Peer.peer_addr, macAddress, 6);
    Peer.encrypt = false;
    Peer.channel = 0;
    if (esp_now_add_peer(&Peer) != ESP_OK) {
        logger->debug("Failed to add receiver peer");
        return;
    }
    logger->debug("Success add receiver peer");
}

void IoTRadioSignal::addSendHandler(esp_now_send_cb_t sendCb) {
    esp_now_register_send_cb(sendCb);
}

bool IoTRadioSignal::exist(string signal) {
    auto signals = getSignals();
    return find(signals.begin(), signals.end(), signal) != signals.end();
}

bool IoTRadioSignal::hasInCurrentSensors(Sensor *sensor) {
    auto iterator = std::find_if(currentSensors.begin(), currentSensors.end(), [sensor](const Sensor &s) {
        return s.signal == sensor->signal;
    });
    return iterator != currentSensors.end();
}

