#include "UiEventHandler.h"

string defaultMqttIp = "ip empty";
string defaultMqttPort = "port empty";
string defaultMqttUsername = "username empty";
string defaultMqttEntityId = "id empty";
string emptyValue;

IoTRadioSignal *globalIot;

void showSuccessNetworkIcon() {
    lv_obj_add_flag(ui_connectionStatusOff, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_connectionStatusOn, LV_OBJ_FLAG_HIDDEN);
}

void showFailedNetworkIcon() {
    lv_obj_clear_flag(ui_connectionStatusOff, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connectionStatusOn, LV_OBJ_FLAG_HIDDEN);
}

void successMqttConnection() {
    lv_obj_set_style_arc_color(ui_Spinner1, lv_color_hex(0x1CD850), LV_PART_INDICATOR | LV_STATE_DEFAULT);
}

void failedMqttConnection() {
    lv_obj_set_style_arc_color(ui_Spinner1, lv_color_hex(0xFF406A), LV_PART_INDICATOR | LV_STATE_DEFAULT);
}

bool scanIsRunning;
struct ReceivedSignal{
    String value;
    void clear() {
        value = "";
    }
};

ReceivedSignal receivedSignal;
void addSensor(void *) {
    while (true) {
        string signalValue = receivedSignal.value.c_str();
        if (!signalValue.empty() && !globalIot->exist(signalValue) && UiMutex::take()) {
            lv_obj_t *component = ui_SensorItem_create(ui_Sensors);
            lv_obj_t *label = ui_comp_get_child(component, UI_COMP_SENSORITEM_SENSORITEMPANEL_SENSORID);
            lv_label_set_text(label, signalValue.c_str());
            lv_obj_clear_flag(ui_comp_get_child(component, UI_COMP_SENSORITEM_SENSORITEMPANEL_LABEL15),LV_OBJ_FLAG_HIDDEN);
            Sensor newSensor = {signalValue, signalValue, true, 50};
            globalIot->save(&newSensor);
            UiMutex::give();
        }
        receivedSignal.clear();
        vTaskDelay(10);
    }
}

void receiveSensor(const uint8_t *mac, const uint8_t *incomingData, int len) {
    if (!scanIsRunning) {
        return;
    }
    memcpy(&receivedSignal, incomingData, sizeof(receivedSignal));
}

void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void UiEventHandler::loadGeneralSettings(lv_event_t *e) {
    string currentPin = preferences->get(SystemPin, emptyValue);
    lv_textarea_set_text(ui_settingsPincodeInput, currentPin.c_str());
}

void UiEventHandler::loadDataInMain(lv_event_t *e) {
    string mqttIp = preferences->get(MqttIp, defaultMqttIp);
    string mqttPort = preferences->get(MqttPort, defaultMqttPort);
    string mqttUsername = preferences->get(MqttUsername, defaultMqttUsername);
    string mqttEntityId = preferences->get(MqttEntityId, defaultMqttEntityId);
    string wifiSsid = preferences->get(WifiSsid, emptyValue);
    int countAll = 0;
    int countActive = 0;
    for (auto sensor: ioTRadioSignal->getCurrentSensors()) {
        countAll++;
        if (sensor.isActive) {
            countActive++;
        }
    }
    string sensorsCount;
    sensorsCount.append("all - " + to_string(countAll));
    sensorsCount.append("\n");
    sensorsCount.append("active - " + to_string(countActive));
    string addr = mqttIp + ":" + mqttPort;
    string credentials = mqttUsername + ":" + mqttEntityId;
    lv_label_set_text(ui_mqttAddrLabel, addr.c_str());
    lv_label_set_text(ui_mqttCredentialsLabel, credentials.c_str());
    lv_label_set_text(ui_wifiSsidLabel, wifiSsid.c_str());
    lv_label_set_text(ui_sensorsQty, sensorsCount.c_str());
}

void UiEventHandler::loadAllSensors(lv_event_t *e) {
    std::vector<Sensor> sensors = ioTRadioSignal->getCurrentSensors();
    lv_obj_clean(ui_Sensors);
    for (Sensor sensor: sensors) {
        lv_obj_t *component = ui_SensorItem_create(ui_Sensors);
        lv_obj_t *label = ui_comp_get_child(component, UI_COMP_SENSORITEM_SENSORITEMPANEL_SENSORID);
        lv_label_set_text(label, sensor.name.c_str());
    }
}

Sensor *inEdit;

void UiEventHandler::loadSensor(lv_event_t *e) {
    if (inEdit != nullptr) {
        lv_textarea_set_text(ui_sensorName, inEdit->name.c_str());
        if (inEdit->isActive) {
            lv_obj_add_state(ui_sensorIsActive, LV_STATE_CHECKED);
        }
        lv_slider_set_value(ui_sensorSensitivity, inEdit->sensivity, LV_ANIM_OFF);
    }
}

void UiEventHandler::loadFiWiSettings(lv_event_t *e) {
    string wifiSsid = preferences->get(WifiSsid, emptyValue);
    string wifiPass = preferences->get(WifiPassword, emptyValue);
    if (!wifiSsid.empty()) {
        lv_textarea_set_text(ui_wifiSsid, wifiSsid.c_str());
    }
    if (!wifiPass.empty()) {
        lv_textarea_set_text(ui_wifiPassword, wifiPass.c_str());
    }
}

void UiEventHandler::loadMqttSettings(lv_event_t *e) {
    string mqttIp = preferences->get(MqttIp, emptyValue);
    string mqttPort = preferences->get(MqttPort, emptyValue);
    string mqttUsername = preferences->get(MqttUsername, emptyValue);
    string mqttEntityId = preferences->get(MqttEntityId, emptyValue);
    string mqttPassword = preferences->get(MqttPassword, emptyValue);
    if (!mqttIp.empty()) {
        lv_textarea_set_text(ui_mqttIp, mqttIp.c_str());
    }
    if (!mqttPort.empty()) {
        lv_textarea_set_text(ui_mqttPort, mqttPort.c_str());
    }
    if (!mqttUsername.empty()) {
        lv_textarea_set_text(ui_mqttUsername, mqttUsername.c_str());
    }
    if (!mqttEntityId.empty()) {
        lv_textarea_set_text(ui_mqttEntityId, mqttEntityId.c_str());
    }
    if (!mqttPassword.empty()) {
        lv_textarea_set_text(ui_mqttPassword, mqttPassword.c_str());
    }
}

void UiEventHandler::saveMqttSettings(lv_event_t *e) {
    string ip = lv_textarea_get_text(ui_mqttIp);
    string port = lv_textarea_get_text(ui_mqttPort);
    string username = lv_textarea_get_text(ui_mqttUsername);
    string entityId = lv_textarea_get_text(ui_mqttEntityId);
    string password = lv_textarea_get_text(ui_mqttPassword);
    preferences->set(MqttIp, ip);
    preferences->set(MqttPort, port);
    preferences->set(MqttUsername, username);
    preferences->set(MqttEntityId, entityId);
    preferences->set(MqttPassword, password);
}

void UiEventHandler::saveWifiSettings(lv_event_t *e) {
    string ssid = lv_textarea_get_text(ui_wifiSsid);
    string pass = lv_textarea_get_text(ui_wifiPassword);
    preferences->set(WifiSsid, ssid);
    preferences->set(WifiPassword, pass);
}

void UiEventHandler::deleteSensor(lv_event_t *e) {
    lv_obj_t *panel = lv_obj_get_parent(e->current_target);
    lv_obj_t *component = lv_obj_get_parent(panel);
    lv_obj_t *label = ui_comp_get_child(component, UI_COMP_SENSORITEM_SENSORITEMPANEL_SENSORID);
    string text = lv_label_get_text(label);
    ioTRadioSignal->forget(ioTRadioSignal->getSensorByName(text)->signal);
    lv_obj_del(component);
}

void UiEventHandler::startScan(lv_event_t *e) {
    taskScheduler->scheduleTask({AddSensorTask, addSensor, TaskPriority::High, 5000});
    scanIsRunning = true;
    ioTRadioSignal->addRecvHandler(receiveSensor);
    ioTRadioSignal->addSendHandler(onSent);
    ioTRadioSignal->startScan();
}

void UiEventHandler::stopScan(lv_event_t *e) {
    scanIsRunning = false;
    taskScheduler->deleteTask(AddSensorTask);
    ioTRadioSignal->stopScan();
    dispatcher->reconnectToCloud();
}

void UiEventHandler::goToEditSensor(lv_event_t *e) {
    lv_obj_t *panel = lv_obj_get_parent(e->current_target);
    lv_obj_t *component = lv_obj_get_parent(panel);
    lv_obj_t *label = ui_comp_get_child(component, UI_COMP_SENSORITEM_SENSORITEMPANEL_SENSORID);
    string text = lv_label_get_text(label);
    inEdit = ioTRadioSignal->getSensorByName(text);
    lv_disp_load_scr(ui_edittSensorScreen);
}

void UiEventHandler::saveSensorSettings(lv_event_t *e) {
    if (inEdit != nullptr) {
        Sensor s = {
                lv_textarea_get_text(ui_sensorName),
                inEdit->signal,
                static_cast<bool>(lv_obj_get_state(ui_sensorIsActive) & LV_STATE_CHECKED),
                lv_slider_get_value(ui_sensorSensitivity)
        };
        ioTRadioSignal->save(&s);
        inEdit = nullptr;
    }
}

void UiEventHandler::lockSystem(lv_event_t *e) {
    string lockState = "1";
    preferences->set(SystemState, lockState);
}

void UiEventHandler::unlockSystem(lv_event_t *e) {
    string currentPin = preferences->get(SystemPin, emptyValue);
    string userPin = lv_textarea_get_text(ui_pincode);
    string unlockState = "0";
    bool isOk = true;

    if (currentPin.empty() && !userPin.empty()) {
        preferences->set(SystemPin, userPin);
        preferences->set(SystemState, unlockState);
        isOk = true;
    } else if (currentPin == userPin) {
        preferences->set(SystemState, unlockState);
        isOk = true;
    }

    if (isOk) {
        lv_disp_load_scr(ui_home);
    }

    lv_textarea_set_text(ui_pincode, emptyValue.c_str());
}

void UiEventHandler::saveGeneralSettings(lv_event_t *e) {
    string newPin = lv_textarea_get_text(ui_settingsPincodeInput);
    preferences->set(SystemPin, newPin);
}

void UiEventHandler::enableReceive(lv_event_t *e) {

}

void UiEventHandler::handleConnections() {
    if (!dispatcher->cloudIsConnected() && UiMutex::take()) {
        failedMqttConnection();
        UiMutex::give();
    } else if (dispatcher->cloudIsConnected() && UiMutex::take()) {
        successMqttConnection();
        UiMutex::give();
    }

    if (!dispatcher->networkIsConnected() && UiMutex::take()) {
        showFailedNetworkIcon();
        UiMutex::give();
    } else if (dispatcher->networkIsConnected() && UiMutex::take()) {
        showSuccessNetworkIcon();
        UiMutex::give();
    }
}

UiEventHandler::UiEventHandler(ProjectPreferences *p, IoTRadioSignal *i, TaskScheduler *t, Dispatcher *d)
        : preferences(p),
          ioTRadioSignal(i),
          taskScheduler(t),
          dispatcher(d) {
    globalIot = i;
}
