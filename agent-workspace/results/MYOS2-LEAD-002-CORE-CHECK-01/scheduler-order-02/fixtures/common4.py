# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-ORDER-02 (scheduler-order-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file (pins, paths and small helpers; same pattern as consumer-fix-02/fixtures/common3.py
#   @ 0d62c4d19711, rewritten for this task's own binding instead of widening the old prefix)
# --------------------------------------------------------------------------------------------------
"""Pins and helpers for scheduler-order-02."""
import hashlib
import json
import os
import re
import subprocess
import sys
import tempfile

REPO = os.environ.get("MYOS2_REPO", "/home/user/MyOS2")
PINS = {
    "time": "a039d9803ade",           # kernel source (unchanged since the pilot)
    "master": "de3bb1df906a",         # PR base
    "lead_prev": "b0aa54db1b70",      # lead head pinned by RECHECK-02
    "lead": "6706013a079a",           # 15 contract + RECHECK-02 closing review, fixed at the start of this task
    "reviewed": "0d62c4d19711",       # execution head reviewed by CORE-CHECK-01-RECHECK-02-REVIEW-001 (all files frozen)
    "consumer_results": "77faf51e9a43",
}
WORK_BRANCH = "claude/dazzling-cori-q0dnyt"
PACKET = "MYOS2-LEAD-002-CORE-CHECK-01"
FOLLOWUP = "CORE-SCHED-ORDER-02"
RESULTS_ROOT = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/"
PREFIX = RESULTS_ROOT + "scheduler-order-02/"
LEAD = "agent-workspace/lead/MYOS2-LEAD-002/"
HEX40 = re.compile(r"[0-9a-fA-F]{40}")
PY = sys.executable
_cache = {}


def git(*args, check=True, timeout=60):
    p = subprocess.run(["git", "-C", REPO, *args], stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=timeout)
    if check and p.returncode != 0:
        raise RuntimeError("git %s rc=%d %s" % (args[0], p.returncode, p.stderr.decode(errors="replace")[:200]))
    return p


def full(rev):
    if rev not in _cache:
        _cache[rev] = git("rev-parse", "--verify", rev + "^{commit}").stdout.decode().strip()
    return _cache[rev]


def short12(rev):
    return git("rev-parse", "--short=12", full(rev)).stdout.decode().strip()


def blob(rev, path):
    return git("show", "%s:%s" % (full(rev), path)).stdout


def tree(rev, prefix=""):
    out = {}
    for ln in git(*(["ls-tree", "-r", full(rev)] + ([prefix] if prefix else []))).stdout.decode().split("\n"):
        if "\t" in ln:
            meta, path = ln.split("\t", 1)
            out[path] = meta.split()[2]
    return out


def is_ancestor(a, b):
    return git("merge-base", "--is-ancestor", full(a), full(b), check=False).returncode == 0


def sha_segments(data):
    h = hashlib.sha256(data).hexdigest()
    return [h[i:i + 16] for i in range(0, 64, 16)]


def file_id(path):
    with open(path, "rb") as f:
        d = f.read()
    return {"bytes": len(d), "sha256_segments": sha_segments(d)}


def fresh_dir(parent, prefix):
    os.makedirs(parent, exist_ok=True)
    d = os.path.realpath(tempfile.mkdtemp(prefix=prefix, dir=parent))
    if os.listdir(d):
        raise RuntimeError("fresh dir not empty: %s" % d)
    return d


def dump(obj):
    return json.dumps(obj, ensure_ascii=False, indent=1, sort_keys=False)


def emit(obj, path):
    text = dump(obj)
    if HEX40.search(text):
        sys.stderr.write("REFUSED: 40-hex sequence in %s\n" % path)
        raise SystemExit(3)
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, "w", encoding="utf-8") as f:
        f.write(text + "\n")
    return path
