#include "MqttConfigScreen.h"

using namespace std;

void MqttConfigScreen::loadMqttSettings(lv_event_t *e) {
    string mqttIp = store.get(Store::MqttIp, "");
    string mqttPort = store.get(Store::MqttPort, "");
    string mqttUsername = store.get(Store::MqttUsername, "");
    string mqttEntityId = store.get(Store::MqttEntityId, "");
    string mqttPassword = store.get(Store::MqttPassword, "");
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

void MqttConfigScreen::saveMqttSettings(lv_event_t *e) {
    string ip = lv_textarea_get_text(ui_mqttIp);
    string port = lv_textarea_get_text(ui_mqttPort);
    string username = lv_textarea_get_text(ui_mqttUsername);
    string entityId = lv_textarea_get_text(ui_mqttEntityId);
    string password = lv_textarea_get_text(ui_mqttPassword);
    store.set(Store::MqttIp, ip);
    store.set(Store::MqttPort, port);
    store.set(Store::MqttUsername, username);
    store.set(Store::MqttEntityId, entityId);
    store.set(Store::MqttPassword, password);
}
