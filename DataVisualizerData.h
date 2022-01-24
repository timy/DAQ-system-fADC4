#pragma once
#include <cstdio>

struct DataVisualizerData {

public:
    DataVisualizerData(int nDataMax_) : nDataMax(nDataMax_) {
        x = new float[nDataMax];
        y = new float[nDataMax];

        nTicksX = 11;
        nTicksY = 5;

        createAxes();

    }
    virtual ~DataVisualizerData() {

        deleteAxes();

        delete[] x;
        delete[] y;
    }

    void setupAxes() {
        // find min/max of x
        xMin = x[0];
        xMax = x[0];
        for (int i = 1; i < nData; i++) {
            if (x[i] > xMax)
                xMax = x[i];
            if (x[i] < xMin)
                xMin = x[i];
        }
        // set ticks of x-axis
        float dx = (xMax - xMin) / (nTicksX - 1);
        for (int i = 0; i < nTicksX; i++) {
            swprintf(ticksX[i], nTickChars, L"%10.1f", xMin + i * dx); // NOTE: the format may be modified to adapt to too large/small numbers
        }

        // find min/max of y
        yMin = y[0];
        yMax = y[0];
        for (int i = 1; i < nData; i++) {
            if (y[i] > yMax)
                yMax = y[i];
            if (y[i] < yMin)
                yMin = y[i];
        }
        // set ticks of y-axis
        float dy = (yMax - yMin) / (nTicksY - 1);
        for (int i = 0; i < nTicksY; i++) {
            swprintf(ticksY[i], nTickChars, L"%10.1f", yMin + i * dy);
        }
    }


    bool isReady;

    long nDataMax;
    long nData;
    float* x;
    float* y;

    float xMin;
    float xMax;
    float yMin;
    float yMax;
    int nTicksX;
    int nTicksY;
    wchar_t** ticksX;
    wchar_t** ticksY;
    const size_t nTickChars = 64;

private:

    void createAxes() {

        // allocation
        ticksX = new wchar_t* [nTicksX];
        for (int i = 0; i < nTicksX; i++)
            ticksX[i] = new wchar_t[nTickChars];
        ticksY = new wchar_t* [nTicksY];
        for (int i = 0; i < nTicksY; i++)
            ticksY[i] = new wchar_t[nTickChars];
    }
    void deleteAxes() {

        // deallocation
        for (int i = 0; i < nTicksX; i++)
            delete[] ticksX[i];
        delete[] ticksX;

        for (int i = 0; i < nTicksY; i++)
            delete[] ticksY[i];
        delete[] ticksY;
    }



};