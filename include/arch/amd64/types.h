#ifndef __X64_TYPES_H__
#define __X64_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

	typedef signed long				off_t;
	typedef signed long long		loff_t;

	typedef unsigned long			irq_flags_t;

	typedef unsigned long long		virtual_addr_t;
	typedef unsigned long long		virtual_size_t;
	typedef unsigned long long		physical_addr_t;
	typedef unsigned long long		physical_size_t;

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
