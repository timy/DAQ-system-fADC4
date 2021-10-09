//#include <Windows.h>


#include "MainWindow.h"

#include "LogSystem.h"
#include "StatusBox.h"
//#include "LogDisplay.h"
#include "DaqSystem.h"
#include "Device/ADC4/CfgDlgAdc4.h"
#include "Device/ADC4/DeviceAdc4Params.h"
#include "Device/ADC4/DevMonitorDataAdc4Type.h"

#include <ShlObj_core.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "C:\\Program Files\\cronologic\\Ndigo\\driver\\x64\\ndigo_driver_64.lib")
#pragma comment(lib, "C:\\Program Files\\cronologic\\Ndigo\\driver\\x64\\crono_tools_64.lib")


// void showStatusBox(HDC hdc, int x, int y, COLORREF rgb, const wchar_t* str, int w = 100, int h = 40);
bool chooseFilePath(wchar_t* pathName, size_t size);
bool chooseDirectory(wchar_t* pathName);

// MainWindow 原则上通用于任何采集卡，无需知道特定采集卡（如 ADC4）的具体细节。
// 它提供了通用功能，包括通用入口按钮（config，start/stop, plot），通用状态显示，
// 通用业务逻辑（启用，停止，显示，配置界面，导入导出等）
// 
// DAQSystem 是硬件设备基类，提供了通用的设备控制与查询接口，包括启动、停止、配置、
// 询问当前设备状态等。其核心包含 DataProducer 与 DataSender
// 其中，关联于 DataProducer 中的 BaseDevice 为被操作的硬件基类，应当提供操作接口，
// 如启动、停止等，需由特定设备所继承，如 DeviceAdc4
// 
// DeviceItemParams 是硬件参数结构体基类，提供了赋值 “=” 的接口，需由特定设备所继承
// 并实现，如 DeviceADC4Params，但需要注意，DeviceItemParams 仅代表一个设备（板卡）
// 
// 因此，对于 DeviceItemParams 的集合，将由 DeviceParams 统一管理，
// 如：设备是否 enabled，桥接 DeviceItemParams 与 DAQSystem 中的硬件设备，
// 桥接 DeviceItemParams 与 配置文件系统 SimpleConfig，桥接 DeviceItemParams
// 与配置界面 CfgDlg，以及为 MainWindow 主界面提供状态信息
// 

// NOTE!!!! DeviceTypeParams may be extended to other types of devices. 
// If so, one should provide customerized DeviceTypeParams class, 
// which specifies a different set of parameters from what we see for ADC4 here.
// Moreover, the CfgDlg should also be replaced by the user-defined one, than add an
// extra function DisplayModalDialog for the different type as the 2nd argument. 
// Refer to CfgDlgAdc4 for detailed implementation.
using DeviceTypeParams = typename DeviceAdc4Params;

#include "SimpleConfig.h"

// DeviceParams 的管理类，针对（多个）DeviceItemParams，整合 CfgFileItems 导入导出变量、UI
class DeviceParams {
public:
    // typedef typename x y;
    DeviceParams(size_t nDevices_) : nDevices(nDevices_) {
        devices = new DeviceItemParams * [nDevices];
        for (int i = 0; i < nDevices; i++) {
            devices[i] = new DeviceTypeParams;
            devices[i]->id = i;
        }
        // 默认主触发通道为 card 0, channel 0
        // NOTE!!!! 考虑是否将 idxMasterChannel 属性置于 devices 的基类而不必强制转型为特定的继承类？
        // 对其他类型采集设备而言，idxMasterChannel 是否足够通用？其下的 postCheckValidity 也会用到
        static_cast<DeviceTypeParams*>(devices[0])->idxMasterChannel = 0; 
        
        cfi = new CfgFileItems;
        // bind variables with SimpleConfig via the interface of DeviceItemParams
        for (int i = 0; i < nDevices; i++) {
            devices[i]->attachCfgFileItem(cfi); 
        }
    }
    ~DeviceParams() {
        delete cfi;
        for (int i = 0; i < nDevices; i++) {
            delete devices[i];
        }
        delete[] devices;
    }
    
    // 提供设备卡 i 的指针
    DeviceItemParams* operator [] (size_t i) {
        return devices[i];
    }

    virtual void importFromFile(std::ifstream& file) {
        cfi->read(file);
        std::cout << "Configuration parameters were loaded from file" << std::endl << std::endl;
    }

    virtual void exportToFile(std::ofstream& file) {
        cfi->write(file);
        std::cout << "Configuration parameters were written to file" << std::endl << std::endl;
    }

    // 在对单个板卡配置完成 apply 后，检查全局（所有板卡）参数配置合理性
    void postCheckValidity(int i) {

        // 如果当前板卡 i 被设置了主 trigger，则检查其他板卡是否也设置了主 trigger
        // 如果是，则取消其他板卡的 trigger 设置
        if (static_cast<DeviceTypeParams*>(devices[i])->idxMasterChannel > -1) {
            for (int j = 0; j < nDevices; j++) {
                if (j != i) {
                    if (static_cast<DeviceTypeParams*>(devices[j])->idxMasterChannel > -1) {
                        std::cout << "[INFO] Change master trigger from card " 
                            << j
                            << " channel " << static_cast<DeviceTypeParams*>(devices[j])->idxMasterChannel 
                            << " to card " 
                            << i 
                            << " channel " << static_cast<DeviceTypeParams*>(devices[i])->idxMasterChannel 
                            << std::endl;
                        static_cast<DeviceTypeParams*>(devices[j])->idxMasterChannel = -1;
                    }
                }
            }
        }
    }

    DeviceItemParams** devices;
    size_t nDevices;

private:
    CfgFileItems* cfi;

};


MainWindow::MainWindow() {
    logSystem = new LogSystem;
    logDisplay = logSystem->getDisplay();
}

MainWindow::~MainWindow() {
    delete logSystem;
}


BOOL MainWindow::Show(int nCmdShow) {
    return ShowWindow(m_hwnd, nCmdShow) && UpdateWindow(m_hwnd);
}

void MainWindow::initializeDaqSystem() {
    daq = new DaqSystem;
    // 根据硬件采集卡数量，产生各采集卡的配置按钮
    nCards = daq->getNumberOfDevices();
    if (nCards > 0) {
        deviceParams = new DeviceParams(nCards);
        std::cout << "MainWindow::initializeDaqSystem: generating " << nCards << " deviceParams" << std::endl;
    }
    daq->setDevMonitor(this, funcUpdateByDevMonitor);
}

void MainWindow::finalizeDaqSystem() {
    // finalize the deviceParams
    if (nCards > 0) {
        delete deviceParams;
    }
    // finalize the DAQ
    delete daq;
}

void MainWindow::initializeStatusBox() {
    statusBoxDevices = new StatusBox[nCards];
    for (int i = 0; i < nCards; i++) {
        statusBoxDevices[i].create(
            btnDaqCfg[i]->x - 105,
            btnDaqCfg[i]->y - btnDaqCfg[i]->h / 2,
            70, 40);
        //statusBoxDevices[i].setStatus(L"enabled", STATUS_READY);
        //statusBoxDevices[i].repaint(m_hwnd);
    }

    statusBoxNetwork = new StatusBox;
    statusBoxNetwork->create(330, 170, 100, 40);
    statusBoxNetwork->setStatus(L"disconnected", STATUS_UNAVAILABLE);
}

void MainWindow::finalizeStatusBox() {
    delete statusBoxNetwork;
    delete[] statusBoxDevices;
}

void MainWindow::onCreate(WPARAM wParamm, LPARAM lParam) {

    HDC hdc = GetDC(m_hwnd);
    // 获取系统字体尺寸
    TEXTMETRIC textMetric;
    GetTextMetrics(hdc, &textMetric);
    chH = textMetric.tmHeight + textMetric.tmExternalLeading;
    chW = LONG((textMetric.tmPitchAndFamily & 1 ? 3 : 2) * (textMetric.tmAveCharWidth / 2.));
    ReleaseDC(m_hwnd, hdc);

    // 创建各类命令控件（处理消息 WM_COMMAND)
    btnTest.Create(this, 390, 520, &MainWindow::cmd_btnTest, L"TEST");
    btnLogClear.Create(this, 530, 520, &MainWindow::cmd_btnLogClear, L"Clear");
    btnDaqStart.Create(this, 220, 570, &MainWindow::cmd_btnDaqStart, L"Start");
    btnDaqImport.Create(this, 90, 510, &MainWindow::cmd_btnDaqImport, L"Import");
    btnDaqExport.Create(this, 220, 510, &MainWindow::cmd_btnDaqExport, L"Export");

    ckbLocal.Create(this, 360, 280, &MainWindow::cmd_ckbLocal, L"Save", 60, 30);
    btnProcLocalChoose.Create(this, 520, 285, &MainWindow::cmd_btnProcLocalChoose, L"Choose...");
    btnProcStart.Create(this, 470, 570, &MainWindow::cmd_btnProcStart, L"Start/Stop");
    // 远程数据传输 - 编辑框 - IP地址
    edtProcIp.Create(this, 490, 120, &MainWindow::cmd_edtProcIp, L"", 150, 20);
    // 远程数据传输 - 编辑框 - 端口
    edtProcPort.Create(this, 490, 150, &MainWindow::cmd_edtProcPort, L"", 150, 20);
    // 远程数据传输 - 按钮 - 连接/断开服务器
    btnProcConnect.Create(this, 520, 190, &MainWindow::cmd_btnProcConnect, L"Connect");

    SetWindowText(edtProcIp.hwnd, L"10.2.2.191");
    SetWindowText(edtProcPort.hwnd, L"80");

    // 创建日志显示区
    logDisplay->onCreate(m_hwnd, chH);

    // 初始化 DAQ 硬件
    initializeDaqSystem();


    for (int i = 0; i < nCards; i++) {
        int y = 100 + i * 50;
        btnDaqCfg.push_back(new Widget<MainWindow, btn_t>);
        btnDaqCfg.back()->Create(this, 185, y, &MainWindow::cmd_btnDaqCfg, L"Config", 55, 40);
    }

    for (int i = 0; i < nCards; i++) {
        int y = 100 + i * 50;
        btnDaqPlot.push_back(new Widget<MainWindow, btn_t>);
        btnDaqPlot.back()->Create(this, 250, y, &MainWindow::cmd_btnDaqPlot, L"Plot", 55, 40);
    }

    initializeStatusBox();
    

}

void MainWindow::onPaint(WPARAM wParam, LPARAM lParam) {
    static PAINTSTRUCT pc;
    HDC hdc;
    hdc = BeginPaint(m_hwnd, &pc);

    // 主 panel
    Rectangle(hdc, 10, 30, 300, 600);
    Rectangle(hdc, 310, 30, 600, 600);

    // panel 数据采集
    Rectangle(hdc, 20, 70, 290, 540);

    // panel 数据处理
    Rectangle(hdc, 320, 70, 590, 220); // “发送远程服务器”
    Rectangle(hdc, 320, 230, 590, 390); // “保存本地文件”

    wchar_t lbDAQ[] = L"Acquisition Hardware";
    TextOut(hdc, 20, 40, lbDAQ, (int)wcslen(lbDAQ));
    wchar_t lbProc[] = L"Data Processing";
    TextOut(hdc, 320, 40, lbProc, (int)wcslen(lbProc));
    wchar_t lbStat[] = L"System Log";
    TextOut(hdc, 620, 40, lbStat, (int)wcslen(lbStat));

    // Card i
    for (int i = 0; i < nCards; i++) {
        int x = 30;
        int y = 90 + i * 50;
        wchar_t text[64];
        wsprintf(text, L"Card %d", i);
        TextOut(hdc, x, y, text, (int)wcslen(text));
    }

    // 远程服务器
    wchar_t lbProcRemoteTitle[] = L"Send to Remote Server";
    TextOut(hdc, 330, 80, lbProcRemoteTitle, (int)wcslen(lbProcRemoteTitle));
    wchar_t lbProcRemoteIp[] = L"IP Address";
    TextOut(hdc, 330, 110, lbProcRemoteIp, (int)wcslen(lbProcRemoteIp));
    wchar_t lbProcRemotePort[] = L"Port";
    TextOut(hdc, 330, 140, lbProcRemotePort, (int)wcslen(lbProcRemotePort));

    // 本地数据保存
    wchar_t lbProcLocalTitle[] = L"Save to Local Directory";
    TextOut(hdc, 330, 240, lbProcLocalTitle, (int)wcslen(lbProcLocalTitle));

    // 设置状态
    //Rectangle(hdc, 330, 170, 420, 230);
    //wchar_t lbStatus[] = L"Status";
    //TextOut(hdc, 350, 175, lbStatus, (int)wcslen(lbStatus));

    //hbrush = CreateSolidBrush(RGB(0, 255, 0));
    //SetRect(&rect, 341, 196, 409, 219);
    //Rectangle(hdc, rect.left-1, rect.top-1, rect.right+1, rect.bottom+1);
    //FillRect(hdc, &rect, hbrush);

    //showStatusBox(hdc, 330, 170, RGB(200, 200, 200), L"disconnected");

    //// draw status of cards on the LHS of "Config" buttons of acquisition cards
    //for (int i = 0; i < nCards; i++) {
    //    if ((*deviceParams)[i]->isCardEnabled)
    //        showStatusBox(hdc, 
    //            btnDaqCfg[i]->x - 105, 
    //            btnDaqCfg[i]->y - btnDaqCfg[i]->h / 2, 
    //            RGB(0, 255, 0), L"enabled", 70);
    //    else
    //        showStatusBox(hdc, 
    //            btnDaqCfg[i]->x - 105, 
    //            btnDaqCfg[i]->y - btnDaqCfg[i]->h / 2,
    //            RGB(200, 200, 200), L"disabled", 70);
    //}

    for (int i = 0; i < nCards; i++)
        statusBoxDevices[i].paint(hdc);
    statusBoxNetwork->paint(hdc);

    logDisplay->onPaint(hdc);
    EndPaint(m_hwnd, &pc);
}

void MainWindow::onVScroll(WPARAM wParam, LPARAM lParam) {
    logDisplay->onVScroll(wParam, lParam);
}

void MainWindow::onSize(WPARAM wParam, LPARAM lParam) {
    clientH = HIWORD(lParam);
    clientW = LOWORD(lParam);
    logDisplay->onSize(clientH, clientW);
}

void MainWindow::onCommand(WPARAM wParam, LPARAM lParam) {
    int id = LOWORD(wParam), evt = HIWORD(wParam);
    cmdProcess((WID)id, evt, lParam); // NOTE!!! require check!
}

LRESULT MainWindow::onClose(WPARAM wParam, LPARAM lParam) {
    if (MessageBoxEx(m_hwnd, L"Confirm to quit?", L"", MB_YESNO,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)) == IDYES) {
        DestroyWindow(m_hwnd);
    }
    return 0;
}

void MainWindow::onDestroy(WPARAM wParam, LPARAM lParam) {

    finalizeStatusBox();

    while (!btnDaqCfg.empty()) {
        delete btnDaqCfg.back();
        btnDaqCfg.pop_back();
    }
    while (!btnDaqPlot.empty()) {
        delete btnDaqPlot.back();
        btnDaqPlot.pop_back();
    }

    finalizeDaqSystem();
}



void MainWindow::cmd_btnTest(WID id, int evt, LPARAM lParam) {}

void MainWindow::cmd_btnLogClear(WID id, int evt, LPARAM lParam) {
    logDisplay->ClearLog();
}

void MainWindow::cmd_btnDaqStart(WID id, int evt, LPARAM lParam) {

    daq->toggleStart(deviceParams->devices);

    // toggle the text of the "start/stop" button, and disable all config buttons
    bool isRunning = daq->isRunning();    
    SetWindowText((HWND)lParam, isRunning ? L"Stop" : L"Start");
    for (auto btn : btnDaqCfg)
        btn->Enable(!isRunning);
    btnDaqImport.Enable(!isRunning);
    if (!isRunning) {
        for (int i = 0; i < nCards; i++) {
            if ((*deviceParams)[i]->isCardEnabled) {
                statusBoxDevices[i].setStatus(L"enabled", STATUS_READY);
                statusBoxDevices[i].repaint(m_hwnd);
            }
        }
    }
}

void MainWindow::cmd_btnDaqImport(WID id, int evt, LPARAM lParams) {
    wchar_t pathName[1024];
    if (chooseFilePath(pathName, sizeof(pathName))) {
        std::ifstream file(pathName);
        deviceParams->importFromFile(file);
        daq->setConfigRequired();
    }

    const wchar_t* str[2] = { L"disabled", L"enabled" };
    const StatusValue status[2] = { STATUS_UNAVAILABLE, STATUS_READY };
    for (int i = 0; i < nCards; i++) {
        int idx = (*deviceParams)[i]->isCardEnabled;
        statusBoxDevices[i].setStatus(str[idx], status[idx]);
        statusBoxDevices[i].repaint(m_hwnd);
    }
}

void MainWindow::cmd_btnDaqExport(WID id, int evt, LPARAM lParams) {
    wchar_t pathName[1024];
    if (chooseFilePath(pathName, sizeof(pathName))) {
        std::ofstream file(pathName);
        deviceParams->exportToFile(file);
    }
}

void MainWindow::cmd_ckbLocal(WID id, int evt, LPARAM lParam) {}
void MainWindow::cmd_btnProcLocalChoose(WID id, int evt, LPARAM lParam) {
    wchar_t pathName[1024];
    if (chooseDirectory(pathName)) {

    }
}
void MainWindow::cmd_btnProcStart(WID id, int evt, LPARAM lParam) {}
void MainWindow::cmd_edtProcIp(WID id, int evt, LPARAM lParam) {}
void MainWindow::cmd_edtProcPort(WID id, int evt, LPARAM lParam) {}
void MainWindow::cmd_btnProcConnect(WID id, int evt, LPARAM lParam) {}

void MainWindow::cmd_btnDaqCfg(WID id, int evt, LPARAM lParam) {
    int i = (int)(id - btnDaqCfg[0]->id); // card id
    // create a temp struct to hold params of card[i], which will be shown in the device config pannel
    DeviceTypeParams cardTmp = *static_cast<DeviceTypeParams*>((*deviceParams)[i]); // 向下类型转换
    // show the config pannel for user to start parameters configuration
    if (DisplayModalDialog(m_hwnd, (DeviceTypeParams*)&cardTmp)) {
        // if the "Apply" button is clicked, return bApply = true, then we
        // fill back card[i] with the updated params stored in the temp struct
        // 
        // NOTE!!!!! the master trigger need be checked
        *static_cast<DeviceTypeParams*>((*deviceParams)[i]) = cardTmp; // 向上类型转换
        deviceParams->postCheckValidity(i);
        // once the parameters are given, inform the Daq that configuration is required
        daq->setConfigRequired(); // NOTE!!!! 检查何时需要将 isRequired 设为 false

            // inform the main window to redraw card status

        //if ((*deviceParams)[i]->isCardEnabled)
        //    showStatusBox(hdc,
        //        btnDaqCfg[i]->x - 105,
        //        btnDaqCfg[i]->y - btnDaqCfg[i]->h / 2,
        //        RGB(0, 255, 0), L"enabled", 70);
        
        const wchar_t* str[2] = { L"disabled", L"enabled" };
        const StatusValue status[2] = { STATUS_UNAVAILABLE, STATUS_READY };
        int idx = (*deviceParams)[i]->isCardEnabled;
        statusBoxDevices[i].setStatus(str[idx], status[idx]);
        statusBoxDevices[i].repaint(m_hwnd);

        //RECT rectCardStat = {
        //    btnDaqCfg[i]->x - 105,
        //    btnDaqCfg[i]->y - btnDaqCfg[i]->h / 2,
        //    btnDaqCfg[i]->x - 105 + 70,
        //    btnDaqCfg[i]->y + btnDaqCfg[i]->h / 2 };
        //InvalidateRect(m_hwnd, &rectCardStat, TRUE);
    }
    // std::cout << "threshold: " << static_cast<DeviceTypeParams*>(card[i])->channel[0].threshold << std::endl;
}
void MainWindow::cmd_btnDaqPlot(WID id, int evt, LPARAM lParam) {
    int i = (int)(id - btnDaqPlot[0]->id);
}

/// <summary>
/// a friend function of MainWindow which is regularly called by a DevMonitor thread with a timer, 
/// showing the status of the device
/// </summary>
/// <param name="status">user-defined data structure containing status information of interest</param>
void funcUpdateByDevMonitor(MainWindow* wnd, DevMonitorDataBaseType* status) {

    static long countDataOld[2] = { 0, 0 };

    if (!wnd->daq->isRunning()) {
        for (int i = 0; i< _countof(countDataOld); i ++)
            countDataOld[i] = 0;
        return;
    }

    DevMonitorDataAdc4Type* st = static_cast<DevMonitorDataAdc4Type*>(status);
    std::cout << "cyclesTotal " << st->cyclesReadTotal
    	<< "; cyclesValid " << st->cyclesReadValid
    	<< "; packets=" << st->countPacketsTotal
    	<< "; data=" << st->countDataTotal << std::endl;

    
    wchar_t str[64];
    for (int i = 0; i < wnd->nCards; i++) {
        if ((*wnd->deviceParams)[i]->isCardEnabled) {
            wsprintf(str, L"%ld /s", st->countData[i] - countDataOld[i]);
            wnd->statusBoxDevices[i].setStatus(str, STATUS_RUNNING);
            wnd->statusBoxDevices[i].repaint(wnd->m_hwnd);
        }
        countDataOld[i] = st->countData[i];
    }

    

    // NOTE!!!! when cyclesValid approaches cycleTotal, it indicates the PC may not be fast enough to process
    // the data transferred from the ADC card due to, for instance, the IO bottleneck, i.e., the PC has consumed 
    // too much time saving data to disk.
}



void showStatusBox(HDC hdc, int x, int y, COLORREF rgb, const wchar_t* str, int w, int h) {

    HBRUSH hbrush;
    RECT rect;

    Rectangle(hdc, x, y, x + w, y + h);
    hbrush = CreateSolidBrush(rgb);
    SetRect(&rect, x + 5, y + 5, x + w - 5, y + h - 5);
    Rectangle(hdc, rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1);
    FillRect(hdc, &rect, hbrush);
    TextOut(hdc, x + 7, y + 12, str, (int)wcslen(str));
}

bool chooseFilePath(wchar_t* pathName, size_t size) {
    ZeroMemory(pathName, size);
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetForegroundWindow();
    ofn.lpstrFilter = L"Text Files\0*.ini\0";
    ofn.lpstrFile = pathName;
    ofn.nMaxFile = (DWORD)size;
    ofn.lpstrTitle = L"Choose file";
    return GetOpenFileName(&ofn);
}

bool chooseDirectory(wchar_t* pathName) {

    BROWSEINFO bi = { 0 };
    bi.hwndOwner = GetForegroundWindow();
    bi.pidlRoot = NULL;
    bi.pszDisplayName = pathName;
    bi.lpszTitle = L"Choose directory";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_UAHINT;
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.iImage = 0;
    
    LPITEMIDLIST pidl;
    pidl = SHBrowseForFolder(&bi);
    return SHGetPathFromIDList(pidl, pathName);
}
