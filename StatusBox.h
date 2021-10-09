#pragma once

#include <Windows.h>

enum StatusValue {
    STATUS_READY,
    STATUS_RUNNING,
    STATUS_ERROR,
    STATUS_NOTHING,
    STATUS_UNAVAILABLE,
    STATUS_SIZE
};

class StatusBox {
public:
	StatusBox() : x(0), y(0), w(100), h(40), str(L""), status(STATUS_NOTHING) {}
	virtual ~StatusBox() {}
    void create(int x, int y, int w, int h);
    void setStatus(const wchar_t* str, StatusValue status);
    void paint(HDC hdc);
    void repaint(HWND hwnd);
private:
    int x;
    int y;
    int w;
    int h;
    wchar_t str[64];
    StatusValue status;
};