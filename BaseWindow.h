#pragma once

#include "AbstractWindow.h"
#include <vector>

template <class DERIVED_TYPE>
class BaseWindow : public AbstractWindow<DERIVED_TYPE> {
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        DERIVED_TYPE* pThis = NULL;

        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = reinterpret_cast<DERIVED_TYPE*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
            pThis->m_hwnd = hwnd;
        }
        else {
            pThis = reinterpret_cast<DERIVED_TYPE*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }
        if (pThis) {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    BaseWindow() : m_hwnd(NULL) {}
    virtual ~BaseWindow() {}

    virtual BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    ) {
        WNDCLASS wc = { 0 };

        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.lpszClassName = this->ClassName();
        wc.lpfnWndProc = DERIVED_TYPE::WindowProc; // key to the msg loop
        wc.hInstance = GetModuleHandle(NULL); //
        wc.style = CS_VREDRAW | CS_HREDRAW;
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName = NULL; // menu

        RegisterClass(&wc);

        m_hwnd = CreateWindowEx(
            dwExStyle, this->ClassName(), lpWindowName, dwStyle, x, y,
            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this);

        return (m_hwnd ? TRUE : FALSE);
    }

    HWND getHwnd() { return m_hwnd; }

protected:

    virtual PCWSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
};