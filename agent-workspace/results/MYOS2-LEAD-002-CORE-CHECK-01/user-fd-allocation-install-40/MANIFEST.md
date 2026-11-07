---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FD-ALLOCATION-INSTALL-40
record_type: additive_prior_document_correction_and_independent_fd_executor_delivery
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
conversation_display_name: MYOS2-A-C04a 代码分析执行
mainline_owner: MYOS2-A-C04 内核分析主线续建
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-05
read_channel: connector
base_snapshot: time
source_pin_short12: a039d9803ade
execution_branch: claude/dazzling-cori-q0dnyt
execution_base_short12: 4ea455865a47
lead_branch: agent/MYOS2-LEAD-002
lead_observed_short12: a70d22015ae5
master_observed_short12: e8e60cd19540
contract65_short12: a70d22015ae5
review39_short12: a70d22015ae5
rule63_short12: a70d22015ae5
pr_execution: 17
pr_mainline: 16
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fd-allocation-install-40/
allowed_files: [MANIFEST.md, fd-allocation-install.md, facts-and-boundaries.yaml, fd-allocation-install-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen_since: 4ea455865a47
prior39_verdict: RETURN
prior39_returned_scope: [R39-01]
prior39_return_kind: DOCUMENTATION_ONLY
prior39_correction_status: CORRECTION_SUBMITTED_PENDING_MAINLINE
prior39_closure: PENDING_C04_REVIEW
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
owner_decisions_requested_now: []
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  options_flags_cmake: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  panic_c: "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 55
  quotes_reconfirmed: 55
  downgraded_to_inferred: 0
  correction_R39C_quotes: 8
  independent_FD40_quotes: 47
  source_definition_files: 9
  one_to_five_lines_per_quote: true
  legacy39_quote_ids: 69
  legacy39_mismatched_symbol_id: OC39-A069
  legacy39_historical_not_rechecked: 68
  legacy39_all_tags_valid: false
  object_identity_records: 22
  finite_state_models: 26
  local_state_transition_rows: 13
  return_and_failure_matrix_rows: 8
  new_deferred_candidates: 3
  historical_qualified_links: 12
  graph_explicit_nodes: 53
  graph_edges: 62
  graph_undefined_endpoints: 0
  full_40hex_git_sha_in_artifacts: 0
  six_files_expected: true
  source_C_ASM_or_repo_scripts_executed: false
  preprocessor_compilation_QEMU_hardware: NOT_RUN
  full_local_P9_gate: NOT_RUN
write_retry_log:
  status: not_needed
  staging_write_requests: 6
  staging_transient_GitHub_API_failures: 0
  branch_ref_update_attempts_before_manifest: 0
  delivery_comment_attempts_before_manifest: 0
  actual_transient_retries_before_manifest: 0
  logical_operation: "blob-stage success; tree/commit/ref/comment to follow with expected-head and rule63 safety preflight"
  error_kind: none
  remote_head_before_retry: not_applicable
  remote_effect_observed: "unattached blobs only; branch still subject to final live check"
  delay_or_intervening_work: not_applicable
  retry_action: not_needed
  final_readback: PENDING_AT_MANIFEST_CREATION
  local_non_API_validation: "An initial citation-width draft failed local range QA; revised before staging. First DOT draft corrected misleading source edges, no branch changes."
  rule63: "On transient error: check remote first, then spaced retries ~10/20/40s, reuse object SHA, expected-head/force=false; no invented attempts."
inputs_read_current_turn:
  - "65-user-fd-allocation-install-contract.md @ a70d22015ae5 [complete]"
  - "reviews/CORE-USER-OPEN-COMPLETION-39-review.md @ a70d22015ae5 [complete, R39-01 and R39-S01/S02]"
  - "63-github-write-retry-and-readback.md @ a70d22015ae5 [complete]"
  - "PR17 own batch39 delivery comment 6648914733 [retrieved]"
  - "user-open-completion-39/evidence.md @ 4ea455865a47 [last A069 and old count checked]"
  - "user-open-completion-39/MANIFEST.md @ 4ea455865a47 [old 69/69 self_check exact]"
  - "user-open-completion-39/open-completion.md @ 4ea455865a47 [historical ENFILE and flag sections]"
  - "user-open-completion-39/facts-and-boundaries.yaml @ 4ea455865a47 [old bit model]"
  - "user-vfs-baseline-05/deferred-findings.yaml @ 4ea455865a47 [UV-11 full-table issue and UV-04 old return scope]"
  - "user-syscall-stdio-08/deferred-findings.yaml @ 4ea455865a47 [US-02 bound and US-03 close-on-exec]"
  - "time mykernel/fs/vfs/file.c [complete get_unused_fd_flags,alloc_fd,fd_install,put_unused_fd,init_files,selected dup_fd/f_dupfd]"
  - "time mykernel/include/linux/kernel/fdtable.h [NR_OPEN_DEFAULT/MAX,files_struct_s]"
  - "time mykernel/fs/vfs/open.c [do_sys_openat2 complete selected]"
  - "time mykernel/include/linux/fs/fs.h [OPEN_FMODE,__FMODE_NONOTIFY,FMODE_32/64BITHASH,file_s]"
  - "time mykernel/include/uapi/asm-generic/fcntl.h [O_CLOEXEC macro]"
  - "time mykernel/include/uapi/asm-generic/errno-base.h [EXDEV/ENFILE/EMFILE/ENOMEM/EINVAL]"
  - "time mykernel/include/linux/kernel/asm-generic/bug.h [conditional BUG_ON and BUG definitions]"
  - "time mykernel/include/linux/kernel/err.h [ERR_PTR/PTR_ERR/IS_ERR]"
  - "time mykernel/fs/vfs/file_table.c [__alloc_file/alloc_empty_file/fput one layer]"
  - "time mykernel/scripts/options_flags.cmake and mykernel/debug/panic.c exact canaries; master versions negative"
  - "live repo permission, PR16/17, open PR set, direct refs/time/master, PR16 changed filenames, absent target directory"
inputs_inherited_not_reloaded:
  - "MyOS2 00-work-order, agent-workspace/conventions.md, tasks/00-gpt-task-protocol-v2 P1-P13, lead/18 deferred/24 role separation/28 same-turn review; prior C04a receiver"
  - "Mnemosyne guidance applied earlier for provenance and single writer; no independent Mnemosyne maintenance or cross-project action"
  - "R38-S01/S02, R37-S01/S02, R36C-S01, R13-S01 and prior OF30 DOC-FIX were not re-reviewed wholesale"
open_questions:
  - "R39-01 document RETURN can only be closed by C04 after this new correction is independently reviewed."
  - "current->files instance and fd_count actual runtime source not certified by init_files/dup_fd definitions."
  - "Full/empty scan reads unassigned unsigned fd; no defined error code or actual machine result attested."
  - "No reservation write in alloc_fd; abstract two-selection collision lacks real concurrent runtime provenance."
  - "fd_install has no local index bound; BUG_ON active macro binding and actual CPU result not certified."
  - "do_sys_openat2 does not invoke put_unused_fd and no slot reservation proven, hence no concluded leaked reservation."
---

# 第40批 MANIFEST：第39批纠正和独立fd源码分析的双线交付

## F00｜第39批单项RETURN勘误

本轮完整读取65号、39回执及63号，保留R39-S01/S02；R39-01只作追加纠正，`CORRECTION_SUBMITTED_PENDING_MAINLINE`、`closure=PENDING_C04_REVIEW`。原第39批六件与作者评论完全保留不改。新正文F00明确旧`OC39-A069`的`::ENFILE`标签实际对应`EXDEV=18`第24行；正确单宏`ENFILE=23`在第29行，新引文`R39C-A001`，原错配文字`R39C-A002`。旧68条未重新认证，“69/69字节比对”不等于“69/69符号身份正确”。新单宏补齐`__FMODE_NONOTIFY`、OPEN_FMODE、FMODE_32/64BITHASH并区分八进制/十进制。旧报告正文、证据、MANIFEST、事实、评论的追加消费方式另存于新facts，**不以本执行者声明闭合**。

## F01–F03｜独立fd选择、安装和消费

仅确认已选`file.c`、`fdtable.h`与open调用：fd_array固定256个file指针；alloc_fd锁内查NULL并只给局部fd赋值，解锁后返回，未预留；零长度或满表时fd未赋值，不自动产生-EMFILE或-ENFILE。fd_install读屏障后以fps[fd]进行占用BUG_ON并写槽，没有本地idx<256门；put_unused_fd只有锁/解锁，无活动撤销写入。selected do_sys_openat2先getname有效性测试、再选择fd、fd>=0时查file，IS_ERR(f)时覆盖fd为错误而不安装；有效file需要历史UV04/SB38未闭合条件，不把测试用辅助输入说成实机成功。

全局init_files的fd_count=0和dup_fd新对象的fd_count=256只说明两种静态来源，不能自动证明某一任务current->files或其它槽位初始化。历史UV-11已经登记表满未赋值；US-02记录f_dupfd上界，US-03记录CLOEXEC；OF30-F01记录build_open_flags返回被覆盖，以上不重复列为新的独立根因。新FD40-F01..F03只是条件性延期，不等于三个实际漏洞；关系图的双扫描示意不等于实机竞态认证。

## F04｜只限六件与P9作者侧检查

1. `MANIFEST.md`：本文件、身份、固定输入、金丝雀、分类计数、重试日志、STOP。
2. `fd-allocation-install.md`：F00完整39勘误 + F01–F03独立fd链 + F04执行边界。
3. `facts-and-boundaries.yaml`：JSON兼容YAML1.2，R39-01状态/旧标签消费、22对象、13转移、8返回类别、26有限案例。
4. `fd-allocation-install-paths.dot`：只画40独立fd控制/数据/分析前提，53显式节点62条边/零未定义端点；不重画39图。
5. `deferred-findings.yaml`：新FD40-F01..F03及12个原qualified_id的具体继承与反证。
6. `evidence.md`：新R39C 8段和新FD40 47段，**合计55段**单定义体1–5行，来自9个固定time源，不混旧69个标签。

已实际从固定time再次读取9份源，55段逐条对照**55/55字节一致、定义标签与单宏身份逐个核查**。P9-1六件文本中不含完整40位Git提交SHA；P9-2/4/13为固定源单定义短引文、宽度和精确对应；P9-3本轮输入/继承分栏；P9-5三数`verified_claims=55`、`quotes_reconfirmed=55`、`downgraded_to_inferred=0`仅指新55段，不再次担保旧39全部69条；P9-6外部私有研究不适用；P9-7/8实际双金丝雀和conventions逐字公约；P9-9/10/12无内核/用户程序/仓库脚本/patch，NOT_APPLICABLE；P9-11**完整本地P9与仓库C/ASM/编译/运行NOT_RUN**，局部结构/文本/有限整数模型不冒充实际执行。

只可在现有PR17执行分支`claude/dazzling-cori-q0dnyt`的`user-fd-allocation-install-40/`下新增上述六件及本人PR17交付评论；旧件、主线lead/PR16、master/time、PR标题/body/Draft/他人评论全部冻结。不得新建竞争分支/PR、合并关闭强推删分支、进入阶段3；不操作Owner机器、libc、镜像、凭据或额外付费API。正式提交前重新核对expected-head`4ea455865a47`、新目录缺席及PR16零交集；使用非强推force=false。之后新固定提交与活动分支分别全文+blob读六件，compare仅1个提交六件A新增无旧件净改删；本执行者本人评论应**分别**申请R39-01文档闭合及第40批独立验收，写回确认后`STOP_WRITING`，不自行开始41。
