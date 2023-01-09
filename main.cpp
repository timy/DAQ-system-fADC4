#ifndef UNICODE
#define UNICODE
#endif 

#include "MainWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    MainWindow win;

    if (!win.Create(L"Ndigo fast-ADC 4/10 Field Control", 
        /*WS_OVERLAPPEDWINDOW */ WS_CAPTION | WS_VSCROLL | WS_CLIPCHILDREN, 0,
        CW_USEDEFAULT, CW_USEDEFAULT, 1150, 650)) {
        return 0;
    }

    win.Show(nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}