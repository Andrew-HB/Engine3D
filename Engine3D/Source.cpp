#ifndef UNICODE
#define UNICODE
#endif

#include "Engine3D.h"
#include <chrono>

#define WNDWIDTH 720
#define WNDHEIGHT 720

Engine3D *demo;
RECT clientRect;
float fElapsedTime;

std::chrono::system_clock::time_point prevT;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,_In_ int nCmdShow) {
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    demo = new Engine3D(WNDWIDTH - 20, WNDHEIGHT - 43);
    prevT = std::chrono::system_clock::now();

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    // Create the window.

    HWND hWnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"3D Engine",                   // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 
        WNDWIDTH,
        WNDHEIGHT,
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hWnd == NULL)
    {
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    
    while (true) {
        if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));

            HDC hdc = GetDC(hWnd);

            // Buffering
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBM = CreateCompatibleBitmap(hdc, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
            SelectObject(memDC, memBM);

            // Clear the screen
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, brush);
            //Rectangle(memDC, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

            // Count elapsed time
            std::chrono::duration<float> elapsedTime = std::chrono::system_clock::now() - prevT;
            prevT = std::chrono::system_clock::now();
            fElapsedTime = elapsedTime.count();

            // Update and draw screen
            demo->OnUserUpdate(memDC, fElapsedTime);

            // Copping buffering frame
            BitBlt(hdc, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, memDC, 0, 0, SRCCOPY);

            SelectObject(hdc, hOldBrush);
            
            DeleteDC(memDC);
            ReleaseDC(hWnd, hdc);
            DeleteObject(memBM);
            DeleteObject(brush);
        }
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_KEYDOWN:
        if (wParam == VK_UP) demo->moveCameraUp(fElapsedTime);
        if (wParam == VK_LEFT) demo->rotateCameraLeft(fElapsedTime);
        if (wParam == VK_DOWN) demo->moveCameraDown(fElapsedTime);
        if (wParam == VK_RIGHT) demo->rotateCameraRight(fElapsedTime);

        if (wParam == 'W') demo->moveCameraForward(fElapsedTime);
        if (wParam == 'A') demo->moveCameraLeft(fElapsedTime);
        if (wParam == 'S') demo->moveCameraBackward(fElapsedTime);
        if (wParam == 'D') demo->moveCameraRight(fElapsedTime);
        break;
    case WM_DESTROY:
    {
        delete demo;
        PostQuitMessage(0);
        break;
    }
    case WM_SIZE:
    {
        GetClientRect(hWnd, &clientRect);
        demo->UpdateWindowSize(clientRect.right, clientRect.bottom);
    }
    default:
    {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }
    }

    return 0;
}