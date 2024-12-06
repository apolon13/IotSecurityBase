#include "Store.h"
#include "StoreMutex.h"
#include <map>

using namespace std;

std::map<int, string> Store::cache = {};

void Store::set(Store::Key property, const string &value) {
    if (StoreMutex::take()) {
        write(convertProperty(property), value);
        cache.insert_or_assign(property, value);
        StoreMutex::give();
    }
}

string Store::get(Store::Key property, string defaultValue) {
    if (StoreMutex::take()) {
        try {
            auto val = cache.at(property);
            StoreMutex::give();
            return val;
        } catch (out_of_range &e) {
            auto value = read(convertProperty(property), std::move(defaultValue)).value;
            cache.insert(std::make_pair(property, value));
            StoreMutex::give();
            return value;
        }
    }
}

void Store::lockSystem() {
    set(SystemState, ON);
}

void Store::unlockSystem() {
    set(SystemState, OFF);
}

bool Store::systemIsLocked() {
    return get(SystemState, OFF) == ON;
}

string Store::getSecurityTimeout() {
    return get(SecurityTimeout, "30");
}

string Store::getConnectionTimeout() {
    return get(ConnectionTimeout, "3");
}

string Store::getConnectionAttemptsBeforeRestart() {
    return get(ConnectionAttemptsBeforeRestart, "10");
}

bool Store::networkModeIsWifi() {
    return !networkModeIsSim() && get(WifiNetworkMode, "1") == "1";
}

bool Store::networkModeIsSim() {
    return get(SimNetworkMode, "0") == "1";
}

void Store::setNetworkMode(Store::Key mode) {
    if (mode == Key::SimNetworkMode) {
        set(Store::SimNetworkMode, ON);
        set(Store::WifiNetworkMode, OFF);
    }

    if (mode == Key::WifiNetworkMode) {
        set(Store::SimNetworkMode, OFF);
        set(Store::WifiNetworkMode, ON);
    }
}


