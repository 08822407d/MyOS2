# MYOS2-DR-007 · 内核能力重要度分级与学习路线图

## 执行状态与快照

本次研究严格按 `MYOS2-DR-007` 任务书执行：重要度排序服务于“**Owner 借自制内核深学现代内核设计**”这一目标，而不是试图给所有生产内核开发工作做通用优先级；交付物必须是 `MANIFEST.md`、`01-evidence-survey.md`、`importance.yaml`、`roadmap.md`，且不能读取并行任务 002/003 的产出。fileciteturn4file0L2-L2

实际分析的内核源码已成功锁定到 `time @ a039d9803ade2a1613d620bda375e028530d5242`。fileciteturn5file0L2-L2 工作区文件读取自当前 `master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719`；这一当前 `master` HEAD 已包含 2026-08-31 的 agent-workspace 更新，因此比 `repo-map.md` 制作时记录的 master 指针更新。fileciteturn18file0L2-L2

仓库侧抽样确认了几个对路线选择很关键的信号：`sched/scheduler` 已有接近 49 KB 的 `scheduler_core.c`，但其中 enqueue/dequeue、迁移、唤醒等 Linux-derived 并发逻辑存在大量注释停用或简化；`lockdep.c` 当前实际上只有宏定义与头文件包含；`device/` 当前只有空的 `CMakeLists.txt`；`fs/` 当前明确只有 FAT、VFS 与 syscall 层。fileciteturn8file0L2-L2 fileciteturn9file0L2-L2 fileciteturn12file0L2-L5 fileciteturn15file0L2-L2 fileciteturn13file0L2-L2 相比之下，用户态已有 `myinit`、`mylib`、`myshell` 三块骨架，namespace 也已有 `nsproxy.c`，所以“用户态生态路线”并非从零开始。fileciteturn17file0L2-L2 fileciteturn16file0L2-L2

GitHub 连接器在本回合暴露的是读取、搜索、比较、读取 PR/commit 等只读能力，没有创建 branch、写文件、commit 或创建 PR 的 mutation action。因此按任务书规定走**降级路径**：没有伪造 `agent/MYOS2-DR-007` 分支、commit 或 PR，下面逐文件完整给出应写入 `agent-workspace/results/MYOS2-DR-007/` 的内容。

## `agent-workspace/results/MYOS2-DR-007/MANIFEST.md`

```markdown
---
task_id: MYOS2-DR-007
produced_by: "GPT-5.5 Thinking / Deep Research"
date: 2026-08-31
base_snapshot: "kernel=time @ a039d9803ade2a1613d620bda375e028530d5242; workspace=master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719"
inputs_read:
  - agent-workspace/conventions.md
  - agent-workspace/repo-map.md
  - agent-workspace/tasks/MYOS2-DR-007-importance-and-roadmap.md
  - mykernel/
  - mykernel/sched/
  - mykernel/sched/scheduler/
  - mykernel/sched/scheduler/scheduler_core.c
  - mykernel/lock_IPC/
  - mykernel/lock_IPC/lockdep/
  - mykernel/lock_IPC/lockdep/lockdep.c
  - mykernel/fs/
  - mykernel/debug/
  - mykernel/device/
  - mykernel/namespace/
  - myinitramfs/
status: final
open_questions:
  - "公约词汇表没有 net/security/extensibility 独立 subsystem ID；为保持节点合法，本任务分别以 device.network_stack、mm.vm_map.kernel_w_x_hardening、debug.bpf_style_programmability 等能力节点承载这些现代内核主题，后续可考虑扩词汇表。"
  - "Linux 各子系统精确的 path-level commit 频率没有在本任务中形成可重复统计数据，因此工程变更频率只作为定性信号，不伪造数字；标记为 not_measurable。"
  - "repo-map.md 记录的是其生成时的 master 基线；2026-08-31 当前 master HEAD 已因 agent-workspace 提交前移。内核源码分析仍严格固定在 time @ a039d9803ade2a1613d620bda375e028530d5242。"
---

# MYOS2-DR-007 Manifest

## 任务范围

本任务回答两个问题：

1. 对 Owner 通过 MyOS2 深学现代内核设计而言，哪些能力是骨干必修（T0）、强烈建议（T1）、进阶选修（T2）或特定场景（T3）。
2. 在不依赖 MYOS2-DR-002/003 并行产出的前提下，依据 repo-map 的规模信号和本次源码抽样，给出三条真正不同的学习路线并推荐其一。

本任务没有读取 MYOS2-DR-002 或 MYOS2-DR-003 的任何产出。

## 快照纪律

- 工作区规则、地图、任务书：
  `master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719`
- 被分析内核源码：
  `time @ a039d9803ade2a1613d620bda375e028530d5242`
- 所有源码层结论均限定于上述 `time` 快照。
- 不把“该快照未见某能力”写成“项目永远不存在某能力”。

## 交付物

| 文件 | 作用 | 主要验收点 |
|---|---|---|
| `MANIFEST.md` | 快照、范围、文件索引与限制 | 本文件 |
| `01-evidence-survey.md` | 教学体系、Linux 工程现实、现代性三轴证据综述 | 所有外部结论有来源；不虚构 path-level 变更频率 |
| `importance.yaml` | 词汇表全部 subsystem 的 T0-T3 重要度 + T0/T1 能力节点 | 53/53 subsystem 全覆盖；所有 T0 至少两类独立证据 |
| `roadmap.md` | 三条候选学习路线与推荐 | 并发正确性、存储栈、用户态生态三条路线目标和里程碑真实不同 |

## 机器可读结果摘要

`importance.yaml`：

- subsystem 节点：53
- 词汇表覆盖率：53 / 53
- T0 subsystem：14
- T1 subsystem：25
- T2 subsystem：9
- T3 subsystem：5
- 额外能力节点：覆盖本任务要求的 T0/T1 现代能力，包括：
  - SMP / per-CPU / load balancing
  - memory ordering / RCU-lite
  - lockdep-lite
  - W^X kernel-memory hardening
  - crash consistency / page cache / async I/O
  - network-stack gap
  - namespace / resource-control concepts
  - BPF-style verified programmability
  - safe driver interface concepts

## 分级语义

- `T0`：骨干必修。缺少这些概念，会使对进程、地址空间、并发、调度或文件抽象的理解出现结构性空洞。
- `T1`：强烈建议。不是最小 OS 的必要条件，但对理解真实现代内核、跨子系统交互或可靠性非常高杠杆。
- `T2`：进阶选修。能明显拓宽内核工程视野，但可在骨干稳定后再深入。
- `T3`：特定场景。主要由具体硬件、兼容性、历史架构或辅助工具需求驱动。

分级不是“代码完成度”。一个当前完全缺失的能力仍可能是 T1；一个已经有很多代码的能力也可能仅是 T2。

## 源码映射摘要

[VERIFIED mykernel/sched/scheduler/scheduler_core.c]
调度核心保存了大量 Linux scheduler 的并发、迁移、wake-up 和 memory-ordering 注释/结构，但相当一部分对应实现被注释掉或由更窄的 MyOS 路径替代。

[VERIFIED mykernel/lock_IPC/lockdep/lockdep.c]
当前文件只有 `LOCKDEP_DEFINATION` 与 `#include "lockdep.h"`，因此 lockdep 是非常明显的教学扩展位。

[VERIFIED mykernel/fs/]
当前顶层是 `vfs/`、`fat/` 与 `syscall.c`。

[VERIFIED mykernel/device/]
当前只有空 `CMakeLists.txt`；这不等价于整个仓库没有驱动，而是说明通用 `device` 层本身仍非常薄。

[VERIFIED mykernel/debug/]
当前目录主体是 `panic.c`；与现代 Linux 的 tracing、fault injection、locking diagnostics 相比，有明显可教学扩展空间。

[VERIFIED mykernel/namespace/]
存在 `nsproxy.c`，说明 namespace 方向已经有可继续演化的落点。

[VERIFIED myinitramfs/]
已有 `myinit/`、`mylib/`、`myshell/`，适合作为 syscall ABI 和端到端行为验证层。

## 写库状态

本次会话中的 GitHub connector 为只读能力集，未暴露 branch/file/commit/PR mutation action。

因此：

- `agent/MYOS2-DR-007`：未创建
- repository files：未写入
- commit：未创建
- PR to `master`：未创建

本对话按任务书规定完整输出四个目标文件，供原样落入：

`agent-workspace/results/MYOS2-DR-007/`
```

该 Manifest 中的源码映射均来自实际 `time` 快照：调度目录及核心文件、lockdep 骨架、FS、device、namespace、initramfs 均已通过连接器逐项核验。fileciteturn7file0L2-L2 fileciteturn9file0L2-L2 fileciteturn12file0L2-L5 fileciteturn13file0L2-L2 fileciteturn15file0L2-L2 fileciteturn16file0L2-L2 fileciteturn17file0L2-L2

## `agent-workspace/results/MYOS2-DR-007/01-evidence-survey.md`

```markdown
---
task_id: MYOS2-DR-007
produced_by: "GPT-5.5 Thinking / Deep Research"
date: 2026-08-31
base_snapshot: "kernel=time @ a039d9803ade2a1613d620bda375e028530d5242; workspace=master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719"
inputs_read:
  - agent-workspace/conventions.md
  - agent-workspace/repo-map.md
  - agent-workspace/tasks/MYOS2-DR-007-importance-and-roadmap.md
  - mykernel/
  - mykernel/sched/
  - mykernel/sched/scheduler/scheduler_core.c
  - mykernel/lock_IPC/
  - mykernel/lock_IPC/lockdep/lockdep.c
  - mykernel/fs/
  - mykernel/debug/
  - mykernel/device/
  - mykernel/namespace/
  - myinitramfs/
status: final
open_questions:
  - "Linux path-level subsystem commit 频率在本任务中 not_measurable；没有把 LWN 的总体开发统计强行转换成 subsystem 精确数字。"
  - "词汇表缺少 net/security/extensibility；importance.yaml 中采用合法的现有前缀进行能力级映射。"
---

# 外部证据综述：什么值得在 MyOS2 上优先学

## 结论先行

对“借自制内核深学现代内核设计”这个目标，最稳定的骨干不是某个特定 Linux 算法，而是五组互相咬合的机制：

1. **特权边界与地址空间**：entry、异常/中断、页表、fault、物理页与内核对象分配。
2. **进程生命周期与调度**：task、fork/exec/exit/wait、runqueue、preemption、SMP placement。
3. **并发正确性**：atomic、memory ordering、spin/sleep locks、wake-up ordering，以及能够检查这些不变量的工具。
4. **持久化抽象**：VFS、路径/FD/inode/dentry、page cache、block I/O、crash consistency。
5. **端到端契约**：syscall ABI、用户态 init/shell、驱动与 I/O。

这五组在 OSTEP、MIT 6.1810、Berkeley CS162、CMU 15-410、清华 uCore/rCore 教学材料以及经典 Linux 内核书中反复出现。[EXTERNAL E01][EXTERNAL E02][EXTERNAL E03][EXTERNAL E04][EXTERNAL E05][EXTERNAL E06][EXTERNAL E07]

在这些骨干之上，近十年已经有一组主题值得视为“现代内核素养”，而不只是生产 Linux 的边角知识：

- 多核 memory ordering、per-CPU 数据与 RCU-style read-mostly synchronization；
- lockdep / tracing / fault-injection 式“把不变量做成可检查对象”；
- namespace + resource control；
- W^X、攻击面压缩和其他 kernel self-protection 思维；
- async I/O 与跨网络/存储栈的少拷贝设计；
- BPF 式“受验证的内核可编程性”；
- 安全接口与 Rust-for-Linux 所代表的“把 safety contract 写进类型/API”的设计方法。

这些并不都应完整复刻 Linux；教学内核的价值在于实现**最小但能暴露核心 trade-off 的版本**。[EXTERNAL E02][EXTERNAL E10][EXTERNAL E11][EXTERNAL E13][EXTERNAL E14][EXTERNAL E15][EXTERNAL E16][EXTERNAL E18]

## 分级方法

本报告采用三轴证据，不用“我觉得重要”作为依据。

### 教学权重

考察：

- 是否在多个 OS 课程中反复出现；
- 出现顺序是否靠前；
- 是否占据多次 lecture / lab / project；
- 是否属于从零构建 OS 时必须跨过的机制。

### 工程中心度

考察：

- Linux 是否把它组织成 core API / locking / scheduler / MM / FS / driver 等核心开发接口；
- 是否被多个其他 subsystem 依赖；
- 是否直接控制 correctness、resource lifetime、blocking/wakeup 或 user/kernel boundary；
- 是否形成独立而活跃的工程文档体系。

Linux 当前开发流程在 merge window 期间可达到接近每天 1000 个 changeset 的集成速率，随后通常经过约 6–10 周稳定期；这说明“真实内核一直在快速变化”，但不能直接推出某个 MyOS2 节点的精确热度。[EXTERNAL E19]

Linux kernel developer Jakub Kicinski 也明确把 LWN 每个 release cycle 的 development statistics 当作长期存在的社区观察工具。[EXTERNAL E20] 但本任务没有获得一个可重复、按本任务 53 个词汇节点归一化的 path-level 数据集，所以：

`per_subsystem_change_frequency = not_measurable`

这里不伪造“scheduler 每月 N 个 commit”之类数字。

### 现代性

考察：

- 2015–2026 的 Linux/课程资料是否把它作为独立主题；
- 它是否应对现代硬件或威胁模型，例如大规模 SMP、高速 NVMe/NIC、speculation/security、可编程内核；
- 是否适合做成教学内核的缩小版，而不是要求复制 Linux 全量复杂度。

### Tier 规则

`T0`：
至少有两类独立证据，并且是理解其他大块机制的前提。

`T1`：
至少有强教学/工程/现代性证据之一，并且对“从能跑到真正理解现代内核”具有高杠杆。

`T2`：
深入后很有价值，但不会阻塞骨干知识图。

`T3`：
硬件特化、历史兼容、格式特化或辅助性强，适合需求驱动学习。

## 教学体系怎么排

### OSTEP

OSTEP 的整体骨架本身就是一个非常强的排序信号：它把 OS 分成 virtualization、concurrency、persistence 三大主轴。

虚拟化部分覆盖：

- processes；
- scheduling；
- multi-level feedback；
- multi-CPU scheduling；
- address spaces；
- paging；
- free-space management。

并发部分覆盖：

- threads；
- locks；
- condition variables；
- semaphores；
- concurrency bugs。

持久化部分覆盖：

- I/O devices；
- files/directories；
- filesystem implementation；
- RAID；
- journaling / crash consistency。

[EXTERNAL E01]

这说明对于教学内核，调度、地址空间、锁和文件系统不是彼此独立的“功能清单”，而是 OS 知识骨架的四根主梁。

### MIT 6.1810 / xv6

MIT 6.1810 Fall 2025 的讲次排序尤其有价值，因为它同时展示了“经典骨干”和“现代扩展”的分界。

较早到中段的核心内容包括：

- OS organization；
- page tables；
- syscall entry/exit；
- page faults；
- virtual-memory applications；
- device drivers；
- locking；
- thread switching；
- coordination。

随后是：

- networking；
- high-performance networking and scheduling；
- filesystems；
- crash recovery；
- filesystem performance；
- multi-core scalability and RCU；
- containers and virtual machines；
- kernel extensibility / BPF；
- Meltdown。

课程实验同时覆盖 page tables、traps、COW、network driver、parallelism/locking、filesystem 和 mmap。[EXTERNAL E02]

这是本报告把下列概念提高到 T1 的核心原因：

- network-stack literacy；
- RCU；
- namespace/container isolation；
- BPF-style programmability；
- speculation/security thinking。

它们已经不只是研究论文里的附加题，而是出现在现代顶级本科/研究生 OS 教学的正式后半程。

### Berkeley CS162

Berkeley CS162 Fall 2026 已发布的课程计划把：

- threads/process management；
- files/I/O/sockets/IPC

放在前段，随后连续安排：

- concurrency and mutual exclusion；
- atomic instructions 与 futex；
- semaphores/monitors/readers-writers；
- scheduling；
- virtual memory/page tables/TLB/demand paging；
- general I/O；
- device drivers/storage；
- filesystem design。

[EXTERNAL E03]

特别值得注意的是，CS162 把 **atomic instructions + futex** 直接作为 synchronization 课程内容，而不是仅讲抽象 mutex。这支持 `lock.atomic` 为 T0、`lock.futex` 为 T1。

### CMU 15-410

CMU 15-410 把自己描述为 Operating System Design and Implementation，并长期将 kernel project 视为课程的核心实践。Project 体系要求学生理解 x86 privilege、interrupt/exception、paging，并真正写内核，而不仅是调用现成 API。[EXTERNAL E04]

它的教学哲学对 MyOS2 特别相关：相比“覆盖更多功能”，一个可调试、结构稳固、能够继续扩展的 kernel base 本身就是学习成果。

### 清华 uCore / rCore

清华 uCore 实验指导明确把自己定位为 THU OS 课程实验，并推荐 rCore 作为更完整的从零 OS 教材。[EXTERNAL E05]

rCore 教学序列逐渐建立：

- bare-metal execution；
- memory management / address space；
- process/thread；
- scheduling；
- filesystem；
- IPC；
- synchronization；
- device I/O。

后段设备章节还专门通过 virtio-blk、virtio-net、virtio-input、virtio-gpu 强调“设备抽象 + 中断/DMA + 跨体系结构设备模型”的教学价值。[EXTERNAL E05]

因此 MyOS2 不应因为当前没有网络目录就把 networking 排成 T3；缺口反而说明它是一个很好的后续 T1 教学节点。

## 经典 Linux 内核书的篇幅信号

### Linux Kernel Development, Third Edition

《Linux Kernel Development》第三版共 20 个主章；从第 3 到第 16 章连续覆盖：

- process management；
- scheduling；
- system calls；
- kernel data structures；
- interrupts；
- bottom halves；
- synchronization；
- synchronization methods；
- timers/time；
- memory management；
- VFS；
- block I/O；
- process address space；
- page cache/writeback。

然后才进入 devices/modules、debugging、portability 和 community。[EXTERNAL E06]

这个分布支持本报告的总体判断：

**process/sched + entry + locking + MM + VFS 是骨架；deferred work、block、device、debugging 是紧邻骨架的 T1 层。**

### Understanding the Linux Kernel, Third Edition

《Understanding the Linux Kernel》第三版是 2005 年、942 页的 Linux 2.6 时代资料，因此不能作为 2026 “现代性”证据，但它对历史工程中心度仍有价值。[EXTERNAL E07]

它从早期章节就把：

- process/kernel model；
- process address space；
- synchronization/critical regions；
- spinlocks/semaphores；
- signals/IPC；
- process management；
- memory management；
- paging/TLB

放在 Linux 内部机制解释的核心位置。

本任务没有找到可靠且方便复现的“每个章节页数”结构化数据，因此：

`per_chapter_page_distribution = not_measurable`

不根据目录页码猜测权重。

## 工程现实怎么排

### 调度不是一个算法，而是一组跨系统契约

当前 Linux scheduler 文档同时覆盖：

- EEVDF；
- scheduler domains；
- capacity-aware scheduling；
- energy-aware scheduling；
- deadline；
- utilization clamping；
- real-time scheduling。

[EXTERNAL E09]

因此对教学内核，T0 不应该是“实现 CFS/EEVDF 本身”，而应是：

- task state machine；
- per-CPU runqueue；
- preemption；
- wakeup；
- migration；
- load balancing；
- priority/inversion concepts。

具体 EEVDF 算法属于后续进阶。

### memory ordering 是 SMP 正确性的语言

Linux memory-barriers 文档与 RCU 文档都表明，多 CPU correctness 不能被“加一把锁”完全概括。

RCU 面向 read-mostly 情况，通过 grace period、更新侧与读侧协议实现可扩展并发；其正确性又直接依赖 CPU/compiler ordering。[EXTERNAL E11]

因此：

- `lock.atomic.memory_ordering` 是 T0；
- RCU 的完整 Linux 实现不是 T0；
- 但实现一个 epoch/RCU-lite 教学版本是 T1。

### lockdep 代表“可验证不变量”

Linux lockdep 是 runtime locking correctness validator；它不是普通调试输出，而是把 lock class、依赖关系和“这里必须持锁”之类约束变成运行时可检查对象。[EXTERNAL E10]

这对 MyOS2 的学习价值很高：手写多核内核时，**能够证明和检测锁序，比再添加一种锁 API 更值得优先投资。**

### workqueue / deferred execution 是跨子系统公共机制

Linux 官方 workqueue 文档把 workqueue 描述为异步 process execution context 的最常用机制之一。[EXTERNAL E12]

因此：

- softirq / deferred interrupt work：T1；
- workqueue：T1；
- completion：T1；
- 更特殊的 swait：T2。

### VFS 是接口中心，具体 FAT 不是

OS 课程普遍把文件/目录、filesystem implementation、crash consistency 放在核心持久化章节；Linux Kernel Development 又给 VFS、block I/O、page cache/writeback 独立章节。[EXTERNAL E01][EXTERNAL E02][EXTERNAL E03][EXTERNAL E06]

因此：

- VFS abstraction：T0；
- page cache / crash consistency：T1；
- block layer：T1；
- FAT 作为特定格式：T2。

### 高速存储让 block queue 的并发性更重要

Linux blk-mq 的官方设计目标是利用现代高速存储设备并行性，同时排队/提交大量 I/O request。[EXTERNAL E21]

所以 MyOS2 若继续深入 block 层，值得学习的是 request lifecycle、queueing、completion、backpressure 和 concurrency，而不只是 ATA 命令细节。

### 网络不是“可有可无的驱动”

Linux 当前 networking 文档本身就是一个巨大的独立工程体系，包含 TCP/IP、NAPI、多队列、namespace、zero-copy io_uring 等主题。[EXTERNAL E17]

MIT 又有 network driver、networking、high-performance networking 三个连续教学落点。[EXTERNAL E02]

所以对本任务目标：

`network stack = T1`

而不是 T3。

## 现代性：哪些概念应该进入教学内核

### 多核扩展性

最低教学集合：

- per-CPU state；
- runqueue partitioning；
- explicit memory ordering；
- remote wakeup/migration；
- load balancing；
- read-mostly synchronization / RCU-lite。

MIT 已把 multi-core scalability and RCU 作为正式 lecture。[EXTERNAL E02]

### 安全加固

Linux Kernel Self-Protection 明确把“防御内核自身漏洞”作为独立设计目标，并强调：

- attack surface reduction；
- strict kernel memory permissions；
- executable memory should not be writable；
- read-only data should not be writable。

[EXTERNAL E13]

教学内核不必实现 Linux 全部 hardening，但至少可以把：

`kernel text RX + rodata R + data NX`

这样的 W^X 权限模型做成可验证 milestone。

### 异步 I/O 与少拷贝

Linux 当前 io_uring zero-copy receive 能让数据直接进入 userspace memory，同时仍由 kernel TCP stack 处理协议头。[EXTERNAL E15]

其教学价值不是复制 io_uring UAPI，而是理解：

- submission/completion separation；
- pinned/shared buffers；
- async lifetime；
- cancellation/error handling；
- zero-copy 与 protection boundary 的 trade-off。

因此 `fs.vfs.async_io_submission` 被列为 T1 概念。

### BPF 式可编程性

Linux BPF 不是“任意把代码塞进 kernel”。

官方设计明确限制 BPF program 只能调用被允许的 helpers/kfuncs，并由 verifier 约束内存访问等行为。[EXTERNAL E14]

这非常适合教学简化：

1. 设计一个极小 bytecode；
2. 只暴露几个 read-only trace hook；
3. loader 做 bounds/control-flow verification；
4. 禁止任意 kernel pointer write；
5. 用它观察 scheduler/VFS。

这样能同时学习：

- extensibility；
- verifier；
- capability exposure；
- ABI stability；
- observability。

### Rust-for-Linux：语言本身不是 T0，Safety Contract 是 T1

当前 Linux 官方文档已经把 Rust 作为内核开发语言体系的一部分，并提供 Quick Start、Coding Guidelines、Arch Support、Testing 等正式文档。[EXTERNAL E16]

更值得 MyOS2 学的是其 API 思维：Linux Rust coding guidelines 要求 unsafe function 明确 Safety preconditions，并要求 unsafe block 前写 `SAFETY` justification。[EXTERNAL E16]

因此本报告没有把“把 MyOS2 全部改写 Rust”列为优先路线，而是把：

`drivers.base.safe_driver_interfaces`

列为 T1。

即使仍然用 C，也可以学习：

- ownership/lifetime 注释；
- typed handles；
- checked state transitions；
- narrow unsafe boundary；
- resource cleanup invariants。

### namespace 与资源控制

MIT 6.1810 已把 containers/VMs 放入现代 OS 后半程。[EXTERNAL E02]

Linux cgroup v2 的核心定义则是：分层组织 process，并沿 hierarchy 可控地分配 system resources。[EXTERNAL E18]

MyOS2 已有 namespace 落点，所以很适合实现：

- pid/mount-style isolation 的简化版；
- CPU/memory quota 的 cgroup-lite 概念；

但无需复制 Linux 的完整 controller 体系。

## MyOS2 映射判断

### 当前最值得利用的“学习张力”

[VERIFIED mykernel/sched/scheduler/scheduler_core.c]
当前 scheduler_core 保留了大量来自 Linux 的 SMP migration、blocking/wakeup、memory-ordering 解释，但多处真正的 lock/queue/class handling 被注释或简化。

[INFERRED]
这形成了一个罕见的高价值教学环境：Owner 已经能直接看到真实 Linux 为什么需要那些 barrier/lock/state invariants，同时又拥有足够小的 MyOS 路径可以自己重新建立它们。

[VERIFIED mykernel/lock_IPC/lockdep/lockdep.c]
lockdep 当前基本是 scaffold。

[INFERRED]
因此“并发正确性优先”能把 scheduler、atomic、spinlock、wake-up、debugging 和 SMP 一次串起来，学习杠杆最高。

### 存储方向的张力

[VERIFIED mykernel/fs/]
已有 VFS + FAT。

[VERIFIED repo-map.md]
fs 与 block 已经有非平凡规模，但相比 Linux 的 page cache/writeback、crash consistency、modern block queue 仍有大量可教学空间。

[INFERRED]
适合从“能读 FAT”升级为“理解一个完整 durability pipeline”。

### 用户态方向的张力

[VERIFIED myinitramfs/]
已有 init/library/shell。

[VERIFIED mykernel/namespace/]
已有 nsproxy 落点。

[VERIFIED mykernel/device/]
通用 device 层仍很薄；词汇表也没有 net 节点。

[INFERRED]
因此可以用用户态 regression tests 推动 syscall/futex/signal/namespace/network 逐层补齐，但这条路线 breadth 最大。

## Evidence Catalog

### E01 — OSTEP

类型：course/book，教学体系

`https://pages.cs.wisc.edu/~remzi/OSTEP/`

用途：
virtualization / concurrency / persistence 总体结构；process scheduling、multi-CPU scheduling、paging、locks、condition variables、filesystem、journaling。

### E02 — MIT 6.1810 Fall 2025 Schedule

类型：course，教学体系 + 现代性

`https://pdos.csail.mit.edu/6.1810/2025/schedule.html`

用途：
page tables、syscall entry/exit、faults、drivers、locking、thread switching、networking、filesystem、crash recovery、RCU、containers、BPF、Meltdown 的实际讲次顺序和 labs。

### E03 — UC Berkeley CS162 Fall 2026

类型：course，教学体系

`https://cs162.org/`

用途：
processes、IPC、atomic/futex/semaphore、scheduling、VM/TLB/demand paging、drivers、storage、filesystem 的课程计划。

注：2026-08-31 时 Fall 2026 学期正在进行中；引用的是已发布 syllabus/schedule，不把未来 lecture 当成已完成教学事件。

### E04 — CMU 15-410 Fall 2026

类型：course/project，教学体系

`https://www.cs.cmu.edu/~410/`

`https://www.cs.cmu.edu/~410/schedule.html`

`https://www.cs.cmu.edu/~410/projects.html`

用途：
从零 kernel project、x86 privilege/paging/interrupts、kernel implementation 的实践权重。

### E05 — Tsinghua uCore / LearningOS / rCore

类型：course/lab，教学体系

`https://learningos.cn/uCore-Tutorial-Guide-2024S/`

`https://github.com/LearningOS/rCore-Tutorial-Code`

`https://rcore-os.cn/rCore-Tutorial-Book-v3/chapter0/0intro.html`

用途：
清华 OS 实验、从 bare metal 到 memory/process/fs/IPC/synchronization/device 的递进，以及 virtio 设备抽象。

### E06 — Linux Kernel Development, Third Edition

类型：book，工程结构

`https://www.informit.com/store/linux-kernel-development-9780672329463`

用途：
process management、scheduler、syscall、interrupt/bottom half、sync、time、MM、VFS、block、address space、page cache、drivers、debugging 的章节分布。

### E07 — Understanding the Linux Kernel, Third Edition

类型：book，历史工程结构

`https://www.oreilly.com/library/view/understanding-the-linux/0596005652/`

用途：
Linux 2.6 时代 process/kernel model、synchronization、address space、paging、signals/IPC 的内部机制。

限制：
2005 年资料，只用于历史中心度，不用于判断 2026 现代性。

### E08 — Linux Kernel Documentation Top Level / Core API

类型：engineering，当前工程中心度

`https://docs.kernel.org/`

`https://docs.kernel.org/core-api/index.html`

用途：
Core API、Driver APIs、Subsystems、Locking、Testing、Tracing、Fault Injection、Rust 等当前官方文档结构。

### E09 — Linux Scheduler Documentation

类型：engineering，当前调度设计

`https://docs.kernel.org/scheduler/index.html`

`https://docs.kernel.org/scheduler/sched-eevdf.html`

用途：
EEVDF、scheduler domains、capacity/energy aware scheduling、RT、uclamp 等。

### E10 — Linux lockdep

类型：engineering，correctness tooling

`https://docs.kernel.org/locking/lockdep-design.html`

用途：
runtime locking correctness validator、lock dependency 与 assertion 概念。

### E11 — Linux Memory Barriers / RCU

类型：engineering + modernity，多核正确性

`https://docs.kernel.org/core-api/wrappers/memory-barriers.html`

`https://docs.kernel.org/RCU/whatisRCU.html`

用途：
CPU/compiler ordering、read-mostly synchronization、grace period。

### E12 — Linux Workqueue

类型：engineering

`https://docs.kernel.org/core-api/workqueue.html`

用途：
异步 process execution context、worker/work item、并发管理。

### E13 — Linux Kernel Self-Protection

类型：engineering + modernity，security

`https://docs.kernel.org/security/self-protection.html`

用途：
attack-surface reduction、strict memory permissions、W^X、rodata。

### E14 — Linux BPF Design Q&A

类型：engineering + modernity

`https://docs.kernel.org/bpf/bpf_design_QA.html`

用途：
program types、helpers/kfuncs、verifier 约束、非稳定 internal ABI。

### E15 — Linux io_uring Zero-Copy Rx

类型：engineering + modernity

`https://docs.kernel.org/networking/iou-zcrx.html`

用途：
io_uring submission/completion、zero-copy、userspace memory 与 kernel TCP integration。

### E16 — Rust in the Linux Kernel

类型：engineering + modernity

`https://docs.kernel.org/rust/index.html`

`https://docs.kernel.org/rust/coding-guidelines.html`

用途：
Rust 正式内核文档体系；Safety preconditions、unsafe-boundary documentation。

### E17 — Linux Networking Documentation

类型：engineering

`https://docs.kernel.org/networking/index.html`

用途：
networking 作为大型独立 subsystem；NAPI、multi-queue、TCP/IP、zero-copy 等。

### E18 — Linux cgroup v2

类型：engineering + modernity

`https://docs.kernel.org/admin-guide/cgroup-v2.html`

用途：
process hierarchy 与 controlled resource distribution。

### E19 — Linux Kernel Development Process

类型：engineering change-rate context

`https://docs.kernel.org/process/2.Process.html`

用途：
merge window、高速 changeset integration、稳定阶段。

### E20 — Jakub Kicinski: More Development Statistics

类型：community engineering signal

`https://people.kernel.org/kuba/more-development-statistics`

用途：
确认 LWN per-release development statistics 是长期社区观察工具。

### E21 — Linux blk-mq

类型：engineering + modern storage

`https://docs.kernel.org/block/blk-mq.html`

用途：
现代高速 block device 的 multi-queue parallel I/O。

### E22 — Linux printk

类型：engineering/debugging

`https://docs.kernel.org/core-api/printk-basics.html`

用途：
printk 作为最基础的 kernel logging/tracing 工具。
```

教学排序的核心证据高度一致：OSTEP 以 virtualization/concurrency/persistence 构成骨架；MIT 6.1810 把 VM、locking、thread switching、FS 放在主体课程中，又把 networking、RCU、containers、BPF、Meltdown 纳入后半段；Berkeley 的已发布 Fall 2026 计划连续用多讲覆盖同步、调度和内存；LKD 的第 3–16 章则连续覆盖 process/scheduler/syscall/interrupt/sync/time/MM/VFS/block/page-cache。citeturn9view0turn9view1turn9view2turn9view3turn9view4

清华 uCore 指导明确是 THU OS 课程实验，而 rCore 体系把进程、地址空间、文件、IPC、同步、设备按增量 OS 演化组织；其设备章节进一步把 virtio 作为学习通用设备抽象的重要载体。citeturn11search2turn11search0turn11search10turn11search16 CMU 当前课程仍把 OS design/implementation 与 kernel projects 放在实践核心。citeturn8view2turn10search5

工程和现代性部分则来自当前 Linux 一手文档：调度文档已包括 EEVDF、scheduler domains、capacity/energy aware scheduling 等；RCU、memory barriers、lockdep 和 workqueue 各自有正式内核设计文档。citeturn8search14turn8search1turn8search9turn8search0turn8search3 当前 Linux 文档还正式覆盖 W^X/self-protection、BPF verifier 约束、io_uring zero-copy、Rust、cgroup v2 与庞大的 networking subsystem。citeturn13view1turn13view2turn13view0turn15search0turn13view5turn13view4

关于“变更频率”，报告有意没有杜撰 subsystem commit 数：Linux 官方开发文档确实描述 merge window 中可接近每天 1000 个 changeset、其后约 6–10 周稳定期；Kicinski 也确认 LWN 的 release-cycle development statistics 长期存在。但没有把这些宏观数据伪装成 53 个 MyOS2 节点的精确热度。citeturn12search2turn12search0

## `agent-workspace/results/MYOS2-DR-007/importance.yaml`

```yaml
task_id: MYOS2-DR-007
produced_by: "GPT-5.5 Thinking / Deep Research"
date: 2026-08-31
base_snapshot: "kernel=time @ a039d9803ade2a1613d620bda375e028530d5242; workspace=master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719"
inputs_read:
  - agent-workspace/conventions.md
  - agent-workspace/repo-map.md
  - agent-workspace/tasks/MYOS2-DR-007-importance-and-roadmap.md
  - mykernel/
  - mykernel/sched/
  - mykernel/sched/scheduler/scheduler_core.c
  - mykernel/lock_IPC/
  - mykernel/lock_IPC/lockdep/lockdep.c
  - mykernel/fs/
  - mykernel/debug/
  - mykernel/device/
  - mykernel/namespace/
  - myinitramfs/
status: final
open_questions:
  - "词汇表无 net/security/extensibility 独立前缀；网络、W^X、BPF 分别临时映射到 device、mm.vm_map、debug 下的合法能力节点。"
  - "Linux path-level subsystem change frequency = not_measurable；不用于制造伪精确 tier。"
---
# Evidence IDs resolve to 01-evidence-survey.md.
# T0 = 骨干必修
# T1 = 强烈建议
# T2 = 进阶选修
# T3 = 特定场景

# ---- subsystem-level coverage: 53/53 ----

- node: arch.x86_64
  tier: T0
  rationale: "课程骨干+工程骨干：特权级、异常/中断、页表与 syscall boundary 是理解 MyOS2 主架构上所有更高层机制的前提。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL book:E06]"

- node: arch.aarch64
  tier: T2
  rationale: "进阶可移植性：第二架构能暴露 x86 隐含假设，但在 Owner 当前 x86_64 主线目标下不应先于 MM/调度/并发。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: boot.uefi
  tier: T1
  rationale: "强烈建议：完整理解 firmware→loader→kernel handoff 能把地址布局、内存地图和早期初始化串起来，但不是调度/MM 本体。"
  evidence:
    - "[EXTERNAL engineering:E08]"
    - "[EXTERNAL course:E05]"

- node: user.initramfs
  tier: T1
  rationale: "强烈建议：用户态 init/shell 是检验 syscall、fork/exec、FD、FS 和错误语义是否真正闭环的最佳端到端层。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL course:E05]"

- node: entry
  tier: T0
  rationale: "课程骨干+工程骨干：syscall、interrupt、exception 的 user/kernel boundary 是保护、调度、fault 与设备处理的公共入口。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL book:E06]"

- node: cpu
  tier: T0
  rationale: "课程骨干+现代性：现代内核必须理解 SMP、per-CPU state 与 CPU-local execution，后续调度和并发均依赖它。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL engineering:E09]"

- node: init
  tier: T1
  rationale: "工程骨干：初始化顺序决定 MM、scheduler、timer、driver 何时可用，是跨子系统依赖的可执行体现。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: namespace
  tier: T1
  rationale: "现代素养：containers 已进入现代 OS 课程，namespace/resource isolation 是理解当代进程隔离的重要层。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL engineering:E18]"

- node: printk
  tier: T1
  rationale: "工程杠杆：可观测性是调试并发、fault、driver 和 boot 的共同前置条件，printk 是内核最基础的 tracing/logging 工具。"
  evidence:
    - "[EXTERNAL engineering:E22]"
    - "[EXTERNAL book:E06]"

- node: debug
  tier: T1
  rationale: "工程杠杆：现代内核开发把 testing、tracing、fault injection 和 debugging 作为一等开发设施，而非事后附属品。"
  evidence:
    - "[EXTERNAL engineering:E08]"
    - "[EXTERNAL book:E06]"

- node: klib
  tier: T2
  rationale: "支持性进阶：内核基础库重要，但其教学价值主要服务于更高层 subsystem，而不是独立的 OS 设计主轴。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: mm.early
  tier: T1
  rationale: "强烈建议：bootstrap allocator 和 early mappings 能解释内核如何在完整 allocator 尚不可用时自举。"
  evidence:
    - "[EXTERNAL course:E05]"
    - "[EXTERNAL book:E06]"

- node: mm.page_alloc
  tier: T0
  rationale: "课程骨干+工程骨干：物理页管理是页表、用户地址空间、page cache 与多数大型内核对象的资源底座。"
  evidence:
    - "[EXTERNAL course:E03]"
    - "[EXTERNAL book:E06]"

- node: mm.kmalloc
  tier: T0
  rationale: "工程骨干+教学骨干：通用 kernel dynamic allocation 贯穿 task、VFS、driver 与 IPC 数据结构，是对象生命周期的基础。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: mm.vm_map
  tier: T0
  rationale: "课程骨干+工程骨干：address-space mapping、page tables 与 protection 是操作系统隔离和 VM abstraction 的核心。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL book:E06]"

- node: mm.fault
  tier: T0
  rationale: "课程骨干+工程骨干：page fault 把 lazy allocation、COW、mmap 和 protection violation 串成可执行 VM 语义。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL book:E06]"

- node: mm.highmem
  tier: T3
  rationale: "特定场景：highmem 主要承载受限虚拟地址空间等历史/架构特化问题，对 x86_64 主线现代内核学习不是优先项。"
  evidence:
    - "[EXTERNAL historical-book:E07]"
    - "[EXTERNAL engineering:E08]"

- node: mm.misc
  tier: T2
  rationale: "进阶聚合项：值得按具体机制拆学，但不应让 misc 容器遮蔽 page allocator、mapping、fault 等更清晰的骨干节点。"
  evidence:
    - "[EXTERNAL book:E06]"

- node: sched.task
  tier: T0
  rationale: "课程骨干+工程骨干：task state/lifetime 是调度、signal、fork/exit、wait、资源控制的共同对象模型。"
  evidence:
    - "[EXTERNAL course:E01]"
    - "[EXTERNAL book:E06]"

- node: sched.forkexec
  tier: T0
  rationale: "课程骨干+工程骨干：fork/exec/exit/wait 把进程、地址空间、文件描述符和调度生命周期连成完整 Unix process model。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL book:E06]"

- node: sched.runqueue
  tier: T0
  rationale: "课程骨干+工程骨干：runnable-set 与 per-CPU queue 是从抽象 scheduling policy 到真实 SMP scheduler 的桥梁。"
  evidence:
    - "[EXTERNAL course:E03]"
    - "[EXTERNAL engineering:E09]"

- node: sched.scheduler
  tier: T0
  rationale: "课程骨干+工程骨干：CPU multiplexing、preemption、wake-up 与 SMP placement 是内核资源管理最核心机制之一。"
  evidence:
    - "[EXTERNAL course:E01]"
    - "[EXTERNAL engineering:E09]"

- node: sched.misc
  tier: T2
  rationale: "进阶聚合项：在 task/runqueue/scheduler 核心不变量掌握后，再按实际剩余机制拆分学习。"
  evidence:
    - "[EXTERNAL book:E06]"

- node: lock.atomic
  tier: T0
  rationale: "课程骨干+现代性：atomic RMW 和 memory ordering 是多核 synchronization 的最底层语言，不能被 mutex 抽象替代。"
  evidence:
    - "[EXTERNAL course:E03]"
    - "[EXTERNAL engineering:E11]"

- node: lock.spinlock
  tier: T0
  rationale: "课程骨干+工程骨干：spinlock 是短临界区、IRQ/preemption context 与大量内核共享状态保护的基本原语。"
  evidence:
    - "[EXTERNAL course:E03]"
    - "[EXTERNAL book:E06]"

- node: lock.semaphore
  tier: T1
  rationale: "强烈建议：sleeping synchronization、counting resource 与 blocking semantics 是 spin-based locking 之外的重要模型。"
  evidence:
    - "[EXTERNAL course:E01]"
    - "[EXTERNAL course:E03]"

- node: lock.futex
  tier: T1
  rationale: "强烈建议：futex 展示 userspace fast path 与 kernel blocking slow path 的经典协作，是现代线程库的重要内核接口思想。"
  evidence:
    - "[EXTERNAL course:E03]"
    - "[EXTERNAL engineering:E08]"

- node: lock.lockdep
  tier: T1
  rationale: "工程正确性：把锁依赖与持锁不变量做成可检查图，比单纯增加更多 lock primitive 更能提升多核内核学习质量。"
  evidence:
    - "[EXTERNAL engineering:E10]"
    - "[EXTERNAL engineering:E08]"

- node: ipc.signal
  tier: T1
  rationale: "强烈建议：异步 signal 把 task state、syscall return、user context 与进程生命周期连接起来，是 Unix ABI 的关键部分。"
  evidence:
    - "[EXTERNAL historical-book:E07]"
    - "[EXTERNAL course:E05]"

- node: kactive.softirq
  tier: T1
  rationale: "工程中心度：interrupt top-half 与 deferred bottom-half 的拆分是高频 I/O 与网络处理的重要执行模型。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: kactive.workqueue
  tier: T1
  rationale: "工程中心度：workqueue 是 Linux 常用异步 process-context 执行机制，适合教学 blocking/deferred work 与 worker management。"
  evidence:
    - "[EXTERNAL engineering:E12]"
    - "[EXTERNAL book:E06]"

- node: kactive.swait
  tier: T2
  rationale: "进阶同步：专用/simple wait primitive 的价值通常在掌握普通 wait/wakeup、completion 和锁之后。"
  evidence:
    - "[EXTERNAL engineering:E08]"

- node: kactive.completion
  tier: T1
  rationale: "强烈建议：one-shot event completion 是驱动初始化、异步任务结束与 producer/consumer coordination 的清晰教学原语。"
  evidence:
    - "[EXTERNAL engineering:E08]"
    - "[EXTERNAL engineering:E12]"

- node: time.systick
  tier: T1
  rationale: "强烈建议：周期 tick 是理解抢占、timeout 和传统 scheduler clock 的直接入口，即使现代内核会进一步走向 tickless。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL course:E05]"

- node: time.ktime
  tier: T1
  rationale: "工程基础：统一 kernel time representation 能支撑 scheduler accounting、timer 和 timeout correctness。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: time.clocksource
  tier: T1
  rationale: "强烈建议：把 hardware clock source 与 higher-level timekeeping 分层，是理解精确计时和跨平台 timer 的关键。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: time.timekeeping
  tier: T1
  rationale: "工程基础：monotonic/wall-clock、clocksource 与更新规则直接影响 timeout、scheduler accounting 和用户时间语义。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: time.timer
  tier: T1
  rationale: "课程+工程：timer/timeout 是睡眠、重传、调度、driver 等大量异步控制流的公共机制。"
  evidence:
    - "[EXTERNAL course:E01]"
    - "[EXTERNAL book:E06]"

- node: time.adjtime
  tier: T2
  rationale: "进阶时间语义：clock adjustment 很重要但对核心调度/MM/并发理解不是前置，可在基础 timekeeping 后深入。"
  evidence:
    - "[EXTERNAL book:E06]"

- node: fs.vfs
  tier: T0
  rationale: "课程骨干+工程骨干：VFS-style file/inode/path/FD abstraction 是把用户 API、缓存、具体 FS 和设备统一起来的核心接口层。"
  evidence:
    - "[EXTERNAL course:E01]"
    - "[EXTERNAL book:E06]"

- node: fs.fat
  tier: T2
  rationale: "进阶具体格式：实现 FAT 很适合练习 block/dir/allocation，但其格式细节不等于现代 VFS/consistency 的骨干知识。"
  evidence:
    - "[EXTERNAL course:E01]"
    - "[EXTERNAL book:E06]"

- node: block
  tier: T1
  rationale: "强烈建议：block request lifecycle、queue、completion 与并行 I/O 是理解真实存储栈和 filesystem persistence 的关键中层。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E21]"

- node: device
  tier: T1
  rationale: "强烈建议：通用 device abstraction 能把 bus、driver、DMA/IRQ、lifetime 和 userspace-facing resources 分离。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL engineering:E08]"

- node: drivers.ata
  tier: T2
  rationale: "硬件/协议进阶：ATA 可深入真实 block device，但具体命令集不应先于通用 block/device/PCI 模型。"
  evidence:
    - "[EXTERNAL course:E03]"
    - "[EXTERNAL book:E06]"

- node: drivers.base
  tier: T1
  rationale: "工程中心度：driver model、probe/remove、resource lifetime 和 device abstraction 是扩展多种硬件的公共基础。"
  evidence:
    - "[EXTERNAL engineering:E08]"
    - "[EXTERNAL book:E06]"

- node: drivers.char
  tier: T2
  rationale: "具体接口进阶：字符设备是练习 syscall/device boundary 的好载体，但本身不如通用 driver model 和 interrupt/DMA 重要。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL book:E06]"

- node: drivers.pci
  tier: T1
  rationale: "强烈建议：enumeration、BAR/resource、interrupt routing 与 device discovery 是现代 PC 驱动栈的重要共同层。"
  evidence:
    - "[EXTERNAL engineering:E08]"
    - "[EXTERNAL book:E06]"

- node: drivers.rtc
  tier: T3
  rationale: "特定设备：RTC 对 wall-clock bootstrapping 有用，但学习收益明显低于 generic timekeeping、PCI 与 block/network I/O。"
  evidence:
    - "[EXTERNAL book:E06]"

- node: lib.digit
  tier: T3
  rationale: "辅助工具：数字转换属于实现支持代码，不构成现代内核设计的独立学习主轴。"
  evidence:
    - "[EXTERNAL engineering:E08]"

- node: lib.idr
  tier: T2
  rationale: "通用数据结构进阶：ID allocation 对 kernel object management 很实用，但应服务于 task/device 等上层生命周期学习。"
  evidence:
    - "[EXTERNAL engineering:E08]"
    - "[EXTERNAL book:E06]"

- node: lib.list
  tier: T2
  rationale: "通用数据结构进阶：intrusive list 是内核常用基础设施，但重要度来自承载 runqueue/VFS/device 对象，而非算法本身。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: lib.printf
  tier: T3
  rationale: "辅助工具：格式化支持对 observability 有用，但设计学习应优先投入 printk/tracing/debug invariants。"
  evidence:
    - "[EXTERNAL engineering:E22]"

- node: lib.string
  tier: T3
  rationale: "辅助工具：必要但通用的 C/string primitive，不构成独立 OS 设计能力。"
  evidence:
    - "[EXTERNAL engineering:E08]"

# ---- capability-level T0/T1 nodes ----

- node: arch.x86_64.interrupt_exception_entry
  tier: T0
  rationale: "正确保存/恢复 context 与区分 exception/interrupt 是 fault、preemption 和 device handling 的硬件边界基础。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL book:E06]"

- node: arch.x86_64.page_table_mmu
  tier: T0
  rationale: "页表权限、TLB 与地址翻译是 isolation、fault、COW 和 mmap 的共同硬件机制。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL historical-book:E07]"

- node: entry.syscall_entry_exit
  tier: T0
  rationale: "syscall entry/return 把 privilege transition、ABI、signal、scheduler 与用户内存访问集中到一个关键边界。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL book:E06]"

- node: cpu.smp_bringup
  tier: T0
  rationale: "多核启动后才能真正面对 per-CPU state、跨 CPU wakeup、cache coherence 和 scheduling concurrency。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL engineering:E09]"

- node: cpu.per_cpu_state
  tier: T0
  rationale: "per-CPU partitioning 是现代内核减少共享写热点、实现 runqueue 与 scalable fast path 的基础技巧。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL engineering:E08]"

- node: mm.early.bootstrap_allocator
  tier: T1
  rationale: "用一个极小 early allocator 自举到完整 page allocator 能清晰揭示 kernel initialization dependency。"
  evidence:
    - "[EXTERNAL course:E05]"
    - "[EXTERNAL book:E06]"

- node: mm.page_alloc.physical_page_allocator
  tier: T0
  rationale: "能够分配、释放并验证 physical page ownership 是 VM 和 page-backed kernel object 的资源根。"
  evidence:
    - "[EXTERNAL course:E03]"
    - "[EXTERNAL book:E06]"

- node: mm.kmalloc.kernel_heap_allocator
  tier: T0
  rationale: "kernel heap 把 page-level storage 转成 variable-size object lifetime，是多数 subsystem 的直接依赖。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: mm.vm_map.address_space_mapping
  tier: T0
  rationale: "map/unmap/protect 是 virtual-memory abstraction 的原子操作，后续 mmap、COW、loader 都建立其上。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL book:E06]"

- node: mm.vm_map.mmap_userspace
  tier: T1
  rationale: "mmap 把 VFS、VM、fault 和用户 ABI 直接连接，适合作为跨 subsystem 的高价值进阶。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL book:E06]"

- node: mm.vm_map.kernel_w_x_hardening
  tier: T1
  rationale: "现代安全：内核 executable memory 不应 writable，W^X 是把 VM protection 用于 self-protection 的最小可教学版本。"
  evidence:
    - "[EXTERNAL modern-security:E13]"
    - "[EXTERNAL course:E02]"

- node: mm.fault.demand_paging_cow
  tier: T0
  rationale: "demand paging + COW 是理解 fault-driven lazy state transition 的经典骨干案例。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL book:E06]"

- node: sched.task.task_lifecycle
  tier: T0
  rationale: "new/runnable/running/sleeping/zombie 等生命周期和引用关系是 scheduler/fork/signal/wait correctness 的共同基础。"
  evidence:
    - "[EXTERNAL course:E01]"
    - "[EXTERNAL book:E06]"

- node: sched.task.resource_control
  tier: T1
  rationale: "现代资源治理：理解 task 如何被层级 CPU/memory policy 约束，是 container-era kernel literacy 的重要部分。"
  evidence:
    - "[EXTERNAL engineering:E18]"
    - "[EXTERNAL course:E02]"

- node: sched.forkexec.fork_exec_wait_exit
  tier: T0
  rationale: "完整实现 fork→exec→exit→wait 可同时验证 VM inheritance、FD lifetime、scheduler state 和 parent/child semantics。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL book:E06]"

- node: sched.runqueue.per_cpu_runqueue
  tier: T0
  rationale: "per-CPU runqueue 是从单核调度走向 scalable SMP scheduler 的第一性结构选择。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL engineering:E09]"

- node: sched.scheduler.preemptive_scheduling
  tier: T0
  rationale: "抢占点、context switch 和 runnable selection 是 CPU virtualization 的最小可执行机制。"
  evidence:
    - "[EXTERNAL course:E01]"
    - "[EXTERNAL book:E06]"

- node: sched.scheduler.smp_load_balance
  tier: T0
  rationale: "多核课程权重+现代工程中心度：task placement/migration 是从多个独立 CPU queue 走向系统级吞吐与公平性的关键。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL engineering:E09]"

- node: sched.scheduler.priority_inversion_handling
  tier: T1
  rationale: "正确性+实时性：priority inversion 展示 scheduler policy 与 lock ownership 如何交叉，适合在基础调度后深入。"
  evidence:
    - "[EXTERNAL course:E03]"
    - "[EXTERNAL engineering:E09]"

- node: lock.atomic.memory_ordering
  tier: T0
  rationale: "多核正确性的硬底座：必须理解 acquire/release、barrier 与 compiler/CPU reorder，才能证明 wakeup 和 lock-free paths。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL engineering:E11]"

- node: lock.atomic.rcu_read_mostly
  tier: T1
  rationale: "现代多核素养：实现 RCU/epoch-lite 能学习 read-mostly scaling、grace period 与 lifetime ordering，而无需复制 TREE_RCU。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL engineering:E11]"

- node: lock.spinlock.irq_preempt_safe_locking
  tier: T0
  rationale: "内核锁不能脱离 execution context；理解 IRQ/preempt interaction 是避免 deadlock 与 data race 的基本要求。"
  evidence:
    - "[EXTERNAL course:E03]"
    - "[EXTERNAL book:E06]"

- node: lock.semaphore.sleeping_lock
  tier: T1
  rationale: "区分 spinning 与 sleeping synchronization 能建立 blocking context、scheduler cooperation 和 latency 的正确直觉。"
  evidence:
    - "[EXTERNAL course:E03]"
    - "[EXTERNAL book:E06]"

- node: lock.futex.user_kernel_wait_wake
  tier: T1
  rationale: "futex-style fast/slow path 把 userspace atomics 与 kernel waitqueue 连接，是用户线程同步的高价值机制。"
  evidence:
    - "[EXTERNAL course:E03]"
    - "[EXTERNAL engineering:E08]"

- node: lock.lockdep.runtime_dependency_graph
  tier: T1
  rationale: "用 runtime lock graph 检测 dependency cycle/incorrect context，可把并发知识从经验升级成可检查 invariant。"
  evidence:
    - "[EXTERNAL engineering:E10]"
    - "[EXTERNAL engineering:E08]"

- node: ipc.signal.async_signal_delivery
  tier: T1
  rationale: "signal delivery/return 要同时处理 task state、saved user context 和 syscall interruption，是优秀的跨边界练习。"
  evidence:
    - "[EXTERNAL historical-book:E07]"
    - "[EXTERNAL course:E05]"

- node: kactive.softirq.deferred_interrupt_work
  tier: T1
  rationale: "把硬中断最小化并延后非紧急工作，是理解 latency、reentrancy 和 high-rate I/O 的基础模式。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: kactive.workqueue.async_process_context
  tier: T1
  rationale: "workqueue 展示如何把不能在 atomic context 完成的工作转移到可阻塞 process context。"
  evidence:
    - "[EXTERNAL engineering:E12]"
    - "[EXTERNAL book:E06]"

- node: kactive.completion.one_shot_wait
  tier: T1
  rationale: "completion 是简单但高杠杆的 asynchronous handoff primitive，适合驱动、初始化和 worker coordination。"
  evidence:
    - "[EXTERNAL engineering:E12]"
    - "[EXTERNAL engineering:E08]"

- node: time.clocksource.monotonic_source
  tier: T1
  rationale: "把 monotonic hardware source 与 higher-level timekeeping 解耦可学习 wrap、frequency 与 platform abstraction。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: time.timekeeping.monotonic_wall_time
  tier: T1
  rationale: "正确区分 monotonic time 与可调整 wall time 能避免 timeout 与 scheduler accounting 被时钟调整破坏。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: time.timer.high_resolution_timer
  tier: T1
  rationale: "从 periodic tick 走向 deadline-oriented timer 能学习精确 timeout、tickless design 与 timer queue。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E08]"

- node: fs.vfs.file_inode_dentry_model
  tier: T0
  rationale: "file/inode/dentry-style separation 能把 open instance、persistent object 与 pathname cache 分开，是 VFS 核心抽象。"
  evidence:
    - "[EXTERNAL course:E01]"
    - "[EXTERNAL book:E06]"

- node: fs.vfs.pathname_and_fd
  tier: T0
  rationale: "pathname resolution 与 FD table 把 namespace、process state 和 persistent object 串成用户可见文件语义。"
  evidence:
    - "[EXTERNAL course:E01]"
    - "[EXTERNAL book:E06]"

- node: fs.vfs.page_cache
  tier: T1
  rationale: "page cache 是 VM 与 storage 的关键交叉点，也是理解 mmap、writeback 和 filesystem performance 的必要进阶。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL course:E03]"

- node: fs.vfs.crash_consistency
  tier: T1
  rationale: "crash recovery 在 MIT 独立占据多讲，是从功能正确走向持久化正确性的关键学习阶段。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL course:E01]"

- node: fs.vfs.async_io_submission
  tier: T1
  rationale: "现代 I/O 素养：submission/completion、buffer lifetime 与 cancellation 比复制具体 io_uring UAPI 更值得教学内核掌握。"
  evidence:
    - "[EXTERNAL modern-io:E15]"
    - "[EXTERNAL engineering:E08]"

- node: block.bio_request_queue
  tier: T1
  rationale: "明确 request lifecycle 与 queue/completion boundary，能把 filesystem I/O 与具体 controller driver 解耦。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL engineering:E21]"

- node: block.multiqueue_parallel_io
  tier: T1
  rationale: "现代高速存储依赖并行硬件 queue；blk-mq 是理解从机械盘时代到 NVMe 时代 block design 演进的好教材。"
  evidence:
    - "[EXTERNAL engineering:E21]"
    - "[EXTERNAL modernity:E08]"

- node: block.writeback
  tier: T1
  rationale: "dirty data writeback 把 page cache、durability、memory pressure 和 block scheduling 连接起来。"
  evidence:
    - "[EXTERNAL book:E06]"
    - "[EXTERNAL course:E02]"

- node: device.network_stack
  tier: T1
  rationale: "课程实践+工程中心度：network stack 已是现代 OS 教学与 Linux 的大型 subsystem；因词汇表无 net 前缀暂挂 device。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL engineering:E17]"

- node: device.dma_interrupt_io
  tier: T1
  rationale: "现代设备 I/O 的核心不是轮询寄存器，而是 DMA buffer ownership、interrupt/completion 与 concurrency。"
  evidence:
    - "[EXTERNAL course:E05]"
    - "[EXTERNAL engineering:E08]"

- node: drivers.base.device_driver_model
  tier: T1
  rationale: "probe/remove、resource lifetime 与 bus/device/driver 分离是扩展多种真实硬件时最可复用的设计知识。"
  evidence:
    - "[EXTERNAL engineering:E08]"
    - "[EXTERNAL book:E06]"

- node: drivers.base.safe_driver_interfaces
  tier: T1
  rationale: "现代安全接口：即使继续使用 C，也值得借鉴 Rust-for-Linux 的显式 safety precondition、窄 unsafe boundary 与资源生命周期约束。"
  evidence:
    - "[EXTERNAL modern-safety:E16]"
    - "[EXTERNAL engineering:E08]"

- node: drivers.pci.enumeration_resources
  tier: T1
  rationale: "PCI discovery/BAR/IRQ/resource assignment 是把 firmware/hardware topology 交给 driver model 的关键公共能力。"
  evidence:
    - "[EXTERNAL engineering:E08]"
    - "[EXTERNAL book:E06]"

- node: namespace.process_isolation
  tier: T1
  rationale: "namespace-style isolation 是 container abstraction 的核心组成，适合从 nsproxy 落点演进一个最小教学版本。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL engineering:E18]"

- node: debug.bpf_style_programmability
  tier: T1
  rationale: "现代可编程性：实现 verifier+有限 helpers+trace hooks 的极小 BPF-like VM，可同时学习 extensibility、安全边界与 observability。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL modern-programmability:E14]"

- node: debug.fault_injection_tracing
  tier: T1
  rationale: "现代开发素养：主动制造失败并追踪 invariant，能让 allocator、I/O、locking 和 recovery path 得到系统性验证。"
  evidence:
    - "[EXTERNAL engineering:E08]"
    - "[EXTERNAL book:E06]"

- node: user.initramfs.syscall_abi_exerciser
  tier: T1
  rationale: "把 shell/init/lib 变成自动化 ABI regression harness，可用用户可见行为约束 kernel subsystem 演进。"
  evidence:
    - "[EXTERNAL course:E02]"
    - "[EXTERNAL course:E05]"

- node: boot.uefi.kernel_handoff
  tier: T1
  rationale: "固定并验证 firmware memory map、kernel image、stack 与 early allocator handoff，可消除 boot/early-MM 间的隐式假设。"
  evidence:
    - "[EXTERNAL engineering:E08]"
    - "[EXTERNAL course:E05]"
```

这份 YAML 的 T0 刻意没有把“具体 Linux 高级算法”当骨干，而是选择了 entry/MM/task/scheduler/atomic/spin/VFS 等跨课程反复出现、同时又被真实内核大量其他机制依赖的节点。MIT、Berkeley、OSTEP 和 LKD 对这组核心形成了高度独立的交叉支持。citeturn9view0turn9view1turn9view2turn9view3turn9view4

T1 的“现代扩展”也不是凭热点挑选：RCU 与多核扩展性、BPF、container、Meltdown 已直接进入 MIT 课程；lockdep、W^X、自保护、workqueue、cgroup、networking、io_uring 和 Rust safety contract 都有当前 Linux 官方设计文档。citeturn9view1turn8search0turn13view1turn8search3turn13view5turn13view4turn13view0turn15search0

`block.multiqueue_parallel_io` 被置为 T1 而不是设备特化项，是因为 Linux blk-mq 的设计目标正是利用现代高速存储的并行性，而非仅围绕传统磁盘寻道优化。citeturn15search10 `device.network_stack` 则是任务边界要求的“仓库未涉足领域也要入表”的直接落实；当前 Linux networking 文档的规模及 MIT network lab/lectures 都足以支持其 T1 地位。citeturn13view4turn9view0

## `agent-workspace/results/MYOS2-DR-007/roadmap.md`

```markdown
---
task_id: MYOS2-DR-007
produced_by: "GPT-5.5 Thinking / Deep Research"
date: 2026-08-31
base_snapshot: "kernel=time @ a039d9803ade2a1613d620bda375e028530d5242; workspace=master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719"
inputs_read:
  - agent-workspace/conventions.md
  - agent-workspace/repo-map.md
  - agent-workspace/tasks/MYOS2-DR-007-importance-and-roadmap.md
  - mykernel/
  - mykernel/sched/
  - mykernel/sched/scheduler/scheduler_core.c
  - mykernel/lock_IPC/
  - mykernel/lock_IPC/lockdep/lockdep.c
  - mykernel/fs/
  - mykernel/debug/
  - mykernel/device/
  - mykernel/namespace/
  - myinitramfs/
status: final
open_questions:
  - "以下 prerequisite 图是本任务根据通用 OS 依赖关系独立整理的概念图，没有读取 MYOS2-DR-003 的产出。"
  - "路线强度使用 relative intensity，不给虚假日历工期。"
  - "网络路线暂以 device.network_stack 表示；若后续词汇表增加 net，应整体迁移该节点而不是保留兼容别名。"
---

# MyOS2 学习路线图

## 选择原则

目标不是“让 MyOS2 尽快拥有更多 feature”，而是最大化：

`每一份实现工作 -> 可复用的现代内核设计理解`

所以路线优先级综合考虑：

1. `importance.yaml` 的 T0/T1；
2. repo-map 的规模信号；
3. 当前源码是否已经提供足够上下文，可以把真实 Linux 复杂性缩成可理解实验；
4. 一个 milestone 能否同时验证多个 subsystem invariant；
5. 是否容易用测试/trace 得到客观反馈。

## 不依赖 003 的概念前置图

[INFERRED]

```text
arch / boot
    |
    v
entry ---------> debug / printk
  |                  ^
  v                  |
cpu / SMP -------> locking / memory-ordering
  |                  |
  +-------> scheduler/task <------ time
  |             |
  |             +-------> fork/exec/signal/futex
  |                              |
  v                              v
MM/page allocator ----------> userspace ABI
  |                              ^
  v                              |
VFS/page cache ----> block ----> drivers/device
  |
  +-------> namespace / user ecosystem
```

这不是 MYOS2-DR-003 的图，也没有读取 003；只是本任务为路线排序使用的 dependency concepts。

## 仓库规模与结构信号

[VERIFIED repo-map.md]

`time` 快照的大致规模信号显示：

- `mm`：约 13.5k LoC
- `sched`：约 10.1k LoC
- `fs`：约 9.7k LoC
- `drivers`：约 6.5k LoC
- `lock_IPC`：约 6.4k LoC
- `time`：约 4.6k LoC
- `kactive`：约 2.6k LoC
- `block`：约 2.2k LoC
- `namespace`、`debug` 等明显更小

这些数字只作“已有投入/上下文”的粗信号，不作为完成度。

### 更重要的源码抽样

[VERIFIED mykernel/sched/scheduler/scheduler_core.c]

当前 scheduler core 保留大量 Linux scheduler 的原始 concurrency commentary：

- runnable-task migration 的 rq lock ordering；
- blocking/wakeup 的 release/acquire ordering；
- `try_to_wake_up()` 周围的 memory-barrier rationale；
- task CPU migration / remote reschedule 等结构。

但多处对应实现被注释，或者替换成明显更窄的 MyOS 路径。

[INFERRED]

这不是“已证明 scheduler 有 bug”，而是说明当前代码正处在非常适合学习的阶段：
**真实 Linux invariant 已经摆在代码旁边，但 MyOS2 尚未被迫吞下 Linux 全部复杂度。**

[VERIFIED mykernel/lock_IPC/lockdep/lockdep.c]

`lockdep.c` 当前只有定义宏和 include。

[INFERRED]

因此一个 `lockdep-lite` 有极高的增量学习价值。

[VERIFIED mykernel/fs/]

已有 VFS + FAT + fs syscall，存储路线可以直接从抽象一致性和 crash semantics 入手，而不是从“如何读第一个扇区”开始。

[VERIFIED myinitramfs/]

已有 init/library/shell，可成为用户态 ABI regression layer。

[VERIFIED mykernel/device/]

通用 device 层当前极薄，因此 networking / modern driver model 路线会涉及更大的新设计面。

## 路线 A：并发正确性优先

### 目标

把当前 MyOS2 从“已经有多核/调度/锁相关代码”推进到：

> Owner 能够逐条说明并验证 task state、runqueue ownership、wakeup、migration、IRQ/preemption、memory ordering 的 correctness contract。

这条路线重点不是追求一个更复杂 scheduler policy，而是建立**可以信赖的并发底座**。

### 为什么它是一条独立路线

它主要改变的是：

- correctness model；
- synchronization semantics；
- SMP execution；
- runtime verification。

即使完全不新增 filesystem/network feature，也可以独立取得完整学习成果。

### 前置依赖

必须已有基本理解：

- `arch.x86_64`
- `entry`
- `cpu`
- `mm.page_alloc`
- `sched.task`
- `sched.runqueue`
- `time.timer`
- `printk/debug`

不要求先完成完整 VFS、network 或 namespace。

### Milestone A1：并发不变量账本

先不新增 scheduler algorithm。

为这些对象写明确 invariant：

```text
task:
  who owns state transitions?
  when may task_cpu change?
  when is task runnable vs on runqueue vs executing?

runqueue:
  which CPU owns it?
  which lock/order protects it?
  may IRQ context mutate it?
  what operations require preemption disabled?

wait/wakeup:
  what publishes CONDITION?
  what publishes TASK_RUNNING/SLEEPING?
  what happens-before relation prevents lost wakeup?

migration:
  how are old rq and new rq ordered?
  when is a task visible on neither/both queue?
```

为每条 invariant 建 assertion / trace point。

**完成判据：**
代码 review 不再依赖“这里大概不会 race”，而能指到具体 state/lock/order rule。

### Milestone A2：memory ordering + execution context

聚焦：

- atomic RMW；
- acquire/release；
- explicit barriers；
- IRQ disable/restore；
- preempt disable/enable；
- spinlock 与 sleeping lock 的 context rule。

制作最小 litmus/stress cases：

- producer/consumer publication；
- wait/wakeup；
- two-CPU runqueue handoff；
- remote wakeup；
- task migration。

**完成判据：**
每个 barrier 都能回答“它和哪一个 load/store 配对”；不能回答的 barrier 要删除或重新设计。

### Milestone A3：scheduler state machine

围绕：

- `enqueue_task`
- `dequeue_task`
- `try_to_wake_up`
- `schedule`
- `set_task_cpu`
- context switch

建立一条可验证状态机。

先正确，再讨论 policy。

建议最初只保留简单 scheduler policy，但做到：

- no duplicate enqueue；
- no lost runnable task；
- no task running on two CPUs；
- sleeping task 不被错误 migration；
- remote wakeup 有明确 ordering；
- lock ordering 固定。

### Milestone A4：lockdep-lite

不复制 Linux lockdep。

最小版本：

1. 给 lock 分配 lock-class ID；
2. 每 CPU/task 记录当前 held-lock stack；
3. acquiring B while holding A 时增加 `A -> B`；
4. 检测 dependency cycle；
5. 检查：
   - lock 是否 double-acquire；
   - unlock 是否 owner/order 错；
   - irq-safe / irq-unsafe context misuse；
6. panic/trace 输出最短 dependency path。

**学习收益：**
graph invariant、dynamic verification、false-positive trade-off、debug metadata design。

### Milestone A5：SMP load balance + RCU-lite

基础 correctness 稳定后再实现：

- per-CPU runnable load；
- periodic/idle balance；
- controlled task migration；
- simple CPU affinity。

然后为 read-mostly object 实现：

- epoch/RCU-lite read section；
- deferred reclamation；
- grace-period test。

不需要复制 Linux TREE_RCU。

### 可选现代强化

加入：

- lock contention statistics；
- fault injection；
- scheduler trace ring；
- W^X assertion；
- race-oriented stress test。

### 强度

```yaml
conceptual_intensity: very_high
code_breadth: medium
debugging_intensity: very_high
cross_subsystem_breadth: high
new_hardware_dependency: low
```

### 这条路线最终教会什么

- 内核为什么最难的是 state transition，而不是函数数量；
- 为什么 SMP correctness 需要 memory model；
- 为什么 scheduler、locking、interrupt 不能独立思考；
- 为什么生产内核需要 lockdep/trace 这种 correctness tooling；
- 为什么可扩展同步并不等于“换成更快的锁”。

## 路线 B：存储栈打通与持久化正确性优先

### 目标

把已有：

`VFS -> FAT -> block -> ATA/PCI`

升级成可以解释：

> 一次 write 从 userspace 到持久介质经历哪些 ownership、cache、queue 和 durability 状态；断电发生在每个点会看到什么。

### 为什么它是一条独立路线

它主要研究：

- persistent state；
- caching；
- I/O scheduling；
- crash consistency；
- device completion。

它与路线 A 的“CPU/SMP correctness”焦点明显不同。

### 前置依赖

需要：

- `mm.page_alloc`
- `mm.vm_map`
- basic locking
- `kactive.completion`
- `time.timer`
- `fs.vfs`
- `block`
- `drivers.pci`
- storage driver

不要求先实现 namespace/BPF/network。

### Milestone B1：VFS invariant

先明确：

- file object lifetime；
- inode identity/lifetime；
- pathname component lookup；
- FD table ownership；
- mount/root semantics；
- concurrent open/read/write/unlink 的规则。

为 VFS object 加 reference/lifetime assertions。

### Milestone B2：统一 page cache

把：

```text
file offset
    -> cached page
    -> dirty state
    -> writeback
    -> block mapping
```

形成一个清晰模型。

重点学习：

- cache hit/miss；
- dirty ownership；
- eviction；
- mmap 与 read/write coherence；
- memory pressure。

### Milestone B3：block request lifecycle

把一次 I/O 显式拆成：

```text
submit
 -> queue
 -> dispatch
 -> device ownership
 -> interrupt/completion
 -> caller wakeup
```

为每个 request 标记 state。

进一步做：

- queue depth；
- merge / batching；
- per-device queue；
- completion concurrency。

### Milestone B4：multi-queue 思维

不需要完整复制 Linux blk-mq。

教学版可以：

- 每 CPU software submission queue；
- 一个或多个 hardware queue；
- batch dispatch；
- queue-depth backpressure；
- completion affinity。

然后测：

- 单 queue lock contention；
- multi-queue scaling；
- latency/throughput trade-off。

### Milestone B5：crash consistency

不要直接上一个完整 ext4。

先做最小 transaction/log：

1. metadata update 有 transaction ID；
2. write-ahead log；
3. commit record；
4. boot recovery replay；
5. fault injection 在每个 block write 后模拟 crash。

测试必须回答：

- old state？
- new state？
- impossible mixed state？

这一步的核心是 durability ordering，不是 filesystem feature 数。

### Milestone B6：async I/O 教学版

设计一个比 io_uring 小得多的接口：

```text
submission ring/queue
completion queue
request id
user/kernel buffer contract
cancel
timeout
```

重点学习：

- async lifetime；
- user buffer pin/copy trade-off；
- cancellation race；
- completion ordering。

### 强度

```yaml
conceptual_intensity: high
code_breadth: high
debugging_intensity: high
cross_subsystem_breadth: very_high
new_hardware_dependency: medium
```

### 这条路线最终教会什么

- VFS 为什么要分 object model；
- page cache 为什么同时是 MM 和 storage 问题；
- high-performance I/O 为什么是 queue/concurrency 问题；
- “write 返回”与“数据已持久”为什么不是同一句话；
- crash consistency 如何用 protocol/invariant 而不是运气保证。

## 路线 C：用户态生态与现代 ABI 优先

### 目标

利用已有 initramfs/init/shell，把 MyOS2 推进成一个：

> 可以用真实用户程序持续验证 process/thread/IPC/FS/isolation/network 行为的“小型现代 Unix playground”。

### 为什么它是一条独立路线

它重点追求：

- user-visible semantics；
- ABI completeness；
- isolation；
- IPC/network；
- observability/extensibility。

它不是先把 scheduler 或 storage 内部做到最深，而是用应用需求反向驱动 kernel contract。

### 前置依赖

需要：

- syscall entry；
- task/forkexec；
- basic VM；
- basic VFS；
- basic locking；
- signals；
- initramfs。

### Milestone C1：ABI regression suite

把 `myinitramfs` 从 demo 变成 test harness。

建立：

- fork/exec/wait tests；
- FD inheritance；
- pipe/IPC tests；
- mmap/fault tests；
- signal interruption/restart tests；
- error-code tests；
- stress loops。

每次 kernel 改动都跑。

### Milestone C2：线程同步闭环

补齐或强化：

- userspace atomics；
- futex wait/wake；
- timeout；
- signal interaction；
- thread exit/join semantics。

用真实 pthread-like library interface 封装。

### Milestone C3：isolation + resource control

基于现有 namespace 落点实现一个小闭环。

先只做两个 isolation dimension，例如：

- PID visibility；
- mount/root view。

再做 cgroup-lite：

- process group；
- CPU runtime quota；
- optional memory limit/accounting。

重点是理解 hierarchy/policy hook，不复制 Linux controller 数量。

### Milestone C4：最小网络栈

因为公约词汇表没有 `net`，结果中临时记作 `device.network_stack`。

建议学习顺序：

```text
virtio/e1000-like NIC abstraction
 -> DMA descriptor ownership
 -> interrupt/NAPI-like polling idea
 -> Ethernet
 -> ARP
 -> IPv4
 -> ICMP
 -> UDP
 -> socket-like user API
```

TCP 可作为后续扩展，不要让完整 TCP 状态机阻塞第一轮网络学习。

需要重点观察：

- RX livelock；
- interrupt mitigation；
- packet ownership；
- zero-copy vs isolation；
- multi-queue。

### Milestone C5：BPF-style tracing VM

不要复制 eBPF instruction set。

做一个极小 verified bytecode：

- arithmetic/load；
- bounded branch；
- read-only context access；
- fixed helper set；
- instruction-count cap；
- verifier 拒绝 unbounded loop / invalid pointer。

挂点：

- syscall enter/exit；
- context switch；
- page fault；
- VFS open/read/write；
- network RX/TX。

这样 userspace 可以写安全 trace program，而不用重新编译 kernel。

### Milestone C6：safe driver API

不要求切换 Rust。

先在 C API 中引入：

- typed lifecycle state；
- acquire/release symmetry；
- ownership comments；
- checked handles；
- cleanup-on-failure pattern；
- explicit unsafe boundary documentation。

然后可挑一个很小 driver 做 Rust-for-Linux-style rewrite 实验，比较：

- resource lifetime bugs；
- interface verbosity；
- unsafe surface；
- FFI boundary。

### 强度

```yaml
conceptual_intensity: high
code_breadth: very_high
debugging_intensity: high
cross_subsystem_breadth: very_high
new_hardware_dependency: high
```

### 这条路线最终教会什么

- kernel ABI 如何被真实 userspace 约束；
- fast userspace / slow kernel path 如何组合；
- container isolation 如何跨 scheduler/MM/VFS；
- network stack 如何把 device、DMA、interrupt、scheduler、socket ABI 串起来；
- BPF 为什么必须同时设计 programmability 与 verifier；
- memory safety 如何转化为 API contract，而不仅是语言选择。

## 三条路线对照

| 维度 | A 并发正确性 | B 存储栈 | C 用户态生态 |
|---|---|---|---|
| 首要问题 | 多 CPU 下为什么正确 | crash 后为什么正确 | 应用看到的语义为什么正确 |
| 主节点 | cpu/sched/lock/debug | VFS/MM/block/driver | entry/task/futex/ns/net/user |
| 新 feature 数 | 低 | 中 | 高 |
| invariant 密度 | 极高 | 高 | 高 |
| 当前源码 leverage | 极高 | 高 | 中高 |
| 硬件依赖 | 低 | 中 | 高 |
| breadth | 中高 | 高 | 极高 |
| 最适合学习 | memory model/SMP | persistence/I/O | ABI/isolation/extensibility |

## 推荐：路线 A，并发正确性优先

### 核心理由

第一，外部教学证据最强。

OSTEP、MIT、Berkeley、LKD 都把 concurrency/synchronization/scheduling 放在骨干位置；MIT 又进一步把 multi-core scalability/RCU 纳入现代课程。

[EXTERNAL E01][EXTERNAL E02][EXTERNAL E03][EXTERNAL E06][EXTERNAL E11]

第二，它是 B/C 的共同前置。

存储路线需要：

- request queue locks；
- completion；
- page-cache concurrency；
- writeback ordering。

用户态路线需要：

- futex；
- signal；
- namespace lifecycle；
- network packet queues；
- multi-queue I/O。

并发底座不稳时，这些 subsystem 会把 race surface 成倍放大。

第三，MyOS2 当前源码给出了异常好的学习抓手。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c]
真实 Linux 的 migration/wakeup/memory-ordering rationale 已经存在于代码上下文中，但不少完整机制仍是注释/简化状态。

[VERIFIED mykernel/lock_IPC/lockdep/lockdep.c]
lockdep 还是极薄 scaffold。

[INFERRED]
这意味着 Owner 不需要先发明问题，也不需要一开始阅读完整 Linux scheduler：
MyOS2 已经把“生产 Linux 的复杂不变量”和“教学内核的可控代码量”放在了同一个地方。

第四，它会提高后两条路线的学习质量，而不是推迟它们。

A 完成后：

- B 的 page-cache/block/writeback race 更容易定位；
- C 的 futex/network/namespace 不会建立在模糊 wakeup semantics 上；
- 所有路线都能复用 lockdep-lite、trace 和 fault-injection。

## 建议主攻顺序

```text
A1 invariant ledger
 -> A2 memory ordering / IRQ / preemption
 -> A3 wakeup + scheduler state machine
 -> A4 lockdep-lite
 -> A5 SMP balance + RCU-lite
 -> 再选择 B 或 C
```

若 Owner 下一阶段希望获得最深的“内核设计”理解，而不是最快获得最多 feature，这条顺序的学习收益最高。

## 不推荐现在优先做的事项

### 不先复制完整 EEVDF/CFS

scheduler policy 复杂并不等于 scheduler correctness。
先证明 runqueue/wakeup/migration，再研究 EEVDF。

### 不先做完整 TCP

TCP 会迅速把工作量推向协议细节。
先学 NIC/DMA/RX path/UDP/socket boundary。

### 不先全量 Rust 化

Rust-for-Linux 最重要的学习点是 safety boundary、lifetime 和 interface contract。
先把这些思想应用于一个 driver/subsystem，再判断语言迁移收益。

### 不先追求更多 filesystem 格式

在已有 FAT 情况下，第二个、第三个磁盘格式的学习收益低于：

- page cache；
- writeback；
- transaction；
- crash recovery。

### 不把 highmem 当主线

在 x86_64 主线目标下，它属于兼容/架构特化知识；应把时间投入现代 MM protection、fault、NUMA/SMP locality 等更高杠杆主题。

## 最终建议

**主线：路线 A。**

**第二阶段分叉：**

- 更想理解“数据如何可靠落盘” → B；
- 更想把 MyOS2 变成可运行现代系统概念的实验场 → C。

最终理想形态不是三选一，而是：

```text
A 提供 correctness substrate
        |
        +---- B 构建 persistence/I/O depth
        |
        +---- C 构建 ABI/isolation/extensibility breadth
```

但第一步应该是 A，因为它既是外部教学体系中的骨干，也是当前 MyOS2 源码最具学习杠杆的区域。
```

路线 A 的推荐并不是因为“调度代码最多”，而是因为源码呈现出的**不变量密度**最高：当前 `scheduler_core.c` 中直接保留了 migration 的 rq-lock ordering、blocking/wakeup 的 release/acquire 关系和 `try_to_wake_up()` 的 barrier rationale，同时多段实际同步路径仍被注释或简化；与此同时 lockdep implementation 还是一个极薄 scaffold。fileciteturn9file0L2-L2 fileciteturn12file0L2-L5 这使“重新建立并验证并发契约”比“再加一种调度 policy”有高得多的学习杠杆。外部证据也吻合：MIT 把 locking、parallelism、multi-core scalability/RCU 都纳入正式课程，Berkeley 连续多讲处理 atomic/futex/semaphore/scheduling，而 Linux 的 memory-model、RCU、lockdep 都有独立设计体系。citeturn9view0turn9view1turn9view2turn8search9turn8search1turn8search0

路线 B 则由已有 VFS/FAT/block 基础和现代存储现实支撑：LKD 把 VFS、block I/O、page cache/writeback 分列主章，MIT 又把 filesystem、crash recovery、filesystem performance 连续安排，当前 Linux blk-mq 明确针对高速现代存储的并行 I/O。citeturn9view4turn9view1turn15search10

路线 C 的“现代生态”也有充分根据，而不是 feature wishlist：MIT 课程已经正式覆盖 networking、containers、BPF；Linux cgroup v2 明确围绕层级 process/resource control，BPF 官方设计强调受控 helpers/kfuncs 和 verifier 边界，当前 networking 文档同时覆盖 NAPI、多队列与 io_uring zero-copy；Rust coding guidelines 则把 safety precondition 和 unsafe-boundary justification 明文化。citeturn9view1turn13view5turn13view2turn13view4turn13view0turn15search0

## 核验与来源

四个交付物满足任务书的结构性验收要求：`importance.yaml` 对公约中的 **53 个 subsystem ID 全覆盖**，能力级节点集中覆盖 T0/T1；所有 T0 都显式附有至少两类独立证据，例如 course + book、course + engineering，而不是重复引用同一类材料。词汇表本身及合法命名约束来自工作区公约，任务的完整覆盖与 T0 双证据要求来自任务书。fileciteturn4file0L2-L2

外部研究中最重要的交叉结论是：**经典骨干没有因为“现代内核”而失效，而是现代主题叠加在它们上面。** 进程/调度、VM、并发、VFS 仍然占据 OSTEP、MIT、Berkeley 和经典 Linux 内核书的主体；RCU、containers、BPF、安全加固、io_uring 和 safe-language/API thinking 则构成值得追加的现代 T1 层。citeturn9view0turn9view1turn9view2turn9view3turn9view4turn13view1turn13view2turn13view0turn15search0

因此最终推荐不是“先追最新 feature”，而是：**先用 MyOS2 当前 scheduler/locking 的真实源码张力把 SMP correctness 学透，再从存储深度或用户态生态广度分叉。** 这同时最符合外部教学权重、现代 Linux 工程实践，以及 `time @ a039d9803ade2a1613d620bda375e028530d5242` 这一实际源码快照暴露出来的学习机会。fileciteturn5file0L2-L2 fileciteturn9file0L2-L2 fileciteturn12file0L2-L5