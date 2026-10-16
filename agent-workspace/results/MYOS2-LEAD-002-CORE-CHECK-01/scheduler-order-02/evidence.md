---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-SCHED-ORDER-02
phase: scheduler_order_witness
record_type: scheduler_order_evidence
produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"
execution_model_selection: unknown_or_not_attestable
execution_model_selection_source: "执行者所在平台的规则不允许在推送到仓库的产物中写模型标识（执行者自述）"
date: "2026-10-02"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；主线 6706013a079a（开工时固定）；冻结执行头 0d62c4d19711（短 SHA）"
results_commit_short12: 5078686e8267
status: final_for_scheduler_order_02
transcription: "§1.1、§1.3 与 §9 的推送输出是会话命令的手工转录；其余数值、表格与输出由 fixtures/build_evidence4.py 从 observations/ 与 results.yaml 生成。observations 下的文件是程序写出的原文件。"
file_moves: "observations/after_results/ 三份记录先写在会话临时目录，再逐字节复制（cmp 相同）；readback2 的 rbwork-* 下载目录未入库。"
redaction: "未发现需脱敏内容；push 输出经 40 位十六进制过滤（无命中）；runs.json 中可执行文件路径记为 <build>/fx_order。"
open_questions: []
---

# CORE-SCHED-ORDER-02 证据：pick_next_task_myos 回插与计费时点的八组宿主见证

**八组场景全部完成，每组两次独立运行，记录完整且两次相同。主线的两个候选都得到有限实测见证：W01 的游标问题（A 被插到 idle 之后、队尾），W02 与 W07 第 1 步的计费时点问题（先按 15 插到 C20 前，再记成 25）。主线推导逐项相符，没有反证。** 这只是一个原函数在宿主进程中的边界行为，不是 MyOS2 运行，也不是内核正确性结论。

## 1. 输入、绑定与 PR

### 1.1 读取（手工转录）

```text
$ curl -sS -o $S/15.md -w 'http=%{http_code} bytes=%{size_download}\n' https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/15-scheduler-ordering-witness-contract.md
http=200 bytes=15008
15 raw == git object
$ curl ... reviews/CORE-CHECK-01-recheck-02-review.md
http=200 bytes=10591
review raw == git object
$ git show origin/agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/14-scheduler-ordering-followup.md   (全文读取)
```

开工时 origin/agent/MYOS2-LEAD-002 位于 `6706013a079a`，相对上一轮固定的 `b0aa54db1b70` 只新增三个文件；本轮把 `6706013a079a` 固定为主线对象。

### 1.2 编译前的保护门（guard4）

```json
{
 "gate": {
  "review_binding": true,
  "contract_binding": true,
  "analysis14_present": true,
  "lead_advanced_by_additions_only": true,
  "reviewed_chain": true,
  "on_execution_branch": true,
  "head_descends_from_0d62": true,
  "0d62_files_unchanged_only_additions_under_prefix": true,
  "remote_on_same_line_and_contains_0d62": true,
  "kernel_source_equals_pin": true,
  "ok": true
 },
 "review_fields": {
  "record_id": "CORE-CHECK-01-RECHECK-02-REVIEW-001",
  "status": "BOUNDED_RECHECK_CLOSED",
  "acceptance_verdict": "PASS_PENDING_LOCAL",
  "reviewed_pr": 17,
  "reviewed_branch": "claude/dazzling-cori-q0dnyt",
  "reviewed_commit_short12": "0d62c4d19711",
  "results_frozen_short12": "77faf51e9a43",
  "remaining_return_items_in_contract13": []
 },
 "contract_fields": {
  "followup_id": "CORE-SCHED-ORDER-02",
  "required_review_record": "CORE-CHECK-01-RECHECK-02-REVIEW-001",
  "reviewed_execution_short12": "0d62c4d19711",
  "consumer_results_frozen_short12": "77faf51e9a43",
  "kernel_short12": "a039d9803ade",
  "execution_branch": "claude/dazzling-cori-q0dnyt",
  "initial_execution_pr": 17,
  "allowed_write_prefix": "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-order-02/",
  "kernel_change_authorized": false,
  "phase3_authorized": false,
  "acceptance_ceiling": "PASS_PENDING_LOCAL"
 },
 "lead_changes_since_previous_pin": [
  "A\t15-scheduler-ordering-witness-contract.md",
  "A\tcheckpoints/2026-10-02-consumer-closed-scheduler-witness-ready.md",
  "A\treviews/CORE-CHECK-01-recheck-02-review.md"
 ],
 "execution": {
  "branch": "claude/dazzling-cori-q0dnyt",
  "head_short12": "0d62c4d19711",
  "remote_relation": "equal"
 },
 "frozen_0d62_files": {
  "under_results_root": 84,
  "whole_tree": 1254
 },
 "remote_refs_equal_pins": {
  "master": true,
  "time": true,
  "agent/MYOS2-LEAD-002": true
 }
}
```

guard4 是本任务自己的绑定：15 号任务书与 RECHECK-02 收口回执的字段、主线只追加、77fa → 0d62 只新增、HEAD/索引/工作树相对 0d62 只在 scheduler-order-02/ 下新增、time 未动、远端同线。它没有把 guard3 的 consumer-fix-02 前缀放宽成白名单。

### 1.3 PR 处置（手工转录）

开工时 PR #17 为 open、Ready（主线收口后转为 Ready）、未合并；master 仍为 `de3bb1df906a`。按 15 号 §7，新实验开始时已把 PR #17 转回 Draft 并复用；没有新开 PR。open PR 只有 #16（主线，写区 lead/）与 #17。

## 2. 原函数抽取与构建

被测函数 `mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`，time `a039d9803ade` 第 5–58 行，1598 字节，SHA-256 `2ff36b570d0422f3 ccb84857384d5cd7 c8df90beee74d36a adda9dacb713e54f`。原样插入，未改一个字符。

全部 `//@@ORIG` 抽取（由 0d62 冻结的 build2.Builder.expand 从 time 对象逐字复制）：

| 来源 | 种类 | 符号 | 行 | 字节 | SHA-256[0] |
|---|---|---|---|---|---|
| mykernel/lib/list/double_list_types.h | struct | list_head | 8–11 | 57 | ebf1c027f3463abc |
| mykernel/lib/list/double_list_types.h | struct | list_hdr | 13–16 | 56 | d248e3e72255d766 |
| mykernel/lib/list/double_list_const.h | macro | LIST_POISON1 | 5–5 | 39 | 4208b26ec9006c27 |
| mykernel/lib/list/double_list_const.h | macro | LIST_POISON2 | 6–6 | 39 | 7e513a95411558d5 |
| mykernel/include/uapi/linux/stddef.h | macro | container_of | 9–13 | 263 | ae12efae715f1169 |
| mykernel/lib/list/double_list.h | macro | INIT_LIST_HEAD | 125–125 | 48 | 0c5f3ae9c2784896 |
| mykernel/lib/list/double_list.h | macro | list_is_head | 132–132 | 42 | 7eadc5411f450e4d |
| mykernel/lib/list/double_list_macro.h | macro | LIST_INIT | 5–8 | 65 | 3a4d4e1a6894cb52 |
| mykernel/lib/list/double_list_macro.h | macro | LIST_HEADER_INIT | 9–12 | 106 | fd948458e96905d1 |
| mykernel/lib/list/double_list_macro.h | macro | list_container | 25–26 | 80 | 98d6a6e49e50f5a3 |
| mykernel/lib/list/double_list_macro.h | macro | list_entry | 27–27 | 34 | 18fedd137eb90cba |
| mykernel/lib/list/double_list_macro.h | macro | list_first_entry | 37–38 | 88 | 9158858037a4a4dc |
| mykernel/lib/list/double_list_macro.h | macro | list_for_each | 88–91 | 126 | 0d8764ba976cf21c |
| mykernel/lib/list/double_list_macro.h | macro | list_headr_first_container | 334–335 | 109 | 68549932735e1f43 |
| mykernel/lib/list/double_list_macro.h | macro | list_header_foreach | 340–341 | 83 | 63e3a3c429a91383 |
| mykernel/lib/list/double_list.h | func | INIT_LIST_S | 147–152 | 129 | 6503fe0d66ff1558 |
| mykernel/lib/list/double_list.h | func | INIT_LIST_HEADER_S | 153–158 | 136 | 53598975b6d1421a |
| mykernel/lib/list/double_list.h | func | __list_add_valid | 160–168 | 247 | 3ed8f4cedd0f9fda |
| mykernel/lib/list/double_list.h | func | __list_del_entry_valid | 169–179 | 306 | 13f263662c49e0ff |
| mykernel/lib/list/double_list.h | func | __list_add_between | 187–197 | 248 | 9147161d2d4ba947 |
| mykernel/lib/list/double_list.h | func | list_add_to_next | 206–210 | 127 | 1388a0b7eda6e30b |
| mykernel/lib/list/double_list.h | func | list_add_to_prev | 219–223 | 127 | 69a63f65475e8438 |
| mykernel/lib/list/double_list.h | func | __list_del | 233–238 | 133 | 4b2d41eef303900b |
| mykernel/lib/list/double_list.h | func | __list_del_entry | 253–260 | 162 | 1e6b8fa214378a31 |
| mykernel/lib/list/double_list.h | func | list_del_init | 278–283 | 120 | 69ca4826f39fa5d2 |
| mykernel/lib/list/double_list.h | func | list_is_head_anchor | 419–423 | 121 | 2115a38e9262a008 |
| mykernel/lib/list/double_list.h | func | list_is_empty_entry | 428–432 | 118 | a917ea3ed5156f20 |
| mykernel/lib/list/double_list.h | func | list_header_is_empty | 628–632 | 125 | 6d2bbba923422901 |
| mykernel/lib/list/double_list.h | func | list_header_contains | 633–642 | 212 | d2c885447e83747d |
| mykernel/lib/list/double_list.h | func | list_header_add_to_head | 644–649 | 157 | bdf1ccbe85075372 |
| mykernel/lib/list/double_list.h | func | list_header_remove_head | 650–660 | 245 | c36e35500b6e3970 |
| mykernel/lib/list/double_list.h | func | list_header_add_to_tail | 662–667 | 157 | e7451841f25dda30 |
| mykernel/lib/list/double_list.h | func | list_header_delete_node | 681–691 | 224 | 239d2d2bf75873c5 |
| mykernel/sched/scheduler/scheduler_types.h | struct | sched_entity | 8–40 | 811 | 2ae073a5d065956e |
| mykernel/sched/scheduler/scheduler_types.h | struct | sched_rt_entity | 42–58 | 454 | b5dd1d6f5523a96d |
| mykernel/arch/x86_64/sched/context/thread_info_types_arch.h | struct | thread_info | 19–24 | 189 | 848398c2f9f642ad |
| mykernel/sched/runqueue/runqueue_types.h | struct | myos_rq | 193–197 | 131 | 6470522644a0034b |
| mykernel/sched/task/task_const.h | macro | TASK_RUNNING | 26–26 | 34 | 1eb741bd7c9b5641 |
| mykernel/sched/task/task_const.h | macro | TASK_UNINTERRUPTIBLE | 28–28 | 40 | 160d93bd1d542e1b |
| mykernel/sched/scheduler/myos_rt.c | func | pick_next_task_myos | 5–58 | 1598 | 2ff36b570d0422f3 |

复用的 0d62 冻结文件（从提交对象逐字节解出，导入前核对）：

| 0d62 路径（results 根下） | 字节 | SHA-256[0] | 用途 |
|---|---|---|---|
| core/fixtures/fx_common.h | 1854 | 5c3b5baaeb652c16 | fixture header (copied next to the template) |
| core/fixtures/fx_list.inc.c | 3054 | 146d23b7aa2d1e4b | verbatim list primitives via //@@ORIG (copied next to the template) |
| core/fixtures/locate.py | 11783 | 2b89aff5cc0524fa | locator used by build2.Builder |
| core/fixtures/fx_sched.c | 10795 | 65c8f613d9854c93 | reference only: type choices; not compiled |
| core/recheck-01/fixtures/build2.py | 6681 | d3979aa307bbdb87 | Builder.expand, CFLAGS, COMPILE_LIMIT_S, RUN_LIMIT_S |
| core/recheck-01/fixtures/harness2.py | 7501 | 9b9c5c23698a13a1 | run_pg (bounded runs), blob/short12 of the time pin |

模板 fixtures/fx_order.c：{"bytes": 8846, "sha256_segments": ["05c6dfdfb1d459d7", "5380b805bc15e5d5", "ede472de08000a83", "41eec20b29b991dd"]}；展开后源码 observations/fx_order.expanded.c：{"bytes": 23713, "sha256_segments": ["716a2646484eb144", "32a736e4092a75ec", "70ccb3fc7b534641", "fd330f5833bb40f1"]}。编译：`gcc -std=gnu11 -O0 -g -fno-strict-aliasing -Wall -Wno-unused-label -Wno-unused-variable -Wno-unused-function`，退出码 0，超时 False，stderr 0 字节（无警告），二进制未入库。工具：`{"gcc": "gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0", "python3": "3.11.15", "git": "git version 2.43.0"}`。上限：{"compile_s": 60, "run_s": 5, "walk_links": 16}。

## 3. 夹具做了什么、没做什么

- host user-space process; pick_next_task_myos and the list primitives are verbatim from time a039d9803ade
- task_struct/runqueue reduced to the members the function touches (type choice of core/fixtures/fx_sched.c @ 0d62)
- current, need_resched() and jiffies are explicit fixture inputs, not a clock, an interrupt or preemption
- W07/W08: making the returned task current is a sequence model, not a context switch
- single CPU, no concurrent modification, time_slice=100, last_jiffies initial 100 set once per scenario
- observers only before/after each call; no probe inside the original function; queue walk by address, max 16 links
- 旧 fx_sched.c 的 pick() 在每次调用前执行 `rq->myos.last_jiffies = jiffies`，本夹具没有沿用：last_jiffies 每个场景只设一次初值 100，之后只由原函数更新。
- 任务节点全部真实分配、互不重复；current 不在队列中；current 不是 idle 时 idle 在队列中；current 是 idle 时 idle 不重复入队。没有删 idle，也没有用空 anchor 冒充任务。
- 遍历快照先与 anchor 比较，再按地址与五个已知节点比较；未知链接只报告、不转换，最多 16 个链接。

## 4. W01–W08 实际结果

队列写作“任务+vruntime”，I 为 idle。每组两次运行，下表来自第 1 次；第 2 次 stdout 与之逐字节相同（SHA-256 首段见“运行”列）。完整两次 stdout、stderr、退出码与终态见 observations/runs.json。

| W | 输入 | 步 | 调用前队列 | 返回 | 调用后队列 | 记账（实测/oracle） | last_jiffies 后 | P 非 idle / idle 尾 | 运行（rc、终态、stdout 字节/sha0）×2 |
|---|---|---|---|---|---|---|---|---|---|
| W01 | cur=A jf=100 nr=1 A=25 | 1 | B10 C20 D30 I0 | B | C20 D30 I0 A25 | {} / {} | 100 | VIOLATED / VIOLATED | 0,exited,1120/a756bcd6dc5a2d63; 0,exited,1120/a756bcd6dc5a2d63 |
| W02 | cur=A jf=110 nr=1 A=15 | 1 | B10 C20 D30 I0 | B | A25 C20 D30 I0 | {"A": 10} / {"A": 10} | 110 | VIOLATED / HOLDS | 0,exited,1121/c0f6589fbb0589bb; 0,exited,1121/c0f6589fbb0589bb |
| W03 | cur=A jf=100 nr=1 A=15 | 1 | B10 C20 D30 I0 | B | A15 C20 D30 I0 | {} / {} | 100 | HOLDS / HOLDS | 0,exited,1120/60bcc910f2a470f5; 0,exited,1120/60bcc910f2a470f5 |
| W04 | cur=A jf=105 nr=1 A=15 | 1 | B10 C20 D30 I0 | B | A20 C20 D30 I0 | {"A": 5} / {"A": 5} | 105 | HOLDS / HOLDS | 0,exited,1120/3c09117baf541e8c; 0,exited,1120/3c09117baf541e8c |
| W05 | cur=A jf=110 nr=1 A=15(UNINT) | 1 | B10 C20 D30 I0 | B | C20 D30 I0 | {"A": 10} / {"A": 10} | 110 | HOLDS / HOLDS | 0,exited,1114/cd71f83b1c54f937; 0,exited,1114/cd71f83b1c54f937 |
| W06 | cur=I jf=110 nr=1 A=15 I=7 | 1 | B10 C20 D30 | B | C20 D30 I7 | {} / {} | 110 | HOLDS / HOLDS | 0,exited,1106/107268ab5ee29e7c; 0,exited,1106/107268ab5ee29e7c |
| W07 | cur=A jf=110 nr=1 A=15 | 1 | B10 C20 D30 I0 | B | A25 C20 D30 I0 | {"A": 10} / {"A": 10} | 110 | VIOLATED / HOLDS | 0,exited,2147/aa5a43e8b0b4ec44; 0,exited,2147/aa5a43e8b0b4ec44 |
| W07 |  | 2 | A25 C20 D30 I0 | A | B10 C20 D30 I0 | {} / {} | 110 | HOLDS / HOLDS | 同上 |
| W08 | cur=A jf=105 nr=0 A=15 | 1 | B10 C20 D30 I0 | A | B10 C20 D30 I0 | {} / {} | 100 | HOLDS / HOLDS | 0,exited,2146/a39b0ff7d6fde1e5; 0,exited,2146/a39b0ff7d6fde1e5 |
| W08 |  | 2 | B10 C20 D30 I0 | B | A20 C20 D30 I0 | {"A": 5} / {"A": 5} | 105 | HOLDS / HOLDS | 同上 |

与主线推导逐项比较（[实测, 主线, 判定]）：

| W | 比较 | 全部相符 |
|---|---|---|
| W01 | {"selected": [[3], [3], "AGREES"], "final_order": [[4, 5, 0, 2], [4, 5, 0, 2], "possible", "AGREES"], "used": [[0], [0], "AGREES"], "P_nonidle": ["VIOLATED", "VIOLATED", "AGREES"]} | True |
| W02 | {"selected": [[3], [3], "AGREES"], "final_order": [[2, 4, 5, 0], [2, 4, 5, 0], "AGREES"], "P_nonidle": ["VIOLATED", "VIOLATED", "AGREES"], "a_final": [25, 25, "AGREES"]} | True |
| W03 | {"selected": [[3], [3], "AGREES"], "final_order": [[2, 4, 5, 0], [2, 4, 5, 0], "AGREES"], "P_nonidle": ["HOLDS", "HOLDS", "AGREES"], "a_final": [15, 15, "AGREES"]} | True |
| W04 | {"selected": [[3], [3], "AGREES"], "P_nonidle": ["HOLDS", "HOLDS", "AGREES"], "a_final": [20, 20, "AGREES"]} | True |
| W05 | {"selected": [[3], [3], "AGREES"], "a_final": [25, 25, "AGREES"], "a_requeued": [false, false, "AGREES"]} | True |
| W06 | {"selected": [[3], [3], "AGREES"], "final_order": [[4, 5, 0], [4, 5, 0], "AGREES"], "idle_final": [7, 7, "AGREES"]} | True |
| W07 | {"step1_a_delta": [10, 10, "AGREES"], "step2_switched_out_delta": [0, 0, "AGREES"], "total_charged": [10, 10, "AGREES"]} | True |
| W08 | {"step2_a_delta": [5, 5, "AGREES"], "step1_unchanged": [true, true, "AGREES"], "total_charged": [5, 5, "AGREES"]} | True |

第 1 次运行的原始 stdout（第 2 次相同）：

<details><summary>W01</summary>

```json
{"case":"W01","ev":"scenario","time_slice":100,"last_jiffies_initial":100,"jiffies":100,"need_resched":1,"current":2,"a_state":0,"a_vruntime":25,"idle_vruntime":0}
{"case":"W01","ev":"snap","where":"before","step":1,"jiffies":100,"last_jiffies":100,"used_external":0,"need_resched":1,"current":{"id":2,"state":0,"vruntime":25,"time_slice":100},"rq_curr":2,"rq_idle":0,"tasks":[[0,0,0],[2,25,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[3,10],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":0}
{"case":"W01","ev":"call","step":1,"returned":3,"rq_curr":3}
{"case":"W01","ev":"snap","where":"after","step":1,"jiffies":100,"last_jiffies":100,"used_external":0,"need_resched":1,"current":{"id":2,"state":0,"vruntime":25,"time_slice":100},"rq_curr":3,"rq_idle":0,"tasks":[[0,0,0],[2,25,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[4,20],[5,30],[0,0],[2,25]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":1}
{"case":"W01","ev":"end","steps":1}
```

</details>

<details><summary>W02</summary>

```json
{"case":"W02","ev":"scenario","time_slice":100,"last_jiffies_initial":100,"jiffies":110,"need_resched":1,"current":2,"a_state":0,"a_vruntime":15,"idle_vruntime":0}
{"case":"W02","ev":"snap","where":"before","step":1,"jiffies":110,"last_jiffies":100,"used_external":10,"need_resched":1,"current":{"id":2,"state":0,"vruntime":15,"time_slice":100},"rq_curr":2,"rq_idle":0,"tasks":[[0,0,0],[2,15,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[3,10],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":0}
{"case":"W02","ev":"call","step":1,"returned":3,"rq_curr":3}
{"case":"W02","ev":"snap","where":"after","step":1,"jiffies":110,"last_jiffies":110,"used_external":0,"need_resched":1,"current":{"id":2,"state":0,"vruntime":25,"time_slice":100},"rq_curr":3,"rq_idle":0,"tasks":[[0,0,0],[2,25,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[2,25],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":1}
{"case":"W02","ev":"end","steps":1}
```

</details>

<details><summary>W03</summary>

```json
{"case":"W03","ev":"scenario","time_slice":100,"last_jiffies_initial":100,"jiffies":100,"need_resched":1,"current":2,"a_state":0,"a_vruntime":15,"idle_vruntime":0}
{"case":"W03","ev":"snap","where":"before","step":1,"jiffies":100,"last_jiffies":100,"used_external":0,"need_resched":1,"current":{"id":2,"state":0,"vruntime":15,"time_slice":100},"rq_curr":2,"rq_idle":0,"tasks":[[0,0,0],[2,15,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[3,10],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":0}
{"case":"W03","ev":"call","step":1,"returned":3,"rq_curr":3}
{"case":"W03","ev":"snap","where":"after","step":1,"jiffies":100,"last_jiffies":100,"used_external":0,"need_resched":1,"current":{"id":2,"state":0,"vruntime":15,"time_slice":100},"rq_curr":3,"rq_idle":0,"tasks":[[0,0,0],[2,15,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[2,15],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":1}
{"case":"W03","ev":"end","steps":1}
```

</details>

<details><summary>W04</summary>

```json
{"case":"W04","ev":"scenario","time_slice":100,"last_jiffies_initial":100,"jiffies":105,"need_resched":1,"current":2,"a_state":0,"a_vruntime":15,"idle_vruntime":0}
{"case":"W04","ev":"snap","where":"before","step":1,"jiffies":105,"last_jiffies":100,"used_external":5,"need_resched":1,"current":{"id":2,"state":0,"vruntime":15,"time_slice":100},"rq_curr":2,"rq_idle":0,"tasks":[[0,0,0],[2,15,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[3,10],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":0}
{"case":"W04","ev":"call","step":1,"returned":3,"rq_curr":3}
{"case":"W04","ev":"snap","where":"after","step":1,"jiffies":105,"last_jiffies":105,"used_external":0,"need_resched":1,"current":{"id":2,"state":0,"vruntime":20,"time_slice":100},"rq_curr":3,"rq_idle":0,"tasks":[[0,0,0],[2,20,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[2,20],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":1}
{"case":"W04","ev":"end","steps":1}
```

</details>

<details><summary>W05</summary>

```json
{"case":"W05","ev":"scenario","time_slice":100,"last_jiffies_initial":100,"jiffies":110,"need_resched":1,"current":2,"a_state":2,"a_vruntime":15,"idle_vruntime":0}
{"case":"W05","ev":"snap","where":"before","step":1,"jiffies":110,"last_jiffies":100,"used_external":10,"need_resched":1,"current":{"id":2,"state":2,"vruntime":15,"time_slice":100},"rq_curr":2,"rq_idle":0,"tasks":[[0,0,0],[2,15,2],[3,10,0],[4,20,0],[5,30,0]],"queue":[[3,10],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":0}
{"case":"W05","ev":"call","step":1,"returned":3,"rq_curr":3}
{"case":"W05","ev":"snap","where":"after","step":1,"jiffies":110,"last_jiffies":110,"used_external":0,"need_resched":1,"current":{"id":2,"state":2,"vruntime":25,"time_slice":100},"rq_curr":3,"rq_idle":0,"tasks":[[0,0,0],[2,25,2],[3,10,0],[4,20,0],[5,30,0]],"queue":[[4,20],[5,30],[0,0]],"count":3,"walk_len":3,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":0}
{"case":"W05","ev":"end","steps":1}
```

</details>

<details><summary>W06</summary>

```json
{"case":"W06","ev":"scenario","time_slice":100,"last_jiffies_initial":100,"jiffies":110,"need_resched":1,"current":0,"a_state":0,"a_vruntime":15,"idle_vruntime":7}
{"case":"W06","ev":"snap","where":"before","step":1,"jiffies":110,"last_jiffies":100,"used_external":10,"need_resched":1,"current":{"id":0,"state":0,"vruntime":7,"time_slice":100},"rq_curr":0,"rq_idle":0,"tasks":[[0,7,0],[2,15,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[3,10],[4,20],[5,30]],"count":3,"walk_len":3,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":0,"current_occurrences":0}
{"case":"W06","ev":"call","step":1,"returned":3,"rq_curr":3}
{"case":"W06","ev":"snap","where":"after","step":1,"jiffies":110,"last_jiffies":110,"used_external":0,"need_resched":1,"current":{"id":0,"state":0,"vruntime":7,"time_slice":100},"rq_curr":3,"rq_idle":0,"tasks":[[0,7,0],[2,15,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[4,20],[5,30],[0,7]],"count":3,"walk_len":3,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":1}
{"case":"W06","ev":"end","steps":1}
```

</details>

<details><summary>W07</summary>

```json
{"case":"W07","ev":"scenario","time_slice":100,"last_jiffies_initial":100,"jiffies":110,"need_resched":1,"current":2,"a_state":0,"a_vruntime":15,"idle_vruntime":0}
{"case":"W07","ev":"snap","where":"before","step":1,"jiffies":110,"last_jiffies":100,"used_external":10,"need_resched":1,"current":{"id":2,"state":0,"vruntime":15,"time_slice":100},"rq_curr":2,"rq_idle":0,"tasks":[[0,0,0],[2,15,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[3,10],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":0}
{"case":"W07","ev":"call","step":1,"returned":3,"rq_curr":3}
{"case":"W07","ev":"snap","where":"after","step":1,"jiffies":110,"last_jiffies":110,"used_external":0,"need_resched":1,"current":{"id":2,"state":0,"vruntime":25,"time_slice":100},"rq_curr":3,"rq_idle":0,"tasks":[[0,0,0],[2,25,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[2,25],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":1}
{"case":"W07","ev":"sequence","before_step":2,"current_set_to_returned":3,"jiffies":110,"need_resched":1}
{"case":"W07","ev":"snap","where":"before","step":2,"jiffies":110,"last_jiffies":110,"used_external":0,"need_resched":1,"current":{"id":3,"state":0,"vruntime":10,"time_slice":100},"rq_curr":3,"rq_idle":0,"tasks":[[0,0,0],[2,25,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[2,25],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":0}
{"case":"W07","ev":"call","step":2,"returned":2,"rq_curr":2}
{"case":"W07","ev":"snap","where":"after","step":2,"jiffies":110,"last_jiffies":110,"used_external":0,"need_resched":1,"current":{"id":3,"state":0,"vruntime":10,"time_slice":100},"rq_curr":2,"rq_idle":0,"tasks":[[0,0,0],[2,25,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[3,10],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":1}
{"case":"W07","ev":"end","steps":2}
```

</details>

<details><summary>W08</summary>

```json
{"case":"W08","ev":"scenario","time_slice":100,"last_jiffies_initial":100,"jiffies":105,"need_resched":0,"current":2,"a_state":0,"a_vruntime":15,"idle_vruntime":0}
{"case":"W08","ev":"snap","where":"before","step":1,"jiffies":105,"last_jiffies":100,"used_external":5,"need_resched":0,"current":{"id":2,"state":0,"vruntime":15,"time_slice":100},"rq_curr":2,"rq_idle":0,"tasks":[[0,0,0],[2,15,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[3,10],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":0}
{"case":"W08","ev":"call","step":1,"returned":2,"rq_curr":2}
{"case":"W08","ev":"snap","where":"after","step":1,"jiffies":105,"last_jiffies":100,"used_external":5,"need_resched":0,"current":{"id":2,"state":0,"vruntime":15,"time_slice":100},"rq_curr":2,"rq_idle":0,"tasks":[[0,0,0],[2,15,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[3,10],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":0}
{"case":"W08","ev":"sequence","before_step":2,"current_set_to_returned":2,"jiffies":105,"need_resched":1}
{"case":"W08","ev":"snap","where":"before","step":2,"jiffies":105,"last_jiffies":100,"used_external":5,"need_resched":1,"current":{"id":2,"state":0,"vruntime":15,"time_slice":100},"rq_curr":2,"rq_idle":0,"tasks":[[0,0,0],[2,15,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[3,10],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":0}
{"case":"W08","ev":"call","step":2,"returned":3,"rq_curr":3}
{"case":"W08","ev":"snap","where":"after","step":2,"jiffies":105,"last_jiffies":105,"used_external":0,"need_resched":1,"current":{"id":2,"state":0,"vruntime":20,"time_slice":100},"rq_curr":3,"rq_idle":0,"tasks":[[0,0,0],[2,20,0],[3,10,0],[4,20,0],[5,30,0]],"queue":[[2,20],[4,20],[5,30],[0,0]],"count":4,"walk_len":4,"walk_complete":true,"unknown_link":false,"duplicate_nodes":false,"idle_occurrences":1,"current_occurrences":1}
{"case":"W08","ev":"end","steps":2}
```

</details>

## 5. 判读

每组先核记录完整（两次都退出 0、未超时、exited、已回收、stderr 为空、stdout 相同、必需事件与字段齐全），再由独立的值列表 oracle 按声明输入计算：是否切换、选中队首、调用后成员、各任务 vruntime 增量、last_jiffies、总计费。oracle 不复制原函数的指针遍历与计费次序，只对实测队列检验候选约束 P。

- 结构 True，记账与 oracle 一致 True，选择一致 True；输入保真全部成立。
- 候选 P：`{"W01": "VIOLATED", "W02": "VIOLATED", "W03": "SATISFIED", "W04": "SATISFIED", "W05": "SATISFIED", "W06": "SATISFIED", "W07": "VIOLATED", "W08": "SATISFIED"}`；违反的步：`{"W01": {"1": ["VIOLATED", "VIOLATED"]}, "W02": {"1": ["VIOLATED", "HOLDS"]}, "W07": {"1": ["VIOLATED", "HOLDS"]}}`。
- 主线推导全部相符=True，不相符项 `{}`。
- 选中任务不是队列最小键的步：`[["W07", 2, 2, [4, 20]]]`（W07 第 2 步：因第 1 步的失序，A25 位于队首，被选在 C20 之前）。

**为什么只修游标可能不够。** W02 实测：插入时 A 的键为 15，取走 B 后的队首是 C20，A 被放在第 0 位，随后才记入 10。按 myos_rt.c 第 37–50 行，键 15 不大于 20，插入循环一次也不进入，A 直接放在队首；运行时间在插入之后才加上。只改循环内部（让比较对象随游标移动）改变不了这种情形，它只影响 W01 那种循环一直走到 anchor 的情形。这是源码阅读加实测边界值，没有运行任何改动后的函数。

另一条源码推断（未执行）：循环先比较 vruntime，再检查 `tmp_list != anchor`。现在比较对象一直停在首节点，所以 W01 走到 anchor 时没有非法读取。如果改成每步从 tmp_list 重新取比较对象，却不调换这两个条件的顺序，走到 anchor 时就会先按 anchor 计算一个并不存在的容器并读取它。

## 6. 三项消费正负控

| 控制 | 变造 | 证据状态 | P 非 idle | 与主线 P 判定 | 满足 |
|---|---|---|---|---|---|
| C1_complete_W03 | 无（真实 W03 记录） | VALID | HOLDS | ["HOLDS", "HOLDS", "AGREES"] | True |
| C2_after_snapshot_removed | HARNESS_META_TEST: the snap/after/1 line removed from both runs' stdout | INCOMPLETE_EVIDENCE | None | null | True |
| C3_complete_but_different | HARNESS_META_TEST: in both runs the snap/after/1 queue reordered to C20,A15,D30,I (same members, count, keys) | VALID | VIOLATED | ["VIOLATED", "HOLDS", "DIFFERS"] | True |

变造只作用于内存中的副本，没有写回 runs.json。

## 7. 发现与限制

- 见证完成不等于内核正确：本轮只说明这个函数在八组有限输入下的边界行为。
- 游标候选（W01）：比较对象停在首节点 C20，游标却一路前进到 anchor，A25 落在 D30 与 idle 之后、队尾；P 的非 idle 条件与 idle 尾部条件都被破坏。
- 计费时点候选（W02、W07 第 1 步）：回插按计费前的键定位，计费在回插之后，得到 A25 在 C20 之前；与游标无关。
- 相等键（W04）与零增量（W03）都满足 P；阻塞任务（W05）不回队但仍计费 10；idle（W06）回到队尾且不计费；W07 第 2 步没有重复计费；W08 第一次不切换时队列、vruntime、last_jiffies 都不变，第二次才记入 5。
- W07 第 2 步显示失序的后果：下一次选择取到 A25 而不是更小的 C20。
- 全局可达性（CA-02）、真实时钟、IRQ、SMP、上下文切换都未执行；P 是 15 号提出的候选约束，不是 Owner 已选的政策；没有修改内核，也没有生成补丁。
- 执行模型未知；没有 CI 或人工逐行复核。

## 8. 过程记录

- 正式运行前在会话临时目录做过一次开发构建与运行，用于编写判读程序；正式记录是之后在全新目录中的一次构建与两次运行。
- 判读程序在开发后加入了逐步 P 与“选中是否为最小键”的观察项；夹具模板在两次运行之间没有改动（runs.json 记录的模板 SHA-256 与提交的 fx_order.c 相同，由 final_check4 核对）。
- 没有运行原仓库脚本、旧的 run_all/run_recheck/meta_tests 主入口、H00 或全树扫描。

## 9. 提交、推送与回读

结果批 `5078686e8267`，提交前运行 `final_check4.py results`（§9.1）。推送输出（手工转录）：

```text
To https://github.com/08822407d/MyOS2
   0d62c4d..5078686  claude/dazzling-cori-q0dnyt -> claude/dazzling-cori-q0dnyt
```

```text
$ cd <scratch>/rbtool    # 0d62 的 recheck-01/fixtures/readback2.py（SHA-256[0] 41019aeb61216b01）与 harness2.py（9b9c5c23698a13a1）
$ python3 readback2.py 5078686e8267 <scheduler-order-02/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode object --out <scratch>/readback/readback_results.object.json
readback object exit=0
$ python3 readback2.py 5078686e8267 <scheduler-order-02/results.yaml> packet_id=MYOS2-LEAD-002-CORE-CHECK-01 --mode branch --out <scratch>/readback/readback_results.branch.json
readback branch exit=0
$ python3 guard4.py <scratch>/readback/guard_after_push.json
guard exit=0
$ cp <scratch>/readback/*.json ../observations/after_results/   (cmp 逐字节相同)
```

- object：ok=True，分支头关系=不适用（object 模式按提交读取），期望 36524 字节；通道 `[["raw.githubusercontent.com", "200", 0, false, 36524, true, true], ["api.github.com_contents_raw", "200", 0, false, 36524, true, true]]`
- branch：ok=True，分支头关系=equal，期望 36524 字节；通道 `[["raw.githubusercontent.com", "200", 0, false, 36524, true, true], ["api.github.com_contents_raw", "200", 0, false, 36524, true, true]]`
- 推送后保护门：gate.ok=`True`，执行=`{"branch": "claude/dazzling-cori-q0dnyt", "head_short12": "5078686e8267", "remote_relation": "equal"}`，相对 0d62 的已提交变化 16 项，全部是 scheduler-order-02/ 下的新增。

文档批（本文件、MANIFEST.md、build_evidence4.py、after_results/ 三份记录）在其后提交；提交前运行 `final_check4.py docs 5078686e8267`（§9.2）。

### 9.1 结果批提交前的检查

```json
{
 "check": "FINAL_SCOPE_SCHEDULER_ORDER_02",
 "mode": "results",
 "branch": "claude/dazzling-cori-q0dnyt",
 "head_short12": "0d62c4d19711",
 "remote_branch_equals_head": true,
 "remote_master_equals_pin": true,
 "diff_base": "merge-base(HEAD, remote master) = de3bb1df906a",
 "index_vs_base_count": 100,
 "outside_allowed_dirs": [],
 "non_added_vs_base": [],
 "since_0d62_not_addition_under_prefix": [],
 "worktree_outside_prefix": [],
 "prefix_unstaged_modifications": [],
 "prefix_untracked": [],
 "prefix_files_staged": [
  "fixtures/common4.py",
  "fixtures/controls4.py",
  "fixtures/evaluate_order.py",
  "fixtures/final_check4.py",
  "fixtures/frozen_0d62.py",
  "fixtures/fx_order.c",
  "fixtures/guard4.py",
  "fixtures/make_results4.py",
  "fixtures/run_order.py",
  "observations/controls.json",
  "observations/frozen_0d62_manifest.json",
  "observations/fx_order.expanded.c",
  "observations/guard_pre_results.json",
  "observations/guard_start.json",
  "observations/runs.json",
  "results.yaml"
 ],
 "new_or_changed_vs_head": [
  "A\tfixtures/common4.py",
  "A\tfixtures/controls4.py",
  "A\tfixtures/evaluate_order.py",
  "A\tfixtures/final_check4.py",
  "A\tfixtures/frozen_0d62.py",
  "A\tfixtures/fx_order.c",
  "A\tfixtures/guard4.py",
  "A\tfixtures/make_results4.py",
  "A\tfixtures/run_order.py",
  "A\tobservations/controls.json",
  "A\tobservations/frozen_0d62_manifest.json",
  "A\tobservations/fx_order.expanded.c",
  "A\tobservations/guard_pre_results.json",
  "A\tobservations/guard_start.json",
  "A\tobservations/runs.json",
  "A\tresults.yaml"
 ],
 "binary_like": [],
 "hex40_hits": {},
 "secret_pattern_hits": [],
 "json_parse_failures": [],
 "python_syntax_failures": [],
 "python_without_provenance_header": [],
 "c_template_has_provenance_header": true,
 "results_yaml_reproduced_from_observations": true,
 "expanded_source_matches_runs_record": true,
 "template_matches_runs_record": true,
 "results_yaml": {
  "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
  "followup_id": "CORE-SCHED-ORDER-02",
  "scenarios_completed": 8
 },
 "all_as_expected": true
}
```

### 9.2 文档批提交前的检查

```json
{
 "check": "FINAL_SCOPE_SCHEDULER_ORDER_02",
 "mode": "docs",
 "branch": "claude/dazzling-cori-q0dnyt",
 "head_short12": "5078686e8267",
 "remote_branch_equals_head": true,
 "remote_master_equals_pin": true,
 "diff_base": "merge-base(HEAD, remote master) = de3bb1df906a",
 "index_vs_base_count": 106,
 "outside_allowed_dirs": [],
 "non_added_vs_base": [],
 "since_0d62_not_addition_under_prefix": [],
 "worktree_outside_prefix": [],
 "prefix_unstaged_modifications": [],
 "prefix_untracked": [],
 "prefix_files_staged": [
  "MANIFEST.md",
  "evidence.md",
  "fixtures/build_evidence4.py",
  "fixtures/common4.py",
  "fixtures/controls4.py",
  "fixtures/evaluate_order.py",
  "fixtures/final_check4.py",
  "fixtures/frozen_0d62.py",
  "fixtures/fx_order.c",
  "fixtures/guard4.py",
  "fixtures/make_results4.py",
  "fixtures/run_order.py",
  "observations/after_results/guard_after_push.json",
  "observations/after_results/readback_results.branch.json",
  "observations/after_results/readback_results.object.json",
  "observations/controls.json",
  "observations/frozen_0d62_manifest.json",
  "observations/fx_order.expanded.c",
  "observations/guard_pre_results.json",
  "observations/guard_start.json",
  "observations/runs.json",
  "results.yaml"
 ],
 "new_or_changed_vs_head": [
  "A\tMANIFEST.md",
  "A\tevidence.md",
  "A\tfixtures/build_evidence4.py",
  "A\tobservations/after_results/guard_after_push.json",
  "A\tobservations/after_results/readback_results.branch.json",
  "A\tobservations/after_results/readback_results.object.json"
 ],
 "binary_like": [],
 "hex40_hits": {},
 "secret_pattern_hits": [],
 "json_parse_failures": [],
 "python_syntax_failures": [],
 "python_without_provenance_header": [],
 "c_template_has_provenance_header": true,
 "results_yaml_reproduced_from_observations": true,
 "expanded_source_matches_runs_record": true,
 "template_matches_runs_record": true,
 "results_yaml": {
  "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
  "followup_id": "CORE-SCHED-ORDER-02",
  "scenarios_completed": 8
 },
 "results_commit": "5078686e8267",
 "results_yaml_changed_since_results_commit": false,
 "non_added_since_results_commit": [],
 "docs": {
  "MANIFEST.md": {
   "yaml_parses": true,
   "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
   "verified_tag_count": 0,
   "other_pass_token_lines": []
  },
  "evidence.md": {
   "yaml_parses": true,
   "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
   "verified_tag_count": 0,
   "other_pass_token_lines": []
  },
  "results.yaml": {
   "yaml_parses": true,
   "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
   "verified_tag_count": 0,
   "other_pass_token_lines": []
  }
 },
 "manifest_self_check_consistent": true,
 "manifest_data_summary_mismatches": [],
 "manifest_file_list_matches_staged": true,
 "evidence_results_commit_matches": true,
 "all_as_expected": true
}
```

