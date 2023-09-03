// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_DESC_H
#define _ASM_X86_DESC_H

	#include <asm/desc_defs.h>
	// #include <asm/ldt.h>
	// #include <asm/mmu.h>
	// #include <asm/fixmap.h>
	#include <asm/irq_vectors.h>
	// #include <asm/cpu_entry_area.h>

	// #include <linux/debug_locks.h>
	#include <linux/smp/smp.h>
	#include <linux/smp/percpu.h>


	#include <asm/segment.h>
	#include <asm/processor.h>
	#include <asm/page_types.h>

	extern struct desc_ptr idt_descr;
	extern struct gdt_page gdt_page;
	extern struct tss_struct cpu_tss_rw;


	struct gdt_page {
		desc_s gdt[GDT_ENTRIES];
	} __attribute__((aligned(PAGE_SIZE)));

	/* Provide the original GDT */
	static inline desc_s *get_cpu_gdt_rw(unsigned int cpu) {
		return per_cpu(gdt_page, cpu).gdt;
	}

	static inline void pack_gate(gate_desc *gate, unsigned type,
			void (*func)(void), unsigned dpl, unsigned ist) {
		gate->offset_low	= (u16) (u64)func;
		gate->bits.p		= 1;
		gate->bits.dpl		= dpl;
		gate->bits.zero		= 0;
		gate->bits.type		= type;
		gate->offset_middle	= (u16) ((u64)func >> 16);
		gate->segment		= __KERNEL_CS;
		gate->bits.ist		= ist;
		gate->reserved		= 0;
		gate->offset_high	= (u32) ((u64)func >> 32);
	}

	#define load_tr(tr)					asm volatile("ltr %0"::"m" (tr))
	#define load_ldt(ldt)				asm volatile("lldt %0"::"m" (ldt))

	// static inline void
	// native_write_gdt_entry(desc_s *gdt, int entry, const void *desc, int type)
	static inline void
	write_gdt_entry(desc_s *gdt, int entry, const void *desc, int type) {
		unsigned int size;

		switch (type) {
			case DESC_TSS:	size = sizeof(tss_desc);	break;
			case DESC_LDT:	size = sizeof(ldt_desc);	break;
			default:		size = sizeof(*gdt);		break;
		}

		memcpy(&gdt[entry], desc, size);
	}

	static inline void
	set_tssldt_descriptor(void *d, unsigned long addr, unsigned type, unsigned size) {
		struct ldttss_desc *desc = d;

		memset(desc, 0, sizeof(*desc));

		desc->limit0	= (u16) size;
		desc->base0		= (u16) addr;
		desc->base1		= (addr >> 16) & 0xFF;
		desc->type		= type;
		desc->p			= 1;
		desc->limit1	= (size >> 16) & 0xF;
		desc->base2		= (addr >> 24) & 0xFF;
		desc->base3		= (u32) (addr >> 32);
	}

	// static inline void
	// __set_tss_desc(unsigned cpu, unsigned int entry, x86_hw_tss_s *addr)
	static inline void
	set_tss_desc(unsigned cpu, x86_hw_tss_s *addr) {
		desc_s *d = get_cpu_gdt_rw(cpu);
		tss_desc tss;

		set_tssldt_descriptor(&tss, (unsigned long)addr, DESC_TSS,
					__KERNEL_TSS_LIMIT);
		write_gdt_entry(d, GDT_ENTRY_TSS, &tss, DESC_TSS);
	}


	// static inline void native_load_gdt(const struct desc_ptr *dtr)
	static inline void
	load_gdt(const struct desc_ptr *dtr) {
		asm volatile(	"lgdt	%0		\t\n"
					:
					:	"m" (*dtr)
					);
	}

	// static __always_inline void native_load_idt(const struct desc_ptr *dtr)
	static __always_inline void
	load_idt(const struct desc_ptr *dtr) {
		asm volatile(	"lidt	%0		\t\n"
					:
					:	"m" (*dtr)
					);
	}


	/*
	 * The LTR instruction marks the TSS GDT entry as busy. On 64-bit, the GDT is
	 * a read-only remapping. To prevent a page fault, the GDT is switched to the
	 * original writeable version when needed.
	 */
	// static inline void native_load_tr_desc(void)
	static inline void load_TR_desc(void)
	{
		int cpu = raw_smp_processor_id();

		load_direct_gdt(cpu);
		asm volatile(	"ltr	%w0		\t\n"
					:
					:	"q" (GDT_ENTRY_TSS*8)
					);
	}


	#define load_current_idt() load_idt(&idt_descr)
	extern void idt_setup_early_handler(void);
	extern void idt_setup_apic_and_irq_gates(void);


	// Not Linux Code, long jmp refresh seg-reg
	static inline void refresh_segment_registers()
	{
		asm volatile(	"movq	%%rsp,		%%rax		\n\t"
						"mov 	%0,			%%ss		\n\t"
						"movq	%%rax,		%%rsp		\n\t"
						"xor	%%rax,		%%rax		\n\t"
						"leaq	1f(%%rip),	%%rax		\n\t"
						"pushq	%1						\n\t"
						"pushq	%%rax					\n\t"
						"lretq							\n\t"
						"1:								\n\t"
						"xorq	%%rax, %%rax			\n\t"
					:
					:	"r"(__KERNEL_DS),
						"rsi"((uint64_t)__KERNEL_CS)
					:	"rax"
					);
	}

#endif /* _ASM_X86_DESC_H */
