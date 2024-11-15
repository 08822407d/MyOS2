#ifndef _SIGNAL_API_H_
#define _SIGNAL_API_H_

	#include "signal.h"


	bool get_signal(ksignal_s *ksig);

	void prepare_kill_siginfo(int sig,
			kernel_siginfo_t *info, enum pid_type type);

	int kill_something_info(int sig,
			kernel_siginfo_t *info, pid_t pid);

	void __set_current_blocked(const sigset_t *newset);
	void set_current_blocked(sigset_t *newset);
	int sigprocmask(int how, sigset_t *set, sigset_t *oldset);

	int copy_siginfo_to_user(siginfo_t *to, const kernel_siginfo_t *from);

	int do_sigaction(int sig, k_sigaction_s *act, k_sigaction_s *oact);

	void signals_init(void);

#endif /* _SIGNAL_API_H_ */