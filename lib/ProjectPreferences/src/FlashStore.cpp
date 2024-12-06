#include "FlashStore.h"

using namespace std;

Property FlashStore::read(const std::string &name, std::string defaultValue) {
    string value = flash.getString(name.c_str(), defaultValue.c_str()).c_str();
    return {value, 0, flash.isKey(name.c_str())};
}

void FlashStore::write(const std::string &name, const std::string &value) {
    flash.putString(name.c_str(), value.c_str());
}

FlashStore::FlashStore(const std::string& nameSpace) {
    flash.begin(nameSpace.c_str(), false);
}
