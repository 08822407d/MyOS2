#ifndef _LINUX_SCHED_MISC_API_H_
#define _LINUX_SCHED_MISC_API_H_

    #include "misc/sched_misc_types.h"
    #include "misc/sched_misc.h"

    #include "misc/fs_struct_types.h"
    #include "misc/fs_struct.h"


	extern asmlinkage void schedule_tail(task_s *prev);
	extern int sched_fork(unsigned long clone_flags, task_s *p);
	extern int copy_thread(task_s *p, const kclone_args_s *args);
	extern void exit_files(task_s *);
	extern pid_t kernel_clone(kclone_args_s *kargs);
	extern pid_t kernel_thread(int (*fn)(void *), void *arg,
			const char *name, unsigned long flags);


	/*
	 * For CONFIG_THREAD_INFO_IN_TASK kernels we need <asm/current.h> for the
	 * definition of current, but for !CONFIG_THREAD_INFO_IN_TASK kernels,
	 * including <asm/current.h> can cause a circular dependency on some platforms.
	 */
	// #include <asm/current.h>
	// #define current_thread_info() ((struct thread_info *)current)
	#define current_thread_info() (&(current->thread_info))

#endif /* _LINUX_SCHED_MISC_API_H_ */