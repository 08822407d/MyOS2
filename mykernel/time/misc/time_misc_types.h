#ifndef _TIME_MISC_TYPES_H_
#define _TIME_MISC_TYPES_H_

	#include "../time_type_declaration.h"


	struct timespec64 {
		time64_t	tv_sec;			/* seconds */
		long		tv_nsec;		/* nanoseconds */
	};

	struct itimerspec64 {
		timespec64_s	it_interval;
		timespec64_s	it_value;
	};

#endif /* _TIME_MISC_TYPES_H_ */
