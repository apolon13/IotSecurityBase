#include "Security.h"

Security *selfSecurity;

#define GUARD "Guard"
#define DISARM "Disarm"
#define MUTE "Mute"
#define ALARM "Alarm"
#define RESTART "Restart"

uint8_t receiverAddress[] = {0x08, 0xD1, 0xF9, 0xEB, 0x02, 0x58};
using namespace std;

void Security::guard() {
    IoTRadio::sendMessageToPeer({true, false});
}

void Security::disarm() {
    IoTRadio::sendMessageToPeer({true, false});
}

void Security::alarm() {
    IoTRadio::sendMessageToPeer({true, !muteMode});
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
        muteMode = false;
        guard();
        projectPreferences.lockSystem();
        eventId = SecurityEvent::EventOnGuard;
    }

    if (cmd == DISARM) {
        muteMode = false;
        disarm();
        eventId = SecurityEvent::EventOnDisarm;
        if (projectPreferences.systemIsLocked()) {
            projectPreferences.unlockSystem();
        }
    }

    if (cmd == MUTE) {
        muteMode = true;
        mute();
        eventId = SecurityEvent::EventOnMute;
    }

    if (cmd == ALARM) {
        muteMode = false;
        alarm();
        eventId = SecurityEvent::EventOnAlarm;
    }

    if (cmd == RESTART) {
        muteMode = false;
        eventId = SecurityEvent::EventOnRestart;
    }

    if (needTriggerEvent && eventId != SecurityEvent::UnknownEvent) {
        triggerEvent((int) eventId);
    }

    if (previousCmd != cmd) {
        previousCmd = cmd;
        pubSub.publish(PubSub::Receive, cmd);
    }

    if (cmd == RESTART) {
        restart();
    }
}

void Security::handleDetect(const string &signal) {
    long now = millis();
    auto detectSensor = selfSecurity->ioTRadioDetect.getSensorBySignal(signal);
    if (projectPreferences.systemIsLocked() && detectSensor->isActive) {
        if ((now - lastAlarmEventTime) > 5000) {
            string msg = "Сработал датчик - ";
            msg.append(detectSensor->name);
            pubSub.publish(PubSub::Alarm, msg);
            pubSub.publish(PubSub::Receive, ALARM);
            lastAlarmEventTime = now;
        }
        alarm();
    }
}

Security::Security(IoTRadioDetect &d, IotRadioControl &c, ProjectPreferences &p, PubSub &ps)
        : ioTRadioDetect(d),
          iotRadioControl(c),
          projectPreferences(p),
          pubSub(ps) {
    selfSecurity = this;
    projectPreferences.lockSystem();
    IoTRadio::addReceiver(receiverAddress);
    listen();
}

void Security::listen() {
    listenRadioCommands();
}

void Security::lockSystem(bool silent) {
    handleControl(GUARD, !silent);
}

void Security::unlockSystem(bool silent) {
    handleControl(DISARM, !silent);
}

void Security::restart() {
    ESP.restart();
}

void Security::runCommand(SecurityCommand command) {
    switch (command) {
        case SecurityCommand::Restart:
            handleControl(RESTART);
            break;
        case SecurityCommand::Guard:
            handleControl(GUARD);
            break;
        case SecurityCommand::Disarm:
            handleControl(DISARM);
            break;
        case SecurityCommand::Alarm:
            handleControl(ALARM);
            break;
        case SecurityCommand::Mute:
            handleControl(RESTART);
            break;
    }
}
