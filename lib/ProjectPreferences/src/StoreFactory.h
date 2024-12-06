#include <string>
#include <utility>
#include "Store.h"

#ifndef IOTSECURITYBASE_STOREFACTORY_H
#define IOTSECURITYBASE_STOREFACTORY_H


class StoreFactory {
    std::string ns;
public:
    explicit StoreFactory(std::string ns): ns(std::move(ns)) {};
    std::unique_ptr<Store> create();
};


#endif
