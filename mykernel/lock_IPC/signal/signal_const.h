#ifndef _SIGNAL_CONSTS_H_
#define _SIGNAL_CONSTS_H_


	/*
	 * How these fields are to be accessed.
	 */
	#define si_pid				_sifields._kill._pid
	#define si_uid				_sifields._kill._uid
	#define si_tid				_sifields._timer._tid
	#define si_overrun			_sifields._timer._overrun
	#define si_sys_private  	_sifields._timer._sys_private
	#define si_status			_sifields._sigchld._status
	#define si_utime			_sifields._sigchld._utime
	#define si_stime			_sifields._sigchld._stime
	#define si_value			_sifields._rt._sigval
	#define si_int				_sifields._rt._sigval.sival_int
	#define si_ptr				_sifields._rt._sigval.sival_ptr
	#define si_addr				_sifields._sigfault._addr
	#define si_trapno			_sifields._sigfault._trapno
	#define si_addr_lsb			_sifields._sigfault._addr_lsb
	#define si_lower			_sifields._sigfault._addr_bnd._lower
	#define si_upper			_sifields._sigfault._addr_bnd._upper
	#define si_pkey				_sifields._sigfault._addr_pkey._pkey
	#define si_perf_data		_sifields._sigfault._perf._data
	#define si_perf_type		_sifields._sigfault._perf._type
	#define si_perf_flags		_sifields._sigfault._perf._flags
	#define si_band				_sifields._sigpoll._band
	#define si_fd				_sifields._sigpoll._fd
	#define si_call_addr		_sifields._sigsys._call_addr
	#define si_syscall			_sifields._sigsys._syscall
	#define si_arch				_sifields._sigsys._arch

	/*
	 * si_code values
	 * Digital reserves positive values for kernel-generated signals.
	 */
	#define SI_USER					0		/* sent by kill, sigsend, raise */
	#define SI_KERNEL				0x80	/* sent by the kernel from somewhere */
	#define SI_QUEUE				-1		/* sent by sigqueue */
	#define SI_TIMER				-2		/* sent by timer expiration */
	#define SI_MESGQ				-3		/* sent by real time mesq state change */
	#define SI_ASYNCIO				-4		/* sent by AIO completion */
	#define SI_SIGIO				-5		/* sent by queued SIGIO */
	#define SI_TKILL				-6		/* sent by tkill system call */
	#define SI_DETHREAD				-7		/* sent by execve() killing subsidiary threads */
	#define SI_ASYNCNL				-60		/* sent by glibc async name lookup completion */

	#define SI_FROMUSER(siptr)		((siptr)->si_code <= 0)
	#define SI_FROMKERNEL(siptr)	((siptr)->si_code > 0)

	/*
	 * SIGILL si_codes
	 */
	#define ILL_ILLOPC				1	/* illegal opcode */
	#define ILL_ILLOPN				2	/* illegal operand */
	#define ILL_ILLADR				3	/* illegal addressing mode */
	#define ILL_ILLTRP				4	/* illegal trap */
	#define ILL_PRVOPC				5	/* privileged opcode */
	#define ILL_PRVREG				6	/* privileged register */
	#define ILL_COPROC				7	/* coprocessor error */
	#define ILL_BADSTK				8	/* internal stack error */
	#define ILL_BADIADDR			9	/* unimplemented instruction address */
	#define __ILL_BREAK				10	/* illegal break */
	#define __ILL_BNDMOD			11	/* bundle-update (modification) in progress */
	#define NSIGILL					11

#endif /* _SIGNAL_CONSTS_H_ */