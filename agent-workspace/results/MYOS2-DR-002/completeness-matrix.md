---
task_id: MYOS2-DR-002
produced_by: GPT-5.6 Pro
date: 2026-09-01
base_snapshot: time @ a039d9803ade2a1613d620bda375e028530d5242
inputs_read:
  - agent-workspace/conventions.md
  - agent-workspace/repo-map.md
  - agent-workspace/tasks/MYOS2-DR-002-subsystem-completeness.md
  - mykernel/mm/
  - mykernel/sched/
  - mykernel/lock_IPC/
  - mykernel/kactive/
  - mykernel/time/
  - mykernel/fs/
  - mykernel/arch/x86_64/
  - mykernel/arch/aarch64/
  - mykernel/block/
  - mykernel/device/
  - mykernel/drivers/ata/
  - mykernel/drivers/base/
  - mykernel/drivers/char/
  - mykernel/drivers/pci/
  - mykernel/drivers/rtc/
  - mykernel/init/
  - mykernel/entry/
  - mykernel/cpu/
  - mykernel/namespace/
  - mykernel/printk/
  - mykernel/debug/
  - mykernel/klib/
  - mykernel/lib/
  - myloader/uefi/
  - myinitramfs/
status: final
open_questions:
  - 本任务为静态源码盘点，未在该提交上执行完整编译、QEMU 启动或硬件回归。
  - myloader/BOOTX64.EFI 为仓库内预构建文件，顶层 CMake 未重建它；二进制与所读 UEFI 源码是否完全对应未验证。
  - 外部磁盘镜像、GPT/FAT 内容未纳入读取，PID 1、/boot/sh 与根文件系统的实际集成状态未运行验证。
  - 通用 timer wheel 的到期执行入口在已读路径中未定位；是否存在于未纳入构建的路径仍待链接图或运行验证。
---

# MyOS2 内核子系统完成度矩阵

## 1. 口径

本矩阵只描述 `time @ a039d9803ade2a1613d620bda375e028530d5242` 的静态源码状态，不把目录名、注释中的 Linux 原实现、TODO 或预构建二进制本身当作已完成能力。`implemented` 表示核心能力有连贯源码实现，不等于已经运行验证；`partial` 表示已有真实路径但关键语义或集成链未闭合；`missing` 表示所读快照中只有空壳、立即自旋或没有有效主体。

| 成熟度 | 定义 |
|---:|---|
| 0 | 空壳或不可用入口；没有可识别的有效基本路径 |
| 1 | 胚胎期/可编译骨架；只有局部操作，无法独立形成主要能力 |
| 2 | 基本路径可进入；边界、并发、回收或错误处理明显不完整 |
| 3 | 主要能力较完整，边界清楚；仍未达到成熟内核的全面并发正确性 |
| 4 | 功能完整，并具有可信的并发、生命周期和错误恢复语义 |

## 2. 总体统计

| 状态 | 数量 | 成熟度 | 数量 |
|---|---:|---|---:|
| `implemented` | 5 | 0 | 5 |
| `partial` | 43 | 1 | 13 |
| `missing` | 5 | 2 | 30 |
| `unknown` | 0 | 3 | 5 |
| 合计 | 53 | 4 | 0 |

[INFERRED] 整体呈现“广覆盖、浅闭环”的教学内核形态：x86-64 启动、物理/虚拟内存、进程、VFS、FAT32、ATA、基本用户态和 UEFI 装载都有真实代码，但 SMP、引用生命周期、计时器到期执行、错误恢复与用户内存安全是跨子系统的共同薄弱面。

## 3. 架构、启动与核心服务

| ID | 状态 / 成熟度 / 风险 | 已实现能力与证据 | 明显缺失 | 静态风险 |
|---|---|---|---|---|
| `arch.x86_64` | `partial` / 2 / high | [VERIFIED mykernel/arch/x86_64/kernel/head64.c; mykernel/arch/x86_64/kernel/setup.c; mykernel/arch/x86_64/entry/entry_64.S; mykernel/arch/x86_64/kernel/idt.c; mykernel/arch/x86_64/kernel/apic/apic.c; mykernel/arch/x86_64/smp/smpboot.c] 具备 64 位入口、临时及直接映射页表、E820/架构初始化、系统调用与异常/外部中断入口、Local APIC/x2APIC、per-CPU 区和 AP trampoline。 | [INFERRED] AP 上线握手与加入调度、动态 IRQ descriptor/vector、完整 FPU 初始化、KASLR/KPTI、漏洞缓解和多核 TSC 同步未形成。 | [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S; mykernel/arch/x86_64/entry/common.c] AP 进入 64 位后原地自旋；未知系统调用也进入无限循环；多项入口保护与通用钩子被注释。 |
| `arch.aarch64` | `missing` / 0 / high | [VERIFIED mykernel/arch/aarch64/CMakeLists.txt] 所读快照中只有空的构建占位。 | [INFERRED] 未见启动入口、异常向量、MMU、定时器、SMP、系统调用和上下文切换实现。 | [VERIFIED mykernel/arch/aarch64/CMakeLists.txt] 当前不能形成 AArch64 可启动目标。 |
| `boot.uefi` | `partial` / 2 / critical | [VERIFIED myloader/uefi/bootloader.c; myloader/uefi/ElfLib/Elf64Lib.c; myloader/uefi/multiboot2_utils.c] 可选择 GOP 模式、读取并装载 ELF64 内核，构造 framebuffer、E820、ACPI 和 CPU topology 的 Multiboot2 标签后跳转内核；仓库含 `BOOTX64.EFI`。 | [INFERRED] 缺少由顶层工程重建 UEFI 二进制的闭环、initrd 装载、配置/回退菜单、Secure Boot 支持和可靠的 `ExitBootServices` 重试。 | [VERIFIED myloader/uefi/bootloader.c; myloader/CMakeLists.txt] 内存映射缓冲被释放后才调用 `ExitBootServices`，其返回值未处理；退出 Boot Services 后仍调用 ACPI/MP Services；顶层 CMake 只安装预构建二进制。 |
| `user.initramfs` | `partial` / 2 / high | [VERIFIED myinitramfs/myinit/initd.c; myinitramfs/myshell/myshell.c; myinitramfs/mylib/myglibc/syscall/syscall.c] 含 CRT/轻量 libc/系统调用桥、PID 1 和基础 shell；init 重定向 console、fork/exec `/boot/sh`，shell 提供 `cd/ls/pwd/cat/touch/rm/mkdir/rmdir/reboot`。 | [INFERRED] 缺少 cpio 打包与内核解包集成、子进程回收、作业控制、管道/重定向、稳健 errno 语义和可移植构建。 | [VERIFIED myinitramfs/CMakeLists.txt; mykernel/init/do_mounts.c; myinitramfs/myshell/myshell.c] 构建硬编码本机 musl 路径，内核 initramfs 路径被注释；系统调用包装不设置 `errno`；shell 输入无边界保护并有多处分配泄漏。 |
| `entry` | `partial` / 1 / high | [VERIFIED mykernel/entry/common.c; mykernel/arch/x86_64/entry/common.c; mykernel/arch/x86_64/entry/entry_64.S] x86-64 syscall 汇编入口、编号分派、SYSRET 安全判断和用户返回前信号/重启调用存在。 | [INFERRED] 缺少完整 TIF work loop、返回前重调度、notify/audit/seccomp、RCU/context tracking 等通用入口语义。 | [VERIFIED mykernel/entry/common.c] work flag 检查被注释，返回路径无条件进入信号处理，难以保证只在需要时执行。 |
| `cpu` | `partial` / 2 / medium | [VERIFIED mykernel/cpu/hypervisor.c; mykernel/arch/x86_64/kernel/cpu/common.c] 可读取 BSP 的 CPUID 特性、地址位宽、缓存/型号信息，并识别 VMware、KVM 等 hypervisor。 | [INFERRED] 缺少厂商注册与专用初始化、完整 FPU 系统初始化、漏洞缓解、CPU hotplug 和 AP 逐核识别。 | [VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] 大量 vendor/FPU/mitigation 初始化被关闭，当前状态主要代表 BSP。 |
| `init` | `partial` / 2 / high | [VERIFIED mykernel/init/main.c; mykernel/init/do_mounts.c] 启动序列可初始化内存、调度、IRQ/HPET、VFS 和驱动，创建 PID 1/kthreadd，并尝试挂载/切换根与 exec。 | [INFERRED] 缺少正式 SMP 完成链、常规 workqueue 初始化、通用 PCI 扫描、initramfs 解包、init section 回收和多级 init fallback。 | [VERIFIED mykernel/init/main.c; mykernel/init/do_mounts.c] SMP/workqueue/PCI 等关键初始化被跳过或注释；存储或 init 失败路径多为原地自旋。 |
| `namespace` | `partial` / 1 / medium | [VERIFIED mykernel/namespace/nsproxy.c; mykernel/sched/task/pid_namespace.c] 有 nsproxy 复制/释放骨架和 PID namespace 的复制、引用与层级结构。 | [INFERRED] 未形成 mount、UTS、IPC、network、cgroup、time、user namespace 的隔离语义。 | [VERIFIED mykernel/namespace/nsproxy.c] 共享快速路径、引用保护与能力校验多被注释，跨任务并发语义不完整。 |
| `printk` | `partial` / 2 / high | [VERIFIED mykernel/printk/printk.c] 有固定环形日志缓冲、日志级别/时间戳、console 注册与 framebuffer console 分发，`printk/vprintk` 主路径可见。 | [INFERRED] 缺少可靠 console/logbuf 锁、完整递归保护、syslog 读取、持久日志和 SMP 序列化。 | [VERIFIED mykernel/printk/printk.c] 主要锁与级别过滤被注释，并发写入可能破坏索引或交错；`unregister_console()` 无有效返回路径。 |
| `debug` | `partial` / 1 / medium | [VERIFIED mykernel/debug/panic.c] `panic()` 可关闭本地中断/抢占、格式化并输出终止信息，随后不返回。 | [INFERRED] 缺少多 CPU 停机、可靠栈回溯、oops/taint、kmsg dump、crash kernel 和重启策略。 | [VERIFIED mykernel/debug/panic.c] 其他 CPU 不会被停下，panic 后系统全局状态不可控。 |
| `klib` | `partial` / 2 / low | [VERIFIED mykernel/klib/bitmap.c; mykernel/klib/font.c; mykernel/klib/printk.c] 提供基本位图操作、字体数据和早期彩色输出辅助。 | [INFERRED] 缺少边界检查、并发语义和动态位图管理。 | [VERIFIED mykernel/klib/bitmap.c] 位索引无范围校验，错误调用会直接访问越界。 |

## 4. 内存管理

| ID | 状态 / 成熟度 / 风险 | 已实现能力与证据 | 明显缺失 | 静态风险 |
|---|---|---|---|---|
| `mm.early` | `implemented` / 3 / medium | [VERIFIED mykernel/mm/early/earlymem_api.h; mykernel/mm/early/memblock.c] 实现 memblock 式内存区登记、保留、裁剪、对齐分配、DMA/DMA32 范围处理及向页分配器移交。 | [INFERRED] 未见 NUMA-aware early allocation、复杂固件冲突解析和热插拔早期内存模型。 | [VERIFIED mykernel/mm/early/memblock.c] 结构容量和启动期假设较静态，异常固件图的容错边界有限。 |
| `mm.page_alloc` | `partial` / 2 / critical | [VERIFIED mykernel/mm/page_alloc/page_alloc_api.h; mykernel/mm/page_alloc/buddy.c; mykernel/mm/page_alloc/zone.c] 有 DMA/DMA32/Normal zone、buddy 分配/释放/合并、高阶页和 folio/compound page 基础。 | [INFERRED] 缺少 reclaim、OOM、per-CPU page lists、compaction、NUMA、内存热插拔和 huge page 管理。 | [VERIFIED mykernel/mm/page_alloc/buddy.c] 分配侧链表锁与水位/回收路径不完整；高阶页清零指针计算存在可疑路径，可能写错页。 |
| `mm.kmalloc` | `partial` / 2 / critical | [VERIFIED mykernel/mm/kmalloc/slub.c; mykernel/mm/kmalloc/slab_common.c; mykernel/mm/kmalloc/kmalloc_api.h] 有 kmalloc 尺寸类、对象 cache、SLUB partial/full 链表和大对象转页分配。 | [INFERRED] 缺少 per-CPU fastpath、cache destroy/shrink、非法释放检测、poison/redzone 和完整 hardening。 | [VERIFIED mykernel/mm/kmalloc/slub.c; mykernel/mm/kmalloc/slab_common.c] 全局 cache 管理锁被注释；部分状态在解锁后检查，slab 回收条件可能释放仍含对象的 slab。 |
| `mm.vm_map` | `partial` / 2 / critical | [VERIFIED mykernel/mm/vm_map/mmap.c; mykernel/mm/vm_map/vm_map_api.h] 有 VMA 列表、合并/拆分、匿名与文件 `mmap`、`munmap`、`brk` 和未映射区搜索。 | [INFERRED] 缺少平衡索引、`mprotect/mremap`、完整锁/记账、ASLR 和共享映射一致性。 | [VERIFIED mykernel/mm/vm_map/mmap.c] gap 搜索存在空 VMA 解引用路径及赋值代替比较，边界条件可产生错误地址或崩溃。 |
| `mm.fault` | `partial` / 2 / critical | [VERIFIED mykernel/mm/fault/fault.c; mykernel/mm/fault/memfault_api.h] 有多级页表分配、匿名缺页、文件只读缺页、私有 COW 和 fork 页表复制。 | [INFERRED] 缺少 swap、huge page、共享可写 fault、成熟 rmap/refcount 和并发 TLB shootdown。 | [VERIFIED mykernel/mm/fault/fault.c] 共享可写及若干不支持路径直接无限循环；错误处理会将可恢复情况转为系统挂死。 |
| `mm.highmem` | `partial` / 1 / medium | [VERIFIED mykernel/mm/highmem/highmem.c; mykernel/mm/highmem/highmem.h] 直接映射条件下可做页间复制辅助。 | [INFERRED] 未形成 `kmap/kunmap`、临时映射槽和真正高端内存管理。 | [VERIFIED mykernel/mm/highmem/highmem.c] `memcpy_page/memset_page/memzero_page` 等若干公开辅助为空操作，调用者可能得到静默错误结果。 |
| `mm.misc` | `partial` / 2 / high | [VERIFIED mykernel/mm/misc/mm_misc_api.h; mykernel/mm/misc/mm_init.c; mykernel/mm/misc/gup.c; mykernel/mm/misc/percpu.c] 有 `init_mm`/内存总初始化、ioremap、简化 GUP、per-CPU 区和 mm 生命周期骨架。 | [INFERRED] 缺少成熟 NUMA、长期 pin 记账、MMU notifier、热插拔和完整 mm teardown。 | [VERIFIED mykernel/mm/misc/gup.c; mykernel/mm/misc/mm_init.c] 简化 GUP/生命周期依赖上层正确性，遇到并发解除映射或退出时保护不足。 |

## 5. 任务与调度

| ID | 状态 / 成熟度 / 风险 | 已实现能力与证据 | 明显缺失 | 静态风险 |
|---|---|---|---|---|
| `sched.task` | `partial` / 2 / high | [VERIFIED mykernel/sched/task/task_api.h; mykernel/sched/task/task.c; mykernel/sched/task/pid.c; mykernel/sched/task/pid_namespace.c] 有 `init_task`、PID 分配/查找、PID namespace、idle/new task 和唤醒接口。 | [INFERRED] 缺少完整进程组/session、cgroup、CPU affinity、SMP 放置与任务统计。 | [VERIFIED mykernel/sched/task/pid.c; mykernel/sched/task/task.c] PID 与任务关系的并发保护和退出期生命周期依赖未闭合的调度/RCU 语义。 |
| `sched.forkexec` | `partial` / 2 / critical | [VERIFIED mykernel/sched/forkexec/fork.c; mykernel/sched/forkexec/exec.c; mykernel/sched/forkexec/exit.c] `fork/clone` 可复制地址空间、文件表和命名空间；exec 有 binfmt 分派、argv/env 栈与 ELF 接口；exit 会释放文件/目录并重挂孤儿。 | [INFERRED] 缺少 `wait4`、mm 回收、线程组退出/de-thread、exec 凭据转换和完整 zombie 回收。 | [VERIFIED mykernel/sched/forkexec/exit.c; mykernel/sched/forkexec/fork.c] `wait4` 为空，exit 不释放 mm；sighand 复制关键赋值被注释，并存在重复 `fput`/硬自旋路径。 |
| `sched.runqueue` | `partial` / 1 / high | [VERIFIED mykernel/sched/runqueue/runqueue.h; mykernel/sched/runqueue/runqueue.c] 有 runqueue 数据结构、优先级和队列辅助宏。 | [INFERRED] 缺少有效 nice/weight 调整、通用 enqueue/dequeue、负载跟踪与迁移。 | [VERIFIED mykernel/sched/runqueue/runqueue.c] `set_user_nice()` 的有效逻辑全部注释，调用表现为无操作。 |
| `sched.scheduler` | `partial` / 2 / critical | [VERIFIED mykernel/sched/scheduler/scheduler_core.c; mykernel/sched/scheduler/myos_rt.c; mykernel/sched/scheduler/scheduler_api.h] 有 `schedule/__schedule`、抢占入口、架构上下文切换、idle 与按 `time_slice/vruntime` 轮转选择器。 | [INFERRED] 缺少 SMP runqueue 锁、reschedule IPI、负载均衡、迁移同步和完整调度类层次。 | [VERIFIED mykernel/sched/scheduler/scheduler_core.c] 多个 enqueue/dequeue/wakeup 分支无完整返回或锁；任务主要固定 CPU0，远端调度语义未形成。 |
| `sched.misc` | `partial` / 2 / medium | [VERIFIED mykernel/sched/misc/sched_misc_api.h; mykernel/sched/misc/fs_struct.c; mykernel/sched/misc/idle.c] 有 fs root/pwd 复制释放、idle 线程和线程辅助。 | [INFERRED] 缺少 idle governor、CPU hotplug 协调及成熟 fs_struct 并发引用。 | [VERIFIED mykernel/sched/misc/fs_struct.c; mykernel/sched/misc/idle.c] 正确性直接依赖尚不完整的 VFS 引用与 SMP 调度路径。 |

## 6. 锁、IPC 与异步执行

| ID | 状态 / 成熟度 / 风险 | 已实现能力与证据 | 明显缺失 | 静态风险 |
|---|---|---|---|---|
| `lock.atomic` | `implemented` / 3 / medium | [VERIFIED mykernel/lock_IPC/atomic/atomic_api.h; mykernel/lock_IPC/atomic/atomic.h; mykernel/arch/x86_64/include/asm/atomic.h] 提供 32/64 位原子、`cmpxchg/xchg`、位操作、atomic_long 和 refcount 基础。 | [INFERRED] 缺少非 x86 后端、系统化内存序测试和 refcount 饱和诊断。 | [VERIFIED mykernel/arch/x86_64/include/asm/atomic.h] 当前正确性绑定 x86 指令语义，跨架构不可复用。 |
| `lock.spinlock` | `partial` / 2 / high | [VERIFIED mykernel/lock_IPC/spinlock/spinlock_smp.h; mykernel/arch/x86_64/include/asm/spinlock.h] 有普通、irq、irqsave 和抢占配合的自旋锁路径。 | [INFERRED] 缺少 lockdep、完整 trylock、可信 BH/softirq 语义及公平性。 | [VERIFIED mykernel/lock_IPC/spinlock/spinlock_smp.h] BH 变体未真正控制可执行 softirq，且全局并发正确性受 softirq/SMP 未完成影响。 |
| `lock.semaphore` | `partial` / 1 / high | [VERIFIED mykernel/lock_IPC/semaphore/semaphore.c; mykernel/lock_IPC/semaphore/semaphore.h] 普通计数信号量有等待队列、阻塞、超时和唤醒主路径。 | [INFERRED] `rwsem`、公平性、优先级继承及完整中断语义未形成。 | [VERIFIED mykernel/lock_IPC/semaphore/rwsem.c] 读写信号量核心函数体被注释，部分非 void 路径无返回值。 |
| `lock.futex` | `missing` / 0 / critical | [VERIFIED mykernel/lock_IPC/futex/futex.c] `do_futex()` 入口立即无限循环。 | [INFERRED] 未见 WAIT/WAKE/REQUEUE、robust list 或 PI futex。 | [VERIFIED mykernel/lock_IPC/futex/futex.c] 任何到达该入口的用户调用都会挂住当前执行流。 |
| `lock.lockdep` | `missing` / 0 / high | [VERIFIED mykernel/lock_IPC/lockdep/lockdep.c; mykernel/lock_IPC/lockdep/lockdep.h] 所读实现为占位/空桩。 | [INFERRED] 未见锁依赖图、顺序检查、IRQ 上下文验证和递归/死锁报告。 | [VERIFIED mykernel/lock_IPC/lockdep/] 锁误用不会由该子系统检测。 |
| `ipc.signal` | `partial` / 2 / critical | [VERIFIED mykernel/lock_IPC/signal/signal.c; mykernel/lock_IPC/signal/signal_api.h; mykernel/lock_IPC/syscall.c; mykernel/arch/x86_64/kernel/signal.c] 有 pending bit/队列、`sigaction`、信号掩码、正 PID 发送、`get_signal` 和 x86 用户信号帧。 | [INFERRED] 缺少权限检查、进程组/全局广播、可靠目标唤醒、线程组退出、RT 排队顺序和 stop/continue 语义。 | [VERIFIED mykernel/lock_IPC/signal/signal.c; mykernel/lock_IPC/syscall.c] 入队后未调用 `complete_signal()`；`pid<=0` 的 kill 路径自旋；致命默认动作未进入真实 group exit。 |
| `kactive.softirq` | `partial` / 1 / high | [VERIFIED mykernel/kactive/softirq/softirq.c] 可设置 pending 位并有唤醒辅助。 | [INFERRED] 缺少 handler 注册、`do_softirq` 执行循环、预算、tasklet 和 ksoftirqd。 | [VERIFIED mykernel/kactive/softirq/softirq.c] 已读路径未见 pending 位消费者，延期工作可能永久不执行。 |
| `kactive.workqueue` | `partial` / 2 / high | [VERIFIED mykernel/kactive/workqueue/workqueue.c; mykernel/kactive/workqueue/workqueue_api.h] 有 per-CPU/BH worker pool、workqueue 分配、排队、kworker 创建和回调执行。 | [INFERRED] 缺少 pending 去重、flush/cancel/destroy、CPU hotplug、完整 unbound pool 与 rescuer。 | [VERIFIED mykernel/kactive/workqueue/workqueue.c] pending test/set 被注释，重复排队可破坏链；BH worker 自旋，unbound 错误路径可能返回未初始化值。 |
| `kactive.swait` | `partial` / 2 / medium | [VERIFIED mykernel/kactive/swait/swait.c; mykernel/kactive/swait/swait.h] 有简单等待队列初始化、prepare/finish、单个与全部唤醒。 | [INFERRED] 缺少完整条件等待宏族、独占/优先级策略和独立超时集成。 | [VERIFIED mykernel/kactive/swait/swait.c] 正确性依赖尚不完整的任务状态与调度锁语义。 |
| `kactive.completion` | `partial` / 2 / medium | [VERIFIED mykernel/kactive/completion/completion.c; mykernel/kactive/completion/completion.h] 在 swait 上实现计数式 `complete`、阻塞等待及信号/超时通用内核。 | [INFERRED] 缺少完整公开 API、可靠 `complete_all/reinit` 组合和成熟超时测试。 | [VERIFIED mykernel/kactive/completion/completion.c] 超时与唤醒行为继承 timer/scheduler 的未闭合路径。 |

## 7. 时间

| ID | 状态 / 成熟度 / 风险 | 已实现能力与证据 | 明显缺失 | 静态风险 |
|---|---|---|---|---|
| `time.systick` | `partial` / 2 / high | [VERIFIED mykernel/time/systick/systick.c; mykernel/time/systick/systick_api.h; mykernel/arch/x86_64/kernel/hpet.c] 有全局 jiffies、低评级 jiffies clocksource，HPET legacy 周期中断会调用 `do_timer(1)`。 | [INFERRED] 缺少 per-CPU tick、NO_HZ、broadcast tick 和 clockevent 抽象。 | [VERIFIED mykernel/arch/x86_64/kernel/hpet.c; mykernel/time/timekeeping/timekeeping.c] HPET handler 先自增 `jiffies` 又调用会递增 jiffies 的 `do_timer()`，存在每 tick 双推进风险。 |
| `time.ktime` | `implemented` / 3 / low | [VERIFIED mykernel/time/ktime/ktime.h] 提供 64 位纳秒标量、加减、比较和 timespec/timeval 转换。 | [INFERRED] 主要缺少系统化边界测试和架构优化，不缺核心语义。 | [VERIFIED mykernel/time/ktime/ktime.h] 风险集中于调用者溢出/单位使用，子系统本身结构简单。 |
| `time.clocksource` | `partial` / 2 / high | [VERIFIED mykernel/time/clocksource/clocksource.c; mykernel/time/clocksource/clocksource_api.h; mykernel/arch/x86_64/kernel/tsc.c; mykernel/arch/x86_64/kernel/hpet.c] 有 mult/shift 计算、按 rating 注册/排序、选择与 unstable 标记，并有 HPET/TSC 候选。 | [INFERRED] 缺少 watchdog、运行期切换锁、跨 CPU TSC 同步和成熟 suspended/resume 语义。 | [VERIFIED mykernel/arch/x86_64/kernel/tsc.c; mykernel/time/clocksource/clocksource.c] TSC 校准失败硬退 3 GHz，随后正式初始化又标记不稳定；选择/切换锁与 watchdog 大量关闭。 |
| `time.timekeeping` | `partial` / 2 / high | [VERIFIED mykernel/time/timekeeping/timekeeping.c; mykernel/time/timekeeping/timekeeping_api.h] 有 RTC 初始墙钟、默认 clocksource 绑定、cycle→ns、`ktime_get`/realtime 读取和 `do_timer`。 | [INFERRED] 缺少完整 seqcount/锁、连续 wall/monotonic 推进、NTP 校正、suspend/resume 与 leap second。 | [VERIFIED mykernel/time/timekeeping/timekeeping.c] 读写一致性和长期时间推进逻辑不完整，多 CPU 并发读取或长期运行可能产生不连续/陈旧时间。 |
| `time.timer` | `partial` / 1 / critical | [VERIFIED mykernel/time/timer/timer.c; mykernel/time/timer/hrtimer.c; mykernel/time/timer/posix-timers.c; mykernel/time/syscall.c] 有 timer wheel 插入/修改/删除、timeout 建立、有限 POSIX realtime clock 读取和安全加法辅助。 | [INFERRED] 已读路径未定位到期扫描/回调执行；缺少可用 hrtimer、非 realtime POSIX clocks 和可靠 sleep。 | [VERIFIED mykernel/time/timer/timer.c] `schedule_timeout()` 中真正的 `schedule()` 被注释，`msleep` 等调用可能忙等或永久不返回。 |
| `time.adjtime` | `missing` / 0 / medium | [VERIFIED mykernel/time/adjtime/adjtime.c; mykernel/time/adjtime/adjtime_api.h] 只有空实现壳和接口占位。 | [INFERRED] 未见 NTP PLL、`adjtimex`、频率/相位校正和 leap 状态。 | [VERIFIED mykernel/time/adjtime/adjtime.c] 墙钟无法通过该层校正。 |

## 8. 文件系统、块层与驱动

| ID | 状态 / 成熟度 / 风险 | 已实现能力与证据 | 明显缺失 | 静态风险 |
|---|---|---|---|---|
| `fs.vfs` | `partial` / 2 / critical | [VERIFIED mykernel/fs/vfs/open.c; mykernel/fs/vfs/read_write.c; mykernel/fs/vfs/namei.c; mykernel/fs/vfs/namespace.c; mykernel/fs/vfs/dcache.c; mykernel/fs/vfs/inode.c; mykernel/fs/vfs/binfmt_elf.c] 有 filesystem 注册、fd/open/read/write/lseek/fcntl、路径与挂载遍历、create/mkdir/unlink、dcache/inode、getdents、rootfs/shmem 和 ELF 装载框架。 | [INFERRED] 缺少 `mount_bdev`、完整 symlink/权限、引用生命周期、page cache/writeback、可靠 `O_CREAT` 和成熟 mount namespace。 | [VERIFIED mykernel/fs/vfs/open.c; mykernel/fs/vfs/dcache.c; mykernel/fs/vfs/inode.c; mykernel/fs/vfs/binfmt_elf.c] close 直接 `kfree(file)`，`dput/iput` 为空；多函数缺返回；ELF 栈 VMA 变量在未初始化状态下使用。 |
| `fs.fat` | `partial` / 2 / critical | [VERIFIED mykernel/fs/fat/myos_fat32.c; mykernel/fs/fat/myos_fat32_namei.c; mykernel/fs/fat/myos_fat32_iobuf.c; mykernel/fs/fat/inode.c] 有 FAT32 表/簇链、文件读写与 seek、VFAT 目录枚举/查找、create/mkdir/rmdir/unlink，并可从 GPT 分区构造根 superblock。 | [INFERRED] 缺少 rename、fsck、崩溃一致性、并发元数据锁和稳健缓存/写回。 | [VERIFIED mykernel/fs/fat/myos_fat32.c; mykernel/fs/fat/myos_fat32_iobuf.c] 新文件写入使用未初始化 sector；非首簇仍访问 `clusters[0]`；扩容可多分配且链连接不可靠，dirty/delete 标志不一致。 |
| `block` | `partial` / 1 / critical | [VERIFIED mykernel/block/bdev.c; mykernel/block/genhd.c; mykernel/block/fops.c; mykernel/block/partitions/efi.c] 有 bdev pseudo-fs、gendisk、分区扫描/GPT 和文件操作切换骨架。 | [INFERRED] 缺少通用 read/write、bio/request queue、设备 claim、完整引用与缓存一致性。 | [VERIFIED mykernel/block/bdev.c; mykernel/block/genhd.c; mykernel/block/fops.c] 分区 open 固定 `-ENXIO`，按路径打开为空；`__alloc_disk_node()` 成功分配后仍释放并返回 NULL；close 不释放 bdev 引用。 |
| `device` | `missing` / 0 / high | [VERIFIED mykernel/device/CMakeLists.txt] 顶层 `device` 目录只有空构建占位；实际局部设备模型位于 `drivers.base`。 | [INFERRED] 该 ID 下未见独立 device core、bus、class、probe/remove 或 sysfs 生命周期。 | [VERIFIED mykernel/device/CMakeLists.txt] 以该子系统 ID 为边界当前不可用。 |
| `drivers.ata` | `partial` / 2 / critical | [VERIFIED mykernel/drivers/ata/myos_libata.c; mykernel/drivers/ata/libata-core.c; mykernel/drivers/ata/libata-sff.c] 有 legacy PIO 端口探测、IDENTIFY/LBA 容量解析、扇区读写以及 gendisk/devtmpfs 注册。 | [INFERRED] 缺少 DMA/NCQ/AHCI、错误恢复、热插拔和成熟 block queue 集成。 | [VERIFIED mykernel/drivers/ata/myos_libata.c] read/write 不返回正确字节数、不推进位置并泄漏临时缓冲/bdev 引用；错误码被忽略，探测命令序列可疑。 |
| `drivers.base` | `partial` / 1 / critical | [VERIFIED mykernel/drivers/base/base_init.c; mykernel/drivers/base/core.c; mykernel/drivers/base/devtmpfs.c; mykernel/drivers/base/class.c] 有 devtmpfs、device/class 结构和设备号映射的局部实现。 | [INFERRED] 缺少 bus/class/firmware/platform 的完整初始化、驱动匹配/probe/remove、uevent/sysfs 和可靠引用模型。 | [VERIFIED mykernel/drivers/base/core.c; mykernel/drivers/base/devtmpfs.c] `device_add()` 可解引用空 parent 且保留 `-EINVAL` 返回；devtmpfs 请求有完成后释放再读取及释放栈对象路径。 |
| `drivers.char` | `partial` / 1 / critical | [VERIFIED mykernel/drivers/char/char_dev.c; mykernel/drivers/char/mem.c; mykernel/drivers/char/myos_tty.c] 有 cdev 映射/open 分派、null/zero 类设备和 framebuffer+键盘 TTY。 | [INFERRED] 缺少 tty core、line discipline、termios、pty、session/controlling tty。 | [VERIFIED mykernel/drivers/char/char_dev.c; mykernel/drivers/char/mem.c; mykernel/drivers/char/myos_tty.c] cdev 注册保存循环内栈副本地址；`/dev/zero` 直接 memset 用户指针；TTY open 返回 1，read 忙轮询并忽略 copy 失败。 |
| `drivers.pci` | `partial` / 1 / high | [VERIFIED mykernel/drivers/pci/probe.c; mykernel/drivers/pci/access.c; mykernel/arch/x86_64/pci/direct.c] 有配置空间访问、vendor/class/header/subsystem 读取和总线扫描骨架。 | [INFERRED] 缺少多 function/bridge 拓扑、BAR 资源、IRQ/MSI/MSI-X、结果持久化、设备注册和驱动匹配。 | [VERIFIED mykernel/drivers/pci/probe.c; mykernel/drivers/pci/access.c] 扫描未编码 function、未先过滤不存在设备，使用栈上 pci_dev 且不注册；访问锁/校验基本被注释。 |
| `drivers.rtc` | `partial` / 2 / medium | [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c; mykernel/drivers/rtc/lib.c] 可读取 MC146818 CMOS，并完成 BCD/日历与 Unix 时间转换。 | [INFERRED] 缺少 UIP 稳定读取、binary/BCD 与 12/24h 模式处理、set/alarm/IRQ 和 RTC class。 | [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c] 跨秒读取及模式假设可能产生错误墙钟。 |

## 9. 通用库

| ID | 状态 / 成熟度 / 风险 | 已实现能力与证据 | 明显缺失 | 静态风险 |
|---|---|---|---|---|
| `lib.digit` | `implemented` / 3 / low | [VERIFIED mykernel/lib/digit/kstrtox.c; mykernel/lib/digit/hexdump.c] 有整数字符串转换、进制自动识别、溢出检查、布尔解析和 hexdump。 | [INFERRED] `from_user` 变体未启用，缺少系统化测试。 | [VERIFIED mykernel/lib/digit/kstrtox.c] 核心转换逻辑完整，主要风险来自调用者未检查返回码。 |
| `lib.idr` | `partial` / 1 / high | [VERIFIED mykernel/lib/idr/idr.c; mykernel/lib/idr/idr.h] 有固定数组版 alloc/cyclic/find/remove/replace。 | [INFERRED] 缺少有效 `for_each/get_next`、动态扩容、严格 range/max、RCU 和内部锁。 | [VERIFIED mykernel/lib/idr/idr.c] 分配忽略部分 max 语义，find/remove 直接按未校验 ID 下标访问；迭代函数为空。 |
| `lib.list` | `partial` / 2 / high | [VERIFIED mykernel/lib/list/double_list.h; mykernel/lib/list/double_list_macro.h; mykernel/lib/list/hash_list.h; mykernel/lib/list/rcu_list.h] 有双向链表、hlist、遍历宏和部分 RCU-list 接口，主体以内联/宏实现。 | [INFERRED] 缺少完整 RCU grace-period 语义以及若干 replace/swap/careful 操作。 | [VERIFIED mykernel/lib/list/double_list.h] 结构校验失败时直接无限循环；并发安全完全依赖外部锁/RCU，而后者未成熟。 |
| `lib.printf` | `partial` / 2 / medium | [VERIFIED mykernel/lib/printf/vsprintf.c; mykernel/lib/printf/kasprintf.c] 有较完整 `vsnprintf/vsprintf` 格式化和 `kvasprintf/kasprintf` 动态分配。 | [INFERRED] 缺少 fortify、广泛格式测试和完整 const 优化路径。 | [VERIFIED mykernel/lib/printf/kasprintf.c] `kvasprintf_const()` 主体被注释且无返回值，调用结果不确定。 |
| `lib.string` | `implemented` / 3 / low | [VERIFIED mykernel/lib/string/string.c; mykernel/lib/string/string_helpers.c; mykernel/lib/string/strnlen_user.c] 有常用字符串/内存拷贝、比较、搜索、`strscpy`、辅助格式和用户字符串长度实现。 | [INFERRED] 缺少完整 fortify、架构优化和部分可选 API。 | [VERIFIED mykernel/lib/string/string.c] 传统 `strcpy/strcat` 等不带边界的接口仍依赖调用者保证缓冲区容量。 |

## 10. 跨子系统风险簇

1. **生命周期与引用闭环不足。** [VERIFIED mykernel/fs/vfs/open.c; mykernel/fs/vfs/dcache.c; mykernel/fs/vfs/inode.c; mykernel/sched/forkexec/exit.c; mykernel/block/fops.c; mykernel/drivers/base/devtmpfs.c] file/path/inode/dentry/mm/bdev/device 请求的获取、发布与失败回滚不一致，包含直接 `kfree`、空 `dput/iput`、退出不释放 mm、完成后使用已释放对象等路径。
2. **SMP 与锁语义未闭合。** [VERIFIED mykernel/arch/x86_64/smp/smpboot.c; mykernel/sched/scheduler/scheduler_core.c; mykernel/printk/printk.c; mykernel/time/clocksource/clocksource.c] AP 尚未上线，runqueue/logbuf/clocksource/VFS 等关键锁大量关闭；当前多数“可运行”判断只能代表单 BSP 基本路径。
3. **不支持路径常以无限循环表示。** [VERIFIED mykernel/lock_IPC/futex/futex.c; mykernel/mm/fault/fault.c; mykernel/lock_IPC/syscall.c; mykernel/arch/x86_64/entry/common.c] futex、共享写缺页、部分 signal/kill 和未知 syscall 会挂住执行流，而不是返回受控错误。
4. **定时与异步执行链不完整。** [VERIFIED mykernel/time/timer/timer.c; mykernel/kactive/softirq/softirq.c; mykernel/kactive/workqueue/workqueue.c] timer 到期执行未定位、`schedule_timeout` 不调度、softirq 无执行循环、workqueue 去重/销毁不完整；多个等待与延迟 API 因此不能单独视为可靠。
5. **用户地址访问边界不统一。** [VERIFIED mykernel/drivers/char/mem.c; mykernel/drivers/char/myos_tty.c; mykernel/fs/fat/myos_fat32.c] 有直接访问用户指针、忽略 `copy_to/from_user` 返回值或不完整检查的路径，错误用户地址可能升级为内核故障或静默数据损坏。
6. **启动与用户态集成未由源码构建闭环证明。** [VERIFIED myloader/CMakeLists.txt; myloader/uefi/bootloader.c; myinitramfs/CMakeLists.txt; mykernel/init/do_mounts.c] loader 顶层只安装预构建 EFI，initramfs 构建依赖本机路径且内核解包链被关闭；静态存在的各段不能直接等同于可复现端到端启动。

## 11. 未决问题

- UEFI 预构建二进制是否由当前 `myloader/uefi/` 源码生成，静态仓库内容不足以确认。
- PID 1、`/boot/sh`、外部 GPT/FAT 根盘和内核 exec 路径是否在该提交实际共同启动，需构建/镜像/运行证据。
- timer wheel 到期扫描是否经未读链接入口间接调用，需完整链接图或运行追踪确认。
- 本矩阵没有执行并发、故障注入或持久化一致性测试；风险项是源码可见路径，不是已复现缺陷清单。
