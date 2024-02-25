#include "Security.h"

Security *selfSecurity;

#define GUARD "Guard"
#define DISARM "Disarm"
#define MUTE "Mute"
#define ALARM "Alarm"

uint8_t receiverAddress[] = {0x08, 0xD1, 0xF9, 0xEB, 0x02, 0x58};

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


void Security::handleControl(const string& name) {
    auto lockScreen = ui->getEventHandler()->getLockScreen();
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

    receiveCmdTopic->publish(name);
}

void Security::handleDetect(const string& signal) {
    long now = millis();
    auto detectSensor = selfSecurity->ioTRadioDetect->getSensorBySignal(signal);
    if (projectPreferences->systemIsLocked() && detectSensor->isActive) {
        if ((now - lastAlarmEvent) > 5000) {
            receiveCmdTopic->publish(ALARM);
            lastAlarmEvent = now;
        }
        alarm();
    }
}

Security::Security(IoTRadioDetect *d, IotRadioControl *c, UiControl *ui, ProjectPreferences *p, Topic *cmd, Topic *rcv)
        : ioTRadioDetect(d),
          iotRadioControl(c),
          ui(ui),
          projectPreferences(p),
          securityCmdTopic(cmd),
          receiveCmdTopic(rcv) {
    selfSecurity = this;
    projectPreferences->lockSystem();
    IoTRadio::addReceiver(receiverAddress);
    listen();
}

void Security::listenMqttCommands() {
    securityCmdTopic->refreshHandlers();
    securityCmdTopic->addHandler([this](const string& payload) {
        handleControl(payload);
    });
}

void Security::listen() {
    listenRadioCommands();
    listenMqttCommands();
}