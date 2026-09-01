---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "mykernel/arch/x86_64/entry/entry_64.S"
  - "mykernel/arch/x86_64/kernel/idt.c"
  - "mykernel/arch/x86_64/kernel/traps.c"
  - "mykernel/arch/x86_64/kernel/irqinit.c"
  - "mykernel/arch/x86_64/myos/interrupt.c"
  - "mykernel/arch/x86_64/include/asm/irq_vectors.h"
  - "mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h"
  - "mykernel/arch/x86_64/include/asm/irqflags.h"
status: final
open_questions:
  - "默认异常处理返回后是否会重复触发同一 fault，需要按各向量构造 QEMU 测试。"
  - "外部 IRQ 入口在调用 C 前执行 sti 的设计意图与允许的嵌套深度未见文档。"
  - "两个向量命名体系（irq_vectors.h 与 obsolete/myos_irq_vectors.h）的长期收敛方案未确定。"
---

# 速查卡：中断、异常与向量

## 1. 向量空间

| 范围/向量 | 体系定义 | MyOS2 |
|---|---|---|
| `0x00–0x1F` | 架构异常 | 汇编生成异常入口 |
| `0x20` | 第一外部向量 | `FIRST_EXTERNAL_VECTOR` |
| `0x80` | legacy int80 | 常量存在；主系统调用走 SYSCALL |
| `0xEC` | `LOCAL_TIMER_VECTOR` | 新头文件体系 |
| `0xEE` | `LOCAL_APIC_TIMER_IRQ` | obsolete/custom 体系中的活动 LVT timer |
| `0x22` | `HPET_IRQ` | custom HPET timer0 |
| `0xC8+` | IPI 区域 | custom 头保留 |
| `0xFF` | spurious | APIC 常见保留 |

[VERIFIED mykernel/arch/x86_64/include/asm/irq_vectors.h] [VERIFIED mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h]

同一快照中同时存在“新式通用向量头”和 `obsolete/` 自定义向量头；活动 HPET/LVT/设备代码仍依赖后者。新增向量前必须先确认没有跨体系冲突。[INFERRED]

## 2. 架构异常全表

| # | 助记 | 名称 | error code | MyOS2 当前处理 |
|---:|---|---|---|---|
| 0 | #DE | Divide Error | 无 | 通用未实现路径 |
| 1 | #DB | Debug | 无 | 通用未实现路径 |
| 2 | NMI | Non-maskable Interrupt | 无 | 未见 paranoid/NMI 完整路径 |
| 3 | #BP | Breakpoint | 无 | 通用未实现路径 |
| 4 | #OF | Overflow | 无 | 通用未实现路径 |
| 5 | #BR | BOUND Range Exceeded | 无 | 通用未实现路径 |
| 6 | #UD | Invalid Opcode | 无 | 通用未实现路径 |
| 7 | #NM | Device Not Available | 无 | 未接 lazy-FPU 恢复 |
| 8 | #DF | Double Fault | 0 | 未见 IST 专用栈 |
| 9 | — | Coprocessor Segment Overrun（保留/旧） | 无 | 不应使用 |
| 10 | #TS | Invalid TSS | 有 | 专门打印/处理 |
| 11 | #NP | Segment Not Present | 有 | 专门打印/处理 |
| 12 | #SS | Stack-Segment Fault | 有 | 专门打印/处理 |
| 13 | #GP | General Protection | 有 | 专门打印/处理 |
| 14 | #PF | Page Fault | 有 | 进入页故障路径 |
| 15 | — | Reserved | — | 不应分配 |
| 16 | #MF | x87 Floating-Point Error | 无 | 未见完整处理 |
| 17 | #AC | Alignment Check | 0 | 未见完整处理 |
| 18 | #MC | Machine Check | 无 | 未见 MCE 初始化 |
| 19 | #XM/#XF | SIMD Floating-Point Exception | 无 | 未见完整处理 |
| 20 | #VE | Virtualization Exception | 无 | 未见完整处理 |
| 21 | #CP | Control Protection | 有 | CET 未启用 |
| 22–27 | — | Reserved | — | 不应分配 |
| 28 | #HV（AMD） | Hypervisor Injection | 无 | AMD 专用；未见 |
| 29 | #VC（AMD） | VMM Communication | 有 | AMD SEV-ES/SNP；未见 |
| 30 | #SX（AMD） | Security Exception | 有 | AMD 专用；未见 |
| 31 | — | Reserved | — | 不应分配 |

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S] [VERIFIED mykernel/arch/x86_64/myos/interrupt.c] [EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 6] [EXTERNAL AMD APM 24593 Rev. 3.44, Vol. 2]

“error code 有/无”决定汇编栈形状。无硬件 error code 的入口必须人工压入占位 0，才能与公共 `pt_regs` 布局一致；有 error code 的异常不得再重复压入。[EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 6]

## 3. IDT 门字段

64 位 IDT gate 共 16 字节：

| 字段 | 位/宽度 | 作用 |
|---|---:|---|
| offset low | 16 | handler RIP[15:0] |
| selector | 16 | 内核代码段 selector |
| IST | 3 | 0=使用当前/RSP0；1–7 选择 TSS.IST |
| type | 4 | 0xE interrupt gate；0xF trap gate |
| DPL | 2 | 软件 INT 可调用所需 CPL |
| P | 1 | present |
| offset mid/high | 16+32 | handler RIP[63:16] |
| reserved | 32 | 必须为 0 |

Interrupt gate 在进入时清 IF；trap gate 不清 IF。MyOS2 汇编公共 IRQ 路径随后显式 `sti`，所以嵌套策略不是由 gate 类型单独决定。[VERIFIED mykernel/arch/x86_64/entry/entry_64.S] [VERIFIED mykernel/arch/x86_64/kernel/idt.c] [EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 6]

## 4. MyOS2 入口/分派路径

```text
CPU vector
  -> IDT gate
  -> idt_handler_array / irq_entries_start
  -> 保存寄存器、构造 pt_regs
  -> C dispatcher
       exception: do_error_trap / do_page_fault / custom handler
       irq: hwint_irq_handler
  -> controller ack
  -> 若 !in_atomic()，可能直接 schedule()
  -> 恢复寄存器、iretq
```

[VERIFIED mykernel/arch/x86_64/kernel/idt.c] [VERIFIED mykernel/arch/x86_64/entry/entry_64.S] [VERIFIED mykernel/arch/x86_64/myos/interrupt.c]

### 当前确定问题

1. `hwint_irq_handler()` 声明局部 `irq_nr = 0`，最终把 0 传给控制器 `ack`，而不是实际 vector/IRQ。对需要 IRQ 编号的控制器会错误 EOI/ack。[VERIFIED mykernel/arch/x86_64/myos/interrupt.c]
2. 外部 IRQ 汇编在调用 C 前执行 `sti`，但未见 `irq_enter()/irq_exit()`、per-CPU hardirq nesting 和栈切换；嵌套中断可重入控制器/驱动路径。[VERIFIED mykernel/arch/x86_64/entry/entry_64.S]
3. 默认未实现异常路径主要打印；对 fault 类异常若返回原 RIP，可能立即再次 fault。[VERIFIED mykernel/arch/x86_64/myos/interrupt.c] [INFERRED]
4. #DF/NMI/#MC 未见 IST 专用栈；当前栈损坏时可能无法可靠诊断。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]
5. IRQ 注册/注销未见完整的向量范围、重复注册、并发发布/撤销保护。[VERIFIED mykernel/arch/x86_64/myos/interrupt.c]

## 5. 异常 error code 的通用字段

### selector 类（#TS/#NP/#SS/#GP）

| 位 | 含义 |
|---:|---|
| 0 | EXT：事件来自处理器外部 |
| 1 | IDT：selector index 指向 IDT |
| 2 | TI：非 IDT 时，1=LDT，0=GDT |
| 15:3 | selector/table index |
| 31:16 | 保留 |

### #PF

见 `paging.md`；CR2 给出 fault linear address。读取 CR2 应在可能发生另一个 page fault 前完成。[EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 6]

## 6. IF 与本地中断原语

`irqflags.h` 提供 `cli/sti`、保存 RFLAGS、恢复 IF 的内联封装。[VERIFIED mykernel/arch/x86_64/include/asm/irqflags.h]

规则：

- `local_irq_save(flags)` 必须保存完整旧 IF 状态；不能无条件在退出时 `sti`。
- `cli` 只屏蔽 maskable interrupts，不屏蔽 NMI、SMI、#MC。
- 在持有仅本 CPU 数据的锁时，若中断处理器也会获取同一锁，必须配合 irqsave。
- 在 SMP 上 `cli` 不是全局锁；其他 CPU 继续执行。
- 中断返回使用 `iretq` 时会恢复 RIP/CS/RFLAGS/RSP/SS（跨特权级时），所有帧字段必须规范。

[EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 6 and Ch. 8]

## 7. 建议的最小验证矩阵

| 测试 | 预期观察 |
|---|---|
| `int3` | #BP 帧、RIP 语义正确，能继续执行 |
| `ud2` | #UD 不应返回无限 fault；应终止当前任务/进入 panic 策略 |
| 用户访问 unmapped page | #PF error code U/S、P、W/R 正确，CR2 正确 |
| 内核 null fault | 可稳定打印且不二次破坏异常栈 |
| 人工外部 IRQ | ack 收到实际 IRQ/vector，而非 0 |
| IRQ 中再触发 IRQ | 嵌套计数、锁和栈不损坏 |
| #DF 注入/坏栈 | 独立 IST 栈可用后才算通过 |
| spurious APIC vector | 不误报设备 IRQ，正确 EOI/统计 |
