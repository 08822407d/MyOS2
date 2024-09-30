#ifndef _LINUX_LOCKDEP_MACRO_H_
#define _LINUX_LOCKDEP_MACRO_H_


	#ifndef CONFIG_LOCK_STAT

	#  define lock_contended(lockdep_map, ip)	do {} while (0)
	#  define lock_acquired(lockdep_map, ip)	do {} while (0)

	#  define LOCK_CONTENDED(_lock, try, lock)			lock(_lock)
	#  define LOCK_CONTENDED_RETURN(_lock, try, lock)	lock(_lock)

	#endif /* CONFIG_LOCK_STAT */

#endif /* _LINUX_LOCKDEP_MACRO_H_ */