#include "window.h"

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            /*if (Window::instance != NULL)
            {
                Window::instance->showDialog();
            }*/
            break;
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_TIMER:
            if (Window::instance != NULL)
                Window::instance->update(wParam);
            InvalidateRect(hWnd, 0, true);
            break;
        case WM_ERASEBKGND:
            return TRUE;
        case WM_PAINT:
            if (Window::instance != NULL){
                GraphicEngine* graphics = new GraphicEngine(hWnd);
                Window::instance->render(*graphics);
                delete graphics;
            }
            break;
        case WM_LBUTTONDOWN:
            if (Window::instance != NULL)
                Window::instance->input(VK_LBUTTON);
            break;
        case WM_KEYDOWN:
            if (Window::instance != NULL)
                Window::instance->input(wParam);
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
    setTimer(TIMER_UPDATE_PHYSICS, 1000.0/60.0);
    setTimer(TIMER_UPDATE_LIGHTS, 6000.0);
}

void Window::showDialog(){
    /*DWORD dwStyle;
    DWORD dwExStyle;

    RECT rcAdjust = { 0, 0, 400, 400 };
    AdjustWindowRectEx(&rcAdjust, dwStyle, FALSE, dwExStyle);
    int cxDlg = rcAdjust.right - rcAdjust.left;
    int cyDlg = rcAdjust.bottom - rcAdjust.top;

    POINT pt = { 400, 400 };
    ClientToScreen(m_hWnd, &pt);

    BOOL fWasVisible = dwStyle & WS_VISIBLE;
    dwStyle &= ~WS_VISIBLE;

    HWND hdlg = CreateWindowEx(
            dwExStyle, nullptr, nullptr, dwStyle & 0xFFFF0000,
            pt.x, pt.y,cxDlg, cyDlg, m_hWnd, NULL, m_hInstance, NULL);

    SetWindowLongPtr(hdlg, DWLP_DLGPROC, (LPARAM) About);


    std::cout << "Showing dialog box\n";*/
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
void Window::update(WPARAM param) { m_hUpdate(delta, param); }
void Window::input(WPARAM param) { m_hInput(param); }

void Window::setRenderCallback(std::function<void(GraphicEngine)> render) { m_hRender = render; }
void Window::setUpdateCallback(std::function<void(float, WPARAM)> update) { m_hUpdate = update; }
void Window::setInputCallback(std::function<void(WPARAM)> input) { m_hInput = input; }

void Window::setTimer(UINT_PTR timer, UINT period) {
    SetTimer(m_hWnd, timer, period, NULL);
}



