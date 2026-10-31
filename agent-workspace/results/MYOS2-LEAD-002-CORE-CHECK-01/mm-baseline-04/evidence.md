---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-MM-BASELINE-04
record_type: evidence_log
produced_by: "Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable"
date: 2026-10-02
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；lead=agent/MYOS2-LEAD-002 c2176ad4da02；execution base ee9e6a738224"
inputs_read: "见 facts-and-dependencies.yaml 的 inputs_read"
status: final
open_questions: "见 memory-baseline.md §0 与 YAML gaps"
---

# CORE-MM-BASELINE-04 证据记录

本文记录本批实际执行的命令、检查结果、独立审查过程和提交回读。所有命令都只读 git 对象，或用 Python 处理文本、YAML、JSON；没有编译，也没有运行 C/ASM、cmake、内核、QEMU 或仓库原有脚本。Python 运行时设置了 `PYTHONDONTWRITEBYTECODE=1`（原因见 §8）。

## 1. 开工绑定（`scripts/mm_scope.py start` → `records/scope_start.json`）

| 项 | 结果 |
|---|---|
| 主线对象 | 远端 `agent/MYOS2-LEAD-002` = c2176ad4da02（relation: equal）；17 号任务书与 INTEGRATION-03 回执的关键头字段全部与预期一致（任务书 14 项、回执 7 项） |
| 任务书来源 | raw URL 读取的 17-memory-baseline-contract.md 与 git 对象逐字节一致（HTTP 200，13678 字节） |
| 回执 | `CORE-SCHED-INTEGRATION-03-REVIEW-001`：被审头 ee9e6a738224、结果 b7fa83583e35、RETURN、returned_items = IR-01/IR-02、next_followup = CORE-MM-BASELINE-04 |
| 执行分支 | `claude/dazzling-cori-q0dnyt` 头 = ee9e6a738224；包含 ee9e；开工时相对 ee9e 只有前缀内未跟踪新增 |
| 固定对象 | 远端 time = a039d9803ade，master = de3bb1df906a；master 不含 ee9e（PR17 未合并） |
| PR17 | 开工时经 GitHub 读取：open、draft、未合并，head = 执行分支 |

## 2. 读取范围

- **主线：** 17 号任务书全文；INTEGRATION-03 审查回执全文。
- **ee9e 原件：** scheduler-integration-03 的 MANIFEST.md；facts-and-regressions.yaml 的 `candidate_changes`、`owner_decisions`、`future_regressions`、`gaps`；integration.md §2 C3 与 §4–§7；`records/d01_index.json` 的 definitions。
- **master 旧输入：** 仅 ID 与节点名，见 memory-baseline.md §1。
- **源码：** 文件清单见 YAML 的 `inputs_read`。源码先用 `git archive a039d9803ade mykernel` 解到会话 scratch 中的新目录，只供阅读；引文核对一律回到 git 对象（`git show a039d9803ade:<path>`）。

## 3. 方法与机械核对（`scripts/mm_facts_check.py`）

**引文。** 共 80 条，按类型分布 {"build": 1, "in_body": 71, "in_body_comment": 6, "macro": 1, "type_member": 1}。核对规则如下：

- 每条引文必须与固定对象上声明的行号区间逐行一致，且只有 1 到 5 行。
- `in_body`：位于同名函数体内（行号大于 `{` 所在行），且至少有一行活动代码。
- `in_body_comment`：位于函数体内，且全部是注释。
- `macro`：首行为 `#define 名称`，其余行是续行。
- `type_member`：位于同名 struct 或 enum 的体内。
- `build`：位于 CMake 文件中，且名称出现在引文首行或前面的 `set(`/`file(` 中。
- 每条引文记录的 `cpp_conditions` 必须等于计算出的非 include-guard `#if` 栈。

旧 verify_anchors.py 中“文件作用域命名”“注释块上方的 define”这两种宽松分支都没有沿用。

**复用边界。** 复用方式是 import，而不是复制 `scheduler-integration-03/scripts/scan_index.py` 的 `analyse()`/`enclosing()`/`show()`，用它们取得函数大括号范围、注释掩码和 `#if` 栈。这个词法器是启发式的：它把 `__alloc_size(1)` 之类的属性宏误当成函数名。本脚本另加 `span_name()` 从函数头重取真正的名字，同时保持“函数头第一个非属性宏标识符”的严格规则。

**Markdown 标签。** `[VERIFIED path::symbol]` 后面的代码块，必须是该文件中逐字连续的行，并且严格落在该符号的函数体内。

**检索。** 共 43 个 `git grep` 或文件尺寸查询，命令、范围、返回码与命中都写入 `records/queries.json`。每条命中都按注释掩码标出 active 或 comment，并给出所在函数名。负检索按“没有 active 命中”判定；返回码 ≥ 2 记为工具失败，不当作 0 命中。边表中引用的每个命中，都会核对“该查询确有此行、此行是 active、所在函数一致”。

**结构。** 检查内容包括：ID 唯一、引用不悬空、上限（24/40/80/6，每个子系统 3–6 个能力）、四个固定子系统、各证据轴的取值、runtime 全为 NOT_RUN、每条疑点都有 premise/path/counter_evidence/unknown、counts 块等于重算值。

**M00 算术。** 对 NR-6 的三张值表逐步核对：区间与 current 是否衔接、计费是否等于区间（idle 为 0）、合计是否等于预期、是否满足“普通任务计费之和 + idle 区间 = 跨度”。

**结果（`records/facts_check.json`）：**

| 项 | 结果 |
|---|---|
| 引文 | 80/80 通过；按类型 {"build": 1, "in_body": 71, "in_body_comment": 6, "macro": 1, "type_member": 1} |
| Markdown 标签 | integration-03-disposition.md 4/4；memory-baseline.md 8/8 |
| 结构 | 错误 0；上限 {"capabilities": [21, 24], "edges": [39, 40], "anchors": [80, 80], "candidates": [6, 6], "per_subsystem": {"mm.page_alloc": 5, "mm.kmalloc": 5, "mm.vm_map": 6, "mm.fault": 5}} |
| 检索 | 43 个，工具失败 0，与期望不符 0 |
| M00 | original_idle_key_zero {"N2": 50, "N1": 40, "idle_interval": 10}（守恒 True）；original_idle_key_max {"N2": 60, "N1": 40, "idle_interval": 0}（守恒 True）；candidate_A_or_B_spec_both_idle_keys {"N2": 60, "N1": 40, "idle_interval": 0}（守恒 True） |
| 总判定 | all_ok = True |

**检查器自身的负面探针（`records/checker_probes.json`）。** 共 9 个探针，每个只在临时副本中改坏一处：引文类型、行号平移、符号名、删掉 `#if` 条件、悬空的边引文、检索期望计数、M00 预期值、runtime 改为 PASS、边命中的行号。9 个全部被拒绝（返回码 1，all_ok 为 false）。这只是一次性的自检，不是测试框架，探针副本没有交付。

## 4. 独立审查与主线回源

**第一轮：两个只读代理分别复核页分配与堆（8 条）、虚拟映射与缺页（13 条）两组初稿疑点。** 它们只读源码、不运行代码，要求“尽力推翻”。结果：

- **页分配与堆：** MC-02、MC-05、MC-07 确认；MC-03、MC-04、MC-09 确认但属于潜伏；MC-01 与 MC-08 的有害后果在本 pin 不可达。
- **虚拟映射与缺页（13 条）：** 12 条确认，其中若干条带修正；MC-17 被驳回；MC-12 的触发场景比初稿更具体。

**主线回源确认的关键反证**（这些反证都已写入引文或检索）：

- PageBuddy 判定 `page_type` 的类型基值（A-PA17）；`page_mapcount_reset` 被注释（A-PA18）；mm 与 arch mm 目录中 page_type 的写入者只有按位清置与 `atomic_inc(_mapcount)`（Q34）。
- `copy_sighand` 的安装语句被注释（A-KM14）；`->sighand` 的唯一写入是 init_task 的静态初值（Q33）；带 ctor 的 cache 共有三个（Q11）。
- 新 VMA 带 `dummy_vm_ops`（A-VM18），`vma_is_anonymous` 判定的是 `!vm_ops`，`shmem_zero_setup` 的函数体被注释（主线阅读 shmem.c）；共享匿名 VMA 因此走 `do_fault` 的 `->fault` 缺失分支自旋（A-FT27）。
- 中断与异常出口在 `!in_atomic()` 时调用 `schedule()`（Q32）。
- `ELF_ET_DYN_BASE` = 窗口的 2/3，`TASK_UNMAPPED_BASE` 基于 `TASK_SIZE_LOW`（只核了宏定义，没有核数值，也没有运行）。

修正后的说法登记在 YAML 的 `refuted_claims`（RF-01…RF-05）中；收窄的条目登记为 GAP-M10。

**第二轮：一个只读代理对成稿做终审**，检查过度断言、前后矛盾、M00 复算和任务书要求是否齐全：

终审代理报告了 11 处问题，全部已在结果提交 eb75b3100601 之前改正，并重新通过机械核对：

1. **取页失败的路径写错。** `folio_alloc_noprof` 中的 `page_folio(NULL)` 会先读空指针，所以 fault.c 的 `if (!folio)` 判断到不了。已改写 anonymous 与 cow 的 failure_path、MC-24 的 effect 和 memory-baseline §0 第 2 条，并新增 Q42、Q43。主线回源确认了 `_compound_head` 的读法。
2. **交付文件当时缺失。** MANIFEST 与 evidence 本来就按计划放在文档提交中；records 中的 queries.json、facts_check.json 已随结果提交交付。
3. **“page_type 从不被设置”说过头了。** 已改为“从不被设置成类型基值”；buddy_free 的“合并正文完整”也改为准确描述。
4. **munmap 仍引用已驳回的 MC-17。** 已改为 MC-13、MC-15、MC-16。
5. **“只依赖关中断自旋锁”不准确。** 已改为分列：关中断的 zone 锁与 slab 链表锁、不关中断的页表锁。
6. **MC-12 反证中有一个无依据的“必然”。** 已改为列出 break 的两个条件。
7. **brk_stack_exec 标为 connected_body 却没有正文锚点。** 已新增 A-VM19（do_brk_flags 正文），引文总数变为 80，达到上限。
8. **MC-21 的说法与自身反证矛盾。** 已改为“物理页原有数据，来源限于内核自身释放过的页或启动后未写过的内存”。
9. **审查计数不一致。** 已改为初稿 21 条，并按“驳回 2、收窄 1、更正 1、驳回唯一式断言 1”分开写。
10. **B-K1 与 B-K2 冲突。** 已改为“非 idle 任务的最小键；idle 按身份排在尾部”。
11. **两处小问题。** pgtable_walk 的用途改为 PUD/PMD/PTE（P4D 为折叠层）；E10、E13 已补进 buddy_free 的边列表。检查器同时新增“边与端点能力双向一致”的规则（探针 p10）。

终审对 M00 的独立复算与本文三张表逐格一致。它还指出 `m00_check` 只核对区间、计费、守恒与“上一步选中 = 本步 current”，不核对队列列；这一限制如实保留。

## 5. 关键数字

| 项 | 数 |
|---|---|
| 子系统 / 能力（每子系统） | 4 / 21（{"mm.page_alloc": 5, "mm.kmalloc": 5, "mm.vm_map": 6, "mm.fault": 5}） |
| 实现证据分布 | {"connected_body": 16, "declaration_only": 0, "not_assessed": 0, "not_located_in_scope": 0, "partial_body": 5} |
| 边 / 链路断点 | 39 / 9 |
| 静态疑点 / 依赖风险 / 驳回或收窄的初稿判断 | 20 / 6 / 5 |
| 引文 / 检索 | 80 / 43 |
| 下一步验证候选（NOT_RUN） | 6 |
| 缺口（open / 合计） | 9 / 10 |

## 6. 提交与远端回读

- **结果提交 `eb75b3100601`：** 新增 9 个文件（三份主正文、两个脚本、四份记录），已推送到 `claude/dazzling-cori-q0dnyt`。
- **远端回读：** 推送后用 ee9e 冻结的 `core/recheck-01/fixtures/readback2.py` 与 `harness2.py` 回读。两者在会话 scratch 中的副本与 ee9e 对象逐字节一致，未作修改。object 模式按提交对象取文件，branch 模式按分支名取文件并要求分支头与目标相同或为其后代且文件一致。每种模式都走 raw.githubusercontent.com 与 api.github.com 两个通道。

| 记录 | 模式 | 分支关系 | 期望字节 | 通道 | ok |
|---|---|---|---|---|---|
| readback_facts-and-dependencies.branch.json | branch | equal | 148940 | raw.githubusercontent.com 200 148940 字节 一致=True；api.github.com_contents_raw 200 148940 字节 一致=True | True |
| readback_facts-and-dependencies.object.json | object | — | 148940 | raw.githubusercontent.com 200 148940 字节 一致=True；api.github.com_contents_raw 200 148940 字节 一致=True | True |
| readback_facts_check.branch.json | branch | equal | 2340 | raw.githubusercontent.com 200 2340 字节 一致=True；api.github.com_contents_raw 200 2340 字节 一致=True | True |
| readback_facts_check.object.json | object | — | 2340 | raw.githubusercontent.com 200 2340 字节 一致=True；api.github.com_contents_raw 200 2340 字节 一致=True | True |
| readback_integration-03-disposition.branch.json | branch | equal | 14400 | raw.githubusercontent.com 200 14400 字节 一致=True；api.github.com_contents_raw 200 14400 字节 一致=True | True |
| readback_integration-03-disposition.object.json | object | — | 14400 | raw.githubusercontent.com 200 14400 字节 一致=True；api.github.com_contents_raw 200 14400 字节 一致=True | True |
| readback_memory-baseline.branch.json | branch | equal | 50335 | raw.githubusercontent.com 200 50335 字节 一致=True；api.github.com_contents_raw 200 50335 字节 一致=True | True |
| readback_memory-baseline.object.json | object | — | 50335 | raw.githubusercontent.com 200 50335 字节 一致=True；api.github.com_contents_raw 200 50335 字节 一致=True | True |

- **文档提交：** 只新增 MANIFEST.md、evidence.md、`records/scope_stage.json` 与 `records/after_results/` 下的 8 份回读记录，不修改结果提交中的任何文件（见 §7）。

## 7. 范围边界检查

| 阶段 | 结果 |
|---|---|
| start（开工） | all_ok=True；见 records/scope_start.json |
| stage（结果提交前） | all_ok=True；新增 9 个文件，均位于前缀内；越界变更 无；新脚本 scripts/mm_facts_check.py、scripts/mm_scope.py；卫生（无 40 位十六进制、无凭据模式、无 NUL/ZIP、YAML/JSON 可解析、带出处头与模型标记）全部通过；见 records/scope_stage.json |
| docs（文档提交前） | all_ok=True；结果提交 eb75b3100601 是 HEAD 的祖先=True；相对结果提交新增 11 个文件、修改结果文件 无；MANIFEST 缺列 无、多列 无；新增文件卫生全部通过=True；主线对象关系 equal；time/master 远端 a039d9803ade/de3bb1df906a |

## 8. 卫生与未做事项

- **字节码缓存：** 第一次 import 复用模块时，Python 在 `scheduler-integration-03/scripts/` 和本目录的 `scripts/` 下各生成了一个 `__pycache__`。前者是 ee9e 原件目录中的未跟踪文件，暂存前已删除，且从未进入索引或提交。此后所有运行都设置 `PYTHONDONTWRITEBYTECODE=1`。
- **中间文件：** 解出的源码、探针副本、生成脚本都放在会话 scratch 的新目录中，没有交付。
- **没有做的事：** 没有编译或运行任何 C/ASM，没有运行 cmake、内核或 QEMU，也没有执行仓库原有脚本；没有重跑 W/V/M/N，没有安装工具，没有提权，没有输出凭据。
- **没有修改的内容：** 没有修改内核、ee9e 中的任何原件、主线分支、master 或 time；没有合并，没有删除分支，没有 force-push。
