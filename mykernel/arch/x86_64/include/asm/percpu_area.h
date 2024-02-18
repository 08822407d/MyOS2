#ifndef _ASM_X86_PERCPU_AREA_H
#define _ASM_X86_PERCPU_AREA_H

	#include <asm/percpu.h>

	struct pcpu_hot;
	typedef struct pcpu_hot pcpu_hot_s;
	DECLARE_PER_CPU_CACHE_ALIGNED(pcpu_hot_s, pcpu_hot);

	struct gdt_page;
	DECLARE_PER_CPU_PAGE_ALIGNED(struct gdt_page, gdt_page);

	struct tss_struct;
	DECLARE_PER_CPU_PAGE_ALIGNED(struct tss_struct, cpu_tss_rw);

#endif /* _ASM_X86_PERCPU_AREA_H */
