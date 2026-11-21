# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file; derived in part from core/fixtures/harness.py @ a1e7c2277705 (frozen, unchanged)
# change vs frozen harness.py: R02/R04 - bounded process-group termination with explicit terminal
#   states (group already gone, collection timeout, reap confirmed only when observed), fresh work
#   directories that refuse existing non-empty paths, pins for frozen objects kept separate from the
#   moving execution-branch head.
# --------------------------------------------------------------------------------------------------
"""Shared helpers for recheck-01. Git objects are always read from pinned commits; full object ids
stay inside the process and are never printed (emit() refuses any 40-hex run)."""
import hashlib
import json
import os
import re
import signal
import subprocess
import sys
import tempfile
import time

REPO = os.environ.get("MYOS2_REPO", "/home/user/MyOS2")
PINS = {
    "time": "a039d9803ade",          # kernel source baseline
    "master": "de3bb1df906a",        # workspace rules
    "taskbook": "57a7c3e0eebf",      # six original technical inputs
    "pilot_review": "f79b3a281616",  # ALLOW_CORE receipt
    "lead_head": "ec62453e76d2",     # core-review + 12 contract, fixed at recheck start
    "core_frozen": "a1e7c2277705",   # reviewed core head: frozen verifier and old core results
    "core_results": "7e2fa84a9823",  # first commit of old core results.yaml
    "pilot_head": "0851af4fc08b",    # reviewed pilot head
    "pilot_result": "10ecb7dd0bdb",  # first commit of pilot result.yaml
}
WORK_BRANCH = "claude/dazzling-cori-q0dnyt"
PACKET = "MYOS2-LEAD-002-CORE-CHECK-01"
RESULTS_ROOT = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/"
RECHECK_PREFIX = RESULTS_ROOT + "core/recheck-01/"
HEX40 = re.compile(rb"[0-9a-fA-F]{40}")
_cache = {}


def git(*args, check=True, repo=None, timeout=60):
    p = subprocess.run(["git", "-C", repo or REPO, *args], stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE, timeout=timeout)
    if check and p.returncode != 0:
        raise RuntimeError("git %s rc=%d %s" % (args[0], p.returncode, p.stderr.decode(errors="replace")[:200]))
    return p


def full(rev, repo=None):
    """Full object id of a revision (kept internal)."""
    key = (repo, rev)
    if key not in _cache:
        _cache[key] = git("rev-parse", "--verify", rev + "^{commit}", repo=repo).stdout.decode().strip()
    return _cache[key]


def pin(name, repo=None):
    return full(PINS[name], repo)


def short12(rev, repo=None):
    return git("rev-parse", "--short=12", full(rev, repo), repo=repo).stdout.decode().strip()


def blob(rev, path, repo=None):
    return git("show", "%s:%s" % (full(rev, repo), path), repo=repo).stdout


def exists(rev, path, repo=None):
    return git("cat-file", "-e", "%s:%s" % (full(rev, repo), path), check=False, repo=repo).returncode == 0


def is_ancestor(a, b, repo=None):
    return git("merge-base", "--is-ancestor", full(a, repo), full(b, repo), check=False, repo=repo).returncode == 0


def sha_segments(data):
    h = hashlib.sha256(data).hexdigest()
    return [h[i:i + 16] for i in range(0, 64, 16)]


def fresh_dir(parent, prefix):
    """Create a brand-new empty directory under parent (never reuse or clean an existing one)."""
    os.makedirs(parent, exist_ok=True)
    d = os.path.realpath(tempfile.mkdtemp(prefix=prefix, dir=parent))
    if os.listdir(d):
        raise RuntimeError("fresh dir not empty: %s" % d)
    return d


def require_empty_or_absent(path):
    """For user-given work dirs: refuse an existing non-empty directory."""
    if os.path.exists(path) and (not os.path.isdir(path) or os.listdir(path)):
        raise RuntimeError("refusing existing non-empty work dir: %s" % path)
    os.makedirs(path, exist_ok=True)
    return os.path.realpath(path)


def _group_members(pgid):
    n = 0
    for e in os.listdir("/proc"):
        if not e.isdigit():
            continue
        try:
            with open("/proc/%s/stat" % e) as f:
                fields = f.read().rsplit(")", 1)[1].split()
            if int(fields[2]) == pgid:
                n += 1
        except (OSError, IndexError, ValueError):
            continue
    return n


def run_pg(cmd, timeout, cwd=None, env=None, collect_timeout=5.0, _killpg=None):
    """Bounded run in a new session. On timeout only the child's own group is signalled.
    terminal_state: exited | timeout_group_killed | timeout_group_already_gone |
                    timeout_kill_error | + '_collect_timeout' suffix when collection also timed out.
    reaped_confirmed is True only when a return code was actually observed."""
    killpg = _killpg or os.killpg
    t0 = time.monotonic()
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.DEVNULL,
                         cwd=cwd, env=env, start_new_session=True)
    pgid = p.pid  # start_new_session => setsid() => the child leads its own group
    res = {"cmd": [str(c) for c in cmd], "timeout_s": timeout, "pid": p.pid, "pgid": pgid,
           "pgid_differs_from_own": pgid != os.getpgrp(), "timed_out": False, "kill": None,
           "collect_timed_out": False, "output_lost": False}
    out = err = b""
    try:
        out, err = p.communicate(timeout=timeout)
        state = "exited"
    except subprocess.TimeoutExpired:
        res["timed_out"] = True
        if pgid == os.getpgrp():
            res["kill"] = "refused_own_group"
            state = "timeout_kill_error"
        else:
            try:
                killpg(pgid, signal.SIGKILL)
                res["kill"] = "group_signalled"
                state = "timeout_group_killed"
            except ProcessLookupError:
                res["kill"] = "group_already_gone"
                state = "timeout_group_already_gone"
            except OSError as e:
                res["kill"] = "kill_error:%s" % type(e).__name__
                state = "timeout_kill_error"
        try:
            out, err = p.communicate(timeout=collect_timeout)
        except subprocess.TimeoutExpired:
            res["collect_timed_out"] = True
            res["output_lost"] = True
            state += "_collect_timeout"
            try:
                p.kill()  # our direct child only
                p.wait(timeout=collect_timeout)
            except (subprocess.TimeoutExpired, OSError):
                pass
    rc = p.poll()
    res.update({
        "returncode": rc,
        "reaped_confirmed": rc is not None,
        "terminal_state": state,
        "stdout": out.decode(errors="replace"),
        "stderr": err.decode(errors="replace"),
        "elapsed_s": round(time.monotonic() - t0, 3),
        "group_members_after": _group_members(pgid) if res["timed_out"] else None,
    })
    return res


def dump(obj):
    return json.dumps(obj, ensure_ascii=False, indent=1, sort_keys=False)


def emit(obj, path, echo=False):
    text = dump(obj)
    if HEX40.search(text.encode()):
        sys.stderr.write("REFUSED: 40-hex sequence in %s\n" % path)
        raise SystemExit(3)
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, "w", encoding="utf-8") as f:
        f.write(text + "\n")
    if echo:
        print(text)
    return path
