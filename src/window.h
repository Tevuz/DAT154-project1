#include <windows.h>
#include <iostream>
#include <functional>
#include <map>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
public:
    static Window* instance;
public:
    Window();
    ~Window();

    void start();
    void render(HWND hWnd);
    void update();

    void setRenderCallback(std::function<void(HWND)> render);
    void setUpdateCallback(std::function<void()> update);

    bool ProcessMessages();
private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;

    std::function<void(HWND)> m_hRender = [](auto a){};
    std::function<void()> m_hUpdate = [](){};
};
