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
    static const UINT_PTR TIMER_UPDATE_PHYSICS = 1u;
    static const UINT_PTR TIMER_UPDATE_LIGHTS = 2u;
public:
    Window();
    ~Window();

    void start();
    void render(GraphicEngine hWnd);
    void update(WPARAM param);
    void input(WPARAM param);

    void showDialog();

    void setTimer(UINT_PTR timer, UINT period);

    void setRenderCallback(std::function<void(GraphicEngine)> render);
    void setUpdateCallback(std::function<void(float delta, WPARAM param)> update);
    void setInputCallback(std::function<void(WPARAM)> input);

    bool ProcessMessages();
private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;

    const float delta = 1.0 / 60.0;

    std::function<void(GraphicEngine)> m_hRender = [](auto){};
    std::function<void(float, WPARAM)> m_hUpdate = [](float, WPARAM){};
    std::function<void(WPARAM)> m_hInput = [](WPARAM){};

};
