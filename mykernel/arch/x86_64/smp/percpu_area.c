#define PERCPU_VAR_DEFINATION


#include <linux/kernel/sched.h>
#include <linux/kernel/lib.h>
#include <asm/insns.h>

#include <obsolete/glo.h>

char init_stack[THREAD_SIZE] __page_aligned_data;

__visible DEFINE_PER_CPU_CACHE_ALIGNED(pcpu_hot_s, pcpu_hot);

__visible DEFINE_PER_CPU(task_s, idle_threads);

__visible DEFINE_PER_CPU_CACHE_ALIGNED(rq_s, runqueues);

/*
 * per-CPU TSS segments. Threads are completely 'soft' on Linux,
 * no more per-task TSS's. The TSS size is kept cacheline-aligned
 * so they are allowed to end up in the .data..cacheline_aligned
 * section. Since TSS's are completely CPU-local, we want them
 * on exact cacheline boundaries, to eliminate cacheline ping-pong.
 */
__visible DEFINE_PER_CPU_PAGE_ALIGNED(struct tss_struct, cpu_tss_rw);

DEFINE_PER_CPU_PAGE_ALIGNED(struct gdt_page, gdt_page) = { .gdt = {
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


void myos_init_per_cpu_var(void)
{
	myos_init_pid_allocator();

	task_s *this_idle_thread		= &per_cpu(idle_threads, 0);
	memset(this_idle_thread, 0, sizeof(task_s));
	spin_lock_init(&this_idle_thread->alloc_lock);
	INIT_LIST_S(&this_idle_thread->rt.run_list);
	INIT_LIST_S(&this_idle_thread->sibling);
	INIT_LIST_S(&this_idle_thread->pid_links);
	INIT_LIST_HEADER_S(&this_idle_thread->children);
	this_idle_thread->parent		= this_idle_thread;
	this_idle_thread->__state		= TASK_RUNNING;
	this_idle_thread->flags			= PF_KTHREAD;
	this_idle_thread->rt.time_slice	= 2;
	this_idle_thread->se.vruntime	= -1;
	this_idle_thread->fs			= &init_fs;
	this_idle_thread->files			= &init_files;
	this_idle_thread->stack			= (void *)init_stack;
	this_idle_thread->pid			= pid_nr(&init_struct_pid);
	this_idle_thread->thread_pid	= &init_struct_pid;
	memcpy(this_idle_thread->comm, "cpu0_idle", sizeof("cpu0_idle"));
	attach_pid(this_idle_thread, PIDTYPE_PID);


	x86_hw_tss_s *this_x86_tss		= &(per_cpu(cpu_tss_rw, 0).x86_tss);
	this_x86_tss->sp0				= task_top_of_stack(this_idle_thread);
	this_x86_tss->io_bitmap_base	= IO_BITMAP_OFFSET_INVALID;


	pcpu_hot_s *this_pcpu_hot		= &per_cpu(pcpu_hot, 0);
	memset(this_pcpu_hot, 0, sizeof(pcpu_hot_s));
	this_pcpu_hot->current_task		= this_idle_thread;
	this_pcpu_hot->preempt_count	= INIT_PREEMPT_COUNT;
	this_pcpu_hot->top_of_stack		= TOP_OF_INIT_STACK;


	struct gdt_page *this_gdt_page	= &per_cpu(gdt_page, 0);
	memcpy(this_gdt_page, (const void *)phys_to_virt((phys_addr_t)&gdt_page), sizeof(struct gdt_page));


	rq_s *this_runqueues			= &per_cpu(runqueues, 0);
	memset(this_runqueues, 0, sizeof(rq_s));
	INIT_LIST_HEADER_S(&this_runqueues->myos.running_lhdr);
	this_runqueues->curr			=
	this_runqueues->idle			= this_idle_thread;
}