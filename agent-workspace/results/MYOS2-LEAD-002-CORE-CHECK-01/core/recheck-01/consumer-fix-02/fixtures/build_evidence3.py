# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-02 (consumer-fix-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file (publication tool; not used by the checks); structure follows
#   recheck-01/fixtures/build_evidence2.py @ b843d475367a (frozen, unchanged)
# purpose: assemble consumer-fix-02/evidence.md from observations/ and results.yaml. Tables and quoted
#   outputs are generated; the few session commands and push outputs are marked as manual transcription.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 build_evidence3.py <consumer-fix-02 dir> <out.md> <results_commit_short12> [final_check_results.txt] [final_check_docs.txt]"""
import json
import os
import re
import sys

import yaml

D, OUT, RC = sys.argv[1:4]
FC_R = sys.argv[4] if len(sys.argv) > 4 else None
FC_D = sys.argv[5] if len(sys.argv) > 5 else None
obs = lambda n: json.load(open(os.path.join(D, "observations", n), encoding="utf-8"))
old, ct, gs, gp, gc = (obs(n) for n in ("old_side_s01_s03.json", "consumer_tests.json", "guard_start.json", "guard_pre_results.json",
                                        "guard_pre_results_closed_stray_pyc.json"))
fms, fmt = obs("frozen_b843_manifest_start.json"), obs("frozen_b843_manifest_tests.json")
rbo, rbb, gap = (obs("after_results/" + n) for n in ("readback_results.object.json", "readback_results.branch.json", "guard_after_push.json"))
res = yaml.safe_load(open(os.path.join(D, "results.yaml"), encoding="utf-8"))
n6 = yaml.safe_load(open(os.path.join(D, "observations", "n06_revalidation.yaml"), encoding="utf-8"))
T = {t["id"]: t for t in ct["tests"]}
L = []
w = L.append


def fence(x, lang="json"):
    w("```" + lang)
    w(x if isinstance(x, str) else json.dumps(x, ensure_ascii=False, indent=1))
    w("```")


def cell(x):
    return json.dumps(x, ensure_ascii=False).replace("|", "/") if not isinstance(x, str) else x.replace("|", "/")


def st(x):
    r = (x or {}).get("returned") or {}
    return "%s / %s" % (r.get("status"), r.get("result"))


w("---")
for k, v in [("task_id", "MYOS2-LEAD-002-CORE-CHECK-01"), ("packet_id", "MYOS2-LEAD-002-CORE-CHECK-01"),
             ("followup_id", "CORE-CHECK-01-RECHECK-02"), ("phase", "evidence_consumer_recheck"), ("record_type", "consumer_fix_evidence")]:
    w("%s: %s" % (k, v))
w('produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"')
w("execution_model_selection: unknown_or_not_attestable")
w('execution_model_selection_source: "执行者所在平台的规则不允许在推送到仓库的产物中写模型标识（执行者自述）"')
w('date: "2026-10-01"')
w('base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；主线 b0aa54db1b70（开工时固定）；被审输入 b843d475367a；recheck-01 results 冻结于 efb9846b88ec（短 SHA）"')
w("results_commit_short12: %s" % RC)
w("status: final_for_recheck_02")
w('transcription: "§1.1 与 §9 的推送输出是会话命令的手工转录；其余数值、表格与输出由 fixtures/build_evidence3.py 从 observations/ 与 results.yaml 生成。observations 下的 JSON/YAML 是程序写出的原文件。"')
w('file_moves: "old_side_s01_s03.json、guard_start.json、frozen_b843_manifest_start.json 由修订前的首次运行写在会话临时目录，随后逐字节复制；after_results/ 三份记录同样先写在临时目录再复制（cmp 相同）。readback2 的 rbwork-* 下载目录未入库。"')
w('redaction: "未发现需脱敏内容；push 输出经 40 位十六进制过滤（无命中）。"')
w("open_questions: []")
w("---")
w("")
w("# RECHECK-02 证据：数据读取层的旧负例、修订、N01–N06 与已交观测复判")
w("")
w("**S01–S03 先在未改的 b843 程序与已交观测上实跑，三项主线推导全部复现。新读取层在 N01–N06 中全部满足；N05 还暴露出一处主线未列的同类缺口（S04），已一并修正。已交观测按新读取层复判，与 efb9846b88ec 的冻结结果逐项相同。** 本轮只做 Python 数据层实验：没有编译或运行 C 夹具，没有重做 H00，没有全树扫描，没有改内核。")
w("")
w("## 1. 输入与绑定")
w("")
w("### 1.1 读取任务书与回执（手工转录）")
w("")
fence("$ curl -sS -o $S/13.md -w 'http=%{http_code} bytes=%{size_download}\\n' https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/13-core-evidence-consumer-recheck.md\n"
      "http=200 bytes=14502\n$ git show origin/agent/MYOS2-LEAD-002:<同路径> | cmp - $S/13.md && echo '13 raw == git object'\n13 raw == git object\n"
      "$ curl ... reviews/CORE-CHECK-01-recheck-01-review.md\nhttp=200 bytes=13217\nreview raw == git object", "text")
w("")
w("开工时 origin/agent/MYOS2-LEAD-002 位于 `b0aa54db1b70`，相对上一轮固定的 `ec62453e76d2` 只新增四个文件（下表 lead_changes）。本轮把 `b0aa54db1b70` 固定为主线对象。14 号文件（调度回插后续）不属于本次执行范围，只读未执行。")
w("")
w("### 1.2 开工时的保护门（guard3，修订前）")
w("")
fence({"gate": gs.get("gate"), "review_fields": gs.get("review_fields"), "contract13_fields": gs.get("contract13_fields"),
       "lead_changes_since_previous_pin": gs.get("lead_changes_since_previous_pin"), "efb_to_b843": gs.get("efb_to_b843"),
       "b843_files": gs.get("b843_files"), "execution": gs.get("execution"), "remote_refs_equal_pins": gs.get("remote_refs_equal_pins")})
w("")
w("门的含义：回执与 13 号绑定；主线只追加；a1e7 → f9ae → efb → b843 链成立，且 efb → b843 只新增六件、results.yaml 未变；HEAD、索引、工作树相对 b843 只在 consumer-fix-02/ 下新增；远端同线。冻结的 recheck-01 identity2 从 b843 解出后作为辅助运行，它只保护 pilot/core 原件，b843 全部 32 件由上面的新增检查覆盖。")
w("")
w("### 1.3 复用的冻结程序")
w("")
w("从 `b843d475367a` 逐字节解出 recheck-01 的 15 个程序与 14 个观测到全新目录（清单一致=%s/%s，与 HEAD 相同=%s）。本轮实际导入或运行的模块：" % (fms.get("fixture_names_match_expected"), fms.get("observation_names_match_expected"), fms.get("all_unchanged_at_head")))
w("")
w("| b843 路径（recheck-01/ 下） | 字节 | SHA-256[0] | 用途 |")
w("|---|---|---|---|")
use = {"fixtures/evaluate2.py": "S01/S02 旧侧；新读取层在进程内包装 validate_case 与 judge", "fixtures/make_results2.py": "S03 旧 CLI；新入口复用 build_doc/render",
       "fixtures/harness2.py": "make_results2/identity2/readback2 的依赖", "fixtures/identity2.py": "guard3 的辅助检查",
       "fixtures/readback2.py": "远端回读 CLI"}
for f in fms.get("files", []):
    if f.get("used_by_consumer_fix_02"):
        w("| %s | %d | %s | %s |" % (f["path"], f["bytes"], f["sha256_segments"][0], use.get(f["path"], "")))
w("")
w("每个新侧调用都在导入前把解出目录与 b843 对象逐字节比对（frozen_b843.import_frozen）；不读工作树中的旧文件。")
w("")
w("## 2. S01–S03：冻结程序实跑（修订之前）")
w("")
w("`fixtures/old_side.py`，工作目录 `%s`。每个实验是独立 Python 进程（30 s 上限），以 b843 解出目录为工作目录。变造输入只写在该目录下，标为 HARNESS_META_TEST。" % old.get("workdir"))
w("")
w("| 项 | 输入 | 旧程序真实返回 | 主线预测 | 复现 |")
w("|---|---|---|---|---|")
s1 = old["S01"]
w("| S01 | 实际 run.json 的 fx_sched/v04_noncurrent_wake，删去 run 中 `%s`（其余字段剩 %s） | `%s` | %s | %s |" % (
    "`、`".join(s1["deleted_run_keys"]), cell(s1["keys_left_in_run"]), st(s1["deleted_four_keys"]), " / ".join(s1["lead_prediction"]), s1["reproduced"]))
w("| S01 对照 | 未改记录 | `%s` | - | - |" % st(s1["control_unmodified"]))
for k, v in old["S02"]["variants"].items():
    w("| S02 %s | 实际 v01.json，%s | `%s` | %s | %s |" % (k, "三处同时清空" if k == "empty_all_three" else "清空 " + k.replace("empty_", ""), st(v),
                                                 " / ".join(old["S02"]["lead_prediction"]), old["S02"]["reproduced"][k]))
w("| S02 对照 | 未改 v01.json | `%s` | - | - |" % st(old["S02"]["control_unmodified"]))
s3 = old["S03"]
c3 = s3["control_complete_copy"]
w("| S03 对照 | 完整复制七个输入，冻结 make_results2 CLI | exit %s；报告与 efb 的 results.yaml 逐字节相同=%s | - | - |" % (c3["returncode"], c3["report_identical_to_efb_results_yaml"]))
om = s3["static_omitted"]
r = om.get("report") or {}
w("| S03 缺 static.json | 其余六个输入原样 | exit %s；顶层 `%s`；V08 `%s/%s`，限定模型 `%s`；V14 `%s`；CA-02 `%s` | V14 源码层缺证据而 V08 沿用缓存 VALID | %s |" % (
    om["returncode"], r.get("status"), (r.get("V08") or {}).get("evidence_status"), (r.get("V08") or {}).get("result"),
    (r.get("V08") or {}).get("limited_model_reachability"), (r.get("V14") or {}).get("evidence_status"), r.get("CA-02"), s3["reproduced"]["static_omitted"]))
tr = s3["static_truncated_json"]
w("| S03 坏 JSON | static.json 内容只有 `{` | exit %s；报告写出=%s；stderr 末行 `%s` | 错误收集前抛 JSONDecodeError，无部分报告 | %s |" % (
    tr["returncode"], tr["report_written"], cell(tr["stderr_tail"][-1:]), s3["reproduced"]["static_truncated_json"]))
w("")
w("结论：三项推导全部由旧程序实际复现，没有反证。S03 中旧入口在缺 static.json 时顶层仍写 `generated_from_run_status_COMPLETE`：源运行当时完成的事实被当成了当前交接包的状态。")
w("")
w("## 3. 修订（只新增于 consumer-fix-02/fixtures/）")
w("")
w("| 文件 | 作用 |")
w("|---|---|")
for n, why in [
    ("common3.py", "固定对象、全新目录、30 s 子进程、拒绝 40 位十六进制的输出"),
    ("frozen_b843.py", "从 b843 逐字节解出旧程序与观测；导入前逐字节核对"),
    ("guard3.py", "回执/13 号绑定、主线只追加、结果链、b843 全部原件不变、远端同线；辅助运行冻结 identity2"),
    ("old_side.py", "S01–S03 旧侧实跑（修订前运行，之后未改）"),
    ("expected_objects3.py + expected_objects.json", "S02：从冻结输入（taskbook 的 07 报告头、map、MANIFEST self_check）一次提取应检查的对象集合；final_check3 会重新提取并比较"),
    ("consumer3.py", "S01：RAN 案例的必需字段缺失→INCOMPLETE_EVIDENCE [MISSING_FIELD]，类型错→INVALID_EVIDENCE [WRONG_TYPE]，冻结检查漏掉的显式失败（output_lost、重复运行终态）→INVALID_EVIDENCE；S02：v01 对象集合与预期比较；S03：每个输入文件的读取/结构状态，按当前可用原件重算全部项目，不用缓存判定；S04：judge 修正"),
    ("make_results3.py", "完整入口：读取→结构/覆盖→各项消费状态→YAML；退出码 0 完整、2 部分（报告已写）、4 读取器自身失败"),
    ("consumer_tests.py", "N01–N06（N05 复制了 meta_tests 的 M01–M05/M08/M09 变造逻辑，未导入或调用会运行 C 的旧入口）"),
    ("make_summary3.py", "由 observations 生成 results.yaml（同输入同字节）"),
    ("final_check3.py / build_evidence3.py", "发布前检查与本文件生成；不参与核验")]:
    w("| %s | %s |" % (n, why))
w("")
w("冻结的 evaluate2/make_results2 文件本身没有改动。新读取层在自己的进程里把 `evaluate2.validate_case` 与 `evaluate2.judge` 换成包装函数：包装先取冻结函数的原判定，再追加本轮的检查，所以冻结检查已拒绝的情形照样拒绝。")
w("")
w("**S04（本轮发现，13 号未列）**：冻结 `judge` 用观测值去比一个固定的“错误预测”做负控。若一条完整、有效的观测恰好等于该错误值（V04 `ret=1`，V01 hex40 命中数 1），旧程序返回 `ERROR / COMPARATOR_INVALID`，把一个真实发现记成基础设施错误。修正：只有当负控的错误值与预测值本身相同（负控无意义）时才保留 ERROR；否则按发现判定。实际旧/新返回：`%s`。" % cell(res["summary"]["S04_old_new"]))
w("")
w("## 4. N01–N06")
w("")
w("`fixtures/consumer_tests.py`，工作目录 `%s`。全部满足=%s。旧侧是 b843 程序，新侧是 consumer3/make_results3；每个实验都是独立进程（30 s 上限）。" % (ct.get("workdir"), ct.get("all_met")))
w("")
for tid in ("N01", "N02"):
    t = T[tid]
    w("### %s：%s（满足=%s）" % (tid, t.get("requirement"), t.get("met")))
    w("")
    w("| 变体 | 旧 | 新 | 新错误类别 | 满足 |")
    w("|---|---|---|---|---|")
    for k, v in t["variants"].items():
        w("| %s | %s | %s | %s | %s |" % (k, cell(v.get("old")), cell(v.get("new")), cell(v.get("new_categories")), v.get("met")))
    w("")
w("N02 的完整对照中，记录里的错误列表本来就是空的：`%s`。这类空集合是合法结果，不被拒绝；清空的是“应检查的对象清单”时才判缺证据。" % cell(T["N02"]["variants"]["control_complete_empty_error_lists"].get("empty_error_lists_in_record")))
w("")
for tid in ("N03", "N04"):
    t = T[tid]
    w("### %s：%s（满足=%s）" % (tid, t.get("requirement"), t.get("met")))
    w("")
    w("| 变体 | 旧 CLI | 新 CLI | 新报告中非 VALID 的项 | 新报告的输入问题 |")
    w("|---|---|---|---|---|")
    for k, v in t["variants"].items():
        o, n = v.get("old") or {}, v.get("new") or {}
        nr = n.get("report") or {}
        w("| %s | %s | exit %s；%s | %s | %s |" % (
            k, ("exit %s；报告=%s；%s" % (o.get("returncode"), o.get("report_written"), cell((o.get("stderr_tail") or [""])[-1:]))) if o else "-",
            n.get("returncode"), nr.get("status") or ("错误记录已写=%s" % n.get("error_record_written")),
            cell(sorted(c for c, s in (nr.get("cases") or {}).items() if s[0] != "VALID")), cell(nr.get("input_problems"))))
    w("")
    w("判据：`%s`" % cell(t.get("checks")))
    w("")
t = T["N05"]
w("### N05：%s（满足=%s）" % (t.get("requirement"), t.get("met")))
w("")
w("| 变体 | 旧（b843） | 新 | 满足 |")
w("|---|---|---|---|")
for k, v in t["variants"].items():
    if k == "through_cli":
        continue
    w("| %s | %s | %s | %s |" % (k, cell(v.get("old", "per-item: %s" % v.get("old_all"))), cell(v.get("new", "per-item: %s" % v.get("new_all"))), v.get("met")))
w("")
w("经真实读取/成文入口（make_results3 CLI）：")
w("")
w("| 变体 | 旧 CLI 退出码 | 新 CLI | 新报告中非 VALID 的项 | 满足 |")
w("|---|---|---|---|---|")
for k, v in t["variants"]["through_cli"].items():
    nc = v.get("new_cli") or {}
    w("| %s | %s | exit %s，%s | %s | %s |" % (k, (v.get("old_cli") or {}).get("returncode"), nc.get("returncode"), nc.get("status"),
                                               cell({c: s for c, s in (nc.get("cases") or {}).items() if s[0] != "VALID"}), v.get("met")))
w("")
w("## 5. 已交观测复判（N06，RECORDED_OBSERVATION_REVALIDATION）")
w("")
t = T["N06"]
cv = n6["consumer_validation"]
w("不是新的宿主实跑：用新入口读取 b843 中未改的观测，重新判读。源运行事实与本次消费检查分列：")
w("")
w("| | 源运行（历史事实） | 本次消费检查 |")
w("|---|---|---|")
w("| 身份/日期 | identity 读取时间 %s；执行头 %s | %s；%s |" % (t["source_execution"]["identity_read_time_utc"], t["source_execution"]["execution_head_short12"],
                                                       t["consumer_check"]["date_utc"], t["consumer_check"]["programs"]))
w("| 状态 | %s（exit %s） | %s |" % (((n6.get("source_execution") or {}).get("run") or {}).get("status"), ((n6.get("source_execution") or {}).get("run") or {}).get("exit_code"), cv["status"]))
w("| 输入 | 运行时写出 | %s |" % cell([[i["file"], i["state"]] for i in cv["inputs"]]))
w("")
w("判据：`%s`。与 efb 冻结结果相比：不同的项 `%s`，CA 全部相同=%s，counts 相同=%s；缓存判定与消费判定不同的项 `%s`。" % (
    cell(t["checks"]), cell(t["diff_vs_recheck01_frozen_results"]["cases_different"]),
    all(v["same"] for v in t["diff_vs_recheck01_frozen_results"]["ca_rulings"].values()), t["diff_vs_recheck01_frozen_results"]["counts_same"],
    cell(cv["cached_evaluation_vs_consumer"]["differs"])))
w("")
w("| 项 | 证据状态 | 结果 |")
w("|---|---|---|")
for c in n6.get("cases", []):
    w("| %s | %s | %s |" % (c["case_id"], c.get("evidence_status"), c.get("result")))
w("")
w("原 A46 失败保留，A46-C/A46-ASM 分列；未执行层（`%s`）没有升级。完整复判报告：observations/n06_revalidation.yaml。" % cell(n6["counts"]["layers_not_executed"]))
w("")
w("## 6. 未完成与限制")
w("")
for item in [
    "本轮只判读已交观测：数据完整时得到的仍是 recheck-01 已有的受限结论，没有新增内核证据。CA-02 全局可达性、V08 回插顺序候选（14 号 CORE-SCHED-ORDER-02 未在此执行）、真实 ELF/IRQ/SMP/上下文切换继续开放。",
    "重复运行的一致性只能读 producer 写下的布尔摘要与第二次的退出码/终态；第二次运行的原始 stdout 没有保存，本轮不补造。",
    "没有 run.json 时，阶段完成记录也随之缺失：本读取层把依赖阶段完成的静态项一并判为证据不完整（N04 run_unparsable_control）。这是保守选择，也可以另行约定接受独立的阶段文件。",
    "结构检查只覆盖读取层实际使用的字段与对象集合，不是任意损坏或恶意输入的通用证明。",
    "S04 的修正只改变负控判定的一种情形；冻结 evaluate2 中各案例的预测与负控取值没有改动。",
    "执行模型未知；没有 CI 或人工逐行复核。"]:
    w("- " + item)
w("")
w("## 7. 对“无调用者”说法的撤回（导航）")
w("")
w("- 撤回对象：PR #17 旧摘要中“`swake_up_all_locked` 与 `finish_swait` 在 swait.c 外无调用者”。recheck-01 已交的 static_checks 扫描没有覆盖这一点，撤回不等于已经发现调用者。PR 正文已于 2026-10-01 更正，主线 recheck-01 回执 §2 已登记接受。")
w("- 仍保留的范围限定说法：`msleep` 在 time a039d9803ade 的 mykernel/ 下 843 个 `*.c/*.h/*.S/*.lds` 文件中、按 static_checks 的文本正则与去注释规则，只命中 timer_api.h 中的声明（recheck-01/observations/static.json 的 V09）。它不是全局不可达的证明。")
w("- 本轮没有为这条说法重新扫描源码。")
w("")
w("## 8. 过程记录")
w("")
for item in [
    "开发运行中，N02 的 hex40 命中对照与 N05 的“有效但与预测不同”对照在新侧也得到 ERROR，由此发现 S04。修正 consumer3 后重跑；正式记录是修正后的一次完整运行。",
    "提交前保护门第一次关闭（observations/guard_pre_results_closed_stray_pyc.json）：本会话早先在旧 recheck-01/fixtures 下运行一条只读查看命令时没有设 PYTHONDONTWRITEBYTECODE，生成了未入库的 `__pycache__/evaluate2.cpython-311.pyc`。没有 b843 文件被改，但它在写区之外。已删除这个本会话生成的缓存文件后重跑保护门，门打开（guard_pre_results.json）。",
    "old_side.py 只在修订前运行一次，其记录即正式记录；之后该文件未改。",
    "以上都是读取层或测试本身的问题；没有改动被测原函数、07/09 预测或任何 b843 文件。"]:
    w("- " + item)
w("")
w("## 9. 提交、推送与回读")
w("")
cm = [x.split("\t", 1)[1] for x in (gap.get("changes_vs_b843") or {}).get("committed") or []]
w("结果批 `%s`（fixtures %d 个文件、observations %d 个文件、results.yaml），提交前运行 `final_check3.py results`（§9.1）。推送输出（手工转录）：" % (
    RC, sum(1 for p in cm if p.startswith("<cf02>/fixtures/")), sum(1 for p in cm if p.startswith("<cf02>/observations/"))))
w("")
fence("To https://github.com/08822407d/MyOS2\n   b843d47..%s  claude/dazzling-cori-q0dnyt -> claude/dazzling-cori-q0dnyt" % RC[:7], "text")
w("")
fence("$ cd <b843 解出目录>/fixtures\n$ python3 readback2.py %s <consumer-fix-02/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode object --out <scratch>/readback/readback_results.object.json\nreadback object exit=0\n"
      "$ python3 readback2.py %s <consumer-fix-02/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode branch --out <scratch>/readback/readback_results.branch.json\nreadback branch exit=0\n"
      "$ python3 guard3.py <b843 解出目录> <scratch>/readback/guard_after_push.json\nguard exit=0\n$ cp <scratch>/readback/*.json ../observations/after_results/   (cmp 逐字节相同)" % (RC, RC), "text")
w("")
for tag, r in (("object", rbo), ("branch", rbb)):
    w("- %s：ok=%s，分支头关系=%s，期望 %s 字节；通道 `%s`" % (tag, r.get("ok"), r.get("branch_head_relation") or "不适用（object 模式按提交读取）", r.get("expected_bytes"),
      cell([[c.get("channel"), c.get("http_code"), c.get("curl_exit"), c.get("timed_out"), c.get("bytes"), c.get("byte_identical"), c.get("field_ok")] for c in r.get("channels", [])])))
w("- 推送后保护门：gate.ok=`%s`，执行=`%s`，相对 b843 的已提交变化 %d 项、全部是 consumer-fix-02/ 下的新增。" % ((gap.get("gate") or {}).get("ok"), cell(gap.get("execution")),
                                                                                  len((gap.get("changes_vs_b843") or {}).get("committed") or [])))
w("")
w("文档批（本文件、MANIFEST.md、build_evidence3.py、after_results/ 三份记录）在其后提交；提交前运行 `final_check3.py docs %s`（§9.2）。" % RC)
w("")
w("### 9.1 结果批提交前的检查")
w("")
if FC_R and os.path.exists(FC_R):
    fence(open(FC_R, encoding="utf-8").read().rstrip("\n"))
else:
    w("(not supplied)")
w("")
w("### 9.2 文档批提交前的检查")
w("")
if FC_D and os.path.exists(FC_D):
    fence(open(FC_D, encoding="utf-8").read().rstrip("\n"))
else:
    w("(final_check docs-mode output not yet produced)")
w("")
text = "\n".join(L) + "\n"
if re.search(r"[0-9a-fA-F]{40}", text):
    sys.exit("REFUSED: 40-hex in evidence.md")
open(OUT, "w", encoding="utf-8").write(text)
print("wrote %s (%d lines)" % (OUT, text.count("\n")))
