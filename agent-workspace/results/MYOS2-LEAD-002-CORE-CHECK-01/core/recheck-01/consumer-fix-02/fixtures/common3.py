# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-02 (consumer-fix-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file (pins and small helpers for the data-layer recheck; no process-group machinery)
# scope: Git reads of pinned objects, fresh directories, bounded Python subprocesses (30 s default),
#   JSON output that refuses 40-hex runs. Nothing here compiles or runs C fixtures.
# --------------------------------------------------------------------------------------------------
"""Shared pins and helpers for consumer-fix-02."""
import hashlib
import json
import os
import re
import subprocess
import sys
import tempfile
import time

REPO = os.environ.get("MYOS2_REPO", "/home/user/MyOS2")
PINS = {
    "time": "a039d9803ade",            # kernel source baseline (unchanged)
    "master": "de3bb1df906a",          # workspace rules
    "taskbook": "57a7c3e0eebf",        # six original technical inputs
    "lead_recheck01": "ec62453e76d2",  # 12 contract + core-review (previous round)
    "lead_recheck02": "b0aa54db1b70",  # 13 contract + recheck-01 review, fixed at the start of this round
    "core_frozen": "a1e7c2277705",     # reviewed core head
    "core_results": "7e2fa84a9823",    # old core results.yaml
    "batch1": "f9ae2bce9a64",          # recheck-01 batch 1
    "frozen_results": "efb9846b88ec",  # recheck-01 results.yaml frozen here
    "reviewed_input": "b843d475367a",  # recheck-01 head reviewed by CORE-CHECK-01-RECHECK-01-REVIEW-001
}
WORK_BRANCH = "claude/dazzling-cori-q0dnyt"
PACKET = "MYOS2-LEAD-002-CORE-CHECK-01"
FOLLOWUP = "CORE-CHECK-01-RECHECK-02"
RESULTS_ROOT = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/"
R01 = RESULTS_ROOT + "core/recheck-01/"
PREFIX = R01 + "consumer-fix-02/"
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


def pin(name):
    return full(PINS[name])


def short12(rev):
    return git("rev-parse", "--short=12", full(rev)).stdout.decode().strip()


def blob(rev, path):
    return git("show", "%s:%s" % (full(rev), path)).stdout


def tree(rev, prefix=""):
    """{path: blob id} under prefix at rev (blob ids stay internal)."""
    out = {}
    args = ["ls-tree", "-r", full(rev)] + ([prefix] if prefix else [])
    for ln in git(*args).stdout.decode().split("\n"):
        if "\t" in ln:
            meta, path = ln.split("\t", 1)
            out[path] = meta.split()[2]
    return out


def is_ancestor(a, b):
    return git("merge-base", "--is-ancestor", full(a), full(b), check=False).returncode == 0


def sha_segments(data):
    h = hashlib.sha256(data).hexdigest()
    return [h[i:i + 16] for i in range(0, 64, 16)]


def fresh_dir(parent, prefix):
    """A brand-new empty directory under parent; existing directories are never reused or cleaned."""
    os.makedirs(parent, exist_ok=True)
    d = os.path.realpath(tempfile.mkdtemp(prefix=prefix, dir=parent))
    if os.listdir(d):
        raise RuntimeError("fresh dir not empty: %s" % d)
    return d


def run_py(args, cwd, timeout=30, env_extra=None):
    """One bounded Python experiment in its own process (plain subprocess timeout, no group handling)."""
    env = dict(os.environ, PYTHONDONTWRITEBYTECODE="1", MYOS2_REPO=REPO, **(env_extra or {}))
    t0 = time.monotonic()
    try:
        p = subprocess.run([PY, *args], cwd=cwd, env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=timeout)
        return {"returncode": p.returncode, "timed_out": False, "stdout": p.stdout.decode(errors="replace"),
                "stderr": p.stderr.decode(errors="replace"), "elapsed_s": round(time.monotonic() - t0, 3)}
    except subprocess.TimeoutExpired as e:
        return {"returncode": None, "timed_out": True, "stdout": (e.stdout or b"").decode(errors="replace"),
                "stderr": (e.stderr or b"").decode(errors="replace"), "elapsed_s": round(time.monotonic() - t0, 3)}


def last_json(stdout):
    """Parse the last stdout line as JSON; None when absent or unparsable."""
    lines = [x for x in (stdout or "").strip().splitlines() if x.strip()]
    try:
        return json.loads(lines[-1]) if lines else None
    except ValueError:
        return None


def tail(text, n=3):
    return [x for x in (text or "").strip().splitlines()][-n:]


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
