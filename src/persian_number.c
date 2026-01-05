#include "persian_number.h"
#include <wchar.h>

static const wchar_t persian_digits[] = L"۰۱۲۳۴۵۶۷۸۹";

void to_persian_number(int num, wchar_t* buffer, int bufsize)
{
    if (!buffer || bufsize <= 0) return;

    wchar_t temp[32];

    _snwprintf_s(temp, sizeof(temp) / sizeof(temp[0]), _TRUNCATE, L"%d", num);

    for (int i = 0; temp[i] != L'\0' && i < bufsize - 1; i++) {
        if (temp[i] >= L'0' && temp[i] <= L'9')
            buffer[i] = persian_digits[temp[i] - L'0'];
        else
            buffer[i] = temp[i];
        buffer[i + 1] = L'\0';
    }
}
