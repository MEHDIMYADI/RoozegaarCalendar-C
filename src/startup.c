#include <windows.h>
#include <wchar.h>
#include <shlwapi.h>
#include "startup.h"

#pragma comment(lib, "shlwapi.lib")

#define RUN_KEY L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"

BOOL AddToStartup(LPCWSTR appName, LPCWSTR exePath, BOOL silent)
{
    if (!PathFileExistsW(exePath))
        return FALSE;

    wchar_t exeWithArgs[MAX_PATH + 16] = { 0 };

    wcscpy_s(exeWithArgs, sizeof(exeWithArgs) / sizeof(wchar_t), exePath);

    if (silent)
    {
        wcscat_s(exeWithArgs, sizeof(exeWithArgs) / sizeof(wchar_t), L" --silent");
    }

    HKEY hKey;
    if (RegOpenKeyExW(
        HKEY_CURRENT_USER,
        RUN_KEY,
        0,
        KEY_SET_VALUE,
        &hKey) != ERROR_SUCCESS)
        return FALSE;

    LONG result = RegSetValueExW(
        hKey,
        appName,
        0,
        REG_SZ,
        (const BYTE*)exeWithArgs,
        (DWORD)((wcslen(exeWithArgs) + 1) * sizeof(WCHAR))
    );

    RegCloseKey(hKey);
    return result == ERROR_SUCCESS;
}

BOOL RemoveFromStartup(LPCWSTR appName)
{
    HKEY hKey;
    if (RegOpenKeyExW(
        HKEY_CURRENT_USER,
        RUN_KEY,
        0,
        KEY_SET_VALUE,
        &hKey) != ERROR_SUCCESS)
        return FALSE;

    LONG result = RegDeleteValueW(hKey, appName);

    RegCloseKey(hKey);
    return result == ERROR_SUCCESS;
}

BOOL IsInStartup(LPCWSTR appName, LPWSTR outPath, DWORD outPathSize)
{
    HKEY hKey;
    if (RegOpenKeyExW(
        HKEY_CURRENT_USER,
        RUN_KEY,
        0,
        KEY_QUERY_VALUE,
        &hKey) != ERROR_SUCCESS)
        return FALSE;

    DWORD type = REG_SZ;
    DWORD size = outPathSize * sizeof(WCHAR);

    LONG result = RegQueryValueExW(
        hKey,
        appName,
        NULL,
        &type,
        (LPBYTE)outPath,
        outPath ? &size : NULL
    );

    RegCloseKey(hKey);
    return result == ERROR_SUCCESS;
}