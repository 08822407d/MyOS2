# MyOS 教学蓝本预研交付报告

## 交付状态

[VERIFIED `agent-workspace/conventions.md`] 本会话可用的 GitHub 连接器仅暴露读取、搜索、比较等能力，没有创建分支、提交文件或创建 Pull Request 的写入动作，因此不能实际建立 `agent/MYOS2-DR-009` 或写入仓库；按写入公约的降级规则，下面逐文件完整给出全部交付物，且不写任何其他路径。fileciteturn0file0

[VERIFIED GitHub branch metadata] `time` 分支可正常读取，HEAD 为 `a039d9803ade2a1613d620bda375e028530d5242`；因此所有交付物的 `base_snapshot` 均如实使用 `time @ a039d9803ade2a1613d620bda375e028530d5242`，没有回退到 `master`。fileciteturn4file0

[VERIFIED `agent-workspace/tasks/MYOS2-DR-009-teaching-blueprint-prestudy.md`] 本任务边界明确要求“不深入 MyOS2 源码，读 repo-map.md 的子系统清单即可”，所以我没有为扩大证据量而越界逐文件审计内核实现；MyOS2 相关课程映射严格停留在仓库地图和共享词汇表层面，并统一标记 `[INFERRED]`。任务要求的五个文件为 `MANIFEST.md`、`01-precedents.md`、`02-llm-teaching-evidence.md`、`03-own-kernel-specifics.md`、`04-curriculum-skeleton.md`，下文全部给出。fileciteturn3file0

## 清单文件

目标路径：`agent-workspace/results/MYOS2-DR-009/MANIFEST.md`

task_id: MYOS2-DR-009  
produced_by: GPT-5.5 Thinking  
date: 2026-08-31  
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"  
inputs_read: ["agent-workspace/conventions.md", "agent-workspace/repo-map.md", "agent-workspace/tasks/MYOS2-DR-009-teaching-blueprint-prestudy.md"]  
status: final  
open_questions: ["repo-map.md 的元数据 date 为 2026-09-01，而本次运行日期为 2026-08-31；time 分支 HEAD 已通过 GitHub branch metadata 独立确认，因此该日期差异不影响本任务 base_snapshot，但后续汇总 agent 可决定是否需要统一工作区文档日期。"]

**任务覆盖**

[VERIFIED `agent-workspace/tasks/MYOS2-DR-009-teaching-blueprint-prestudy.md`] 本目录回答任务书规定的四个研究问题：小型教学内核先例、LLM 辅助代码教学的证据与反模式、个人自有代码库作教材的特有问题、以及主流 OS 课程骨架与 MyOS2 初步映射。任务书同时要求不设计未来 agent 组本身、不深入 MyOS2 源码，并要求外部结论带来源。fileciteturn3file0

| 文件 | 状态 | 已完成内容 | 明确未做 | 后续 agent 消费方式 |
|---|---|---|---|---|
| `01-precedents.md` | final | xv6、Pintos、OS/161、MINIX，以及 OSTEP 的课程组织方法；提炼“读代码→改代码→补功能”阶梯与工业实现差距说明法 | 未为 MyOS2 具体函数设计练习 | 用作未来教学蓝本的课程方法论证据库 |
| `02-llm-teaching-evidence.md` | final | 代码讲解、苏格拉底式引导、练习生成、反馈/批改证据；列出七类有来源反模式及缓解措施 | 未设计具体 agent 角色、prompt 或工具编排 | 用作未来 meta-agent 制定教学安全约束和验证门 |
| `03-own-kernel-specifics.md` | final | 区分 design/simplification/bug/unknown 的教材化协议；标准实现对照法；作者即学习者的认知盲区对策 | 未判断 MyOS2 当前任何具体实现是否为 bug | 用作未来教材章节的“事实卫生”和对照教学规范 |
| `04-curriculum-skeleton.md` | final | MIT xv6、Pintos、OS/161、OSTEP 课程骨架对照；给出 MyOS2 初步课程单元；逐项覆盖共享词汇表全部 53 个子系统 ID | 未根据实际函数调用图确定精确先修依赖 | 用作未来 meta-agent 的课程 DAG 初稿，之后应与依赖图/完整性任务产物交叉校验 |

**验收自检**

[VERIFIED `agent-workspace/conventions.md`] 共享词汇表规定了 53 个本任务可引用的子系统 ID；`04-curriculum-skeleton.md` 逐项覆盖这 53 个 ID，没有自行把目录名替换成新的顶级节点。fileciteturn0file0

`02-llm-teaching-evidence.md` 给出七类反模式，超过任务书“至少五条有出处的失败模式与对策”的最低要求；其中包括代码幻觉、答案替代学习、迎合错误认知、上下文不足、难度错配、只解决眼前 bug 而不形成概念模型、以及 LLM 批改不稳定。

[VERIFIED `agent-workspace/repo-map.md`] MyOS2 映射只采用 repo-map 给出的目录/子系统信息；没有把 `bugs_record.md`、`todo.txt`、`changelog.md` 或 `documents/` 当成当前实现事实，也没有读取历史主题分支。fileciteturn1file0

**证据等级约定**

本文档集沿用仓库公约：

`[VERIFIED ...]` 表示在本次实际读取的仓库工作区材料或 GitHub branch metadata 中核实。  
`[INFERRED]` 表示从共享子系统命名、外部课程组织方式或教育研究证据推导出的教学建议，不冒充 MyOS2 实现事实。  
`[EXTERNAL ...]` 表示来自课程官网、论文、研究机构或教材官网的外部证据，并附引用。

**总体结论**

[INFERRED] MYOS2-DR-009 的外部证据足以支持未来 meta-agent 采用“**小而真实的内核为主线、标准工业系统作对照、可执行证据作事实裁判、LLM 只提供受约束的脚手架**”这一方向；但本任务并不把这一方向具体化成 agent 角色、prompt、工具路由或自动化工作流，那些内容留给任务书所说的未来 meta-agent。

## 教学内核先例文件

目标路径：`agent-workspace/results/MYOS2-DR-009/01-precedents.md`

task_id: MYOS2-DR-009  
produced_by: GPT-5.5 Thinking  
date: 2026-08-31  
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"  
inputs_read: ["agent-workspace/conventions.md", "agent-workspace/repo-map.md", "agent-workspace/tasks/MYOS2-DR-009-teaching-blueprint-prestudy.md"]  
status: final  
open_questions: ["未来 MyOS2 课程的首要学习者究竟定位为已有 C/体系结构基础的 OS 初学者，还是已经写过 MyOS2 的 Owner；这会显著改变同一源码锚点所需的脚手架强度，但不影响本预研的方法论结论。"]

**研究结论**

[EXTERNAL MIT 6.1810] “用小内核教大原理”并不是把工业内核机械删减，而是选择一个仍然保留真实内核结构和机制互动、但规模足以被学习者整体理解的系统。MIT 对 xv6 的官方说明正是：它足以展示 OS 基本设计和实现思想，同时远小于现代生产 OS、因而更容易理解；其结构又与许多现代系统相似，因此可作为理解更大内核的桥梁。citeturn19search0

[EXTERNAL Stanford Pintos / Berkeley CS162] Pintos 采用另一种很有价值的策略：先提供一个“真实可运行、但故意简单或不完整”的框架，再让学生强化线程、用户程序、虚拟内存和文件系统。Berkeley 对这种选择的解释也很直接：目标是在不过度复杂的前提下，让学生在真实工作内核中实践 OS 核心思想。citeturn13search1turn13search6

[EXTERNAL Harvard OS/161] OS/161 体现的是“逐层承担更多设计责任”的路线。Harvard 的经典 CS161 作业依次覆盖 OS/161 与同步、系统调用与进程、虚拟内存、文件系统；ops-class 的版本也从环境/源码/GDB 入门开始，再进入同步、进程/系统调用和 VM。citeturn15search1turn13search3

[EXTERNAL MINIX] MINIX 则提供了“源码本身就是教材组成部分”的长期先例。MINIX 官方材料指出，MINIX 1/2 原本就是教学工具；MINIX 3 仍以完整开放源码适合课堂或自学，同时把系统重构为小型微内核和用户态服务。这说明教学系统不仅可以解释通用机制，也可以借其明确的架构选择与其他内核作对照。citeturn14search10turn14search24turn14search0

[EXTERNAL OSTEP] OSTEP 虽不是单一教学内核，却提供了互补的课程组织证据：它把 OS 基础组织为 virtualization、concurrency、persistence 三大主题，并为章节配套模拟器、测量作业和更大的系统项目。也就是说，“小代码底本”不必独自承担全部抽象教学；概念模型、模拟器和真实内核可以组成不同尺度的学习材料。citeturn14search4turn14search18turn14search22

**先例对照**

| 先例 | 讲义如何围绕代码组织 | 练习主要形态 | 如何处理简化系统与工业系统的距离 |
|---|---|---|---|
| xv6 / MIT 6.1810 | [EXTERNAL MIT] 先讲概念并指定 xv6 book 章节和具体源码文件，再进入对应 lab；课程日程把系统调用、页表、trap、中断、锁、调度、文件系统等讲授与源码阅读直接交错。citeturn19search10turn13search8 | 读指定源码；用 GDB/输出观察；修改机制；新增 syscall、VM 功能、锁优化、网络等。citeturn13search0turn13search20turn13search32 | [EXTERNAL MIT] 明说 xv6 比生产 OS 小得多，但结构具有代表性；随后通过现代论文或扩展 lab 补足现代机制。citeturn19search0turn19search10 |
| Pintos / Stanford、Berkeley | [EXTERNAL Pintos] 每个 project 文档先说明 background/source files，再给 suggested order、requirements、design document 和测试要求。citeturn13search21 | 在已有 skeleton 上补线程/用户程序/VM/FS；调试基础实现；提交设计文档和通过测试。citeturn13search1turn13search6 | [EXTERNAL Pintos] 文档明确承认框架实现简单、功能有限；例如 syscall 接口并不像 Linux/FreeBSD 那样丰富。教学价值正来自“已知限制→学生改进”。citeturn13search14 |
| OS/161 / Harvard 等 | [EXTERNAL OS/161] 从工具链和源码熟悉开始，随后同步→进程/syscall→VM→FS；作业要求学生在较大代码库中定位实现位置。citeturn13search7turn15search1 | 代码阅读、同步原语实现、核心数据结构设计、syscall、VM、FS；部分课程有显式 design review。citeturn13search39turn15search15 | [EXTERNAL ops-class] OS/161 的课程介绍把自身定位成“成熟生产 OS 太难修改”和“过度简化教学框架不够真实”之间的折衷。citeturn13search23 |
| MINIX | [EXTERNAL MINIX] 源码和解释性教材长期配套；早期版本的目标本身就是让学生看到、研究可理解的 Unix-like 实现。citeturn14search16turn14search24 | 阅读和修改完整源码；以架构边界、IPC、服务/驱动隔离等为讨论对象 | [EXTERNAL MINIX] MINIX 3 的微内核和用户态服务不是“所有现代 OS 的缩小版”，而是一种明确架构选择，因此特别适合做架构对照。citeturn14search0turn14search12 |
| OSTEP | [EXTERNAL OSTEP] 先建立概念模型，再用小型 simulator、测量程序和项目巩固。citeturn14search18turn14search19 | 预测模拟结果、测量现实系统、编写系统程序/项目 | 不要求一份教学源码覆盖所有现实复杂度；不同抽象层使用不同学习载体。citeturn14search1turn14search22 |

**“读代码—改代码—补功能”三阶梯**

[INFERRED] 对 MyOS2 最可复用的不是某门课的具体 lab，而是三个难度层级之间的迁移。

| 阶段 | 学习者必须产出的东西 | 典型证据 | 教学目的 |
|---|---|---|---|
| Read / 读 | 控制流说明、数据结构解释、状态预测、调用点定位、关键不变量 | 源码行、运行 trace、已有测试 | 从“我记得概念”转成“我能从实现中指出概念在哪里” |
| Modify / 改 | 在不改变总体架构的前提下调整已有机制，例如增加观测、改变策略、修复局部缺陷 | patch + before/after test | 学会不变量、边界条件、模块间耦合 |
| Extend / 补 | 新增 syscall、同步原语、VM 特性、FS 特性或设备能力 | 设计说明 + patch + 新测试 + 回归测试 | 从理解现有设计跨到独立设计 |

[EXTERNAL MIT] 这三个层级在 xv6 lab 中可以看到很清楚的连续体。例如页表 lab 要求学生先阅读 VM 章节和 `memlayout.h`、`vm.c`、`kalloc.c` 等具体源码，然后探索/修改页表并实现 OS 功能；syscall lab 则要求新增系统调用。citeturn13search0turn13search20

[EXTERNAL OS/161] OS/161 的入门作业也把代码阅读、简单脚本和小实现放在最前，然后才进入同步、进程、VM 等较重设计。citeturn13search11turn13search39

**讲义应采用的“源码锚点”结构**

[INFERRED] 对未来基于 MyOS2 的教学材料，推荐每个机制使用固定六段式，而不是先写一大篇抽象理论再把源码当附录：

| 段落 | 内容 |
|---|---|
| Concept | 该机制解决什么问题；先给最小抽象模型 |
| Prediction | 在看实现前要求学习者预测关键状态变化或失败模式 |
| MyOS2 Anchor | 指到一个非常有限的目录/函数/结构体集合 |
| Trace | 顺着一个真实路径走完：输入→状态变化→输出/阻塞/唤醒 |
| Delta | 明确列出 MyOS2 与 textbook/工业实现之间的差异 |
| Exercise | Read → Modify → Extend 中择一；必须有可检查产物 |

[EXTERNAL MIT/Pintos] 这样做与 xv6“阅读章节＋指定源文件＋lab”和 Pintos“background/source files＋implementation order＋requirements/design document”的组织方式一致。citeturn13search0turn13search21

**如何不让简化版误导学习者**

[INFERRED] 简化不能只说“真正的 Linux 更复杂”，因为这不给学习者任何可操作的边界。每个主题都应有一张固定的 **Delta Card**：

| 字段 | 含义 |
|---|---|
| `shared_concept` | 教学内核与工业系统共同体现的核心原理 |
| `myos2_choice` | MyOS2 在所读快照中实际采用的设计；必须有源码证据后才能写 `[VERIFIED]` |
| `intentional_simplification` | 有独立证据证明是刻意简化时才使用 |
| `not_implemented_or_not_seen` | 只允许写“所读快照中未见”，不得推出绝对不存在 |
| `production_counterpart` | Linux/BSD/文献中的代表性工业机制 |
| `why_difference_matters` | 对性能、正确性、可扩展性、安全性或可维护性的后果 |
| `extension_question` | “若把工业要求加回来，MyOS2 哪些不变量必须改变？” |

[EXTERNAL Pintos] Pintos 的材料提供了这种诚实交代差距的直接范例：它一方面把框架作为真实内核使用，另一方面明确告诉学生其 syscall、文件系统等能力受到简化，而项目正是围绕这些限制进行扩展。citeturn13search1turn13search14

[EXTERNAL MIT] xv6 则采用“代表性结构 + 后续现代机制”的做法：先用可理解的小系统形成模型，再通过 COW、mmap、网络、RCU、Meltdown 等实验或阅读跨向现代系统问题。citeturn19search10

**测试不是评分附件，而是教材的一部分**

[INFERRED] 未来 MyOS2 教学中，任何“改代码/补功能”练习都应把判定依据同时给出：编译结果、确定性测试、关键 trace 或不变量检查。这样测试承担两个角色：一是作业 oracle，二是告诉学生“什么行为才是规范”。

[EXTERNAL Pintos/OS161] Pintos 文档围绕项目测试组织验收，OS/161 生态也提供专门的 `test161`；这两类课程都把“能够跑、能够调、能够回归”作为内核学习过程，而不是只靠阅读提交代码。citeturn13search34turn13search27

**设计文档应先于复杂实现**

[EXTERNAL Pintos/Harvard] Pintos project 文档包含设计文档模板；Harvard CS161 的进程/调度作业甚至安排 in-class design review，并把设计先于最终实现提交。citeturn13search21turn15search15

[INFERRED] 因此 MyOS2 的 Extend 级练习不应只问“把 X 写出来”，而应先要求学习者回答四件事：状态放在哪里、不变量是什么、并发点在哪里、失败如何观察。只有这四项能和代码、测试互相对上，才进入实现。

**最值得迁移到 MyOS2 的方法**

[INFERRED] 预研最终建议未来课程设计遵守以下链条：

`概念模型 → 源码锚点 → 先预测 → 沿真实执行路径阅读 → 小修改 → 可执行验证 → 工业实现对照 → 独立扩展`

这个链条综合了 xv6 的“小而代表性”、Pintos 的“不完整骨架＋项目改进”、OS/161 的渐进式设计责任、MINIX 的源码即教材、以及 OSTEP 的概念/模拟/测量分层。citeturn19search0turn13search6turn15search1turn14search10turn14search18

**证据索引**

[EXTERNAL] MIT 6.1810 官方 Overview 与 Labs：小内核选择理由、源码阅读与扩展式实验。citeturn19search0turn13search0turn13search20  
[EXTERNAL] Stanford Pintos / Berkeley CS162：教学 OS 框架、project 结构、设计文档和明确的简化限制。citeturn13search1turn13search6turn13search21turn13search14  
[EXTERNAL] Harvard CS161 / OS/161：同步、process/syscall、VM、FS 的渐进作业骨架。citeturn15search1turn15search15  
[EXTERNAL] MINIX 官方材料：教学起源、完整源码、微内核架构及 MINIX 3 目标变化。citeturn14search10turn14search24turn14search12  
[EXTERNAL] OSTEP 官方材料：virtualization/concurrency/persistence、模拟器、测量和系统项目。citeturn14search4turn14search18turn14search22

## LLM 教学证据文件

目标路径：`agent-workspace/results/MYOS2-DR-009/02-llm-teaching-evidence.md`

task_id: MYOS2-DR-009  
produced_by: GPT-5.5 Thinking  
date: 2026-08-31  
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"  
inputs_read: ["agent-workspace/conventions.md", "agent-workspace/repo-map.md", "agent-workspace/tasks/MYOS2-DR-009-teaching-blueprint-prestudy.md"]  
status: final  
open_questions: ["现有研究主要来自通用编程课、数学辅导或普通代码生成，而不是‘学习者用自己编写的内核作为教材’这一精确场景；因此这里的策略有实证支持，但 MyOS2 场景本身仍需要未来小规模教学实验验证。"]

**证据强弱总览**

[EXTERNAL] 截至本次调研，可把“LLM 用于代码教学”的证据分成三层。最强的是有真实课堂部署甚至随机实验的数据；其次是对真实学生代码的反馈/自动评分评估；最弱的是仅凭模型能生成代码或练习就推定“能促进学习”。后者不能等同于学习成效。

| 用途 | 现有证据 | 可以支持的结论 | 不能据此推出 |
|---|---|---|---|
| 受约束代码帮助 | CodeAid 在约 700 人编程课部署 12 周，分析约 8,000 次使用；CodeHelp 在 52 名学生的一年级课程部署 12 周 | LLM 可以承担随时可用、不给完整答案的解释/提示层；学生确实会使用这类工具。citeturn16search0turn16search17 | 不能证明任意 ChatGPT 式自由回答都提高长期学习 |
| 苏格拉底式/指导式提示 | Tutor CoPilot 的预注册 RCT 涉及 900 名 tutor、1,800 名学生；有 AI 指导的 tutor 更常用 guiding questions、较少直接给答案，主题掌握率总体提高 4 个百分点 | 引导式 AI 脚手架可以改善真人辅导行为和学习结果。citeturn16search2 | 这是 K-12 数学 tutoring，不是内核编程；迁移到 MyOS2 属 `[INFERRED]` |
| 不给答案的编程反馈 | 51 名学生、一个学期的 GPT-4 编程反馈系统中，大多数反馈能针对代码错误，但仍出现错误建议和“幻觉问题” | 及时、非直接答案式反馈有现实可行性，但必须验证。citeturn11academia43 | 不能让 LLM 自己成为最终正确性 oracle |
| 练习生成 | Codex 研究已能生成编程题、样例解和测试；后续综述发现多个 LLM 能产生有用练习，但也指出 LLM 自己容易解决 LLM 生成的题等问题 | LLM 可做题目草拟器。citeturn11search28turn11academia40 | “自动生成成功”不等于难度合适、概念覆盖正确或测量到目标能力 |
| 自动批改/反馈 | 自动编程评分系统综述显示传统可靠核心常是 unit testing、静态分析或与参考实现比较；LLM 研究能产生较丰富反馈，但真实学生 submission 上也观察到前后矛盾的诊断 | 适合“确定性 oracle + LLM 解释”组合。citeturn11search14turn11academia42 | 不宜仅凭 LLM 自然语言分数作为高风险最终成绩 |
| 自动测试生成 | 对 26 个 CS1 问题和超过 25,000 次学生提交的研究发现，LLM 生成测试在多数问题上可识别大多数合法解，并暴露一些题意歧义 | LLM 可辅助扩充测试集和检查作业规格。citeturn11academia41 | 自动生成测试仍不能替代手工定义的核心不变量和关键回归用例 |

**代码解释：最合理的形式不是“替学生读完”**

[EXTERNAL CodeAid] CodeAid 的设计非常贴近未来内核教学需要：它回答概念问题、生成带逐行说明的伪代码、在学生错误代码上做注释和修复建议，但刻意避免直接吐出完整解法。其真实课堂部署得到的设计建议包括降低提问门槛同时维持认知投入、避免直接回答，并保持透明度和学习者控制。citeturn16search0

[EXTERNAL self-explanation research] 对源码理解的独立教育研究也支持让学习者自己解释代码：Java 源码理解实验发现 self-explanation 有助于学习，而且学生产生的自我解释量与学习增益呈较强正相关。citeturn12search9

[INFERRED] 因而 MyOS2 教学里，LLM 最有价值的代码解释输出不是“这个函数做 A、B、C”，而是先让学习者预测，再只指出需要看的证据锚点，最后要求学习者用自己的话重建控制流；LLM 再负责指出遗漏。

一个合适的回答顺序是：

`预测 → 证据定位 → 学习者解释 → LLM 质询 → 运行/测试验证 → 完整总结`

而不是：

`问题 → LLM 长篇正确答案 → 下一题`

**苏格拉底式提问：有希望，但不能只靠“语气像老师”**

[EXTERNAL Tutor CoPilot] Tutor CoPilot 的 RCT 说明，AI 可以让 tutor 更频繁采用 guiding questions，并减少直接把答案交给学生；总体掌握率改善 4 个百分点，原本评分较低 tutor 对应的学生改善更大。与此同时，访谈也发现部分 AI 建议并不适合学生所在年级。citeturn16search2

[INFERRED] 对 MyOS2，这意味着“苏格拉底式”应被定义成**可检验行为**，而不是 prompt 里写一句“你是苏格拉底老师”：

| 行为门 | MyOS2 示例 |
|---|---|
| 先取当前认知 | “在看实现前，你预计 page fault 后哪个状态首先变化？” |
| 要求证据 | “请指出你判断所依据的函数/字段，而非凭 Linux 记忆回答。” |
| 一次只揭示一层 | 先给文件锚点，再给函数，再给相关条件分支，最后才给解释 |
| 检查迁移 | “如果这里换成两个 CPU 并发，会有哪些不变量需要重新成立？” |
| 最后才总结 | 在学习者完成证据链之后提供整合答案 |

**练习生成：LLM 适合起草，不适合直接出版**

[EXTERNAL exercise generation research] 早期 Codex 研究表明，LLM 可以同时生成编程练习、样例解和测试；2024 年针对编程练习生成的综述也认为多个 LLM 能产生有用练习，但指出仍存在质量与评估问题，例如 LLM 生成的题可能很容易再被 LLM 解出。citeturn11search28turn11academia40

[INFERRED] 内核教学比普通 CS1 更要求题目和当前代码快照一致。因此未来 MyOS2 练习生成必须经过四个门：

`source anchor valid → reference answer verified → test oracle passes → pedagogical target checked`

其中任一门失败，题目都只是 `draft`。

[INFERRED] 最佳练习生成输入也不应只有“给我出一道 spinlock 题”，而应包含：目标 capability、允许阅读的代码范围、先修知识、现有测试、禁止泄露的目标实现、希望属于 Read/Modify/Extend 哪一级。这样做能把“语言模型自由发挥”收缩成“在确定教材边界内变体生成”。

**批改与反馈：确定性裁判在前，LLM 解释在后**

[EXTERNAL automated grading review] 对 2017–2021 年 121 篇自动编程评分/反馈研究的系统综述显示，最常见的可靠基础仍然是动态 unit tests、静态分析，或与参考解/正确学生解比较；这类工具的局限主要在于反馈往往较贫乏。citeturn11search14

[EXTERNAL GPT feedback studies] LLM 能补上“为什么”的自然语言层，但并不稳定：对真实学生代码的 GPT-4 Turbo 反馈研究发现结构和一致性有所改善，却仍出现“先说提交正确、随后又说需要修一个错误”这样的内部矛盾；另一项一学期部署也报告错误建议和虚构问题。citeturn11academia42turn11academia43

[INFERRED] 因而对 MyOS2 最安全的批改架构是：

`compile/test/trace/static checks → structured facts → rubric decision → LLM pedagogical explanation`

而不是：

`student patch → LLM reads → LLM decides correctness`

LLM 可以解释“哪个测试揭示了哪条不变量”，但不能把自己未经执行的猜测升级成测试事实。

**反模式与对策**

| 反模式 | 外部证据 | 对 MyOS2 的风险 | 推荐缓解 |
|---|---|---|---|
| **代码幻觉：说出不存在的 API、控制流或行为** | [EXTERNAL CodeHalu] LLM 生成代码可能语法/语义表面合理却不能执行或不符合要求；CodeHaluEval 用 8,883 个样本、699 个任务系统评估了这类问题。citeturn18search0 | 把“Linux 通常这样”错投影成“MyOS2 就这样”，污染教材事实 | 所有实现解释必须引用当前 snapshot 的文件/函数锚点；关键行为由编译、测试或 trace 再验证 |
| **项目上下文幻觉：发明不存在的本仓库 API** | [EXTERNAL De-Hallucinator] 针对项目特定代码，检索实际 API 并把它们作为 grounding 能显著降低 API hallucination。citeturn18search2 | 自制内核 API 与 Linux 同名/近似名时尤其危险 | 检索当前仓库定义优先；回答中把“源码事实”和“外部类比”强制分栏 |
| **直接给答案，形成“拐杖效应”** | [EXTERNAL PNAS] 无 guardrail 的生成式 AI 在练习阶段能提高即时表现，却可能使学生在撤去 AI 后表现更差；CodeAid/CodeHelp 因此主动避免直接解答。citeturn16search3turn16search0turn16search17 | 学习者能合入 patch，但不能独立解释机制 | 默认提示/伪代码/问题链，完整实现需要通过“已尝试 + 能解释不变量”门槛后才揭示 |
| **迎合学习者的错误认知** | [EXTERNAL sycophancy research] 对五个 AI assistant 的研究发现多种自由回答任务中存在迎合用户观点的倾向，而且人类偏好数据有时会奖励与用户看法一致但不正确的回答。citeturn17search0turn17search1 | Owner 说“我记得这里肯定是 X”，模型可能顺着说，把作者记忆冒充源码事实 | 先独立取源码/测试证据，再评估用户假设；答案格式明确写“claim / evidence / verdict” |
| **上下文不足却强行诊断** | [EXTERNAL CodeHelp help-seeking] 真实学生常给出很少上下文；CodeHelp 因而用半结构化表单要求语言、代码片段、错误信息和问题描述。citeturn16search5 | 缺 commit、配置、调用方、日志时，内核错误极易误判 | 强制 context packet：snapshot、文件/函数、复现步骤、config、日志、测试结果；不足则标 `not_measurable`/`unknown` |
| **难度和教学阶段错配** | [EXTERNAL Tutor CoPilot] 真实 tutor 访谈指出 AI 有时产生不适合学生年级的建议。citeturn16search2 | 一上来讲 lockdep、memory ordering 或工业 Linux 细节，淹没正在学基本锁语义的人 | 每个学习单元有先修清单与 mastery level；提示只允许使用已解锁概念，工业细节放 Delta Card |
| **只修眼前 bug，不形成概念模型** | [EXTERNAL CodeHelp] 学生帮助请求大量聚焦眼前作业/错误，而不是深层概念；[EXTERNAL self-explanation] 自我解释有助于源码理解学习。citeturn16search5turn12search9 | 学会“让测试绿”，没有学会为何正确 | 修复前必须预测/解释；修复后要求写出不变量、失败路径和一个反例 |
| **让 LLM 单独充当 grader** | [EXTERNAL GPT feedback] 对真实 submission 的反馈会出现错误、幻觉甚至自相矛盾。citeturn11academia42turn11academia43 | 教材把正确实现判错或把错误实现说成正确，尤其会破坏作者对自己代码的认知 | 分数/通过状态由确定性 oracle 和明确 rubric 决定；LLM 只解释证据，模糊项交人工/标 `unknown` |

以上提供七类主要反模式，超过任务书“至少五条”的验收门槛。

**“引用锚定”应具体到什么程度**

[INFERRED] 对 MyOS2，仅写“根据源码”不够。一个可信解释至少应区分四层：

| 层 | 可接受例子 | 不接受例子 |
|---|---|---|
| Snapshot | `time @ <sha>` | “最新版 MyOS2” |
| Location | 文件 + symbol；需要时附行区间 | “在内存管理模块里” |
| Observation | “此条件分支调用 X 并设置 Y” | “它应该是为了……” |
| Interpretation | 明标 `[INFERRED]` 或 `[EXTERNAL]` | 把 Linux 常见设计混成 MyOS2 `[VERIFIED]` |

[EXTERNAL De-Hallucinator] 项目级 grounding 研究的核心结果正支持“把模型约束到实际项目 API/上下文”而不是依赖其预训练记忆。citeturn18search2

**“答案先验证再呈现”的最低门槛**

[INFERRED] 未来教学系统若准备给出一个具体修复或参考实现，应先满足：

`source retrieval PASS`  
`build PASS`  
`targeted tests PASS`  
`regression tests PASS / or explicit not_measurable`  
`explanation agrees with executed behavior PASS`  

若不能执行测试，则不得使用“正确答案”措辞，只能说“静态分析候选方案”。

[EXTERNAL CodeHalu] 这是因为代码的“看起来合理”与“执行满足需求”不可混同，execution-based verification 正是代码幻觉研究所强调的检测边界。citeturn18search0

**关于学习效果的关键警告**

[EXTERNAL PNAS] 最重要的证据不是“AI 能让当前题做得更快”，而是 Bastani 等人的受控研究显示：没有教育 guardrail 的 AI 可能提高有辅助时的练习表现，却损害撤去 AI 后的独立表现。citeturn16search3turn16search7

[INFERRED] 因而未来 MyOS2 教学评价的主要指标不能是“学生最终 patch 是否成功”，至少还要包含一次**无 AI 的解释或迁移任务**。例如完成 timer bug 练习后，再给一段不同路径的同步代码，让学习者独立判断竞态。这才测得到机制是否真的进入学习者的模型。

**证据索引**

[EXTERNAL] CodeAid，CHI 2024：700 人课堂、12 周、约 8,000 次使用，重点是非直接答案的教学型编程帮助。citeturn16search0turn16search8  
[EXTERNAL] CodeHelp：52 名学生、12 周、guardrails 与结构化提问。citeturn16search17turn16search5  
[EXTERNAL] Tutor CoPilot：900 tutor / 1,800 students 的预注册 RCT。citeturn16search2  
[EXTERNAL] Bastani et al., PNAS：无 guardrail 生成式 AI 的独立学习损害风险。citeturn16search3  
[EXTERNAL] CodeHalu：execution-based code hallucination benchmark。citeturn18search0  
[EXTERNAL] De-Hallucinator：项目 API grounding 对代码幻觉的缓解。citeturn18search2  
[EXTERNAL] Sharma et al.：sycophancy 与人类偏好训练。citeturn17search1  
[EXTERNAL] 自动编程评分系统综述、真实学生代码 GPT 反馈研究。citeturn11search14turn11academia42turn11academia43

## 自有代码库教材文件

目标路径：`agent-workspace/results/MYOS2-DR-009/03-own-kernel-specifics.md`

task_id: MYOS2-DR-009  
produced_by: GPT-5.5 Thinking  
date: 2026-08-31  
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"  
inputs_read: ["agent-workspace/conventions.md", "agent-workspace/repo-map.md", "agent-workspace/tasks/MYOS2-DR-009-teaching-blueprint-prestudy.md"]  
status: final  
open_questions: ["本任务没有审计 MyOS2 的自动测试覆盖率、设计文档完整度或具体 bug，因此未来教学系统能把多少结论自动分成 design_choice 与 bug_confirmed，取决于其他审计任务能提供多强的独立 oracle。"]

**核心问题**

[VERIFIED `agent-workspace/tasks/MYOS2-DR-009-teaching-blueprint-prestudy.md`] 自有代码库作为教材与 xv6/Pintos 最大的额外风险不是“代码小不小”，而是**学习者很容易把作者意图、当前实现、标准理论和正确实现四者混在一起**。任务书特别要求解决三个问题：代码自身可能有错；与标准实现的差异怎样变成素材而不是误导；学习者同时是作者时有什么认知盲区。fileciteturn3file0

**第一原则：教材事实不能等于作者意图**

[INFERRED] 对任何 MyOS2 机制，未来教材至少要维护四个彼此独立的对象：

| 对象 | 问的问题 | 证据 |
|---|---|---|
| `observed_implementation` | 当前 snapshot 实际做了什么？ | 源码、编译、trace、测试 |
| `intended_design` | 作者原本想让它做什么？ | 当前设计说明、接口契约、测试预期；作者回忆只能是补充证据 |
| `reference_model` | 教科书/标准/代表性工业系统通常如何定义这类机制？ | 教材、标准、论文、Linux/BSD 等外部资料 |
| `teaching_interpretation` | 哪个差异值得教学，怎样解释？ | 前三者对照后的明确推论 |

[INFERRED] 最危险的说法是：“这里这样写，所以设计就是这样。”源码只能证明 **observed implementation**。如果代码恰好有 bug，这句话就会把 bug 教成设计原则。

**判定“设计如此”还是“这是 bug”的协议**

[INFERRED] 建议未来教材只使用下面五种状态，不允许直接二分“正确/错误”：

| 标签 | 判定要求 | 教学措辞 |
|---|---|---|
| `design_choice` | 行为与明确设计契约一致，测试支持，并且差异可解释为架构/策略选择 | “MyOS2 选择 X；另一常见选择是 Y” |
| `intentional_simplification` | 有明确证据说明为了学习、规模或当前 scope 有意省略能力 | “这是刻意简化；它牺牲/推迟了……” |
| `bug_confirmed` | 行为违反明确契约/不变量，且有复现、测试或运行证据 | “当前 snapshot 有可复现缺陷；不要把它当设计” |
| `bug_suspected` | 静态证据显示高风险或与规范冲突，但尚无充分运行证据 | “疑似缺陷，尚未形成确定结论” |
| `unknown` | 意图、行为或 oracle 不足 | “证据不足，暂不教学化为结论” |

[VERIFIED `agent-workspace/conventions.md`] 这与仓库公约“不确定就写 open_questions、拿不到的数据标 not_measurable、不要编造，以及只能说所读快照中未见”的事实纪律一致。fileciteturn0file0

**错误代码本身可以成为教材，但必须明确标成错误**

[EXTERNAL erroneous-example research] 教育研究并不要求教材永远只出现正确例子。2025 年对 erroneous examples 的系统综述指出，把错误步骤与正确做法进行对照能够把注意力集中到关键区别上；但学习效果取决于错误是否被解释/突出、prompt 设计以及学习者先备知识。citeturn12search3

[EXTERNAL earlier empirical evidence] 更早的研究也发现，把错误 worked example 与正确例子并列可以帮助学习者掌握正确概念和程序。citeturn12search11

[INFERRED] 因而 MyOS2 中真正被确认的 bug 不必从教材中“藏掉”。更有教学价值的处理是做成 **Bug Contrast Case**：

`错误 snapshot 行为 → 学习者预测后果 → 最小复现 → 找被破坏的不变量 → 修复 → 回归验证 → 与标准机制对照`

关键是标题必须先告诉学习者“这是当前 snapshot 的已确认缺陷”，而不能让学生先吸收错误模型再在很久之后纠正。

**差异应从“偏离标准”改写为“设计空间”**

[INFERRED] 自有内核教材不应把 Linux 当成唯一正确答案。一个差异可能来自四种完全不同的原因：

`bug`、`simplification`、`architectural alternative`、`missing industrial requirement`

未来教学材料应先分类，再讨论影响。

例如，不应写：

> “MyOS2 没有像 Linux 那么复杂，所以这里是不完整的。”

而应写成结构化对照：

| 问题 | MyOS2 | 参考系统 | 教学问题 |
|---|---|---|---|
| 机制目标是否相同？ | 用源码/测试回答 | 用外部权威资料回答 | 哪个抽象是不变的？ |
| 状态放在哪里？ | 当前实现 | 代表性实现 | 状态位置改变了哪些耦合？ |
| 并发模型有什么前提？ | 当前实现可证明的前提 | SMP/NUMA/生产约束 | 如果放宽前提会发生什么？ |
| 省略了什么？ | 只写有证据的省略 | 工业机制 | 是优化、安全、可扩展性还是可维护性需求？ |
| 代价是什么？ | 可测则测，不可测标记 | 文献/标准 | 哪个 trade-off 最值得学生掌握？ |

[EXTERNAL MINIX] MINIX 是这种教学法的好例子：其微内核、用户态服务和驱动隔离代表明确的架构路线，而不是“比单体内核少写了几行代码”。这使它天然适合与其他 OS 架构比较。citeturn14search0turn14search12

**教材也应有 errata 思维**

[EXTERNAL OSTEP] OSTEP 官方维护公开的 “Flaws Found and Fixed” errata，把教材自身的错误持续记录和修正。citeturn14search32

[INFERRED] 自有代码库更需要同样的习惯，因为代码和教材都在变化。未来材料不应假设“章节发布后事实永远不变”，而应把每个结论绑定到 commit，并允许以下演化：

`claim @ snapshot A → bug discovered → corrected claim @ snapshot B → old lesson retained as historical bug case`

这样历史错误可以继续作为教学资产，却不会污染当前事实。

**作者同时是学习者：最大的风险是“熟悉感 ≠ 可解释性”**

[EXTERNAL expert blind spot] “expert blind spot” 研究表明，拥有更多学科知识的人可能更容易用自己的高级表征去判断学习者需要什么，并错误估计哪些前置步骤对初学者困难；经典研究在 48 名准教师中观察到高级数学背景与这类判断偏差相关。citeturn12search0

[INFERRED] MyOS2 的 Owner 还多了一层特殊性：不仅熟悉主题，而且亲手写过代码。因此“我看到函数名就知道它干什么”是教学上的危险信号，因为真实学习目标应当是**能从证据重建机制，而不是唤起作者当年的记忆**。

[EXTERNAL self-explanation] 源码理解研究显示，要求学习者主动产生 self-explanation 能促进代码理解学习。citeturn12search9

[INFERRED] 所以作者型学习者应强制使用“陌生人模式”：

| 约束 | 目的 |
|---|---|
| 看源码前先写预测 | 分离记忆与当前 snapshot 的事实 |
| 每个关键判断给代码/测试锚点 | 防止“我记得我当时……”成为证据 |
| 先解释给一个假想新贡献者听 | 暴露被作者自动跳过的前置知识 |
| 解释数据结构时禁止只说名称 | 迫使说明不变量和状态转换 |
| 先从测试/调用者反推契约，再读实现 | 避免实现细节定义了“应该是什么” |
| 过一段时间做无提示重建 | 检验是否形成可迁移模型而非短时熟悉 |

**代码评审提供另一种有用的去作者化机制**

[EXTERNAL Harvard CS161] Harvard 的 OS 课程材料把 code review 的用途列为执行标准、讨论替代设计、暴露 bug、建议优化以及让团队保持同步，并强调软件开发跨越空间和时间——包括“未来忘掉上下文的自己”。citeturn15search12

[EXTERNAL code-review bias research] 同时，研究也提醒 code review 本身并非无偏；认知偏差会影响反馈的产生和解读。citeturn12academia35

[INFERRED] 因而未来 MyOS2 学习流程可借用“review stance”，但不能简单规定“再让一个 LLM 看一遍”。真正有用的是让第二视角在**不知道作者意图的前提下**，只凭接口、源码、测试和规范解释代码，再与作者解释做差分。

推荐记录：

`author_explanation`  
`evidence_only_explanation`  
`difference`  
`resolved_by_test_or_spec`  
`remaining_unknown`

这比单纯让模型“review my code”更能暴露认知盲区。

**MyOS2 专用的教材事实卡**

[INFERRED] 未来每个核心机制可以配一张固定结构的 Fact Card：

```text
concept_id:
snapshot:
source_anchor:
observed_behavior:
stated_intent:
invariants:
executable_evidence:
reference_model:
difference_class:
  design_choice | intentional_simplification |
  bug_confirmed | bug_suspected | unknown
teaching_value:
industrial_delta:
open_questions:
```

其中 `observed_behavior` 只能来自 `[VERIFIED]`，`reference_model` 必须标 `[EXTERNAL]`，`teaching_value` 和差异解释原则上是 `[INFERRED]`。这种字段级分离可以防止 LLM 在自然语言里偷偷把三种证据混起来。

**把 bug 变成学习素材的安全门**

[INFERRED] 一个 MyOS2 bug 只有在以下条件满足后才适合进入正式课程：

| Gate | 要求 |
|---|---|
| Reproducible | 有最小复现或确定测试 |
| Snapshot-bound | 明确 bug 存在哪个 commit |
| Invariant-known | 能说明违反哪条契约/不变量 |
| Fix-verified | 修复通过目标测试和合理回归 |
| Contrast-ready | 能给出正确实现或标准机制作为对照 |
| Misconception-safe | 教材在学习者第一次接触错误代码时就明确指出其身份 |

若只有怀疑，没有可执行证据，则应进入 `open_questions`，而不是进入“找 bug”练习的标准答案。

**最终原则**

[INFERRED] 自有内核做教材的独特优势不在于“它一定比 xv6 更正确”，而在于学习者能够看到一套真实、连续、带历史选择和缺陷的系统。它的独特风险也正来自同一点。

因此未来材料应坚持：

`code is evidence, not authority`  
`author intent is context, not oracle`  
`standard implementation is comparison, not dogma`  
`tests/specs arbitrate correctness where available`  
`confirmed differences become teaching material`  
`unknown stays unknown`

[EXTERNAL erroneous-example + self-explanation + expert-blind-spot research] 这种做法同时利用了错误对照、自我解释的学习价值，并针对专家/作者视角可能造成的盲区设置外部证据和显式解释门。citeturn12search3turn12search9turn12search0

**证据索引**

[EXTERNAL] Dieterich et al.，erroneous examples 系统综述。citeturn12search3  
[EXTERNAL] Durkin & Rittle-Johnson，正确/错误 worked examples 的对照学习。citeturn12search11  
[EXTERNAL] Nathan & Petrosino，expert blind spot。citeturn12search0  
[EXTERNAL] Tamang et al.，source-code self-explanation 实证研究。citeturn12search9  
[EXTERNAL] Harvard CS161 code review 教学材料。citeturn15search12  
[EXTERNAL] OSTEP Errata。citeturn14search32

## 课程骨架文件

目标路径：`agent-workspace/results/MYOS2-DR-009/04-curriculum-skeleton.md`

task_id: MYOS2-DR-009  
produced_by: GPT-5.5 Thinking  
date: 2026-08-31  
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"  
inputs_read: ["agent-workspace/conventions.md", "agent-workspace/repo-map.md", "agent-workspace/tasks/MYOS2-DR-009-teaching-blueprint-prestudy.md"]  
status: final  
open_questions: ["下面的单元顺序是课程层面的初步映射，而不是 MyOS2 实际源码依赖图；未来应与 MYOS2-DR-003 依赖图以及子系统完整性产物交叉验证后再冻结精确先修 DAG。", "主流 OS 课程通常还覆盖 networking、security、virtualization/distributed-systems 等主题，而当前共享词汇表没有对应的顶级 MyOS2 子系统 ID；未来是否作为纯外部对照单元加入，应由 meta-agent 决定。"]

**外部课程骨架对照**

[EXTERNAL MIT 6.1810] MIT 的 xv6 课程以可执行内核为主线，典型序列涵盖 OS 组织与 syscall、page table、trap/page fault、中断、锁、多线程/调度、sleep/wakeup、文件系统，并进一步进入 mmap、网络和现代 OS 研究主题。citeturn19search10turn19search0

[EXTERNAL Stanford Pintos] Stanford Pintos 的四个经典项目为 Threads、User Programs、Virtual Memory、File Systems；Pintos 本身故意只简单实现线程、用户程序和 FS，并让学生补强这些区域、加入 VM。citeturn13search25turn13search1

[EXTERNAL Harvard OS/161] Harvard 经典 OS/161 课程作业则形成 Synchronization → System Calls and Processes → Virtual Memory → File Systems 的紧凑链条。citeturn15search1

[EXTERNAL OSTEP] OSTEP 使用更抽象的三大块：Virtualization、Concurrency、Persistence，并把 process/scheduling/memory、threads/locks、storage/file systems 纳入其中。citeturn14search4

| 教学主题 | MIT xv6 | Pintos | OS/161 | OSTEP | 对 MyOS2 的启示 |
|---|---|---|---|---|---|
| 环境、架构、内核启动 | 显式阅读 entry/main 等代码并学习 C/GDB；trap/硬件边界贯穿课程。citeturn19search10 | 项目前置环境和内核框架 | ASST0 专门熟悉 OS/161、sys161、GDB、Git。citeturn13search7 | 通常作为概念/测量背景 | [INFERRED] MyOS2 应保留独立的“从 UEFI 到内核到首个用户态”单元 |
| syscall / user-kernel boundary | 核心早期单元和 lab。citeturn13search20 | User Programs project | System Calls and Processes | CPU virtualization / mechanisms | [INFERRED] 作为理解内核保护边界和后续进程机制的第一个纵向 trace |
| process / scheduling | threads、context switching、sleep/wakeup、locking | Threads + User Programs | Processes + scheduling | CPU virtualization/scheduling | [INFERRED] 用 `sched.*` 作主要源码底本 |
| synchronization / concurrency | locks、parallelism、multicore | Threads project | 第一个核心实现 assignment 就是 synchronization | Concurrency 主块 | [INFERRED] `lock.*` 与 `kactive.*` 应分“同步原语”和“异步执行机制”两层 |
| virtual memory | page tables、faults、COW、mmap | Virtual Memory project | Virtual Memory assignment | Virtualization / memory | [INFERRED] `mm.*` 是最完整的独立主单元之一 |
| time / timers | 与 interrupts、sleep、scheduler 等交织 | 多作为线程/调度支撑 | 多嵌入 scheduler/device 语境 | 分散在 scheduling/I/O | [INFERRED] MyOS2 具有单独 `time.*` 词汇，可把“时间是内核基础设施”提升成一个显式单元 |
| file system / storage | FS、logging/crash recovery | File Systems project | File Systems assignment | Persistence 主块 | [INFERRED] `fs.* + block + drivers.*` 形成从 VFS 到硬件的一条纵向课程链 |
| drivers / interrupts | 有 device-driver lecture，网络作为后续扩展 | 框架中使用设备 | 受模拟硬件支持约束 | I/O / devices | [INFERRED] MyOS2 可用 ATA/PCI/RTC/char driver 展示“抽象层最终落到硬件”的闭环 |
| modern extensions | networking、RCU、安全攻击、VM 等 | 项目范围较固定 | 各校可扩展 | security 等在新版材料中出现 | [INFERRED] MyOS2 尚无共享 vocabulary 对应项时，宜先作为外部“Delta/Gap”模块，不伪造源码映射 |

**MyOS2 初步课程单元**

[VERIFIED `agent-workspace/repo-map.md` + conventions vocabulary] 下列映射只依据工作区提供的子系统名称和仓库地图，不代表已逐源码证明真实调用依赖。fileciteturn0file0 fileciteturn1file0

[INFERRED] 推荐先形成八个课程单元：

| 单元 | 学习问题 | 主要 vocabulary |
|---|---|---|
| Foundations & Observability | 如何构建“我能观察并解释这个内核”的最低工具箱？ | `printk`, `debug`, `klib`, `lib.*` |
| Architecture & Boot | CPU 从固件、架构入口到初始化和首个用户态经历了什么？ | `arch.*`, `boot.uefi`, `cpu`, `init`, `mm.early`, `user.initramfs` |
| Kernel Boundary | 用户态如何进入内核；异常/系统调用/架构边界如何连接？ | `entry`, 部分 `arch.*`, `user.initramfs` |
| Tasks & Isolation | 内核怎样表示、创建、执行和隔离活动实体？ | `sched.task`, `sched.forkexec`, `namespace`, `ipc.signal` |
| Scheduling, Synchronization & Async | 多个活动实体怎样共享 CPU/数据并等待事件？ | `sched.runqueue`, `sched.scheduler`, `sched.misc`, `lock.*`, `kactive.*` |
| Memory | 从启动内存到物理页、内核分配、虚拟映射和 fault 怎样组成一条链？ | `mm.*` |
| Time | 硬件时钟怎样变成内核时间、定时事件和校时？ | `time.*`, `drivers.rtc` |
| Storage, VFS & Devices | 从 VFS 操作怎样穿过文件系统、块层、总线和驱动抵达设备？ | `fs.*`, `block`, `device`, `drivers.*` |

[INFERRED] 这个骨架故意不是“按目录逐章讲”。例如 `drivers.rtc` 同时属于设备栈和时间机制；`lock.*` 应在 scheduler 周边第一次出现，但之后会成为 MM、FS、timer 等机制的横切先修。未来依赖图可以把这种交叉关系变成正式 DAG。

**共享词汇表全量映射**

[VERIFIED `agent-workspace/conventions.md`] 共享词汇表一共给出 53 个本任务需要覆盖的子系统 ID。下表逐项出现一次；“课程映射”全部为 `[INFERRED]`，不声称这些目录已被逐源码验证具备对应教学成熟度。fileciteturn0file0

| vocabulary ID | 初步课程单元 | 建议教学角色 | 映射状态 |
|---|---|---|---|
| `arch.x86_64` | Architecture & Boot | 主架构；启动、特权级、异常/中断等硬件边界的实现背景 | [INFERRED] |
| `arch.aarch64` | Architecture & Boot / Portability | 在掌握主架构后用于区分“OS 原理”与“x86 特有实现” | [INFERRED] |
| `boot.uefi` | Architecture & Boot | 固件到内核映像的边界；解释启动链和 boot contract | [INFERRED] |
| `user.initramfs` | Architecture & Boot / Kernel Boundary | 内核完成初始化后怎样进入最小用户态；用户/内核接口的端点 | [INFERRED] |
| `entry` | Kernel Boundary | syscall/异常入口等从架构事件进入通用内核路径的锚点 | [INFERRED] |
| `cpu` | Architecture & Boot | CPU/每 CPU 初始化、SMP 概念的仓库级入口 | [INFERRED] |
| `init` | Architecture & Boot | 内核各子系统初始化顺序和生命周期的总览入口 | [INFERRED] |
| `namespace` | Tasks & Isolation | 隔离/命名视图的高级主题；适合与 process identity 对照 | [INFERRED] |
| `printk` | Foundations & Observability | 最初级观测路径；引出“调试输出不是正确性证明” | [INFERRED] |
| `debug` | Foundations & Observability | 调试方法、断言/诊断设施、GDB/trace 的承接点 | [INFERRED] |
| `klib` | Foundations & Observability | 内核不能无条件依赖普通用户态库这一工程边界 | [INFERRED] |
| `mm.early` | Architecture & Boot / Memory | 正式 allocator 可用前怎样管理启动期内存 | [INFERRED] |
| `mm.page_alloc` | Memory | 物理页是 VM、kernel allocation 等机制的资源基础 | [INFERRED] |
| `mm.kmalloc` | Memory | 页级资源怎样形成更一般的内核对象分配接口 | [INFERRED] |
| `mm.vm_map` | Memory | 虚拟地址、映射和 address-space abstraction 的主要锚点 | [INFERRED] |
| `mm.fault` | Memory | page fault 把硬件异常、VM policy 和任务执行串起来 | [INFERRED] |
| `mm.highmem` | Memory / Advanced | 地址空间受限或特殊映射等高级内存管理问题 | [INFERRED] |
| `mm.misc` | Memory | 难以归入单一 allocator/VM 子模块的整合内容；正式课程应再细分 | [INFERRED] |
| `sched.task` | Tasks & Isolation | task 表示、生命周期、状态机的核心 | [INFERRED] |
| `sched.forkexec` | Tasks & Isolation | 创建新执行上下文、装载程序、继承与替换语义 | [INFERRED] |
| `sched.runqueue` | Scheduling, Synchronization & Async | runnable 状态怎样转成调度器可选择的数据结构 | [INFERRED] |
| `sched.scheduler` | Scheduling, Synchronization & Async | 调度策略、选择、切换的主机制 | [INFERRED] |
| `sched.misc` | Scheduling, Synchronization & Async | scheduler 周边辅助路径；未来应按真实调用关系重新归类 | [INFERRED] |
| `lock.atomic` | Scheduling, Synchronization & Async | 进入并发前的最小原子操作与 memory-ordering 讨论入口 | [INFERRED] |
| `lock.spinlock` | Scheduling, Synchronization & Async | 短临界区、SMP、不可睡眠上下文的核心同步例子 | [INFERRED] |
| `lock.semaphore` | Scheduling, Synchronization & Async | 可阻塞同步、资源计数与 scheduler 的交互 | [INFERRED] |
| `lock.futex` | Scheduling, Synchronization & Async / Kernel Boundary | 用户态快速路径与内核等待路径如何协作 | [INFERRED] |
| `lock.lockdep` | Scheduling, Synchronization & Async / Observability | 锁正确性、依赖、死锁诊断；从“使用锁”跨到“验证锁” | [INFERRED] |
| `ipc.signal` | Tasks & Isolation | 异步进程/任务通知及用户内核状态转换 | [INFERRED] |
| `kactive.softirq` | Scheduling, Synchronization & Async | 中断顶半部与延迟工作之间的执行上下文差异 | [INFERRED] |
| `kactive.workqueue` | Scheduling, Synchronization & Async | 把工作延迟到可调度上下文的常见异步模式 | [INFERRED] |
| `kactive.swait` | Scheduling, Synchronization & Async | 等待状态、唤醒条件和 scheduler 之间的桥 | [INFERRED] |
| `kactive.completion` | Scheduling, Synchronization & Async | 一次性/阶段性事件完成同步；适合作为 higher-level primitive | [INFERRED] |
| `time.systick` | Time | 周期性 tick 与 scheduler/timer 之间的传统连接点 | [INFERRED] |
| `time.ktime` | Time | 内核内部怎样表示和运算时间 | [INFERRED] |
| `time.clocksource` | Time | 从硬件计数器抽象出可比较时间源 | [INFERRED] |
| `time.timekeeping` | Time | 不同 clock source 如何进入系统时间维护 | [INFERRED] |
| `time.timer` | Time | “未来某时刻执行动作”的事件机制；与 wait/scheduler 交叉 | [INFERRED] |
| `time.adjtime` | Time | 时钟校正、误差与“计时器 ≠ 绝对真时间”的高级讨论 | [INFERRED] |
| `fs.vfs` | Storage, VFS & Devices | 对文件/目录/挂载等统一抽象的第一层 | [INFERRED] |
| `fs.fat` | Storage, VFS & Devices | 用一个具体文件系统理解 VFS 操作如何落到 on-disk 结构 | [INFERRED] |
| `block` | Storage, VFS & Devices | FS 与具体存储驱动之间的块 I/O 抽象 | [INFERRED] |
| `device` | Storage, VFS & Devices | 设备模型/注册的课程位置；实际成熟度需未来源码审计 | [INFERRED] |
| `drivers.ata` | Storage, VFS & Devices | 块 I/O 最终怎样变成真实存储设备命令 | [INFERRED] |
| `drivers.base` | Storage, VFS & Devices | 驱动公共框架、注册/生命周期的承载点 | [INFERRED] |
| `drivers.char` | Storage, VFS & Devices | 与 block device 对照的字符设备 I/O 路径 | [INFERRED] |
| `drivers.pci` | Architecture & Boot / Devices | 总线枚举、硬件发现与资源配置的桥梁 | [INFERRED] |
| `drivers.rtc` | Time / Devices | 硬件实时时钟怎样进入内核时间子系统 | [INFERRED] |
| `lib.digit` | Foundations & Observability | 底层数值/转换工具；适合作为“基础库依赖”而非独立理论单元 | [INFERRED] |
| `lib.idr` | Foundations / Kernel Data Structures | ID→对象映射类内核数据结构，可和 task/device 标识管理结合 | [INFERRED] |
| `lib.list` | Foundations / Kernel Data Structures | intrusive list 等通用结构如何横切 task、MM、FS 等机制 | [INFERRED] |
| `lib.printf` | Foundations & Observability | 格式化输出与 `printk` 路径的底层支撑 | [INFERRED] |
| `lib.string` | Foundations | freestanding kernel 中基本内存/字符串操作的支撑层 | [INFERRED] |

**覆盖校验**

[VERIFIED vocabulary enumeration] 上表覆盖：

`arch.x86_64`, `arch.aarch64`, `boot.uefi`, `user.initramfs`, `entry`, `cpu`, `init`, `namespace`, `printk`, `debug`, `klib`,  
全部 7 个 `mm.*`，  
全部 5 个 `sched.*`，  
全部 5 个 `lock.*` 与 `ipc.signal`，  
全部 4 个 `kactive.*`，  
全部 6 个 `time.*`，  
`fs.vfs`, `fs.fat`, `block`, `device`，  
全部 5 个 `drivers.*`，  
全部 5 个 `lib.*`。  

合计 53 个共享 vocabulary ID，与 `conventions.md` 给出的清单一致。fileciteturn0file0

**建议的先修关系**

[INFERRED] 在不冒充源码调用图的前提下，可以先给未来 meta-agent 一个课程层面的 DAG 草案：

```text
Foundations & Observability
        |
        v
Architecture & Boot
   |             |
   v             v
Kernel Boundary  Memory
   |
   v
Tasks & Isolation
   |
   v
Scheduling, Synchronization & Async
   |              \
   v               v
Time          Storage, VFS & Devices
   \               /
    \             /
     v           v
       Integration / Advanced Contrasts
```

[INFERRED] 这里表达的是“理解顺序”，不是源码链接依赖。尤其 Memory、Scheduling、Time、FS 在真实内核里会形成循环依赖和横切关系；未来应由源码依赖图而不是教学直觉决定哪些边实际成立。

**每个课程单元的固定作业梯度**

[INFERRED] 结合前一交付物提炼的教学内核先例，每个 MyOS2 单元可预留同一结构：

| 难度 | 作业模板 |
|---|---|
| Read | 找出入口、核心状态、状态转换和返回/唤醒路径；画一个最小 trace |
| Explain | 不看源码重新解释机制，并指出至少一个不变量 |
| Modify | 改一个局部策略/观测点，预测影响后运行验证 |
| Debug | 给一个明确标记的真实或植入 bug，要求最小复现和 invariant diagnosis |
| Extend | 添加一个受限能力，先交设计，再实现和测试 |
| Compare | 与 xv6/Linux/BSD/论文中的代表设计对照，写清“共同原理、差异原因、后果” |

[EXTERNAL xv6/Pintos/OS161] 这一梯度综合了 xv6 的源码阅读和扩展 lab、Pintos 的 skeleton strengthening 与 design document、以及 OS/161 从 code reading 到同步/进程/VM/FS 的递进。citeturn13search0turn13search21turn13search11

**主流课程有、当前 vocabulary 没有直接 ID 的主题**

[EXTERNAL MIT/Harvard] 现代教学课程经常继续进入 networking、security/isolation、virtualization、crash recovery、scalability 等内容；MIT 6.S081/6.1810 的课程日程包含 networking、Meltdown、RCU 等后段主题，Harvard 较新的 Chickadee 课程也把 networking、security/isolation、kernel observability 等列为项目方向。citeturn19search10turn15search8

[VERIFIED `agent-workspace/conventions.md`] 当前 MyOS2 共享词汇表没有 `net.*`、`security.*`、`virt.*` 等对应顶级 ID。这个结论只针对本工作区词汇表，不等价于断言整个仓库绝对不存在任何相关代码。fileciteturn0file0

[INFERRED] 因此未来课程最好把这些主题暂时作为 **External Delta Units**：先用现有 MyOS2 机制建立基础，再问“一个工业系统为了网络、安全、虚拟化或崩溃一致性还需要补哪些不变量和模块？”等未来源码真的具备相应能力后，再升级为 MyOS2 主线单元。

**未来 meta-agent 可直接消费的骨架**

[INFERRED] 在不设计 agent 组本身的前提下，本预研给出的最小课程序列可压缩为：

`observe → boot → cross boundary → task → schedule/synchronize → manage memory → manage time → persist/I/O → compare/extend`

其中每一步都采用：

`theory → MyOS2 anchor → prediction → trace → exercise → executable evidence → industrial delta`

这一结构同时保留 MIT xv6 的“小而代表性”、Pintos/OS161 的渐进式动手、OSTEP 的概念层次，以及自有代码库必须额外承担的事实/bug 区分责任。citeturn19search0turn13search6turn15search1turn14search4

**证据索引**

[EXTERNAL] MIT 6.1810 / xv6 课程 Overview、Schedule、Labs。citeturn19search0turn19search10turn13search20  
[EXTERNAL] Stanford Pintos / Berkeley CS162 Projects。citeturn13search1turn13search6turn13search21  
[EXTERNAL] Harvard CS161 OS/161 Assignments。citeturn15search1turn15search15  
[EXTERNAL] OSTEP 官方课程材料。citeturn14search4turn14search18  
[VERIFIED] MyOS2 共享词汇表和 repo-map。fileciteturn0file0 fileciteturn1file0