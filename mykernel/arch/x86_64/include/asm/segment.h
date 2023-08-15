/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SEGMENT_H
#define _ASM_X86_SEGMENT_H

	#include <linux/kernel/const.h>
	#include <asm/alternative.h>


	// /*
	//  * Constructor for a conventional segment GDT (or LDT) entry.
	//  * This is a macro so it can be used in initializers.
	//  */
	// #define GDT_ENTRY(flags, base, limit)						\
	// 			((((base)  & _AC(0xff000000,ULL)) << (56-24)) |	\
	// 			(((flags) & _AC(0x0000f0ff,ULL)) << 40) |		\
	// 			(((limit) & _AC(0x000f0000,ULL)) << (48-16)) |	\
	// 			(((base)  & _AC(0x00ffffff,ULL)) << 16) |		\
	// 			(((limit) & _AC(0x0000ffff,ULL))))

	// /* Simple and small GDT entries for booting only: */

	// #define GDT_ENTRY_BOOT_CS				2
	// #define GDT_ENTRY_BOOT_DS				3
	// #define GDT_ENTRY_BOOT_TSS				4
	// #define __BOOT_CS						(GDT_ENTRY_BOOT_CS*8)
	// #define __BOOT_DS						(GDT_ENTRY_BOOT_DS*8)
	// #define __BOOT_TSS						(GDT_ENTRY_BOOT_TSS*8)

	/*
	 * Bottom two bits of selector give the ring
	 * privilege level
	 */
	#define SEGMENT_RPL_MASK				0x3

	/*
	 * When running on Xen PV, the actual privilege level of the kernel is 1,
	 * not 0. Testing the Requested Privilege Level in a segment selector to
	 * determine whether the context is user mode or kernel mode with
	 * SEGMENT_RPL_MASK is wrong because the PV kernel's privilege level
	 * matches the 0x3 mask.
	 *
	 * Testing with USER_SEGMENT_RPL_MASK is valid for both native and Xen PV
	 * kernels because privilege level 2 is never used.
	 */
	#define USER_SEGMENT_RPL_MASK			0x2

	/* Kernel mode is privilege level 0: */
	#define KRNL_RPL						0x0
	/* User mode is privilege level 3: */
	#define USER_RPL						0x3

	// /* Bit 2 is Table Indicator (TI): selects between LDT or GDT */
	// #define SEGMENT_TI_MASK					0x4
	// /* LDT segment has TI set ... */
	// #define SEGMENT_LDT						0x4
	// /* ... GDT has it cleared */
	// #define SEGMENT_GDT						0x0

	#define GDT_ENTRY_INVALID_SEG			0

	#include <asm/cache.h>

	#if defined(CONFIG_INTER_X64_GDT_LAYOUT)
	#	define GDT_ENTRY_KERNEL_CS				1
	#	define GDT_ENTRY_KERNEL_DS				2
	#	define GDT_ENTRY_DEFAULT_USER_DS		3
	#	define GDT_ENTRY_DEFAULT_USER_CS		4
	#	define GDT_ENTRY_DEFAULT_USER_CS_DUP	5
	#	define GDT_ENTRY_DEFAULT_USER_DS_DUP	6
	#else
	#	define GDT_ENTRY_KERNEL32_CS			1
	#	define GDT_ENTRY_KERNEL_CS				2
	#	define GDT_ENTRY_KERNEL_DS				3

	/*
	 * We cannot use the same code segment descriptor for user and kernel mode,
	 * not even in long flat mode, because of different DPL.
	 *
	 * GDT layout to get 64-bit SYSCALL/SYSRET support right. SYSRET hardcodes
	 * selectors:
	 *
	 *   if returning to 32-bit userspace: cs = STAR.SYSRET_CS,
	 *   if returning to 64-bit userspace: cs = STAR.SYSRET_CS+16,
	 *
	 * ss = STAR.SYSRET_CS+8 (in either case)
	 *
	 * thus USER_DS should be between 32-bit and 64-bit code selectors:
	 */
	#	define GDT_ENTRY_DEFAULT_USER32_CS		4
	#	define GDT_ENTRY_DEFAULT_USER_DS		5
	#	define GDT_ENTRY_DEFAULT_USER_CS		6
	#endif

	/* Needs two entries */
	#define GDT_ENTRY_TSS					8
	/* Needs two entries */
	#define GDT_ENTRY_LDT					10

	#define GDT_ENTRY_TLS_MIN				12
	#define GDT_ENTRY_TLS_MAX				14

	#define GDT_ENTRY_CPUNODE				15

	/*
	 * Number of entries in the GDT table:
	 */
	#define GDT_ENTRIES						16

	#if defined(CONFIG_INTER_X64_GDT_LAYOUT)
	#	define __KERNEL_CS			(GDT_ENTRY_KERNEL_CS * 8)
	#	define __KERNEL_DS			(GDT_ENTRY_KERNEL_DS * 8)
	#	define __USER_DS			(GDT_ENTRY_DEFAULT_USER_DS * 8 + 3)
	#	define __USER_CS			(GDT_ENTRY_DEFAULT_USER_CS * 8 + 3)
	#	define __USER_CS_DUP		(GDT_ENTRY_DEFAULT_USER_CS_DUP * 8 + 3)
	#	define __USER_DS_DUP		(GDT_ENTRY_DEFAULT_USER_DS_DUP * 8 + 3)
	#else
	/*
	 * Segment selector values corresponding to the above entries:
	 *
	 * Note, selectors also need to have a correct RPL,
	 * expressed with the +3 value for user-space selectors:
	 */
	#	define __KERNEL32_CS		(GDT_ENTRY_KERNEL32_CS * 8)
	#	define __KERNEL_CS			(GDT_ENTRY_KERNEL_CS * 8)
	#	define __KERNEL_DS			(GDT_ENTRY_KERNEL_DS * 8)
	#	define __USER32_CS			(GDT_ENTRY_DEFAULT_USER32_CS * 8 + 3)
	#	define __USER_DS			(GDT_ENTRY_DEFAULT_USER_DS * 8 + 3)
	#	define __USER32_DS			__USER_DS
	#	define __USER_CS			(GDT_ENTRY_DEFAULT_USER_CS * 8 + 3)
	#	define __CPUNODE_SEG		(GDT_ENTRY_CPUNODE * 8 + 3)
	#endif
	#define __TSS_SEG				(GDT_ENTRY_TSS * 8)

	#define IDT_ENTRIES						256
	#define NUM_EXCEPTION_VECTORS			32

	// /* Bitmask of exception vectors which push an error code on the stack: */
	// #define EXCEPTION_ERRCODE_MASK		0x20027d00

	#define GDT_SIZE						(GDT_ENTRIES * 8)
	// #define GDT_ENTRY_TLS_ENTRIES			3
	// #define TLS_SIZE						(GDT_ENTRY_TLS_ENTRIES* 8)

	// /* Bit size and mask of CPU number stored in the per CPU data (and TSC_AUX) */
	// #define VDSO_CPUNODE_BITS				12
	// #define VDSO_CPUNODE_MASK				0xfff

	// #ifndef __ASSEMBLY__

	// /* Helper functions to store/load CPU and node numbers */

	// static inline unsigned long
	// vdso_encode_cpunode(int cpu, unsigned long node) {
	// 	return (node << VDSO_CPUNODE_BITS) | cpu;
	// }

	// static inline void
	// vdso_read_cpunode(unsigned *cpu, unsigned *node) {
	// 	unsigned int p;

	// 	/*
	// 	 * Load CPU and node number from the GDT.  LSL is faster than RDTSCP
	// 	 * and works on all CPUs.  This is volatile so that it orders
	// 	 * correctly with respect to barrier() and to keep GCC from cleverly
	// 	 * hoisting it out of the calling function.
	// 	 *
	// 	 * If RDPID is available, use it.
	// 	 */
	// 	alternative_io ("lsl %[seg],%[p]",
	// 			".byte 0xf3,0x0f,0xc7,0xf8", /* RDPID %eax/rax */
	// 			X86_FEATURE_RDPID,
	// 			[p] "=a" (p), [seg] "r" (__CPUNODE_SEG));

	// 	if (cpu)
	// 		*cpu = (p & VDSO_CPUNODE_MASK);
	// 	if (node)
	// 		*node = (p >> VDSO_CPUNODE_BITS);
	// }

	// #endif /* !__ASSEMBLY__ */

	#ifdef __KERNEL__

	/*
	 * early_idt_handler_array is an array of entry points referenced in the
	 * early IDT.  For simplicity, it's a real array with one entry point
	 * every nine bytes.  That leaves room for an optional 'push $0' if the
	 * vector has no error code (two bytes), a 'push $vector_number' (two
	 * bytes), and a jump to the common entry code (up to five bytes).
	 */
	#	define EARLY_IDT_HANDLER_SIZE		9

	// /*
	//  * xen_early_idt_handler_array is for Xen pv guests: for each entry in
	//  * early_idt_handler_array it contains a prequel in the form of
	//  * pop %rcx; pop %r11; jmp early_idt_handler_array[i]; summing up to
	//  * max 8 bytes.
	//  */
	// #	define XEN_EARLY_IDT_HANDLER_SIZE	8

	#	ifndef __ASSEMBLY__

			extern const char early_idt_handler_array
					[NUM_EXCEPTION_VECTORS][EARLY_IDT_HANDLER_SIZE];
	// 		extern void early_ignore_irq(void);

	// #		ifdef CONFIG_XEN_PV
	// 			extern const char xen_early_idt_handler_array
	// 					[NUM_EXCEPTION_VECTORS][XEN_EARLY_IDT_HANDLER_SIZE];
	// #		endif

	/*
	 * Load a segment. Fall back on loading the zero segment if something goes
	 * wrong.  This variant assumes that loading zero fully clears the segment.
	 * This is always the case on Intel CPUs and, even on 64-bit AMD CPUs, any
	 * failure to fully clear the cached descriptor is only observable for
	 * FS and GS.
	 */
	// #		define __loadsegment_simple(seg, value)						\
	// 				do {												\
	// 					unsigned short __val = (value);					\
	// 					asm volatile(	"							\n"	\
	// 									"1:	movl %k0,%%" #seg "		\n"	\
	// 									_ASM_EXTABLE_TYPE_REG(1b, 1b, EX_TYPE_ZERO_REG, %k0)\
	// 								:	"+r" (__val)					\
	// 								:									\
	// 								:	"memory");						\
	// 				} while (0)
	#		define __loadsegment_simple(seg, value)						\
					do {												\
						unsigned short __val = (value);					\
						asm volatile(	"movl	%k0,	%%" #seg "	\n"	\
									:	"+r" (__val)					\
									:									\
									:	"memory");						\
					} while (0)

	#		define __loadsegment_ss(value) __loadsegment_simple(ss, (value))
	#		define __loadsegment_ds(value) __loadsegment_simple(ds, (value))
	#		define __loadsegment_es(value) __loadsegment_simple(es, (value))

			static inline void __loadsegment_fs(unsigned short value) {
				asm volatile(	"						\n"
								"1:	movw %0, %%fs		\n"
								"2:						\n"
								// _ASM_EXTABLE_TYPE(1b, 2b, EX_TYPE_CLEAR_FS)

							:
							:	"rm" (value)
							:	"memory");
			}

	/* __loadsegment_gs is intentionally undefined.  Use load_gs_index instead. */

	#		define loadsegment(seg, value) __loadsegment_ ## seg (value)

	/*
	 * Save a segment register away:
	 */
	#		define savesegment(seg, value) \
					asm("mov %%" #seg ",%0":"=r" (value) : : "memory")

	#	endif /* !__ASSEMBLY__ */
	#endif /* __KERNEL__ */

#endif /* _ASM_X86_SEGMENT_H */
