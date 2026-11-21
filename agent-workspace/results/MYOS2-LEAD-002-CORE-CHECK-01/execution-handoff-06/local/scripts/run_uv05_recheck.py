# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-EXECUTION-HANDOFF-06 (execution-handoff-06/local, L02)
# executor: Claude Code desktop app, local session; model: claude-opus-5-5 as declared by the runtime
#   (not independently attestable)
# source: new file; no code copied from earlier packages.
# purpose: path-only adapter for the frozen 05 checker chain (all three files read-only, unmodified):
#   user-vfs-baseline-05/scripts/uv_facts_check.py -> mm-baseline-04/scripts/mm_facts_check.py
#   -> scheduler-integration-03/scripts/scan_index.py
#   scan_index.REPO is the cloud container path /home/user/MyOS2. This entry loads the original
#   uv_facts_check by its own file path (it imports the other two by its original relative layout),
#   rebinds only the shared module attribute scan_index.REPO to the given clone root, and calls the
#   original uv_facts_check.main(). PIN, REVIEWED, inputs, expected values and every rule stay as
#   frozen. Bytecode writing is disabled before any import. One JSON line describing the binding is
#   written to stderr; stdout and the return code are the checker's own.
# ---------------------------------------------------------------------------------------------------
"""Usage: PYTHONDONTWRITEBYTECODE=1 python3 -B -P run_uv05_recheck.py <repo_root> <package_dir> <out.json> [<file.md> ...]"""
import sys
sys.dont_write_bytecode = True

import importlib.util  # noqa: E402
import json  # noqa: E402
import os  # noqa: E402


def main(argv):
    repo, pkg, rest = os.path.realpath(argv[0]), argv[1], argv[2:]
    uv_path = os.path.join(pkg, "scripts", "uv_facts_check.py")
    spec = importlib.util.spec_from_file_location("uv_facts_check", uv_path)
    uv = importlib.util.module_from_spec(spec)
    sys.modules["uv_facts_check"] = uv
    spec.loader.exec_module(uv)
    S = uv.S
    binding = {"adapter": "run_uv05_recheck", "repo_frozen": S.REPO, "repo_rebound": repo,
               "shared_module": S is sys.modules.get("scan_index") and uv.M.S is S,
               "pin": S.PIN, "reviewed": uv.REVIEWED,
               "modules": {n: os.path.relpath(sys.modules[n].__file__, repo)
                           for n in ("uv_facts_check", "mm_facts_check", "scan_index")}}
    if not binding["shared_module"]:
        sys.stderr.write(json.dumps(dict(binding, refused="scan_index is not one shared module"), ensure_ascii=False) + "\n")
        return 4
    S.REPO = repo
    sys.stderr.write(json.dumps(binding, ensure_ascii=False) + "\n")
    return uv.main([pkg] + rest)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
