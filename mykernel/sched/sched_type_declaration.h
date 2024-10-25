#ifndef _SCHED_TYPE_DECLARATION_H_
#define _SCHED_TYPE_DECLARATION_H_


	/* task_struct member predeclarations (sorted alphabetically): */
	struct rlimit;
	typedef struct rlimit rlimit_s;
	struct files_struct;
	typedef struct files_struct files_struct_s;

	#include <asm/sched_types.h>
	#include <linux/kernel/types.h>
	#include <linux/kernel/lib_types.h>
	#include <linux/kernel/mm_type_declaration.h>
	#include <linux/kernel/lock_ipc_type_declaration.h>
	#include <linux/kernel/swait.h>
	#include <linux/kernel/completion.h>
	#include <linux/kernel/resource.h>


	/* task misc */
	struct fs_struct;
	typedef struct fs_struct taskfs_s;
	struct kernel_clone_args;
	typedef struct kernel_clone_args kclone_args_s;

	/* runqueueu */
	struct cfs_rq;
	typedef struct cfs_rq cfs_rq_s;
	struct rt_rq;
	typedef struct rt_rq rt_rq_s;
	struct dl_rq;
	typedef struct dl_rq dl_rq_s;
	struct myos_rq;
	typedef struct myos_rq myos_rq_s;
	struct runqueue;
	typedef struct runqueue rq_s;
	struct rq_flags;
	typedef struct rq_flags rq_flags_s;

	/* scheduler */
	struct sched_entity;
	typedef struct sched_entity sched_entity_s;
	struct sched_rt_entity;
	typedef struct sched_rt_entity sched_rt_entity_s;
	struct sched_class;
	typedef struct sched_class sched_class_s;

	/* task */
	struct upid;
	typedef struct upid upid_s;
	struct pid;
	typedef struct pid pid_s;
	struct task_struct;
	typedef struct task_struct task_s;

#endif /* _SCHED_TYPE_DECLARATION_H_ */