#ifndef _WORKQUEUE_CONSTS_H_
#define _WORKQUEUE_CONSTS_H_

#include <asm-generic/bitsperlong.h>
#include <linux/kernel/sched_const.h>
#include <linux/kernel/threads.h>


	enum worker_pool_flags {
		/*
		 * worker_pool flags
		 *
		 * A bound pool is either associated or disassociated with its CPU.
		 * While associated (!DISASSOCIATED), all workers are bound to the
		 * CPU and none has %WORKER_UNBOUND set and concurrency management
		 * is in effect.
		 *
		 * While DISASSOCIATED, the cpu may be offline and all workers have
		 * %WORKER_UNBOUND set and concurrency management disabled, and may
		 * be executing on any CPU.  The pool behaves as an unbound one.
		 *
		 * Note that DISASSOCIATED should be flipped only while holding
		 * wq_pool_attach_mutex to avoid changing binding state while
		 * worker_attach_to_pool() is in progress.
		 *
		 * As there can only be one concurrent BH execution context per CPU, a
		 * BH pool is per-CPU and always DISASSOCIATED.
		 */
		POOL_BH					= 1 << 0,		/* is a BH pool */
		POOL_MANAGER_ACTIVE		= 1 << 1,		/* being managed */
		POOL_DISASSOCIATED		= 1 << 2,		/* cpu can't serve workers */
		POOL_BH_DRAINING		= 1 << 3,		/* draining after CPU offline */
	};

	enum worker_flags {
		/* worker flags */
		WORKER_DIE				= 1 << 1,		/* die die die */
		WORKER_IDLE				= 1 << 2,		/* is idle */
		WORKER_PREP				= 1 << 3,		/* preparing to run works */
		WORKER_CPU_INTENSIVE	= 1 << 6,		/* cpu intensive */
		WORKER_UNBOUND			= 1 << 7,		/* worker is unbound */
		WORKER_REBOUND			= 1 << 8,		/* worker was rebound */

		WORKER_NOT_RUNNING		= WORKER_PREP | WORKER_CPU_INTENSIVE |
									WORKER_UNBOUND | WORKER_REBOUND,
	};

	enum work_cancel_flags {
		WORK_CANCEL_DELAYED		= 1 << 0,		/* canceling a delayed_work */
		WORK_CANCEL_DISABLE		= 1 << 1,		/* canceling to disable */
	};

	enum wq_internal_consts {
		NR_STD_WORKER_POOLS		= 2,			/* # standard pools per cpu */

		UNBOUND_POOL_HASH_ORDER	= 6,			/* hashed by pool->attrs */
		BUSY_WORKER_HASH_ORDER	= 6,			/* 64 pointers */

		MAX_IDLE_WORKERS_RATIO	= 4,			/* 1/4 of busy can be idle */
		IDLE_WORKER_TIMEOUT		= 300 * HZ,		/* keep idle ones for 5 mins */

		MAYDAY_INITIAL_TIMEOUT  = HZ / 100 >= 2 ? HZ / 100 : 2,
												/* call for help after 10ms
												   (min two ticks) */
		MAYDAY_INTERVAL			= HZ / 10,		/* and then every 100ms */
		CREATE_COOLDOWN			= HZ,			/* time to breath after fail */

		/*
		 * Rescue workers are used only on emergencies and shared by
		 * all cpus.  Give MIN_NICE.
		 */
		RESCUER_NICE_LEVEL		= MIN_NICE,
		HIGHPRI_NICE_LEVEL		= MIN_NICE,

		WQ_NAME_LEN				= 32,
		WORKER_ID_LEN			= 10 + WQ_NAME_LEN,		/* "kworker/R-" + WQ_NAME_LEN */
	};


	enum work_bits {
		WORK_STRUCT_PENDING_BIT		= 0,		/* work item is pending execution */
		WORK_STRUCT_INACTIVE_BIT,				/* work item is inactive */
		WORK_STRUCT_PWQ_BIT,					/* data points to pwq */
		WORK_STRUCT_LINKED_BIT,					/* next work is linked to this one */
	#ifdef CONFIG_DEBUG_OBJECTS_WORK
		WORK_STRUCT_STATIC_BIT,					/* static initializer (debugobjects) */
	#endif
		WORK_STRUCT_FLAG_BITS,

		/* color for workqueue flushing */
		WORK_STRUCT_COLOR_SHIFT		= WORK_STRUCT_FLAG_BITS,
		WORK_STRUCT_COLOR_BITS		= 4,

		/*
		 * When WORK_STRUCT_PWQ is set, reserve 8 bits off of pwq pointer w/
		 * debugobjects turned off. This makes pwqs aligned to 256 bytes (512
		 * bytes w/ DEBUG_OBJECTS_WORK) and allows 16 workqueue flush colors.
		 *
		 * MSB
		 * [ pwq pointer ] [ flush color ] [ STRUCT flags ]
		 *                     4 bits        4 or 5 bits
		 */
		WORK_STRUCT_PWQ_SHIFT		= WORK_STRUCT_COLOR_SHIFT + WORK_STRUCT_COLOR_BITS,

		/*
		 * data contains off-queue information when !WORK_STRUCT_PWQ.
		 *
		 * MSB
		 * [ pool ID ] [ disable depth ] [ OFFQ flags ] [ STRUCT flags ]
		 *                  16 bits          1 bit        4 or 5 bits
		 */
		WORK_OFFQ_FLAG_SHIFT		= WORK_STRUCT_FLAG_BITS,
		WORK_OFFQ_BH_BIT			= WORK_OFFQ_FLAG_SHIFT,
		WORK_OFFQ_FLAG_END,
		WORK_OFFQ_FLAG_BITS			= WORK_OFFQ_FLAG_END - WORK_OFFQ_FLAG_SHIFT,

		WORK_OFFQ_DISABLE_SHIFT		= WORK_OFFQ_FLAG_SHIFT + WORK_OFFQ_FLAG_BITS,
		WORK_OFFQ_DISABLE_BITS		= 16,

		/*
		 * When a work item is off queue, the high bits encode off-queue flags
		 * and the last pool it was on. Cap pool ID to 31 bits and use the
		 * highest number to indicate that no pool is associated.
		 */
		WORK_OFFQ_POOL_SHIFT		= WORK_OFFQ_DISABLE_SHIFT + WORK_OFFQ_DISABLE_BITS,
		WORK_OFFQ_LEFT				= BITS_PER_LONG - WORK_OFFQ_POOL_SHIFT,
		WORK_OFFQ_POOL_BITS			= WORK_OFFQ_LEFT <= 31 ? WORK_OFFQ_LEFT : 31,
	};

	enum work_flags {
		WORK_STRUCT_PENDING		= 1 << WORK_STRUCT_PENDING_BIT,
		WORK_STRUCT_INACTIVE	= 1 << WORK_STRUCT_INACTIVE_BIT,
		WORK_STRUCT_PWQ			= 1 << WORK_STRUCT_PWQ_BIT,
		WORK_STRUCT_LINKED		= 1 << WORK_STRUCT_LINKED_BIT,
	// #ifdef CONFIG_DEBUG_OBJECTS_WORK
	// 	WORK_STRUCT_STATIC		= 1 << WORK_STRUCT_STATIC_BIT,
	// #else
		WORK_STRUCT_STATIC		= 0,
	// #endif
	};

	enum wq_misc_consts {
		WORK_NR_COLORS			= (1 << WORK_STRUCT_COLOR_BITS),

		/* not bound to any CPU, prefer the local CPU */
		WORK_CPU_UNBOUND		= NR_CPUS,

		/* bit mask for work_busy() return values */
		WORK_BUSY_PENDING		= 1 << 0,
		WORK_BUSY_RUNNING		= 1 << 1,

		/* maximum string length for set_worker_desc() */
		WORKER_DESC_LEN			= 32,
	};

	/* Convenience constants - of type 'unsigned long', not 'enum'! */
	#define WORK_OFFQ_BH			(1ul << WORK_OFFQ_BH_BIT)
	#define WORK_OFFQ_FLAG_MASK		(((1ul << WORK_OFFQ_FLAG_BITS) - 1) << WORK_OFFQ_FLAG_SHIFT)
	#define WORK_OFFQ_DISABLE_MASK	(((1ul << WORK_OFFQ_DISABLE_BITS) - 1) << WORK_OFFQ_DISABLE_SHIFT)
	#define WORK_OFFQ_POOL_NONE		((1ul << WORK_OFFQ_POOL_BITS) - 1)
	#define WORK_STRUCT_NO_POOL		(WORK_OFFQ_POOL_NONE << WORK_OFFQ_POOL_SHIFT)
	#define WORK_STRUCT_PWQ_MASK	(~((1ul << WORK_STRUCT_PWQ_SHIFT) - 1))

	#define WORK_DATA_INIT()		ATOMIC_LONG_INIT(	\
				(unsigned long)WORK_STRUCT_NO_POOL		\
			)
	#define WORK_DATA_STATIC_INIT()	ATOMIC_LONG_INIT(	\
				(unsigned long)(WORK_STRUCT_NO_POOL |	\
					WORK_STRUCT_STATIC)					\
			)

	/*
	 * Workqueue flags and constants.  For details, please refer to
	 * Documentation/core-api/workqueue.rst.
	 */
	enum wq_flags {
		WQ_BH					= 1 << 0,	/* execute in bottom half (softirq) context */
		WQ_UNBOUND				= 1 << 1,	/* not bound to any cpu */
		WQ_FREEZABLE			= 1 << 2,	/* freeze during suspend */
		WQ_MEM_RECLAIM			= 1 << 3,	/* may be used for memory reclaim */
		WQ_HIGHPRI				= 1 << 4,	/* high priority */
		WQ_CPU_INTENSIVE		= 1 << 5,	/* cpu intensive workqueue */
		WQ_SYSFS				= 1 << 6,	/* visible in sysfs, see workqueue_sysfs_register() */

		/*
		 * Per-cpu workqueues are generally preferred because they tend to
		 * show better performance thanks to cache locality.  Per-cpu
		 * workqueues exclude the scheduler from choosing the CPU to
		 * execute the worker threads, which has an unfortunate side effect
		 * of increasing power consumption.
		 *
		 * The scheduler considers a CPU idle if it doesn't have any task
		 * to execute and tries to keep idle cores idle to conserve power;
		 * however, for example, a per-cpu work item scheduled from an
		 * interrupt handler on an idle CPU will force the scheduler to
		 * execute the work item on that CPU breaking the idleness, which in
		 * turn may lead to more scheduling choices which are sub-optimal
		 * in terms of power consumption.
		 *
		 * Workqueues marked with WQ_POWER_EFFICIENT are per-cpu by default
		 * but become unbound if workqueue.power_efficient kernel param is
		 * specified.  Per-cpu workqueues which are identified to
		 * contribute significantly to power-consumption are identified and
		 * marked with this flag and enabling the power_efficient mode
		 * leads to noticeable power saving at the cost of small
		 * performance disadvantage.
		 *
		 * http://thread.gmane.org/gmane.linux.kernel/1480396
		 */
		WQ_POWER_EFFICIENT		= 1 << 7,

		__WQ_DESTROYING			= 1 << 15,	/* internal: workqueue is destroying */
		__WQ_DRAINING			= 1 << 16,	/* internal: workqueue is draining */
		__WQ_ORDERED			= 1 << 17,	/* internal: workqueue is ordered */
		__WQ_LEGACY				= 1 << 18,	/* internal: create*_workqueue() */

		/* BH wq only allows the following flags */
		__WQ_BH_ALLOWS			= WQ_BH | WQ_HIGHPRI,
	};

	enum wq_consts {
		WQ_MAX_ACTIVE			= 512,		/* I like 512, better ideas? */
		WQ_UNBOUND_MAX_ACTIVE	= WQ_MAX_ACTIVE,
		WQ_DFL_ACTIVE			= WQ_MAX_ACTIVE / 2,

		/*
		 * Per-node default cap on min_active. Unless explicitly set, min_active
		 * is set to min(max_active, WQ_DFL_MIN_ACTIVE). For more details, see
		 * workqueue_struct->min_active definition.
		 */
		WQ_DFL_MIN_ACTIVE		= 8,
	};

	/*
	 * Per-pool_workqueue statistics. These can be monitored using
	 * tools/workqueue/wq_monitor.py.
	 */
	enum pool_workqueue_stats {
		PWQ_STAT_STARTED,			/* work items started execution */
		PWQ_STAT_COMPLETED,			/* work items completed execution */
		PWQ_STAT_CPU_TIME,			/* total CPU time consumed */
		PWQ_STAT_CPU_INTENSIVE,		/* wq_cpu_intensive_thresh_us violations */
		PWQ_STAT_CM_WAKEUP,			/* concurrency-management worker wakeups */
		PWQ_STAT_REPATRIATED,		/* unbound workers brought back into scope */
		PWQ_STAT_MAYDAY,			/* maydays to rescuer */
		PWQ_STAT_RESCUED,			/* linked work items executed by rescuer */

		PWQ_NR_STATS,
	};


#endif /* _WORKQUEUE_CONSTS_H_ */