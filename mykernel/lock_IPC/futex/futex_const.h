#ifndef _LINUX_FUTEX_CONST_H_
#define _LINUX_FUTEX_CONST_H_

	/*
	 * Futex flags used to encode options to functions and preserve them across
	 * restarts.
	 */
	#define FLAGS_SIZE_8		0x0000
	#define FLAGS_SIZE_16		0x0001
	#define FLAGS_SIZE_32		0x0002
	#define FLAGS_SIZE_64		0x0003

	#define FLAGS_SIZE_MASK		0x0003

	#ifdef CONFIG_MMU
	#  define FLAGS_SHARED		0x0010
	#else
		/*
		 * NOMMU does not have per process address space. Let the compiler optimize
		 * code away.
		 */
	#  define FLAGS_SHARED		0x0000
	#endif
	#define FLAGS_CLOCKRT		0x0020
	#define FLAGS_HAS_TIMEOUT	0x0040
	#define FLAGS_NUMA			0x0080
	#define FLAGS_STRICT		0x0100

#endif /* _LINUX_FUTEX_CONST_H_ */