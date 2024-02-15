#include <linux/kernel/sched.h>
#include <linux/lib/list.h>

#include <asm/processor.h>
#include <asm/desc.h>


#include <obsolete/glo.h>

char init_stack[THREAD_SIZE] __page_aligned_data;

DEFINE_PER_CPU_ALIGNED(pcpu_hot_s, pcpu_hot) = {
	.current_task	= &idle_threads.task,
	.preempt_count	= INIT_PREEMPT_COUNT,
	// .top_of_stack	= TOP_OF_INIT_STACK,
};

__visible DEFINE_PER_CPU(files_struct_s, idle_taskfilps);

__visible DEFINE_PER_CPU(taskfs_s, idle_taskfs) = {
	.in_exec		= 0,
	.umask			= 0,
	.users			= 0,
	.pwd.mnt		= NULL,
	.root.mnt		= NULL,
};

__visible DEFINE_PER_CPU(PCB_u, idle_threads)  __aligned(THREAD_SIZE) = {
	.task.tasks				= LIST_INIT(idle_threads.task.tasks),
	.task.parent			= &idle_threads.task,
	.task.sibling			= LIST_INIT(idle_threads.task.sibling),
	.task.children			= LIST_HEADER_INIT(idle_threads.task.children),
	.task.__state			= TASK_RUNNING,
	.task.flags				= PF_KTHREAD,
	.task.rt.time_slice		= 20,
	.task.se.vruntime		= -1,
	.task.mm				= &init_mm,
	.task.fs				= &idle_taskfs,
	.task.files				= &idle_taskfilps,
	.task.pid_links 		= LIST_INIT(idle_threads.task.pid_links),
	.task.stack				= (void *)init_stack + THREAD_SIZE,
};

__visible DEFINE_PER_CPU(cpudata_u, cpudata) ={
	.data = {
		.idle_task			= &idle_threads.task,
		.curr_task			= &idle_threads.task,
		.running_lhdr		= LIST_HEADER_INIT(cpudata.data.running_lhdr),
		.is_idle_flag		= 1,
		.scheduleing_flag	= 0,
		.preempt_count		= 0,
		.last_jiffies		= 0,
		.cpustack_p			= (reg_t)(&cpudata + 1),
	},
};


/*
 * per-CPU TSS segments. Threads are completely 'soft' on Linux,
 * no more per-task TSS's. The TSS size is kept cacheline-aligned
 * so they are allowed to end up in the .data..cacheline_aligned
 * section. Since TSS's are completely CPU-local, we want them
 * on exact cacheline boundaries, to eliminate cacheline ping-pong.
 */
__visible DEFINE_PER_CPU_PAGE_ALIGNED(struct tss_struct, cpu_tss_rw) = {
// struct tss_struct cpu_tss_rw = {
	.x86_tss = {
		/*
		 * .sp0 is only used when entering ring 0 from a lower
		 * privilege level.  Since the init task never runs anything
		 * but ring 0 code, there is no need for a valid value here.
		 * Poison it.
		 */
		// .sp0 = (1UL << (BITS_PER_LONG-1)) + 1,
		.sp0 = (reg_t)init_stack + THREAD_SIZE,
		.io_bitmap_base	= IO_BITMAP_OFFSET_INVALID,
	 },
};

DEFINE_PER_CPU_PAGE_ALIGNED(struct gdt_page, gdt_page) = { .gdt = {
// struct gdt_page gdt_page = { .gdt = {
	[GDT_ENTRY_INVALID_SEG]			= GDT_ENTRY_INIT(0, 0, 0),
#if defined(CONFIG_INTEL_X64_GDT_LAYOUT)
	[GDT_ENTRY_KERNEL_CS]			= GDT_ENTRY_INIT(0xa09b, 0, 0xfffff),
	[GDT_ENTRY_KERNEL_DS]			= GDT_ENTRY_INIT(0xc093, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER_CS]		= GDT_ENTRY_INIT(0xa0fb, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER_DS]		= GDT_ENTRY_INIT(0xc0f3, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER_CS_DUP]	= GDT_ENTRY_INIT(0xa0fb, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER_DS_DUP]	= GDT_ENTRY_INIT(0xc0f3, 0, 0xfffff),
#else
	/*
	 * We need valid kernel segments for data and code in long mode too
	 * IRET will check the segment types  kkeil 2000/10/28
	 * Also sysret mandates a special GDT layout
	 *
	 * TLS descriptors are currently at a different place compared to i386.
	 * Hopefully nobody expects them at a fixed place (Wine?)
	 */
	[GDT_ENTRY_KERNEL32_CS]			= GDT_ENTRY_INIT(0xc09b, 0, 0xfffff),
	[GDT_ENTRY_KERNEL_CS]			= GDT_ENTRY_INIT(0xa09b, 0, 0xfffff),
	[GDT_ENTRY_KERNEL_DS]			= GDT_ENTRY_INIT(0xc093, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER32_CS]	= GDT_ENTRY_INIT(0xc0fb, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER_DS]		= GDT_ENTRY_INIT(0xc0f3, 0, 0xfffff),
	[GDT_ENTRY_DEFAULT_USER_CS]		= GDT_ENTRY_INIT(0xa0fb, 0, 0xfffff),
#endif
}};