---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-VFS-BASELINE-05
record_type: evidence_log
produced_by: "Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable"
date: "2026-10-02"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；主线 agent/MYOS2-LEAD-002 0d066801371d（开工时）；被审执行头 de7c96ede546；MM 结果冻结 eb75b3100601"
inputs_read: "见 §2"
status: final
open_questions: "见 user-vfs-baseline.md §8 与 facts-and-dependencies.yaml 的 gaps"
---

# CORE-USER-VFS-BASELINE-05 证据记录

本文记录本批实际执行的命令、核对结果、独立审查过程和提交回读。所有命令都只读 git 对象，或用 Python 处理文本、YAML、JSON。没有编译，没有运行 C/ASM、cmake、内核、QEMU、宿主安装脚本或仓库原有脚本；没有重跑旧测试。Python 运行时一律设置 `PYTHONDONTWRITEBYTECODE=1`，交付的两个脚本也在 import 之前设置了 `sys.dont_write_bytecode = True`。

## 1. 开工绑定（`scripts/uv_scope.py start` → `records/scope_start.json`）

| 项 | 结果 |
|---|---|
| 主线对象 | 远端 `agent/MYOS2-LEAD-002` = 0d066801371d（relation: equal）。19 号任务书 16 个关键头字段、审查回执 8 个字段与预期全部一致；同时记录了 18 号约定的 status、owner_action_for_existing_findings 与 kernel_change_authorized |
| 任务书来源 | 经 raw URL 读取的 19 号任务书与 git 对象逐字节一致（HTTP 200，14505 字节） |
| 回执 | CORE-MM-BASELINE-04-REVIEW-001：被审头 de7c96ede546、结果 eb75b3100601、RETURN、returned_items = MR-01/MR-02、next_followup = CORE-USER-VFS-BASELINE-05 |
| 执行分支 | `claude/dazzling-cori-q0dnyt` 的头为 de7c96ede546；开工时相对 de7c96ede546 只有本前缀内的未跟踪新增 |
| 固定对象 | 远端 time = a039d9803ade。master = de3bb1df906a，作为事实记录，不固定；master 不含 de7c96ede546（PR17 未合并） |
| PR17 | 开工时经 GitHub 读取：open、draft、未合并，head 为执行分支 |

开工记录中 `start_note` 的文字仍写着上一批的基准号 ee9e6a738224，这是改写脚本时遗留的字面串；实际比较用的是 `PINS["reviewed"]` = de7c96ede546，记录中的 `changes_vs_reviewed` 也只列出本前缀内的一个未跟踪文件。脚本中的这段字面串，以及用法说明和注释中两处遗留的旧名称，已在结果提交前改正；开工记录按运行时的原样保留。

## 2. 读取范围

- **主线：** 19 号任务书全文；18 号约定全文；CORE-MM-BASELINE-04 审查回执全文。
- **de7c96ede546 原件（mm-baseline-04）：** MANIFEST.md、memory-baseline.md、integration-03-disposition.md §4、evidence.md、facts-and-dependencies.yaml 中与 MR-01/MR-02 相关的行（逐行定位见 mm-04-disposition.md 的 supersedes_scope）；该包的 concerns、dependency_risks、link_breaks、refuted_claims、gaps 的 ID 与标题；scheduler-integration-03 中 A-R6、B-R1…B-R6、SF-C3-37、DV-19 的定义行；core/results.yaml 中 V02、V10 的头部。
- **master 旧输入：** 只读 DR-002 completeness.yaml 中 `sched.forkexec`、`fs.vfs` 两个条目的 id 与 node 名，以及 DR-003 deps.yaml 中与两者相交的端点名。
- **源码：** 先用 `git archive a039d9803ade mykernel myinitramfs` 解到会话 scratch 的新目录，只供阅读；引文与检索的核对一律回到 git 对象（`git show a039d9803ade:<path>`、`git grep … a039d9803ade`）。宿主脚本 `make_install.sh`、`scripts/make_install_initranfs.sh` 只读文本，没有运行。

## 3. 方法与机械核对（`scripts/uv_facts_check.py`）

**生成方式：** 能力、边、链、断点、条目等内容由会话 scratch 中的生成脚本组装成 YAML，生成脚本不交付。引文行号不手写：按“函数名 + 首行文字 + 行数”在固定对象上定位，再取逐字内容；检索的期望计数取自生成时的实际运行结果，并逐条人工看过命中行。

**引文（90 条，build 2, in_body 80, in_body_comment 6, macro 1, type_member 1）：** 规则与 MM 包相同：1 到 5 行，逐字一致；in_body 必须严格位于同名函数体内，且至少有一行活动代码；in_body_comment 位于函数体内且全是注释；macro 首行为 `#define 名称`，其余为续行；type_member 位于同名类型体内；build 位于 CMake 文件中，且名称出现在首行。记录的 `cpp_conditions` 必须等于计算出的非 include-guard `#if` 栈。

**复用边界：** 以 import 方式复用 `mm-baseline-04/scripts/mm_facts_check.py` 的 check_anchor、check_md_tags、run_query、span_name，后者又 import scheduler-integration-03 的 scan_index（启发式词法器）。本脚本只加了一条规则：函数头为 `MYOS_SYSCALL_DEFINEn(name, …)` 时命名为 `sys_<name>`，因为复用的词法器会把这类函数按宏名命名。

**新增的核对：**

- 词法结构检查：7 项，针对单个函数体做“没有活动的某个记号”“函数体为空”“恰好 N 处赋值”三种检查（SC-01…SC-07）。
- 值表：VT-MR01 按键下限规则逐步重算队列，VT-FR01 按源码公式重算每页长度。
- U00 旧文字定位：mm-04-disposition.md 头部列出的 40 处（33 处取代，7 处复查后保留），回到 de7c96ede546 对象逐处核对“片段在所述行上”。
- 延期记录：每个条目的必填字段、qualified_id 格式、status 与 kind 取值、`owner_action_now: none`、证据引用能在 facts 中解析、与能力双向列出。
- 间接调用：凡有 `indirect` 字段的边，调用点、绑定、接收方三项都必须能解析为引文，或为某个检索中的活动命中。
- 整包文本中不得出现 40 位十六进制串。

**检索：** 44 个 `git grep` 或文件尺寸查询，命令、范围、返回码与命中都写入 `records/queries.json`。每个命中按注释掩码标为 active 或 comment，并记录所在函数。负检索按“没有活动命中”判定；返回码不低于 2 记为工具失败，不当作 0 命中。

**结果（`records/facts_check.json`）：**

| 项 | 结果 |
|---|---|
| 引文 | 90/90 通过（build 2, in_body 80, in_body_comment 6, macro 1, type_member 1） |
| Markdown 标签 | user-vfs-baseline.md 31/31；mm-04-disposition.md 无标签 |
| 结构 | 错误 0；上限 能力 20/20、边 43/45、引文 90/90、候选 6/6；两主对象各 9、9 个能力 |
| 延期记录 | 错误 0 |
| 检索 | 44 个，工具失败 0，与记录不符 0 |
| 词法结构检查 | 7/7 |
| 值表 | VT-MR01 7 行、VT-FR01 5 行，全部一致 |
| U00 旧文字定位 | 40/40（取代 33，复查保留 7） |
| 40 位十六进制 | 包内 0 处 |
| all_ok | True |

**检查器的负面探针（`records/checker_probes.json`）：** 共 12 个探针，每个只在 scratch 副本中改坏一处：引文行号平移、引文类型、删掉 #if 条件、间接调用点行号、VT-MR01 预期队首、VT-FR01 预期长度、旧文字行号、条目 owner_action_now、runtime 改为 PASS、counts 块、负检索改成会命中的模式、结构检查换函数名。12 个全部被拒绝（返回码 1，all_ok 为 false）；未修改的副本 p0 通过。这是一次性自检，不是测试框架；探针驱动与副本留在 scratch，没有交付。

## 4. 独立审查

一个只读代理逐条复核 9 组说法，要求尽力反驳：UV-04、UV-05、UV-06 与 VT-FR01、UV-09、UV-14、UV-15、C1 首个程序入口、C5 后端与 ROOTBLK_NVME、MR-01 值表。它只读源码，不构建、不运行，也不写文件。

- **确认（6 组）：** UV-04、UV-05、UV-15、C1、C5、MR-01（T1–T7 重算一致）。审查补充了对本批结论有利的反证范围：do_open/myos_do_dentry_open 只有一处定义且无宏改写；ROOTBLK_NVME 在任何 CMake 文件或头文件中都没有活动定义；kjmp_to_doexecve 与 kernel_execve 各只有一个活动调用者；没有其他回收死亡任务的活动路径。
- **确认但需更正（3 组），均已采纳：**
  - VT-FR01 的 R3 不会“返回 -100”：负长度作为 size_t 交给 memcpy（逐字节 `while (count--)`），越过页面拷贝，函数不会正常返回。值表字段改名为 `expect_sum_len`，表头改为“Σlen（公式）”，并加 R3 注释；UV-06 中“页对齐即短读”收窄为“最后一页从页首开始时”。新增 Q41。
  - UV-09：对象本身经 virt_to_slab 回到自己的 kmalloc slab，只有链表记账（节点、min_partial、free_slab）用 filp_cachep；副本只在父进程 f_count 为 1 时才被释放。条目与 §0 的说法相应改写。新增 Q42。
  - UV-14：virt_to_slab 对非 slab 页返回 NULL，free_to_partial_list 随后读 slab->inuse，所以第一次 1 → 0 很可能就出错；“之后计数下溢”撤回。新增 Q43。
- **过度断言（5 处），均已改正：** simple_filemap_fault 并非“总是返回 0”（vm_file/f_mapping 为空时返回 VM_FAULT_ERROR，槽位已有页时 BUG_ON）；R3 的“返回 -100”；页对齐短读的条件；§0 把 needs_evidence 的 UV-09 写成既成事实；UV-14 的“计数下溢”。
- **审查顺带发现：** dup_task_struct 中 `if (node == NUMA_NO_NODE)` 的下一行被注释，分配语句成了 if 的唯一语句；唯一调用点传 NUMA_NO_NODE，目前不触发。登记为 UV-16（潜伏），新增 Q44。

主线对每一处更正都回到源码复读（string.c 的 memcpy、slub.h 的 virt_to_slab、slub.c 的 free_to_partial_list 与 slab_free、fork.c 第 278–280 行、filemap.c 第 119–123 行），随后重新生成、重跑检查器和全部探针。审查过程不替代原始来源，结论仍以引文与检索为准。

## 5. 关键数字

- 能力 20（sched.forkexec 9、fs.vfs 9、boundary 2）；实现证据 connected_body 15、declaration_only 1、partial_body 4。
- 边 43（build 1、call 34、config 1、data 3、init_order 4）；其中带三项证据的间接调用 11 条。
- 引文 90、检索 44、词法结构检查 7、值表 2、链路断点 10、依赖风险 6、候选 6（全部 NOT_RUN）、缺口 10（全部 open）、旧 ID 映射 19。
- 延期记录：16 项新条目、2 项文档勘误、10 项旧条目状态变更、13 项交叉链接；新条目中 deferred_owner_not_ready 12、needs_evidence 4。
- U00：取代 33 处旧文字，复查保留 7 处。

## 6. 提交与远端回读

| 项 | 结果 |
|---|---|
| 结果提交 | 22934124fe92（父提交 de7c96ede546），已推送到 claude/dazzling-cori-q0dnyt；推送后远端分支头为 22934124fe92 |
| 结果提交内容 | 11 个新文件，全部在本前缀下：四份主件（mm-04-disposition.md、user-vfs-baseline.md、facts-and-dependencies.yaml、deferred-findings.yaml）、2 个脚本、5 个记录 |
| 远端回读 | 12 条记录全部通过：6 个文件 × object/branch 两种模式，每条都经 raw.githubusercontent.com 与 api.github.com contents 两个通道，HTTP 200 且与提交中的 blob 逐字节一致；YAML/JSON 文件另核 followup_id 字段（JSON 与脚本只核逐字节） |
| 回读中的一次失败 | 两个 Markdown 文件第一次带字段运行时，回读工具把整个文件当 YAML 解析，得到 PARSE_ERROR；传输与逐字节一致都已通过。随后按逐字节一致重跑并通过，front matter 中的 followup_id 已在本地对同一字节解析确认（见 records/readback_results.json 的 note） |
| 文档提交 | 本文件、MANIFEST.md 与 records/readback_results.json 在结果提交之后另行提交，不改动结果提交中的任何文件；它的短号见 PR17 顶部，本文件不自引用 |

## 7. 范围边界检查

- **start**（records/scope_start.json）：all_ok 为 True；主线 relation 为 equal；master 为 de3bb1df906a，不含 de7c96ede546。
- **stage**（records/scope_stage.json，结果提交前）：all_ok 为 True；相对 de7c96ede546 只有本前缀内的新增（10 个文件，越界 0）；新脚本 2 个（上限 2）；各新文件无 NUL/ZIP、无 40 位十六进制、无凭据模式，YAML/JSON 可解析，脚本有来源头。
- **docs**（文档批暂存后运行）：结果提交是 HEAD 的祖先，文档批不改动结果提交中的任何文件，MANIFEST 列出的文件与实际新增文件完全一致。为避免自引用，这次输出不入库，结果记在 PR17 顶部。
- 交付前后 time 均为 a039d9803ade，主线分支头均为 0d066801371d；两个写区（执行分支、主线分支）没有相交修改。

## 8. 卫生与未做事项

- **字节码缓存：** 所有 Python 运行都设置了 `PYTHONDONTWRITEBYTECODE=1`；交付前检查过整个 agent-workspace 下没有 `__pycache__`。
- **中间文件：** 解出的源码、生成脚本、探针副本都放在会话 scratch 的新目录中，没有交付。
- **没有做的事：** 没有编译或运行 C/ASM、cmake、内核、QEMU、宿主脚本或仓库原有脚本；没有重跑 H00/V/W/M/N；没有安装工具，没有挂盘，没有提权，没有输出凭据。
- **没有修改的内容：** 内核、de7c96ede546 中的任何旧文件、主线分支、master、time 都没有修改；没有合并，没有删除分支，没有 force-push。
