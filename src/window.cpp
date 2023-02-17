#include "window.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_TIMER:
            if (Window::instance != NULL)
                Window::instance->update();
            InvalidateRect(hWnd, 0, true);
            break;
        case WM_PAINT:
            if (Window::instance != NULL){
                GraphicEngine* g = new GraphicEngine(hWnd);
                Window::instance->render(*g);
                delete g;
            }
            break;
        case WM_LBUTTONDOWN:
            if (Window::instance != NULL)
                Window::instance->input(VK_LBUTTON);
            break;
        case WM_KEYDOWN:
            if (Window::instance != NULL)
                Window::instance->input(wParam);
            std::cout << "keydown " << wParam << std::endl;
            break;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

Window* Window::instance;

Window::Window()
    : m_hInstance(GetModuleHandle(nullptr))
{
    Window::instance = this;

    const wchar_t* CLASS_NAME = L"Traffic Light Window Class";

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = m_hInstance;
    wcex.hIcon          = LoadIcon(nullptr, IDI_WINLOGO);
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = CLASS_NAME;
    wcex.hIconSm        = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassExW(&wcex))
        std::cout << "Could not register class: " << GetLastError() << std::endl;

    m_hWnd = CreateWindowW(
            CLASS_NAME,
            L"Traffic Light Demo",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            0,
            CW_USEDEFAULT,
            0,
            nullptr,
            nullptr,
            m_hInstance,
            nullptr
            );

    if (m_hWnd == 0)
        std::cout << "Could not create Window: " << GetLastError() << std::endl;

}

Window::~Window()
{
    const wchar_t* CLASS_NAME = L"Traffic Light Window Class";
    UnregisterClassW(CLASS_NAME, m_hInstance);
    KillTimer(m_hWnd, 1001u);
}

void Window::start()
{
    ShowWindow(m_hWnd, SW_SHOW);
    SetTimer(m_hWnd, 1001u, 1000.0/60.0, NULL);
}

bool Window::ProcessMessages()
{
    MSG msg = {};

    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

void Window::render(GraphicEngine hWnd) { m_hRender(hWnd); }
void Window::update() { m_hUpdate(delta); }
void Window::input(WPARAM param) { m_hInput(param); }

void Window::setRenderCallback(std::function<void(GraphicEngine)> render) { m_hRender = render; }
void Window::setUpdateCallback(std::function<void(float)> update) { m_hUpdate = update; }
void Window::setInputCallback(std::function<void(WPARAM)> input) { m_hInput = input; }



