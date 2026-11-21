# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-EXECUTION-HANDOFF-06 (execution-handoff-06/local)
# executor: Claude Code desktop app, local session; model: claude-opus-5-5 as declared by the runtime
#   (not independently attestable)
# source: new file; no code copied from earlier packages.
# purpose: write the minimal local acceptance records with read-only git commands and JSON reads:
#   l01      identity of the fixed objects, 293 ancestry, byte identity of every file tracked at 293,
#            commits after 293 and their write prefixes, cloud asset sizes/sha256 vs cloud/HANDOFF.md;
#   l02      the L02 run (command, return code, versions, checker-chain blobs) and a per-dimension
#            comparison of the local recheck output with the frozen 05 records;
#   l03      mechanical spot check of resume-map.yaml (paths, ids, verbatim verdict values);
#   boundary L04 write-area check of the index (--staged) or of a commit (--rev), plus the remote
#            head of the execution branch (git ls-remote, read-only).
#   It never checks out, fetches, commits or runs repository code. Commit ids are written as
#   12-hex short ids and sha256 values in four 16-hex groups; a 40-hex run refuses the output.
# ---------------------------------------------------------------------------------------------------
"""Usage: python3 -B -P make_local_records.py <repo_root> l01 <out.json> [<downloaded_contract.md>]
       python3 -B -P make_local_records.py <repo_root> l02 <out.json>
       python3 -B -P make_local_records.py <repo_root> l03 <out.json>
       python3 -B -P make_local_records.py <repo_root> boundary <out.json> (--staged | --rev <rev>)"""
import sys
sys.dont_write_bytecode = True

import hashlib  # noqa: E402
import json  # noqa: E402
import os  # noqa: E402
import re  # noqa: E402
import subprocess  # noqa: E402

FROZEN = "293a9e3ef5f4"          # research freeze point (05 document commit)
CLOUD_HEAD = "9c36fb3ba7ff"      # CLOUD_CLOSE commit, parent 293
EXEC = "claude/dazzling-cori-q0dnyt"
LEAD = "agent/MYOS2-LEAD-002"
# prev_exec_05_start: execution head when 05 started (04 document commit; uv_facts_check.REVIEWED);
# the 05 review itself read 293a9e3ef5f4
FIXED = {"results_05": "22934124fe92", "results_04": "eb75b3100601", "prev_exec_05_start": "de7c96ede546",
         "time_pin": "a039d9803ade", "cloud_master_view": "de3bb1df906a"}
PKG = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/"
CLOUD = PKG + "execution-handoff-06/cloud/"
LOCAL = PKG + "execution-handoff-06/local/"
UV05 = PKG + "user-vfs-baseline-05/"
CHAIN = [UV05 + "scripts/uv_facts_check.py", PKG + "mm-baseline-04/scripts/mm_facts_check.py",
         PKG + "scheduler-integration-03/scripts/scan_index.py"]
CONTRACT = "agent-workspace/lead/MYOS2-LEAD-002/20-cloud-to-local-handoff.md"
HEX40 = re.compile(r"\b[0-9a-f]{40}\b")
REPO = None
COMMANDS = []


def git(*args, check=True):
    COMMANDS.append(" ".join(["git"] + list(args)))
    p = subprocess.run(["git", "-C", REPO] + list(args), capture_output=True)
    if check and p.returncode != 0:
        raise SystemExit("git %s failed rc=%d: %s" % (" ".join(args), p.returncode, p.stderr.decode()[:200]))
    return p


def out(*args):
    return git(*args).stdout.decode("utf-8", "replace")


def short(rev):
    p = git("rev-parse", "--short=12", "%s^{commit}" % rev, check=False)
    return p.stdout.decode().strip() if p.returncode == 0 else None


def sha_groups(data):
    h = hashlib.sha256(data).hexdigest()
    return " ".join(h[i:i + 16] for i in range(0, 64, 16))


def tree(rev):
    """path -> (mode, type, blob short id) for every file tracked at rev."""
    rows = {}
    for rec in out("ls-tree", "-r", "-z", "--full-tree", rev).split("\0"):
        if rec:
            meta, path = rec.split("\t", 1)
            mode, typ, obj = meta.split()
            rows[path] = (mode, typ, obj[:12])
    return rows


def index_tree():
    rows = {}
    for rec in out("ls-files", "-s", "-z").split("\0"):
        if rec:
            meta, path = rec.split("\t", 1)
            mode, obj, _stage = meta.split()
            rows[path] = (mode, "blob", obj[:12])
    return rows


def identity(base, other, prefix=""):
    paths = sorted(p for p in base if p.startswith(prefix))
    diff = [p for p in paths if p in other and other[p] != base[p]]
    missing = [p for p in paths if p not in other]
    return {"files": len(paths), "identical": len(paths) - len(diff) - len(missing), "differing": diff,
            "missing": missing, "method": "mode, type and blob id per path; equal blob id means equal bytes"}


def name_status(a, b=None, cached=False):
    args = ["diff", "--name-status", "--no-renames"] + (["--cached"] if cached else []) + [a] + ([b] if b else [])
    rows = []
    for ln in out(*args).splitlines():
        st, path = ln.split("\t", 1)
        rows.append({"status": st, "path": path})
    return rows


def commits_after(base, head):
    rows = []
    for ln in out("rev-list", "--reverse", "--parents", "%s..%s" % (base, head)).splitlines():
        ids = [x[:12] for x in ln.split()]   # short ids also in commands_run
        c = ids[0]
        files = name_status(ids[1], c) if len(ids) == 2 else []
        prefixes = sorted({"cloud" if f["path"].startswith(CLOUD) else "local" if f["path"].startswith(LOCAL) else "other"
                           for f in files})
        rows.append({"commit": c[:12], "parents": [p[:12] for p in ids[1:]],
                     "subject": out("log", "-1", "--format=%s", c).strip(), "files": len(files),
                     "statuses": sorted({f["status"] for f in files}), "write_prefixes": prefixes})
    return rows


def handoff_assets(text):
    rows = {}
    for m in re.finditer(r"^\|\s*(A\d)\s*\|\s*(assets/\S+)\s*\|\s*([\d,]+)\s*\|\s*([0-9a-f ]{67})\s*\|", text, re.M):
        rows[m.group(2)] = {"id": m.group(1), "bytes": int(m.group(3).replace(",", "")), "sha256": m.group(4).strip()}
    return rows


def l01(argv):
    res = {"record": "L01_INPUT_IDENTITY", "repo_root": REPO}
    refs = {"HEAD": "HEAD", "exec_remote": "origin/" + EXEC, "time_remote": "origin/time",
            "master_remote": "origin/master", "lead_remote": "origin/" + LEAD, "frozen": FROZEN, "cloud_head": CLOUD_HEAD}
    refs.update(FIXED)
    res["refs"] = {k: {"ref": v if not re.match(r"^[0-9a-f]{12}$", v) else "short id", "short12": short(v),
                       "date": out("log", "-1", "--format=%cI", v).strip() if short(v) else None}
                   for k, v in refs.items()}
    res["time_matches_pin"] = res["refs"]["time_remote"]["short12"] == FIXED["time_pin"]
    res["frozen_is_ancestor_of_head"] = git("merge-base", "--is-ancestor", FROZEN, "HEAD", check=False).returncode == 0
    res["cloud_master_view_is_ancestor_of_master"] = git("merge-base", "--is-ancestor", FIXED["cloud_master_view"],
                                                         "origin/master", check=False).returncode == 0
    res["master_commits_after_cloud_view"] = [ln[:12] + ln[40:] for ln in
                                              out("log", "--first-parent", "--format=%H %s",
                                                  "%s..origin/master" % FIXED["cloud_master_view"]).splitlines()]
    res["commits_after_frozen"] = commits_after(FROZEN, "HEAD")
    res["diff_frozen_to_head"] = name_status(FROZEN, "HEAD")
    res["frozen_files_identity_at_head"] = identity(tree(FROZEN), tree("HEAD"))
    head_tree = tree("HEAD")
    hand = out("show", "HEAD:%sHANDOFF.md" % CLOUD)
    m = re.search(r"^cloud_writer_state:\s*(\S+)", hand, re.M)
    res["cloud_handoff"] = {"path": CLOUD + "HANDOFF.md", "present": True,
                            "cloud_writer_state": m.group(1) if m else None,
                            "status": (re.search(r"^status:\s*(\S+)", hand, re.M) or [None, None])[1],
                            "scratch_preservation": (re.search(r"^scratch_preservation:\s*(\S+)", hand, re.M) or [None, None])[1]}
    table = handoff_assets(hand)
    assets = []
    for p in sorted(x for x in head_tree if x.startswith(CLOUD + "assets/")):
        data = git("show", "HEAD:" + p).stdout
        rel = p[len(CLOUD):]
        want = table.get(rel, {})
        got = {"bytes": len(data), "sha256": sha_groups(data)}
        assets.append({"path": rel, "id": want.get("id"), **got,
                       "matches_handoff_table": bool(want) and want["bytes"] == got["bytes"] and want["sha256"] == got["sha256"]})
    res["cloud_assets"] = {"rows": assets, "table_rows": len(table), "all_match": bool(assets) and all(a["matches_handoff_table"] for a in assets)
                           and len(table) == len(assets)}
    lead_blob = git("show", "origin/%s:%s" % (LEAD, CONTRACT)).stdout
    res["contract"] = {"path": CONTRACT, "branch": LEAD, "bytes": len(lead_blob), "sha256": sha_groups(lead_blob)}
    if argv:
        raw = open(argv[0], "rb").read()
        res["contract"]["downloaded_raw_bytes_equal"] = raw == lead_blob
    res["working_tree_porcelain"] = out("status", "--porcelain", "--untracked-files=all").splitlines()
    return res


def counts(r):
    return {"anchors": [r["anchors"]["ok"], r["anchors"]["total"]], "anchors_by_kind": r["anchors"]["by_kind"],
            "markdown_tags": {k: [v["ok"], v["total"]] for k, v in r["markdown_tags"].items()},
            "structure": {"ok": r["structure"]["ok"], "errors": len(r["structure"]["errors"]), "limits": r["structure"]["limits"],
                          "recount": r["structure"]["recount"]},
            "deferred_findings": {"ok": r["deferred_findings"]["ok"], "errors": len(r["deferred_findings"]["errors"]),
                                  "recount": r["deferred_findings"]["recount"]},
            "queries": {k: r["queries"][k] if k != "errors" else len(r["queries"][k]) for k in ("total", "tool_failures", "errors", "ok")},
            "structure_checks": [r["structure_checks"]["ok"], r["structure_checks"]["total"]],
            "structure_check_rows": [[x["id"], x["check"], x["ok"]] for x in r["structure_checks"]["rows"]],
            "value_tables": {"ok": r["value_tables"]["ok"],
                             "VT-MR01_rows_ok": [sum(x["ok"] for x in r["value_tables"]["VT-MR01"]), len(r["value_tables"]["VT-MR01"])],
                             "VT-FR01_rows_ok": [sum(x["ok"] for x in r["value_tables"]["VT-FR01"]), len(r["value_tables"]["VT-FR01"])],
                             "VT-FR01_differs_from_intended": sum(x["differs_from_intended"] for x in r["value_tables"]["VT-FR01"])},
            "supersedes_scope": {"ok": r["supersedes_scope"]["ok"], "total": r["supersedes_scope"]["total"],
                                 "superseded": r["supersedes_scope"]["superseded"],
                                 "reviewed_retained": r["supersedes_scope"]["total"] - r["supersedes_scope"]["superseded"]},
            "hex40_in_package": len(r["hex40_in_package"]), "all_ok": r["all_ok"]}


def l02(argv):
    rec = os.path.join(REPO, LOCAL, "records", "l02")
    frozen = os.path.join(REPO, UV05, "records")
    res = {"record": "L02_UV05_STATIC_RECHECK"}
    rd = lambda *p: open(os.path.join(*p), "rb").read()  # noqa: E731
    run = json.loads(rd(rec, "run.json"))
    res["run"] = run
    import yaml
    res["versions"] = {"python": sys.version.split()[0], "pyyaml": yaml.__version__, "pyyaml_origin": yaml.__file__,
                       "git": subprocess.run(["git", "--version"], capture_output=True).stdout.decode().strip()}
    res["checker_chain_unmodified"] = [{"path": p, "blob_at_frozen": tree(FROZEN)[p][2], "blob_at_head": tree("HEAD")[p][2],
                                        "sha256_worktree": sha_groups(rd(REPO, p))} for p in CHAIN]
    files = {}
    for name in ("facts_check.json", "queries.json"):
        a, b = rd(rec, name), rd(frozen, name)
        files[name] = {"local_bytes": len(a), "frozen_bytes": len(b), "byte_identical": a == b,
                       "local_sha256": sha_groups(a), "frozen_sha256": sha_groups(b),
                       "frozen_last_commit": out("log", "-1", "--format=%h", "--abbrev=12", "--", UV05 + "records/" + name).strip()}
    res["outputs_vs_frozen"] = files
    lc, fc = counts(json.loads(rd(rec, "facts_check.json"))), counts(json.loads(rd(frozen, "facts_check.json")))
    res["dimensions"] = {k: {"local": lc[k], "frozen": fc[k], "equal": lc[k] == fc[k]} for k in lc}
    lq, fq = json.loads(rd(rec, "queries.json")), json.loads(rd(frozen, "queries.json"))
    res["queries_record"] = {"local_total": len(lq["queries"]), "frozen_total": len(fq["queries"]),
                             "per_query_equal": sum(1 for x, y in zip(lq["queries"], fq["queries"]) if x == y),
                             "returncodes": {str(k): sum(1 for q in lq["queries"] if q["returncode"] == k)
                                             for k in sorted({q["returncode"] for q in lq["queries"]})},
                             "pin_time": [lq["pin_time"], fq["pin_time"]]}
    res["all_dimensions_equal"] = all(v["equal"] for v in res["dimensions"].values())
    res["scope_note"] = ("mechanical text consistency at the time pin only; not a semantic review of the 90 anchors, "
                         "not a kernel run; the 12 negative probes were not re-run")
    return res


def l03(argv):
    """Mechanical spot check of resume-map.yaml: entry files exist at HEAD, frozen ids are commits, the
    mainline review exists and each verdict field value appears on its own line in it, errata paths exist.
    Fields prefixed with a parenthesised source note are taken from other files and are not checked here."""
    import yaml
    m = yaml.safe_load(open(os.path.join(REPO, LOCAL, "resume-map.yaml"), encoding="utf-8"))
    head = set(out("ls-tree", "-r", "--name-only", "HEAD").splitlines())
    lead = set(out("ls-tree", "-r", "--name-only", "origin/" + LEAD).splitlines())
    rows = []
    for p in m["packages"]:
        txt = out("show", "origin/%s:%s" % (LEAD, p["mainline_review_path"]))
        files = [f.split("（")[0] for f in p["entry_files"]]
        fields = [v for v in p["verdict_fields_verbatim"] if not v.startswith("（")]
        found = [v for v in fields if v in txt or re.search(r"^%s:\s*%s\s*$" % tuple(map(re.escape, v.split(": ", 1))), txt, re.M)]
        paths = [x for e in p["later_errata_or_supersession"]
                 for x in re.findall(r"agent-workspace/[\w./-]+\.(?:md|yaml|json|py)", e["ref"])]
        rows.append({"package": p["package"],
                     "entry_files_exist": [[f, any(x.startswith(f) for x in head) if f.endswith("/") else f in head] for f in files],
                     "frozen_is_commit": git("cat-file", "-t", p["results_frozen_short12"], check=False).stdout.decode().strip() == "commit",
                     "review_exists": p["mainline_review_path"] in lead,
                     "verdict_fields_checked": len(fields), "verdict_fields_verbatim_ok": len(found),
                     "errata_paths_checked": len(paths), "errata_paths_ok": sum(x in head or x in lead for x in paths)})
    return {"record": "L03_MAP_SPOTCHECK", "source": LOCAL + "resume-map.yaml", "rows": rows,
            "scope_note": "paths, ids and verbatim field values only; not a semantic check of the map text",
            "all_ok": all(all(x[1] for x in r["entry_files_exist"]) and r["frozen_is_commit"] and r["review_exists"]
                          and r["verdict_fields_checked"] == r["verdict_fields_verbatim_ok"]
                          and r["errata_paths_checked"] == r["errata_paths_ok"] for r in rows)}


def boundary(argv):
    res = {"record": "L04_WRITE_AREA_BOUNDARY"}
    if argv[0] == "--staged":
        other, res["target"] = index_tree(), "index (staged)"
        changes = name_status(CLOUD_HEAD, cached=True)
        res["unstaged"] = out("diff", "--name-status", "--no-renames").splitlines()
        res["untracked"] = out("ls-files", "--others", "--exclude-standard").splitlines()
    else:
        rev = argv[1]
        other, res["target"] = tree(rev), "commit %s" % short(rev)
        changes = name_status(CLOUD_HEAD, rev)
        res["commits_after_cloud_head"] = commits_after(CLOUD_HEAD, rev)
    res["changes_vs_cloud_head"] = changes
    res["only_additions_under_local"] = bool(changes) and all(c["status"] == "A" and c["path"].startswith(LOCAL) for c in changes)
    res["frozen_files_identity"] = identity(tree(FROZEN), other)
    res["cloud_files_identity"] = identity(tree(CLOUD_HEAD), other, CLOUD)
    rem = out("ls-remote", "origin", "refs/heads/" + EXEC).split()
    res["remote_exec_head"] = rem[0][:12] if rem else None
    res["remote_exec_head_is_cloud_head"] = bool(rem) and rem[0].startswith(git("rev-parse", CLOUD_HEAD).stdout.decode().strip()[:40])
    return res


def main(argv):
    global REPO
    REPO, mode, outp = os.path.realpath(argv[0]), argv[1], argv[2]
    res = {"l01": l01, "l02": l02, "l03": l03, "boundary": boundary}[mode](argv[3:])
    res["commands_run"] = sorted(set(COMMANDS))
    text = json.dumps(res, ensure_ascii=False, indent=1)
    if HEX40.search(text):
        sys.stderr.write("REFUSED: 40-hex in output\n")
        return 3
    with open(outp, "w", encoding="utf-8") as fh:
        fh.write(text + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
