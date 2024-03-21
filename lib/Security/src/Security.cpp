#include "Security.h"

Security *selfSecurity;

#define GUARD "Guard"
#define DISARM "Disarm"
#define MUTE "Mute"
#define ALARM "Alarm"

uint8_t receiverAddress[] = {0x08, 0xD1, 0xF9, 0xEB, 0x02, 0x58};
using namespace std;

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
        if (selfSecurity->ioTRadioDetect.exist(signalValue)) {
            selfSecurity->handleDetect(signalValue);
        }
        if (selfSecurity->iotRadioControl.exist(signalValue)) {
            auto sensor = selfSecurity->iotRadioControl.getSensorBySignal(signalValue);
            selfSecurity->handleControl(sensor->name, true);
        }
    });
    IoTRadio::startScan();
}


void Security::handleControl(const string &cmd, bool needTriggerEvent) {
    auto eventId = SecurityEvent::UnknownEvent;
    if (cmd == GUARD) {
        guard();
        projectPreferences.lockSystem();
        eventId = SecurityEvent::EventOnGuard;
    }

    if (cmd == DISARM) {
        disarm();
        if (projectPreferences.systemIsLocked()) {
            projectPreferences.unlockSystem();
            eventId = SecurityEvent::EventOnDisarm;
        }
    }

    if (cmd == MUTE) {
        mute();
        eventId = SecurityEvent::EventOnMute;
    }

    if (cmd == ALARM) {
        alarm();
        eventId = SecurityEvent::EventOnAlarm;
    }

    if (needTriggerEvent && eventId != SecurityEvent::UnknownEvent) {
        triggerEvent((int) eventId);
    }

    if (previousCmd != cmd) {
        previousCmd = cmd;
        receiveCmdTopic.publish(cmd);
    }
}

void Security::handleDetect(const string &signal) {
    long now = millis();
    auto detectSensor = selfSecurity->ioTRadioDetect.getSensorBySignal(signal);
    if (projectPreferences.systemIsLocked() && detectSensor->isActive) {
        if ((now - lastAlarmEventTime) > 5000) {
            string msg = "Сработал датчик - ";
            msg.append(detectSensor->name);
            alarmTopic.publish(msg);
            receiveCmdTopic.publish(ALARM);
            lastAlarmEventTime = now;
        }
        alarm();
    }
}

Security::Security(IoTRadioDetect &d, IotRadioControl &c, ProjectPreferences &p, Topic &cmd, Topic &rcv, Topic &al)
        : ioTRadioDetect(d),
          iotRadioControl(c),
          projectPreferences(p),
          securityCmdTopic(cmd),
          receiveCmdTopic(rcv),
          alarmTopic(al) {
    selfSecurity = this;
    projectPreferences.lockSystem();
    IoTRadio::addReceiver(receiverAddress);
    listen();
}

void Security::listenMqttCommands() {
    securityCmdTopic.refreshHandlers();
    securityCmdTopic.addHandler([this](const string &payload) {
        handleControl(payload, true);
    });
}

void Security::listen() {
    listenRadioCommands();
    listenMqttCommands();
}

void Security::lockSystem(bool silent) {
    handleControl(GUARD, !silent);
}

void Security::unlockSystem(bool silent) {
    handleControl(DISARM, !silent);
}

void Security::listenSmsCommands(TinyGsmSim7670 &modem) {
    auto response = modem.readSmsByIndex(lastSmsIndex, GUARD, DISARM, MUTE, ALARM);

    if (response == 0) {
        lastSmsIndex = lastSmsIndex > 1 ? lastSmsIndex - 1 : 1;
    }

    if (response == -1) {
        lastSmsIndex++;
    }

    switch (response) {
        case 1:
            handleControl(GUARD);
            break;
        case 2:
            handleControl(DISARM);
            break;
        case 3:
            handleControl(MUTE);
            break;
        case 4:
            handleControl(ALARM);
            break;
    }
}
