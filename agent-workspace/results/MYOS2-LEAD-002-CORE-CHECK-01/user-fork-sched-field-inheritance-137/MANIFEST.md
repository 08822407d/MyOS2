---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-FORK-SCHED-FIELD-INHERITANCE-137"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-21"
record_type: "phase2_executor_manifest"
date: "2026-11-23"
phase: 2
produced_by: "GPT-6（文本模型族；后端未认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
actual_backend_model: UNATTESTED
actual_backend_effort: UNATTESTED
base_snapshot: "time（分支名）"
read_channel: connector
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
self_check: {"verified_claims":73,"quotes_reconfirmed":73,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
write_retry_log: "not_needed"
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
owner_decisions_requested_now: []
open_questions: []
---
# 第137批 · 固定输入、G00、成果与验收边界

**作者静态交付已准备，待G04远端确认；mainline_acceptance=PENDING**。保持MyOS2阶段2，原执行分支claude/dazzling-cori-q0dnyt及原Draft PR17。此清单不预填尚未发生的提交与正式评论。只有六件纯新增，旧136原件以及原135等历史记录均不改。

## G00 新核事实

```json
{
  "direct_refs_before": {
    "lead": "0b6cffc26e2d",
    "exec": "95b6af84d8f6",
    "time": "a039d9803ade",
    "master": "e8e60cd19540"
  },
  "open_PR_page_lengths": [
    2,
    0
  ],
  "PR16": "Open/Ready, head=agent/MYOS2-LEAD-002 to master, unmerged",
  "PR17": "Open/Draft, head=claude/dazzling-cori-q0dnyt to master, unmerged; original PR title preserved",
  "PR17_top_comments_page_lengths": [
    100,
    100,
    62,
    0
  ],
  "PR17_comment_unique_ids": 262,
  "author136_receipt_id": 7096187292,
  "author136_receipt_read_twice": true,
  "author136_full_marker_unique": true,
  "C09_137_contract_receipt_id": 7096722711,
  "C09_both_136_review_and_137_publication_markers_unique": true,
  "prior136": {
    "parent_short12": "d01efe19ccec",
    "head_short12": "95b6af84d8f6",
    "compare": "ahead1 behind0, six added, zero modified/deleted",
    "lines": 3506,
    "bytes": 116476,
    "fixed_active_fulltext_equal": 6,
    "git_blob_SHA1_recomputed_match": 6
  },
  "lead_release": {
    "short12": "0b6cffc26e2d",
    "parent_short12": "9d333c5959a8",
    "one_commit_four_added": true
  },
  "canaries": {
    "time_exact_hit": [
      true,
      true
    ],
    "master_exact_miss": [
      true,
      true
    ]
  },
  "startup_convention_direct_master": true,
  "new137_directory_before": "404 expected",
  "new137_formal_author_marker_count_before": 0,
  "expected_exec_before": "95b6af84d8f6",
  "source_read_method": "connector",
  "full_local_P9": "NOT_RUN"
}
```

## 交接与适用指导的边界

同一会话同一作者身份`MYOS2-LEAD-002-CHAT-EXEC-21`：136既有交接接收及Mnemosyne行为指导刷新，真实记录在冻结136 MANIFEST；**137没有重新执行接收或刷新**，本轮只新增自己对适用范围的补正记录。`task_local_only=true`取自188/00明确要求，适用于MyOS2执行线，不会导入Mnemosyne的项目维护/部署任务或写权限。135原作者是C08c/EXEC-20；136补正作者及137新增作者是C09a/EXEC-21。C09用Pro作独立验收。

## 严格六件清单

| 文件 | UTF-8字节 | 行数 | 内容 |
|---|---:|---:|---|
| MANIFEST.md | 正式作者评论列准确值 | 正式作者评论列准确值 | 本G00、P3/P6启动、输入及自检 |
| fork-sched-field-inheritance.md | 10307 | 102 | D00 and A/B/C Chinese report |
| facts-and-boundaries.yaml | 76550 | 2900 | four D00 corrections, A/B/C models, 93 nodes 96 typed edges |
| fork-sched-field-inheritance-paths.dot | 15182 | 193 | same IDs/kind/label projection |
| deferred-findings.yaml | 4975 | 119 | deferred risks/remaining unknowns |
| evidence.md | 28776 | 813 | 73 new formal windows and 38 old mapping |

机器件事实/图 **93节点、96条唯一类型方向边**；DOT与facts按ID、label、kind和端点应相等。引文73条均为本批活动定义体窗口，旧136 E001–038 的来源、映射与失效分类另在evidence及facts登记；旧014拆注释、旧022/023纯注释不作为本批正式窗。完整本地P9未执行，不冒称编译运行。

## 本批D00与A/B/C责任

D00：R136-01确认公约及分支正反金丝雀、范围与作者；R136-02 38旧窗重映、73新窗、CONFIG_BUG及列表定义条件；R136-03 B四次合法链六谓词各为假、拒绝分支跳过，need_resched初值不强加；R136-04真实3>=10假、C1 K仍TASK_RUNNING、C2旧键与anchor短路。本四项均只是作者`CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW`。

A字段来源：整结构复制P到子，子链指针复制瞬间仍指P节点；后续CPU三时间、vruntime、状态、on_cpu和节点指针按活动代码重置。时间片与切换计数选定路径不清零。B题设P slice 4→9→12，K继承4、J继承9，最终队首J K Q R,count4,rq.curr=P。C独立used=6，K取Q且切换计数上升、J继续J且跳过外层切换分支。clock与jiffies两个来源分开；context_switch只到调用边界。

## P9及运行禁令

```json
{
  "P9_1": "no 40hex across six (before write confirmation)",
  "P9_2": "new windows 73/73 from exact physical time definition bodies, local command NOT_RUN",
  "P9_3": "listed source and old files exist via GitHub reads",
  "P9_4": "all given physical line ranges <= file lengths",
  "P9_5": "73 formal labels, 73 reaffirmed, 0 downgraded",
  "P9_6": "no external claims",
  "P9_7": "two time quotes positive direct master negative",
  "P9_8": "master convention §1.2 startup quote and read_channel present",
  "P9_9": "N/A; no generated kernel implementation code",
  "P9_10": "N/A; no patch",
  "P9_11": "restricted certainty phrases checked 0",
  "P9_12": "N/A; no chat downgrade",
  "P9_13": "D00 plus A/B/C all mandatory symbol anchors covered",
  "full_local_P9": "NOT_RUN",
  "preprocess_compile_link": "NOT_RUN",
  "repo_script_or_program": "NOT_RUN",
  "Graphviz": "NOT_RUN",
  "QEMU": "NOT_RUN",
  "real_process_or_hardware": "NOT_RUN"
}
```

完整本地P9、预处理、编译链接、仓库程序与脚本、Graphviz、QEMU、实机、真实进程、PID、时钟、调度、文件操作均NOT_RUN。没有补丁、没有源码修改，也没有向其它项目写入。未知的全局time_slice及on_rq完整写者仍按R136-G04延期。

## 本任务输入与来源

- `agent-workspace/README.md`
- `agent-workspace/conventions.md`
- `agent-workspace/tasks/00-gpt-task-protocol-v2.md`
- `agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md`
- `agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md`
- `agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md`
- `agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md`
- `agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md`
- `agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md`
- `agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md`
- `agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md`
- `agent-workspace/lead/MYOS2-LEAD-002/182-continuation/03-C08-to-C09-handoff-CONFIRMED.md`
- `agent-workspace/lead/MYOS2-LEAD-002/188-user-new-task-runqueue-entry-contract.md`
- `agent-workspace/lead/MYOS2-LEAD-002/188-executor-continuation/00-C08c-to-C09a-handoff-package.md`
- `agent-workspace/lead/MYOS2-LEAD-002/189-user-fork-sched-field-inheritance-contract.md`
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-review.md`
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136-D00-135-review.md`
- `agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-new-task136-reviewed-fork-fields137.md`
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TASK-STORAGE-LIFETIME-74-review.md`
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-YIELD-TASK-SELECTION-132-review.md`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/MANIFEST.md`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry.md`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/facts-and-boundaries.yaml`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/new-task-runqueue-entry-paths.dot`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/deferred-findings.yaml`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/evidence.md`
- `mykernel/sched/task/pid.c`
- `mykernel/sched/forkexec/fork.c`
- `mykernel/lock_IPC/signal/signal.h`
- `mykernel/arch/x86_64/sched/context/process_arch.c`
- `mykernel/sched/scheduler/scheduler_core.c`
- `mykernel/lib/list/double_list.h`
- `mykernel/sched/task/task_const.h`
- `mykernel/sched/scheduler/myos_rt.c`
- `mykernel/sched/task/task_types.h`
- `mykernel/sched/scheduler/scheduler_types.h`
- `mykernel/sched/runqueue/runqueue_types.h`
- `mykernel/sched/scheduler/scheduler.h`
- `mykernel/lib/list/rcu_list_macro.h`
- `mykernel/include/linux/kernel/asm-generic/bug.h`
- `mykernel/scripts/options_flags.cmake`
- `mykernel/lib/list/double_list.c`
- `mykernel/debug/panic.c`

## 远端操作边界

仅在 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-sched-field-inheritance-137/` 新增此六件。先固定六件同一单父快进再比对固定/活动UTF-8、字节与Git Blob；确认PR17无since评论全分页无重复后发布唯一正式作者回执，再按评论ID双读及全页查唯一、四refs和两PR和固定/活动六件完成G04。**正式评论后不再写任何文件或第二条评论**；G04成功即STOP_WRITING，交原C09主线使用Pro独审并同轮安排下一批。
