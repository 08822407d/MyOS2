---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SHELL-FILE-COMMANDS-120
record_type: batch120_six_artifact_manifest
produced_by: "GPT-6（当前会话自报，后台身份与effort不可独立认证）"
model_per_owner: "GPT-6＋Extra（Owner明确选择）"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-18
conversation_display_name: "MYOS2-A-C08a 代码分析执行"
date: 2026-11-21
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_contract_short12: 9df81502ae54
execution_parent_short12: 9957ff1d927f
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_C08_REVIEW
D00_pending: [R117-02, R118-02, R119-01]
D00_previously_closed: {R118-01: CLOSED_DOCUMENTATION_ONLY}
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  time_options: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  time_panic: "\tthis_cpu = smp_processor_id();"
  master_options: "\t-m64 -mcmodel=large -fPIE \\"
  master_panic: "\tthis_cpu = raw_smp_processor_id();"
self_check: {verified_claims: 81, quotes_reconfirmed: 81, downgraded_to_inferred: 0}
per_file_literal_prefix: {MANIFEST.md: 0, shell-file-commands.md: 0, facts-and-boundaries.yaml: 0, shell-file-commands-paths.dot: 0, deferred-findings.yaml: 0, evidence.md: 81}
new_P2_eligible_windows: 81
new_literal_prefix_total: 81
full_local_P9: NOT_RUN
build_QEMU_hardware: NOT_RUN
graphviz: NOT_RUN
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/170-user-shell-file-commands-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-INPUT-DISPATCH-119-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-INPUT-DISPATCH-119-D00-117-118-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-21-shell-input119-reviewed-file-commands120.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/168-executor-continuation/01-MYOS2-A-C08a-receiver-startup-prompt.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/169-user-shell-input-dispatch-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md"
  - "agent-workspace/tasks/00-gpt-task-protocol-v2.md"
  - "agent-workspace/conventions.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FAT-DIRECTORY-MUTATION-57-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-VFS-MUTATION-GATES-58-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FAT-DIRECTORY-MUTATION-57-DOC-CLOSURE-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-VFS-MUTATION-GATES-58-DOC-CLOSURE-review.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/MANIFEST.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch-paths.dot"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/deferred-findings.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/evidence.md"
  - "myinitramfs/myshell/myshell.c"
  - "mykernel/fs/vfs/namei.c"
  - "myinitramfs/mylib/myglibc/io/sys/stat.h"
  - "myinitramfs/mylib/myglibc/bits/stat.h"
  - "mykernel/include/linux/kernel/stat.h"
  - "mykernel/include/uapi/linux/stat.h"
  - "myinitramfs/mylib/myglibc/syscall/syscall.c"
  - "mykernel/arch/x86_64/entry/syscall_64.c"
  - "mykernel/arch/x86_64/include/asm/syscall_wrapper.h"
  - "mykernel/arch/x86_64/include/asm/unistd_64.h"
  - "mykernel/arch/x86_64/include/asm/syscalls_64.h"
  - "myinitramfs/mylib/myglibc/stdio/printf.c"
  - "myinitramfs/mylib/myglibc/stdio/errlist.c"
  - "myinitramfs/mylib/myglibc/stdio/errlist_internal.c"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/debug/panic.c"
  - "mykernel/fs/vfs/d_path.c"
guidance_refresh:
  operation: behavior_constraint_refresh
  task_local_only: true
  original_identity_preserved: MYOS2-LEAD-002-CHAT-EXEC-18
  current_task_preserved: CORE-USER-SHELL-FILE-COMMANDS-120
  mnemosyne_fixed_master_short12: e581eb9df6c4
  project_rules: same_active_conversation_nine_constraints_previous_receive_plus_63_110_124_refresh
  mnemosyne_guidance: yes
  actual_core_read: [README.md, current/human-approved-spec.md, commands/load-mnemosyne-guidance.md, current/user-operation-next-step-capability-and-intent-guard.md, current/guard-registry.yaml]
  actual_conditional_read:
    - "current/user-operation-next-step-capability-and-intent-guard.md"
    - "current/run-context-and-pr-provenance-guard.md"
    - "current/source-artifact-preservation-and-design-rationale-guard.md"
    - "current/next-step-repository-write-visibility-guard.md"
    - "current/github-single-active-pr-lineage-guard.md"
  active_task_authority: owner_and_MyOS2_170_not_Mnemosyne
  other_project_live_route_imported: false
  backend_effort_attestation: unknown
G00:
  direct_ref_short12: {lead: 9df81502ae54, exec: 9957ff1d927f, time: a039d9803ade, master: e8e60cd19540}
  PR16: OPEN_READY_UNMERGED
  PR17: OPEN_DRAFT_UNMERGED
  open_PR_pagination: [2, 0]
  old119_fixed_active_six_blobs: MATCHED_6_OF_6
  old119_blob_short12: {MANIFEST.md: 7fbfc4a08a42, shell-input-dispatch.md: d01a010e2c4e, facts-and-boundaries.yaml: 1d3e4cc06c8c, shell-input-dispatch-paths.dot: aa2d14324cc5, deferred-findings.yaml: fff0742e440a, evidence.md: eab92a98e260}
  target120_files_at_parent: ZERO
  comment_pagination_to_empty: [100, 100, 26, 0]
  119_author_comment_id: 7062147403
  119_review_and_120_release_comment_id: 7062355498
  C08_120_publication_marker_count: 1
  120_author_marker_initial_count: 0
  direct_master_startup_quote_fresh: true
  two_branch_canary_pairs_fresh: true
write_retry_log:
  authoring_validation: "source quote/range draft check caught two pre-blob construction errors; corrected before external write; not transient GitHub errors"
  initial_five_blob_creation: "all five Git data blobs created once each, all returned SHA, no transient GitHub errors"
  manifest_blob: "created from these bytes; result checked after creation, not an assertion of branch commit"
  branch_update: "not yet attempted at MANIFEST serialization; post-write proof in formal comment and final report"
  formal_comment: "not yet attempted at MANIFEST serialization; verify marker before posting"
  final_remote_readback: "not yet run at MANIFEST serialization; perform after branch update and comment"
---

# 第120批六件MANIFEST｜CHAT-EXEC-18

**作者已完成有界阶段2纸面分析，READY_FOR_MAINLINE_REVIEW，主线独立验收仍PENDING。** 170号由C08正式发布，继续原执行身份/分支/PR17，fresh G00后将D00三项差量与touch、mkdir、rm/rmdir三专题一起提交。无需Owner处理源码疑点；D00不能自签CLOSED。

## 文件说明及供后续如何消费

| 文件 | 意义与证据边界 |
|---|---|
| MANIFEST.md | 身份与固定输入、任务局部指导刷新、fresh G00、P6、原句和金丝雀、Git发布前记录 |
| shell-file-commands.md | D00三项补正；touch的0666/creat、mkdir的0777/mode、rm和rmdir的返回和错误格式案例 |
| facts-and-boundaries.yaml | 兼容YAML的JSON机器表，D120五项显式case_id，A4/B6/C7个纸面条件案例及历史D00状态 |
| shell-file-commands-paths.dot | 六簇129节点、164条带类型边；13组逐段有向路径检查；与真实完整CFG及Graphviz区分 |
| deferred-findings.yaml | 三项OPEN待C08验收及7项有界来源/运行缺口，无需Owner现在修复 |
| evidence.md | 81个真正定义体新短窗，12个普通位置；历史117/118/119资格差量仅追加解释 |

## 计数、历史裁定及本次证据纪律

按全部**本批六件**字面正式前缀搜索0/0/0/0/0/81，合计81，真正合格新短窗81，P6三数81/81/0。每个新短窗限制在固定time已读函数/宏/结构体定义内的连续1–5行，已按文件、符号、行号、命题回源；非定义体的全局表、syscall宏调用实例及映射调用单列普通位置。117原88窗/89字面；118原91窗/92字面、两处资格降为普通位置→89候选；119原46窗/46字面、三处全局表重分类→43候选。旧原件和评论不改，历史计数不计入本批自检。

已读time活动C配置和direct master对应金丝雀，公约§1第2条按direct master重新复制。P9可行文本核验含40位SHA禁止、源短窗、行界、路径、普通位置与P2资格、引用ID、字面计数、模型解析、DOT端点、CMake/panic双金丝雀、无外部证据假借、写区和身份。**完整本地P9、编译/链接/仓库程序、Graphviz、QEMU/实机和真实文件操作一律NOT_RUN**，没有以静态模型冒充实测。

## 写后终检与停止边界

仅将上述恰好六件新增到`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/`，单父为`9957ff1d927f`；不得改旧件、内核、lead、PR标题和Draft状态。先独立核新Git树、提交、原PR17执行ref和六件固定/活动全文及Blob，确认后在PR17发表唯一作者FORMAL_DELIVERY评论，再按ID双读、评论全集分页至空、四ref/两PR/六件回读。GitHub暂态异常依63号先查真实远端并用expected-head、force=false安全重试。若并发/身份冲突不盲写。终检后STOP_WRITING，C08使用Pro独立验收三项D00及A/B/C，并在同一Pro回合规划后继。
