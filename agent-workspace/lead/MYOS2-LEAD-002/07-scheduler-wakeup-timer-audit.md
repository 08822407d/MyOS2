---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
conversation_display_name: "MYOS2-A-C02 内核分析主线"
record_type: bounded_source_audit
evidence_class: "time 源码逐字锚点；静态推导与运行事实分开"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-09-24
base_snapshot: "kernel=time（分支名）；工作区=master（分支名）；写入=agent/MYOS2-LEAD-002"
read_channel: connector
content_origin: "主线本轮新核查；不是 002R/003R/004R/007R 原研究正文的恢复"
inputs_read:
  - mykernel/scripts/options_flags.cmake
  - mykernel/debug/panic.c
  - mykernel/sched/scheduler/scheduler_core.c
  - mykernel/sched/scheduler/scheduler.h
  - mykernel/sched/scheduler/myos_rt.c
  - mykernel/sched/scheduler/scheduler_macro.h
  - mykernel/time/timer/timer.c
  - mykernel/time/timer/timer.h
  - mykernel/time/systick/systick.c
  - mykernel/time/timekeeping/timekeeping.c
  - mykernel/kactive/completion/completion.c
  - mykernel/kactive/completion/completion.h
  - mykernel/kactive/swait/swait.c
  - mykernel/lib/list/double_list.h
  - mykernel/lib/list/double_list_macro.h
  - mykernel/init/main.c
  - mykernel/kactive/softirq/softirq.c
  - mykernel/kactive/softirq/softirq.h
  - mykernel/arch/x86_64/myos/interrupt.c
  - mykernel/arch/x86_64/myos/LVT_timer.c
  - mykernel/arch/x86_64/kernel/hpet.c
  - mykernel/arch/x86_64/kernel.lds
  - mykernel/CMakeLists.txt
  - mykernel/scripts/target_kernel.cmake
  - mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h
  - mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h
  - mykernel/arch/x86_64/kernel/myos_APboot.S
  - mykernel/include/linux/kernel/asm-generic/bug.h
  - mykernel/printk/printk.c
  - mykernel/sched/forkexec/fork.c
  - .vscode-kdbg/run-qemu-gdb-myos2.sh
  - agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md
  - agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-09-24-scheduling-sync-followup.md
  - "Owner 提供的第二波课题合集及九份 MANIFEST，作为历史断言与任务缺口输入"
input_scope: "scheduler_core/scheduler.h/myos_rt、completion、swait、配置、主入口、timer.c/timer.h、softirq.c/softirq.h 全文；list 只读本次所用原语与宏；fork 读 copy_process 局部及 kernel_clone/kernel_thread；printk 只读 console_init/临时 console 后端；其他具体边界见正文。"
status: STATIC_AUDIT_AND_VERIFICATION_DESIGN_DELIVERED
runtime_verified: false
kernel_modified: false
whole_wave2_complete: false
acceptance_ceiling: PASS_PENDING_LOCAL
local_validation: "待本地；未运行命令、编译、解析器、虚拟机或测试。"
open_questions:
  - "选定源码与用户本机实际构建产物是否一致，尚未由本地核验。"
  - "计数错配、返回值、有限超时及计时增量需要独立机械/运行见证，不能据静态推导宣称已复现。"
  - "定时器到期分发、完整上下文切换、实际硬件频率和 SMP 在线路径未完成全树核查。"
---

# 先保留真实实现，再修复已经查清的契约缺口

**最重要的更正：当前所读源码并非“唤醒永远不入队”。** 它没有走完整 Linux 风格的 `ttwu_queue/enqueue_task`，但有活动的 MyOS2 自有链：`try_to_wake_up → set_task_cpu → running_lhdr`，调度器也从这个链表取任务。旧评审把上游骨架的注释状态等同于整个调度机制失效，漏掉了这个实现。不能据此把已有调度器推倒重写。

同时发现的可行动问题是：唤醒可以产生状态/队列副作用却正常返回 0；有限超时返回值不递减；swait 的链表删除与计数维护不配套；HPET 路径可能对同一链接符号连续递增两次。本报告完成源码核对、静态反例和验证设计，不冒称这些现象已经在用户机器上复现。

## 1. 读的是什么，以及没做什么

本件是 LEAD-002 在工作令可读核查范围内做的独立补充，不接管原 007R 作者身份，不受它“只读外部材料”的研究角色限制；也不改写原研究或分级。源码来自 `time`，规则/记录来自 `master`。代码搜索仅用于找路径，默认分支搜索片段没有当作 time 证据。

实际核对的分支金丝雀：time 的 C 参数为 `-m64 -mcmodel=kernel -fno-pie -fno-pic`，对应 master 为 `-m64 -mcmodel=large -fPIE`；time 的 panic 赋值使用 `smp_processor_id()`，master 使用 `raw_smp_processor_id()`。因此旧评审关于某次“内核零差异”的历史陈述不能充当本次两分支相同的证明。机械金丝雀检查仍待本地。

A01｜配置面先行：所读 C 参数定义包含以下内容。后文否定性判断只针对实际读过的定义体，不能推广成全仓不可达证明。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```

未执行构建、预处理、全树 grep、ELF 检查、QEMU、GDB 或任何原脚本。文件进入构建输入、某个分支在预处理后保留、链接产物包含代码、实际到达代码、行为正确，是五个不同结论。

## 2. 唤醒的活动实现与旧评审更正

### CA-01｜自有入队路径存在；旧“永不入队”结论撤回到正确范围

A02｜`set_task_cpu` 的活动尾部不仅设置目标，还设置任务状态并操作自有运行链表：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::set_task_cpu]
```c
	rq_s *target_rq = &(per_cpu(runqueues, new_cpu));
	p->__state = TASK_RUNNING;
	if (!list_header_contains(&target_rq->myos.running_lhdr, &p->rt.run_list))
		list_header_add_to_head(&target_rq->myos.running_lhdr, &p->rt.run_list);
```

A03｜`try_to_wake_up` 初始化目标 CPU 和返回量：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::try_to_wake_up]
```c
	ulong flags;
	int cpu = 0, success = 0;
```

A04｜非 current 路径实际调用选择函数：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::try_to_wake_up]
```c
	cpu = select_task_rq(p, cpu, wake_flags | WF_TTWU);
```

A05｜同一路径实际调用上述自有入队函数：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::try_to_wake_up]
```c
		set_task_cpu(p, cpu);
```

A06｜函数结尾正常返回 success：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::try_to_wake_up]
```c
	preempt_enable();

	return success;
```

A07｜本次读到的选择函数保留传入 CPU；其内部策略与亲和性选择代码被注释：

[VERIFIED mykernel/sched/scheduler/scheduler.h::select_task_rq]
```c
			return cpu;
```

A08｜外部唤醒入口直接传递该返回值：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_process]
```c
	return try_to_wake_up(p, TASK_NORMAL, 0);
```

A09｜新任务的路径也直接操作同一个自有队列；完整函数前部选的是 CPU 0 的 rq：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task]
```c
	list_header_add_to_head(&rq->myos.running_lhdr, &p->rt.run_list);
```

A10｜自有调度类确实消费这个队列，不是只有一个无人使用的插入函数：

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		List_s * next_lp = list_header_remove_head(&myos_rq->running_lhdr);
		while (next_lp == NULL);
```

A11｜回调被写入调度类实例：

[VERIFIED mykernel/sched/scheduler/myos_rt.c::myos_rt_sched_class]
```c
	.yield_task			= yield_task_fair_myos,
	.pick_next_task		= pick_next_task_myos,
```

A12｜类遍历实际调用该类接口：

[VERIFIED mykernel/sched/scheduler/scheduler.h::__pick_next_task]
```c
					p = class->pick_next_task(rq, prev);
					if (p)
						return p;
```

**[INFERRED] 静态裁定：** A02/A04/A05 构成对“因 ttwu_queue 被注释，所以永不入队”的直接反例。完整读取该函数后，没有看到对 success 的活动更新；因此正常返回值仍为 0。`p == current` 分支设置 TASK_RUNNING 后直接跳出，其状态匹配检查同样被注释；非 current 路径也未执行原有状态掩码筛选。不能把返回 0 解释成“没有发生任何唤醒副作用”。这是源码契约不一致，不是所有场景均挂死的证明。

**旧结论处置：** 对 WAVE-1-REVIEW §4.4 及引用该句的材料，只撤回“永不入队/整条活动链不存在”的扩大结论；“success 返回值没有闭合”“generic enqueue_task 主体被注释”仍分别保留。旧文件不改，通过本件修正消费规则。不得将旧问题直接映射成“补上 ttwu_queue 即完成修复”，否则可能与自有入队发生重复。

### CA-02｜调度有活动路径，但空队列与阻塞状态需要独立检验

A13｜普通 schedule 循环会调用核心调度：

[VERIFIED mykernel/sched/scheduler/scheduler.h::__schedule_loop]
```c
			do {
				preempt_disable();
				__schedule(sched_mode);
				sched_preempt_enable_no_resched();
			} while (need_resched());
```

A14｜所读中断/异常公共处理函数在条件满足时也调用 schedule：

[VERIFIED mykernel/arch/x86_64/myos/interrupt.c::excep_hwint_context]
```c
	if (!in_atomic())
		schedule();
```

A33｜自有选择器把非空队列作为整个取下一任务分支的条件：

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	if ((need_resched() ||
		curr_task == rq->idle ||
		used_jiffies >= rt->time_slice ||
		curr_task->__state != TASK_RUNNING) &&
		myos_rq->running_lhdr.count > 0)
```

A34｜它的默认候选是 current：

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	retval = curr_task = current;
```

A44｜核心函数实际选择下一任务：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	next = pick_next_task(rq, prev, &rf);
```

A45｜prev 与 next 不同的分支实际调用上下文切换：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
		rq = context_switch(rq, prev, next, &rf);
```

**[INFERRED] 有条件反例：** 若入口是非 runnable 的 current 且自有队列 count 为 0，A33 分支不执行，函数尾部返回默认 current。本轮没有证明实际工作负载必然达到这个组合；但应先明确 idle、运行中任务、队列成员和 count 的不变量，再验证边界。不能仅凭已能切换两个任务就证明阻塞语义正确，也不能凭 generic block_task 被注释就忽略自有选择器中“只重新插入 TASK_RUNNING”的逻辑。

## 3. 有限等待与无限等待不能混为一谈

### CA-03｜有限 timeout 的返回值不随到期推进

A15｜有限非负 timeout 路径设置栈上定时器后立即删除，调度调用是注释：

[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
	timer_setup_on_stack(&timer.timer, process_timeout, 0);
	__mod_timer(&timer.timer, expire, MOD_TIMER_NOTPENDING);
	// schedule();
	del_timer_sync(&timer.timer);
```

A16｜完整函数中未计算剩余时间，返回式保留原非负入参：

[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
	return timeout < 0 ? 0 : timeout;
```

A17｜msleep 依赖这个返回量降至 0：

[VERIFIED mykernel/time/timer/timer.c::msleep]
```c
	while (timeout)
		timeout = schedule_timeout_uninterruptible(timeout);
```

A18｜不可中断包装先改变状态，再调用 timeout：

[VERIFIED mykernel/time/timer/timer.c::schedule_timeout_uninterruptible]
```c
	__set_current_state(TASK_UNINTERRUPTIBLE);
	return schedule_timeout(timeout);
```

**[INFERRED] 精确推导：** 对普通有限正 timeout，在辅助调用正常返回的条件下，循环递推为 T_next = T，不能由这个返回式正常结束 msleep。辅助调用若自身停住，也不构成成功等待。不能进一步声称本地已经观察到死循环、所有非零毫秒值均无其他边界，或把该问题写成“缺少一个 schedule 调用”就算完整解决：返回剩余时间、任务状态、到期调度和定时器生命周期都必须一起核对。

A19｜特殊无限等待分支确有调度调用：

[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
		schedule();
		goto out;
```

A20｜公开的 completion 等待入口使用的正是该特殊值：

[VERIFIED mykernel/kactive/completion/completion.c::wait_for_completion]
```c
	wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
```

因此不能把有限 timeout 的错误直接推广成“wait_for_completion 从不调度”。需要分别验证已经完成的快路径、先等待后通知、无限等待、有限超时。

A40｜启动侧存在完成通知：

[VERIFIED mykernel/init/main.c::rest_init]
```c
	complete(&kthreadd_done);
```

A41｜kernel_init 会等待它：

[VERIFIED mykernel/init/main.c::kernel_init]
```c
	wait_for_completion(&kthreadd_done);
```

**[INFERRED] 验证限制：** 单次启动越过这个点不证明阻塞唤醒分支经过；通知先发生时可走 done 已设置的路径。这里不假定所有抢占时序，也不把源码顺序当成运行跟踪。

## 4. swait 的链表与计数契约

### CA-04｜一次摘链后 count 可能仍然为 1

A21｜准备等待时通过带头计数的接口入队：

[VERIFIED mykernel/kactive/swait/swait.c::__prepare_to_swait]
```c
	wait->task = current;
	if (list_is_empty_entry(&wait->task_list))
		list_header_add_to_tail(&q->task_list_hdr, &wait->task_list);
```

A22｜该接口增加头部 count：

[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_tail]
```c
			list_add_to_prev(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
```

A23｜唤醒时先按头计数检查，然后使用不携带头部参数的摘链原语：

[VERIFIED mykernel/kactive/swait/swait.c::swake_up_locked]
```c
	if (list_header_is_empty(&q->task_list_hdr))
		return;
	curr = list_headr_first_container(&q->task_list_hdr, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
```

A24｜这个摘链函数只处理节点链接并重新自指：

[VERIFIED mykernel/lib/list/double_list.h::list_del_init]
```c
			__list_del_entry(entry);
			INIT_LIST_HEAD(entry);
```

A25｜空表判断却只检查 count：

[VERIFIED mykernel/lib/list/double_list.h::list_header_is_empty]
```c
			return READ_ONCE(lhdr_p->count) == 0;
```

A26｜收尾在节点已经自指时不会调用带 count 维护的删除：

[VERIFIED mykernel/kactive/swait/swait.c::__finish_swait]
```c
	__set_current_state(TASK_RUNNING);
	if (!list_is_empty_entry(&wait->task_list))
		list_header_delete_node(&q->task_list_hdr, &wait->task_list);
```

A27｜取首节点的宏从 anchor.next 得到容器，而不是再次检查 count/anchor 是否一致：

[VERIFIED mykernel/lib/list/double_list_macro.h::list_headr_first_container]
```c
	#define list_headr_first_container(ptr, type, member) \
				list_first_entry(&((ptr)->anchor), type, member)
```

A42｜completion 的完成方会使用该唤醒过程：

[VERIFIED mykernel/kactive/completion/completion.c::complete]
```c
	if (x->done != UINT_MAX)
		x->done++;
	swake_up_locked(&x->wait, 0);
```

**[INFERRED] 静态见证：** 初始为空；单个合法 waiter 入队后 count=1；在没有并发修改、唤醒调用正常返回的情况下，A23/A24 摘去唯一节点，而头计数不减；于是物理链接为空但 A25 仍报非空，A26 不修正计数。下一次唤醒可能把 anchor 当作 waiter 容器。需要独立局部复现验证这个状态序列及后果，不把“可能错误访问”写成已经崩溃。`swake_up_all_locked` 也按同一空表判据循环，须列入验证。

本项作为主线新候选 CA-04 保存，不自行占用 DR008 新编号，也不把未由旧编号授权的缺陷悄悄写进 002R 的 correctness_flag。

## 5. 时钟前进不等于到期回调已经工作

### CA-05｜HPET 的两个增量通过链接脚本指向同一存储

A28｜HPET handler 连续进行两次操作：

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::HPET_handler]
```c
	jiffies++;
	do_timer(1);
```

A29｜do_timer 增加 jiffies_64：

[VERIFIED mykernel/time/timekeeping/timekeeping.c::do_timer]
```c
	jiffies_64 += ticks;
```

A30｜所读 x86 链接脚本将两者定义为同一地址：

[VERIFIED mykernel/arch/x86_64/kernel.lds::jiffies]
```ld
jiffies = jiffies_64;
```

A31｜目标配置选用该架构的链接脚本：

[VERIFIED mykernel/scripts/target_kernel.cmake::target_link_options]
```cmake
target_link_options(kernel PRIVATE
    -T ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kernel.lds
)
```

A46｜源码收集由根 CMake 明示，不能仅因子目录 CMake 为空就认定代码未编入：

[VERIFIED mykernel/CMakeLists.txt::KERNEL_C_SRCS]
```cmake
file(GLOB_RECURSE KERNEL_C_SRCS ${PROJECT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE KERNEL_ASM_SRCS ${PROJECT_SOURCE_DIR}/*.S)
```

**[INFERRED] 条件性结论：** 在所读 x86 构建和符号绑定保持一致、handler 正常走完的条件下，一次调用有两个对同一计数对象的增量。验证应先核 ELF 符号/类型及一次 handler 前后值，再判断实际时基；这里没有证明实际中断频率、墙钟两倍或所有 timeout 的真实倍率。

A32｜LVT 的 handler 则增加另外的调试计数：

[VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c::LVT_timer_handler]
```c
	lvt_count++;
```

A43｜定时器到期回调的目标是唤醒任务：

[VERIFIED mykernel/time/timer/timer.c::process_timeout]
```c
	proc_timer_s *timeout = from_timer(timeout, t, timer);
	wake_up_process(timeout->task);
```

本輪补齐读取了 timer.c 的中间段及 timer.h、softirq.h。已读 HPET/LVT handler、timer.c/timer.h、softirq.c/softirq.h 仍未给出“计时推进 → 取出到期 timer → 调用 process_timeout”的完整分发链；其他潜在分发者及最终链接输入尚待本地全树核查。因此本件明确标为 **链未闭合**，不写“全仓不存在 runner”。中断出口有 A14 调度，也不能代替到期回调。定时器注释宣称到期会调用 function，同样不能替代活动分发语句。

## 6. 验证底座与已知原语问题

### CA-06｜现有观测工具不能把缺少输出当作通过

A35｜所读 CONFIG_BUG 分支的 WARN_ON 只计算条件：

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::WARN_ON]
```c
	#		define WARN_ON(condition) (condition)
```

A36｜现有 QEMU 启动脚本为测试盘指定普通 drive：

[VERIFIED .vscode-kdbg/run-qemu-gdb-myos2.sh::(top-level)]
```bash
  -drive "file=${DISK_IMG},if=virtio,format=raw" \
```

A37｜脚本的 READY 对应 GDB 端口可连接：

[VERIFIED .vscode-kdbg/run-qemu-gdb-myos2.sh::(top-level)]
```bash
    nc -z 127.0.0.1 "$GDB_PORT" >/dev/null 2>&1 && { ready=1; log_ready; break; }
```

完整脚本还固定 `-smp 1`、使用 `-S`，并会处理 PID 文件、旧进程和可写固件变量副本。本轮没有运行它。**[INFERRED]** 端口 READY 不等于内核启动完成；该 drive 行没有提供测试盘只读或快照保证，不能直接将脚本用于无人值守的新实验。局部验证优先不启动虚拟机；需要 VM 时必须另确认隔离副本、工具版本、可观察通道与超时终止合同，不接触用户原测试盘。

### CA-07｜两个原语问题仍可独立核查，不能由 lockdep 代证

A38｜加法判负函数的活动汇编是 subl：

[VERIFIED mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h::arch_atomic_add_test_negative]
```c
			bool c;
			asm volatile(LOCK_PREFIX
							"subl	%2,		%0		\n\t"
```

A39｜trylock 活动体没有申请票据或写锁所有权：

[VERIFIED mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h::arch_spin_trylock]
```c
			u64 old = arch_atomic64_read(&lock->val);
			if ((old >> 32) != (old & 0xffffffff))
				return false;
			else
				return true;
```

**[INFERRED]** 应分别检测算术结果与返回标志、trylock 成功后锁状态及第二次竞争结果。不能据此说所有 spin_lock 路径都同样错误：实际 arch_spin_lock 另有 xadd 实现。本件不复制一个猜测补丁，也不把“lockdep 无报警”当作原语互斥证明。

A47｜AP 调试入口尾部保留原地跳转：

[VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::AP_DEBUG]
```asm
	jmp		.
```

这与所读主入口的 SMP 调用注释、现有启动脚本的单 CPU 设置一起，限制了当前可提出的并发测试前提；没有全树扫描或运行观察，不以此单独宣称所有可能 AP 路径均不存在。

## 7. 由本次证据形成的验证优先序（提案，不是正式阶段 3 路线）

| 顺序 | 先证明什么 | 为什么这样安排 | 不能代替的下一层 |
|---|---|---|---|
| V0 | 输入分支、构建引用、47 个锚点与结构化记录一致 | 避免再次用错分支/旧评审/空模板 | 不证明运行 |
| V1 | 单线程局部的 swait 计数与原语契约 | 不依赖未完成的 SMP/定时器即可发现确定性状态错配 | 不证明唤醒整链 |
| V2 | 自有队列的入队、重复唤醒、状态掩码、空队列/idle | 先保住 MyOS2 的活动实现及其不变量 | 不证明定时到期 |
| V3 | 有限 timeout 与无限 completion 的不同合同 | 避免把一个修复扩成所有等待都通过 | 不证明硬件时基 |
| V4 | ELF 的计数别名和安全环境中的单事件增量 | 防止用错误时基解释等待测试 | 不证明长期精度或 SMP |

具体 15 个验证条目、隔离与返回合同已交 `09-local-verification-contract.md`，供本地执行面一次完成可做部分。不会要求 Owner 先答内部流程问卷，也不要求再启动九项深度研究。现阶段源码结论尚不能签发运行验收；跨证据审查完成后，下一步真正需要的是独立机械证据，不是另一次主线对同一源码的同义复述。

## 8. 可消费范围与回溯

本报告 47 个源码锚点按 A01-A47 标识，编号为证据索引，不是新任务号。配套 `07-core-audit-map.yaml` 存储问题、实际调用关系、未闭合边、反例前提和验证入口；其节点不替换 002R 全矩阵、003R 全图或 007R 分级。

源码引用统一解析为 `https://github.com/08822407d/MyOS2/blob/time/<path>`；配置对照用 master 的同名路径。旧“永不入队”出处为 `agent-workspace/WAVE-1-REVIEW.md` §4.4，亦见 Owner 提供课题合集背景节。本轮没有将历史材料的独立发现直接升级为本轮核实结论。

同族局限声明：本裁定由 GPT 主线对 GPT 研究材料及旧评审继续核查，不构成异族独立复核。源码逐字存在、静态反例成立、实际构建可复现、系统行为正确分别记录；最后两层待本地。附件、完整聊天历史和外部资料未在本轮完成精确字节归档。
