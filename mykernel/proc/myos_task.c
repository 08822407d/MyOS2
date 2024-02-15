#include <linux/kernel/sched.h>
#include <linux/kernel/types.h>
#include <linux/kernel/stddef.h>
#include <linux/mm/mm.h>
#include <linux/mm/memblock.h>
#include <linux/lib/string.h>
#include <linux/kernel/fdtable.h>

#include <asm-generic/sections.h>

#include <obsolete/glo.h>
#include <obsolete/proto.h>

// de attention that before entering start_kernel, rsp had already point to stack of task0,
// in myos_early_init_system() .bss section will be set 0, so here arrange task0 in .data section

void myos_init_task(size_t lcpu_nr)
{
	task_s *task0 = &per_cpu(idle_threads, 0).task;

	myos_init_pid_allocator();
	attach_pid(task0, PIDTYPE_PID);
	set_task_comm(task0, "cpu0_idle");
}