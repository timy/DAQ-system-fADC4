#include "DeviceParamsBase.h"
#include "SimpleConfig.h"

DeviceParamsBase::DeviceParamsBase(unsigned int nCards) : nCards(nCards), cards(nullptr), cfi(nullptr) {
    cfi = new CfgFileItems;
}

DeviceParamsBase::~DeviceParamsBase() {
    if (cfi != nullptr) {
        delete cfi;
        cfi = nullptr;
    }
}
