"""V00: mechanical check of the 07 report anchors A01-A47 against the pinned time commit,
branch canaries (time vs master), and the executor's write boundary.

Per anchor: tag path/symbol, A-id, language, quote length (1-5), byte-exact contiguous whole-line
match in time, definition-boundary candidates for the tagged symbol, whether the matched quote lies
inside a candidate, quote lines inside comments, enclosing preprocessor conditionals.
Semantic support is NOT decided here; it is merged from v00_semantic_review.yaml (executor reading).
Usage: python3 v00_anchors.py [--context DIR]
"""
import os
import re
import sys

import yaml

import harness as H
import locate as L

REPORT = "agent-workspace/lead/MYOS2-LEAD-002/07-scheduler-wakeup-timer-audit.md"
MANIFEST = "agent-workspace/lead/MYOS2-LEAD-002/MANIFEST.md"
HERE = os.path.dirname(os.path.abspath(__file__))
TAG = re.compile(r"^\[" + "VERIFIED" + r" ([^\]]+)\]\s*$")
AID = re.compile(r"^A(\d{2})｜")


def parse_report(text):
    lines = text.split("\n")
    anchors, last_a, i = [], None, 0
    while i < len(lines):
        m = AID.match(lines[i])
        if m:
            last_a = "A" + m.group(1)
        t = TAG.match(lines[i])
        if t:
            path, _, sym = t.group(1).partition("::")
            fence = lines[i + 1] if i + 1 < len(lines) else ""
            q, j = [], i + 2
            ok_fence = fence.startswith("```")
            while ok_fence and j < len(lines) and lines[j] != "```":
                q.append(lines[j]); j += 1
            anchors.append({"id": last_a, "tag_line": i + 1, "path": path, "symbol": sym,
                            "fence_lang": fence[3:].strip() if ok_fence else None,
                            "fence_ok": ok_fence and j < len(lines), "quote": q})
            last_a = None
            i = j
        i += 1
    return anchors


def comment_only_view(path, text):
    if path.endswith((".c", ".h", ".S", ".lds")):
        return L.strip_c(text)
    # CMake / shell: blank '#' comments but keep quoted text
    out = []
    for ln in text.split("\n"):
        q, cut = None, None
        for k, ch in enumerate(ln):
            if q:
                if ch == q:
                    q = None
            elif ch in "\"'" and not path.endswith((".cmake", "CMakeLists.txt")) or ch == "\"":
                q = ch
            elif ch == "#" and (k == 0 or ln[k - 1] != "$"):
                cut = k; break
        out.append(ln if cut is None else ln[:cut] + " " * (len(ln) - cut))
    return "\n".join(out)


def candidates(path, text, sym):
    if path.endswith((".c", ".h")):
        c = L.c_function(text, sym) or L.c_macro(text, sym) or L.c_struct(text, sym)
        if not c and sym.endswith("_sched_class"):
            c = L.c_macro_generated_initializer(text, "DEFINE_SCHED_CLASS", sym[:-len("_sched_class")])
        return c
    if path.endswith((".cmake", "CMakeLists.txt")):
        return L.cmake_command(text, sym)
    if path.endswith(".lds"):
        return L.ld_assignment(text, sym)
    if path.endswith(".S"):
        return L.asm_symbol(text, sym)
    return []


def check_anchor(a, ctx_dir=None):
    r = {k: a[k] for k in ("id", "tag_line", "path", "symbol", "fence_lang", "fence_ok")}
    q = a["quote"]
    r["quote_lines"] = len(q)
    r["length_1_to_5"] = 1 <= len(q) <= 5
    r["quote_has_tab"] = any("\t" in x for x in q)
    r["quote_has_backslash"] = any("\\" in x for x in q)
    r["path_exists_in_time"] = H.exists("time", a["path"])
    if not r["path_exists_in_time"]:
        r["verdict_mech"] = "PATH_MISSING"
        return r
    data = H.blob("time", a["path"])
    text = data.decode("utf-8")
    lines = text.split("\n")
    r["file_lines"] = len(lines) - (1 if text.endswith("\n") else 0)
    hits = L.find_contiguous(lines, q)
    r["contiguous_hits"] = hits
    r["substring_hits_first_line"] = sum(1 for x in lines if q and q[0] in x)
    if a["symbol"] == "(top-level)" and a["path"].endswith(".sh"):
        fns = L.shell_functions(text)
        r["shell_functions"] = [{"name": f[0], "start": f[1], "end": f[2]} for f in fns]
        inside_fn = [h for h in hits for f in fns if f[1] <= h and h + len(q) - 1 <= f[2]]
        r["boundary_method"] = "heuristic shell function ranges; quote must lie outside all of them"
        r["within_definition"] = bool(hits) and not inside_fn
        r["definition_candidates"] = []
    else:
        cands = candidates(a["path"], text, a["symbol"])
        r["definition_candidates"] = cands
        within, partial = [], []
        for h in hits:
            e = h + len(q) - 1
            for c in cands:
                if c["start"] <= h and e <= c["end"]:
                    within.append({"hit": h, "cand": [c["start"], c["end"]]})
                elif c["start"] <= e and h <= c["end"]:
                    partial.append({"hit": h, "cand": [c["start"], c["end"]],
                                    "lines_outside": [x for x in range(h, e + 1)
                                                      if not c["start"] <= x <= c["end"]]})
        r["within"] = within
        r["partial_overlap"] = partial
        r["within_definition"] = bool(within)
    view = comment_only_view(a["path"], text)
    r["quote_lines_in_comment"] = sorted({x for h in hits for x in L.lines_blank_in_view(view, h, len(q), text)})
    r["quote_blank_lines"] = sum(1 for x in q if not x.strip())
    if a["path"].endswith((".c", ".h", ".S")) and hits:
        r["pp_stack_at_quote"] = L.pp_stack(text, hits[0])
    nonblank = len(q) - r["quote_blank_lines"]
    if not hits:
        v = "QUOTE_NOT_FOUND"
    elif not r["within_definition"]:
        v = "OUTSIDE_OR_PARTIAL_DEFINITION" if r.get("partial_overlap") else "BOUNDARY_NOT_LOCATED"
    elif r["quote_lines_in_comment"] and len(r["quote_lines_in_comment"]) >= nonblank * len(hits):
        v = "MATCH_BUT_ALL_LINES_COMMENTED"
    elif r["quote_lines_in_comment"]:
        v = "MATCH_IN_DEFINITION_INCLUDES_COMMENT_LINE"
    elif not r["length_1_to_5"]:
        v = "LENGTH_OUT_OF_RANGE"
    else:
        v = "MATCH_IN_DEFINITION"
    r["verdict_mech"] = v
    if ctx_dir and hits:
        c = (r.get("within") or r.get("partial_overlap") or [{"cand": [max(1, hits[0] - 15), hits[0] + 15]}])[0]["cand"]
        s, e = max(1, c[0] - 3), min(len(lines), c[1] + 2)
        with open(os.path.join(ctx_dir, "%s.txt" % a["id"]), "w", encoding="utf-8") as f:
            f.write("%s %s::%s  lines %d-%d (hit %s)\n" % (a["id"], a["path"], a["symbol"], s, e, hits))
            for k in range(s, e + 1):
                f.write("%5d%s %s\n" % (k, ">" if any(h <= k < h + len(q) for h in hits) else " ", lines[k - 1]))
    return r


def canaries():
    fm_text = H.blob("taskbook", MANIFEST).decode("utf-8")
    fm = yaml.safe_load(fm_text[4:fm_text.index("\n---\n", 4)])
    src = {"options_flags_cmake": "mykernel/scripts/options_flags.cmake",
           "panic_c_panic": "mykernel/debug/panic.c"}
    out = []
    for key, path in src.items():
        q = fm["branch_canary_quotes"]["time"][key].encode("utf-8")
        t = H.blob("time", path); m = H.blob("master", path)
        tl = sum(1 for x in t.split(b"\n") if x == q)
        out.append({"key": key, "path": path, "quote_repr": repr(q.decode()),
                    "time_whole_line": tl, "master_substring": m.count(q),
                    "verdict": "DISCRIMINATES" if tl >= 1 and m.count(q) == 0 else
                    ("CANARY_NO_LONGER_DISCRIMINATES" if tl >= 1 else "TIME_QUOTE_NOT_FOUND")})
    return out


def protection():
    st = H._git("status", "--porcelain", "--untracked-files=all").stdout.decode().split("\n")
    core = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/"
    outside = [l for l in st if l and not l[3:].startswith(core)]
    head = H._git("rev-parse", "HEAD").stdout.decode().strip()
    anc = H._git("merge-base", "--is-ancestor", H.commit("pilot_head"), head, check=False).returncode == 0
    pilot_diff = H._git("diff", "--name-only", H.commit("pilot_head"), head, "--",
                        "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/").stdout.decode().split()
    branch = H._git("rev-parse", "--abbrev-ref", "HEAD").stdout.decode().strip()
    return {"work_branch": branch, "head_short12": H._git("rev-parse", "--short=12", head).stdout.decode().strip(),
            "head_descends_from_pilot_head": anc, "pilot_files_changed_since_pilot_head": pilot_diff,
            "worktree_changes_outside_core_dir": outside}


def main():
    ctx = None
    if "--context" in sys.argv:
        ctx = sys.argv[sys.argv.index("--context") + 1]
        os.makedirs(ctx, exist_ok=True)
    rep = H.blob("taskbook", REPORT)
    text = rep.decode("utf-8")
    anchors = parse_report(text)
    res = {"check": "V00", "pins": {k: H.short12(k) for k in ("time", "master", "taskbook")},
           "report": {"path": REPORT, "bytes": len(rep), "sha256_segments": H.sha256_segments(rep)}}
    ids = [a["id"] for a in anchors]
    res["tag_count_regex"] = len(anchors)
    res["tag_count_raw_substring"] = text.count("[" + "VERIFIED")
    res["ids_unique"] = len(set(ids)) == len(ids)
    res["ids_missing_from_A01_A47"] = ["A%02d" % k for k in range(1, 48) if "A%02d" % k not in ids]
    res["ids_duplicated"] = sorted({x for x in ids if ids.count(x) > 1 and x})
    res["ids_unassigned_tags"] = [a["tag_line"] for a in anchors if not a["id"]]
    res["ids_order_in_report"] = ids
    res["anchors"] = [check_anchor(a, ctx) for a in anchors]
    sem_path = os.path.join(HERE, "v00_semantic_review.yaml")
    sem = yaml.safe_load(open(sem_path, encoding="utf-8")) if os.path.exists(sem_path) else {}
    for r in res["anchors"]:
        s = (sem.get("anchors") or {}).get(r["id"])
        r["semantic"] = s if s else {"verdict": "NOT_REVIEWED"}
    counts = {}
    for r in res["anchors"]:
        counts[r["verdict_mech"]] = counts.get(r["verdict_mech"], 0) + 1
    res["mechanical_verdict_counts"] = counts
    sc = {}
    for r in res["anchors"]:
        sc[r["semantic"]["verdict"]] = sc.get(r["semantic"]["verdict"], 0) + 1
    res["semantic_verdict_counts"] = sc
    res["canaries"] = canaries()
    res["protection"] = protection()
    H.emit(res, os.path.join(H.WORK, "v00.json"), echo=False)
    print("tags=%d raw=%d ids_unique=%s missing=%s mech=%s sem=%s" % (
        res["tag_count_regex"], res["tag_count_raw_substring"], res["ids_unique"],
        res["ids_missing_from_A01_A47"], counts, sc))


if __name__ == "__main__":
    main()
