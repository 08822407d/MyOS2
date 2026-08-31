---
task_id: MYOS2-DR-003
produced_by: ChatGPT Pro / GPT-5.6 Pro
date: '2026-09-01'
generated_at: '2026-09-01T00:56:24+08:00'
agent_id: openai-gpt-5.6-pro
base_snapshot: workspace master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719; kernel source time @ a039d9803ade2a1613d620bda375e028530d5242
inputs_read:
- agent-workspace/conventions.md
- agent-workspace/repo-map.md
- agent-workspace/tasks/MYOS2-DR-003-dependency-graph.md
- mykernel/arch/x86_64/kernel.lds
- mykernel/arch/x86_64/kernel/head_64.S
- mykernel/arch/x86_64/kernel/head64.c
- mykernel/arch/x86_64/myos/pre_init.c
- mykernel/arch/x86_64/kernel/setup.c
- mykernel/arch/x86_64/smp/setup_percpu.c
- mykernel/arch/x86_64/kernel/traps.c
- mykernel/mm/misc/mm_init.c
- mykernel/sched/scheduler/scheduler_core.c
- mykernel/arch/x86_64/kernel/irqinit.c
- mykernel/arch/x86_64/kernel/time.c
- mykernel/init/main.c
- mykernel/fs/vfs/init.c
- mykernel/fs/vfs/myos_vfs.c
- mykernel/arch/x86_64/entry/entry_64.S
- mykernel/arch/x86_64/entry/common.c
- mykernel/entry/common.c
- mykernel/arch/x86_64/myos/arch_task.c
content_type: initialization_sequence_markdown
source_scope: 从 x86_64 物理入口到 boot idle 与 /boot/init 的实际主链
status: final
analysis_coverage: static_source_only
limitations:
- 仅对 time 快照做静态源码分析，未在连接器环境中执行干净构建、链接、启动或设备 I/O 实测。
- 递归 GLOB 会把大量源码加入 kernel 目标；本文区分“进入构建输入”与“运行时实际可达”，但未生成链接 map 验证 --gc-sections 的最终保留集合。
- 动态注册、函数指针、宏展开与条件编译的闭包仅覆盖启动主链和关键功能链，不能视为逐符号完备图。
- 部分 Linux 通用框架代码被注释或保留但未进入当前调用路径；未把“源码存在”自动视为“当前生效”。
open_questions:
- 当前正式发布/测试使用的 CMake 构建类型、ARCH 参数与根块设备选择宏是什么？
- ROOTBLK_TRANSFER 在目标机器上最终绑定 ATA 还是 NVMe，是否存在稳定的替代根文件系统配置？
- kernel_init 中手工 do_initcalls() 与链接器 initcall section 机制未来是否计划并存，还是前者为临时过渡？
- workqueue_init()、sched_init_smp() 等被注释路径是有意禁用、尚未移植完成，还是由其他私有路径替代？
- 目标平台是否保证 HPET；若不保证，PIT/LAPIC/TSC-deadline 的降级策略在哪里完成？
---

# MyOS2 实际初始化时序

## 读法与结论边界

- `[VERIFIED path:line]`：在 `time @ a039d9803ade2a1613d620bda375e028530d5242` 中核实到的调用、定义或顺序。
- `[INFERRED]`：由调用语义或内核通用约束推出，源码未直接声明“必须”。
- 本文说“先于”时区分两种含义：**硬前置**表示后一步在当前实现中不能脱离前一步；**观测顺序**只表示源码按此顺序调用，未证明不可交换。
- `deps.yaml` 的 `init_order` 边方向为“后执行者 → 必须先完成者”；本文按时间正序叙述。

## 一句话总链

[VERIFIED] `phys_startup_64 / startup_64` → `x86_64_start_kernel()` → `myos_early_init_system()` → `start_kernel()` → `rest_init()` → `kernel_init`/`kthreadd` 分叉 → `driver_init()` 与手工 `do_initcalls()` → ATA/NVMe/XHCI 请求线程与 PCI/存储初始化 → `myos_switch_to_root_disk()` → `kernel_execve("/boot/init")`；boot task 另一支在完成首次调度后进入 `cpu_startup_entry()`。

## 阶段 0：加载器交接与链接入口

1. [VERIFIED `mykernel/arch/x86_64/kernel.lds:1-7`] 链接脚本把 ELF 入口设为 `phys_startup_64`，并把它定义为 `startup_64` 相对高半区基址的物理入口。
2. [VERIFIED `mykernel/arch/x86_64/kernel/head_64.S:24-64`] `startup_64` 所在启动段同时带有 Multiboot2 头；当前编译配置还定义 `GRUB2_BOOTUP_SUPPORT`。[VERIFIED `mykernel/scripts/options_flags.cmake:52-79`]
3. [INFERRED] 外部加载器把控制权交给 `phys_startup_64`。本任务没有把 `myloader`、GRUB 配置和镜像制作链闭包为一个唯一启动方案，因此“究竟由哪个加载器进入”保留为开放问题。

**可用性里程碑：**CPU 已在 64 位启动路径，尚不能使用常规内存分配、调度或设备服务。

## 阶段 1：汇编早期启动

1. [VERIFIED `mykernel/arch/x86_64/kernel/head_64.S:24-143`] `startup_64` 关闭中断、保存 Multiboot 信息、建立临时栈和早期 GDT/IDT，调用 `__startup_64()` 修正页表并切换 CR3。
2. [VERIFIED `mykernel/arch/x86_64/kernel/head_64.S:220-296`] `common_startup_64` 配置 GSBASE、EFER.SCE/NX、CR0 与 init stack，随后通过 `initial_code` 调用 `x86_64_start_kernel()`。
3. [VERIFIED `mykernel/arch/x86_64/kernel/head_64.S:18-24`] 汇编入口包含 `generated/asm-offsets.h`，因此构建时 `asm-offset` 目标是这条启动链的硬前置。

**硬前置：**链接布局、生成的 asm offsets、早期页表和 init stack。

## 阶段 2：C 早期入口与页表交接

`x86_64_start_kernel()` 的有效顺序如下：

1. [VERIFIED `mykernel/arch/x86_64/kernel/head64.c:145-177`] 清空 BSS。
2. [VERIFIED] 清空 `init_top_pgt`，安装早期 IDT handler。
3. [VERIFIED] 调用 `myos_early_init_system()`；该函数把加载器/早期页表中的内核映射复制到正式顶级页表，并设置 `init_mm.pgd`。[VERIFIED `mykernel/arch/x86_64/myos/pre_init.c:1-45`]
4. [VERIFIED] 把高半区入口写入 `init_top_pgt[511]`。
5. [VERIFIED] 调用 `start_kernel()`。

**硬前置：**在进入通用初始化前，BSS、早期异常入口和高半区页表必须处于一致状态。

## 阶段 3：`start_kernel()` 主序列

下表严格按源码调用顺序排列。[VERIFIED `mykernel/init/main.c:112-177`]

| 序号 | 节点 ID / 调用 | 形成的最低能力 | 前置性质 |
|---:|---|---|---|
| 1 | `mm.early.setup_arch` / `setup_arch()` | 解析启动内存信息、memblock、直接映射、zone 边界 | 硬前置 |
| 2 | `cpu.percpu_areas` / `setup_per_cpu_areas()` | boot CPU 的正式 per-CPU 区与偏移 | 硬前置 |
| 3 | `entry.trap_init` / `trap_init()` | 异常门与 IDT | 硬前置 |
| 4 | `mm.page_alloc.mm_core_init` / `mm_core_init()` | 页分配器、slab/kmalloc、MM caches | 硬前置 |
| 5 | `sched.scheduler.sched_init` / `sched_init()` | boot runqueue、idle task、基础调度 | 硬前置 |
| 6 | `entry.irq_init` / `init_IRQ()` | IRQ/vector 基础 | 硬前置 |
| 7 | `kactive.softirq.init` / `softirq_init()` | 软中断基础 | 调用点核实；注册闭包未完备 |
| 8 | `time.timekeeping.init` / `timekeeping_init()` | 时间维护核心 | 观测顺序 |
| 9 | `printk.console_init` / `console_init()` | 早期控制台 | 观测顺序；诊断强依赖 |
| 10 | `time.systick.late_time_init` / `late_time_init()` | HPET 周期源与 TSC 初始化 | 当前实现硬前置 |
| 11 | `cpu.arch_finalize` / `arch_cpu_finalize_init()` | CPU 架构最终化 | 观测顺序 |
| 12 | `lib.idr.pid_init` / `pid_idr_init()` | PID 分配基础 | 创建任务前硬前置 |
| 13 | `device.core_init` / `myos_devices_init()` | MyOS 设备基础 | 当前设备链前置 |
| 14 | `sched.task.proc_caches_init` / `proc_caches_init()` | 进程/任务缓存 | 创建新任务前硬前置 |
| 15 | `fs.vfs.caches_init` / `vfs_caches_init()` | VFS 缓存与初始目录树 | 根切换前硬前置 |
| 16 | `ipc.signal.init` / `signals_init()` | 信号基础 | 用户态前置 |
| 17 | `debug.cpu_bug_check` / `check_bugs()` | CPU 缺陷检查 | 诊断/安全前置，语义可裁性待验证 |
| 18 | `entry.percpu_interrupt_init` / `myos_init_percpu_intr()` | per-CPU 中断路径 | 任务/设备运行前硬前置 |
| 19 | `init.rest_init` / `rest_init()` | 进入多任务阶段 | 上述基础设施的汇合点 |

### 三个关键硬约束

- [VERIFIED `mykernel/init/main.c:126-141`] 源码注释明确要求 `sched_init()` 先于任何可能触发调度的中断。
- [VERIFIED `mykernel/mm/misc/mm_init.c:430-650`] `mm_core_init()` 依次让页分配、对象分配和 MM 缓存可用；这些又依赖 `setup_arch()` 建好的早期内存状态。
- [VERIFIED `mykernel/arch/x86_64/kernel/time.c:29-59`] 当前 `late_time_init()` 直接走 `hpet_time_init()`，注释写明假定平台必定支持 HPET，然后再 `tsc_init()`；本快照中未见这里的 PIT/LAPIC 降级选择。

## 阶段 4：`rest_init()` 的双分支

[VERIFIED `mykernel/init/main.c:76-123`]

1. 先创建 `kernel_init` 内核线程，使其取得 PID 1；该线程会阻塞等待 `kthreadd_done`。
2. 再创建 `kthreadd`，保存 `kthreadd_task`。
3. `complete(&kthreadd_done)` 释放 PID 1。
4. boot task 调用 `schedule_preempt_disabled()`，至少发生一次真实调度，然后进入 `cpu_startup_entry()`。

这产生两个并行里程碑：

- **idle 分支：**boot CPU 已进入调度/idle 循环。
- **PID 1 分支：**只有 `kthreadd` 完成建立后，`kernel_init` 才继续做可能需要 kthread 的初始化。

[VERIFIED] 当前源码未在这条路径调用 `sched_init_smp()`；对应 Linux 通用调用仍被注释。[VERIFIED `mykernel/init/main.c:300-332`] 因此不能把“完整 SMP 拓扑/负载均衡已按通用路径启动”当作已证事实。

## 阶段 5：PID 1 的基础设施与手工 initcall

`kernel_init()` 等待 completion 后，执行：

1. [VERIFIED `mykernel/init/main.c:258-279`] `do_name()` 创建 `dev`、`root`、`.`、`kernel`、`conf`、`etc`、`usr` 等初始目录；实际定义见 [VERIFIED `mykernel/fs/vfs/init.c:116-126`]。
2. [VERIFIED `mykernel/init/main.c:300-338`] `kernel_init_freeable()`：
   - `do_pre_smp_initcalls()` 当前只调用 `init_zero_pfn()`；
   - `do_basic_setup()` 先调用 `driver_init()`，再调用本文件中的手工 `do_initcalls()`。
3. [VERIFIED `mykernel/init/main.c:193-237`] 手工 `do_initcalls()` 的实际固定顺序：
   1. `init_sigframe_size()`
   2. `init_elf_binfmt()`
   3. `tty_class_init()`
   4. `clocksource_done_booting()`
   5. `chr_dev_init()`
   6. `pid_namespaces_init()`
   7. `register_diskfs()`
4. [VERIFIED `mykernel/fs/vfs/myos_vfs.c:55-66`] `register_diskfs()` 当前直接调用 `init_fat32_fs()`；`init_vfat_fs()` 被注释。

### 注册机制判断

- [VERIFIED] 当前真正执行的是 `main.c` 中的**手工函数列表**。
- [VERIFIED `mykernel/arch/x86_64/kernel.lds:92-165`] 链接脚本保留了一大段 Linux initcall section 布局文本，但该段整体位于注释中。
- [VERIFIED `mykernel/init/main.c:300-360`] 通用 `workqueue_init()`、`smp_init()`、`sched_init_smp()`、`wait_for_initramfs()`、`prepare_namespace()` 等调用也被注释。
- [INFERRED] 因而当前 initcall 机制更接近“手工白名单”，不是已启用的完整 section 扫描器；是否准备迁回通用机制属于开放问题。

## 阶段 6：设备线程、扫描与根文件系统

[VERIFIED `mykernel/init/main.c:276-293`] `kernel_init_freeable()` 返回后按固定顺序执行：

1. `init_NVMErqd()`
2. `init_ATArqd()`
3. `init_XHCIrqd()`
4. `scan_PCI_devices()`
5. 打开 `DEBUG_show_jiffies`
6. `myos_ata_probe()`
7. `NVMe_IOqueue_init()`
8. `myos_switch_to_root_disk()`
9. `kjmp_to_doexecve()`

这里的顺序是已核实的事实，但“NVMe 与 ATA 两套路径是否都必须成功”并未由该调用顺序证明。它们可能是并行支持的后端，也可能存在临时无条件初始化。

### 当前根切换的具体约束

[VERIFIED `mykernel/fs/vfs/myos_vfs.c:69-154`]

- 通过 `ROOTBLK_TRANSFER` 读取 MBR/GPT。
- MBR 第一个分区项必须是 GPT protective/EFI 类型，否则进入死循环。
- 扫描 GPT entries；对 EFI system partition GUID 读取引导扇区。
- 当分区索引等于 `BOOT_FS_IDX`（当前为 0）时，以 `"FAT32"` 调用 `mount_fs()`。
- 建立 `myos_root_mnt`，把 PID 1 的 cwd/root 指向新根。
- 随后执行 `init_mount("devtmpfs", "/dev", "devtmpfs", MS_SILENT)`。

因此，在**当前启动配置**下，`fs.fat + fs.vfs + block + 选定根块设备后端 + kmalloc` 形成根文件系统硬依赖链。只有先实现另一套可启动根方案，FAT32 才能从“当前硬核”降为可选文件系统。

## 阶段 7：首个用户进程

[VERIFIED `mykernel/arch/x86_64/myos/arch_task.c:7-45`]

1. `kjmp_to_doexecve()` 清除 PID 1 的 `PF_KTHREAD`。
2. 固定选择 `initd_name = "/boot/init"`。
3. 把 `current->mm` 设置为 `active_mm` 后调用 `kernel_execve()`。
4. 最后跳到 `syscall_return_via_sysret`，转入用户态。

[VERIFIED `mykernel/arch/x86_64/entry/entry_64.S:130-214`] 用户态后续系统调用从 `entry_SYSCALL_64` 进入，构造 `pt_regs` 并调用 `do_syscall_64()`；[VERIFIED `mykernel/arch/x86_64/entry/common.c:18-82`] 后者通过 `x64_sys_call()` 分派，并在退出时执行用户态返回检查。

**“内核可用”判据：**本文把 `cpu_startup_entry()` 已运行、根文件系统已建立、ELF binfmt 已注册、`/boot/init` 已经 exec 并且 SYSCALL/SYSRET 路径可用视为最低可用里程碑。该判据是任务用途上的定义，不代表已通过实机验证。

## 必须前置矩阵

| 目标能力 | 必须先完成 | 证据强度 | 失败表现 |
|---|---|---|---|
| 通用动态分配 | `setup_arch` → memblock/zone → `mm_core_init` | VERIFIED | 任务、VFS、驱动对象无法安全创建 |
| 基础多任务 | per-CPU、`mm_core_init`、`sched_init` | VERIFIED + 少量 INFERRED | PID 1/kthreadd 无法运行 |
| 中断驱动调度 | `sched_init` → `init_IRQ` → per-CPU IRQ | VERIFIED | 中断可能进入未初始化调度状态 |
| 当前周期 tick | IRQ 基础 → HPET init | VERIFIED | 当前实现没有已核实的替代 tick |
| PID 1 继续初始化 | `kthreadd` 建立并 complete | VERIFIED | `kernel_init` 永久等待 |
| 根文件系统 | VFS caches、FAT32 注册、根块 I/O、设备后端 | VERIFIED | GPT/FAT 读取或 mount 失败；当前代码可能停机循环 |
| `/boot/init` | 根文件系统、ELF binfmt、VM/exec、调度 | VERIFIED + INFERRED | exec 失败或无法转入用户态 |
| 可交互用户态 | 上述 + syscall entry/dispatch + 必需设备节点 | VERIFIED + 配置相关 | 用户进程无法继续系统调用或访问设备 |

## 条件、延迟与未生效路径

- [VERIFIED] `scan_XHCI_devices()` 与 `USB_Keyborad_init()` 当前被注释；XHCI request thread 仍无条件创建。
- [VERIFIED] `workqueue_init()`、`sched_init_smp()`、initramfs/`prepare_namespace()` 通用路径被注释。
- [VERIFIED] `sysenter/sysexit` 路径被注释；当前有效路径是 `SYSCALL/SYSRET`。
- [VERIFIED] 大量通用 `exit_to_user_mode_loop` 工作项被注释，保留下来的有效调用主要是 `arch_do_signal_or_restart()`。[VERIFIED `mykernel/entry/common.c:1-77`]
- [INFERRED] 未经实际启动与链接 map，不能证明所有手工初始化函数都被正确保留，也不能证明递归编译进来的所有子系统在运行时可达。
