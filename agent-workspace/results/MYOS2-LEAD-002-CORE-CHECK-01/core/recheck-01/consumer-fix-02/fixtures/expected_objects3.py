# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-02 (consumer-fix-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file
# purpose (S02): the object sets a consumer must find in the stage records, extracted ONCE from frozen
#   inputs - never from the record being checked. Data sources: taskbook 57a7c3e0eebf (07 report front
#   matter, 07-core-audit-map.yaml, lead MANIFEST.md self_check). Selection rules and key lists are those
#   of the producer programs that wrote the records (core/fixtures/v01_structure.py @ a1e7c2277705 for
#   V01; recheck-01 evaluate2.v08 / make_results2.v14_source @ b843d475367a for the static keys they
#   read). The result is written to fixtures/expected_objects.json and re-derived by final_check3.
#   Reads a few pinned blobs; no kernel tree scan.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 expected_objects3.py <out.json>"""
import sys

import yaml

import common3 as C

MAP = C.LEAD + "07-core-audit-map.yaml"
REPORT = C.LEAD + "07-scheduler-wakeup-timer-audit.md"
MANIFEST = C.LEAD + "MANIFEST.md"
V01_PRODUCER = C.RESULTS_ROOT + "core/fixtures/v01_structure.py"


def front(data):
    t = data.decode("utf-8")
    return yaml.safe_load(t[4:t.index("\n---\n", 4)])


def extract():
    tb = C.PINS["taskbook"]
    raw = {p: C.blob(tb, p) for p in (MAP, REPORT, MANIFEST)}
    m = yaml.safe_load(raw[MAP].decode("utf-8"))
    rep = front(raw[REPORT])
    man = front(raw[MANIFEST])
    # V01 rules as in v01_structure.py: map.report, map.validation_contract, first token of old_claim_ref
    ref = {"map.report": m.get("report"), "map.validation_contract": m.get("validation_contract")}
    for i in m.get("issues", []):
        if i.get("old_claim_ref"):
            ref["map.%s.old_claim_ref" % i["issue_id"]] = i["old_claim_ref"].split()[0]
    inputs = [p for p in rep.get("inputs_read", []) if isinstance(p, str) and "/" in p and " " not in p]
    scope = list((man.get("self_check") or {}).get("scope_files") or [])
    n_quotes = (m.get("validation") or {}).get("source_quote_count_in_report_expected")
    return {
        "check": "EXPECTED_OBJECTS3",
        "sources": {p.replace(C.LEAD, "lead/"): {"commit_short12": C.short12(tb), "bytes": len(d), "sha256_segments": C.sha_segments(d)}
                    for p, d in raw.items()},
        "selection_rules_from": {"v01": "core/fixtures/v01_structure.py @ %s (FILES, paths, report inputs_read filter, scope_files)"
                                        % C.short12(C.PINS["core_frozen"]),
                                 "static": "recheck-01 evaluate2.v08 and make_results2.v14_source @ %s" % C.short12(C.PINS["reviewed_input"])},
        "v01": {
            "referenced_paths": ref,
            "report_inputs_read": inputs,
            "report_inputs_read_unique": len(set(inputs)) == len(inputs),
            "manifest_scope_files": scope,
            "hex40_scope_file_keys": [f.split("/")[-1] for f in scope],
            "parse_keys": ["report", "map", "contract09", "pilot10", "core11", "manifest"],
            "completion_flag_keys": ["whole_002R_complete", "whole_003R_complete", "whole_007R_complete", "whole_wave2_complete",
                                     "acceptance_verdict", "kernel_modified", "map.replacement_for_002R_003R_007R"],
        },
        "v00": {"anchor_ids": ["A%02d" % k for k in range(1, (n_quotes or 0) + 1)], "count_source": "map.validation.source_quote_count_in_report_expected",
                "consumed_anchor_keys": ["id", "verdict_mech", "contiguous_hits", "length_1_to_5", "path_exists_in_time", "semantic"]},
        "a46": {"anchor_ids": ["A46-C", "A46-ASM"], "source": "reviews/CORE-CHECK-01-core-review.md @ %s" % C.short12(C.PINS["lead_recheck01"])},
        "static": {"V08": ["active_idle_state_writers", "rest_init_state_changes", "active_rq_idle_assignments", "init_task_state_initializer"],
                   "V14": ["lds_alias", "target_link_options", "active_c_definitions_of_jiffies", "active_c_definitions_of_jiffies_64",
                           "elf_files_in_time_tree"]},
    }


if __name__ == "__main__":
    d = extract()
    C.emit(d, sys.argv[1])
    print(C.dump({"referenced_paths": len(d["v01"]["referenced_paths"]), "report_inputs_read": len(d["v01"]["report_inputs_read"]),
                  "hex40_scope_files": len(d["v01"]["hex40_scope_file_keys"]), "v00_anchor_ids": len(d["v00"]["anchor_ids"])}))
