#pragma once
#include <windows.h>

BOOL AddToStartup(LPCWSTR appName, LPCWSTR exePath, BOOL silent);
BOOL RemoveFromStartup(LPCWSTR appName);
BOOL IsInStartup(LPCWSTR appName, LPWSTR outPath, DWORD outPathSize);