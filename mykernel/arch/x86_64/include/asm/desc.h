/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_DESC_H
#define _ASM_X86_DESC_H

	#include <asm/desc_defs.h>
	// #include <asm/ldt.h>
	// #include <asm/mmu.h>
	// #include <asm/fixmap.h>
	// #include <asm/irq_vectors.h>
	// #include <asm/cpu_entry_area.h>

	// #include <linux/debug_locks.h>
	// #include <linux/smp.h>
	#include <linux/smp/percpu.h>


	#include <asm/segment.h>
	#include <asm/processor.h>
	#include <asm/page_types.h>

	extern struct gdt_page gdt_page;
	extern struct tss_struct cpu_tss_rw;


	// static inline void fill_ldt(desc_s *desc, const struct user_desc *info)
	// {
	// 	desc->limit0		= info->limit & 0x0ffff;

	// 	desc->base0		= (info->base_addr & 0x0000ffff);
	// 	desc->base1		= (info->base_addr & 0x00ff0000) >> 16;

	// 	desc->type		= (info->read_exec_only ^ 1) << 1;
	// 	desc->type	       |= info->contents << 2;
	// 	/* Set the ACCESS bit so it can be mapped RO */
	// 	desc->type	       |= 1;

	// 	desc->s			= 1;
	// 	desc->dpl		= 0x3;
	// 	desc->p			= info->seg_not_present ^ 1;
	// 	desc->limit1		= (info->limit & 0xf0000) >> 16;
	// 	desc->avl		= info->useable;
	// 	desc->d			= info->seg_32bit;
	// 	desc->g			= info->limit_in_pages;

	// 	desc->base2		= (info->base_addr & 0xff000000) >> 24;
	// 	/*
	// 	* Don't allow setting of the lm bit. It would confuse
	// 	* user_64bit_mode and would get overridden by sysret anyway.
	// 	*/
	// 	desc->l			= 0;
	// }

	struct gdt_page {
		desc_s gdt[GDT_ENTRIES];
	} __attribute__((aligned(PAGE_SIZE)));

	// DECLARE_PER_CPU_PAGE_ALIGNED(struct gdt_page, gdt_page);

	/* Provide the original GDT */
	static inline desc_s *get_cpu_gdt_rw(unsigned int cpu) {
		// return per_cpu(gdt_page, cpu).gdt;
		return &gdt_page.gdt[0];
	}

	// /* Provide the current original GDT */
	// static inline desc_s *get_current_gdt_rw(void)
	// {
	// 	return this_cpu_ptr(&gdt_page)->gdt;
	// }

	/* Provide the fixmap address of the remapped GDT */
	static inline desc_s *get_cpu_gdt_ro(int cpu) {
		// return (desc_s *)&get_cpu_entry_area(cpu)->gdt;
		return &gdt_page.gdt[0];
	}

	// /* Provide the current read-only GDT */
	// static inline desc_s *get_current_gdt_ro(void)
	// {
	// 	return get_cpu_gdt_ro(smp_processor_id());
	// }

	// /* Provide the physical address of the GDT page. */
	// static inline phys_addr_t get_cpu_gdt_paddr(unsigned int cpu)
	// {
	// 	return per_cpu_ptr_to_phys(get_cpu_gdt_rw(cpu));
	// }

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

	// static inline int desc_empty(const void *ptr)
	// {
	// 	const u32 *desc = ptr;

	// 	return !(desc[0] | desc[1]);
	// }

	// #ifdef CONFIG_PARAVIRT_XXL
	// #include <asm/paravirt.h>
	// #else
	// #define load_TR_desc()				native_load_tr_desc()
	// #define load_gdt(dtr)				native_load_gdt(dtr)
	// #define load_idt(dtr)				native_load_idt(dtr)
	#define load_tr(tr)					asm volatile("ltr %0"::"m" (tr))
	#define load_ldt(ldt)				asm volatile("lldt %0"::"m" (ldt))

	// #define store_gdt(dtr)				native_store_gdt(dtr)
	// #define store_tr(tr)				(tr = native_store_tr())

	// #define load_TLS(t, cpu)			native_load_tls(t, cpu)
	// #define set_ldt					native_set_ldt

	// #define write_ldt_entry(dt, entry, desc)	native_write_ldt_entry(dt, entry, desc)
	// #define write_gdt_entry(dt, entry, desc, type)	native_write_gdt_entry(dt, entry, desc, type)
	// #define write_idt_entry(dt, entry, g)		native_write_idt_entry(dt, entry, g)

	// static inline void paravirt_alloc_ldt(desc_s *ldt, unsigned entries)
	// {
	// }

	// static inline void paravirt_free_ldt(desc_s *ldt, unsigned entries)
	// {
	// }
	// #endif	/* CONFIG_PARAVIRT_XXL */

	// #define store_ldt(ldt) asm("sldt %0" : "=m"(ldt))

	// static inline void native_write_idt_entry(gate_desc *idt, int entry, const gate_desc *gate)
	// {
	// 	memcpy(&idt[entry], gate, sizeof(*gate));
	// }

	// static inline void native_write_ldt_entry(desc_s *ldt, int entry, const void *desc)
	// {
	// 	memcpy(&ldt[entry], desc, 8);
	// }

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

		desc->limit0		= (u16) size;
		desc->base0		= (u16) addr;
		desc->base1		= (addr >> 16) & 0xFF;
		desc->type		= type;
		desc->p			= 1;
		desc->limit1		= (size >> 16) & 0xF;
		desc->base2		= (addr >> 24) & 0xFF;
		desc->base3		= (u32) (addr >> 32);
	}

	// static inline void
	// __set_tss_desc(unsigned cpu, unsigned int entry, x86_hw_tss_s *addr)
	static inline void
	set_tss_desc(unsigned cpu, x86_hw_tss_s *addr) {
		desc_s *d = gdt_page.gdt;
		tss_desc tss;

		set_tssldt_descriptor(&tss, (unsigned long)addr, DESC_TSS,
					__KERNEL_TSS_LIMIT);
		write_gdt_entry(d, GDT_ENTRY_TSS, &tss, DESC_TSS);
	}
	// #define set_tss_desc(cpu, addr) __set_tss_desc(cpu, GDT_ENTRY_TSS, addr)

	// static inline void native_set_ldt(const void *addr, unsigned int entries)
	// {
	// 	if (likely(entries == 0))
	// 		asm volatile("lldt %w0"::"q" (0));
	// 	else {
	// 		unsigned cpu = smp_processor_id();
	// 		ldt_desc ldt;

	// 		set_tssldt_descriptor(&ldt, (unsigned long)addr, DESC_LDT,
	// 					entries * LDT_ENTRY_SIZE - 1);
	// 		write_gdt_entry(get_cpu_gdt_rw(cpu), GDT_ENTRY_LDT,
	// 				&ldt, DESC_LDT);
	// 		asm volatile("lldt %w0"::"q" (GDT_ENTRY_LDT*8));
	// 	}
	// }

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

	// static inline void native_store_gdt(struct desc_ptr *dtr)
	static inline void
	store_gdt(struct desc_ptr *dtr) {
		asm volatile(	"sgdt	%0		\t\n"
					:	"=m" (*dtr)
					);
	}

	// static inline void store_idt(struct desc_ptr *dtr)
	// {
	// 	asm volatile("sidt %0":"=m" (*dtr));
	// }

	// static inline void native_gdt_invalidate(void)
	// {
	// 	const struct desc_ptr invalid_gdt = {
	// 		.address = 0,
	// 		.size = 0
	// 	};

	// 	native_load_gdt(&invalid_gdt);
	// }

	// static inline void native_idt_invalidate(void)
	// {
	// 	const struct desc_ptr invalid_idt = {
	// 		.address = 0,
	// 		.size = 0
	// 	};

	// 	native_load_idt(&invalid_idt);
	// }

	/*
	 * The LTR instruction marks the TSS GDT entry as busy. On 64-bit, the GDT is
	 * a read-only remapping. To prevent a page fault, the GDT is switched to the
	 * original writeable version when needed.
	 */
	// static inline void native_load_tr_desc(void)
	static inline void load_TR_desc(void)
	{
		struct desc_ptr gdt;
		int cpu = 0;
		// int cpu = raw_smp_processor_id();
		bool restore = 0;
		desc_s *fixmap_gdt;

		store_gdt(&gdt);
		fixmap_gdt = get_cpu_gdt_ro(cpu);

		/*
		 * If the current GDT is the read-only fixmap, swap to the original
		 * writeable version. Swap back at the end.
		 */
		if (gdt.address == (unsigned long)fixmap_gdt) {
			load_direct_gdt(cpu);
			restore = 1;
		}
		asm volatile(	"ltr	%w0		\t\n"
					:
					:	"q" (GDT_ENTRY_TSS*8)
					);
		if (restore)
			load_fixmap_gdt(cpu);
	}

	// static inline unsigned long native_store_tr(void)
	// {
	// 	unsigned long tr;

	// 	asm volatile("str %0":"=r" (tr));

	// 	return tr;
	// }

	// static inline void native_load_tls(struct thread_struct *t, unsigned int cpu)
	// {
	// 	desc_s *gdt = get_cpu_gdt_rw(cpu);
	// 	unsigned int i;

	// 	for (i = 0; i < GDT_ENTRY_TLS_ENTRIES; i++)
	// 		gdt[GDT_ENTRY_TLS_MIN + i] = t->tls_array[i];
	// }

	// DECLARE_PER_CPU(bool, __tss_limit_invalid);

	// static inline void force_reload_TR(void)
	// {
	// 	desc_s *d = get_current_gdt_rw();
	// 	tss_desc tss;

	// 	memcpy(&tss, &d[GDT_ENTRY_TSS], sizeof(tss_desc));

	// 	/*
	// 	* LTR requires an available TSS, and the TSS is currently
	// 	* busy.  Make it be available so that LTR will work.
	// 	*/
	// 	tss.type = DESC_TSS;
	// 	write_gdt_entry(d, GDT_ENTRY_TSS, &tss, DESC_TSS);

	// 	load_TR_desc();
	// 	this_cpu_write(__tss_limit_invalid, false);
	// }

	// /*
	// * Call this if you need the TSS limit to be correct, which should be the case
	// * if and only if you have TIF_IO_BITMAP set or you're switching to a task
	// * with TIF_IO_BITMAP set.
	// */
	// static inline void refresh_tss_limit(void)
	// {
	// 	DEBUG_LOCKS_WARN_ON(preemptible());

	// 	if (unlikely(this_cpu_read(__tss_limit_invalid)))
	// 		force_reload_TR();
	// }

	// /*
	// * If you do something evil that corrupts the cached TSS limit (I'm looking
	// * at you, VMX exits), call this function.
	// *
	// * The optimization here is that the TSS limit only matters for Linux if the
	// * IO bitmap is in use.  If the TSS limit gets forced to its minimum value,
	// * everything works except that IO bitmap will be ignored and all CPL 3 IO
	// * instructions will #GP, which is exactly what we want for normal tasks.
	// */
	// static inline void invalidate_tss_limit(void)
	// {
	// 	DEBUG_LOCKS_WARN_ON(preemptible());

	// 	if (unlikely(test_thread_flag(TIF_IO_BITMAP)))
	// 		force_reload_TR();
	// 	else
	// 		this_cpu_write(__tss_limit_invalid, true);
	// }

	// /* This intentionally ignores lm, since 32-bit apps don't have that field. */
	// #define LDT_empty(info)					\
	// 	((info)->base_addr		== 0	&&	\
	// 	(info)->limit			== 0	&&	\
	// 	(info)->contents		== 0	&&	\
	// 	(info)->read_exec_only		== 1	&&	\
	// 	(info)->seg_32bit		== 0	&&	\
	// 	(info)->limit_in_pages		== 0	&&	\
	// 	(info)->seg_not_present	== 1	&&	\
	// 	(info)->useable		== 0)

	// /* Lots of programs expect an all-zero user_desc to mean "no segment at all". */
	// static inline bool LDT_zero(const struct user_desc *info)
	// {
	// 	return (info->base_addr		== 0 &&
	// 		info->limit		== 0 &&
	// 		info->contents		== 0 &&
	// 		info->read_exec_only	== 0 &&
	// 		info->seg_32bit		== 0 &&
	// 		info->limit_in_pages	== 0 &&
	// 		info->seg_not_present	== 0 &&
	// 		info->useable		== 0);
	// }

	// static inline void clear_LDT(void)
	// {
	// 	set_ldt(NULL, 0);
	// }

	// static inline unsigned long get_desc_base(const desc_s *desc)
	// {
	// 	return (unsigned)(desc->base0 | ((desc->base1) << 16) | ((desc->base2) << 24));
	// }

	// static inline void set_desc_base(desc_s *desc, unsigned long base)
	// {
	// 	desc->base0 = base & 0xffff;
	// 	desc->base1 = (base >> 16) & 0xff;
	// 	desc->base2 = (base >> 24) & 0xff;
	// }

	// static inline unsigned long get_desc_limit(const desc_s *desc)
	// {
	// 	return desc->limit0 | (desc->limit1 << 16);
	// }

	// static inline void set_desc_limit(desc_s *desc, unsigned long limit)
	// {
	// 	desc->limit0 = limit & 0xffff;
	// 	desc->limit1 = (limit >> 16) & 0xf;
	// }

	// void alloc_intr_gate(unsigned int n, const void *addr);

	// static inline void init_idt_data(struct idt_data *data, unsigned int n,
	// 				const void *addr)
	// {
	// 	BUG_ON(n > 0xFF);

	// 	memset(data, 0, sizeof(*data));
	// 	data->vector	= n;
	// 	data->addr	= addr;
	// 	data->segment	= __KERNEL_CS;
	// 	data->bits.type	= GATE_INTERRUPT;
	// 	data->bits.p	= 1;
	// }

	// static inline void idt_init_desc(gate_desc *gate, const struct idt_data *d)
	// {
	// 	unsigned long addr = (unsigned long) d->addr;

	// 	gate->offset_low	= (u16) addr;
	// 	gate->segment		= (u16) d->segment;
	// 	gate->bits		= d->bits;
	// 	gate->offset_middle	= (u16) (addr >> 16);
	// 	gate->offset_high	= (u32) (addr >> 32);
	// 	gate->reserved		= 0;
	// }

	// extern unsigned long system_vectors[];

	extern void load_current_idt(void);
	extern void idt_setup_early_handler(void);
	// extern void idt_setup_early_traps(void);
	// extern void idt_setup_traps(void);
	extern void idt_setup_apic_and_irq_gates(void);
	// extern bool idt_is_f00f_address(unsigned long address);

	// extern void idt_setup_early_pf(void);

	// extern void idt_invalidate(void);



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
