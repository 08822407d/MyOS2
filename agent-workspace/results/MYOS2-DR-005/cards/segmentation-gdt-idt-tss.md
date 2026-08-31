---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "mykernel/arch/x86_64/processor/desc_arch.h"
  - "mykernel/arch/x86_64/processor/desc_types_arch.h"
  - "mykernel/arch/x86_64/processor/desc_const_arch.h"
  - "mykernel/arch/x86_64/processor/processor_types_arch.h"
  - "mykernel/arch/x86_64/kernel/idt.c"
  - "mykernel/arch/x86_64/kernel/cpu/common.c"
  - "mykernel/arch/x86_64/smp/percpu_area.c"
  - "mykernel/arch/x86_64/sched/context/process64_arch.c"
status: final
open_questions:
  - "TSS.io_bitmap_base 的实际初始化值和 TSS descriptor limit 需运行时 dump 验证。"
  - "各异常 IDT gate 的 IST/DPL/type 最终值需从构建出的 IDT 二进制核对。"
---

# 速查卡：分段、GDT、IDT 与 TSS

## 1. 64 位模式仍然需要哪些分段结构

IA-32e 64-bit mode 中，普通代码/数据段的 base/limit 大多不参与线性地址形成，但以下结构仍是硬依赖：

- `CS` 的 L 位、DPL 和 type 决定 64 位代码与特权；
- `SS/DS/ES` 的 selector/权限仍参与部分检查；
- `FS.base`、`GS.base` 仍是有效 64 位基址，通常由 MSR/FSGSBASE 管理；
- GDT 提供代码/数据/TSS descriptor；
- IDT 提供中断/异常门；
- TSS 提供特权级栈 `RSP0..2`、IST1..7 和 I/O permission bitmap。

[EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 3, Ch. 5–7]

## 2. 普通 8 字节 segment descriptor

| 字段 | 宽度 | 含义 |
|---|---:|---|
| limit 15:0 | 16 | 段 limit 低位 |
| base 15:0 | 16 | base 低位 |
| base 23:16 | 8 | base 中位 |
| type | 4 | code/data/system subtype |
| S | 1 | 1=code/data，0=system |
| DPL | 2 | descriptor privilege |
| P | 1 | present |
| limit 19:16 | 4 | limit 高位 |
| AVL | 1 | OS 可用 |
| L | 1 | 64-bit code segment |
| D/B | 1 | default operand/stack size；64-bit code 中须按规范 |
| G | 1 | limit 粒度 |
| base 31:24 | 8 | base 高位 |

MyOS2 在 `desc_arch.h` 中封装 descriptor 打包、GDT/IDT 装载和 TSS descriptor 设置。[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h]

### 常用 type

| 类别 | type | 说明 |
|---|---:|---|
| data, read/write | `0x2`（加 accessed 后 bit0） | 内核/用户数据段 |
| code, execute/read | `0xA`（加 accessed 后 bit0） | 内核/用户代码段 |
| 64-bit available TSS | `0x9` | `LTR` 前应为 available |
| 64-bit busy TSS | `0xB` | 硬件置 busy 语义 |
| interrupt gate | `0xE` | IDT |
| trap gate | `0xF` | IDT |

[EXTERNAL Intel SDM 253668-092US, Vol. 3A, descriptor tables]

## 3. 64 位 TSS descriptor

64 位 TSS descriptor 占连续两个 GDT slot（16 字节）。除普通 system descriptor 字段外，第二个 8 字节包含 base[63:32]，其余保留位必须为 0。limit 应覆盖硬件 TSS 和允许的 I/O bitmap 范围。[EXTERNAL Intel SDM 253668-092US, Vol. 3A, Ch. 7]

MyOS2：

1. 为每 CPU 分配/定义 `cpu_tss_rw`；
2. `set_tss_desc(cpu, &per_cpu(cpu_tss_rw, cpu).x86_tss)` 写 GDT；
3. `load_TR_desc()` 执行 `ltr`；
4. 任务切换更新 `x86_tss.sp0`；
5. SYSCALL 入口借用 `sp2` 保存用户 RSP。

[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c] [VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] [VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c] [VERIFIED mykernel/arch/x86_64/entry/entry_64.S]

## 4. 64 位硬件 TSS 布局

| 字段 | 用途 | MyOS2 |
|---|---|---|
| reserved1 | 保留 | 结构中存在 |
| `sp0` | CPL3→CPL0 等特权切换的内核栈 | 任务切换更新 |
| `sp1` | CPL→1 栈 | 未使用 |
| `sp2` | CPL→2 栈；Linux/MyOS2 可借作入口 scratch | SYSCALL 保存 user RSP |
| reserved2 | 保留 | — |
| `ist[0..6]` | IST1..IST7 栈顶 | 结构存在，初始化被注释 |
| reserved3/4/5 | 保留 | — |
| `io_bitmap_base` | 相对 TSS base 的 I/O bitmap offset | setup 被注释 |
| bitmap | 每个 I/O port 一位，1=拒绝 | `tss_struct` 中有存储，未启用 |

[VERIFIED mykernel/arch/x86_64/processor/processor_types_arch.h]

若 `io_bitmap_base` 超出 TSS limit，CPL3 的端口 I/O 按 IOPL/权限规则触发 #GP；MyOS2 定义 `IO_BITMAP_OFFSET_INVALID`，但未见完整任务级 bitmap 切换。[VERIFIED mykernel/arch/x86_64/processor/processor_const_arch.h]

## 5. IDT gate

64 位 IDT gate 也是 16 字节，字段见 `interrupts-exceptions.md`。MyOS2 的全局 `idt_table` 页对齐，`trap_init()`/`idt_setup_traps()` 从汇编 handler 数组生成异常门，IRQ 区从 `irq_entries_start` 生成。[VERIFIED mykernel/arch/x86_64/kernel/idt.c] [VERIFIED mykernel/arch/x86_64/kernel/traps.c]

当前强化缺口：

- CPU entry area/只读 IDT 映射被注释；
- #DF/NMI/#MC 等 IST 栈 setup 被注释；
- 未见 guard page/stack overflow 专用入口；
- gate 的用户 DPL 仅应开放明确的软件入口（例如 #BP 或 int80），不能统一 DPL3。

[VERIFIED mykernel/arch/x86_64/kernel/idt.c] [VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]

## 6. GDT、GDTR、LDTR、TR

| 寄存器/指令 | 作用 | MyOS2 |
|---|---|---|
| GDTR / `lgdt` | GDT base + limit | `load_direct_gdt()` |
| IDTR / `lidt` | IDT base + limit | `load_current_idt()` |
| LDTR / `lldt` | LDT selector | 未见活动 LDT |
| TR / `ltr` | TSS selector | `load_TR_desc()` |
| `sgdt/sidt/str` | 读 descriptor state | UMIP 未启用时用户态可受规则影响 |
| far return/jump | 重载 CS | 段刷新 helper |
| `mov` to DS/ES/SS | 重载数据 selector | context switch/初始化 |

[VERIFIED mykernel/arch/x86_64/processor/desc_arch.h] [VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]

## 7. 当前 per-CPU 关系

```text
per_cpu area
  ├─ current task pointer / runqueue
  ├─ cpu_gdt_rw
  ├─ cpu_tss_rw
  └─ other arch state

GS base
  └─ points at current CPU's per_cpu base

GDT.TSS descriptor
  └─ points at that CPU's cpu_tss_rw

TR
  └─ caches TSS descriptor

context switch
  └─ updates TSS.sp0 to next task's top of kernel stack
```

[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c] [VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c]

## 8. 易错点

1. TSS descriptor 是 16 字节，不是普通 8 字节 descriptor；相邻 slot 不得复用。
2. `LTR` 会缓存 descriptor；修改 GDT 中 TSS base 后必须重新加载 TR。
3. IST index 0 表示不使用 IST，1–7 对应数组项 0–6。
4. IDT gate selector 必须指向 present 64-bit code segment。
5. 64 位下不能依赖 DS/ES base 做地址隔离；FS/GS base 才是有效基址机制。
6. `SWAPGS` 的安全性依赖入口来源与嵌套状态；NMI 不能机械地无条件 swap。
7. I/O bitmap 的 limit/base 必须同时正确；仅分配数组不等于已启用。
8. `sp0` 必须在允许用户返回前指向当前任务的有效内核栈，否则下一次特权切换会写坏内存。

## 9. 最小验证

- 启动后 dump GDTR/IDTR/TR、GDT TSS 16 字节和 TSS base/limit；
- 对每 CPU 检查 GS base、GDT、TSS、`sp0` 是否独立；
- 触发用户→内核中断，确认使用 next task 的 `sp0`；
- 建立 #DF IST 后故意制造坏栈，确认仍能进入 handler；
- 用户执行 `inb`，确认无授权 bitmap 时 #GP；
- 将 IDT 页改为只读后运行设备 IRQ 压力测试。
