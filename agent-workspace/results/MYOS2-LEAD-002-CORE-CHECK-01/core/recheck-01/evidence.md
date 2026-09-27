---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-CHECK-01-RECHECK-01
phase: core_recheck_01
record_type: verifier_recheck_evidence
produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"
execution_model_selection: unknown_or_not_attestable
execution_model_selection_source: "执行者所在平台的规则不允许在推送到仓库的产物中写模型标识（执行者自述）"
date: "2026-09-27"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；taskbook 57a7c3e0eebf；主线回执头 ec62453e76d2；冻结 core a1e7c2277705（短 SHA）"
batch1_commit_short12: f9ae2bce9a64
results_commit_short12: efb9846b88ec
status: final_for_recheck_01
transcription: "§1.1、§1.3 与 §9 中的推送输出是会话内命令的手工转录；其余数值、表格与输出由 fixtures/build_evidence2.py 从 observations/ 与 results.yaml 生成。observations 下的 JSON 是程序写出的原文件，未做换行或脱敏变换。"
file_moves: "observations/after_batch1/ 的三个文件在第一批推送后写在 observations/ 根下，第二批提交前原样移入该子目录（内容未变）；after_batch2/ 的三个文件先写在会话临时目录，再逐字节复制（cmp 相同）。readback2 在 --out 同目录建立的 rbwork-* 下载目录没有入库。第一批的 run.json、v00.json、meta_tests.json 与 results.yaml 被第二批重跑的输出替换，旧版本保留在提交 f9ae2bce9a64 中。"
redaction: "未发现需脱敏内容。push 输出经 40 位十六进制替换过滤（实际无命中）；回读 URL 中的完整提交号在程序内替换为 <commit:短号>。"
open_questions: []
---

# CORE-CHECK-01 RECHECK-01 证据：旧反例实跑、验证器修订、M01–M12 与同源复跑

**先用冻结旧验证器实跑了主线给出的四类反例，全部复现。修订后的验证器在 M01–M12 中逐项满足，并完成同源 V00–V14 复跑（run exit 0，状态 COMPLETE）。** 第一批提交 `f9ae2bce9a64` 之后，在整理文档时又发现一处同类缺口：静态输入缺失时，V08 仍被判为有效。已在第二批 `efb9846b88ec` 中修正，并重跑一次同源运行与 M01–M12。所有执行都是本次云端会话中的普通用户态进程，不是 MyOS2 在 CPU 上运行。

## 1. 输入、身份与准入

### 1.1 任务书与回执读取（手工转录）

```text
$ curl -sS -o $S/12.md -w 'http=%{http_code} bytes=%{size_download}\n' https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/12-core-check-recheck-contract.md
http=200 bytes=16734
$ cmp $S/12.md <git show origin/agent/MYOS2-LEAD-002:同路径> && echo IDENTICAL
IDENTICAL
$ curl ... reviews/CORE-CHECK-01-core-review.md
http=200 bytes=15032
IDENTICAL
```

会话开始时执行了 `git fetch`，origin/agent/MYOS2-LEAD-002 由 `f79b3a281616` 前进到 `ec62453e76d2`。新增的是 12 号任务书、core-review 回执和一个检查点（`git diff --stat` 显示三个新增文件）。本轮把 `ec62453e76d2` 固定为主线对象（harness2.PINS.lead_head）。

### 1.2 身份门（identity2，同源复跑时的机械记录）

```json
{
 "gate": {
  "core_review_record": true,
  "core_review_input_refs": true,
  "contract12_binding": true,
  "technical_inputs_unchanged": true,
  "lead_head_descends_from_taskbook": true,
  "on_execution_branch": true,
  "head_descends_from_reviewed_core": true,
  "originals_unchanged": true,
  "remote_on_same_line": true,
  "remote_descends_from_reviewed_core": true,
  "worktree_clean_outside_recheck": true,
  "ok": true
 },
 "core_review_fields": {
  "record_id": "CORE-CHECK-01-CORE-REVIEW-001",
  "disposition": "RETURN",
  "reviewed_pr": 17,
  "reviewed_branch": "claude/dazzling-cori-q0dnyt",
  "reviewed_commit_short12": "a1e7c2277705",
  "frozen_results_commit_short12": "7e2fa84a9823",
  "pilot_head_short12": "0851af4fc08b",
  "followup_id": "CORE-CHECK-01-RECHECK-01",
  "allowed_write_prefix": "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/"
 },
 "execution": {
  "branch": "claude/dazzling-cori-q0dnyt",
  "head_short12": "f9ae2bce9a64",
  "remote_relation": "equal"
 },
 "originals": {
  "frozen_files": 26,
  "changed_or_missing": [],
  "added_outside_recheck": [],
  "pilot_changed_since_pilot_head": []
 },
 "technical_inputs_unchanged": {
  "07-scheduler-wakeup-timer-audit.md": true,
  "07-core-audit-map.yaml": true,
  "MANIFEST.md": true,
  "09-local-verification-contract.md": true,
  "10-cloud-pilot-and-github-handoff.md": true,
  "11-core-verification-cloud.md": true,
  "reviews/CORE-CHECK-01-pilot-review.md": true
 },
 "remote_refs_equal_pins": {
  "master": true,
  "time": true,
  "agent/MYOS2-LEAD-002": true
 },
 "pins": {
  "time": "a039d9803ade",
  "master": "de3bb1df906a",
  "taskbook": "57a7c3e0eebf",
  "pilot_review": "f79b3a281616",
  "lead_head": "ec62453e76d2",
  "core_frozen": "a1e7c2277705",
  "core_results": "7e2fa84a9823",
  "pilot_head": "0851af4fc08b",
  "pilot_result": "10ecb7dd0bdb"
 }
}
```

### 1.3 开工时的仓库状态（手工转录）

- 执行分支头与远端都是 `a1e7c2277705`，工作树干净。open PR 只有 [08822407d/MyOS2#16](https://github.com/08822407d/MyOS2/pull/16)（主线，写区 lead/）和 [08822407d/MyOS2#17](https://github.com/08822407d/MyOS2/pull/17)（本执行，Draft），两者写区不相交。
- 会话仓库是浅克隆（`git rev-parse --is-shallow-repository` = true），因此 M10 的临时仓库需要特殊构建方式（见 §6）。

## 2. Phase A：冻结旧验证器实跑反例（修订之前）

`fixtures/old_counterexamples.py` 从 `a1e7c2277705` 逐字节解出旧 core/fixtures（20 个文件，清单一致=True）到全新目录。每个实验在独立子进程中运行，以旧目录为工作目录。工作目录：`/tmp/claude-0/-home-user-MyOS2/e3ce16b3-588b-596b-86aa-5e369173435c/scratchpad/recheck-runs/phaseA-jw7vf60z`。

| 项 | 旧入口与输入 | 旧程序真实结果 | 与主线推导 |
|---|---|---|---|
| R01 | `evaluate.v05({"cases": {}})` | `OBSERVED_AS_PREDICTED` | 一致（空观测判为符合预测） |
| R01 补充 | `run_all.evaluate` 于五个夹具均为空 cases | `{"V02": "COUNTEREVIDENCE", "V03": "COMPARATOR_INVALID", "V09": "COUNTEREVIDENCE", "V10": "COUNTEREVIDENCE", "V11": "COUNTEREVIDENCE", "V04": "COUNTEREVIDENCE", "V05": "OBSERVED_AS_PREDICTED", "V06": "COUNTEREVIDENCE", "V07": "COUNTEREVIDENCE", "V08": {"function_level_combinations": "COUNTEREVIDENCE", "sequence_with_idle_requeue": "COUNTEREVIDENCE", "sequence_when_idle_switched_out_blocked": "COUNTEREVIDENCE", "extra_observation_requeue_order": null}, "V12": "COUNTEREVIDENCE", "V13": "COUNTEREVIDENCE", "V14": "COUNTEREVIDENCE"}` | 主线未列：其余评估器在空观测下给出假的“COUNTEREVIDENCE”，V03 为 COMPARATOR_INVALID |
| R02 gate_false | `run_all.main`，计数替身 | 动态入口调用 2；stage 调用 ["h00_hardening.py", "v00_anchors.py", "v01_structure.py", "static_checks.py"]；异常 None | 一致（门未阻止执行） |
| R02 identity_raises | `run_all.main`，计数替身 | 动态入口调用 2；stage 调用 ["h00_hardening.py", "v00_anchors.py", "v01_structure.py", "static_checks.py"]；异常 None | 一致（门未阻止执行） |
| R02 h00_fails | `run_all.main`，计数替身 | 动态入口调用 2；stage 调用 ["h00_hardening.py", "v00_anchors.py", "v01_structure.py", "static_checks.py"]；异常 None | 一致（门未阻止执行） |
| R04 identity | 旧 `identity()` 于已前进的执行分支 | remote_heads_match=`{"master==pin.master": true, "time==pin.time": true, "agent/MYOS2-LEAD-002==pin.review": false, "claude/dazzling-cori-q0dnyt==pin.pilot_head": false}`；gate=`true` | 一致（记录了不一致，但不阻止） |
| R04 readback | 旧 `h00.probe_readback()` | as_expected=`false`；两通道 `[{"http_code": "200", "transport_ok": true, "byte_identical": true, "field_ok": true, "ok": true}, {"http_code": "200", "transport_ok": true, "byte_identical": true, "field_ok": true, "ok": true}]` | 一致（字节相同，却因分支头前进判失败） |
| R04 CLI no_field_with_out | 旧 `readback.py ... --out <file>` | exit 1；stderr 末行 `["ValueError: not enough values to unpack (expected 2, got 1)"]` | 一致（--out 的值被当作 field=value） |
| R04 CLI with_field_with_out | 旧 `readback.py ... transport_marker=MYOS2-CLOUD-PILOT-20260925-K7P4 --out <file>` | exit 1；stderr 末行 `[""]` | 带 field 时能运行，但仍因分支头前进判失败 |
| R02/R04 重放 | 冻结旧 `run_all.py` 完整运行 | exit 0；h00 stage 退出 1；夹具仍运行=True；评估 `{"V02": "OBSERVED_AS_PREDICTED", "V03": "OBSERVED_AS_PREDICTED", "V09": "OBSERVED_AS_PREDICTED", "V10": "OBSERVED_AS_PREDICTED", "V11": "OBSERVED_AS_PREDICTED", "V04": "OBSERVED_AS_PREDICTED", "V05": "OBSERVED_AS_PREDICTED", "V06": "NO_FAILURE_IN_SCOPE", "V07": "OBSERVED_AS_PREDICTED", "V08": {"function_level_combinations": "OBSERVED_AS_PREDICTED", "sequence_with_idle_requeue": "OBSERVED_AS_PREDICTED", "sequence_when_idle_switched_out_blocked": "OBSERVED_AS_PREDICTED", "extra_observation_requeue_order": null}, "V12": "OBSERVED_AS_PREDICTED", "V13": "OBSERVED_AS_PREDICTED", "V14": "OBSERVED_AS_PREDICTED"}` | 一致（H00 失败后仍运行夹具） |
| R03 baseline_real_replay | 旧 `make_results.py` | exit 0；生成=True；V01=`["NO_FAILURE_IN_SCOPE", "EXECUTED"]`；错误末行 `[""]` | 对照 |
| R03 v01_paths_missing_and_parse_failed | 旧 `make_results.py` | exit 0；生成=True；V01=`["NO_FAILURE_IN_SCOPE", "EXECUTED"]`；错误末行 `[""]` | 一致（仍写 NO_FAILURE_IN_SCOPE） |
| R03 missing_static_stage | 旧 `make_results.py` | exit 1；生成=False；V01=`null`；错误末行 `["FileNotFoundError: [Errno 2] No such file or directory: '/tmp/claude-0/-home-user-MyOS2/e3ce16b3-588b-596b-86aa-5e369173435c/scratchpad/recheck-runs/phaseA-jw7vf60z/a5_missing_static_stage/static.json'"]` | 一致（缺阶段或夹具阻断时崩溃） |
| R03 fixture_blocked_compile | 旧 `make_results.py` | exit 1；生成=False；V01=`null`；错误末行 `["KeyError: 'run'"]` | 一致（缺阶段或夹具阻断时崩溃） |
| 进程组 | 旧 `run_pg`，注入 killpg 抛 ProcessLookupError | 返回=False；`ProcessLookupError: HARNESS_META_TEST injected: group already gone` | 组已消失时异常外抛，没有终态记录 |

结论：主线 R01–R04 的四项推导都由旧程序实跑复现。另外发现，空观测在其余评估器上会变成假的内核 COUNTEREVIDENCE。旧 `run_pg` 在收集超时路径上没有把未确认的子进程写成“已回收”（`child_reaped` 取自返回码），这一点没有复现为错误；组已消失的路径则会抛异常。

## 3. 修订内容（仅新增于 recheck-01/fixtures/）

| 文件 | 来源 | 处理的问题 |
|---|---|---|
| harness2.py | 新写（参考冻结 harness.py） | R02/R04：进程组有界收尾，终态明确；全新目录；钉住冻结对象 |
| frozen.py | 新写 | 所有旧文件从 a1e7c2277705 逐字节解出，不读工作树 |
| old_counterexamples.py | 新写 | Phase A：旧程序实跑 |
| identity2.py | 新写（替代冻结 run_all.identity） | R02/R04：分开核对回执与冻结输入、原件逐字节、移动头的后继关系、远端关系；任何异常都关门 |
| evaluate2.py | 修订自冻结 evaluate.py | R01：每个案例有固定的证据合同（退出码类、事件序列、字段类型、运行状态、重复一致）；只有 VALID 才与原预测比较。第二批：V08 缺静态判据时为 INCOMPLETE_EVIDENCE |
| build2.py | 修订自冻结 build.py | 展开逻辑逐行保持；模板与定位器取自冻结解出目录；用 harness2 运行；元测试注入点 |
| h00_2.py | 修订自冻结 h00_hardening.py | 新增快速退出、组已消失、收集超时三个探针；回读探针按对象读取 |
| a46_check.py | 新写 | 单独核对主线的 A46-C/A46-ASM 两条；旧 A46 结论保留 |
| run_recheck.py | 修订自冻结 run_all.py | R02：身份门→H00→只读阶段→动态夹具；退出码 0/2/3/4；执行完成度、核验发现、基础设施错误分列。第二批：静态阶段未完成时其输出不进入评估 |
| make_results2.py | 修订自冻结 make_results.py | R03：结果、层次、计数、CA 裁定都由数据计算；固定文字只放在 *_template 键。第二批：阶段输出按完成状态取用；V08 分层；CA-02/CA-06 的缺证据处理 |
| readback2.py | 修订自冻结 readback.py | R04：argparse CLI；object/branch 两种绑定 |
| meta_tests.py | 新写 | M01–M12。第二批：M09 加严，并新增静态阶段失败变体 |
| compare_runs2.py | 新写（第二批） | 比较第一批提交的观测与第二批重跑输出 |
| final_check2.py | 新写（参考冻结 final_check.py） | 发布前范围、卫生与一致性检查（results/docs 两种模式） |
| build_evidence2.py | 新写（参考冻结 build_evidence.py） | 生成本文件；不参与核验运行 |

冻结旧文件（20 个，清单一致=True）在同源运行中原样复用：`v00_anchors.py`、`v01_structure.py`、`static_checks.py`、`locate.py`、`v00_semantic_review.yaml` 与全部 C 模板。各文件的字节数与 SHA-256 首段：

| 文件 | 字节 | SHA-256[0] |
|---|---|---|
| build.py | 5927 | c6871aeb7a6f794c |
| build_evidence.py | 20214 | 0c1f65359731d6f6 |
| evaluate.py | 14653 | bba70f48e40d4ef5 |
| final_check.py | 4142 | c370015eecce89fc |
| fx_common.h | 1854 | 5c3b5baaeb652c16 |
| fx_jiffies.c | 2040 | 33bc74720fa60fe5 |
| fx_list.inc.c | 3054 | 146d23b7aa2d1e4b |
| fx_prims.c | 3605 | 0181a8e6985f37d8 |
| fx_sched.c | 10795 | 65c8f613d9854c93 |
| fx_wait.c | 15937 | 87bae891c6a2e272 |
| h00_hardening.py | 7633 | 4fd0cd4e28c6fb64 |
| harness.py | 4131 | c6d71a86a3c297e3 |
| locate.py | 11783 | 2b89aff5cc0524fa |
| make_results.py | 29260 | 0f61e45e8889e41f |
| readback.py | 3454 | e15fb7070bdcb919 |
| run_all.py | 8906 | 0a3c7205f2d156a5 |
| static_checks.py | 6372 | 2a2bc27f8f973e4d |
| v00_anchors.py | 10507 | 41e07b5bd9d970ea |
| v00_semantic_review.yaml | 6910 | 497ae5488068d98f |
| v01_structure.py | 7249 | d69d71b6eed855ce |

## 4. 修订版 H00（同源运行中的动态安全门）

| 探针 | 符合预期 | 终态 | kill | 已确认回收 | 收集超时 | 门控动态 |
|---|---|---|---|---|---|---|
| group_kill | True | timeout_group_killed | group_signalled | True | None | True |
| fast_exit | True | exited | None | True | None | True |
| group_already_gone_injected | True | timeout_group_already_gone | group_already_gone | True | None | True |
| collect_timeout_escaped_grandchild | True | timeout_group_killed_collect_timeout | group_signalled | True | True | True |
| compile_failure | True | None | None | None | None | True |
| case_exception_isolated | True | None | None | None | None | True |
| readback_criteria | True | None | None | None | None | False |

dynamic_safety_ok=`True`，transport_ok=`True`。收集超时探针中逃逸的孙进程由探针按精确 pid 清理（该进程是探针自己创建的）。

## 5. 同源复跑（M12，第二批代码）

命令（在 recheck-01/fixtures 下执行；RECHECK_ROOT 是新建的空父目录）：

```text
$ export PYTHONDONTWRITEBYTECODE=1 RECHECK_ROOT=<scratch>/recheck-runs/canonical2
$ python3 run_recheck.py --out $RECHECK_ROOT/run.json; echo "run_recheck exit=$?"
run_recheck exit=0    (stdout/stderr 均为 0 字节)
$ cp $RECHECK_ROOT/run.json ../observations/ && cp $RECHECK_ROOT/run-*/stages/{v00,v01,static,a46}.json ../observations/
```

工作目录 `/tmp/claude-0/-home-user-MyOS2/e3ce16b3-588b-596b-86aa-5e369173435c/scratchpad/recheck-runs/canonical2/run-l9jn2nzf`；状态 `COMPLETE`；execution_complete=`True`；dynamic_calls=`{"h00": 1, "fixtures": 1}`；stage_outputs_usable=`{"v00": true, "v01": true, "static": true, "a46": true}`；infrastructure_errors=`[]`；verification_findings=`["V00", "V08"]`。

只读阶段：

| 阶段 | 来源 | 退出码 | 超时 | 终态 |
|---|---|---|---|---|
| v00_anchors.py | frozen a1e7c2277705 | 0 | False | exited |
| v01_structure.py | frozen a1e7c2277705 | 0 | False | exited |
| static_checks.py | frozen a1e7c2277705 | 0 | False | exited |
| a46_check.py | recheck-01 | 0 | False | exited |

同源：冻结旧构建器和修订构建器展开出的夹具源码，SHA-256 首段为 `{"frozen_old_builder": {"fx_wait": "036edc1945a17973", "fx_sched": "657e9616e16446d0", "fx_prims": "e1788bdb3d9a2e43", "fx_jiffies": "5369f8fda7307d88"}, "revised_builder": {"fx_wait": "036edc1945a17973", "fx_sched": "657e9616e16446d0", "fx_prims": "e1788bdb3d9a2e43", "fx_jiffies": "5369f8fda7307d88"}, "identical": true}`（逐项相同=True）。

动态夹具（每个案例运行两次，比较 stdout 字节与退出码）：

| 夹具 | 状态 | 案例：退出码/终态/重复一致 |
|---|---|---|
| fx_wait | BUILT | v02_single:0/exited/True; v03_second_wake_direct:42/exited/True; v03_second_wake_via_complete:42/exited/True; v03_all_two_waiters:42/exited/True; v09_schedule_timeout_values:0/exited/True; v09_uninterruptible_wrapper:0/exited/True; v09_msleep_bounded:43/exited/True; v10_done_preset_fast_path:0/exited/True; v10_infinite_notify_during_schedule:0/exited/True; v10_finite_timeout_no_notifier:43/exited/True; v11_wait_then_notify_then_reuse:42/exited/True |
| fx_sched | BUILT | v04_noncurrent_wake:0/exited/True; v05_state_not_in_mask:0/exited/True; v06_double_wake:0/exited/True; v07_cpu_metadata:0/exited/True; v08_pick_combinations:0/exited/True; v08_sequence_idle_requeue:0/exited/True; v08_sequence_idle_switched_out_blocked:0/exited/True; v08_vruntime_requeue_order:0/exited/True |
| fx_prims | BUILT | v12_add_test_negative:0/exited/True; v13_trylock:0/exited/True |
| fx_jiffies | BUILT | v14:0/exited/True |
| fx_jiffies_control | BUILT | v14_control:0/exited/True |

逐项结果（results.yaml cases；先判证据状态，再给结果）：

| 项 | 证据状态 | 结果 | 执行的层 | 未执行的层 |
|---|---|---|---|---|
| V00 | VALID | COUNTEREVIDENCE | source_match | - |
| V01 | VALID | NO_FAILURE_IN_SCOPE | structure | - |
| V02 | VALID | OBSERVED_AS_PREDICTED | host_original_slice | - |
| V03 | VALID | OBSERVED_AS_PREDICTED | host_original_slice | - |
| V04 | VALID | OBSERVED_AS_PREDICTED | host_original_slice | smp_or_real_scheduler |
| V05 | VALID | OBSERVED_AS_PREDICTED | host_original_slice | irq_or_signal_context |
| V06 | VALID | NO_FAILURE_IN_SCOPE | host_original_slice_serial | concurrent_wakeups |
| V07 | VALID | OBSERVED_AS_PREDICTED | host_original_slice | ap_migration |
| V08 | VALID | COUNTEREVIDENCE | host_original_slice_function_level, limited_model_reachability | global_reachability |
| V09 | VALID | OBSERVED_AS_PREDICTED | host_original_slice_stub_timers | real_timer_expiry |
| V10 | VALID | OBSERVED_AS_PREDICTED | host_original_slice_scripted_interleaving | real_timer_or_scheduler |
| V11 | VALID | OBSERVED_AS_PREDICTED | local_sequence_with_scripted_interleaving | real_context_switch |
| V12 | VALID | OBSERVED_AS_PREDICTED | host_original_asm_single_thread | multicore_atomicity |
| V13 | VALID | OBSERVED_AS_PREDICTED | host_original_slice_serial | smp_stress |
| V14 | VALID | OBSERVED_AS_PREDICTED | source_and_build_reference, host_link_model | existing_elf |
| V00_A46_correction | VALID | OBSERVED_AS_PREDICTED | - | - |

V00 未通过边界检查的锚点：`[{"id": "A46", "verdict_mech": "OUTSIDE_OR_PARTIAL_DEFINITION"}]`（旧 47 条中 A46 仍失败，原文未改）。A46 更正单列：`[["A46-C", "MATCH_IN_DEFINITION", [30]], ["A46-ASM", "MATCH_IN_DEFINITION", [31]]]`。

V08 判据：`{"function_level": "OBSERVED_AS_PREDICTED", "limited_model_reachability": "COUNTEREVIDENCE_IN_LIMITED_MODEL", "global_reachability": "NOT_ESTABLISHED_BY_THIS_CHECK", "static_criteria": {"no_active_idle_state_writer": true, "no_explicit_state_change_in_rest_init": true, "single_rq_idle_assignment": true, "init_task_starts_running": true}}`；额外观察（不在 07 中）：`{"order_before": [3, 4, 5], "order_after": [4, 5, 2], "vruntime_sorted_would_be": [4, 2, 5]}`。

CA 裁定（按判据计算）：

| CA | 裁定 | 观察 |
|---|---|---|
| CA-01 | SUPPORTED_IN_SCOPE | {"V04": ["VALID", "OBSERVED_AS_PREDICTED"], "V05": ["VALID", "OBSERVED_AS_PREDICTED"], "V06": ["VALID", "NO_FAILURE_IN_SCOPE"], "V07": ["VALID", "OBSERVED_AS_PREDICTED"]} |
| CA-02 | FUNCTION_LEVEL_SUPPORTED; LIMITED_MODEL_REACHABILITY=COUNTEREVIDENCE_IN_LIMITED_MODEL; GLOBAL_REACHABILITY=NOT_ESTABLISHED_BY_THIS_CHECK | {"V08": ["VALID", "COUNTEREVIDENCE"], "result_basis": {"function_level": "OBSERVED_AS_PREDICTED", "limited_model_reachability": "COUNTEREVIDENCE_IN_LIMITED_MODEL", "global_reachability": "NOT_ESTABLISHED_BY_THIS_CHECK", "static_criteria": {"no_active_idle_state_writer": true, "no_explicit_state_change_in_rest_init": true, "single_rq_idle_assignment": true, "init_task_starts_running": true}}} |
| CA-03 | SUPPORTED_IN_SCOPE | {"V09": ["VALID", "OBSERVED_AS_PREDICTED"], "V10": ["VALID", "OBSERVED_AS_PREDICTED"], "V11": ["VALID", "OBSERVED_AS_PREDICTED"]} |
| CA-04 | SUPPORTED_IN_SCOPE | {"V02": ["VALID", "OBSERVED_AS_PREDICTED"], "V03": ["VALID", "OBSERVED_AS_PREDICTED"], "V11": ["VALID", "OBSERVED_AS_PREDICTED"]} |
| CA-05 | SUPPORTED_AT_HOST_LINK_MODEL_AND_SOURCE_AND_BUILD_REFERENCE | {"V14": ["VALID", "OBSERVED_AS_PREDICTED"], "layers": {"source_and_build_reference": "EXECUTED", "host_link_model": "EXECUTED", "existing_elf": "NOT_RUN"}} |
| CA-06 | SUPPORTED_AT_SOURCE_LEVEL | {"A01": ["MATCH_IN_DEFINITION", "SUPPORTS_WITH_NOTE"], "A35": ["MATCH_IN_DEFINITION", "SUPPORTS_WITH_NOTE"], "A36": ["MATCH_IN_DEFINITION", "SUPPORTS"], "A37": ["MATCH_IN_DEFINITION", "SUPPORTS"], "A35_under_CONFIG_BUG": true} |
| CA-07 | SUPPORTED_IN_SCOPE | {"V12": ["VALID", "OBSERVED_AS_PREDICTED"], "V13": ["VALID", "OBSERVED_AS_PREDICTED"]} |

各项检查的实测值与预测值见 results.yaml `cases[].checks`，每行为 [标签, 实测, 预测, 一致]。原始事件行见 observations/run.json 的 `fixtures.*.cases.*.run.stdout`。

### 5.1 与第一批同源运行的比较（compare_runs2.py）

```text
$ python3 compare_runs2.py f9ae2bce9a64 ../observations ../observations/rerun_vs_batch1.json
{"same_source_results_reproduced": true, "stage_output_diff_paths": {"v00.json": ["/protection/head_short12"], "v01.json": [], "static.json": [], "a46.json": []}}
compare exit=0
```

run 摘要逐项相同=`{"status": true, "exit_code": true, "execution_complete": true, "verification_findings": true, "infrastructure_errors": true, "dynamic_calls": true}`；H00 探针相同=True；evaluation 完全相同=True；夹具 stdout 与退出码逐案例相同=`{"fx_jiffies": "1/1", "fx_jiffies_control": "1/1", "fx_prims": "2/2", "fx_sched": "8/8", "fx_wait": "11/11"}`。v00.json 唯一的差异是 `protection/head_short12`，记录的是运行时的执行分支头：第一批运行时为 `a1e7c2277705`，本次为 `f9ae2bce9a64`，因为分支头已被第一批提交推进。

### 5.2 V03 守卫的边界（不声称消除全部 C 未定义行为）

守卫位于夹具替换的 `try_to_wake_up` 入口（冻结模板 fx_wait.c，未改）。在它停下之前，原函数 `swake_up_locked` 已经实际执行了以下步骤：

1. `list_header_is_empty(&q->task_list_hdr)` 判为非空，因为 count 仍为 1。
2. `list_headr_first_container(...)`（即 container_of）从链表 anchor 算出一个并不存在的 `swqueue_s` 容器地址。
3. 读取该“容器”的 `curr->task` 字段。它的位置与队列锁字重叠：layout.anchor_container_task_aliases_lock=`True`，direct.task_field_is_lock_word=`True`，via_complete.task_ptr_nonzero_while_lock_held=`True`。
4. 以该值调用 `try_to_wake_up`（直接路径在第 2 次调用时停下）。

替换函数先比较指针；若不是已知任务，就以退出码 42 停止并输出 `ttwu_invalid_task`。真实 `try_to_wake_up` 对该“任务”的解引用和写入没有执行，其后的 `list_del_init` 也没有执行。第 2、3 步的容器构造与越界读取在 C 语义上已属未定义行为；守卫只阻止了之后的危险写入，并不消除这些。

## 6. M01–M12

`meta_tests.py` 在同源运行之后执行，输入为该次 run.json 与 Phase A 记录。工作目录 `/tmp/claude-0/-home-user-MyOS2/e3ce16b3-588b-596b-86aa-5e369173435c/scratchpad/recheck-runs/canonical2-meta/meta-csmqto5c`；全部满足=True。变造输入都标为 HARNESS_META_TEST，不是 MyOS2 原函数的结果。

```text
$ export PYTHONDONTWRITEBYTECODE=1 RECHECK_ROOT=<scratch>/recheck-runs/canonical2-meta
$ python3 meta_tests.py <scratch>/recheck-runs/canonical2/run.json ../observations/old_counterexamples.json $RECHECK_ROOT/meta_tests.json
{
 "M01": true,
 "M02": true,
 "M03": true,
 "M04": true,
 "M05": true,
 "M06": true,
 "M07": true,
 "M08": true,
 "M09": true,
 "M10": true,
 "M11": true,
 "M12": true
}
meta_tests exit=0
$ cp $RECHECK_ROOT/meta_tests.json ../observations/
```

| M | 要求 | 旧入口真实结果 | 修订版结果 | 满足 |
|---|---|---|---|---|
| M01 | no valid observation -> no OBSERVED_AS_PREDICTED / NO_FAILURE / COUNTEREVIDENCE | v05 -> OBSERVED_AS_PREDICTED; dispatch false results: ['COMPARATOR_INVALID', 'COUNTEREVIDENCE', 'OBSERVED_AS_PREDICTED', 'composite'] | v05 -> INCOMPLETE_EVIDENCE; behaviour verdicts on empty input: 0 | True |
| M02 | missing label/association detected | {"drop_label_group_task_new": "OBSERVED_AS_PREDICTED", "drop_q_of_uninterruptible_mask": "COUNTEREVIDENCE", "truncate_last_two_events": "OBSERVED_AS_PREDICTED"} | {"drop_label_group_task_new": "INCOMPLETE_EVIDENCE", "drop_q_of_uninterruptible_mask": "INCOMPLETE_EVIDENCE", "truncate_last_two_events": "INCOMPLETE_EVIDENCE", "control_delivered_five_groups": "VALID"} | True |
| M03 | duplicates/conflicts/type errors/unparsable lines give an invalid-evidence state | {"duplicate_conflicting_ret": "OBSERVED_AS_PREDICTED", "wrong_field_type": "COUNTEREVIDENCE", "unparsable_line": "OBSERVED_AS_PREDICTED", "duplicate_q_dict_override": "COMPARATOR_INVALID"} | {"duplicate_conflicting_ret": "INVALID_EVIDENCE", "wrong_field_type": "INVALID_EVIDENCE", "unparsable_line": "INVALID_EVIDENCE", "duplicate_q_dict_override": "INVALID_EVIDENCE"} | True |
| M04 | unreliable runs are not behaviour evidence; not reported as kernel counterevidence | {"timed_out": "OBSERVED_AS_PREDICTED", "collect_timed_out": "OBSERVED_AS_PREDICTED", "case_status_ERROR_with_output": "OBSERVED_AS_PREDICTED", "repeat_not_identical": "OBSERVED_AS_PREDICTED", "terminal_state_killed": "OBSERVED_AS_PREDICTED"} | {"timed_out": "INVALID_EVIDENCE", "collect_timed_out": "INVALID_EVIDENCE", "case_status_ERROR_with_output": "ERROR", "repeat_not_identical": "INVALID_EVIDENCE", "terminal_state_killed": "INVALID_EVIDENCE"} | True |
| M05 | exit code checked together with events/case contract; 0/42/43 controls accepted | {"v02_valid_events_exit_42": "COUNTEREVIDENCE", "v03_stop_event_exit_0": "COUNTEREVIDENCE", "v09_msleep_step_cap_exit_0": "OBSERVED_AS_PREDICTED", "v03_exit_42_without_stop_event": "COMPARATOR_INVALID"} | {"v02_valid_events_exit_42": "INVALID_EVIDENCE", "v03_stop_event_exit_0": "INVALID_EVIDENCE", "v09_msleep_step_cap_exit_0": "INVALID_EVIDENCE", "v03_exit_42_without_stop_event": "INVALID_EVIDENCE", "controls_delivered": {"exit_0_v02": "VALID", "exit_42_v03": "VALID", "exit_43_v09": "VALID"}} | True |
| M06 | identity false/exception -> zero dynamic calls, accurate blocked/error record | {"gate_false": 2, "identity_raises": 2, "h00_fails": 2} | {"gate_false": [3, "BLOCKED_IDENTITY", {"h00": 0, "stage": 0, "fixtures": 0}], "identity_raises": [3, "ERROR_IDENTITY", {"h00": 0, "stage": 0, "fixtures": 0}], "real_identity_on_bad_repo": [3, "ERROR_IDENTITY", {"h00": 0, "stage": 0, "fixtures": 0}]} | True |
| M07 | failed/timed-out H00 blocks dynamic fixtures; independent read-only check continues; partial report | {"stage_script": ["h00_hardening.py", "v00_anchors.py", "v01_structure.py", "static_checks.py"], "fixtures": 1, "evaluate": 1} | {"h00_probe_failed": [2, 0, ["VALID", "NO_FAILURE_IN_SCOPE"], ["BLOCKED", null]], "h00_raised_timeout": [2, 0, ["VALID", "NO_FAILURE_IN_SCOPE"], ["BLOCKED", null]]} | True |
| M08 | no hard-coded NO_FAILURE / fixed conclusions in YAML, text or CA summary | {"baseline_real_replay": ["NO_FAILURE_IN_SCOPE", "EXECUTED"], "v01_paths_missing_and_parse_failed": ["NO_FAILURE_IN_SCOPE", "EXECUTED"], "missing_static_stage": null, "fixture_blocked_compile": null} | {"real_base": ["COUNTEREVIDENCE", "NO_FAILURE_IN_SCOPE", "SUPPORTED_AT_SOURCE_LEVEL"], "v01_referenced_paths_missing": ["COUNTEREVIDENCE", "COUNTEREVIDENCE", "SUPPORTED_AT_SOURCE_LEVEL"], "v01_parse_failure": ["COUNTEREVIDENCE", "COUNTEREVIDENCE", "SUPPORTED_AT_SOURCE_LEVEL"], "v00_A46_changed_to_match": ["OBSERVED_AS_PREDICTED", "NO_FAILURE_IN_SCOPE", "SUPPORTED_AT_SOURCE_LEVEL"], "v00_A35_changed_to_quote_not_found": ["COUNTEREVIDENCE", "NO_FAILURE_IN_SCOPE", "CONTRADICTED_OR_UNSUPPORTED_AT_SOURCE_LEVEL"]} | True |
| M09 | no crash; full list of not-executed layers, affected CA, still-valid items; complete control = M12 | {"missing_static_stage": {"returncode": 1, "generated": false, "error_tail": ["FileNotFoundError: [Errno 2] No such file or directory: '/tmp/claude-0/-home-user-MyOS2/e3ce16b3-588b-596b-86aa-5e369173435c/scratchpad/recheck-runs/phaseA-jw7vf60z/a5_missing_static_stage/static.json'"]}, "fixture_blocked_compile": {"returncode": 1, "generated": false, "error_tail": ["KeyError: 'run'"]}} | {"exit": 2, "cases": {"V00": ["VALID", "COUNTEREVIDENCE"], "V01": ["VALID", "NO_FAILURE_IN_SCOPE"], "V02": ["VALID", "OBSERVED_AS_PREDICTED"], "V03": ["VALID", "OBSERVED_AS_PREDICTED"], "V04": ["VALID", "OBSERVED_AS_PREDICTED"], "V05": ["VALID", "OBSERVED_AS_PREDICTED"], "V06": ["ERROR", null], "V07": ["VALID", "OBSERVED_AS_PREDICTED"], "V08": ["INCOMPLETE_EVIDENCE", null], "V09": ["VALID", "OBSERVED_AS_PREDICTED"], "V10": ["VALID", "OBSERVED_AS_PREDICTED"], "V11": ["VALID", "OBSERVED_AS_PREDICTED"], "V12": ["BLOCKED", null], "V13": ["BLOCKED", null], "V14": ["INCOMPLETE_EVIDENCE", null], "V00_A46_correction": ["VALID", "OBSERVED_AS_PREDICTED"]}, "ca": {"CA-01": "UNDETERMINED_MISSING_EVIDENCE", "CA-02": "FUNCTION_LEVEL_SUPPORTED; LIMITED_MODEL_REACHABILITY=UNDETERMINED_STATIC_INPUT_MISSING; GLOBAL_REACHABILITY=NOT_ESTABLISHED_BY_THIS_CHECK", "CA-03": "SUPPORTED_IN_SCOPE", "CA-04": "SUPPORTED_IN_SCOPE", "CA-05": "UNDETERMINED_MISSING_EVIDENCE", "CA-06": "SUPPORTED_AT_SOURCE_LEVEL", "CA-07": "UNDETERMINED_MISSING_EVIDENCE"}, "static_stage_failed": {"exit": 2, "V08": ["INCOMPLETE_EVIDENCE", null], "V14": ["INCOMPLETE_EVIDENCE", null], "CA-02": "FUNCTION_LEVEL_SUPPORTED; LIMITED_MODEL_REACHABILITY=UNDETERMINED_STATIC_INPUT_MISSING; GLOBAL_REACHABILITY=NOT_ESTABLISHED_BY_THIS_CHECK", "CA-05": "UNDETERMINED_MISSING_EVIDENCE"}} | True |
| M10 | legit successor replays; wrong branch / non-descendant / changed originals / bad remote are rejected | old identity has no head/branch/originals inputs; it flags the legit advance as a mismatch and the run continues anyway | {"legit_successor_head": [true, []], "legit_equal_head": [true, []], "legit_local_ahead_of_remote": [true, []], "wrong_branch_name": [false, ["on_execution_branch"]], "non_descendant_head": [false, ["head_descends_from_reviewed_core", "originals_unchanged", "remote_descends_from_reviewed_core"]], "pilot_original_changed": [false, ["originals_unchanged"]], "core_original_changed": [false, ["originals_unchanged"]], "remote_reset_to_master": [false, ["remote_descends_from_reviewed_core"]], "remote_ahead_of_local": [false, ["remote_on_same_line"]]} | True |
| M11 | --out parsed with/without field=value; HTTP/timeout/object/bytes/field all in status | {"no_field_with_out": 1, "with_field_with_out": 1} | {"with_field_object": 0, "without_field_object": 0, "with_field_branch_mode": 0, "missing_path": 1, "wrong_field_value": 1, "malformed_field": 2, "injected_curl_timeout": false} | True |
| M12 | frozen sources/predictions, full valid data -> scoped conclusions; old A46 still fails; A46-C/ASM checked separately | n/a (this row is the real replay) | {"V02": ["VALID", "OBSERVED_AS_PREDICTED"], "V03": ["VALID", "OBSERVED_AS_PREDICTED"], "V04": ["VALID", "OBSERVED_AS_PREDICTED"], "V05": ["VALID", "OBSERVED_AS_PREDICTED"], "V06": ["VALID", "NO_FAILURE_IN_SCOPE"], "V07": ["VALID", "OBSERVED_AS_PREDICTED"], "V09": ["VALID", "OBSERVED_AS_PREDICTED"], "V10": ["VALID", "OBSERVED_AS_PREDICTED"], "V11": ["VALID", "OBSERVED_AS_PREDICTED"], "V12": ["VALID", "OBSERVED_AS_PREDICTED"], "V13": ["VALID", "OBSERVED_AS_PREDICTED"], "V08": ["VALID", "COUNTEREVIDENCE"], "V14_model": ["VALID", "OBSERVED_AS_PREDICTED"], "V00": ["VALID", "COUNTEREVIDENCE"], "V01": ["VALID", "NO_FAILURE_IN_SCOPE"], "V00_A46_correction": ["VALID", "OBSERVED_AS_PREDICTED"]} | True |

M10 明细（临时仓库：`git init + alternates (read-only) + copied shallow file; objects written only to scratch`；会话仓库浅克隆=true；主仓库对象库未变=True）：

| 变体 | gate | 关门原因 | 远端关系 |
|---|---|---|---|
| legit_successor_head | True | - | equal |
| legit_equal_head | True | - | equal |
| legit_local_ahead_of_remote | True | - | remote_is_ancestor_of_head |
| wrong_branch_name | False | on_execution_branch | equal |
| non_descendant_head | False | head_descends_from_reviewed_core, originals_unchanged, remote_descends_from_reviewed_core | equal |
| pilot_original_changed | False | originals_unchanged | equal |
| core_original_changed | False | originals_unchanged | equal |
| remote_reset_to_master | False | remote_descends_from_reviewed_core | remote_is_ancestor_of_head |
| remote_ahead_of_local | False | remote_on_same_line | remote_is_descendant_of_head |

M11 明细（readback2 CLI，真实网络读取；超时变体为注入）：

| 变体 | 参数尾 | 退出码 | ok | 通道 |
|---|---|---|---|---|
| with_field_object | ["transport_marker=MYOS2-CLOUD-PILOT-20260925-K7P4", "--out", "<file>"] | 0 | True | [{"http_code": "200", "curl_exit": 0, "timed_out": false, "byte_identical": true, "field_ok": true, "ok": true}, {"http_code": "200", "curl_exit": 0, "timed_out": false, "byte_identical": true, "field_ok": true, "ok": true}] |
| without_field_object | ["--out", "<file>"] | 0 | True | [{"http_code": "200", "curl_exit": 0, "timed_out": false, "byte_identical": true, "field_ok": true, "ok": true}, {"http_code": "200", "curl_exit": 0, "timed_out": false, "byte_identical": true, "field_ok": true, "ok": true}] |
| with_field_branch_mode | ["transport_marker=MYOS2-CLOUD-PILOT-20260925-K7P4", "--mode", "branch", "--out", "<file>"] | 0 | True | [{"http_code": "200", "curl_exit": 0, "timed_out": false, "byte_identical": true, "field_ok": true, "ok": true}, {"http_code": "200", "curl_exit": 0, "timed_out": false, "byte_identical": true, "field_ok": true, "ok": true}] |
| missing_path | ["--out", "<file>"] | 1 | False | [{"http_code": "404", "curl_exit": 0, "timed_out": false, "byte_identical": false, "field_ok": true, "ok": false}, {"http_code": "404", "curl_exit": 0, "timed_out": false, "byte_identical": false, "field_ok": true, "ok": false}] |
| wrong_field_value | ["transport_marker=WRONG-HARNESS_META_TEST", "--out", "<file>"] | 1 | False | [{"http_code": "200", "curl_exit": 0, "timed_out": false, "byte_identical": true, "field_ok": false, "ok": false}, {"http_code": "200", "curl_exit": 0, "timed_out": false, "byte_identical": true, "field_ok": false, "ok": false}] |
| malformed_field | ["transport_marker", "--out", "<file>"] | 2 | None | null |
| injected_curl_timeout | null | None | False | [{"timed_out": true, "transport_ok": false, "ok": false}, {"timed_out": true, "transport_ok": false, "ok": false}] |

M09 明细：

- 部分运行（注入 `{"broken_fixture": "fx_prims", "case_error": ["fx_sched", "v06_double_wake"], "skip_stage": "static_checks.py"}`）：exit 2，状态 PARTIAL，YAML 可生成=True。V08=`["INCOMPLETE_EVIDENCE", null]`，V14=`["INCOMPLETE_EVIDENCE", null]`。未执行或无效的层：`["V04.smp_or_real_scheduler", "V05.irq_or_signal_context", "V06.host_original_slice_serial", "V06.concurrent_wakeups", "V07.ap_migration", "V08.limited_model_reachability", "V08.global_reachability", "V09.real_timer_expiry", "V10.real_timer_or_scheduler", "V11.real_context_switch", "V12.host_original_asm_single_thread", "V12.multicore_atomicity", "V13.host_original_slice_serial", "V13.smp_stress", "V14.source_and_build_reference", "V14.existing_elf"]`。CA：`{"CA-01": "UNDETERMINED_MISSING_EVIDENCE", "CA-02": "FUNCTION_LEVEL_SUPPORTED; LIMITED_MODEL_REACHABILITY=UNDETERMINED_STATIC_INPUT_MISSING; GLOBAL_REACHABILITY=NOT_ESTABLISHED_BY_THIS_CHECK", "CA-03": "SUPPORTED_IN_SCOPE", "CA-04": "SUPPORTED_IN_SCOPE", "CA-05": "UNDETERMINED_MISSING_EVIDENCE", "CA-06": "SUPPORTED_AT_SOURCE_LEVEL", "CA-07": "UNDETERMINED_MISSING_EVIDENCE"}`。
- 变体 B（第二批新增）：static_checks 真实运行并写出 static.json（真实退出码 0），但记录为退出 1；夹具记录取自 M12 运行的真实记录，H00 为替身。结果：exit 2，状态 PARTIAL，static.json 仍在阶段目录=True，stage_outputs_usable=`{"v00": true, "v01": true, "static": false, "a46": true}`。V08=`["INCOMPLETE_EVIDENCE", null]`，V14=`["INCOMPLETE_EVIDENCE", null]`，CA-02=`FUNCTION_LEVEL_SUPPORTED; LIMITED_MODEL_REACHABILITY=UNDETERMINED_STATIC_INPUT_MISSING; GLOBAL_REACHABILITY=NOT_ESTABLISHED_BY_THIS_CHECK`，CA-05=`UNDETERMINED_MISSING_EVIDENCE`。

## 7. 与冻结 core 的差异

逐项结果与冻结 `results.yaml @ 7e2fa84a9823` 比较，结果不同的项：`[]`。CA 裁定的差异：

| CA | 冻结 | 本次 | 相同 |
|---|---|---|---|
| CA-01 | SUPPORTED_IN_SCOPE | SUPPORTED_IN_SCOPE | True |
| CA-02 | FUNCTION_LEVEL_SUPPORTED_REACHABILITY_COUNTEREVIDENCE | FUNCTION_LEVEL_SUPPORTED; LIMITED_MODEL_REACHABILITY=COUNTEREVIDENCE_IN_LIMITED_MODEL; GLOBAL_REACHABILITY=NOT_ESTABLISHED_BY_THIS_CHECK | False |
| CA-03 | SUPPORTED_IN_SCOPE | SUPPORTED_IN_SCOPE | True |
| CA-04 | SUPPORTED_IN_SCOPE | SUPPORTED_IN_SCOPE | True |
| CA-05 | SUPPORTED_AT_SOURCE_AND_MODEL_LEVEL | SUPPORTED_AT_HOST_LINK_MODEL_AND_SOURCE_AND_BUILD_REFERENCE | False |
| CA-06 | SUPPORTED_AT_SOURCE_LEVEL | SUPPORTED_AT_SOURCE_LEVEL | True |
| CA-07 | SUPPORTED_BY_HOST_ORIGINAL_SLICE | SUPPORTED_IN_SCOPE | False |

结果层面没有新的内核反证：冻结 core 的 15 项结论在完整有效数据上重现。区别在于，本次每一项都先通过证据合同，裁定由数据计算，名称也更细：CA-02 拆出了有限模型与全局可达性，CA-05 列出了实际执行的层。

## 8. 开发期与发布后的修正

- 开发运行 dev1（run_recheck 首次完整运行）即全部 VALID；没有为迎合输出改动任何案例合同。
- 编写 M10 时发现 identity2 的缺口：远端分支若被重置到更早的提交，它仍是本地 HEAD 的祖先。已加入 `remote_descends_from_reviewed_core`，由 M10 的 remote_reset_to_master 变体验证。
- run_recheck 在自定义 identity 抛异常时原本会落到内部错误分支；改为按关门处理（ERROR_IDENTITY，退出码 3，动态调用 0）。
- M09 判据中有一处 and/or 优先级错误，已修正，并补入 CA-05 未定的检查。
- M10 首版用 `git clone --shared`。在浅克隆的会话仓库上，该选项只复制分支可达的对象，导致 identity 在临时仓库中抛异常；改为 git init + alternates + 复制 shallow 边界。
- readback2 CLI 会在 --out 同目录新建 rbwork-* 下载目录。第一批时这两个目录建在 observations/ 下，发布前已移出（未入库）；第二批的回读把 --out 指向会话临时目录，再复制 JSON。
- **第一批提交之后（发布后修正）**：整理 MANIFEST 时复核 M09 的部分运行记录，发现 static_checks 阶段缺失时，V08 仍为 VALID/OBSERVED_AS_PREDICTED（只剩函数层结论），limited_model_reachability 层仍记为 EXECUTED。这与 R01/R03 是同一类问题，只是影响限于部分运行，同源运行不受影响。第二批的修正：V08 缺静态判据时为 INCOMPLETE_EVIDENCE，不给案例级结论；阶段输出只在该阶段完成时使用；CA-02 在这种情况下只保留函数层部分；CA-06 与 V14 源码层要求 V00 证据有效。M09 相应加严，并新增变体 B。修正后同源运行与 M01–M12 各重跑一次，§5.1 显示同源结果与第一批相同。
- 以上都是验证器或元测试自身的问题；被测 MyOS2 原函数文本与 07/09 预测均未改动。

## 9. 提交、推送与远端回读

### 9.1 第一批 `f9ae2bce9a64`（fixtures 12 个文件、observations 7 个 JSON、results.yaml）

推送输出（手工转录）：

```text
To https://github.com/08822407d/MyOS2
   a1e7c22..f9ae2bc  claude/dazzling-cori-q0dnyt -> claude/dazzling-cori-q0dnyt
```

```text
$ python3 readback2.py f9ae2bce9a64 <recheck-01/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode object --out ../observations/readback_results.object.json
readback object exit=0
$ python3 readback2.py f9ae2bce9a64 <recheck-01/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode branch --out ../observations/readback_results.branch.json
readback branch exit=0
```

- object：ok=True，分支头关系=不适用（object 模式按提交读取），期望 39087 字节；通道 `[["raw.githubusercontent.com", "200", 0, false, 39087, true, true], ["api.github.com_contents_raw", "200", 0, false, 39087, true, true]]`
- branch：ok=True，分支头关系=equal，期望 39087 字节；通道 `[["raw.githubusercontent.com", "200", 0, false, 39087, true, true], ["api.github.com_contents_raw", "200", 0, false, 39087, true, true]]`
- 推送后身份门（after_batch1/identity_after_push.json）：gate.ok=`True`，执行=`{"branch": "claude/dazzling-cori-q0dnyt", "head_short12": "f9ae2bce9a64", "remote_relation": "equal"}`。

### 9.2 第二批 `efb9846b88ec`（修正、重跑输出、重新生成的 results.yaml）

提交前运行 `final_check2.py results`，输出见 §9.4。推送输出（手工转录）：

```text
To https://github.com/08822407d/MyOS2
   f9ae2bc..efb9846  claude/dazzling-cori-q0dnyt -> claude/dazzling-cori-q0dnyt
branch 'claude/dazzling-cori-q0dnyt' set up to track 'origin/claude/dazzling-cori-q0dnyt'.
```

```text
$ python3 readback2.py efb9846b88ec <recheck-01/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode object --out <scratch>/readback-batch2/readback_results.object.json
readback object exit=0
$ python3 readback2.py efb9846b88ec <recheck-01/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode branch --out <scratch>/readback-batch2/readback_results.branch.json
readback branch exit=0
$ python3 -c "import identity2, harness2 as H; H.emit(identity2.check(), '<scratch>/readback-batch2/identity_after_push.json')"
identity exit=0
$ cp <scratch>/readback-batch2/*.json ../observations/after_batch2/   (cmp 逐字节相同)
```

- object：ok=True，分支头关系=不适用（object 模式按提交读取），期望 39525 字节；通道 `[["raw.githubusercontent.com", "200", 0, false, 39525, true, true], ["api.github.com_contents_raw", "200", 0, false, 39525, true, true]]`
- branch：ok=True，分支头关系=equal，期望 39525 字节；通道 `[["raw.githubusercontent.com", "200", 0, false, 39525, true, true], ["api.github.com_contents_raw", "200", 0, false, 39525, true, true]]`
- 推送后身份门（after_batch2/identity_after_push.json）：gate.ok=`True`，执行=`{"branch": "claude/dazzling-cori-q0dnyt", "head_short12": "efb9846b88ec", "remote_relation": "equal"}`。执行分支已两次前进（a1e7c2277705 → f9ae2bce9a64 → efb9846b88ec），旧原件不变，门仍然打开。这就是 R04 要求的“合法续提交、旧原件不变、可重放”的真实实例。

### 9.3 第三批（本文件）

results.yaml 自第二批提交后冻结。第三批只新增 evidence.md、MANIFEST.md、build_evidence2.py 与 observations/after_batch2/ 的三份记录；提交前运行 `final_check2.py docs efb9846b88ec`，输出见 §9.5。

### 9.4 第二批提交前的检查（final_check2.py results）

```json
{
 "check": "FINAL_SCOPE_RECHECK_01",
 "mode": "results",
 "branch": "claude/dazzling-cori-q0dnyt",
 "head_short12": "f9ae2bce9a64",
 "remote_branch_equals_head": true,
 "remote_master_equals_pin": true,
 "diff_base": "merge-base(HEAD, remote master) = de3bb1df906a",
 "index_vs_base_count": 52,
 "outside_pilot_core": [],
 "non_added_vs_base": [],
 "since_core_frozen_outside_recheck": [],
 "since_core_frozen_non_added": [],
 "pilot_changed_since_pilot_head": [],
 "worktree_outside_recheck": [],
 "recheck_unstaged_modifications": [],
 "recheck_untracked": [
  "fixtures/build_evidence2.py"
 ],
 "recheck_files_staged": [
  "fixtures/a46_check.py",
  "fixtures/build2.py",
  "fixtures/compare_runs2.py",
  "fixtures/evaluate2.py",
  "fixtures/final_check2.py",
  "fixtures/frozen.py",
  "fixtures/h00_2.py",
  "fixtures/harness2.py",
  "fixtures/identity2.py",
  "fixtures/make_results2.py",
  "fixtures/meta_tests.py",
  "fixtures/old_counterexamples.py",
  "fixtures/readback2.py",
  "fixtures/run_recheck.py",
  "observations/a46.json",
  "observations/after_batch1/identity_after_push.json",
  "observations/after_batch1/readback_results.branch.json",
  "observations/after_batch1/readback_results.object.json",
  "observations/meta_tests.json",
  "observations/old_counterexamples.json",
  "observations/rerun_vs_batch1.json",
  "observations/run.json",
  "observations/static.json",
  "observations/v00.json",
  "observations/v01.json",
  "results.yaml"
 ],
 "new_or_changed_vs_head": [
  "A\tfixtures/compare_runs2.py",
  "M\tfixtures/evaluate2.py",
  "A\tfixtures/final_check2.py",
  "M\tfixtures/make_results2.py",
  "M\tfixtures/meta_tests.py",
  "M\tfixtures/run_recheck.py",
  "A\tobservations/after_batch1/identity_after_push.json",
  "A\tobservations/after_batch1/readback_results.branch.json",
  "A\tobservations/after_batch1/readback_results.object.json",
  "M\tobservations/meta_tests.json",
  "A\tobservations/rerun_vs_batch1.json",
  "M\tobservations/run.json",
  "M\tobservations/v00.json",
  "M\tresults.yaml"
 ],
 "binary_like": [],
 "hex40_hits": {},
 "secret_pattern_hits": [],
 "json_parse_failures": [],
 "python_syntax_failures": [],
 "python_without_provenance_header": [],
 "results_yaml_reproduced_from_observations": true,
 "results_yaml": {
  "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
  "status": "generated_from_run_status_COMPLETE",
  "generator_errors": 0
 },
 "all_as_expected": true
}
```

### 9.5 第三批提交前的检查（final_check2.py docs efb9846b88ec）

```json
{
 "check": "FINAL_SCOPE_RECHECK_01",
 "mode": "docs",
 "branch": "claude/dazzling-cori-q0dnyt",
 "head_short12": "efb9846b88ec",
 "remote_branch_equals_head": true,
 "remote_master_equals_pin": true,
 "diff_base": "merge-base(HEAD, remote master) = de3bb1df906a",
 "index_vs_base_count": 58,
 "outside_pilot_core": [],
 "non_added_vs_base": [],
 "since_core_frozen_outside_recheck": [],
 "since_core_frozen_non_added": [],
 "pilot_changed_since_pilot_head": [],
 "worktree_outside_recheck": [],
 "recheck_unstaged_modifications": [],
 "recheck_untracked": [],
 "recheck_files_staged": [
  "MANIFEST.md",
  "evidence.md",
  "fixtures/a46_check.py",
  "fixtures/build2.py",
  "fixtures/build_evidence2.py",
  "fixtures/compare_runs2.py",
  "fixtures/evaluate2.py",
  "fixtures/final_check2.py",
  "fixtures/frozen.py",
  "fixtures/h00_2.py",
  "fixtures/harness2.py",
  "fixtures/identity2.py",
  "fixtures/make_results2.py",
  "fixtures/meta_tests.py",
  "fixtures/old_counterexamples.py",
  "fixtures/readback2.py",
  "fixtures/run_recheck.py",
  "observations/a46.json",
  "observations/after_batch1/identity_after_push.json",
  "observations/after_batch1/readback_results.branch.json",
  "observations/after_batch1/readback_results.object.json",
  "observations/after_batch2/identity_after_push.json",
  "observations/after_batch2/readback_results.branch.json",
  "observations/after_batch2/readback_results.object.json",
  "observations/meta_tests.json",
  "observations/old_counterexamples.json",
  "observations/rerun_vs_batch1.json",
  "observations/run.json",
  "observations/static.json",
  "observations/v00.json",
  "observations/v01.json",
  "results.yaml"
 ],
 "new_or_changed_vs_head": [
  "A\tMANIFEST.md",
  "A\tevidence.md",
  "A\tfixtures/build_evidence2.py",
  "A\tobservations/after_batch2/identity_after_push.json",
  "A\tobservations/after_batch2/readback_results.branch.json",
  "A\tobservations/after_batch2/readback_results.object.json"
 ],
 "binary_like": [],
 "hex40_hits": {},
 "secret_pattern_hits": [],
 "json_parse_failures": [],
 "python_syntax_failures": [],
 "python_without_provenance_header": [],
 "results_yaml_reproduced_from_observations": true,
 "results_yaml": {
  "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
  "status": "generated_from_run_status_COMPLETE",
  "generator_errors": 0
 },
 "results_commit": "efb9846b88ec",
 "results_yaml_changed_since_results_commit": false,
 "non_added_since_results_commit": [],
 "docs": {
  "MANIFEST.md": {
   "yaml_parses": true,
   "first_key": "task_id",
   "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
   "verified_tag_count": 0,
   "other_pass_token_lines": []
  },
  "evidence.md": {
   "yaml_parses": true,
   "first_key": "task_id",
   "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
   "verified_tag_count": 0,
   "other_pass_token_lines": []
  },
  "results.yaml": {
   "yaml_parses": true,
   "first_key": "task_id",
   "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
   "verified_tag_count": 0,
   "other_pass_token_lines": []
  }
 },
 "manifest_self_check_consistent": true,
 "startup_selfcheck_quote_in_conventions": true,
 "manifest_data_summary_mismatches": [],
 "manifest_file_list_matches_staged": true,
 "manifest_tables": {
  "V_rows": 15,
  "M_rows": 12,
  "CA_rows": 7,
  "mismatches": []
 },
 "evidence_results_commit_matches": true,
 "all_as_expected": true
}
```

