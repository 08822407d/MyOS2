# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: revised from core/fixtures/build.py @ a1e7c2277705 (frozen original unchanged)
# change: expansion logic kept line-for-line (same markers, same #line layout, same compile flags),
#   but templates and the locator are loaded from the frozen extraction directory, runs use the
#   bounded harness2.run_pg (explicit terminal states), repeat comparison is on raw stdout bytes plus
#   exit code, and a meta-test hook can inject a case exception. Compile/extraction failure still
#   blocks every case of the fixture; nothing stale is executed.
# --------------------------------------------------------------------------------------------------
"""Build and run frozen fixture templates against the pinned time commit."""
import importlib.util
import os
import shutil

import harness2 as H

CC = shutil.which("gcc")
CFLAGS = ["-std=gnu11", "-O0", "-g", "-fno-strict-aliasing", "-Wall", "-Wno-unused-label",
          "-Wno-unused-variable", "-Wno-unused-function"]
COMPILE_LIMIT_S = 60
RUN_LIMIT_S = 5


def load_frozen_locate(frozen_dir):
    spec = importlib.util.spec_from_file_location("locate_frozen", os.path.join(frozen_dir, "locate.py"))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


class Builder:
    def __init__(self, frozen_dir, repo=None):
        self.frozen_dir = frozen_dir
        self.L = load_frozen_locate(frozen_dir)
        self.repo = repo

    def locate_slice(self, pin, path, kind, name):
        text = H.blob(H.PINS[pin], path, self.repo).decode("utf-8")
        L = self.L
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
        return "\n".join(lines[s - 1:e]), s, e

    def expand(self, template_path, out_path=None):
        src = open(template_path, encoding="utf-8").read().split("\n")
        out, manifest = [], []
        tname = os.path.basename(template_path)
        for k, ln in enumerate(src):
            if ln.startswith("//@@INCLUDE "):
                inc = os.path.join(os.path.dirname(template_path), ln.split()[1])
                sub_text, sub_man = self.expand(inc)
                out.append(sub_text)
                out.append('#line %d "%s"' % (k + 2, tname))
                manifest += sub_man
            elif ln.startswith("//@@ORIG "):
                _, pin, path, kind, name = ln.split()
                sl, s, e = self.locate_slice(pin, path, kind, name)
                b = sl.encode("utf-8")
                manifest.append({"pin": pin, "commit_short12": H.short12(H.PINS[pin], self.repo), "path": path,
                                 "kind": kind, "name": name, "lines": [s, e], "bytes": len(b),
                                 "sha256_segments": H.sha_segments(b)})
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

    def build_and_run(self, name, template, cases, workdir, extra_cflags=(), extra_inputs=(), inject_case_error=None):
        os.makedirs(workdir, exist_ok=True)
        rec = {"fixture": name, "template": os.path.basename(template), "cases": {}}
        exe = os.path.join(workdir, name)
        csrc = os.path.join(workdir, name + ".expanded.c")
        for p in (exe, csrc):
            if os.path.exists(p):
                os.remove(p)
        rec["stale_binary_removed_before_compile"] = not os.path.exists(exe)
        try:
            rec["extraction"] = self.expand(template, csrc)
            with open(csrc, "rb") as f:
                data = f.read()
            rec["expanded_source"] = {"bytes": len(data), "sha256_segments": H.sha_segments(data)}
        except Exception as e:  # noqa: BLE001
            rec["extraction_error"] = "%s: %s" % (type(e).__name__, e)
            rec["status"] = "BLOCKED_EXTRACTION"
            for c in cases:
                rec["cases"][c] = {"status": "BLOCKED", "reason": "extraction failed"}
            return rec
        if CC is None:
            rec["status"] = "BLOCKED_NO_COMPILER"
            for c in cases:
                rec["cases"][c] = {"status": "BLOCKED", "reason": "no compiler"}
            return rec
        cmd = [CC, *CFLAGS, *extra_cflags, "-I", os.path.dirname(os.path.abspath(template)), "-o", exe, csrc, *extra_inputs]
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
                if inject_case_error == c:
                    raise RuntimeError("HARNESS_META_TEST injected case exception")
                r1 = H.run_pg([exe, c], RUN_LIMIT_S, cwd=workdir)
                r2 = H.run_pg([exe, c], RUN_LIMIT_S, cwd=workdir)
                rec["cases"][c] = {"status": "RAN", "run": r1, "repeat_identical": r1["stdout"] == r2["stdout"]
                                   and r1["returncode"] == r2["returncode"], "repeat_returncode": r2["returncode"],
                                   "repeat_terminal_state": r2["terminal_state"]}
            except Exception as e:  # noqa: BLE001
                rec["cases"][c] = {"status": "ERROR", "reason": "%s: %s" % (type(e).__name__, e)}
        return rec
