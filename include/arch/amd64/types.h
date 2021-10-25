#ifndef __X64_TYPES_H__
#define __X64_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 *	definations for MyOS2
	 */
	typedef __UINT64_TYPE__ reg_t;
	typedef __UINT64_TYPE__ bitmap_t;
	#define BITMAP_UNITSIZE 64
	typedef void*	phys_addr;
	typedef void*	virt_addr;


	typedef signed char				s8_t;
	typedef unsigned char			u8_t;

	typedef signed short			s16_t;
	typedef unsigned short			u16_t;

	typedef signed int				s32_t;
	typedef unsigned int			u32_t;

	typedef signed long long		s64_t;
	typedef unsigned long long		u64_t;

	typedef signed long long		intmax_t;
	typedef unsigned long long		uintmax_t;

	typedef signed long	long		ptrdiff_t;
	typedef signed long	long		intptr_t;
	typedef unsigned long long		uintptr_t;

	typedef unsigned long long		size_t;
	typedef signed long	long		ssize_t;

	typedef signed long				off_t;
	typedef signed long long		loff_t;

	typedef signed int				bool_t;
	typedef unsigned long			irq_flags_t;

	typedef unsigned long long		virtual_addr_t;
	typedef unsigned long long		virtual_size_t;
	typedef unsigned long long		physical_addr_t;
	typedef unsigned long long		physical_size_t;

	typedef	s32_t					pid_t;		/* process id */
	typedef u64_t					dev_t;

	typedef struct {
		volatile int counter;
	} atomic_t;

	typedef struct {
		volatile int lock;
	} spinlock_t;

#ifdef __cplusplus
}
#endif

#endif /* __X64_TYPES_H__ */
