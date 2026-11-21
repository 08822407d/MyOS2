---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-STDIO-OPEN-FLAG-ABI-122
record_type: executor_122_six_artifact_manifest
status: READY_FOR_MAINLINE_REVIEW
produced_by: "GPT-6（会话自报；后台effort不可独立认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-18
conversation_display_name: "MYOS2-A-C08a 代码分析执行"
date: 2026-11-22
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
source_pin_short12: a039d9803ade
master_direct_short12: e8e60cd19540
lead_release_short12: 72abc20e7a70
execution_parent_short12: e1272a3f9b69
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_pr: 17
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_C08_REVIEW
D00_open: [R120-02, R121-01, R121-02]
historical_closed: [R117-02, R120-01]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  time_options: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  time_panic: "\tthis_cpu = smp_processor_id();"
  master_options: "\t-m64 -mcmodel=large -fPIE \\"
  master_panic: "\tthis_cpu = raw_smp_processor_id();"
self_check: {verified_claims: 85, quotes_reconfirmed: 85, downgraded_to_inferred: 0}
per_file_literal_prefix_counts: {MANIFEST.md: 0, stdio-open-flag-abi.md: 0, facts-and-boundaries.yaml: 0, stdio-open-flag-abi-paths.dot: 0, deferred-findings.yaml: 0, evidence.md: 85}
new_qualified_windows: 85
new_literal_prefix_total: 85
full_local_P9: NOT_RUN
build_link_QEMU_hardware_real_file_operations: NOT_RUN
Graphviz: NOT_RUN
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/172-user-stdio-open-flag-abi-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-FILE-OPEN-121-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-FILE-OPEN-121-D00-117-120-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-stdio-open121-reviewed-flag-abi122.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-OPEN-FLAGS-30-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-OPEN-FLAGS-30-DOC-FIX-01-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/168-executor-continuation/01-MYOS2-A-C08a-receiver-startup-prompt.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/171-user-stdio-file-open-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md"
  - "agent-workspace/tasks/00-gpt-task-protocol-v2.md"
  - "agent-workspace/conventions.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/MANIFEST.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/stdio-file-open.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/stdio-file-open-paths.dot"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/deferred-findings.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/evidence.md"
  - "myinitramfs/mylib/myglibc/libio/fileops.c"
  - "myinitramfs/mylib/myglibc/libio/iofopen.c"
  - "myinitramfs/mylib/myglibc/libio/libio.h"
  - "myinitramfs/mylib/myglibc/bits/fcntl.h"
  - "myinitramfs/mylib/myglibc/include/fcntl.h"
  - "myinitramfs/mylib/myglibc/io/fcntl.h"
  - "myinitramfs/mylib/myglibc/include/features.h"
  - "myinitramfs/mylib/myglibc/CMakeLists.txt"
  - "myinitramfs/CMakeLists.txt"
  - "myinitramfs/mylib/myglibc/syscall/syscall.c"
  - "myinitramfs/myshell/myshell.c"
  - "mykernel/fs/vfs/namei.c"
  - "mykernel/fs/vfs/open.c"
  - "mykernel/fs/syscall.c"
  - "mykernel/include/uapi/asm-generic/fcntl.h"
  - "mykernel/include/uapi/linux/fcntl.h"
  - "mykernel/include/uapi/linux/openat2.h"
  - "mykernel/include/linux/kernel/fcntl.h"
  - "mykernel/include/linux/kernel/stat.h"
  - "mykernel/include/uapi/linux/stat.h"
  - "mykernel/arch/x86_64/include/asm/unistd_64.h"
  - "mykernel/arch/x86_64/include/asm/syscalls_64.h"
  - "mykernel/arch/x86_64/entry/syscall_64.c"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/debug/panic.c"
task_local_guidance_refresh:
  operation: behavior_constraint_refresh
  scope: current_task_only
  identity_preserved: MYOS2-LEAD-002-CHAT-EXEC-18
  task_id_preserved: CORE-USER-STDIO-OPEN-FLAG-ABI-122
  model_per_owner: "GPT-6＋Extra"
  mnemosyne_master_short12: e581eb9df6c4
  priority: "MyOS2 Owner and fixed172 control writes; Mnemosyne supplies task-local behavioral constraints"
  Mnemosyne_maintenance_authorized: false
  guidance_source_reads:
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
  four_direct_refs_short12: {lead: 72abc20e7a70, exec: e1272a3f9b69, time: a039d9803ade, master: e8e60cd19540}
  two_PRs: {PR16: OPEN_READY_UNMERGED, PR17: OPEN_DRAFT_UNMERGED}
  open_PR_pages: [2, 0]
  prior121_fixed_active_six_blobs: MATCHED_6_OF_6
  prior121_blob_short12: {MANIFEST.md: 9a60fb811de9, stdio-file-open.md: 8a4d72181a93, facts-and-boundaries.yaml: 0b1ad1e11a1d, stdio-file-open-paths.dot: 522099473929, deferred-findings.yaml: dd21c48434fe, evidence.md: cac9cb33aab0}
  new122_files_on_execution_parent: ZERO
  PR17_comment_pagination_to_empty: [100, 100, 30, 0]
  121_author_comment_id: 7065547517
  121_review_and_122_release_comment_id: 7066063910
  122_author_marker_before_write: ZERO
  direct_master_original_quote_checked: true
  both_branch_canary_pairs_checked: true
text_validation:
  new_source_windows_each_one_to_five_lines: 85
  source_windows_exact_against_fixed_time: 85
  eligible_definition_windows: 85
  source_files_checked: 18
  new_abi_cases: 4
  D00_corrected_cases: 13
  R121_original_return_cases: 5
  derived_numeric_abi_mask_hex: 0x7FFFC3
  graph_clusters: 5
  graph_unique_nodes: 217
  graph_typed_edges: 200
  graph_positive_paths_checked: 31
  graph_forbidden_paths_checked: 18
  runtime_confirmed: false
write_retry_log:
  artifact_authoring: "Five blobs prepared; multiple local in-tool graph draft validation errors stopped before blob creation, final graph passed 31 positive/18 forbidden checks; these were not GitHub write failures"
  initial_five_blobs: "created successfully and read by blob SHA; no transient GitHub error"
  manifest_blob: "created only after prior checks; serialization is before branch commit"
  remote_branch_update: "not yet attempted at manifest serialization; require expected_sha and force=false"
  formal_comment: "not yet posted; marker must be unique and ID double-read after posting"
  final_remote_verification: "not yet performed at manifest serialization; postwrite real checks to be reported in PR17 delivery"
---

# 第122批 · 五项任务证据的六件交付清单

作者：**MYOS2-A-C08a / CHAT-EXEC-18**。本次合同由C08正式#7066063910于固定172号发布，Owner已转发，fresh G00通过。执行者只在原执行分支和既有PR17继续，第121批及更旧历史六件保持冻结；不修改kernel、lead、协议或评论。

## 六件用途与真实性

| 文件 | 内容与后续主线消费 |
|---|---|
| MANIFEST.md | 当前身份、固定四ref、授权写区、原句金丝雀、局部行为规范刷新、源输入、P6与Git前置检查 |
| stdio-open-flag-abi.md | D00三项纠正；两套头来源、r/w/a+/wbx逐位双编码、两种输入到build_open_how逐格结果 |
| facts-and-boundaries.yaml | JSON兼容YAML机器件，ABIx4、D00x13、C121x5、A121-03，所有输入、字段、门及类型来源分离 |
| stdio-open-flag-abi-paths.dot | 五簇217唯一节点、200带类型边，31条实例正向路径和18条禁止交叉路径，源码与模型边区别 |
| deferred-findings.yaml | 三项文档OPEN待C08验收，七项源码/编译/链接/运行范围缺口 |
| evidence.md | 85个固定time真实定义体或活动CMake命令1–5行短窗，18个普通位置；历史勘误来源另列 |

## 自检与历史范围

本批六件最终须字面检索正式短窗前缀0/0/0/0/0/85，计数85、合格85、降级0；前五件文本中不得额外出现正式前缀。旧121有69短窗，原171合格资格与E023标题纠正分开，不能把旧窗计入本批。R117-02和R120-01已由C08正式文档闭合，122仅追加R120-02、R121-01、R121-02。图的真实直接调用、条件桥、重新编码和数值相等采用不同边型；不把比特计算误写成已运行的系统调用。

可行文本P9核本次固定来源、短号、双金丝雀、精确源码窗口/符号、CMake条件、两侧常量位与进制、8个how值、D00正反图、所有case_id、JSON结构、DOT无悬空、六件写区与Git Blob；完整本地P9、编译预处理、仓库程序、Graphviz、QEMU、实机和真实文件操作均NOT_RUN。仓外musl真实头与实际sh链接为UNKNOWN，不能由仓内存在的实现自动推出。

## Git写入及正式交回

仅向`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-open-flag-abi-122/`新增上述六件，单父固定执行`e1272a3f9b69`。在安全expected-head及force=false下推进原执行分支、沿原Draft PR17，不新增PR、不改旧件；GitHub异常按63号先读回再安全重试。写后比较恰六纯新增及单父、逐件固定/活动字节和Git Blob独立重算，正式评论先查重，发表一次并按ID双读、PR17全分页至空、复核四ref/两PR/六件。仅在实际完成后回报`READY_FOR_MAINLINE_REVIEW`与`mainline_acceptance=PENDING`并STOP_WRITING，交C08 Pro独立裁定。
