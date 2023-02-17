#include "GraphicEngine.h"

GraphicEngine::GraphicEngine(HWND hWnd) {
    const RECT winDefaultBorder {7, 30 , 7, 7};
    RECT rect;
    GetWindowRect(hWnd, &rect);
    width = rect.right - rect.left - winDefaultBorder.left - winDefaultBorder.right;
    height = rect.bottom - rect.top - winDefaultBorder.top - winDefaultBorder.bottom;

    m_struct;
    m_context = BeginPaint(hWnd, &m_struct);
    m_secondary = CreateCompatibleDC(m_context);
    m_bitmap = CreateCompatibleBitmap(m_context,width,height);
    SelectObject(m_secondary, m_bitmap);

    m_brush = CreateSolidBrush(RGB(255,255,255));
    m_pen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
}

GraphicEngine::~GraphicEngine() {
    if (m_brush != nullptr)
        DeleteObject(m_brush);
    if (m_pen != nullptr)
        DeleteObject(m_pen);

    SelectObject(m_context, brush_orig);
    SelectObject(m_context, pen_orig);

    BitBlt(m_context, 0, 0, width, height, m_secondary, 0, 0, SRCCOPY);

    DeleteDC(m_context);
    DeleteDC(m_secondary);
    DeleteObject(m_bitmap);

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

void GraphicEngine::select() {
    if (m_brush != nullptr)
        brush_orig = SelectObject(m_secondary, m_brush);
    if (m_pen != nullptr)
        pen_orig = SelectObject(m_secondary, m_pen);
};

void GraphicEngine::fillRect(int px, int py, int width, int height) {
    select();
    Rectangle(m_secondary, px, py, px + width, py + height);
}

void GraphicEngine::fillEllipse(int px, int py, int width, int height) {
    select();
    Ellipse(m_secondary, px, py, px + width, py + height);
}

void GraphicEngine::setFont() {

}

void GraphicEngine::setTextColor(int r, int g, int b) {

}

void GraphicEngine::drawText(std::string text, int x, int y) {
    std::wstring temp = std::wstring(text.begin(), text.end());
    LPCWSTR data = temp.c_str();
    TextOutW(m_secondary, x, y, data, text.length());
}

