"""Shared harness for MYOS2-LEAD-002-CORE-CHECK-01 core phase.

- All inputs are read from pinned Git commits (short ids below), never from moving branch names.
- run_pg(): hardened bounded runner. Each command runs in its own session/process group; on timeout
  the whole group (and only that group) gets SIGKILL, output collection itself is time-bounded.
- emit(): refuses to write any 40-hex run.
Environment: MYOS2_REPO (default /home/user/MyOS2), CORE_WORK (scratch dir for builds/outputs).
"""
import hashlib
import json
import os
import re
import signal
import subprocess
import sys
import time

REPO = os.environ.get("MYOS2_REPO", "/home/user/MyOS2")
WORK = os.environ.get("CORE_WORK", os.path.join(os.getcwd(), "run"))
PINS = {
    "time": "a039d9803ade",       # kernel source (branch time)
    "master": "de3bb1df906a",     # workspace rules (branch master)
    "taskbook": "57a7c3e0eebf",   # original technical inputs 07/09/10/11/MANIFEST
    "review": "f79b3a281616",     # taskbook head carrying the ALLOW_CORE review
    "pilot_head": "0851af4fc08b",  # reviewed pilot head on the execution branch
}
HEX40 = re.compile(rb"[0-9a-fA-F]{40}")
_commit_cache = {}


def _git(*args, check=True):
    p = subprocess.run(["git", "-C", REPO, *args], stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE, timeout=60)
    if check and p.returncode != 0:
        raise RuntimeError("git %s rc=%d %s" % (args[0], p.returncode,
                                                p.stderr.decode(errors="replace")[:200]))
    return p


def commit(pin):
    """Full commit object for a pin name (kept internal, never printed)."""
    if pin not in _commit_cache:
        _commit_cache[pin] = _git("rev-parse", "--verify", PINS[pin] + "^{commit}").stdout.decode().strip()
    return _commit_cache[pin]


def short12(pin):
    return _git("rev-parse", "--short=12", commit(pin)).stdout.decode().strip()


def blob(pin, path):
    """Exact bytes of <path> at pinned commit."""
    return _git("show", "%s:%s" % (commit(pin), path)).stdout


def exists(pin, path):
    return _git("cat-file", "-e", "%s:%s" % (commit(pin), path), check=False).returncode == 0


def sha256_segments(data):
    h = hashlib.sha256(data).hexdigest()
    return [h[i:i + 16] for i in range(0, 64, 16)]


def run_pg(cmd, timeout, cwd=None, collect_timeout=5.0, env=None):
    """Run cmd in a new session (own process group). On timeout SIGKILL that group only."""
    t0 = time.monotonic()
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=cwd,
                         start_new_session=True, env=env)
    pgid = os.getpgid(p.pid)
    res = {"cmd": cmd, "timeout_s": timeout, "pid": p.pid, "pgid": pgid,
           "own_pgid": os.getpgrp(), "timed_out": False, "group_killed": False,
           "collect_timed_out": False}
    try:
        out, err = p.communicate(timeout=timeout)
    except subprocess.TimeoutExpired:
        res["timed_out"] = True
        if pgid != os.getpgrp():  # never signal our own group
            os.killpg(pgid, signal.SIGKILL)
            res["group_killed"] = True
        try:
            out, err = p.communicate(timeout=collect_timeout)
        except subprocess.TimeoutExpired:
            res["collect_timed_out"] = True
            p.kill()
            out, err = b"", b""
    res.update({
        "returncode": p.returncode,
        "stdout": out.decode(errors="replace"),
        "stderr": err.decode(errors="replace"),
        "elapsed_s": round(time.monotonic() - t0, 3),
        "child_reaped": p.returncode is not None,
        "proc_entry_exists_after": os.path.exists("/proc/%d" % p.pid),
    })
    return res


def dump(obj):
    return json.dumps(obj, ensure_ascii=False, indent=2, sort_keys=False)


def emit(obj, path, echo=True):
    text = dump(obj)
    if HEX40.search(text.encode()):
        sys.stderr.write("REFUSED: 40-hex sequence in %s\n" % path)
        sys.exit(3)
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, "w", encoding="utf-8") as f:
        f.write(text + "\n")
    if echo:
        print(text)
