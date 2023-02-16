#include <iostream>
#include "GraphicEngine.h"

GraphicEngine::GraphicEngine(HWND hWnd) {
    m_struct;
    m_context = BeginPaint(hWnd, &m_struct);

    const RECT winDefaultBorder {7, 30 , 7, 7};
    RECT rect;
    GetWindowRect(hWnd, &rect);
    width = rect.right - rect.left - winDefaultBorder.left - winDefaultBorder.right;
    height = rect.bottom - rect.top - winDefaultBorder.top - winDefaultBorder.bottom;

    m_brush = CreateSolidBrush(RGB(255,255,255));
    m_pen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
}

GraphicEngine::~GraphicEngine() {
    if (m_brush != nullptr)
        DeleteObject(m_brush);
    if (m_pen != nullptr)
        DeleteObject(m_pen);

    EndPaint(m_window, &m_struct);
}

void GraphicEngine::setFillColorDisabled() {
    if (m_brush != nullptr)
        DeleteObject(m_brush);
}

void GraphicEngine::setFillColor(int r, int g, int b) {
    if (m_brush != nullptr)
        DeleteObject(m_brush);

    m_brush = CreateSolidBrush(RGB(r,g,b));
}

void GraphicEngine::setLineColorDisabled() {
    if (m_pen != nullptr)
        DeleteObject(m_pen);
    m_pen = CreatePen(PS_NULL, 0, RGB(0,0,0));
}

void GraphicEngine::setLineColor(int r, int g, int b) {
    if (m_pen != nullptr)
        DeleteObject(m_pen);

    m_pen = CreatePen(PS_SOLID, 1, RGB(r,g,b));
}

void GraphicEngine::select()
{
        if (m_brush != nullptr)
        SelectObject(m_context, m_brush);
        if (m_pen != nullptr)
        SelectObject(m_context, m_pen);
};

void GraphicEngine::fillRect(int px, int py, int width, int height) {
    select();
    Rectangle(m_context, px, py, px + width, py + height);
}

void GraphicEngine::fillCircle(int px, int py, int radius) {
    select();
    Ellipse(m_context, px - radius, py - radius, radius, radius);
}

