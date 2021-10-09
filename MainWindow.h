#pragma once
#include "BaseWindow.h"
#include "Widget.h"




class MainWindow;
class LogSystem;
class LogDisplay;
class DaqSystem;
// class DeviceItemParams;
class DeviceParams;
//struct CardParamsADC4;
class DevMonitorDataBaseType;
void funcUpdateByDevMonitor(MainWindow* wnd, DevMonitorDataBaseType* data);

class StatusBox;

class MainWindow : public BaseWindow<MainWindow> {
public:
    MainWindow();
    virtual ~MainWindow();

    inline PCWSTR ClassName() const { return L"Sample Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) {

        case WM_PAINT:
            onPaint(wParam, lParam);
            return 0;
        case WM_SIZE:
            onSize(wParam, lParam);
            return 0;
        case WM_VSCROLL:
            onVScroll(wParam, lParam);
            return 0;
        case WM_COMMAND:
            onCommand(wParam, lParam);
            return 0;
        case WM_CREATE:
            onCreate(wParam, lParam);
            return 0;
        case WM_DESTROY:
            onDestroy(wParam, lParam);
            PostQuitMessage(0);
            return 0;
        case WM_CLOSE:
            return onClose(wParam, lParam);
        }
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    BOOL Show(int nCmdShow = SW_SHOWNORMAL);


    int nCards; // NOTE!!!


protected:

    virtual void onCreate(WPARAM wParamm, LPARAM lParam);
    virtual void onPaint(WPARAM wParam, LPARAM lParam);
    virtual void onVScroll(WPARAM wParam, LPARAM lParam);
    virtual void onSize(WPARAM wParam, LPARAM lParam);
    virtual void onCommand(WPARAM wParam, LPARAM lParam);
    virtual LRESULT onClose(WPARAM wParam, LPARAM lParam);
    virtual void onDestroy(WPARAM wParam, LPARAM lParam);

    LONG chH, chW;
    //HWND hBtnTest;
    //HWND hBtnLogClear;
    
    // create
    //HWND hBtnDAQStart, hBtnDAQImport, hBtnDAQExport, hCkbLocal, hBtnProcLocalChoose, hBtnProcStart, hEdtProcIp, hEdtProcPort,
    // HWND  hBtnProcConnect;
    HWND hCkbRemote;
    HWND hSttInfoDAQ, hSttLog;
    std::vector<HWND> hBtnDAQCfg, hBtnDAQPlot;

    // scroll
    WORD clientW, clientH;



    void cmd_btnTest(WID id, int evt, LPARAM lParam);
    void cmd_btnLogClear(WID id, int evt, LPARAM lParam);
    void cmd_btnDaqStart(WID id, int evt, LPARAM lParam);
    void cmd_btnDaqImport(WID id, int evt, LPARAM lParams);
    void cmd_btnDaqExport(WID id, int evt, LPARAM lParams);
    void cmd_ckbLocal(WID id, int evt, LPARAM lParam);
    void cmd_btnProcLocalChoose(WID id, int evt, LPARAM lParam);
    void cmd_btnProcStart(WID id, int evt, LPARAM lParam);
    void cmd_edtProcIp(WID id, int evt, LPARAM lParam);
    void cmd_edtProcPort(WID id, int evt, LPARAM lParam);
    void cmd_btnProcConnect(WID id, int evt, LPARAM lParam);
    
    void cmd_btnDaqCfg(WID id, int evt, LPARAM lParam);
    void cmd_btnDaqPlot(WID id, int evt, LPARAM lParam);


    Widget<MainWindow, btn_t> btnTest;
    Widget<MainWindow, btn_t> btnLogClear;

    Widget<MainWindow, btn_t> btnDaqStart;
    Widget<MainWindow, btn_t> btnDaqImport;
    Widget<MainWindow, btn_t> btnDaqExport;

    Widget<MainWindow, ckb_t> ckbLocal;
    Widget<MainWindow, btn_t> btnProcLocalChoose;
    Widget<MainWindow, btn_t> btnProcStart;
    Widget<MainWindow, edt_t> edtProcIp;
    Widget<MainWindow, edt_t> edtProcPort;
    Widget<MainWindow, btn_t> btnProcConnect;

    std::vector<Widget<MainWindow, btn_t>*> btnDaqCfg;
    std::vector<Widget<MainWindow, btn_t>*> btnDaqPlot;

    StatusBox* statusBoxDevices;
    StatusBox* statusBoxNetwork;



private:

    void initializeDaqSystem();
    void finalizeDaqSystem();

    void initializeStatusBox();
    void finalizeStatusBox();

    friend void funcUpdateByDevMonitor(MainWindow* wnd, DevMonitorDataBaseType* data);

    LogSystem* logSystem;
    LogDisplay* logDisplay;
    DaqSystem* daq;

    DeviceParams* deviceParams;
};







