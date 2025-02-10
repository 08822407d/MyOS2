#ifndef _COMPLETION_API_H_
#define _COMPLETION_API_H_

	#include "completion.h"

	extern void wait_for_completion(completion_s *);
	extern void complete(completion_s *);

#endif /* _COMPLETION_API_H_ */