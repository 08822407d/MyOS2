#ifndef _ASM_X86_LOCK_IPC_TYPE_DECLARATIONS_H_
#define _ASM_X86_LOCK_IPC_TYPE_DECLARATIONS_H_

	struct ucontext;
	typedef struct ucontext ucontext_s;

	#include <linux/kernel/types.h>
	#include <uapi/asm-generic/signal.h>


	/* arch atomic */
	struct atomic;
	typedef struct atomic atomic_t;
	struct atomic64;
	typedef struct atomic64 atomic64_t;
	typedef struct atomic64 atomic_long_t;

	/* arch spinlock */
	struct tspinlock;
	typedef struct tspinlock arch_spinlock_t;
	typedef struct tspinlock spinlock_t;

	/* arch signal */
	struct sigcontext_64;
	typedef struct sigcontext_64 sigctx_64_s;
	struct rt_sigframe;
	typedef struct rt_sigframe rt_sigframe_s;

	/*
	 * Create the real 'struct sigcontext' type:
	 */
	#ifdef __i386__
	#  define sigcontext	sigcontext_32
	#  define sigctx_s		sigctx_32_s
	#else
	#  define sigcontext	sigcontext_64
	#  define sigctx_s		sigctx_64_s
	#endif

#endif /* _ASM_X86_LOCK_IPC_TYPE_DECLARATIONS_H_ */