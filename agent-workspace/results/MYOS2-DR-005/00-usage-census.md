---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "agent-workspace/conventions.md"
  - "agent-workspace/repo-map.md"
  - "agent-workspace/tasks/MYOS2-DR-005-x86-reference-pack.md"
  - "mykernel/CMakeLists.txt"
  - "mykernel/scripts/target_kernel.cmake"
  - "mykernel/init/main.c"
  - "mykernel/arch/x86_64/**"
  - "mykernel/time/**"
status: final
open_questions:
  - "AP 启动段由谁复制到低端物理内存并触发 INIT/SIPI，所读快照中未找到调用闭环。"
  - "BSP 在使用 NX 页表位之前是否已可靠置 EFER.NXE，需要用启动寄存器转储或 QEMU 断点确认。"
  - "CONFIG_INTEL_X64_GDT_LAYOUT 的实际构建取值未在所读 CMake/头文件中定位到。"
---

# x86_64 用途普查

## 1. 范围与判定方法

本普查以 `time @ a039d9803ade2a1613d620bda375e028530d5242` 为唯一源码快照。任务书中列举的旧式 `arch/x86/**`、`arch/x86_64/{cpu,entry}/**` 路径在该快照中未见；实际实现集中在 `mykernel/arch/x86_64/**`，通用时间框架位于 `mykernel/time/**`。[VERIFIED mykernel/arch/x86_64/] [VERIFIED mykernel/time/]

构建并非依赖各子目录的 `CMakeLists.txt` 显式列文件：顶层 CMake 递归收集 `mykernel/**/*.c` 和 `*.S`，再剔除 `arch/${ARCH}/kbuild/*`。因此“翻译单元进入构建”和“机制有有效调用闭环”是两件事；下表状态同时考虑编译收集、初始化调用和关键副作用。[VERIFIED mykernel/CMakeLists.txt] [VERIFIED mykernel/scripts/target_kernel.cmake]

状态含义：

| 状态 | 含义 |
|---|---|
| `active` | 所读快照中存在初始化/调用闭环，并产生对应硬件或内核状态副作用。 |
| `partial` | 主路径可达，但关键状态、错误路径、安全检查或多 CPU 语义缺失。 |
| `stub` | 有类型、常量或函数壳，但主要实现被注释、无调用者或停在占位循环。 |
| `headers-only` | 仅见宏/类型/内联原语，未确认运行时消费者。 |
| `not-seen` | 在本次范围和检索方法中未见；不等同于仓库历史上从未存在。 |

## 2. 启动时序锚点

`start_kernel()` 依次执行 `setup_arch()`、`setup_per_cpu_areas()`、`trap_init()`、`sched_init()`、`init_IRQ()`、`timekeeping_init()`、`late_time_init()`、`arch_cpu_finalize_init()` 与 `myos_init_percpu_intr()`。这条顺序决定了本资料包对 IDT、per-CPU、时钟源和本地 APIC 的状态判断。[VERIFIED mykernel/init/main.c]

## 3. 机制总表

| 机制 | MyOS2 实际触点 | 当前用途 | 状态 | 对应卡片 |
|---|---|---|---|---|
| CPUID 基本/扩展叶 | `kernel/cpu/common.c`、`include/asm/cpufeatures.h`、`processor/processor_types_arch.h` | 读取厂商字符串、family/model/stepping、11 个原始能力字、地址宽度、品牌和缓存摘要 | `partial`：厂商注册、依赖过滤、跨 CPU 交集和多数启用逻辑被注释 | `cards/cpuid.md`、`cpuid-atlas.md` |
| CR0 | `kernel/fpu/init.c`、启动汇编与 `processor_const_arch.h` | 清/置 `TS`、`EM`；长模式启动使用 `PE/PG` 等 | `partial`：FPU 只完成最低限度 CR0 配置 | `cards/msr-control-registers.md`、`cards/fpu-xstate.md` |
| CR3 / TLB | `mm/**`、`insns/special_insns.h`、上下文切换相关头 | 装载顶级页表、局部 TLB 刷新 | `active/partial`：PCID/INVPCID 策略未形成闭环 | `cards/paging.md` |
| CR4 | `processor_const_arch.h`、`kernel/cpu/common.c`、`kernel/myos_APboot.S`、`kernel/fpu/init.c` | PAE/PGE/长模式辅助；AP 汇编无条件置 FSGSBASE；SMEP/SMAP/UMIP/OSXSAVE 代码被注释 | `partial` | `cards/msr-control-registers.md`、`cards/fpu-xstate.md` |
| EFER | `insns/x86msr_const.h`、启动汇编、系统调用初始化 | 长模式、SYSCALL/SYSRET、NX 定义 | `partial`：NXE 在 BSP 的可靠置位尚待运行时核对 | `cards/msr-control-registers.md`、`cards/paging.md` |
| SYSCALL MSR | `kernel/cpu/common.c`、`entry/entry_64.S` | 写 `STAR/LSTAR/SFMASK`，进入 64 位系统调用汇编 | `partial`：安全返回判断未控制 IRET/SYSRET 分支 | `cards/syscall-context.md`、`cards/msr-control-registers.md` |
| FS/GS MSR 与 SWAPGS | `entry/entry_64.S`、`insns/fsgsbase.h`、`sched/context/process64_arch.c` | per-CPU 入口、用户 FS/GS 快照 | `partial`：任务切换恢复函数主体被注释 | `cards/syscall-context.md` |
| 页表项格式 | `mm/pgtable/pgtable_const_arch.h`、`pgtable_arch.h` | 4 KiB/2 MiB/1 GiB 映射、权限、缓存、NX、软件位 | `active/partial`：PKU/加密固定为 0，LA57 未启用 | `cards/paging.md` |
| GDT | `processor/desc_arch.h`、`smp/percpu_area.c`、`kernel/cpu/common.c` | per-CPU GDT、TSS 描述符、段寄存器刷新 | `active/partial` | `cards/segmentation-gdt-idt-tss.md` |
| IDT | `kernel/idt.c`、`entry/entry_64.S`、`kernel/traps.c` | 生成异常和 IRQ 门，`lidt` 装载 | `active/partial`：只读映射/IST 强化未完成 | `cards/interrupts-exceptions.md`、`cards/segmentation-gdt-idt-tss.md` |
| TSS | `processor/processor_types_arch.h`、`kernel/cpu/common.c`、`sched/context/process64_arch.c` | `RSP0`、SYSCALL 临时 `sp2`、任务切换更新内核栈 | `active/partial`：IST 与 I/O bitmap 初始化被注释 | `cards/segmentation-gdt-idt-tss.md` |
| 异常入口 | `entry/entry_64.S`、`myos/interrupt.c` | 0–31 向量公共入口；少数异常有专门处理 | `partial`：多数异常仅打印/返回或停机 | `cards/interrupts-exceptions.md` |
| 外部 IRQ | `entry/entry_64.S`、`kernel/irqinit.c`、`myos/interrupt.c` | 向量分派、控制器回调、IRQ 后调度 | `partial`：ack 参数固定为局部变量 0；入口在 C 前 `sti` | `cards/interrupts-exceptions.md` |
| xAPIC/x2APIC | `kernel/apic/apic.c`、`lapic.c`、`include/asm/apic*.h` | SPIV、LVT、EOI、ICR、x2APIC MSR 访问 | `partial` | `cards/apic-ioapic.md` |
| I/O APIC | `kernel/apic/io_apic.c`、`myos/apic.c` | 固定物理地址映射、24 个 RTE、屏蔽/安装/EOI | `partial`：固定 BSP 目标、无 ACPI 枚举、索引窗口无锁 | `cards/apic-ioapic.md` |
| 8259A PIC | `kernel/myos_i8259.c` | 重编程后全部屏蔽，保留 mask/unmask/EOI 原语 | `active/legacy` | `cards/io-ports.md` |
| SMP/per-CPU | `smp/setup_percpu.c`、`percpu_area.c`、`kernel/myos_APboot.S`、`smp/smpboot.c` | BSP per-CPU 区、GDT/GSBASE；AP 过渡汇编 | `partial/stub`：AP 末尾永久循环，未见启动调用闭环 | `cards/apic-ioapic.md`、`cards/syscall-context.md` |
| TSC | `kernel/tsc.c` | 频率估计、cycle→ns、`native_sched_clock()` | `partial`：不注册为 timekeeping clocksource，并被标 unstable | `cards/timers.md` |
| HPET | `kernel/hpet.c` | 固定地址映射、周期 timer0、递增 jiffies、注册候选 clocksource | `partial`：注册后未真正切换 timekeeper；错误返回被忽略 | `cards/timers.md` |
| LAPIC timer | `myos/LVT_timer.c` | 周期 LVT 定时中断、调试计数显示 | `partial`：未承担 `do_timer()` | `cards/timers.md` |
| PIT/i8253 | `kernel/time.c`、`myos/pre_init.c` | 保留初始化接口和注释代码 | `stub`：函数体/调用被注释，未见活动 tick | `cards/timers.md`、`cards/io-ports.md` |
| RTC/CMOS | `kernel/rtc.c` | 从 0x70/0x71 读取启动墙钟 | `active`：不承担持续单调计时 | `cards/timers.md`、`cards/io-ports.md` |
| KVM clock | `kernel/kvmclock.c` | 探测 KVM 特性和选择 MSR 编号 | `stub`：per-CPU 页、校准、sched_clock、clocksource 注册均注释，未见调用者 | `cards/timers.md` |
| PCI 配置 I/O | `pci/direct.c` | CF8/CFC 配置机制 #1 | `active/partial`：配置访问锁被注释 | `cards/io-ports.md` |
| PS/2 键盘 | `myos/keyboard.c` | 0x60/0x64 控制器、IOAPIC IRQ、扫描码缓冲 | `active/partial` | `cards/io-ports.md` |
| ATA PIO | `myos/ide.c`、`include/obsolete/ide.h` | LBA28/LBA48、`insw/outsw`、IRQ 完成 | `active/partial`：大量无超时忙等 | `cards/io-ports.md` |
| 原子操作 | `lock_IPC/atomic/atomic_arch.h` | `xadd/cmpxchg/xchg` 和算术/位测试 | `partial`：`arch_atomic_add_test_negative` 实际执行减法 | `cards/atomic-locks.md` |
| ticket spinlock | `lock_IPC/spinlock/spinlock_smp_arch.h` | head/tail ticket 锁与 pause 自旋 | `partial`：`arch_spin_trylock()` 未获取锁即返回成功 | `cards/atomic-locks.md` |
| FPU/xstate | `kernel/fpu/init.c`、`kernel/cpu/common.c`、`sched/context/process64_arch.c` | CPUID 能力记录、CR0 最低限度初始化 | `partial/high-risk`：OSFXSR/OSXSAVE/XCR0 和任务态保存恢复缺失 | `cards/fpu-xstate.md` |
| 任务上下文切换 | `sched/context/process*_arch.c`、`switch_to_*` | 被调用者保存寄存器、栈切换、current/TSS 更新、fork 栈帧 | `partial`：FS/GS、FPU、TLS、PKRU 等未完整恢复 | `cards/syscall-context.md` |

## 4. 当前实现的关键边界

1. **检测、定义与启用必须分开读。** `cpufeatures.h` 中存在某一位，不代表 `get_cpu_cap()` 已填充，也不代表 CR4/EFER/XCR0 已启用；本资料包所有卡片均分别列三层。[VERIFIED mykernel/arch/x86_64/include/asm/cpufeatures.h] [VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]
2. **时间机制不是“HPET 或 TSC 二选一”。** 所读快照中 timekeeper 初始绑定 `jiffies`；HPET timer0 推进 jiffies；TSC 服务 sched_clock；LAPIC timer 仅调试计数。[VERIFIED mykernel/time/timekeeping/timekeeping.c] [VERIFIED mykernel/arch/x86_64/kernel/hpet.c] [VERIFIED mykernel/arch/x86_64/kernel/tsc.c] [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c]
3. **SMP 数据结构存在不等于 AP 已上线。** per-CPU 分配和 BSP GS/GDT 是活动路径，但 AP 汇编未见启动调用且最终 `jmp .`。[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c] [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S]
4. **汇编入口能返回不等于 ABI 安全。** 系统调用入口确实建立 `pt_regs` 并执行 `sysretq`，但 C 层返回的安全判定没有驱动 IRET 回退。[VERIFIED mykernel/arch/x86_64/entry/entry_64.S] [VERIFIED mykernel/arch/x86_64/entry/common.c]
5. **静态结论未替代运行时验证。** 本任务未编译、未启动 QEMU/VMware、未读取寄存器转储；需运行时确认的项目集中列在 `open_questions.md`。