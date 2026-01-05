#ifndef CALENDAR_H
#define CALENDAR_H

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        int year;
        int month;
        int day;
    } PersianDate;

    PersianDate gregorian_to_persian(int gy, int gm, int gd);
    PersianDate persian_to_gregorian(int jy, int jm, int jd);

    const wchar_t* persian_month_name(int month);
    const wchar_t* day_of_week_name(int wDayOfWeek);

    void format_persian_date(PersianDate date, char* buffer, int bufsize);
    void print_month_calendar(int year, int month);

    int is_gregorian_leap(int gy);
    int gregorian_to_jdn(int gy, int gm, int gd);

#ifdef __cplusplus
}
#endif

#endif
