#include "Store.h"
#include "ui.h"
#include "Screen.h"

#ifndef DISPLAY_MQTTCONFIGSCREEN_H
#define DISPLAY_MQTTCONFIGSCREEN_H

class MqttConfigScreen : public Screen {
private:
    Store &store;
public:
    MqttConfigScreen(Store &p) : store(p) {};

    void loadMqttSettings(lv_event_t *e);

    void saveMqttSettings(lv_event_t *e);
};

#endif
