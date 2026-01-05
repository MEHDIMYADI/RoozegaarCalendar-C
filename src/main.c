#include "window.h"
#include <windows.h>
#include <wchar.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    BOOL startHidden = FALSE;

    if (lpCmdLine && wcsstr(lpCmdLine, L"--silent"))
    {
        startHidden = TRUE;
    }

    int cmdShow = startHidden ? SW_HIDE : nCmdShow;

    return RunMainWindow(hInstance, cmdShow);
}