#include "DetectSensorsScreen.h"
#include "UiMutex.h"

DetectSensorsScreen *selfDetectSensorsScreen;

void DetectSensorsScreen::saveNewSensor(void *) {
    if (UiMutex::take()) {
        string signalValue = currentSignal->value.c_str();
        lv_obj_t *component = ui_SensorItem_create(ui_Sensors);
        lv_obj_t *label = ui_comp_get_child(component, UI_COMP_SENSORITEM_SENSORITEMPANEL_SENSORID);
        lv_label_set_text(label, signalValue.c_str());
        lv_obj_clear_flag(ui_comp_get_child(component, UI_COMP_SENSORITEM_SENSORITEMPANEL_LABEL15), LV_OBJ_FLAG_HIDDEN);
        Sensor newSensor = {signalValue, signalValue, true, 50};
        ioTRadioDetect.save(&newSensor);
        UiMutex::give();
    }
    currentSignal->clear();
}

void DetectSensorsScreen::loadAllSensors(lv_event_t *e) {
    std::vector<Sensor> sensors = ioTRadioDetect.getCurrentSensors();
    lv_obj_clean(ui_Sensors);
    for (Sensor sensor: sensors) {
        lv_obj_t *component = ui_SensorItem_create(ui_Sensors);
        lv_obj_t *label = ui_comp_get_child(component, UI_COMP_SENSORITEM_SENSORITEMPANEL_SENSORID);
        lv_label_set_text(label, sensor.name.c_str());
    }
}

void DetectSensorsScreen::startScan(lv_event_t *e) {
    beforeScan();
    IoTRadio::addRecvHandler([](const uint8_t *mac, const uint8_t *incomingData, int len) {
        selfDetectSensorsScreen->handleReceiveSensor(incomingData);
    });
    IoTRadio::startScan();
}

void DetectSensorsScreen::stopScan(lv_event_t *e) {
    IoTRadio::stopScan();
    afterScan();
}

void DetectSensorsScreen::goToEditSensor(lv_event_t *e) {
    lv_obj_t *panel = lv_obj_get_parent(e->current_target);
    lv_obj_t *component = lv_obj_get_parent(panel);
    lv_obj_t *label = ui_comp_get_child(component, UI_COMP_SENSORITEM_SENSORITEMPANEL_SENSORID);
    string text = lv_label_get_text(label);
    projectPreferences.set(ProjectPreferences::EditableDetectSensor, text);
    lv_disp_load_scr(ui_edittSensorScreen);
}

void DetectSensorsScreen::handleReceiveSensor(const uint8_t *incomingData) {
    if (!scanIsRunning || ioTRadioDetect.getCurrentSensors().size() > 8) {
        IoTRadio::stopScan();
        afterScan();
        return;
    }
    currentSignal = (ReceivedSignal *) incomingData;
    string signalValue = currentSignal->value.c_str();
    if (!signalValue.empty() && !ioTRadioDetect.exist(signalValue)
        && !iotRadioControl.exist(signalValue)) {
        Task task = {
                [this](void *data) {
                    this->saveNewSensor(data);
                },
                nullptr
        };
        queueTask.addTask(&task);
    }
}

DetectSensorsScreen::DetectSensorsScreen(IoTRadioDetect &d, IotRadioControl &c, QueueTask &q, ProjectPreferences &p)
        : ioTRadioDetect(d),
          queueTask(q),
          iotRadioControl(c),
          projectPreferences(p) {
    selfDetectSensorsScreen = this;
}

void DetectSensorsScreen::deleteSensor(lv_event_t *e) {
    lv_obj_t *panel = lv_obj_get_parent(e->current_target);
    lv_obj_t *component = lv_obj_get_parent(panel);
    lv_obj_t *label = ui_comp_get_child(component, UI_COMP_SENSORITEM_SENSORITEMPANEL_SENSORID);
    string text = lv_label_get_text(label);
    ioTRadioDetect.forget(ioTRadioDetect.getSensorByName(text)->signal);
    lv_obj_del(component);
}
