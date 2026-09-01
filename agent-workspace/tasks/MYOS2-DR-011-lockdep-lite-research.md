# MYOS2-DR-011 · lockdep-lite 设计调研（教学化最小可行死锁检测）

```yaml
task_id: MYOS2-DR-011
mode: 深度研究 Pro（外部调研为主；可读仓库做落地映射）
priority: P2
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-011/
prerequisites: 先读 agent-workspace/conventions.md、agent-workspace/WAVE-1-REVIEW.md
```

## 为什么选这个题

第一波评审后，lockdep-lite 是各方证据汇聚度最高的一项：

- `mykernel/lock_IPC/lockdep/lockdep.c` 实测**仅 2 行**（纯绿地，无历史包袱，不必先理解遗留代码）；
- MYOS2-DR-007 将其列为推荐学习路线 A 的 A4 里程碑，并称其外部教学证据最强；
- 已有**现成的真 bug 可抓**：`arch_spin_trylock`（`spinlock_smp_arch.h:50-57`）不做 CAS，多 CPU 可同时获锁成功；MYOS2-DR-005 另查出两处两步硬件协议缺互斥（IOAPIC index/data 窗口、PCI CF8/CFC 事务且 `pci_config_lock` 取放被注释）；
- 它是任何 SMP 化工作的前置（当前 AP trampoline 原地自旋、SMP 未上线）。

## 研究问题（深度研究部分）

1. **Linux lockdep 的机制拆解**：lock class 的定义与 key 机制、held-lock 栈、依赖图边的建立与环检测、`__acquire/__release` 注入点、报告格式（含最短依赖路径输出）。要求给出**可解析出处**（内核文档 `Documentation/locking/lockdep-design.rst` 的公开链接、源码文件链接）。
2. **最小可行子集**：若只要抓"同类锁的 AB-BA 环 ＋ 中断上下文违规（irq-safe/irq-unsafe 混用）"，最少需要哪些数据结构与钩子？业界/教学项目有无更小的实现可参考（其他教学内核、Rust/其他语言生态的运行时死锁检测器、valgrind DRD/helgrind 的思路）。
3. **教学化取舍**：完整 lockdep 的哪些部分对学习者是必要的、哪些是 Linux 特有的工程包袱（如 `lockdep_map` 的静态 key、cross-release 等历史特性）？给出"教学版保留什么、砍什么、砍掉的代价是什么"的取舍表。
4. **验证方法**：如何验证一个 lockdep 实现本身是对的？（构造已知 AB-BA 场景、已知假阳性场景、递归锁场景等的负测清单）
5. **单核环境下的价值**：MyOS2 当前 SMP 未上线（AP 原地自旋），在 UP＋抢占/中断的条件下 lockdep 能抓到哪些类别的错误、抓不到哪些？——这决定它现在做值不值。

## 落地映射（读仓库部分，可选但加分）

MyOS2 现有锁原语位于 `mykernel/lock_IPC/`（atomic/spinlock/semaphore/futex/lockdep）与 `mykernel/arch/x86_64/lock_IPC/`。给出注入点建议（在哪些函数加 acquire/release 钩子）与数据结构落点建议。注意：**不要基于任何行号做断言**，第一波产出的行号已证实大量偏移；引用请用函数名与文件路径。

## 出处纪律（硬性）

同 MYOS2-DR-010：每条外部结论附可解析 URL/DOI，内部检索句柄不计，拿不到出处的不写、列 open_questions。

## 交付物（放入 write_zone）

`MANIFEST.md`＋`01-lockdep-mechanism.md`（研究问题 1）＋`02-minimal-subset.md`（问题 2、3，含取舍表）＋`03-validation-suite.md`（问题 4 的负测清单）＋`04-myos2-mapping.md`（问题 5 与落地映射）。

## 边界

不写内核实现代码（实现由 Owner 亲手做，这是学习价值所在）；不假设 SMP 已可用；不基于行号断言。

## 验收判据

问题 1~5 各有独立成文回答；取舍表明确列出"砍掉的代价"；负测清单 ≥ 8 条且含至少 2 条假阳性场景；可解析 URL/DOI ≥ 15 且内部句柄 = 0。
