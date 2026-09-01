---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "mykernel/arch/x86_64/kernel/fpu/init.c"
  - "mykernel/arch/x86_64/kernel/cpu/common.c"
  - "mykernel/arch/x86_64/processor/processor_types_arch.h"
  - "mykernel/arch/x86_64/sched/context/process64_arch.c"
  - "mykernel/arch/x86_64/sched/context/process_arch.c"
  - "mykernel/arch/x86_64/include/asm/cpufeatures.h"
status: final
open_questions:
  - "用户态是否已实际执行 SSE/AVX 指令且未发生跨任务污染，需要 QEMU 两任务寄存器花样测试。"
  - "编译器对内核自身生成 SSE 指令的选项未在本卡做完整构建审计。"
---

# 速查卡：FPU、SSE/AVX 与 XSAVE 状态

## 1. 三层启用模型

### 硬件枚举

| 能力 | CPUID |
|---|---|
| x87 FPU | `CPUID.1:EDX[0]` |
| FXSR | `CPUID.1:EDX[24]` |
| SSE/SSE2 | `CPUID.1:EDX[25]/[26]` |
| XSAVE | `CPUID.1:ECX[26]` |
| OSXSAVE 状态回报 | `CPUID.1:ECX[27]` |
| AVX | `CPUID.1:ECX[28]` |
| xstate feature set/layout | `CPUID.0D:*` |

[EXTERNAL Intel SDM 253665-092US, Vol. 1, Ch. 13 and Ch. 21]

### OS 控制

| 寄存器位 | 作用 |
|---|---|
| CR0.EM | 1 时浮点指令触发 #UD/仿真语义 |
| CR0.MP | WAIT/FWAIT 与 TS 交互 |
| CR0.TS | 1 时多数 FPU/SIMD 指令触发 #NM |
| CR0.NE | x87 错误使用内部 #MF |
| CR4.OSFXSR | OS 支持 FXSAVE/FXRSTOR |
| CR4.OSXMMEXCPT | OS 支持 #XM |
| CR4.OSXSAVE | OS 支持 XSAVE/XRSTOR 与 XCR0 |
| XCR0 | 哪些 user xstate 组件由 OS 管理 |

### 任务态生命周期

- 为每个任务分配足够、正确对齐的 xstate buffer；
- fork/exec 初始化；
- context switch 保存/恢复，或实现严格 lazy protocol；
- signal frame/ptrace/core dump 序列化；
- kernel 使用 SIMD 时保护当前用户状态。

三层任一缺失都不能声称“支持 AVX/XSAVE”。

## 2. XCR0 常用位

| 位 | 组件 | 依赖/约束 |
|---:|---|---|
| 0 | x87 | 通常必须置 1 |
| 1 | SSE | AVX 前必须置 1 |
| 2 | AVX/YMM upper | 需要 XSAVE+AVX，且位 1 同时置 |
| 3 | MPX BNDREGS | 已废弃/少用，须与 bit4成组 |
| 4 | MPX BNDCSR | 与 bit3成组 |
| 5 | AVX-512 opmask | 与 6、7 及 SSE/AVX 依赖成组 |
| 6 | ZMM_Hi256 | AVX-512 |
| 7 | Hi16_ZMM | AVX-512 |
| 9 | PKRU | PKU 状态 |
| 17 | XTILECFG | AMX tile config |
| 18 | XTILEDATA | AMX tile data，缓冲很大 |

[EXTERNAL Intel SDM 253665-092US, Vol. 1, Ch. 13]

不能按 CPUID feature 名单直接把所有 XCR0 位 OR 上去；必须使用 `CPUID.0D.0:EAX/EDX` 的支持 mask，并满足组件依赖。[EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.0DH]

## 3. CPUID.0D 结构

| 子叶 | 关键返回 |
|---|---|
| 0 | XCR0 支持位图、当前 enabled user state 所需大小、所有支持 user state 最大大小 |
| 1 | XSAVEOPT/XSAVEC/XSAVES/XFD 等能力；supervisor state 位图的一部分 |
| n>=2 | 对应 xstate component 的 size、offset、alignment/compact-format 属性 |

MyOS2 仅把 subleaf 1 EAX 存入 capability word，没有读取 subleaf 0 的 mask/尺寸，也未遍历组件。因此无法据此分配安全的 XSAVE buffer。[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]

## 4. MyOS2 当前实现

`fpu__init_system_early_generic()` 检查 x87 能力并调整 CR0.EM/TS；CR4.OSFXSR/OSXMMEXCPT、XSAVE system init、XCR0 和 signal frame 大多被注释。[VERIFIED mykernel/arch/x86_64/kernel/fpu/init.c]

`thread_struct` 中 FPU 成员被注释；`__switch_to()` 的 FPU prepare/finish 被注释；`copy_thread()` 的 FPU clone 被注释。[VERIFIED mykernel/arch/x86_64/processor/processor_types_arch.h] [VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c] [VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c]

结论：所读快照只完成“最低限度探测与 CR0 处理”，未形成多任务 FPU/SIMD 状态隔离。状态为 `partial/high-risk`。

## 5. 立即风险

1. **跨任务泄漏/破坏。** 若用户能执行 x87/SSE/AVX，而内核不切换状态，任务 A 的寄存器可被 B 读取或覆盖。[INFERRED]
2. **内核编译器生成 SIMD。** 即使源代码未显式使用，错误编译选项也可能让编译器生成 XMM 指令；在无保存协议的内核上下文中会破坏用户状态。[INFERRED]
3. **#NM 无恢复。** CR0.TS 若被置起而 #NM handler 未接 lazy restore，任务会失败/循环异常。[VERIFIED mykernel/arch/x86_64/myos/interrupt.c]
4. **AVX enable 条件。** 仅 CPUID.AVX=1 不足；XSAVE、OSXSAVE、XCR0[2:1] 都需满足。
5. **buffer size。** 用固定 FXSAVE 512 字节承载 AVX-512/AMX 会越界；必须按 CPUID.0D。
6. **XSAVE header 清理。** 新任务/exec 必须清除未初始化组件与 XSTATE_BV，避免旧状态泄漏。
7. **PKRU。** 若未来启用 PKU，PKRU 属于任务状态，当前切换路径没有保存恢复。

## 6. 建议的最小实现顺序

1. 构建层禁止内核自动使用 MMX/SSE/AVX（除受控 helper）。
2. 完成 CPUID.0D.0/1/组件遍历，计算 `xfeatures_mask`、标准/压缩尺寸和对齐。
3. 按能力置 CR4.OSFXSR、OSXMMEXCPT、OSXSAVE。
4. 用 `XSETBV` 只开启已支持且依赖闭合的最小集合（先 x87+SSE，后 AVX）。
5. 为每任务分配并初始化 xstate。
6. 先实现 eager save/restore；正确后再考虑 lazy。
7. fork/exec/signal/ptrace 逐项接入。
8. 添加 kernel_fpu_begin/end 风格保护，或继续严格禁止内核 SIMD。
9. 最后才开放 AVX-512/AMX 等大状态。

## 7. 保存指令选择

| 指令族 | 覆盖 | 注意 |
|---|---|---|
| FNSAVE/FRSTOR | 旧 x87 | 会改变/初始化状态，现代不推荐 |
| FXSAVE/FXRSTOR | x87+MMX+SSE | 固定 512B；依赖 CR4.OSFXSR |
| XSAVE/XRSTOR | XCR0 user components | 按 requested-feature bitmap |
| XSAVEOPT | 优化未修改组件 | CPUID.0D.1[0] |
| XSAVEC | compact layout | offset 与标准布局不同 |
| XSAVES/XRSTORS | user+supervisor state | 更严格特权与 XSS 规则 |

[EXTERNAL Intel SDM, Vol. 2, FXSAVE/XSAVE instruction reference]

实现时必须把“buffer 格式”作为 ABI：不能用 XSAVEC 保存后按标准 XSAVE offset 解释。

## 8. 最小测试

1. 两用户任务分别在 XMM/YMM 写不同 pattern，百万次切换后各自不变。
2. fork 后子任务初始状态符合 ABI，修改不影响父。
3. exec 后浮点环境重置，无旧寄存器泄漏。
4. #NM 路径（若 lazy）只恢复当前任务且可重入保护。
5. CPUID.0D 模拟不同 feature masks/size，分配不越界。
6. AVX 指令在 OSXSAVE/XCR0 未满足时被拒绝，而不是误执行。
7. signal handler 修改 FP state 后 sigreturn 正确。
8. 内核中断发生在用户 AVX 运算中，不破坏寄存器。
9. 开启 AMX 前验证大 buffer、quota/清零与 context-switch 成本。
