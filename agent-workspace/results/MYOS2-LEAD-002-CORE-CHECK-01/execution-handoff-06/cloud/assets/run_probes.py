"""Scratch probe driver (not delivered): each probe breaks one thing in a copy of the package."""
import sys, os, shutil, subprocess, json, re
sys.dont_write_bytecode = True
import yaml
PKG = "/home/user/MyOS2/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-vfs-baseline-05"
CHK = PKG + "/scripts/uv_facts_check.py"
HERE = os.path.dirname(os.path.abspath(__file__))
def yl(p): return yaml.safe_load(open(p, encoding="utf-8"))
def yd(p, d): yaml.safe_dump(d, open(p, "w", encoding="utf-8"), allow_unicode=True, sort_keys=False, width=120)
def facts(fn):
    def f(dirp):
        p = dirp + "/facts-and-dependencies.yaml"; d = yl(p); fn(d); yd(p, d)
    return f
def dfind(fn):
    def f(dirp):
        p = dirp + "/deferred-findings.yaml"; d = yl(p); fn(d); yd(p, d)
    return f
def anc(i): return lambda d: d["anchors"][[a["id"] for a in d["anchors"]].index(i)]
def p1(d): a = anc("A-C4-10")(d); a["lines"] = [a["lines"][0] + 1, a["lines"][1] + 1]
def p2(d): anc("A-C3-07")(d)["kind"] = "in_body"
def p3(d): anc("A-C5-08")(d)["cpp_conditions"] = []
def p4(d): e = [x for x in d["edges"] if x["id"] == "E32"][0]; e["indirect"]["call_site"]["line"] += 1
def p5(d): v = [x for x in d["value_tables"] if x["id"] == "VT-MR01"][0]; v["rows"][0]["expect_head"] = "W"
def p6(d): v = [x for x in d["value_tables"] if x["id"] == "VT-FR01"][0]; v["rows"][1]["expect_lens"] = [4096]; v["rows"][1]["expect_sum_len"] = 4096
def p7(dirp):
    p = dirp + "/mm-04-disposition.md"; t = open(p, encoding="utf-8").read()
    t = t.replace("line: 268, fragment:", "line: 269, fragment:", 1); open(p, "w", encoding="utf-8").write(t)
def p8(d): d["findings"][0]["owner_action_now"] = "owner_fix_now"
def p9(d): d["capabilities"][0]["runtime_evidence"] = "PASS"
def p10(d): d["counts"]["edges"] += 1
def p11(d): q = [x for x in d["queries"] if x["id"] == "Q13"][0]; q["args"] = ["-n", "force_fatal_sig\\|bprm_execve"]
def p12(d): s = [x for x in d["structure_checks"] if x["id"] == "SC-01"][0]; s["symbol"] = "path_openat"
PROBES = [("p1", "anchor line shift", facts(p1)), ("p2", "anchor kind comment->body", facts(p2)),
          ("p3", "drop #if condition", facts(p3)), ("p4", "indirect call-site line", facts(p4)),
          ("p5", "VT-MR01 expected head", facts(p5)), ("p6", "VT-FR01 expected len", facts(p6)),
          ("p7", "supersedes line number", p7), ("p8", "finding owner_action_now", dfind(p8)),
          ("p9", "runtime PASS", facts(p9)), ("p10", "counts block", facts(p10)),
          ("p11", "negative query gets active hits", facts(p11)), ("p12", "structure check symbol", facts(p12))]
rows = []
for name, what, fn in [("p0", "unmodified copy", lambda d: None)] + PROBES:
    dirp = HERE + "/" + name
    shutil.rmtree(dirp, ignore_errors=True)
    shutil.copytree(PKG, dirp, ignore=shutil.ignore_patterns("__pycache__"))
    fn(dirp)
    r = subprocess.run([sys.executable, CHK, dirp, dirp + "/out.json"], capture_output=True, env=dict(os.environ, PYTHONDONTWRITEBYTECODE="1"))
    summ = r.stdout.decode().strip().splitlines()[-1] if r.stdout else r.stderr.decode()[-200:]
    try:
        all_ok = json.loads(summ)["all_ok"]
    except Exception:
        all_ok = None
    rows.append({"probe": name, "mutation": what, "returncode": r.returncode, "all_ok": all_ok, "summary": summ})
    print(name, r.returncode, summ[:230])
expected = all(r["returncode"] == 1 and r["all_ok"] is False for r in rows[1:]) and rows[0]["returncode"] == 0 and rows[0]["all_ok"] is True
rec = {"record": "checker_negative_probes", "checker": "scripts/uv_facts_check.py", "driver": "session scratch, not delivered",
       "note": "each probe modifies one thing in a scratch copy of the package; p0 is the unmodified copy; copies not delivered",
       "probes": rows, "all_as_expected": expected}
open(HERE + "/checker_probes.json", "w", encoding="utf-8").write(json.dumps(rec, ensure_ascii=False, indent=1) + "\n")
print("all_as_expected", expected)
