#include "StatusBox.h"

struct {
    DaqStatusType value;
    COLORREF rgb;
} listStatus[static_cast<int>(DaqStatusType::SIZE)] = {
    {DaqStatusType::READY, RGB(150, 200, 255)},         // blue
    {DaqStatusType::RUNNING, RGB(0, 255, 0)},           // green
    {DaqStatusType::ERR, RGB(255, 0, 0)},               // red
    {DaqStatusType::NOTHING, RGB(255, 255, 255)},       // white
    {DaqStatusType::UNAVAILABLE, RGB(200, 200, 200)},   // grey
};

StatusBoxBackgroundBrush::StatusBoxBackgroundBrush() {
    for (unsigned int i = 0; i < static_cast<int>(DaqStatusType::SIZE); i++)
        hStatusBrush[i] = CreateSolidBrush(listStatus[i].rgb);
}

StatusBoxBackgroundBrush::~StatusBoxBackgroundBrush() {
    for (unsigned int i = 0; i < static_cast<int>(DaqStatusType::SIZE); i++)
        DeleteObject(hStatusBrush[i]);
}

// the unique group of brushes with different colors, shared by all status boxes
StatusBoxBackgroundBrush StatusBox::hStatusBrush; 

void StatusBox::create(int x_, int y_, int w_, int h_) {
    x = x_;
    y = y_;
    w = w_;
    h = h_;
}

void StatusBox::setStatus(const wchar_t* str_, DaqStatusType status_) {
    status = status_;
    wcscpy_s(str, _countof(str), str_);
}

void StatusBox::paint(HDC hdc) {
    RECT rect;
    Rectangle(hdc, x, y, x + w, y + h);
    SetRect(&rect, x + 1, y + 1, x + w - 1, y + h - 1);
    //Rectangle(hdc, rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1);
    FillRect(hdc, &rect, hStatusBrush[status]);
    int oldBkMode = GetBkMode(hdc);
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, x + 1, y + 1, str, (int)wcslen(str));
    SetBkMode(hdc, oldBkMode);
}

void StatusBox::repaint(HWND hwnd) {
    RECT rect = { x, y, x + w, y + h };
    InvalidateRect(hwnd, &rect, TRUE);
}
