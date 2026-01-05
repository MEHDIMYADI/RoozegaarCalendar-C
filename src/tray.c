#include <windows.h>
#include <shellapi.h>
#include "tray.h"
#include "resource.h"
#include "persian_number.h"
#include "calendar.h"

static NOTIFYICONDATAW nid;
#define ID_TRAY_SHOW 1001
#define ID_TRAY_EXIT 1002

static void ShowTrayMenu(HWND hWnd, POINT pt)
{
    HMENU hMenu = CreatePopupMenu();
    if (!hMenu) return;

    InsertMenuW(hMenu, 0, MF_BYPOSITION, ID_TRAY_SHOW, L"گاه‌شمار روزگار");
    InsertMenuW(hMenu, 1, MF_BYPOSITION, ID_TRAY_EXIT, L"خروج");

    SetForegroundWindow(hWnd);

    TrackPopupMenu(
        hMenu,
        TPM_RIGHTBUTTON | TPM_BOTTOMALIGN,
        pt.x,
        pt.y,
        0,
        hWnd,
        NULL
    );

    DestroyMenu(hMenu);
}

void InitTrayIcon(HWND hWnd)
{
    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(NOTIFYICONDATAW);
    nid.hWnd = hWnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO;
    nid.uCallbackMessage = WM_USER + 1;
    
    nid.hIcon = (HICON)LoadImage(
        GetModuleHandle(NULL),
        MAKEINTRESOURCE(IDI_ICON1),
        IMAGE_ICON,
        16, 16,
        LR_DEFAULTCOLOR
    );

    Shell_NotifyIconW(NIM_ADD, &nid);

    wchar_t balloonText[128];
    SYSTEMTIME st;
    GetLocalTime(&st);

    PersianDate pd = gregorian_to_persian(st.wYear, st.wMonth, st.wDay);

    wchar_t year[8], month[4], day[4];
    to_persian_number(pd.year, year, 8);
    to_persian_number(pd.month, month, 4);
    to_persian_number(pd.day, day, 4);

    swprintf_s(balloonText, sizeof(balloonText) / sizeof(balloonText[0]),
        L"امروز: %s\nتاریخ: %s/%s/%s",
        day_of_week_name(st.wDayOfWeek),
        year, month, day);

    lstrcpyW(nid.szTip, balloonText);
    lstrcpyW(nid.szInfo, balloonText);
    lstrcpyW(nid.szInfoTitle, L"گاه‌شمار روزگار");

    nid.uTimeout = 5000;
    nid.dwInfoFlags = NIIF_INFO;
    Shell_NotifyIconW(NIM_MODIFY, &nid);
}

void ShowBalloonOnHover(HWND hWnd)
{
    wchar_t balloonText[256];
    SYSTEMTIME st;
    GetLocalTime(&st);

    PersianDate pd = gregorian_to_persian(st.wYear, st.wMonth, st.wDay);

    wchar_t year[8], month[4], day[4];
    to_persian_number(pd.year, year, 8);
    to_persian_number(pd.month, month, 4);
    to_persian_number(pd.day, day, 4);

    swprintf_s(balloonText, sizeof(balloonText) / sizeof(balloonText[0]),
        L"امروز: %s\nتاریخ: %s/%s/%s",
        day_of_week_name(st.wDayOfWeek),
        year, month, day);

    lstrcpyW(nid.szInfo, balloonText);
    lstrcpyW(nid.szInfoTitle, L"گاه‌شمار روزگار");
    nid.uFlags |= NIF_INFO;
    nid.uTimeout = 5000;
    nid.dwInfoFlags = NIIF_INFO;

    Shell_NotifyIconW(NIM_MODIFY, &nid);
}

void RemoveTrayIcon(void)
{
    Shell_NotifyIconW(NIM_DELETE, &nid);
}


void HandleTrayMessage(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    if (wParam != nid.uID) return;

    switch (LOWORD(lParam))
    {
    case WM_RBUTTONUP:
    {
        POINT pt;
        GetCursorPos(&pt);
        ShowTrayMenu(hWnd, pt);
        break;
    }
    case WM_LBUTTONDBLCLK:
        ShowWindow(hWnd, SW_SHOW);
        SetForegroundWindow(hWnd);
        break;
    }
}