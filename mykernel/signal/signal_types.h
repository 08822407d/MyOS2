#ifndef _SIGNAL_TYPES_H_
#define _SIGNAL_TYPES_H_

	#include "signal_type_declaration.h"


	typedef union sigval {
		int			sival_int;
		void __user	*sival_ptr;
	} sigval_t;

	/*
	 * The default "si_band" type is "long", as specified by POSIX.
	 * However, some architectures want to override this to "int"
	 * for historical compatibility reasons, so we allow that.
	 */
	#ifndef __ARCH_SI_BAND_T
	#  define __ARCH_SI_BAND_T		long
	#endif

	#ifndef __ARCH_SI_CLOCK_T
	#  define __ARCH_SI_CLOCK_T		__kernel_clock_t
	#endif

	#ifndef __ARCH_SI_ATTRIBUTES
	#  define __ARCH_SI_ATTRIBUTES
	#endif

	/*
	 * Be careful when extending this union.  On 32bit siginfo_t is 32bit
	 * aligned.  Which means that a 64bit field or any other field that
	 * would increase the alignment of siginfo_t will break the ABI.
	 */
	typedef union __sifields {
		/* kill() */
		struct {
			__kernel_pid_t		_pid;			/* sender's pid */
			__kernel_uid32_t	_uid;			/* sender's uid */
		} _kill;

		/* POSIX.1b timers */
		struct {
			__kernel_timer_t	_tid;			/* timer id */
			int					_overrun;		/* overrun count */
			sigval_t			_sigval;		/* same as below */
			int					_sys_private;	/* not to be passed to user */
		} _timer;

		/* POSIX.1b signals */
		struct {
			__kernel_pid_t		_pid;			/* sender's pid */
			__kernel_uid32_t	_uid;			/* sender's uid */
			sigval_t _sigval;
		} _rt;

		/* SIGCHLD */
		struct {
			__kernel_pid_t		_pid;			/* which child */
			__kernel_uid32_t	_uid;			/* sender's uid */
			int					_status;		/* exit code */
			__ARCH_SI_CLOCK_T	_utime;
			__ARCH_SI_CLOCK_T	_stime;
		} _sigchld;

		// /* SIGILL, SIGFPE, SIGSEGV, SIGBUS, SIGTRAP, SIGEMT */
		// struct {
		// 	void __user *_addr; /* faulting insn/memory ref. */

	// #define __ADDR_BND_PKEY_PAD	(						\
	// 			__alignof__(void *) < sizeof(short) ?	\
	// 				sizeof(short) : __alignof__(void *)	\
	// 		)
		// 	union {
		// 		/* used on alpha and sparc */
		// 		int _trapno;	/* TRAP # which caused the signal */
		// 		/*
		// 		 * used when si_code=BUS_MCEERR_AR or
		// 		 * used when si_code=BUS_MCEERR_AO
		// 		 */
		// 		short _addr_lsb; /* LSB of the reported address */
		// 		/* used when si_code=SEGV_BNDERR */
		// 		struct {
		// 			char _dummy_bnd[__ADDR_BND_PKEY_PAD];
		// 			void __user *_lower;
		// 			void __user *_upper;
		// 		} _addr_bnd;
		// 		/* used when si_code=SEGV_PKUERR */
		// 		struct {
		// 			char _dummy_pkey[__ADDR_BND_PKEY_PAD];
		// 			__u32 _pkey;
		// 		} _addr_pkey;
		// 		/* used when si_code=TRAP_PERF */
		// 		struct {
		// 			unsigned long _data;
		// 			__u32 _type;
		// 			__u32 _flags;
		// 		} _perf;
		// 	};
		// } _sigfault;

		/* SIGPOLL */
		struct {
			__ARCH_SI_BAND_T	_band;			/* POLL_IN, POLL_OUT, POLL_MSG */
			int _fd;
		} _sigpoll;

		/* SIGSYS */
		struct {
			void __user			*_call_addr;	/* calling user insn */
			int					_syscall;		/* triggering system call number */
			uint				_arch;			/* AUDIT_ARCH_* of syscall */
		} _sigsys;
	} __sifields_u;

	typedef struct kernel_siginfo {
		int				si_signo;
		int				si_errno;
		int				si_code;
		__sifields_u	_sifields;
	} kernel_siginfo_t;

#endif /* _SIGNAL_TYPES_H_ */