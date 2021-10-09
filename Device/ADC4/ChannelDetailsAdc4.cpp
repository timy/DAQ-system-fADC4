#include "ChannelDetailsAdc4.h"
#include "Widget.h"
#include "BaseWindow.h"
#include "CfgDlgAdc4.h"

ChannelDetailsAdc4::ChannelDetailsAdc4() {
    ws.push_back(w["rdbMasterChannel"] = new Widget<CfgDlgAdc4, rdb_t>);
    params.push_back(new WG_PARAMS({ 12, 12 }));
    wsCmdFunc.push_back(&CfgDlgAdc4::cmd_rdbMasterChannel);

    ws.push_back(w["edtPrecursor"] = new Widget<CfgDlgAdc4, edt_t>);
    params.push_back(new WG_PARAMS({ 50, 20 }));
    wsCmdFunc.push_back(&CfgDlgAdc4::cmd_edtPrecursor);

    ws.push_back(w["edtLength"] = new Widget<CfgDlgAdc4, edt_t>);
    params.push_back(new WG_PARAMS({ 50, 20 }));
    wsCmdFunc.push_back(&CfgDlgAdc4::cmd_edtLength);

    ws.push_back(w["ckbRetrigger"] = new Widget<CfgDlgAdc4, ckb_t>);
    params.push_back(new WG_PARAMS({ 12, 12 }));
    wsCmdFunc.push_back(&CfgDlgAdc4::cmd_ckbRetrigger);

    ws.push_back(w["ckbEdgeMode"] = new Widget<CfgDlgAdc4, ckb_t>);
    params.push_back(new WG_PARAMS({ 12, 12 }));
    wsCmdFunc.push_back(&CfgDlgAdc4::cmd_ckbEdgeMode);

    ws.push_back(w["ckbRising"] = new Widget<CfgDlgAdc4, ckb_t>);
    params.push_back(new WG_PARAMS({ 12, 12 }));
    wsCmdFunc.push_back(&CfgDlgAdc4::cmd_ckbRising);

    ws.push_back(w["edtThreshold"] = new Widget<CfgDlgAdc4, edt_t>);
    params.push_back(new WG_PARAMS({ 50, 20 }));
    wsCmdFunc.push_back(&CfgDlgAdc4::cmd_edtThreshold);

    ws.push_back(w["edtOffset"] = new Widget<CfgDlgAdc4, edt_t>);
    params.push_back(new WG_PARAMS({ 50, 20 }));
    wsCmdFunc.push_back(&CfgDlgAdc4::cmd_edtOffset);
}

ChannelDetailsAdc4::~ChannelDetailsAdc4() {
    
    for (auto i : w)
        delete i.second;
    w.clear();
    ws.clear();
    wsCmdFunc.clear();

    while (!params.empty()) {
        delete params.back();
        params.pop_back();
    }
}

void ChannelDetailsAdc4::Create(CfgDlgAdc4* wnd, int x) {
    for (size_t i = 0; i < ws.size(); i++) {
        int y = 110 + int(i) * 40;
        ws[i]->Create(wnd, x, y, wsCmdFunc[i], L"", params[i]->w, params[i]->h);
    }
}

void ChannelDetailsAdc4::EnableAll(bool bEnable) {
    for (auto i : ws)
        i->Enable(bEnable);
}

// HWND ChannelDetailsAdc4::getHwnd() { return pWnd->getHwnd(); }
