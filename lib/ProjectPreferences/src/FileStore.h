#include "Store.h"
#include "SdFat.h"
#include <utility>

#ifndef DISPLAY_FILESTORE_H
#define DISPLAY_FILESTORE_H

class FileStore : public Store {
protected:
    std::string filename;
    SdFat sdFat;

    Property read(const std::string &name, std::string defaultValue) override;

    void write(const std::string &name, const std::string &value) override;

public:
    explicit FileStore(std::string fn);
};

#endif
