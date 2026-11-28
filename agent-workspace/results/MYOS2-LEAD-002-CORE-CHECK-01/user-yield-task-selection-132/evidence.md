---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-YIELD-TASK-SELECTION-132
record_type: phase2_source_windows
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
produced_by: GPT-6 (self-report; backend/effort UNATTESTED)
model_per_owner: GPT-6＋Extra
date: 2026-11-23
base_snapshot: time
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
self_check: {verified_claims: 58, quotes_reconfirmed: 58, downgraded_to_inferred: 0}
---
# 第132批新增源码短窗与声明定位

本页仅新增第132批源窗，旧130的44条及旧131的48条均不计入本批P6。按time固定Git对象逐行、连续1–5物理行核对；保留tab及源文件CRLF。表项与源码主体有效，注释、定义头、分派清单与金丝雀分别作为定位，不证明预处理/链接/实际执行。

## E132-001 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::myos_rt]
```c
	.yield_task			= yield_task_fair_myos,
```

固定time连续物理行 66–66；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-002 · 正式定义窗

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

固定time连续物理行 62–65；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-003 · 独立定位

[SOURCE_LOCATION mykernel/sched/scheduler/myos_rt.c::myos_rt]
```c
DEFINE_SCHED_CLASS(myos_rt) = {
```

固定time连续物理行 65–65；用于区分定义头、注释、配置金丝雀或分派登记表，不作正式P2引文。

## E132-004 · 独立定位

[SOURCE_LOCATION mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
```

固定time连续物理行 41–41；用于区分定义头、注释、配置金丝雀或分派登记表，不作正式P2引文。

## E132-005 · 正式定义窗

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	va_start(ap, fmt);
	rv = vsnprintf(buf, SZ_4K, fmt, ap);
	va_end(ap);
```

固定time连续物理行 17–19；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-006 · 正式定义窗

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	write(stdout->_fileno, buf, rv);
```

固定time连续物理行 22–22；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-007 · 正式定义窗

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	return rv;
```

固定time连续物理行 25–25；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-008 · 正式定义窗

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				case 's':		/* String */
					sarg = va_arg(ap, const char *);
					sarg = sarg ? sarg : "(null)";
					slen = strlen(sarg);
```

固定time连续物理行 783–786；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-009 · 正式定义窗

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
						for (i = slen; i; i--)
						{
							sch = *sarg++;
							EMIT(sch);
```

固定time连续物理行 805–808；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-010 · 正式定义窗

[VERIFIED myinitramfs/myshell/myshell.c::run_cmd]
```c
	shell_internal_cmd[index].function(argc, argv);
```

固定time连续物理行 141–141；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-011 · 正式定义窗

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
	if (argc != 2)
		return -1;
```

固定time连续物理行 352–353；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-012 · 正式定义窗

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	if (argc != 2)
		return -1;
```

固定time连续物理行 366–367；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-013 · 正式定义窗

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	char prog_name[] = "/boot/sh";
```

固定time连续物理行 24–24；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-014 · 正式定义窗

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	char *const args[] =
			{ prog_name , "userarg_test_1", "userarg_test_2", NULL };
```

固定time连续物理行 25–26；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-015 · 正式定义窗

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	char *const envs[] =
			{ "userenv_test_1", "userenv_test_2", "userenv_test_3", NULL };
```

固定time连续物理行 27–28；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-016 · 正式定义窗

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	int rv = fork();
```

固定time连续物理行 30–30；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-017 · 正式定义窗

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
		execve(prog_name, args, envs);
```

固定time连续物理行 35–35；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-018 · 正式定义窗

[VERIFIED myinitramfs/myinit/initd.c::main]
```c
	return normal_boot(argc, argv);
```

固定time连续物理行 18–18；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-019 · 正式定义窗

[VERIFIED mykernel/sched/syscall.c::sched_yield]
```c
		rq = this_rq_lock_irq(&rf);
```

固定time连续物理行 29–29；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-020 · 正式定义窗

[VERIFIED mykernel/sched/syscall.c::sched_yield]
```c
		current->sched_class->yield_task(rq);
```

固定time连续物理行 32–32；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-021 · 正式定义窗

[VERIFIED mykernel/sched/syscall.c::sched_yield]
```c
		preempt_disable();
		// rq_unlock_irq(rq, &rf);
		sched_preempt_enable_no_resched();
```

固定time连续物理行 34–36；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-022 · 正式定义窗

[VERIFIED mykernel/sched/syscall.c::sched_yield]
```c
		schedule();
```

固定time连续物理行 38–38；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-023 · 正式定义窗

[VERIFIED mykernel/sched/syscall.c::sched_yield]
```c
	return 0;
```

固定time连续物理行 40–40；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-024 · 独立定位

[SOURCE_LOCATION mykernel/sched/syscall.c::sched_yield]
```c
		// rq_unlock_irq(rq, &rf);
```

固定time连续物理行 35–35；用于区分定义头、注释、配置金丝雀或分派登记表，不作正式P2引文。

## E132-025 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::myos_rt]
```c
	.pick_next_task		= pick_next_task_myos,
```

固定time连续物理行 67–67；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-026 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::yield_task_fair_myos]
```c
	resched_curr(rq);
```

固定time连续物理行 62–62；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-027 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::pick_next_task]
```c
#define pick_next_task	__pick_next_task
```

固定time连续物理行 976–976；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-028 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	cpu = smp_processor_id();
	rq = cpu_rq(cpu);
	prev = rq->curr;
```

固定time连续物理行 1032–1034；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-029 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	switch_count = &prev->nivcsw;
```

固定time连续物理行 1069–1069；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-030 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	prev_state = READ_ONCE(prev->__state);
```

固定time连续物理行 1078–1078；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-031 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	next = pick_next_task(rq, prev, &rf);
```

固定time连续物理行 1116–1116；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-032 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	clear_tsk_need_resched(prev);
	clear_preempt_need_resched();
```

固定time连续物理行 1118–1119；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-033 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	if (likely(prev != next)) {
		rq->nr_switches++;
```

固定time连续物理行 1124–1125；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-034 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
		RCU_INIT_POINTER(rq->curr, next);
```

固定time连续物理行 1130–1130；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-035 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
		++*switch_count;
```

固定time连续物理行 1153–1153；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-036 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
		rq = context_switch(rq, prev, next, &rf);
```

固定time连续物理行 1162–1162；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-037 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	local_irq_enable();
```

固定time连续物理行 1169–1169；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-038 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	local_irq_disable();
```

固定time连续物理行 1041–1041；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-039 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	rq->clock_update_flags <<= 1;
	update_rq_clock(rq);
	rq->clock_update_flags = RQCF_UPDATED;
```

固定time连续物理行 1065–1067；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-040 · 独立定位

[SOURCE_LOCATION mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	// 	switch_count = &prev->nvcsw;
```

固定time连续物理行 1113–1113；用于区分定义头、注释、配置金丝雀或分派登记表，不作正式P2引文。

## E132-041 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::schedule]
```c
	task_s *tsk = current;
```

固定time连续物理行 1203–1203；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-042 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::schedule]
```c
	if (!task_is_running(tsk))
		sched_submit_work(tsk);
	__schedule_loop(SM_NONE);
	sched_update_worker(tsk);
```

固定time连续物理行 1209–1212；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-043 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler.h::__pick_next_task]
```c
			for_each_class(class) {
```

固定time连续物理行 258–258；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-044 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler.h::__pick_next_task]
```c
					p = class->pick_next_task(rq, prev);
					if (p)
						return p;
```

固定time连续物理行 264–266；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-045 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler.h::__schedule_loop]
```c
			do {
				preempt_disable();
				__schedule(sched_mode);
				sched_preempt_enable_no_resched();
			} while (need_resched());
```

固定time连续物理行 282–286；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-046 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_macro.h::for_each_class]
```c
	#define for_each_class(class)			\
			for_class_range(class,			\
				__sched_class_highest,		\
					__sched_class_lowest)
```

固定time连续物理行 25–28；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-047 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/scheduler_const.h::SM_NONE]
```c
	#define SM_NONE				0
```

固定time连续物理行 14–14；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-048 · 独立定位

[SOURCE_LOCATION mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
__schedule(int sched_mode) {
```

固定time连续物理行 1018–1018；用于区分定义头、注释、配置金丝雀或分派登记表，不作正式P2引文。

## E132-049 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	retval = curr_task = current;
	sched_rt_entity_s *rt = &curr_task->rt;
```

固定time连续物理行 9–10；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-050 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	ulong used_jiffies = jiffies - myos_rq->last_jiffies;
```

固定time连续物理行 12–12；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-051 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	if ((need_resched() ||
		curr_task == rq->idle ||
		used_jiffies >= rt->time_slice ||
		curr_task->__state != TASK_RUNNING) &&
		myos_rq->running_lhdr.count > 0)
```

固定time连续物理行 14–18；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-052 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		BUG_ON(curr_task->__state != TASK_RUNNING &&
				myos_rq->running_lhdr.count <= 0);
```

固定time连续物理行 20–21；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-053 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		List_s * next_lp = list_header_remove_head(&myos_rq->running_lhdr);
		while (next_lp == NULL);
```

固定time连续物理行 24–25；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-054 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		if (curr_task->__state == TASK_RUNNING)
		{
			if (curr_task == rq->idle)
```

固定time连续物理行 28–30；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-055 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
				List_s * tmp_list = myos_rq->running_lhdr.anchor.next;
				sched_rt_entity_s *tmp_rt = container_of(tmp_list, sched_rt_entity_s, run_list);
				while ((curr_task->se.vruntime > container_of(tmp_rt, task_s, rt)->se.vruntime) &&
						tmp_list != &myos_rq->running_lhdr.anchor)
```

固定time连续物理行 37–40；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-056 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
				{
					tmp_list = tmp_list->next;
				}
				list_add_to_prev(&rt->run_list, tmp_list);
				myos_rq->running_lhdr.count++;
```

固定time连续物理行 41–45；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-057 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		if (curr_task != rq->idle)
			curr_task->se.vruntime += used_jiffies;
```

固定time连续物理行 49–50；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-058 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		sched_rt_entity_s *next_rt = container_of(next_lp, sched_rt_entity_s, run_list);
		retval = container_of(next_rt, task_s, rt);
		myos_rq->last_jiffies = jiffies;
```

固定time连续物理行 52–54；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-059 · 正式定义窗

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	rq->curr = retval;
	return retval;
```

固定time连续物理行 56–57；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-060 · 正式定义窗

[VERIFIED mykernel/lib/list/double_list.h::list_header_remove_head]
```c
			if (lhdr_p->count > 0) {
				List_s *ret_val = lhdr_p->anchor.next;
				list_del_init(ret_val);
				lhdr_p->count--;
				return ret_val;
```

固定time连续物理行 653–657；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-061 · 正式定义窗

[VERIFIED mykernel/lib/list/double_list.h::list_add_to_prev]
```c
			__list_add_between(new, head->prev, head);
```

固定time连续物理行 222–222；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-062 · 正式定义窗

[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_tail]
```c
			list_add_to_prev(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
```

固定time连续物理行 665–666；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-063 · 正式定义窗

[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_sched_yield]
```c
	#define __NR_sched_yield	24
```

固定time连续物理行 32–32；位于所示函数体、活动宏、类初始化器或CMake设置正文。

## E132-064 · 独立定位

[SOURCE_LOCATION mykernel/arch/x86_64/include/asm/syscalls_64.h::sys_sched_yield]
```c
__SYSCALL(__NR_sched_yield, sys_sched_yield)
```

固定time连续物理行 24–24；用于区分定义头、注释、配置金丝雀或分派登记表，不作正式P2引文。

## E132-065 · 独立定位

[SOURCE_LOCATION mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

固定time连续物理行 79–79；用于区分定义头、注释、配置金丝雀或分派登记表，不作正式P2引文。
