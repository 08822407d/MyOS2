# MYOS2-DR-007 · 内核能力重要度分级与学习路线图

```yaml
task_id: MYOS2-DR-007
mode: 深度研究 Pro（外部证据为主；连接器读仓库做映射）
priority: P1
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-007/
prerequisites: 先读 agent-workspace/conventions.md（尤其 §3 词汇表）与 agent-workspace/repo-map.md
```

## 背景

Owner 要一张"重要程度"图为下一步学习和实践定方向：哪些内核能力是现代内核设计的骨干必修、哪些是进阶选修、哪些是特定场景才需要。这张图与完成度矩阵（MYOS2-DR-002，并行执行、互不依赖）交叉后就能得出"重要但未完成"的学习主攻清单。重要度的依据不能拍脑袋，要有外部证据。

## 研究问题（深度研究部分）

1. 教学体系怎么排：OSTEP、MIT 6.1810/6.S081（xv6）、CMU/Berkeley/清华等 OS 课程的主题顺序与课时权重；《Linux Kernel Development》《Understanding the Linux Kernel》等经典书的篇幅分布。
2. 工程现实怎么排：Linux 各子系统的中心度（被依赖程度）、变更频率、LWN 等社区讨论热度所反映的枢纽地位。
3. 现代性维度：近十年内核领域的重要演进方向（多核扩展性、安全加固、异步 IO、eBPF 式可编程性、rust-for-linux 等）中，哪些概念已成为"现代内核素养"的一部分、适合教学内核引入简化版。

## 映射与产出（读仓库部分）

1. 用词汇表节点建 `importance.yaml`：

```yaml
- node: sched.scheduler.smp_load_balance
  tier: T0 | T1 | T2 | T3   # T0 骨干必修 / T1 强烈建议 / T2 进阶选修 / T3 特定场景
  rationale: 一句话（课程权重/工程中心度/现代性，注明证据类别）
  evidence: [EXTERNAL 出处]
```

覆盖词汇表全部子系统，能力级节点覆盖 T0/T1 即可。

2. `roadmap.md`：结合 repo-map.md 的规模信号（不必等 002 的精确完成度），给 Owner 三条候选学习路线（如"并发正确性优先""存储栈打通优先""用户态生态优先"），每条：目标、前置依赖（引用 003 的图谱概念即可）、里程碑、预计强度；并推荐其一，说明理由。

## 交付物（放入 write_zone）

`MANIFEST.md`＋`01-evidence-survey.md`（三个研究问题的综述，带出处）＋`importance.yaml`＋`roadmap.md`。

## 边界

分级针对"Owner 借自制内核深学现代内核设计"这一目的，不是通用内核开发排序；MyOS2 未涉足的领域（如网络栈）也要入表分级——缺口本身是路线图的输入；不读 002/003 的产出（并行执行，交叉分析留给下一波）。

## 验收判据

importance.yaml 覆盖词汇表全部子系统且节点命名合法；每个 T0 分级至少两类独立证据；三条路线各自成立且差异真实。
