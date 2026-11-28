---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LIBC-STRING-COMPARE-123
record_type: batch123_executor_manifest
produced_by: "GPT-6（会话自报，后端与effort不可独立认证）"
model_per_owner: "GPT-6＋Extra（Owner明确要求）"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-18
conversation_display_name: "MYOS2-A-C08a 代码分析执行"
date: 2026-11-22
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_release_short12: 561261755177
execution_parent_short12: 7d0e810ec4ff
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_C08_REVIEW
D00_open: [R122-01]
D00_closed_by_C08: [R120-02, R121-01, R121-02]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  time_options: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  time_panic: "\tthis_cpu = smp_processor_id();"
  master_options: "\t-m64 -mcmodel=large -fPIE \\"
  master_panic: "\tthis_cpu = raw_smp_processor_id();"
self_check: {verified_claims: 59, quotes_reconfirmed: 59, downgraded_to_inferred: 0}
per_file_literal_prefix: {MANIFEST.md: 0, libc-string-compare.md: 0, facts-and-boundaries.yaml: 0, libc-string-compare-paths.dot: 0, deferred-findings.yaml: 0, evidence.md: 59}
new_eligible_source_windows: 59
new_literal_prefix_total: 59
full_local_P9: NOT_RUN
build_link_Graphviz_QEMU_hardware_real_files: NOT_RUN
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/173-user-libc-string-compare-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-D00-120-121-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-open-flag-abi122-reviewed-string-compare123.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-INPUT-DISPATCH-119-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CWD-DISPLAY-118-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIR-STREAM-LS-116-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/172-user-stdio-open-flag-abi-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/168-executor-continuation/01-MYOS2-A-C08a-receiver-startup-prompt.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md"
  - "agent-workspace/tasks/00-gpt-task-protocol-v2.md"
  - "agent-workspace/conventions.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/MANIFEST.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/stdio-open-flag-abi.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/stdio-open-flag-abi-paths.dot"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/deferred-findings.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/evidence.md"
  - "myinitramfs/mylib/myglibc/string/strcmp.c"
  - "myinitramfs/mylib/myglibc/string/strncmp.c"
  - "myinitramfs/mylib/myglibc/string/string.h"
  - "myinitramfs/mylib/myglibc/include/string.h"
  - "myinitramfs/mylib/myglibc/string/CMakeLists.txt"
  - "myinitramfs/mylib/myglibc/CMakeLists.txt"
  - "myinitramfs/CMakeLists.txt"
  - "myinitramfs/myshell/CMakeLists.txt"
  - "myinitramfs/myshell/myshell.c"
  - "mykernel/fs/vfs/open.c"
  - "mykernel/fs/vfs/namei.c"
  - "myinitramfs/mylib/myglibc/libio/fileops.c"
  - "myinitramfs/mylib/myglibc/libio/iofopen.c"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/debug/panic.c"
guidance_refresh:
  operation: behavior_constraint_refresh
  scope: task_local_only
  existing_executor_identity_preserved: MYOS2-LEAD-002-CHAT-EXEC-18
  active_task_preserved: CORE-USER-LIBC-STRING-COMPARE-123
  nine_prior_MyOS2_rules: reused_only_from_same_active_conversation_not_claimed_reloaded_now
  project_supplemental_rules_reloaded: [28, 63, 79, 110, 124, 168]
  Mnemosyne_master_short12: e581eb9df6c4
  MyOS2_Owner_173_is_write_authority: true
  Mnemosyne_maintenance_permission: false
  guidance_files_read:
    - "README.md"
    - "current/human-approved-spec.md"
    - "commands/load-mnemosyne-guidance.md"
    - "current/guard-registry.yaml"
    - "current/user-operation-next-step-capability-and-intent-guard.md"
    - "current/run-context-and-pr-provenance-guard.md"
    - "current/github-single-active-pr-lineage-guard.md"
    - "current/source-artifact-preservation-and-design-rationale-guard.md"
    - "current/next-step-repository-write-visibility-guard.md"
    - "current/model-tier-and-subtask-cost-routing-guard.md"
G00:
  direct_refs_short12: {lead: 561261755177, exec: 7d0e810ec4ff, time: a039d9803ade, master: e8e60cd19540}
  PR16: OPEN_READY_UNMERGED_MASTER_BASE
  PR17: OPEN_DRAFT_UNMERGED_MASTER_BASE
  open_PR_pages_to_empty: [2, 0]
  prior122_fixed_active_blobs: MATCHED_6_OF_6
  prior122_blobs_short12: {MANIFEST.md: afd1f6eb1446, stdio-open-flag-abi.md: 4853515055e3, facts-and-boundaries.yaml: 7f5bef9a8bb0, stdio-open-flag-abi-paths.dot: 675543dcc100, deferred-findings.yaml: 4eb84040e826, evidence.md: bba2056f0707}
  target123_files_at_parent: ZERO
  PR17_top_comments_pages_to_empty: [100, 100, 32, 0]
  prior122_author_comment_id: 7066384317
  C08_122_review_123_release_comment_id: 7066555185
  original_author123_marker_before_delivery: ZERO
  all_source_and_branch_canaries_reconfirmed: true
feasible_text_validation:
  new_source_definition_windows_with_1_to_5_physical_lines: 59
  exact_quote_matches_at_source: 59
  source_files_pinned_and_checked: 8
  model_case_counts: {strcmp: 6, strncmp: 9, find_cmd: 6, cd: 2, ls: 3}
  new_model_case_total: 26
  R122_01_actual_direct_callsite_edges_from_source_ledger: 17
  graph_clusters: 8
  graph_unique_nodes: 244
  graph_typed_edges: 241
  graph_positive_case_paths: 33
  graph_required_source_direct_call_pairs: 19
  graph_forbidden_DIRECT_CALL_pairs: 7
  graph_forbidden_scenario_reach_pairs: 9
  actual_sh_linked_libc: UNKNOWN
  C_or_ASM_code_executed: false
write_retry_log:
  pre_blob_authoring: "First source-window draft failed 1-5 physical-line check for two candidate windows; corrected before external write. One graph draft received text syntax validation errors; final graph passed all typed-call and case-path checks. No GitHub transient writes."
  first_five_blob_creation: "All five final evidence, facts, report, DOT and deferred blobs were created successfully; verified by SHA retrieval"
  manifest_blob: "produced from this text and checked before any branch move"
  branch_commit: "not yet written at manifest serialization; use parent e1272a3f9b69, expected-head force=false"
  formal_comment: "not yet created at manifest serialization; marker must be checked unique before one POST"
  terminal_remote_readback: "not yet run at manifest serialization; must be reported separately after actual checks"
---

# 第123批作者六件清单 / CHAT-EXEC-18

第123批沿用原执行会话、原执行分支与原Draft PR17，唯一父为第122批执行头`7d0e810ec4ff`。fresh G00已核四直接ref、两PR、六件旧122固定/活动同一性及初始123目录空、PR17评论232条分页至空。C08裁定122整体RETURN，只允许本批补R122-01；更早R120-02/R121-01/R121-02已文档关闭。原文件冻结。

## 六件用途

| 文件 | 内容及主线独立消费 |
|---|---|
| MANIFEST.md | 当前身份、实际读取、接收/局部指导刷新、G00、双金丝雀、P6及写后审计要求 |
| libc-string-compare.md | R122-01真实直接调用所有者/顺序/数据补正；strcmp/strncmp逐字节；find/cd/ls条件与输出请求 |
| facts-and-boundaries.yaml | 机器可解析JSON兼容YAML：D00 17项拥有者调用、A6、B9、C find6/cd2/ls3，具体每次读字节、调用数、NULL与返回 |
| libc-string-compare-paths.dot | 8簇244唯一节点、241有类型边；33个实例路径、19项直接调用存在、7项不得误标DIRECT_CALL及9个禁止案例路径 |
| deferred-findings.yaml | 一个R122-01仍OPEN的补正申请、八个附带静态边界与后续证据入口 |
| evidence.md | 59个真实定义体或活动CMake命令的1–5行短窗，15个普通位置；独立于旧122的85个引文 |

## 自检与暂停门

本批六件预计正式前缀分布为0/0/0/0/0/59，P6三数59/59/0；必须在提交后按GitHub固定六件重新计算。字符串比较的精确差值、读字节数与返回仅属于选择本仓实现的纸面模型；shell条件判断按等零/非零和&&短路另行消费，绝不能把未知外部musl的精确整数当成已证。n=0用有效指针、无解引用；高位字节按unsigned char比较非Unicode排序。D00不重做122八个how数值。

完整本地P9、用户库预处理/编译、仓库程序、Graphviz、QEMU、实机及真实文件操作均NOT_RUN。可行文本核验包括来源短12、真实引文符号/字节/行界、逐case字节数、C判定、图的调用边类型与正反路径、build来源、公约原句、双金丝雀、唯一目标写区与P1禁40位SHA。

## 写入与交接约束

仅向`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/`新增六件、单父快进到原执行分支，禁止改源码、旧件、lead、PR标题与Draft。先核远端安全和完整对象，create_tree/commit后检查父/树，expected-head与force=false更新；如63号暂态异常先远端核已提交情况再重试。发布前对PR17本人正式标记查重，发布恰一评论，按ID双读、顶层全分页到空、四refs/两PR/六件远端终检。本人只提交READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING；终检后STOP_WRITING，交回C08 Pro独立验收而不自签R122-01或发布124。
