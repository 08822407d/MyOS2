// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_DESC_H_
#define _ASM_X86_DESC_H_

	#include <linux/compiler/myos_debug_option.h>
	#include <linux/smp/percpu.h>

	#include <asm-generic/bug.h>
	#include <asm/processor.h>
	#include <asm/sched.h>

	#include "desc_const_arch.h"
	#include "desc_types_arch.h"


	#ifdef DEBUG

		extern desc_s
		*get_cpu_gdt_rw(unsigned int cpu);

		extern void
		pack_gate(gate_desc *gate, unsigned type,
				void (*func)(void), unsigned dpl, unsigned ist);

		extern void
		write_idt_entry(gate_desc *idt, int entry, const gate_desc *gate);

		extern void
		write_gdt_entry(desc_s *gdt, int entry,  const void *desc, int type);

		extern void
		set_tssldt_descriptor(void *d, unsigned long addr,
				unsigned type, unsigned size);

		extern void
		set_tss_desc(unsigned cpu, x86_hw_tss_s *addr);

		extern unsigned long
		gate_offset(const gate_desc *g);

		extern unsigned long
		gate_segment(const gate_desc *g);

		extern void
		load_gdt(const struct desc_ptr *dtr);

		extern void
		load_idt(const struct desc_ptr *dtr);

		extern void
		load_TR_desc(void);

		extern void
		__loadsegment_fs(unsigned short value);

		extern void
		init_idt_data(struct idt_data *data,
				unsigned int n, const void *addr);

		extern void
		idt_init_desc(gate_desc *gate, const struct idt_data *d);

		extern void
		refresh_segment_registers();

	#endif

	#if defined(ARCH_DESC_DEFINATION) || !(DEBUG)
	
		/* Provide the original GDT */
		PREFIX_STATIC_INLINE
		desc_s
		*get_cpu_gdt_rw(unsigned int cpu) {
			return per_cpu(gdt_page, cpu).gdt;
		}

		PREFIX_STATIC_INLINE
		void
		pack_gate(gate_desc *gate, unsigned type,
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

		// static inline void
		// native_write_idt_entry(gate_desc *idt, int entry, const gate_desc *gate)
		PREFIX_STATIC_INLINE
		void
		write_idt_entry(gate_desc *idt, int entry, const gate_desc *gate) {
			memcpy(&idt[entry], gate, sizeof(*gate));
		}

		// static inline void
		// native_write_gdt_entry(desc_s *gdt, int entry, const void *desc, int type)
		PREFIX_STATIC_INLINE
		void
		write_gdt_entry(desc_s *gdt, int entry,  const void *desc, int type) {
			unsigned int size;

			switch (type) {
				case DESC_TSS:	size = sizeof(tss_desc);	break;
				case DESC_LDT:	size = sizeof(ldt_desc);	break;
				default:		size = sizeof(*gdt);		break;
			}

			memcpy(&gdt[entry], desc, size);
		}

		PREFIX_STATIC_INLINE
		void
		set_tssldt_descriptor(void *d, unsigned long addr,
				unsigned type, unsigned size) {

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
		PREFIX_STATIC_INLINE
		void
		set_tss_desc(unsigned cpu, x86_hw_tss_s *addr) {
			desc_s *d = get_cpu_gdt_rw(cpu);
			tss_desc tss;

			set_tssldt_descriptor(&tss, (unsigned long)addr,
					DESC_TSS, __KERNEL_TSS_LIMIT);
			write_gdt_entry(d, GDT_ENTRY_TSS, &tss, DESC_TSS);
		}

		PREFIX_STATIC_INLINE
		unsigned long
		gate_offset(const gate_desc *g) {
			return g->offset_low | ((unsigned long)g->offset_middle << 16) |
					((unsigned long) g->offset_high << 32);
		}

		PREFIX_STATIC_INLINE
		unsigned long
		gate_segment(const gate_desc *g) {
			return g->segment;
		}

		// static inline void native_load_gdt(const struct desc_ptr *dtr)
		PREFIX_STATIC_INLINE
		void
		load_gdt(const struct desc_ptr *dtr) {
			asm volatile(	"lgdt	%0		\t\n"
						:
						:	"m" (*dtr)
						);
		}

		// static __always_inline void native_load_idt(const struct desc_ptr *dtr)
		PREFIX_STATIC_AWLWAYS_INLINE
		void
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
		PREFIX_STATIC_INLINE
		void
		load_TR_desc(void) {
			int cpu = raw_smp_processor_id();
			load_direct_gdt(cpu);
			asm volatile(	"ltr	%w0		\t\n"
						:
						:	"q" (GDT_ENTRY_TSS*8)
						);
		}

		PREFIX_STATIC_INLINE
		void
		__loadsegment_fs(unsigned short value) {
			asm volatile(	"						\n"
							"1:	movw %0, %%fs		\n"
							"2:						\n"
						:
						:	"rm" (value)
						:	"memory"
						);
		}

		PREFIX_STATIC_INLINE
		void
		init_idt_data(struct idt_data *data,
				unsigned int n, const void *addr) {

			BUG_ON(n > 0xFF);

			memset(data, 0, sizeof(*data));
			data->vector		= n;
			data->addr			= addr;
			data->segment		= __KERNEL_CS;
			data->bits.type		= GATE_INTERRUPT;
			data->bits.p		= 1;
		}

		PREFIX_STATIC_INLINE
		void
		idt_init_desc(gate_desc *gate, const struct idt_data *d) {
			unsigned long addr	= (unsigned long) d->addr;

			gate->offset_low	= (u16) addr;
			gate->segment		= (u16) d->segment;
			gate->bits			= d->bits;
			gate->offset_middle	= (u16) (addr >> 16);
			gate->offset_high	= (u32) (addr >> 32);
			gate->reserved		= 0;
		}

		// Not Linux Code, long jmp refresh seg-reg
		PREFIX_STATIC_INLINE
		void
		refresh_segment_registers() {

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

	#endif

#endif /* _ASM_X86_DESC_H_ */
