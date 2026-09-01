---
task_id: MYOS2-DR-007
produced_by: "GPT-5.5 Thinking / Deep Research"
date: 2026-08-31
base_snapshot: "kernel=time @ a039d9803ade2a1613d620bda375e028530d5242; workspace=master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719"
inputs_read:
  - agent-workspace/conventions.md
  - agent-workspace/repo-map.md
  - agent-workspace/tasks/MYOS2-DR-007-importance-and-roadmap.md
  - mykernel/
  - mykernel/sched/
  - mykernel/sched/scheduler/scheduler_core.c
  - mykernel/lock_IPC/
  - mykernel/lock_IPC/lockdep/lockdep.c
  - mykernel/fs/
  - mykernel/debug/
  - mykernel/device/
  - mykernel/namespace/
  - myinitramfs/
status: final
open_questions:
  - "以下 prerequisite 图是本任务根据通用 OS 依赖关系独立整理的概念图，没有读取 MYOS2-DR-003 的产出。"
  - "路线强度使用 relative intensity，不给虚假日历工期。"
  - "网络路线暂以 device.network_stack 表示；若后续词汇表增加 net，应整体迁移该节点而不是保留兼容别名。"
---

# MyOS2 学习路线图

## 选择原则

目标不是“让 MyOS2 尽快拥有更多 feature”，而是最大化：

`每一份实现工作 -> 可复用的现代内核设计理解`

所以路线优先级综合考虑：

1. `importance.yaml` 的 T0/T1；
2. repo-map 的规模信号；
3. 当前源码是否已经提供足够上下文，可以把真实 Linux 复杂性缩成可理解实验；
4. 一个 milestone 能否同时验证多个 subsystem invariant；
5. 是否容易用测试/trace 得到客观反馈。

## 不依赖 003 的概念前置图

[INFERRED]

```text
arch / boot
    |
    v
entry ---------> debug / printk
  |                  ^
  v                  |
cpu / SMP -------> locking / memory-ordering
  |                  |
  +-------> scheduler/task <------ time
  |             |
  |             +-------> fork/exec/signal/futex
  |                              |
  v                              v
MM/page allocator ----------> userspace ABI
  |                              ^
  v                              |
VFS/page cache ----> block ----> drivers/device
  |
  +-------> namespace / user ecosystem
