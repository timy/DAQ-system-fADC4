#pragma once

#include "BaseWindow.h"
#include "Widget.h"
#include "ChannelDetailsAdc4.h"

class DeviceAdc4Params;

class CfgDlgAdc4 : public BaseWindow<CfgDlgAdc4> {
public:
    CfgDlgAdc4(DeviceAdc4Params* dev_) : card(dev_), bApply(false) {}

    PCWSTR ClassName() const { return L"Modal Dialog Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lPaBram);
    BOOL Show(int nCmdShow = SW_SHOW);
    BOOL isApply() { return bApply; }

protected:

    virtual LRESULT onClose(WPARAM wParam, LPARAM lParam);
    virtual void onPaint(WPARAM wParam, LPARAM lParam);

    Widget<CfgDlgAdc4, ckb_t> ckbCardEnabled;
    Widget<CfgDlgAdc4, cbb_t> cbbADCMode;
    Widget<CfgDlgAdc4, btn_t> btnApply;

    friend ChannelDetailsAdc4;
    static const int nChannels = 4;
    ChannelDetailsAdc4 chd[nChannels];

    virtual void onCreate(WPARAM wParamm, LPARAM lParam);
    virtual void onCommand(WPARAM wParam, LPARAM lParam);

private:
    DeviceAdc4Params* card;
    bool bApply;
    bool bChannelEnabled[nChannels];

    // 获得cmd消息的控件，其id映射到对应的处理函数
    void cmd_ckbCardEnabled(WID id, int evt, LPARAM lParam);
    void cmd_cbbADCMode(WID id, int evt, LPARAM lParam);
    void cmd_btnApply(WID id, int evt, LPARAM lParam);
    // 子区域上的控件处理函数
    void cmd_rdbMasterChannel(WID id, int evt, LPARAM lParam);
    void cmd_edtPrecursor(WID id, int evt, LPARAM lParam);
    void cmd_edtLength(WID id, int evt, LPARAM lParam);
    void cmd_ckbRetrigger(WID id, int evt, LPARAM lParam);
    void cmd_ckbEdgeMode(WID id, int evt, LPARAM lParam);
    void cmd_ckbRising(WID id, int evt, LPARAM lParam);
    void cmd_edtThreshold(WID id, int evt, LPARAM lParam);
    void cmd_edtOffset(WID id, int evt, LPARAM lParam);

    // utilities
    // convert from WID id to group index
    inline size_t indexOfChannelDetailsGroup(std::string label, WID id) {
        return (id - chd[0].w[label]->id) / (chd[0].w.size());
    }
    void UpdateCardEnabled();
    void UpdateChannelEnabled();
    void RetrieveIntFromEdit(HWND hWnd, unsigned int& value);
    void UpdateEditFromInt(HWND hWnd, unsigned int value);
};

bool DisplayModalDialog(HWND hParent, DeviceAdc4Params* card);
