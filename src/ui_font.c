#include "ui_font.h"

static HFONT g_hUIFont = NULL;

HFONT GetMainUIFont(void)
{
    if (!g_hUIFont)
    {
        g_hUIFont = CreateFontW(
            20, 0, 0, 0, FW_NORMAL,
            FALSE, FALSE, FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS,
            L"Segoe UI"
        );
    }
    return g_hUIFont;
}

void ReleaseUIFont(void)
{
    if (g_hUIFont)
    {
        DeleteObject(g_hUIFont);
        g_hUIFont = NULL;
    }
}