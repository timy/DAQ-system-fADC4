#pragma once

#include <Windows.h>

class EditValue {
public:
    EditValue(HWND hwnd, const wchar_t* err_msg = L"value error")
        : hwnd(hwnd), nBuffer(0), buffer(nullptr), str(nullptr) {
        size_t len = (size_t)SendMessage(hwnd, WM_GETTEXTLENGTH, 0, 0);
        if (len == 0) {
            MessageBox(NULL, err_msg, L"Input error", MB_OK);
            return;
        }
        nBuffer = len + 1; // + 1 for terminator '\0'        
        buffer = new wchar_t[nBuffer]();
        str = new char[nBuffer]();

        SendMessage(hwnd, WM_GETTEXT, (WPARAM)nBuffer, (LPARAM)buffer);

        size_t nConverted;
        errno_t err = wcstombs_s(&nConverted, str, nBuffer, buffer, nBuffer);
    }
    virtual ~EditValue() {
        delete[] str;
        delete[] buffer;
    }
    void get(unsigned short& val) {
        val = _wtoi(buffer);
    }
    void get(unsigned int& val) {
        val = _wtoi(buffer);
    }
    void get(int& val) {
        val = _wtoi(buffer);
    }
    void get(char* val) {
        strcpy_s(val, nBuffer, str);
    }
    void get(std::string& val) {
        val = str;
    }
    size_t size() const { return nBuffer; }

private:
    HWND hwnd;
    size_t nBuffer;
    wchar_t* buffer;
    char* str;
};