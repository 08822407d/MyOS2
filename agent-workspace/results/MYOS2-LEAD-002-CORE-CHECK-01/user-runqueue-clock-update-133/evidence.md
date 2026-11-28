---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-RUNQUEUE-CLOCK-UPDATE-133
record_type: phase2_new_source_windows
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
produced_by: GPT-6 self-reported; actual backend and effort UNATTESTED
model_per_owner: "GPT-6＋Extra"
date: 2026-11-23
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
self_check: {verified_claims: 35, quotes_reconfirmed: 35, downgraded_to_inferred: 0}
---
# 第133批新证据 · 只取固定time源码短窗

共35条正式带路径与函数/宏/字段归属的1–5物理行短窗，另7个头部/注释/声明/金丝雀定位。两类各自记账，不含第132批的58条或历史其他批次引文。直接以GitHub connector回读源码，tab与CRLF保真。源内的注释不是可执行分支，现场采样、预处理、编译与实际运行均NOT_RUN。

## E133-001 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::update_rq_clock]
```c
	s64 delta;
```

固定time物理行 22–22；活动函数体、字段、宏、类表项或配置正文。

## E133-002 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::update_rq_clock]
```c
	delta = local_clock() - rq->clock;
```

固定time物理行 24–24；活动函数体、字段、宏、类表项或配置正文。

## E133-003 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::update_rq_clock]
```c
	if (delta < 0)
		return;
```

固定time物理行 25–26；活动函数体、字段、宏、类表项或配置正文。

## E133-004 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::update_rq_clock]
```c
	rq->clock += delta;
```

固定time物理行 27–27；活动函数体、字段、宏、类表项或配置正文。

## E133-005 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	cpu = smp_processor_id();
	rq = cpu_rq(cpu);
	prev = rq->curr;
```

固定time物理行 1032–1034；活动函数体、字段、宏、类表项或配置正文。

## E133-006 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	rq->clock_update_flags <<= 1;
	update_rq_clock(rq);
	rq->clock_update_flags = RQCF_UPDATED;
```

固定time物理行 1065–1067；活动函数体、字段、宏、类表项或配置正文。

## E133-007 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	switch_count = &prev->nivcsw;
```

固定time物理行 1069–1069；活动函数体、字段、宏、类表项或配置正文。

## E133-008 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	prev_state = READ_ONCE(prev->__state);
```

固定time物理行 1078–1078；活动函数体、字段、宏、类表项或配置正文。

## E133-009 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	next = pick_next_task(rq, prev, &rf);
picked:
	clear_tsk_need_resched(prev);
	clear_preempt_need_resched();
```

固定time物理行 1116–1119；活动函数体、字段、宏、类表项或配置正文。

## E133-010 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	if (likely(prev != next)) {
		rq->nr_switches++;
```

固定time物理行 1124–1125；活动函数体、字段、宏、类表项或配置正文。

## E133-011 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
		RCU_INIT_POINTER(rq->curr, next);
```

固定time物理行 1130–1130；活动函数体、字段、宏、类表项或配置正文。

## E133-012 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
		++*switch_count;
```

固定time物理行 1153–1153；活动函数体、字段、宏、类表项或配置正文。

## E133-013 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
		rq = context_switch(rq, prev, next, &rf);
```

固定time物理行 1162–1162；活动函数体、字段、宏、类表项或配置正文。

## E133-014 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	local_irq_enable();
```

固定time物理行 1169–1169；活动函数体、字段、宏、类表项或配置正文。

## E133-015 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler_const.h::RQCF_REQ_SKIP]
```c
	#define RQCF_REQ_SKIP		0x01
	#define RQCF_ACT_SKIP		0x02
	#define RQCF_UPDATED		0x04
```

固定time物理行 46–48；活动函数体、字段、宏、类表项或配置正文。

## E133-016 · 正式连续短窗

[VERIFIED mykernel/sched/runqueue/runqueue_types.h::runqueue]
```c
		uint			clock_update_flags;
		u64				clock;
```

固定time物理行 263–264；活动函数体、字段、宏、类表项或配置正文。

## E133-017 · 正式连续短窗

[VERIFIED mykernel/sched/runqueue/runqueue_types.h::runqueue]
```c
		u64				clock_task;
		u64				clock_pelt;
```

固定time物理行 266–267；活动函数体、字段、宏、类表项或配置正文。

## E133-018 · 正式连续短窗

[VERIFIED mykernel/include/linux/sched/clock.h::local_clock]
```c
		return native_sched_clock();
```

固定time物理行 53–53；活动函数体、字段、宏、类表项或配置正文。

## E133-019 · 正式连续短窗

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_sched_clock]
```c
	if (__use_tsc) {
		u64 tsc_now = rdtsc();
		/* return the value in ns */
		return cycles_2_ns(tsc_now);
```

固定time物理行 222–225；活动函数体、字段、宏、类表项或配置正文。

## E133-020 · 正式连续短窗

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_sched_clock]
```c
	return (jiffies_64 - INITIAL_JIFFIES) * (1000000000 / HZ);
```

固定time物理行 237–237；活动函数体、字段、宏、类表项或配置正文。

## E133-021 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	retval = curr_task = current;
```

固定time物理行 9–9；活动函数体、字段、宏、类表项或配置正文。

## E133-022 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	ulong used_jiffies = jiffies - myos_rq->last_jiffies;
```

固定time物理行 12–12；活动函数体、字段、宏、类表项或配置正文。

## E133-023 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	if ((need_resched() ||
		curr_task == rq->idle ||
		used_jiffies >= rt->time_slice ||
		curr_task->__state != TASK_RUNNING) &&
		myos_rq->running_lhdr.count > 0)
```

固定time物理行 14–18；活动函数体、字段、宏、类表项或配置正文。

## E133-024 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		List_s * next_lp = list_header_remove_head(&myos_rq->running_lhdr);
		while (next_lp == NULL);
```

固定time物理行 24–25；活动函数体、字段、宏、类表项或配置正文。

## E133-025 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		if (curr_task->__state == TASK_RUNNING)
		{
			if (curr_task == rq->idle)
```

固定time物理行 28–30；活动函数体、字段、宏、类表项或配置正文。

## E133-026 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
				List_s * tmp_list = myos_rq->running_lhdr.anchor.next;
				sched_rt_entity_s *tmp_rt = container_of(tmp_list, sched_rt_entity_s, run_list);
				while ((curr_task->se.vruntime > container_of(tmp_rt, task_s, rt)->se.vruntime) &&
						tmp_list != &myos_rq->running_lhdr.anchor)
```

固定time物理行 37–40；活动函数体、字段、宏、类表项或配置正文。

## E133-027 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
				list_add_to_prev(&rt->run_list, tmp_list);
				myos_rq->running_lhdr.count++;
```

固定time物理行 44–45；活动函数体、字段、宏、类表项或配置正文。

## E133-028 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		if (curr_task != rq->idle)
			curr_task->se.vruntime += used_jiffies;
```

固定time物理行 49–50；活动函数体、字段、宏、类表项或配置正文。

## E133-029 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		sched_rt_entity_s *next_rt = container_of(next_lp, sched_rt_entity_s, run_list);
		retval = container_of(next_rt, task_s, rt);
		myos_rq->last_jiffies = jiffies;
```

固定time物理行 52–54；活动函数体、字段、宏、类表项或配置正文。

## E133-030 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	rq->curr = retval;
	return retval;
```

固定time物理行 56–57；活动函数体、字段、宏、类表项或配置正文。

## E133-031 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::myos_rt]
```c
	.pick_next_task		= pick_next_task_myos,
```

固定time物理行 67–67；活动函数体、字段、宏、类表项或配置正文。

## E133-032 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler.h::__pick_next_task]
```c
					p = class->pick_next_task(rq, prev);
					if (p)
						return p;
```

固定time物理行 264–266；活动函数体、字段、宏、类表项或配置正文。

## E133-033 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler.h::__pick_next_task]
```c
			BUG(); /* The idle class should always have a runnable task. */
```

固定time物理行 276–276；活动函数体、字段、宏、类表项或配置正文。

## E133-034 · 正式连续短窗

[VERIFIED mykernel/sched/scheduler/scheduler.h::__schedule_loop]
```c
			do {
				preempt_disable();
				__schedule(sched_mode);
				sched_preempt_enable_no_resched();
			} while (need_resched());
```

固定time物理行 282–286；活动函数体、字段、宏、类表项或配置正文。

## E133-035 · 正式连续短窗

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

固定time物理行 62–65；活动函数体、字段、宏、类表项或配置正文。

## E133-036 · 声明/头部/注释定位

[SOURCE_LOCATION mykernel/sched/scheduler/scheduler_core.c::update_rq_clock]
```c
void update_rq_clock(rq_s *rq)
```

固定time物理行 20–20；不计正式P2定义体引文，仅用于说明来源归属。

## E133-037 · 声明/头部/注释定位

[SOURCE_LOCATION mykernel/include/linux/sched/clock.h::local_clock]
```c
	static __always_inline u64 local_clock(void) {
```

固定time物理行 51–51；不计正式P2定义体引文，仅用于说明来源归属。

## E133-038 · 声明/头部/注释定位

[SOURCE_LOCATION mykernel/arch/x86_64/kernel/tsc.c::native_sched_clock]
```c
native_sched_clock(void) {
```

固定time物理行 221–221；不计正式P2定义体引文，仅用于说明来源归属。

## E133-039 · 声明/头部/注释定位

[SOURCE_LOCATION mykernel/arch/x86_64/include/asm/timer.h::native_sched_clock]
```c
	unsigned long long native_sched_clock(void);
```

固定time物理行 12–12；不计正式P2定义体引文，仅用于说明来源归属。

## E133-040 · 声明/头部/注释定位

[SOURCE_LOCATION mykernel/sched/scheduler/scheduler_const.h::RQCF_comment]
```c
	 * rq::clock_update_flags bits
	 *
	 * %RQCF_REQ_SKIP - will request skipping of clock update on the next
	 *  call to __schedule(). This is an optimisation to avoid
	 *  neighbouring rq clock updates.
```

固定time物理行 24–28；不计正式P2定义体引文，仅用于说明来源归属。

## E133-041 · 声明/头部/注释定位

[SOURCE_LOCATION mykernel/scripts/options_flags.cmake::time_canary]
```c
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
```

固定time物理行 41–41；不计正式P2定义体引文，仅用于说明来源归属。

## E133-042 · 声明/头部/注释定位

[SOURCE_LOCATION mykernel/debug/panic.c::panic_canary]
```c
	this_cpu = smp_processor_id();
```

固定time物理行 79–79；不计正式P2定义体引文，仅用于说明来源归属。
