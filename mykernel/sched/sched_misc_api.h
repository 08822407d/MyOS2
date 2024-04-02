#ifndef _LINUX_SCHED_MISC_API_H_
#define _LINUX_SCHED_MISC_API_H_

    #include "misc/sched_misc_types.h"
    #include "misc/sched_misc.h"


	extern asmlinkage void schedule_tail(task_s *prev);
	extern int sched_fork(unsigned long clone_flags, task_s *p);
	void __noreturn do_task_dead(void);

	extern void fork_init(void);
	extern void release_task(task_s * p);
	extern int copy_thread(task_s *p, const kclone_args_s *args);

	extern void flush_thread(void);
	extern void exit_thread(task_s *tsk);

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



    #include "misc/fs_struct_types.h"
    #include "misc/fs_struct.h"

	#include <linux/kernel/fdtable.h>


	extern files_struct_s init_files;
	extern taskfs_s init_fs;

	extern kmem_cache_s *files_cachep;
	extern kmem_cache_s *fs_cachep;

	extern void set_fs_root(taskfs_s *, const path_s *);
	extern void set_fs_pwd(taskfs_s *, const path_s *);
	extern void free_fs_struct(taskfs_s *);
	extern void exit_fs(task_s *);
	extern taskfs_s *copy_fs_struct(taskfs_s *);


#endif /* _LINUX_SCHED_MISC_API_H_ */