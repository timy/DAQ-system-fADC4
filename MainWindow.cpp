//#include <Windows.h>


#include "MainWindow.h"

#include "LogSystem.h"
#include "StatusBox.h"
#include "DaqSystem.h"
#include "Device/ADC4/DeviceParamsAdc4.h"
#include "Device/ADC4/DaqStatusAdc4Type.h"
#include "DataVisualizer.h"
#include "Timer.h"

#include <ShlObj_core.h>

#pragma comment(lib, "comctl32.lib")

#ifndef _SIMULATE
#pragma comment(lib, "C:\\Program Files\\cronologic\\Ndigo\\driver\\x64\\ndigo_driver_64.lib")
#pragma comment(lib, "C:\\Program Files\\cronologic\\Ndigo\\driver\\x64\\crono_tools_64.lib")
#endif

bool chooseFilePath(wchar_t* pathName, unsigned int size);
bool chooseDirectory(wchar_t* pathName);
inline void setLabel(HDC hdc, int x, int y, const wchar_t text[]) {
    TextOut(hdc, x, y, text, (int)wcslen(text));
}

// MainWindow 原则上通用于任何采集卡，无需知道特定采集卡（如 ADC4）的具体细节。
// 它提供了通用功能，包括通用入口按钮（config，start/stop, plot），通用状态显示，
// 通用业务逻辑（启用，停止，显示，配置界面，导入导出等）
// 
// DAQSystem 是硬件设备基类，提供了通用的设备控制与查询接口，包括启动、停止、配置、
// 询问当前设备状态等。其核心包含 DataProducer 与 DataSender
// 其中，关联于 DataProducer 中的 BaseDevice 为被操作的硬件基类，应当提供操作接口，
// 如启动、停止等，需由特定设备所继承，如 DeviceAdc4
// 
// CardParamsBase 是硬件参数结构体基类，提供了赋值 “=” 的接口，需由特定设备所继承
// 并实现，如 DeviceADC4Params，但需要注意，DeviceItemParams 仅代表一个设备（板卡）
// 
// 因此，对于 CardParamsBase 的集合，将由 DeviceParamsAdc4 统一管理，
// 如：设备是否 enabled，桥接 CardParamsBase 与 DAQSystem 中的硬件设备，
// 桥接 CardParamsBase 与 配置文件系统 SimpleConfig，桥接 CardParamsBase
// 与配置界面 CfgDlg，以及为 MainWindow 主界面提供状态信息
// 

// NOTE!!!! DeviceTypeParams may be extended to other types of devices. 
// If so, one should provide customerized DeviceTypeParams class, 
// which specifies a different set of parameters from what we see for ADC4 here.
// Moreover, the CfgDlg should also be replaced by the user-defined one, than add an
// extra function DisplayModalDialog for the different type as the 2nd argument. 
// Refer to CfgDlgAdc4 for detailed implementation.
//using DeviceTypeParams = typename CardParamsAdc4;

#include "DeviceParamsBase.h"

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
    nCards = daq->getNumberOfDevices();
    if (nCards > 0) {
        deviceParams = new DeviceParamsAdc4(nCards);
        std::cout << "MainWindow::initializeDaqSystem: generating " << nCards << " deviceParams" << std::endl;
    }
}

void MainWindow::finalizeDaqSystem() {
    // finalize deviceParams
    if (nCards > 0) {
        delete deviceParams;
    }
    nCards = 0;
    // finalize the DAQ
    delete daq;
}

void MainWindow::initializeStatusBox() {
    statusBoxDevices = new StatusBox[nCards];
    for (unsigned int i = 0; i < nCards; i++) {
        statusBoxDevices[i].create(
            btnDaqCfg[i]->x + 40,
            btnDaqCfg[i]->y - btnDaqCfg[i]->h / 2,
            180, 20);
        //statusBoxDevices[i].setStatus(L"enabled", DAQ_STATUS_READY);
        //statusBoxDevices[i].repaint(m_hwnd);
    }

    statusBoxNetwork = new StatusBox;
    statusBoxNetwork->create(btnProcConnect.x - 190, btnProcConnect.y - btnProcConnect.h / 2, 130, 20);
    statusBoxNetwork->setStatus(L"disconnected", DaqStatusType::UNAVAILABLE);

    timer = new Timer;
}

void MainWindow::finalizeStatusBox() {
    delete timer;
    delete statusBoxNetwork;
    delete[] statusBoxDevices;
}

void MainWindow::initializeVisualizer() {
    vis = new DataVisualizer;
    vis->initialize(m_hwnd, daq->getDeviceStatus()); 
}

void MainWindow::finalizeVisualizer() {
    vis->finalize();
    delete vis;
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
    btnDaqStart.Create(this, 220, 570, &MainWindow::cmd_btnDaqStart, L"Start");
    btnDaqPlot.Create(this, 90, 570, &MainWindow::cmd_btnDaqPlot, L"Plot");
    btnDaqPlot.Enable(false);
    btnDaqImport.Create(this, 90, 510, &MainWindow::cmd_btnDaqImport, L"Import");
    btnDaqExport.Create(this, 220, 510, &MainWindow::cmd_btnDaqExport, L"Export");

    btnProcLocalChoose.Create(this, 520, 280, &MainWindow::cmd_btnProcLocalChoose, L"Choose...");
    ckbLocal.Create(this, 360, btnProcLocalChoose.y, &MainWindow::cmd_ckbLocal, L"Save", 60);
    btnProcStart.Create(this, 530, 570, &MainWindow::cmd_btnProcStart, L"Start/Stop");
    btnLogClear.Create(this, 530, 520, &MainWindow::cmd_btnLogClear, L"Clear");
    // 远程数据传输 - 编辑框 - IP地址
    edtProcIp.Create(this, 490, 120, &MainWindow::cmd_edtProcIp, L"", 150, 20);
    // 远程数据传输 - 编辑框 - 端口
    edtProcPort.Create(this, 490, 150, &MainWindow::cmd_edtProcPort, L"", 150, 20);
    // 远程数据传输 - 按钮 - 连接/断开服务器
    btnProcConnect.Create(this, 520, 190, &MainWindow::cmd_btnProcConnect, L"Connect");

    SetWindowText(edtProcIp.hwnd, L"10.11.1.18");
    SetWindowText(edtProcPort.hwnd, L"8080");

    // 创建日志显示区
    logDisplay->onCreate(m_hwnd, chH);

    // 初始化 DAQ 硬件
    initializeDaqSystem();

    // DAQ 子卡配置按钮
    for (unsigned int i = 0; i < nCards; i++) {
        wchar_t label[64];
        wsprintf(label, L"Card %u", i);
        unsigned int y = 120 + i * 30;
        btnDaqCfg.push_back(new Widget<MainWindow, btn_t>);
        btnDaqCfg.back()->Create(this, 60, y, &MainWindow::cmd_btnDaqCfg, label, 60);
    }

    initializeStatusBox();
    initializeVisualizer();
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

    // 主类别
    setLabel(hdc, 20, 40, L"Acquistion Hardware");
    setLabel(hdc, 320, 40, L"Data Processing");

    // 采集卡
    setLabel(hdc, 40, 80, L"Cards");
    setLabel(hdc, 170, 80, L"Status");

    // 远程服务器
    setLabel(hdc, 330, 80, L"Send to Remote Server");
    setLabel(hdc, 330, 110, L"IP Address");
    setLabel(hdc, 330, 140, L"Port");

    // 本地数据保存
    setLabel(hdc, 330, 240, L"Save to Local Directory");

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
    //    if ((*deviceParams)[i]->isEnabled)
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

    for (unsigned int i = 0; i < nCards; i++)
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
    if (daq->isRunning()) {
        MessageBox(m_hwnd, L"Data acquisition is still running. Stop it before exit.", L"", MB_OK);
        return 0;
    }
    if (MessageBoxEx(m_hwnd, L"Confirm to quit?", L"", MB_YESNO,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)) == IDYES) {
        DestroyWindow(m_hwnd);
    }
    return 0;
}

void MainWindow::onDestroy(WPARAM wParam, LPARAM lParam) {

    finalizeVisualizer();
    finalizeStatusBox();

    while (!btnDaqCfg.empty()) {
        delete btnDaqCfg.back();
        btnDaqCfg.pop_back();
    }

    finalizeDaqSystem();
}

void MainWindow::cmd_btnLogClear(WID id, int evt, LPARAM lParam) {
    logDisplay->ClearLog();
}

void MainWindow::cmd_btnDaqStart(WID id, int evt, LPARAM lParam) {

    daq->toggleStartDataAcquisition(deviceParams);

    // toggle the text of the "start/stop" button, and disable all config buttons
    bool isRunning = daq->isRunning();
    SetWindowText((HWND)lParam, isRunning ? L"Stop" : L"Start");
    for (auto btn : btnDaqCfg)
        btn->Enable(!isRunning);
    btnDaqImport.Enable(!isRunning);
    btnDaqPlot.Enable(isRunning);
    if (isRunning) {
        // TODO
        timer->start(1000, std::bind(&MainWindow::updateDaqStatusByTimer, 
            this, daq->getDeviceStatus()));
    } else {
        timer->stop();
        for (unsigned int i = 0; i < nCards; i++) {
            if (deviceParams->cards[i]->isEnabled) {
                statusBoxDevices[i].setStatus(L"enabled", DaqStatusType::READY);
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
    const DaqStatusType status[2] = { DaqStatusType::UNAVAILABLE, DaqStatusType::READY };
    for (unsigned int i = 0; i < nCards; i++) {
        bool enabled = deviceParams->cards[i]->isEnabled;
        statusBoxDevices[i].setStatus(str[enabled], status[enabled]);
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

void MainWindow::cmd_btnDaqCfg(WID id, int evt, LPARAM lParam) {
    unsigned int i = (unsigned int)(id - btnDaqCfg[0]->id); // card id
    if (deviceParams->applyCardConfiguration(i, m_hwnd)) {
        // once parameters are set, notify the Daq that configuration is required
        daq->setConfigRequired(); // NOTE!!!! 检查何时需要将 isRequired 设为 false
        const wchar_t* str[2] = { L"disabled", L"enabled" };
        const DaqStatusType status[2] = { DaqStatusType::UNAVAILABLE, DaqStatusType::READY };
        bool enabled = deviceParams->cards[i]->isEnabled;
        statusBoxDevices[i].setStatus(str[enabled], status[enabled]);
        statusBoxDevices[i].repaint(m_hwnd);
    }
}

void MainWindow::cmd_btnDaqPlot(WID id, int evt, LPARAM lParam) {
    vis->start();
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



#include "EditValue.h" // EditValue

void MainWindow::cmd_btnProcConnect(WID id, int evt, LPARAM lParam) {

    EditValue ev_ip(edtProcIp.hwnd, L"IP address should be provided!");
    std::string ip;
    ev_ip.get(ip);

    EditValue ev_port(edtProcPort.hwnd, L"port should be provided!");
    unsigned short port;
    ev_port.get(port);

    daq->toggleStartRemoteConnection(ip, port);

    //std::thread thrDataSend(&DataSender::start, ds);
    //ThreadWrapper wrapThrDataSend(thrDataSend);
}



/// <summary>
/// regularly called by a timer, showing the status of the device
/// </summary>
/// <param name="status">user-defined data structure containing status information of interest</param>
void MainWindow::updateDaqStatusByTimer(DaqStatusBaseType* data) {

    static long countDataOld[2] = { 0, 0 };

    if (!daq->isRunning()) {
        for (unsigned int i = 0; i < _countof(countDataOld); i++)
            countDataOld[i] = 0;
        return;
    }

    DaqStatusAdc4Type* st = static_cast<DaqStatusAdc4Type*>(data);
    std::cout << "cyclesTotal " << st->cyclesReadTotal
        << "; cyclesValid " << st->cyclesReadValid
        << "; packets=" << st->countPacketsTotal
        << "; data=" << st->countDataTotal << std::endl;

    
    wchar_t str[64];
    for (unsigned int i = 0; i < nCards; i++) {
        if (deviceParams->cards[i]->isEnabled) {
            wsprintf(str, L"%ld /s", st->countData[i] - countDataOld[i]);
            statusBoxDevices[i].setStatus(str, DaqStatusType::RUNNING);
            statusBoxDevices[i].repaint(m_hwnd);
        }
        countDataOld[i] = st->countData[i];
    }
    
    // NOTE!!!! when cyclesValid approaches cycleTotal, it indicates the PC may not be fast enough to process
    // the data transferred from the ADC card due to, for instance, the IO bottleneck, i.e., the PC has consumed 
    // too much time saving data to disk.
}

bool chooseFilePath(wchar_t* pathName, unsigned int size) {
    ZeroMemory(pathName, size);
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetForegroundWindow();
    ofn.lpstrFilter = L"Text Files\0*.ini\0";
    ofn.lpstrFile = pathName;
    ofn.nMaxFile = size;
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
