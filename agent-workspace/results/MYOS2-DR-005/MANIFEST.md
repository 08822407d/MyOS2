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
  - "mykernel/init/main.c"
  - "mykernel/arch/x86_64/**"
  - "mykernel/time/**"
status: final
open_questions:
  - "所有需运行时确认的项目集中在 open_questions.md；本任务未编译或启动内核。"
---

# MYOS2-DR-005 交付清单

## 1. 交付摘要

本目录提供一套**绑定 MyOS2 `time` 快照源码位置**的 x86_64 查表资料包，覆盖 CPUID、MSR/控制寄存器、分页、异常/中断、GDT/IDT/TSS、APIC/IOAPIC、计时、端口 I/O、系统调用/上下文切换、原子锁和 FPU/xstate。

手册口径以 Intel SDM revision 092 为主；AMD 专有扩展单列 AMD APM；HPET 使用 IA-PC HPET Specification 1.0a。[EXTERNAL Intel SDM 253665/253668/335592-092US] [EXTERNAL AMD APM 24593 Rev. 3.44] [EXTERNAL IA-PC HPET Specification Rev. 1.0a]

## 2. 文件逐项状态

| 文件 | 覆盖状态 | 已完成 | 明确未做 | 后续消费方式 |
|---|---|---|---|---|
| `MANIFEST.md` | final | 文件索引、范围、限制、风险摘要 | 不记录运行时测试结果 | 合并/复核入口 |
| `00-usage-census.md` | final | 机制→实际源码→状态→卡片的普查；构建路径漂移 | 未逐文件统计行数 | 002/003/007 可引用状态边界 |
| `cards/cpuid.md` | final | 当前叶、能力槽、关键完整位表、依赖与测试 | 07H.1 快速演进位未逐位展开 | CPU 初始化与能力层重构 |
| `cpuid-atlas.md` | final | 0x0–0x1F、常用 0x80000000+、hypervisor 范围，标记 used/unused | AMD 0x80000020 以上未逐位展开 | 新增 CPUID 支持的索引 |
| `cards/msr-control-registers.md` | final | CR0/CR3/CR4/EFER/RFLAGS、当前 MSR、顺序陷阱 | 模型相关 MSR 未做全 CPU 型号矩阵 | 启动/安全/FPU/APIC 修改前查阅 |
| `cards/paging.md` | final | 4 级页表、PTE 位、页大小、#PF、TLB/PAT 陷阱 | 未做页表 dump 或 shootdown 压测 | MM/fault/安全审计 |
| `cards/interrupts-exceptions.md` | final | 0–31 异常表、IDT gate、向量体系、当前入口缺口 | 未注入异常/IRQ | trap/IRQ 重构与测试 |
| `cards/segmentation-gdt-idt-tss.md` | final | descriptor、TSS/IST/I/O bitmap、per-CPU 关系 | 未 dump GDTR/IDTR/TR | 启动、入口和 per-CPU 教学 |
| `cards/apic-ioapic.md` | final | APIC_BASE、SVR/LVT/ICR、IOAPIC RTE、SMP 边界 | 未解析实际 ACPI MADT；未启动 AP | APIC/SMP/IRQ affinity |
| `cards/timers.md` | final | TSC/HPET/LAPIC/PIT/RTC/KVM role matrix、寄存器和公式 | 未测漂移、频率和中断路由 | timekeeping/clockevent 重构 |
| `cards/io-ports.md` | final | CMOS/PIC/PS2/PCI/ATA/PIT 端口及协议陷阱 | 未接真实设备故障注入 | legacy 驱动和资源锁审计 |
| `cards/syscall-context.md` | final | SYSCALL/SYSRET、STAR、pt_regs、switch_to、状态矩阵 | 未运行 ABI/坏 RCX/NMI 测试 | syscall/调度上下文修复 |
| `cards/atomic-locks.md` | final | x86 原子语义、ticket lock、两个确定错误、测试矩阵 | 未执行并发压力/litmus | lock/atomic 修复任务 |
| `cards/fpu-xstate.md` | final | CR0/CR4/XCR0、CPUID.0D、保存格式、实现顺序 | 未运行寄存器污染测试 | FPU/SIMD 子系统实现 |
| `open_questions.md` | final | 34 个未闭合问题，含影响、证据和最小 probe | 未替 Owner 作设计选择 | 测试/路线图/债务登记输入 |

## 3. 覆盖状态

### 做完

- 按实际 `mykernel/arch/x86_64/**` 路径完成用途普查，而不是沿用任务书示例的过时根路径。
- 区分“头文件定义”“CPUID 检测”“控制寄存器/MSR 启用”“运行时消费者”四层。
- 将 TSC、HPET、jiffies、LAPIC timer、RTC、KVM clock 的实际角色分开。
- 将 BSP per-CPU 初始化与尚未完成的 AP bring-up 分开。
- 对关键寄存器给出位表，并把未使用但邻近可扩展项列入卡片。
- 建立 CPUID 基本叶 0x0–0x1F 与常用扩展叶 atlas。
- 记录静态可确定的高风险语义错误。

### 明确未做

- 未修改任何内核源码、构建脚本或其他任务目录。
- 未编译、链接、启动 QEMU/VMware/物理机。
- 未声称现有代码通过测试。
- 未从历史 `bugs_record.md`、`todo.txt`、`documents/` 推断当前状态。
- 未把厂商/型号相关 MSR 位写成跨 CPU 通用事实。
- 未穷举 Intel/AMD 当期手册中的全部未来 CPUID/MSR；与 MyOS2 无触点的内容只在邻近项层面列出。

## 4. 关键静态发现

### 高风险 correctness

1. `arch_spin_trylock()` 在锁空闲时返回成功，却没有原子增加 ticket，成功者实际上未持锁。[VERIFIED mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h]
2. `arch_atomic_add_test_negative()` 名为 add，汇编却执行 `subl`。[VERIFIED mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h]
3. 系统调用退出无条件 `sysretq`，没有消费 C 层的安全判定并回退 `iretq`。[VERIFIED mykernel/arch/x86_64/entry/entry_64.S] [VERIFIED mykernel/arch/x86_64/entry/common.c]
4. IRQ dispatcher 把局部常量 0 传给控制器 ack，而非实际 IRQ/vector。[VERIFIED mykernel/arch/x86_64/myos/interrupt.c]
5. FS/GS base、FPU/xstate、TLS、PKRU 等上下文未完整保存恢复。[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c] [VERIFIED mykernel/arch/x86_64/processor/processor_types_arch.h]

### 架构边界

1. per-CPU/GDT/TSS/BSP GSBASE 是活动实现；AP trampoline 未见启动闭环且最终永久循环。[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c] [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S]
2. timekeeper 初始化绑定 jiffies；HPET 中断推进 jiffies；TSC 只注册 sched_clock；HPET clocksource 的切换通知被注释。[VERIFIED mykernel/time/timekeeping/timekeeping.c] [VERIFIED mykernel/time/clocksource/clocksource.c]
3. CPUID 原始采集较广，但厂商规范化、依赖过滤、跨 CPU 交集和 feature enable 多数未闭合。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]
4. 页表支持 NX 模板，但 BSP 的 EFER.NXE 启用顺序需运行时核对。[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h]
5. I/O APIC 固定 24 RTE、固定 BSP 目标、无 MADT route/override 和 index/data 锁。[VERIFIED mykernel/arch/x86_64/myos/apic.c]

## 5. 质量与证据限制

- `[VERIFIED path]`：直接来自所读源码快照。
- `[INFERRED]`：由控制流、缺失调用或硬件语义推断，未运行验证。
- `[EXTERNAL ...]`：来自官方架构/平台规范。
- “所读快照中未见”不是对其他历史分支或未来提交的存在性断言。
- Intel CPUID 表按 2026-06 revision 092 的 Volume 1 做了页级核对；Vol. 3A/Vol. 4 的模型相关部分保持保守，只列本任务可高置信确认的架构字段。
- 所有需实机/虚拟机确认的事项统一放入 `open_questions.md`。

## 6. 协调快照

在创建本任务分支前，GitHub 上 `master` 为 `c7e68edb3dd76f4e4808dff12b5ca2021035e719`；分析源码固定为 `time @ a039d9803ade2a1613d620bda375e028530d5242`。当时开放 PR 数为 0；存在其他任务分支不能证明其 DOING/REVIEW 状态，因此本文件不代替协调者状态表。[VERIFIED GitHub branch/PR metadata]

本任务分支：`agent/MYOS2-DR-005`；目标：`master`。

## 7. 建议消费顺序

1. 先读 `00-usage-census.md` 建立“活动/partial/stub”地图。
2. CPU/启动修改：`cpuid.md` → `msr-control-registers.md` → `fpu-xstate.md`。
3. 入口/调度修改：`interrupts-exceptions.md` → `segmentation-gdt-idt-tss.md` → `syscall-context.md`。
4. SMP/设备修改：`apic-ioapic.md` → `atomic-locks.md` → `io-ports.md`。
5. 时间修改：`timers.md`，同时核对 `cpuid-atlas.md` 的 0x15/0x16。
6. 任何实现任务开工前，从 `open_questions.md` 选择对应最小 probe，先把静态不确定性变成可复现结果。
