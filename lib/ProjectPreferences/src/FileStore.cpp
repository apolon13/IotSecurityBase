#include "FileStore.h"
#include "StoreMutex.h"

#define DELIMITER '='
#define TERMINATOR '\n'

using namespace std;

FileStore::FileStore(std::string fn): filename(std::move(fn)) {
    if (StoreMutex::take()) {
        if (!sdFat.begin()) {
            throw FailedInitialization();
        }
        if (!sdFat.exists(filename.c_str())) {
            auto file = sdFat.open(filename.c_str(), O_CREAT);
            file.close();
        }
        StoreMutex::give();
    }
}

void FileStore::write(const string &name, const string &value) {
    Property currentValue = read(name, "");
    string newValue = name + DELIMITER + value + TERMINATOR;
    auto dataProvider = sdFat.open(filename.c_str(), (O_RDWR | O_CREAT | O_AT_END));
    if (currentValue.exist) {
        dataProvider.seek(currentValue.position);
    } else {
        dataProvider.seek(dataProvider.size());
    }
    dataProvider.print(newValue.c_str());
    dataProvider.close();
}

Property FileStore::read(const string &name, std::string defaultValue) {
    auto dataProvider = sdFat.open(filename.c_str());
    Property p = {std::move(defaultValue), 0, false};
    dataProvider.seek(0);
    while (dataProvider.available()) {
        size_t position = dataProvider.position();
        string line = dataProvider.readStringUntil(TERMINATOR).c_str();
        if (line.find(name) != string::npos) {
            p = {
                    line.substr(line.find(DELIMITER) + 1),
                    position,
                    true
            };
            break;
        }
    }
    dataProvider.close();
    StoreMutex::give();
    return p;
}
