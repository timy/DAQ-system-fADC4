#pragma once

#include "Libs/Ndigo_interface.h"
#include "../../DeviceParamsBase.h"

//////////////////////////////////////////////////////////////////
// Channel 
//////////////////////////////////////////////////////////////////
struct ChannelParamsAdc4 {
    ChannelParamsAdc4() : 
        precursor(0),
        length(0),
        isRetrigger(false),
        isEdgeMode(true),
        isRising(false),
        threshold(0),
        offset(0) {}
    virtual ~ChannelParamsAdc4() {}

    ChannelParamsAdc4& operator = (const ChannelParamsAdc4& ch) {
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
    int threshold;
    int offset;
};

enum class ADC_MODE {
    ABCD = NDIGO_ADC_MODE_ABCD,
    A = NDIGO_ADC_MODE_A,
    B = NDIGO_ADC_MODE_B,
    C = NDIGO_ADC_MODE_C,
    D = NDIGO_ADC_MODE_D,
};

const struct {
    ADC_MODE mode;
    const wchar_t* label;
} ADC_mode_list[] = {
    {ADC_MODE::ABCD, L"ABCD"},
    {ADC_MODE::A,    L"A"},
    {ADC_MODE::B,    L"B"},
    {ADC_MODE::C,    L"C"},
    {ADC_MODE::D,    L"D"}
};

unsigned int AdcModeIndex(ADC_MODE mode);

//////////////////////////////////////////////////////////////////
// Card
//////////////////////////////////////////////////////////////////
#include "../../CardParamsBase.h"
#include <intrin.h> // _lzcnt_u32


class CfgFileItems;

class CardParamsAdc4 : public CardParamsBase {
public:
    CardParamsAdc4();
    virtual ~CardParamsAdc4();

    virtual CardParamsAdc4& operator = (const CardParamsAdc4& card);

    inline unsigned int triggerChannel() const {
        return indexOfMaskBit(maskTriggerChannel) - 1;
    }

    virtual void bindCfgFileItem(CfgFileItems* cfi);

    ADC_MODE mode;
    unsigned int maskTriggerChannel;

    static const unsigned int nChannels = 5; // A, B, C, D, and digital T
    ChannelParamsAdc4 channels[nChannels];

private:
    inline static unsigned int indexOfMaskBit(unsigned int x) {
        return 32 - _lzcnt_u32(x ^ (x >> 31)); // evaluate log2(x) for 32-bit mask x
    }
};

#include <fstream> // ifstream, ofstream

//////////////////////////////////////////////////////////////////
// Device
//////////////////////////////////////////////////////////////////
// DeviceParamsAdc4 的管理类，针对（多个）DeviceItemParams，整合 CfgFileItems 导入导出变量、UI
class DeviceParamsAdc4 : public DeviceParamsBase {
public:
    DeviceParamsAdc4(unsigned int nCards);
    virtual ~DeviceParamsAdc4();

    virtual void importFromFile(std::ifstream& file);
    virtual void exportToFile(std::ofstream& file);
    virtual bool applyCardConfiguration(unsigned int i, HWND hwnd);

    unsigned int trigger_card;
    unsigned int trigger_channel;
    int range_start; // ns
    int range_end;   // ns

private:

    void bindCfgFileItem();

    // update maskTriggerChannel when trigCard & trigChannel are set
    void updateToTrigChannelMask();
};
