---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
phase: pilot
record_type: cloud_pilot_evidence
transport_marker: MYOS2-CLOUD-PILOT-20260925-K7P4
produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"
execution_model_selection: unknown_or_not_attestable
execution_model_selection_source: "平台规则禁止在推送到仓库的产物中写模型标识；主线如需可向 Owner 核对会话界面所选项"
date: "2026-09-27"
base_snapshot: "workspace=master（分支名）；taskbook=agent/MYOS2-LEAD-002（分支名）；kernel=time（分支名）；短标识见 §1、§2"
inputs_read:
  - "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/10-cloud-pilot-and-github-handoff.md"
  - "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/MANIFEST.md"
  - "master:agent-workspace/conventions.md"
  - "master:agent-workspace/tasks/00-gpt-task-protocol-v2.md"
  - "time 与 master:mykernel/scripts/options_flags.cmake、mykernel/debug/panic.c"
status: final_for_pilot
redaction: "未发现需脱敏内容；push 输出经 40 位十六进制替换过滤（实际无命中）。每次 Bash 调用后平台附加的一行 'Shell cwd was reset to /home/user/MyOS2' 是会话外壳提示，不是命令输出，下文省略。"
transcription: "§2-§6 的脚本全文与 stdout 由 build_evidence.py 从磁盘文件原样嵌入；§1 的前置命令输出为手工转录，已如实标注。"
open_questions: []
---

# CORE-CHECK-01 pilot 证据：脚本全文、命令、输出与回读

**P00–P03 全部按预期完成；所有运行只发生在本次云端会话容器的普通用户态进程中，不是 MyOS2 内核运行证明。**

工作目录 `W=/tmp/claude-0/-home-user-MyOS2/e3ce16b3-588b-596b-86aa-5e369173435c/scratchpad/pilot-work`（本会话 scratchpad 下新建，执行者未删除；容器回收后不保留）。下文命令中的 `$W` 即此字面路径。所有脚本只读 Git 对象，不切换工作分支、不合并 time 或任务包分支。

## 1. 前置：读取身份与冲突检查（手工转录）

### 1.1 取远端对象

```text
$ git fetch origin agent/MYOS2-LEAD-002 time master 2>&1 | tail -5
 * branch            agent/MYOS2-LEAD-002 -> FETCH_HEAD
 * branch            time                 -> FETCH_HEAD
 * branch            master               -> FETCH_HEAD
 * [new branch]      agent/MYOS2-LEAD-002 -> origin/agent/MYOS2-LEAD-002
 * [new branch]      time                 -> origin/time
```

会话起始时 `git status` 显示当前分支 `claude/dazzling-cori-q0dnyt`、工作树干净；该分支起点与 origin/master 相同（见 §2 P00 的 `work_head_equals_origin_master`）。

### 1.2 10 号任务文件：raw URL 与 Git 对象逐字节比较

```text
$ curl -sS -o $S/10.md -w 'http=%{http_code} bytes=%{size_download}\n' https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/10-cloud-pilot-and-github-handoff.md; echo "curl_exit=$?"
$ git show origin/agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/10-cloud-pilot-and-github-handoff.md > $S/10.git.md; cmp $S/10.md $S/10.git.md && echo IDENTICAL; wc -c $S/10.md $S/10.git.md
http=200 bytes=16036
curl_exit=0
IDENTICAL
16036 .../scratchpad/raw/10.md
16036 .../scratchpad/raw/10.git.md
32072 total
```

（`$S` 为 scratchpad 下新建的 `raw/` 目录；`wc` 输出中的长路径以 `...` 缩写。）全文已完整读取。`11-core-verification-cloud.md` 未读取、未执行。

### 1.3 open PR 与相交路径

- GitHub MCP `list_pull_requests(state=open)`：仅 1 个 open PR，[08822407d/MyOS2#16](https://github.com/08822407d/MyOS2/pull/16)，head=`agent/MYOS2-LEAD-002`，base=`master`，draft=false。它是主线文档交付，本执行者未向其写入。
- 该 PR 的 9 个文件全部位于 `agent-workspace/lead/MYOS2-LEAD-002/`（MCP `get_files` 与本地 `git diff --name-only origin/master...origin/agent/MYOS2-LEAD-002` 均为 9 个）。
- `git diff --name-only origin/master...origin/agent/MYOS2-LEAD-002 | grep '^agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01'` → 无输出（`no intersection`）；`git ls-tree -r --name-only origin/master agent-workspace/results/ | grep CORE-CHECK` → 无输出。故不存在本核验包的既有执行 PR/目录，新建本会话分支结果不与之竞争。

### 1.4 自动执行配置只读检查

- `git ls-tree -r --name-only origin/master | grep -E '^(\.github/|\.claude/|CLAUDE\.md|\.gitmodules|\.devcontainer)'` → 无命中。
- master 有 `.vscode/`（c_cpp_properties.json、generate_compdb.py、launch.json、settings.json、tasks.json）；`git show origin/master:.vscode/tasks.json | grep -i -E 'runOn|folderOpen'` → 无命中。本会话不使用 VS Code，未执行其中任何任务。
- 工作树无 `.claude/`。平台在提示词前完成的容器初始化与克隆未由执行者审计。

### 1.5 已读说明文件身份（脚本输出，原样嵌入）

共享辅助模块 `common.py`（后续所有脚本共用）：

```python
"""Shared helpers for MYOS2-LEAD-002-CORE-CHECK-01 pilot scripts (P00-P03)."""
import hashlib
import json
import os
import re
import subprocess
import sys
import time

REPO = "/home/user/MyOS2"
HEX40 = re.compile(rb"[0-9a-fA-F]{40}")


def git(*args, check=True):
    """Run a read-only git command in the repo, return stdout bytes."""
    p = subprocess.run(["git", "-C", REPO, *args], stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE, timeout=60)
    if check and p.returncode != 0:
        raise RuntimeError("git %s failed rc=%d: %s" % (
            " ".join(args), p.returncode, p.stderr.decode(errors="replace")[:300]))
    return p.stdout


def short12(ref):
    return git("rev-parse", "--short=12", ref).decode().strip()


def sha256_segments(data):
    """SHA-256 as 4 x 16 hex segments; digest = segments joined in order, no separator."""
    h = hashlib.sha256(data).hexdigest()
    return [h[i:i + 16] for i in range(0, 64, 16)]


def run_limited(cmd, timeout, cwd=None):
    """Run cmd with a hard timeout; on timeout kill and reap the child."""
    t0 = time.monotonic()
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=cwd)
    try:
        out, err = p.communicate(timeout=timeout)
        timed_out = False
    except subprocess.TimeoutExpired:
        timed_out = True
        p.kill()
        out, err = p.communicate()
    return {
        "cmd": cmd,
        "timeout_s": timeout,
        "timed_out": timed_out,
        "returncode": p.returncode,
        "stdout": out.decode(errors="replace"),
        "stderr": err.decode(errors="replace"),
        "elapsed_s": round(time.monotonic() - t0, 3),
        "child_reaped": p.returncode is not None,
        "proc_entry_exists_after": os.path.exists("/proc/%d" % p.pid),
    }


def emit(result, path):
    """Write JSON result; refuse to emit any 40-hex run (exit 3)."""
    text = json.dumps(result, ensure_ascii=False, indent=2)
    if HEX40.search(text.encode()):
        sys.stderr.write("REFUSED: 40-hex sequence in output\n")
        sys.exit(3)
    with open(path, "w", encoding="utf-8") as f:
        f.write(text + "\n")
    print(text)
```

`pre_inputs.py`：

```python
"""Pre-flight identity of the instruction/convention files that were read (bytes + SHA-256 segments),
and extraction of conventions.md section-1 item 2 for startup_selfcheck_quote."""
import sys

from common import git, short12, sha256_segments, emit

RAW_COPY = sys.argv[1]  # file saved earlier by curl from raw.githubusercontent.com
items = [
    ("origin/agent/MYOS2-LEAD-002", "agent-workspace/lead/MYOS2-LEAD-002/10-cloud-pilot-and-github-handoff.md"),
    ("origin/master", "agent-workspace/conventions.md"),
    ("origin/master", "agent-workspace/tasks/00-gpt-task-protocol-v2.md"),
]
res = {"check": "PRE", "files": []}
for ref, path in items:
    data = git("show", "%s:%s" % (ref, path))
    res["files"].append({"ref": ref, "short12": short12(ref), "path": path, "bytes": len(data),
                         "sha256_segments": sha256_segments(data)})
raw = open(RAW_COPY, "rb").read()
obj = git("show", "%s:%s" % items[0])
res["raw_url_copy_of_10"] = {"bytes": len(raw), "byte_identical_to_git_object": raw == obj}
conv = git("show", "origin/master:agent-workspace/conventions.md").decode("utf-8").split("\n")
res["conventions_s1_item2_line"] = next(l for l in conv if l.startswith("2. **唯一可写区**"))
emit(res, "pre.json")
```

命令与结果：

```text
$ cd "$W" && python3 pre_inputs.py ../raw/10.md > pre.stdout 2> pre.stderr; echo "pre exit=$?" | tee pre.exit
pre exit=0
stderr: 0 bytes
--- pre.stdout ---
{
  "check": "PRE",
  "files": [
    {
      "ref": "origin/agent/MYOS2-LEAD-002",
      "short12": "57a7c3e0eebf",
      "path": "agent-workspace/lead/MYOS2-LEAD-002/10-cloud-pilot-and-github-handoff.md",
      "bytes": 16036,
      "sha256_segments": [
        "4160a37fcfd8936d",
        "dd60ae08ed371c03",
        "60ae06a39b83343f",
        "b535cfc009ba9657"
      ]
    },
    {
      "ref": "origin/master",
      "short12": "de3bb1df906a",
      "path": "agent-workspace/conventions.md",
      "bytes": 6041,
      "sha256_segments": [
        "78cf52c177ed1e0d",
        "ed53524b872d4095",
        "a0103929db0ad501",
        "91306e0135b66698"
      ]
    },
    {
      "ref": "origin/master",
      "short12": "de3bb1df906a",
      "path": "agent-workspace/tasks/00-gpt-task-protocol-v2.md",
      "bytes": 10239,
      "sha256_segments": [
        "3a38f239f4a8cdcc",
        "a865d358b10b27eb",
        "b9df62e6c32ccf92",
        "f2db66897c9ee8bb"
      ]
    }
  ],
  "raw_url_copy_of_10": {
    "bytes": 16036,
    "byte_identical_to_git_object": true
  },
  "conventions_s1_item2_line": "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
}
```

## 2. P00｜环境与输入身份

只调用 `--version` 类安全查询；不枚举环境变量或凭据，不安装工具。`ls-remote` 输出只在脚本内与本地远端跟踪引用比较，仅输出布尔值与 12 位短标识。

```python
"""P00: environment and input identity (safe version queries only; no env/credential dump)."""
import datetime
import platform
import shutil
import sys

from common import REPO, git, short12, run_limited, emit

REFS = {
    "workspace_base": "origin/master",
    "kernel_source": "origin/time",
    "taskbook": "origin/agent/MYOS2-LEAD-002",
}
INPUT_FILES = [
    ("origin/time", "mykernel/scripts/options_flags.cmake"),
    ("origin/time", "mykernel/debug/panic.c"),
    ("origin/master", "mykernel/scripts/options_flags.cmake"),
    ("origin/master", "mykernel/debug/panic.c"),
    ("origin/agent/MYOS2-LEAD-002", "agent-workspace/lead/MYOS2-LEAD-002/MANIFEST.md"),
]


def first_line(cmd):
    exe = shutil.which(cmd[0])
    if exe is None:
        return {"present": False}
    r = run_limited(cmd, timeout=10)
    line = (r["stdout"] or r["stderr"]).splitlines()
    return {"present": True, "path": exe, "returncode": r["returncode"],
            "version_line": line[0] if line else ""}


def os_pretty():
    try:
        for ln in open("/etc/os-release", encoding="utf-8"):
            if ln.startswith("PRETTY_NAME="):
                return ln.split("=", 1)[1].strip().strip('"')
    except OSError:
        pass
    return "unknown"


res = {"check": "P00", "read_time_utc": datetime.datetime.now(datetime.timezone.utc)
       .strftime("%Y-%m-%dT%H:%M:%SZ")}
res["environment"] = {
    "os_pretty_name": os_pretty(),
    "kernel_release": platform.release(),
    "cpu_arch": platform.machine(),
    "python3": sys.version.split()[0],
    "gcc": first_line(["gcc", "--version"]),
    "clang": first_line(["clang", "--version"]),
    "git": first_line(["git", "--version"]),
}
try:
    import yaml
    res["environment"]["yaml_parser"] = {"module": "PyYAML", "version": yaml.__version__,
                                         "cyaml_loader": hasattr(yaml, "CSafeLoader")}
except ImportError:
    res["environment"]["yaml_parser"] = {"module": None}

# Work branch and base.
res["work_branch"] = git("rev-parse", "--abbrev-ref", "HEAD").decode().strip()
res["work_head_short12"] = short12("HEAD")
res["work_head_equals_origin_master"] = (
    git("rev-parse", "HEAD") == git("rev-parse", "origin/master"))

# Remote freshness: compare local remote-tracking refs with ls-remote (only booleans/short ids emitted).
ls = git("ls-remote", "origin", "refs/heads/master", "refs/heads/time",
         "refs/heads/agent/MYOS2-LEAD-002").decode().split("\n")
remote = {}
for ln in ls:
    if "\t" in ln:
        oid, name = ln.split("\t", 1)
        remote[name.replace("refs/heads/", "origin/")] = oid
res["inputs"] = {}
for role, ref in REFS.items():
    local = git("rev-parse", ref).decode().strip()
    res["inputs"][role] = {"ref": ref, "short12": short12(ref),
                           "matches_ls_remote": remote.get(ref) == local}

# Materials must be retrievable from the three branches.
res["materials"] = []
for ref, path in INPUT_FILES:
    ok = run_limited(["git", "-C", REPO, "cat-file", "-e", "%s:%s" % (ref, path)],
                     timeout=30)["returncode"] == 0
    size = int(git("cat-file", "-s", "%s:%s" % (ref, path)).decode()) if ok else None
    res["materials"].append({"ref": ref, "path": path, "retrievable": ok, "bytes": size})

res["all_materials_retrievable"] = all(m["retrievable"] for m in res["materials"])
res["all_refs_fresh"] = all(v["matches_ls_remote"] for v in res["inputs"].values())
emit(res, "p00.json")
sys.exit(0 if res["all_materials_retrievable"] and res["all_refs_fresh"] else 1)
```

```text
$ cd "$W" && python3 p00_env.py > p00.stdout 2> p00.stderr; echo "p00 exit=$?" | tee p00.exit
p00 exit=0
stderr: 0 bytes
--- p00.stdout ---
{
  "check": "P00",
  "read_time_utc": "2026-09-27T04:36:43Z",
  "environment": {
    "os_pretty_name": "Ubuntu 24.04.4 LTS",
    "kernel_release": "6.18.44-fc-v37",
    "cpu_arch": "x86_64",
    "python3": "3.11.15",
    "gcc": {
      "present": true,
      "path": "/usr/bin/gcc",
      "returncode": 0,
      "version_line": "gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0"
    },
    "clang": {
      "present": true,
      "path": "/usr/bin/clang",
      "returncode": 0,
      "version_line": "Ubuntu clang version 18.1.3 (1ubuntu1)"
    },
    "git": {
      "present": true,
      "path": "/usr/bin/git",
      "returncode": 0,
      "version_line": "git version 2.43.0"
    },
    "yaml_parser": {
      "module": "PyYAML",
      "version": "6.0.1",
      "cyaml_loader": false
    }
  },
  "work_branch": "claude/dazzling-cori-q0dnyt",
  "work_head_short12": "de3bb1df906a",
  "work_head_equals_origin_master": true,
  "inputs": {
    "workspace_base": {
      "ref": "origin/master",
      "short12": "de3bb1df906a",
      "matches_ls_remote": true
    },
    "kernel_source": {
      "ref": "origin/time",
      "short12": "a039d9803ade",
      "matches_ls_remote": true
    },
    "taskbook": {
      "ref": "origin/agent/MYOS2-LEAD-002",
      "short12": "57a7c3e0eebf",
      "matches_ls_remote": true
    }
  },
  "materials": [
    {
      "ref": "origin/time",
      "path": "mykernel/scripts/options_flags.cmake",
      "retrievable": true,
      "bytes": 2658
    },
    {
      "ref": "origin/time",
      "path": "mykernel/debug/panic.c",
      "retrievable": true,
      "bytes": 6472
    },
    {
      "ref": "origin/master",
      "path": "mykernel/scripts/options_flags.cmake",
      "retrievable": true,
      "bytes": 2546
    },
    {
      "ref": "origin/master",
      "path": "mykernel/debug/panic.c",
      "retrievable": true,
      "bytes": 6476
    },
    {
      "ref": "origin/agent/MYOS2-LEAD-002",
      "path": "agent-workspace/lead/MYOS2-LEAD-002/MANIFEST.md",
      "retrievable": true,
      "bytes": 7093
    }
  ],
  "all_materials_retrievable": true,
  "all_refs_fresh": true
}
```

结论：x86_64、Ubuntu 24.04.4 LTS、Python 3.11.15、gcc 13.3.0、clang 18.1.3、git 2.43.0、PyYAML 6.0.1 均实际存在；三分支材料可取且与远端 `ls-remote` 一致。

## 3. P01｜金丝雀引文与负控

引文从任务包 MANIFEST 的 front matter 用 PyYAML 解析取得，不由执行者重打。比较按字节：整行相等计数（按 `\n` 切行）与原始子串计数；制表符保持原样，另列“制表符换空格”变体以示空格替代不会命中。负控由 time 引文把 `-mcmodel=kernel` 改为 `-mcmodel=kernal`，先断言其与原文不同且不在 time 源文中，再用同一匹配函数检查。

```python
"""P01: branch canary quotes (time vs master) + one deliberately tampered negative control.

Quotes are taken from the taskbook MANIFEST front matter (parsed with PyYAML), not retyped.
Matching is byte-exact: whole-line equality (lines split on b"\\n") and raw substring count.
"""
import sys

import yaml

from common import git, short12, sha256_segments, emit

MANIFEST_REF = "origin/agent/MYOS2-LEAD-002"
MANIFEST_PATH = "agent-workspace/lead/MYOS2-LEAD-002/MANIFEST.md"
SRC = {"options_flags_cmake": "mykernel/scripts/options_flags.cmake",
       "panic_c_panic": "mykernel/debug/panic.c"}
BRANCHES = ["origin/time", "origin/master"]


def front_matter(data):
    text = data.decode("utf-8")
    assert text.startswith("---\n"), "MANIFEST has no YAML front matter"
    end = text.index("\n---\n", 4)
    return yaml.safe_load(text[4:end])


def match(data, quote):
    """Return (whole-line equal count, raw substring count) for bytes quote in bytes data."""
    lines = data.split(b"\n")
    return sum(1 for ln in lines if ln == quote), data.count(quote)


manifest_bytes = git("show", "%s:%s" % (MANIFEST_REF, MANIFEST_PATH))
fm = front_matter(manifest_bytes)
bcq = fm["branch_canary_quotes"]

res = {"check": "P01", "manifest": {"ref": MANIFEST_REF, "short12": short12(MANIFEST_REF),
                                    "path": MANIFEST_PATH, "bytes": len(manifest_bytes),
                                    "sha256_segments": sha256_segments(manifest_bytes)}}

# Source files as read from git objects of each branch.
files = {}
res["sources"] = []
for br in BRANCHES:
    for key, path in SRC.items():
        data = git("show", "%s:%s" % (br, path))
        files[(br, key)] = data
        res["sources"].append({"ref": br, "short12": short12(br), "path": path,
                               "bytes": len(data), "sha256_segments": sha256_segments(data)})

res["canaries"] = []
all_ok = True
for key in SRC:
    q = bcq["time"][key].encode("utf-8")
    t_line, t_sub = match(files[("origin/time", key)], q)
    m_line, m_sub = match(files[("origin/master", key)], q)
    spaced = q.replace(b"\t", b" ")
    sp_line, sp_sub = match(files[("origin/time", key)], spaced)
    mq = bcq["master_observed_comparison"][key].encode("utf-8")
    mq_m_line, _ = match(files[("origin/master", key)], mq)
    mq_t_line, mq_t_sub = match(files[("origin/time", key)], mq)
    if t_line >= 1 and m_sub == 0:
        verdict = "DISCRIMINATES"
    elif t_line >= 1 and m_sub > 0:
        verdict = "CANARY_NO_LONGER_DISCRIMINATES"
    else:
        verdict = "TIME_QUOTE_NOT_FOUND"
    all_ok &= verdict == "DISCRIMINATES"
    res["canaries"].append({
        "key": key, "path": SRC[key], "quote_repr": repr(q.decode()),
        "quote_contains_real_tab": b"\t" in q,
        "time": {"whole_line_matches": t_line, "substring_matches": t_sub},
        "master": {"whole_line_matches": m_line, "substring_matches": m_sub},
        "tab_to_space_variant_in_time": {"whole_line_matches": sp_line, "substring_matches": sp_sub},
        "master_observed_comparison": {"quote_repr": repr(mq.decode()),
                                       "master_whole_line_matches": mq_m_line,
                                       "time_whole_line_matches": mq_t_line,
                                       "time_substring_matches": mq_t_sub},
        "verdict": verdict,
    })

# Auxiliary independent derivation (not the gate): candidate lines by the protocol's descriptions.
aux = {}
for br in BRANCHES:
    opt = files[(br, "options_flags_cmake")].split(b"\n")
    pan = files[(br, "panic_c_panic")].split(b"\n")
    aux[br] = {
        "options_lines_with_mcmodel_and_fno_pie": [repr(l.decode()) for l in opt
                                                   if b"-mcmodel=" in l and b"-fno-pie" in l],
        "options_lines_with_mcmodel": [repr(l.decode()) for l in opt if b"-mcmodel=" in l],
        "panic_lines_starting_this_cpu": [repr(l.decode()) for l in pan
                                          if l.lstrip().startswith(b"this_cpu = ")],
    }
res["auxiliary_line_scan"] = aux

# Negative control: tamper the time options quote; must differ and be absent before the check.
orig = bcq["time"]["options_flags_cmake"].encode("utf-8")
tampered = orig.replace(b"-mcmodel=kernel", b"-mcmodel=kernal")
pre_differs = tampered != orig
pre_absent = files[("origin/time", "options_flags_cmake")].count(tampered) == 0
n_line, n_sub = match(files[("origin/time", "options_flags_cmake")], tampered)
neg_ok = pre_differs and pre_absent and n_line == 0 and n_sub == 0
all_ok &= neg_ok
res["negative_control"] = {
    "derived_from": "time.options_flags_cmake quote, '-mcmodel=kernel' -> '-mcmodel=kernal'",
    "tampered_repr": repr(tampered.decode()),
    "precondition_differs_from_original": pre_differs,
    "precondition_absent_from_time_file": pre_absent,
    "time": {"whole_line_matches": n_line, "substring_matches": n_sub},
    "expected": "not hit", "verdict": "NOT_HIT_AS_EXPECTED" if neg_ok else "UNEXPECTED",
}
res["all_as_expected"] = bool(all_ok)
emit(res, "p01.json")
sys.exit(0 if all_ok else 1)
```

```text
$ cd "$W" && python3 p01_canary.py > p01.stdout 2> p01.stderr; echo "p01 exit=$?" | tee p01.exit
p01 exit=0
stderr: 0 bytes
--- p01.stdout ---
{
  "check": "P01",
  "manifest": {
    "ref": "origin/agent/MYOS2-LEAD-002",
    "short12": "57a7c3e0eebf",
    "path": "agent-workspace/lead/MYOS2-LEAD-002/MANIFEST.md",
    "bytes": 7093,
    "sha256_segments": [
      "1d591d6b4b2ee7dd",
      "adbe161e66f22345",
      "25dd693c8d14b274",
      "382474374a0c9916"
    ]
  },
  "sources": [
    {
      "ref": "origin/time",
      "short12": "a039d9803ade",
      "path": "mykernel/scripts/options_flags.cmake",
      "bytes": 2658,
      "sha256_segments": [
        "67d64e8ae02fcdd6",
        "fcd57c0bec27177e",
        "82ff30e871429bf3",
        "05c6b07fe2252e28"
      ]
    },
    {
      "ref": "origin/time",
      "short12": "a039d9803ade",
      "path": "mykernel/debug/panic.c",
      "bytes": 6472,
      "sha256_segments": [
        "08104a5fa836babd",
        "84006dc74bdaf492",
        "90ac1779c46c9f74",
        "1d9d365c00feefc0"
      ]
    },
    {
      "ref": "origin/master",
      "short12": "de3bb1df906a",
      "path": "mykernel/scripts/options_flags.cmake",
      "bytes": 2546,
      "sha256_segments": [
        "fb42dce04a4a56a5",
        "4c51e3512faa83ab",
        "a749f9e48dcb3ead",
        "5c850f3b154a1818"
      ]
    },
    {
      "ref": "origin/master",
      "short12": "de3bb1df906a",
      "path": "mykernel/debug/panic.c",
      "bytes": 6476,
      "sha256_segments": [
        "45f2862ffa295316",
        "ba71a20cbc54c21a",
        "f124494c9526076e",
        "5f8af303ad70da57"
      ]
    }
  ],
  "canaries": [
    {
      "key": "options_flags_cmake",
      "path": "mykernel/scripts/options_flags.cmake",
      "quote_repr": "'\\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\\\'",
      "quote_contains_real_tab": true,
      "time": {
        "whole_line_matches": 1,
        "substring_matches": 1
      },
      "master": {
        "whole_line_matches": 0,
        "substring_matches": 0
      },
      "tab_to_space_variant_in_time": {
        "whole_line_matches": 0,
        "substring_matches": 0
      },
      "master_observed_comparison": {
        "quote_repr": "'\\t-m64 -mcmodel=large -fPIE \\\\'",
        "master_whole_line_matches": 1,
        "time_whole_line_matches": 0,
        "time_substring_matches": 0
      },
      "verdict": "DISCRIMINATES"
    },
    {
      "key": "panic_c_panic",
      "path": "mykernel/debug/panic.c",
      "quote_repr": "'\\tthis_cpu = smp_processor_id();'",
      "quote_contains_real_tab": true,
      "time": {
        "whole_line_matches": 1,
        "substring_matches": 1
      },
      "master": {
        "whole_line_matches": 0,
        "substring_matches": 0
      },
      "tab_to_space_variant_in_time": {
        "whole_line_matches": 0,
        "substring_matches": 0
      },
      "master_observed_comparison": {
        "quote_repr": "'\\tthis_cpu = raw_smp_processor_id();'",
        "master_whole_line_matches": 1,
        "time_whole_line_matches": 0,
        "time_substring_matches": 0
      },
      "verdict": "DISCRIMINATES"
    }
  ],
  "auxiliary_line_scan": {
    "origin/time": {
      "options_lines_with_mcmodel_and_fno_pie": [
        "'\\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\\\'"
      ],
      "options_lines_with_mcmodel": [
        "'\\t-m64 -mcmodel=kernel -fverbose-asm \\\\'",
        "'\\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\\\'"
      ],
      "panic_lines_starting_this_cpu": [
        "'\\tthis_cpu = smp_processor_id();'"
      ]
    },
    "origin/master": {
      "options_lines_with_mcmodel_and_fno_pie": [],
      "options_lines_with_mcmodel": [
        "'\\t-m64 -mcmodel=large -fverbose-asm \\\\'",
        "'\\t-m64 -mcmodel=large -fPIE \\\\'"
      ],
      "panic_lines_starting_this_cpu": [
        "'\\tthis_cpu = raw_smp_processor_id();'"
      ]
    }
  },
  "negative_control": {
    "derived_from": "time.options_flags_cmake quote, '-mcmodel=kernel' -> '-mcmodel=kernal'",
    "tampered_repr": "'\\t-m64 -mcmodel=kernal -fno-pie -fno-pic \\\\'",
    "precondition_differs_from_original": true,
    "precondition_absent_from_time_file": true,
    "time": {
      "whole_line_matches": 0,
      "substring_matches": 0
    },
    "expected": "not hit",
    "verdict": "NOT_HIT_AS_EXPECTED"
  },
  "all_as_expected": true
}
```

结论：两条 time 引文各在 time 文件整行命中 1 次、在 master 文件子串命中 0 次，判定 `DISCRIMINATES`；未出现 `CANARY_NO_LONGER_DISCRIMINATES`。负控 `NOT_HIT_AS_EXPECTED`。辅助扫描显示 master 对应行为 `-mcmodel=large -fPIE` 与 `raw_smp_processor_id()`，与任务包 `master_observed_comparison` 一致。辅助扫描未按函数边界限定 `panic` 定义体，只是全文件行扫描。

## 4. P02｜ENVIRONMENT_SMOKE：真实编译、运行与超时

此项不是 MyOS2 原函数验证。小程序：

```c
/* P02 ENVIRONMENT_SMOKE: not MyOS2 code. Adds two volatile ints (2 + 3). */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	volatile int a = 2;
	volatile int b = 3;
	int actual = a + b;
	long expected;

	if (argc != 2) {
		fprintf(stderr, "usage: %s EXPECTED\n", argv[0]);
		return 2;
	}
	expected = strtol(argv[1], NULL, 10);
	printf("actual=%d expected=%ld\n", actual, expected);
	return (actual == expected) ? 0 : 7;
}
```

驱动（编译上限 30 s，每次运行上限 5 s；超时探针为 Python `sleep(2)`、限 0.2 s；驱动逐项核对 stdout 与退出码，不吞非零退出码）：

```python
"""P02 driver: compile smoke.c for real, run it twice, and prove timeouts are caught and reaped.

Limits: compile 30 s, each smoke run 5 s; timeout probe = python sleep(2) under a 0.2 s limit.
Exit 0 only if every observed output/exit code matches its expectation, else 1.
"""
import os
import shutil
import sys

from common import run_limited, emit

HERE = os.path.dirname(os.path.abspath(__file__))
CC = shutil.which("gcc") or shutil.which("clang")
res = {"check": "P02", "class": "ENVIRONMENT_SMOKE", "compiler": CC, "steps": []}
ok = True

if CC is None:
    res["status"] = "BLOCKED_NO_COMPILER"
    emit(res, "p02.json")
    sys.exit(1)

binary = os.path.join(HERE, "smoke")
if os.path.exists(binary):
    os.remove(binary)

c = run_limited([CC, "-std=c11", "-O2", "-Wall", "-Wextra", "-Werror",
                 "-o", binary, os.path.join(HERE, "smoke.c")], timeout=30, cwd=HERE)
c["name"] = "compile"
c["expect"] = {"returncode": 0, "timed_out": False, "binary_exists": True}
c["binary_exists"] = os.path.exists(binary)
c["as_expected"] = c["returncode"] == 0 and not c["timed_out"] and c["binary_exists"]
ok &= c["as_expected"]
res["steps"].append(c)

for arg, want_rc in (("5", 0), ("6", 7)):
    r = run_limited([binary, arg], timeout=5, cwd=HERE)
    r["name"] = "run_expect_%s" % arg
    r["expect"] = {"stdout": "actual=5 expected=%s\n" % arg, "returncode": want_rc,
                   "timed_out": False}
    r["as_expected"] = (r["stdout"] == r["expect"]["stdout"] and r["returncode"] == want_rc
                        and not r["timed_out"])
    ok &= r["as_expected"]
    res["steps"].append(r)

t = run_limited([sys.executable, "-c", "import time; time.sleep(2)"], timeout=0.2, cwd=HERE)
t["name"] = "timeout_probe"
t["expect"] = {"timed_out": True, "returncode": -9, "elapsed_s_lt": 2.0,
               "child_reaped": True, "proc_entry_exists_after": False}
t["as_expected"] = (t["timed_out"] and t["returncode"] == -9 and t["elapsed_s"] < 2.0
                    and t["child_reaped"] and not t["proc_entry_exists_after"])
t["label"] = "TIMEOUT_EXPECTED" if t["as_expected"] else "TIMEOUT_NOT_CAUGHT"
ok &= t["as_expected"]
res["steps"].append(t)

res["all_as_expected"] = bool(ok)
res["status"] = "RAN_ALL_AS_EXPECTED" if ok else "MISMATCH"
emit(res, "p02.json")
sys.exit(0 if ok else 1)
```

```text
$ cd "$W" && python3 p02_driver.py > p02.stdout 2> p02.stderr; echo "p02 exit=$?" | tee p02.exit
p02 exit=0
stderr: 0 bytes
--- p02.stdout ---
{
  "check": "P02",
  "class": "ENVIRONMENT_SMOKE",
  "compiler": "/usr/bin/gcc",
  "steps": [
    {
      "cmd": [
        "/usr/bin/gcc",
        "-std=c11",
        "-O2",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-o",
        "/tmp/claude-0/-home-user-MyOS2/e3ce16b3-588b-596b-86aa-5e369173435c/scratchpad/pilot-work/smoke",
        "/tmp/claude-0/-home-user-MyOS2/e3ce16b3-588b-596b-86aa-5e369173435c/scratchpad/pilot-work/smoke.c"
      ],
      "timeout_s": 30,
      "timed_out": false,
      "returncode": 0,
      "stdout": "",
      "stderr": "",
      "elapsed_s": 0.195,
      "child_reaped": true,
      "proc_entry_exists_after": false,
      "name": "compile",
      "expect": {
        "returncode": 0,
        "timed_out": false,
        "binary_exists": true
      },
      "binary_exists": true,
      "as_expected": true
    },
    {
      "cmd": [
        "/tmp/claude-0/-home-user-MyOS2/e3ce16b3-588b-596b-86aa-5e369173435c/scratchpad/pilot-work/smoke",
        "5"
      ],
      "timeout_s": 5,
      "timed_out": false,
      "returncode": 0,
      "stdout": "actual=5 expected=5\n",
      "stderr": "",
      "elapsed_s": 0.001,
      "child_reaped": true,
      "proc_entry_exists_after": false,
      "name": "run_expect_5",
      "expect": {
        "stdout": "actual=5 expected=5\n",
        "returncode": 0,
        "timed_out": false
      },
      "as_expected": true
    },
    {
      "cmd": [
        "/tmp/claude-0/-home-user-MyOS2/e3ce16b3-588b-596b-86aa-5e369173435c/scratchpad/pilot-work/smoke",
        "6"
      ],
      "timeout_s": 5,
      "timed_out": false,
      "returncode": 7,
      "stdout": "actual=5 expected=6\n",
      "stderr": "",
      "elapsed_s": 0.001,
      "child_reaped": true,
      "proc_entry_exists_after": false,
      "name": "run_expect_6",
      "expect": {
        "stdout": "actual=5 expected=6\n",
        "returncode": 7,
        "timed_out": false
      },
      "as_expected": true
    },
    {
      "cmd": [
        "/usr/local/bin/python3",
        "-c",
        "import time; time.sleep(2)"
      ],
      "timeout_s": 0.2,
      "timed_out": true,
      "returncode": -9,
      "stdout": "",
      "stderr": "",
      "elapsed_s": 0.201,
      "child_reaped": true,
      "proc_entry_exists_after": false,
      "name": "timeout_probe",
      "expect": {
        "timed_out": true,
        "returncode": -9,
        "elapsed_s_lt": 2.0,
        "child_reaped": true,
        "proc_entry_exists_after": false
      },
      "as_expected": true,
      "label": "TIMEOUT_EXPECTED"
    }
  ],
  "all_as_expected": true,
  "status": "RAN_ALL_AS_EXPECTED"
}
```

结论：gcc 实际编译成功（0.195 s）；期望 5 → 打印 `actual=5 expected=5`、退出 0；期望 6 → 打印 `actual=5 expected=6`、退出 7；超时探针在 0.201 s 被捕获，子进程被 SIGKILL（返回码 -9）并回收，`/proc/<pid>` 已不存在，记 `TIMEOUT_EXPECTED`。未执行 x86 特权指令、内核代码或并发压力。

## 5. result.yaml 生成、提交与首次推送

`make_result.py` 只从 p00/p01/p02 的 JSON 输出与退出码文件生成 YAML，写前做 40 位十六进制扫描并用 `yaml.safe_load` 回读与原数据相等：

```python
"""Generate pilot/result.yaml from p00/p01/p02 JSON outputs (no hand-copied values)."""
import json
import re
import sys

import yaml

p0, p1, p2 = (json.load(open("p0%d.json" % i, encoding="utf-8")) for i in range(3))
exits = {k: int(open("%s.exit" % k).read().strip().split("exit=")[1]) for k in ("p00", "p01", "p02")}
MARKER = "MYOS2-CLOUD-PILOT-20260925-K7P4"


def step(name):
    return next(s for s in p2["steps"] if s["name"] == name)


doc = {
    # --- metadata ---
    "task_id": "MYOS2-LEAD-002-CORE-CHECK-01",
    "track_id": "MYOS2-LEAD-002",
    "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
    "phase": "pilot",
    "record_type": "cloud_pilot_structured_result",
    "transport_marker": MARKER,
    "produced_by": "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection",
    "execution_model_selection": "unknown_or_not_attestable",
    "execution_model_selection_source": "平台规则禁止在推送到仓库的产物中写模型标识；主线如需可向 Owner 核对会话界面所选项",
    "date": "2026-09-27",
    "base_snapshot": "workspace=master（分支名）；taskbook=agent/MYOS2-LEAD-002（分支名）；kernel=time（分支名）；短标识见 source_refs",
    "inputs_read": [
        "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/10-cloud-pilot-and-github-handoff.md",
        "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/MANIFEST.md",
        "master:agent-workspace/conventions.md",
        "master:agent-workspace/tasks/00-gpt-task-protocol-v2.md",
        "time:mykernel/scripts/options_flags.cmake",
        "time:mykernel/debug/panic.c",
        "master:mykernel/scripts/options_flags.cmake",
        "master:mykernel/debug/panic.c",
    ],
    "status": "frozen_before_P03_readback",
    "p03_note": "本文件在 P03 远端回读之前生成并冻结；回读结论只写在 evidence.md 与 MANIFEST.md，不回填本文件。",
    "open_questions": [],
    # --- body ---
    "source_refs": {
        "read_time_utc": p0["read_time_utc"],
        "read_method": "git fetch 取远端对象后用 git show <ref>:<path> 读取；ls-remote 比对本地远端跟踪引用",
        "work_branch": p0["work_branch"],
        "work_branch_start_short12": p0["work_head_short12"],
        "work_branch_start_equals_master": p0["work_head_equals_origin_master"],
        "refs": {role: {"branch": v["ref"].replace("origin/", ""), "short12": v["short12"],
                        "matches_ls_remote": v["matches_ls_remote"]}
                 for role, v in p0["inputs"].items()},
        "files": [dict(ref=s["ref"].replace("origin/", ""), path=s["path"], bytes=s["bytes"],
                       sha256_segments=s["sha256_segments"]) for s in p1["sources"]]
        + [dict(ref="agent/MYOS2-LEAD-002", path=p1["manifest"]["path"],
                bytes=p1["manifest"]["bytes"], sha256_segments=p1["manifest"]["sha256_segments"])],
        "sha256_segments_rule": "4 段各 16 个小写十六进制字符，按数组顺序无分隔拼接即完整 SHA-256",
    },
    "environment": {
        "os": p0["environment"]["os_pretty_name"],
        "kernel_release": p0["environment"]["kernel_release"],
        "cpu_arch": p0["environment"]["cpu_arch"],
        "python3": p0["environment"]["python3"],
        "gcc": p0["environment"]["gcc"]["version_line"],
        "clang": p0["environment"]["clang"]["version_line"],
        "git": p0["environment"]["git"]["version_line"],
        "yaml_parser": "PyYAML %s" % p0["environment"]["yaml_parser"]["version"],
        "tools_installed_by_executor": False,
    },
    "checks": {
        "P00": {
            "what": "环境与输入身份",
            "script_exit": exits["p00"],
            "all_materials_retrievable": p0["all_materials_retrievable"],
            "all_refs_match_ls_remote": p0["all_refs_fresh"],
            "expected": "三分支材料均可取且与远端一致",
            "as_expected": p0["all_materials_retrievable"] and p0["all_refs_fresh"],
        },
        "P01": {
            "what": "time 两条金丝雀逐字节整行命中且 master 不命中；篡改负控不命中",
            "script_exit": exits["p01"],
            "quote_source": "agent/MYOS2-LEAD-002 MANIFEST.md front matter branch_canary_quotes（PyYAML 解析）",
            "method": "bytes 按 \\n 切行做整行相等计数 + 原始子串计数；真实制表符保留",
            "canaries": [{
                "key": c["key"], "path": c["path"], "quote_repr": c["quote_repr"],
                "quote_contains_real_tab": c["quote_contains_real_tab"],
                "time_whole_line_matches": c["time"]["whole_line_matches"],
                "master_substring_matches": c["master"]["substring_matches"],
                "tab_to_space_variant_time_substring_matches":
                    c["tab_to_space_variant_in_time"]["substring_matches"],
                "master_observed_comparison_master_whole_line_matches":
                    c["master_observed_comparison"]["master_whole_line_matches"],
                "verdict": c["verdict"],
                "expected": "DISCRIMINATES",
                "as_expected": c["verdict"] == "DISCRIMINATES",
            } for c in p1["canaries"]],
            "negative_control": {
                "tampered_repr": p1["negative_control"]["tampered_repr"],
                "precondition_differs_from_original": p1["negative_control"]["precondition_differs_from_original"],
                "precondition_absent_from_time_file": p1["negative_control"]["precondition_absent_from_time_file"],
                "time_whole_line_matches": p1["negative_control"]["time"]["whole_line_matches"],
                "time_substring_matches": p1["negative_control"]["time"]["substring_matches"],
                "verdict": p1["negative_control"]["verdict"],
                "expected": "NOT_HIT_AS_EXPECTED",
                "as_expected": p1["negative_control"]["verdict"] == "NOT_HIT_AS_EXPECTED",
            },
            "as_expected": p1["all_as_expected"],
        },
        "P02": {
            "what": "ENVIRONMENT_SMOKE：真实编译并运行普通 C 小程序；超时捕捉与回收",
            "not": "不是 MyOS2 原函数或内核运行验证",
            "script_exit": exits["p02"],
            "compiler": p2["compiler"],
            "compile": {k: step("compile")[k] for k in ("returncode", "timed_out", "elapsed_s", "binary_exists", "as_expected")},
            "run_expect_5": {"stdout": step("run_expect_5")["stdout"], "returncode": step("run_expect_5")["returncode"],
                             "expected_returncode": 0, "as_expected": step("run_expect_5")["as_expected"]},
            "run_expect_6": {"stdout": step("run_expect_6")["stdout"], "returncode": step("run_expect_6")["returncode"],
                             "expected_returncode": 7, "as_expected": step("run_expect_6")["as_expected"]},
            "timeout_probe": {k: step("timeout_probe")[k] for k in (
                "timeout_s", "timed_out", "returncode", "elapsed_s", "child_reaped",
                "proc_entry_exists_after", "label", "as_expected")},
            "as_expected": p2["all_as_expected"],
        },
        "P03": {
            "state_in_this_file": "not_yet_run",
            "where_reported": "evidence.md §P03 与 MANIFEST.md",
        },
    },
    "evidence_refs": [
        "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/evidence.md",
        "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/MANIFEST.md",
    ],
    "limitations": [
        "所有运行仅限本次云端会话容器内的普通用户态进程；不是 MyOS2 内核运行、启动或硬件行为证明。",
        "P02 只证明普通 C 编译/运行与退出码、超时捕捉可用；未执行 x86 特权指令、内核代码或并发压力。",
        "P01 只比较两条金丝雀行，不代表其余源码或正式 V00-V14 已核。",
        "未运行仓库脚本、完整构建、QEMU；未安装工具；未改内核。",
        "平台在提示词前完成的容器初始化与克隆未由执行者审计。",
    ],
}

text = yaml.safe_dump(doc, sort_keys=False, allow_unicode=True, width=1000)
if re.search(r"[0-9a-fA-F]{40}", text):
    sys.exit("REFUSED: 40-hex in result.yaml")
assert yaml.safe_load(text) == doc
open(sys.argv[1], "w", encoding="utf-8").write(text)
print("wrote %s (%d bytes)" % (sys.argv[1], len(text.encode())))
```

```text
$ cd "$W" && D=/home/user/MyOS2/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot && mkdir -p $D && python3 make_result.py $D/result.yaml; echo "make_result exit=$?"
wrote /home/user/MyOS2/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/result.yaml (6741 bytes)
make_result exit=0
```

提交（手工转录）：只加入 `result.yaml`，提交信息以 `MYOS2-LEAD-002-CORE-CHECK-01: ` 开头。

```text
$ git add agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/result.yaml && git status --short && git commit -q -F - <<'EOF' ... EOF
A  agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/result.yaml
commit exit=0
$ git log --oneline -2 --abbrev=12
10ecb7dd0bdb MYOS2-LEAD-002-CORE-CHECK-01: pilot result.yaml (P00-P02, frozen before P03 readback)
de3bb1df906a Merge pull request #15 from 08822407d/agent/MYOS2-LEAD-002
$ git show --stat HEAD | tail -3
 .../MYOS2-LEAD-002-CORE-CHECK-01/pilot/result.yaml | 187 +++++++++++++++++++++
 1 file changed, 187 insertions(+)
```

推送（stdout/stderr 经 `sed -E 's/[0-9a-f]{40}/<40hex-redacted>/g'` 过滤后保存为 push1.log，实际无替换）：

```text
$ git push -u origin claude/dazzling-cori-q0dnyt 2>&1 | sed -E '...' | tee push1.log; echo "push exit=${PIPESTATUS[0]}"
remote: 
remote: Create a pull request for 'claude/dazzling-cori-q0dnyt' on GitHub by visiting:        
remote:      https://github.com/08822407d/MyOS2/pull/new/claude/dazzling-cori-q0dnyt        
remote: 
To https://github.com/08822407d/MyOS2
 * [new branch]      claude/dazzling-cori-q0dnyt -> claude/dazzling-cori-q0dnyt
branch 'claude/dazzling-cori-q0dnyt' set up to track 'origin/claude/dazzling-cori-q0dnyt'.
push exit=0
```

注：会话开始时本地已有 `remotes/origin/claude/dazzling-cori-q0dnyt` 跟踪引用，但推送输出为 `[new branch]`，即远端此前并无该分支。只推送了本会话分支。

## 6. P03｜GitHub 独立回读

从远端读取，不读工作树：通道 A 为 `raw.githubusercontent.com`（分支名路径），通道 B 为 `api.github.com` contents 端点（`Accept: application/vnd.github.raw`，`ref=` 分支名）。两者都与 `git show 10ecb7dd0bdb:<path>` 的已提交 blob 逐字节比较，并用 PyYAML 解析回读内容取 `transport_marker`；另以 `ls-remote` 确认远端分支头即该提交。

```python
"""P03: read pilot/result.yaml back from GitHub (not the local work tree) and byte-compare
with the committed blob. Channels: raw.githubusercontent.com and api.github.com contents (raw media).
Usage: python3 p03_readback.py <commit-ish>
"""
import os
import sys

import yaml

from common import git, short12, sha256_segments, run_limited, emit

OWNER_REPO = "08822407d/MyOS2"
BRANCH = "claude/dazzling-cori-q0dnyt"
PATH = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/result.yaml"
MARKER = "MYOS2-CLOUD-PILOT-20260925-K7P4"
commit = sys.argv[1]

committed = git("show", "%s:%s" % (commit, PATH))
res = {"check": "P03", "branch": BRANCH, "path": PATH,
       "committed": {"commit_short12": short12(commit), "bytes": len(committed),
                     "sha256_segments": sha256_segments(committed)}}

# Remote branch head must equal the commit we compare against.
remote_head = git("ls-remote", "origin", "refs/heads/" + BRANCH).decode().split("\t")[0]
res["remote_branch_head_equals_commit"] = remote_head == git("rev-parse", commit).decode().strip()

channels = {
    "raw.githubusercontent.com": ["curl", "-sS", "-w", "%{http_code}", "-o", "rb_raw.bin",
                                  "https://raw.githubusercontent.com/%s/%s/%s" % (OWNER_REPO, BRANCH, PATH)],
    "api.github.com_contents_raw": ["curl", "-sS", "-w", "%{http_code}", "-o", "rb_api.bin",
                                    "-H", "Accept: application/vnd.github.raw",
                                    "https://api.github.com/repos/%s/contents/%s?ref=%s" % (OWNER_REPO, PATH, BRANCH)],
}
res["channels"] = []
ok = res["remote_branch_head_equals_commit"]
for name, cmd in channels.items():
    out = cmd[cmd.index("-o") + 1]
    if os.path.exists(out):
        os.remove(out)
    r = run_limited(cmd, timeout=30)
    body = open(out, "rb").read() if os.path.exists(out) else b""
    ch = {"channel": name, "url": cmd[-1], "curl_exit": r["returncode"], "http_code": r["stdout"],
          "curl_stderr": r["stderr"], "bytes": len(body), "sha256_segments": sha256_segments(body),
          "byte_identical_to_committed": body == committed}
    try:
        ch["parsed_transport_marker"] = yaml.safe_load(body.decode("utf-8")).get("transport_marker")
    except Exception as e:  # noqa: BLE001 - report parse failure verbatim
        ch["parsed_transport_marker"] = "PARSE_ERROR: %s" % type(e).__name__
    ch["marker_ok"] = ch["parsed_transport_marker"] == MARKER
    ok &= ch["byte_identical_to_committed"] and ch["marker_ok"]
    res["channels"].append(ch)

res["all_as_expected"] = bool(ok)
emit(res, "p03.json")
sys.exit(0 if ok else 1)
```

```text
$ cd "$W" && python3 p03_readback.py 10ecb7dd0bdb > p03.stdout 2> p03.stderr; echo "p03 exit=$?" | tee p03.exit
p03 exit=0
stderr: 0 bytes
--- p03.stdout ---
{
  "check": "P03",
  "branch": "claude/dazzling-cori-q0dnyt",
  "path": "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/result.yaml",
  "committed": {
    "commit_short12": "10ecb7dd0bdb",
    "bytes": 6741,
    "sha256_segments": [
      "28ea9895bb723d58",
      "9d097982311512a4",
      "e3a7f3ff950c53a3",
      "7aa18b85772da2ed"
    ]
  },
  "remote_branch_head_equals_commit": true,
  "channels": [
    {
      "channel": "raw.githubusercontent.com",
      "url": "https://raw.githubusercontent.com/08822407d/MyOS2/claude/dazzling-cori-q0dnyt/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/result.yaml",
      "curl_exit": 0,
      "http_code": "200",
      "curl_stderr": "",
      "bytes": 6741,
      "sha256_segments": [
        "28ea9895bb723d58",
        "9d097982311512a4",
        "e3a7f3ff950c53a3",
        "7aa18b85772da2ed"
      ],
      "byte_identical_to_committed": true,
      "parsed_transport_marker": "MYOS2-CLOUD-PILOT-20260925-K7P4",
      "marker_ok": true
    },
    {
      "channel": "api.github.com_contents_raw",
      "url": "https://api.github.com/repos/08822407d/MyOS2/contents/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/result.yaml?ref=claude/dazzling-cori-q0dnyt",
      "curl_exit": 0,
      "http_code": "200",
      "curl_stderr": "",
      "bytes": 6741,
      "sha256_segments": [
        "28ea9895bb723d58",
        "9d097982311512a4",
        "e3a7f3ff950c53a3",
        "7aa18b85772da2ed"
      ],
      "byte_identical_to_committed": true,
      "parsed_transport_marker": "MYOS2-CLOUD-PILOT-20260925-K7P4",
      "marker_ok": true
    }
  ],
  "all_as_expected": true
}
```

结论：远端分支头 = 提交 `10ecb7dd0bdb`；两个远端通道均 HTTP 200、6741 字节、SHA-256 与已提交版本相同、逐字节一致，识别串 `MYOS2-CLOUD-PILOT-20260925-K7P4` 解析正确。`result.yaml` 自此冻结，本回读结论只写入本文件与 MANIFEST.md。

## 7. 第二次提交前的范围与卫生检查

第一次运行（未嵌入本节输出前）退出 1：evidence.md 因原样嵌入 final_check.py 源码，自身含 1 个源码标签字面量与 4 个禁用短语字面量，导致 `manifest_self_check_verified_claims_equals_tag_count: false`。已把这些字面量改为拼接构造（见脚本注释），并把禁用短语命中也纳入失败条件，然后重跑；下方为修正后的脚本与输出。

在 MANIFEST.md 与 evidence.md 暂存后运行；输出不含 md 文件的大小/摘要，故嵌入本节后重跑结果应不变（重跑结果在 PR 说明中报告）。

```python
"""Pre-commit scope and hygiene check for the pilot directory (run with files staged).
Does not print file sizes/digests of the md files, so re-running after embedding its own
output into evidence.md must yield identical output."""
import re
import sys

import yaml

from common import git, emit

PREFIX = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/"
FILES = ["MANIFEST.md", "evidence.md", "result.yaml"]
# Literals are built by concatenation so that embedding this source in evidence.md
# does not itself produce hits (first run failed on exactly that self-reference).
TAG = "[" + "VERIFIED"
BANNED = ["可直接" + "编译", "可直接" + "运行", "已" + "验证", "已" + "测试"]


def fm(text):
    assert text.startswith("---\n")
    return yaml.safe_load(text[4:text.index("\n---\n", 4)])


res = {"check": "FINAL_SCOPE"}
names = git("diff", "--cached", "--name-status", "origin/master").decode().split("\n")
names = [n for n in names if n]
res["index_vs_origin_master"] = names
res["all_paths_in_pilot_dir"] = all(n.split("\t", 1)[1].startswith(PREFIX) for n in names)
res["all_added"] = all(n.startswith("A\t") for n in names)
res["unstaged_or_untracked_outside"] = [l for l in git("status", "--porcelain").decode().split("\n")
                                        if l and not l[3:].startswith(PREFIX)]
res["files"] = {}
ok = res["all_paths_in_pilot_dir"] and res["all_added"] and not res["unstaged_or_untracked_outside"]
for f in FILES:
    raw = open("/home/user/MyOS2/" + PREFIX + f, "rb").read()
    text = raw.decode("utf-8")
    d = {"hex40_hits": len(re.findall(rb"[0-9a-fA-F]{40}", raw)),
         "verified_tag_count": text.count(TAG),
         "banned_phrase_hits": {b: text.count(b) for b in BANNED if b in text},
         "marker_present": "MYOS2-CLOUD-PILOT-20260925-K7P4" in text}
    meta = yaml.safe_load(text) if f.endswith(".yaml") else fm(text)
    d["yaml_parses"] = isinstance(meta, dict)
    d["first_key"] = next(iter(meta))
    ok &= (d["hex40_hits"] == 0 and d["yaml_parses"] and d["marker_present"]
          and not d["banned_phrase_hits"])
    res["files"][f] = d
man = fm(open("/home/user/MyOS2/" + PREFIX + "MANIFEST.md", encoding="utf-8").read())
conv = git("show", "origin/master:agent-workspace/conventions.md").decode("utf-8")
res["startup_selfcheck_quote_in_conventions"] = man["startup_selfcheck_quote"] in conv
src = git("show", "origin/agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/MANIFEST.md").decode()
res["canary_quotes_equal_taskbook"] = man["branch_canary_quotes"]["time"] == fm(src)["branch_canary_quotes"]["time"]
res["manifest_self_check_verified_claims_equals_tag_count"] = (
    man["self_check"]["verified_claims"] == sum(v["verified_tag_count"] for v in res["files"].values()))
ok &= res["startup_selfcheck_quote_in_conventions"] and res["canary_quotes_equal_taskbook"] \
    and res["manifest_self_check_verified_claims_equals_tag_count"]
res["all_as_expected"] = bool(ok)
emit(res, "final.json")
sys.exit(0 if ok else 1)
```

```text
$ git add agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/ && cd "$W" && python3 final_check.py > final.stdout 2> final.stderr; echo "final exit=$?" | tee final.exit
final exit=0
stderr: 0 bytes
--- final.stdout ---
{
  "check": "FINAL_SCOPE",
  "index_vs_origin_master": [
    "A\tagent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/MANIFEST.md",
    "A\tagent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/evidence.md",
    "A\tagent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/result.yaml"
  ],
  "all_paths_in_pilot_dir": true,
  "all_added": true,
  "unstaged_or_untracked_outside": [],
  "files": {
    "MANIFEST.md": {
      "hex40_hits": 0,
      "verified_tag_count": 0,
      "banned_phrase_hits": {},
      "marker_present": true,
      "yaml_parses": true,
      "first_key": "task_id"
    },
    "evidence.md": {
      "hex40_hits": 0,
      "verified_tag_count": 0,
      "banned_phrase_hits": {},
      "marker_present": true,
      "yaml_parses": true,
      "first_key": "task_id"
    },
    "result.yaml": {
      "hex40_hits": 0,
      "verified_tag_count": 0,
      "banned_phrase_hits": {},
      "marker_present": true,
      "yaml_parses": true,
      "first_key": "task_id"
    }
  },
  "startup_selfcheck_quote_in_conventions": true,
  "canary_quotes_equal_taskbook": true,
  "manifest_self_check_verified_claims_equals_tag_count": true,
  "all_as_expected": true
}
```

## 8. 本文件的组装脚本

```python
"""Assemble evidence.md from evidence.tmpl.md: <<FILE:name>> -> verbatim file content,
<<OUT:name>> -> verbatim saved output (or a pending marker if not produced yet)."""
import os
import re
import sys

tmpl = open("evidence.tmpl.md", encoding="utf-8").read()


def sub(m):
    kind, name = m.group(1), m.group(2)
    if kind == "OUT" and not os.path.exists(name):
        return "(pending: %s not produced yet)" % name
    return open(name, encoding="utf-8").read().rstrip("\n")


out = re.sub(r"<<(FILE|OUT):([A-Za-z0-9_.]+)>>", sub, tmpl)
open(sys.argv[1], "w", encoding="utf-8").write(out)
print("wrote %s" % sys.argv[1])
```

`evidence.tmpl.md` 为本文件模板（占位符替换为上列文件原文），未单独提交。
