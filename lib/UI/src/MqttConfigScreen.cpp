#include "MqttConfigScreen.h"

void MqttConfigScreen::loadMqttSettings(lv_event_t *e) {
    string mqttIp = projectPreferences.get(ProjectPreferences::MqttIp, "");
    string mqttPort = projectPreferences.get(ProjectPreferences::MqttPort, "");
    string mqttUsername = projectPreferences.get(ProjectPreferences::MqttUsername, "");
    string mqttEntityId = projectPreferences.get(ProjectPreferences::MqttEntityId, "");
    string mqttPassword = projectPreferences.get(ProjectPreferences::MqttPassword, "");
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
    projectPreferences.set(ProjectPreferences::MqttIp, ip);
    projectPreferences.set(ProjectPreferences::MqttPort, port);
    projectPreferences.set(ProjectPreferences::MqttUsername, username);
    projectPreferences.set(ProjectPreferences::MqttEntityId, entityId);
    projectPreferences.set(ProjectPreferences::MqttPassword, password);
}
