#ifndef _SIGNAL_API_H_
#define _SIGNAL_API_H_

	#include "signal.h"

	void prepare_kill_siginfo(int sig,
			kernel_siginfo_t *info, enum pid_type type);

	int kill_something_info(int sig,
			kernel_siginfo_t *info, pid_t pid);

#endif /* _SIGNAL_API_H_ */