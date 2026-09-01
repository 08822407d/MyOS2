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
