/* SPDX-License-Identifier: GPL-2.0 */
/*
 * syscall_wrapper.h - x86 specific wrappers to syscall definitions
 */

#ifndef _ASM_X86_SYSCALL_WRAPPER_H
#define _ASM_X86_SYSCALL_WRAPPER_H

	// pt_regs_s;

	extern long __x64_sys_ni_syscall(const pt_regs_s *regs);
	// extern long __ia32_sys_ni_syscall(const pt_regs_s *regs);

	#define __X86_64_MAP0(m, ...)
	#define __X86_64_MAP1(m, t1, a1)											\
				m(t1, regs->di)
	#define __X86_64_MAP2(m, t1, a1, t2, a2)									\
				m(t1, regs->di), m(t2, regs->si)
	#define __X86_64_MAP3(m, t1, a1, t2, a2, t3, a3)							\
				m(t1, regs->di), m(t2, regs->si), m(t3, regs->dx)
	#define __X86_64_MAP4(m, t1, a1, t2, a2, t3, a3, t4, a4)					\
				m(t1, regs->di), m(t2, regs->si), m(t3, regs->dx),				\
				m(t4, regs->r10)
	#define __X86_64_MAP5(m, t1, a1, t2, a2, t3, a3, t4, a4, t5, a5)			\
				m(t1, regs->di), m(t2, regs->si), m(t3, regs->dx),				\
				m(t4, regs->r10), m(t5, regs->r8)
	#define __X86_64_MAP6(m, t1, a1, t2, a2, t3, a3, t4, a4, t5, a5, t6, a6)	\
				m(t1, regs->di), m(t2, regs->si), m(t3, regs->dx),				\
				m(t4, regs->r10), m(t5, regs->r8), m(t6, regs->r9)
	#define __ARCH_MAP(n, ...)	__X86_64_MAP##n(__VA_ARGS__)


	/*
	 * Instead of the generic __SYSCALL_DEFINEx() definition, the x86 version takes
	 * pt_regs_s *regs as the only argument of the syscall stub(s) named as:
	 * __x64_sys_*()         - 64-bit native syscall
	 * __ia32_sys_*()        - 32-bit native syscall or common compat syscall
	 * __ia32_compat_sys_*() - 32-bit compat syscall
	 * __x64_compat_sys_*()  - 64-bit X32 compat syscall
	 *
	 * The registers are decoded according to the ABI:
	 * 64-bit: RDI, RSI, RDX, R10, R8, R9
	 * 32-bit: EBX, ECX, EDX, ESI, EDI, EBP
	 *
	 * The stub then passes the decoded arguments to the __se_sys_*() wrapper to
	 * perform sign-extension (omitted for zero-argument syscalls).  Finally the
	 * arguments are passed to the __do_sys_*() function which is the actual
	 * syscall.  These wrappers are marked as inline so the compiler can optimize
	 * the functions where appropriate.
	 *
	 * Example assembly (slightly re-ordered for better readability):
	 *
	 * <__x64_sys_recv>:		<-- syscall with 4 parameters
	 *	callq	<__fentry__>
	 *
	 *	mov	0x70(%rdi),%rdi	<-- decode regs->di
	 *	mov	0x68(%rdi),%rsi	<-- decode regs->si
	 *	mov	0x60(%rdi),%rdx	<-- decode regs->dx
	 *	mov	0x38(%rdi),%rcx	<-- decode regs->r10
	 *
	 *	xor	%r9d,%r9d	<-- clear %r9
	 *	xor	%r8d,%r8d	<-- clear %r8
	 *
	 *	callq	__sys_recvfrom	<-- do the actual work in __sys_recvfrom()
	 *				    which takes 6 arguments
	 *
	 *	cltq			<-- extend return value to 64-bit
	 *	retq			<-- return
	 *
	 * This approach avoids leaking random user-provided register content down
	 * the call chain.
	 */

	/* Mapping of registers to parameters for syscalls on x86-64 and x32 */
	// #define SC_X86_64_REGS_TO_ARGS(x, ...)				\
	// 			__MAP(x,__SC_ARGS						\
	// 				,,regs->di,,regs->si,,regs->dx		\
	// 				,,regs->r10,,regs->r8,,regs->r9)	\

	// /* Mapping of registers to parameters for syscalls on i386 */
	// #define SC_IA32_REGS_TO_ARGS(x, ...)					\
	// 	__MAP(x,__SC_ARGS						\
	// 		,,(unsigned int)regs->bx,,(unsigned int)regs->cx		\
	// 		,,(unsigned int)regs->dx,,(unsigned int)regs->si		\
	// 		,,(unsigned int)regs->di,,(unsigned int)regs->bp)

	// #define __SYS_STUB0(abi, name)						\
	// 	long __##abi##_##name(const pt_regs_s *regs);		\
	// 	ALLOW_ERROR_INJECTION(__##abi##_##name, ERRNO);			\
	// 	long __##abi##_##name(const pt_regs_s *regs)		\
	// 		__alias(__do_##name);
	#define __SYS_STUB0(abi, name)								\
				long __##abi##_##name(const pt_regs_s *regs);	\
				long __##abi##_##name(const pt_regs_s *regs)	\
					__alias(__do_##name);

	// #define __SYS_STUBx(abi, name, ...)							\
	// 			long __##abi##_##name(const pt_regs_s *regs);	\
	// 			ALLOW_ERROR_INJECTION(__##abi##_##name, ERRNO);	\
	// 			long __##abi##_##name(const pt_regs_s *regs)	\
	// 			{												\
	// 				return __se_##name(__VA_ARGS__);			\
	// 			}	
	#define __SYS_STUBx(abi, name, ...)							\
				long __##abi##_##name(const pt_regs_s *regs);	\
				long __##abi##_##name(const pt_regs_s *regs)	\
				{												\
					return __se_##name(regs);					\
				}	

	// #define __COND_SYSCALL(abi, name)					\
	// 	__weak long __##abi##_##name(const pt_regs_s *__unused);	\
	// 	__weak long __##abi##_##name(const pt_regs_s *__unused)	\
	// 	{								\
	// 		return sys_ni_syscall();				\
	// 	}

	// #define __SYS_NI(abi, name)						\
	// 	SYSCALL_ALIAS(__##abi##_##name, sys_ni_posix_timers);

	#define __X64_SYS_STUB0(name)								\
				__SYS_STUB0(x64, sys_##name)

	// #define __X64_SYS_STUBx(x, name, ...)						\
	// 			__SYS_STUBx(x64, sys##name,						\
	// 				SC_X86_64_REGS_TO_ARGS(x, __VA_ARGS__))

	// #define __X64_COND_SYSCALL(name)					\
	// 	__COND_SYSCALL(x64, sys_##name)

	// #define __X64_SYS_NI(name)						\
	// 	__SYS_NI(x64, sys_##name)

	// #define __IA32_SYS_STUB0(name)
	// #define __IA32_SYS_STUBx(x, name, ...)
	// #define __IA32_COND_SYSCALL(name)
	// #define __IA32_SYS_NI(name)

	// #define __IA32_COMPAT_SYS_STUB0(name)
	// #define __IA32_COMPAT_SYS_STUBx(x, name, ...)
	// #define __IA32_COMPAT_COND_SYSCALL(name)
	// #define __IA32_COMPAT_SYS_NI(name)

	// #define __X32_COMPAT_SYS_STUB0(name)
	// #define __X32_COMPAT_SYS_STUBx(x, name, ...)
	// #define __X32_COMPAT_COND_SYSCALL(name)
	// #define __X32_COMPAT_SYS_NI(name)


	// #ifdef CONFIG_COMPAT
	// /*
	// * Compat means IA32_EMULATION and/or X86_X32. As they use a different
	// * mapping of registers to parameters, we need to generate stubs for each
	// * of them.
	// */
	// #define COMPAT_SYSCALL_DEFINE0(name)					\
	// 	static long							\
	// 	__do_compat_sys_##name(const pt_regs_s *__unused);		\
	// 	__IA32_COMPAT_SYS_STUB0(name)					\
	// 	__X32_COMPAT_SYS_STUB0(name)					\
	// 	static long							\
	// 	__do_compat_sys_##name(const pt_regs_s *__unused)

	// #define COMPAT_SYSCALL_DEFINEx(x, name, ...)					\
	// 	static long __se_compat_sys##name(__MAP(x,__SC_LONG,__VA_ARGS__));	\
	// 	static inline long __do_compat_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));\
	// 	__IA32_COMPAT_SYS_STUBx(x, name, __VA_ARGS__)				\
	// 	__X32_COMPAT_SYS_STUBx(x, name, __VA_ARGS__)				\
	// 	static long __se_compat_sys##name(__MAP(x,__SC_LONG,__VA_ARGS__))	\
	// 	{									\
	// 		return __do_compat_sys##name(__MAP(x,__SC_DELOUSE,__VA_ARGS__));\
	// 	}									\
	// 	static inline long __do_compat_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))

	// /*
	// * As some compat syscalls may not be implemented, we need to expand
	// * COND_SYSCALL_COMPAT in kernel/sys_ni.c and COMPAT_SYS_NI in
	// * kernel/time/posix-stubs.c to cover this case as well.
	// */
	// #define COND_SYSCALL_COMPAT(name) 					\
	// 	__IA32_COMPAT_COND_SYSCALL(name)				\
	// 	__X32_COMPAT_COND_SYSCALL(name)

	// #define COMPAT_SYS_NI(name)						\
	// 	__IA32_COMPAT_SYS_NI(name)					\
	// 	__X32_COMPAT_SYS_NI(name)

	// #endif /* CONFIG_COMPAT */

	// #define __SYSCALL_DEFINEx(x, name, ...)					\
	// 	static long __se_sys##name(__MAP(x,__SC_LONG,__VA_ARGS__));	\
	// 	static inline long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));\
	// 	__X64_SYS_STUBx(x, name, __VA_ARGS__)				\
	// 	__IA32_SYS_STUBx(x, name, __VA_ARGS__)				\
	// 	static long __se_sys##name(__MAP(x,__SC_LONG,__VA_ARGS__))	\
	// 	{								\
	// 		long ret = __do_sys##name(__MAP(x,__SC_CAST,__VA_ARGS__));\
	// 		__MAP(x,__SC_TEST,__VA_ARGS__);				\
	// 		__PROTECT(x, ret,__MAP(x,__SC_ARGS,__VA_ARGS__));	\
	// 		return ret;						\
	// 	}								\
	// 	static inline long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))

	// /*
	// * As the generic SYSCALL_DEFINE0() macro does not decode any parameters for
	// * obvious reasons, and passing pt_regs_s *regs to it in %rdi does not
	// * hurt, we only need to re-define it here to keep the naming congruent to
	// * SYSCALL_DEFINEx() -- which is essential for the COND_SYSCALL() and SYS_NI()
	// * macros to work correctly.
	// */
	// #define SYSCALL_DEFINE0(sname)						\
	// 	SYSCALL_METADATA(_##sname, 0);					\
	// 	static long __do_sys_##sname(const pt_regs_s *__unused);	\
	// 	__X64_SYS_STUB0(sname)						\
	// 	__IA32_SYS_STUB0(sname)						\
	// 	static long __do_sys_##sname(const pt_regs_s *__unused)

	// #define COND_SYSCALL(name)						\
	// 	__X64_COND_SYSCALL(name)					\
	// 	__IA32_COND_SYSCALL(name)

	// #define SYS_NI(name)							\
	// 	__X64_SYS_NI(name)						\
	// 	__IA32_SYS_NI(name)


	// /*
	// * For VSYSCALLS, we need to declare these three syscalls with the new
	// * pt_regs-based calling convention for in-kernel use.
	// */
	// long __x64_sys_getcpu(const pt_regs_s *regs);
	// long __x64_sys_gettimeofday(const pt_regs_s *regs);
	// long __x64_sys_time(const pt_regs_s *regs);


	// #define MYOS_SYSCALL_DEFINE0(sname)									\
	// 			long __do_sys_##sname(const pt_regs_s *__unused);		\
	// 			__X64_SYS_STUB0(sname)									\
	// 			long __do_sys_##sname(const pt_regs_s *__unused)
	
	#define __MYOS_SYSCALL_DEFINEx(x, name, ...)						\
				long __se_sys##name(const pt_regs_s *regs);				\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
				__SYS_STUBx(x64, sys##name)								\
				long __se_sys##name(const pt_regs_s *regs)				\
				{														\
					long ret = __do_sys##name(							\
								__ARCH_MAP(x,__SC_CAST,__VA_ARGS__));	\
					return ret;											\
				}														\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))


#endif /* _ASM_X86_SYSCALL_WRAPPER_H */
