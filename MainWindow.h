#pragma once
#include "BaseWindow.h"
#include "Widget.h"

class LogSystem;
class LogDisplay;
class DaqSystem;
// class CardParamsBase;
class DeviceParamsAdc4;
//struct CardParamsADC4;
class DaqStatusBaseType;
class DataVisualizer;
class Timer;

class StatusBox;

class MainWindow : public BaseWindow<MainWindow> {
public:
    MainWindow();
    virtual ~MainWindow();

    inline PCWSTR ClassName() const { return L"Sample Window Class"; }
    BOOL Show(int nCmdShow = SW_SHOWNORMAL);

    unsigned int nCards; // NOTE!!!

protected:

    virtual void onCreate(WPARAM wParamm, LPARAM lParam) override;
    virtual void onPaint(WPARAM wParam, LPARAM lParam) override;
    virtual void onVScroll(WPARAM wParam, LPARAM lParam) override;
    virtual void onSize(WPARAM wParam, LPARAM lParam) override;
    virtual LRESULT onClose(WPARAM wParam, LPARAM lParam) override;
    virtual void onDestroy(WPARAM wParam, LPARAM lParam) override;

    LONG chH, chW;

    HWND hCkbRemote;
    HWND hSttInfoDAQ, hSttLog;
    std::vector<HWND> hBtnDAQCfg, hBtnDAQPlot;

    // scroll
    WORD clientW, clientH;



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
    
    void cmd_btnDaqCfgGlobal(WID id, int evt, LPARAM lParam);
    void cmd_btnDaqCfgCards(WID id, int evt, LPARAM lParam);
    void cmd_btnDaqPlot(WID id, int evt, LPARAM lParam);


    Widget<MainWindow, btn_t> btnLogClear;

    Widget<MainWindow, btn_t> btnDaqStart;
    Widget<MainWindow, btn_t> btnDaqPlot;
    Widget<MainWindow, btn_t> btnDaqImport;
    Widget<MainWindow, btn_t> btnDaqExport;

    Widget<MainWindow, ckb_t> ckbLocal;
    Widget<MainWindow, btn_t> btnProcLocalChoose;
    Widget<MainWindow, btn_t> btnProcStart;
    Widget<MainWindow, edt_t> edtProcIp;
    Widget<MainWindow, edt_t> edtProcPort;
    Widget<MainWindow, btn_t> btnProcConnect;

    Widget<MainWindow, btn_t> btnDaqCfgGlobal;
    std::vector<Widget<MainWindow, btn_t>*> btnDaqCfgCards;

    StatusBox* statusBoxDevices;
    StatusBox* statusBoxNetwork;



private:

    void initializeDaqSystem(); // 原则上应为 virtual。未来，应在具体设备的继承类中重载
    void finalizeDaqSystem();

    void initializeStatusBox();
    void finalizeStatusBox();

    void initializeVisualizer();
    void finalizeVisualizer();

    void updateDaqStatusByTimer(DaqStatusBaseType* data);

    LogSystem* logSystem;
    LogDisplay* logDisplay;
    DaqSystem* daq;

    // 原则上，对于不同的具体设备，应设计一个新类，从 MainWindow 中继承，传入具体的 DeviceParams 。可以考虑 pimpl 设计
    DeviceParamsAdc4* deviceParams; // 未来，应声明为 DeviceParamsBase*，在 initializeDaqSystem 中赋 DevcieParamsAdc4 的实例
    DataVisualizer* vis;
    Timer* timer;
};







