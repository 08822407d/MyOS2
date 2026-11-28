---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134
record_type: phase2_new_fixed_source_windows
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
produced_by: GPT-6 self-reported, backend model and effort UNATTESTED
model_per_owner: "GPT-6＋Extra"
date: 2026-11-23
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
self_check: {verified_claims: 47, quotes_reconfirmed: 47, downgraded_to_inferred: 0}
---
# 第134批新增固定time源码证据

本批有47个正式短窗，另16个独立定位。E134-001、E134-002、E134-003各含不同RQCF宏的一行定义；它们既作为新批3个实际P6记录，也构成旧E133-015完整替代视图中的一对三更正，旧133冻结35/35与7定位不改。每个窗按GitHub time固定文本匹配1–5连续物理行，tab/CRLF不做推断式改写；声明、注释、表项和纳入语境不冒充本函数的已执行效果。未预处理、编译或运行。

## E134-001 · 正式引文

[VERIFIED mykernel/sched/scheduler/scheduler_const.h::RQCF_REQ_SKIP]
```c
	#define RQCF_REQ_SKIP		0x01
```

固定time原始物理行 46–46；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-002 · 正式引文

[VERIFIED mykernel/sched/scheduler/scheduler_const.h::RQCF_ACT_SKIP]
```c
	#define RQCF_ACT_SKIP		0x02
```

固定time原始物理行 47–47；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-003 · 正式引文

[VERIFIED mykernel/sched/scheduler/scheduler_const.h::RQCF_UPDATED]
```c
	#define RQCF_UPDATED		0x04
```

固定time原始物理行 48–48；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-004 · 正式引文

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::update_rq_clock]
```c
	delta = local_clock() - rq->clock;
	if (delta < 0)
		return;
	rq->clock += delta;
```

固定time原始物理行 24–27；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-005 · 正式引文

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	rq->clock_update_flags <<= 1;
	update_rq_clock(rq);
	rq->clock_update_flags = RQCF_UPDATED;
```

固定time原始物理行 1065–1067；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-006 · 正式引文

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	int rv = fork();
	
	if (rv == 0) {
```

固定time原始物理行 30–32；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-007 · 正式引文

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
		pid_t pid = getpid();
```

固定time原始物理行 33–33；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-008 · 正式引文

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
		execve(prog_name, args, envs);
```

固定time原始物理行 35–35；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-009 · 正式引文

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
		pid_t pid = getpid();
```

固定time原始物理行 38–38；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-010 · 正式引文

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
		while (1) {
			sched_yield();
		}
```

固定time原始物理行 40–42；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-011 · 正式引文

[VERIFIED mykernel/sched/syscall.c::getpid]
```c
	return task_tgid_vnr(current);
```

固定time原始物理行 47–47；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-012 · 正式引文

[VERIFIED mykernel/sched/syscall.c::getppid]
```c
	int pid;
```

固定time原始物理行 117–117；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-013 · 正式引文

[VERIFIED mykernel/sched/syscall.c::getppid]
```c
	pid = task_tgid_vnr(current->real_parent);
```

固定time原始物理行 120–120；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-014 · 正式引文

[VERIFIED mykernel/sched/syscall.c::getppid]
```c
	return pid;
```

固定time原始物理行 123–123；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-015 · 正式引文

[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_getpid]
```c
	#define __NR_getpid			39
```

固定time原始物理行 50–50；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-016 · 正式引文

[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_getppid]
```c
	#define __NR_getppid		110
```

固定time原始物理行 135–135；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-017 · 正式引文

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) extern long __x64_##sym(const pt_regs_s *);
```

固定time原始物理行 10–10；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-018 · 正式引文

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym)	[nr] = __x64_##sym,
```

固定time原始物理行 19–19；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-019 · 正式引文

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

固定time原始物理行 26–26；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-020 · 正式引文

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

固定time原始物理行 29–31；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-021 · 正式引文

[VERIFIED mykernel/sched/task/pid.h::task_pid]
```c
		*task_pid(task_s *task) {
			return task->thread_pid;
```

固定time原始物理行 124–125；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-022 · 正式引文

[VERIFIED mykernel/sched/task/pid.h::ns_of_pid]
```c
			pid_ns_s *ns = NULL;
			if (pid)
				ns = pid->numbers[pid->level].ns;
			return ns;
```

固定time原始物理行 85–88；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-023 · 正式引文

[VERIFIED mykernel/sched/task/pid.h::task_pid_vnr]
```c
			return __task_pid_nr_ns(tsk, PIDTYPE_PID, NULL);
```

固定time原始物理行 144–144；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-024 · 正式引文

[VERIFIED mykernel/sched/task/pid.h::task_tgid_vnr]
```c
			return __task_pid_nr_ns(tsk, PIDTYPE_TGID, NULL);
```

固定time原始物理行 173–173；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-025 · 正式引文

[VERIFIED mykernel/sched/task/pid_api.h::task_active_pid_ns]
```c
	#define task_active_pid_ns(tsk)	ns_of_pid(task_pid(tsk))
```

固定time原始物理行 12–12；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-026 · 正式引文

[VERIFIED mykernel/sched/task/pid.c::task_pid_ptr]
```c
	if (type == PIDTYPE_PID)
		return &task->thread_pid;
	else
		return &task->signal->pids[type];
```

固定time原始物理行 316–319；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-027 · 正式引文

[VERIFIED mykernel/sched/task/pid.c::pid_nr_ns]
```c
	pid_t nr = 0;
```

固定time原始物理行 450–450；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-028 · 正式引文

[VERIFIED mykernel/sched/task/pid.c::pid_nr_ns]
```c
	if (pid && ns->level <= pid->level) {
		upid = &pid->numbers[ns->level];
		if (upid->ns == ns)
			nr = upid->nr;
```

固定time原始物理行 452–455；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-029 · 正式引文

[VERIFIED mykernel/sched/task/pid.c::pid_nr_ns]
```c
	return nr;
```

固定time原始物理行 457–457；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-030 · 正式引文

[VERIFIED mykernel/sched/task/pid.c::__task_pid_nr_ns]
```c
	pid_t nr = 0;
```

固定time原始物理行 470–470；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-031 · 正式引文

[VERIFIED mykernel/sched/task/pid.c::__task_pid_nr_ns]
```c
	if (!ns)
		ns = task_active_pid_ns(current);
	nr = pid_nr_ns(rcu_dereference(*task_pid_ptr(task, type)), ns);
```

固定time原始物理行 473–475；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-032 · 正式引文

[VERIFIED mykernel/sched/task/pid.c::__task_pid_nr_ns]
```c
	return nr;
```

固定time原始物理行 478–478；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-033 · 正式引文

[VERIFIED mykernel/sched/task/pid_types.h::upid]
```c
		int			nr;
		pid_ns_s	*ns;
```

固定time原始物理行 45–46；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-034 · 正式引文

[VERIFIED mykernel/sched/task/pid_types.h::pid]
```c
		refcount_t		count;
		uint			level;
```

固定time原始物理行 50–51；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-035 · 正式引文

[VERIFIED mykernel/sched/task/pid_types.h::pid]
```c
		HList_hdr_s		tasks[PIDTYPE_MAX];
```

固定time原始物理行 56–56；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-036 · 正式引文

[VERIFIED mykernel/sched/task/pid_types.h::pid]
```c
		upid_s			numbers[];
```

固定time原始物理行 61–61；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-037 · 正式引文

[VERIFIED mykernel/sched/task/pid_types.h::pid_namespace]
```c
		uint			pid_allocated;
		task_s			*child_reaper;
		kmem_cache_s	*pid_cachep;
		uint			level;
		pid_ns_s		*parent;
```

固定time原始物理行 67–71；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-038 · 正式引文

[VERIFIED mykernel/sched/task/pid_const.h::pid_type]
```c
		PIDTYPE_PID,
		PIDTYPE_TGID,
		PIDTYPE_PGID,
		PIDTYPE_SID,
		PIDTYPE_MAX,
```

固定time原始物理行 7–11；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-039 · 正式引文

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		pid_t			pid;
		pid_t			tgid;
```

固定time原始物理行 217–218；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-040 · 正式引文

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		task_s __rcu	*real_parent;
		/* Recipient of SIGCHLD, wait4() reports: */
		task_s __rcu	*parent;
```

固定time原始物理行 231–233；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-041 · 正式引文

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		pid_s			*thread_pid;
		HList_s			pid_links[PIDTYPE_MAX];
```

固定time原始物理行 252–253；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-042 · 正式引文

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		signal_s		*signal;
```

固定time原始物理行 352–352；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-043 · 正式引文

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::signal_struct]
```c
		pid_s			*pids[PIDTYPE_MAX];
```

固定time原始物理行 149–149；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-044 · 正式引文

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

固定time原始物理行 62–65；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-045 · 正式引文

[VERIFIED mykernel/sched/task/pid.h::task_pid_nr]
```c
			return tsk->pid;
```

固定time原始物理行 132–132；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-046 · 正式引文

[VERIFIED mykernel/sched/task/pid.h::task_tgid_nr]
```c
			return tsk->tgid;
```

固定time原始物理行 150–150；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-047 · 正式引文

[VERIFIED mykernel/sched/task/pid.c::pid_vnr]
```c
	return pid_nr_ns(pid, task_active_pid_ns(current));
```

固定time原始物理行 463–463；函数体、结构成员、有效宏或配置定义内的1–5行。

## E134-048 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/include/asm/syscalls_64.h::__SYSCALL_mapping_getpid]
```c
__SYSCALL(__NR_getpid, sys_getpid)
```

固定time原始物理行 29–29；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-049 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/include/asm/syscalls_64.h::__SYSCALL_mapping_getppid]
```c
__SYSCALL(__NR_getppid, sys_getppid)
```

固定time原始物理行 57–57；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-050 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL_include_extern]
```c
#	include <asm/syscalls_64.h>
```

固定time原始物理行 11–11；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-051 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL_include_table]
```c
const sys_call_ptr_t sys_call_table[] = {
#include <asm/syscalls_64.h>
};
```

固定time原始物理行 20–22；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-052 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL_include_switch]
```c
	#include <asm/syscalls_64.h>
```

固定time原始物理行 30–30；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-053 · 独立定位

[SOURCE_LOCATION mykernel/sched/syscall.c::getpid_header]
```c
MYOS_SYSCALL_DEFINE0(getpid)
```

固定time原始物理行 45–45；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-054 · 独立定位

[SOURCE_LOCATION mykernel/sched/syscall.c::getppid_header]
```c
MYOS_SYSCALL_DEFINE0(getppid)
```

固定time原始物理行 115–115；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-055 · 独立定位

[SOURCE_LOCATION mykernel/sched/syscall.c::getppid_rcu_comment]
```c
	// rcu_read_lock();
```

固定time原始物理行 119–119；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-056 · 独立定位

[SOURCE_LOCATION mykernel/sched/syscall.c::getppid_rcu_comment]
```c
	// rcu_read_unlock();
```

固定time原始物理行 121–121；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-057 · 独立定位

[SOURCE_LOCATION mykernel/sched/task/pid.c::__task_pid_nr_ns_comment]
```c
	// rcu_read_lock();
```

固定time原始物理行 472–472；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-058 · 独立定位

[SOURCE_LOCATION mykernel/sched/task/pid.c::__task_pid_nr_ns_comment]
```c
	// rcu_read_unlock();
```

固定time原始物理行 476–476；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-059 · 独立定位

[SOURCE_LOCATION myinitramfs/myinit/initd.c::normal_boot_print_comment]
```c
		// printf("child task, %d\n", rv);
```

固定time原始物理行 34–34；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-060 · 独立定位

[SOURCE_LOCATION myinitramfs/myinit/initd.c::normal_boot_print_comment]
```c
		// printf("parent task, %d\n", rv);
```

固定time原始物理行 39–39；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-061 · 独立定位

[SOURCE_LOCATION mykernel/scripts/options_flags.cmake::time_canary]
```c
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
```

固定time原始物理行 41–41；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-062 · 独立定位

[SOURCE_LOCATION mykernel/debug/panic.c::time_canary]
```c
	this_cpu = smp_processor_id();
```

固定time原始物理行 79–79；头、注释、分派表项或编译金丝雀，不计入正式短窗。

## E134-063 · 独立定位

[SOURCE_LOCATION mykernel/sched/scheduler/scheduler_core.c::update_rq_clock_header]
```c
void update_rq_clock(rq_s *rq)
```

固定time原始物理行 20–20；头、注释、分派表项或编译金丝雀，不计入正式短窗。
