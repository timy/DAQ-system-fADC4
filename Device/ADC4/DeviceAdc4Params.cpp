#include "DeviceAdc4Params.h"
#include "SimpleConfig.h"

/// <summary>
/// SimpleConfig 中，针对 ADC_MODE 的输入输出而特化的变量对象类
/// </summary>
/// 
template <>
class CFG_TYPE_VAR<ADC_MODE> : public CFG_TYPE_ABSTRACT {
public:
    CFG_TYPE_VAR(std::string key, ADC_MODE& value) : CFG_TYPE_ABSTRACT(key), v(value) {}

    virtual void write(std::ostringstream& ss) final {
        ss << k << " = " << v;
    }
    virtual void read(std::istringstream& ss) final {
        std::string strKey, strEqu, strVal;
        ss >> strKey >> strEqu >> strVal;
        v = static_cast<ADC_MODE>(atoi(strVal.c_str()));
    }
protected:
    ADC_MODE& v;
};

/// <summary>
/// Implementation of class DeviceAdc4Params
/// </summary>
/// 
DeviceAdc4Params::DeviceAdc4Params() :
    DeviceItemParams(),
    mode(ADC_MODE_A),
    idxMasterChannel(-1) {
}

DeviceAdc4Params::~DeviceAdc4Params() {}

/// <summary>
/// Bind parameteric variables of DeviceAdc4Params with SimpleConfig  
/// </summary>
/// <param name="cfi">
/// a class object in SimpleConfig with a vector storing items, of which map to lines in configuration file
/// </param>
/// 
void DeviceAdc4Params::attachCfgFileItem(CfgFileItems* cfi) {
    
    char title[64];
    sprintf_s(title, sizeof(title), "[card %d]", this->id);
    cfi->add(title);
    // for card
    cfi->add("id", this->id);
    cfi->add("idxMasterChannel", this->idxMasterChannel);
    cfi->add("isCardEnabled", this->isCardEnabled);
    cfi->add("mode", this->mode);
    cfi->add();
    // for channels
    for (int i = 0; i < nChannels; i++) {
        sprintf_s(title, sizeof(title), "[channel %d]", i);
        cfi->add(std::string(title));
        cfi->add("precursor", this->channel[i].precursor);
        cfi->add("length", this->channel[i].length);
        cfi->add("isRetrigger", this->channel[i].isRetrigger);
        cfi->add("isEdgeMode", this->channel[i].isEdgeMode);
        cfi->add("isRising", this->channel[i].isRising);
        cfi->add("threshold", this->channel[i].threshold);
        cfi->add("offset", this->channel[i].offset);
        cfi->add();
    }
    cfi->add();
}

