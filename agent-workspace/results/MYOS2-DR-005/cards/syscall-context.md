---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "mykernel/arch/x86_64/entry/entry_64.S"
  - "mykernel/arch/x86_64/entry/common.c"
  - "mykernel/arch/x86_64/entry/syscall_64.c"
  - "mykernel/arch/x86_64/kernel/cpu/common.c"
  - "mykernel/arch/x86_64/sched/context/process64_arch.c"
  - "mykernel/arch/x86_64/sched/context/process_arch.c"
  - "mykernel/arch/x86_64/sched/context/switch_to_arch.h"
  - "mykernel/arch/x86_64/sched/context/switch_to_types_arch.h"
  - "mykernel/sched/scheduler/scheduler_core.c"
status: final
open_questions:
  - "CONFIG_INTEL_X64_GDT_LAYOUT 的实际值与 SYSENTER 分支是否可链接尚未确认。"
  - "系统调用号越界时最终用户可见返回值需运行时确认；C 路径未见明确 -ENOSYS 写入。"
  - "从内核线程/用户进程切换时 FS/GS base 的真实硬件值需寄存器跟踪验证。"
---

# 速查卡：SYSCALL/SYSRET 与任务上下文切换

## 1. SYSCALL 硬件语义

在 64 位模式下执行 `SYSCALL`：

- `RCX ← user RIP`；
- `R11 ← user RFLAGS`；
- `RIP ← IA32_LSTAR`；
- `CS/SS` 从 `IA32_STAR` 派生；
- `RFLAGS ← user RFLAGS & ~IA32_FMASK`；
- **硬件不会自动切换 RSP**；
- 不会自动保存通用寄存器。

因此入口必须在可信 per-CPU/CPU entry state 中保存 user RSP，并切到当前任务内核栈。[EXTERNAL Intel SDM, Vol. 2, SYSCALL; Vol. 3A, MSR/64-bit entry]

## 2. STAR/LSTAR/SFMASK

| MSR | 当前写入 |
|---|---|
| STAR | kernel CS 基值与 user 32-bit selector 基值 |
| LSTAR | `entry_SYSCALL_64` |
| SFMASK | 清算术 flags、TF、IF、DF、IOPL、NT、RF、AC、ID |
| CSTAR | helper 存在，主初始化中被注释 |
| SYSENTER trio | 条件编译分支写入；活动配置未确认 |

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]

STAR 的字段是 selector **基值**，SYSRET 会按架构规则加固定偏移得到用户 CS/SS；GDT 布局必须与这些偏移精确匹配。[EXTERNAL Intel SDM, Vol. 3A, IA32_STAR]

## 3. MyOS2 SYSCALL 路径

```text
user SYSCALL
  -> entry_SYSCALL_64
     swapgs
     per_cpu(TSS.sp2) <- user RSP
     RSP <- current kernel stack
     build pt_regs
     call do_syscall_64(regs, nr)
        -> syscall table dispatch
     restore
     swapgs
     sysretq
```

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S] [VERIFIED mykernel/arch/x86_64/entry/common.c] [VERIFIED mykernel/arch/x86_64/entry/syscall_64.c]

### 当前确定缺口

1. `do_syscall_64()` 计算“能否安全 SYSRET”的布尔值，但汇编没有依据返回值分支，最终无条件 `sysretq`；未见 IRET 慢路径。[VERIFIED mykernel/arch/x86_64/entry/entry_64.S] [VERIFIED mykernel/arch/x86_64/entry/common.c]
2. 无效 syscall 号路径未见明确把 `regs->ax` 写成 `-ENOSYS`。[VERIFIED mykernel/arch/x86_64/entry/common.c]
3. tracing、seccomp、audit、ptrace、restart、signal work、RCU/context tracking 等入口/退出层大多被注释。[VERIFIED mykernel/arch/x86_64/entry/common.c]
4. CSTAR/compat 和 SYSENTER 安全禁用/入口不完整。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]
5. `swapgs` 缺少 NMI/paranoid entry 级别的来源判定；普通 SYSCALL 可工作不代表任意异常嵌套安全。[VERIFIED mykernel/arch/x86_64/entry/entry_64.S] [INFERRED]

## 4. 为什么 SYSRET 需要安全判定

`SYSRETQ` 对返回地址、RFLAGS 和 selector 有严格限制；常见必须回退 IRET 的情况包括：

- RCX 不是 canonical user address；
- R11 含不能由 SYSRET 安全恢复的状态；
- 需要返回 compat/非标准 selector；
- 单步/调试、信号或工作标志要求慢路径；
- CPU errata/安全策略要求 IRET。

错误使用 SYSRET 可导致 #GP 在特权级/栈状态不利的位置发生。正确结构是：

```text
if (sysret_safe(regs))
    fast SYSRETQ
else
    restore full IRET frame
    IRETQ
```

[EXTERNAL Intel SDM, Vol. 2, SYSRET; Vol. 3A, 64-bit mode exceptions]

## 5. `pt_regs` 与 ABI

MyOS2 汇编把系统调用参数/保存寄存器整理成 C 可见 `pt_regs`，系统调用号来自 `RAX`，返回值写回 `RAX`。[VERIFIED mykernel/arch/x86_64/entry/entry_64.S] [VERIFIED mykernel/arch/x86_64/entry/common.c]

Linux x86-64 syscall 约定通常为：

| 角色 | 寄存器 |
|---|---|
| syscall number | RAX |
| arg1..6 | RDI, RSI, RDX, R10, R8, R9 |
| return | RAX |
| hardware clobber | RCX, R11 |

MyOS2 的 syscall table/调用层应以实际用户 libc 与 `entry_64.S` 为准，不能误用 SysV C 函数第 4 参数 RCX。[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c] [EXTERNAL x86-64 syscall ABI]

## 6. 调度到汇编切换

```text
schedule()
  -> context_switch(prev, next)
  -> switch_to(prev, next, last)
     -> __switch_to_asm
        push r15,r14,r13,r12,rbx,rbp
        prev->thread.sp = rsp
        rsp = next->thread.sp
        pop ...
        jmp __switch_to(prev,next)
```

[VERIFIED mykernel/sched/scheduler/scheduler_core.c] [VERIFIED mykernel/arch/x86_64/sched/context/switch_to_arch.h] [VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c]

`inactive_task_frame` 字段顺序与汇编 push/pop 必须严格一致。[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_types_arch.h]

## 7. 当前保存/恢复矩阵

| 状态 | 保存/恢复 | 证据/缺口 |
|---|---|---|
| R15/R14/R13/R12/RBX/RBP | 是 | `__switch_to_asm` |
| RSP | 是 | `thread.sp` |
| RIP | 间接是 | inactive frame return address |
| current task pointer | 是 | `__switch_to()` |
| kernel stack/TSS.sp0 | 是 | next top-of-stack |
| DS/ES selectors | 部分 | C 层保存/加载 |
| FS/GS selectors | 部分 | 字段存在；流程不完整 |
| FS base | **未完整恢复** | `x86_fsgsbase_load()` 主体注释 |
| GS base/user GS | **未完整恢复** | 同上；还受 SWAPGS 约束 |
| TLS descriptors | 否 | `tls_array` 与 load_TLS 注释 |
| FPU/SIMD/xstate | 否 | switch hooks 注释 |
| PKRU | 否 | 字段/切换注释 |
| debug registers/perf | 否 | 字段/切换注释 |
| I/O bitmap | 否 | 注释 |
| address space/TLB | 通用 scheduler/mm 层部分负责 | 本卡未证明 PCID/完整 TLB switch |
| speculation controls | 否 | 注释 |

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c] [VERIFIED mykernel/arch/x86_64/processor/processor_types_arch.h]

## 8. 新任务/fork frame

`copy_thread()` 在新任务内核栈构造 `fork_frame`：

- inactive frame 的 return address 指向 fork 返回 trampoline；
- 复制父 `pt_regs`；
- 子返回值置 0；
- 保存 FS/GS selector/base 快照；
- kernel thread 与 user task 路径分开设置入口/参数。

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c]

缺口：

- `CLONE_SETTLS` 处理被注释；
- FPU state clone 被注释；
- I/O bitmap clone 被注释；
- FS/GS 虽写入 thread 字段，后续 load 函数为空，不能形成端到端保证。

## 9. 用户/内核 GS 状态机

常见安全模型：

| 运行状态 | GS base | KERNEL_GS_BASE |
|---|---|---|
| user | user GS | per-CPU GS |
| after `swapgs` in kernel | per-CPU GS | user GS |

入口必须只在从 user 进入时 swap；从 kernel 发生的中断若再 swap 会反转到用户 GS。退出也必须与入口配对。NMI 可以在任意时刻到达，因此需要通过 saved CS、MSR/GS state 或 paranoid protocol 判断。[EXTERNAL Intel SDM, SWAPGS instruction and 64-bit entry guidance]

MyOS2 普通 SYSCALL 使用 `swapgs`，但异常/NMI 的完整状态机未见。[VERIFIED mykernel/arch/x86_64/entry/entry_64.S]

## 10. 最小验证

1. syscall 参数 1–6 与返回值 ABI 测试。
2. syscall number 越界应返回 `-ENOSYS`。
3. 构造非 canonical RCX，确认走 IRET 而非内核 #GP。
4. 单步 TF、signal pending、不同 selector 的退出测试。
5. 两个任务循环切换不同 FS base/GS base，用户读取必须保持各自值。
6. AVX/SSE 寄存器跨任务污染测试（在 FPU 支持完成前应禁止用户使用或明确失败）。
7. 内核线程↔用户任务切换，检查 TSS.sp0/current/GS。
8. NMI/IRQ 在 SYSCALL 入口不同指令点注入，检查 swapgs 配对。
