---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-SCHED-INTEGRATION-03
phase: scoped_source_integration
record_type: scheduler_integration_manifest
produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"
execution_model_selection: unknown_or_not_attestable
execution_surface: "claude.ai/code 托管云端会话容器；只读 git 对象查询与 Python 文本处理（git 2.43.0、Python 3.11.15，均为容器既有工具）；与 pilot/core/recheck/scheduler-order-02 同一会话、同一执行分支"
date: "2026-10-02"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；主线 40bc4faa4202（16 号任务书与 CORE-SCHED-ORDER-02 审查回执，开工时固定）；被审执行头 f36b89b8a53a；见证结果 5078686e8267。均为短 SHA"
work_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
results_commit_short12: b7fa83583e35
acceptance_ceiling: PASS_PENDING_LOCAL
status: final_for_scheduler_integration_03
kernel_correctness_verdict: NOT_ISSUED
kernel_change_made: false
patch_or_diff_produced: false
c_or_asm_compiled_or_run: false
startup_selfcheck_quote: '2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。'
write_authority_note: "只在 scheduler-integration-03/ 新增，由 Owner 本轮指令与 16 号任务书 allowed_write_prefix 授权（check_scope.py 机械核对）。f36b89b8a53a 中全部文件不变；未写 agent/MYOS2-LEAD-002、master、time。PR17 由 Ready 转回 Draft 后复用。"
inputs_read:
  - "agent/MYOS2-LEAD-002 @ 40bc4faa4202：16-scheduler-integration-contract.md（全文，raw 与对象逐字节一致）"
  - "agent/MYOS2-LEAD-002 @ 40bc4faa4202：reviews/CORE-SCHED-ORDER-02-review.md（全文）；checkpoints/2026-10-02-scheduler-witness-reviewed-integration-ready.md（全文）"
  - "f36b89b8a53a：scheduler-order-02/MANIFEST.md 与 results.yaml（全文）；core/MANIFEST.md 与 core/results.yaml 的 V04–V08 条目"
  - "time @ a039d9803ade：myos_rt.c 全文；scheduler_core.c 的 set_task_cpu、try_to_wake_up、wake_up_*、__sched_fork、sched_fork、wake_up_new_task、resched_curr、kick_process、__schedule、schedule*、init_idle、sched_init；scheduler.h、scheduler_macro.h；double_list.h 的 header 原语区段；init_task.c、main.c（rest_init、start_kernel）、fork.c（dup_task_struct、copy_process 调度相关行、kernel_clone、kernel_thread）、idle.c、interrupt.c、hpet.c、timekeeping.c、percpu_area.c、kernel.lds 的相关行；其余按 d01 命中定点打开"
self_check:
  verified_claims: 10
  quotes_reconfirmed: 10
  downgraded_to_inferred: 0
  by_file: {"integration.md": 10, "evidence.md": 0}
  method: "VERIFIED 标签全部由 scripts/verify_anchors.py 在 time 对象上逐行核对；MANIFEST 与 evidence.md 本身不新增源码标签"
d01_d05:
  D01: "完成：定点索引 records/d01_index.json（可由 scan_index.py 重建）；限直接文本"
  D02: "完成（含明示缺口）：五条链分行，C3 分离队与唤醒两步；set_task_cpu 的 MyOS2 副作用与上游职责分开；首次入队与普通唤醒分开；零键不外推"
  D03: "完成：计费与队列写入者分表；高键唤醒四栏推导；正常行为 W03–W08 的源码来源"
  D04: "完成（规格，不是实现）：候选 A、B 与两项 Owner 取舍；无 C 代码或 diff"
  D05: "完成：W/V 对应表；新增回归 6 项，全部 NOT_RUN"
data_summary: {"source_facts": 151, "source_facts_mechanically_verified": "151/151", "integration_verified_tags": "10/10", "lead_review_G_anchors": "7/7", "derivations": 24, "gaps": 12, "chains": ["C1", "C2", "C3", "C2×C3", "C4", "C5"], "candidates": ["A", "B"], "owner_decisions": 2, "future_regressions_not_run": 6, "d01_queries": 44, "d01_lines": 495, "d01_by_class": {"conditional_unresolved": 20, "declaration_or_comment": 235, "visible_active_text": 143}, "d01_definitions_unlocated": ["__set_task_cpu", "myos_wake_up_new_task", "fair_sched_class", "idle_sched_class", "rt_sched_class", "dl_sched_class", "stop_sched_class", "HAVE_ARCH_BUG_ON", "ttwu_queue", "ttwu_state_match"]}
delivered_files:
  - MANIFEST.md
  - evidence.md
  - facts-and-regressions.yaml
  - integration.md
  - records/after_results/readback_facts-and-regressions.branch.json
  - records/after_results/readback_facts-and-regressions.object.json
  - records/after_results/readback_integration.branch.json
  - records/after_results/readback_integration.object.json
  - records/anchors_check.json
  - records/d01_index.json
  - records/scope_start.json
  - scripts/check_scope.py
  - scripts/scan_index.py
  - scripts/verify_anchors.py
---

# CORE-SCHED-INTEGRATION-03 交付清单

## 先读限制

- **范围。** 这是静态文本查询加推导的结果：没有编译或运行任何 C/ASM，没有重跑 W/V/M/N，没有改内核，也没有给出补丁。
- **直接检索的边界。** 只覆盖直接文本：别名、函数指针、汇编、宏生成名不排除；“无命中”从不写成“无写入者/无调用者”。
- **已核与未核。** 结论限于单 CPU0、固定源码 a039d9803ade。真实中断时机、`in_atomic`、SMP、上下文切换、CA-02 全局可达性都没有执行验证。
- **政策状态。** 候选 A、B 与 OD-1、OD-2 都是提案，没有被采用或授权；NR-1…NR-6 是未来回归规格，状态 NOT_RUN。
- **验收边界。** 验收上限 PASS_PENDING_LOCAL；不签发内核正确性结论。执行模型记为 unknown_or_not_attestable。

## D01–D05 完成情况

| 项 | 状态 | 主要位置 |
|---|---|---|
| D01 定点索引 | 完成：44 个查询、495 行命中，需定位的定义全部找到 | records/d01_index.json，evidence.md §2 |
| D02 五条链 | 完成，缺口明示（GAP-01…GAP-09） | integration.md §1，yaml `chains` |
| D03 计费与不变量 | 完成，含高键唤醒四栏推导 | integration.md §2，yaml `accounting`、`high_key_wake_state_derivation` |
| D04 候选规格 | 完成：A（只改 pick）、B（统一顺序），两项取舍 | integration.md §3–4，yaml `candidate_changes`、`owner_decisions` |
| D05 回归对应 | 完成：W/V 对应表，新增 6 项 NOT_RUN | integration.md §5，yaml `witness_refs`、`future_regressions` |

## 结论与建议（详见 integration.md）

- **写入者。** 在直接文本命中里，运行队列只有四个写入者（别名、函数指针未排除）：`init_idle`、`wake_up_new_task`、`set_task_cpu`、`pick_next_task_myos`。选择永远是摘队首、不读键。
- **候选 A 可以先准备。** 它只改 `pick_next_task_myos`，满足 A-R1…A-R6：先计费再定位、比较对象随游标、先判 anchor、以身份把插回限制在 idle 之前、链接与计数一体、插在第一个键不小于自己的节点之前。A 能消除 W01/W02 两类失序和现有的伪任务指针读取，W03–W06、W08 的记录不变，W07 保持“不重复计费”但第 2 步改选 C20；它不能承诺全局升序。
- **候选 B 需要政策决定。** 全队升序、选中队列（不含 current）的最小键，要改普通唤醒的插入位置并给首次入队补前置保护；键高于队首的被唤醒任务失去插队，新任务仍在队首。
- **建议。** 先实施 A，把 B 定为后续目标。
- **重要发现。** 现有 pick 在“单个普通任务 + idle、时间片到期”时已经会经 anchor 求出伪任务指针并读取（DV-11，三个读取面独立得到，未执行），这更正了此前“只改游标才会读 anchor”的说法。
- **反证与修正。** 零键新任务头插不制造逆序；入队造成的逆序只来自高键普通唤醒，且会在后续切换中被消费；HPET 不置 `need_resched`，时间片到期靠中断后的 `schedule()`；引导 idle 的键为 2^64-1（W 夹具用 0/7）；W04 在原函数中不是并列见证。
- **新的风险，可达性都未定。**
  - `TASK_NEW` 任务若在首次入队前被唤醒，可能自旋挂死或破环（DV-13）。
  - 写了非 RUNNING、还没检查条件就被中断切出的任务不回队（DV-23）。
  - 有限超时 `schedule_timeout` 不睡眠却删除定时器（DV-19）。

## 需要人工决定的事项

- **OD-1：** 保留唤醒/新任务头插优先（A 为终态），还是采用全队按更新后 vruntime 非降序（B）。建议先 A、后 B；不答时只做 A。
- **OD-2：** B 下是否给新任务与唤醒任务设键下限。建议设 max(自身键, 当时队首非 idle 键)；不答时 B 不实施。

## 文件

| 文件 | 作用 |
|---|---|
| integration.md | 五条链、计费责任、两种候选、修改边界与取舍（主报告） |
| facts-and-regressions.yaml | 源码事实（逐字引文）、调用边、别名与条件、推导、见证引用、候选修改点、Owner 取舍、NR-1…NR-6（NOT_RUN）、缺口与停止点；各层分列 |
| evidence.md | 实际命令与输出、检索范围、引文核对、并行核验摘要、未解项、未执行声明、提交与回读 |
| scripts/scan_index.py | D01 定点检索与命中分类（重建 records/d01_index.json） |
| scripts/verify_anchors.py | 源码引文与 G01–G07 的机械核对（重建 records/anchors_check.json） |
| scripts/check_scope.py | 开工/推送前的绑定、冻结与写区核对 |
| records/ | 命中索引、开工核对、引文核对、结果批回读记录 |

## 唯一重建入口

```text
cd agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-integration-03/scripts
python3 scan_index.py ../records/d01_index.json
python3 verify_anchors.py ../facts-and-regressions.yaml ../records/anchors_check.json ../integration.md
python3 check_scope.py start
```

三者只读 git 对象（需要本地有 time a039d9803ade 与主线 40bc4faa4202 对象），不编译、不运行内核代码。

## 下一步建议

- 若 Owner 授权内核修改：按候选 A 准备实现。验证时在宿主夹具上重跑 W01–W08（两次完整输出），并补 NR-3、NR-4、NR-6 的 A 列。
- B 只在 OD-1 选 B 并答复 OD-2 后再准备。
- 不追加新的研究轮次、补丁或 QEMU 任务。
