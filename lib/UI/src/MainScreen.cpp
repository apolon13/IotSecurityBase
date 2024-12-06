#include "MainScreen.h"
#include "UiMutex.h"

using namespace std;

void MainScreen::showSuccessNetworkIcon() {
    lv_obj_add_flag(ui_connectionStatusWifiOff, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connectionStatusSimOff, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_connectionStatusWifiOn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_connectionStatusSimOn, LV_OBJ_FLAG_HIDDEN);
}

void MainScreen::showFailedNetworkIcon() {
    lv_obj_clear_flag(ui_connectionStatusWifiOff, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_connectionStatusSimOff, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connectionStatusWifiOn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_connectionStatusSimOn, LV_OBJ_FLAG_HIDDEN);
}

void MainScreen::successMqttConnection() {
    lv_obj_set_style_arc_color(ui_Spinner1, lv_color_hex(0x1CD850), LV_PART_INDICATOR | LV_STATE_DEFAULT);
}

void MainScreen::failedMqttConnection() {
    lv_obj_set_style_arc_color(ui_Spinner1, lv_color_hex(0xFF406A), LV_PART_INDICATOR | LV_STATE_DEFAULT);
}

void MainScreen::loadDataInMain(lv_event_t *e) {
    string mqttIp = store.get(Store::MqttIp, "ip empty");
    string mqttPort = store.get(Store::MqttPort, "port empty");
    string mqttUsername = store.get(Store::MqttUsername, "username empty");
    string mqttEntityId = store.get(Store::MqttEntityId, "id empty");
    string wifiSsid = store.get(Store::WifiSsid, "");
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
    lv_label_set_text(ui_sensorsQty, sensorsCount.c_str());

    if (store.networkModeIsWifi()) {
        lv_obj_clear_flag(ui_wifiHome, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_simHome, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(ui_wifiSsidLabel, wifiSsid.c_str());
    }

    if (store.networkModeIsSim()) {
        lv_obj_clear_flag(ui_simHome, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_wifiHome, LV_OBJ_FLAG_HIDDEN);
    }
}

void MainScreen::changeConnectionIcons(bool cloudIsConnected, bool networkIsConnected) {
    if (!cloudIsConnected && UiMutex::take()) {
        failedMqttConnection();
        UiMutex::give();
    } else if (cloudIsConnected && UiMutex::take()) {
        successMqttConnection();
        UiMutex::give();
    }

    if (!networkIsConnected && UiMutex::take()) {
        showFailedNetworkIcon();
        UiMutex::give();
    } else if (networkIsConnected && UiMutex::take()) {
        showSuccessNetworkIcon();
        UiMutex::give();
    }
}

void MainScreen::goTo(bool lockScreenState) {
    auto cb = []() {
        lv_disp_load_scr(ui_home);
    };

    if (lockScreenState) {
        mutexWrap(cb);
    } else {
        cb();
    }
}
