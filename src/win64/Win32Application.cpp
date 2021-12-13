#include "Win32Application.h"

Win32Application::Win32Application(HINSTANCE hInstance, std::wstring className, std::wstring windowTitle)
{
    // Do member var init
    _className = className;
    _windowTitle = windowTitle;
    _windowsHandle = NULL;

    // Init window
    InitWindow(hInstance);
}

int Win32Application::Run(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    ShowWindow(_windowsHandle, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK Win32Application::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);



        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool Win32Application::InitWindow(HINSTANCE hInstance)
{
    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = _className.c_str();

    RegisterClass(&wc);

    // Create the window.

    _windowsHandle = CreateWindowEx(
        0,                              // Optional window styles.
        _className.c_str(),             // Window class
        _windowTitle.c_str(),           // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    // If windows creation failed for any reason, return 0;
    if (_windowsHandle == NULL)
    {
        return false;
    }

    return true;
}
