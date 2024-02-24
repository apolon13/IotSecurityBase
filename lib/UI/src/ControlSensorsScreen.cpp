#include "ControlSensorsScreen.h"
#include "map"
#include "UiMutex.h"

ControlSensorsScreen *selfControlSensorsScreen;

lv_obj_t *getChildByClass(lv_obj_t *parent, const lv_obj_class_t *className) {
    int i;
    for (i = 0; i < lv_obj_get_child_cnt(parent); i++) {
        lv_obj_t *child = lv_obj_get_child(parent, i);
        if (lv_obj_check_type(child, className)) {
            return child;
        }
    }
}

void ControlSensorsScreen::loadAllControls(lv_event_t *e) {
    std::vector<Sensor> sensors = iotRadioControl->getCurrentSensors();
    std::map<string, vector<Sensor>> groupedSensors;
    int panels[] = {
            UI_COMP_CONTROLITEM_CONTROLPANEL_GUARDPANEL,
            UI_COMP_CONTROLITEM_CONTROLPANEL_DISARMPANEL,
            UI_COMP_CONTROLITEM_CONTROLPANEL_MUTEPANEL,
            UI_COMP_CONTROLITEM_CONTROLPANEL_ALARMPANEL
    };
    lv_obj_clean(ui_Controls);
    for (Sensor sensor: sensors) {
        string group = iotRadioControl->extractGroup(sensor.signal);
        if (groupedSensors.find(group) != groupedSensors.end()) {
            auto current = groupedSensors.at(group);
            current.push_back(sensor);
            groupedSensors.erase(group);
            groupedSensors.insert(make_pair(group, current));
        } else {
            vector<Sensor> newSensors = {sensor};
            groupedSensors.insert(make_pair(group, newSensors));
        }
    }

    for (const auto &[key, sensorsInMap]: groupedSensors) {
        auto component = ui_ControlItem_create(ui_Controls);
        auto controlId = ui_comp_get_child(component, UI_COMP_CONTROLITEM_CONTROLPANEL_CONTROLID);
        lv_label_set_text(controlId, key.c_str());
        for (auto &&panelId: panels) {
            for (auto sensor: sensorsInMap) {
                lv_obj_t *panel = ui_comp_get_child(component, panelId);
                string action = lv_label_get_text(getChildByClass(panel, &lv_label_class));
                if (sensor.name == action) {
                    lv_obj_set_style_bg_color(panel, lv_color_hex(0x1CD850), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
}

void ControlSensorsScreen::addControl(lv_event_t *e) {
    auto countSensors = lv_obj_get_child_cnt(ui_Controls);
    if (countSensors < 2) {
        ui_ControlItem_create(ui_Controls);
    }
}

void ControlSensorsScreen::saveNewControl(void *) {
    if (UiMutex::take()) {
        string signalValue = currentSignalWithControl->value.c_str();
        lv_obj_t *container = lv_obj_get_parent(currentSignalWithControl->controlAction);
        lv_obj_t *groupLabel = getChildByClass(container, &lv_label_class);
        string action = lv_label_get_text(getChildByClass(currentSignalWithControl->controlAction, &lv_label_class));
        string group = lv_label_get_text(groupLabel);
        bool allowGroup = group.empty() || (group == iotRadioControl->extractGroup(signalValue));

        if (group.empty()) {
            auto groupId = iotRadioControl->extractGroup(signalValue);
            lv_label_set_text(groupLabel, groupId.c_str());
        }

        if (allowGroup) {
            lv_obj_set_style_bg_color(currentSignalWithControl->controlAction, lv_color_hex(0x1CD850),
                                      LV_PART_MAIN | LV_STATE_DEFAULT);
            Sensor newControl = {action, signalValue, true, 100};
            iotRadioControl->save(&newControl);
        }
        UiMutex::give();
    }
    currentSignalWithControl->clear();
    IoTRadio::stopScan();
    controlAction = nullptr;
    afterScan();
}

void ControlSensorsScreen::handleReceiveControl(const uint8_t *incomingData) {
    long now = millis();
    if (!scanIsRunning || (now - scanStartedAt) > 5000 || controlAction == nullptr) {
        IoTRadio::stopScan();
        afterScan();
        return;
    }
    currentSignalWithControl = (ReceivedSignalWithControl *) incomingData;
    string signalValue = currentSignalWithControl->value.c_str();
    if (!signalValue.empty() && !iotRadioControl->exist(signalValue)
        && !ioTRadioDetect->exist(signalValue)) {
        currentSignalWithControl->controlAction = controlAction;
        Task task = {
                [this](void *data) {
                    this->saveNewControl(data);
                },
                nullptr
        };
        queueTask->addTask(&task);
    }
}

void ControlSensorsScreen::addControlAction(lv_event_t *e) {
    beforeScan();
    controlAction = e->target;
    IoTRadio::addRecvHandler([](const uint8_t *mac, const uint8_t *incomingData, int len) {
        selfControlSensorsScreen->handleReceiveControl(incomingData);
    });
    IoTRadio::startScan();
}

void ControlSensorsScreen::deleteControl(lv_event_t *e) {
    lv_obj_t *panel = lv_obj_get_parent(e->current_target);
    lv_obj_t *component = lv_obj_get_parent(panel);
    lv_obj_t *groupIdLabel = ui_comp_get_child(component, UI_COMP_CONTROLITEM_CONTROLPANEL_CONTROLID);
    string groupId = lv_label_get_text(groupIdLabel);
    auto sensors = iotRadioControl->getCurrentSensors();
    for (auto sensor: sensors) {
        if (iotRadioControl->extractGroup(sensor.signal) == groupId) {
            iotRadioControl->forget(sensor.signal);
        }
    }
    lv_obj_del(component);
}

ControlSensorsScreen::ControlSensorsScreen(IoTRadioDetect *d, IotRadioControl *c, QueueTask *q) : iotRadioControl(c),
                                                                                                  ioTRadioDetect(d),
                                                                                                  queueTask(q) {
    selfControlSensorsScreen = this;
}
