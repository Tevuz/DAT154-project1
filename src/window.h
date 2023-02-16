#pragma once

#include <windows.h>
#include <iostream>
#include <functional>
#include <map>
#include "GraphicEngine.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
public:
    static Window* instance;
public:
    Window();
    ~Window();

    void start();
    void render(GraphicEngine hWnd);
    void update();

    void setRenderCallback(std::function<void(GraphicEngine)> render);
    void setUpdateCallback(std::function<void()> update);

    bool ProcessMessages();
private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;

    std::function<void(GraphicEngine)> m_hRender = [](auto a){};
    std::function<void()> m_hUpdate = [](){};
};
