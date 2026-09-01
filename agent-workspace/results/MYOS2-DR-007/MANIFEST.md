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
  - mykernel/sched/scheduler/
  - mykernel/sched/scheduler/scheduler_core.c
  - mykernel/lock_IPC/
  - mykernel/lock_IPC/lockdep/
  - mykernel/lock_IPC/lockdep/lockdep.c
  - mykernel/fs/
  - mykernel/debug/
  - mykernel/device/
  - mykernel/namespace/
  - myinitramfs/
status: final
open_questions:
  - "公约词汇表没有 net/security/extensibility 独立 subsystem ID；为保持节点合法，本任务分别以 device.network_stack、mm.vm_map.kernel_w_x_hardening、debug.bpf_style_programmability 等能力节点承载这些现代内核主题，后续可考虑扩词汇表。"
  - "Linux 各子系统精确的 path-level commit 频率没有在本任务中形成可重复统计数据，因此工程变更频率只作为定性信号，不伪造数字；标记为 not_measurable。"
  - "repo-map.md 记录的是其生成时的 master 基线；2026-08-31 当前 master HEAD 已因 agent-workspace 提交前移。内核源码分析仍严格固定在 time @ a039d9803ade2a1613d620bda375e028530d5242。"
---

# MYOS2-DR-007 Manifest

## 任务范围

本任务回答两个问题：

1. 对 Owner 通过 MyOS2 深学现代内核设计而言，哪些能力是骨干必修（T0）、强烈建议（T1）、进阶选修（T2）或特定场景（T3）。
2. 在不依赖 MYOS2-DR-002/003 并行产出的前提下，依据 repo-map 的规模信号和本次源码抽样，给出三条真正不同的学习路线并推荐其一。

本任务没有读取 MYOS2-DR-002 或 MYOS2-DR-003 的任何产出。

## 快照纪律

- 工作区规则、地图、任务书：
  `master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719`
- 被分析内核源码：
  `time @ a039d9803ade2a1613d620bda375e028530d5242`
- 所有源码层结论均限定于上述 `time` 快照。
- 不把“该快照未见某能力”写成“项目永远不存在某能力”。

## 交付物

| 文件 | 作用 | 主要验收点 |
|---|---|---|
| `MANIFEST.md` | 快照、范围、文件索引与限制 | 本文件 |
| `01-evidence-survey.md` | 教学体系、Linux 工程现实、现代性三轴证据综述 | 所有外部结论有来源；不虚构 path-level 变更频率 |
| `importance.yaml` | 词汇表全部 subsystem 的 T0-T3 重要度 + T0/T1 能力节点 | 53/53 subsystem 全覆盖；所有 T0 至少两类独立证据 |
| `roadmap.md` | 三条候选学习路线与推荐 | 并发正确性、存储栈、用户态生态三条路线目标和里程碑真实不同 |

## 机器可读结果摘要

`importance.yaml`：

- subsystem 节点：53
- 词汇表覆盖率：53 / 53
- T0 subsystem：14
- T1 subsystem：25
- T2 subsystem：9
- T3 subsystem：5
- 额外能力节点：覆盖本任务要求的 T0/T1 现代能力，包括：
  - SMP / per-CPU / load balancing
  - memory ordering / RCU-lite
  - lockdep-lite
  - W^X kernel-memory hardening
  - crash consistency / page cache / async I/O
  - network-stack gap
  - namespace / resource-control concepts
  - BPF-style verified programmability
  - safe driver interface concepts

## 分级语义

- `T0`：骨干必修。缺少这些概念，会使对进程、地址空间、并发、调度或文件抽象的理解出现结构性空洞。
- `T1`：强烈建议。不是最小 OS 的必要条件，但对理解真实现代内核、跨子系统交互或可靠性非常高杠杆。
- `T2`：进阶选修。能明显拓宽内核工程视野，但可在骨干稳定后再深入。
- `T3`：特定场景。主要由具体硬件、兼容性、历史架构或辅助工具需求驱动。

分级不是“代码完成度”。一个当前完全缺失的能力仍可能是 T1；一个已经有很多代码的能力也可能仅是 T2。

## 源码映射摘要

[VERIFIED mykernel/sched/scheduler/scheduler_core.c]
调度核心保存了大量 Linux scheduler 的并发、迁移、wake-up 和 memory-ordering 注释/结构，但相当一部分对应实现被注释掉或由更窄的 MyOS 路径替代。

[VERIFIED mykernel/lock_IPC/lockdep/lockdep.c]
当前文件只有 `LOCKDEP_DEFINATION` 与 `#include "lockdep.h"`，因此 lockdep 是非常明显的教学扩展位。

[VERIFIED mykernel/fs/]
当前顶层是 `vfs/`、`fat/` 与 `syscall.c`。

[VERIFIED mykernel/device/]
当前只有空 `CMakeLists.txt`；这不等价于整个仓库没有驱动，而是说明通用 `device` 层本身仍非常薄。

[VERIFIED mykernel/debug/]
当前目录主体是 `panic.c`；与现代 Linux 的 tracing、fault injection、locking diagnostics 相比，有明显可教学扩展空间。

[VERIFIED mykernel/namespace/]
存在 `nsproxy.c`，说明 namespace 方向已经有可继续演化的落点。

[VERIFIED myinitramfs/]
已有 `myinit/`、`mylib/`、`myshell/`，适合作为 syscall ABI 和端到端行为验证层。

## 写库状态

本次会话中的 GitHub connector 为只读能力集，未暴露 branch/file/commit/PR mutation action。

因此：

- `agent/MYOS2-DR-007`：未创建
- repository files：未写入
- commit：未创建
- PR to `master`：未创建

本对话按任务书规定完整输出四个目标文件，供原样落入：

`agent-workspace/results/MYOS2-DR-007/`
