# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: revised from core/fixtures/readback.py @ a1e7c2277705 (frozen original unchanged)
# change (R04): argparse CLI (the old one left the --out value among positionals); two modes:
#   object - fetch the path at the pinned commit itself (unaffected by later branch pushes);
#   branch - fetch by branch name and accept only an equal head or a descendant head whose blob for
#            the path is identical to the target blob.
#   Every channel records curl exit, timeout, terminal state, HTTP status, bytes, digest, byte identity
#   and the optional parsed field. Full commit ids are used internally and redacted in output.
# --------------------------------------------------------------------------------------------------
"""Usage:
  python3 readback2.py <commit-short12> <path> [field=value] [--mode object|branch] [--branch NAME] --out FILE
Exit: 0 all criteria met, 1 criteria not met, 2 usage/internal error.
"""
import argparse
import os
import sys

import yaml

import harness2 as H

OWNER_REPO = "08822407d/MyOS2"


def _urls(ref, path):
    return {"raw.githubusercontent.com": ("https://raw.githubusercontent.com/%s/%s/%s" % (OWNER_REPO, ref, path), []),
            "api.github.com_contents_raw": ("https://api.github.com/repos/%s/contents/%s?ref=%s" % (OWNER_REPO, path, ref),
                                            ["-H", "Accept: application/vnd.github.raw"])}


def remote_read(target, path, mode="object", branch=H.WORK_BRANCH, field=None, value=None, workdir=None, repo=None, tag="rb"):
    workdir = workdir or H.fresh_dir(os.environ.get("RECHECK_ROOT", "/tmp"), "rb-")
    tfull = H.full(target, repo)
    tshort = H.short12(tfull, repo)
    exp = H.git("show", "%s:%s" % (tfull, path), check=False, repo=repo)
    res = {"mode": mode, "target_commit_short12": tshort, "path": path, "field": field,
           "expected_blob_available": exp.returncode == 0,
           "expected_bytes": len(exp.stdout) if exp.returncode == 0 else None,
           "expected_sha256_segments": H.sha_segments(exp.stdout) if exp.returncode == 0 else None, "channels": []}
    ok = res["expected_blob_available"]
    if mode == "object":
        ref, shown = tfull, "<commit:%s>" % tshort
        res["binding"] = "fetched at the target commit itself"
    else:
        head = H.git("ls-remote", "origin", "refs/heads/" + branch, repo=repo).stdout.decode().split("\t")[0].strip()
        rel = "unknown"
        if head == tfull:
            rel = "equal"
        elif head and H.git("cat-file", "-e", head + "^{commit}", check=False, repo=repo).returncode == 0 \
                and H.is_ancestor(tfull, head, repo):
            same = H.git("show", "%s:%s" % (head, path), check=False, repo=repo)
            rel = "descendant_with_identical_blob" if same.returncode == 0 and same.stdout == exp.stdout else "descendant_with_different_blob"
        elif head:
            rel = "not_on_target_line_or_unknown_locally"
        res["branch"] = branch
        res["branch_head_relation"] = rel
        ok = ok and rel in ("equal", "descendant_with_identical_blob")
        ref, shown = branch, branch
    for name, (url, hdr) in _urls(ref, path).items():
        out = os.path.join(workdir, "%s_%s.bin" % (tag, name.split(".")[0]))
        r = H.run_pg(["curl", "-sS", "--max-time", "25", "-w", "%{http_code}", "-o", out, *hdr, url], 30)
        body = open(out, "rb").read() if os.path.exists(out) else b""
        ch = {"channel": name, "url": url.replace(ref, shown) if mode == "object" else url,
              "curl_exit": r["returncode"], "timed_out": r["timed_out"], "terminal_state": r["terminal_state"],
              "http_code": r["stdout"].strip(), "curl_stderr": r["stderr"].strip()[:300], "bytes": len(body),
              "sha256_segments": H.sha_segments(body)}
        ch["transport_ok"] = ch["curl_exit"] == 0 and not ch["timed_out"] and ch["terminal_state"] == "exited" and ch["http_code"] == "200"
        ch["byte_identical"] = res["expected_blob_available"] and body == exp.stdout
        if field:
            try:
                got = (yaml.safe_load(body.decode("utf-8")) or {}).get(field) if ch["transport_ok"] else None
            except Exception as e:  # noqa: BLE001
                got = "PARSE_ERROR:%s" % type(e).__name__
            ch["field_value"] = got
            ch["field_ok"] = got is not None and str(got) == value
        else:
            ch["field_ok"] = True
        ch["ok"] = ch["transport_ok"] and ch["byte_identical"] and ch["field_ok"]
        ok = ok and ch["ok"]
        res["channels"].append(ch)
    res["ok"] = bool(ok)
    return res


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument("target")
    ap.add_argument("path")
    ap.add_argument("field_value", nargs="?", default=None, help="optional NAME=VALUE checked in the parsed YAML")
    ap.add_argument("--mode", choices=("object", "branch"), default="object")
    ap.add_argument("--branch", default=H.WORK_BRANCH)
    ap.add_argument("--out", required=True)
    try:
        a = ap.parse_args(argv)
    except SystemExit as e:
        return 2 if e.code else 0
    field = value = None
    if a.field_value is not None:
        if "=" not in a.field_value:
            sys.stderr.write("field_value must be NAME=VALUE\n")
            return 2
        field, value = a.field_value.split("=", 1)
    try:
        r = remote_read(a.target, a.path, a.mode, a.branch, field, value,
                        workdir=H.fresh_dir(os.path.dirname(os.path.abspath(a.out)) or ".", "rbwork-"))
    except Exception as e:  # noqa: BLE001
        sys.stderr.write("readback error %s: %s\n" % (type(e).__name__, e))
        return 2
    H.emit(r, a.out)
    return 0 if r["ok"] else 1


if __name__ == "__main__":
    sys.exit(main())
