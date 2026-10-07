// prototype-windowed - independent Win32 frame addon for Prototype (2009)
// Build as a 32-bit DLL/ASI and export InitializeASI.
//
// PrototypeFix stays installed as prototype_fix.asi.
// This addon is loaded separately as prototype_windowed.asi.

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static DWORD g_pid = 0;
static HWND g_window = nullptr;

static BOOL CALLBACK FindGameWindow(HWND hwnd, LPARAM)
{
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid != g_pid || !IsWindowVisible(hwnd))
        return TRUE;

    RECT rc{};
    if (!GetClientRect(hwnd, &rc))
        return TRUE;

    if ((rc.right - rc.left) < 640 || (rc.bottom - rc.top) < 480)
        return TRUE;

    g_window = hwnd;
    return FALSE;
}

static void ApplyFrame(HWND hwnd, bool center)
{
    LONG style = GetWindowLongA(hwnd, GWL_STYLE);
    LONG exStyle = GetWindowLongA(hwnd, GWL_EXSTYLE);

    style |= WS_OVERLAPPEDWINDOW;
    style &= ~WS_POPUP;
    exStyle &= ~WS_EX_TOPMOST;

    SetWindowLongA(hwnd, GWL_STYLE, style);
    SetWindowLongA(hwnd, GWL_EXSTYLE, exStyle);

    if (center) {
        RECT frame{0, 0, 1920, 1080};
        AdjustWindowRectEx(&frame, style, FALSE, exStyle);

        const int width = frame.right - frame.left;
        const int height = frame.bottom - frame.top;
        const int x = max(0, (GetSystemMetrics(SM_CXSCREEN) - width) / 2);
        const int y = max(0, (GetSystemMetrics(SM_CYSCREEN) - height) / 2);

        SetWindowPos(hwnd, HWND_NOTOPMOST, x, y, width, height,
            SWP_FRAMECHANGED | SWP_NOACTIVATE);
    } else {
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
            SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
    }
}

static DWORD WINAPI WindowGuardThread(LPVOID)
{
    // Important for the independent-addon version: let PrototypeFix finish
    // its startup/window styling first, regardless of ASI load order.
    Sleep(1200);

    g_pid = GetCurrentProcessId();

    for (;;) {
        g_window = nullptr;
        EnumWindows(FindGameWindow, 0);

        if (g_window) {
            ApplyFrame(g_window, true);
            break;
        }
        Sleep(100);
    }

    for (;;) {
        Sleep(250);

        if (!IsWindow(g_window))
            break;

        LONG style = GetWindowLongA(g_window, GWL_STYLE);
        if ((style & WS_CAPTION) == 0 || (style & WS_THICKFRAME) == 0)
            ApplyFrame(g_window, false);
    }

    return 0;
}

extern "C" __declspec(dllexport) void InitializeASI()
{
    CreateThread(nullptr, 0, WindowGuardThread, nullptr, 0, nullptr);
}
