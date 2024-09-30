#ifndef _LINUX_READWRITE_SEMAPHORE_MACRO_H_
#define _LINUX_READWRITE_SEMAPHORE_MACRO_H_


	#ifdef CONFIG_DEBUG_LOCK_ALLOC
	#  define __RWSEM_DEP_MAP_INIT(lockname)			\
				.dep_map = {							\
					.name = #lockname,					\
					.wait_type_inner = LD_WAIT_SLEEP,	\
				},
	#else
	#  define __RWSEM_DEP_MAP_INIT(lockname)
	#endif
	
	#define __RWSEM_COUNT_INIT(name)	\
				.count = ATOMIC_LONG_INIT(RWSEM_UNLOCKED_VALUE)

	/* Common initializer macros and functions */

	#ifdef CONFIG_DEBUG_RWSEMS
	#  define __RWSEM_DEBUG_INIT(lockname)	.magic = &lockname,
	#else
	#  define __RWSEM_DEBUG_INIT(lockname)
	#endif

	#ifdef CONFIG_RWSEM_SPIN_ON_OWNER
	#  define __RWSEM_OPT_INIT(lockname)	.osq = OSQ_LOCK_UNLOCKED,
	#else
	#  define __RWSEM_OPT_INIT(lockname)
	#endif

	#define __RWSEM_INITIALIZER(name)	{							\
				__RWSEM_COUNT_INIT(name),							\
				.owner = ATOMIC_LONG_INIT(0),						\
				__RWSEM_OPT_INIT(name)								\
				.wait_lock = __SPIN_LOCK_UNLOCKED(name.wait_lock),	\
				.wait_list = LIST_HEAD_INIT((name).wait_list),		\
				__RWSEM_DEBUG_INIT(name)							\
				__RWSEM_DEP_MAP_INIT(name)							\
			}

	#define DECLARE_RWSEM(name) \
				rwsem_t name = __RWSEM_INITIALIZER(name)

	// #define init_rwsem(sem)								\
	// 		do {										\
	// 			static struct lock_class_key __key;		\
	// 			__init_rwsem((sem), #sem, &__key);		\
	// 		} while (0)
	#define init_rwsem(sem)						\
			do {								\
				__init_rwsem((sem), #sem);		\
			} while (0)

#endif /* _LINUX_READWRITE_SEMAPHORE_MACRO_H_ */