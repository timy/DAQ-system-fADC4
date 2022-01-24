#pragma once

#include <Windows.h>
#include "BaseWindow.h"
#include "Widget.h"

struct DataVisualizerData;
struct ID2D1Factory;
struct ID2D1HwndRenderTarget;
struct ID2D1SolidColorBrush;
struct IDWriteFactory;
struct IDWriteTextFormat;

class Timer;
class DaqStatusBaseType;

class DataVisualizerWindow : public BaseWindow<DataVisualizerWindow> {
public:
    DataVisualizerWindow(DaqStatusBaseType* src_);
    ~DataVisualizerWindow();
    PCWSTR ClassName() const { return L"Modal Dialog Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    BOOL Show(int nCmdShow = SW_SHOW);
    virtual BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0);

protected:

    virtual LRESULT onClose(WPARAM wParam, LPARAM lParam);
    virtual void onPaint(WPARAM wParam, LPARAM lParam);

    void cmd_sttCanvas(WID id, int evt, LPARAM lParam);
    void cmd_ckbDisplayChannels(WID id, int evt, LPARAM lParam);

    HWND hEdtTest;
    HWND hBtnTest;

    std::vector<Widget<DataVisualizerWindow, ckb_t>*> ckbDisplayChannels;
    Widget<DataVisualizerWindow, stt_t> sttCanvas;
    Widget<DataVisualizerWindow, btn_t> btnTest;
    
    virtual void onCreate(WPARAM wParamm, LPARAM lParam);
    virtual void onDestroy(WPARAM wParam, LPARAM lParam);
    virtual void onCommand(WPARAM wParam, LPARAM lParam);
    virtual void onSize(WPARAM wParam, LPARAM lParam);

    ID2D1Factory* m_pD2dFactory;
    ID2D1HwndRenderTarget* m_pRenderTarget;
    ID2D1SolidColorBrush* m_pBlackBrush;
    ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
    ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
    RECT rc;

    IDWriteFactory* m_pDWriteFactory;
    IDWriteTextFormat* m_pTextFormat;
private:
    HRESULT CreateDeviceIndependentResources();
    HRESULT CreateDeviceResources();
    void DiscardDeviceResources();

    // coordinates transform
    inline float xTransform(float x, float x0, float x1) {
        return X0 + (x - x0) / (x1 - x0) * (X1 - X0);
    }
    inline float yTransform(float y, float y0, float y1) {
        return (1.f - (y - y0) / (y1 - y0)) * (Y1 - Y0) + Y0;
    }

    void initialize();
    void finalize();
    void updatePlotByTimer();

    // coordinates of canvas region 
    float X0; // left
    float X1; // right
    float Y0; // top
    float Y1; // bottom

    // coordinates of canvas widget
    const int canvasX0 = 23; // left
    const int canvasX1 = 767; // right
    const int canvasY0 = 80; // top
    const int canvasY1 = 560; // bottom


    DaqStatusBaseType* src;
    DataVisualizerData* data; // NOTE!!!! requires assignment，其中的 x、y 应当与 DaqStatusAdc4Type 中预留的 nDataMax 相等
    size_t countChannels;
    wchar_t** labelChannels;
    bool* isChannelSelected;

    Timer* timer;

    struct MapIndexChannel {
        int card;
        int channel;
    } *mapIdxChannel;

    std::vector<MapIndexChannel> idxChannelSelected;

};
