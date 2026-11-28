"""Remote readback with explicit success criteria (review sec.3 item 3).

ok requires, per channel: curl exit 0, no timeout, HTTP 200, remote branch head == expected commit
(ls-remote), response bytes == blob of <path> at the expected commit, and (optionally) YAML parse
yielding field == value. A worktree read is never used as the comparison source.
Usage: python3 readback.py <branch> <commit-short> <path> [<yaml_field>=<value>] [--out FILE]
"""
import os
import sys

import yaml

import harness as H

OWNER_REPO = "08822407d/MyOS2"


def remote_read(branch, commit_short, path, field=None, value=None, tag="rb"):
    full = H._git("rev-parse", "--verify", commit_short + "^{commit}").stdout.decode().strip()
    ref = H._git("ls-remote", "origin", "refs/heads/" + branch).stdout.decode().split("\t")[0]
    exp = H._git("show", "%s:%s" % (full, path), check=False)
    res = {"branch": branch, "commit_short12": commit_short[:12], "path": path,
           "remote_head_equals_commit": ref == full,
           "expected_blob_available": exp.returncode == 0,
           "expected_bytes": len(exp.stdout) if exp.returncode == 0 else None,
           "expected_sha256_segments": H.sha256_segments(exp.stdout) if exp.returncode == 0 else None,
           "channels": []}
    urls = {
        "raw.githubusercontent.com": ["https://raw.githubusercontent.com/%s/%s/%s" % (OWNER_REPO, branch, path), []],
        "api.github.com_contents_raw": ["https://api.github.com/repos/%s/contents/%s?ref=%s" % (OWNER_REPO, path, branch),
                                        ["-H", "Accept: application/vnd.github.raw"]],
    }
    ok = res["remote_head_equals_commit"] and res["expected_blob_available"]
    for name, (url, hdr) in urls.items():
        out = os.path.join(H.WORK, "%s_%s.bin" % (tag, name.split(".")[0]))
        if os.path.exists(out):
            os.remove(out)
        r = H.run_pg(["curl", "-sS", "--max-time", "25", "-w", "%{http_code}", "-o", out, *hdr, url], 30)
        body = open(out, "rb").read() if os.path.exists(out) else b""
        ch = {"channel": name, "url": url, "curl_exit": r["returncode"], "timed_out": r["timed_out"],
              "http_code": r["stdout"].strip(), "curl_stderr": r["stderr"].strip(), "bytes": len(body),
              "sha256_segments": H.sha256_segments(body)}
        ch["transport_ok"] = ch["curl_exit"] == 0 and not ch["timed_out"] and ch["http_code"] == "200"
        ch["byte_identical"] = res["expected_blob_available"] and body == exp.stdout
        if field:
            try:
                got = (yaml.safe_load(body.decode("utf-8")) or {}).get(field)
            except Exception as e:  # noqa: BLE001
                got = "PARSE_ERROR:%s" % type(e).__name__
            ch["field"] = {field: got}
            ch["field_ok"] = str(got) == value
        else:
            ch["field_ok"] = True
        ch["ok"] = ch["transport_ok"] and ch["byte_identical"] and ch["field_ok"]
        ok = ok and ch["ok"]
        res["channels"].append(ch)
    res["ok"] = bool(ok)
    return res


if __name__ == "__main__":
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    field = value = None
    if len(args) > 3:
        field, value = args[3].split("=", 1)
    out = sys.argv[sys.argv.index("--out") + 1] if "--out" in sys.argv else os.path.join(H.WORK, "readback.json")
    r = remote_read(args[0], args[1], args[2], field, value)
    H.emit(r, out)
    sys.exit(0 if r["ok"] else 1)
