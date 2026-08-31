---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "mykernel/arch/x86_64/insns/x86msr_const.h"
  - "mykernel/arch/x86_64/insns/x86msr.h"
  - "mykernel/arch/x86_64/processor/processor_const_arch.h"
  - "mykernel/arch/x86_64/kernel/cpu/common.c"
  - "mykernel/arch/x86_64/kernel/apic/apic.c"
  - "mykernel/arch/x86_64/kernel/myos_APboot.S"
  - "mykernel/arch/x86_64/kernel/fpu/init.c"
  - "mykernel/arch/x86_64/entry/entry_64.S"
status: final
open_questions:
  - "BSP 进入 start_kernel 前 CR0/CR4/EFER 的最终值未做运行时转储。"
  - "MSR_PLATFORM_INFO/MSR_FSB_FREQ 是否在所有目标 CPU 上安全可读，需结合 CPUID family/model 和异常安全读接口确认。"
---

# 速查卡：MSR、控制寄存器与 RFLAGS

## 1. 原则

- CR0/CR4/EFER 的保留位不能按“读出后随意重写”处理；应使用明确 mask，且先检查对应 CPUID 能力。[EXTERNAL Intel SDM 253668-092US, Vol. 3A, system control registers]
- `RDMSR/WRMSR` 访问未实现的 MSR 会触发 `#GP(0)`；模型相关 MSR 必须由 CPUID、family/model 或安全异常包装保护。[EXTERNAL Intel SDM 335592-092US, Vol. 4]
- “头文件定义了位”与“当前 CPU 支持/内核已启用”必须分栏记录。[VERIFIED mykernel/arch/x86_64/insns/x86msr_const.h]

## 2. CR0 完整架构位

| 位 | 名称 | 作用 | MyOS2 当前触点 |
|---:|---|---|---|
| 0 | PE | Protected Mode Enable | 启动汇编/长模式前置 |
| 1 | MP | Monitor Coprocessor | 定义在 `CR0_STATE`；FPU 完整策略未实现 |
| 2 | EM | Emulation | 无 x87 时置 1；有 x87 时清 0 |
| 3 | TS | Task Switched | FPU 初始化清 0；未实现 lazy-FPU 切换 |
| 4 | ET | Extension Type | 固定历史位，`CR0_STATE` 包含 |
| 5 | NE | Numeric Error | `CR0_STATE` 包含 |
| 6–15 | Reserved | 保留 | 保持规范值 |
| 16 | WP | Ring 0 也受页表 R/W 保护 | `CR0_STATE` 包含；应在启动后保持 1 |
| 17 | Reserved | 保留 | — |
| 18 | AM | Alignment Mask | `CR0_STATE` 包含 |
| 19–28 | Reserved | 保留 | — |
| 29 | NW | Not Write-through | 仅缓存控制；不应与 CD 形成非法组合 |
| 30 | CD | Cache Disable | 仅缓存控制 |
| 31 | PG | Paging Enable | 长模式启动所需 |

[VERIFIED mykernel/arch/x86_64/processor/processor_const_arch.h] [VERIFIED mykernel/arch/x86_64/kernel/fpu/init.c] [EXTERNAL Intel SDM 253668-092US, Vol. 3A, CR0 definition]

## 3. CR3 格式

| 条件 | 低位解释 | 高位解释 |
|---|---|---|
| `CR4.PCIDE=0`，长模式分页 | 位 3=PWT，位 4=PCD；其余低 12 位按规范保留/忽略 | 物理地址宽度内为 PML4（或 PML5）基址 |
| `CR4.PCIDE=1` | 位 11:0=PCID | 位 63 写 1 可请求保留该 PCID 的 TLB 条目；其余为顶级表基址 |
| 内存加密 | 厂商定义的地址位可能兼作 C-bit | MyOS2 `_PAGE_ENC=0`，未实现 |

MyOS2 定义 `CR3_ADDR_MASK`、`CR3_PCID_MASK` 与 `CR3_NOFLUSH`，但所读快照中未见完整 PCID 分配/回收和 INVPCID 策略。[VERIFIED mykernel/arch/x86_64/processor/processor_const_arch.h] [EXTERNAL Intel SDM 253668-092US, Vol. 3A, Paging]

## 4. CR4 完整当前常用位

| 位 | 名称 | 含义 | MyOS2 |
|---:|---|---|---|
| 0 | VME | vm86 extensions | 未使用 |
| 1 | PVI | Protected-mode virtual interrupts | 未使用 |
| 2 | TSD | CPL>0 禁止 RDTSC/RDTSCP | 未见策略 |
| 3 | DE | Debugging Extensions | 未见策略 |
| 4 | PSE | 大页扩展 | 启动/页表相关 |
| 5 | PAE | 物理地址扩展；IA-32e 必需 | AP 启动设置 |
| 6 | MCE | Machine Check Enable | 未见 MCE 初始化 |
| 7 | PGE | Global pages | AP 启动设置 |
| 8 | PCE | CPL>0 可读 perf counter | 未使用 |
| 9 | OSFXSR | OS 支持 FXSAVE/FXRSTOR | 启用代码被注释 |
| 10 | OSXMMEXCPT | OS 支持未屏蔽 SIMD 异常 | 启用代码被注释 |
| 11 | UMIP | 限制 SGDT/SIDT/SLDT/SMSW/STR | `setup_umip` 被注释 |
| 12 | LA57 | 5 级分页 | 未启用 |
| 13 | VMXE | VMX enable | 未启用 |
| 14 | SMXE | SMX enable | 未启用 |
| 15 | Reserved | 保留 | — |
| 16 | FSGSBASE | 允许 RDFSBASE/RDGSBASE/WRFSBASE/WRGSBASE | AP 汇编无条件置位；BSP 按 CPUID 启用逻辑被注释 |
| 17 | PCIDE | PCID enable | 未启用 |
| 18 | OSXSAVE | OS 使用 XSAVE/XRSTOR/XCR0 | 启用代码被注释 |
| 19 | Reserved | 保留 | — |
| 20 | SMEP | supervisor 禁止执行 user 页 | 启用被注释 |
| 21 | SMAP | supervisor 默认禁止访问 user 页 | 启用被注释 |
| 22 | PKE | user protection keys | 未启用；页表 pkey 宏为 0 |
| 23 | CET | Control-flow Enforcement | 所读常量头未定义/未用 |
| 24 | PKS | supervisor protection keys | 未使用 |
| 25 | UINTR | user interrupts | 未使用 |
| 26–63 | Reserved/current extensions | 按当期 SDM | 未使用 |

[VERIFIED mykernel/arch/x86_64/processor/processor_const_arch.h] [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S] [VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] [EXTERNAL Intel SDM 253668-092US, Vol. 3A, CR4 definition]

**陷阱：** AP 入口直接 OR `CR4.FSGSBASE`，没有先检查 CPUID.07H.0:EBX[0]；在不支持该位的 CPU 上写 CR4 保留位可能导致 `#GP`。BSP 又未见等价启用，形成 CPU 间状态不对称。[VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S] [INFERRED]

## 5. EFER

| 位 | Intel 名称 | 作用 | MyOS2 |
|---:|---|---|---|
| 0 | SCE | 启用 SYSCALL/SYSRET | `syscall_init()` 依赖；需启动时已置位 |
| 1–7 | Reserved | 保留 | — |
| 8 | LME | Long Mode Enable | 启动路径所需 |
| 9 | Reserved | 保留 | — |
| 10 | LMA | Long Mode Active，只读 | 可用于诊断 |
| 11 | NXE | 允许页表 XD/NX 位 | 页表大量使用 NX；BSP 置位链待核对 |
| 12 | SVME（AMD） | SVM enable | Intel 保留；MyOS2 未启用 |
| 13 | LMSLE（AMD） | Long-mode segment-limit enable | 未使用 |
| 14 | FFXSR（AMD） | Fast FXSAVE/FXRSTOR | 未使用 |
| 15 | TCE（部分 AMD） | Translation cache extension | MyOS2 常量未定义 |
| 21 | AUTOIBRS（AMD） | automatic IBRS | 仅常量定义，未启用 |

[VERIFIED mykernel/arch/x86_64/insns/x86msr_const.h] [EXTERNAL Intel SDM 335592-092US, Vol. 4, IA32_EFER] [EXTERNAL AMD APM 24593 Rev. 3.44, Vol. 2, EFER]

## 6. 当前实际使用的 MSR

| MSR | 编号 | 关键字段/语义 | MyOS2 位置与状态 |
|---|---:|---|---|
| `IA32_APIC_BASE` | `0x1B` | bit 8 BSP（只读）；bit 10 x2APIC enable；bit 11 APIC global enable；xAPIC 模式下高位含 APIC 基址 | APIC 探测与模式切换 [VERIFIED mykernel/arch/x86_64/kernel/apic/apic.c] |
| `IA32_SYSENTER_CS` | `0x174` | SYSENTER 内核 CS | 仅条件编译分支写入 |
| `IA32_SYSENTER_ESP` | `0x175` | SYSENTER 内核栈 | 仅条件编译分支写入 |
| `IA32_SYSENTER_EIP` | `0x176` | SYSENTER 入口 | 仅条件编译分支写入；对应汇编实现未确认 |
| `EFER` | `0xC0000080` | SCE/LME/LMA/NXE 等 | 常量与启动相关 |
| `STAR` | `0xC0000081` | SYSCALL 内核 CS 在 47:32；SYSRET 用户 selector 基值在 63:48 | `syscall_init()` 写入 |
| `LSTAR` | `0xC0000082` | 64 位 SYSCALL RIP | 指向 `entry_SYSCALL_64` |
| `CSTAR` | `0xC0000083` | compat SYSCALL RIP | 写入函数存在但调用被注释 |
| `SFMASK` | `0xC0000084` | SYSCALL 时清除指定 RFLAGS 位 | 清 CF/PF/AF/ZF/SF/TF/IF/DF/OF/IOPL/NT/RF/AC/ID |
| `FS_BASE` | `0xC0000100` | 64 位 FS base | 初始化清零、任务快照读取 |
| `GS_BASE` | `0xC0000101` | 当前 GS base | per-CPU/任务相关 |
| `KERNEL_GS_BASE` | `0xC0000102` | SWAPGS 影子 base | 初始化清零、入口切换 |
| `TSC_AUX` | `0xC0000103` | RDTSCP/RDPID auxiliary value | 常量存在；未见完整每 CPU 编程 |
| x2APIC MSR window | `0x800–0x8FF` | APIC 寄存器以 offset/16 编码为 MSR | x2APIC read/write 路径 |
| `MSR_FSB_FREQ` | `0xCD` | 模型相关总线比率 | TSC 校准候选；需模型门控 |
| `MSR_PLATFORM_INFO` | `0xCE` | 模型相关平台比率 | TSC 校准候选；需模型门控 |

[VERIFIED mykernel/arch/x86_64/insns/x86msr_const.h] [VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] [VERIFIED mykernel/arch/x86_64/kernel/tsc.c] [EXTERNAL Intel SDM 335592-092US, Vol. 4]

## 7. `SFMASK` 清除的 RFLAGS

| 位 | 名称 | 为什么入口通常清除 |
|---:|---|---|
| 0 | CF | 不继承用户算术状态 |
| 2 | PF | 同上 |
| 4 | AF | 同上 |
| 6 | ZF | 同上 |
| 7 | SF | 同上 |
| 8 | TF | 防止用户单步直接穿入普通内核入口 |
| 9 | IF | 内核显式决定何时开中断 |
| 10 | DF | 保证字符串操作方向为递增 |
| 11 | OF | 不继承用户算术状态 |
| 12–13 | IOPL | 不让用户影响内核 I/O 特权语义 |
| 14 | NT | 禁止旧任务嵌套语义 |
| 16 | RF | 不继承 resume |
| 18 | AC | 配合 SMAP/对齐策略 |
| 21 | ID | 不继承 CPUID 检测修改状态 |

`VM/VIF/VIP` 在 64 位 SYSCALL 主路径中不是当前掩码项。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c] [EXTERNAL Intel SDM 253668-092US, Vol. 3A, EFLAGS/RFLAGS]

## 8. 安全使用清单

1. 所有 `WRMSR` 前确定 MSR 存在；模型相关项用 family/model 表或 safe wrapper。
2. 写 CR4 时用 `new = (old & ~clear) | set`，但 `set` 只能来自已验证能力；当前 `cr4_update_irqsoff()` 是空壳。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]
3. 启用 NX 的顺序：确认 CPUID NX → 置 EFER.NXE → 才允许任何 present PTE 置 NX。
4. 启用 XSAVE 的顺序：CPUID.XSAVE → CR4.OSXSAVE → 根据 CPUID.0D 计算允许的 XCR0 → `XSETBV` → 建立任务态缓冲。
5. 切 x2APIC 前确认 local APIC 和 x2APIC 能力，并遵守 IA32_APIC_BASE 模式转换约束。
6. `SWAPGS` 只在 GS 当前确为用户态值时执行；NMI/嵌套异常需 paranoid entry 语义，当前入口未完整实现。
