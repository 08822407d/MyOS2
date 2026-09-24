---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
subject_task: MYOS2-DR-007R
record_type: bounded_t0_learning_evidence_review
evidence_class: "一手课程与工程文档的限定命题核查；学习重要性和练习建议为推断，不是内核现状或最终路线"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-09-24
base_snapshot: "MyOS2 工作区读取 master；写入 agent/MYOS2-LEAD-002；未读取 MyOS2 内核源码。"
inputs_read:
  - agent-workspace/results/MYOS2-DR-007/importance.yaml
  - agent-workspace/tasks/MYOS2-DR-007R-importance-evidence-and-tiers.md
  - agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md
  - agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-09-10-007r-errata-followup.md
  - "本对话已收 007R MANIFEST 与 Owner 转述的部分 errata"
  - "本文件 §4 的 C01-C13、E01-E19 一手页面"
input_scope: "importance 本轮分三段读至最后节点，仅使用 ID、层级、tier 与待核论证结构；任务书和旧检查点沿用本对话已有完整正文。外部页面仅核 §4 所列段落，不申报所有长文全文或链接内附件。"
content_origin: "LEAD-002 本轮新做的证据核查，不是原 007R 完整报告的恢复。"
status: BOUNDED_REVIEW_COMPLETE_WITH_TWO_SPECIFIC_DESIGN_GAPS
canonical_007r_package_complete: false
original_tier_assignments_changed: false
kernel_fact_verification: not_performed
acceptance_ceiling: PASS_PENDING_LOCAL
local_validation: "待本地；未运行命令、YAML 解析器、自动计数、构建或测试。"
manual_coverage:
  subsystem_rows: 14
  capability_rows: 18
  bounded_concept_supported: 30
  specific_design_partial: 2
  source_pages_catalogued: 32
  context_only_pages: 2
open_questions:
  - "每 CPU 运行队列、动态负载均衡的专门教学论证仍不足以证明其必须先于基本调度实现；两个条目不判完整 T0 证据通过。"
  - "MyOS2 的完成度、实际调用依赖和可运行实验前提尚未核查，不能据本报告决定立即改哪些代码。"
  - "其余 71 个非 T0 节点、旧综述逐断言勘误、完整字段映射和 A/B/C 路线仍未由本件完成。"
  - "源材料精确字节归档、机器重计和独立复核待本地。"
---

# T0 学习依据：基础机制可以落实，具体多核方案不能一概前置

**本件把原有 14 个 T0 子系统、18 个 T0 能力节点逐项变成了可追溯的学习概念与证据对照。** 30 项在本文收窄后的概念范围内有课程和工程内容支持；每 CPU 运行队列、动态负载均衡两项保留专门的证据缺口。没有因此把 007R 整项改成完成，也没有改动旧 tier。

最有用的变化是：内核堆分配已有具体教学材料，不再只靠两份工程资料；而“有多个 CPU”“每 CPU 有调度器”“每 CPU 有独立运行队列”被明确分开。研究应帮助 Owner 理解对象、状态和约束，不把完整 Linux 数据结构或某一种算法当作入门门槛。

配套机器记录为 `05-t0-evidence-map.yaml`。本件的 C/E 编号只在本次证据包内有效，**不是旧 Evidence Catalog 的 E01-E22**。

## 1. 如何读这份核查

每一行先限定要支持的命题，再分别指向课程与工程资料。§4 的来源表给出 URL、具体章节、实际读取范围和短引文。`概念支持` 仅表示这些来源能支撑所列概念；不表示来源自行给出了 MyOS2 的 T0，也不表示 Owner 尚未掌握该知识。

所有“建议作为基础、延后某种方案、适合怎样练习”的判断均为 **[INFERRED]**。它们受 Owner 学习目标约束，但不是 Owner 已采纳的路线。`原 tier=T0` 只是 v1 字段摘录；不把统计修正伪装成重新分级。系统节点与能力节点分别计数，不将父节点证据无条件继承给所有子节点。

跨类别核对在这里要求**不同页面、不同用途的实际内容**：课程/教材的具体讲解或练习，加工程接口/机制文档。相同页面即使同时涉及“现代性”，也不重复计作第二类。C01/C02 仅提供课程结构背景，不独自满足某个具体实现的直接教学证据。

## 2. 十四个子系统

下列每项均为原 T0；`S` 编号只是本表索引，不是新子系统 ID。

| 索引与节点 | 已核查的限定学习概念 | 课程内容 + 工程内容 | 核查结果与学习边界 |
|---|---|---|---|
| S01 `arch.x86_64` | 特权边界、异常入口与地址翻译属于理解上层机制的硬件接口。 | [C11] 的 x86 中断练习、[C05] 的页表练习；[E01] 的 x86-64 入口、[E02] 的页表层次。 | 概念支持；CMU 的旧 x86 与 MIT 的 RISC-V 材料不能直接证明 x86-64 的位域和入口指令序列。 |
| S02 `entry` | 用户到内核的转移需要管理保存状态、内核执行环境与恢复边界。 | [C09] 的入口/返回讲解；[E01] 的多种 x86 入口约定。 | 概念支持；统一理解边界，不把所有入口套成同一栈布局。 |
| S03 `cpu` | 区分 CPU 私有状态、共享状态以及处理器进入可服务状态的过程。 | [C13] 的 BSP/AP 教学流程；[E06] 的 per-CPU 访问、[E07] 的上线阶段。 | 概念支持；CPU 状态基础和 SMP 扩展分层，不等于现在就执行多核上线。 |
| S04 `mm.page_alloc` | 管理页的分配、归还、连续性与碎片。 | [C12] A.5.1；[E03] Zones 的 free_area/buddy 说明。 | 概念支持；位图和伙伴分配是可比较的方案，不要求先复制生产内核优化。 |
| S05 `mm.kmalloc` | 在页之上提供内核对象分配，并明确失败、释放和执行上下文。 | [C12] A.5.2 的 kernel-pool block allocator；[E04] 的 allocator 选择与 GFP 约束。 | 概念支持；这是真正的内核堆教学依据，不是把用户态 malloc 当作 kmalloc 证明。 |
| S06 `mm.vm_map` | 地址映射、权限与翻译缓存一致性。 | [C05] 的只读映射与页表观察；[E02]/[E05] 的页表和 TLB 更新接口。 | 概念支持；建立/撤销/保护的契约比照搬某套完整 VM 管理更基础。 |
| S07 `mm.fault` | 缺页既可报告权限错误，也可承载延迟分配和 COW。 | [C04] 的 COW fault；[E02] Page Fault Handling。 | 概念支持；不能把所有 fault 都当同一错误，更不把实验成功等同全部 VM 正确。 |
| S08 `sched.task` | 可恢复的执行状态、任务状态与退出后的生命周期。 | [C06] 的状态/上下文；[E19] 状态接口、[E18] 等待与回收。 | 概念支持；用户接口状态不是一份完整的内核内部状态机枚举。 |
| S09 `sched.forkexec` | 创建、替换程序、结束、等待之间的资源交接。 | [C04] 的 fork/COW、[C07] 的 exit/wait；[E16]/[E17]/[E18] 的接口契约。 | 概念支持；exec 不等于新建进程；不自动要求一次实现 Linux 全部继承例外。 |
| S10 `sched.runqueue` | 就绪集合、状态改变与任务选择必须一致。 | [C06] 的共享 proc[] 扫描；[E09] enqueue/dequeue 的工程示例。 | 概念支持；队列是表示选择，不能把 per-CPU 队列强加给所有基础调度器。 |
| S11 `sched.scheduler` | 时间复用、抢占和上下文交接。 | [C06] timer/yield；[E09] 调度实体接口；[C01] 仅作课程结构背景。 | 概念支持；目标不是首先复刻 CFS/EEVDF。 |
| S12 `lock.atomic` | 不可分割的读改写与访问顺序是两个分别需要说明的问题。 | [C08] 分离检查/写入的失败案例；[E11] RMW 和 ordering 规则。 | 概念支持；不能把 atomic 一词理解为对任意地址都具有全屏障效果。 |
| S13 `lock.spinlock` | 临界区所有权必须与中断、抢占和锁类型的上下文规则配套。 | [C08] 自旋与 push_off 问题；[E10] raw/non-RT/RT 锁语义。 | 概念支持；不套用“任何 spinlock 都关闭中断”的错误统一说法。 |
| S14 `fs.vfs` | 将名字、文件对象与具体文件系统实现区分。 | [C10] 文件层次与引用；[E13] VFS、inode、dentry、file。 | 概念支持；先讲接口/生命周期，不把某个磁盘格式细节替代 VFS 抽象。 |

## 3. 十八个能力节点

| 索引与节点 | 限定命题与实际出处 | 核查结论、不能外推的部分 |
|---|---|---|
| K01 `arch.x86_64.interrupt_exception_entry` | [C11] 要求中断包装保存/恢复状态；[E01] 区分 x86-64 入口种类。 | 概念支持；教学例子的 32 位寄存器、PIC 和栈形态不能移植成 MyOS2 的事实。 |
| K02 `arch.x86_64.page_table_mmu` | [C05] 观察 PTE 和权限；[E02] 映射层次及 MMU/TLB，另用 [E05] 约束更新。 | 概念支持；RISC-V 具体 PTE 位不得当作 x86 位表。 |
| K03 `entry.syscall_entry_exit` | [C09] 用户/内核状态切换；[E01] syscall 与其他入口有不同调用约定。 | 概念支持；这里只证明应理解其边界，不核 MyOS2 的参数传递或返回路径。 |
| K04 `cpu.smp_bringup` | [C13] AP bootstrap 与就绪握手；[E07] 资源准备、目标 CPU 启动、上线服务分阶段。 | 概念支持，**SMP 阶段条件项**；热插拔不是启动全过程，JOS 的 BIOS/32 位流程也不是 UEFI/x86-64 实施规范。 |
| K05 `cpu.per_cpu_state` | [C03] 分配器按 CPU 分散并在本地空闲链表耗尽时借用其他 CPU 的空闲页；[E06] 本地访问和远端写入限制。 | 概念支持；必须讲迁移和共享例外，不能说“per-CPU 数据永远不需要同步”。 |
| K06 `mm.page_alloc.physical_page_allocator` | [C12] 页池、位图及连续分配碎片；[E03] 按 order 分裂/合并。 | 概念支持；比较所有权、空闲集合和失败条件，不把某一算法视为唯一正确实现。 |
| K07 `mm.kmalloc.kernel_heap_allocator` | [C12] 页上层 block allocator；[E04] 小对象、页分配及可睡眠约束。 | 概念支持；核心是对象生命周期与接口契约，完整 SLUB 性能调优不是本轮结论。 |
| K08 `mm.vm_map.address_space_mapping` | [C05] 建立受权限约束的映射；[E05] 页表改变后的缓存翻译处理。 | 概念支持；map/unmap/protect 是接口操作，不应笼统称为并发意义的“原子操作”。 |
| K09 `mm.fault.demand_paging_cow` | [C04] 延迟复制、写 fault 和最后引用释放；[E02] fault 用途、[E16] Linux COW 实现说明。 | 概念支持；需要分清合法延迟处理与非法写入，不能据此认定 MyOS2 已支持 COW。 |
| K10 `sched.task.task_lifecycle` | [C07] exit/wait 的资源分工；[E18] 等待终止状态、[E19] 状态接口。 | 概念支持；重点是“停止执行”和“资源可回收”的区别，不机械照抄状态字母。 |
| K11 `sched.forkexec.fork_exec_wait_exit` | [C04]/[C07] 教学实例；[E16] 继承、[E17] 替换映像、[E18] 回收。 | 概念支持；错误路径、共享描述和回收须分开设计，实际 ABI 范围待 MyOS2 核查。 |
| K12 `sched.runqueue.per_cpu_runqueue` | [E08] 支持 Linux 的分 CPU 运行队列平衡；[C06] 却给出各 CPU 扫描共享 proc[] 的有效对照。 | **具体方案证据部分完成**；课程对照证明有替代方案，不是该实现必修的直接正证。保留原 T0，不宣布新分级，也不要求其成为基本调度的先决实现。 |
| K13 `sched.scheduler.preemptive_scheduling` | [C06] 无主动 syscall 时由 timer 驱动交接；[E09] runnable 实体进入/离开及调度接口。 | 概念支持；策略算法与抢占机制分开，不能因存在 tick 就推定完整调度正确。 |
| K14 `sched.scheduler.smp_load_balance` | [E08] topology/span 与队列间任务移动；[C01] Multi-CPU Scheduling、[C02] 高性能调度仅给课程背景。 | **具体方案证据部分完成**；本轮没读这两处所链接的完整多核调度正文，不能据标题闭合动态平衡的专门教学论证。需要基础多核可用与明确负载目标后再定实践优先级。 |
| K15 `lock.atomic.memory_ordering` | [C08] 编译器/CPU 重排问题；[E12] acquire/release 单向约束，另有 [E11] 原子操作顺序分类。 | 概念支持；互斥、顺序、可见性和访问对象分别陈述，不把局部关中断当作跨 CPU 同步证明。 |
| K16 `lock.spinlock.irq_preempt_safe_locking` | [C08] 同 CPU 中断再次需锁的案例；[E10] 锁类别与 PREEMPT_RT 条件。 | 概念支持；先明示上下文/IRQ 规则，再讨论钩子与不变量。不是 MyOS2 锁已正确的结论。 |
| K17 `fs.vfs.file_inode_dentry_model` | [C10] 名字、inode、open 引用的分工；[E13] Linux 三类对象和引用关系。 | 概念支持；课程用于生命周期对照，Linux 特定 dentry 方案不能冒称 xv6 采用。 |
| K18 `fs.vfs.pathname_and_fd` | [C10] fd 在 rename/unlink 后的语义；[E14] 名字解析与并发，另用 [E15] fdtable/file 的引用管理。 | 概念支持；路径查找、打开引用和关闭回收分开，不能以一个文件可读用例替代全部 VFS 语义。 |

### 已形成的三个实际改进

**[INFERRED] 堆分配可以进入基础练习，但不必先复刻 SLUB。** [C12] 的教学内核已经明确把页分配和任意大小块分配分层；[E04] 补充现代工程接口对上下文、失败和释放的要求。新证据补的是教学用途，不是把旧 E06/E08 换个类别名称。

**[INFERRED] 多核相关 T0 应分层理解。** [C13] 支持学习 AP 启动与就绪握手，[E07] 支持上线前资源准备的重要性；但 [C06] 展示了不采用独立 per-CPU runqueue 的调度组织。K12/K14 仍有专门证据缺口，因此本件不将它们强制排到所有实验之前。Owner 是否优先做 SMP 仍不是本报告代答的决定。

**[INFERRED] 可解释的契约比算法品牌更适合当前目标。** [E09] 自己提示 CFS 向 EEVDF 的演进；应先能解释 runnable 集合、保存状态、抢占与唤醒，而不是用“移植了某算法”代替理解。[E10] 的 RT 条件也说明相同 API 名不能脱离配置解释。

## 4. 本次独立来源目录与读取边界

所有页面于 **2026-09-24** 实际打开。C01-C13 为课程/教学来源，E01-E19 为工程文档/接口文档。这里只摘极短定位片段；完整解释以链接正文为准。历史课程按所标年份使用，不当作 2026 新授课事实；Linux `/6.18/` 页面采用其标示版本，未带版本的页面仅称当次读取版本，man-pages 版本不冒称内核 6.18。不同 URL 数量不等于独立研究机构数量。

### 课程与教材

| ID | 来源与实际读取位置 | 定位短引文或标题 | 支持边界 |
|---|---|---|---|
| [C01] | OSTEP 首页介绍、三部分章节目录 | `Multi-CPU Scheduling` | 仅课程结构背景；未打开各章 PDF，不给页数、课时或算法权重。 |
| [C02] | MIT 6.1810 Fall 2025 schedule，locking/threads/fs/高性能调度等行 | `High-performance networking and scheduling` | 仅日程背景；不把讲次标题当专门机制的完整证据。 |
| [C03] | MIT 2025 Lab lock，Memory allocator 的问题与 per-CPU free-list 设计 | `a free list per CPU` | 支持分散共享写入与借用的教学实例；不是 per-CPU runqueue 证明。 |
| [C04] | MIT 2025 Lab cow，Problem/Solution、reference-count 与错误情况 | `defer allocating and copying physical memory pages` | 支持 COW 生命周期；未执行实验或复制其评分结果。 |
| [C05] | MIT 2025 Lab page tables，Inspect、Speed up system calls、Print | `map one read-only page` | 支持映射与权限；具体架构是 RISC-V。 |
| [C06] | MIT 2025 Thread switching，保存状态、scheduler thread、timer/yield | `keeps scanning the proc[] process table` | 支持基本调度及共享集合反例；未运行讲义中的 GDB。 |
| [C07] | MIT 2025 Coordination，lost wakeup、sleep/wakeup、exit/wait | `lost wakeup` | 支持状态/条件与回收问题；不认证本项目调用链。 |
| [C08] | MIT 2025 Locking，原子交换、push_off、重排及锁粒度 | `the compiler AND the CPU re-order memory accesses!` | 支持互斥与顺序；不移植 RISC-V 汇编。 |
| [C09] | MIT 2025 系统调用入口讲义，用户/内核转移和状态恢复 | `system calls` | 支持通用边界；不是 x86 入口细节手册。 |
| [C10] | MIT 2025 File systems，层次、rename/unlink 与引用、路径解析示例 | `inode` | 支持名字/对象/打开引用区分；不声称 xv6 有 Linux dcache。 |
| [C11] | CMU 15-410 Project 1，Interrupts、Traps and Exceptions、Writing an Interrupt Handler | `save all the general purpose registers` | 旧 32 位 x86 教学实例；不沿用 PIC、位宽或描述符字段为现代通用规范。 |
| [C12] | Stanford Pintos Reference Guide A.5.1/A.5.2 | `It is layered on top of the page allocator` | kernel-pool block allocator 是直接教学依据；未读整本或运行 Pintos。 |
| [C13] | MIT 6.828 2018 Lab 4 Part A，Multiprocessor Support、Application Processor Bootstrap | `waits for the AP to signal a CPU_STARTED flag` | 支持 AP 分阶段启动；BIOS/32 位 JOS 不等于 UEFI/x86-64 的可直接使用方案。 |

### 工程接口与机制

| ID | 来源与实际读取位置 | 定位短引文或标题 | 支持边界 |
|---|---|---|---|
| [E01] | Linux x86 Kernel Entries，入口列表与 calling convention/IST 说明 | `different calling conventions` | x86-64 入口差异；未查厂商逐位规范。 |
| [E02] | Linux 6.18 Page Tables，层次、页表条目、MMU/TLB、Page Fault Handling | `Page Fault Handling` | 支持通用 VM 概念；不当作每种 fault 错误路径的完备说明。 |
| [E03] | Linux 6.18 Physical Memory，Zones 的 PCP/free_area/lock 段 | `The buddy allocator uses this structure` | 支持页管理与分裂/合并；文内 Stub 节不是证据。 |
| [E04] | Linux Memory Allocation Guide，GFP、Selecting memory allocator | `the calling context must be allowed to sleep` | 支持接口选择和失败处理；动态页面未固定到 6.18。 |
| [E05] | Linux Cache and TLB Flushing，TLB 接口说明 | `stale translations` | 页表改变后需要考虑缓存翻译；不是所有架构都照抄同一 flush 实现。 |
| [E06] | Linux 6.18 this_cpu operations，访问、变体、Remote access | `Remote write accesses to percpu data structures` | 处理本地/远端例外，不把局部性质推广为无条件无锁。 |
| [E07] | Linux CPU hotplug，CPU maps、coordination、state machine 三段 | `CPU hotplug state machine` | 支持上线服务的分阶段组织；不覆盖 AP 启动的全部硬件序列。 |
| [E08] | Linux Scheduler Domains，domain/span、group、balance 说明 | `per-CPU` | 支持 Linux 多核平衡设计；不是所有内核必须采取该组织的证明。 |
| [E09] | Linux 6.18 CFS Scheduler，Overview、scheduling classes 的 runnable 接口 | `CFS is making room for EEVDF` | 一个工程设计实例及其演进提示，不声称唯一当前算法。 |
| [E10] | Linux 6.18 Lock types，raw_spinlock_t/spinlock_t 与 RT 条件 | `The semantics of spinlock_t change` | API 语义受配置影响；不把 raw/RT/non-RT 合并成一句。 |
| [E11] | Linux 6.18 Atomic types，RMW 和 ORDERING | `non-RMW operations are unordered` | 顺序需看操作、成功失败和后缀；不是原子类型自动全屏障。 |
| [E12] | Linux 6.18 Memory barriers，ACQUIRE/RELEASE、锁与关中断约束 | `one-way barriers` | 仅相关段落；未通读长文，不把 acquire/release 等同 full barrier。 |
| [E13] | Linux 6.18 VFS，Introduction、dcache、Inode Object、File Object | `The File Object` | 分层与对象关系；不消费后面的全部旧 API 字段为实施规范。 |
| [E14] | Linux 6.18 Pathname lookup，Introduction、dcache、REF-walk | `Holding a reference on a dentry` | 支持命名、引用与并发控制；没宣称完成整篇 RCU-walk 核对。 |
| [E15] | Linux 6.18 File management，开头 files/file/fdtable 引用说明 | `The file descriptor table contains multiple elements` | 只解释 fdtable 与 file 对象，不声称读完全部无锁算法。 |
| [E16] | Linux man-pages fork(2)，Description、返回值、继承 fd、Notes | `fork() creates a new process` | Linux 用户接口参考，不将其中全部例外作为 MyOS2 需求。 |
| [E17] | Linux man-pages execve(2)，Description、process attributes | `execve() does not return on success` | 程序替换与资源变化，不等于创建另一进程。 |
| [E18] | Linux man-pages wait(2)，Description 和 zombie Notes | `release the resources associated with the child` | 等待/回收接口；本件不枚举全部信号及 waitid 选项。 |
| [E19] | Linux man-pages proc_pid_stat(5)，state 字段 | `Sleeping in an interruptible wait` | 用户可见状态例子，不是一份完整内部状态机规范。 |

[C01]: https://pages.cs.wisc.edu/~remzi/OSTEP/
[C02]: https://pdos.csail.mit.edu/6.1810/2025/schedule.html
[C03]: https://pdos.csail.mit.edu/6.1810/2025/labs/lock.html
[C04]: https://pdos.csail.mit.edu/6.1810/2025/labs/cow.html
[C05]: https://pdos.csail.mit.edu/6.1810/2025/labs/pgtbl.html
[C06]: https://pdos.csail.mit.edu/6.1810/2025/lec/l-threads.txt
[C07]: https://pdos.csail.mit.edu/6.1810/2025/lec/l-coordination.txt
[C08]: https://pdos.csail.mit.edu/6.1810/2025/lec/l-lockv2.txt
[C09]: https://pdos.csail.mit.edu/6.1810/2025/lec/l-internal.txt
[C10]: https://pdos.csail.mit.edu/6.1810/2025/lec/l-fs.txt
[C11]: https://www.cs.cmu.edu/~410/p1/proj1.html
[C12]: https://web.stanford.edu/class/cs140/projects/pintos/pintos_6.html
[C13]: https://pdos.csail.mit.edu/6.828/2018/labs/lab4/
[E01]: https://docs.kernel.org/arch/x86/entry_64.html
[E02]: https://docs.kernel.org/6.18/mm/page_tables.html
[E03]: https://docs.kernel.org/6.18/mm/physical_memory.html
[E04]: https://docs.kernel.org/core-api/memory-allocation.html
[E05]: https://docs.kernel.org/core-api/cachetlb.html
[E06]: https://docs.kernel.org/6.18/core-api/this_cpu_ops.html
[E07]: https://docs.kernel.org/core-api/cpu_hotplug.html
[E08]: https://docs.kernel.org/scheduler/sched-domains.html
[E09]: https://docs.kernel.org/6.18/scheduler/sched-design-CFS.html
[E10]: https://docs.kernel.org/6.18/locking/locktypes.html
[E11]: https://docs.kernel.org/6.18/core-api/wrappers/atomic_t.html
[E12]: https://docs.kernel.org/6.18/core-api/wrappers/memory-barriers.html
[E13]: https://docs.kernel.org/6.18/filesystems/vfs.html
[E14]: https://docs.kernel.org/6.18/filesystems/path-lookup.html
[E15]: https://docs.kernel.org/6.18/filesystems/files.html
[E16]: https://man7.org/linux/man-pages/man2/fork.2.html
[E17]: https://man7.org/linux/man-pages/man2/execve.2.html
[E18]: https://man7.org/linux/man-pages/man2/wait.2.html
[E19]: https://man7.org/linux/man-pages/man5/proc_pid_stat.5.html

**读取失败也保留边界**：若干 `/6.18/` 的 CPU hotplug、scheduler domains、cache/TLB 地址本轮未成功取得，已使用上表明确的无版本官方页面，不能冒称固定版读取成功。`/6.18/mm/page_allocation.html` 本轮只取得标题/页脚，不用作页分配证据；E03 是实际取得的替代页。搜索结果中的 PDF、二手博客和课程作业答案未采用。

## 5. 用于后续学习设计的检查问题，不是已执行测试

以下 **[INFERRED]** 问题帮助判断 Owner 是否真正理解机制，也为以后获准的实验设计提供起点。没有运行命令、生成内核补丁或要求 Owner 现在完成作业。

| 主题 | 能说明理解的回答/观察 | 还需什么实际输入 |
|---|---|---|
| 入口与上下文 | 能区分用户 trapframe 与调度 context，并说明缺一类保存会破坏什么。 | MyOS2 入口与上下文代码的实际锚点。 |
| 内存生命周期 | 能解释一次分配失败、一次最后引用释放、一次映射撤销各由谁负责。 | 002R 的页/堆/VM 完成度与现有接口。 |
| COW 与 fault | 能分清原本只读页和可写后共享页，指出错误写入与合法复制的区别。 | 实际 fault 路径、页引用和用户 ABI。 |
| 调度与唤醒 | 能画出条件改变、任务变就绪、离开旧栈的顺序，给出一个 lost-wakeup 反例。 | 003R/002R 的真实状态与调用链，不引用旧评审替代源码。 |
| 锁与顺序 | 能分别说明互斥保证、IRQ 重入风险与 acquire/release 的限制。 | 当前配置、锁原语和可观察手段；011 不替锁实现本身证明互斥。 |
| 文件生命周期 | 能说明名字被删除后为什么打开的对象仍可能存活，何时才可释放。 | VFS/FD/引用计数的当前实现及最小用户程序。 |

## 6. 本次交付完成了什么，仍缺什么

这是一份**完整交出的、范围限定的 T0 核查件**：32 行不留目录占位，每行有具体命题、来源和边界；配套 YAML 保留原节点身份和判定，不装成原任务的 `importance-v2.yaml`。K12/K14 的部分完成是核查结果，不通过凑来源数量消除。其余节点也只支持本文范围，不代表全量原 rationale 无需修改。

没有完成：其余 39 个子系统和 32 个能力节点的证据核查；旧证据综述每句勘误；三套完整值域映射；A/B/C 里程碑；002R/003R 的真实现状核验；原件字节归档及本地机械闸门。因此**不得把本件改称 007R 全包通过或正式学习路线**。历史 54 出处声明与当前 32 页来源包身份不同，不能用新统计倒填旧清单。

下一项可独立推进的工作是延续其余节点的证据核查，并把已有清晰概念与后续实际完成度/依赖材料匹配。进入正式综合路线仍须具备可消费输入并遵守工作令的阶段门；不会为凑齐九项而重跑全批，也不再要求 Owner 催取同一份无法提供的旧 errata。

同族局限声明：本件由 GPT 主线复核 GPT 研究结构并形成新证据，不构成异族独立复核。只做可读检查，最终机械与运行事项继续待本地。
