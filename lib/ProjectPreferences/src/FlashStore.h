#include "Store.h"
#include "Preferences.h"

#ifndef DISPLAY_FLASHSTORE_H
#define DISPLAY_FLASHSTORE_H
class FlashStore: public Store {
protected:
    Preferences flash;

    Property read(const std::string &name, std::string defaultValue) override;

    void write(const std::string &name, const std::string &value) override;

public:
    explicit FlashStore(const std::string& nameSpace);
};
#endif
