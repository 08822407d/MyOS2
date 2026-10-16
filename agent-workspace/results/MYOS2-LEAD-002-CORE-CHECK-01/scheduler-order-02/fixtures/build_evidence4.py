# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-ORDER-02 (scheduler-order-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file (publication tool; not used by the experiment)
# purpose: assemble scheduler-order-02/evidence.md from observations/ and results.yaml. Tables and quoted
#   outputs are generated; session commands and push outputs are marked as manual transcription.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 build_evidence4.py <scheduler-order-02 dir> <out.md> <results_commit_short12> [final_check_results.txt] [final_check_docs.txt]"""
import json
import os
import re
import sys

import yaml

D, OUT, RC = sys.argv[1:4]
FC_R = sys.argv[4] if len(sys.argv) > 4 else None
FC_D = sys.argv[5] if len(sys.argv) > 5 else None
obs = lambda n: json.load(open(os.path.join(D, "observations", n), encoding="utf-8"))
runs, ctl, gs, gp = obs("runs.json"), obs("controls.json"), obs("guard_start.json"), obs("guard_pre_results.json")
rbo, rbb, gap = (obs("after_results/" + n) for n in ("readback_results.object.json", "readback_results.branch.json", "guard_after_push.json"))
res = yaml.safe_load(open(os.path.join(D, "results.yaml"), encoding="utf-8"))
L = []
w = L.append


def fence(x, lang="json"):
    w("```" + lang)
    w(x if isinstance(x, str) else json.dumps(x, ensure_ascii=False, indent=1))
    w("```")


def cell(x):
    return json.dumps(x, ensure_ascii=False).replace("|", "/") if not isinstance(x, str) else x.replace("|", "/")


def q(queue):
    names = {0: "I", 2: "A", 3: "B", 4: "C", 5: "D"}
    return " ".join("%s%s" % (names.get(t, t), v) for t, v in queue)


w("---")
for k, v in [("task_id", "MYOS2-LEAD-002-CORE-CHECK-01"), ("packet_id", "MYOS2-LEAD-002-CORE-CHECK-01"),
             ("followup_id", "CORE-SCHED-ORDER-02"), ("phase", "scheduler_order_witness"), ("record_type", "scheduler_order_evidence")]:
    w("%s: %s" % (k, v))
w('produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"')
w("execution_model_selection: unknown_or_not_attestable")
w('execution_model_selection_source: "执行者所在平台的规则不允许在推送到仓库的产物中写模型标识（执行者自述）"')
w('date: "2026-10-02"')
w('base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；主线 6706013a079a（开工时固定）；冻结执行头 0d62c4d19711（短 SHA）"')
w("results_commit_short12: %s" % RC)
w("status: final_for_scheduler_order_02")
w('transcription: "§1.1、§1.3 与 §9 的推送输出是会话命令的手工转录；其余数值、表格与输出由 fixtures/build_evidence4.py 从 observations/ 与 results.yaml 生成。observations 下的文件是程序写出的原文件。"')
w('file_moves: "observations/after_results/ 三份记录先写在会话临时目录，再逐字节复制（cmp 相同）；readback2 的 rbwork-* 下载目录未入库。"')
w('redaction: "未发现需脱敏内容；push 输出经 40 位十六进制过滤（无命中）；runs.json 中可执行文件路径记为 <build>/fx_order。"')
w("open_questions: []")
w("---")
w("")
w("# CORE-SCHED-ORDER-02 证据：pick_next_task_myos 回插与计费时点的八组宿主见证")
w("")
s = res["summary"]
w("**八组场景全部完成，每组两次独立运行，记录完整且两次相同。主线的两个候选都得到有限实测见证：W01 的游标问题（A 被插到 idle 之后、队尾），W02 与 W07 第 1 步的计费时点问题（先按 15 插到 C20 前，再记成 25）。主线推导逐项相符，没有反证。** 这只是一个原函数在宿主进程中的边界行为，不是 MyOS2 运行，也不是内核正确性结论。")
w("")
w("## 1. 输入、绑定与 PR")
w("")
w("### 1.1 读取（手工转录）")
w("")
fence("$ curl -sS -o $S/15.md -w 'http=%{http_code} bytes=%{size_download}\\n' https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/15-scheduler-ordering-witness-contract.md\n"
      "http=200 bytes=15008\n15 raw == git object\n$ curl ... reviews/CORE-CHECK-01-recheck-02-review.md\nhttp=200 bytes=10591\nreview raw == git object\n"
      "$ git show origin/agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/14-scheduler-ordering-followup.md   (全文读取)", "text")
w("")
w("开工时 origin/agent/MYOS2-LEAD-002 位于 `6706013a079a`，相对上一轮固定的 `b0aa54db1b70` 只新增三个文件；本轮把 `6706013a079a` 固定为主线对象。")
w("")
w("### 1.2 编译前的保护门（guard4）")
w("")
fence({"gate": gs.get("gate"), "review_fields": gs.get("review_fields"), "contract_fields": gs.get("contract_fields"),
       "lead_changes_since_previous_pin": gs.get("lead_changes_since_previous_pin"), "execution": gs.get("execution"),
       "frozen_0d62_files": gs.get("frozen_0d62_files"), "remote_refs_equal_pins": gs.get("remote_refs_equal_pins")})
w("")
w("guard4 是本任务自己的绑定：15 号任务书与 RECHECK-02 收口回执的字段、主线只追加、77fa → 0d62 只新增、HEAD/索引/工作树相对 0d62 只在 scheduler-order-02/ 下新增、time 未动、远端同线。它没有把 guard3 的 consumer-fix-02 前缀放宽成白名单。")
w("")
w("### 1.3 PR 处置（手工转录）")
w("")
w("开工时 PR #17 为 open、Ready（主线收口后转为 Ready）、未合并；master 仍为 `de3bb1df906a`。按 15 号 §7，新实验开始时已把 PR #17 转回 Draft 并复用；没有新开 PR。open PR 只有 #16（主线，写区 lead/）与 #17。")
w("")
w("## 2. 原函数抽取与构建")
w("")
ii = res["input_identity"]
pk = ii["pick_next_task_myos"]
w("被测函数 `%s::%s`，time `%s` 第 %d–%d 行，%d 字节，SHA-256 `%s`。原样插入，未改一个字符。" % (
    pk["path"], pk["name"], pk["commit_short12"], pk["lines"][0], pk["lines"][1], pk["bytes"], " ".join(pk["sha256_segments"])))
w("")
w("全部 `//@@ORIG` 抽取（由 0d62 冻结的 build2.Builder.expand 从 time 对象逐字复制）：")
w("")
w("| 来源 | 种类 | 符号 | 行 | 字节 | SHA-256[0] |")
w("|---|---|---|---|---|---|")
for p, kind, name, lines, b, h in ii["orig_slices"]:
    w("| %s | %s | %s | %d–%d | %d | %s |" % (p, kind, name, lines[0], lines[1], b, h))
w("")
w("复用的 0d62 冻结文件（从提交对象逐字节解出，导入前核对）：")
w("")
w("| 0d62 路径（results 根下） | 字节 | SHA-256[0] | 用途 |")
w("|---|---|---|---|")
for p, b, h, role in ii["frozen_0d62_reused"]:
    w("| %s | %d | %s | %s |" % (p, b, h, role))
w("")
w("模板 fixtures/fx_order.c：%s；展开后源码 observations/fx_order.expanded.c：%s。编译：`gcc %s`，退出码 %s，超时 %s，stderr %d 字节（无警告），二进制未入库。工具：`%s`。上限：%s。" % (
    cell(ii["template"]), cell(ii["expanded_source"]), " ".join(ii["cflags"]), ii["compile"]["returncode"], ii["compile"]["timed_out"],
    ii["compile_stderr_bytes"], cell(ii["tools"]), cell(ii["limits"])))
w("")
w("## 3. 夹具做了什么、没做什么")
w("")
for item in res["fidelity_template"]:
    w("- " + item)
w("- 旧 fx_sched.c 的 pick() 在每次调用前执行 `rq->myos.last_jiffies = jiffies`，本夹具没有沿用：last_jiffies 每个场景只设一次初值 100，之后只由原函数更新。")
w("- 任务节点全部真实分配、互不重复；current 不在队列中；current 不是 idle 时 idle 在队列中；current 是 idle 时 idle 不重复入队。没有删 idle，也没有用空 anchor 冒充任务。")
w("- 遍历快照先与 anchor 比较，再按地址与五个已知节点比较；未知链接只报告、不转换，最多 16 个链接。")
w("")
w("## 4. W01–W08 实际结果")
w("")
w("队列写作“任务+vruntime”，I 为 idle。每组两次运行，下表来自第 1 次；第 2 次 stdout 与之逐字节相同（SHA-256 首段见“运行”列）。完整两次 stdout、stderr、退出码与终态见 observations/runs.json。")
w("")
w("| W | 输入 | 步 | 调用前队列 | 返回 | 调用后队列 | 记账（实测/oracle） | last_jiffies 后 | P 非 idle / idle 尾 | 运行（rc、终态、stdout 字节/sha0）×2 |")
w("|---|---|---|---|---|---|---|---|---|---|")
names = {0: "I", 2: "A", 3: "B", 4: "C", 5: "D"}
for n, sc in res["scenarios"].items():
    di = sc["declared_inputs"]
    inp = "cur=%s jf=%d nr=%d A=%d%s" % (names[di["current"]], di["jiffies"], di["need_resched"], di["a_vr"],
                                       "(UNINT)" if di["a_state"] == 2 else "") + (" I=%d" % di["idle_vr"] if di["current"] == 0 else "")
    rr = "; ".join("%s,%s,%d/%s" % (r["returncode"], r["terminal_state"], r["stdout_bytes"], r["stdout_sha256_0"]) for r in sc["runs"])
    for st in sc.get("steps", []):
        w("| %s | %s | %d | %s | %s | %s | %s / %s | %s | %s / %s | %s |" % (
            n, inp if st["step"] == 1 else "", st["step"], q(st["queue_before"]), names.get(st["returned"], st["returned"]), q(st["queue_after"]),
            cell({names[int(k)]: v for k, v in st["deltas_recorded"].items()}), cell({names[int(k)]: v for k, v in st["deltas_oracle"].items()}),
            st["last_jiffies_after"], st["P"]["P_nonidle"], st["P"]["idle_tail"], rr if st["step"] == 1 else "同上"))
w("")
w("与主线推导逐项比较（[实测, 主线, 判定]）：")
w("")
w("| W | 比较 | 全部相符 |")
w("|---|---|---|")
for n, sc in res["scenarios"].items():
    w("| %s | %s | %s |" % (n, cell(sc.get("lead_derivation")), sc.get("lead_derivation_all_agree")))
w("")
w("第 1 次运行的原始 stdout（第 2 次相同）：")
w("")
for n in res["scenarios"]:
    w("<details><summary>%s</summary>" % n)
    w("")
    fence(runs["scenarios"][n]["runs"][0]["stdout"].rstrip("\n"))
    w("")
    w("</details>")
    w("")
w("## 5. 判读")
w("")
w("每组先核记录完整（两次都退出 0、未超时、exited、已回收、stderr 为空、stdout 相同、必需事件与字段齐全），再由独立的值列表 oracle 按声明输入计算：是否切换、选中队首、调用后成员、各任务 vruntime 增量、last_jiffies、总计费。oracle 不复制原函数的指针遍历与计费次序，只对实测队列检验候选约束 P。")
w("")
w("- 结构 %s，记账与 oracle 一致 %s，选择一致 %s；输入保真全部成立。" % (s["all_structure_ok"], s["all_accounting_matches_oracle"], s["all_selection_matches_oracle"]))
w("- 候选 P：`%s`；违反的步：`%s`。" % (cell(s["candidate_P"]), cell(s["P_violations_by_step"])))
w("- 主线推导全部相符=%s，不相符项 `%s`。" % (s["lead_derivations_all_agree"], cell(s["lead_derivations_differing"])))
w("- 选中任务不是队列最小键的步：`%s`（W07 第 2 步：因第 1 步的失序，A25 位于队首，被选在 C20 之前）。" % cell(s["selected_task_not_queue_minimum"]))
w("")
cb = res["cursor_fix_alone_basis"]
hd = cb["W02_head_after_removal"] or [None, None]
w("**为什么只修游标可能不够。** W02 实测：插入时 A 的键为 %s，取走 B 后的队首是 %s%s，A 被放在第 %s 位，随后才记入 %s。按 myos_rt.c 第 37–50 行，键 %s 不大于 %s，插入循环一次也不进入，A 直接放在队首；运行时间在插入之后才加上。只改循环内部（让比较对象随游标移动）改变不了这种情形，它只影响 W01 那种循环一直走到 anchor 的情形。这是源码阅读加实测边界值，没有运行任何改动后的函数。" % (
    cb["W02_current_key_at_insertion"], names.get(hd[0], hd[0]), hd[1], cb["W02_insert_position_recorded"], cb["W02_charged_after"],
    cb["W02_current_key_at_insertion"], hd[1]))
w("")
w("另一条源码推断（未执行）：循环先比较 vruntime，再检查 `tmp_list != anchor`。现在比较对象一直停在首节点，所以 W01 走到 anchor 时没有非法读取。如果改成每步从 tmp_list 重新取比较对象，却不调换这两个条件的顺序，走到 anchor 时就会先按 anchor 计算一个并不存在的容器并读取它。")
w("")
w("## 6. 三项消费正负控")
w("")
w("| 控制 | 变造 | 证据状态 | P 非 idle | 与主线 P 判定 | 满足 |")
w("|---|---|---|---|---|---|")
for k, v in res["controls"]["results"].items():
    w("| %s | %s | %s | %s | %s | %s |" % (k, v["transformation"] or "无（真实 W03 记录）", v["evidence_status"], v["P_nonidle"], cell(v["lead_P_nonidle"]), v["met"]))
w("")
w("变造只作用于内存中的副本，没有写回 runs.json。")
w("")
w("## 7. 发现与限制")
w("")
for item in [
    "见证完成不等于内核正确：本轮只说明这个函数在八组有限输入下的边界行为。",
    "游标候选（W01）：比较对象停在首节点 C20，游标却一路前进到 anchor，A25 落在 D30 与 idle 之后、队尾；P 的非 idle 条件与 idle 尾部条件都被破坏。",
    "计费时点候选（W02、W07 第 1 步）：回插按计费前的键定位，计费在回插之后，得到 A25 在 C20 之前；与游标无关。",
    "相等键（W04）与零增量（W03）都满足 P；阻塞任务（W05）不回队但仍计费 10；idle（W06）回到队尾且不计费；W07 第 2 步没有重复计费；W08 第一次不切换时队列、vruntime、last_jiffies 都不变，第二次才记入 5。",
    "W07 第 2 步显示失序的后果：下一次选择取到 A25 而不是更小的 C20。",
    "全局可达性（CA-02）、真实时钟、IRQ、SMP、上下文切换都未执行；P 是 15 号提出的候选约束，不是 Owner 已选的政策；没有修改内核，也没有生成补丁。",
    "执行模型未知；没有 CI 或人工逐行复核。"]:
    w("- " + item)
w("")
w("## 8. 过程记录")
w("")
for item in [
    "正式运行前在会话临时目录做过一次开发构建与运行，用于编写判读程序；正式记录是之后在全新目录中的一次构建与两次运行。",
    "判读程序在开发后加入了逐步 P 与“选中是否为最小键”的观察项；夹具模板在两次运行之间没有改动（runs.json 记录的模板 SHA-256 与提交的 fx_order.c 相同，由 final_check4 核对）。",
    "没有运行原仓库脚本、旧的 run_all/run_recheck/meta_tests 主入口、H00 或全树扫描。"]:
    w("- " + item)
w("")
w("## 9. 提交、推送与回读")
w("")
w("结果批 `%s`，提交前运行 `final_check4.py results`（§9.1）。推送输出（手工转录）：" % RC)
w("")
fence("To https://github.com/08822407d/MyOS2\n   0d62c4d..%s  claude/dazzling-cori-q0dnyt -> claude/dazzling-cori-q0dnyt" % RC[:7], "text")
w("")
fence("$ cd <scratch>/rbtool    # 0d62 的 recheck-01/fixtures/readback2.py（SHA-256[0] 41019aeb61216b01）与 harness2.py（9b9c5c23698a13a1）\n"
      "$ python3 readback2.py %s <scheduler-order-02/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode object --out <scratch>/readback/readback_results.object.json\nreadback object exit=0\n"
      "$ python3 readback2.py %s <scheduler-order-02/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode branch --out <scratch>/readback/readback_results.branch.json\nreadback branch exit=0\n"
      "$ python3 guard4.py <scratch>/readback/guard_after_push.json\nguard exit=0\n$ cp <scratch>/readback/*.json ../observations/after_results/   (cmp 逐字节相同)" % (RC, RC), "text")
w("")
for tag, r in (("object", rbo), ("branch", rbb)):
    w("- %s：ok=%s，分支头关系=%s，期望 %s 字节；通道 `%s`" % (tag, r.get("ok"), r.get("branch_head_relation") or "不适用（object 模式按提交读取）", r.get("expected_bytes"),
      cell([[c.get("channel"), c.get("http_code"), c.get("curl_exit"), c.get("timed_out"), c.get("bytes"), c.get("byte_identical"), c.get("field_ok")] for c in r.get("channels", [])])))
w("- 推送后保护门：gate.ok=`%s`，执行=`%s`，相对 0d62 的已提交变化 %d 项，全部是 scheduler-order-02/ 下的新增。" % (
    (gap.get("gate") or {}).get("ok"), cell(gap.get("execution")), len((gap.get("changes_vs_0d62") or {}).get("committed") or [])))
w("")
w("文档批（本文件、MANIFEST.md、build_evidence4.py、after_results/ 三份记录）在其后提交；提交前运行 `final_check4.py docs %s`（§9.2）。" % RC)
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
