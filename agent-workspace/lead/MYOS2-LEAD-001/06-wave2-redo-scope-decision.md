# 第二波重做范围决策记录（MYOS2-LEAD-001 · 响应 Owner 补充说明第 3 条）

```yaml
task_id: MYOS2-LEAD-001
track_id: MYOS2-LEAD-001
record_type: wave2_redo_scope_decision
evidence_class: B（主导会话判断，依据 C 类评审与本会话取证）
produced_by: claude-fable-5.1@claude-code-vscode（主导会话，UUID 3bfbb342-1b15-4461-a4c3-987f17aad103）
date: 2026-09-05
base_snapshot: 工作区 origin/master @ c06048260c7076588bb126c9206b61284391784a（PR #6 合并后）；内核 time @ a039d9803ade2a1613d620bda375e028530d5242
authorization_ref: 00-owner-work-order-verbatim.md §13 第 3 条（Owner 2026-09-05）
inputs_read: [WAVE-1-REVIEW.md 全文, tasks/ 全部 12 份原任务书, results/*/MANIFEST*.md, 05-wave1-fabrication-ledger-and-redo-plan.md, Mnemosyne multi-writer-attribution-convention.md §6]
status: final（决策记录）；任务书与发射清单为 draft
open_questions:
  - 004R 执行者归属（OI-1）仍待 Owner；本记录按"GPT 执行、本地核验"列入发射清单
  - 008R 是否发射由 Owner 按额度定
```

## 1. Owner 的问题："是不是第一次的 ChatGPT Pro 任务都最好重做一遍？"

答：**接近全部，但不是机械地全部**。判断标准只有一条：**该产出在协议 v1 的机械闸门下能不能被验收**。第一波九份没有一份能过（要么溯源字段编造，要么证据锚点腐烂，要么外部出处不可解析），所以：

| 类别 | 任务 | 处置 | 为什么不是"照原任务书再跑一次" |
|---|---|---|---|
| 含编造，必须重做 | 001、003、005 | 001R、003R、005R（阶段 0 已起草） | 重做以勘误为主件、旧目录不改，避免把可用的框架也扔掉 |
| 无编造，但证据层过不了闸门 | 002、007、009 | 002R、007R、009R（本次起草） | 002 缺双轴与 UP/SMP；007/009 的外部证据 0 个可解析出处；三者内容框架可复用 |
| 无编造，机械问题为主 | 008 | 008R **可选**；本地 L4/L5 可替代 | 行号重锚是机械活，本地更便宜；GPT 版本只在"扩覆盖"上有增量 |
| 零交付 | 004 | 004R（前会话已起草） | 无产出可勘误，是真正的复跑 |
| 深度研究＋落地混合 | 006 | 010（外部部分，已起草）＋本地 L6 与假 SHA 替换 | 落地部分需要跑 QEMU，GPT 做不了 |

不重做的部分：006 的落地映射（本地）；008 的机械重锚若 Owner 不发 008R 则本地做；007 的三条路线与 009 的四篇正文框架**保留**（只补证据与过滤）。

## 2. 交付清单（本次新增）

- `agent-workspace/tasks/MYOS2-DR-002R-subsystem-completeness-reanchor.md`
- `agent-workspace/tasks/MYOS2-DR-007R-importance-evidence-and-tiers.md`
- `agent-workspace/tasks/MYOS2-DR-008R-tech-debt-register-reanchor.md`
- `agent-workspace/tasks/MYOS2-DR-009R-teaching-blueprint-sources.md`
- `agent-workspace/WAVE-2-LAUNCH.md`（发射清单 v3：逐任务判定、10 个对话的总表、两套启动提示词、研究课题标题、回收/闸门/署名流程、勾选表）

连同阶段 0 的 `tasks/00-gpt-task-protocol-v1.md`、001R/003R/005R，以及前会话的 004R/010/011，第二波 GPT 侧共 **10 个任务、10 份任务书、2 套提示词**。

## 3. 署名标注规则（Owner 2026-09-05 指示：回收后标注为 gpt6-astra 完成）

- GPT 会话自报的 `produced_by` **原样保留**（它是会话侧证据，可信度低但不可改写）；
- 主导会话在回收登记处（`results/ARCHIVE-RECEIPTS.md` 批次 2、WAVE-2-REVIEW 每条记录、本目录的回收检查点）写 `model_per_owner: gpt6-astra（Owner 2026-09-05 告知，direct_user_instruction）`；
- 依据：Mnemosyne 多写入方署名惯例 §6（ChatGPT 表面无可靠运行时自识别；Owner 手动切换模型时的告知按 direct_user_instruction 记录）；工作令 S12；
- 若 Owner 某次对话用了别的模型，Owner 一句话即改该条；主导会话不做任何推断。

## 4. 与既有文件的关系

- WAVE-2-PLAN.md §0/§2/§3/§4 继续有效；其 §1 与 §5.2 的启动提示词被 WAVE-2-LAUNCH.md §3 取代（supersedes 已在 WAVE-2-LAUNCH 头部声明，WAVE-2-PLAN 原文不改）。
- LAUNCH.md 是第一波历史记录，不动。
- 05 文件 §4 的"起草的任务书"表因本次新增 002R/007R/008R/009R 而不完整，以 WAVE-2-LAUNCH.md §1 为准（05 文件不改，按只增原则由本记录补充）。

## 5. 本地配套（发射前后各一件，均待 Owner 一字批示后取号）

- 发射前：引文闸门脚本（本地 A）——没有它，回收件只能人工验收，协议 P9 落空；
- 回收后：假 SHA 机械替换 -v2（本地 B）、配置面真相表（本地 D）。

## 6. 同族局限声明

本记录的"应重做/可选"判断是主导会话（Fable 5.1）对同族前会话评审结论的再判断，不构成独立复核；任务书草案未经对抗评审（计划在提交后用子代理做一轮反驳式评审并出修订版）。

## 7. 对抗评审结果与 v2 改版（2026-09-05 晚追加）

- 评审工作流：11 份面向 GPT 的文件 × 3 视角＋1 完整性批评者，共 37 个子代理。**只有 004R 的三个视角完成**（约 28 万 token、60 次工具调用）；其余 34 个因 Owner 账号的月度用量上限（提示 21:00 重置）失败，未消耗。
- 004R 三视角一致判"不能放行"，共 27 条发现（3 blocker、13 major、11 minor），全部已吸收：v1 任务书与协议 P1 硬冲突（要求粘贴完整 commit）；"至少四条历史 S1 线索"在 008 台账里不可满足（三 S1 一 S2）；把含编造的 001 当衔接对象；十件"可编译代码"对不能运行编译器的执行者不可兑现且不可检；003 的坏行号会被转抄；输入体量约 200 KB 不现实；lockdep 实现与 011 冲突；串口日志规范与决策 5 矛盾；[VERIFIED] 无引文格式要求、self_check 数字自报、开工自检落在聊天里、读错分支闸门抓不到。
- 可推广的六类漏洞 → 协议 v2（`tasks/00-gpt-task-protocol-v2.md`，自足，13 条）：40 位十六进制正则拒收；引文须在符号定义体内、强制锚点不得降级；分支金丝雀（两行在 master/time 不同）；self_check.verified_claims 绑定标签计数、开工自检引文进 MANIFEST；代码件用改动块＋symbols_referenced；第一波产出使用纪律（只取结构/函数名/ID）；输入文件级清单与输出上限；降级交付一文件一回复。
- 任务书处置：004R → v2 全文重写；001R/003R/005R（已在 master）→ 各出 v2 文件并 supersedes；002R/007R/008R/009R（未合并）→ 原地补强（协议 v2、输入清单、强制锚点、MANIFEST 字段、验收项）；010/011（前会话起草，已在 master）→ 不改，协议要求由提示词带入。发射清单 v3 修订 1 已同步。
- 本会话对协议 v2 与七份任务书的补强是**自评自改**，未经独立复核（同族局限）；待 Owner 决定是否补评。
