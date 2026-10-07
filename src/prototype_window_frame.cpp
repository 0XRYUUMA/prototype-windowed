// prototype-windowed - Win32 frame wrapper for Prototype (2009)
//
// Reference implementation of the behavior used by the tested binary in /dist.
// Build as a 32-bit DLL/ASI and export InitializeASI.
//
// The wrapper deliberately leaves PrototypeFix's BorderlessWindow path enabled.
// It loads the original PrototypeFix, calls its InitializeASI export, waits for
// the game's final HWND, then reapplies a normal Windows frame afterwards.

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>

static DWORD g_pid = 0;
static HWND  g_window = nullptr;
static bool  g_initialPlacementDone = false;

static void Log(const char* text)
{
    HANDLE h = CreateFileA("prototype_window_frame.log", FILE_APPEND_DATA,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return;

    DWORD written = 0;
    WriteFile(h, text, (DWORD)lstrlenA(text), &written, nullptr);
    CloseHandle(h);
}

static BOOL CALLBACK FindGameWindow(HWND hwnd, LPARAM)
{
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid != g_pid || !IsWindowVisible(hwnd))
        return TRUE;

    RECT rc{};
    if (!GetClientRect(hwnd, &rc))
        return TRUE;

    // Ignore tiny/helper windows.
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

    RECT frame{0, 0, 1920, 1080};
    AdjustWindowRectEx(&frame, style, FALSE, exStyle);
    const int width  = frame.right - frame.left;
    const int height = frame.bottom - frame.top;

    UINT flags = SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOACTIVATE;

    if (center) {
        const int screenW = GetSystemMetrics(SM_CXSCREEN);
        const int screenH = GetSystemMetrics(SM_CYSCREEN);
        const int x = (screenW - width) / 2;
        const int y = (screenH - height) / 2;

        SetWindowPos(hwnd, HWND_NOTOPMOST, x, y, width, height,
                     SWP_FRAMECHANGED | SWP_NOACTIVATE);
    } else {
        // Restore frame only; do not fight the user's chosen position/size.
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                     flags | SWP_NOMOVE | SWP_NOSIZE);
    }
}

static DWORD WINAPI WindowGuardThread(LPVOID)
{
    Log("window guard thread started\r\n");

    // Give PrototypeFix/game time to create and finish styling the real window.
    for (;;) {
        g_window = nullptr;
        EnumWindows(FindGameWindow, 0);

        if (g_window) {
            Log("game window found\r\n");
            ApplyFrame(g_window, true);
            g_initialPlacementDone = true;
            Log("frame applied: normal border + centered 1920x1080\r\n");
            break;
        }
        Sleep(100);
    }

    // During startup another hook may reapply the borderless style.
    // Repair only the frame from this point on, without recentering/resizing.
    for (;;) {
        Sleep(250);
        if (!IsWindow(g_window))
            break;

        LONG style = GetWindowLongA(g_window, GWL_STYLE);
        if ((style & WS_CAPTION) == 0 || (style & WS_THICKFRAME) == 0) {
            ApplyFrame(g_window, false);
            Log("frame restored after external style change\r\n");
        }
    }
    return 0;
}

extern "C" __declspec(dllexport) void InitializeASI()
{
    Log("wrapper InitializeASI entered\r\n");

    HMODULE original = LoadLibraryA("prototype_fix_original.dll");
    if (!original) {
        Log("ERROR: prototype_fix_original.dll failed to load\r\n");
        return;
    }
    Log("original PrototypeFix loaded\r\n");

    using InitializeASIFn = void (*)();
    auto initializeOriginal =
        reinterpret_cast<InitializeASIFn>(GetProcAddress(original, "InitializeASI"));

    if (!initializeOriginal) {
        Log("ERROR: original InitializeASI export not found\r\n");
        return;
    }

    initializeOriginal();
    Log("original InitializeASI called\r\n");

    g_pid = GetCurrentProcessId();
    CreateThread(nullptr, 0, WindowGuardThread, nullptr, 0, nullptr);
}
