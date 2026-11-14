# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file (batch 2)
# purpose: compare the observations committed in an earlier recheck-01 commit with a new set on disk,
#   so that the re-run after the batch-2 fix is shown to reproduce the batch-1 same-source results
#   (or the differences are listed). Read-only; runs nothing.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 compare_runs2.py <old_commit_short12> <new_observations_dir> <out.json>"""
import json
import os
import sys

import harness2 as H

OBS = H.RECHECK_PREFIX + "observations/"


def diff_paths(a, b, path=""):
    """List the JSON paths where two values differ."""
    if isinstance(a, dict) and isinstance(b, dict):
        out = []
        for k in sorted(set(a) | set(b)):
            out += diff_paths(a.get(k), b.get(k), "%s/%s" % (path, k)) if (k in a and k in b) else ["%s/%s (only in %s)" % (path, k, "old" if k in a else "new")]
        return out
    if isinstance(a, list) and isinstance(b, list) and len(a) == len(b):
        out = []
        for i, (x, y) in enumerate(zip(a, b)):
            out += diff_paths(x, y, "%s[%d]" % (path, i))
        return out
    return [] if a == b else [path or "/"]


def main(old_c, new_dir, out):
    old = lambda n: json.loads(H.blob(old_c, OBS + n))
    new = lambda n: json.load(open(os.path.join(new_dir, n), encoding="utf-8"))
    ro, rn = old("run.json"), new("run.json")
    res = {"check": "COMPARE_RUNS2", "old_commit_short12": H.short12(old_c), "new_dir": os.path.realpath(new_dir)}
    res["run_summary"] = {k: {"old": ro.get(k), "new": rn.get(k), "same": ro.get(k) == rn.get(k)}
                          for k in ("status", "exit_code", "execution_complete", "verification_findings", "infrastructure_errors",
                                    "dynamic_calls")}
    res["identity_gate_ok"] = {"old": (ro.get("identity") or {}).get("gate", {}).get("ok"),
                               "new": (rn.get("identity") or {}).get("gate", {}).get("ok")}
    res["identity_head"] = {"old": (ro.get("identity") or {}).get("execution"), "new": (rn.get("identity") or {}).get("execution")}
    probes = lambda r: [[p.get("probe"), p.get("as_expected"), p.get("terminal_state"), p.get("kill"), p.get("reaped_confirmed")]
                        for p in (r.get("h00") or {}).get("probes", [])]
    res["h00_probes_same"] = probes(ro) == probes(rn)
    res["evaluation_identical"] = json.dumps(ro.get("evaluation"), sort_keys=True) == json.dumps(rn.get("evaluation"), sort_keys=True)
    res["evaluation_diff_paths"] = diff_paths(ro.get("evaluation"), rn.get("evaluation"))
    fx = {}
    for name in sorted(set(ro.get("fixtures") or {}) | set(rn.get("fixtures") or {})):
        a, b = (ro.get("fixtures") or {}).get(name) or {}, (rn.get("fixtures") or {}).get(name) or {}
        cases = sorted(set(a.get("cases") or {}) | set(b.get("cases") or {}))
        same_cases = [c for c in cases if ((a.get("cases") or {}).get(c) or {}).get("run", {}).get("stdout") ==
                      ((b.get("cases") or {}).get(c) or {}).get("run", {}).get("stdout")
                      and ((a.get("cases") or {}).get(c) or {}).get("run", {}).get("returncode") ==
                      ((b.get("cases") or {}).get(c) or {}).get("run", {}).get("returncode")]
        fx[name] = {"cases": len(cases), "stdout_and_exit_identical": len(same_cases),
                    "expanded_source_sha256_first_segment": [(a.get("expanded_source") or {}).get("sha256_segments", [None])[0],
                                                             (b.get("expanded_source") or {}).get("sha256_segments", [None])[0]]}
    res["fixtures"] = fx
    res["stage_outputs"] = {n: diff_paths(old(n), new(n)) for n in ("v00.json", "v01.json", "static.json", "a46.json")}
    res["same_source_results_reproduced"] = bool(
        all(v["same"] for v in res["run_summary"].values()) and res["h00_probes_same"] and res["evaluation_identical"]
        and all(v["cases"] == v["stdout_and_exit_identical"] and v["expanded_source_sha256_first_segment"][0] ==
                v["expanded_source_sha256_first_segment"][1] for v in fx.values())
        and all(all("head_short12" in p for p in v) for v in res["stage_outputs"].values()))
    res["note_template"] = "differences limited to the recorded execution-branch head are expected: the head moved by the batch-1 commit"
    H.emit(res, out)
    print(json.dumps({"same_source_results_reproduced": res["same_source_results_reproduced"],
                      "stage_output_diff_paths": res["stage_outputs"]}, ensure_ascii=False))
    return 0 if res["same_source_results_reproduced"] else 1


if __name__ == "__main__":
    sys.exit(main(*sys.argv[1:4]))
