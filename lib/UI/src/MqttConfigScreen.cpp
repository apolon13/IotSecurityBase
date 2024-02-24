#include "MqttConfigScreen.h"

void MqttConfigScreen::loadMqttSettings(lv_event_t *e) {
    string mqttIp = projectPreferences->get(MqttIp, "");
    string mqttPort = projectPreferences->get(MqttPort, "");
    string mqttUsername = projectPreferences->get(MqttUsername, "");
    string mqttEntityId = projectPreferences->get(MqttEntityId, "");
    string mqttPassword = projectPreferences->get(MqttPassword, "");
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
    projectPreferences->set(MqttIp, ip);
    projectPreferences->set(MqttPort, port);
    projectPreferences->set(MqttUsername, username);
    projectPreferences->set(MqttEntityId, entityId);
    projectPreferences->set(MqttPassword, password);
}
