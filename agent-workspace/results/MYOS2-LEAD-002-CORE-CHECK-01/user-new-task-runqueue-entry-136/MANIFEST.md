---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-21
record_type: manifest
date: 2026-11-23
produced_by: GPT-6（后端未认证）
model_per_owner: GPT-6＋Extra（Owner指定）
base_snapshot: time（分支名）
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
self_check:
  verified_claims: 38
  quotes_reconfirmed: 38
  downgraded_to_inferred: 0
read_channel: connector
---

# 第136批作者清单 / 接收、指导刷新及fresh G00

六件只写原执行分支的指定新目录；状态 **READY_FOR_MAINLINE_REVIEW**，主线 **PENDING**。本文件在G04前生成，不会提前宣称已提交或评论。以下接收、指导刷新是两个独立操作记录。

## mnemosyne_handoff_receive

```json
{
  "package_id": "MYOS2-A-C08c-TO-C09a-136-20261123",
  "package_present": true,
  "package_status": "non_execution_source_transfer_artifact",
  "receiver_guidance_load": {
    "project_guidance": "required",
    "mnemosyne_guidance": "yes",
    "refresh_completed": true
  },
  "execution_source": "Mnemosyne current/human-approved-spec.md (for Mnemosyne behavior only); MyOS2 Owner/188 governs task",
  "receiving_model_tier_declared": "next_tier GPT-6+Extra",
  "receiving_model_tier_actual": "unknown backend/UI effort; GPT-6 family",
  "re_tier_on_arrival": "adequate for bounded execution, Pro required for mainline independent acceptance",
  "evidence_paths_checked": [
    "188/00",
    "188/01",
    "188 contract",
    "135 review and six original files",
    "182/03",
    "PR17 formal records",
    "four refs and two PRs"
  ],
  "missing_or_unchecked": [],
  "current_task_from_package": "CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136",
  "forbidden_actions": [
    "kernel edit",
    "run",
    "new PR or branch",
    "force-push",
    "mainline independent acceptance"
  ],
  "safe_next_action": "fresh G00 then append six only"
}
```

## mnemosyne_guidance_refresh

```json
{
  "operation": "behavior_constraint_refresh",
  "current_conversation_task_preserved": true,
  "handoff_started": false,
  "maintenance_live_route_imported": false,
  "auto_handoff_detection_performed": false,
  "execution_source": "Mnemosyne current/human-approved-spec.md",
  "mnemosyne_master_short12": "e581eb9df6c4",
  "guards_loaded_this_refresh": {
    "core": [
      "README.md",
      "current/human-approved-spec.md",
      "current/user-operation-next-step-capability-and-intent-guard.md",
      "commands/load-mnemosyne-guidance.md"
    ],
    "conditional_triggered": [
      "current/github-single-active-pr-lineage-guard.md",
      "current/run-context-and-pr-provenance-guard.md",
      "current/artifact-delivery-and-direct-generation-guard.md",
      "current/cross-conversation-execution-intent-and-operator-flow-guard.md",
      "current/external-research-display-name-guard.md",
      "current/source-artifact-preservation-and-design-rationale-guard.md",
      "current/model-tier-and-subtask-cost-routing-guard.md",
      "current/next-step-repository-write-visibility-guard.md",
      "current/agent-product-ready-pr-and-frontier-efficiency-guard.md"
    ]
  },
  "applied_constraints": [
    "task_preservation",
    "objective_evidence",
    "single_open_PR",
    "no_source_rewrite",
    "source_quote_guard",
    "independent_review",
    "user_operation_separation",
    "repository_write_visibility"
  ],
  "record_location": "this MANIFEST; no additional file permitted",
  "Mnemosyne_maintenance_import": false
}
```

## G00

```json
{
  "refs_before": {
    "lead": "9d333c5959a8",
    "exec": "d01efe19ccec",
    "time": "a039d9803ade",
    "master": "e8e60cd19540"
  },
  "PR16": "Open/Ready",
  "PR17": "Open/Draft",
  "only_open_PRs": "16 and 17 to master; both unmerged",
  "PR17_comment_pages": "100/100/60/0;260 unique",
  "old_author_comment": "7095153852 read twice, unique",
  "C09_release_comment": "7095602435 three markers",
  "old_135_compare": "one parent f0d46c448367; six added only",
  "old_135_six_blob_matches": "6/6 fixed versus active, full content match",
  "old_135_totals": {
    "lines": 9784,
    "bytes": 386205
  },
  "time_vs_master_canaries": "both time quotes differ from master",
  "new_136_dir": "404 expected not previously published",
  "new_author_marker": "zero"
}
```

## 限定六件

| 文件 | UTF-8字节 | 换行行数 | 内容 |
|---|---:|---:|---|
| MANIFEST.md | 最终作者评论记录 | 最终作者评论记录 | 本清单、接收、指导、G00、自检 |
| new-task-runqueue-entry.md | 7221 | 85 | D00/A/B/C中文正文 |
| facts-and-boundaries.yaml | 69962 | 2455 | 机器可读记录 |
| new-task-runqueue-entry-paths.dot | 15155 | 210 | 同源类型图 |
| deferred-findings.yaml | 3552 | 103 | 机器可读记录 |
| evidence.md | 10204 | 413 | 源码证据窗 |

固定工作区：`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-new-task-runqueue-entry-136/`。facts与DOT共享 111 个节点及 93 条唯一类型边，无缺失端点。所有源码引文均取固定time符号内1–5行，本批正式引文数38，重新核对38，降级0。

## D00、A、B、C覆盖

D00三项均为 `CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW`，不由作者关闭。十查询旧200键/10返回、成功C1/C2旧53结果键与C3六事件/11未达保全。A静态准备，B两种顺序队首插入，C103/110两独立picker与短路及回插计费。

## self_check与禁止操作

```json
{
  "self_check": {
    "verified_claims": 38,
    "quotes_reconfirmed": 38,
    "downgraded_to_inferred": 0
  },
  "read_channel": "connector",
  "G00_status": "PASS_BOUNDED",
  "full_local_P9": "NOT_RUN",
  "P9_1_to_13_status": "connector-scoped text and source checks only; P9_9_10_12 not applicable without code, patch or downgrade",
  "write_retry_log": "not_needed",
  "limitations": {
    "complete_local_P9": "NOT_RUN",
    "preprocess": "NOT_RUN",
    "compile_link": "NOT_RUN",
    "repository_scripts": "NOT_RUN",
    "graphviz": "NOT_RUN",
    "QEMU": "NOT_RUN",
    "real_hardware_process_PID_schedule_clock": "NOT_RUN",
    "actual_first_CPU_execution": "UNKNOWN",
    "actual_parent_child_order": "UNKNOWN",
    "libc_ABI_user_bridge": "UNKNOWN",
    "no_kernel_change": true,
    "no_patch": true
  }
}
```

不运行内核、预处理/编译、Graphviz、QEMU或任何真实任务；不修改内核、旧件、lead、PR正文；不建立新分支/PR、不合并、不强推。阶段2。G04成功后STOP_WRITING，并交回原C09由Pro独立验收且同轮安排下一批。

## 本次确已读取的项目输入路径

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
- `agent-workspace/lead/MYOS2-LEAD-002/187-user-fork-return-identities-contract.md`
- `agent-workspace/lead/MYOS2-LEAD-002/188-user-new-task-runqueue-entry-contract.md`
- `agent-workspace/lead/MYOS2-LEAD-002/188-executor-continuation/00-C08c-to-C09a-handoff-package.md`
- `agent-workspace/lead/MYOS2-LEAD-002/188-executor-continuation/01-MYOS2-A-C09a-receiver-startup-prompt.md`
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FORK-RETURN-IDENTITIES-135-review.md`
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FORK-RETURN-IDENTITIES-135-D00-133-134-review.md`
- `agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-23-fork135-reviewed-new-task136-C09a.md`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/MANIFEST.md`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities.md`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/facts-and-boundaries.yaml`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/fork-return-identities-paths.dot`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/deferred-findings.yaml`
- `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-return-identities-135/evidence.md`
- `mykernel/sched/task/pid.c`
- `mykernel/sched/forkexec/fork.c`
- `mykernel/lock_IPC/signal/signal.h`
- `mykernel/arch/x86_64/sched/context/process_arch.c`
- `mykernel/sched/scheduler/scheduler_core.c`
- `mykernel/lib/list/double_list.h`
- `mykernel/sched/task/task_const.h`
- `mykernel/sched/scheduler/myos_rt.c`
- `mykernel/debug/panic.c`
- `myinitramfs/myinit/initd.c`
- `mykernel/scripts/options_flags.cmake`

Mnemosyne指导来源（不移植其任务权限）：

- `08822407d/Mnemosyne:README.md`
- `08822407d/Mnemosyne:current/human-approved-spec.md`
- `08822407d/Mnemosyne:commands/receive-mnemosyne-handoff.md`
- `08822407d/Mnemosyne:commands/load-mnemosyne-guidance.md`
- `08822407d/Mnemosyne:current/guard-registry.yaml`
- `08822407d/Mnemosyne:current/user-operation-next-step-capability-and-intent-guard.md`
- `08822407d/Mnemosyne:current/github-single-active-pr-lineage-guard.md`
- `08822407d/Mnemosyne:current/run-context-and-pr-provenance-guard.md`
- `08822407d/Mnemosyne:current/artifact-delivery-and-direct-generation-guard.md`
- `08822407d/Mnemosyne:current/cross-conversation-execution-intent-and-operator-flow-guard.md`
- `08822407d/Mnemosyne:current/external-research-display-name-guard.md`
- `08822407d/Mnemosyne:current/source-artifact-preservation-and-design-rationale-guard.md`
- `08822407d/Mnemosyne:current/model-tier-and-subtask-cost-routing-guard.md`
- `08822407d/Mnemosyne:current/next-step-repository-write-visibility-guard.md`
- `08822407d/Mnemosyne:current/agent-product-ready-pr-and-frontier-efficiency-guard.md`
