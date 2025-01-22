#ifndef _TIME_MISC_API_H_
#define _TIME_MISC_API_H_

	#include "time_misc.h"

	#include "time64.h"

	extern time64_t
	mktime64(const unsigned int year, const unsigned int mon,
			const unsigned int day, const unsigned int hour,
			const unsigned int min, const unsigned int sec);

#endif /* _TIME_MISC_API_H_ */