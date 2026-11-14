---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-SCHED-INTEGRATION-03-REVIEW-001
record_type: integration_review_deferred_repairs_and_spec_errata
conversation_display_name: "MYOS2-A-C02 内核分析主线"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-10-02
base_snapshot: "kernel=time；workspace=master；execution=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002（分支名）"
read_channel: connector
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-SCHED-INTEGRATION-03
reviewed_pr: 17
reviewed_branch: claude/dazzling-cori-q0dnyt
reviewed_commit_short12: ee9e6a738224
results_frozen_short12: b7fa83583e35
kernel_short12: a039d9803ade
status: PARTIAL_ACCEPTANCE_SPEC_ERRATA_REQUIRED
acceptance_verdict: RETURN
acceptance_scope: "退回范围仅限候选/未来回归规格的两处一致性问题；已读静态事实和历史见证继续按限界使用。"
accepted_partial_scope: "D01定点索引的可追溯形态、D02/D03局部生命周期与计费分析、已有W/V引用及开放缺口；不是151条语义逐项认证。"
returned_items: [IR-01, IR-02]
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_acceptance_verdict: NOT_ISSUED
reviewer_execution: false
kernel_change_authorized: false
candidate_A_adopted: false
candidate_B_adopted: false
OD_1_status: DEFERRED_NOT_ADOPTED
OD_2_status: DEFERRED_NOT_ADOPTED
branch_release: false
phase3_entered: false
next_followup: CORE-MM-BASELINE-04
next_taskbook: agent-workspace/lead/MYOS2-LEAD-002/17-memory-baseline-contract.md
next_does_not_require_kernel_repair: true
supersedes: null
continues_from: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-SCHED-ORDER-02-review.md
evidence_class: "连接器可读审查、回源核对与手工状态推导；没有主线命令或测试执行。"
inputs_read:
  - "ee9e6a738224:scheduler-integration-03/MANIFEST.md全文；integration.md分段至末尾；evidence.md开头、检索表、引文统计、并行审查与缺口区段，长响应有截断，不申报全文。"
  - "同包scripts/scan_index.py、verify_anchors.py、check_scope.py全文。"
  - "同包facts-and-regressions.yaml的580-780、1850-2050、2270-2410、2600-2888定点区段；长响应截断处以实际显示为限，不申报2888行全文。anchors_check.json开头统计和部分ID。"
  - "time:options_flags.cmake、myos_rt.c、semaphore.h、mm/mm_api.h全文；timer.c的620-850，timer_macro.h开头至末尾。"
  - "本会话既有16号任务全文与ORDER-02审查；本轮再次读取core/MANIFEST.md的头部和V00-V14表，撤回事项仍以之后的主线回执为准。"
  - "PR16/17元数据；f36b89b8a53a到ee9e6a738224及b7fa83583e35到ee9e6a738224远端差异；time的mm目录直接子项。"
self_check:
  scope: this_file_only
  verified_claims: 6
  quotes_reconfirmed: 6
  downgraded_to_inferred: 0
  method: "逐条对照本轮显示的源码并手工计数；未运行机械检查。"
local_validation: "待执行面；没有运行扫描、解析器、哈希、C/ASM、QEMU或内核。"
open_questions:
  - "IR-01/02由下一任务M00追加处置，不改原包、不单独再开一轮调度执行。"
  - "DV-11实际机器读取、DV-13/DV-23运行可达性、DV-19完整时序，以及ELF/IRQ/SMP/上下文切换仍未验证。"
---

# 现在可以不修内核，继续事实盘点；不能把未修机制当作可靠运行前提

**A/B与键下限都不需要现在选择，内核修改仍未授权。** 调度器存在的缺陷不阻碍读取其他子系统、整理实现程度和依赖关系；它们会限制后续动态实验能依赖的调度、阻塞、超时与并发能力。这两种影响不能混成“所有分析都必须先修调度”。

本批有真实的静态正文、命中索引、源码引文和候选规格，可以保留。审查另发现两处未来规格自身的矛盾，局部退回IR-01/02；不是判定全部研究错误，更不是要求Owner现在修内核。下一次云任务先用少量文字和状态表解决它们，再连续盘点四个内存子系统，不为这点勘误单独消耗一次往返。

## 1. 这批材料能支持什么

[VRF] f36到ee9e的远端比较只有14件新增，全部位于scheduler-integration-03；b7fa到ee9e只追加6件清单/证据/回读文件，结果批的integration.md和facts-and-regressions.yaml没有改变。旧实验、旧验证器和内核没有在此净差异中被改写。

D01交付了44个定点查询的索引；495是按查询累计的命中行数，不是495个不同问题或独立调用者。398行分类、97行通用list使用者只计数的区别已写明。151/151引文、10/10标签与7/7旧G锚点是执行方机械核对的申报，主线读了脚本与记录样本，不冒称再次跑过或全面语义认证。

D02/D03把初始化、首次入队、睡眠/唤醒、回插、选取以及计费责任分开。已确认的W01/W02/W07见证继续有效；本次没有新增动态证据。DV-11是有价值的新增源码推导：**原函数在摘下唯一的idle节点后，也可能对空环anchor换算任务并读取，不只是未来修游标后才会出现。** 本轮再次读原函数支持这个条件路径；尚不能声称具体编译产物已产生该内存访问或真实内核已因此崩溃。

D04提供两个可比较方向，不是已授权的实现；D05六项仍是NOT_RUN。报告提出的A规则应作为以后准备修改的输入，不是现在已通过测试的修复。

## 2. A、B和三项风险究竟是什么

| 项目 | 人话含义 | 证据等级与不能外推的部分 |
|---|---|---|
| A：回插与计费 | 任务用完CPU放回队列时，用错比较对象，又拿尚未计入本轮时间的旧键决定位置。六条规则是同一局部修复的要求，不是六个独立实测bug。 | W01/W02/W07已有受限宿主原函数记录；修改后的A未实现、未运行。只在回插前队列有序等前提下保持顺序，不能修好所有唤醒头插导致的逆序。 |
| DV-11：anchor | 链表头只是标记，不是任务。摘下最后一个等待节点后，剩下的是标记，代码却先按任务读取，再检查是否到末尾。 | 源码条件推导。应优先纳入未来局部修复；读取地址、优化后的行为、真实触发尚未验证。 |
| B与OD-2 | B决定唤醒任务是立即排到前面，还是与其他等待任务按累计运行时间排队；OD-2另决定新任务/长睡任务是否获得过大的“历史低键优势”。 | 政策选择，不是仅凭头插即可判定代码错误。采用键下限后也会改变低键任务和新任务的键值及后续顺序，不能总括为“只影响高键唤醒”。 |
| DV-13：过早唤醒 | 新任务尚在准备阶段就被普通唤醒加入队列，之后首次入队再加一次，可能重复链接、空转或破坏成员关系。 | 现有状态筛选缺口有旧V05局部证据；特定窗口中的真实唤醒者及整条故障时序未证实。 |
| DV-23：等之前被切走 | 任务先标记“我要等”，本应再检查事情是否已完成；若在这两步之间被当作真正睡眠任务切走，可能错过此前已发生的通知。 | 取决于中断/抢占与in_atomic等条件，仍是条件风险，不是已复现的丢唤醒。 |
| DV-19：有限超时 | 本应睡眠并等待到期，却缺少该分支的显式schedule，还删除等待用的定时器，返回的剩余时间也不减少。 | 关键源码成立；旧V09使用受限夹具。真实下层锁/定时器删除及中断时序未闭合，不能说每次都立即返回，也不能说每次一定卡死。 |

A是优先考虑的局部修复方向；B和键下限可以更晚决定。这里不把“不答时只做A”视为执行权限。没有明确授权时，只保留规格，**A、B、补丁、内核改动全部不启动**。

## 3. 延后处理的实际边界与既有问题归集

以下是工作安排判断，不是新测量：

| 已有问题或风险 | 当前可继续什么 | 哪类后续工作不能把它当成可靠前提 |
|---|---|---|
| atomic加法判负实际做减法（旧V12） | 源码盘点、接口关系与测试规格 | 依赖该操作数值/符号结果的实际代码路径。不能因其他原子操作正常就忽略此项。 |
| trylock报成功但不取得锁（旧V13） | 静态分析；记录调用点 | 需要真实互斥所有权的测试，尤其SMP与并发承诺。当前已有串行异常记录，不应称为纯SMP理论问题。 |
| swait摘链不维护count（旧V02/V03/V10/V11） | 等待机制分析、其他模块盘点 | 依赖该等待队列计数或重复completion通知的动态验证。 |
| 唤醒确有入队，但返回0且状态掩码缺口（旧V04/V05） | 保留已核入队事实、继续调用关系分析 | 依赖返回值/状态筛选语义的功能与新增调用者；不恢复旧“永不入队”的错误说法。 |
| 有限超时与DV-19 | 静态内存/构建/依赖分析 | down_timeout、msleep等超时可靠性及需要这些机制的端到端实验。 |
| W01/W02/DV-11与DV-13/DV-23 | 现状取证、待修登记、其他子系统分析 | 调度正确性、等待唤醒可靠性和真实切换测试的无条件成功承诺。 |
| HPET/jiffies双增、CPU0放置与CPU元数据等旧结果 | 源码与局部模型判断 | 真实时基、实际ELF和SMP结论；旧V14模型不能替代它们。 |

旧记录中的“msleep没有调用者”“swait两个函数在文件外无调用者”不在本表恢复。静态盘点也必须记录这些依赖风险，不能因不运行代码便给内存模块签发正确性或可运行结论。

因此，本轮安排是**带着问题清单继续分析，不是宣布问题无害、永久不修或已经修好**。启动真实内核实验、实施修复或扩展并发前，再按实际路径核对相关前置；不把全部待修项一律设成所有工作的硬门。

## 4. 两处规格勘误：无需运行内核即可处理

### IR-01：NR-6把idle运行时间错误地全部算给普通任务

来源：本包facts-and-regressions.yaml的future_regressions/NR-6。它一方面承认原函数、idle键0变体在P5选中idle，另一方面在expected/all中仍给全部分支同一组N2=60、N1=40、普通任务合计100。

[INFERRED，未运行] 按该条输入及原pick逐步推导：

| 步 | 当前任务与时刻 | 本步计费 | 原函数、idle键0的返回者 |
|---|---|---|---|
| P1 | I，100 | idle不计费 | N2 |
| P2 | N2，130 | N2 +30 | N1 |
| P3 | 非RUNNING的N1，150 | N1 +20，不回插 | N2 |
| P4 | N2，170（160已唤醒N1） | N2 +20 | N1 |
| P5 | N1，190 | N1 +20 | I |
| P6 | I，200 | idle不计费 | N1 |

故该分支是N2=50、N1=40，普通任务90，加idle区间10才覆盖总时间100。真实idle最大键的原函数分支，以及按规格正确实现A/B的这些声明输入，才对应N2=60、N1=40、idle区间0。未实现的A/B仍是预期，不能标实测。

更一般的守恒应按每段实际current区分普通任务与idle，不能把壁钟跨度全部强加给普通任务。这是**测试规格的错误，不是再新增一条用户内核bug**。未来执行前必须修正文义；下一任务M00用值表复核即可，不重跑W或写C。

### IR-02：B不能无差别继承A的全部保持项

来源：candidate_changes/B/must_keep中的“A的全部保持项”，与A/keeps里“wake_up_new_task与set_task_cpu头插不变”以及B-R2“普通唤醒改有序插入”互相冲突。

应逐项列出B真正保持的状态、计费、CPU0、成员安全等约定；明确排除A的普通唤醒头插政策。OD-2还未定，B对键值的变更必须分“设下限/不设下限”说明。它不改变本批源码事实，但原B规格不能被直接交给实现者无条件照抄。

## 5. 其他消费限界（登记，不另开返工轮）

- MANIFEST的“需定位的定义全部找到”不准确；同包列了10个未定位名字。采用实际名单与范围，不将其变成全局不存在证明。12条gap记录包含11条open和1条narrowed_or_closed，不是12个开放风险。
- scan_index是词法检索：cond_resolution检查的是宏名是否落入已知列表，不求条件真值；visible_active_text不等于实际被预处理选中、链接或运行。verify_anchors也接受某些文件作用域命名/注释邻近情形；151/151不是151条完整语义或通用P2证明。不重造验证器，消费时回源看声明、条件与正文。
- integration同时写signal_wake_up_state使用唤醒返回值，又总括“现有调用者不依赖返回值”。应保留这个条件分支事实；kick_process当前作用有限，也不能据此认定任何调用者都不读返回值。
- DV-11指源码表达式的非法对象解释路径，不直接推出真实机器必定发出一次加载。DV-19描述立即返回时，还需下层调用能够返回；删除器可能忙等的条件单列，不能把两个分支写成同一次必然时序。
- 多代理审查数量与统计是执行方过程说明，原始代理报告未交付；主线不以人数替代证据，也不宣称已独立认证这些过程。

## 6. 本轮回源的最小锚点

F01 配置来源已读；未来内存盘点也不能把CONFIG_SLUB当作运行正确性证明。
[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_SLUB \
```

F02 原pick先按节点解释任务并比较，再检查anchor。
[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
				List_s * tmp_list = myos_rq->running_lhdr.anchor.next;
				sched_rt_entity_s *tmp_rt = container_of(tmp_list, sched_rt_entity_s, run_list);
				while ((curr_task->se.vruntime > container_of(tmp_rt, task_s, rt)->se.vruntime) &&
						tmp_list != &myos_rq->running_lhdr.anchor)
```

F03 有限超时的关键顺序。
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
	__mod_timer(&timer.timer, expire, MOD_TIMER_NOTPENDING);
	// schedule();
	del_timer_sync(&timer.timer);
```

F04 删除器名是宏别名，不是“找不到同名函数就不存在”。
[VERIFIED mykernel/time/timer/timer_macro.h::del_timer_sync]
```c
	#define del_timer_sync	timer_delete_sync
```

F05 同步删除只在ret小于0时执行这个忙等分支。
[VERIFIED mykernel/time/timer/timer.c::__timer_delete_sync]
```c
		if (unlikely(ret < 0)) {
			// del_timer_wait_running(timer);
			cpu_relax();
		}
	} while (ret < 0);
```

F06 信号量等待把状态写入与超时调用连起来。
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.h::___down_common]
```c
				__set_current_state(state);
				spin_unlock_irq(&sem->lock);
				timeout = schedule_timeout(timeout);
```

## 7. 下一步与PR

下一任务见17-memory-baseline-contract.md。M00追加IR-01/02处置；随后连续完成页分配、内核堆、虚拟映射、缺页处理四个子系统的静态实现/依赖小基线，直接推进002R/003R当前缺失的可用依据。它不意味着原两项完整课题已经交付，也不是正式阶段3综合。

PR17保持Draft，等待这两处文字规格处置与新材料回收；不要求现在合并PR16或17。保留两条分支；新任务不覆盖旧14件文件，不因修正AI规格而修改内核。主线未启动外部会话；Owner只需转发一个启动块，之后回PR链接。

来源入口均为本仓库：执行ee9e6a738224的scheduler-integration-03四主件与scripts/；旧core和scheduler-order-02在各自冻结路径；本轮源码按time读取。读回、最终差异和PR更新以本轮后续连接器结果为准，不预先宣称已完成。
