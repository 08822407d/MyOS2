---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-CHECK-01-RECHECK-02
phase: evidence_consumer_recheck
record_type: consumer_fix_evidence
produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"
execution_model_selection: unknown_or_not_attestable
execution_model_selection_source: "执行者所在平台的规则不允许在推送到仓库的产物中写模型标识（执行者自述）"
date: "2026-10-01"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；主线 b0aa54db1b70（开工时固定）；被审输入 b843d475367a；recheck-01 results 冻结于 efb9846b88ec（短 SHA）"
results_commit_short12: 77faf51e9a43
status: final_for_recheck_02
transcription: "§1.1 与 §9 的推送输出是会话命令的手工转录；其余数值、表格与输出由 fixtures/build_evidence3.py 从 observations/ 与 results.yaml 生成。observations 下的 JSON/YAML 是程序写出的原文件。"
file_moves: "old_side_s01_s03.json、guard_start.json、frozen_b843_manifest_start.json 由修订前的首次运行写在会话临时目录，随后逐字节复制；after_results/ 三份记录同样先写在临时目录再复制（cmp 相同）。readback2 的 rbwork-* 下载目录未入库。"
redaction: "未发现需脱敏内容；push 输出经 40 位十六进制过滤（无命中）。"
open_questions: []
---

# RECHECK-02 证据：数据读取层的旧负例、修订、N01–N06 与已交观测复判

**S01–S03 先在未改的 b843 程序与已交观测上实跑，三项主线推导全部复现。新读取层在 N01–N06 中全部满足；N05 还暴露出一处主线未列的同类缺口（S04），已一并修正。已交观测按新读取层复判，与 efb9846b88ec 的冻结结果逐项相同。** 本轮只做 Python 数据层实验：没有编译或运行 C 夹具，没有重做 H00，没有全树扫描，没有改内核。

## 1. 输入与绑定

### 1.1 读取任务书与回执（手工转录）

```text
$ curl -sS -o $S/13.md -w 'http=%{http_code} bytes=%{size_download}\n' https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/13-core-evidence-consumer-recheck.md
http=200 bytes=14502
$ git show origin/agent/MYOS2-LEAD-002:<同路径> | cmp - $S/13.md && echo '13 raw == git object'
13 raw == git object
$ curl ... reviews/CORE-CHECK-01-recheck-01-review.md
http=200 bytes=13217
review raw == git object
```

开工时 origin/agent/MYOS2-LEAD-002 位于 `b0aa54db1b70`，相对上一轮固定的 `ec62453e76d2` 只新增四个文件（下表 lead_changes）。本轮把 `b0aa54db1b70` 固定为主线对象。14 号文件（调度回插后续）不属于本次执行范围，只读未执行。

### 1.2 开工时的保护门（guard3，修订前）

```json
{
 "gate": {
  "review_receipt_binding": true,
  "contract13_binding": true,
  "lead_advanced_by_additions_only": true,
  "inputs_unchanged": true,
  "results_chain": true,
  "on_execution_branch": true,
  "head_descends_from_b843": true,
  "b843_files_unchanged_only_additions_under_prefix": true,
  "remote_on_same_line_and_contains_b843": true,
  "kernel_source_branch_equals_pin": true,
  "auxiliary_identity2_open": true,
  "ok": true
 },
 "review_fields": {
  "record_id": "CORE-CHECK-01-RECHECK-01-REVIEW-001",
  "disposition": "RETURN",
  "reviewed_pr": 17,
  "reviewed_branch": "claude/dazzling-cori-q0dnyt",
  "reviewed_commit_short12": "b843d475367a",
  "results_frozen_short12": "efb9846b88ec",
  "first_batch_short12": "f9ae2bce9a64",
  "previous_core_short12": "a1e7c2277705",
  "followup_id": "CORE-CHECK-01-RECHECK-02",
  "followup_allowed_write_prefix": "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/consumer-fix-02/"
 },
 "contract13_fields": {
  "followup_id": "CORE-CHECK-01-RECHECK-02",
  "required_review_record": "CORE-CHECK-01-RECHECK-01-REVIEW-001",
  "execution_branch": "claude/dazzling-cori-q0dnyt",
  "execution_pr": 17,
  "reviewed_input_short12": "b843d475367a",
  "frozen_results_short12": "efb9846b88ec",
  "allowed_write_prefix": "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/consumer-fix-02/",
  "old_files_must_remain_unchanged": true,
  "kernel_fixture_execution_authorized": false,
  "full_tree_scan_authorized": false
 },
 "lead_changes_since_previous_pin": [
  "A\t13-core-evidence-consumer-recheck.md",
  "A\t14-scheduler-ordering-followup.md",
  "A\tcheckpoints/2026-10-01-recheck01-reviewed-consumer-fix-ready.md",
  "A\treviews/CORE-CHECK-01-recheck-01-review.md"
 ],
 "efb_to_b843": [
  "A\tMANIFEST.md",
  "A\tevidence.md",
  "A\tfixtures/build_evidence2.py",
  "A\tobservations/after_batch2/identity_after_push.json",
  "A\tobservations/after_batch2/readback_results.branch.json",
  "A\tobservations/after_batch2/readback_results.object.json"
 ],
 "b843_files": {
  "under_recheck01": 32,
  "under_results_root": 58,
  "whole_tree": 1228
 },
 "execution": {
  "branch": "claude/dazzling-cori-q0dnyt",
  "head_short12": "b843d475367a",
  "remote_relation": "equal"
 },
 "remote_refs_equal_pins": {
  "master": true,
  "time": true,
  "agent/MYOS2-LEAD-002": true
 }
}
```

门的含义：回执与 13 号绑定；主线只追加；a1e7 → f9ae → efb → b843 链成立，且 efb → b843 只新增六件、results.yaml 未变；HEAD、索引、工作树相对 b843 只在 consumer-fix-02/ 下新增；远端同线。冻结的 recheck-01 identity2 从 b843 解出后作为辅助运行，它只保护 pilot/core 原件，b843 全部 32 件由上面的新增检查覆盖。

### 1.3 复用的冻结程序

从 `b843d475367a` 逐字节解出 recheck-01 的 15 个程序与 14 个观测到全新目录（清单一致=True/True，与 HEAD 相同=True）。本轮实际导入或运行的模块：

| b843 路径（recheck-01/ 下） | 字节 | SHA-256[0] | 用途 |
|---|---|---|---|
| fixtures/evaluate2.py | 38706 | 6a61e29a3b0b1c80 | S01/S02 旧侧；新读取层在进程内包装 validate_case 与 judge |
| fixtures/harness2.py | 7501 | 9b9c5c23698a13a1 | make_results2/identity2/readback2 的依赖 |
| fixtures/identity2.py | 8884 | b4e36c60fa2dc53d | guard3 的辅助检查 |
| fixtures/make_results2.py | 22174 | dbeda2acada94bc1 | S03 旧 CLI；新入口复用 build_doc/render |
| fixtures/readback2.py | 6039 | 41019aeb61216b01 | 远端回读 CLI |

每个新侧调用都在导入前把解出目录与 b843 对象逐字节比对（frozen_b843.import_frozen）；不读工作树中的旧文件。

## 2. S01–S03：冻结程序实跑（修订之前）

`fixtures/old_side.py`，工作目录 `/tmp/claude-0/-home-user-MyOS2/e3ce16b3-588b-596b-86aa-5e369173435c/scratchpad/recheck02/dev-oldside/old-side-yg3q_ak8`。每个实验是独立 Python 进程（30 s 上限），以 b843 解出目录为工作目录。变造输入只写在该目录下，标为 HARNESS_META_TEST。

| 项 | 输入 | 旧程序真实返回 | 主线预测 | 复现 |
|---|---|---|---|---|
| S01 | 实际 run.json 的 fx_sched/v04_noncurrent_wake，删去 run 中 `collect_timed_out`、`terminal_state`、`reaped_confirmed`、`stderr`（其余字段剩 ["cmd", "elapsed_s", "group_members_after", "kill", "output_lost", "pgid", "pgid_differs_from_own", "pid", "returncode", "stdout", "timed_out", "timeout_s"]） | `VALID / OBSERVED_AS_PREDICTED` | VALID / OBSERVED_AS_PREDICTED | True |
| S01 对照 | 未改记录 | `VALID / OBSERVED_AS_PREDICTED` | - | - |
| S02 empty_referenced_paths | 实际 v01.json，清空 referenced_paths | `VALID / NO_FAILURE_IN_SCOPE` | VALID / NO_FAILURE_IN_SCOPE | True |
| S02 empty_report_inputs_read | 实际 v01.json，清空 report_inputs_read | `VALID / NO_FAILURE_IN_SCOPE` | VALID / NO_FAILURE_IN_SCOPE | True |
| S02 empty_hex40_hits_in_scope_files | 实际 v01.json，清空 hex40_hits_in_scope_files | `VALID / NO_FAILURE_IN_SCOPE` | VALID / NO_FAILURE_IN_SCOPE | True |
| S02 empty_all_three | 实际 v01.json，三处同时清空 | `VALID / NO_FAILURE_IN_SCOPE` | VALID / NO_FAILURE_IN_SCOPE | True |
| S02 对照 | 未改 v01.json | `VALID / NO_FAILURE_IN_SCOPE` | - | - |
| S03 对照 | 完整复制七个输入，冻结 make_results2 CLI | exit 0；报告与 efb 的 results.yaml 逐字节相同=True | - | - |
| S03 缺 static.json | 其余六个输入原样 | exit 0；顶层 `generated_from_run_status_COMPLETE`；V08 `VALID/COUNTEREVIDENCE`，限定模型 `COUNTEREVIDENCE_IN_LIMITED_MODEL`；V14 `INCOMPLETE_EVIDENCE`；CA-02 `FUNCTION_LEVEL_SUPPORTED; LIMITED_MODEL_REACHABILITY=COUNTEREVIDENCE_IN_LIMITED_MODEL; GLOBAL_REACHABILITY=NOT_ESTABLISHED_BY_THIS_CHECK` | V14 源码层缺证据而 V08 沿用缓存 VALID | True |
| S03 坏 JSON | static.json 内容只有 `{` | exit 1；报告写出=False；stderr 末行 `["json.decoder.JSONDecodeError: Expecting property name enclosed in double quotes: line 1 column 2 (char 1)"]` | 错误收集前抛 JSONDecodeError，无部分报告 | True |

结论：三项推导全部由旧程序实际复现，没有反证。S03 中旧入口在缺 static.json 时顶层仍写 `generated_from_run_status_COMPLETE`：源运行当时完成的事实被当成了当前交接包的状态。

## 3. 修订（只新增于 consumer-fix-02/fixtures/）

| 文件 | 作用 |
|---|---|
| common3.py | 固定对象、全新目录、30 s 子进程、拒绝 40 位十六进制的输出 |
| frozen_b843.py | 从 b843 逐字节解出旧程序与观测；导入前逐字节核对 |
| guard3.py | 回执/13 号绑定、主线只追加、结果链、b843 全部原件不变、远端同线；辅助运行冻结 identity2 |
| old_side.py | S01–S03 旧侧实跑（修订前运行，之后未改） |
| expected_objects3.py + expected_objects.json | S02：从冻结输入（taskbook 的 07 报告头、map、MANIFEST self_check）一次提取应检查的对象集合；final_check3 会重新提取并比较 |
| consumer3.py | S01：RAN 案例的必需字段缺失→INCOMPLETE_EVIDENCE [MISSING_FIELD]，类型错→INVALID_EVIDENCE [WRONG_TYPE]，冻结检查漏掉的显式失败（output_lost、重复运行终态）→INVALID_EVIDENCE；S02：v01 对象集合与预期比较；S03：每个输入文件的读取/结构状态，按当前可用原件重算全部项目，不用缓存判定；S04：judge 修正 |
| make_results3.py | 完整入口：读取→结构/覆盖→各项消费状态→YAML；退出码 0 完整、2 部分（报告已写）、4 读取器自身失败 |
| consumer_tests.py | N01–N06（N05 复制了 meta_tests 的 M01–M05/M08/M09 变造逻辑，未导入或调用会运行 C 的旧入口） |
| make_summary3.py | 由 observations 生成 results.yaml（同输入同字节） |
| final_check3.py / build_evidence3.py | 发布前检查与本文件生成；不参与核验 |

冻结的 evaluate2/make_results2 文件本身没有改动。新读取层在自己的进程里把 `evaluate2.validate_case` 与 `evaluate2.judge` 换成包装函数：包装先取冻结函数的原判定，再追加本轮的检查，所以冻结检查已拒绝的情形照样拒绝。

**S04（本轮发现，13 号未列）**：冻结 `judge` 用观测值去比一个固定的“错误预测”做负控。若一条完整、有效的观测恰好等于该错误值（V04 `ret=1`，V01 hex40 命中数 1），旧程序返回 `ERROR / COMPARATOR_INVALID`，把一个真实发现记成基础设施错误。修正：只有当负控的错误值与预测值本身相同（负控无意义）时才保留 ERROR；否则按发现判定。实际旧/新返回：`{"N02.control_complete_hex40_hit_found": [["ERROR", null], ["VALID", "COUNTEREVIDENCE"]], "N05.valid_but_different_from_prediction_v04_ret_1": [["ERROR", null], ["VALID", "COUNTEREVIDENCE"]]}`。

## 4. N01–N06

`fixtures/consumer_tests.py`，工作目录 `/tmp/claude-0/-home-user-MyOS2/e3ce16b3-588b-596b-86aa-5e369173435c/scratchpad/recheck02/official/tests/consumer-tests-pj4dud6e`。全部满足=True。旧侧是 b843 程序，新侧是 consumer3/make_results3；每个实验都是独立进程（30 s 上限）。

### N01：missing run metadata is never VALID; error class recorded; explicit failures still rejected; complete control valid（满足=True）

| 变体 | 旧 | 新 | 新错误类别 | 满足 |
|---|---|---|---|---|
| delete_run.collect_timed_out | ["VALID", "OBSERVED_AS_PREDICTED"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_FIELD"] | True |
| delete_run.terminal_state | ["VALID", "OBSERVED_AS_PREDICTED"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_FIELD"] | True |
| delete_run.reaped_confirmed | ["VALID", "OBSERVED_AS_PREDICTED"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_FIELD"] | True |
| delete_run.stderr | ["VALID", "OBSERVED_AS_PREDICTED"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_FIELD"] | True |
| delete_all_four | ["VALID", "OBSERVED_AS_PREDICTED"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_FIELD"] | True |
| delete_case.repeat_terminal_state | ["VALID", "OBSERVED_AS_PREDICTED"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_FIELD"] | True |
| delete_run.output_lost | ["VALID", "OBSERVED_AS_PREDICTED"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_FIELD"] | True |
| explicit_collect_timed_out_true | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | [] | True |
| explicit_terminal_state_killed | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | [] | True |
| explicit_reaped_confirmed_false | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | [] | True |
| explicit_stderr_nonempty | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | [] | True |
| explicit_output_lost_true | ["VALID", "OBSERVED_AS_PREDICTED"] | ["INVALID_EVIDENCE", null] | ["EXPLICIT_FAILURE"] | True |
| explicit_repeat_terminal_state_killed | ["VALID", "OBSERVED_AS_PREDICTED"] | ["INVALID_EVIDENCE", null] | ["EXPLICIT_FAILURE"] | True |
| wrong_type_reaped_confirmed_string | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | ["WRONG_TYPE"] | True |
| control_complete_real_record | ["VALID", "OBSERVED_AS_PREDICTED"] | ["VALID", "OBSERVED_AS_PREDICTED"] | null | True |

### N02：missing checked objects are missing evidence, not 'no failure'; legal empty error lists stay valid; a real False is a finding（满足=True）

| 变体 | 旧 | 新 | 新错误类别 | 满足 |
|---|---|---|---|---|
| drop_one_referenced_path | ["VALID", "NO_FAILURE_IN_SCOPE"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_OBJECT"] | True |
| empty_referenced_paths | ["VALID", "NO_FAILURE_IN_SCOPE"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_OBJECT"] | True |
| drop_one_report_input | ["VALID", "NO_FAILURE_IN_SCOPE"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_OBJECT"] | True |
| empty_report_inputs_read | ["VALID", "NO_FAILURE_IN_SCOPE"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_OBJECT"] | True |
| drop_one_hex40_scope_file | ["VALID", "NO_FAILURE_IN_SCOPE"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_OBJECT"] | True |
| empty_hex40_hits_in_scope_files | ["VALID", "NO_FAILURE_IN_SCOPE"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_OBJECT"] | True |
| empty_all_three | ["VALID", "NO_FAILURE_IN_SCOPE"] | ["INCOMPLETE_EVIDENCE", null] | ["MISSING_OBJECT"] | True |
| duplicate_report_input | ["VALID", "NO_FAILURE_IN_SCOPE"] | ["INVALID_EVIDENCE", null] | ["DUPLICATE_OBJECT"] | True |
| renamed_referenced_path_member | ["VALID", "NO_FAILURE_IN_SCOPE"] | ["INVALID_EVIDENCE", null] | ["MEMBER_IDENTITY"] | True |
| control_complete_empty_error_lists | ["VALID", "NO_FAILURE_IN_SCOPE"] | ["VALID", "NO_FAILURE_IN_SCOPE"] | null | True |
| control_member_exists_false | ["VALID", "COUNTEREVIDENCE"] | ["VALID", "COUNTEREVIDENCE"] | null | True |
| control_complete_hex40_hit_found | ["ERROR", null] | ["VALID", "COUNTEREVIDENCE"] | null | True |

N02 的完整对照中，记录里的错误列表本来就是空的：`{"anchor_refs.dangling": [], "case_refs.dangling": [], "contract09_anchor_refs_dangling": [], "p9_wording_hits_in_scope_files": {}}`。这类空集合是合法结果，不被拒绝；清空的是“应检查的对象清单”时才判缺证据。

### N03：no cached verdict hides a missing original; V08 keeps only the independent function layer; independent items unaffected（满足=True）

| 变体 | 旧 CLI | 新 CLI | 新报告中非 VALID 的项 | 新报告的输入问题 |
|---|---|---|---|---|
| control_full | exit 0；报告=True；[""] | exit 0；consumer_COMPLETE | [] | {} |
| omit_static | exit 0；报告=True；[""] | exit 2；consumer_PARTIAL | ["V08", "V14"] | {"static.json": "static.json MISSING"} |
| omit_v01 | exit 0；报告=True；[""] | exit 2；consumer_PARTIAL | ["V01"] | {"v01.json": "v01.json MISSING"} |

判据：`{"control_complete_exit0": true, "omit_static_partial_exit2": true, "omit_static_V08_not_valid_function_layer_kept": true, "omit_static_CA02_function_level_only": true, "omit_static_V14_source_layer_not_executed": true, "omit_static_cache_not_used": true, "omit_static_independent_items_unchanged": true, "omit_v01_partial_exit2": true, "omit_v01_V01_not_valid": true, "omit_v01_independent_items_unchanged": true, "source_run_status_kept_as_history": true}`

### N04：error/partial report written with matching exit code; no behaviour verdict for affected items; no dynamic claim without run data（满足=True）

| 变体 | 旧 CLI | 新 CLI | 新报告中非 VALID 的项 | 新报告的输入问题 |
|---|---|---|---|---|
| static_syntax_open_brace | exit 1；报告=False；["json.decoder.JSONDecodeError: Expecting property name enclosed in double quotes: line 1 column 2 (char 1)"] | exit 2；consumer_PARTIAL | ["V08", "V14"] | {"static.json": "static.json JSON_SYNTAX_ERROR: JSONDecodeError: Expecting property name enclosed in double quotes: line 1 column 2 (char 1)"} |
| static_empty_file | exit 1；报告=False；["json.decoder.JSONDecodeError: Expecting value: line 1 column 1 (char 0)"] | exit 2；consumer_PARTIAL | ["V08", "V14"] | {"static.json": "static.json EMPTY"} |
| static_wrong_top_type_list | exit 0；报告=True；[""] | exit 2；consumer_PARTIAL | ["V08", "V14"] | {"static.json": "static.json WRONG_TOP_TYPE: top-level list, expected object"} |
| v01_syntax_open_brace | exit 1；报告=False；["json.decoder.JSONDecodeError: Expecting property name enclosed in double quotes: line 1 column 2 (char 1)"] | exit 2；consumer_PARTIAL | ["V01"] | {"v01.json": "v01.json JSON_SYNTAX_ERROR: JSONDecodeError: Expecting property name enclosed in double quotes: line 1 column 2 (char 1)"} |
| v01_empty_file | exit 1；报告=False；["json.decoder.JSONDecodeError: Expecting value: line 1 column 1 (char 0)"] | exit 2；consumer_PARTIAL | ["V01"] | {"v01.json": "v01.json EMPTY"} |
| v01_wrong_top_type_list | exit 0；报告=True；[""] | exit 2；consumer_PARTIAL | ["V01"] | {"v01.json": "v01.json WRONG_TOP_TYPE: top-level list, expected object"} |
| run_unparsable_control | exit 1；报告=False；["json.decoder.JSONDecodeError: Expecting property name enclosed in double quotes: line 1 column 2 (char 1)"] | exit 2；consumer_PARTIAL | ["V00", "V00_A46_correction", "V01", "V02", "V03", "V04", "V05", "V06", "V07", "V08", "V09", "V10", "V11", "V12", "V13", "V14"] | {"run.json": "run.json JSON_SYNTAX_ERROR: JSONDecodeError: Expecting property name enclosed in double quotes: line 1 column 2 (char 1)", "v00.json": "stage completion record unavailable (run.json JSON_SYNTAX_ERROR: JSONDecodeError: Expecting property name enclosed in double quotes: line 1 column 2 (char 1))", "v01.json": "stage completion record unavailable (run.json JSON_SYNTAX_ERROR: JSONDecodeError: Expecting property name enclosed in double quotes: line 1 column 2 (char 1))", "static.json": "stage completion record unavailable (run.json JSON_SYNTAX_ERROR: JSONDecodeError: Expecting property name enclosed in double quotes: line 1 column 2 (char 1))", "a46.json": "stage completion record unavailable (run.json JSON_SYNTAX_ERROR: JSONDecodeError: Expecting property name enclosed in double quotes: line 1 column 2 (char 1))"} |
| consumer_failure_control | - | exit 4；错误记录已写=True | [] | null |

判据：`{"static_syntax_open_brace": true, "static_empty_file": true, "static_wrong_top_type_list": true, "v01_syntax_open_brace": true, "v01_empty_file": true, "v01_wrong_top_type_list": true, "run_unparsable_control": true, "consumer_failure_control": true}`

### N05：old rejections still hold, legal 0/42/43 and a valid-but-different record still accepted; N02/N03 transforms through the real entry（满足=True）

| 变体 | 旧（b843） | 新 | 满足 |
|---|---|---|---|
| M01_empty_cases | per-item: {'V02': ['INCOMPLETE_EVIDENCE', None], 'V03': ['INCOMPLETE_EVIDENCE', None], 'V04': ['INCOMPLETE_EVIDENCE', None], 'V05': ['INCOMPLETE_EVIDENCE', None], 'V06': ['INCOMPLETE_EVIDENCE', None], 'V07': ['INCOMPLETE_EVIDENCE', None], 'V09': ['INCOMPLETE_EVIDENCE', None], 'V10': ['INCOMPLETE_EVIDENCE', None], 'V11': ['INCOMPLETE_EVIDENCE', None], 'V12': ['INCOMPLETE_EVIDENCE', None], 'V13': ['INCOMPLETE_EVIDENCE', None], 'V08': ['INCOMPLETE_EVIDENCE', None], 'V14_model': ['INCOMPLETE_EVIDENCE', None]} | per-item: {'V02': ['INCOMPLETE_EVIDENCE', None], 'V03': ['INCOMPLETE_EVIDENCE', None], 'V04': ['INCOMPLETE_EVIDENCE', None], 'V05': ['INCOMPLETE_EVIDENCE', None], 'V06': ['INCOMPLETE_EVIDENCE', None], 'V07': ['INCOMPLETE_EVIDENCE', None], 'V09': ['INCOMPLETE_EVIDENCE', None], 'V10': ['INCOMPLETE_EVIDENCE', None], 'V11': ['INCOMPLETE_EVIDENCE', None], 'V12': ['INCOMPLETE_EVIDENCE', None], 'V13': ['INCOMPLETE_EVIDENCE', None], 'V08': ['INCOMPLETE_EVIDENCE', None], 'V14_model': ['INCOMPLETE_EVIDENCE', None]} | True |
| M02_drop_label_task_new | ["INCOMPLETE_EVIDENCE", null] | ["INCOMPLETE_EVIDENCE", null] | True |
| M02_drop_q_uninterruptible | ["INCOMPLETE_EVIDENCE", null] | ["INCOMPLETE_EVIDENCE", null] | True |
| M02_truncate_last_two | ["INCOMPLETE_EVIDENCE", null] | ["INCOMPLETE_EVIDENCE", null] | True |
| M03_duplicate_conflicting_ret | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | True |
| M03_wrong_field_type | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | True |
| M03_unparsable_line | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | True |
| M03_duplicate_q | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | True |
| M04_timed_out | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | True |
| M04_collect_timed_out | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | True |
| M04_case_status_ERROR | ["ERROR", null] | ["ERROR", null] | True |
| M04_repeat_not_identical | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | True |
| M04_terminal_state_killed | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | True |
| M05_v02_exit_42 | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | True |
| M05_v03_stop_event_exit_0 | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | True |
| M05_v09_step_cap_exit_0 | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | True |
| M05_v03_exit_42_without_stop_event | ["INVALID_EVIDENCE", null] | ["INVALID_EVIDENCE", null] | True |
| control_exit_0_v02 | ["VALID", "OBSERVED_AS_PREDICTED"] | ["VALID", "OBSERVED_AS_PREDICTED"] | True |
| control_exit_42_v03 | ["VALID", "OBSERVED_AS_PREDICTED"] | ["VALID", "OBSERVED_AS_PREDICTED"] | True |
| control_exit_43_v09 | ["VALID", "OBSERVED_AS_PREDICTED"] | ["VALID", "OBSERVED_AS_PREDICTED"] | True |
| valid_but_different_from_prediction_v04_ret_1 | ["ERROR", null] | ["VALID", "COUNTEREVIDENCE"] | True |

经真实读取/成文入口（make_results3 CLI）：

| 变体 | 旧 CLI 退出码 | 新 CLI | 新报告中非 VALID 的项 | 满足 |
|---|---|---|---|---|
| M08_v01_member_exists_false | 0 | exit 0，consumer_COMPLETE | {} | True |
| M08_v00_A46_changed_to_match | 0 | exit 0，consumer_COMPLETE | {} | True |
| M08_v00_A35_changed_to_quote_not_found | 0 | exit 0，consumer_COMPLETE | {} | True |
| M09_partial_compile_case_skipped_stage | 0 | exit 2，consumer_PARTIAL | {"V06": ["ERROR", null], "V08": ["INCOMPLETE_EVIDENCE", null], "V12": ["BLOCKED", null], "V13": ["BLOCKED", null], "V14": ["INCOMPLETE_EVIDENCE", null]} | True |
| M09B_static_stage_failed_output_present | 0 | exit 2，consumer_PARTIAL | {"V08": ["INCOMPLETE_EVIDENCE", null], "V14": ["INCOMPLETE_EVIDENCE", null]} | True |
| N02_through_cli_empty_referenced_paths | 0 | exit 2，consumer_PARTIAL | {"V01": ["INCOMPLETE_EVIDENCE", null]} | True |
| N02_through_cli_duplicate_report_input | 0 | exit 2，consumer_PARTIAL | {"V01": ["INVALID_EVIDENCE", null]} | True |

## 5. 已交观测复判（N06，RECORDED_OBSERVATION_REVALIDATION）

不是新的宿主实跑：用新入口读取 b843 中未改的观测，重新判读。源运行事实与本次消费检查分列：

| | 源运行（历史事实） | 本次消费检查 |
|---|---|---|
| 身份/日期 | identity 读取时间 2026-09-27T08:58:15Z；执行头 f9ae2bce9a64 | 2026-10-01；consumer-fix-02/fixtures/consumer3.py + make_results3.py; frozen evaluate2/make_results2 from b843d475367a |
| 状态 | COMPLETE（exit 0） | COMPLETE |
| 输入 | 运行时写出 | [["run.json", "OK"], ["v00.json", "OK"], ["v01.json", "OK"], ["static.json", "OK"], ["a46.json", "OK"], ["old_counterexamples.json", "OK"], ["meta_tests.json", "OK"]] |

判据：`{"exit0_consumer_complete": true, "all_cases_identical_to_efb": true, "ca_rulings_identical": true, "counts_identical": true, "r_m_items_identical": true, "old_A46_failure_kept": true, "A46_C_ASM_listed_separately": true, "not_run_layers_not_upgraded": true, "cached_equals_consumer": true}`。与 efb 冻结结果相比：不同的项 `{}`，CA 全部相同=True，counts 相同=True；缓存判定与消费判定不同的项 `[]`。

| 项 | 证据状态 | 结果 |
|---|---|---|
| V00 | VALID | COUNTEREVIDENCE |
| V01 | VALID | NO_FAILURE_IN_SCOPE |
| V02 | VALID | OBSERVED_AS_PREDICTED |
| V03 | VALID | OBSERVED_AS_PREDICTED |
| V04 | VALID | OBSERVED_AS_PREDICTED |
| V05 | VALID | OBSERVED_AS_PREDICTED |
| V06 | VALID | NO_FAILURE_IN_SCOPE |
| V07 | VALID | OBSERVED_AS_PREDICTED |
| V08 | VALID | COUNTEREVIDENCE |
| V09 | VALID | OBSERVED_AS_PREDICTED |
| V10 | VALID | OBSERVED_AS_PREDICTED |
| V11 | VALID | OBSERVED_AS_PREDICTED |
| V12 | VALID | OBSERVED_AS_PREDICTED |
| V13 | VALID | OBSERVED_AS_PREDICTED |
| V14 | VALID | OBSERVED_AS_PREDICTED |
| V00_A46_correction | VALID | OBSERVED_AS_PREDICTED |

原 A46 失败保留，A46-C/A46-ASM 分列；未执行层（`["V04.smp_or_real_scheduler", "V05.irq_or_signal_context", "V06.concurrent_wakeups", "V07.ap_migration", "V08.global_reachability", "V09.real_timer_expiry", "V10.real_timer_or_scheduler", "V11.real_context_switch", "V12.multicore_atomicity", "V13.smp_stress", "V14.existing_elf"]`）没有升级。完整复判报告：observations/n06_revalidation.yaml。

## 6. 未完成与限制

- 本轮只判读已交观测：数据完整时得到的仍是 recheck-01 已有的受限结论，没有新增内核证据。CA-02 全局可达性、V08 回插顺序候选（14 号 CORE-SCHED-ORDER-02 未在此执行）、真实 ELF/IRQ/SMP/上下文切换继续开放。
- 重复运行的一致性只能读 producer 写下的布尔摘要与第二次的退出码/终态；第二次运行的原始 stdout 没有保存，本轮不补造。
- 没有 run.json 时，阶段完成记录也随之缺失：本读取层把依赖阶段完成的静态项一并判为证据不完整（N04 run_unparsable_control）。这是保守选择，也可以另行约定接受独立的阶段文件。
- 结构检查只覆盖读取层实际使用的字段与对象集合，不是任意损坏或恶意输入的通用证明。
- S04 的修正只改变负控判定的一种情形；冻结 evaluate2 中各案例的预测与负控取值没有改动。
- 执行模型未知；没有 CI 或人工逐行复核。

## 7. 对“无调用者”说法的撤回（导航）

- 撤回对象：PR #17 旧摘要中“`swake_up_all_locked` 与 `finish_swait` 在 swait.c 外无调用者”。recheck-01 已交的 static_checks 扫描没有覆盖这一点，撤回不等于已经发现调用者。PR 正文已于 2026-10-01 更正，主线 recheck-01 回执 §2 已登记接受。
- 仍保留的范围限定说法：`msleep` 在 time a039d9803ade 的 mykernel/ 下 843 个 `*.c/*.h/*.S/*.lds` 文件中、按 static_checks 的文本正则与去注释规则，只命中 timer_api.h 中的声明（recheck-01/observations/static.json 的 V09）。它不是全局不可达的证明。
- 本轮没有为这条说法重新扫描源码。

## 8. 过程记录

- 开发运行中，N02 的 hex40 命中对照与 N05 的“有效但与预测不同”对照在新侧也得到 ERROR，由此发现 S04。修正 consumer3 后重跑；正式记录是修正后的一次完整运行。
- 提交前保护门第一次关闭（observations/guard_pre_results_closed_stray_pyc.json）：本会话早先在旧 recheck-01/fixtures 下运行一条只读查看命令时没有设 PYTHONDONTWRITEBYTECODE，生成了未入库的 `__pycache__/evaluate2.cpython-311.pyc`。没有 b843 文件被改，但它在写区之外。已删除这个本会话生成的缓存文件后重跑保护门，门打开（guard_pre_results.json）。
- old_side.py 只在修订前运行一次，其记录即正式记录；之后该文件未改。
- 以上都是读取层或测试本身的问题；没有改动被测原函数、07/09 预测或任何 b843 文件。

## 9. 提交、推送与回读

结果批 `77faf51e9a43`（fixtures 11 个文件、observations 8 个文件、results.yaml），提交前运行 `final_check3.py results`（§9.1）。推送输出（手工转录）：

```text
To https://github.com/08822407d/MyOS2
   b843d47..77faf51  claude/dazzling-cori-q0dnyt -> claude/dazzling-cori-q0dnyt
```

```text
$ cd <b843 解出目录>/fixtures
$ python3 readback2.py 77faf51e9a43 <consumer-fix-02/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode object --out <scratch>/readback/readback_results.object.json
readback object exit=0
$ python3 readback2.py 77faf51e9a43 <consumer-fix-02/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode branch --out <scratch>/readback/readback_results.branch.json
readback branch exit=0
$ python3 guard3.py <b843 解出目录> <scratch>/readback/guard_after_push.json
guard exit=0
$ cp <scratch>/readback/*.json ../observations/after_results/   (cmp 逐字节相同)
```

- object：ok=True，分支头关系=不适用（object 模式按提交读取），期望 33576 字节；通道 `[["raw.githubusercontent.com", "200", 0, false, 33576, true, true], ["api.github.com_contents_raw", "200", 0, false, 33576, true, true]]`
- branch：ok=True，分支头关系=equal，期望 33576 字节；通道 `[["raw.githubusercontent.com", "200", 0, false, 33576, true, true], ["api.github.com_contents_raw", "200", 0, false, 33576, true, true]]`
- 推送后保护门：gate.ok=`True`，执行=`{"branch": "claude/dazzling-cori-q0dnyt", "head_short12": "77faf51e9a43", "remote_relation": "equal"}`，相对 b843 的已提交变化 20 项、全部是 consumer-fix-02/ 下的新增。

文档批（本文件、MANIFEST.md、build_evidence3.py、after_results/ 三份记录）在其后提交；提交前运行 `final_check3.py docs 77faf51e9a43`（§9.2）。

### 9.1 结果批提交前的检查

```json
{
 "check": "FINAL_SCOPE_CONSUMER_FIX_02",
 "mode": "results",
 "branch": "claude/dazzling-cori-q0dnyt",
 "head_short12": "b843d475367a",
 "remote_branch_equals_head": true,
 "remote_master_equals_pin": true,
 "diff_base": "merge-base(HEAD, remote master) = de3bb1df906a",
 "index_vs_base_count": 78,
 "outside_pilot_core": [],
 "non_added_vs_base": [],
 "since_b843_not_addition_under_prefix": [],
 "worktree_outside_prefix": [],
 "prefix_unstaged_modifications": [],
 "prefix_untracked": [],
 "prefix_files_staged": [
  "fixtures/common3.py",
  "fixtures/consumer3.py",
  "fixtures/consumer_tests.py",
  "fixtures/expected_objects.json",
  "fixtures/expected_objects3.py",
  "fixtures/final_check3.py",
  "fixtures/frozen_b843.py",
  "fixtures/guard3.py",
  "fixtures/make_results3.py",
  "fixtures/make_summary3.py",
  "fixtures/old_side.py",
  "observations/consumer_tests.json",
  "observations/frozen_b843_manifest_start.json",
  "observations/frozen_b843_manifest_tests.json",
  "observations/guard_pre_results.json",
  "observations/guard_pre_results_closed_stray_pyc.json",
  "observations/guard_start.json",
  "observations/n06_revalidation.yaml",
  "observations/old_side_s01_s03.json",
  "results.yaml"
 ],
 "new_or_changed_vs_head": [
  "A\tfixtures/common3.py",
  "A\tfixtures/consumer3.py",
  "A\tfixtures/consumer_tests.py",
  "A\tfixtures/expected_objects.json",
  "A\tfixtures/expected_objects3.py",
  "A\tfixtures/final_check3.py",
  "A\tfixtures/frozen_b843.py",
  "A\tfixtures/guard3.py",
  "A\tfixtures/make_results3.py",
  "A\tfixtures/make_summary3.py",
  "A\tfixtures/old_side.py",
  "A\tobservations/consumer_tests.json",
  "A\tobservations/frozen_b843_manifest_start.json",
  "A\tobservations/frozen_b843_manifest_tests.json",
  "A\tobservations/guard_pre_results.json",
  "A\tobservations/guard_pre_results_closed_stray_pyc.json",
  "A\tobservations/guard_start.json",
  "A\tobservations/n06_revalidation.yaml",
  "A\tobservations/old_side_s01_s03.json",
  "A\tresults.yaml"
 ],
 "binary_like": [],
 "hex40_hits": {},
 "secret_pattern_hits": [],
 "json_parse_failures": [],
 "python_syntax_failures": [],
 "python_without_provenance_header": [],
 "results_yaml_reproduced_from_observations": true,
 "expected_objects_reproduced_from_frozen_inputs": true,
 "n06_report_matches_consumer_tests_record": true,
 "results_yaml": {
  "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
  "followup_id": "CORE-CHECK-01-RECHECK-02",
  "all_N_met": true
 },
 "all_as_expected": true
}
```

### 9.2 文档批提交前的检查

```json
{
 "check": "FINAL_SCOPE_CONSUMER_FIX_02",
 "mode": "docs",
 "branch": "claude/dazzling-cori-q0dnyt",
 "head_short12": "77faf51e9a43",
 "remote_branch_equals_head": true,
 "remote_master_equals_pin": true,
 "diff_base": "merge-base(HEAD, remote master) = de3bb1df906a",
 "index_vs_base_count": 84,
 "outside_pilot_core": [],
 "non_added_vs_base": [],
 "since_b843_not_addition_under_prefix": [],
 "worktree_outside_prefix": [],
 "prefix_unstaged_modifications": [],
 "prefix_untracked": [],
 "prefix_files_staged": [
  "MANIFEST.md",
  "evidence.md",
  "fixtures/build_evidence3.py",
  "fixtures/common3.py",
  "fixtures/consumer3.py",
  "fixtures/consumer_tests.py",
  "fixtures/expected_objects.json",
  "fixtures/expected_objects3.py",
  "fixtures/final_check3.py",
  "fixtures/frozen_b843.py",
  "fixtures/guard3.py",
  "fixtures/make_results3.py",
  "fixtures/make_summary3.py",
  "fixtures/old_side.py",
  "observations/after_results/guard_after_push.json",
  "observations/after_results/readback_results.branch.json",
  "observations/after_results/readback_results.object.json",
  "observations/consumer_tests.json",
  "observations/frozen_b843_manifest_start.json",
  "observations/frozen_b843_manifest_tests.json",
  "observations/guard_pre_results.json",
  "observations/guard_pre_results_closed_stray_pyc.json",
  "observations/guard_start.json",
  "observations/n06_revalidation.yaml",
  "observations/old_side_s01_s03.json",
  "results.yaml"
 ],
 "new_or_changed_vs_head": [
  "A\tMANIFEST.md",
  "A\tevidence.md",
  "A\tfixtures/build_evidence3.py",
  "A\tobservations/after_results/guard_after_push.json",
  "A\tobservations/after_results/readback_results.branch.json",
  "A\tobservations/after_results/readback_results.object.json"
 ],
 "binary_like": [],
 "hex40_hits": {},
 "secret_pattern_hits": [],
 "json_parse_failures": [],
 "python_syntax_failures": [],
 "python_without_provenance_header": [],
 "results_yaml_reproduced_from_observations": true,
 "expected_objects_reproduced_from_frozen_inputs": true,
 "n06_report_matches_consumer_tests_record": true,
 "results_yaml": {
  "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
  "followup_id": "CORE-CHECK-01-RECHECK-02",
  "all_N_met": true
 },
 "results_commit": "77faf51e9a43",
 "results_yaml_changed_since_results_commit": false,
 "non_added_since_results_commit": [],
 "docs": {
  "MANIFEST.md": {
   "yaml_parses": true,
   "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
   "verified_tag_count": 0,
   "other_pass_token_lines": []
  },
  "evidence.md": {
   "yaml_parses": true,
   "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
   "verified_tag_count": 0,
   "other_pass_token_lines": []
  },
  "results.yaml": {
   "yaml_parses": true,
   "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
   "verified_tag_count": 0,
   "other_pass_token_lines": []
  }
 },
 "manifest_self_check_consistent": true,
 "manifest_data_summary_mismatches": [],
 "manifest_file_list_matches_staged": true,
 "evidence_results_commit_matches": true,
 "all_as_expected": true
}
```

