#pragma once

class CardParamsBase;
class CfgFileItems;
struct HWND__;
typedef HWND__* HWND;

class DeviceParamsBase {
public:
    DeviceParamsBase(unsigned int nCards = 0);
    virtual ~DeviceParamsBase();
    virtual bool applyCardConfiguration(unsigned int i, HWND hwnd) { return true; };

    unsigned int nCards;
    CardParamsBase** cards;
    CfgFileItems* cfi;
};