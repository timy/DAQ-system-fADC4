#include "StatusBox.h"

struct {
    DaqStatusValue value;
    COLORREF rgb;
} listStatus[DAQ_STATUS_SIZE] = {
    {DAQ_STATUS_READY, RGB(150, 200, 255)},         // blue
    {DAQ_STATUS_RUNNING, RGB(0, 255, 0)},           // green
    {DAQ_STATUS_ERROR, RGB(255, 0, 0)},             // red
    {DAQ_STATUS_NOTHING, RGB(255, 255, 255)},       // white
    {DAQ_STATUS_UNAVAILABLE, RGB(200, 200, 200)},   // grey
};

void StatusBox::create(int x_, int y_, int w_, int h_) {
    x = x_;
    y = y_;
    w = w_;
    h = h_;
}

void StatusBox::setStatus(const wchar_t* str_, DaqStatusValue status_) {
    status = status_;
    wcscpy_s(str, _countof(str), str_);
}

void StatusBox::paint(HDC hdc) {
    RECT rect;
    Rectangle(hdc, x, y, x + w, y + h);
    HBRUSH hbrush = CreateSolidBrush(listStatus[status].rgb);
    SetRect(&rect, x + 5, y + 5, x + w - 5, y + h - 5);
    Rectangle(hdc, rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1);
    FillRect(hdc, &rect, hbrush);
    TextOut(hdc, x + 7, y + 12, str, (int)wcslen(str));
}

void StatusBox::repaint(HWND hwnd) {
    RECT rect = { x, y, x + w, y + h };
    InvalidateRect(hwnd, &rect, TRUE);
}
