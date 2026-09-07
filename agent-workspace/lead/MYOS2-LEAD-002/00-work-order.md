---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: work_order_with_owner_supplements
evidence_class: "Owner 原话登记；不含回收评审或验收裁定"
produced_by: "newest gpt6"
model_per_owner: gpt6
produced_by_source: "Owner 2026-09-07 告知的网页端模型显示名，见 §12 第 1 条；不是本会话直接读取客户端界面。"
effort_per_owner: pro
ios_display_per_owner: "6 pro"
attribution_scope: "本 YAML 头与 §12 补充登记；原工作令起草者仍见下文 drafted_by，原文保留。"
date: 2026-09-07
base_snapshot: "agent/MYOS2-LEAD-002（分支名）"
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md
authorization_ref: "Owner 本轮开工指令；工作令 §2.5、§7、§12 第 1~2 条"
status: ACTIVE_AFTER_OWNER_GO
owner_go_received: true
local_validation: "待本地；本轮未执行命令。"
open_questions: []
---

# 工作令 · MYOS2-LEAD-002（ChatGPT Pro 主导会话；接手 MyOS2 分析计划主线）

> 本文件是一份完整工作令。请**先完整读完全文**（能看到最后一节"§12 Owner 补充说明登记处"才算完整）。读不完整就停止并报告，不要开始任何工作。Owner 把本文件链接发给你即构成任务授权；Owner 可在补充说明中追加、修改或删除任何条款。

```yaml
track_id: MYOS2-LEAD-002
record_type: work_order_for_gpt_lead_session
drafted_by: MYOS2-LEAD-001（本地 Claude Code 主导会话，claude-fable-5.1@claude-code-vscode，2026-09-07）
issued_by: Owner（发送链接即授权）
executor: 一个 ChatGPT Pro 对话（普通对话模式，GitHub 连接器读写；外部调研时可临时开深度研究，但主线记录写库须在普通回合）
predecessor: MYOS2-LEAD-001（其记录目录 agent-workspace/lead/MYOS2-LEAD-001/；本轨道接续其工作，不复用其任务号；MYOS2-LEAD-001 退场与否由 Owner 定）
repo: github.com/08822407d/MyOS2（public）；工作区 = agent-workspace/（master）；内核基线 = 分支 time
role_after_launch: MyOS2 分析计划的主导会话（规划、回收评审、记录、决策清单、交叉综合、下一波任务书、接管包）
not_in_scope: 机械闸门中的本地命令项、本地任务 L/A~E、内核改动、Alaya 归档、跨会话消息、合并 PR（见 §1.2）
status: draft（Owner 发出即生效）
```

## 0. 一句话

你接手 MyOS2 分析计划的主线：读第二波深度研究的回收件、按协议评审裁定、写记录、把要 Owner 定的事用人话列出来、做交叉综合、规划第三波。你没有本机，所以凡是要跑命令的事都不归你（§1.2）；你给出的验收结论最高只能是"PASS_PENDING_LOCAL"，本地闸门过了才是 PASS。**先做 §2 启动仪式，然后停下等 Owner 说"开工"。**

## 1. 职责

### 1.1 你接手的（可做）
1. 第二波回收：对 10 个任务（001R/002R/003R/004R/005R/007R/008R/009R/010/011）的回收件做 §5 的"可读闸门"＋语义评审＋可靠性裁定，写 `agent-workspace/WAVE-2-REVIEW.md`（新建，格式仿 WAVE-1-REVIEW.md）；每份回收件的 MANIFEST 消费说明由你补充（新增文件，不改 GPT 会话交付的原件）。
2. 记录：`agent-workspace/lead/MYOS2-LEAD-002/` 下的启动回执、门台账、续接检查点、open items、决策清单人话版。
3. 工作区维护件的**追加**（README.md、conventions.md、WAVE-2-LAUNCH.md、results/ARCHIVE-RECEIPTS.md、results/README.md）——公约/协议的实质变更先经 Owner。
4. 阶段 3 交叉综合：002R × 007R × 003R 的 YAML → 学习主攻清单与裁剪方案（写 `results/<取号>/`，取号从 013 起；012 预留给本地引文闸门脚本）。
5. 第三波任务书与发射块（沿用 WAVE-2-LAUNCH.md §7 的格式与协议 v2）。
6. 给未来 Meta-Agent 的接管快卡维护（`lead/MYOS2-LEAD-001/09-continuation/00-quick-card-handover.md` 的后继版本放你的目录，写 supersedes）。

### 1.2 你不接手的（由 Owner 本人或本地会话做；你只登记"待本地"）
机械闸门中的本地项（`git apply --check`、全树符号 grep、函数体范围核对、`git cat-file -e`）；本地任务（配置面真相表、假 SHA -v2 替换、事实重锚、构建诊断、QEMU 冒烟、引文闸门脚本）；任何内核本体改动；Alaya 归档（私仓）；与本机其他会话的消息；合并任何 PR；关闭 PR #1~#5；对本机状态（worktree、本地分支、Downloads 文件）的任何断言。

## 2. 启动仪式（第一条回复必须完成；完成后停止等 Owner）

只读；唯一写入是你的启动回执（一个新分支、一个 PR）。

1. **读取顺序**（连接器读不到就用 raw URL `https://raw.githubusercontent.com/08822407d/MyOS2/master/<path>`；两条通道都失败就停止并报告，不猜内容）：
   - `agent-workspace/README.md`、`agent-workspace/conventions.md`、`agent-workspace/tasks/00-gpt-task-protocol-v2.md`（对你也硬性适用）、`agent-workspace/WAVE-2-LAUNCH.md`；
   - `agent-workspace/lead/MYOS2-LEAD-001/09-continuation/00-quick-card-handover.md`（接管快卡：目标/状态/决定/禁令/未知/oracle）；
   - `agent-workspace/lead/MYOS2-LEAD-001/03-provisional-execution-sources.md`（暂定执行源 S1~S13、W1~W5）；
   - `agent-workspace/lead/MYOS2-LEAD-001/01-orientation-report.md` §3~§4（open items 与 12 条决策）；
   - `agent-workspace/lead/MYOS2-LEAD-001/checkpoints/` 下日期最新的一份；
   - 按需（开工后再读）：`WAVE-1-REVIEW.md`、`05-wave1-fabrication-ledger-and-redo-plan.md`、`06-wave2-redo-scope-decision.md`、`07-gpt-lead-takeover-feasibility.md`。
2. **自检**：在回复中逐字引用 conventions.md §1 第 2 条与协议 v2 的 P2 标题行；引不出来说明读取失败，停止。
3. **回执**：新分支 `agent/MYOS2-LEAD-002`（从 master 建），新增 `agent-workspace/lead/MYOS2-LEAD-002/00-startup-receipt.md`：
   ```yaml
   record_type: startup_receipt
   track_id: MYOS2-LEAD-002
   produced_by: <界面显示的模型名，原样>
   model_per_owner: gpt6（Owner 2026-09-07 告知）
   read_channel: connector | raw-url | mixed
   files_read: [实际读取清单]
   startup_selfcheck_quote: "<公约 §1 第 2 条整句>"
   five_things: {现行规则在哪, 做到哪一步, 哪些门待 Owner, 哪些是暂定, 下一步安全动作}   # 用你自己的话各一句
   first_impressions_max_5: [标 [INFERRED]]
   questions_for_owner: [至多 3 个，每个带：人话意思 / 答了会怎样 / 不答会怎样]
   status: READY_WAITING_FOR_OWNER_GO | BLOCKED
   ```
   向 master 开 PR（标题 `MYOS2-LEAD-002: startup receipt`）。
4. **向 Owner 回报**（对话，人话，≤15 行）：核对了什么、对不上的、已就位等"开工"、本轮没有写任何回执以外的文件；问题至多 3 个带三件套；末尾一行"本轮推进了什么｜下一步仓库写入：是/否/待授权"。
5. 然后停止。Owner 的补充说明逐条登记到 §12（逐字，标日期序号）并复述，不据此开工；只有 Owner 说"开工/开始"才进入 §7。

## 3. 硬约束（不因补充说明放松，除非 Owner 逐条明示）
1. 隔离原则：agent-workspace/ 之外一切是 Owner 的内核本体，任何 agent 不得修改；对内核的修改建议只以补丁/改动块放 results/。
2. 一任务号一分支至多一个 open PR；提交前缀 `MYOS2-LEAD-002: `（维护件）或 `<任务号>: `（产出）；PR 到 master 由 Owner 合并；禁 force-push、禁直推 master。
3. 不写 40 位 commit SHA；引用仓库状态只写分支名（协议 P1 对你同样适用）。
4. results/<既有任务号>/ 原件（含错误）一字不改；修订用新文件＋`supersedes:`。
5. 不关闭/合并 PR #1~#5；不删任何分支；不改 remote。
6. 零交付优于假交付：读不到就报告；编造一条＝本轨道记录作废。
7. 公约/协议只追加或 supersedes；实质变更先经 Owner 一句话。
8. 署名：每个你写的文件 YAML 头含 `produced_by: <界面模型名>` 与 `model_per_owner: gpt6（Owner 2026-09-07 告知）`；PR 描述含 `execution_context`（授权引用、复核状态、已知限制）。
9. 你对本机状态一无所知：不断言 worktree/本地分支/Downloads/Alaya 的任何事，需要时写"待 Owner 确认"。
10. Owner 原话 > 本工作令 §12 补充 > 03 文件的 S1~S13 > 你的判断；冲突记 open item 报 Owner，不自裁。

## 4. 暂定执行源
见 `lead/MYOS2-LEAD-001/03-provisional-execution-sources.md`（S1~S13、W1~W5，全部 provisional）。对你最常用的：S2 写入规则、S3 产出格式、S4 词汇表、S5 溯源纪律、S8 第二波硬纪律、S9 归档（你只登记 GitHub 侧）、S11 沟通、协议 v2。

## 5. 你能做的闸门（协议 P9 的"可读子集"）与裁定等级

对每份回收件出 `results/<任务号>/GATE-REPORT-by-LEAD-002.md`（新增文件），YAML 逐项：

| P9 项 | 你怎么做 | 结果值 |
|---|---|---|
| 1 40 位十六进制 | 通读全部交付文件，搜索 40 位 `[0-9a-f]` 串 | pass / fail |
| 2 引文命中 | 强制锚点**全部**、其余抽样 ≥30%：打开 time 分支 raw 文件，逐字比对引文，并确认引文在标签所指函数/宏体内 | pass / fail / partial（写抽样比例） |
| 3 路径存在 | 用 tree URL 逐个确认 | pass / fail |
| 4 行号越界 | 如有行号，对照 raw 文件行数 | pass / fail / n/a |
| 5 self_check 计数 | 数全部 `[VERIFIED` 标签，比对 MANIFEST | pass / fail |
| 6 URL/DOI | 每条 [EXTERNAL] 有 URL/DOI；抽样打开 ≥5 条核"内容支持结论" | pass / fail / partial |
| 7 分支金丝雀 | 对照 time 与 master 的两份 raw 文件 | pass / fail / n/a |
| 8 开工自检引文、read_channel | 对照 conventions.md | pass / fail |
| 9 symbols_referenced | 逐个在 time 分支查（tree/raw）；查不到的列出 | pass / partial / **pending_local** |
| 10 改动块 before 引文 / diff | before 引文按第 2 项核；`git apply --check` **pending_local** | pass / pending_local |
| 11 无"可编译/可运行/已验证"字样 | 通读 | pass / fail |
| 12 围栏可拆分（降级件） | 每文件独立围栏＋路径行 | pass / fail / n/a |
| 13 强制锚点齐全 | 对照任务书 §强制锚点 | pass / fail |

裁定：任一 fail → `RETURN`（退回原对话返工，引用具体判据）；全部 pass 且无 pending → `PASS_PENDING_LOCAL`（你的最高结论；本地闸门过后由 Owner/本地会话改为 PASS）；有 partial/pending 而无 fail → `PASS_PENDING_LOCAL` 并列出待本地项。你**不得**出 `PASS`。

语义评审与可靠性裁定（low/medium/high）写进 WAVE-2-REVIEW.md，每份固定一节"同族局限声明：本裁定由 GPT 会话对 GPT 会话产出作出，不构成异族独立复核"。

## 6. 回收流程
1. GPT 任务会话若开了 PR：不合并，读其分支文件做 §5；结果写在你的分支上（GATE-REPORT）；Owner 决定合并顺序。
2. 对话降级件：Owner 把导出内容贴给你或上传；你在 `results/<任务号>/received/` 原样入库（一字不改，文件名 `<任务号>-<角色>-<日期>.md`），然后按公约拆分成独立文件（拆不开的留原件，仿 EXTRACTION-NOTE.md 登记）。
3. 署名：回收件 MANIFEST 的 `produced_by` 保留原值；你在 GATE-REPORT、WAVE-2-REVIEW、ARCHIVE-RECEIPTS 批次 2 的每条记录写 `model_per_owner: gpt6（Owner 2026-09-07 告知）`。
4. ARCHIVE-RECEIPTS 批次 2：只登记 GitHub 侧（路径、bytes、你数出的字符数；sha256 留空写 `pending_local`）；Alaya 侧留待 Owner/本地会话。
5. 每回收一份，续接检查点追加一行。

## 7. 阶段与门（"开工"后；每门停等 Owner 一字批示）
| 阶段 | 做什么 | 交付（lead/MYOS2-LEAD-002/） |
|---|---|---|
| 0 接手定向 | 读通 §2 按需清单；出"接手报告"（现状复述、待本地项清单、你对 12 条决策的补充问题） | 01-takeover-report.md |
| 2 第二波回收 | §5/§6；WAVE-2-REVIEW.md；每份 GATE-REPORT | results/<任务号>/GATE-REPORT-by-LEAD-002.md、WAVE-2-REVIEW.md |
| 3 交叉综合 | 002R × 007R × 003R → 主攻清单与裁剪方案 v1；三套分级口径映射（用 007R 的提案，交 Owner 定） | results/013-…/ |
| 3b 第三波 | 任务书＋发射块 | tasks/、WAVE-3-LAUNCH.md |
| 收口 | 接管快卡新版 | 09-continuation/ |
阶段 1（事实基线冻结）与阶段 4（实施类）的本地部分不归你，只在门台账登记"待本地"。

## 8. 记录纪律
- 每个文件 YAML 头：task_id / track_id / record_type / evidence_class / produced_by / model_per_owner / date / base_snapshot（写分支名；可加从连接器复制的 12 位短 SHA 并标"短 SHA"）/ inputs_read / status / open_questions。
- 证据标签 `[VERIFIED path::symbol]`（附引文）/ `[INFERRED]` / `[EXTERNAL url]`；对仓库状态类事实可加 [VRF]/[MI]。
- 每个子步骤完成即提交并推到你的分支（信息保全优先）；每门写续接检查点（五件事＋写入清单＋会话外状态"未知"）。
- 上下文吃紧时先落盘检查点再提醒 Owner；允许换新对话接力，新对话从最新检查点起。

## 9. 与 Owner 沟通
回复纯人话，技术进文件；每个问题带三件套（人话意思 / 答了会怎样 / 不答会怎样）；末尾一行"本轮推进了什么｜下一步仓库写入：是/否/待授权"。

## 10. 与本地会话的关系
MYOS2-LEAD-001（本地 Claude）在 Owner 明示前不退场；两者分工：你做 §1.1，它做 §1.2 中的本地项。你们之间不直接通信，一切经 Owner 或经仓库文件（你写"待本地"清单到检查点，它读后执行并回写）。

## 11. 启动提示词（Owner 复制到一个全新的 ChatGPT Pro 普通对话）

```text
你将接手 MyOS2 项目分析计划的主线工作，任务号 MYOS2-LEAD-002。仓库 08822407d/MyOS2 是 public 仓库。
第一步：完整读取工作令 https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md （必须看到最后一节 §12 才算完整；读不完整就停止报告）。连接器读不到就用 raw 链接；两条都失败就停止，不猜内容。
第二步：只做工作令 §2 的启动仪式（按顺序读指定文件、逐字引用公约 §1 第 2 条与协议 v2 的 P2 标题行、在新分支 agent/MYOS2-LEAD-002 上写启动回执并开 PR、用人话向我回报 ≤15 行），然后停下等我。
在我说"开工"之前：不评审任何回收件、不写回执以外的文件、不合并任何 PR、不回答决策清单；我的补充说明可能分几条，每条你只逐字登记到 §12 并复述。
硬性：不写 40 位 commit SHA；你写的每个文件 YAML 头含 produced_by（界面显示的模型名原样）与 model_per_owner: gpt6；你没有本机，凡要跑命令的事一律登记"待本地"，你的验收结论最高只能是 PASS_PENDING_LOCAL。
```

## 12. Owner 补充说明登记处

（由 MYOS2-LEAD-002 会话回填 Owner 在对话中所说的话，逐字、标日期与序号；Owner 不必编辑本文件。与前文冲突时以补充为准并注明改了哪条。）

### 第 1 条 · 2026-09-07 · 模型界面显示

Owner 原话（逐字，含入口标记）：

> @GitHub 在网页端入口可见模型写的是newest gpt6，effort pro；在ios的app界面上看到的是6 pro

关联：§2.3、§3.8 与协议 v2 P13 的署名要求；本条补充启动回执的 STARTUP-Q1。网页端模型名、effort 与 iOS 显示名分别保留，不合成为自拟名称；原启动回执不改写。本条不修改开工条件，不构成“开工”指令。

### 第 2 条 · 2026-09-07 · 开工

Owner 原话（逐字，含入口标记）：

> @GitHub 开工

关联：满足 §2.5 的开工条件，按 §7 先进入阶段 0 接手定向；完成该阶段后仍按“每门停等 Owner 一字批示”停止。此条不替 Owner 回答十二条决策，不取消待本地边界，不授权合并任何 PR；不修改上述硬约束。

### 第 3 条 · 2026-09-07 · 沟通应围绕目标效果，尊重 Owner 的注意力限制

Owner 原话（逐字，含入口标记）：

> @GitHub 现在简单地做些约定.你在向我说明工作内容,工作结果,或者其他希望知悉我的内容是要用人类能看懂的方式组织语言,这里的人类能看懂不止是要求"说人话",最终要的是要考虑作为人类的我信息承载能力有限,不是你输出的每一句话我都会看(包括你写入仓库的内容,不只是在对话界面上输出的文字).我在数个月和AI模型合作的过程中已经确定了一项习惯:在默认情况下,我只关心我希望达成的效果,而不关心也不干涉你们AI怎么工作.如果你不太清楚我是什么意思,你可以参考一下Mnemosyne的相关行为约束的背景.

关联：细化 §9 与 §2.4 的沟通要求，并明确 §8 的完整记录不等于要求 Owner 全文阅读。不能把“技术进文件”理解成只要塞进仓库，就已向 Owner 说明或获得同意。

执行理解（LEAD-002 的复述，不是新增 Owner 原话）：默认围绕目标达成情况、剩余差距、重要风险及确需 Owner 取舍的事项汇报。在已有授权内，工作拆分、核查安排、记录组织等由 AI 负责，不把内部流程选择当成 Owner 的日常管理任务；必须请 Owner 决定时，先说明对结果的影响并给出建议，而不是只摆流程术语。

聊天、仓库文档及 PR 的面向 Owner 入口都应让关键结论先出现，必要细节分层保留供追溯与接手。不能假定 Owner 会读完每句话，不能把“已输出／已入库”视为“已知悉／已同意”；影响目标或决定的重要失败、限制和不确定性仍须直接说明，不得以精简为由隐藏。

本条是沟通约定，不是对既有权限边界的逐项解除；本轮只登记补充，不据此启动第二波评审、合并 PR 或执行本地命令。

### 第 4 条 · 2026-09-07 · Mnemosyne 仓库与行为约束借用来源

Owner 原话（逐字，含入口标记）：

> @GitHub 刚刚我让你参考Mnemosyne的说明,我需要说清楚,这是一个我建立的AI Agent项目,它的相关内容都在名叫Mnemosyne的仓库里,地址是git@github.com:08822407d/Mnemosyne.git,该仓库对本账号开放了较高的访问权限,因此如果你不确定它的内容的话可以自行访问相关文件看看.它当中还有许多行为规范(在该仓库里应该叫做"Mnemosyne约束指导")可供本项目的各agent对话/任务借用.

关联：补充第 3 条的背景出处与读取许可；明确 Mnemosyne 指 Owner 建立的 `08822407d/Mnemosyne` 项目。本轨道可以自行按需读取有关行为约束，不需要 Owner 搬运文件或重复说明。没有获得对 Mnemosyne 的写入、任务接管或操作授权；MyOS2 的既有硬约束不因连接器权限较高而改变。

本轮读取发现（LEAD-002 记录，不是新增 Owner 原话）：已通过连接器读取 Mnemosyne 的 master 分支文件；“加载 MNEMOSYNE 约束指导”的实际入口是 [commands/load-mnemosyne-guidance.md](https://github.com/08822407d/Mnemosyne/blob/master/commands/load-mnemosyne-guidance.md)。它要求保持当前对话任务主线，借用行为约束不等于导入该项目的任务或权限。本轮是针对性参考，不宣称已完成整套指导加载。

对 MyOS2 的借用理解：AI 在已有授权内承担信息筛选、工作组织和内容核查责任；Owner 合并 PR 不构成全文阅读、技术正确或测试通过的证明。给 Owner 的当前操作应显眼且与 AI 内部安排分开；输出和入库不等于告知或同意。第 3 条明确的注意力限制同样适用于仓库文档，因此不能只把大段流程搬进文件，就要求 Owner 自行找要点。仍需本地执行的核对如实登记“待本地”，不能为了减轻 Owner 负担而声称已做。

<details>
<summary>已读来源与借用边界（供后续 agent 按需追溯）</summary>

以下均为 `08822407d/Mnemosyne` 的 master 分支；未转抄原文件的完整提交标识或旧运行结论。

| 来源 | 本轮实际范围与用途 |
|---|---|
| [README.md](https://github.com/08822407d/Mnemosyne/blob/master/README.md) | 全文；确认项目定位与指导入口，不把列出的活动路线当作 MyOS2 任务。 |
| [MNEMOSYNE-AI-START-HERE.md](https://github.com/08822407d/Mnemosyne/blob/master/notes/ai-onboarding/MNEMOSYNE-AI-START-HERE.md) | 全文；识别只读参考、任务接管与写入的区别。 |
| [load-mnemosyne-guidance.md](https://github.com/08822407d/Mnemosyne/blob/master/commands/load-mnemosyne-guidance.md) | 全文；确认命令别名、分层入口及保留当前主线的边界。 |
| [user-operation-next-step-capability-and-intent-guard.md](https://github.com/08822407d/Mnemosyne/blob/master/current/user-operation-next-step-capability-and-intent-guard.md) | §1、§2 全部及 §3.1 的前部；只借用操作显眼、结果分层与问题须有上下文的原则，不申报全文。 |
| [agent-product-ready-pr-and-frontier-efficiency-guard.md](https://github.com/08822407d/Mnemosyne/blob/master/current/agent-product-ready-pr-and-frontier-efficiency-guard.md) | 文件头及 §1 至 §4；用于理解 Owner 不是逐行审阅者、Agent 承担审查责任、实际使用反馈的重要性；未读后续节，不整体移植其 Ready/Draft 等程序规则。 |
| [02-owner-correction-dual-channel-rules.md](https://github.com/08822407d/Mnemosyne/blob/master/notes/cross-model-review-results/FABLE5-REDESIGN-001/09-continuation/02-owner-correction-dual-channel-rules.md) | 全文；为理解本次沟通纠正而定向打开的历史材料。原件自称跨会话转达，不是 Owner 逐字原话；不把其本地执行记录当作本轮事实。 |

本轮没有读取 `current/human-approved-spec.md`、全部 guard 或其他项目的运行材料，不宣称完整加载、全面评审或已向其他会话传播这些约束。这里只提供已读来源与当前借用理解；后续 agent 仍需按自己的任务与适用范围读取。MyOS2 的直接指令及本工作令继续控制本轨道；本轮不改公共公约、任务书或其他轨道，不越过当前等待中的阶段门，不在 Mnemosyne 写入。

</details>
