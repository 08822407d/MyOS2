"""Fixture builder: expand //@@ORIG markers with exact original slices from the pinned time commit,
compile with a hard limit, run each case in its own bounded process, parse JSON event lines.

Marker (own line in a template):  //@@ORIG <pin> <path> <kind> <name>
  kind = func | macro | struct | initializer:<MACRO>   (initializer: <MACRO>(<name>) = {...};)
The slice is copied byte-for-byte (lines start..end of the located definition) and wrapped in
#line directives so diagnostics point at the original file/line.

Hardening (review sec.3): stale binaries are deleted before compiling; a failed/timed-out compile
marks every case BLOCKED and nothing is executed; a Python exception in one case is recorded and
the remaining cases still run.
"""
import json
import os
import shutil

import harness as H
import locate as L

CC = shutil.which("gcc")
CFLAGS = ["-std=gnu11", "-O0", "-g", "-fno-strict-aliasing", "-Wall", "-Wno-unused-label",
          "-Wno-unused-variable", "-Wno-unused-function"]
COMPILE_LIMIT_S = 60
RUN_LIMIT_S = 5


def locate_slice(pin, path, kind, name):
    text = H.blob(pin, path).decode("utf-8")
    if kind == "func":
        c = L.c_function(text, name)
    elif kind == "macro":
        c = L.c_macro(text, name)
    elif kind == "struct":
        c = L.c_struct(text, name)
    elif kind.startswith("initializer:"):
        c = L.c_macro_generated_initializer(text, kind.split(":", 1)[1], name)
    else:
        raise ValueError("unknown kind %s" % kind)
    if len(c) != 1:
        raise ValueError("%s %s::%s has %d candidates" % (kind, path, name, len(c)))
    lines = text.split("\n")
    s, e = c[0]["start"], c[0]["end"]
    if kind == "struct":  # include the terminating '};' line if the brace line ends the struct
        pass
    sl = "\n".join(lines[s - 1:e])
    return sl, s, e


def expand(template_path, out_path=None):
    """Expand markers; //@@INCLUDE <file> inlines another template (same directory)."""
    src = open(template_path, encoding="utf-8").read().split("\n")
    out, manifest = [], []
    tname = os.path.basename(template_path)
    for k, ln in enumerate(src):
        if ln.startswith("//@@INCLUDE "):
            inc = os.path.join(os.path.dirname(template_path), ln.split()[1])
            sub_text, sub_man = expand(inc)
            out.append(sub_text)
            out.append('#line %d "%s"' % (k + 2, tname))
            manifest += sub_man
        elif ln.startswith("//@@ORIG "):
            _, pin, path, kind, name = ln.split()
            sl, s, e = locate_slice(pin, path, kind, name)
            b = sl.encode("utf-8")
            manifest.append({"pin": pin, "commit_short12": H.short12(pin), "path": path, "kind": kind,
                             "name": name, "lines": [s, e], "bytes": len(b),
                             "sha256_segments": H.sha256_segments(b)})
            out.append("/* ORIG %s:%s %s %s lines %d-%d (copied verbatim) */" % (pin, path, kind, name, s, e))
            out.append('#line %d "%s"' % (s, path))
            out.append(sl)
            out.append('#line %d "%s"' % (k + 2, tname))
        else:
            out.append(ln)
    if out_path is None:
        return '#line 1 "%s"\n' % tname + "\n".join(out), manifest
    with open(out_path, "w", encoding="utf-8") as f:
        f.write("\n".join(out))
    return manifest


def build_and_run(name, template, cases, workdir, extra_cflags=(), extra_inputs=()):
    """Return a dict with compile record, extraction manifest and per-case run records."""
    os.makedirs(workdir, exist_ok=True)
    rec = {"fixture": name, "template": os.path.basename(template), "cases": {}}
    exe = os.path.join(workdir, name)
    csrc = os.path.join(workdir, name + ".expanded.c")
    for p in (exe, csrc):
        if os.path.exists(p):
            os.remove(p)
    rec["stale_binary_removed_before_compile"] = not os.path.exists(exe)
    try:
        rec["extraction"] = expand(template, csrc)
    except Exception as e:  # extraction failure blocks the fixture, not the batch
        rec["extraction_error"] = "%s: %s" % (type(e).__name__, e)
        rec["status"] = "BLOCKED_EXTRACTION"
        for c in cases:
            rec["cases"][c] = {"status": "BLOCKED", "reason": "extraction failed"}
        return rec
    if CC is None:
        rec["status"] = "BLOCKED_NO_COMPILER"
        return rec
    cmd = [CC, *CFLAGS, *extra_cflags, "-I", os.path.dirname(os.path.abspath(template)),
           "-o", exe, csrc, *extra_inputs]
    comp = H.run_pg(cmd, COMPILE_LIMIT_S, cwd=workdir)
    comp["binary_exists"] = os.path.exists(exe)
    rec["compile"] = comp
    if comp["timed_out"] or comp["returncode"] != 0 or not comp["binary_exists"]:
        rec["status"] = "BLOCKED_COMPILE"
        for c in cases:
            rec["cases"][c] = {"status": "BLOCKED", "reason": "compile failed or timed out; nothing executed"}
        return rec
    rec["status"] = "BUILT"
    for c in cases:
        try:
            runs = []
            for rep in (1, 2):  # run twice to check stability
                r = H.run_pg([exe, c], RUN_LIMIT_S, cwd=workdir)
                r["events"] = parse_events(r["stdout"])
                runs.append(r)
            stable = [x["events"] for x in runs[0:1]] == [x["events"] for x in runs[1:2]] and \
                runs[0]["returncode"] == runs[1]["returncode"]
            rec["cases"][c] = {"status": "RAN", "run": runs[0], "repeat_identical": stable,
                               "repeat_returncode": runs[1]["returncode"]}
        except Exception as e:
            rec["cases"][c] = {"status": "ERROR", "reason": "%s: %s" % (type(e).__name__, e)}
    return rec


def parse_events(stdout):
    ev = []
    for ln in stdout.splitlines():
        ln = ln.strip()
        if ln.startswith("{"):
            try:
                ev.append(json.loads(ln))
            except ValueError:
                ev.append({"unparsed": ln})
    return ev
