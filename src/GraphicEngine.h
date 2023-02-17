#pragma once

#include <windows.h>
#include <iostream>

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

    void setFont();
    void setTextColor(int r, int g, int b);
    void drawText(std::string txt, int x, int y);


private:
    HWND m_window;
    HDC m_context;
    HDC m_secondary;
    HBITMAP m_bitmap;
    PAINTSTRUCT m_struct;
    HBRUSH m_brush;
    HPEN m_pen;

    void select();
};

