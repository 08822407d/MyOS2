# 暂定执行源清单（MYOS2-LEAD-001 · 阶段 0 交付件 3）

```yaml
task_id: MYOS2-LEAD-001
track_id: MYOS2-LEAD-001
record_type: provisional_execution_sources
evidence_class: B（现状与暂定执行源的清单；规则原文在各出处，本文件不复述规则全文）
produced_by: claude-fable-5.1@claude-code-vscode（主导会话，UUID 3bfbb342-1b15-4461-a4c3-987f17aad103）
date: 2026-09-03
base_snapshot: 工作区 origin/master @ d231708c77e101dd38280ec0dc74e73d16a0446a；内核分析基线 time @ a039d9803ade2a1613d620bda375e028530d5242；Mnemosyne origin/master @ c319397（本地远端跟踪引用，未 fetch）
inputs_read:
  - 00-owner-work-order-verbatim.md §1.1、§4、§5、§13（Owner 原话与起草方的 S 表）
  - origin/master:agent-workspace/README.md
  - origin/master:agent-workspace/conventions.md
  - origin/master:agent-workspace/WAVE-2-PLAN.md
  - origin/master:agent-workspace/results/ARCHIVE-RECEIPTS.md
  - origin/master:agent-workspace/results/EXTRACTION-NOTE.md
  - origin/master:agent-workspace/results/README.md
  - Mnemosyne origin/master:notes/cross-model-review-results/FABLE5-REDESIGN-001/09-continuation/06-archiving-institution-record.md（2,015 B，sha256 d55e9057…）
  - Mnemosyne origin/master:notes/cross-model-review-results/FABLE5-REDESIGN-001/09-continuation/02-owner-correction-dual-channel-rules.md（3,389 B，sha256 c6ca2d10…）
  - Mnemosyne origin/master:notes/registries/multi-writer-attribution-convention.md（7,153 B，sha256 927880c3…）
status: draft（清单本身完整；"追认状态"列待 Owner 表态后由新文件 -v2 更新）
open_questions:
  - S 表内部矛盾清单（004R 任务书 mode 与 WAVE-2-PLAN §2 分流）的逐条核实结果在 01-orientation-report.md，本文件只登记存在矛盾
  - Alaya indexes/research-archiving-convention.md 未读（私档默认不读），S9 第 6 条内容以工作令转述为准
```

## 0. 用法与地位

- 这是**暂定**执行源（provisional）：Meta-Agent 尚无可用首版、行为规范未立，Owner 明令"之前补充的其他一些临时规范也包含进去作为暂定的执行源"（工作令 §1.1，2026-09-02 原话）。未来 Meta-Agent 可整体重组本清单；重组时只需保持各出处文件不被改写（README.md "未来接管协议"第 1、5 条）。
- **冲突处理顺序**（工作令 §5 末段）：Owner 原话（§1.1、§13 补充）＞ 工作令正文 ＞ S1~S13 ＞ 主导会话自己的判断。S 表内部矛盾不自行裁决，记 open item 报 Owner。
- 证据标签：`[VERIFIED 路径]` = 本会话于 2026-09-03 实读该出处并确认内容与摘要一致；`[INFERRED]` = 据工作令转述，本会话未读原件。

## 1. 清单（S1~S13）

| # | 规范内容（摘要，原文见出处） | 出处（工作区 = origin/master d231708c；Mnemosyne = origin/master c319397） | 来源性质 | 追认状态 | 本会话核验 |
|---|---|---|---|---|---|
| S1 | 总纲：agent-workspace/ 是唯一工作区；隔离原则（目录之外是 Owner 内核本体，任何 agent 不得改/移/删）；MYOS2-DR-NNN 编号永不复用；完成状态以 MANIFEST 为准；未来接管协议五条（产出皆证据、来源可追、快照锚定、词汇表共享、重组自由） | agent-workspace/README.md | 起草方（Mnemosyne 主导会话）立；Owner 默认 [MI]——据 Owner 用其跑完九任务、合并 Mnemosyne PR #329/#330 推断 | **待追认** | [VERIFIED agent-workspace/README.md]（"隔离原则"段、"未来接管协议"五条） |
| S2 | 写入规则：一对话＝一任务号；唯一可写区 results/<任务号>/ 只增；分支 agent/<任务号>；提交前缀 `<任务号>: `；PR 到 master；禁 force-push/直推 master；降级路径（对话内逐文件输出）；必交 MANIFEST；违反＝产出作废 | agent-workspace/conventions.md 标题行＋§1 | 同 S1 | **待追认** | [VERIFIED agent-workspace/conventions.md §1 第 1~5 条] |
| S3 | 产出格式：YAML 头七字段（task_id/produced_by/date/base_snapshot/inputs_read/status/open_questions）；[VERIFIED]/[INFERRED]/[EXTERNAL] 三类标注不混写；图/矩阵交 DOT＋YAML、YAML＋表双格式；中文说明英文标识符；修订不覆盖（-v2 ＋ supersedes） | agent-workspace/conventions.md §2 | 同 S1 | **待追认** | [VERIFIED agent-workspace/conventions.md §2] |
| S4 | 共享词汇表：53 个子系统 ID＋能力节点 `<子系统ID>.<能力短语>`；已知缺口 time.misc（time_misc.c 313 行无 ID）；待答决策 6 提议新增 repo.build / deploy.scripts 类目；007 报告另提 net/security/extensibility 类目（尚未采纳） | agent-workspace/conventions.md §3；WAVE-2-PLAN.md §3 第 6 条；results/MYOS2-DR-007/MANIFEST.md | 同 S1 | **待追认**（新增类目本身待决策 6） | [VERIFIED conventions.md §3]（ID 表）；[VERIFIED WAVE-2-PLAN.md §3 第 6 条]；007 MANIFEST 的三类目提议 [INFERRED]（本会话未读该 MANIFEST，工作流读者核对中） |
| S5 | 溯源纪律五条：SHA 只从工具输出复制、入库前 `git cat-file -e`；外部结论要 URL/DOI；行号易腐、优先函数名＋路径；[VERIFIED] 是承诺；断言"不存在/未调用"前必读配置面 options_flags.cmake | agent-workspace/conventions.md §3b | 第一波事故（两处伪造 SHA、假 [VERIFIED]）后起草方新增；Owner 默认 [MI] | **待追认** | [VERIFIED agent-workspace/conventions.md §3b 第 1~5 条] |
| S6 | 事实纪律：分析基线 time @ a039d980；读 time、写 master；结论只说"所读快照中未见"；bugs_record.md/todo.txt/changelog.md/documents/ 是历史材料；拿不到的数据标 not_measurable | agent-workspace/conventions.md §4 | Owner 原话（2026-08-31 开发模式纠正，见工作令 §1.1）＋起草方操作化 | Owner 原话部分**已生效**；操作化部分待追认 | [VERIFIED agent-workspace/conventions.md §4]；Owner 原话 [VERIFIED 00-owner-work-order-verbatim.md §1.1] |
| S7 | 分流原则：需要 grep/编译/git 的走本地 Claude；纯外部文献走 GPT 深度研究（强制可解析 URL/DOI）；有学习价值的实现由 Owner 亲手（本地 Claude 陪跑写回归）；只有 Owner 知道的事实由 Owner 一句话答 | agent-workspace/WAVE-2-PLAN.md §0 | 起草方立（第一波教训） | **待追认** | [VERIFIED agent-workspace/WAVE-2-PLAN.md §0 表] |
| S8 | 第二波硬纪律：SHA 机械校验闸门；删码需"当前 CONFIG 下不可达"证明；行号降级为函数级；实施类任务硬前置（L3 桩、L6 冒烟）；并发结论 UP/SMP 分标 | agent-workspace/WAVE-2-PLAN.md §4 | 起草方立 | **待追认** | [VERIFIED agent-workspace/WAVE-2-PLAN.md §4 第 1~5 条] |
| S9 | 归档：回收件双仓归档（MyOS2 ↔ Alaya research/MYOS2/），bytes/sha256 双向互引；ARCHIVE-RECEIPTS 由主导会话维护、GPT 会话不写；对话侧原件后续统一放 received/；GitHub 侧交付件留本仓不复制 | agent-workspace/results/ARCHIVE-RECEIPTS.md；Mnemosyne 09-continuation/06-archiving-institution-record.md；Alaya indexes/research-archiving-convention.md（第 6 条待追认） | Owner 原话（2026-08-31 归档制度，逐字见 Mnemosyne 06 记录 YAML 头）＋起草方操作化 | Owner 原话部分**已生效**（且该原话本身含 Alaya 写入授权，但工作令 §4.9 对本轨道仍要求 Alaya 写入需 Owner 单独同意，以工作令为准）；Alaya 侧规则第 6 条待追认 | [VERIFIED results/ARCHIVE-RECEIPTS.md]；[VERIFIED Mnemosyne …/06-archiving-institution-record.md]；Alaya 文件 [INFERRED]（未读） |
| S10 | 记录分工：立项/决策记录曾放 Mnemosyne target-projects/myos2/（README.md 末句）；自本轨道起新记录只进 agent-workspace/lead/MYOS2-LEAD-NNN/；Mnemosyne 侧只追加一份移交记录后停笔 | agent-workspace/README.md 末句（旧）→ 工作令 §7.2、§10（新）；本分支 README.md 追加节已登记 | Owner 2026-09-02 迁移指示（工作令 §1.1）＋起草方操作化 | Owner 原话部分**已生效**；lead/ 目录登记待 Owner 合并 PR #6 追认 | [VERIFIED agent-workspace/README.md 末句"对应的立项与决策记录在 Mnemosyne 仓库 target-projects/myos2/ 下"]；[VERIFIED 00-owner-work-order-verbatim.md §1.1 2026-09-02 段] |
| S11 | Owner 沟通：对话回复＝人类频道（纯人话、只写意义与需 Owner 做什么）；文件＝agent 频道（仪式/证据内容进文件）；提问附三件套；回复末尾一行"本轮推进了什么｜下一步仓库写入：是/否/待授权" | Mnemosyne 09-continuation/02-owner-correction-dual-channel-rules.md §3；末尾一行为 Mnemosyne guard 惯例（工作令 §5 转述） | Owner 纠正（跨会话转达件，按 Owner 纠正处理）＋起草方操作化 | 视为**已生效**（Owner 纠正） | [VERIFIED Mnemosyne …/02-owner-correction-dual-channel-rules.md §3 第 1~3 条]；末尾一行的 guard 出处 [INFERRED]（未读 Mnemosyne guard 文件） |
| S12 | 署名：重要提交带四行尾注（Agent-Action-Actor / Agent-Task / Agent-Run-Context / Agent-Content-Producer）＋ Co-Authored-By；Producer 取实际执行模型不得继承模板；无尾注不推定人类 | Mnemosyne notes/registries/multi-writer-attribution-convention.md §1、§4、§5；MyOS2 d231708c 提交已用 | 起草方惯例（Mnemosyne 侧 Owner 终审定稿 v1.0，对 MyOS2 无正式效力） | **待追认**（对 MyOS2 仓） | [VERIFIED Mnemosyne …/multi-writer-attribution-convention.md §1]；[VERIFIED git log -1 d231708c 含四行尾注] |
| S13 | 降级交付拆分纪律：正文未包在代码围栏内、无法机械拆分的交付件，留在 received/ 原件里，不放半截文件 | agent-workspace/results/EXTRACTION-NOTE.md | 起草方立；Owner 默认 [MI] | **待追认** | [VERIFIED agent-workspace/results/EXTRACTION-NOTE.md "宁可留在原件里"段] |

## 2. 本轨道新增的暂定规则（来自工作令正文，Owner "开工"后生效；未来可并入 S 表）

| # | 规范 | 出处 | 状态 |
|---|---|---|---|
| W1 | 硬约束十条：隔离原则、主检出 time 不动、本地 master 过期勿用、推送显式完整 URL（禁 `git push upstream/origin`）、危险脚本绝不执行、SHA 纪律、一任务一分支一 PR、不动 PR #1~#5 与遗留分支/worktree、不写 Mnemosyne/Meta-Agent 仓、自动记忆只存 Owner 偏好 | 00-owner-work-order-verbatim.md §4 | 生效（"不因任何补充说明而放松，除非 Owner 逐条明示"） |
| W2 | 存储底线六条（S1 接管协议五条＋"交接自足"） | 同上 §1.3 | 生效 |
| W3 | 写入授权默认值：worktree /home/cheyh/projs/myos2-agent-ws、分支 agent/MYOS2-LEAD-001、只新建 lead/ 与 results/<新号>/ 与 tasks/<新任务书>；维护件只增或 supersedes；下一个任务号 012 | 同上 §7 | 生效（Owner "开工"＝接受默认） |
| W4 | 记录纪律：产出头七字段＋track_id/record_type/evidence_class；同族局限声明；每子步骤 commit＋push；每门续接检查点；上下文约 70% 先落盘 | 同上 §9 | 生效 |
| W5 | 阶段与门：0 接手定向 → 1 事实基线冻结 → 2 第二波发射与回收 → 3 交叉综合 → 4 实施类 → 5 需求⑤ → 收口；每门停等 Owner 一字批示 | 同上 §8、§12 第 7 条默认 | 生效（草案，Owner 可增删） |

## 3. 已知的 S 表内部矛盾与缺口（登记；裁决权在 Owner）

1. **004R 的执行者**：工作令 §5 末段称 004R 任务书 mode 写"普通对话 Pro"，而 WAVE-2-PLAN §2 把 L3（重跑 004R）派给本地 Claude、§1 又只列 010/011 为 GPT 任务——逐字核对结果与建议见 01-orientation-report.md open items。
2. **S9 与 W1.9 的 Alaya 写入授权**：Owner 2026-08-31 归档原话含"本条即授权 Alaya 写入"（Mnemosyne 06 记录 YAML `alaya_write_authorization`），但本轨道工作令 §4.9 与 §12 第 6 条默认要求单独同意——以本轨道工作令为准，等 Owner 一句话。
3. **S6 的"master @ 63f0785c"表述**：conventions.md §2/§4 写的是内核内容口径；如今 origin/master = d231708c（其上只叠了 5 个工作区提交，内核内容仍等于 63f0785c，`git diff time...origin/master -- mykernel/ myloader/ myinitramfs/` 为空）。不改公约，在定向报告说明。
4. **S1 接管协议五条 vs 工作令 §1.3 六条**：第 6 条"交接自足"是本轨道新增，尚未写入 README（本分支只追加登记了 lead/ 目录）；是否把第 6 条追加进 README "未来接管协议"待 Owner 一句话。
5. **S12 对 MyOS2 无正式效力**：本会话提交沿用四行尾注（已用于本分支全部提交），但 GPT 会话（ChatGPT 连接器）能否写尾注未实测（Mnemosyne 惯例 §7 判为可 fallback 到 PR 来源区块），第二波任务书按"可选、丢失不阻断"处理。

## 4. 同族局限声明

S1~S5、S7、S8、S13 由 Mnemosyne 主导会话（claude-fable-5 族）起草；本会话为同族同模型，对其"是否合理"的复核不构成独立复核。本清单只核验"出处存在且摘要与原文一致"，不裁定其正确性。
