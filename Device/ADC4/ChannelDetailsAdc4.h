#pragma once

#include <map>
#include <vector>
#include <string>
#include <Windows.h>
#include "Widget.h"

class CfgDlgAdc4;

class ChannelDetailsAdc4 {
public:
    ChannelDetailsAdc4();
    virtual ~ChannelDetailsAdc4();
    void Create(CfgDlgAdc4* wnd, int x);
    void EnableAll(bool bEnable);

    std::map<std::string, BaseWidget<CfgDlgAdc4>*> w;

private:
    std::vector<BaseWidget<CfgDlgAdc4>*> ws; // widget of sequence
    typedef void (CfgDlgAdc4::* CmdFunc)(WID, int, LPARAM);
    std::vector<CmdFunc> wsCmdFunc; // command functions

    struct WG_PARAMS {
        int w, h;
    };
    std::vector<WG_PARAMS*> params;
    struct WG_ITEMS {
        int w, h;
    };
};