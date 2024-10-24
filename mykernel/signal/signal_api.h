#ifndef _SIGNAL_API_H_
#define _SIGNAL_API_H_

	#include "signal_type_declaration.h"

	void prepare_kill_siginfo(int sig,
			kernel_siginfo_t *info, enum pid_type type);

#endif /* _SIGNAL_API_H_ */