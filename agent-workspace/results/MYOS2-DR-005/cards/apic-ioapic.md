---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "mykernel/arch/x86_64/kernel/apic/apic.c"
  - "mykernel/arch/x86_64/kernel/apic/lapic.c"
  - "mykernel/arch/x86_64/kernel/apic/io_apic.c"
  - "mykernel/arch/x86_64/myos/apic.c"
  - "mykernel/arch/x86_64/include/asm/apic.h"
  - "mykernel/arch/x86_64/include/asm/apicdef.h"
  - "mykernel/arch/x86_64/kernel/myos_APboot.S"
  - "mykernel/arch/x86_64/smp/percpu_area.c"
status: final
open_questions:
  - "IOAPIC 数量、GSI base、override 与 NMI source 尚未从 ACPI MADT 形成活动枚举。"
  - "AP 启动 trampoline 的复制/唤醒者未见，SMP online 状态不可由静态代码证明。"
  - "x2APIC 模式下目标 APIC ID 宽度与 IPI 路由尚未有多核实测。"
---

# 速查卡：Local APIC、x2APIC、I/O APIC 与 SMP

## 1. IA32_APIC_BASE

| 位 | 名称 | 语义 |
|---:|---|---|
| 8 | BSP | 只读：当前逻辑处理器是否为 BSP |
| 10 | x2APIC enable | 与 bit 11 组合选择 x2APIC 模式 |
| 11 | APIC global enable | 关闭时 local APIC 不工作 |
| 12+ | APIC base | xAPIC MMIO 模式下的物理基址；低位按规范保留 |

典型模式：

| bit11 | bit10 | 模式 |
|---:|---:|---|
| 0 | 0 | APIC disabled |
| 1 | 0 | xAPIC |
| 1 | 1 | x2APIC |
| 0 | 1 | 保留/非法组合 |

[VERIFIED mykernel/arch/x86_64/kernel/apic/apic.c] [EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 10]

MyOS2 有 xAPIC MMIO 和 x2APIC MSR 两套访问器；切换前会检查 CPUID x2APIC 位，但 AP trampoline 又无条件操作相应控制位，形成主路径与 AP 路径不一致。[VERIFIED mykernel/arch/x86_64/kernel/apic/apic.c] [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S]

## 2. Local APIC 常用寄存器

| 名称 | xAPIC offset | x2APIC MSR | 作用 |
|---|---:|---:|---|
| ID | `0x020` | `0x802` | local APIC ID |
| Version | `0x030` | `0x803` | 版本、LVT 数量 |
| TPR | `0x080` | `0x808` | task priority |
| PPR | `0x0A0` | `0x80A` | processor priority |
| EOI | `0x0B0` | `0x80B` | end of interrupt |
| LDR | `0x0D0` | `0x80D` | logical destination |
| SVR | `0x0F0` | `0x80F` | spurious vector + APIC enable |
| ISR/TMR/IRR | `0x100/180/200` | `0x810...` | in-service/trigger/pending |
| ESR | `0x280` | `0x828` | error status |
| ICR low/high | `0x300/310` | `0x830` | IPI command/destination |
| LVT timer | `0x320` | `0x832` | local timer |
| LVT thermal/perf/LINT/error | `0x330..370` | `0x833..837` | local sources |
| Initial count | `0x380` | `0x838` | timer initial count |
| Current count | `0x390` | `0x839` | timer current count |
| Divide config | `0x3E0` | `0x83E` | timer divisor |
| Self IPI | — | `0x83F` | x2APIC self IPI |

[EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 10]

## 3. SVR

| 位 | 字段 | 语义 |
|---:|---|---|
| 7:0 | vector | spurious interrupt vector |
| 8 | APIC software enable | 1=local APIC enabled |
| 9 | focus processor checking | legacy xAPIC feature |
| 11:10 | reserved | 保留 |
| 12 | EOI broadcast suppression | 1=抑制 EOI 广播，需 I/O APIC 能力配合 |
| 31:13 | reserved | 保留 |

MyOS2 配置 SPIV/SVR 与 LVT 屏蔽状态，属于活动 local APIC 初始化的一部分。[VERIFIED mykernel/arch/x86_64/kernel/apic/apic.c]

## 4. LVT 项

| 位 | 字段 | 说明 |
|---:|---|---|
| 7:0 | vector | fixed/timer/error 等向量 |
| 10:8 | delivery mode | fixed、NMI、SMI、ExtINT 等；并非所有 LVT 都支持全部模式 |
| 11 | reserved | — |
| 12 | delivery status | 只读 |
| 13 | polarity | LINT 等适用 |
| 14 | remote IRR | level-triggered LINT 适用，只读 |
| 15 | trigger mode | 0=edge，1=level |
| 16 | mask | 1=屏蔽 |
| 18:17 | timer mode | LVT timer：00 one-shot、01 periodic、10 TSC-deadline、11 reserved |
| 31:19 | reserved | — |

[EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 10]

MyOS2 的 custom LVT timer 使用周期模式和固定向量 `0xEE`，handler 只做调试计数，不推进通用 timer wheel/timekeeper。[VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c]

## 5. ICR

### ICR low

| 位 | 字段 |
|---:|---|
| 7:0 | vector |
| 10:8 | delivery mode：fixed/lowest/SMI/NMI/INIT/Start-Up 等 |
| 11 | destination mode：0 physical，1 logical |
| 12 | delivery status（只读） |
| 13 | reserved |
| 14 | level：deassert/assert |
| 15 | trigger mode：edge/level |
| 17:16 | destination shorthand：none/self/all including self/all excluding self |
| 31:18 | reserved |

xAPIC 的 destination 在 ICR high[31:24]；x2APIC 将 32 位 destination 合并到 64 位 ICR[63:32]。[EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 10]

INIT/SIPI 启动协议的关键顺序是：准备低 1 MiB trampoline → INIT IPI → 等待规范时间 → 一个或两个 SIPI，vector 指出 4 KiB 页号。MyOS2 有 AP transition 汇编，但未见负责复制和发 INIT/SIPI 的完整调用闭环。[VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S] [INFERRED]

## 6. I/O APIC redirection entry

64 位 RTE：

| 位 | 字段 | 语义 |
|---:|---|---|
| 7:0 | vector | 投递 CPU 向量 |
| 10:8 | delivery mode | fixed/lowest/SMI/NMI/INIT/ExtINT |
| 11 | destination mode | physical/logical |
| 12 | delivery status | 只读 |
| 13 | polarity | 0 active-high，1 active-low |
| 14 | remote IRR | level-triggered 只读 |
| 15 | trigger mode | 0 edge，1 level |
| 16 | mask | 1=屏蔽 |
| 55:17 | reserved | 保留 |
| 63:56 | destination | xAPIC 目的字段；扩展/重映射依平台 |

[EXTERNAL Intel SDM 253668-092US, Vol. 3A, I/O APIC discussion; Intel I/O APIC specification]

MyOS2：

- `kernel/apic/io_apic.c` 把默认 I/O APIC 物理地址映射到虚拟地址；
- `myos/apic.c` 实际通过 index/data window 读写 RTE；
- 固定初始化 24 项；
- destination 固定为 BSP/APIC ID 0；
- 未见 ACPI MADT 的 IOAPIC/GSI/interrupt-source-override 参与；
- index/data 两步访问缺少自旋锁，多个 CPU/中断上下文可交叉写坏目标寄存器。

[VERIFIED mykernel/arch/x86_64/kernel/apic/io_apic.c] [VERIFIED mykernel/arch/x86_64/myos/apic.c]

## 7. EOI 规则

- Local APIC fixed/lowest-priority 中断完成后写 EOI。
- 对 level-triggered I/O APIC 中断，设备源必须先解除电平条件，再 EOI；否则会立即重触发。
- MSI/MSI-X 不经过 I/O APIC，但仍在 local APIC 侧形成 vector/EOI 语义。
- spurious vector 在特定条件下不应写 EOI，需按 ISR/规范处理。
- MyOS2 通用 dispatcher 把错误的局部 `irq_nr=0` 传给 ack 回调，是当前控制器语义风险。[VERIFIED mykernel/arch/x86_64/myos/interrupt.c]

## 8. SMP/per-CPU 当前边界

活动部分：

- 分配、复制和重定位 per-CPU 区；
- BSP 重新装载 per-CPU GDT/GS base；
- per-CPU TSS、current、runqueue、timer 数据结构存在。

[VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c] [VERIFIED mykernel/arch/x86_64/smp/percpu_area.c]

不完整部分：

- `smpboot.c` 主要只设置 boot idle；
- AP 汇编假定若干固定地址/状态；
- AP 无条件开 x2APIC/FSGSBASE；
- AP 最终停在 `jmp .`；
- 未见 online mask、secondary CPU identify、scheduler attach 与 per-CPU timer 完整闭环；
- I/O APIC 所有设备 IRQ 仍定向 BSP。

[VERIFIED mykernel/arch/x86_64/smp/smpboot.c] [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S] [VERIFIED mykernel/arch/x86_64/myos/apic.c]

## 9. 邻近可扩展项

| 项 | 价值 | 前置条件 |
|---|---|---|
| ACPI MADT 枚举 | 多 IOAPIC、GSI、IRQ override、NMI | ACPI 表解析 |
| MSI/MSI-X | 绕过 24-pin 固定 IOAPIC 上限 | PCI capability parser、vector allocator |
| x2APIC logical cluster | 大 CPU 数路由 | topology 与 APIC ID 管理 |
| interrupt remapping | IOMMU 安全和大 APIC ID | VT-d/AMD IOMMU |
| TSC-deadline clockevent | 精确 one-shot tick | CPUID.1 ECX[24]、稳定 TSC |
| vector allocator/affinity | IRQ 分散到多个 CPU | online CPU/locking/迁移协议 |
| NMI watchdog | 卡死诊断 | perf/APIC NMI 与 IST |

## 10. 最小验证

1. 启动 dump IA32_APIC_BASE、APIC ID/version/SVR 和所有 LVT。
2. 对每个 IOAPIC pin dump RTE，核对 ACPI polarity/trigger。
3. 双 CPU 并发安装/屏蔽 RTE，证明 index/data window 有锁。
4. 启动 AP 后确认它离开 trampoline、执行 `cpu_init()`、加入 online mask 和 idle loop。
5. 向 self/other CPU 发 fixed IPI、NMI、INIT/SIPI 的受控测试。
6. 设备 level IRQ 压测，确认 source clear→EOI 顺序与 remote IRR。
