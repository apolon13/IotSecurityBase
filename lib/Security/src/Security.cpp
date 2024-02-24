#include "Security.h"

Security *selfSecurity;

#define GUARD "Guard"
#define DISARM "Disarm"
#define MUTE "Mute"
#define ALARM "Alarm"


void Security::guard() {
    IoTRadio::sendMessageToPeer({true, false});
}

void Security::disarm() {
    IoTRadio::sendMessageToPeer({true, false});
}

void Security::alarm() {
    IoTRadio::sendMessageToPeer({true, true});
}

void Security::mute() {
    IoTRadio::sendMessageToPeer({true, false});
}

void Security::listenRadioCommands() {
    IoTRadio::addRecvHandler([](const uint8_t *mac, const uint8_t *incomingData, int len) {
        auto *signal = (ReceivedSignal *) incomingData;
        string signalValue = signal->value.c_str();
        if (selfSecurity->ioTRadioDetect->exist(signalValue)) {
            selfSecurity->handleDetect(signalValue);
        }
        if (selfSecurity->iotRadioControl->exist(signalValue)) {
            auto sensor = selfSecurity->iotRadioControl->getSensorBySignal(signalValue);
            selfSecurity->handleControl(sensor->name);
        }
    });
    IoTRadio::startScan();
}


void Security::handleControl(string name) {
    auto lockScreen = ui->getEventHandler()->getLockScreen();
    Serial.println(name.c_str());
    if (name == GUARD) {
        guard();
        projectPreferences->lockSystem();
        lockScreen->goTo(true);
    }

    if (name == DISARM) {
        disarm();
        if (projectPreferences->systemIsLocked()) {
            lockScreen->unlockSystem(nullptr, true, true);
        }
    }

    if (name == MUTE) {
        mute();
    }

    if (name == ALARM) {
        alarm();
    }
}

void Security::handleDetect(string signal) {
    if (projectPreferences->systemIsLocked()) {
        alarm();
    }
}

Security::Security(IoTRadioDetect *d, IotRadioControl *c, UiControl *ui, ProjectPreferences *p, Topic *t) : ioTRadioDetect(d),
                                                                                                  iotRadioControl(c),
                                                                                                  ui(ui),
                                                                                                  projectPreferences(p),
                                                                                                  securityTopic(t) {
    selfSecurity = this;
    projectPreferences->lockSystem();
}

void Security::listenMqttCommands() {
    securityTopic->refreshHandlers();
    securityTopic->addHandler([this](string payload) {
        handleControl(payload.c_str());
    });
}

void Security::listen() {
    listenRadioCommands();
    listenMqttCommands();
}