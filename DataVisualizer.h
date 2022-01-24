#pragma once

#include <Windows.h>

class DataVisualizer {
public:
    DataVisualizer() {}
    virtual ~DataVisualizer() {}

    virtual void initialize(HWND hParent_, class DaqStatusBaseType* srcData_);
    virtual void finalize();
    virtual void start();

private:

    void show();

    HWND hParent; // handle of parent window
    DaqStatusBaseType* srcData; // source of status data
};

