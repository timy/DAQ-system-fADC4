#pragma once

#include <Windows.h>

enum DaqStatusValue {
    DAQ_STATUS_READY,
    DAQ_STATUS_RUNNING,
    DAQ_STATUS_ERROR,
    DAQ_STATUS_NOTHING,
    DAQ_STATUS_UNAVAILABLE,
    DAQ_STATUS_SIZE
};

class StatusBox {
public:
	StatusBox() : x(0), y(0), w(100), h(40), str(L""), status(DAQ_STATUS_NOTHING) {}
	virtual ~StatusBox() {}
    void create(int x, int y, int w, int h);
    void setStatus(const wchar_t* str, DaqStatusValue status);
    void paint(HDC hdc);
    void repaint(HWND hwnd);
private:
    int x;
    int y;
    int w;
    int h;
    wchar_t str[64];
    DaqStatusValue status;
};