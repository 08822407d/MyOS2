#ifndef _SOFTIRQ_API_H_
#define _SOFTIRQ_API_H_

	#include "softirq.h"


	extern task_s *ksoftirqd;




	void softirq_init(void);

#endif /* _SOFTIRQ_API_H_ */