---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
subject_task: MYOS2-DR-007R
conversation_display_name: "MYOS2-A-C02 内核分析主线"
record_type: bounded_evidence_followup
evidence_class: "一手教学与工程资料的限定命题核查；学习安排为 INFERRED"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-09-24
base_snapshot: "工作区输入 master；写入 agent/MYOS2-LEAD-002；未读 MyOS2 内核源码。"
read_channel: mixed
inputs_read:
  - agent-workspace/results/MYOS2-DR-007/importance.yaml
  - agent-workspace/lead/MYOS2-LEAD-002/05-t0-evidence-review.md
  - agent-workspace/lead/MYOS2-LEAD-002/05-t0-evidence-map.yaml
  - "下列 N01-N11 的指定正文；PDF 相关页面另作截图核对"
input_scope: "v1 本轮读取指定同步子系统与能力、K12/K14 的节点及 tier 字段；05 两文件沿用本对话已全文读回内容，不申报本轮重读。外部范围见 §3，不申报整站或所有链接全文。"
content_origin: "LEAD-002 本轮新增核查，不是旧深度研究正文恢复。"
status: BOUNDED_FOLLOWUP_DELIVERED
supplements:
  - agent-workspace/lead/MYOS2-LEAD-002/05-t0-evidence-review.md
  - agent-workspace/lead/MYOS2-LEAD-002/05-t0-evidence-map.yaml
supersedes_scope: "仅接续 K12/K14 的直接教学证据缺口；原文件保留为当时记录，不改原 tier。"
manual_coverage:
  revisited_t0_capabilities: 2
  newly_reviewed_t1_subsystems: 6
  newly_reviewed_t1_capabilities: 6
  new_t1_bounded_dual_category: 8
  new_t1_engineering_plus_related_education: 4
  cumulative_distinct_nodes_reviewed: 44
  remaining_nodes_not_reviewed: 59
original_tier_assignments_changed: false
canonical_007r_package_complete: false
acceptance_ceiling: PASS_PENDING_LOCAL
local_validation: "待本地；未运行项目命令、解析器、模拟器、编译或测试。"
open_questions:
  - "lockdep 的运行时图与 softirq 专门实现仍缺直接教学实现材料；相关课程只能支持其问题背景。"
  - "44 个节点被处理不等于44个原 tier 已获证明；当前 MyOS2 完成度、正确性和运行条件仍需独立核查。"
  - "剩余59个节点、旧综述逐断言勘误、完整分轴关联和路线均未由本件完成。"
---

# 补齐两项多核依据，并扩展同步与延后执行核查

**两项多核条目已取得直接教学正文；另外新增六个 T1 子系统及其六个能力节点的限定核查。** 两次报告按节点去重后覆盖44项，仍不是完整重要度报告。本件的 N01-N11 只在这个补充包内使用，不覆盖05文件的 C/E 编号或第一波 Evidence Catalog。

## 1. 两项原 T0 缺口的处置

[EXTERNAL https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched-multi.pdf] OSTEP 第10章 v1.10 的 §10.4/§10.5 直接比较单队列和多队列，讨论亲和性、失衡、迁移和工作窃取；章后练习第8题比较每CPU调度及探测间隔。已读相关正文并查看队列/时间线图和练习页，而非只看章节标题。

[EXTERNAL https://docs.kernel.org/scheduler/sched-domains.html] Linux Scheduler Domains 正文给出CPU分组、平衡范围，以及在运行队列间转移任务的工程实例。本轮使用无版本官方页面，不声称是固定6.18文本。

| 原行与节点 | 本轮状态 | 可采用的结论 | 仍不可推出 |
|---|---|---|---|
| K12 `sched.runqueue.per_cpu_runqueue` | 直接教学与工程内容已对应 | 可把共享队列与每CPU队列作为有出处的方案对照。N01 §10.4/§10.5；N02。 | 必须先于基本调度实现、唯一正确方案、当前 MyOS2 已具备相应条件。 |
| K14 `sched.scheduler.smp_load_balance` | 直接教学与工程内容已对应 | 可讨论迁移与局部性/失衡之间的取舍，并以N01练习第8题作为后续练习出处。N02补工程分层。 | 未测量即可给出最优平衡周期或 MyOS2 性能收益。 |

**[INFERRED] 本轮关闭的是“缺直接教学正文”，不是宣布原分级已完成证明。** 原任务要求不同类别的实际出处，并未要求每个T0都必须最先实现；不能把后一个更强命题额外设为验收门。保留原tier，只增加可消费证据及适用边界。N01的历史Linux调度器概述不用于描述当前内核算法。

## 2. 六组 T1：分别保留子系统与能力身份

下面“模式双类”表示课程有对应同步/执行模式、工程文档有具体机制；不意味着课程实现了Linux同名结构。“工程＋相关教学”表示工程机制有正文，而课程只解释相关问题，缺口继续保留。T1不强加原任务只对T0规定的跨类别门，但不把相关背景冒充直接实现证据。

| 本地行号与节点（原tier均T1） | 限定命题及来源 | 结果与必须保留的边界 |
|---|---|---|
| S15 `lock.semaphore` | 资源计数和阻塞同步；N03 A.3.2、N04 semaphore。 | 模式双类。计数、互斥所有权和事件通知不混为一种契约。 |
| K19 `lock.semaphore.sleeping_lock` | 等待时交出CPU及调用上下文；N03 A.3.2/A.3.3、N04 Sleeping locks。 | 模式双类。不是任何上下文都可等待，也不是所有sleeping lock都有相同所有权规则。 |
| S16 `lock.futex` | 用户态同步与内核等待协作；N05 §28.15、N06 DESCRIPTION。 | 模式双类。教材例子不是当前glibc源码，未把完整Linux ABI指定为MyOS2目标。 |
| K20 `lock.futex.user_kernel_wait_wake` | 核对期望值与入睡之间的原子交接；N05、N06，并以N11解释丢唤醒问题。 | 模式双类。单独读值后再睡不是等价替代；具体失败/唤醒路径未在MyOS2核对。 |
| S17 `lock.lockdep` | 锁使用顺序与IRQ使用状态的运行时检查；N07死锁案例、N08。 | 工程＋相关教学。N07没有实现lockdep；不将其当完整教学检测器证据。 |
| K21 `lock.lockdep.runtime_dependency_graph` | 从锁类使用记录建立依赖并检查环；N08 Lock-class/Multi-lock dependency rules。 | 工程＋相关教学。动态图算法、容量和误报控制的教学实现尚未补齐，不替代011。 |
| S18 `kactive.softirq` | 硬中断与延后执行必须分清执行上下文；N03 A.4.3、N09 WQ_BH。 | 工程＋相关教学。Pintos讲的是转交内核线程，不是Linux softirq教学实现。 |
| K22 `kactive.softirq.deferred_interrupt_work` | 延后执行并不自动获得睡眠资格；N09 WQ_BH，N03给问题背景。 | 工程＋相关教学。BH工作项的规则不能据此外推成所有配置下softirq内部实现的完整说明。 |
| S19 `kactive.workqueue` | 将工作描述与执行资源区分；N03 A.4.3、N09 Introduction/The Design。 | 模式双类。课程只给转交线程的模式，Linux具体池管理来自工程文档。 |
| K23 `kactive.workqueue.async_process_context` | 需要可睡眠执行环境时选择线程型执行；N03、N09。 | 模式双类。限定threaded workqueue；不能把WQ_BH也称为可睡眠进程上下文。 |
| S20 `kactive.completion` | 明确事件完成与等待方之间的通知；N03初值0的信号量例子、N10 Usage。 | 模式双类。Pintos例子不是Linux completion的同一实现；完成通知不代替数据互斥。 |
| K24 `kactive.completion.one_shot_wait` | 先通知或先等待均要成立，并管理对象存活期；N03、N10 Initializing/Waiting。 | 模式双类。timeout只结束本次等待，不自动终止异步生产者或允许释放对象。 |

### 三项会改变后续设计的具体限定

[EXTERNAL https://docs.kernel.org/core-api/workqueue.html] **工作队列要写明线程型还是BH型。** 文档明确分两种执行环境，WQ_BH在softirq上下文执行且不能睡眠。同步取消也有“没有并发重新入队”等条件；不能仅凭函数名保证整个生产者生命周期结束。

[EXTERNAL https://docs.kernel.org/scheduler/completion.html] **完成对象必须活得足够久。** 官方明确允许通知先于等待，并警告超时或被信号打断后，异步通知方仍可能使用对象。等待返回与对象安全释放是两件事。

[INFERRED] **锁依赖检查不是锁原语正确性的替代证明。** N08的模型检查锁类、顺序和上下文；N07还单独展示了非原子获取失败。后续应分别验证“真正互斥”和“顺序不会形成环”，不能以检测器没有报警推定CAS、内存顺序及受保护数据都正确。这是基于两种模型边界的推导，不是本轮发现MyOS2新bug。

## 3. 来源目录：只登记实际读到的范围

以下均于2026-09-24访问。引用短语仅作定位；没有存网页/PDF字节快照。N01/N05为作者教材PDF，已用截图核对相关页；未执行教材命令或模拟器。N04是6.18文档；其他无版本Linux链接及man-pages仅作为访问日的文档，不冒称固定内核版本。

| ID | 具体入口 | 实际读取范围与极短定位词 |
|---|---|---|
| N01 | [OSTEP 多处理器调度](https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched-multi.pdf) | 第10章引言、§10.2-10.5、总结与练习；重点印刷页6-10、13的图与第8题。定位：`Multi-Queue Scheduling`。 |
| N02 | [Linux Scheduler Domains](https://docs.kernel.org/scheduler/sched-domains.html) | 正文domain/span/group和平衡流程；无版本URL。定位：`balance process load among these CPUs`。 |
| N03 | [Pintos Reference Guide](https://web.stanford.edu/class/cs140/projects/pintos/pintos_6.html) | A.3.2信号量、A.3.3锁的相关段和A.4.3外部中断。定位：`must not sleep or yield`。旧教学系统限制不推广为全部内核规则。 |
| N04 | [Linux 6.18 Lock types](https://docs.kernel.org/6.18/locking/locktypes.html) | Sleeping locks、Owner semantics、semaphore及PREEMPT_RT。定位：`no concept of owners`。 |
| N05 | [OSTEP Locks](https://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf) | §28.14末部唤醒竞争、§28.15及图28.10、§28.16；印刷页18-20，18/19另截图。定位：`Linux-based Futex Locks`。不把旧glibc示例当当前实现。 |
| N06 | [futex(2)](https://man7.org/linux/man-pages/man2/futex.2.html) | DESCRIPTION的用户态快路径、期望值与等待原子性。定位：`atomic compare-and-block operation`。未核完整PI/robust协议。 |
| N07 | [MIT 2025 Locking](https://pdos.csail.mit.edu/6.1810/2025/lec/l-lockv2.txt) | Problem: deadlock、非原子获取反例、锁粒度建议。未执行讲义修改。 |
| N08 | [Linux lockdep design](https://docs.kernel.org/locking/lockdep-design.html) | Lock-class、State、Single-lock与Multi-lock dependency rules。定位：`class`。未读源码或实现检测器。 |
| N09 | [Linux Workqueue](https://docs.kernel.org/core-api/workqueue.html) | Introduction、The Design、WQ_BH、cancel_work_sync/cancel_delayed_work条款；没有通读3000余行全部API。定位：`BH work items cannot sleep`。 |
| N10 | [Linux Completions](https://docs.kernel.org/scheduler/completion.html) | Introduction、Usage、Initializing、Waiting及超时条款。定位：`before another thread checks`。 |
| N11 | [MIT 2025 Coordination](https://pdos.csail.mit.edu/6.1810/2025/lec/l-coordination.txt) | 条件等待、错误sleep/wakeup时序及锁交接；未执行QEMU命令。定位：`lost wakeups`。 |

N02固定6.18地址本轮未成功取得，故明确采用无版本官方页面。N01的教学背景与Linux工程实例是不同来源用途，但它们不会给出本项目的实测收益、学习者掌握度或最终排序。

## 4. 后续核查接口：不需要Owner现在答题或运行

下面为[INFERRED]的审查要点，不是已执行测试，也不是跨对话任务发射：

| 拟检查的情形 | 应观察或论证的属性 | 需要的MyOS2输入 |
|---|---|---|
| 先通知再等待、先等待再通知 | 通知不会因时序反转而丢失 | wait/wakeup及完成对象的实际函数体 |
| 等待超时后仍有迟到通知 | 不访问已经释放的对象 | 取消、退出和引用计数契约 |
| 空闲CPU旁仍有可运行任务 | 区分队列组织、放置策略和迁移条件 | runqueue状态与实际调度路径 |
| 线程型回调需要阻塞 | 其执行环境允许，且不存在持有原子锁等额外限制 | 当前工作队列类型、配置与锁状态 |
| AB-BA顺序与错误锁原语 | 依赖环检查和真实互斥分别验证 | 011的检测器设计及锁原语的独立证据 |

下一段更有价值的主线工作是把调度/唤醒等核心概念与实际完成度、依赖和可观测条件对接；不必等剩余59个条目全部齐备。这里没有安排改内核，没有使用旧评审替代源码，也没有发布正式阶段3路线。

## 5. 当前完成范围与使用方法

配套 `06-scheduling-sync-evidence-map.yaml` 只存14行增量：两行覆盖05中K12/K14的证据状态，十二行新增节点。联合使用时按node键覆盖两行，不把14行全算新增。这样T0仍是32个不同节点，另加12个T1，共44；原103节点集合剩59项尚未被这两件主线核查处理。统计为人工对应核数，解析器和完整机械验收仍待本地。

其余05条目的核查范围保持原状。本件不替换 `importance.yaml`，不形成 `importance-v2.yaml` 或完整007R验收，不代答Owner的学习优先级选择，不改协议。04指导记录和05结果保留为历史，当前状态由本轮新检查点接续。

同族局限声明：GPT主线对GPT产出结构补充证据，未取得异族独立复核；本项目代码、运行和原件字节归档均未在本轮验证。
