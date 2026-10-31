---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SIGNAL-SEND-WAKE-10
record_type: evidence_methods_and_delivery_preflight
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
produced_by: "ChatGPT普通对话（Owner选择GPT-6 / Extra；backend未独立认证）"
date: "2026-10-31"
read_channel: connector
base_snapshot: "source=time @ a039d9803ade；old_results=037b880e792b；instruction=0e2a53c3dcc3"
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
source_quote_checks: "facts中52条1-5行引文从固定文件行提取；article中20处[VERIFIED]逐字匹配事实锚点"
mechanical_checks: "有界JS字符/ID/图一致核验，未执行C/ASM或仓库检查器"
remote_readback: "留在本批完成后的PR17评论；本文件不自引用最终提交"
owner_action_now: none
open_questions:
  - "实际编译、预处理与内核运行未授权"
  - "全部间接signal生产者、PID并发/全局调度正确性未核"
---

# 证据与检查记录 · S00–S04

## 1. 固定对象、读取范围与真实性等级

**唯一技术基线**为 `time @ a039d9803ade`；任务书固定版本 `065abf7e8677`，其约束来源 `agent/MYOS2-LEAD-002`，旧执行结果固定 `037b880e792b`。本批只读取源码，未用master同名源码作为结论依据；Github默认分支搜索只用于定位实际路径，随后定点在固定time核查。

主要实际读取集合：

| 项 | 固定源码及使用范围 | 限制 |
|---|---|---|
| 配置与系统调用 | `mykernel/scripts/options_flags.cmake` 全文；`lock_IPC/syscall.c` 全文；`include/linux/kernel/syscalls.h` 的定义宏；`arch/x86_64/include/asm/{syscall_wrapper.h,syscalls_64.h,unistd_64.h}` 中实际参数/分派/编号 | 无CMake/预处理/编译；保留外部 `CMAKE_C_FLAGS` 变量可能注入的未知 |
| 发送、队列、接收 | `mykernel/lock_IPC/signal/signal.c` 全文分区/选定段，`signal.h`、`signal_types.h`、`signal_const.h`、`signal_macro.h` 与 `signal_api.h` 的定点范围 | 不全量审计其他异步生产者、PID并发、slab内部 |
| PID与状态 | `sched/task/{pid.c,pid_api.h,pid.h,pid_macro.h,task.h}` 实际相关函数；`sched/forkexec/fork.c` 的 `copy_signal`/`init_sigpending`/相关构造；`include/uapi/asm-generic/signal.h` 的64/32界限 | 不对PID分配回收、锁、线程组初始化作全树正确性证明 |
| 唤醒接口 | `sched/scheduler/{scheduler_core.c,scheduler_api.h}` 的 `kick_process`、`wake_up_state`、`try_to_wake_up` 定点范围，`init/main.c` 的 `start_kernel` 初始化点 | 只作静态接口与注释活动性识别，不追CPU/队列全流程 |
| 旧材料 | 09主说明的范围索引和D02-b、RS-03/04/12/13/14对应原记录；DOC-FIX-01勘误及C03限界验收；07/08只沿既有ID追接口 | 不是重新运行09的262查询、75锚点、全批审查 |

GitHub写入目标仓库 `08822407d/MyOS2` 当前public，因此本批只提交公开源码的最小事实与脱敏分析；不写本机个人路径、凭据、私有资料或完整旧会话。

## 2. 活动/注释界定及额外定点引用

源码中函数存在与当前kill路径上的**活动调用边**必须分开。`__send_signal_locked` 的唯一当前 `complete_signal` 语句是注释（SW10-A26），而潜伏函数内部的目标线程指针没有在当前函数体活动路径获得赋值。新增以下补足引文，用于F05的潜伏定位，不把它提升为当前sys_kill已到达：

[VERIFIED mykernel/lock_IPC/signal/signal.c::complete_signal]（SW10-X01）

~~~c
complete_signal(int sig, task_s *p, enum pid_type type) {
	signal_s *signal = p->signal;
	task_s *t;
~~~

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::try_to_wake_up]（SW10-X02）

~~~c
int try_to_wake_up(task_s *p, uint state, int wake_flags)
{
	ulong flags;
	int cpu = 0, success = 0;
~~~

[VERIFIED mykernel/lock_IPC/signal/signal.c::get_signal]（SW10-X03）

~~~c
relock:
	spin_lock_irq(&sighand->siglock);
~~~

[VERIFIED mykernel/entry/common.c::exit_to_user_mode_loop]（SW10-X04）

~~~c
	// 	if (ti_work & (_TIF_SIGPENDING | _TIF_NOTIFY_SIGNAL))
			arch_do_signal_or_restart(regs);
~~~


`get_signal` 当前实际取用siglock；它只证明接收端局部操作，而不能填充发送端被注释的持锁语句。返回端 `exit_to_user_mode_loop` 内按TIF测试的外层行已注释、调用本身活动，故沿已有09的`syscall_exit_to_user_mode`路径可以扫描；不扩大为任意异常/中断出口均扫描。`try_to_wake_up` 当前实现的成功变量初始化及 `ttwu_queue` 注释由直接接口阅读确认，作为**未接入本kill链的潜伏事实**，没有运行或调度保证。

## 3. 实际机械核对与独立性局限

- [已执行：连接器读取及有界文本处理] 对固定time源码以原行切片生成 `facts-and-boundaries.yaml` 的52段引文；每段1～5行，记录路径、符号和实际行范围。不同引文可能落在相同函数/紧邻行，不把52误说成独立的52项语义证明。
- [已执行：有界JS字符串检查] `signal-send-wake.md` 中20段[VERIFIED]从上述锚点取出，逐字与同一份固定源引文匹配。需要额外控制流解释的地方有明确条件，不把注释、函数名或路径搜索等同运行事实。
- [已执行：图/ID核对] `facts` 有25个节点、26条边、18条输入矩阵、52段源码锚点；`send-paths.dot` 的25个节点ID与26条边ID逐项集合相等。文档与延期条目所用 `SW10-Axx` 均能在结构化锚点中定位；本检查没有调用Graphviz渲染程序。
- [已执行：GitHub增量核对] S00时PR17 head为 `037b880e792b`、open/Draft；master `e8e60cd19540`，time `a039d9803ade`。开放PR第1页只有PR16/17、第2页为空；主线PR16完整变更路径未与本批新目录交集。本批自起点到阶段性 `690025ce4233` 的比较仅有4件A新增、无旧件净修改/删除；不是最终六件交付的回读。完整远端交付对比与最终文件blob身份另记PR17交付评论。
- [未执行] PyYAML/Graphviz完整解析、代码编译器、预处理器、仓库原09检查器、实体内核或QEMU、任何C/ASM片段、真实调度、权限/锁并发测试。下述反证检查均为**源码条件审查**，不冒称动态测试。

## 4. 主动反证关键概括

| 待推翻的过强概括 | 固定源码中的反例/条件 | 结论 |
|---|---|---|
| “kill返回0必有siginfo节点” | SIGKILL/PF_KTHREAD跳过节点分配；分配失败仍可置pending；非特殊信号还受未初始化override_rlimit影响 | **不成立**，return0只限局部定义良好执行 |
| “pending位存在就已通知并唤醒” | `complete_signal` 的当前调用被注释，`signal_wake_up_state`不在kill活动链 | **不成立**，区分置位与通知 |
| “目标即使被唤醒也必执行handler” | blocked/出队顺序、默认/忽略动作、RS-01建帧前提，以及是否再次经过09返回尾均有限制 | **不成立**，必要条件不全 |
| “没有活动调用就证明全仓不存在” | 只核 `sys_kill` 当前直接函数集合及部分相关接口；未核函数指针/宏拼接/其他生产者 | **不作全仓不存在声明** |
| “未知override_rlimit可默认为0或成功” | 赋值分支被注释、实参仍被读取；不因右侧比较可能成立而给未初始化值默认值 | **不成立**，标语言层未定义前提 |
| “普通与实时信号必按Linux标准队列” | `legacy_queue`、按SIGRTMIN选择override、RT溢出处理都已注释 | **不成立**，仅按当前分支记录 |
| “无handler/无信号必不改帧” | 09的返回尾另有重启switch及DOC-FIX-01关于orig_ax/ax条件，不能照抄过时L1 | **沿用已修正09消费口径** |

反证不等于新的运行Bug复现；没有真实内核启动，也没有外部模型的独立交叉审查。

## 5. 执行边界和后续接收

本批专属写区为 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-signal-send-wake-10/`，仅准六文件A新增。原结果 `037b880e792b` 及其之前旧件冻结；不改PR16、PR17标题/正文/Draft状态或他人评论。六文件完成且固定提交/执行分支/远端compare通过之后，由C03主线按其合同做独立验收；执行者不能签PASS。PR17评论将写入最终短提交、六件路径、GitHub对象与增量证明，避免在此文件中自引用最终提交导致修改本件。

本批只生成静态事实、问题延期候选与回读核验，不提供内核修补代码、不构建、不实际排队/唤醒测试。`CORE-USER-RETURN-SIGNAL-09::RS-01`、`CORE-USER-VFS-BASELINE-05::GAP-U04` 的硬件/运行部分和 `GAP-U10` 继续保持旧状态；六件齐全也不证明MyOS2信号可靠性。
