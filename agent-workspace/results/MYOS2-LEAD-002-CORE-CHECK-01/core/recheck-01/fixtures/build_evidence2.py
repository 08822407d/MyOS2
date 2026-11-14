# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file (publication tool; not used by the verification runs); derived in structure from
#   core/fixtures/build_evidence.py @ a1e7c2277705 (frozen, unchanged)
# purpose: assemble recheck-01/evidence.md from observations/ and results.yaml. Values, tables and
#   quoted outputs are generated from those files; the few session commands and push outputs are marked
#   as manual transcription in the text.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 build_evidence2.py <recheck-01 dir> <out.md> <batch1_short12> <results_commit_short12>
                                     [final_check_results.txt] [final_check_docs.txt]"""
import json
import os
import re
import sys

import yaml

D, OUT, C1, C2 = sys.argv[1:5]
FC_RESULTS = sys.argv[5] if len(sys.argv) > 5 else None
FC_DOCS = sys.argv[6] if len(sys.argv) > 6 else None
obs = lambda n: json.load(open(os.path.join(D, "observations", n), encoding="utf-8"))
run, oldc, meta, cmp_ = obs("run.json"), obs("old_counterexamples.json"), obs("meta_tests.json"), obs("rerun_vs_batch1.json")
rb1 = {m: obs("after_batch1/readback_results.%s.json" % m) for m in ("object", "branch")}
rb2 = {m: obs("after_batch2/readback_results.%s.json" % m) for m in ("object", "branch")}
idp1, idp2 = obs("after_batch1/identity_after_push.json"), obs("after_batch2/identity_after_push.json")
res = yaml.safe_load(open(os.path.join(D, "results.yaml"), encoding="utf-8"))
T = {t["id"]: t for t in meta.get("tests", [])}
L = []
w = L.append


def fence(x, lang="json"):
    w("```" + lang)
    w(x if isinstance(x, str) else json.dumps(x, ensure_ascii=False, indent=1))
    w("```")


def cell(x):
    return json.dumps(x, ensure_ascii=False).replace("|", "/") if not isinstance(x, str) else x.replace("|", "/")


def check_of(case_id, label):
    for c in res.get("cases", []):
        if c.get("case_id") == case_id:
            for ch in c.get("checks", []):
                if ch[0] == label:
                    return ch[1]
    return None


w("---")
for k, v in [("task_id", "MYOS2-LEAD-002-CORE-CHECK-01"), ("packet_id", "MYOS2-LEAD-002-CORE-CHECK-01"),
             ("followup_id", "CORE-CHECK-01-RECHECK-01"), ("phase", "core_recheck_01"), ("record_type", "verifier_recheck_evidence")]:
    w("%s: %s" % (k, v))
w('produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"')
w("execution_model_selection: unknown_or_not_attestable")
w('execution_model_selection_source: "执行者所在平台的规则不允许在推送到仓库的产物中写模型标识（执行者自述）"')
w('date: "2026-09-27"')
w('base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；taskbook 57a7c3e0eebf；主线回执头 ec62453e76d2；冻结 core a1e7c2277705（短 SHA）"')
w("batch1_commit_short12: %s" % C1)
w("results_commit_short12: %s" % C2)
w("status: final_for_recheck_01")
w('transcription: "§1.1、§1.3 与 §9 中的推送输出是会话内命令的手工转录；其余数值、表格与输出由 fixtures/build_evidence2.py 从 observations/ 与 results.yaml 生成。observations 下的 JSON 是程序写出的原文件，未做换行或脱敏变换。"')
w('file_moves: "observations/after_batch1/ 的三个文件在第一批推送后写在 observations/ 根下，第二批提交前原样移入该子目录（内容未变）；after_batch2/ 的三个文件先写在会话临时目录，再逐字节复制（cmp 相同）。readback2 在 --out 同目录建立的 rbwork-* 下载目录没有入库。第一批的 run.json、v00.json、meta_tests.json 与 results.yaml 被第二批重跑的输出替换，旧版本保留在提交 %s 中。"' % C1)
w('redaction: "未发现需脱敏内容。push 输出经 40 位十六进制替换过滤（实际无命中）；回读 URL 中的完整提交号在程序内替换为 <commit:短号>。"')
w("open_questions: []")
w("---")
w("")
w("# CORE-CHECK-01 RECHECK-01 证据：旧反例实跑、验证器修订、M01–M12 与同源复跑")
w("")
w("**先用冻结旧验证器实跑了主线给出的四类反例，全部复现。修订后的验证器在 M01–M12 中逐项满足，并完成同源 V00–V14 复跑（run exit %s，状态 %s）。** 第一批提交 `%s` 之后，在整理文档时又发现一处同类缺口：静态输入缺失时，V08 仍被判为有效。已在第二批 `%s` 中修正，并重跑一次同源运行与 M01–M12。所有执行都是本次云端会话中的普通用户态进程，不是 MyOS2 在 CPU 上运行。"
  % (run.get("exit_code"), run.get("status"), C1, C2))
w("")
w("## 1. 输入、身份与准入")
w("")
w("### 1.1 任务书与回执读取（手工转录）")
w("")
fence("$ curl -sS -o $S/12.md -w 'http=%{http_code} bytes=%{size_download}\\n' https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/12-core-check-recheck-contract.md\n"
      "http=200 bytes=16734\n$ cmp $S/12.md <git show origin/agent/MYOS2-LEAD-002:同路径> && echo IDENTICAL\nIDENTICAL\n"
      "$ curl ... reviews/CORE-CHECK-01-core-review.md\nhttp=200 bytes=15032\nIDENTICAL", "text")
w("")
w("会话开始时执行了 `git fetch`，origin/agent/MYOS2-LEAD-002 由 `f79b3a281616` 前进到 `ec62453e76d2`。新增的是 12 号任务书、core-review 回执和一个检查点（`git diff --stat` 显示三个新增文件）。本轮把 `ec62453e76d2` 固定为主线对象（harness2.PINS.lead_head）。")
w("")
w("### 1.2 身份门（identity2，同源复跑时的机械记录）")
w("")
idt = run.get("identity") or {}
fence({"gate": idt.get("gate"), "core_review_fields": idt.get("core_review_fields"), "execution": idt.get("execution"),
       "originals": idt.get("originals"), "technical_inputs_unchanged": idt.get("technical_inputs_unchanged"),
       "remote_refs_equal_pins": idt.get("remote_refs_equal_pins"), "pins": idt.get("pins")})
w("")
w("### 1.3 开工时的仓库状态（手工转录）")
w("")
w("- 执行分支头与远端都是 `a1e7c2277705`，工作树干净。open PR 只有 [08822407d/MyOS2#16](https://github.com/08822407d/MyOS2/pull/16)（主线，写区 lead/）和 [08822407d/MyOS2#17](https://github.com/08822407d/MyOS2/pull/17)（本执行，Draft），两者写区不相交。")
w("- 会话仓库是浅克隆（`git rev-parse --is-shallow-repository` = true），因此 M10 的临时仓库需要特殊构建方式（见 §6）。")
w("")
w("## 2. Phase A：冻结旧验证器实跑反例（修订之前）")
w("")
w("`fixtures/old_counterexamples.py` 从 `a1e7c2277705` 逐字节解出旧 core/fixtures（%d 个文件，清单一致=%s）到全新目录。每个实验在独立子进程中运行，以旧目录为工作目录。工作目录：`%s`。"
  % (len((oldc.get("frozen_manifest") or {}).get("files", [])), (oldc.get("frozen_manifest") or {}).get("names_match_expected"), oldc.get("workdir")))
w("")
a1 = (oldc.get("A1_R01_empty_observation") or {}).get("json") or {}
w("| 项 | 旧入口与输入 | 旧程序真实结果 | 与主线推导 |")
w("|---|---|---|---|")
w("| R01 | `evaluate.v05({\"cases\": {}})` | `%s` | 一致（空观测判为符合预测） |" % cell((a1.get("v05_empty_return") or {}).get("verdict")))
w("| R01 补充 | `run_all.evaluate` 于五个夹具均为空 cases | `%s` | 主线未列：其余评估器在空观测下给出假的“COUNTEREVIDENCE”，V03 为 COMPARATOR_INVALID |" % cell(a1.get("dispatch_evaluate_on_empty_cases")))
for mode, v in (oldc.get("A2_R02_gate_not_enforced") or {}).items():
    j = v.get("json") or {}
    w("| R02 %s | `run_all.main`，计数替身 | 动态入口调用 %s；stage 调用 %s；异常 %s | 一致（门未阻止执行） |" % (mode, j.get("dynamic_entry_calls"), cell((j.get("calls") or {}).get("stage_script")), j.get("main_exception")))
a3 = (oldc.get("A3_R04_old_identity_and_readback_probe") or {}).get("json") or {}
w("| R04 identity | 旧 `identity()` 于已前进的执行分支 | remote_heads_match=`%s`；gate=`%s` | 一致（记录了不一致，但不阻止） |" % (cell(a3.get("old_identity_remote_heads_match")), cell((a3.get("old_identity_gate") or {}).get("ALLOW_CORE_bound_to_this_execution"))))
w("| R04 readback | 旧 `h00.probe_readback()` | as_expected=`%s`；两通道 `%s` | 一致（字节相同，却因分支头前进判失败） |" % (cell((a3.get("old_probe_readback") or {}).get("as_expected")), cell((a3.get("old_probe_readback") or {}).get("frozen_pilot_channels"))))
for k, v in (oldc.get("A3_R04_old_readback_cli") or {}).items():
    w("| R04 CLI %s | 旧 `readback.py ... %s` | exit %s；stderr 末行 `%s` | %s |" % (k, " ".join(v.get("argv_tail", [])), v.get("returncode"),
      cell((v.get("stderr_tail") or "").strip().splitlines()[-1:] or [""]), "一致（--out 的值被当作 field=value）" if k == "no_field_with_out" else "带 field 时能运行，但仍因分支头前进判失败"))
a4 = oldc.get("A4_old_replay") or {}
w("| R02/R04 重放 | 冻结旧 `run_all.py` 完整运行 | exit %s；h00 stage 退出 %s；夹具仍运行=%s；评估 `%s` | 一致（H00 失败后仍运行夹具） |" % (a4.get("returncode"), (a4.get("stage_returncodes") or {}).get("h00_hardening.py"), a4.get("fixtures_called_after_failed_h00"), cell(a4.get("evaluation"))))
for k, v in (oldc.get("A5_R03_old_make_results") or {}).items():
    w("| R03 %s | 旧 `make_results.py` | exit %s；生成=%s；V01=`%s`；错误末行 `%s` | %s |" % (k, v.get("returncode"), v.get("generated"), cell((v.get("case_results") or {}).get("V01")),
      cell((v.get("stderr_tail") or "").strip().splitlines()[-1:] or [""]),
      {"baseline_real_replay": "对照", "v01_paths_missing_and_parse_failed": "一致（仍写 NO_FAILURE_IN_SCOPE）"}.get(k, "一致（缺阶段或夹具阻断时崩溃）")))
a6 = (oldc.get("A6_old_run_pg_group_gone") or {}).get("json") or {}
w("| 进程组 | 旧 `run_pg`，注入 killpg 抛 ProcessLookupError | 返回=%s；`%s` | 组已消失时异常外抛，没有终态记录 |" % (a6.get("old_run_pg_returned"), a6.get("exception")))
w("")
w("结论：主线 R01–R04 的四项推导都由旧程序实跑复现。另外发现，空观测在其余评估器上会变成假的内核 COUNTEREVIDENCE。旧 `run_pg` 在收集超时路径上没有把未确认的子进程写成“已回收”（`child_reaped` 取自返回码），这一点没有复现为错误；组已消失的路径则会抛异常。")
w("")
w("## 3. 修订内容（仅新增于 recheck-01/fixtures/）")
w("")
w("| 文件 | 来源 | 处理的问题 |")
w("|---|---|---|")
for n, src, why in [
    ("harness2.py", "新写（参考冻结 harness.py）", "R02/R04：进程组有界收尾，终态明确；全新目录；钉住冻结对象"),
    ("frozen.py", "新写", "所有旧文件从 a1e7c2277705 逐字节解出，不读工作树"),
    ("old_counterexamples.py", "新写", "Phase A：旧程序实跑"),
    ("identity2.py", "新写（替代冻结 run_all.identity）", "R02/R04：分开核对回执与冻结输入、原件逐字节、移动头的后继关系、远端关系；任何异常都关门"),
    ("evaluate2.py", "修订自冻结 evaluate.py", "R01：每个案例有固定的证据合同（退出码类、事件序列、字段类型、运行状态、重复一致）；只有 VALID 才与原预测比较。第二批：V08 缺静态判据时为 INCOMPLETE_EVIDENCE"),
    ("build2.py", "修订自冻结 build.py", "展开逻辑逐行保持；模板与定位器取自冻结解出目录；用 harness2 运行；元测试注入点"),
    ("h00_2.py", "修订自冻结 h00_hardening.py", "新增快速退出、组已消失、收集超时三个探针；回读探针按对象读取"),
    ("a46_check.py", "新写", "单独核对主线的 A46-C/A46-ASM 两条；旧 A46 结论保留"),
    ("run_recheck.py", "修订自冻结 run_all.py", "R02：身份门→H00→只读阶段→动态夹具；退出码 0/2/3/4；执行完成度、核验发现、基础设施错误分列。第二批：静态阶段未完成时其输出不进入评估"),
    ("make_results2.py", "修订自冻结 make_results.py", "R03：结果、层次、计数、CA 裁定都由数据计算；固定文字只放在 *_template 键。第二批：阶段输出按完成状态取用；V08 分层；CA-02/CA-06 的缺证据处理"),
    ("readback2.py", "修订自冻结 readback.py", "R04：argparse CLI；object/branch 两种绑定"),
    ("meta_tests.py", "新写", "M01–M12。第二批：M09 加严，并新增静态阶段失败变体"),
    ("compare_runs2.py", "新写（第二批）", "比较第一批提交的观测与第二批重跑输出"),
    ("final_check2.py", "新写（参考冻结 final_check.py）", "发布前范围、卫生与一致性检查（results/docs 两种模式）"),
    ("build_evidence2.py", "新写（参考冻结 build_evidence.py）", "生成本文件；不参与核验运行")]:
    w("| %s | %s | %s |" % (n, src, why))
w("")
fm = run.get("frozen_manifest") or {}
w("冻结旧文件（%d 个，清单一致=%s）在同源运行中原样复用：`v00_anchors.py`、`v01_structure.py`、`static_checks.py`、`locate.py`、`v00_semantic_review.yaml` 与全部 C 模板。各文件的字节数与 SHA-256 首段：" % (len(fm.get("files", [])), fm.get("names_match_expected")))
w("")
w("| 文件 | 字节 | SHA-256[0] |")
w("|---|---|---|")
for f in fm.get("files", []):
    w("| %s | %d | %s |" % (f["name"], f["bytes"], f["sha256_segments"][0]))
w("")
w("## 4. 修订版 H00（同源运行中的动态安全门）")
w("")
w("| 探针 | 符合预期 | 终态 | kill | 已确认回收 | 收集超时 | 门控动态 |")
w("|---|---|---|---|---|---|---|")
for p in (run.get("h00") or {}).get("probes", []):
    w("| %s | %s | %s | %s | %s | %s | %s |" % (p.get("probe"), p.get("as_expected"), p.get("terminal_state"), p.get("kill"),
                                             p.get("reaped_confirmed"), p.get("collect_timed_out"), p.get("gates_dynamic")))
w("")
w("dynamic_safety_ok=`%s`，transport_ok=`%s`。收集超时探针中逃逸的孙进程由探针按精确 pid 清理（该进程是探针自己创建的）。" % ((run.get("h00") or {}).get("dynamic_safety_ok"), (run.get("h00") or {}).get("transport_ok")))
w("")
w("## 5. 同源复跑（M12，第二批代码）")
w("")
w("命令（在 recheck-01/fixtures 下执行；RECHECK_ROOT 是新建的空父目录）：")
w("")
fence("$ export PYTHONDONTWRITEBYTECODE=1 RECHECK_ROOT=<scratch>/recheck-runs/canonical2\n$ python3 run_recheck.py --out $RECHECK_ROOT/run.json; echo \"run_recheck exit=$?\"\nrun_recheck exit=%s    (stdout/stderr 均为 0 字节)\n"
      "$ cp $RECHECK_ROOT/run.json ../observations/ && cp $RECHECK_ROOT/run-*/stages/{v00,v01,static,a46}.json ../observations/" % run.get("exit_code"), "text")
w("")
w("工作目录 `%s`；状态 `%s`；execution_complete=`%s`；dynamic_calls=`%s`；stage_outputs_usable=`%s`；infrastructure_errors=`%s`；verification_findings=`%s`。"
  % (run.get("workdir"), run.get("status"), run.get("execution_complete"), cell(run.get("dynamic_calls")), cell(run.get("stage_outputs_usable")),
     cell(run.get("infrastructure_errors")), cell(run.get("verification_findings"))))
w("")
w("只读阶段：")
w("")
w("| 阶段 | 来源 | 退出码 | 超时 | 终态 |")
w("|---|---|---|---|---|")
for k, v in (run.get("stages") or {}).items():
    w("| %s | %s | %s | %s | %s |" % (k, v.get("source"), v.get("returncode"), v.get("timed_out"), v.get("terminal_state")))
w("")
m12 = T.get("M12") or {}
w("同源：冻结旧构建器和修订构建器展开出的夹具源码，SHA-256 首段为 `%s`（逐项相同=%s）。" % (cell(m12.get("expanded_source_sha256_first_segment")), (m12.get("expanded_source_sha256_first_segment") or {}).get("identical")))
w("")
w("动态夹具（每个案例运行两次，比较 stdout 字节与退出码）：")
w("")
w("| 夹具 | 状态 | 案例：退出码/终态/重复一致 |")
w("|---|---|---|")
for n, r in (run.get("fixtures") or {}).items():
    w("| %s | %s | %s |" % (n, r.get("status"), "; ".join("%s:%s/%s/%s" % (c, (v.get("run") or {}).get("returncode"), (v.get("run") or {}).get("terminal_state"), v.get("repeat_identical"))
                                                         for c, v in (r.get("cases") or {}).items())))
w("")
w("逐项结果（results.yaml cases；先判证据状态，再给结果）：")
w("")
w("| 项 | 证据状态 | 结果 | 执行的层 | 未执行的层 |")
w("|---|---|---|---|---|")
for cse in res.get("cases", []):
    ly = cse.get("layers") or {}
    w("| %s | %s | %s | %s | %s |" % (cse["case_id"], cse.get("evidence_status"), cse.get("result"),
                                     ", ".join(k for k, v in ly.items() if v == "EXECUTED") or "-",
                                     ", ".join(k for k, v in ly.items() if v != "EXECUTED") or "-"))
w("")
ev = run.get("evaluation") or {}
w("V00 未通过边界检查的锚点：`%s`（旧 47 条中 A46 仍失败，原文未改）。A46 更正单列：`%s`。" % (cell((ev.get("V00") or {}).get("failing_anchors")), cell([[a.get("id"), a.get("verdict_mech"), a.get("contiguous_hits")] for a in obs("a46.json").get("anchors", [])])))
w("")
w("V08 判据：`%s`；额外观察（不在 07 中）：`%s`。" % (cell((ev.get("V08") or {}).get("result_basis")), cell((ev.get("V08") or {}).get("extra_observation_requeue_order"))))
w("")
w("CA 裁定（按判据计算）：")
w("")
w("| CA | 裁定 | 观察 |")
w("|---|---|---|")
for k, v in (res.get("ca_rulings") or {}).items():
    w("| %s | %s | %s |" % (k, v.get("ruling"), cell(v.get("observed"))))
w("")
w("各项检查的实测值与预测值见 results.yaml `cases[].checks`，每行为 [标签, 实测, 预测, 一致]。原始事件行见 observations/run.json 的 `fixtures.*.cases.*.run.stdout`。")
w("")
w("### 5.1 与第一批同源运行的比较（compare_runs2.py）")
w("")
fence("$ python3 compare_runs2.py %s ../observations ../observations/rerun_vs_batch1.json\n%s\ncompare exit=%d"
      % (C1, json.dumps({"same_source_results_reproduced": cmp_.get("same_source_results_reproduced"),
                         "stage_output_diff_paths": cmp_.get("stage_outputs")}, ensure_ascii=False),
         0 if cmp_.get("same_source_results_reproduced") else 1), "text")
w("")
w("run 摘要逐项相同=`%s`；H00 探针相同=%s；evaluation 完全相同=%s；夹具 stdout 与退出码逐案例相同=`%s`。v00.json 唯一的差异是 `protection/head_short12`，记录的是运行时的执行分支头：第一批运行时为 `%s`，本次为 `%s`，因为分支头已被第一批提交推进。"
  % (cell({k: v.get("same") for k, v in (cmp_.get("run_summary") or {}).items()}), cmp_.get("h00_probes_same"), cmp_.get("evaluation_identical"),
     cell({k: "%d/%d" % (v.get("stdout_and_exit_identical"), v.get("cases")) for k, v in (cmp_.get("fixtures") or {}).items()}),
     ((cmp_.get("identity_head") or {}).get("old") or {}).get("head_short12"), ((cmp_.get("identity_head") or {}).get("new") or {}).get("head_short12")))
w("")
w("### 5.2 V03 守卫的边界（不声称消除全部 C 未定义行为）")
w("")
w("守卫位于夹具替换的 `try_to_wake_up` 入口（冻结模板 fx_wait.c，未改）。在它停下之前，原函数 `swake_up_locked` 已经实际执行了以下步骤：")
w("")
w("1. `list_header_is_empty(&q->task_list_hdr)` 判为非空，因为 count 仍为 1。")
w("2. `list_headr_first_container(...)`（即 container_of）从链表 anchor 算出一个并不存在的 `swqueue_s` 容器地址。")
w("3. 读取该“容器”的 `curr->task` 字段。它的位置与队列锁字重叠：layout.anchor_container_task_aliases_lock=`%s`，direct.task_field_is_lock_word=`%s`，via_complete.task_ptr_nonzero_while_lock_held=`%s`。"
  % (check_of("V03", "layout.anchor_container_task_aliases_lock"), check_of("V03", "direct.task_field_is_lock_word"),
     check_of("V03", "via_complete.task_ptr_nonzero_while_lock_held")))
w("4. 以该值调用 `try_to_wake_up`（直接路径在第 %s 次调用时停下）。" % check_of("V03", "direct.invalid_at_call"))
w("")
w("替换函数先比较指针；若不是已知任务，就以退出码 42 停止并输出 `ttwu_invalid_task`。真实 `try_to_wake_up` 对该“任务”的解引用和写入没有执行，其后的 `list_del_init` 也没有执行。第 2、3 步的容器构造与越界读取在 C 语义上已属未定义行为；守卫只阻止了之后的危险写入，并不消除这些。")
w("")
w("## 6. M01–M12")
w("")
w("`meta_tests.py` 在同源运行之后执行，输入为该次 run.json 与 Phase A 记录。工作目录 `%s`；全部满足=%s。变造输入都标为 HARNESS_META_TEST，不是 MyOS2 原函数的结果。" % (meta.get("workdir"), meta.get("all_met")))
w("")
fence("$ export PYTHONDONTWRITEBYTECODE=1 RECHECK_ROOT=<scratch>/recheck-runs/canonical2-meta\n$ python3 meta_tests.py <scratch>/recheck-runs/canonical2/run.json ../observations/old_counterexamples.json $RECHECK_ROOT/meta_tests.json\n"
      + json.dumps(meta.get("met"), ensure_ascii=False, indent=1) + "\nmeta_tests exit=%d\n$ cp $RECHECK_ROOT/meta_tests.json ../observations/" % (0 if meta.get("all_met") else 1), "text")
w("")
w("| M | 要求 | 旧入口真实结果 | 修订版结果 | 满足 |")
w("|---|---|---|---|---|")
for t in meta.get("tests", []):
    w("| %s | %s | %s | %s | %s |" % (t["id"], cell(t.get("requirement")), cell(t.get("old_summary")), cell(t.get("new_summary")), t.get("met")))
w("")
m10 = T.get("M10") or {}
w("M10 明细（临时仓库：`%s`；会话仓库浅克隆=%s；主仓库对象库未变=%s）：" % (m10.get("scratch_repo_method"), m10.get("session_repo_is_shallow"), m10.get("main_repo_object_store_unchanged")))
w("")
w("| 变体 | gate | 关门原因 | 远端关系 |")
w("|---|---|---|---|")
for k, v in m10.get("variants", {}).items():
    w("| %s | %s | %s | %s |" % (k, v.get("gate_ok"), ", ".join(v.get("closed_by") or []) or "-", v.get("remote_relation")))
w("")
m11 = T.get("M11") or {}
w("M11 明细（readback2 CLI，真实网络读取；超时变体为注入）：")
w("")
w("| 变体 | 参数尾 | 退出码 | ok | 通道 |")
w("|---|---|---|---|---|")
for k, v in m11.get("variants", {}).items():
    w("| %s | %s | %s | %s | %s |" % (k, cell(v.get("argv_tail")), v.get("exit"), v.get("ok"), cell(v.get("channels"))))
w("")
m09 = T.get("M09") or {}
vb = m09.get("variant_static_stage_failed") or {}
w("M09 明细：")
w("")
w("- 部分运行（注入 `%s`）：exit %s，状态 %s，YAML 可生成=%s。V08=`%s`，V14=`%s`。未执行或无效的层：`%s`。CA：`%s`。"
  % (cell(m09.get("injection")), m09.get("exit_code"), m09.get("run_status"), m09.get("yaml_renders"), cell((m09.get("cases") or {}).get("V08")),
     cell((m09.get("cases") or {}).get("V14")), cell(m09.get("layers_not_executed")), cell(m09.get("ca_rulings"))))
w("- 变体 B（第二批新增）：static_checks 真实运行并写出 static.json（真实退出码 %s），但记录为退出 1；夹具记录取自 M12 运行的真实记录，H00 为替身。结果：exit %s，状态 %s，static.json 仍在阶段目录=%s，stage_outputs_usable=`%s`。V08=`%s`，V14=`%s`，CA-02=`%s`，CA-05=`%s`。"
  % (vb.get("static_stage_real_returncode"), vb.get("exit_code"), vb.get("run_status"), vb.get("static_json_left_in_stage_dir"), cell(vb.get("stage_outputs_usable")),
     cell((vb.get("cases") or {}).get("V08")), cell((vb.get("cases") or {}).get("V14")), (vb.get("ca_rulings") or {}).get("CA-02"), (vb.get("ca_rulings") or {}).get("CA-05")))
w("")
w("## 7. 与冻结 core 的差异")
w("")
dif = res.get("diff_vs_frozen_core") or {}
w("逐项结果与冻结 `results.yaml @ 7e2fa84a9823` 比较，结果不同的项：`%s`。CA 裁定的差异：" % [k for k, v in (dif.get("case_results") or {}).items() if not v.get("same")])
w("")
w("| CA | 冻结 | 本次 | 相同 |")
w("|---|---|---|---|")
for k, v in (dif.get("ca_rulings") or {}).items():
    w("| %s | %s | %s | %s |" % (k, v.get("frozen"), v.get("recheck"), v.get("same")))
w("")
w("结果层面没有新的内核反证：冻结 core 的 15 项结论在完整有效数据上重现。区别在于，本次每一项都先通过证据合同，裁定由数据计算，名称也更细：CA-02 拆出了有限模型与全局可达性，CA-05 列出了实际执行的层。")
w("")
w("## 8. 开发期与发布后的修正")
w("")
for item in [
    "开发运行 dev1（run_recheck 首次完整运行）即全部 VALID；没有为迎合输出改动任何案例合同。",
    "编写 M10 时发现 identity2 的缺口：远端分支若被重置到更早的提交，它仍是本地 HEAD 的祖先。已加入 `remote_descends_from_reviewed_core`，由 M10 的 remote_reset_to_master 变体验证。",
    "run_recheck 在自定义 identity 抛异常时原本会落到内部错误分支；改为按关门处理（ERROR_IDENTITY，退出码 3，动态调用 0）。",
    "M09 判据中有一处 and/or 优先级错误，已修正，并补入 CA-05 未定的检查。",
    "M10 首版用 `git clone --shared`。在浅克隆的会话仓库上，该选项只复制分支可达的对象，导致 identity 在临时仓库中抛异常；改为 git init + alternates + 复制 shallow 边界。",
    "readback2 CLI 会在 --out 同目录新建 rbwork-* 下载目录。第一批时这两个目录建在 observations/ 下，发布前已移出（未入库）；第二批的回读把 --out 指向会话临时目录，再复制 JSON。",
    "**第一批提交之后（发布后修正）**：整理 MANIFEST 时复核 M09 的部分运行记录，发现 static_checks 阶段缺失时，V08 仍为 VALID/OBSERVED_AS_PREDICTED（只剩函数层结论），limited_model_reachability 层仍记为 EXECUTED。这与 R01/R03 是同一类问题，只是影响限于部分运行，同源运行不受影响。第二批的修正：V08 缺静态判据时为 INCOMPLETE_EVIDENCE，不给案例级结论；阶段输出只在该阶段完成时使用；CA-02 在这种情况下只保留函数层部分；CA-06 与 V14 源码层要求 V00 证据有效。M09 相应加严，并新增变体 B。修正后同源运行与 M01–M12 各重跑一次，§5.1 显示同源结果与第一批相同。",
    "以上都是验证器或元测试自身的问题；被测 MyOS2 原函数文本与 07/09 预测均未改动。"]:
    w("- " + item)
w("")
w("## 9. 提交、推送与远端回读")
w("")
w("### 9.1 第一批 `%s`（fixtures 12 个文件、observations 7 个 JSON、results.yaml）" % C1)
w("")
w("推送输出（手工转录）：")
w("")
fence("To https://github.com/08822407d/MyOS2\n   a1e7c22..%s  claude/dazzling-cori-q0dnyt -> claude/dazzling-cori-q0dnyt" % C1[:7], "text")
w("")
fence("$ python3 readback2.py %s <recheck-01/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode object --out ../observations/readback_results.object.json\nreadback object exit=0\n"
      "$ python3 readback2.py %s <recheck-01/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode branch --out ../observations/readback_results.branch.json\nreadback branch exit=0" % (C1, C1), "text")
w("")
for tag, r in rb1.items():
    w("- %s：ok=%s，分支头关系=%s，期望 %s 字节；通道 `%s`" % (tag, r.get("ok"), r.get("branch_head_relation") or "不适用（object 模式按提交读取）", r.get("expected_bytes"),
      cell([[c.get("channel"), c.get("http_code"), c.get("curl_exit"), c.get("timed_out"), c.get("bytes"), c.get("byte_identical"), c.get("field_ok")] for c in r.get("channels", [])])))
w("- 推送后身份门（after_batch1/identity_after_push.json）：gate.ok=`%s`，执行=`%s`。" % ((idp1.get("gate") or {}).get("ok"), cell(idp1.get("execution"))))
w("")
w("### 9.2 第二批 `%s`（修正、重跑输出、重新生成的 results.yaml）" % C2)
w("")
w("提交前运行 `final_check2.py results`，输出见 §9.4。推送输出（手工转录）：")
w("")
fence("To https://github.com/08822407d/MyOS2\n   %s..%s  claude/dazzling-cori-q0dnyt -> claude/dazzling-cori-q0dnyt\nbranch 'claude/dazzling-cori-q0dnyt' set up to track 'origin/claude/dazzling-cori-q0dnyt'." % (C1[:7], C2[:7]), "text")
w("")
fence("$ python3 readback2.py %s <recheck-01/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode object --out <scratch>/readback-batch2/readback_results.object.json\nreadback object exit=0\n"
      "$ python3 readback2.py %s <recheck-01/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode branch --out <scratch>/readback-batch2/readback_results.branch.json\nreadback branch exit=0\n"
      "$ python3 -c \"import identity2, harness2 as H; H.emit(identity2.check(), '<scratch>/readback-batch2/identity_after_push.json')\"\nidentity exit=0\n"
      "$ cp <scratch>/readback-batch2/*.json ../observations/after_batch2/   (cmp 逐字节相同)" % (C2, C2), "text")
w("")
for tag, r in rb2.items():
    w("- %s：ok=%s，分支头关系=%s，期望 %s 字节；通道 `%s`" % (tag, r.get("ok"), r.get("branch_head_relation") or "不适用（object 模式按提交读取）", r.get("expected_bytes"),
      cell([[c.get("channel"), c.get("http_code"), c.get("curl_exit"), c.get("timed_out"), c.get("bytes"), c.get("byte_identical"), c.get("field_ok")] for c in r.get("channels", [])])))
w("- 推送后身份门（after_batch2/identity_after_push.json）：gate.ok=`%s`，执行=`%s`。执行分支已两次前进（a1e7c2277705 → %s → %s），旧原件不变，门仍然打开。这就是 R04 要求的“合法续提交、旧原件不变、可重放”的真实实例。" % ((idp2.get("gate") or {}).get("ok"), cell(idp2.get("execution")), C1, C2))
w("")
w("### 9.3 第三批（本文件）")
w("")
w("results.yaml 自第二批提交后冻结。第三批只新增 evidence.md、MANIFEST.md、build_evidence2.py 与 observations/after_batch2/ 的三份记录；提交前运行 `final_check2.py docs %s`，输出见 §9.5。" % C2)
w("")
w("### 9.4 第二批提交前的检查（final_check2.py results）")
w("")
if FC_RESULTS and os.path.exists(FC_RESULTS):
    fence(open(FC_RESULTS, encoding="utf-8").read().rstrip("\n"))
else:
    w("(final_check results-mode output not supplied)")
w("")
w("### 9.5 第三批提交前的检查（final_check2.py docs %s）" % C2)
w("")
if FC_DOCS and os.path.exists(FC_DOCS):
    fence(open(FC_DOCS, encoding="utf-8").read().rstrip("\n"))
else:
    w("(final_check docs-mode output not yet produced)")
w("")
text = "\n".join(L) + "\n"
if re.search(r"[0-9a-fA-F]{40}", text):
    sys.exit("REFUSED: 40-hex in evidence.md")
open(OUT, "w", encoding="utf-8").write(text)
print("wrote %s (%d lines)" % (OUT, text.count("\n")))
