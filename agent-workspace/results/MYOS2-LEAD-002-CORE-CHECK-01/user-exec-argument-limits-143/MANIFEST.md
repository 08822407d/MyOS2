---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-EXEC-ARGUMENT-LIMITS-143"
record_type: "phase2_executor_manifest"
conversation_display_name: "MYOS2-A-C10a 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-22"
date: "2026-11-24"
phase: 2
produced_by: "GPT-6.2（系统模型族；界面具体名称不可独立核实）"
model_per_owner: "次一档GPT-6.2 Sol及Extra／高推理强度"
actual_backend_model: "UNATTESTED"
actual_UI_tier: "UNATTESTED"
actual_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_contract_short12: "c09ad0b19c2e"
execution_parent_short12: "a911aa471305"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
status: "AUTHOR_SELF_CHECKED_PENDING_PUBLICATION_AND_G04"
mainline_acceptance: "PENDING"
D00: []
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: ["实际预处理/工具链、C对象/指针有效性、GUP缺return后的真实页路径、清理及真实用户访问/运行仍未证"]
inputs_read: [{"path":"agent-workspace/lead/MYOS2-LEAD-002/198-repository-first-compact-relay.md","ref":"c09ad0b19c2e","scope":"本轮全文"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/199-user-exec-argument-limits-contract.md","ref":"c09ad0b19c2e","scope":"本轮全文"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-STACK-PLACEMENT-142-review.md","ref":"c09ad0b19c2e","scope":"本轮全文"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-24-stack-placement142-reviewed-argument-limits143.md","ref":"c09ad0b19c2e","scope":"本轮全文"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","ref":"c09ad0b19c2e","scope":"本轮§12第13条；前文按79复用"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","ref":"649668088c07","scope":"原13份同一有效执行会话完整读取，复用"},{"path":"agent-workspace/README.md","ref":"649668088c07","scope":"原13份同一有效执行会话完整读取，复用"},{"path":"agent-workspace/conventions.md","ref":"649668088c07","scope":"原13份同一有效执行会话完整读取，复用"},{"path":"agent-workspace/tasks/00-gpt-task-protocol-v2.md","ref":"649668088c07","scope":"原13份同一有效执行会话完整读取，复用"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","ref":"649668088c07","scope":"原13份同一有效执行会话完整读取，复用"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","ref":"649668088c07","scope":"原13份同一有效执行会话完整读取，复用"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","ref":"649668088c07","scope":"原13份同一有效执行会话完整读取，复用"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","ref":"649668088c07","scope":"原13份同一有效执行会话完整读取，复用"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","ref":"649668088c07","scope":"原13份同一有效执行会话完整读取，复用"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","ref":"649668088c07","scope":"原13份同一有效执行会话完整读取，复用"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","ref":"649668088c07","scope":"原13份同一有效执行会话完整读取，复用"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/193-mainline-context-thresholds-and-model-availability.md","ref":"649668088c07","scope":"原13份同一有效执行会话完整读取，复用"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/194-owner-continuity-and-copyable-return.md","ref":"649668088c07","scope":"原13份同一有效执行会话完整读取，复用"},{"path":"agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-stack-placement-142/MANIFEST.md","ref":"a911aa471305","scope":"本轮固定/活动/API Blob全文核对"},{"path":"agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-stack-placement-142/exec-stack-placement.md","ref":"a911aa471305","scope":"本轮固定/活动/API Blob全文核对"},{"path":"agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-stack-placement-142/facts-and-boundaries.yaml","ref":"a911aa471305","scope":"本轮固定/活动/API Blob全文核对"},{"path":"agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-stack-placement-142/exec-stack-placement-paths.dot","ref":"a911aa471305","scope":"本轮固定/活动/API Blob全文核对"},{"path":"agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-stack-placement-142/deferred-findings.yaml","ref":"a911aa471305","scope":"本轮固定/活动/API Blob全文核对"},{"path":"agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-stack-placement-142/evidence.md","ref":"a911aa471305","scope":"本轮固定/活动/API Blob全文核对"},{"path":"agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/facts-and-boundaries.yaml","ref":"cde2d7f994b5","scope":"本轮重取A.cases[0] A1的字符串/计数，不重审全包"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-ARGUMENT-VECTORS-139-review.md","ref":"e591efa48b3d","scope":"A/B/C小向量及R139-03消费限定"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-ARGV-ENTRY-VIEW-140-D00-138-139-review.md","ref":"e591efa48b3d","scope":"判闭尤其R139-03、清理前字段及正常返回条件"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-STACK-23-review.md","ref":"c09ad0b19c2e","scope":"R23-S01/S02页及GUP返回边界"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ARG-PAGES-24-review.md","ref":"c09ad0b19c2e","scope":"R24-S01/S02页引用/独立片段"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-RESOURCE-OWNERSHIP-70-review.md","ref":"c09ad0b19c2e","scope":"R70-S02清理对象与引用"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-COMMIT-GATES-71-review.md","ref":"c09ad0b19c2e","scope":"R71-S02前置门和正常返回限制"},{"path":"mykernel/scripts/options_flags.cmake","ref":"a039d9803ade","blob":"6019436417ad","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/sched/forkexec/exec.c","ref":"a039d9803ade","blob":"0861ebae6fc2","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/lib/string/strnlen_user.c","ref":"a039d9803ade","blob":"dae5c6c14aa4","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/lib/string/string.c","ref":"a039d9803ade","blob":"cbb2752bc8b2","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/include/uapi/linux/binfmts.h","ref":"a039d9803ade","blob":"a60f4e7c53b0","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/include/uapi/linux/resource.h","ref":"a039d9803ade","blob":"6a683439847c","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/include/uapi/linux/limits.h","ref":"a039d9803ade","blob":"bddfd09e4be2","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/include/linux/fs/binfmts.h","ref":"a039d9803ade","blob":"002c2aec4aa8","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/include/linux/kernel/minmax.h","ref":"a039d9803ade","blob":"d22095ba1acc","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/include/linux/kernel/types.h","ref":"a039d9803ade","blob":"ef531baa8317","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/mm/misc/mm_misc_const.h","ref":"a039d9803ade","blob":"ec983b79166f","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/arch/x86_64/mm/page/page_const_arch.h","ref":"a039d9803ade","blob":"b10e280f26f0","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/include/uapi/asm-generic/posix_types.h","ref":"a039d9803ade","blob":"0246766d9a75","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/include/uapi/linux/const.h","ref":"a039d9803ade","blob":"d8bcaf7e3151","scope":"本轮实际取得固定源，相关有界定义；13份新证据源二次重开"},{"path":"mykernel/debug/panic.c","ref":"a039d9803ade","scope":"canary重取"},{"path":"mykernel/debug/panic.c","ref":"e8e60cd19540","scope":"master反验"},{"path":"mykernel/scripts/options_flags.cmake","ref":"e8e60cd19540","scope":"master反验"},{"path":"agent-workspace/conventions.md","ref":"e8e60cd19540","scope":"本轮startup原句重取"},{"path":"agent-workspace/tasks/00-gpt-task-protocol-v2.md","ref":"e8e60cd19540","scope":"142同会话完整复读；本轮按79复用"}]
self_check:
  verified_claims: 91
  quotes_reconfirmed: 91
  downgraded_to_inferred: 0
---

# 143 · 199号合同执行MANIFEST

同一有效C10a/EXEC-22，Owner本轮短启动词授权连续执行；唯一正式发令[7127316380](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7127316380)。原执行分支claude/dazzling-cori-q0dnyt、PR17 Draft。当前只写agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-limits-143/六件。短码从真实连接器输出复制前十二位。本文件在发表前写入，不预签G04。142 PASS_PENDING_LOCAL，R141-01 CLOSED_DOCUMENTATION_ONLY，141合并消费PASS_PENDING_LOCAL，140及历史五项关闭不变；本批D00为空，不重做旧窗。

## 规范实际复用/新增

```json
{
  "same_execution_conversation": true,
  "executor": "MYOS2-LEAD-002-CHAT-EXEC-22",
  "previous_complete_load": "141同一EXEC-22会话实际完整读取；142按79复用，本轮继续复用已完整读取有效版本，不冒称重读所有全文",
  "project_rules": {
    "count": 13,
    "ref": "649668088c07",
    "fulltext_reused": [
      "agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md",
      "agent-workspace/README.md",
      "agent-workspace/conventions.md",
      "agent-workspace/tasks/00-gpt-task-protocol-v2.md",
      "agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md",
      "agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md",
      "agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md",
      "agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md",
      "agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md",
      "agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md",
      "agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md",
      "agent-workspace/lead/MYOS2-LEAD-002/193-mainline-context-thresholds-and-model-availability.md",
      "agent-workspace/lead/MYOS2-LEAD-002/194-owner-continuity-and-copyable-return.md"
    ],
    "extra": "工作令§12第12条此前读；本轮c09ad0b19c2e新增§12第13条读；本轮重取直接master公约，协议v2同一会话142完整复读并复用",
    "current_addition": {
      "path": "agent-workspace/lead/MYOS2-LEAD-002/198-repository-first-compact-relay.md",
      "ref": "c09ad0b19c2e",
      "scope": "本轮全文已读；替代194§4详细聊天展开，未来14份必传；完整仓库交付/实际G04保留"
    }
  },
  "meta_agent": {
    "direct_master": "b234c6c91a2e",
    "direct_construction": "52b8634f5a33",
    "current_entry_reopened": "https://github.com/08822407d/Meta-Agent/blob/b234c6c91a2e/current/behavior-guidance-entry.md",
    "active_version": "v1.4-r10.266",
    "adoption_reused": "https://github.com/08822407d/Meta-Agent/blob/119597a75517/reviews/META-AGENT-FABLE5-REVIEW-AND-DESIGN-001/06-independent-design/48-guidance-v1.4-r10.266-adoption-2026-11-06.md",
    "fulltext_reused": "https://github.com/08822407d/Meta-Agent/blob/119597a75517/reviews/META-AGENT-FABLE5-REVIEW-AND-DESIGN-001/06-independent-design/47-guidance-v1.4-r10.266-for-review.md",
    "common_archive_reused": "https://github.com/08822407d/Meta-Agent/blob/119597a75517/reviews/META-AGENT-FABLE5-REVIEW-AND-DESIGN-001/06-independent-design/45-r9.260-review-kit.tar.gz",
    "common_version": "1.1-r9.260",
    "common_rules_reused": "01–14及16共15条，141时逐条完整提取阅读",
    "construction_checkpoint_and_review_previously_read": "https://github.com/08822407d/Meta-Agent/blob/52b8634f5a33/reviews/META-AGENT-FABLE5-REVIEW-AND-DESIGN-001/09-continuation/15-checkpoint-2026-10-01.md；同提交05-research-questions/81-guidance-full-review-2026-11-23/00-review.md",
    "freshness": "本轮master与建设ref同141/142，current入口重新取得，无已生效变化"
  },
  "mnemosyne": {
    "direct_master": "e581eb9df6c4",
    "reused": [
      "https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/commands/receive-mnemosyne-handoff.md",
      "https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/README.md",
      "https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/current/human-approved-spec.md",
      "https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/notes/registries/project-research-display-name-registry-v0.1.md §7"
    ],
    "scope": "同一执行身份不变，master实际直接ref未变化，按原接收/命名范围复用；不重载旧通用集"
  },
  "excluded": "无MA/MNE/Alaya建设、维护、写入、付费API任务；仅当前MyOS2",
  "current_task_preserved": true,
  "task_local_only": true,
  "missing_required_materials": []
}
```

198及工作令第13条本轮实际读，未来交接原13份加198；详细材料在GitHub，聊天仅3–6行原生取件通知。主线C10计数3，12/15未到；不启动144。

## G00实际核查

```json
{
  "status": "PASS",
  "refs": {
    "lead": "c09ad0b19c2e",
    "exec": "a911aa471305",
    "time": "a039d9803ade",
    "direct_master": "e8e60cd19540"
  },
  "open_PR_pages": [
    2,
    0
  ],
  "open_PRs": [
    16,
    17
  ],
  "PR16": {
    "title": "MYOS2-LEAD-002: 第142批通过、R141-01关闭，发布第143批参数限额任务",
    "state": "open",
    "draft": false,
    "head": "agent/MYOS2-LEAD-002",
    "base": "master",
    "merged": false
  },
  "PR17": {
    "title": "MYOS2-LEAD-002-CORE-CHECK-01: core verification / recheck-01 / recheck-02 / scheduler-order-02 / scheduler-integration-03 / mm-baseline-04 / user-vfs-baseline-05",
    "state": "open",
    "draft": true,
    "head": "claude/dazzling-cori-q0dnyt",
    "base": "master",
    "merged": false
  },
  "PR17_comments_no_since": {
    "pages": [
      100,
      100,
      77,
      0
    ],
    "unique_ids": 277,
    "markers": [
      {
        "marker": "MYOS2-LEAD-002-CHAT-EXEC-21 / CORE-USER-EXEC-ARGV-ENTRY-VIEW-140 / FORMAL_DELIVERY",
        "id": 7120006816,
        "count": 1
      },
      {
        "marker": "MYOS2-LEAD-002 / CORE-USER-EXEC-ARGV-ENTRY-VIEW-140 / INDEPENDENT_REVIEW",
        "id": 7124005105,
        "count": 1
      },
      {
        "marker": "MYOS2-LEAD-002 / CORE-USER-EXEC-ARGV-ENTRY-VIEW-140 / D00_138_139_CLOSED",
        "id": 7124005105,
        "count": 1
      },
      {
        "marker": "MYOS2-LEAD-002 / CORE-USER-EXEC-AUXV-PAYLOAD-141 / TASK_RELEASE",
        "id": 7124005105,
        "count": 1
      },
      {
        "marker": "MYOS2-LEAD-002 / CORE-USER-EXEC-AUXV-PAYLOAD-141 / EXECUTOR_REASSIGNED",
        "id": 7124317436,
        "count": 1
      },
      {
        "marker": "MYOS2-LEAD-002 / C09a_TO_C10a / OWNER_DIRECTED_RECOVERY_PREPARED",
        "id": 7124317436,
        "count": 1
      },
      {
        "marker": "MYOS2-LEAD-002-CHAT-EXEC-22 / CORE-USER-EXEC-AUXV-PAYLOAD-141 / FORMAL_DELIVERY",
        "id": 7124829269,
        "count": 1
      },
      {
        "marker": "MYOS2-LEAD-002 / CORE-USER-EXEC-AUXV-PAYLOAD-141 / INDEPENDENT_REVIEW",
        "id": 7125650327,
        "count": 1
      },
      {
        "marker": "MYOS2-LEAD-002 / CORE-USER-EXEC-STACK-PLACEMENT-142 / TASK_RELEASE",
        "id": 7125650327,
        "count": 1
      },
      {
        "marker": "MYOS2-LEAD-002-CHAT-EXEC-22 / CORE-USER-EXEC-STACK-PLACEMENT-142 / FORMAL_DELIVERY",
        "id": 7126024963,
        "count": 1
      },
      {
        "marker": "MYOS2-LEAD-002 / CORE-USER-EXEC-STACK-PLACEMENT-142 / INDEPENDENT_REVIEW",
        "id": 7127316380,
        "count": 1
      },
      {
        "marker": "MYOS2-LEAD-002 / CORE-USER-EXEC-STACK-PLACEMENT-142 / D00_141_CLOSED",
        "id": 7127316380,
        "count": 1
      },
      {
        "marker": "MYOS2-LEAD-002 / CORE-USER-EXEC-ARGUMENT-LIMITS-143 / TASK_RELEASE",
        "id": 7127316380,
        "count": 1
      }
    ],
    "new143_author_count": 0,
    "old_EXEC21_141_author_count": 0
  },
  "actual_ID_double_read": [
    {
      "id": 7126024963,
      "body_equal": true
    },
    {
      "id": 7127316380,
      "body_equal": true
    }
  ],
  "execution_tree": {
    "sha_short12": "90f88626f0fb",
    "entries": 2562,
    "truncated": false,
    "target_absent": true
  },
  "prior142": {
    "commit": "a911aa471305",
    "parent": "08f6fee103dd",
    "ahead": 1,
    "behind": 0,
    "one_commit": true,
    "six_additions": true,
    "modified": 0,
    "deleted": 0,
    "lines": 8008,
    "utf8_bytes": 305139,
    "fixed_active_api_blob_equal": 6,
    "stats": [
      [
        "MANIFEST.md",
        517,
        34741,
        "77fbb4537cca"
      ],
      [
        "exec-stack-placement.md",
        288,
        32204,
        "027e02fc55b7"
      ],
      [
        "facts-and-boundaries.yaml",
        6216,
        190993,
        "09721aa960df"
      ],
      [
        "exec-stack-placement-paths.dot",
        220,
        20797,
        "eb62d9f302d0"
      ],
      [
        "deferred-findings.yaml",
        128,
        4205,
        "682cb85ab8e0"
      ],
      [
        "evidence.md",
        639,
        22199,
        "c7ee796026e6"
      ]
    ],
    "verdict": "PASS_PENDING_LOCAL",
    "R141_01": "CLOSED_DOCUMENTATION_ONLY",
    "combined_141": "PASS_PENDING_LOCAL"
  },
  "D00": [],
  "prior140": "PASS_PENDING_LOCAL",
  "prior_closed": [
    "R138-03",
    "R139-01",
    "R139-02",
    "R139-03",
    "R139-04"
  ],
  "startup_master_match": true,
  "canary_time": [
    true,
    true
  ],
  "canary_direct_master": [
    false,
    false
  ],
  "mainline_G04": "Owner转发实际PASS；固定发布评论时的PENDING历史不改",
  "design_count": {
    "C10": 3,
    "threshold_12": "NOT_REACHED",
    "threshold_15": "NOT_REACHED"
  }
}
```

## 本轮六件用途、覆盖、消费

| 文件 | 用途 | 覆盖/未做 | 后继消费 |
|---|---|---|---|
| MANIFEST.md | 身份/G00/规则/新P6及逐件索引 | 本轮标准字段齐，实际终检在正式发表后回件 | 先读本件并按合同/正式评论复核 |
| exec-argument-limits.md | A8/B4/C7边界、数值、拒绝时点和caller条件 | 19模型；无真实执行、内存或映射证明 | 按同ID与facts/证据合读，分离预算/长度/清理 |
| facts-and-boundaries.yaml | 可解析19例及分阶段机器记录、源索引、graph | C五串/空串各7观察点；实际复制NOT_ESTABLISHED | 独立核算模型、核源锚和图 |
| exec-argument-limits-paths.dot | 68节点/81边，源关系与条件模型分层 | 逐项对应facts.graph；Graphviz NOT_RUN | 依节点/边四元组消费，不认定全链可达 |
| deferred-findings.yaml | 增量条件性int和/预扣局部观察、边界及D00空 | 无新实证生产缺陷；不重开旧缺口 | 问题先入库；留待主线复核、授权专项 |
| evidence.md | 91个新1–5连续物理行窄窗及性质/来源索引 | 13固定源重新打开逐窗91/91；完整本地P9 NOT_RUN | 按E143索引逐字、定义归属及支持关系独审 |

## 实际读取/补读与自检范围

先读options_flags，相关类型与成员从真实固定源回源。mm_misc_const.h内容为auxv常量，非页尺寸定义；为PAGE_SIZE补读实际目录树定位的page_const_arch.h及const.h的_AC；为size_t无符号模型补读asm-generic/posix_types.h及linux/posix_types.h入口（本轮读）；不扩到全预处理或GUP重做。历史139 A/B/C与R139-03、140判闭及A1、23/24和70/71指定边界按合同消费。

本轮只读异常：第一次142六件复核时连接器RemoteProtocolError，未做任何写；随后完整顺序重取6件成功。两个尝试路径（arch/x86_64/include/asm/page_const_arch.h、arch/x86_64/include/uapi/asm/posix_types.h）404，停止猜路径、由固定tree定位实际page_const_arch.h和通用posix_types定义。不是写失败。候选五件先作为不可变Git对象暂存；从候选tree取全文与source_index后重新打开13份固定源，核91个不同物理窗逐字、非空、1–5行、标识/性质与实际来源Blob、范围全部相合。根新P6=91/91/0，91+0=91；正式标记全部在evidence.md；旧141/142不重计。未重跑本地符号解析，人工按定义片段核支持范围。

文本/JSON/算术自检：19例（8/4/7），A的int中间和/预算门、B的无符号终止测试、C各7阶段/预扣/失败后继均重算；DOT与facts.graph的68节点/81条边逐一文本比对，零重复/悬空/孤立。生成后补正正文语句及机器件最后观察点标签，成功例止于条件bprm_execve调用前、不作其后清理前快照；仅文本/机器模型修订，不动引文及图。正式提交后逐件回读统计及净差异。

## P9连接器可达部分

| 编号 | 实际核查与界限 |
|---|---|
|1|新六件长标识正则扫描0。|
|2|91个新窗重新打开对应固定源码逐字对照；按实际定义/成员/宏/注释/构建和条件门性质区分；未运行完整本地解析。|
|3|输入、证据、历史固定路径为连接器实际读取或同对话既有复用；两个404探索路径仅记日志，不作为正式证据。|
|4|引文物理行范围均在本次源文件内。|
|5|根标准self_check=91/91/0，六件新标签机械计数91，D00空。|
|6|无新增外部技术事实依赖或内部检索句柄。|
|7|两金丝雀在固定time逐字命中，直接master反验不命中。|
|8|直接master公约整句命中，read_channel=connector。|
|9|NA，无实现代码。|
|10|NA，无补丁或改动块。|
|11|确定性运行禁用措辞扫描0。|
|12|NA，非降级交付。|
|13|强制锚点覆盖四常量、页/类型、active __cmp/max/max_t、预算/rlim注释/负数/ptr门、set_limit、测长/条件generic/len门、预扣与直接比较、页/复制、两数组/计数/调用者/exec/补串/清理门；组合断言以编号合读。|

完整本地P9、实际预处理、编译链接、MyOS2程序／脚本、Graphviz、QEMU、实机及真实操作均NOT_RUN。C模型短预算与A/142独立，条件写入与实际复制分开；没有凭GUP缺return推导真实有效页或成功返回；清理前状态及正常清理条件下外返分开。阶段2，无源码修改、旧件或评论、主线/PR元数据改动；不建分支/PR、不合并、强推、删分支。63写失败须先核实际远端效果后限次间隔恢复；当前候选对象建立无暂态写失败，后续正式回执/G04另实记。
