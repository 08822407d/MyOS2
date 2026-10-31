# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file (uses the frozen locate.py unchanged)
# purpose: check the mainline replacement anchors A46-C / A46-ASM from the core-review (pin
#   lead_head) against time, with the same byte-exact whole-line rule and CMake definition locator as
#   the frozen V00. The old A46 verdict is read from this run's frozen V00 output and kept as is.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 a46_check.py <frozen_dir> <v00.json from this run> <out.json>"""
import json
import re
import sys

import harness2 as H
import build2

REVIEW = "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-core-review.md"
TAG = re.compile(r"^\[" + "VERIFIED" + r" ([^\]]+)\]\s*$")
AID = re.compile(r"^(A46-C|A46-ASM)｜")


def parse(text):
    lines, out, cur, i = text.split("\n"), [], None, 0
    while i < len(lines):
        m = AID.match(lines[i])
        if m:
            cur = m.group(1)
        t = TAG.match(lines[i])
        if t and cur:
            path, _, sym = t.group(1).partition("::")
            q, j = [], i + 2
            ok = lines[i + 1].startswith("```")
            while ok and j < len(lines) and lines[j] != "```":
                q.append(lines[j])
                j += 1
            out.append({"id": cur, "path": path, "symbol": sym, "quote": q, "fence_ok": ok})
            cur, i = None, j
        i += 1
    return out


def main():
    frozen_dir, v00p, outp = sys.argv[1:4]
    L = build2.load_frozen_locate(frozen_dir)
    rv = H.blob(H.PINS["lead_head"], REVIEW)
    res = {"check": "A46_CORRECTION", "review_short12": H.short12(H.PINS["lead_head"]),
           "review_sha256_segments": H.sha_segments(rv), "anchors": []}
    for a in parse(rv.decode("utf-8")):
        text = H.blob(H.PINS["time"], a["path"]).decode("utf-8")
        lines = text.split("\n")
        hits = L.find_contiguous(lines, a["quote"])
        cands = L.cmake_command(text, a["symbol"])
        within = [{"hit": h, "cand": [c["start"], c["end"]]} for h in hits for c in cands
                  if c["start"] <= h and h + len(a["quote"]) - 1 <= c["end"]]
        partial = [{"hit": h, "cand": [c["start"], c["end"]]} for h in hits for c in cands
                   if not (c["start"] <= h and h + len(a["quote"]) - 1 <= c["end"]) and c["start"] <= h + len(a["quote"]) - 1 and h <= c["end"]]
        v = "QUOTE_NOT_FOUND" if not hits else ("MATCH_IN_DEFINITION" if within else
                                                 ("OUTSIDE_OR_PARTIAL_DEFINITION" if partial else "BOUNDARY_NOT_LOCATED"))
        res["anchors"].append({"id": a["id"], "path": a["path"], "symbol": a["symbol"], "quote_lines": len(a["quote"]),
                               "fence_ok": a["fence_ok"], "contiguous_hits": hits,
                               "definition_candidates": [[c["start"], c["end"]] for c in cands], "within": within,
                               "partial": partial, "verdict_mech": v})
    old = {}
    try:
        old = {x["id"]: x for x in json.load(open(v00p, encoding="utf-8"))["anchors"]}
    except Exception as e:  # noqa: BLE001
        res["frozen_v00_read_error"] = "%s: %s" % (type(e).__name__, e)
    res["old_A46_verdict_in_frozen_v00"] = (old.get("A46") or {}).get("verdict_mech")
    H.emit(res, outp)
    print(H.dump({k: res[k] for k in ("old_A46_verdict_in_frozen_v00",)} | {"anchors": [(a["id"], a["verdict_mech"]) for a in res["anchors"]]}))


if __name__ == "__main__":
    main()
