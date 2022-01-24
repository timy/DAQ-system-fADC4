#include "DataVisualizer.h"
#include "DataVisualizerWindow.h"
#include "ThreadWrapper.h"
#include <functional> // std::bind

void DataVisualizer::initialize(HWND hParent_, DaqStatusBaseType* src) {
    hParent = hParent_;
    srcData = src;
}

void DataVisualizer::finalize() {
}

void DataVisualizer::start() {

    if (SUCCEEDED(CoInitialize(NULL))) { // initialize COM

        std::thread thrDataPlot(std::bind(&DataVisualizer::show, this));
        ThreadWrapper wrapThrDataPlot(thrDataPlot);

        CoUninitialize();
    }

}

void DataVisualizer::show() {
    EnableWindow(hParent, FALSE);
    DataVisualizerWindow dlg(srcData);
    
    if (!dlg.Create(L"Real-time data visualization",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, hParent)) {
        return;
    }
    dlg.Show();

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    EnableWindow(hParent, TRUE);
    SetForegroundWindow(hParent);
}