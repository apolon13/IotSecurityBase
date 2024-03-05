#include "MainScreen.h"
#include "UiMutex.h"

void MainScreen::showSuccessNetworkIcon() {
    lv_obj_add_flag(ui_connectionStatusOff, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_connectionStatusOn, LV_OBJ_FLAG_HIDDEN);
}

void MainScreen::showFailedNetworkIcon() {
    lv_obj_clear_flag(ui_connectionStatusOff, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connectionStatusOn, LV_OBJ_FLAG_HIDDEN);
}

void MainScreen::successMqttConnection() {
    lv_obj_set_style_arc_color(ui_Spinner1, lv_color_hex(0x1CD850), LV_PART_INDICATOR | LV_STATE_DEFAULT);
}

void MainScreen::failedMqttConnection() {
    lv_obj_set_style_arc_color(ui_Spinner1, lv_color_hex(0xFF406A), LV_PART_INDICATOR | LV_STATE_DEFAULT);
}

void MainScreen::loadDataInMain(lv_event_t *e) {
    string mqttIp = projectPreferences.get(ProjectPreferences::MqttIp, "ip empty");
    string mqttPort = projectPreferences.get(ProjectPreferences::MqttPort, "port empty");
    string mqttUsername = projectPreferences.get(ProjectPreferences::MqttUsername, "username empty");
    string mqttEntityId = projectPreferences.get(ProjectPreferences::MqttEntityId, "id empty");
    string wifiSsid = projectPreferences.get(ProjectPreferences::WifiSsid, "");
    int countAll = 0;
    int countActive = 0;
    for (auto sensor: ioTRadioDetect.getCurrentSensors()) {
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

void MainScreen::handleConnections() {
    if (!dispatcher.cloudIsConnected() && UiMutex::take()) {
        failedMqttConnection();
        UiMutex::give();
    } else if (dispatcher.cloudIsConnected() && UiMutex::take()) {
        successMqttConnection();
        UiMutex::give();
    }

    if (!dispatcher.networkIsConnected() && UiMutex::take()) {
        showFailedNetworkIcon();
        UiMutex::give();
    } else if (dispatcher.networkIsConnected() && UiMutex::take()) {
        showSuccessNetworkIcon();
        UiMutex::give();
    }
}

void MainScreen::goTo(bool useMutex) {
    auto cb = []() {
        lv_disp_load_scr(ui_home);
    };

    if (useMutex) {
        mutexWrap(cb);
    } else {
        cb();
    }
}
