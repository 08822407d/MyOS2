# 第二波发射清单 v3（2026-09-05；全部 GPT Pro 任务的任务书 / 启动提示词 / 研究课题一览）

```yaml
record_type: launch_sheet
wave: 2
version: 3（修订 2，2026-09-07：Owner 指示全部任务改深度研究执行、署名标 gpt6、暂不评审；新增 §7 逐任务发射块）
date: 2026-09-05
drafted_by: MYOS2-LEAD-001（本地 Claude Code 主导会话；claude-fable-5.1@claude-code-vscode）
supersedes: WAVE-2-PLAN.md §1 的启动提示词与 §5.2 的 v3 提示词（本文件为准；WAVE-2-PLAN 的分流原则、本地任务、决策清单、硬纪律仍有效）
protocol: agent-workspace/tasks/00-gpt-task-protocol-v2.md（所有 GPT 任务硬性适用；与任务书冲突时以协议为准；v1 已被 v2 取代）
status: draft（发射由 Owner 亲手；每个任务一个全新对话）
owner_instruction_2026_09_05: 第一波 ChatGPT Pro 任务按需重做；回收结果由主导会话标注模型（初为 gpt6-astra）
owner_instruction_2026_09_07: 暂不评审任务书；全部重做研究由 ChatGPT 深度研究执行；自此起 GPT 执行的工作一律标 model_per_owner: gpt6
review_coverage: 004R 任务书经 3 视角独立对抗评审（反编造 / 可执行性 / 事实一致性）并据此重写为 v2；其发现中可推广的部分已写入协议 v2 与全部任务书；其余 10 份文件的独立评审因账号用量上限未能运行（待补）
```

## 0. 要不要全部重做？——逐任务回答

| 第一波 | 判定 | 理由（一句话） | 第二波对应 |
|---|---|---|---|
| 001 外围工程审计 | **必须重做** | 假 SHA 20 处、两条问题整条虚构、最危险脚本被判"无害" | **001R**（勘误优先） |
| 002 完成度盘点 | **应重做** | 无编造，但 5 条证据路径不存在、成熟度轴混淆正确性、未分 UP/SMP——新闸门下过不去，且它是②的地基 | **002R**（重锚＋双轴） |
| 003 依赖图 | **必须重做** | 65 个行号引用大面积失效、越过文件末尾、rtc 误判 | **003R**（函数级重锚） |
| 004 调试桩 | 零交付 | 连接器故障，未编造 | **004R**（任务书 v2，经三视角评审重写；执行者归属见 §2 备注） |
| 005 x86 资料包 | **必须重做（勘误）** | 四处手册错值、假 [VERIFIED]、不存在的路径 | **005R** |
| 006 测试与 CI | 外部部分重做；落地部分本地 | 假 SHA 21 处、外部综述未覆盖任务书点名对象 | **010**（已就绪）＋本地 L6/假 SHA 替换 |
| 007 重要度与路线 | **应补做** | 无编造，但外部出处全不可解析、tier 计数错——重要度图不能建立在不可复核的证据上 | **007R**（补出处＋重算） |
| 008 技术债台账 | 可选 | 无编造；行号越界与覆盖不足，本地机械重锚即可解决；GPT 版增量在扩覆盖 | **008R**（可选） |
| 009 教学蓝本预研 | **应补做** | 出处全不可解析；空壳目录被排成教学单元；文件未围栏无法拆分 | **009R**（补出处＋过滤） |
| — | 新题 | lockdep-lite 调研 | **011**（已就绪） |

结论：**九个第一波任务里，三个必须重做（001/003/005），三个应重做或补做（002/007/009），一个可选（008），两个已有对应（004→004R，006→010＋本地）**。全部重做不是浪费——协议 v1 的意义就在于让重做的产出可以被机器验收，而第一波的没有一份能过闸门。

## 1. 任务总表（10 个 GPT 对话）

| 任务 | 标题 | 模式 | 优先 | 任务书 | 依赖 Owner 决策？ |
|---|---|---|---|---|---|
| MYOS2-DR-003R | 依赖图与初始化序列重锚 | **深度研究 Pro** | P0 | tasks/MYOS2-DR-003R-dependency-graph-reanchor-v2.md | 否 |
| MYOS2-DR-002R | 完成度矩阵重锚与双轴化 | **深度研究 Pro** | P0 | tasks/MYOS2-DR-002R-subsystem-completeness-reanchor.md | 否 |
| MYOS2-DR-004R | 调试桩复跑 | **深度研究 Pro** | P0 | tasks/MYOS2-DR-004R-debug-instrumentation-rerun-v2.md | 决策 5（console 通道）可选；执行者归属 OI-1 |
| MYOS2-DR-007R | 重要度证据补全与 tier 重算 | **深度研究 Pro** | P1 | tasks/MYOS2-DR-007R-importance-evidence-and-tiers.md | 否（决策 7 有答案则一并消费） |
| MYOS2-DR-010 | 补做外部测试实践综述 | **深度研究 Pro** | P1 | tasks/MYOS2-DR-010-testing-practice-survey-redo.md | 否 |
| MYOS2-DR-001R | 外围工程审计复跑 | **深度研究 Pro** | P1 | tasks/MYOS2-DR-001R-build-and-scripts-audit-rerun-v2.md | 决策 2/3/4（无则只能再问一遍） |
| MYOS2-DR-011 | lockdep-lite 调研 | **深度研究 Pro** | P2 | tasks/MYOS2-DR-011-lockdep-lite-research.md | 否 |
| MYOS2-DR-009R | 教学蓝本出处补全与过滤 | **深度研究 Pro** | P2 | tasks/MYOS2-DR-009R-teaching-blueprint-sources.md | 否（002R 回收后发更好） |
| MYOS2-DR-005R | x86 资料包勘误 | **深度研究 Pro** | P2 | tasks/MYOS2-DR-005R-x86-reference-pack-errata-v2.md | 否 |
| MYOS2-DR-008R | 技术债台账重锚（可选） | **深度研究 Pro** | P2 | tasks/MYOS2-DR-008R-tech-debt-register-reanchor.md | 否 |

全部并行安全（各写各的 results 目录与 agent/ 分支）。**执行模式：自 2026-09-07 起十个任务全部用深度研究 Pro**（Owner 指示）；任务书 mode 字段已同步。

## 2. 发射顺序建议

- **批 A（现在就能发，互不依赖）**：003R、002R、007R、010、011。
- **批 B（有条件）**：004R（Owner 先答 OI-1"由 GPT 还是本地"；决策 5 可选）、009R（等 002R 回收更准，也可现在发）、005R、008R（可选）。
- **批 C（等决策 2/3/4）**：001R。
- 备注 004R：v1 任务书要求"完整 commit 必须复制粘贴"、"至少四条历史 S1 线索"（008 只有三条 S1）、把含编造的 001 当衔接对象、十件可编译代码——三视角评审全部判"不能放行"；**v2 任务书已逐条修正**（协议 v2、四条历史线索三 S1 一 S2、前五件代码后五件伪码、lockdep 归 011、日志规范与后端解耦、决策 5 未答时按 0xE9 debugcon 兜底）。发射时只贴 v2 文件路径。
- 备注 010/011：任务书由前会话起草、已在 master，未改；协议 v2 的要求由 §3.2 提示词带入（与任务书冲突时以协议为准）。

## 3. 启动提示词 v3

### 3.1 （已作废，2026-09-07）普通对话模板不再使用——全部任务改用 §7 的逐任务发射块（深度研究）。

### 3.2 深度研究 Pro 通用模板（全部十个任务；§7 已按任务填好，直接复制 §7）

```text
你将执行 MyOS2 项目的外部调研任务 MYOS2-DR-<任务号>。仓库 08822407d/MyOS2 是 public 仓库，master 分支。
第一步：读取 https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/conventions.md 、https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/00-gpt-task-protocol-v2.md （反编造协议，硬性；与任务书冲突时以协议为准）、https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/WAVE-1-REVIEW.md ，以及你的任务书 https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/<任务书路径> （只读这一份）。若 GitHub 连接器可用也可直接用连接器读取。
第二步：严格按任务书的研究问题、输入清单、出处纪律、交付物与验收判据执行。
出处纪律是硬性要求（协议 P5）：每条外部结论必须附可解析的 URL 或 DOI；内部检索句柄（fileciteturn 之类）一律不计；引用手册给卷/章/表号；拿不到出处的结论不要写，列进 open_questions 并说明查过哪里。
不要申报任何 40 位 commit SHA；引用仓库状态只写分支名。第一波产出只取结构与结论框架，不转述其外部断言为已证实。
署名：MANIFEST 的 produced_by 原样填写你界面上显示的模型名称，不猜测后端。MANIFEST 还须含 startup_selfcheck_quote（逐字复制 conventions.md §1 第 2 条）、read_channel、self_check。
写入规则：深度研究期间连接器只读。研究完成后回到普通对话回合，若能写库则在新分支 agent/MYOS2-DR-<任务号> 上、agent-workspace/results/MYOS2-DR-<任务号>/ 目录内新增文件并向 master 开 PR；不能写库就在对话里逐文件输出：每条回复只含一个文件，独立代码围栏，围栏前一行写目标路径（第一波因未围栏导致无法拆分）。
开始前用三句话复述：你的任务目标、出处纪律的要求、你的交付物清单。复述无误后直接开工，过程中不要向我提问。
```

### 3.3 研究课题一览（深度研究任务的"课题"就是各任务书的"研究问题"节；此处只列标题便于开对话时命名）

- 007R：教学体系权重 / 工程现实权重 / 现代性维度——重要度证据补全与 tier 重算
- 009R：以简代繁的先例 / LLM 辅助代码教学证据 / 个人代码库作教材 / 课程骨架——出处补全与教学单元过滤
- 010：教学与业余内核测试实践 / Linux 侧补充 / QEMU 无头退出码约定 / GitHub Actions 约束 / bug 转回归
- 011：Linux lockdep 机制拆解 / 最小可行子集 / 教学化取舍 / 验证方法 / 单核价值

## 4. 回收、验收与署名标注（主导会话执行）

1. **回收**：GPT 开了 PR 就先不合并，主导会话拉取分支做闸门；对话降级件由 Owner 存盘（或上传压缩包）交主导会话入库到 `results/<任务号>/received/`。
2. **本地机械闸门**（协议 v2 P9 的十三项：40 位十六进制正则、引文 grep 与函数体范围、路径存在、行号越界、self_check 计数、URL/DOI、分支金丝雀、开工自检引文、symbols_referenced、改动块 before 引文、无"可编译/可运行"字样、围栏可拆分、强制锚点齐全）。任一不过 → 整份退回原对话返工，引用具体判据。闸门脚本＝本地任务 A，发射前应就位。
3. **对抗核查与裁定**：过闸门后按第一波方法（精读＋对抗核查）出 WAVE-2-REVIEW；可靠性裁定进 MANIFEST 的消费说明。
4. **署名标注（Owner 2026-09-05 指示，2026-09-07 修订）**：回收件的 MANIFEST `produced_by` 保留 GPT 会话自报值不改；主导会话在 `results/ARCHIVE-RECEIPTS.md` 批次 2 与 WAVE-2-REVIEW 的每条记录加 `model_per_owner: gpt6（Owner 2026-09-07 告知：自此起所有 GPT 执行的工作已换到 gpt6）`；日后模型再更新由 Owner 一句话改标，不影响已回收件的标记。依据：多写入方署名惯例 §6——ChatGPT 表面的模型自识别可信度低，Owner 告知按 direct_user_instruction 记录。
5. **归档**：过闸门的回收件按双仓制度进 Alaya 批次 2（Alaya 写入需 Owner 单独同意）。
6. **临时消费口径（Owner 2026-09-07 指示）**：本会话额度受限期间，回收件暂按"未编造"假定使用，但每份在 GATE-REPORT / 消费说明中标 `verification_status: provisional_unverified`，下游引用其结论须带此标记；约 2026-09-10 额度恢复后由主导会话按协议 P9 十三项＋对抗核查做正式核实，核实后改为 verified 或退回。此口径不改变验收等级（最高仍为 PASS_PENDING_LOCAL 直到本地闸门通过）。

## 5. 状态跟踪（Owner 手工勾选）

- [ ] 003R 已启动 / [ ] 已回收 / [ ] 过闸门
- [ ] 002R 已启动 / [ ] 已回收 / [ ] 过闸门
- [ ] 004R 已启动 / [ ] 已回收 / [ ] 过闸门
- [ ] 007R 已启动 / [ ] 已回收 / [ ] 过闸门
- [ ] 010 已启动 / [ ] 已回收 / [ ] 过闸门
- [ ] 001R 已启动 / [ ] 已回收 / [ ] 过闸门
- [ ] 011 已启动 / [ ] 已回收 / [ ] 过闸门
- [ ] 009R 已启动 / [ ] 已回收 / [ ] 过闸门
- [ ] 005R 已启动 / [ ] 已回收 / [ ] 过闸门
- [ ] 008R（可选）已启动 / [ ] 已回收 / [ ] 过闸门

## 6. 评审覆盖与已知局限（2026-09-05 晚）

- 对抗评审只完成了 004R 任务书（三视角 × 1 份）；其结论中可推广的六类漏洞已写进协议 v2 并落到全部任务书（输入清单文件级、强制锚点、金丝雀、self_check 计数绑定、改动块、不转抄第一波字段）。其余 10 份文件（协议 v2 本身、001R/002R/003R/005R/007R/008R/009R/010/011、本清单）的独立评审因账号用量上限未运行，属**未经独立复核的草案**；Owner 2026-09-07 指示**暂不评审**，草案按现状发射。
- 本地闸门脚本（本地 A）尚未存在；在它就位前，回收件只能人工按 P9 清单核对。
- 004R 执行者归属（GPT 还是本地）仍待 Owner（OI-1）。

## 7. 逐任务发射块（2026-09-07；每块整段复制到一个全新的 ChatGPT 深度研究对话；顺序即建议发射顺序）
> 前提：本文件所在 PR 合并到 master 后链接才有效；合并前想先发，把每个链接中的 `/master/` 换成 `/agent/MYOS2-LEAD-001/`。四份必读之外的链接（time 源码、results 目录）无需改。

### 7.1 MYOS2-DR-003R · 依赖关系图与初始化序列重锚（函数级）

```text
你将以深度研究模式执行 MyOS2 项目任务 MYOS2-DR-003R（依赖关系图与初始化序列重锚（函数级））。仓库 08822407d/MyOS2 是 public 仓库。
先完整读取这四个文件（GitHub 连接器可用则用连接器，否则直接打开 raw 链接）：
1) 写入公约：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/conventions.md
2) 反编造协议 v2（硬性；与任务书冲突时以协议为准）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/00-gpt-task-protocol-v2.md
3) 第一波评审报告：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/WAVE-1-REVIEW.md
4) 你的任务书（只读这一份，不读同名旧版本）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/MYOS2-DR-003R-dependency-graph-reanchor-v2.md
内核源码以 time 分支为准：https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>（浏览目录用 https://github.com/08822407d/MyOS2/tree/time/mykernel/ ）；第一波产出在 https://github.com/08822407d/MyOS2/tree/master/agent-workspace/results/ 下，只按任务书的输入清单读。
严格按任务书的输入清单、强制锚点、交付物、输出上限与验收判据执行；协议 P1~P13 全部硬性：不写 40 位 commit SHA（base_snapshot 只写分支名）；每条 [VERIFIED 路径::函数名] 紧跟 1~5 行逐字引文且引文在该函数/宏定义体内，不用行号；强制锚点不得降级；说"不存在/未调用/可裁剪"前先读 mykernel/scripts/options_flags.cmake 并引用；外部结论带可解析 URL/DOI；第一波产出只取结构、函数名、ID，不转述其断言、不转抄其行号与文件头字段；MANIFEST 必含 startup_selfcheck_quote（逐字复制公约 §1 第 2 条）、branch_canary_quotes（协议 P3 指定的两行；不读源码的任务免）、read_channel、self_check（verified_claims 等于你全部交付文件中 [VERIFIED 标签的个数）、produced_by（原样填写界面显示的模型名）；编造一条即整份作废。
深度研究期间连接器只读。研究完成后回到普通对话回合：能写库则在新分支 agent/MYOS2-DR-003R 上、agent-workspace/results/MYOS2-DR-003R/ 内新增文件并向 master 开 PR；不能写库就逐文件输出——每条回复只含一个文件，独立代码围栏，围栏前一行写目标路径，按任务书给的顺序。
开始前用三句话复述：任务目标、出处与引文纪律、交付物清单。复述无误后直接开工，过程中不要向我提问，拿不准的写进 open_questions。
```

### 7.2 MYOS2-DR-002R · 子系统完成度矩阵重锚与双轴化

```text
你将以深度研究模式执行 MyOS2 项目任务 MYOS2-DR-002R（子系统完成度矩阵重锚与双轴化）。仓库 08822407d/MyOS2 是 public 仓库。
先完整读取这四个文件（GitHub 连接器可用则用连接器，否则直接打开 raw 链接）：
1) 写入公约：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/conventions.md
2) 反编造协议 v2（硬性；与任务书冲突时以协议为准）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/00-gpt-task-protocol-v2.md
3) 第一波评审报告：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/WAVE-1-REVIEW.md
4) 你的任务书（只读这一份，不读同名旧版本）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/MYOS2-DR-002R-subsystem-completeness-reanchor.md
内核源码以 time 分支为准：https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>（浏览目录用 https://github.com/08822407d/MyOS2/tree/time/mykernel/ ）；第一波产出在 https://github.com/08822407d/MyOS2/tree/master/agent-workspace/results/ 下，只按任务书的输入清单读。
严格按任务书的输入清单、强制锚点、交付物、输出上限与验收判据执行；协议 P1~P13 全部硬性：不写 40 位 commit SHA（base_snapshot 只写分支名）；每条 [VERIFIED 路径::函数名] 紧跟 1~5 行逐字引文且引文在该函数/宏定义体内，不用行号；强制锚点不得降级；说"不存在/未调用/可裁剪"前先读 mykernel/scripts/options_flags.cmake 并引用；外部结论带可解析 URL/DOI；第一波产出只取结构、函数名、ID，不转述其断言、不转抄其行号与文件头字段；MANIFEST 必含 startup_selfcheck_quote（逐字复制公约 §1 第 2 条）、branch_canary_quotes（协议 P3 指定的两行；不读源码的任务免）、read_channel、self_check（verified_claims 等于你全部交付文件中 [VERIFIED 标签的个数）、produced_by（原样填写界面显示的模型名）；编造一条即整份作废。
深度研究期间连接器只读。研究完成后回到普通对话回合：能写库则在新分支 agent/MYOS2-DR-002R 上、agent-workspace/results/MYOS2-DR-002R/ 内新增文件并向 master 开 PR；不能写库就逐文件输出——每条回复只含一个文件，独立代码围栏，围栏前一行写目标路径，按任务书给的顺序。
开始前用三句话复述：任务目标、出处与引文纪律、交付物清单。复述无误后直接开工，过程中不要向我提问，拿不准的写进 open_questions。
```

### 7.3 MYOS2-DR-004R · 调试桩与可观测性方案（复跑）

```text
你将以深度研究模式执行 MyOS2 项目任务 MYOS2-DR-004R（调试桩与可观测性方案（复跑））。仓库 08822407d/MyOS2 是 public 仓库。
先完整读取这四个文件（GitHub 连接器可用则用连接器，否则直接打开 raw 链接）：
1) 写入公约：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/conventions.md
2) 反编造协议 v2（硬性；与任务书冲突时以协议为准）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/00-gpt-task-protocol-v2.md
3) 第一波评审报告：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/WAVE-1-REVIEW.md
4) 你的任务书（只读这一份，不读同名旧版本）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/MYOS2-DR-004R-debug-instrumentation-rerun-v2.md
内核源码以 time 分支为准：https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>（浏览目录用 https://github.com/08822407d/MyOS2/tree/time/mykernel/ ）；第一波产出在 https://github.com/08822407d/MyOS2/tree/master/agent-workspace/results/ 下，只按任务书的输入清单读。
严格按任务书的输入清单、强制锚点、交付物、输出上限与验收判据执行；协议 P1~P13 全部硬性：不写 40 位 commit SHA（base_snapshot 只写分支名）；每条 [VERIFIED 路径::函数名] 紧跟 1~5 行逐字引文且引文在该函数/宏定义体内，不用行号；强制锚点不得降级；说"不存在/未调用/可裁剪"前先读 mykernel/scripts/options_flags.cmake 并引用；外部结论带可解析 URL/DOI；第一波产出只取结构、函数名、ID，不转述其断言、不转抄其行号与文件头字段；MANIFEST 必含 startup_selfcheck_quote（逐字复制公约 §1 第 2 条）、branch_canary_quotes（协议 P3 指定的两行；不读源码的任务免）、read_channel、self_check（verified_claims 等于你全部交付文件中 [VERIFIED 标签的个数）、produced_by（原样填写界面显示的模型名）；编造一条即整份作废。
深度研究期间连接器只读。研究完成后回到普通对话回合：能写库则在新分支 agent/MYOS2-DR-004R 上、agent-workspace/results/MYOS2-DR-004R/ 内新增文件并向 master 开 PR；不能写库就逐文件输出——每条回复只含一个文件，独立代码围栏，围栏前一行写目标路径，按任务书给的顺序。
开始前用三句话复述：任务目标、出处与引文纪律、交付物清单。复述无误后直接开工，过程中不要向我提问，拿不准的写进 open_questions。
```

### 7.4 MYOS2-DR-007R · 重要度分级证据补全与 tier 重算

```text
你将以深度研究模式执行 MyOS2 项目任务 MYOS2-DR-007R（重要度分级证据补全与 tier 重算）。仓库 08822407d/MyOS2 是 public 仓库。
先完整读取这四个文件（GitHub 连接器可用则用连接器，否则直接打开 raw 链接）：
1) 写入公约：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/conventions.md
2) 反编造协议 v2（硬性；与任务书冲突时以协议为准）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/00-gpt-task-protocol-v2.md
3) 第一波评审报告：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/WAVE-1-REVIEW.md
4) 你的任务书（只读这一份，不读同名旧版本）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/MYOS2-DR-007R-importance-evidence-and-tiers.md
内核源码以 time 分支为准：https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>（浏览目录用 https://github.com/08822407d/MyOS2/tree/time/mykernel/ ）；第一波产出在 https://github.com/08822407d/MyOS2/tree/master/agent-workspace/results/ 下，只按任务书的输入清单读。
严格按任务书的输入清单、强制锚点、交付物、输出上限与验收判据执行；协议 P1~P13 全部硬性：不写 40 位 commit SHA（base_snapshot 只写分支名）；每条 [VERIFIED 路径::函数名] 紧跟 1~5 行逐字引文且引文在该函数/宏定义体内，不用行号；强制锚点不得降级；说"不存在/未调用/可裁剪"前先读 mykernel/scripts/options_flags.cmake 并引用；外部结论带可解析 URL/DOI；第一波产出只取结构、函数名、ID，不转述其断言、不转抄其行号与文件头字段；MANIFEST 必含 startup_selfcheck_quote（逐字复制公约 §1 第 2 条）、branch_canary_quotes（协议 P3 指定的两行；不读源码的任务免）、read_channel、self_check（verified_claims 等于你全部交付文件中 [VERIFIED 标签的个数）、produced_by（原样填写界面显示的模型名）；编造一条即整份作废。
深度研究期间连接器只读。研究完成后回到普通对话回合：能写库则在新分支 agent/MYOS2-DR-007R 上、agent-workspace/results/MYOS2-DR-007R/ 内新增文件并向 master 开 PR；不能写库就逐文件输出——每条回复只含一个文件，独立代码围栏，围栏前一行写目标路径，按任务书给的顺序。
开始前用三句话复述：任务目标、出处与引文纪律、交付物清单。复述无误后直接开工，过程中不要向我提问，拿不准的写进 open_questions。
```

### 7.5 MYOS2-DR-010 · 补做外部测试实践综述

```text
你将以深度研究模式执行 MyOS2 项目任务 MYOS2-DR-010（补做外部测试实践综述）。仓库 08822407d/MyOS2 是 public 仓库。
先完整读取这四个文件（GitHub 连接器可用则用连接器，否则直接打开 raw 链接）：
1) 写入公约：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/conventions.md
2) 反编造协议 v2（硬性；与任务书冲突时以协议为准）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/00-gpt-task-protocol-v2.md
3) 第一波评审报告：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/WAVE-1-REVIEW.md
4) 你的任务书（只读这一份，不读同名旧版本）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/MYOS2-DR-010-testing-practice-survey-redo.md
内核源码以 time 分支为准：https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>（浏览目录用 https://github.com/08822407d/MyOS2/tree/time/mykernel/ ）；第一波产出在 https://github.com/08822407d/MyOS2/tree/master/agent-workspace/results/ 下，只按任务书的输入清单读。
严格按任务书的输入清单、强制锚点、交付物、输出上限与验收判据执行；协议 P1~P13 全部硬性：不写 40 位 commit SHA（base_snapshot 只写分支名）；每条 [VERIFIED 路径::函数名] 紧跟 1~5 行逐字引文且引文在该函数/宏定义体内，不用行号；强制锚点不得降级；说"不存在/未调用/可裁剪"前先读 mykernel/scripts/options_flags.cmake 并引用；外部结论带可解析 URL/DOI；第一波产出只取结构、函数名、ID，不转述其断言、不转抄其行号与文件头字段；MANIFEST 必含 startup_selfcheck_quote（逐字复制公约 §1 第 2 条）、branch_canary_quotes（协议 P3 指定的两行；不读源码的任务免）、read_channel、self_check（verified_claims 等于你全部交付文件中 [VERIFIED 标签的个数）、produced_by（原样填写界面显示的模型名）；编造一条即整份作废。
深度研究期间连接器只读。研究完成后回到普通对话回合：能写库则在新分支 agent/MYOS2-DR-010 上、agent-workspace/results/MYOS2-DR-010/ 内新增文件并向 master 开 PR；不能写库就逐文件输出——每条回复只含一个文件，独立代码围栏，围栏前一行写目标路径，按任务书给的顺序。
开始前用三句话复述：任务目标、出处与引文纪律、交付物清单。复述无误后直接开工，过程中不要向我提问，拿不准的写进 open_questions。
```

### 7.6 MYOS2-DR-001R · 外围工程审计复跑（勘误优先）

```text
你将以深度研究模式执行 MyOS2 项目任务 MYOS2-DR-001R（外围工程审计复跑（勘误优先））。仓库 08822407d/MyOS2 是 public 仓库。
先完整读取这四个文件（GitHub 连接器可用则用连接器，否则直接打开 raw 链接）：
1) 写入公约：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/conventions.md
2) 反编造协议 v2（硬性；与任务书冲突时以协议为准）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/00-gpt-task-protocol-v2.md
3) 第一波评审报告：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/WAVE-1-REVIEW.md
4) 你的任务书（只读这一份，不读同名旧版本）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/MYOS2-DR-001R-build-and-scripts-audit-rerun-v2.md
内核源码以 time 分支为准：https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>（浏览目录用 https://github.com/08822407d/MyOS2/tree/time/mykernel/ ）；第一波产出在 https://github.com/08822407d/MyOS2/tree/master/agent-workspace/results/ 下，只按任务书的输入清单读。
严格按任务书的输入清单、强制锚点、交付物、输出上限与验收判据执行；协议 P1~P13 全部硬性：不写 40 位 commit SHA（base_snapshot 只写分支名）；每条 [VERIFIED 路径::函数名] 紧跟 1~5 行逐字引文且引文在该函数/宏定义体内，不用行号；强制锚点不得降级；说"不存在/未调用/可裁剪"前先读 mykernel/scripts/options_flags.cmake 并引用；外部结论带可解析 URL/DOI；第一波产出只取结构、函数名、ID，不转述其断言、不转抄其行号与文件头字段；MANIFEST 必含 startup_selfcheck_quote（逐字复制公约 §1 第 2 条）、branch_canary_quotes（协议 P3 指定的两行；不读源码的任务免）、read_channel、self_check（verified_claims 等于你全部交付文件中 [VERIFIED 标签的个数）、produced_by（原样填写界面显示的模型名）；编造一条即整份作废。
深度研究期间连接器只读。研究完成后回到普通对话回合：能写库则在新分支 agent/MYOS2-DR-001R 上、agent-workspace/results/MYOS2-DR-001R/ 内新增文件并向 master 开 PR；不能写库就逐文件输出——每条回复只含一个文件，独立代码围栏，围栏前一行写目标路径，按任务书给的顺序。
开始前用三句话复述：任务目标、出处与引文纪律、交付物清单。复述无误后直接开工，过程中不要向我提问，拿不准的写进 open_questions。
```

### 7.7 MYOS2-DR-011 · lockdep-lite 设计调研

```text
你将以深度研究模式执行 MyOS2 项目任务 MYOS2-DR-011（lockdep-lite 设计调研）。仓库 08822407d/MyOS2 是 public 仓库。
先完整读取这四个文件（GitHub 连接器可用则用连接器，否则直接打开 raw 链接）：
1) 写入公约：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/conventions.md
2) 反编造协议 v2（硬性；与任务书冲突时以协议为准）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/00-gpt-task-protocol-v2.md
3) 第一波评审报告：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/WAVE-1-REVIEW.md
4) 你的任务书（只读这一份，不读同名旧版本）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/MYOS2-DR-011-lockdep-lite-research.md
内核源码以 time 分支为准：https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>（浏览目录用 https://github.com/08822407d/MyOS2/tree/time/mykernel/ ）；第一波产出在 https://github.com/08822407d/MyOS2/tree/master/agent-workspace/results/ 下，只按任务书的输入清单读。
严格按任务书的输入清单、强制锚点、交付物、输出上限与验收判据执行；协议 P1~P13 全部硬性：不写 40 位 commit SHA（base_snapshot 只写分支名）；每条 [VERIFIED 路径::函数名] 紧跟 1~5 行逐字引文且引文在该函数/宏定义体内，不用行号；强制锚点不得降级；说"不存在/未调用/可裁剪"前先读 mykernel/scripts/options_flags.cmake 并引用；外部结论带可解析 URL/DOI；第一波产出只取结构、函数名、ID，不转述其断言、不转抄其行号与文件头字段；MANIFEST 必含 startup_selfcheck_quote（逐字复制公约 §1 第 2 条）、branch_canary_quotes（协议 P3 指定的两行；不读源码的任务免）、read_channel、self_check（verified_claims 等于你全部交付文件中 [VERIFIED 标签的个数）、produced_by（原样填写界面显示的模型名）；编造一条即整份作废。
深度研究期间连接器只读。研究完成后回到普通对话回合：能写库则在新分支 agent/MYOS2-DR-011 上、agent-workspace/results/MYOS2-DR-011/ 内新增文件并向 master 开 PR；不能写库就逐文件输出——每条回复只含一个文件，独立代码围栏，围栏前一行写目标路径，按任务书给的顺序。
开始前用三句话复述：任务目标、出处与引文纪律、交付物清单。复述无误后直接开工，过程中不要向我提问，拿不准的写进 open_questions。
```

### 7.8 MYOS2-DR-009R · 教学蓝本预研出处补全与教学单元过滤

```text
你将以深度研究模式执行 MyOS2 项目任务 MYOS2-DR-009R（教学蓝本预研出处补全与教学单元过滤）。仓库 08822407d/MyOS2 是 public 仓库。
先完整读取这四个文件（GitHub 连接器可用则用连接器，否则直接打开 raw 链接）：
1) 写入公约：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/conventions.md
2) 反编造协议 v2（硬性；与任务书冲突时以协议为准）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/00-gpt-task-protocol-v2.md
3) 第一波评审报告：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/WAVE-1-REVIEW.md
4) 你的任务书（只读这一份，不读同名旧版本）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/MYOS2-DR-009R-teaching-blueprint-sources.md
内核源码以 time 分支为准：https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>（浏览目录用 https://github.com/08822407d/MyOS2/tree/time/mykernel/ ）；第一波产出在 https://github.com/08822407d/MyOS2/tree/master/agent-workspace/results/ 下，只按任务书的输入清单读。
严格按任务书的输入清单、强制锚点、交付物、输出上限与验收判据执行；协议 P1~P13 全部硬性：不写 40 位 commit SHA（base_snapshot 只写分支名）；每条 [VERIFIED 路径::函数名] 紧跟 1~5 行逐字引文且引文在该函数/宏定义体内，不用行号；强制锚点不得降级；说"不存在/未调用/可裁剪"前先读 mykernel/scripts/options_flags.cmake 并引用；外部结论带可解析 URL/DOI；第一波产出只取结构、函数名、ID，不转述其断言、不转抄其行号与文件头字段；MANIFEST 必含 startup_selfcheck_quote（逐字复制公约 §1 第 2 条）、branch_canary_quotes（协议 P3 指定的两行；不读源码的任务免）、read_channel、self_check（verified_claims 等于你全部交付文件中 [VERIFIED 标签的个数）、produced_by（原样填写界面显示的模型名）；编造一条即整份作废。
深度研究期间连接器只读。研究完成后回到普通对话回合：能写库则在新分支 agent/MYOS2-DR-009R 上、agent-workspace/results/MYOS2-DR-009R/ 内新增文件并向 master 开 PR；不能写库就逐文件输出——每条回复只含一个文件，独立代码围栏，围栏前一行写目标路径，按任务书给的顺序。
开始前用三句话复述：任务目标、出处与引文纪律、交付物清单。复述无误后直接开工，过程中不要向我提问，拿不准的写进 open_questions。
```

### 7.9 MYOS2-DR-005R · x86 查表资料包勘误与出处补全

```text
你将以深度研究模式执行 MyOS2 项目任务 MYOS2-DR-005R（x86 查表资料包勘误与出处补全）。仓库 08822407d/MyOS2 是 public 仓库。
先完整读取这四个文件（GitHub 连接器可用则用连接器，否则直接打开 raw 链接）：
1) 写入公约：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/conventions.md
2) 反编造协议 v2（硬性；与任务书冲突时以协议为准）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/00-gpt-task-protocol-v2.md
3) 第一波评审报告：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/WAVE-1-REVIEW.md
4) 你的任务书（只读这一份，不读同名旧版本）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/MYOS2-DR-005R-x86-reference-pack-errata-v2.md
内核源码以 time 分支为准：https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>（浏览目录用 https://github.com/08822407d/MyOS2/tree/time/mykernel/ ）；第一波产出在 https://github.com/08822407d/MyOS2/tree/master/agent-workspace/results/ 下，只按任务书的输入清单读。
严格按任务书的输入清单、强制锚点、交付物、输出上限与验收判据执行；协议 P1~P13 全部硬性：不写 40 位 commit SHA（base_snapshot 只写分支名）；每条 [VERIFIED 路径::函数名] 紧跟 1~5 行逐字引文且引文在该函数/宏定义体内，不用行号；强制锚点不得降级；说"不存在/未调用/可裁剪"前先读 mykernel/scripts/options_flags.cmake 并引用；外部结论带可解析 URL/DOI；第一波产出只取结构、函数名、ID，不转述其断言、不转抄其行号与文件头字段；MANIFEST 必含 startup_selfcheck_quote（逐字复制公约 §1 第 2 条）、branch_canary_quotes（协议 P3 指定的两行；不读源码的任务免）、read_channel、self_check（verified_claims 等于你全部交付文件中 [VERIFIED 标签的个数）、produced_by（原样填写界面显示的模型名）；编造一条即整份作废。
深度研究期间连接器只读。研究完成后回到普通对话回合：能写库则在新分支 agent/MYOS2-DR-005R 上、agent-workspace/results/MYOS2-DR-005R/ 内新增文件并向 master 开 PR；不能写库就逐文件输出——每条回复只含一个文件，独立代码围栏，围栏前一行写目标路径，按任务书给的顺序。
开始前用三句话复述：任务目标、出处与引文纪律、交付物清单。复述无误后直接开工，过程中不要向我提问，拿不准的写进 open_questions。
```

### 7.10 MYOS2-DR-008R · 技术债台账重锚与覆盖扩展（可选）

```text
你将以深度研究模式执行 MyOS2 项目任务 MYOS2-DR-008R（技术债台账重锚与覆盖扩展（可选））。仓库 08822407d/MyOS2 是 public 仓库。
先完整读取这四个文件（GitHub 连接器可用则用连接器，否则直接打开 raw 链接）：
1) 写入公约：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/conventions.md
2) 反编造协议 v2（硬性；与任务书冲突时以协议为准）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/00-gpt-task-protocol-v2.md
3) 第一波评审报告：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/WAVE-1-REVIEW.md
4) 你的任务书（只读这一份，不读同名旧版本）：https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/tasks/MYOS2-DR-008R-tech-debt-register-reanchor.md
内核源码以 time 分支为准：https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>（浏览目录用 https://github.com/08822407d/MyOS2/tree/time/mykernel/ ）；第一波产出在 https://github.com/08822407d/MyOS2/tree/master/agent-workspace/results/ 下，只按任务书的输入清单读。
严格按任务书的输入清单、强制锚点、交付物、输出上限与验收判据执行；协议 P1~P13 全部硬性：不写 40 位 commit SHA（base_snapshot 只写分支名）；每条 [VERIFIED 路径::函数名] 紧跟 1~5 行逐字引文且引文在该函数/宏定义体内，不用行号；强制锚点不得降级；说"不存在/未调用/可裁剪"前先读 mykernel/scripts/options_flags.cmake 并引用；外部结论带可解析 URL/DOI；第一波产出只取结构、函数名、ID，不转述其断言、不转抄其行号与文件头字段；MANIFEST 必含 startup_selfcheck_quote（逐字复制公约 §1 第 2 条）、branch_canary_quotes（协议 P3 指定的两行；不读源码的任务免）、read_channel、self_check（verified_claims 等于你全部交付文件中 [VERIFIED 标签的个数）、produced_by（原样填写界面显示的模型名）；编造一条即整份作废。
深度研究期间连接器只读。研究完成后回到普通对话回合：能写库则在新分支 agent/MYOS2-DR-008R 上、agent-workspace/results/MYOS2-DR-008R/ 内新增文件并向 master 开 PR；不能写库就逐文件输出——每条回复只含一个文件，独立代码围栏，围栏前一行写目标路径，按任务书给的顺序。
开始前用三句话复述：任务目标、出处与引文纪律、交付物清单。复述无误后直接开工，过程中不要向我提问，拿不准的写进 open_questions。
```
