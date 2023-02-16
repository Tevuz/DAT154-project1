#include <windows.h>
#include <iostream>

//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    MyDlg(HWND, UINT, WPARAM, LPARAM);

class Window {
public:
    Window();
    ~Window();

    bool ProcessMessages();
private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
};
