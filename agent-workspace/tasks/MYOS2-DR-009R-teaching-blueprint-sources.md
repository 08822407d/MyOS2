# MYOS2-DR-009R · 教学蓝本预研出处补全与教学单元过滤（009 补做：出处不可解析、零源码接触导致空壳目录被排成教学单元）

```yaml
task_id: MYOS2-DR-009R
supersedes: MYOS2-DR-009（部分：四份研究文件的证据层；04 的映射表整体替换）
mode: 深度研究 Pro（外部出处为主）；研究完成后回到普通回合写库或降级输出
priority: P2（Owner 目的⑤的证据输入；不阻塞其他任务）
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-009R/
protocol: agent-workspace/tasks/00-gpt-task-protocol-v1.md（全部十条适用；P5 出处纪律最关键）
prerequisites: 先读 conventions.md、上述协议、WAVE-1-REVIEW.md §3.5；results/EXTRACTION-NOTE.md；再读 results/MYOS2-DR-009/received/MYOS2-DR-009-deep-research-report.md（只读，待补做对象；其五份文件内嵌于该原件）；以及 results/MYOS2-DR-002/completeness.yaml（若 MYOS2-DR-002R 已回收则改读 completeness-v2.yaml）
drafted_by: MYOS2-LEAD-001（2026-09-05）
status: draft（发射由 Owner）
```

## 为什么补做

009 无编造、可靠性 medium，四个研究问题都有成文回答；但：
- 外部引用全是内部检索句柄，无一可解析 URL/DOI（WAVE-1-REVIEW §3.5）——作为"未来学习 agent 组的证据输入"，不可复核的证据等于没有；
- 任务书要求它零源码接触，于是它把 `entry`（74 行）、`device`（0 源码）、`debug`（1 文件）都排成了教学单元（§3.5）；课程映射表使用前必须与完成度交叉过滤；
- 五份交付文件未包在代码围栏内，无法机械拆分，只能整体留在 received/（EXTRACTION-NOTE.md）。

## 研究问题（同 009 的四条；本次要求每条结论带可解析出处）

1. 以简代繁的先例（xv6/MIT、Pintos、OS161、MINIX）：课程页面、教材/讲义 URL；练习三类（读/改/补）的具体实例链接。
2. LLM 辅助代码教学：已发表研究（DOI）与公开课程实践（URL）；失败模式 ≥ 5 条各带出处；缓解手段（引用锚定、先验证再呈现）各带出处。
3. 个人代码库作教材的特有问题：可引用的教育学/软件工程文献（DOI）或公开经验报告（URL）。
4. 课程骨架：主流 OS 课程模块划分对照表，每所课程给官方大纲 URL。

## 产出

1. **01-precedents-v2.md、02-llm-teaching-evidence-v2.md、03-own-kernel-specifics-v2.md**：v1 内容逐条 保留（补出处）/ 修正 / 删除（查不到出处）；各写 `supersedes: results/MYOS2-DR-009/received/…（对应内嵌文件标题）`。
2. **04-curriculum-skeleton-v2.md**：映射表覆盖词汇表全部 53 个 ID，新增列 `teachability: teachable | not_teachable_yet | placeholder`，判定依据＝002（或 002R）的 completeness 与 repo-map.md 的行数信号：completeness 0 或源码行数 < 100 的一律 `not_teachable_yet`/`placeholder`，并写明理由；映射仍标 [INFERRED]。
3. **errata.md**：v1→v2 逐条变更；明确列出被降为不可教的单元。
4. **交付形态**：优先写库；若只能对话降级，**每个文件单独包在一个代码围栏内**，围栏前一行写目标路径——第一波未围栏导致无法拆分。

## 交付物（放入 write_zone）

`MANIFEST.md`（必交；含出处统计、`self_check`、`supersedes` 列表）＋`errata.md`＋`01-precedents-v2.md`＋`02-llm-teaching-evidence-v2.md`＋`03-own-kernel-specifics-v2.md`＋`04-curriculum-skeleton-v2.md`。

## 边界

不设计 agent 组本身；不深入内核源码（只读 002/002R 的 YAML 与 repo-map.md）；不改 results/MYOS2-DR-009/；不写 40 位 SHA；深度研究回合连接器只读，写库在普通回合。

## 验收判据（本地闸门按协议 P9 执行）

- 可解析 URL/DOI ≥ 25，内部检索句柄 = 0；
- 失败模式 ≥ 5 条各带出处；
- 04 映射表覆盖 53 个 ID，每行有 `teachability` 与依据；`entry`/`device`/`debug` 三个已知空壳单元不得再标 teachable；
- 若降级交付，每个文件独立围栏（本地机械拆分成功即通过）；
- `self_check` 自洽；全文无 40 位 SHA。
