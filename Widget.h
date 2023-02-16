#pragma once

#include <Windows.h>
#include "typedef.h"

class AbstractWidget {
public:
    AbstractWidget() { id = counter++; } // automatically assigned unique id for each widget
    virtual ~AbstractWidget() {}

    WID id; // id used when invoking SendMessage
private:
    static WID counter; // counter for id
};


template <typename WindowType>
class BaseWidget : public AbstractWidget {
public:
    BaseWidget() : AbstractWidget() {}
    virtual ~BaseWidget() {}
    
    // the prototype of process function defined in derived window class
    typedef void (WindowType::* CmdFunc)(WID, int, LPARAM);
    virtual void Create(WindowType* wnd, unsigned int x, unsigned int y, 
        CmdFunc func, 
        LPCWSTR text = L"", unsigned int w = 120, unsigned int h = 40) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        wnd->bind(id, func);
    }
    virtual void Enable(bool bEnable) {
        if (hwnd != NULL)
            EnableWindow(hwnd, bEnable);
    }
    HWND hwnd;
    unsigned int x, y, w, h;
};

template <typename WindowType, typename T>
class Widget : public BaseWidget<WindowType> {
public:
    Widget() {}
    virtual ~Widget() {}
    virtual void Create(WindowType* wnd, unsigned int x, unsigned int y, 
        typename BaseWidget<WindowType>::CmdFunc func,
        LPCWSTR text = L"", unsigned int w = 120, unsigned int h = 40) {
        BaseWidget<WindowType>::Create(wnd, x, y, nullptr, text, w, h);
    }
};

struct btn_t {}; // button
struct rdb_t {}; // radiobox
struct ckb_t {}; // checkbox
struct edt_t {}; // edit
struct edt_int_t {}; // edit only for integer
struct cbb_t {}; // combobox
struct stt_t {}; // static
struct nil_t {}; // null

template <typename WindowType>
class Widget<WindowType, btn_t> : public BaseWidget<WindowType> { // Button
public:
    virtual void Create(WindowType* wnd, unsigned int x, unsigned int y, 
        typename BaseWidget<WindowType>::CmdFunc func,
        LPCWSTR text = L"", unsigned int w = 100, unsigned int h = 20) {        
        this->hwnd = CreateWindow(L"button", text, 
            WS_VISIBLE | WS_CHILD,
            x - w / 2, y - h / 2, w, h, wnd->getHwnd(), (HMENU)this->id, 
            GetModuleHandle(NULL), NULL);
        BaseWidget<WindowType>::Create(wnd, x, y, func, text, w, h);
    }
};

template <typename WindowType>
class Widget<WindowType, stt_t> : public BaseWidget<WindowType> { // Static
public:
    virtual void Create(WindowType* wnd, unsigned int x, unsigned int y, 
        typename BaseWidget<WindowType>::CmdFunc func,
        LPCWSTR text = L"", unsigned int w = 120, unsigned int h = 20) {
        this->hwnd = CreateWindow(L"static", text, 
            WS_VISIBLE | WS_CHILD,
            x - w / 2, y - h / 2, w, h, wnd->getHwnd(), (HMENU)this->id, 
            GetModuleHandle(NULL), NULL);
        BaseWidget<WindowType>::Create(wnd, x, y, func, text, w, h);
    }
};

template <typename WindowType>
class Widget<WindowType, rdb_t> : public BaseWidget<WindowType> { // Radiobutton
public:
    virtual void Create(WindowType* wnd, unsigned int x, unsigned int y, 
        typename BaseWidget<WindowType>::CmdFunc func,
        LPCWSTR text = L"", unsigned int w = 12, unsigned int h = 12) {
        this->hwnd = CreateWindow(L"button", text, 
            WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
            x - w / 2, y - h / 2, w, h, wnd->getHwnd(), (HMENU)this->id, 
            GetModuleHandle(NULL), NULL);
        BaseWidget<WindowType>::Create(wnd, x, y, func, text, w, h);
    }
};

template <typename WindowType>
class Widget<WindowType, edt_t> : public BaseWidget<WindowType> { // Edit
public:
    virtual void Create(WindowType* wnd, unsigned int x, unsigned int y, 
        typename BaseWidget<WindowType>::CmdFunc func,
        LPCWSTR text = L"", unsigned int w = 50, unsigned int h = 20) {
        this->hwnd = CreateWindow(L"edit", text, 
            WS_VISIBLE | WS_CHILD | WS_BORDER, // | ES_NUMBER,
            x - w / 2, y - h / 2, w, h, wnd->getHwnd(), (HMENU)this->id, 
            GetModuleHandle(NULL), NULL);
        BaseWidget<WindowType>::Create(wnd, x, y, func, text, w, h);
    }
};

template <typename WindowType>
class Widget<WindowType, edt_int_t> : public BaseWidget<WindowType> { // Edit
public:
    virtual void Create(WindowType* wnd, unsigned int x, unsigned int y, 
        typename BaseWidget<WindowType>::CmdFunc func,
        LPCWSTR text = L"", unsigned int w = 50, unsigned int h = 20) {
        this->hwnd = CreateWindow(L"edit", text, 
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
            x - w / 2, y - h / 2, w, h, wnd->getHwnd(), (HMENU)this->id, 
            GetModuleHandle(NULL), NULL);
        BaseWidget<WindowType>::Create(wnd, x, y, func, text, w, h);
    }
};

template <typename WindowType>
class Widget<WindowType, ckb_t> : public BaseWidget<WindowType> { // Checkbox
public:
    virtual void Create(WindowType* wnd, unsigned int x, unsigned int y, 
        typename BaseWidget<WindowType>::CmdFunc func,
        LPCWSTR text = L"", unsigned int w = 12, unsigned int h = 20) {
        this->hwnd = CreateWindow(L"button", text, 
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            x - w / 2, y - h / 2, w, h, wnd->getHwnd(), (HMENU)this->id, 
            GetModuleHandle(NULL), NULL);
        BaseWidget<WindowType>::Create(wnd, x, y, func, text, w, h);
    }
};

template <typename WindowType>
class Widget<WindowType, cbb_t> : public BaseWidget<WindowType> { // Combo box
public:
    virtual void Create(WindowType* wnd, unsigned int x, unsigned int y, 
        typename BaseWidget<WindowType>::CmdFunc func,
        LPCWSTR text = L"", unsigned int w = 60, unsigned int h = 168) {
        this->hwnd = CreateWindow(L"combobox", text, 
            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
            x - w / 2, y, w, h, wnd->getHwnd(), (HMENU)this->id, 
            GetModuleHandle(NULL), NULL);
        BaseWidget<WindowType>::Create(wnd, x, y, func, text, w, h);
    }
};

template <typename WindowType>
class Widget<WindowType, nil_t> : public BaseWidget<WindowType> { // Null
public:
    virtual void Create(WindowType* wnd, unsigned int x = 0, unsigned int y = 0,
        typename BaseWidget<WindowType>::CmdFunc func=nullptr,
        LPCWSTR text = L"", unsigned int w = 0, unsigned int h = 0) {
        this->hwnd = 0;
        BaseWidget<WindowType>::Create(wnd, x, y, func, text, w, h);
    }
};