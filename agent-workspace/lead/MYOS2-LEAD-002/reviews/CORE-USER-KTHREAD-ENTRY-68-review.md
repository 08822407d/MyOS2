---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报，非后台认证）
model_per_owner: gpt pro（沿用Owner此前明确选择）
backend_identity: unknown_or_not_attestable
date: '2026-11-10'
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
publication_status: PREPARED_FOR_FIXED_COMMIT_POSTWRITE_RECEIPT_REQUIRED
status: REVIEW_COMPLETE_IN_BOUNDED_STATIC_SCOPE
runtime_evidence: NOT_RUN
open_questions: []
record_id: CORE-USER-KTHREAD-ENTRY-68-REVIEW-001
record_type: bounded_static_review_with_mainline_supplements
evidence_class: 固定六件、正式评论、固定源码及纯文本/数学检查；非运行验收
reviewed_base_short12: 157bf5604754
reviewed_head_short12: 4d45fa939fa1
reviewed_delivery_comment: 6762093351
contract99_short12: 09aa0ff5dc48
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
subjects:
  D00: ACCEPTED_R67_01_DOCUMENTATION_CLOSED
  A: ACCEPTED_IN_SCOPE
  B: ACCEPTED_WITH_R68_S01
  C: ACCEPTED_WITH_R68_S01_S03
mandatory_supplements:
- R68-S01
- R68-S02
- R68-S03
- R67C-S01
full_local_P9: NOT_RUN
original104_automatic_byte_definition_scan: NOT_RUN
original_six_file_automatic_prefix_scan: NOT_RUN
original_facts_deferred_full_JSON_parse: NOT_RUN
self_check:
  verified_claims: 6
  quotes_reconfirmed: 6
  downgraded_to_inferred: 0
  scope: 本回执6条直接源锚；不与原68的104混计
next_followup: CORE-USER-INIT-EXEC-HANDOFF-69
next_task_ref: ../100-user-init-exec-handoff-contract.md
executor: MYOS2-A-C05b / MYOS2-LEAD-002-CHAT-EXEC-08
inputs_read:
- agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md
- agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md
- agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md
- agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md
- agent-workspace/lead/MYOS2-LEAD-002/99-user-kthread-entry-contract.md
- agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-kthread-entry-68/MANIFEST.md
- agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-kthread-entry-68/kthread-entry.md
- agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-kthread-entry-68/facts-and-boundaries.yaml
- agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-kthread-entry-68/kthread-entry-paths.dot
- agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-kthread-entry-68/deferred-findings.yaml
- agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-kthread-entry-68/evidence.md
- mykernel/sched/forkexec/kthread.c
- mykernel/sched/forkexec/fork.c
- mykernel/init/main.c
- mykernel/arch/x86_64/myos/arch_task.c
- mykernel/sched/forkexec/exit.c
- mykernel/sched/scheduler/scheduler_core.c
---

# 第68批限定接收，无新增返工项

本C05结论：PASS_PENDING_LOCAL，D00及A/B/C按本回执限制接收；R67-01另以文档闭合回执处理。不签运行PASS，不要求执行者为本回执的补充说明另交勘误包。先前受阻的是主线发布，不是第68批交付。本文件此次恢复发布，保留已完成的审查范围和裁定；实际远端提交、导航及评论回读由后置正式评论记录。

## 1. 交付与版本核查

GitHub compare 157bf5604754→4d45fa939fa1为ahead1/behind0、一个提交，精确六件added：MANIFEST141、主件74、facts866、evidence1045、DOT101、deferred147，共2374行；无旧文件净修改/删除。主线/PR16仍09aa0ff5dc48，执行分支/PR17为4d45fa939fa1，direct time=a039d9803ade、master=e8e60cd19540。两PR均Open/unmerged，PR17 Draft；PR返回的base_sha缓存不替代直接master引用。
正式评论6762093351全文已读，时间为2026-11-10 07:55:32Z。03:32Z以来评论集合只返回这一条68正式交付，未见重复。作者说此前网页显示异常后补齐评论；主线仅核实远端交付已存在，不据此诊断客户端故障或声称监控了另一对话进程。

## 2. 可读闸门和实际覆盖

六件全文已读：evidence分1–300、301–620、621–900、901至EOF1045；facts分1–300、301–600、601至EOF866；正文1–52及53至EOF74；MANIFEST141、deferred147、DOT101全文。没有把工具截断的URL尾部误当正文缺失。

| 判据 | 本次结果与方法 |
|---|---|
| P9-1 | 通读未见40位十六进制串；原六件自动扫描NOT_RUN，待本地复核。 |
| P9-2/13 | A1–29、B1–38、C1–37共104短锚已逐组阅读；强制入口/回调/退出锚全部对照真实固定定义，其他旁证人工比对覆盖超过30%；无已发现的空围栏、跨独立定义或悬空引用。全104自动字节/符号边界扫描NOT_RUN。 |
| P9-3/4 | 十二个源码路径及六件固定文件可读；附带行范围按当前/本有效会话同pin原文核，未发现越文件范围；完整本地路径/行数脚本NOT_RUN。 |
| P9-5 | 人工枚举104个源码标签，其他五件无说明性额外命中；MANIFEST为104/104/0；不冒称已执行全六件grep。 |
| P9-6 | 无以未核外部手册作CPU事实的断言；硬件/ABI仍为明确前提。 |
| P9-7/8 | 使用本有效会话已直接取得的time/master两组金丝雀及公约原句核对新MANIFEST，字段齐全；本轮不虚称重新下载四个金丝雀文件。 |
| P9-9/10/12 | 无代码件、补丁或聊天降级交付，相应项N/A。 |
| P9-11 | 未声明内核编译/运行成功；所有此类行为NOT_RUN。 |

新读源包括arch_task.c全文、exit.c1–205（含完整do_exit及前置exit_notify）、scheduler_core.c1160–1258（含完整do_task_dead、schedule_preempt_disabled）、task_const.h98–120、sched_type_declaration.h1–48、switch_to_arch.h全文。其余核对复用本有效C05已实际获取的固定原文：fork.c的kernel_clone/kernel_thread完整体及copy_process窗口，kthread.c全文，main.c全文，process_arch.c完整copy_thread/ret_from_fork，kernel_clone_args类型及entry_64.S完整标签。并非继承C05b的“已读”声明。
对非强制锚的明确核对集合至少包括B01–B38全部38条（其源为本有效会话已完整取得的kthread.c），另外A参数/分支/类型及C启动与终止锚均有相应原文；不存在只核几条却声称达到抽样门的情况。P6标签计数与源命题判断分账。

## 3. A/B/C技术主干

A成立：kernel_thread将fn/arg/name/flags写入独立kclone_args字段，copy_process设置PF_KTHREAD；copy_thread在内核分支初始化bx/r12并早退，所以args.stack中函数地址的数值不能当作普通用户sp覆盖已经执行。kernel_clone返回PID与wake_up_new_task源调用不证明孩子已开始运行。

B成立：首帧入口为包装器，业务回调另存请求与局部变量。
[VERIFIED mykernel/sched/forkexec/kthread.c::create_kthread]
```c
	pid = kernel_thread(kthread, create, create->full_name,
			CLONE_FS | CLONE_FILES | SIGCHLD);
```

包装器的业务返回后走退出调用，而不是直接return到ret_from_fork。
[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
		ret = threadfn(data);
	}
	kthread_exit(ret);
}
```

done非空、对象有效、完成通知与调度恢复、STOP未置位是业务调用的前提；发布create.result/complete(done)与业务函数开始不同层。外部threadfn与首帧kthread不能混为一个函数。
C成立：rest_init的两次直接入口为kernel_init/kthreadd，通用请求入口为kthread；所选函数的局部正常控制未建立返回到67第二处汇编调用的路径。它收窄FR67-F01场景，不证明真实任务已经安全终止或全仓无其他重复路径；FR67-F03的首帧NULL与KT68-F02的业务NULL分开。

## 4. R68-S01：终止后缀不等于整条退出链的正确性证明

do_exit声明了group_dead：
[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	int group_dead;
```

所读完整定义中，其赋值语句处于注释内，后来仍作为实参使用：
[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	exit_notify(tsk, group_dead);
```

这足以要求把数据定义性和前置调用结果列为未闭合条件；即使所读exit_notify内暂未消费该形参，也不能把原do_exit整个C执行过程签成安全。此处只补充源级消费限制，不推演真实故障，不分配一个未经历史去重的“新已证bug”。资源释放、子任务移交、调度/锁及终止可达性保持opaque。

do_task_dead的确含终止后缀：
[VERIFIED mykernel/sched/scheduler/scheduler_core.c::do_task_dead]
```c
	/* Avoid "noreturn function does return" - but don't continue if BUG() is a NOP: */
	for (;;)
		cpu_relax();
}
```

因此有效读法是“若前置表达式/对象有效且沿所读正常控制推进到该后缀，它不提供正常返回边”，不是“调用kthread_exit已经证明整条C路径定义良好、资源清理成功、内核线程必然从CPU消失”。原KT68-U05等仍开放。作者16个模型中完成/业务/退出可达布尔值也均隐含所有先行调用正常推进；它们不是调度活性或C语义全覆盖证明。

## 5. R68-S02：图与模型

主线将完整DOT文本复制到会话工作区并计算Git blob身份，与连接器返回对象f75c80b18d85相符；4659字节、101行、42个节点、54条边、零未知端点。计数排除了graph属性语句，不把它误算为第43个节点。未运行Graphviz，不认证完整CFG。
图中sleep是“状态设置＋调度接口”的聚合节点；真实顺序应读为设置TASK_UNINTERRUPTIBLE、create.result、preempt_disable、complete(done)、schedule_preempt_disabled、preempt_enable，再判STOP。图的CALL_THEN_SCHEDULE不代表schedule函数内部调用complete。
retfork→second还须ret_from_fork整体及用户返回工作正常返回、活跃RBX非空且保留原fn；同参数需要R12保留。包装器→xchg节点只当源级表达式关系，其CALL标签不构成底层确为普通函数调用的认证；本轮不展开xchg宏实现。dequeue→create_kthread、callback→exitwrap等按调用者内的局部控制与后续顺序消费，不能解释为队列库或业务函数直接调用这些后继。
自写Python独立重算16个简化条件案例并核DOT结构/对象身份；未重跑作者JS。原facts/deferred全文JSON机械解析、原六件全量自动前缀/字节扫描、仓库代码/脚本/夹具、预处理/编译/链接、QEMU/硬件均NOT_RUN。

## 6. R68-S03：失败前提与首次exec边界

KT68-F01的第一处任务指针有即时解引用，第二处只是存入kthreadd_task供后继使用，影响时点不同。不能把通用kernel_clone的PIDFD参数冲突分支直接当作这两个具名启动调用的实际失败证据；它们仍需实际copy_process失败及查找行为等前提。KT68-F02仅为未闭合的回调非空契约，未检查不等于契约允许NULL，更不是实测漏洞。两项needs_evidence、8个缺口及旧ID状态均保持。

首次exec桥在调用前写mm，并没有在本直接体中消费返回值：
[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	curr->mm = curr->active_mm;
	kernel_execve(initd_name, argv, envp);
```

C17只显示改RSP/跳转语句的前缀；完整定义中输入操作数是curr_ptregs，hwtss_cpu0赋的是指针变量，而不是写TSS.sp0。不得把“改指针”“发出跳转”当作exec成功或硬件栈绑定成功。本轮为69划界另读固定exec.c中的kernel_execve、bprm_execve、search_binary_handler，以及start_thread_common/start_thread窗口；未审完整ELF加载或认定实际SYSRET成功。

上述补充直接由本主线回执规定消费方式，无新增D00返工。下一包只补首次exec桥、状态返回与寄存器帧交接的有界证据，仍由C05b/08执行、C05独立验收；第69批与本回执同次准备发布，是否已固定入库以回读及正式评论为准；发布不等于执行已启动。
