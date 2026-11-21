"""Canonical core run: identity/gate checks, H00 hardening, V00, V01, static checks, fixtures, evaluation.
Each stage is isolated (subprocess with a hard limit, or try/except); a failing stage is recorded
and the remaining independent stages still run. Output: $CORE_WORK/core_run.json
Usage: CORE_WORK=<scratch dir> python3 run_all.py
"""
import datetime
import os
import platform
import shutil
import sys

import yaml

import harness as H
import build as B
import evaluate as E
import locate as L

HERE = os.path.dirname(os.path.abspath(__file__))
LEAD = "agent-workspace/lead/MYOS2-LEAD-002/"
TECH = ["07-scheduler-wakeup-timer-audit.md", "07-core-audit-map.yaml", "MANIFEST.md",
        "09-local-verification-contract.md", "10-cloud-pilot-and-github-handoff.md", "11-core-verification-cloud.md"]
REVIEW = LEAD + "reviews/CORE-CHECK-01-pilot-review.md"
WORK_BRANCH = "claude/dazzling-cori-q0dnyt"
LIMIT = 600


def stage_script(name):
    r = H.run_pg([sys.executable, os.path.join(HERE, name)], LIMIT, cwd=HERE)
    return {"script": name, "returncode": r["returncode"], "timed_out": r["timed_out"], "elapsed_s": r["elapsed_s"],
            "stderr": r["stderr"][-2000:], "stdout_tail": r["stdout"][-600:]}


def identity():
    idt = {"read_time_utc": datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
           "pins": {k: H.short12(k) for k in H.PINS}}
    remote = {}
    for ln in H._git("ls-remote", "origin", "refs/heads/master", "refs/heads/time",
                     "refs/heads/agent/MYOS2-LEAD-002", "refs/heads/" + WORK_BRANCH).stdout.decode().split("\n"):
        if "\t" in ln:
            oid, ref = ln.split("\t")
            remote[ref.replace("refs/heads/", "")] = oid
    idt["remote_heads_match"] = {
        "master==pin.master": remote.get("master") == H.commit("master"),
        "time==pin.time": remote.get("time") == H.commit("time"),
        "agent/MYOS2-LEAD-002==pin.review": remote.get("agent/MYOS2-LEAD-002") == H.commit("review"),
        WORK_BRANCH + "==pin.pilot_head": remote.get(WORK_BRANCH) == H.commit("pilot_head"),
    }
    idt["taskbook_pin_is_ancestor_of_review_pin"] = H._git(
        "merge-base", "--is-ancestor", H.commit("taskbook"), H.commit("review"), check=False).returncode == 0
    same = {}
    for f in TECH:
        a, b = H.blob("taskbook", LEAD + f), H.blob("review", LEAD + f)
        same[f] = {"identical": a == b, "bytes": len(a), "sha256_segments": H.sha256_segments(a)}
    idt["six_technical_inputs_taskbook_vs_review"] = same
    added = H._git("diff", "--name-status", H.commit("taskbook"), H.commit("review")).stdout.decode().split("\n")
    idt["taskbook_to_review_changes"] = [x for x in added if x]
    rv = H.blob("review", REVIEW)
    t = rv.decode("utf-8")
    fm = yaml.safe_load(t[4:t.index("\n---\n", 4)])
    idt["review"] = {"path": REVIEW, "bytes": len(rv), "sha256_segments": H.sha256_segments(rv),
                     "fields": {k: fm.get(k) for k in ("packet_id", "disposition", "reviewed_pr", "reviewed_branch",
                                                       "reviewed_commit_short12", "pilot_result_commit_short12")},
                     "reviewed_input_refs": fm.get("reviewed_input_refs")}
    f = idt["review"]["fields"]
    idt["gate"] = {
        "disposition_is_ALLOW_CORE": f.get("disposition") == "ALLOW_CORE",
        "packet_matches": f.get("packet_id") == "MYOS2-LEAD-002-CORE-CHECK-01",
        "pr_matches_17": f.get("reviewed_pr") == 17,
        "branch_matches": f.get("reviewed_branch") == WORK_BRANCH,
        "reviewed_head_matches_pin": str(f.get("reviewed_commit_short12")) == H.short12("pilot_head"),
        "reviewed_inputs_match_pins": (fm.get("reviewed_input_refs") or {}).get("kernel", {}).get("short12") == H.short12("time")
        and (fm.get("reviewed_input_refs") or {}).get("workspace", {}).get("short12") == H.short12("master"),
    }
    idt["gate"]["ALLOW_CORE_bound_to_this_execution"] = all(idt["gate"].values())
    return idt


def tools():
    def ver(cmd):
        exe = shutil.which(cmd[0])
        if not exe:
            return None
        r = H.run_pg(cmd, 10)
        return (r["stdout"] or r["stderr"]).splitlines()[0]
    return {"cpu_arch": platform.machine(), "kernel_release": platform.release(), "python3": sys.version.split()[0],
            "gcc": ver(["gcc", "--version"]), "clang": ver(["clang", "--version"]), "git": ver(["git", "--version"]),
            "nm": ver(["nm", "--version"]), "pyyaml": yaml.__version__}


def fixtures():
    W = H.WORK
    fx = {}
    fx["fx_wait"] = B.build_and_run("fx_wait", os.path.join(HERE, "fx_wait.c"), [
        "v02_single", "v03_second_wake_direct", "v03_second_wake_via_complete", "v03_all_two_waiters",
        "v09_schedule_timeout_values", "v09_uninterruptible_wrapper", "v09_msleep_bounded",
        "v10_done_preset_fast_path", "v10_infinite_notify_during_schedule", "v10_finite_timeout_no_notifier",
        "v11_wait_then_notify_then_reuse"], os.path.join(W, "fx_wait"))
    fx["fx_sched"] = B.build_and_run("fx_sched", os.path.join(HERE, "fx_sched.c"), [
        "v04_noncurrent_wake", "v05_state_not_in_mask", "v06_double_wake", "v07_cpu_metadata",
        "v08_pick_combinations", "v08_sequence_idle_requeue", "v08_sequence_idle_switched_out_blocked",
        "v08_vruntime_requeue_order"], os.path.join(W, "fx_sched"))
    if platform.machine() == "x86_64":
        fx["fx_prims"] = B.build_and_run("fx_prims", os.path.join(HERE, "fx_prims.c"),
                                         ["v12_add_test_negative", "v13_trylock"], os.path.join(W, "fx_prims"))
    else:
        fx["fx_prims"] = {"fixture": "fx_prims", "status": "BLOCKED_NOT_X86_64", "cases": {}}
    d = os.path.join(W, "fx_jiffies")
    os.makedirs(d, exist_ok=True)
    lds = H.blob("time", "mykernel/arch/x86_64/kernel.lds").decode("utf-8")
    cand = L.ld_assignment(lds, "jiffies")
    line = lds.split("\n")[cand[0]["start"] - 1]
    with open(os.path.join(d, "alias.ld"), "w") as f:
        f.write(line + "\n")
    fx["fx_jiffies"] = B.build_and_run("fx_jiffies", os.path.join(HERE, "fx_jiffies.c"), ["v14"], d,
                                       extra_inputs=[os.path.join(d, "alias.ld")])
    fx["fx_jiffies"]["alias_ld"] = {"source": "time:mykernel/arch/x86_64/kernel.lds", "line": cand[0]["start"],
                                    "text": line}
    fx["fx_jiffies_control"] = B.build_and_run("fx_jiffies_control", os.path.join(HERE, "fx_jiffies.c"),
                                               ["v14_control"], os.path.join(W, "fx_jiffies_control"),
                                               extra_cflags=["-DCONTROL_SEPARATE"])
    return fx


def evaluate(fx):
    out = {}
    table = [("V02", E.v02, ["fx_wait"]), ("V03", E.v03, ["fx_wait"]), ("V09", E.v09, ["fx_wait"]),
             ("V10", E.v10, ["fx_wait"]), ("V11", E.v11, ["fx_wait"]), ("V04", E.v04, ["fx_sched"]),
             ("V05", E.v05, ["fx_sched"]), ("V06", E.v06, ["fx_sched"]), ("V07", E.v07, ["fx_sched"]),
             ("V08", E.v08, ["fx_sched"]), ("V12", E.v12, ["fx_prims"]), ("V13", E.v13, ["fx_prims"]),
             ("V14", E.v14, ["fx_jiffies", "fx_jiffies_control"])]
    for vid, fn, deps in table:
        recs = [fx.get(d, {}) for d in deps]
        if any(r.get("status") != "BUILT" for r in recs):
            out[vid] = {"verdict": "BLOCKED", "reason": [r.get("status") for r in recs]}
            continue
        try:
            out[vid] = fn(*recs)
        except Exception as e:  # noqa: BLE001
            out[vid] = {"verdict": "ERROR", "reason": "%s: %s" % (type(e).__name__, e)}
    return out


def main():
    res = {"check": "CORE_RUN"}
    for name, fn in (("identity", identity), ("tools", tools)):
        try:
            res[name] = fn()
        except Exception as e:  # noqa: BLE001
            res[name] = {"error": "%s: %s" % (type(e).__name__, e)}
    res["stages"] = {s: stage_script(s) for s in ("h00_hardening.py", "v00_anchors.py", "v01_structure.py", "static_checks.py")}
    try:
        fx = fixtures()
    except Exception as e:  # noqa: BLE001
        fx = {"error": "%s: %s" % (type(e).__name__, e)}
    res["fixtures"] = fx
    res["evaluation"] = evaluate(fx) if "error" not in fx else {}
    H.emit(res, os.path.join(H.WORK, "core_run.json"), echo=False)
    summ = {k: (v.get("verdict") if isinstance(v, dict) and "verdict" in v else
                {kk: vv.get("verdict") for kk, vv in v.items() if isinstance(vv, dict)}) for k, v in res["evaluation"].items()}
    print(H.dump({"gate": res.get("identity", {}).get("gate"), "remote": res.get("identity", {}).get("remote_heads_match"),
                  "stages": {k: [v["returncode"], v["timed_out"]] for k, v in res["stages"].items()},
                  "fixtures": {k: v.get("status") for k, v in fx.items() if isinstance(v, dict)}, "evaluation": summ}))


if __name__ == "__main__":
    main()
