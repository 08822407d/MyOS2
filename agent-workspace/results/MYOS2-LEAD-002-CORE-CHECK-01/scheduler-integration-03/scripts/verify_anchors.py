# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-INTEGRATION-03 (scheduler-integration-03)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file; reuses only analyse()/enclosing() of scan_index.py in this directory
# purpose: mechanical quote check. (1) every source_facts[] entry of facts-and-regressions.yaml:
#   the quote must equal lines [start, end] of the file at time a039d9803ade, byte for byte per line,
#   and the stated symbol must enclose those lines (or be the macro the first line defines, or -
#   for file-scope lines such as per-cpu definitions, #if lines and linker-script entries - be
#   named on the first quoted line, or be the macro defined within 3 lines after a comment block;
#   in .S files the nearest preceding SYM_*_START(name) or label counts as the enclosing symbol);
#   (2) every "[VERIFIED path::symbol]" tag followed by a fenced block in the given Markdown files:
#   the block must occur as consecutive lines of that file inside that symbol; (3) the seven G
#   anchors of the lead review CORE-SCHED-ORDER-02-review.md at the pinned lead object, reported
#   separately (not merged into any older denominator). Prints a summary, writes JSON.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 verify_anchors.py <facts-and-regressions.yaml> <out.json> [<file.md> ...]"""
import json
import os
import re
import sys

import yaml

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import scan_index as S  # noqa: E402

LEAD_PIN = "40bc4faa4202"
REVIEW = "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-SCHED-ORDER-02-review.md"
TAG = re.compile(r"\[VERIFIED ([^\]:]+)::([^\]]+)\]\s*\n```[\w+-]*\n(.*?)\n```", re.S)


def symbol_at(path, a, ln):
    """Name of the definition enclosing line ln (C files), or of the enclosing set(...) block (cmake)."""
    if path.endswith((".cmake", "CMakeLists.txt")):
        for i in range(ln - 1, -1, -1):
            m = re.match(r"\s*set\s*\(\s*(\w+)", a["lines"][i])
            if m:
                return m.group(1)
        return None
    if path.endswith(".S"):                                    # assembly: nearest SYM_*_START(name) or label
        for i in range(ln - 1, -1, -1):
            m = re.match(r"\s*SYM_\w*START\w*\(\s*(\w+)", a["lines"][i]) or re.match(r"^([A-Za-z_]\w*):", a["lines"][i])
            if m:
                return m.group(1)
        return None
    s = S.enclosing(a, ln)
    return s["name"] if s else None


def check_quote(path, symbol, quote, start=None, end=None):
    a = S.analyse(path)
    if a is None:
        return {"path": path, "symbol": symbol, "ok": False, "reason": "file not readable at pin"}
    q = quote.split("\n")
    while q and q[-1] == "":
        q.pop()
    if start is None:
        hits = [i + 1 for i in range(len(a["lines"]) - len(q) + 1) if a["lines"][i:i + len(q)] == q]
    else:
        hits = [start] if a["lines"][start - 1:start - 1 + len(q)] == q and (end is None or end == start + len(q) - 1) else []
    res = {"path": path, "symbol": symbol, "quote_lines": len(q), "found_at": hits}
    if not hits:
        res.update(ok=False, reason="quote is not consecutive verbatim lines%s" % ("" if start is None else " at %s-%s" % (start, end)))
        return res
    good = []
    tok = re.compile(r"(?<![\w.])%s(?![\w])" % re.escape(symbol))
    for h in hits:
        names = {symbol_at(path, a, h), symbol_at(path, a, h + len(q) - 1)}
        if symbol in names or re.match(r"^\s*#\s*define\s+%s\b" % re.escape(symbol), q[0]):
            good.append(h)
        elif any(n and tok.search(n) for n in names):          # e.g. DEFINE_PER_CPU_...(type, symbol) = {
            good.append(h)
        elif names == {None} and tok.search(q[0]):             # file-scope line naming the symbol
            good.append(h)
        elif names == {None} and any(re.match(r"^\s*#\s*define\s+%s\b" % re.escape(symbol), a["lines"][k])
                                     for k in range(h - 1 + len(q), min(h + len(q) + 2, len(a["lines"])))):
            good.append(h)                                     # comment block directly above "#define symbol"
    res["symbol_enclosing"] = sorted(n for n in {symbol_at(path, a, h) for h in hits} if n)
    res["ok"] = bool(good) and 1 <= len(q) <= 5
    if not good:
        res["reason"] = "quote found but not inside %s" % symbol
    elif len(q) > 5:
        res["reason"] = "quote longer than 5 lines"
    return res


def md_tags(text):
    return [(m.group(1).strip(), m.group(2).strip(), m.group(3)) for m in TAG.finditer(text)]


def main(argv):
    facts_path, out = argv[0], argv[1]
    mds = argv[2:]
    res = {"check": "ANCHORS_INTEGRATION_03", "pin_time": S.PIN, "lead_pin": LEAD_PIN}
    doc = yaml.safe_load(open(facts_path, encoding="utf-8"))
    rows = []
    for f in doc.get("source_facts") or []:
        r = check_quote(f["path"], f["symbol"], f["quote"], *(f.get("lines") or [None, None]))
        r["id"] = f["id"]
        rows.append(r)
    res["facts_yaml"] = {"total": len(rows), "ok": sum(r["ok"] for r in rows), "failures": [r for r in rows if not r["ok"]],
                         "ids_checked": [r["id"] for r in rows]}
    res["markdown_tags"] = {}
    for md in mds:
        tags = md_tags(open(md, encoding="utf-8").read())
        rr = [dict(check_quote(p, s, q), tag="%s::%s" % (p, s)) for p, s, q in tags]
        res["markdown_tags"][os.path.basename(md)] = {"total": len(rr), "ok": sum(r["ok"] for r in rr),
                                                      "failures": [r for r in rr if not r["ok"]]}
    p = S.git("show", "%s:%s" % (LEAD_PIN, REVIEW))
    tags = md_tags(p.stdout.decode("utf-8")) if p.returncode == 0 else []
    g = [dict(check_quote(path, sym, q), anchor="G%02d" % (i + 1)) for i, (path, sym, q) in enumerate(tags)]
    res["lead_review_G_anchors"] = {"source": "%s @ %s" % (REVIEW, LEAD_PIN), "total": len(g), "ok": sum(r["ok"] for r in g),
                                    "details": [{k: r.get(k) for k in ("anchor", "path", "symbol", "quote_lines", "found_at", "ok", "reason")} for r in g],
                                    "note": "mechanical recount of the review's own anchors; not merged into the older 47-item denominator"}
    res["all_ok"] = (res["facts_yaml"]["ok"] == res["facts_yaml"]["total"]
                     and all(v["ok"] == v["total"] for v in res["markdown_tags"].values())
                     and res["lead_review_G_anchors"]["ok"] == res["lead_review_G_anchors"]["total"] == 7)
    text = json.dumps(res, ensure_ascii=False, indent=1)
    if S.HEX40.search(text):
        sys.stderr.write("REFUSED: 40-hex in output\n")
        return 3
    with open(out, "w", encoding="utf-8") as fh:
        fh.write(text + "\n")
    print(json.dumps({"facts_yaml": [res["facts_yaml"]["ok"], res["facts_yaml"]["total"]],
                      "markdown_tags": {k: [v["ok"], v["total"]] for k, v in res["markdown_tags"].items()},
                      "lead_review_G_anchors": [res["lead_review_G_anchors"]["ok"], res["lead_review_G_anchors"]["total"]],
                      "all_ok": res["all_ok"]}))
    return 0 if res["all_ok"] else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
