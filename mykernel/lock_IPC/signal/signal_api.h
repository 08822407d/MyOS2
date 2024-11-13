#ifndef _SIGNAL_API_H_
#define _SIGNAL_API_H_

	#include "signal.h"


	bool get_signal(ksignal_s *ksig);

	void prepare_kill_siginfo(int sig,
			kernel_siginfo_t *info, enum pid_type type);

	int kill_something_info(int sig,
			kernel_siginfo_t *info, pid_t pid);

	int do_sigaction(int sig, k_sigaction_s *act, k_sigaction_s *oact);

	void signals_init(void);

#endif /* _SIGNAL_API_H_ */