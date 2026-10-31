---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-VFS-BASELINE-05
record_type: documentation_erratum_disposition
produced_by: "Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable"
date: "2026-10-02"
base_snapshot: "kernel=time a039d9803ade；被审执行头 de7c96ede546；MM 结果冻结 eb75b3100601；主线 agent/MYOS2-LEAD-002 0d066801371d（开工时）。均为短 SHA"
review_ref: "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-MM-BASELINE-04-review.md（agent/MYOS2-LEAD-002）"
review_record_id: CORE-MM-BASELINE-04-REVIEW-001
returned_items: [MR-01, MR-02]
items_kind: documentation_erratum
status: appended_correction_pending_lead_review
kernel_change_made: false
candidate_A_adopted: false
candidate_B_adopted: false
od_2_key_floor_adopted: false
owner_decisions_requested_now: []
supersedes: "仅限下方 supersedes_scope 所列旧文字（de7c96ede546 中 mm-baseline-04 的原件不改动）；其余旧文字与旧证据保持原状"
inputs_read: "审查回执全文；de7c96ede546 中 mm-baseline-04 的 MANIFEST.md、memory-baseline.md、integration-03-disposition.md §4、evidence.md、facts-and-dependencies.yaml 的相关行（逐行定位见 supersedes_scope）；scheduler-integration-03 的 A-R6、B-R1…B-R6 定义"
open_questions: "MC-01 的“不合并”是否在全树成立仍为 needs_evidence（CORE-MM-BASELINE-04::GAP-M08）"
supersedes_scope:
  - {item: MR-01, path: integration-03-disposition.md, line: 162, fragment: "按 A-R6 的并列规则，它们插在队首之前，因此仍会先运行一次", action: superseded}
  - {item: MR-02, path: memory-baseline.md, line: 26, fragment: "用户内存只分配不回收", action: narrowed}
  - {item: MR-02, path: memory-baseline.md, line: 30, fragment: "但释放块永不合并（MC-01）", action: narrowed}
  - {item: MR-02, path: memory-baseline.md, line: 37, fragment: "用户内存只增不减", action: narrowed}
  - {item: MR-02, path: memory-baseline.md, line: 37, fragment: "合在一起，长时间运行会先碎片化，再耗尽。", action: withdrawn}
  - {item: MR-02, path: memory-baseline.md, line: 38, fragment: "所以出错任务大概率只是一直占用时间片，不一定拖住整个系统", action: withdrawn}
  - {item: MR-02, path: memory-baseline.md, line: 46, fragment: "实际上 PageBuddy 恒假，合并分支从不执行，这个后果在本 pin 不可达。", action: narrowed}
  - {item: MR-02, path: memory-baseline.md, line: 223, fragment: "page_type 从未被置成类型基值，PageBuddy 恒假（MC-01）", action: narrowed}
  - {item: MR-02, path: memory-baseline.md, line: 268, fragment: "page_type 从不被设置成该值，合并分支永不进入", action: narrowed}
  - {item: MR-02, path: memory-baseline.md, line: 318, fragment: "释放出的块永远不会与伙伴合并", action: narrowed}
  - {item: MR-02, path: memory-baseline.md, line: 318, fragment: "在本 pin 不可达", action: narrowed}
  - {item: MR-02, path: memory-baseline.md, line: 343, fragment: "本 pin 不可达 | PageBuddy 恒假", action: narrowed}
  - {item: MR-02, path: MANIFEST.md, line: 43, fragment: "用户内存只增不减", action: narrowed}
  - {item: MR-02, path: evidence.md, line: 76, fragment: "MC-01 与 MC-08 的有害后果在本 pin 不可达", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 87, fragment: "但释放块永不合并（MC-01）", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 327, fragment: "page_type 从不被设置成该基值", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 327, fragment: "所以合并分支永不进入（MC-01）", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 357, fragment: "且因 PageBuddy 恒假而不可达（MC-01）", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 360, fragment: "释放块永不合并", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 1304, fragment: "page_type 从未被置成类型基值，PageBuddy 恒假（MC-01）", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 1746, fragment: "page_type 从不被设置成该值，合并分支永不进入", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 1828, fragment: "释放出的块永远不会与伙伴合并", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 1828, fragment: "在本 pin 不可达", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 1834, fragment: "PageBuddy 恒假 → 返回 NULL", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 1836, fragment: "空闲块只会被拆小、不会再合大：长时间运行后高阶分配", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 1855, fragment: "审查代理指出 PageBuddy 恒假使该后果不可达", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 1914, fragment: "（MC-01 会加剧）", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 1930, fragment: "用户数据页与 COW 旧页只增不减", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 1930, fragment: "用户内存基本只分配不回收", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 2136, fragment: "用户内存、页表、VMA 只增不减", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 2234, fragment: "出错进程不会被终止，持续占用 CPU 时间片", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 2302, fragment: "verdict: 本 pin 不可达", action: narrowed}
  - {item: MR-02, path: facts-and-dependencies.yaml, line: 2303, fragment: "PageBuddy 恒假，合并循环体从不执行", action: narrowed}
reviewed_retained:
  - {path: memory-baseline.md, line: 322, fragment: "经 put_page/folio_put 释放的页永远不会回到 buddy。", action: retained_path_local}
  - {path: memory-baseline.md, line: 264, fragment: "扩栈分支不可达", action: retained_function_local}
  - {path: memory-baseline.md, line: 334, fragment: "缺页处理中的栈增长分支不可达", action: retained_function_local}
  - {path: facts-and-dependencies.yaml, line: 1718, fragment: "扩栈分支不可达", action: retained_function_local}
  - {path: facts-and-dependencies.yaml, line: 2205, fragment: "缺页处理中的栈增长分支不可达", action: retained_function_local}
  - {path: facts-and-dependencies.yaml, line: 1070, fragment: "mmap 路径下不可达", action: retained_function_local}
  - {path: facts-and-dependencies.yaml, line: 1839, fragment: "Q34 只检索了 mm 与 arch/x86_64/mm 目录", action: retained_as_premise}
---

# MM-BASELINE-04 两项文档问题的处置（U00）

本文件只改说法，不改结论所依据的源码证据。它追加在新目录中，de7c96ede546 里 mm-baseline-04 的原件一字不改；消费那些原件时，凡是落在上面 `supersedes_scope` 里的旧句，以本文的写法为准。`supersedes_scope` 中每一处的路径、行号和原文片段，都由 `scripts/uv_facts_check.py` 回到 de7c96ede546 对象逐条核对过（结果见 `records/facts_check.json`）。

两项都是 AI 报告的质量问题（documentation_erratum），不是内核缺陷。登记见 `deferred-findings.yaml` 的 errata。A、B 和键下限（OD-2）都没有采用，下面的表格也只是按规格做的算术，不是运行结果。

## 1. MR-01：键下限并不让所有被唤醒任务都排到队首

**被取代的原句**（integration-03-disposition.md 第 162 行）：“按 A-R6 的并列规则，它们插在队首之前，因此仍会先运行一次。”这句话漏掉了“自身键本来就比队首大”的情形。

**规则**（只是复述，没有新增）：

- 键下限：当前有非 idle 队首时，入队键取 max(自身键, 队首非 idle 任务的键)；没有非 idle 队首时，保持自身键。
- A-R6：插在第一个键不小于它的节点之前。
- B-K1、B-K2：队列不含 current；idle 按身份留在尾部，不参与按键排序。

**值表**（VT-MR01，`scripts/uv_facts_check.py` 按上述规则逐步重算，7 行全部一致）：

| 行 | 情形 | 入队前（不含 idle） | 入队任务：原键 → 钳制后 | 入队后 | 队首 |
|---|---|---|---|---|---|
| T1 | 自身键大于队首（35 对 10） | N10 | W：35 → 35 | N10, W35, idle | N（W 不在队首） |
| T2 | 自身键小于队首（5 对 10） | N10 | W：5 → 10 | W10, N10, idle | W |
| T3 | 自身键等于队首（10 对 10） | N10 | W：10 → 10 | W10, N10, idle | W |
| T4 | 新任务原键 0，有非 idle 队首 10 | N10 | W：0 → 10 | W10, N10, idle | W |
| T5 | 没有非 idle 队首 | （空） | W：35 → 35（不钳制） | W35, idle | W |
| T6 | 下次选取前又有插入 | N10 | W1：5 → 10；随后 W2：7 → 10 | W2, W1, N10, idle | W2（W1 已不在队首） |
| T7 | 自身键介于队中两键之间 | N10, M20 | W：15 → 15 | N10, W15, M20, idle | N |

**改写后的说法：**

- 设下限时，自身键**不大于**当时队首非 idle 键的被唤醒任务或新任务，钳制后键等于队首键，按 A-R6 插在原队首之前，成为新队首（T2、T3、T4）。
- 只有在下次选取之前没有别的插入落在它前面（T6 表明后到的同键任务会排到它前面），而且选取条件（B-K3：need_resched、idle、时间片、非 RUNNING、count>0）没有变化时，下一次选取才会选中它。原文中的“仍会先运行一次”只在这一条件下成立。
- 自身键**大于**队首键的任务保持自身键，按有序插入落在队首之后（T1、T7），不保证先运行。
- 没有非 idle 队首时，任务不钳制，以自身键插在 idle 之前，并成为唯一的非 idle 节点（T5）。
- 原段落后半句（“键值从此改变……新任务和长睡任务这类低键任务同样受影响”）不在取代范围内，继续有效。
- integration-03-disposition.md 第 157 行“键 0（或钳制后等于队首键的键）头插的结果恰好等于有序插入”与本表一致（T3、T4），不需要改。

这些都是对未实现规格的算术推导，标 NOT_RUN。OD-2 仍是待以后处理的政策项（CORE-SCHED-INTEGRATION-03::OD-2），本文不替它做选择。

## 2. MR-02：把局部证据的总括改回带前提的说法

**问题所在：** MC-01、RF-01、LB-08 等处写了“PageBuddy 恒假”“永不合并”“本 pin 不可达”，但同一个包的 GAP-M08 仍然开放：Q34 只检索了 mm 与 arch/x86_64/mm，全树其他写入者、别名和共享存储都没有排除。另有几处跨路径的总括：“用户内存只增不减”“先碎片化再耗尽”“大概率只占时间片”。

**收窄依据（全部来自已交资料，不新增 page_type 专项检索）：**

- 初值：mem_map 分配后清零，`__init_single_page` 中 `page_mapcount_reset(page)` 一行被注释（审查回执 K09；MM-04 的 A-PA18）。
- 判定：PageBuddy 要求 `page_type` 的类型基值位（审查回执 K08；MM-04 的 A-PA17）。
- 已查写入：Q34 范围内只有按位清置 PG_buddy/PG_table 与 `_mapcount` 自增（MM-04 facts 第 327、1839 行）。
- 未排除：mm 与 arch/x86_64/mm 以外的写入者、别名、共享存储，以及运行中字段的演变（GAP-M08）。

**逐处改写：**

| 旧说法（位置见 supersedes_scope） | 改写为 | 状态 |
|---|---|---|
| “PageBuddy 恒假”“page_type 从不/从未被设置成类型基值”（memory-baseline.md 第 223、268 行；facts 第 327、1304、1746、1834 行） | 在上述初值与 Q34 已查写入范围内，没有看到把 page_type 置为类型基值的写入，因此 PageBuddy 在这一前提下判为假。全树写入者未排除。 | needs_evidence |
| “释放块永不合并”“释放出的块永远不会与伙伴合并”“合并分支永不进入”（memory-baseline.md 第 30、318 行；facts 第 87、327、360、1828 行） | 在同一前提下，释放块不进入合并分支。这是条件推导，不是全局结论；也不反过来宣布合并正常。 | needs_evidence |
| MC-01 后半句“该缺陷……在本 pin 不可达”，RF-01 的“本 pin 不可达”（memory-baseline.md 第 46、318、343 行；facts 第 357、1828、1855、2302、2303 行；evidence.md 第 76 行） | 合并循环缺少“把伙伴从原链表摘下”的步骤，这一局部疑点保留。RF-01 只否定初稿“已证明全局重复分配”的说法；在已查范围内没有看到能进入该循环的路径，但有限检索不能证明全局不可达，可达性 needs_evidence。evidence.md 第 76 行的 MC-08 部分改为“在已查范围内没有找到受影响的使用者”。 | MC-01 局部疑点：static_concern；可达性：needs_evidence |
| facts 第 1836 行“空闲块只会被拆小、不会再合大：长时间运行后高阶分配……可能在总空闲足够时失败”；第 1914 行“（MC-01 会加剧）” | 若上述前提成立，释放块不合大，高阶分配可能在总空闲足够时失败；会不会发生、何时发生都没有测量。第 1914 行改为“若 MC-01 的前提成立，会加剧”。 | 条件推导 |
| “用户内存只分配不回收”“只增不减”（memory-baseline.md 第 26、37 行；MANIFEST.md 第 43 行；facts 第 1930、2136 行） | 在所读路径中有这些回收断点：经 put_page/folio_put 降到 0 的页在此路径上不回 buddy（MC-05、LB-01）；munmap 只摘 VMA，不撤销页表（MC-16、LB-02）；__mmput 为空，退出与 exec 换下的地址空间不拆除（MC-16、LB-03）；释放块在上述前提下不合并（MC-01）。未读路径上是否另有归还，没有排除。 | 各断点原状态不变 |
| “合在一起，长时间运行会先碎片化，再耗尽”（memory-baseline.md 第 37 行） | 撤回。几个断点出现在不同路径上，先后顺序和耗尽时间都没有测量，不给确定时序。 | 撤回 |
| “出错任务大概率只是一直占用时间片，不一定拖住整个系统”（memory-baseline.md 第 38 行）；“持续占用 CPU 时间片”（facts 第 2234 行） | 撤回概率判断。缺页错误不会让进程终止（没有信号路径，LB-05）；出错任务能否被换下、会不会拖住系统，取决于中断出口时 in_atomic() 的实际值和调度行为（DR-01、GAP-M05），没有测量。 | 撤回概率，局部疑点保留 |

**复查后保留、不在取代范围内的句子**（见 front matter 的 `reviewed_retained`）：

- MC-05 的“经 put_page/folio_put 释放的页永远不会回到 buddy”：说的是这一条路径本身（__folio_put 为空），不是全部用户内存，范围本来就对。
- LB-04、MC-22 的“扩栈分支不可达”，RF-04 的“mmap 路径下不可达”：依据是一个已通读函数的局部控制流，或已锚定的赋值，不依赖全树写入者，保留原话。
- facts 第 1839 行写明了 Q34 只覆盖 mm 与 arch/x86_64/mm。这正是本次收窄的前提，保留。

## 3. 不变的部分

- MM-04 的 21 个代表能力、39 条依赖、80 条锚点、43 个检索和 M00 值表都不受影响（审查回执 §3 已说明）。
- 20 个 MC、6 个 DR、9 个 LB、RF-01…RF-05、GAP 和 NV 保持原编号与原类别。状态变化只有 `deferred-findings.yaml` 的 status_changes 中列出的几项。
- 本文不运行原函数，不重建调度或内存测试，不改用户代码，也不改内核。

## 4. 与延期记录的关系

- `CORE-MM-BASELINE-04-REVIEW-001::MR-01`、`::MR-02` 登记为 documentation_erratum，状态 superseded_interpretation，与内核缺陷分开。
- `CORE-MM-BASELINE-04::MC-01`、`::RF-01`、`::LB-08` 的状态变化，以及 MC-05、MC-16、MC-23 的措辞收窄，见 `deferred-findings.yaml` 的 status_changes。
- A、B、OD-1、OD-2 只引用原延期项，不再提出选择。
