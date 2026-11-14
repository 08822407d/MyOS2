"""Static source checks on the pinned time commit for V05, V07, V08, V09, V14 (no execution).

'active' = the line still has text after C comments are blanked (locate.strip_c). Searches cover
all *.c/*.h/*.S/*.lds files under mykernel/ in the pinned commit (git ls-tree), nothing else.
"""
import os
import re

import harness as H
import locate as L


def tree_files():
    out = H._git("ls-tree", "-r", "--name-only", H.commit("time"), "mykernel/").stdout.decode().split("\n")
    return [p for p in out if p.endswith((".c", ".h", ".S", ".lds"))]


def active_hits(pattern, files, only=None):
    rx = re.compile(pattern)
    hits = []
    for p in files:
        if only and not only(p):
            continue
        text = H.blob("time", p).decode("utf-8", errors="replace")
        view = L.strip_c(text).split("\n")
        for n, ln in enumerate(view):
            if rx.search(ln):
                hits.append({"path": p, "line": n + 1, "text": text.split("\n")[n].strip()})
    return hits


def lines_with(path, needles):
    text = H.blob("time", path).decode("utf-8").split("\n")
    return [{"line": n + 1, "text": l.strip()} for n, l in enumerate(text) for s in needles if s in l]


def main():
    files = tree_files()
    res = {"check": "STATIC", "time_short12": H.short12("time"), "files_scanned": len(files)}
    core = "mykernel/sched/scheduler/scheduler_core.c"
    # V05: documented state-mask contract vs active code
    res["V05"] = {
        "ttwu_doc_contract": lines_with(core, ["Conceptually does:", "If (@state & @p->state)",
                                               "Return: %true if @p->state changes"]),
        "sched_fork_contract": lines_with(core, ["We mark the process as NEW here", "nobody will actually run it",
                                                 "event cannot wake it up and insert it on the runqueue"]),
        "ttwu_state_match_active_calls": active_hits(r"\bttwu_state_match\s*\(", files),
    }
    # V07: who writes task CPU metadata; is __set_task_cpu active anywhere
    res["V07"] = {
        "active___set_task_cpu": active_hits(r"\b__set_task_cpu\s*\(", files),
        "active_thread_info_cpu_writes": active_hits(r"(thread_info\s*(\.|->)\s*cpu|task_thread_info\([^)]*\)\s*->\s*cpu)\s*=[^=]", files),
        "wake_up_new_task_rq_line": lines_with(core, ["rq_s *rq = &(per_cpu(runqueues, 0));"]),
        "ttwu_select_line": lines_with(core, ["cpu = select_task_rq(p, cpu, wake_flags | WF_TTWU);"]),
    }
    # V08: every active modification/consumer of running_lhdr; idle assignment; idle state writers
    res["V08"] = {
        "active_running_lhdr_sites": active_hits(r"running_lhdr", files),
        "active_rq_idle_assignments": active_hits(r"->idle\s*=[^=]", files),
        "init_task_state_initializer": active_hits(r"\.__state\s*=", files, only=lambda p: p.endswith("init/init_task.c")),
        "schedule_idle_expects_running": lines_with(core, ["WARN_ON_ONCE(current->__state);"]),
        "rest_init_state_changes": [h for h in active_hits(r"set_current_state|__state\s*=", files,
                                                           only=lambda p: p.endswith("init/main.c"))],
        "active_idle_state_writers": active_hits(r"idle\s*->\s*__state\s*=[^=]", files),
    }
    # V09: independent conditions on the finite path
    res["V09"] = {
        "finite_path_lines": lines_with("mykernel/time/timer/timer.c",
                                        ["__mod_timer(&timer.timer, expire, MOD_TIMER_NOTPENDING);",
                                         "// schedule();", "del_timer_sync(&timer.timer);",
                                         "return timeout < 0 ? 0 : timeout;"]),
        "active_process_timeout_references": active_hits(r"\bprocess_timeout\b", files),
        "active_msleep_references_outside_timer_c": [h for h in active_hits(r"\bmsleep\s*\(", files)
                                                     if not h["path"].endswith("time/timer/timer.c")],
        "active_timer_expiry_dispatch_candidates": active_hits(
            r"\b(call_timer_fn|expire_timers|__run_timers|run_timer_softirq|run_timer_base|run_local_timers|"
            r"timer_expire_remote)\s*\(|->function\s*\(|\bfn\s*\(\s*timer\s*\)", files),
        "active_schedule_timeout_family_references_outside_timer_c": [
            h for h in active_hits(r"\bschedule_timeout(_\w+)?\s*[\(,)]", files)
            if not h["path"].endswith("time/timer/timer.c")],
    }
    # V14: build references and symbol definitions
    lds = "mykernel/arch/x86_64/kernel.lds"
    res["V14"] = {
        "lds_alias": lines_with(lds, ["jiffies = jiffies_64;"]),
        "target_link_options": lines_with("mykernel/scripts/target_kernel.cmake", ["-T ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kernel.lds"]),
        "arch_default": lines_with("mykernel/scripts/options_flags.cmake", ["set(ARCH x86_64)"]),
        "active_c_definitions_of_jiffies": [h for h in active_hits(r"\bjiffies\b", files)
                                            if re.search(r"^\s*(__visible\s+)?(u64|ulong|unsigned long)\b[^(]*\bjiffies\b\s*[;=_]", h["text"])
                                            and "extern" not in h["text"]],
        "active_c_definitions_of_jiffies_64": [h for h in active_hits(r"\bjiffies_64\b", files)
                                               if re.search(r"^\s*(__visible\s+)?(u64|ulong|unsigned long)\b[^(]*\bjiffies_64\b", h["text"])
                                               and "extern" not in h["text"]],
        "late_time_init_callers": active_hits(r"\blate_time_init\s*\(", files),
        "active_extern_jiffies_declarations": [h for h in active_hits(r"extern\b.*\bjiffies\b", files)],
        "hpet_registration": active_hits(r"\bmyos_HPET_init\s*\(|\bhpet_time_init\s*\(|&HPET_handler", files),
    }
    elf = []
    for p in H._git("ls-tree", "-r", "--name-only", H.commit("time")).stdout.decode().split("\n"):
        if p and H.blob("time", p)[:4] == b"\x7fELF":
            elf.append(p)
    res["V14"]["elf_files_in_time_tree"] = elf
    H.emit(res, os.path.join(H.WORK, "static.json"), echo=False)
    print(H.dump({k: (v if k != "V08" else {kk: (len(vv) if isinstance(vv, list) and kk == "active_running_lhdr_sites" else vv)
                                           for kk, vv in v.items()}) for k, v in res.items()})[:6000])


if __name__ == "__main__":
    main()
