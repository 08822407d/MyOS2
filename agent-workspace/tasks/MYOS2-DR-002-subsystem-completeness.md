# MYOS2-DR-002 · 子系统能力完成度盘点

```yaml
task_id: MYOS2-DR-002
mode: 普通对话 Pro（GitHub 连接器）
priority: P0
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-002/
prerequisites: 先读 agent-workspace/conventions.md（尤其 §3 词汇表）与 agent-workspace/repo-map.md
```

## 背景

Owner 需要搞清楚：各子系统目前实现到什么程度、支持哪些能力、还欠缺哪些能力。这张"完成度矩阵"是后续学习方向选择、依赖图（MYOS2-DR-003）、重要度分级（MYOS2-DR-007）共同的地基。

## 目标

对 mykernel/ 各子系统（词汇表 §3 所列，另含 boot.uefi 与 user.initramfs），逐个产出：

1. **已实现能力**：列能力节点（按词汇表命名），每条给源码证据 `[VERIFIED 路径]`（关键函数/文件即可，不必逐行）。
2. **明显缺失能力**：对照"一个教学级 Unix-like 内核通常应有什么"的参照系（以 Linux 概念框架为准但按教学内核合理裁剪，参照系本身也要写出来），列缺失项，标 `[INFERRED]`。
3. **成熟度分级**：每个子系统 0~4 级（0=空壳 / 1=雏形可编译 / 2=基本路径可跑 / 3=较完整有边界处理 / 4=完整且有并发正确性考虑），给一句判级理由。
4. **风险点**：阅读中发现的可疑实现（如无锁保护的共享结构、未处理的错误路径），记录但不深究——这是给 MYOS2-DR-004 调试桩任务的输入。

## 优先顺序（读不完时按此弃尾并在 MANIFEST 声明覆盖状态）

mm → sched → lock_IPC → kactive → time → fs → arch.x86_64（重点 kbuild、中断/异常、页表、SMP）→ block/drivers → init/entry/cpu → 其余。

## 交付物（放入 write_zone）

- `MANIFEST.md`（必交；覆盖状态尤其重要）
- `completeness-matrix.md` 人读版（分子系统章节）
- `completeness.yaml` 机器可读版，行格式：

```yaml
- node: mm.page_alloc.buddy_system      # 词汇表 §3 命名
  status: implemented | partial | missing | unknown
  maturity: 0-4                          # 子系统级另有汇总表
  evidence: [mykernel/mm/page_alloc/xxx.c]
  notes: 一句话
```

## 边界

不做修改建议（那是 004 的事）；不评价代码风格；发现与 repo-map.md 推断不符时以你的实际阅读为准并在 open_questions 指出。

## 验收判据

completeness.yaml 全部 node 命名合法（词汇表前缀）；每个 implemented 至少一条 evidence；MANIFEST 覆盖状态诚实（没读到的子系统标 unknown 而非猜测）。
