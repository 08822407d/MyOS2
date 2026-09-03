# 工作令 · MYOS2-LEAD-001（MyOS2 内核分析计划 · 本地主导会话；自 Mnemosyne 会话迁出）

> 本文件是一份完整工作令。请**先完整读完全文**（能看到最后一节"§13 Owner 补充说明登记处"才算完整）。若内容不完整或哈希与启动提示词所载不符，回复"工作令不可用"并停止，不要开始任何工作。
> 本 Downloads 原件自哈希登记起**冻结**（含 Owner 在内任何人不改）；Owner 的补充说明按 §13 的规则另行登记。
> 本工作令的骨架借自同日起草的 META-AGENT-FABLE5-REVIEW-AND-DESIGN-001 工作令（同目录，同一起草方；C 类参考件），并按 Owner 2026-09-02 指示增补了 §1.3 存储底线、§5 暂定执行源、§6 分析目的与维度。本文件自足，不依赖读那一份。

```yaml
track_id: MYOS2-LEAD-001
record_type: work_order_for_new_claude_code_session
predecessor_record: Mnemosyne origin/master target-projects/myos2/00-intake-and-wave-001-launch.md（MYOS2-ANALYSIS-001 立项）与 01-wave-1-intake-and-review.md（第一波回收评审）——本轨道接续其工作，不复用其任务号
drafted_by: Mnemosyne 主导会话（session f0e60aba-25ea-419b-b350-71be428ec822，claude-fable-5.1@claude-code-vscode；起草时机器名 mnemosyne-e8）——即 MyOS2 分析计划迄今的主导会话
drafted_on: 2026-09-02
issued_by: Owner（Owner 将本工作令发给新会话即构成任务授权；Owner 可在补充说明中追加、修改或删除任何条款，每次补充逐字登记）
executor: 新开 Claude Code 本地会话（Fable 5.1），工作目录 /home/cheyh/projs/MyOS2（GitHub 08822407d/MyOS2，public）
execute_in: 该新会话本身；本工作令不由 Mnemosyne 会话执行
role_after_launch: MyOS2 分析计划的**本地主导会话**——接替 Mnemosyne 会话的全部 MyOS2 职责（§2.2）；Mnemosyne 会话此后对 MyOS2 只读、不再写入
pinned_base_at_drafting:
  origin/master: d231708c（agent-workspace/ 在 master 及 agent/ PR 分支上，不在 time 上；最后五个提交全部只动 agent-workspace/）
  kernel_analysis_baseline: 分支 time @ a039d980（完整 a039d9803ade2a1613d620bda375e028530d5242；= origin/time；Owner 的私人开发分支，也是本机主检出所在分支）
  local_master_ref: 63f0785c（**过期**，落后 origin/master 5 提交；勿以本地 master 为基准）
target_execution_source: MyOS2 没有正式执行源；暂定执行源见 §5（Owner 已明示"之前补充的其他一些临时规范也包含进去作为暂定的执行源"）；内核本体（agent-workspace/ 之外）的规则只有 Owner 本人
receiver_guidance_load:
  project_guidance: required（§5 暂定执行源）
  mnemosyne_guidance: no（本次交接不加载 Mnemosyne 的执行源/guard；§2.4-B 的 Mnemosyne 文件只作历史记录与方法参考。Owner 可改为 yes）
execution_intent:
  response_role: ANALYSIS_AND_PREPARATION
  execution_disposition: RUN_AFTER_GATE_REQUIRED（门 = Owner 的补充说明与一句"开工"；门前只做 §2 启动仪式）
  external_execution_or_quota_authorized: false（本会话只起草 GPT 任务书，发射与额度由 Owner 亲手）
evidence_class_of_this_file: DIRECT_OWNER_INSTRUCTION_after_Owner_sends_it（§1.1 为 Owner 原话逐字节选；其余为起草方的操作化解释，标 [MI]）
```

---

## 0. 一句话

你接手 MyOS2（Owner 自制的学习/实验用 x86-64 内核）的**分析计划**，成为它的本地主导会话：把 Mnemosyne 会话临时插入的这项工作整体迁到你这里，按 Owner 的五点需求（不增不减）和"不限于五点、要看大型代码项目普遍关心的问题"的分析口径继续推进；存储方案的底线是**让后续更完善版本的 agent（未来的 Meta-Agent）能无损接手**。但**现在先不要开工**：做完 §2 的启动仪式就停下，等 Owner 的补充说明和一句"开工"。

## 1. 任务

### 1.1 Owner 原话（逐字节选；"……"为略去的、与本轨道无关的段落）

**2026-09-02（本次迁移指示）**
> ……第二是把本任务中之前临时插入的MyOS2项目结构和源码分析的工作迁移到新开的claude code本地任务中,以免它干扰mnemosyne记录的属于自己的工作内容.要求要借用前一个关于MA的,但还需要补充更多,主要是之前开始时说过的由于Meta-Agent还没建设到一个可用的首版,因此许多行为规范还没有建立起来,因此建立的存储方案以"有利于后续完善版本agent接手"为底线,之前补充的其他一些临时规范也包含进去作为暂定的执行源,进行项目分析的目的暂时没有要补充的还是那5点,分析细节还是之前说的基于那五方面但不能仅限于它们,而是要考虑其他大型代码项目普遍关心的问题.好了,你现在给出这两个任务的启动提示词吧,注意让它们不要直接开始工作,而是要在确认启动正确后等待我进一步补充一些说明和要求.

**2026-08-31（立项原话：背景、五点需求、四条原则）**
> 这个需求就是:我之前数年自己模仿linux内核开发了一个学习和实验用的操作系统内核,但由于没有足够的环境让我进行更深入全面的学习和实践,我遇到了瓶颈.而幸运地是现在的顶尖LLM已经非常强大了,我要借助它们来帮助我深度学习os内核设计.那么第一步就是要分析我目前的实现,具体要分析哪些,怎么分析我并不是很清楚,我只知道目前我希望对我内核实现做的改善有:1,整理和优化外围脚本比如我主要使用的cmake,以及相关的各种自动化调试脚本,打包安装脚本等;2,搞清楚我的内核中各个子系统/模块/机制目前的完成度,能够支持哪些能力,还欠缺哪些能力,以及按照相关部分中各种能力的重要程度建立一个清晰的"重要程度"和"依赖关系"图,重要程度图表的作用是为我下一步学习和实践确定方向,而依赖关系图的作用则是快速搞清楚要实现一个功能/子功能还必须实现哪些更基础的部分,此外如果要模仿linux用menuconfig或其他方式实现内核裁剪,那么这种依赖关系也是必须理清楚的;3,为我建议一些基本的调试桩可以快速定位bug,由于我本身是野路子做内核,现在积累了许多问题,运行起来经常触发bug但找不到问题所在,所以我已经实现的内容有必要加入一些基本的确保这部分实现没有问题的调试代码;4,有一些属于纯查表查手册的工作很耗时耗费精力且没多少价值(主要指的是对我自身的技术和理论水平的价值低)的工作可以让agent轻松就替我完成,比如x86的cpuid指令能获取到的各种信息,显然agent要轻松完成这部分工作就得对我当前的实现比较了解才行,不然得读大量源码进行分析;5,我还会建立一些专门用于帮我学习现代操作系统内核理论和实现的agent组,我认为凭linux内核源码讲解不是很合适(因为linux源码为了各种兼容性和性能目的现在已经非常复杂了),那么基于我自己的内核来讲解就是个有助于我学习的蓝本.目前我能想到这些,但应该不止这些,不过现在无所谓了.……有几个原则给你参考,第一是考虑到这个需求属于急迫性质所需的一些基础设施还未完善,因此注意后续基础设施完善后要能完美接手先前的工作和吸收工作成果(也就时说允许当前工作有不完美,但不能让以后的agent无法接手只能从头来);二是内容保存到我的内核的仓库里但要注意与现有内容完全区分清楚,不要混在现有内容里,等以后完善了再考虑重新设计文件组织结构,仓库在"git@github.com:08822407d/MyOS2.git",本地位置是"/home/cheyh/projs/MyOS2",所以这部分就得利用mnemosyne的建设成果帮你做设计,做完之后需要考虑我开启多个chatgpt pro对话时它们能轻松地启动;三是你得预估一下其他可能的需求,也就是说设计分析方法和目标时不能仅限于我提出的这几点,而是借鉴大型系统开发中普遍关注的问题;四是你给出chatgpt pro的启动提示词/任务书时要考虑它们并行工作写入仓库的情况(这是一定会发生的,并且chatgpt pro只能读写github仓库无法访问本地的源码),因此要保证它们的写入不会互相干扰,此外你可以先粗略阅读一下我这个内核项目的文件夹组织,因为我已经把部分子系统/模块/机制分类放置在不同的自文件夹中,不像linux那样全平铺到一个文件夹中,因此你大概率可以仅通过文件夹名称就判断出其中的源码的内容.……

**2026-08-31（开发模式纠正）**
> 我补充一点说明,从两年前开始MyOS2的所有开发工作都在本机上进行,我的开发习惯是新建一个分支来专门实现某个机制/子系统/模块(但显然增加和修改的代码并不都是仅属于这个子系统/模块/机制的源码),基本达到我的要求后才会合并到master,所以本机项目仓库当前所在分支才是最新版本,你自己思考和设计,以及在给gpt pro任务书的时候需要说清楚这点.

**2026-08-31（研究结论双仓归档制度；原文存 Mnemosyne 09-continuation/06-archiving-institution-record.md）**
> 同时注意把它们在Alaya中归档，并且这个归档动作应该形成制度，也就是每个确认无误的研究结论文件都应该及时地随研究课题文件归档到Alaya中正确的位置同时要给它一个符合Alaya规则的文件名。当然如果要在所属agent仓库里归档的话也要在所属仓库中改成符合自己仓库规则的文件名。

**2026-09-02（会话转录归档裁定；起草方判断可类推到本轨道的会话，待 Owner 追认 [MI]）**
> 你提到的存档规则我确实没考虑到,不过Alaya那边是可以完整存档的,而在mnemosyne中(确切的是是对话/任务所属的agent的仓库)也应该保存有效内容,如何脱敏由你来决定.而Alaya仓库拉取到本地的位置是"/home/cheyh/projs/Alaya".

### 1.2 为什么迁到你这里 [MI]
MyOS2 分析是 Owner 在 Mnemosyne 主线上"插入"的任务；继续放在 Mnemosyne 会话里会把 MyOS2 的工作记录混进 Mnemosyne 自己的记录。此项工作原本要等 Meta-Agent 建成后由它规划；Meta-Agent 尚无可用首版、行为规范未立，所以由你以"允许不完美、但必须可接管"的方式先做，并把一切按未来 Meta-Agent 能接手的形态存放。

### 1.3 存储底线（Owner 明令："以'有利于后续完善版本agent接手'为底线"）
操作化为六条 [MI]，全部沿用并强化 MyOS2 `agent-workspace/README.md` 已写的"未来接管协议"：
1. **产出皆证据、非规范**：results/ 与你的记录都是带出处的证据（S1 级），不是裁决过的权威；接管方可重评、替代、重组，但不得改写原件——修订只增不改（新文件 + `supersedes:`）。
2. **来源可追**：每份产出带 YAML 头（任务号、模型、日期、实际所读快照、读过的输入、状态、open_questions）；事实断言标 `[VERIFIED 路径]` / `[INFERRED]` / `[EXTERNAL 出处]`，三者不混写。
3. **快照锚定**：每份结论写明所读分支＋commit（SHA 只从工具输出复制，入库前 `git cat-file -e` 校验）。
4. **词汇表共享**：子系统/能力节点用 conventions.md §3 词汇表命名，机器可读产出（YAML/DOT）可直接合并；词汇表缺口只增不改。
5. **重组自由、链条不断**：agent-workspace/ 可被未来 agent 整体迁移或重构，唯一要求是 results/ 原件、MANIFEST 链条与归档收据（bytes/sha256 双侧互引）不丢。
6. **交接自足**：你的记录目录（§7.2）随时能让一个全新会话仅凭 MyOS2 仓库内容恢复五件事——现行规则在哪、做到哪一步、哪些门待 Owner、哪些是暂定/待追认、下一步安全动作是什么。**已知缺口**：立项决策 D1~D6、接管清单、第一波评审记录目前只在 Mnemosyne 仓（§2.4-B）；阶段 0 默认把它们逐字镜像进 §7.2 目录（见 §12 第 2 条），镜像前本条不成立。每个门结束写续接检查点。

## 2. 启动仪式（第一条回复必须完成；完成后停止等待 Owner）

**只读**＝不改工作树、不改本地分支/HEAD、不建/删任何引用、不建 worktree；唯一允许的仓库状态变化是 `git fetch origin master time`（只更新 origin/* 远端跟踪引用；不带冒号，禁止 `master:master` 之类更新本地分支的写法；不 prune）。不 pull、不 merge、不 reset、不 rebase、不 stash、不 checkout、不建分支、不建 PR。**尤其：主检出停在 Owner 私人开发分支 time，绝不切换、绝不弄脏。** 不创建 CLAUDE.md / AGENTS.md / .claude/（若 Owner 预置了 .claude/settings.local.json，不改不删）。不联网研究。不运行仓库内任何脚本、不构建。**不向任何会话发送消息**；启动仪式期间若收到跨会话消息：不回复、不执行，只把发送方名与摘要登记进回执 notes，补充说明之后再按 §10 处理。

### 2.1 环境核验（expected / observed 逐项，写进回执）

| # | 项 | expected | 不符时 |
|---|---|---|---|
| 1 | `pwd` | /home/cheyh/projs/MyOS2 | FATAL：停止并报 Owner |
| 2 | `git remote -v` | origin = https://github.com/08822407d/MyOS2.git（fetch/push）；upstream = git@github.com:08822407d/MyOS2.git（fetch），其 push 有两条 URL：github 同址 ＋ git@gitee.com:<REDACTED-owner-backup-mirror>（Owner 的备份镜像） | WARNING：如实记录（推送规则见 §4.4） |
| 3 | `git rev-parse --abbrev-ref HEAD` | time | FATAL（绝不自行切换） |
| 4 | `git status --porcelain` | 空（若 Owner 预置了 .claude/，允许仅出现 `?? .claude/`） | WARNING：如实记录，不清理 |
| 5 | `git fetch origin master time && git rev-parse --short origin/master && git rev-parse --short origin/time` | d231708c / a039d980 | WARNING：记录新 SHA，不据此推断 |
| 6 | `git rev-parse --short master`（本地 ref） | 63f0785c（过期） | 只记录；不更新（需 Owner 授权） |
| 7 | `git ls-tree --name-only origin/master agent-workspace/` | LAUNCH.md README.md WAVE-1-REVIEW.md WAVE-2-PLAN.md conventions.md repo-map.md results tasks | FATAL |
| 8 | `git ls-tree --name-only time agent-workspace/` | 空（工作区不在 time 上） | WARNING：记录 |
| 9 | `git worktree list` 与 `git branch --list 'agent-*' 'archive-*'` | 主检出 ＋ 可能残留一条 Mnemosyne 会话的 scratchpad worktree（分支 archive-receipts-wave1 @ d231708c，路径在 /tmp 下，会话结束后成悬空条目）；本地还有 Mnemosyne 会话建的三条已合并分支 agent-workspace-bootstrap、agent-wave-001-integration、archive-receipts-wave1 | 只记录；不 prune、不删分支（由 Mnemosyne 会话经 Owner 同意后自清） |
| 10 | `gh auth status`；`gh pr list --state open` | 已登录（回执只写"已登录 08822407d"，不粘贴 token 行）；5 个 OPEN（#1~#5，均为第一波 GPT 会话所开，内容已整合进 master） | 只记录；不关不合 |
| 11 | `sha256sum /home/cheyh/Downloads/startup-packages-2026-09-02/MYOS2-LEAD-001-work-order.md`（对文件本身计算，不要对 Read 工具的显示内容计算） | 与启动提示词所载一致 | FATAL |
| 12 | 模型自述 | Fable 5.1 | 如实记录，继续 |

### 2.2 你接手的职责清单（只读确认，不执行）
1. agent-workspace/ 的总纲、公约、任务书、波次计划的维护（"主导会话"角色）；
2. 第二波及以后：起草 GPT 任务书（发射与额度由 Owner 亲手）、本地任务 L1~L11 的执行或委派、回收与对抗核查、可靠性裁定；
3. 归档：回收件按双仓制度归 Alaya `research/MYOS2/` 并维护 `results/ARCHIVE-RECEIPTS.md`（`maintained_by` 改为你）；
4. 给未来 Meta-Agent 的接管清单与交接包的持续维护；
5. 与 Mnemosyne 会话的职责迁移确认（§10）。

### 2.3 记录自身会话标识（只调用 ListAgents 读取，不发送任何消息）
调用 ListAgents，记下自己的机器名（形如 `myos2-xx [......]`；**每次进程重启会变**）与本会话 UUID（`~/.claude/projects/-home-cheyh-projs-MyOS2/` 下的 jsonl 文件名；该目录/文件在收到第一条消息后才出现，若有多个取 mtime 最新且仍在增长者；无法确定则写 `session_uuid: unknown` 并在 notes 说明，不算 FATAL）。两者写进回执。

### 2.4 读序（分别读、各守其位；工作区文件一律用 `git show origin/master:<path>` 读——它们不在 time 分支上）

**A. MyOS2 工作区（暂定执行源与现状）**

启动必读（本仪式只读这四件）：
1. agent-workspace/README.md（总纲、隔离原则、编号、未来接管协议）
2. agent-workspace/conventions.md（§1 写入规则、§2 产出格式、§3 词汇表、§3b 溯源纪律、§4 事实纪律）
3. agent-workspace/WAVE-2-PLAN.md（分流原则、GPT 010/011 提示词、本地任务 L1~L11、Owner 12 条待答决策、§4 硬纪律）
4. agent-workspace/results/ARCHIVE-RECEIPTS.md

阶段前必读（开工后阶段 0 再读，启动仪式不读）：
5. agent-workspace/WAVE-1-REVIEW.md（第一波可信度裁定、降权清单、独立发现、主攻清单、8 项风险）
6. agent-workspace/results/EXTRACTION-NOTE.md、results/README.md、LAUNCH.md、repo-map.md
7. agent-workspace/tasks/ 全部 12 份任务书的 YAML 头；重点 MYOS2-DR-004R、010、011（就绪未发射）
8. agent-workspace/results/*/MANIFEST*.md（9 份）

**B. Mnemosyne 侧记录（C 类；只读；启动仪式只读前两件；`git -C /home/cheyh/projs/Mnemosyne show origin/master:<path>`——Mnemosyne 主检出停在工作分支，不得切换、不得写）**
- target-projects/myos2/00-intake-and-wave-001-launch.md（立项、D1~D6 决策、§4 留给 meta-agent 的接管清单、§6 基线修正）
- target-projects/myos2/01-wave-1-intake-and-review.md（回收方式、004 诊断、方法论回授、§7 未做清单）
- （阶段前）notes/cross-model-review-results/FABLE5-REDESIGN-001/09-continuation/06-archiving-institution-record.md（双仓归档制度）
- （阶段前）notes/cross-model-review-results/FABLE5-REDESIGN-001/09-continuation/02-owner-correction-dual-channel-rules.md（Owner 沟通规则：跨会话转达件，按 Owner 纠正处理）
- （阶段前）notes/registries/multi-writer-attribution-convention.md §1（四行提交尾注）

**C. 内核本体（只读；分析基线 time 即主检出，可直接读文件，但不得改、不得构建、不得运行任何脚本）**
- 启动阶段只需浏览目录结构（mykernel/ myloader/ myinitramfs/ scripts/ 等），不做分析。

**D. 不读 / 不当真相**：`bugs_record.md`、`todo.txt`、`changelog.md`、`documents/` 是历史材料（可引用不可当现状）；Alaya 私档默认不读（需要时向 Owner 申请；引用 ≤200 字/处且不含隐私）。同目录的 META-AGENT 工作令是平行任务的 C 类参考件，不必读。

### 2.5 回执文件
写到 `/home/cheyh/Downloads/startup-packages-2026-09-02/receipts/MYOS2-LEAD-001-startup-receipt.md`（不写仓库）。内容：

```yaml
record_type: startup_receipt
track_id: MYOS2-LEAD-001
session_machine_name: <ListAgents 所见>
session_uuid: <jsonl 文件名 | unknown>
work_order_sha256_observed: <sha256sum 结果>
environment_checks: [{item, expected, observed, verdict}]   # §2.1 十二项
files_read: [<实际读取清单：只应是 §2.4-A 前四件 ＋ §2.4-B 前两件>]
files_unreadable_or_missing: []
first_impressions_max_5: [<只读六件后的 5 条以内观察，标 [INFERRED]；不是结论>]
incoming_messages_during_ritual: [<若有：发送方名＋摘要；未回复>]
blockers: []
questions_for_owner: [<至多 3 个，每个带三件套：人话意思 / 答了会怎样 / 不答会怎样>]
status: READY_WAITING_FOR_OWNER_SUPPLEMENT | BLOCKED
```

### 2.6 向 Owner 回报（对话回复，人话，≤15 行）
只写：核对了什么、有没有对不上的、你已就位在等补充说明、本轮**没有**写任何仓库/没建 worktree/没动 time 分支；若有问题，至多 3 个，每个附三件套（人话意思 / 答了会怎样 / 不答会怎样）。技术细节一律"已存回执文件 <路径>"。

### 2.7 然后停止
- "确认启动正确" = Owner 读到 §2.6 回报后的一句确认；未确认前视同未启动。Owner 不说"确认"而直接开始补充说明，亦视为确认。
- 补充说明可能分多条：每条只逐字登记（§13 规则）并用一两句人话复述确认，**不据此开工**。
- 只有当 Owner 明确说"开工"/"开始"（或等价的话）时，才进入 §8 阶段 0；Owner 说"开始"即视为同意本工作令写好的全部默认值（§7），想改哪条另说。
- 不回答 12 条决策、不处置 PR、不发射任务——那些是 Owner 的事或阶段 0 之后的事。

## 3. 材料分级
- **A 类（Owner 原话）**：§1.1；§13 补充说明。逐字存档、不改写。
- **B 类（现状与暂定执行源）**：§2.4-A；§5。
- **C 类（历史/方法参考，非路线来源）**：§2.4-B；第一波九份 GPT 产出（按 WAVE-1-REVIEW 降权使用）；同目录的 META-AGENT 工作令。
- **私档**：Alaya `research/MYOS2/`（18 件原件与任务书）与 `indexes/`——只读且默认不读。

## 4. 硬约束（来自环境事实与已发生的事故；不因任何补充说明而放松，除非 Owner 逐条明示）
1. **隔离原则**：agent-workspace/ 之外的一切（mykernel/ myloader/ myinitramfs/ scripts/ documents/ user-guide/ 及根目录文件）是 Owner 的内核本体，任何 agent 不得修改、移动、删除；对内核源码的修改建议以补丁/代码片段放进 results/ 由 Owner 决定。（Owner 原话到"与现有内容完全区分清楚"为止；"不得修改内核本体"是起草方据此立的规则，Owner 已默认 [MI]；列为硬约束。）
2. **主检出在 Owner 私人开发分支 time**：不得 checkout/切换/stash/构建/弄脏。写工作区必须另开 worktree（§7.1），读内核可直接读主检出文件或 `git show time:<path>`。
3. **本地 master ref 过期**：任何以本地 `master` 为基准的 diff/merge-base 都错；一律用 `origin/master`。更新本地 ref 需 Owner 授权。
4. **推送一律显式写完整 URL**：`git push git@github.com:08822407d/MyOS2.git HEAD:refs/heads/<分支>`。禁止 `git push upstream …`（该 remote 配有双 push URL，会同时推到 Owner 的 gitee 备份库 g11de/my-os2bak）；禁止 `git push origin …`（https 无凭据）；不改 remote 配置。
5. **危险脚本绝不执行**：`scripts/phys_nvme_install.sh`（无条件 `rm -rf /mnt1/*`）、`scripts/part_vdisk.sh`（末尾无条件调用 do_part，内部按分区数/类型差异决定重建 GPT 与 mkfs，目标硬编码为 Owner 的真实 VMDK）、`make_install.sh`（一运行即 `rm -rf ./build/*` 并经 make_install_*.sh 挂载 /dev/dm-0）——第一波评审已实证；任何构建/运行动作需 Owner 逐项授权且在隔离副本中做。
6. **SHA 纪律**：commit SHA 只从工具输出原样复制；入库前 `git cat-file -e`；第一波两处伪造 SHA（001 MANIFEST、006 MANIFEST 与 proposed/ci.yml）按"不改写原件"原则保留原样，只能用 -v2 + supersedes 覆盖。
7. **一任务一分支一 PR，禁直推 master**（公约 §1.3，对所有 agent 会话含本会话）；主导会话此前直推 master 是 Mnemosyne 立项记录 D2 所载做法、未写入本仓公约——本会话默认走分支＋PR、Owner 合并，Owner 可改为直推。
8. 不关闭/合并 PR #1~#5、不删远端 agent/ 分支、不 prune worktree、不删本地分支、不回答 12 条 Owner 决策——都是 Owner 或 Mnemosyne 会话（自清）的事。
9. 不写 Mnemosyne、Meta-Agent 仓库；Alaya 写入需 Owner 单独同意。
10. Claude Code 自动记忆（~/.claude/projects/-home-cheyh-projs-MyOS2/memory/）：只允许写 Owner 偏好类条目，不得写任务状态或 Owner 决定（真相只在仓库）。

## 5. 暂定执行源（Owner 明令"之前补充的其他一些临时规范也包含进去作为暂定的执行源"；全部标 provisional，未来 Meta-Agent 可重组）

| # | 规范 | 出处（origin/master d231708c） | 来源性质 |
|---|---|---|---|
| S1 | 总纲：唯一工作区 agent-workspace/；隔离原则；MYOS2-DR-NNN 编号永不复用；完成状态以 MANIFEST 为准；未来接管协议五条 | agent-workspace/README.md | 起草方立、Owner 默认 [MI]（据 Owner 用其跑完九任务、合并 Mnemosyne PR #329/#330 推断，待追认） |
| S2 | 写入规则：一对话=一任务号；唯一可写区 results/<任务号>/ 只增；分支 agent/<任务号>；提交前缀；PR 到 master；禁 force-push/直推；降级路径；必交 MANIFEST；违反=产出作废 | conventions.md 头＋§1 | 同上 |
| S3 | 产出格式：YAML 头七字段；三类标注不混写；图/矩阵双格式；中文说明英文标识符；修订不覆盖（-v2 + supersedes） | conventions.md §2 | 同上 |
| S4 | 共享词汇表：53 个子系统 ID＋能力节点 `<子系统ID>.<能力短语>`；已知缺口 time.misc（WAVE-1-REVIEW §4.11）；待答决策 6 提议新增 repo.build / deploy.scripts 类目；007 报告另提 net/security/extensibility 类目（results/MYOS2-DR-007/MANIFEST.md，尚未采纳） | conventions.md §3；WAVE-2-PLAN §3；results/MYOS2-DR-007 | 同上 |
| S5 | 溯源纪律五条（SHA 只复制、外部结论要 URL/DOI、行号易腐、[VERIFIED] 是承诺、"不存在"须先查配置面 options_flags.cmake） | conventions.md §3b | 第一波事故后新增；Owner 默认 [MI] |
| S6 | 事实纪律：分析基线 time @ a039d980，读 time 写 master；只说"所读快照中未见"；历史材料不当现状；拿不到的数据标 not_measurable | conventions.md §4 | Owner 原话（开发模式纠正）＋起草方操作化 |
| S7 | 分流原则：需要 grep/编译/git 的走本地 Claude；纯外部文献走 GPT 深度研究；有学习价值的实现由 Owner 亲手（本地 Claude 陪跑写回归）；只有 Owner 知道的事实由 Owner 一句话答 | WAVE-2-PLAN.md §0 | 起草方立、待 Owner 追认 |
| S8 | 第二波硬纪律：SHA 机械闸门；删码需"当前 CONFIG 下不可达"证明；行号降级为函数级；实施类任务硬前置（L3 桩、L6 冒烟）；UP/SMP 分标 | WAVE-2-PLAN.md §4 | 起草方立、待 Owner 追认 |
| S9 | 归档：回收件双仓归档（MyOS2 ↔ Alaya research/MYOS2/），bytes/sha256 双向互引；ARCHIVE-RECEIPTS 由主导会话维护、GPT 会话不写；对话侧原件后续统一放 received/；GitHub 侧交付件留本仓不复制 | results/ARCHIVE-RECEIPTS.md；Alaya indexes/research-archiving-convention.md（第 6 条待 Owner 追认） | Owner 原话（2026-08-31 双仓归档制度，见 §1.1）＋起草方操作化 |
| S10 | 记录分工：立项/决策记录曾放 Mnemosyne target-projects/myos2/（README.md L45）；**自本轨道起**：新记录进 MyOS2 §7.2 目录；Mnemosyne 侧只追加一份移交记录后停笔 | README.md L45（旧）→ 本工作令（新） | 本次迁移指示 |
| S11 | Owner 沟通：回复纯人话、技术进文件、提问带三件套；回复末尾一行"本轮推进了什么｜下一步仓库写入：是/否/待授权" | Mnemosyne 09-continuation/02（跨会话转达件）；下一步写入行为 Mnemosyne guard 惯例 | Owner 纠正（转达件，按 Owner 纠正处理）＋起草方操作化 |
| S12 | 署名：重要提交带四行尾注（Agent-Action-Actor / Agent-Task / Agent-Run-Context / Agent-Content-Producer）＋ Co-Authored-By | Mnemosyne multi-writer-attribution-convention §1；MyOS2 d231708c 已用 | 起草方惯例 |
| S13 | 降级交付拆分纪律：正文未包在代码围栏内、无法机械拆分的交付件，宁可留在 received/ 原件里，也不放半截文件 | results/EXTRACTION-NOTE.md | 起草方立、Owner 默认 [MI] |

冲突处理：Owner 原话 > 本工作令 §13 补充 > S1~S13 > 你自己的判断。发现 S 表内部矛盾（例如 004R 任务书 mode 写"普通对话 Pro"而 WAVE-2-PLAN §2 把 L3 派给本地 Claude）→ 记入 open items，报 Owner 定，不自行裁。

## 6. 分析目的与维度

### 6.1 目的（Owner 明令"暂时没有要补充的还是那5点"）
①外围脚本整理优化（CMake/调试/打包安装）；②各子系统完成度＋"重要程度"图（定学习方向）＋"依赖关系"图（实现前提查询＋menuconfig 式裁剪基础）；③基本调试桩快速定位 bug；④查表查手册类低价值工作由 agent 代劳（如 cpuid），前提是 agent 熟悉实现；⑤未来以自制内核为蓝本建学习 agent 组。**不增不减。**

### 6.2 维度（Owner 明令"基于那五方面但不能仅限于它们，而是要考虑其他大型代码项目普遍关心的问题"）
起草方给出"分析维度登记表 v0.1 候选" [MI]，供你在阶段 0 核对、补齐后作为本轨道的维度台账（只增不删，每条注明服务于目的①~⑤中的哪一条，或标"通用关注"）：

| 维度 | 服务目的 | 第一波已覆盖？ | 备注 |
|---|---|---|---|
| 构建系统与可复现性（CMake 结构、工具链钉住、产物链唯一性，如 BOOTX64.EFI 双来源） | ① | 001 部分 | 决策 2/3 待 Owner |
| 配置管理（CONFIG 宏真相表 options_flags.cmake、menuconfig 式裁剪的可行性） | ②裁剪 | 否（L2） | 删码/裁剪判断的前置 |
| 子系统完成度矩阵 | ② | 002 | 需 UP/SMP 分标 |
| 依赖关系图与初始化序列 | ② | 003 | 行号需重锚 |
| 重要程度分级与学习路线 | ② | 007 | 三套分级口径待统一（决策 7） |
| 调试桩、可观测性、断言/自检（instrumentation） | ③ | 004 失败 → 004R | L3；console 通道待决策 5 |
| 正确性嫌疑与并发（自旋、唤醒链、调度状态机、lockdep-lite） | ③ | 008 部分、011 | 主攻清单第 1/4/6 项由 Owner 亲手做（决策 8） |
| SMP bring-up 与 per-CPU 基础（AP trampoline、INIT/SIPI、per-CPU 数据） | ③ | 否 | 决策 11；所有并发结论 UP/SMP 分标的前提 |
| 内存安全与资源生命周期（分配器、页表、释放路径） | ③ | 008 部分 | 通用关注 |
| 用户/内核边界与安全性（syscall 参数校验、权限） | ③ | 否 | 通用关注；学习价值高 |
| 用户态运行时与自制 libc / initramfs（mylib 子树停用还是漏接） | ② | 否 | 决策 1 |
| 测试与 CI（回归防线、QEMU 冒烟、契约测试） | 通用关注 | 006（低可信）→ 010 | L6 |
| 查表资料包（cpuid、MSR、ACPI 表、中断向量…）绑定实际用到的位置 | ④ | 005（cpuid） | 可扩展到其他查表面 |
| 可移植性/架构抽象（mykernel/arch/aarch64 空壳、arch 层边界） | 通用关注 | 否 | 决策 10 |
| 性能基线与测量方法（有无、怎么测、先不优化） | 通用关注 | 否 | 只建基线不调优 |
| 文档与知识传承（教学蓝本、术语表、设计说明） | ⑤ | 009 | 未来学习 agent 组的证据输入 |
| 依赖与许可卫生（musl、EDK2、第三方代码来源与许可证） | 通用关注 | 否 | 公开仓库尤需 |
| 发布/打包/部署脚本的安全性（破坏性动作、干跑模式、参数校验；物理盘同步范围） | ① | 001 部分、评审 §7.4 | 危险脚本清单已知；决策 4 |
| 技术债台账与已知问题 | 通用关注 | 008 | 与 bugs_record 历史材料对照 |
| 代码质量：编译警告、静态分析、UB、风格一致性 | 通用关注 | 否 | 先建"零成本可得"的基线（-Wall 计数等） |
| 仓库与分支卫生（分支模型、历史主题分支去留、PR 残留） | 通用关注 | 部分 | 决策由 Owner |
| 存储栈与设备路径（ATA/NVMe、根盘策略） | ② | 部分（L11） | 决策 9 |

## 7. 写入授权（起草方提议的默认值；Owner 一句"开始/开工"即视为确认默认值；确认前一律只读）

### 7.1 工作副本
- 新建 master 的 worktree：`git worktree add /home/cheyh/projs/myos2-agent-ws -b agent/MYOS2-LEAD-001 origin/master`（路径、分支名 Owner 可改；分支按公约 §1.3 用 `agent/<任务号>`）。主检出 /home/cheyh/projs/MyOS2 永远留在 time、只读。
- 内核本体只读：直接读主检出文件；需要 grep 时在主检出内只读操作；需要构建/运行时另立"隔离副本＋逐项授权"（§4.5）。

### 7.2 路径
- **只新建**：`agent-workspace/lead/MYOS2-LEAD-001/`（主导会话记录：工作令与补充说明原文、启动回执、Mnemosyne 记录镜像、门台账、续接检查点、维度台账、交接记录）；`agent-workspace/results/<新任务号>/`（本地任务产出，一任务一目录）；`agent-workspace/tasks/<新任务书>`；`agent-workspace/tools/`（如 SHA 闸门脚本等工作区内工具，需 Owner 同意）。
- **阶段 0 须同批登记**：README.md 增补一节，登记 lead/ 目录、MYOS2-LEAD-NNN 序列（主导会话记录用）、lead 分支命名——同一 PR 落表（读-占-写同批），不改 README 既有文字（追加或 supersedes）。
- **可修改（工作区维护件，只增或标 supersedes）**：README.md、conventions.md、LAUNCH.md、WAVE-2-PLAN.md、results/ARCHIVE-RECEIPTS.md、results/README.md——公约文本的实质变更须先经 Owner。
- **一律不动**：results/<既有任务号>/ 的原件（含错误）；agent-workspace/ 之外一切。
- 编号：本地任务沿用 `MYOS2-DR-NNN` 序列（下一个可用 012；L1~L11 是计划编号不是任务号，落地时取号）或由 Owner 另定；序号永不复用。

### 7.3 谱系与每批写入
一任务号一分支至多一 open PR；每批写入前 fetch＋钉 SHA＋枚举 open PR＋在续接检查点记录 `authorization_ref: §13 第 N 次补充 / Owner "开工" 原话`；提交前缀 `MYOS2-LEAD-001: `（主导会话维护件）或 `<任务号>: `（任务产出）；四行尾注 `Agent-Action-Actor: claude-fable-5.1@claude-code-vscode / Agent-Task: <任务号> / Agent-Run-Context: <记录路径> / Agent-Content-Producer: claude-fable-5.1`＋`Co-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>`；推送按 §4.4 显式 URL。入库前脱敏核对（Owner 原话中与任务无关的个人信息不入公开仓，是否保留由 Owner 一句话定）。

## 8. 阶段与门（草案；每门停下等 Owner 一字批示。Owner 补充说明可增删）

| 阶段 | 做什么 | 交付（agent-workspace/lead/MYOS2-LEAD-001/） |
|---|---|---|
| 0 接手定向 | 建 worktree；存档工作令＋补充说明原文、启动回执；镜像 Mnemosyne 两份记录（只复制不改，附 bytes/sha256）；README 登记 lead/；读通 A/B；产出"现状盘点＋维度台账 v0.1＋暂定执行源清单（含待追认项）＋open items（含 S 表内部矛盾、PR #1~#5、12 条决策用人话逐条呈给 Owner、悬空 worktree、本地 master ref）"；向 Mnemosyne 会话发接手确认（§10） | 00-owner-work-order-verbatim.md、00-startup-receipt.md、predecessor/（Mnemosyne 00/01 镜像＋收据）、01-orientation-report.md、02-analysis-facets-register.md、03-provisional-execution-sources.md |
| 1 事实基线冻结（原 L1/L2/L4） | 工作区与 time 快照对齐；配置面真相表（options_flags.cmake）；第一波证据的行号重锚为函数级；SHA 闸门脚本化 | results/<取号>/ |
| 2 第二波发射与回收 | 004R/010/011 任务书定稿（发射由 Owner）；回收→对抗核查→可靠性裁定→归档（Alaya 批次 2 + 收据） | tasks/、results/、ARCHIVE-RECEIPTS 批次 2 |
| 3 交叉综合 | completeness × importance × deps → 学习主攻清单与裁剪方案（接管清单第 1 项）；三套分级口径统一 | results/<取号>/ |
| 4 实施类本地任务 | L3 桩、L6 冒烟、L8 构建诊断、L11 存储栈——**前置**：Owner 逐项授权、隔离副本、决策 5 | results/<取号>/ |
| 5 需求⑤ | 学习 agent 组的正式设计输入（009 证据过滤、教学表）；agent 组本身的设计留给未来 Meta-Agent | results/<取号>/ |
| 收口/交接 | 给未来 Meta-Agent 的接管包（Quick Card 式：目标、状态、决定、禁令、未知、oracle、来源指针、隐藏依赖） | 09-continuation/ |

## 9. 记录纪律（自足，不依赖其他工作令）
- 产出文件头：conventions.md §2 的七字段（task_id / produced_by / date / base_snapshot / inputs_read / status / open_questions），外加 `track_id: MYOS2-LEAD-001`、`record_type`、`evidence_class`。
- 证据标签用公约的 `[VERIFIED 路径]/[INFERRED]/[EXTERNAL 出处]`；对仓库状态类事实可加 [VRF]/[MI]。
- 同族局限：第一波 GPT 产出由 Fable 族核查属跨族；本会话对 Mnemosyne 会话（同族同模型）产出的复核不构成独立复核——每份裁定文件固定一节声明。
- 每个子步骤完成即 commit＋push（信息保全优先于整洁）；每门写续接检查点；上下文约 70% 先落盘再提醒 Owner。
- 对话回复：人话＋三件套；末尾一行"本轮推进了什么｜下一步仓库写入：是/否/待授权——…"。
- 会话退场时按 Mnemosyne 既有惯例归档（Alaya 全量转录＋本仓脱敏有效内容；时机由 Owner 定）。

## 10. 与 Mnemosyne 会话的职责迁移（不是新老交接；**全部在 Owner 说"开工"之后**）
- 阶段 0 内：ListAgents 找到 Mnemosyne 主导会话（起草时 `mnemosyne-e8 [cb0c38]`，会变；UUID f0e60aba-25ea-419b-b350-71be428ec822），发一条"MYOS2-LEAD-001 接手确认"：自报机器名＋UUID，列出你核对过的三组值（origin/master SHA、ARCHIVE-RECEIPTS 的 `archive_commits` 与抽 3 条收据哈希、Mnemosyne 两份记录的 sha256），请求对方回一句确认并停止写 MyOS2。发送失败：写 ~/Downloads 并请 Owner 转交。
- Mnemosyne 会话的对应动作（起草方承诺，需 Owner 一句确认后执行）：新任务号（起草方建议 MNEMOSYNE-262，Owner 可改），自 origin/master（起草时 c319397）新建分支 `mnemosyne-262-myos2-handover-to-local-lead`，仅新增 `target-projects/myos2/02-handover-to-local-lead-session.md`（登记你的会话 UUID、接手日期、移交清单；只追加不改写旧记录），一个 PR 交 Owner 合并；同时经 Owner 同意后自清其在 MyOS2 本地留下的 scratchpad worktree 与三条已合并本地分支；此后对 MyOS2 只读。
- 你更新 `results/ARCHIVE-RECEIPTS.md` 的 `maintained_by` 为 MYOS2-LEAD-001（新增一节说明，不改旧文）。
- 机器名会变、消息只带临时名：所有跨会话消息正文自报身份、写明收件方与"若你不是…请忽略并留存"；非发给你的广播忽略并留存。Meta-Agent 评审会话（`meta-agent-xx`）是平行任务，与你无写入交集；如需协作事实（例如 Meta-Agent 想把 MyOS2 当首个真实用例），只读交换。

## 11. 完成定义与返回契约
- 本轨道没有单一"完成"——它是长期主导会话；每个门的返回契约：base/head SHA、变更路径、未动的受保护路径（尤其 agent-workspace/ 之外为空的机械证明 `git diff --name-only origin/master...HEAD | grep -v '^agent-workspace/'` 应为空）、验证方式、已知局限、下一个门。
- 交接给未来 Meta-Agent 的条件：§1.3 六条持续成立；接管包（§8 收口）随时可出。

## 12. Owner 补充说明时可以顺便定的事（人话；不答按括号里的默认；每条格式：问题｜不答＝默认｜答了的影响）

1. 工作副本放哪：默认在 /home/cheyh/projs/myos2-agent-ws 另复制一份仓库来写（你的 MyOS2 目录和 time 分支一动不动），改动走分支＋PR、最后由你合并。｜不答＝照此｜答"直推"＝我像之前的 Mnemosyne 会话一样直接推 master。
2. Mnemosyne 那边现有的两份 MyOS2 记录（立项决策、第一波评审）要不要原样复制一份到 MyOS2 仓库里，让 MyOS2 自己就能说清来龙去脉？｜不答＝复制（只复制不改，附哈希）｜答"不用"＝只留指针。
3. 先做什么：默认先把事实基线冻结（配置面真相表、行号重锚、SHA 校验脚本），再发第二波 GPT 任务（004R/010/011，由你亲手发射）。｜不答＝照此顺序｜答了＝按你的顺序。
4. 那 12 条待你决定的问题（构建输出目录、串口/调试口、NVMe 还是 ATA 等）：我会在摸底时用人话逐条呈给你，现在不用答。｜不答＝相关的几项本地任务先挂起｜答了＝对应任务解锁。
5. 五个还开着的 PR（#1~#5，内容早已进 master）和之前会话留下的临时分支：默认我不碰。｜不答＝留着｜答"清"＝告诉我是你清还是让 Mnemosyne 会话清。
6. 回收的研究原件存进私档 Alaya：我直接写，还是留给你/Mnemosyne 会话？｜不答＝我不碰 Alaya，到时再问｜答"你写"＝我按归档规则写并给你两边哈希。
7. 节奏：每做完一个阶段停下等你一句"可以"，还是一口气做完再报？｜不答＝每阶段停｜答"一口气"＝中途不打扰你，但你只能最后否决。
8. 需要编译或在 QEMU 里跑的工作（冒烟测试、构建诊断）：默认每次单独问你，并且只在另外复制的隔离副本里做。｜不答＝照此｜答了＝按你说的位置和方式。
9. 其他你想加的约束或材料。

## 13. Owner 补充说明登记处

（本节由会话回填 Owner 在对话中所说的话；Owner 不必、也请不要编辑本文件。）

登记规则：本 Downloads 原件冻结不改。写入授权前，每条补充逐字登记到同目录 `receipts/MYOS2-LEAD-001-owner-supplements.md`（逐条标日期、序号）；写入授权后，工作令原文＋全部补充按序进入仓库目录 `agent-workspace/lead/MYOS2-LEAD-001/00-owner-work-order-verbatim.md`，此后新补充继续追加在该仓库副本。与前文冲突时以补充为准，并在登记处注明改了哪条。哈希核验只针对首次读取的 Downloads 原件；之后以回执登记的哈希为准。

---

## §13 登记（仓库副本追加部分）

> 本行以上是 Downloads 原件 `MYOS2-LEAD-001-work-order.md` 的逐字节副本（42,603 bytes，sha256 `be27006ae81788c76476e0baa74b70898d272d8bbdbaf108ee44dc1dc7452ab8`；可用 `head -c 42603 本文件 | sha256sum` 复核）。自本节起为会话按 §13 规则追加的 Owner 补充说明登记；Owner 原话逐字，会话复述标 [MI]。
> 启动确认：Owner 未单独说"确认"，于 2026-09-03 直接开始补充说明——按 §2.7 视为已确认启动正确。写入授权前的登记副本在 Downloads `receipts/MYOS2-LEAD-001-owner-supplements.md`；自写入授权起以本文件为准。

### 第 1 条 · 2026-09-03（Owner 对话原话，逐字）

> 在之前一个claude code对话中让它设计了一个MyOS2代码分析任务书给几个chatgpt pro对话执行.它分析说其中一些对话的结果存在编造的情况.你看看能不能找回它的分析然后再设计一次补充/重做任务书.

会话复述 [MI]：Owner 要求 (a) 找回前一个 Claude Code 会话（Mnemosyne 主导会话）对第一波 ChatGPT Pro 产出"编造"问题的评审分析（已定位：origin/master:agent-workspace/WAVE-1-REVIEW.md，2026-09-01，17 子代理对抗核查）；(b) 在其基础上再设计一轮补充/重做任务书。对应工作令 §8 阶段 2 与 WAVE-2-PLAN 已就绪的 004R/010/011；001/003/005/006 的编造问题此前只有本地任务安排、尚无重做任务书，本条视为要求补齐。主导会话回复时声明：开工后将此项排在第一位。是否改变 §12 第 3 条默认顺序未明示，记为 open item。

### 第 2 条 · 2026-09-03（Owner 对话原话，逐字）

> 开工

会话复述 [MI]：进入 §8 阶段 0（接手定向）。按 §2.7，"开工"即同意 §7 全部默认值与 §12 各条括号内默认（工作副本 /home/cheyh/projs/myos2-agent-ws、分支 agent/MYOS2-LEAD-001、分支＋PR 由 Owner 合并；镜像 Mnemosyne 两份记录；不碰 PR #1~#5 与遗留分支；不写 Alaya；每阶段停；编译/运行逐次单独问）。启动回执中的两个问题（mnemosyne-0c 身份、Mnemosyne 仓是否可 fetch）Owner 未答，按回执所写兜底执行。第 1 条的补充/重做任务书起草排入阶段 0，交付为 draft 任务书，发射由 Owner 亲手。登记时会话标识：myos2-62 [c480bd]，UUID 3bfbb342-1b15-4461-a4c3-987f17aad103。

### 脱敏登记（会话操作，2026-09-03）

仓库副本对以上原件做了 **1 处**可逆脱敏替换：Owner 的 gitee 备份镜像地址（出现于 §2.1 第 2 行与 §4 第 4 条）替换为 `git@gitee.com:<REDACTED-owner-backup-mirror>`；00-startup-receipt.md 中同一地址同样替换。原因：该地址在公开仓（origin/master 与 time 两棵树）中此前从未出现，且非任务所必需（§4.4 的推送规则只需知道"禁止 git push upstream"）。原件逐字节版本仍在 Downloads（sha256 见上）。Owner 一句"保留"即改回逐字节副本；未表态则维持脱敏。因此上文 `head -c 42603` 复核法在脱敏版本上**不成立**，以 Downloads 原件为准。
