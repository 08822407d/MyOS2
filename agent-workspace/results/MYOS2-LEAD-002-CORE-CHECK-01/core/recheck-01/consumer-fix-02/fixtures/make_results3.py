# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-02 (consumer-fix-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file; complete entry replacing the make_results2.py CLI for consumers of recorded
#   observations (make_results2.py @ b843d475367a stays frozen and is still used for build_doc/render).
# change (S03): file reading -> schema/coverage checks -> per-item consumer status -> YAML. A missing,
#   empty, unparsable or wrongly typed input is recorded with file name, error class and affected items,
#   the remaining independent items are still judged, and the report is written. The source run's own
#   status is kept as a historical fact (source_execution); the consumer status is separate.
# exit codes: 0 consumer COMPLETE (findings allowed); 2 PARTIAL, report written; 4 consumer failure
#   (frozen extraction mismatch or an internal exception; an error record is written when possible).
# --------------------------------------------------------------------------------------------------
"""Usage: python3 make_results3.py --frozen <b843 extraction>/fixtures <observations dir> <out.yaml>"""
import argparse
import sys
import traceback

import common3 as C
import consumer3


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument("--frozen", required=True)
    ap.add_argument("obs_dir")
    ap.add_argument("out")
    try:
        a = ap.parse_args(argv)
    except SystemExit as e:
        return 2 if e.code else 0
    try:
        consumer3.bind(a.frozen)
        doc, code = consumer3.consume(a.obs_dir)
        text = consumer3.MR.render(doc)
    except Exception as e:  # noqa: BLE001 - reader/generator failure is not a verification finding
        err = {"status": "consumer_FAILURE", "error": "%s: %s" % (type(e).__name__, e), "trace_tail": traceback.format_exc()[-1200:]}
        try:
            C.emit(err, a.out + ".error.json")
        except SystemExit:
            pass
        sys.stderr.write("consumer failure: %s\n" % err["error"])
        return 4
    with open(a.out, "w", encoding="utf-8") as f:
        f.write(text)
    cv = doc["consumer_validation"]
    print(C.dump({"consumer_status": cv["status"], "input_problems": cv["input_problems"],
                  "items_not_valid": sorted(cv["items_not_valid"]), "generator_errors": len(doc.get("generator_errors") or [])}))
    return code


if __name__ == "__main__":
    sys.exit(main())
