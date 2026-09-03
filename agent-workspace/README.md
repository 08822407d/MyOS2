# agent-workspace · MyOS2 的 LLM 协作工作区（总纲）

```yaml
record_type: workspace_charter
workspace_id: MYOS2-AGENT-WS
created: 2026-09-01
created_by: claude-fable-5（Mnemosyne 主导会话，代 Owner 搭建）
status: bootstrap（紧急启用版；未来由 meta-agent 接管并可重组结构）
task_series: MYOS2-DR-NNN（本目标项目专用序列，从 001 起编；先例：meta-agent 项目的 MA-DR 序列）
base_snapshot: 分析基线=分支 time @ a039d980（当前最新开发版）；工作区与产出写入=master（读写分离，见 conventions.md §4）
```

## 这是什么

MyOS2 是 Owner 数年间模仿 Linux 独立开发的学习/实验用 OS 内核。本目录是**所有 LLM agent 对该内核做分析、建议、资料整理的唯一工作区**，用于承载"借助 LLM 深度学习内核设计"这一长期计划的第一步：现状分析。

**隔离原则（最重要的一条）**：本目录之外的一切（mykernel/ myloader/ myinitramfs/ scripts/ documents/ user-guide/ 及根目录文件）是 Owner 的内核本体，任何 agent **不得修改、移动、删除**。所有 agent 产出只进本目录。对内核源码的修改建议以补丁文件/代码片段形式放在各自 results 目录里，由 Owner 决定是否采用。

## 目录结构

```
agent-workspace/
  README.md          ← 本文件（总纲与接管协议）
  conventions.md     ← 写入公约＋共享词汇表（每个 agent 会话开工前必读）
  repo-map.md        ← 仓库地图（预编译的定向材料，省得每个会话重新摸索）
  LAUNCH.md          ← Owner 的发射清单（每个任务的启动提示词）
  tasks/             ← 任务书，一任务一文件，文件名=任务号
  results/           ← 产出区，一任务一子目录，目录名=任务号（各会话唯一可写区）
```

## 任务序列与状态

任务编号 `MYOS2-DR-NNN` 一经分配永不复用。当前分配见 tasks/ 目录（001~009 为第一波，2026-09-01 发出）。每个任务的完成状态以其 `results/MYOS2-DR-NNN/MANIFEST.md` 为准。

## 未来接管协议（给 meta-agent / 后续 agent 的话）

本工作区在基础设施未完善时紧急启用，允许不完美，但保证以下事项使你可以无损接管：

1. **产出皆证据、非规范**：results/ 里的一切是带出处的分析证据（S1 级），不是裁决过的权威结论；接管方可以重评、替代、重组，但不得改写原件——用新文件加 supersedes 标注。
2. **来源可追**：每份产出带 YAML 头（任务号、模型、日期、读过的输入、置信度），正文中事实性断言标注 [VERIFIED]/[INFERRED]/[EXTERNAL]（见 conventions.md）。
3. **快照锚定**：第一波分析基线为分支 time @ a039d980（最新开发版；连接器读不到时允许降级用 master，但实际所读快照必须写进产出 YAML 头）；接管方据此判断结论时效。
4. **词汇表共享**：子系统/能力节点命名统一用 conventions.md §3 词汇表，各任务的机器可读产出（YAML/DOT）因此可以直接合并。
5. **重组自由**：未来重新设计文件组织时，本目录可整体迁移或重构，唯一要求是保留 results/ 原件与其 MANIFEST 链条。

对应的立项与决策记录在 Mnemosyne 仓库 target-projects/myos2/ 下。

## 主导会话记录目录 lead/（2026-09-03 追加登记；MYOS2-LEAD-001）

- `lead/MYOS2-LEAD-NNN/`：主导会话（非 GPT 任务会话）的记录目录，一条主导轨道一个目录。序列 `MYOS2-LEAD-NNN` 从 001 起编、永不复用，与任务序列 `MYOS2-DR-NNN` 互不占用。首条 MYOS2-LEAD-001：本地 Claude Code 主导会话（Fable 5.1），2026-09-03 自 Mnemosyne 会话接手本计划的全部 MyOS2 职责。
- 目录内容：Owner 工作令与补充说明原文（`00-owner-work-order-verbatim.md`，现行规则的最高来源）、启动回执、前任记录镜像（`predecessor/`）、定向报告、分析维度台账、暂定执行源清单、续接检查点（`checkpoints/`）、交接包（`09-continuation/`）。一个全新会话应能仅凭该目录恢复：现行规则在哪、做到哪一步、哪些门待 Owner、哪些是暂定/待追认、下一步安全动作是什么。
- 分支命名：主导会话维护件走 `agent/MYOS2-LEAD-NNN`，提交前缀 `MYOS2-LEAD-NNN: `，PR 到 master 由 Owner 合并；任务产出仍走 conventions.md §1.3 的 `agent/MYOS2-DR-NNN`。
- 记录分工变更：上文末句"对应的立项与决策记录在 Mnemosyne 仓库 target-projects/myos2/ 下"自 2026-09-03 起不再适用于新记录——此后新记录只进本目录；Mnemosyne 侧既有两份记录已逐字节镜像至 `lead/MYOS2-LEAD-001/predecessor/`（附双侧哈希收据），其后仅追加一份移交记录后停笔。上文原句按"只增不改"保留。
