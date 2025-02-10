#ifndef _SWAIT_API_H_
#define _SWAIT_API_H_

	#include "swait.h"

	extern void __init_swait_queue_head(swqueue_hdr_s *q);
	extern void swake_up_locked(swqueue_hdr_s *q);
	extern void __finish_swait(swqueue_hdr_s *q, swqueue_s *wait);
	void __prepare_to_swait(swqueue_hdr_s *q, swqueue_s *wait);

#endif /* _SWAIT_API_H_ */