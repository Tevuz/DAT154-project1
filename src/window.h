#pragma once

#include <windows.h>
#include <iostream>
#include <functional>
#include <map>
#include "GraphicEngine.h"
#include "Keymap.h"

using Event = TrafficApp::Event;

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
    void input(WPARAM e);

    void setRenderCallback(std::function<void(GraphicEngine)> render);
    void setUpdateCallback(std::function<void(float delta)> update);
    void setInputCallback(std::function<void(WPARAM)> input);

    bool ProcessMessages();
private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;

    const float delta = 1.0 / 60.0;

    std::function<void(GraphicEngine)> m_hRender = [](auto){};
    std::function<void(float)> m_hUpdate = [](float){};
    std::function<void(WPARAM)> m_hInput = [](WPARAM){};
};
