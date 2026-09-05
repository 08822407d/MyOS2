# MYOS2-DR-007R · 重要度分级证据补全与 tier 重算（007 补做：外部出处不可解析、tier 计数错）

```yaml
task_id: MYOS2-DR-007R
supersedes: MYOS2-DR-007（部分：01-evidence-survey 与 importance.yaml 的证据层、MANIFEST 计数；roadmap 的三条路线保留）
mode: 深度研究 Pro（外部出处为主）；研究完成后回到普通回合写库或降级输出
priority: P1（Owner 目的②"重要程度图"；阶段 3 交叉综合需要可复核的重要度证据）
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-007R/
protocol: agent-workspace/tasks/00-gpt-task-protocol-v2.md（全部十三条适用，硬性；本任务 P5 出处纪律最关键）
prerequisites: 先读 conventions.md、上述协议、WAVE-1-REVIEW.md §3.5、§5、§6；再读 results/MYOS2-DR-007/{MANIFEST.md,01-evidence-survey.md,importance.yaml,roadmap.md}（只读，待补做对象）
drafted_by: MYOS2-LEAD-001（2026-09-05）
status: draft（发射由 Owner）
```

## 为什么补做

007 是第一波可靠性 medium、无编造的一份，其 T0~T3 分级与三条路线（并发正确性优先 / 存储栈打通优先 / 用户态生态优先）仍是 Owner 定学习方向的主要依据。但：
- 它（与 006/009 一起）的外部引用全部是内部检索句柄（三份共 175 处），**没有一个可解析的 URL 或 DOI**——"每个 T0 至少两类独立证据"的验收判据实际无法复核（WAVE-1-REVIEW §3.5）；
- MANIFEST 的 tier 计数错（自报 T1 25 / T2 9，评审重算 T1 23 / T2 11）；
- 三套分级口径（002 risk_level / 007 tier / 008 severity）互不映射，三份 YAML 不能安全叉乘（WAVE-1-REVIEW §7.5；Owner 待决第 7 条）。

## 研究问题（深度研究部分；每条结论附 URL/DOI）

1. **教学体系权重**：OSTEP 章节结构、MIT 6.1810/6.S081 课程日程与实验序列、Berkeley CS162、CMU 15-410/15-213 相关部分、清华 rCore/uCore 课程大纲——各自的主题顺序与课时/实验权重，给出官方页面 URL。
2. **工程现实权重**：Linux 子系统中心度的可引用来源（如 MAINTAINERS 结构、内核文档索引、LWN 子系统专题页）；"变更频率"若无可复核数据则标 `not_measurable`，不估数。
3. **现代性维度**：rust-for-linux、io_uring、eBPF、per-CPU/RCU 扩展性、内核加固（W^X、KASLR）等主题的官方文档/内核文档 URL，以及"哪些适合教学内核引入简化版"的判断依据（引用课程或教材而非自述）。

## 映射与产出

1. **01-evidence-survey-v2.md**：v1 的每条外部结论逐条 保留（补 URL/DOI）/ 修正 / 删除（查不到出处）；`supersedes: results/MYOS2-DR-007/01-evidence-survey.md`。
2. **importance-v2.yaml**：节点集合与 tier 沿用 v1，`evidence` 字段改为可解析出处列表；任何 tier 变更在 errata 说明；MANIFEST 的 tier 计数由 YAML 机械统计得出并与 YAML 一致。
3. **tier-mapping-proposal.md**：给 Owner 待决第 7 条一个**提案**（不裁决）：002 的 completeness/risk、007 的 tier、008 的 severity 三者如何映射到一张交叉表，含映射表与不可映射项说明。
4. **roadmap-v2.md**：保留 A/B/C 三条路线，用 WAVE-1-REVIEW §5（三重交集 7 个节点）与 §6（主攻清单，其中 1/4/6 项为 Owner 亲手）更新里程碑；每条里程碑标"Owner 亲手 / 治理类（本地）"。
5. **errata.md**：v1→v2 逐条变更（含 tier 计数修正）。

## 输入范围与 MANIFEST（v2 协议增补，据对抗评审）

**输入清单（文件/节级；协议 P11）**：`results/MYOS2-DR-007/importance.yaml`（全文，勘误对象）；`results/MYOS2-DR-007/01-evidence-survey.md`（全文，勘误对象）；`results/MYOS2-DR-007/roadmap.md`（全文）；`results/MYOS2-DR-007/MANIFEST.md`（只取 tier 计数）；`WAVE-1-REVIEW.md` §3.5、§5、§6、§7.5。不读 `received/` 的 84 KB 原件（其内容已拆出）；不读内核源码。

**本任务无源码引文锚点**（不读内核），故 MANIFEST 免 `branch_canary_quotes`；但 tier 计数必须由 YAML 机械得出：MANIFEST 写 `tier_counts: {T0: n, T1: n, T2: n, T3: n}`，闸门用 YAML 重算比对。

`MANIFEST.md` 必备字段（协议 P3/P6/P13）：`base_snapshot: time（分支名）`、`read_channel`、`startup_selfcheck_quote`、`branch_canary_quotes`（读源码的任务）、`self_check`（`verified_claims` 须等于全部交付文件的 `[VERIFIED` 标签数）、`produced_by`（界面显示的模型名原样）、覆盖表。

降级交付按协议 P12：每条回复只含一个文件、独立围栏、围栏前一行写目标路径；读不完按本任务书的优先顺序弃尾并在覆盖表声明。

## 交付物（放入 write_zone）

`MANIFEST.md`（必交；含出处统计 URL/DOI 条数与内部句柄条数、tier 机械计数、`self_check`）＋`errata.md`＋`01-evidence-survey-v2.md`＋`importance-v2.yaml`＋`tier-mapping-proposal.md`＋`roadmap-v2.md`。

## 边界

不读内核源码（映射只用 v1 与 WAVE-1-REVIEW 的已核事实）；不改 results/MYOS2-DR-007/；不裁决三套口径（只提案）；不写 40 位 SHA；深度研究回合连接器只读，写库在普通回合。

## 验收判据（本地闸门按协议 P9 执行）

- 可解析 URL/DOI ≥ 30，内部检索句柄 = 0；
- 每个 T0 节点 ≥ 2 条不同类别（课程/工程/现代性）的可解析出处；
- MANIFEST tier 计数与 importance-v2.yaml 机械统计一致；
- tier-mapping-proposal 覆盖三套口径的全部取值；
- `self_check` 自洽；全文无 40 位 SHA。
- MANIFEST 含 `startup_selfcheck_quote`、`read_channel`、`self_check`、`tier_counts`（与 importance-v2.yaml 机械重算一致）；正则 `[0-9a-f]{40}` 0 命中（协议 P9-1、5、8）。

