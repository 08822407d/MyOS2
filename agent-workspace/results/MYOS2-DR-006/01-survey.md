---
task_id: MYOS2-DR-006
deliverable: survey
base_snapshot:
  branch: time
  commit: a039d9803893e0ce54164413aa8a0c29eacf5c78
  tree: fa8d26ffa2cf2b827c183452039a0b780f82eab2
status: proposal
---

# Testing and CI Survey for MyOS2

## Executive conclusion

MyOS2 不应该把“CI 测试内核”理解为只有一个：

`make && qemu-system-*`

的 job。

更稳健的结构是把测试拆成四个责任不同的层次：

| Layer | Question answered | Feedback speed | Failure localization |
|---|---|---:|---|
| L1 boot smoke | 内核镜像能否在标准虚拟机配置下启动并达到确定 milestone？ | 快 | 中 |
| L2 unit | 一个可以隔离的逻辑单元是否正确？ | 最快 | 高 |
| L3 syscall functional | 用户程序能否通过真实 ABI 得到正确 kernel-visible 行为？ | 中 | 高 |
| L4 CI | 前三层能否在每个变化上自动、可重复、超时受控地执行？ | — | — |

核心原则是：

**用最便宜的测试定位局部错误，用真实 guest 测试覆盖不能从 host 模拟的边界。**

这与成熟内核测试体系中的分层方式一致：KUnit 面向可隔离的 kernel unit；kselftest 面向从用户空间可观察的内核接口；KTAP 提供结构化结果；QEMU 提供自动化 guest 执行环境；CI 再负责编排这些测试。

## Survey: Linux KUnit

Linux KUnit 是 Linux 内核中的白盒单元测试框架。官方文档把 unit test 定位为针对相对独立的代码单元，并提供 assertion、suite/case 组织和可机器处理的测试输出。

Source:
`https://docs.kernel.org/dev-tools/kunit/`

### Relevant lesson for MyOS2

MyOS2 不需要复制整个 KUnit 实现，真正值得采用的是三个设计边界：

1. unit test 应当小而确定；
2. 测试结果必须由程序判断而非人工看屏幕；
3. 测试代码与生产代码之间需要明确的 seam。

对于教学/小型内核，最危险的做法是为了“unit test”而模拟整个机器。例如测试一个包含：

- page allocator
- interrupt controller
- scheduler
- serial device
- process table

的对象图，最终会产生一个比被测代码还复杂的 mock 系统。

更合适的做法是先找纯逻辑，例如：

- bitmap/index calculation
- bounded queue/ring bookkeeping
- pathname/token parsing
- flags validation
- size/alignment calculation
- table lookup/dispatch metadata
- time conversion helper

前提是这些具体候选必须从当前 MyOS2 源码逐项确认，而不能仅因为其他 OS 通常具有它们就宣称 MyOS2 已有对应模块。

### MyOS2 mapping

第一阶段不建议“先实现一个 MyKUnit”。

优先顺序应当是：

`可复用纯函数 -> host-side unit harness -> 必要时 guest kernel micro-test`

原因是 host unit tests：

- 不需要启动 QEMU；
- 崩溃可直接由 host process 表达；
- 容易在 CI 并行执行；
- 更适合 sanitizer；
- 失败定位比完整 kernel boot 清楚。

但是任何含 privileged instruction、port I/O、interrupt state、真实页表或用户/内核 privilege transition 的路径，都不应该因测试方便而被假装成 host unit。

## Survey: Linux kselftest

Linux kselftest 从用户空间调用正在运行的内核接口，验证用户可见行为。

Source:
`https://docs.kernel.org/dev-tools/kselftest.html`

它与 unit test 的重要区别不是代码放在哪里，而是**测试观察点在哪里**。

### Relevant lesson for MyOS2

对 syscall 而言，正确的测试不是：

```c
sys_write_impl(...);
