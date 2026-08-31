---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "agent-workspace/conventions.md"
  - "agent-workspace/repo-map.md"
  - "mykernel/init/main.c"
  - "mykernel/arch/x86_64/**"
  - "mykernel/time/**"
status: final
open_questions:
  - "本文件本身即未解决问题登记表；问题关闭时应新增 v2 文件而非覆盖。"
---

# MYOS2-DR-005 Open Questions

本表只收录无法由 `time @ a039d9803ade2a1613d620bda375e028530d5242` 静态源码可靠回答的事项。`next_probe` 是最小验证动作，不代表本任务已经执行。

| ID | 主题 | 问题 | 影响 | 当前证据 | next_probe |
|---|---|---|---|---|---|
| OQ-001 | AP bring-up | 谁把 `_APboot_start..end` 复制到低 1 MiB 并发送 INIT/SIPI？ | 决定 SMP 是否真正可用 | trampoline 汇编存在但未见调用者 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S] | 链接 map 搜符号引用；QEMU `-d int,cpu_reset`；ICR 写断点 |
| OQ-002 | AP completion | AP 为什么在长模式入口末尾 `jmp .`，预期后续 C 入口是什么？ | AP 无法 online/schedule | 明确永久循环 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S] | 设计 secondary_startup_64→cpu_init→idle 协议 |
| OQ-003 | master build config | `CONFIG_INTEL_X64_GDT_LAYOUT` 的实际取值在哪里产生？ | STAR/SYSENTER selector 与链接正确性 | 搜索未定位 define；代码含条件分支 [VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] | 保存 CMake compile_commands + `-dM` 预处理宏 |
| OQ-004 | NX | BSP 在任何 NX PTE present 前是否已置 EFER.NXE？ | 未置会产生 #PF.RSVD/启动失败 | 页表模板用 NX；静态置位闭环未确认 [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h] | 早期串口打印 EFER；在首个 NX 映射前断点 |
| OQ-005 | CR4 asymmetry | BSP 与 AP 的 CR4.FSGSBASE 是否一致，且都先检查 CPUID？ | 不支持 CPU #GP；支持 CPU 上 FS/GS 指令行为不一致 | AP 无条件置；BSP enable 被注释 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S] [VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] | 每 CPU dump CR4/CPUID.7；统一 enable helper |
| OQ-006 | SYSRET | `do_syscall_64()` 的 bool 返回为何未被汇编使用？ | non-canonical RCX 可在 kernel exit #GP | 汇编无条件 SYSRET [VERIFIED mykernel/arch/x86_64/entry/entry_64.S] | 加 IRET fallback；构造 bad RCX 测试 |
| OQ-007 | invalid syscall | 越界 syscall 的用户返回值是什么？ | ABI 错误/信息泄漏 | C 路径未见显式 `-ENOSYS` [VERIFIED mykernel/arch/x86_64/entry/common.c] | 用户程序调用最大号+1并断言 |
| OQ-008 | GS/NMI | NMI/内核态异常如何判断是否需要 SWAPGS？ | per-CPU 指针错乱 | 普通入口有 SWAPGS，未见 paranoid state machine [VERIFIED mykernel/arch/x86_64/entry/entry_64.S] | 指令级 NMI 注入测试；实现 saved-CS/MSR 判定 |
| OQ-009 | FS/GS switch | `x86_fsgsbase_load()` 何时恢复任务 FS/GS base？ | TLS/用户运行时跨任务污染 | 函数主体注释 [VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c] | 两任务不同 FSBASE 循环切换 |
| OQ-010 | FPU | 用户态当前能否执行 SSE/AVX；若能，是否发生跨任务泄漏？ | 数据破坏/机密泄漏 | FPU state 不在 thread，switch hooks 注释 | 两任务 XMM/YMM pattern 测试；启动时临时禁用未支持能力 |
| OQ-011 | IDT/IST | #DF/NMI/#MC 是否有任何非注释 IST 初始化？ | 坏栈时无法诊断 | TSS 有 IST 数组，setup 被注释 | dump IDT IST + TSS IST；坏栈双重故障测试 |
| OQ-012 | IRQ ack | `hwint_irq_handler` 把 0 传给 ack 是否为遗留接口还是确定 bug？ | PIC/控制器 ack 错 IRQ | 局部 `irq_nr=0` [VERIFIED mykernel/arch/x86_64/myos/interrupt.c] | 对 ack callback 加 trace；修为 descriptor IRQ/vector |
| OQ-013 | IRQ nesting | 中断公共入口 `sti` 后允许何种嵌套？ | 驱动重入、栈耗尽、死锁 | C 前显式 STI，缺 hardirq nesting | IRQ storm + 嵌套计数；定义入口契约 |
| OQ-014 | vector maps | `0xEC` 与 `0xEE` 两个 local timer vector 哪个是长期 ABI？ | 向量冲突/维护错误 | 新/obsolete 头并存 | 生成全仓 vector map；单一 allocator/头 |
| OQ-015 | IOAPIC topology | 是否有非默认 IOAPIC、GSI base 或 interrupt source override？ | 物理机 IRQ 错路由 | 固定地址、24 RTE、无 MADT 闭环 | 解析 ACPI MADT/HPET 并与 dump 对照 |
| OQ-016 | IOAPIC lock | index/data window 在 SMP/嵌套 IRQ 下是否已发生串写？ | 错 RTE/丢 IRQ | 访问无锁 [VERIFIED mykernel/arch/x86_64/myos/apic.c] | 双 CPU 并发不同 index 读写压力 |
| OQ-017 | spin trylock | 哪些调用者依赖错误的 `arch_spin_trylock()`？ | 无互斥数据竞争 | 实现不 CAS [VERIFIED mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h] | 全仓 caller 列表；修复后 TSAN/压力测试 |
| OQ-018 | atomic add-negative | 错误的减法实现是否已有调用者？ | 逻辑反向 | `subl` 与函数名相反 [VERIFIED mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h] | caller 搜索 + 表驱动测试 |
| OQ-019 | PCI config lock | CF8/CFC 的锁为何被注释？ | 并发访问错 BDF | selector/data 两步无锁 [VERIFIED mykernel/arch/x86_64/pci/direct.c] | SMP 并发配置读取，读 vendor/device 固定值 |
| OQ-020 | 8259 EOI | slave IRQ EOI 顺序是否故意为 master→slave？ | 若 PIC 重新启用可丢/卡 IRQ | 当前代码先 master [VERIFIED mykernel/arch/x86_64/kernel/myos_i8259.c] | 对照 8259 ISR/IRR 实测并修正 |
| OQ-021 | ATA timeout | 所有 ATA busy/DRQ loop 的系统级超时策略是什么？ | 设备故障永久挂核 | 多个无限忙等 [VERIFIED mykernel/arch/x86_64/myos/ide.c] | 统一 deadline helper + 故障注入 |
| OQ-022 | HPET address | `0xFED00000` 与固定 route 是否对所有目标平台成立？ | 访问错误 MMIO/中断不达 | 未由 ACPI HPET 表证明 | 解析 HPET table；QEMU/VMware/物理机对照 |
| OQ-023 | HPET conversion | HPET period→Hz→mult/shift 计算是否量纲正确？ | time drift/overflow | 静态表达式可疑 | 已知 period_fs 纯函数测试和 10 秒漂移 |
| OQ-024 | clocksource switch | 为什么 `__clocksource_select()` 的 `timekeeping_notify` 被注释？ | HPET 永远只是候选，timekeeper 停留 jiffies | 源码明确注释 [VERIFIED mykernel/time/clocksource/clocksource.c] | 恢复切换协议；打印活动 clock |
| OQ-025 | TSC 0x15 | 当前 leaf 0x15 计算为何返回 denominator 量级？ | 固定 3 GHz fallback/错误 sched_clock | 与 SDM 公式不符 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c] | 单元测试 `Hz=ECX*EBX/EAX`；128-bit 防溢出 |
| OQ-026 | LAPIC timer | LVT timer 是否计划成为 per-CPU clockevent/scheduler tick？ | 当前只调试显示，SMP tick 不完整 | handler 不调用 timer/scheduler | 明确角色；注册 clockevent 或删除重复 tick |
| OQ-027 | KVM clock | `kvmclock_init()` 为什么无调用者？ | 虚拟机稳定时钟未使用 | 功能主体/注册注释 | 在 platform clock init 接入或标死代码 |
| OQ-028 | CPUID vendor | vendor device 注册为何被注释，`x86_vendor` 长期为 unknown 吗？ | Intel/AMD 分支与 MSR 选择失效 | common.c 厂商注册流程注释 | 启动打印 vendor enum；恢复最小 vendor table |
| OQ-029 | CPUID cross-CPU | secondary CPU 能力如何与 boot CPU 求交？ | 在较弱 CPU 执行非法指令 | 交集/identify secondary 未闭合 | 上线 AP 后逐 CPU identify + global mask |
| OQ-030 | CPUID representation | C bit-field overlay 是否锁定 GCC/Clang 布局？ | 换编译器后位名错位 | union raw array + bitfield [VERIFIED mykernel/arch/x86_64/processor/processor_types_arch.h] | 双编译器 static_assert/生成 mask；优先显式 bitops |
| OQ-031 | page cache type | `__PAGE_KERNEL_NOCACHE` 为何可执行（NX=0）？ | MMIO/数据映射扩大执行面 | 源码模板明确 0 [VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h] | 审查所有 caller；默认加 NX |
| OQ-032 | PCID/TLB | 定义了 PCID/no-flush，但是否有 ASID 生命周期与 shootdown？ | 陈旧翻译/权限 | 常量存在，策略未见 | 全仓 CR3/INVPCID caller 图 + SMP TLB test |
| OQ-033 | user I/O | 是否计划支持任务级 TSS I/O bitmap？ | 用户驱动/安全模型 | storage 存在，setup/switch 注释 | 明确“不支持”或实现 bitmap 生命周期 |
| OQ-034 | runtime baseline | 当前目标 QEMU/VMware/物理机的 CPUID/APIC/HPET 实际值是什么？ | 多项静态假设不可证 | 本任务未运行 | 生成一次 boot hardware snapshot 附件 |
