#include "IoTRadio.h"
#include <utility>
#include "WiFi.h"
#include "esp_wifi.h"

#define SENSORS_DELIMITER '_'
#define ATTRIBUTE_DELIMITER ','

esp_now_peer_info_t Peer;
TaskScheduler *taskScheduler;

using namespace std;

esp_now_recv_cb_t IoTRadio::currentRcvCallback = nullptr;
PeerMessage IoTRadio::lastMessage = {true};

void ping(void *) {
    while (true) {
        IoTRadio::sendMessageToPeer(IoTRadio::lastMessage);
        vTaskDelay(1000);
    }
}

string IoTRadio::getSensorsConfig() {
    return projectPreferences.get(getPreferencesConfigKey(), "");
}

string IoTRadio::buildConfigStringBySensor(Sensor *s) {
    return s->name + ATTRIBUTE_DELIMITER +
           s->signal + ATTRIBUTE_DELIMITER +
           to_string(s->isActive) + ATTRIBUTE_DELIMITER +
           to_string(s->sensitivity);
}

vector<string> IoTRadio::getConfigLines() {
    stringstream stream(getSensorsConfig());
    vector<string> lines;
    string segment;
    while (getline(stream, segment, SENSORS_DELIMITER)) {
        lines.push_back(segment);
    }
    return lines;
}

vector<string> IoTRadio::parseSensorString(string sensorString) {
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

void addToConfig(string *config, const string &value) {
    config->append(!config->empty() ? SENSORS_DELIMITER + value : value);
}

Sensor IoTRadio::buildSensorByConfigString(string configString) {
    int key = 0;
    Sensor sensor;
    vector<string> parsed = parseSensorString(std::move(configString));
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
                sensor.sensitivity = stoi(n);
                break;
        }
        key++;
    });
    return sensor;
}

IoTRadio::IoTRadio(ProjectPreferences &p, TaskScheduler &sc) : projectPreferences(p) {
    taskScheduler = &sc;
}

void IoTRadio::startScan() {
    PeerMessage msg = {true, false};
    esp_now_send(Peer.peer_addr, (uint8_t *) &msg, sizeof(msg));
}

void IoTRadio::stopScan() {
    PeerMessage msg = {false, false};
    esp_now_send(Peer.peer_addr, (uint8_t *) &msg, sizeof(msg));
}

void IoTRadio::sendMessageToPeer(PeerMessage msg) {
    lastMessage = msg;
    esp_now_send(Peer.peer_addr, (uint8_t *) &msg, sizeof(msg));
}

void IoTRadio::forget(const string &signal) {
    vector<string> signals = getSignals();
    auto iteratorSignals = find(signals.begin(), signals.end(), signal);
    if (iteratorSignals != signals.end()) {
        string newConfig;
        for (Sensor &sensor: currentSensors) {
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
        projectPreferences.set(getPreferencesConfigKey(), newConfig);
    }
}

vector<string> IoTRadio::getSignals() {
    vector<string> signals;
    for_each(currentSensors.begin(), currentSensors.end(), [&signals](Sensor &s) {
        signals.push_back(s.signal);
    });
    return signals;
}


void IoTRadio::save(Sensor *sensor) {
    vector<string> signals = getSignals();
    string newConfig;
    string newSensorConfigString = buildConfigStringBySensor(sensor);
    if (find(signals.begin(), signals.end(), sensor->signal) == signals.end()) {
        string currentConfig = getSensorsConfig();
        addToConfig(&newConfig, currentConfig);
        addToConfig(&newConfig, newSensorConfigString);
    } else {
        for (const string &line: getConfigLines()) {
            Sensor inConfig = buildSensorByConfigString(line);
            addToConfig(&newConfig,
                        (inConfig.isEqual(sensor) ? newSensorConfigString : buildConfigStringBySensor(&inConfig)));
        }
    }

    if (!hasInCurrentSensors(sensor)) {
        currentSensors.push_back(*sensor);
    } else {
        std::replace_if(currentSensors.begin(), currentSensors.end(), [sensor](const Sensor &s) {
            return s.signal == sensor->signal;
        }, *sensor);
    }
    projectPreferences.set(getPreferencesConfigKey(), newConfig);
}


void IoTRadio::loadCurrentSensors() {
    currentSensors = {};
    auto config = getConfigLines();
    for (const string &line: config) {
        Sensor newSensor = buildSensorByConfigString(line);
        currentSensors.push_back(newSensor);
    }
}

vector<Sensor> IoTRadio::getCurrentSensors() {
    return currentSensors;
}

void IoTRadio::addRecvHandler(esp_now_recv_cb_t recvCb) {
    currentRcvCallback = recvCb;
    auto resp = esp_now_register_recv_cb(recvCb);
    if (resp != ESP_OK) {
        return;
    }
    esp_now_register_send_cb([](const uint8_t *mac_addr, esp_now_send_status_t status) {
        if (status == ESP_NOW_SEND_FAIL) {
            /** reconnect attempt **/
            esp_now_del_peer(Peer.peer_addr);
            IoTRadio::addReceiver(Peer.peer_addr);
            IoTRadio::addRecvHandler(currentRcvCallback);
        }
    });
}

void IoTRadio::addReceiver(const uint8_t macAddress[]) {
    esp_now_deinit();
    if (esp_now_init() != ESP_OK) {
        return;
    }

    if (esp_now_is_peer_exist(macAddress)) {
        return;
    }

    memcpy(Peer.peer_addr, macAddress, 6);
    Peer.encrypt = false;
    if (esp_now_add_peer(&Peer) != ESP_OK) {
        return;
    }

    taskScheduler->addTask({
           "ping",
           ping,
           TaskPriority::Low,
           3000

    });
    taskScheduler->schedule();
}

bool IoTRadio::exist(const string &signal) {
    auto signals = getSignals();
    return find(signals.begin(), signals.end(), signal) != signals.end();
}

bool IoTRadio::hasInCurrentSensors(Sensor *sensor) {
    auto iterator = std::find_if(currentSensors.begin(), currentSensors.end(), [sensor](const Sensor &s) {
        return s.signal == sensor->signal;
    });
    return getSensorByPredicate([sensor](const Sensor &s) { return s.signal == sensor->signal; }) != nullptr;
}

Sensor *IoTRadio::getSensorByName(const string &name) {
    return getSensorByPredicate([name](const Sensor &s) {
        return s.name == name;
    });
}

Sensor *IoTRadio::getSensorBySignal(const string &signal) {
    return getSensorByPredicate([signal](const Sensor &s) {
        return s.signal == signal;
    });
}

Sensor *IoTRadio::getSensorByPredicate(std::function<bool(const Sensor &s)> predicate) {
    auto iterator = std::find_if(currentSensors.begin(), currentSensors.end(), std::move(predicate));
    return iterator == currentSensors.end() ? nullptr : iterator.operator->();
}
