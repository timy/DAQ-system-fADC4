#pragma once

#include <Windows.h>
#include <iostream>

template <typename ParamsType, typename DlgType>
bool ShowModalDialog(HWND hParent, ParamsType* params, const wchar_t title[], unsigned int w, unsigned int h) {

    EnableWindow(hParent, FALSE);

    DlgType dlg(params);
    if (!dlg.Create(title, WS_CAPTION | WS_VISIBLE, 0,
        CW_USEDEFAULT, CW_USEDEFAULT, w, h, hParent)) {
        std::cout << "[ERR] Fail to create Dialog: " << title << "\n";
        return false;
    }
    dlg.Show();

    // Run the message loop.
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    EnableWindow(hParent, TRUE);
    SetForegroundWindow(hParent);

    return dlg.isApply();
}