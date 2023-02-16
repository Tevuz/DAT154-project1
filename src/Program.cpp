#include "Program.h"
#include <iostream>

Program::Program()
{

}

Program::~Program()
{

}

void Program::update()
{
    std::cout << "Update" << std::endl;
}

void Program::render(HWND hWnd)
{
    std::cout << "Render" << std::endl;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
    HGDIOBJ hOrg = SelectObject(hdc, brush);
    Ellipse(hdc, 10, 10, 200, 200);
    SelectObject(hdc, hOrg);
    DeleteObject(brush);
    EndPaint(hWnd, &ps);
}
