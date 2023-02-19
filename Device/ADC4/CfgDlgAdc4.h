#pragma once

#include "../../BaseWindow.h"
#include "../../Widget.h"
#include "ChannelDetailsAdc4.h"
#include "DeviceParamsAdc4.h"

class CfgDlgAdc4 : public BaseWindow<CfgDlgAdc4> {
public:
    CfgDlgAdc4(CardParamsAdc4* dev_) : card(dev_), bApply(false) {}

    PCWSTR ClassName() const { return L"Modal Dialog Class"; }
    BOOL Show(int nCmdShow = SW_SHOW);
    BOOL isApply() { return bApply; }

protected:

    virtual void onCreate(WPARAM wParamm, LPARAM lParam) override;
    virtual LRESULT onClose(WPARAM wParam, LPARAM lParam) override;
    virtual void onPaint(WPARAM wParam, LPARAM lParam) override;

    Widget<CfgDlgAdc4, ckb_t> ckbCardEnabled;
    Widget<CfgDlgAdc4, cbb_t> cbbADCMode;
    Widget<CfgDlgAdc4, btn_t> btnApply;

    friend ChannelDetailsAdc4;
    // 4 channels for A, B, C, D and 1 channel for T
    static const unsigned int nChannels = CardParamsAdc4::nChannels; 
    ChannelDetailsAdc4 chd[nChannels] = { true, true, true, true, false };

private:
    CardParamsAdc4* card;
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
    template <typename T> void RetrieveIntFromEdit(HWND hWnd, T& value); // T for int or unsigned int
    template <typename T> void UpdateEditFromInt(HWND hWnd, T value); // T for int or unsigned int
};
