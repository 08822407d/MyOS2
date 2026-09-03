# MyOS2 目标项目立项与第一波分析发射记录（MYOS2-ANALYSIS-001）

```yaml
record_type: target_project_intake
track_id: MYOS2-ANALYSIS-001
date: 2026-09-01
executed_by: 继任主导会话（claude-fable-5@claude-code-vscode，FABLE5-HANDOFF-001 接收方）
trigger: Owner 插入紧急任务——ChatGPT Pro 周额度十余小时后刷新，消耗额度＋检验 Fable5 理解力与 Mnemosyne 建设成果；原计划此需求待 meta-agent 建成后由其规划，现提前以"允许不完美、但必须可接管"方式启动
target_repo: github.com/08822407d/MyOS2（本地 /home/cheyh/projs/MyOS2）
workspace_commit: MyOS2 master @ c83ded07（agent-workspace/ 引导提交；分析基线快照=其父 63f0785c）
task_series: MYOS2-DR-NNN（本项目专用序列，先例=meta-agent 之 MA-DR；不占用 MNE-DR 序列，MNE 线 next 仍为 034）
```

## 1. Owner 需求摘录（S0 见对话原文，此为工作摘要）

背景：Owner 数年模仿 Linux 自研学习用内核 MyOS2，遇学习瓶颈，欲借顶尖 LLM 深度学习内核设计，第一步是分析现有实现。明示五项需求：①整理优化外围脚本（CMake/调试/打包）；②各子系统完成度＋"重要程度"图（定学习方向）＋"依赖关系"图（实现前提查询＋menuconfig 式裁剪基础）；③基本调试桩以快速定位 bug；④查表查手册类低价值工作由 agent 代劳（如 cpuid 信息，前提是 agent 熟悉其实现）；⑤未来以自制内核为蓝本建学习 agent 组（Linux 太复杂不宜作教学底本）。Owner 声明需求不止这些。

四项原则：①急用先行、允许不完美，但后续基建完善后必须能完美接手并吸收成果（禁止只能推倒重来的格式）；②成果入 MyOS2 仓库但与现有内容完全区分，未来再重设文件组织；多个 Pro 对话须能轻松启动；③设计不限于明示需求，须借鉴大型系统开发普遍关注点预估其他需求；④任务书须支持多对话并行写库不互扰（必然发生；ChatGPT Pro 只能读写 GitHub 仓库、无法访问本地）。

## 2. 设计决策（应用 Mnemosyne/SYN-2 成果之处）

| 决策 | 内容 | 依据 |
|---|---|---|
| D1 编号 | 新开 MYOS2-DR 序列 | MA-DR 先例；不触 MNE 注册表（维护线路径，must_not_do） |
| D2 落点 | MyOS2 根下唯一新目录 agent-workspace/，纯新增、零改动现有内容，直推 master | Owner 原则②＋连接器读默认分支最顺 |
| D3 并行写协议 | 一任务=一对话=一分支（agent/任务号）=一 results 子目录；PR 合并互斥目录可乱序；无法写库则降级为对话内逐文件输出（人工入库） | Owner 原则④；ANNEX-C 分层/降级思想 |
| D4 可接管性 | 产出定位为 S1 证据非规范；YAML 头带出处；[VERIFIED]/[INFERRED]/[EXTERNAL] 三类标注；修订只增不改（supersedes）；MANIFEST 强制申报覆盖状态；共享节点词汇表使各任务 YAML 可机器合并；基线快照锚定 63f0785c | Owner 原则①；N-14/N-19、收据与 not_measurable 纪律的轻量移植 |
| D5 任务面 | 明示需求→001~005/007；预估需求（原则③）→006 测试与 CI、008 技术债台账、009 教学蓝本预研 | 大型系统普遍关注：回归防线、债务清点、知识传承 |
| D6 波次 | 第一波九任务全并行（互不依赖，重要度/完成度/依赖三图并行产出靠词汇表对齐）；交叉综合（重要×未完成×依赖→主攻清单）留第二波，不烧本周额度 | 并行安全＋额度窗口 |

## 3. 第一波任务清单

001 外围工程审计（普/P0）· 002 子系统完成度盘点（普/P0）· 003 依赖关系图（普/P0）· 004 调试桩与可观测性（普/P0）· 005 x86 查表资料包（普/P1）· 006 测试与 CI 策略（深/P1）· 007 重要度分级与路线图（深/P1）· 008 技术债台账（普/P2）· 009 教学蓝本预研（深/P2）。任务书全文与启动提示词见 MyOS2 仓库 agent-workspace/{tasks/,LAUNCH.md}。

## 4. 留给 meta-agent（接管清单）

1. 第二波交叉综合：completeness.yaml × importance.yaml × deps.yaml → 学习主攻清单与裁剪方案（词汇表已对齐，可机器合并）。
2. 回收质检：按各任务书验收判据核产出，不合格者标注（不返工历史对话，新任务补）。
3. 结构重组权：agent-workspace/ 可整体迁移重构，唯 results/ 原件与 MANIFEST 链条不可丢。
4. 需求⑤（学习 agent 组）正式设计：009 预研为其证据输入。
5. 本记录后续增补（回收状态、第二波取号）以新文件/新章节追加，不改写本文件既有内容。

## 5. 风险与已知不完美（诚实登记）

- ChatGPT 连接器能否建分支/开 PR 未在 MyOS2 上实测（Mnemosyne 仓 7 月实测可写，PR #326），故降级路径写死在公约与启动提示词中；
- repo-map.md 为目录级扫描＋抽读，未逐行核实，已在文中自我标注；
- master 快照可能落后于主题分支（time/mmap/slub…）最新进展，公约 §4 已约束结论表述为"master 快照"；
- 九任务由同族模型（GPT）执行，无异族复核——第一波定位为证据采集非裁决，风险可接受。

## 6. 增补（2026-09-01，Owner 补充说明后的基线修正）

Owner 补充：两年来 MyOS2 全部开发在本机进行，习惯为每个机制/子系统开专门分支（改动不限于该子系统源码），基本达标才合并 master——**本机当前检出分支才是最新版本**。核实：当前分支 `time` @ a039d980 已同步 GitHub，master（63f0785c）是其祖先、落后 44 提交/324 文件（含 time 新增的 mykernel/cpu/ 目录）。

处置（MyOS2 master @ 02ec5877）：分析基线由 master 改为 **time @ a039d980**；确立"读 time、写 master"分工（工作区与产出仍在 master，results 纯新增、未来 time 合并 master 无冲突）；conventions/repo-map/LAUNCH 提示词同步改写，产出 YAML 头 base_snapshot 改为强制申报实际所读分支＋commit（连接器读不到 time 时允许降级用 master 分析）。新增风险：连接器能否读非默认分支未实测，降级申报机制对冲；repo-map 原本就是按本地 time 检出扫描生成，内容无需重做，仅更正了标注。
