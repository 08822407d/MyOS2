"""Scratch generator (not delivered): assemble facts-and-dependencies.yaml and deferred-findings.yaml."""
import sys
sys.dont_write_bytecode = True
import yaml  # noqa: E402
from resolve import resolve, M  # noqa: E402
from anchor_specs import SPECS, K  # noqa: E402
from query_specs import QSPECS  # noqa: E402
import gen_caps as GC  # noqa: E402
import gen_graph as GG  # noqa: E402
import gen_chains as CH  # noqa: E402
import gen_findings as GF  # noqa: E402

OUT = "/home/user/MyOS2/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-vfs-baseline-05/"

DROP = {"A-C1-04", "A-C1-08", "A-C1-12", "A-C1-13", "A-C2-01", "A-C2-02", "A-C2-04", "A-C2-06", "A-C3-02",
        "A-C3-08", "A-C3-17", "A-C4-04", "A-C4-06", "A-C4-27", "A-C5-13", "A-C6-01", "A-C6-02", "A-C6-05",
        "A-C6-06", "A-C6-13"}
FIX = {"A-C4-08": {5: 4}, "A-C4-16": {5: 4}, "A-C4-11": {2: "fat_fill_inode"},
       "A-C4-12": {4: "spin_unlock(&files->file_lock);", 5: 2}}
ADD = [("A-C2-16", K + "sched/forkexec/fork.c", "proc_caches_init", "in_body", "files_cachep =", 5, 1,
        "files_cache 创建时没有构造函数，标志中也没有清零")]


def build_anchors():
    out = []
    for sp in list(SPECS) + ADD:
        sp = list(sp)
        if sp[0] in DROP:
            continue
        for k, v in FIX.get(sp[0], {}).items():
            sp[k] = v
        an, nc = resolve(sp[:7])
        an["role"] = sp[7]
        out.append(an)
    out.sort(key=lambda a: (a["id"][:4], int(a["id"].split("-")[2])))
    return out


def build_queries():
    qs, runs = [], {}
    for qid, purpose, args, paths, expect in QSPECS:
        if expect == "size":
            q = {"id": qid, "purpose": purpose, "type": "size", "paths": paths, "expect": "size"}
            r = M.run_query(q)
            q["expected_bytes"] = [h["bytes"] for h in r["hits"]]
        else:
            q = {"id": qid, "purpose": purpose, "args": args, "paths": paths, "expect": expect}
            r = M.run_query(q)
            if r["tool_failure"]:
                raise SystemExit("tool failure %s %s" % (qid, r["stderr"]))
            q["expected_rc"] = r["returncode"]
            q["expected_hits_total"] = r["hits_total"]
            q["expected_hits_active"] = r["hits_active"]
            if expect == "negative_active" and r["hits_active"]:
                raise SystemExit("negative query %s has active hits" % qid)
        qs.append(q)
        runs[qid] = r
    return qs, runs


def fill_hit(h, runs):
    r = runs[h["query"]]
    m = [x for x in r["hits"] if x["path"] == h["path"] and x["line"] == h["line"]]
    if not m or not m[0]["active"]:
        raise SystemExit("hit not found/active: %s" % h)
    return {"query": h["query"], "path": h["path"], "line": h["line"], "enclosing": m[0]["enclosing"]}


def main():
    anchors = build_anchors()
    queries, runs = build_queries()
    edges = []
    for e in GG.EDGES:
        e = dict(e)
        e["hits"] = [fill_hit(h, runs) for h in e.get("hits") or []]
        if "indirect" in e:
            e["indirect"] = {k: (fill_hit(v, runs) if isinstance(v, dict) else v) for k, v in e["indirect"].items()}
        edges.append(e)
    caps = []
    for c in GC.CAPS:
        c = dict(c)
        c["id_status"] = "proposed_id"
        c["runtime_evidence"] = "NOT_RUN"
        c.setdefault("structure_checks", [])
        c.setdefault("value_tables", [])
        c["edges"] = [e["id"] for e in edges if c["id"] in (e["from"], e["to"]) or c["id"] in (e.get("also_applies_to") or [])]
        order = ["id", "id_status", "object", "old_refs", "chains", "purpose", "entries", "key_helpers", "normal_path",
                 "failure_path", "source_condition", "anchors", "queries", "structure_checks", "value_tables", "edges",
                 "findings", "implementation_evidence", "correctness_evidence", "runtime_evidence",
                 "concurrency_evidence", "not_covered"]
        caps.append({k: c[k] for k in order})
    per = {o["id"]: sum(1 for c in caps if c["object"] == o["id"]) for o in GC.OBJECTS}
    impl = {}
    for c in caps:
        impl[c["implementation_evidence"]["value"]] = impl.get(c["implementation_evidence"]["value"], 0) + 1
    counts = {"objects": len(GC.OBJECTS), "capabilities": len(caps), "capabilities_per_object": per,
              "anchors": len(anchors), "anchors_by_kind": {k: sum(1 for a in anchors if a["kind"] == k) for k in sorted(M.KINDS)},
              "edges": len(edges), "edges_by_kind": {k: sum(1 for e in edges if e["kind"] == k) for k in sorted(M.EDGE_KINDS)},
              "queries": len(queries), "structure_checks": len(CH.STRUCTURE_CHECKS), "value_tables": 2,
              "chains": len(CH.CHAINS), "link_breaks": len(GG.LINK_BREAKS), "dependency_risks": len(GG.DEP_RISKS),
              "next_validation_candidates": len(CH.CANDIDATES), "gaps": len(CH.GAPS),
              "gaps_open": sum(1 for g in CH.GAPS if g["status"] == "open"), "old_inputs_index": len(CH.OLD_INDEX),
              "implementation_evidence": {k: impl.get(k, 0) for k in sorted(M.IMPL)}}
    facts = {
        "task_id": "MYOS2-LEAD-002-CORE-CHECK-01", "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
        "followup_id": "CORE-USER-VFS-BASELINE-05", "record_type": "user_process_and_vfs_static_baseline_facts",
        "produced_by": "Claude Code cloud session (claude.ai/code)", "executor": "Claude Code cloud session (claude.ai/code)",
        "model": "unknown_or_not_attestable", "date": "2026-10-02",
        "base_snapshot": "kernel=time a039d9803ade；workspace=master de3bb1df906a（开工时事实，未固定）；主线 agent/MYOS2-LEAD-002 0d066801371d（开工时）；被审执行头 de7c96ede546；MM 结果冻结 eb75b3100601。均为短 SHA",
        "pins": {"kernel_time": "a039d9803ade", "workspace_master_at_start": "de3bb1df906a", "lead_object_at_start": "0d066801371d",
                 "execution_reviewed": "de7c96ede546", "mm_results_frozen": "eb75b3100601"},
        "status": "final_for_user_vfs_baseline_05",
        "evidence_class": "静态源码盘点（git 对象只读查询 + 逐字引文 + 定点检索 + 词法结构检查 + 规则算术）；没有编译，没有运行 C/ASM、内核、QEMU 或仓库脚本。",
        "inputs_read": "见 evidence.md §2；源码文件清单为各 anchors/queries 的 path 并集",
        "open_questions": "见 gaps（全部 open）与 deferred-findings.yaml 中 status 为 needs_evidence 的条目",
        "axes": {"implementation_evidence": "declaration_only / partial_body / connected_body / not_located_in_scope / not_assessed；本批局部证据状态，不是成熟度分数，不折算旧 002 的 0–4",
                 "correctness_evidence": "not_assessed / static_concern / bounded_prior_observation；static_concern 必须指向 deferred-findings.yaml 中的条目",
                 "runtime_evidence": "本批统一 NOT_RUN",
                 "concurrency_evidence": "not_assessed / source_only（没有 SMP 实测）",
                 "source_condition.resolution": "not_conditional / resolved / partially_resolved / unknown"},
        "edge_direction": {"call": "调用方 → 被调用方", "data": "读取/消费方 → 产生/提供方",
                           "init_order": "后执行或后可用者 → 必须先完成者（后依赖先）", "config": "受影响的代码 → 提供开关的配置",
                           "build": "被编译的代码 → 构建脚本"},
        "edge_rules": ["include 不是 call；GLOB 不是运行可达；词法 active 不是预处理或链接确认",
                       "间接调用必须给出调用点、对象或回调绑定、接收方三项证据（indirect 字段），缺一项就登记为断开",
                       "宏展开（ROOTBLK_TRANSFER）按宏定义加配置前提记为调用，并另记 config 边"],
        "negative_assertion_rule": "所有否定断言都带查询范围、返回码与活动命中数（queries），或词法结构检查（structure_checks）；只说明所查范围内没有，不推广为全树结论。",
        "anchor_id_note": "锚点编号按链分组（A-C1…A-C6），裁剪到上限时删去的编号不复用，编号间断无含义。",
        "inputs_old": {"sources": [
            {"path": "agent-workspace/results/MYOS2-DR-002/completeness.yaml", "object": "master de3bb1df906a",
             "read": "sched.forkexec 与 fs.vfs 两个条目的 id 与 node 名；status/maturity/risk/evidence/notes 未导入"},
            {"path": "agent-workspace/results/MYOS2-DR-003/deps.yaml", "object": "master de3bb1df906a",
             "read": "edges 中与两个对象相交的端点名；evidence、行号、hardness/confidence 未导入"}],
            "rule": "旧 ID 原样保留；本批新增的细粒度能力一律标 proposed_id，不改公共词汇表；旧断言不继承。"},
        "counts": counts,
        "objects": GC.OBJECTS,
        "capabilities": caps,
        "chains": CH.CHAINS,
        "edges": edges,
        "link_breaks": GG.LINK_BREAKS,
        "dependency_risks": GG.DEP_RISKS,
        "u04_usability": {"layer1": "见各 chains[].segments：connected / connected_with_concern / connected_under_config_text / build_text_only / endpoint / broken",
                          "layer2": CH.U04_LAYER2},
        "next_validation_candidates": CH.CANDIDATES,
        "old_inputs_index": CH.OLD_INDEX,
        "structure_checks": CH.STRUCTURE_CHECKS,
        "value_tables": [CH.VT_MR01, CH.VT_FR01],
        "anchors": anchors,
        "queries": queries,
        "gaps": CH.GAPS,
    }
    with open(OUT + "facts-and-dependencies.yaml", "w", encoding="utf-8") as fh:
        yaml.safe_dump(facts, fh, allow_unicode=True, sort_keys=False, width=120)
    nf = len(GF.FINDINGS)
    df = {
        "task_id": "MYOS2-LEAD-002-CORE-CHECK-01", "followup_id": "CORE-USER-VFS-BASELINE-05",
        "record_type": "deferred_findings_increment",
        "produced_by": "Claude Code cloud session (claude.ai/code)", "model": "unknown_or_not_attestable", "date": "2026-10-02",
        "policy_ref": "agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md（agent/MYOS2-LEAD-002）",
        "policy_status": "ACTIVE_RECORD_FIRST_OWNER_REPAIR_DEFERRED",
        "owner_action_now": "none", "owner_decisions_requested_now": [],
        "kernel_change_authorized": False, "candidate_A_adopted": False, "candidate_B_adopted": False,
        "scope": "只登记本批新增条目（UV-01…UV-%02d）、本批处置的两项文档勘误，以及对旧记录的状态变更与交叉链接；不复制历史问题全文，不重编号旧条目。" % nf,
        "empty_set_statement": "本批发现集合非空：%d 项新条目、2 项文档勘误、%d 项旧条目状态变更。检查范围见 facts-and-dependencies.yaml 的 chains 与 coverage_limits；没有登记的部分不等于已确认安全。" % (nf, len(GF.STATUS_CHANGES)),
        "kinds_allowed": ["source_fact", "conditional_inference", "limited_host_observation", "policy_proposal", "evidence_gap", "documentation_erratum"],
        "statuses_allowed": ["deferred_owner_not_ready", "needs_evidence", "superseded_interpretation"],
        "resume_order": ["agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md（agent/MYOS2-LEAD-002）",
                         "本轨道最新主线检查点与对应审查回执（agent/MYOS2-LEAD-002 的 reviews/）",
                         "本包 user-vfs-baseline.md 与 facts-and-dependencies.yaml",
                         "被选条目的 qualified_id（本文件）",
                         "原证据（anchors/queries 所指 time 源码）与当时最新的 time"],
        "counts": {"findings": nf, "errata": len(GF.ERRATA), "status_changes": len(GF.STATUS_CHANGES), "cross_links": len(GF.CROSS_LINKS),
                   "findings_by_status": {s: sum(1 for f in GF.FINDINGS if f["status"] == s) for s in ("deferred_owner_not_ready", "needs_evidence")}},
        "findings": GF.FINDINGS,
        "errata": GF.ERRATA,
        "status_changes": GF.STATUS_CHANGES,
        "cross_links": GF.CROSS_LINKS,
    }
    with open(OUT + "deferred-findings.yaml", "w", encoding="utf-8") as fh:
        yaml.safe_dump(df, fh, allow_unicode=True, sort_keys=False, width=120)
    print("anchors", len(anchors), "edges", len(edges), "caps", len(caps), per, "queries", len(queries), "findings", nf)


if __name__ == "__main__":
    main()
