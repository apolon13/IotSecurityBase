#include "EditDetectSensorScreen.h"

void EditDetectSensorScreen::loadSensor(lv_event_t *e) {
    auto name = projectPreferences->get(EditableDetectSensor, "");
    if (!name.empty()) {
        auto sensor = ioTRadioDetect->getSensorByName(name);
        lv_textarea_set_text(ui_sensorName, sensor->name.c_str());
        if (sensor->isActive) {
            lv_obj_add_state(ui_sensorIsActive, LV_STATE_CHECKED);
        }
        lv_slider_set_value(ui_sensorSensitivity, sensor->sensitivity, LV_ANIM_OFF);
    }
}


void EditDetectSensorScreen::saveSensorSettings(lv_event_t *e) {
    auto name = projectPreferences->get(EditableDetectSensor, "");
    if (!name.empty()) {
        auto sensor = ioTRadioDetect->getSensorByName(name);
        Sensor s = {
                lv_textarea_get_text(ui_sensorName),
                sensor->signal,
                static_cast<bool>(lv_obj_get_state(ui_sensorIsActive) & LV_STATE_CHECKED),
                lv_slider_get_value(ui_sensorSensitivity)
        };
        ioTRadioDetect->save(&s);
        projectPreferences->set(EditableDetectSensor, "");
    }
}