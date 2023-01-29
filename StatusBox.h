#pragma once

#include <Windows.h>

enum class DaqStatusType {
    READY,
    RUNNING,
    ERR,
    NOTHING,
    UNAVAILABLE,
    SIZE
};

class StatusBoxBackgroundBrush {
public:
    StatusBoxBackgroundBrush();
    virtual ~StatusBoxBackgroundBrush();
    HBRUSH operator [] (const DaqStatusType status) const {
        return hStatusBrush[static_cast<int>(status)];
    }
private:
    HBRUSH hStatusBrush[static_cast<int>(DaqStatusType::SIZE)];
};

class StatusBox {
public:
    StatusBox() : x(0), y(0), w(100), h(40), str(L""), status(DaqStatusType::NOTHING) {}
    virtual ~StatusBox() {}
    void create(int x, int y, int w, int h);
    void setStatus(const wchar_t* str, DaqStatusType status);
    void paint(HDC hdc);
    void repaint(HWND hwnd);
private:
    int x;
    int y;
    int w;
    int h;
    wchar_t str[64];
    static StatusBoxBackgroundBrush hStatusBrush;
    DaqStatusType status;
};