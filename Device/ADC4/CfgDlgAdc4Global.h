#pragma once

#include "../../BaseWindow.h"
#include "../../Widget.h"
#include "DeviceParamsAdc4.h"

class CfgDlgAdc4Global : public BaseWindow<CfgDlgAdc4Global> {
public:
    CfgDlgAdc4Global(DeviceParamsAdc4* dev_) : device(dev_), bApply(false) {}

    PCWSTR ClassName() const { return L"Modal Dialog Class"; }
    BOOL Show(int nCmdShow = SW_SHOW);
    BOOL isApply() { return bApply; }

protected:
    virtual void onCreate(WPARAM wParamm, LPARAM lParam) override;
    virtual LRESULT onClose(WPARAM wParam, LPARAM lParam) override;
    virtual void onPaint(WPARAM wParam, LPARAM lParam) override;

    Widget<CfgDlgAdc4Global, btn_t> btnApply;
    Widget<CfgDlgAdc4Global, edt_t> edtRangeStart;
    Widget<CfgDlgAdc4Global, edt_t> edtRangeEnd;

private:
    DeviceParamsAdc4* device;
    bool bApply;

    void cmd_btnApply(WID id, int evt, LPARAM lParam);
    void cmd_edtRangeStart(WID id, int evt, LPARAM lParam);
    void cmd_edtRangeEnd(WID id, int evt, LPARAM lParam);

};
