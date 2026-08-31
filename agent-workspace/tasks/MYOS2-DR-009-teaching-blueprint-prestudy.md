# MYOS2-DR-009 · 教学蓝本预研：以自制内核为底本的学习 agent 组设计依据

```yaml
task_id: MYOS2-DR-009
mode: 深度研究 Pro（外部证据为主）
priority: P2
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-009/
prerequisites: 先读 agent-workspace/conventions.md 与 agent-workspace/repo-map.md
```

## 背景

Owner 计划将来建一组专门的学习 agent，用**自己的内核**（而非过度复杂的 Linux 源码）作讲解现代内核理论与实现的蓝本。agent 组本身的设计由未来的 meta-agent 主导，本任务只做预研：把外部证据收齐，让未来设计不用从零调研。这也是对"用个人代码库做个性化教学"这一模式本身的可行性检验。

## 研究问题

1. **以简代繁的先例**：xv6 之于 MIT、Pintos/OS161 之于各校、MINIX 之于教科书——用小内核教大原理的课程设计方法：讲义怎么围绕代码组织、练习怎么设计（读代码/改代码/补功能三类）、简化版与工业实现的差距怎么交代。
2. **LLM 辅助代码教学的现状**：用 LLM 做代码讲解/苏格拉底式提问/练习生成与批改的已验证做法与失败模式（幻觉讲错代码、迁就学生错误认知等），以及缓解手段（引用锚定、答案先验证再呈现）。
3. **个人代码库作教材的特有问题**：代码本身可能有错（教学时如何区分"设计如此"与"这是 bug"）；与标准实现的差异如何变成教学素材而非误导；学习者同时是作者时的认知盲区。
4. **课程骨架素材**：主流 OS 课程的模块划分对照表（供未来把 MyOS2 子系统映射成课程单元）。

## 交付物（放入 write_zone）

`MANIFEST.md`＋`01-precedents.md`（教学内核课程设计方法综述）＋`02-llm-teaching-evidence.md`（LLM 教学证据与反模式）＋`03-own-kernel-specifics.md`（自有代码库作教材的问题与对策）＋`04-curriculum-skeleton.md`（课程骨架对照表＋MyOS2 初步映射，映射用词汇表 ID，标 [INFERRED] 即可）。

## 边界

不设计 agent 组本身（角色分工/提示词/工具留给 meta-agent）；不深入 MyOS2 源码（读 repo-map.md 的子系统清单即可）；一切外部结论带出处。

## 验收判据

四个研究问题各有独立成文的回答且证据充分；04 的映射表覆盖词汇表全部子系统；反模式部分至少给出五条有出处的失败模式与对策。
