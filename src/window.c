#include <windows.h>
#include "window.h"
#include "ui_font.h"
#include "tray.h"
#include "resource.h"
#include "calendar.h"
#include "persian_number.h"
#include "startup.h"

static const wchar_t CLASS_NAME[] = L"RoozegaarCalendarWindow";
#define ID_TRAY_SHOW 1001
#define ID_TRAY_EXIT 1002

#define ID_CHECK_STARTUP 2001
static HWND hChkStartup = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        InitTrayIcon(hWnd);

        hChkStartup = CreateWindowExW(
            WS_EX_RIGHT | WS_EX_RTLREADING,
            L"BUTTON",
            L"اجرا هنگام شروع ویندوز",
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_RIGHT,
            0, 0, 0, 0,
            hWnd,
            (HMENU)ID_CHECK_STARTUP,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        SendMessageW(hChkStartup, WM_SETFONT, (WPARAM)GetMainUIFont(), TRUE);

        if (IsInStartup(L"RoozegaarCalendar", NULL, 0))
        {
            SendMessageW(hChkStartup, BM_SETCHECK, BST_CHECKED, 0);
        }
    }
    break;

    case WM_CLOSE:
        ShowWindow(hWnd, SW_HIDE);
        return 0;

    case WM_DESTROY:
        ReleaseUIFont();
        RemoveTrayIcon();
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        DrawMainView(hWnd, hdc);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_USER + 1:
        HandleTrayMessage(hWnd, wParam, lParam);
        break;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_TRAY_SHOW:
            ShowWindow(hWnd, SW_SHOW);
            SetForegroundWindow(hWnd);
            break;

        case ID_TRAY_EXIT:
            DestroyWindow(hWnd);
            break;

        case ID_CHECK_STARTUP:
        {
            BOOL checked =
                (SendMessageW(hChkStartup, BM_GETCHECK, 0, 0) == BST_CHECKED);

            wchar_t exePath[MAX_PATH];
            GetModuleFileNameW(NULL, exePath, MAX_PATH);

            if (checked)
            {
                wchar_t exePath[MAX_PATH];
                GetModuleFileNameW(NULL, exePath, MAX_PATH);
                AddToStartup(L"RoozegaarCalendar", exePath, TRUE);
            }
            else
            {
                RemoveFromStartup(L"RoozegaarCalendar");
            }
            break;
        }
        }
    }
    break;

    case WM_SIZE:
    {
        if (hChkStartup)
        {
            RECT rc;
            GetClientRect(hWnd, &rc);

            int width = 240;
            int height = 24;
            int x = (rc.right - width) / 2;
            int y = rc.bottom - height - 12;

            MoveWindow(hChkStartup, x, y, width, height, TRUE);
        }
    }
    break;
    }

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

int RunMainWindow(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    RegisterClassW(&wc);

    HWND hWnd = CreateWindowW(
        CLASS_NAME,
        L"Roozegaar Calendar",
        WS_OVERLAPPED | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        360, 320,
        NULL, NULL,
        hInstance, NULL
    );

    ShowWindow(hWnd, nCmdShow);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}

void DrawMainView(HWND hWnd, HDC hdc) {
    SYSTEMTIME st;
    GetLocalTime(&st);

    PersianDate pd = gregorian_to_persian(st.wYear, st.wMonth, st.wDay);

    wchar_t dayStr[8], yearStr[16];
    to_persian_number(pd.day, dayStr, sizeof(dayStr) / sizeof(dayStr[0]));
    to_persian_number(pd.year, yearStr, sizeof(yearStr) / sizeof(yearStr[0]));

    const wchar_t* monthName = persian_month_name(pd.month);
    const wchar_t* wdayName = day_of_week_name(st.wDayOfWeek);

    wchar_t fullDateStr[64];
    swprintf(fullDateStr, 64, L"%ls/%ls/%ls", dayStr, monthName, yearStr);

    RECT rect;
    GetClientRect(hWnd, &rect);

    HFONT hFont = GetMainUIFont();
    HFONT hOld = (HFONT)SelectObject(hdc, hFont);

    int lineCount = 3;
    int lineHeight = 50;
    int totalHeight = lineHeight * lineCount;

    int startY = (rect.bottom - totalHeight) / 2;

    RECT r = rect;

    r.top = startY;
    r.bottom = r.top + lineHeight;
    DrawTextW(hdc, dayStr, -1, &r, DT_CENTER | DT_RTLREADING | DT_SINGLELINE);

    r.top += lineHeight;
    r.bottom = r.top + lineHeight;
    DrawTextW(hdc, wdayName, -1, &r, DT_CENTER | DT_RTLREADING | DT_SINGLELINE);

    r.top += lineHeight;
    r.bottom = r.top + lineHeight;
    DrawTextW(hdc, fullDateStr, -1, &r, DT_CENTER | DT_RTLREADING | DT_SINGLELINE);

    SelectObject(hdc, hOld);
}

