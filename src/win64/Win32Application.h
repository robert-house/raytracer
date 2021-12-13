#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#include <Windows.h>
#include <string>

class Win32Application
{
public:
    Win32Application(HINSTANCE hInstance, std::wstring className, std::wstring windowTitle);
    int Run(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow);

private:
    static LRESULT CALLBACK WindowProc(HWND handle, UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool InitWindow(HINSTANCE hInstance);

    std::wstring _className;
    std::wstring _windowTitle;
    HWND         _windowsHandle;
};