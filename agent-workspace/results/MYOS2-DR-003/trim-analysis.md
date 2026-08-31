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
- mykernel/CMakeLists.txt
- mykernel/scripts/options_flags.cmake
- mykernel/scripts/target_kbuild.cmake
- mykernel/scripts/target_kernel.cmake
- mykernel/arch/x86_64/kernel.lds
- mykernel/init/main.c
- mykernel/arch/x86_64/kernel/head_64.S
- mykernel/arch/x86_64/kernel/head64.c
- mykernel/arch/x86_64/kernel/setup.c
- mykernel/mm/misc/mm_init.c
- mykernel/sched/scheduler/scheduler_core.c
- mykernel/arch/x86_64/kernel/time.c
- mykernel/fs/vfs/myos_vfs.c
- mykernel/arch/x86_64/myos/arch_task.c
- mykernel/arch/x86_64/entry/entry_64.S
content_type: trim_analysis_markdown
source_scope: 基于构建、初始化和功能依赖图的 menuconfig 粒度与当前/可选核心分析
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

# MyOS2 初步裁剪分析

## 结论摘要

[VERIFIED] 当前构建不是按 feature target 选择源码，而是用 `file(GLOB_RECURSE)` 把 `mykernel/` 下几乎全部 `.c`/`.S` 加入单一 `kernel` 目标，仅排除 `arch/${ARCH}/kbuild`；随后依靠 `-ffunction-sections -fdata-sections` 与链接器 `--gc-sections` 回收未引用 section。[VERIFIED `mykernel/CMakeLists.txt:26-35`; `mykernel/scripts/options_flags.cmake:31-82`]

因此，**第一优先级不是删除目录，而是把“构建输入、配置开关、初始化调用、运行时选择”拆成可验证的 feature 边界**。否则某个子系统即使运行时不可达，仍可能因编译错误、头文件耦合或全局符号而阻断整个构建；反过来，链接器回收了某段代码也不代表其依赖关系已被显式管理。

本文不提供删除命令，也不实现裁剪机制；所有“可选”都带有前置替代方案与验证门槛。

## 分析配置

当前“不可裁剪”结论限定在以下已核实配置：

- 架构：x86_64。
- 入口：`phys_startup_64` / Multiboot2 风格启动路径。
- tick：`late_time_init()` 直接启用 HPET，然后 `tsc_init()`。
- 根：GPT 中 `BOOT_FS_IDX == 0` 的 EFI/FAT32 分区。
- 用户态：固定执行 `/boot/init`。
- 根块 I/O：经 `ROOTBLK_TRANSFER`，实际 ATA/NVMe 绑定未闭包。
- 初始化：`main.c` 手工 `do_initcalls()`，不是已证实启用的完整 linker-section initcall 扫描。

改变任一项，核心集合都可能变化。

## 1. 当前不可裁剪核

### 1.1 构建与入口核

| 节点/边界 | 理由 | 替代条件 | 裁剪风险 |
|---|---|---|---|
| `arch.x86_64.build_profile` | freestanding、固定宏、section 粒度和链接行为由此定义 | 新架构/新 profile 提供等价 flags | 生成不可启动或 ABI 不一致镜像 |
| `arch.x86_64.asm_offsets` | `head_64.S` 与 `entry_64.S` 包含生成头，`kernel` 显式依赖目标 | 改为可靠的另一套 offset 生成机制 | 汇编访问 C 结构偏移错误 |
| `arch.x86_64.linker_layout` | 定义入口、高半区、init/per-CPU/stack 布局 | 新架构链接脚本 | 入口、地址与段边界失效 |
| `init.kernel_target` + `init.kernel_sources` | 当前唯一内核产物和源集合 | 引入真正的 feature targets/object libraries | 构建链断裂 |
| `arch.x86_64.phys_startup_64` → `x86_64_start_kernel` | 当前唯一核实的早期启动链 | 另一套加载器入口必须最终建立同等 CPU/页表状态 | 无法进入 C 内核 |

[VERIFIED `mykernel/scripts/target_kbuild.cmake:1-32`; `mykernel/arch/x86_64/kernel.lds:1-194`; `mykernel/arch/x86_64/kernel/head_64.S:24-296`]

### 1.2 内存核

硬链为：

`mm.early.setup_arch` → `mm.early.memblock` → `mm.page_alloc.buddy_system` → `mm.kmalloc.object_allocator`

[VERIFIED `mykernel/arch/x86_64/kernel/setup.c:120-300`; `mykernel/mm/misc/mm_init.c:430-650`]

当前任务、VFS、GPT/FAT 根切换、驱动对象都使用通用动态分配；没有发现可以把系统维持在纯静态分配状态并仍执行 `/boot/init` 的替代路径。可裁剪的是具体 zone/NUMA/highmem 等 profile，不是这条“早期内存 → 页 → 对象”的骨架。

### 1.3 CPU、异常、中断与调度核

最低链为：

- `cpu.percpu_areas`
- `entry.trap_init`
- `sched.scheduler.sched_init`
- `entry.irq_init`
- `entry.percpu_interrupt_init`
- `sched.task.kernel_init_spawn`
- `sched.task.kthreadd_spawn`
- `sched.task.boot_idle`

[VERIFIED `mykernel/init/main.c:118-171`; `mykernel/init/main.c:76-123`]

源码明确要求调度器先于可能触发调度的中断。PID 1 又显式等待 `kthreadd_done`，boot task 必须至少完成一次 schedule 才进入 idle。对当前“内核 + 用户态”目标，这些不是叶节点。

需要注意：[VERIFIED] `scheduler_core.c` 中大量 Linux 通用调度代码被注释，当前有效行为主要落在 MyOS 自定义实时/优先级队列；因此不能简单按文件规模判定“哪些调度代码可删”，应先做符号可达性和调度场景测试。

### 1.4 当前时间核

`time.timekeeping.init` 与 `time.systick.late_time_init` 位于启动主链；当前 `late_time_init()` 明确假定 HPET，并调用 `myos_HPET_init()` 和 `tsc_init()`。[VERIFIED `mykernel/arch/x86_64/kernel/time.c:29-59`]

在没有实现并验证 PIT、LAPIC timer 或 TSC-deadline 替代路径前，`time.systick.hpet_periodic` 是**当前 profile 的硬核**。未来可把它降为 `TIMER_HPET` 可选实现，但必须引入“至少一个 clockevent/tick provider”约束。

### 1.5 用户态与根文件系统核

当前硬链为：

`drivers/base + 选定存储后端 + block.root_transfer`
→ `fs.vfs`
→ `fs.fat.fat32_root`
→ `fs.vfs.root_mount`
→ `sched.forkexec.elf_binfmt_init`
→ `mm.vm_map`
→ `sched.forkexec.exec_boot_init`
→ `entry.syscall_64`

[VERIFIED `mykernel/init/main.c:193-293`; `mykernel/fs/vfs/myos_vfs.c:55-154`; `mykernel/arch/x86_64/myos/arch_task.c:7-45`; `mykernel/arch/x86_64/entry/entry_64.S:130-214`]

在当前配置下：

- `fs.fat` 不是普通可选叶，因为根就是 FAT32。
- VFS、块 I/O、ELF binfmt、用户 VM、SYSCALL/SYSRET 都不可裁。
- `/boot/init` 的路径是硬编码；除非改成另一首进程位置或纯内核 appliance，否则不能裁掉对应目录/文件系统内容。
- `ROOTBLK_TRANSFER` 的具体后端未闭包，因此不能同时裁掉 ATA 和 NVMe，也不能断言两者都必需。

## 2. 很可能需要，但可通过 profile 降级

| 候选 | 当前证据 | 可替代/降级方案 | 风险 |
|---|---|---|---|
| `printk` / `printk.console_init` | 位于主序列，HPET/根盘失败路径会打印诊断 | 极小 early console 或 ring buffer-only profile | 早期故障变成无观测死机 |
| `ipc.signal` | `signals_init()` 在主链；syscall 退出路径调用 `arch_do_signal_or_restart()` | 极简无信号用户 ABI | `/boot/init` 或 libc 可能隐式依赖 |
| `namespace.pid` | 手工 initcall 无条件执行 | 单 namespace profile | PID/进程语义与未来接口变化 |
| `drivers.char` / TTY | `tty_class_init()`、`chr_dev_init()` 无条件执行 | headless profile，只保留必需字符设备 | init/console/设备节点可能失败 |
| `fs.vfs.devtmpfs_mount` | 根切换后无条件挂载 `/dev` | 静态 `/dev` 或不需要设备节点的 init | 用户态找不到控制台/块/字符设备 |
| `drivers.pci` | `scan_PCI_devices()` 无条件执行 | 固定平台、非 PCI 根后端 | NVMe/XHCI 等设备消失 |
| `device.xhci_request_thread` | 无条件创建，但扫描/键盘调用被注释 | 无 USB profile | 无意义 kthread 与依赖；误裁可能影响控制器路径 |
| `debug.cpu_bug_check` | `check_bugs()` 位于主链 | release profile 只保留必要 CPU quirk | 未应用必要规避导致隐蔽错误 |

这些不应先按目录移除；应先把无条件初始化改成受 profile 控制，并明确调用者接受的降级结果。

## 3. 三个以上可选叶

### 3.1 `drivers.rtc`

- [VERIFIED] 在已追踪的启动到 `/boot/init` 主链中未见 RTC 初始化或读取。
- [INFERRED] 若时间仅需 monotonic/tick，RTC 墙钟可作为独立设备叶。
- 开关粒度建议：`RTC` 总开关 + 具体控制器驱动。
- 验证：无 RTC 时 timekeeping、文件时间戳和用户态启动仍可接受；不得把 HPET clocksource 与 RTC 混为同一功能。

### 3.2 `lock.futex`

- [VERIFIED] 启动主链未直接调用 futex 初始化。
- [INFERRED] 极简、单线程 `/boot/init` 可以不需要 futex；常规 libc、线程库或进程同步很可能需要。
- 开关粒度建议：`FUTEX_SYSCALL`，并让用户 ABI/profile 显式声明依赖。
- 风险：内核仍能启动，但用户程序在创建线程、条件变量或锁时失败；必须做用户态 ABI smoke test。

### 3.3 `arch.aarch64`

- [VERIFIED] 当前默认与实读启动路径均是 x86_64。
- 对单架构镜像，它是天然互斥叶；不是“删掉架构抽象”，而是构建时只选一个 arch provider。
- 开关粒度建议：顶层 `ARCH` choice，而不是在同一目标内混编多架构文件。

### 3.4 `user.initramfs`

- [VERIFIED `mykernel/init/main.c:341-360`] `wait_for_initramfs()`、`prepare_namespace()` 等通用 initramfs 路径被注释；当前从磁盘 FAT32 根执行 `/boot/init`。
- 因而 initramfs 是当前 profile 的可选叶/未生效方案。
- 风险：未来若要无盘启动、早期固件或根解密，它会反过来成为硬依赖。

### 3.5 `kactive.workqueue`

- [VERIFIED `mykernel/init/main.c:300-332`] `workqueue_init()` 在当前 PID 1 路径中被注释。
- [INFERRED] 若没有其他隐藏初始化，通用 workqueue 当前不是“启动到 `/boot/init`”硬核。
- 风险：某些驱动可能排队 work 而没有 worker；在确认所有 `queue_work`/延迟 work 调用者之前只能标为 uncertain-optional，不能直接移除。

### 3.6 调试与额外设备叶

`debug.runtime_diagnostics`、未使用的 USB/XHCI 输入路径、具体 RTC 驱动、未选择的存储后端都适合做叶开关。这里的关键是“未选择的后端”，不是同时关闭所有根设备路径。

## 4. 条件可选、互锁或替代组

### 4.1 ATA 与 NVMe

当前 `kernel_init()` 无条件创建两套请求线程、执行 ATA probe 和 NVMe queue init。[VERIFIED `mykernel/init/main.c:276-287`]

推荐表达为 choice/OR 约束：

- `ROOT_BLOCK_ATA => drivers.ata + block + IRQ`
- `ROOT_BLOCK_NVME => drivers.pci + NVMe request thread + block + IRQ`
- `ROOT_DISK_BOOT => ROOT_BLOCK_ATA || ROOT_BLOCK_NVME || other_verified_backend`

在 `ROOTBLK_TRANSFER` 最终绑定未确认前，两者都属于“不可安全裁剪的候选替代组”，而不是两个确定硬核。

### 4.2 FAT32

- 当前配置：硬核。
- menuconfig 设计：`FS_FAT32` 可以是文件系统叶，但 `ROOT_FS_FAT32` 必须强制选择 `FS_FAT32 + FS_VFS + BLOCK + ROOT_BACKEND`。
- 若引入 ext4、initramfs 或内存根，才可关闭 FAT32。

### 4.3 HPET

建议把“timekeeping 核”与“timer provider”分开：

- `TIMEKEEPING`：核心能力。
- `TIMER_HPET`：当前 provider。
- 未来 `TIMER_LAPIC` / `TIMER_TSC_DEADLINE` / `TIMER_PIT`：替代 provider。
- 约束：`SCHED_PREEMPTIVE => at least one clockevent provider`。

当前源码没有已核实的 fallback，故现阶段关闭 HPET 的风险是直接失去周期 tick。

### 4.4 SMP

per-CPU 基础在 boot CPU 上已使用，因此即使做 UP profile，也不能简单删除全部 per-CPU 机制。可选的是 AP bring-up、CPU hotplug、SMP 调度拓扑和跨 CPU 中断。

[VERIFIED] `sched_init_smp()`/`smp_init()` 在当前 `kernel_init_freeable()` 中被注释，因此本快照的“完整 SMP 可用性”本身需要先澄清，再谈裁剪。

## 5. 建议的 menuconfig 粒度

| 建议开关 | 对应节点 | 依赖规则 |
|---|---|---|
| `ARCH_X86_64` | `arch.x86_64.*` | 顶层架构 choice；选择 linker/entry/per-CPU provider |
| `SMP` | `cpu.*`, `sched.scheduler.*` 的 AP/SMP 部分 | 依赖 per-CPU、IPI、APIC；UP 仍保留 boot per-CPU |
| `TIMER_HPET` | `time.systick.hpet_periodic` | 依赖 IRQ、MMIO；当前默认硬选 |
| `BLOCK_ATA` | `drivers.ata.*` | 依赖 block、IRQ；可作为 root backend |
| `BLOCK_NVME` | `block.nvme_*`, `drivers.pci` | 依赖 PCI、IRQ、DMA/queue |
| `USB_XHCI` | `device.xhci_request_thread` | 依赖 PCI、IRQ；扫描/输入子开关分离 |
| `FS_VFS` | `fs.vfs` | 用户态/rootfs 总开关；当前不可关闭 |
| `FS_FAT32` | `fs.fat` | 依赖 VFS、block；当前 `ROOT_FS_FAT32` 强制选择 |
| `DEVTMPFS` | `fs.vfs.devtmpfs_mount` | 依赖 VFS、driver core |
| `USER_ELF` | `sched.forkexec.elf_binfmt_init` | 依赖 VM map、VFS、调度 |
| `USER_SYSCALL64` | `entry.syscall_64` | 依赖 x86 entry、per-CPU stack/TSS |
| `INITRAMFS` | `user.initramfs` | 与磁盘根可形成替代方案 |
| `FUTEX` | `lock.futex` | 由用户 ABI/profile 选择 |
| `RTC` | `drivers.rtc` | 独立设备叶 |
| `WORKQUEUE` | `kactive.workqueue` | 依赖 kthreadd/调度；驱动反向 select |
| `DEBUG_DIAGNOSTICS` | `debug`, `printk` 的非必要部分 | release 可降级，但保留最小故障通道 |

### 不建议的开关粒度

- 不建议直接用“整个 `mm/`、`sched/`、`entry/` 目录”作为开关。
- 不建议仅靠 `--gc-sections` 充当配置系统。
- 不建议把“源码被注释”当作可安全删除证据。
- 不建议让驱动通过未声明的全局符号自动拉入依赖；应由配置规则显式 `depends on`/`select`。

## 6. 强连通核与叶的解释

严格的 SCC 计算需要更完整的双向调用图；本任务的依赖边按“能力 → 依赖”方向，主要形成 DAG 风格的上游链，不能仅凭当前粗图把数学 SCC 等同于真实链接 SCC。

可用于工程决策的“核集合”是所有当前目标都汇聚到的上游：

1. x86_64 entry/linker/early page tables；
2. memblock → page allocator → object allocator；
3. per-CPU + traps + scheduler + IRQ；
4. PID/task/kthreadd；
5. VFS + 当前 root block/FAT32；
6. ELF/VM/syscall；
7. 至少一个 timer provider。

“可选叶”则是没有任何当前硬目标依赖，或只被特定 profile 依赖的节点：RTC、AArch64、initramfs、futex（极简 ABI 下）、未选择的存储/USB 后端、额外调试设施。

## 7. 裁剪前验证门

任何开关进入默认关闭前，至少通过以下验证；这些是门槛，不是本任务执行过的测试：

1. **构建门：**干净配置、所有目标、生成头与 UAPI 导出成功。
2. **链接门：**保存 linker map，确认入口、init/per-CPU 段和期望符号；记录 GC 前后差异。
3. **早期启动门：**能到 `console_init()`，能确认 memblock/page/slab、IDT/IRQ 状态。
4. **调度门：**PID 1、kthreadd、boot idle 均运行；定时 tick 与显式 schedule 都测试。
5. **设备门：**每个 root backend 单独 profile 启动，失败时能诊断而非死循环。
6. **文件系统门：**GPT/FAT32 root、`/dev`、`/boot/init` 均可访问；替代 root profile 独立验证。
7. **用户 ABI 门：**ELF exec、基本文件 I/O、进程、信号、时间、线程/futex（若启用）逐项 smoke test。
8. **负配置门：**关闭某功能后，配置系统应在构建前拒绝不满足依赖的组合，而不是等到链接或启动时失败。

## 8. 最先应做的结构化改造

1. 把递归 GLOB 替换为由 feature/object library 明确贡献源文件的清单。
2. 把 `main.c` 中无条件的 ATA/NVMe/XHCI/PCI/FAT32 初始化收进各自 provider，并由配置选择。
3. 建立统一 initcall/注册机制；明确手工列表与 linker-section 机制的唯一真相源。
4. 给 timer、root block、root filesystem 建立 provider/choice 接口。
5. 为每个开关输出可机器校验的 `depends_on` 与 `selects`，并以 `deps.yaml` 为初始种子。
