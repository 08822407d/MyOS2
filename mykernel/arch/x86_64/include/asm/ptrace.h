/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PTRACE_H
#define _ASM_X86_PTRACE_H

	#include <asm/page_types.h>
	#include <asm/uapi_ptrace.h>

	#ifndef __ASSEMBLY__

		typedef struct pt_regs {
		/*
		 * C ABI says these regs are callee-preserved. They aren't saved on kernel entry
		 * unless syscall needs a complete, fully filled "pt_regs_s".
		 */
			reg_t	r15;
			reg_t	r14;
			reg_t	r13;
			reg_t	r12;
			reg_t	bp;
			reg_t	bx;
		/* These regs are callee-clobbered. Always saved on kernel entry. */
			reg_t	r11;
			reg_t	r10;
			reg_t	r9;
			reg_t	r8;
			reg_t	ax;
			reg_t	cx;
			reg_t	dx;
			reg_t	si;
			reg_t	di;

		// myos push irq vec number separately
			reg_t	irq_nr;
		/*
		 * On syscall entry, this is syscall#. On CPU exception, this is error code.
		 * On hw interrupt, it's IRQ number:
		 */
			reg_t	orig_ax;
		/* Return frame for iretq */
			reg_t	ip;
			reg_t	cs;
			reg_t	flags;
			reg_t	sp;
			reg_t	ss;
		/* top of stack page */
		} pt_regs_s;

	// #	ifdef CONFIG_PARAVIRT
	// #		include <asm/paravirt_types.h>
	// #	endif

	// #	include <asm/proto.h>

	// 	cpuinfo_x86_s;
	// 	struct task_struct;

	// 	extern unsigned long profile_pc(pt_regs_s *regs);

	// 	extern unsigned long
	// 	convert_ip_to_linear(task_s *child, pt_regs_s *regs);
	// 	extern void send_sigtrap(pt_regs_s *regs, int error_code, int si_code);


	// 	static inline unsigned long regs_return_value(pt_regs_s *regs) {
	// 		return regs->ax;
	// 	}

	// 	static inline void
	// 	regs_set_return_value(pt_regs_s *regs, unsigned long rc) {
	// 		regs->ax = rc;
	// 	}

		/*
		 * user_mode(regs) determines whether a register set came from user
		 * mode.  On x86_32, this is true if V8086 mode was enabled OR if the
		 * register set was from protected mode with RPL-3 CS value.  This
		 * tricky test checks that with one comparison.
		 *
		 * On x86_64, vm86 mode is mercifully nonexistent, and we don't need
		 * the extra check.
		 */
		static __always_inline int user_mode(pt_regs_s *regs) {
			return !!(regs->cs & 3);
		}

	// 	static inline int v8086_mode(pt_regs_s *regs) {
	// 		return 0;	/* No V86 mode support in long mode */
	// 	}

	// 	static inline bool user_64bit_mode(pt_regs_s *regs) {
	// 	#ifndef CONFIG_PARAVIRT_XXL
	// 		/*
	// 		 * On non-paravirt systems, this is the only long mode CPL 3
	// 		 * selector.  We do not allow long mode selectors in the LDT.
	// 		 */
	// 		return regs->cs == __USER_CS;
	// 	#else
	// 		/* Headers are too twisted for this to go in paravirt.h. */
	// 		return regs->cs == __USER_CS || regs->cs == pv_info.extra_user_64bit_cs;
	// 	#endif
	// 	}

	// 	/*
	// 	 * Determine whether the register set came from any context that is running in
	// 	 * 64-bit mode.
	// 	 */
	// 	static inline bool any_64bit_mode(pt_regs_s *regs) {
	// 		return false;
	// 	}

	// #	define current_user_stack_pointer()	current_pt_regs()->sp
	// #	define compat_user_stack_pointer()	current_pt_regs()->sp

	// 	static __always_inline bool ip_within_syscall_gap(pt_regs_s *regs) {
	// 		bool ret = (regs->ip >= (unsigned long)entry_SYSCALL_64 &&
	// 				regs->ip <  (unsigned long)entry_SYSCALL_64_safe_stack);

	// 	#ifdef CONFIG_IA32_EMULATION
	// 		ret = ret || (regs->ip >= (unsigned long)entry_SYSCALL_compat &&
	// 				regs->ip <  (unsigned long)entry_SYSCALL_compat_safe_stack);
	// 	#endif

	// 		return ret;
	// 	}

	// 	static inline unsigned long kernel_stack_pointer(pt_regs_s *regs) {
	// 		return regs->sp;
	// 	}

	// 	static inline unsigned long instruction_pointer(pt_regs_s *regs) {
	// 		return regs->ip;
	// 	}

	// 	static inline void
	// 	instruction_pointer_set(pt_regs_s *regs, unsigned long val) {
	// 		regs->ip = val;
	// 	}

	// 	static inline unsigned long frame_pointer(pt_regs_s *regs) {
	// 		return regs->bp;
	// 	}

	// 	static inline unsigned long user_stack_pointer(pt_regs_s *regs) {
	// 		return regs->sp;
	// 	}

	// 	static inline void
	// 	user_stack_pointer_set(pt_regs_s *regs, unsigned long val) {
	// 		regs->sp = val;
	// 	}

	// 	static __always_inline bool regs_irqs_disabled(pt_regs_s *regs) {
	// 		return !(regs->flags & X86_EFLAGS_IF);
	// 	}

	// 	/* Query offset/name of register from its name/offset */
	// 	extern int regs_query_register_offset(const char *name);
	// 	extern const char *regs_query_register_name(unsigned int offset);
	// #	define MAX_REG_OFFSET (offsetof(pt_regs_s, ss))

	// 	/**
	// 	 * regs_get_register() - get register value from its offset
	// 	 * @regs:	pt_regs from which register value is gotten.
	// 	 * @offset:	offset number of the register.
	// 	 *
	// 	 * regs_get_register returns the value of a register. The @offset is the
	// 	 * offset of the register in pt_regs_s address which specified by @regs.
	// 	 * If @offset is bigger than MAX_REG_OFFSET, this returns 0.
	// 	 */
	// 	static inline unsigned long
	// 	regs_get_register(pt_regs_s *regs, unsigned int offset) {
	// 		if (unlikely(offset > MAX_REG_OFFSET))
	// 			return 0;
	// 		return *(unsigned long *)((unsigned long)regs + offset);
	// 	}

	// 	/**
	// 	 * regs_within_kernel_stack() - check the address in the stack
	// 	 * @regs:	pt_regs which contains kernel stack pointer.
	// 	 * @addr:	address which is checked.
	// 	 *
	// 	 * regs_within_kernel_stack() checks @addr is within the kernel stack page(s).
	// 	 * If @addr is within the kernel stack, it returns true. If not, returns false.
	// 	 */
	// 	static inline int
	// 	regs_within_kernel_stack(pt_regs_s *regs, unsigned long addr) {
	// 		return ((addr & ~(THREAD_SIZE - 1)) ==
	// 				(regs->sp & ~(THREAD_SIZE - 1)));
	// 	}

	// 	/**
	// 	 * regs_get_kernel_stack_nth_addr() - get the address of the Nth entry on stack
	// 	 * @regs:	pt_regs which contains kernel stack pointer.
	// 	 * @n:		stack entry number.
	// 	 *
	// 	 * regs_get_kernel_stack_nth() returns the address of the @n th entry of the
	// 	 * kernel stack which is specified by @regs. If the @n th entry is NOT in
	// 	 * the kernel stack, this returns NULL.
	// 	 */
	// 	static inline unsigned long
	// 	*regs_get_kernel_stack_nth_addr(pt_regs_s *regs, unsigned int n) {
	// 		unsigned long *addr = (unsigned long *)regs->sp;

	// 		addr += n;
	// 		if (regs_within_kernel_stack(regs, (unsigned long)addr))
	// 			return addr;
	// 		else
	// 			return NULL;
	// 	}

	// 	/* To avoid include hell, we can't include uaccess.h */
	// 	extern long
	// 	copy_from_kernel_nofault(void *dst, const void *src, size_t size);

	// 	/**
	// 	 * regs_get_kernel_stack_nth() - get Nth entry of the stack
	// 	 * @regs:	pt_regs which contains kernel stack pointer.
	// 	 * @n:		stack entry number.
	// 	 *
	// 	 * regs_get_kernel_stack_nth() returns @n th entry of the kernel stack which
	// 	 * is specified by @regs. If the @n th entry is NOT in the kernel stack
	// 	 * this returns 0.
	// 	 */
	// 	static inline unsigned long
	// 	regs_get_kernel_stack_nth(pt_regs_s *regs, unsigned int n) {
	// 		unsigned long *addr;
	// 		unsigned long val;
	// 		long ret;

	// 		addr = regs_get_kernel_stack_nth_addr(regs, n);
	// 		if (addr) {
	// 			ret = copy_from_kernel_nofault(&val, addr, sizeof(val));
	// 			if (!ret)
	// 				return val;
	// 		}
	// 		return 0;
	// 	}

	// 	/**
	// 	 * regs_get_kernel_argument() - get Nth function argument in kernel
	// 	 * @regs:	pt_regs of that context
	// 	 * @n:		function argument number (start from 0)
	// 	 *
	// 	 * regs_get_argument() returns @n th argument of the function call.
	// 	 * Note that this chooses most probably assignment, in some case
	// 	 * it can be incorrect.
	// 	 * This is expected to be called from kprobes or ftrace with regs
	// 	 * where the top of stack is the return address.
	// 	 */
	// 	static inline unsigned long
	// 	regs_get_kernel_argument(pt_regs_s *regs, unsigned int n) {
	// 		static const unsigned int argument_offs[] = {
	// 			offsetof(pt_regs_s, di),
	// 			offsetof(pt_regs_s, si),
	// 			offsetof(pt_regs_s, dx),
	// 			offsetof(pt_regs_s, cx),
	// 			offsetof(pt_regs_s, r8),
	// 			offsetof(pt_regs_s, r9),
	// 	#define NR_REG_ARGUMENTS 6
	// 		};

	// 		if (n >= NR_REG_ARGUMENTS) {
	// 			n -= NR_REG_ARGUMENTS - 1;
	// 			return regs_get_kernel_stack_nth(regs, n);
	// 		} else
	// 			return regs_get_register(regs, argument_offs[n]);
	// 	}

	// #	define arch_has_single_step()		(1)
	// #	ifdef CONFIG_X86_DEBUGCTLMSR
	// #		define arch_has_block_step()	(1)
	// #	else
	// #		define arch_has_block_step()	(boot_cpu_data.x86 >= 6)
	// #	endif
	
	// #	define ARCH_HAS_USER_SINGLE_STEP_REPORT

	// 	struct user_desc;
	// 	extern int
	// 	do_get_thread_area(task_s *p, int idx, struct user_desc __user *info);
	// 	extern int
	// 	do_set_thread_area(task_s *p, int idx, struct user_desc __user *info,
	// 			int can_allocate);

	// #	define do_set_thread_area_64(p, s, t)	do_arch_prctl_64(p, s, t)

	#endif /* !__ASSEMBLY__ */

#endif /* _ASM_X86_PTRACE_H */
