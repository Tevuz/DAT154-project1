#pragma once

#include <windows.h>

class GraphicEngine {
public:
    GraphicEngine(HWND hWnd);
    ~GraphicEngine();

    int width;
    int height;

    void setLineColorDisabled();
    void setLineColor(int r, int g, int b);
    void setFillColorDisabled();
    void setFillColor(int r, int g, int b);

    void fillRect(int px, int py, int width, int height);
    void fillEllipse(int px, int py, int width, int height);


private:
    HWND m_window;
    HDC m_context;
    PAINTSTRUCT m_struct;
    HBRUSH m_brush;
    HPEN m_pen;

    void select();
};

