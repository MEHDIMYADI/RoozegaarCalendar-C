#ifndef PERSIAN_NUMBER_H
#define PERSIAN_NUMBER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <wchar.h>

	void to_persian_number(int num, wchar_t* buffer, int bufsize);

#ifdef __cplusplus
}
#endif

#endif
