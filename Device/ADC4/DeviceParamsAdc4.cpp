#include "DeviceParamsAdc4.h"
#include "../../SimpleConfig.h"
#include <iostream>

/// <summary>
/// SimpleConfig 中，针对 ADC_MODE 的输入输出而特化的变量对象类
/// </summary>
/// 
template <>
class CFG_TYPE_VAR<ADC_MODE> : public CFG_TYPE_ABSTRACT {
public:
    CFG_TYPE_VAR(std::string key, ADC_MODE& value) : CFG_TYPE_ABSTRACT(key), v(value) {}

    virtual void write(std::ostringstream& ss) final {
        ss << k << " = " << static_cast<int>(v);
    }
    virtual void read(std::istringstream& ss) final {
        std::string strKey, strEqu, strVal;
        ss >> strKey >> strEqu >> strVal;
        v = static_cast<ADC_MODE>(atoi(strVal.c_str()));
    }
protected:
    ADC_MODE& v;
};

unsigned int AdcModeIndex(ADC_MODE mode) {
    unsigned int count = 0;
    for (auto i : ADC_mode_list) {
        if (mode == i.mode)
            break;
        count++;
    }
    return count;
}

//////////////////////////////////////////////////////////////////
// Card
//////////////////////////////////////////////////////////////////

/// <summary>
/// Implementation of class CardParamsAdc4
/// </summary>
/// 
CardParamsAdc4::CardParamsAdc4() :
    CardParamsBase(),
    mode(ADC_MODE::A),
    maskTriggerChannel(0)
{
}

CardParamsAdc4::~CardParamsAdc4() {}

CardParamsAdc4& CardParamsAdc4::operator = (const CardParamsAdc4& card) {
    this->id = card.id;
    this->maskTriggerChannel = card.maskTriggerChannel;
    this->isEnabled = card.isEnabled;
    this->mode = card.mode;
    for (unsigned int i = 0; i < nChannels; i++) {
        this->channels[i] = card.channels[i];
    }
    return *this;
}

/// <summary>
/// Bind parameteric variables of CardParamsAdc4 with SimpleConfig  
/// </summary>
/// <param name="cfi">
/// a class object in SimpleConfig with a vector storing items, of which map to lines in configuration file
/// </param>
/// 
void CardParamsAdc4::bindCfgFileItem(CfgFileItems* cfi) {
    
    char title[64];
    sprintf_s(title, sizeof(title), "[card %u]", this->id);
    cfi->add(title);
    // for card
    cfi->add("id", this->id);
    cfi->add("maskTriggerChannel", this->maskTriggerChannel);
    cfi->add("isEnabled", this->isEnabled);
    cfi->add("mode", this->mode);
    cfi->add();
    // for channels
    for (unsigned int i = 0; i < nChannels; i++) {
        sprintf_s(title, sizeof(title), "[channel %u]", i);
        cfi->add(std::string(title));
        cfi->add("precursor", this->channels[i].precursor);
        cfi->add("length", this->channels[i].length);
        cfi->add("isRetrigger", this->channels[i].isRetrigger);
        cfi->add("isEdgeMode", this->channels[i].isEdgeMode);
        cfi->add("isRising", this->channels[i].isRising);
        cfi->add("threshold", this->channels[i].threshold);
        cfi->add("offset", this->channels[i].offset);
        cfi->add();
    }
    cfi->add();
}

//////////////////////////////////////////////////////////////////
// Device
//////////////////////////////////////////////////////////////////

DeviceParamsAdc4::DeviceParamsAdc4(unsigned int nCards) : DeviceParamsBase(nCards),
trigger_card(0), trigger_channel(0),
range_start(-1000), range_end(+1000) {

    if (nCards > 0) {
        cards = new CardParamsBase * [nCards];
        for (unsigned int i = 0; i < nCards; i++) {
            cards[i] = new CardParamsAdc4;
            cards[i]->id = i;
        }

        updateToTrigChannelMask();

        // bind parameters to configuration file with SimpleConfig
        bindCfgFileItem();
    }
}

DeviceParamsAdc4::~DeviceParamsAdc4() {
    if (nCards > 0) {
        for (unsigned int i = 0; i < nCards; i++) {
            delete cards[i];
        }
        delete[] cards;
        cards = nullptr;
    }
}

void DeviceParamsAdc4::importFromFile(std::ifstream& file) {
    cfi->read(file);
    std::cout << "Configuration parameters were loaded from file" << std::endl << std::endl;
}

void DeviceParamsAdc4::exportToFile(std::ofstream& file) {
    cfi->write(file);
    std::cout << "Configuration parameters were written to file" << std::endl << std::endl;
}

#include "CfgDlgAdc4.h" // DisplayModalDialog
bool DeviceParamsAdc4::applyCardConfiguration(unsigned int i, HWND hwnd) {

    // create a temp struct to hold params of card[i], which will be shown in the device config pannel
    CardParamsAdc4 newCard = *static_cast<CardParamsAdc4*>(cards[i]);

    // show the config pannel for user to start parameters configuration
    // return bApply = true if the "Apply" button is clicked
    if (DisplayModalDialog(hwnd, &newCard)) {
        CardParamsAdc4* card = static_cast<CardParamsAdc4*>(cards[newCard.id]);
        // fill the corresponding card with updated params stored in newCard
        *card = newCard;
        if (card->isEnabled) {
            // trigger set in the current card
            if (card->maskTriggerChannel > 0) {
                // if trigger card has changed, reset the mask of the old one and update global params
                if (card->id != trigger_card) {
                    CardParamsAdc4* cardOldTrig = static_cast<CardParamsAdc4*>(cards[trigger_card]);
                    cardOldTrig->maskTriggerChannel = 0;
                    std::cout << "[INFO] Change master trigger:"
                        << " card "
                        << trigger_card
                        << " channel "
                        << trigger_channel
                        << " -> card "
                        << card->id
                        << " channel "
                        << card->triggerChannel()
                        << std::endl;
                    trigger_card = card->id;
                }
                // if trigger channel has changed, update global params
                if (card->triggerChannel() != trigger_channel) {
                    trigger_channel = card->triggerChannel();
                }
            }
        }
        return true;
    }
    return false;
}

void DeviceParamsAdc4::bindCfgFileItem() {
    // params of device
    char title[64];
    sprintf_s(title, sizeof(title), "[device ADC4]");
    cfi->add(title);
    cfi->add("trigger_card", trigger_card);
    cfi->add("trigger_channel", trigger_channel);
    cfi->add("range_start", range_start);
    cfi->add("range_end", range_end);
    cfi->add();
    // params of cards
    for (unsigned int i = 0; i < nCards; i++) {
        cards[i]->bindCfgFileItem(cfi);
    }
}

// update maskTriggerChannel when trigCard & trigChannel are set
void DeviceParamsAdc4::updateToTrigChannelMask() {
    for (unsigned int i = 0; i < nCards; i++) {
        CardParamsAdc4* card = static_cast<CardParamsAdc4*>(cards[i]);
        if (i == trigger_card)
            card->maskTriggerChannel = 1 << trigger_channel;
        else
            card->maskTriggerChannel = 0;
    }
}