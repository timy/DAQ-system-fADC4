#pragma once

#include "../../CoboldPC_2011_R5-2c_fADC4_2020-09-17/Sources/Libs/Ndigo_interface.h"

struct ChannelAdc4Params {
    ChannelAdc4Params() : 
        precursor(2),
        length(4),
        isRetrigger(true),
        isEdgeMode(false),
        isRising(false),
        threshold(10000),
        offset(0) {}
    virtual ~ChannelAdc4Params() {}

    ChannelAdc4Params& operator = (const ChannelAdc4Params& ch) {
        this->isEdgeMode = ch.isEdgeMode;
        this->isRetrigger = ch.isRetrigger;
        this->isRising = ch.isRising;
        this->length = ch.length;
        this->offset = ch.offset;
        this->precursor = ch.precursor;
        this->threshold = ch.threshold;
        return *this;
    }

    unsigned int precursor;
    unsigned int length;
    bool isRetrigger;
    bool isEdgeMode;
    bool isRising;
    unsigned int threshold;
    unsigned int offset;
};

enum ADC_MODE {
    ADC_MODE_ABCD = NDIGO_ADC_MODE_ABCD,
    ADC_MODE_A = NDIGO_ADC_MODE_A,
    ADC_MODE_B = NDIGO_ADC_MODE_B,
    ADC_MODE_C = NDIGO_ADC_MODE_C,
    ADC_MODE_D = NDIGO_ADC_MODE_D,
};

const struct {
    ADC_MODE mode;
    const wchar_t* label;
} ADC_mode_list[] = {
    {ADC_MODE_ABCD, L"ABCD"},
    {ADC_MODE_A,    L"A"},
    {ADC_MODE_B,    L"B"},
    {ADC_MODE_C,    L"C"},
    {ADC_MODE_D,    L"D"}
};

inline int AdcModeIndex(ADC_MODE mode) {
    int count = 0;
    for (auto i : ADC_mode_list) {
        if (mode == i.mode)
            break;
        count++;
    }
    return count;
}

#include "DeviceParams.h"
class CfgFileItems;

class DeviceAdc4Params : public DeviceItemParams {
public:
    DeviceAdc4Params();
    virtual ~DeviceAdc4Params();

    virtual DeviceAdc4Params& operator = (const DeviceAdc4Params& card) {
        this->id = card.id;
        this->idxMasterChannel = card.idxMasterChannel;
        this->isCardEnabled = card.isCardEnabled;
        this->mode = card.mode;
        for (int i = 0; i < nChannels; i++) {
            this->channel[i] = card.channel[i];
        }
        return *this;
    }

    virtual void attachCfgFileItem(CfgFileItems* cfi);

    ADC_MODE mode;
    int idxMasterChannel;

    static const int nChannels = 4;
    ChannelAdc4Params channel[nChannels];
};



