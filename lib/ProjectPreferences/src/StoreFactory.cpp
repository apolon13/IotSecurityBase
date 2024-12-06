#include "StoreFactory.h"
#include "FileStore.h"
#include "FlashStore.h"


std::unique_ptr<Store> StoreFactory::create() {
    try {
        return std::make_unique<FileStore>(ns);
    } catch (FailedInitialization &e) {
        return std::make_unique<FlashStore>(ns);
    }
}
