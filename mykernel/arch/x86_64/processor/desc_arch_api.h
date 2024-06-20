#ifndef _ASM_X86_DESC_API_H_
#define _ASM_X86_DESC_API_H_

	#include "desc_const_arch.h"
	#include "desc_types_arch.h"
	#include "desc_arch.h"

	extern struct desc_ptr idt_descr;
	extern unsigned long system_vectors[];


	#define load_tr(tr)			asm volatile("ltr %0"::"m" (tr))
	#define load_ldt(ldt)		asm volatile("lldt %0"::"m" (ldt))
	#define load_current_idt()	load_idt(&idt_descr)

	/*
	 * Constructor for a conventional segment GDT (or LDT) entry.
	 * This is a macro so it can be used in initializers.
	 */
	#define GDT_ENTRY(flags, base, limit)	(					\
				(((base)  & _AC(0xff000000,ULL)) << (56-24)) |	\
				(((flags) & _AC(0x0000f0ff,ULL)) << 40) |		\
				(((limit) & _AC(0x000f0000,ULL)) << (48-16)) |	\
				(((base)  & _AC(0x00ffffff,ULL)) << 16) |		\
				(((limit) & _AC(0x0000ffff,ULL)))				\
			)

	#  define GDT_ENTRY_INIT(flags, base, limit)	{		\
					.limit0		= (u16) (limit),			\
					.limit1		= ((limit) >> 16) & 0x0F,	\
					.base0		= (u16) (base),				\
					.base1		= ((base) >> 16) & 0xFF,	\
					.base2		= ((base) >> 24) & 0xFF,	\
					.type		= (flags & 0x0f),			\
					.s			= (flags >> 4) & 0x01,		\
					.dpl		= (flags >> 5) & 0x03,		\
					.p			= (flags >> 7) & 0x01,		\
					.avl		= (flags >> 12) & 0x01,		\
					.l			= (flags >> 13) & 0x01,		\
					.d			= (flags >> 14) & 0x01,		\
					.g			= (flags >> 15) & 0x01,		\
				}

	/*
	 * Load a segment. Fall back on loading the zero segment if something goes
	 * wrong.  This variant assumes that loading zero fully clears the segment.
	 * This is always the case on Intel CPUs and, even on 64-bit AMD CPUs, any
	 * failure to fully clear the cached descriptor is only observable for
	 * FS and GS.
	 */
	#define __loadsegment_simple(seg, value)					\
			do {												\
				unsigned short __val = (value);					\
				asm volatile(	"movl	%k0,	%%" #seg "	\n"	\
							:	"+r" (__val)					\
							:									\
							:	"memory");						\
			} while (0)

	#define __loadsegment_ss(value) __loadsegment_simple(ss, (value))
	#define __loadsegment_ds(value) __loadsegment_simple(ds, (value))
	#define __loadsegment_es(value) __loadsegment_simple(es, (value))


	/* __loadsegment_gs is intentionally undefined.  Use load_gs_index instead. */
	#define loadsegment(seg, value) __loadsegment_ ## seg (value)

	/*
	 * Save a segment register away:
	 */
	#define savesegment(seg, value) \
				asm("mov %%" #seg ",%0":"=r" (value) : : "memory")

	extern void idt_setup_early_handler(void);
	extern void idt_setup_apic_and_irq_gates(void);
	extern void switch_gdt_and_percpu_base(int);
	extern void load_direct_gdt(int);
	extern void cpu_init(void);
	extern void cpu_init_exception_handling(void);

#endif /* _ASM_X86_DESC_API_H_ */