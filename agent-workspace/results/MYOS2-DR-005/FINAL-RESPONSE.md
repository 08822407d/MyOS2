---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "agent-workspace/results/MYOS2-DR-005/MANIFEST.md"
  - "agent-workspace/results/MYOS2-DR-005/00-usage-census.md"
  - "agent-workspace/results/MYOS2-DR-005/cpuid-atlas.md"
  - "agent-workspace/results/MYOS2-DR-005/open_questions.md"
  - "GitHub pull request #5"
status: final
open_questions: []
---

> 本文件保存上一条助手回复的完整正文；对话界面的 `filecite` 标记按原样保留。

# MYOS2-DR-005 已完成

**PR：#5 `MYOS2-DR-005: x86 reference pack`**，当前状态为开放、非草稿且可合并；目标分支为 `master`，来源分支为 `agent/MYOS2-DR-005`。fileciteturn173file0L2-L16

| 项目 | 结果 |
|---|---|
| 分析源码快照 | `time @ a039d9803ade2a1613d620bda375e028530d5242` |
| 工作区基线 | `master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719` |
| 任务分支 | `agent/MYOS2-DR-005` |
| 提交 | `bfc977ceb931588ed30b970213a89e97a82fa69e` |
| 唯一写入目录 | `agent-workspace/results/MYOS2-DR-005/` |
| PR 变更 | 1 个提交、15 个新增文件、2498 行新增、0 行删除 fileciteturn173file0L32-L37 |

## 交付物

共提交 15 个文件：

- `MANIFEST.md`
- `00-usage-census.md`
- 11 张机制速查卡：CPUID、MSR/控制寄存器、分页、异常与中断、GDT/IDT/TSS、APIC/IOAPIC、计时器、端口 I/O、系统调用与上下文切换、原子与锁、FPU/xstate
- `cpuid-atlas.md`
- `open_questions.md`

MANIFEST 已逐项记录覆盖状态、明确未做事项和后续消费方式，并明确声明本任务没有编译或启动内核。fileciteturn168file0L2-L6  
CPUID 全景表覆盖基本叶 `0x0–0x1F`、常用 `0x80000000+` 扩展叶和 hypervisor 范围，并区分 `direct`、`stored`、`transient`、`stub`、`unused` 与 `reserved`。fileciteturn169file0L2-L6  
所有不能靠静态源码可靠回答的内容已集中为 34 个带影响、证据和最小验证动作的问题。fileciteturn170file0L2-L6

## 主要结论

1. **原子与锁存在两个确定的正确性错误。**  
   `arch_spin_trylock()` 在锁空闲时直接返回成功，却没有通过 CAS 或 ticket 增量真正取得锁；`arch_atomic_add_test_negative()` 名称与注释表示加法，汇编却执行 `subl`。

2. **系统调用快速返回路径不完整。**  
   C 层计算了 SYSRET 安全判定，但汇编没有消费返回值，最终无条件执行 `sysretq`；没有发现针对非 canonical RCX、特殊 RFLAGS 或慢路径工作的 `iretq` 回退。

3. **IRQ 分派与入口重入存在高风险。**  
   通用 IRQ dispatcher 把局部常量 `irq_nr = 0` 传给控制器 `ack`；外部 IRQ 汇编又在调用 C 处理器前执行 `sti`，但没有发现完整 hardirq 嵌套计数、IRQ 栈和相应锁契约。

4. **任务上下文保存不完整。**  
   通用寄存器、栈、`current` 和 TSS `sp0` 已接通，但 FS/GS base 恢复主体被注释，FPU/xstate、TLS、PKRU、I/O bitmap 等也没有形成完整的保存、复制和恢复闭环。

5. **SMP 数据结构存在，但 AP 上线闭环未形成。**  
   per-CPU 区、GDT、TSS 和 GSBASE 的 BSP 路径是活动实现；不过没有找到完整的 trampoline 复制与 INIT/SIPI 调用者，AP 汇编最终停在永久循环，也没有形成 secondary C entry、online mask、调度器接入和 idle loop。

6. **APIC/IOAPIC 当前更接近 BSP-only 实现。**  
   I/O APIC 固定初始化 24 个 RTE，目标固定为 BSP，未接入 ACPI MADT 的 IOAPIC/GSI/interrupt-source override；index/data 窗口也没有锁。

7. **三种计时角色必须分开理解。**  
   timekeeper 启动时绑定 `jiffies`；HPET timer0 中断推进 `jiffies` 和 `do_timer()`；TSC 只注册为 `sched_clock`；LAPIC timer 当前主要用于调试计数。HPET 虽注册为候选 clocksource，但实际通知 timekeeping 切换的代码被注释。

8. **CPUID 是“采集较广、规范化和启用不足”。**  
   当前代码保存多个基本叶和扩展叶的原始能力字，但厂商注册、能力依赖过滤、跨 CPU 交集、SMEP/SMAP/UMIP/FSGSBASE 等启用路径以及 `/proc/cpuinfo` 式输出均未闭合；TSC leaf `0x15` 的现有换算也与架构公式不一致。

这些结论已写入 PR 摘要和相应卡片。fileciteturn173file0L8-L8

## 校验结果与边界

- 分支相对创建时的 `master` **恰好领先一个提交**。
- PR 文件列表为 **15 个 `added` 文件**，全部位于唯一允许目录；没有修改源码、控制文件、任务书或构建文件。
- 抽查的 MANIFEST、CPUID atlas 和开放问题文件均具有统一 YAML 头，且 `base_snapshot` 全部固定为同一个 `time` 提交。
- PR 当前可合并。fileciteturn173file0L4-L16
- GitHub 当前没有返回该提交的状态检查或 PR workflow 运行；这不表示构建通过。
- 本次只完成静态源码、构建收集规则和架构手册核查，没有编译、链接或启动 QEMU、VMware、物理机，也没有把静态推断冒充运行时结论。

外部架构字段以 Intel 2026 年 6 月、revision 092 的 SDM 为主要口径，并对 CPUID 表所在 PDF 页面进行了可视核对；HPET 字段以 IA-PC HPET Specification 1.0a 为准。([Intel SDM](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html))
