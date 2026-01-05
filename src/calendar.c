#include <stdio.h>
#include <wchar.h>

typedef struct {
    int year;
    int month;
    int day;
} PersianDate;

static const wchar_t* persian_months[12] = {
    L"فروردین", L"اردیبهشت", L"خرداد", L"تیر", L"مرداد", L"شهریور",
    L"مهر", L"آبان", L"آذر", L"دی", L"بهمن", L"اسفند"
};

static const wchar_t* week_days[7] = {
    L"یکشنبه", L"دوشنبه", L"سه‌شنبه", L"چهارشنبه",
    L"پنجشنبه", L"جمعه", L"شنبه"
};

PersianDate gregorian_to_persian(int gy, int gm, int gd) {
    long out[3];
    long days;
    long gy2 = (gm > 2) ? (gy + 1) : gy;
    long g_d_m[12] = { 0,31,59,90,120,151,181,212,243,273,304,334 };
    days = 355666 + 365 * gy + (gy2 + 3) / 4 - (gy2 + 99) / 100 + (gy2 + 399) / 400 + gd + g_d_m[gm - 1];

    long jy = -1595 + 33 * (days / 12053);
    days %= 12053;
    jy += 4 * (days / 1461);
    days %= 1461;
    if (days > 365) {
        jy += (days - 1) / 365;
        days = (days - 1) % 365;
    }

    int jm, jd;
    if (days < 186) {
        jm = 1 + days / 31;
        jd = 1 + days % 31;
    }
    else {
        jm = 7 + (days - 186) / 30;
        jd = 1 + (days - 186) % 30;
    }

    PersianDate pd = { (int)jy, jm, jd };
    return pd;
}

PersianDate persian_to_gregorian(int jy, int jm, int jd) {
    long out[3];
    jy += 1595;
    long days = -355668 + 365 * jy + (jy / 33) * 8 + ((jy % 33 + 3) / 4);
    if (jm < 7) days += (jm - 1) * 31;
    else days += (jm - 7) * 30 + 186;
    days += jd;

    long gy = 400 * (days / 146097);
    days %= 146097;
    if (days > 36524) {
        gy += 100 * (--days / 36524);
        days %= 36524;
        if (days >= 365) days++;
    }
    gy += 4 * (days / 1461);
    days %= 1461;
    if (days > 365) {
        gy += (days - 1) / 365;
        days = (days - 1) % 365;
    }
    long gd = days + 1;
    long gm;
    long sal_a[13] = { 0,31, ((gy % 4 == 0 && gy % 100 != 0) || (gy % 400 == 0)) ? 29 : 28,31,30,31,30,31,31,30,31,30,31 };
    for (gm = 0; gm<13 && gd>sal_a[gm]; gm++) gd -= sal_a[gm];

    PersianDate pd = { (int)gy, (int)gm, (int)gd };
    return pd;
}

const wchar_t* persian_month_name(int month) {
    if (month < 1 || month > 12) return L"";
    return persian_months[month - 1];
}

const wchar_t* day_of_week_name(int wDayOfWeek) {
    return week_days[wDayOfWeek];
}

void format_persian_date(PersianDate date, char* buffer, int bufsize) {
    snprintf(buffer, bufsize, "%04d/%02d/%02d", date.year, date.month, date.day);
}

int j_month_days_count(int year, int month) {
    if (month <= 6) return 31;
    if (month <= 11) return 30;
    if ((year % 33 == 1) || (year % 33 == 5) || (year % 33 == 9) || (year % 33 == 13) || (year % 33 == 17) || (year % 33 == 22) || (year % 33 == 26) || (year % 33 == 30))
        return 30;
    return 29;
}

void print_month_calendar(int year, int month) {
    PersianDate first_day = persian_to_gregorian(year, month, 1);
    int start_week = (gregorian_to_jdn(first_day.year, first_day.month, first_day.day) + 1) % 7;

    int days_in_month = j_month_days_count(year, month);

    wprintf(L"      %ls %d\n", persian_month_name(month), year);
    for (int i = 0; i < 7; i++) wprintf(L"%-10ls", week_days[i]);
    wprintf(L"\n");

    for (int i = 0; i < start_week; i++) wprintf(L"          ");
    for (int day = 1; day <= days_in_month; day++) {
        wprintf(L"%-10d", day);
        if ((day + start_week) % 7 == 0) wprintf(L"\n");
    }
    wprintf(L"\n");
}

int is_gregorian_leap(int gy) {
    return (gy % 4 == 0 && gy % 100 != 0) || (gy % 400 == 0);
}

int gregorian_to_jdn(int gy, int gm, int gd) {
    int a = (14 - gm) / 12;
    int y = gy + 4800 - a;
    int m = gm + 12 * a - 3;
    return gd + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
}
