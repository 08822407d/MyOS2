#ifndef _LINUX_SIGNAL_MACRO_H_
#define _LINUX_SIGNAL_MACRO_H_

	// #define tasklist_empty() \
	// 	list_empty(&init_task.tasks)

	// #define next_task(p) \
	// 	list_entry_rcu((p)->tasks.next, struct task_struct, tasks)

	// #define for_each_process(p) \
	// 	for (p = &init_task ; (p = next_task(p)) != &init_task ; )

	/* These can be the second arg to send_sig_info/send_group_sig_info.  */
	#define SEND_SIG_NOINFO	((kernel_siginfo_t *) 0)
	#define SEND_SIG_PRIV	((kernel_siginfo_t *) 1)

	// #define SI_EXPANSION_SIZE (sizeof(struct siginfo) - sizeof(struct kernel_siginfo))

	// static inline void signal_wake_up(task_s *t, bool resume)
	// {
	// 	signal_wake_up_state(t, resume ? TASK_WAKEKILL : 0);
	// }
	#define signal_wake_up(t, resume)	\
			signal_wake_up_state(t, resume ? TASK_WAKEKILL : 0)
	// static inline void ptrace_signal_wake_up(task_s *t, bool resume)
	// {
	// 	signal_wake_up_state(t, resume ? __TASK_TRACED : 0);
	// }
	#define ptrace_signal_wake_up(t, resume)	\
			signal_wake_up_state(t, resume ? __TASK_TRACED : 0)

#endif /* _LINUX_SIGNAL_MACRO_H_ */