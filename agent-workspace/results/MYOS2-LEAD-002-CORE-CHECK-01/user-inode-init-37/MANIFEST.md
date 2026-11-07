---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-INODE-INIT-37
record_type: correction_and_new_inode_executor_delivery_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
conversation_display_name: MYOS2-A-C04a 代码分析执行
mainline_owner: MYOS2-A-C04 内核分析主线续建
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-04
read_channel: connector
base_snapshot: time
source_pin_short12: a039d9803ade
execution_branch: claude/dazzling-cori-q0dnyt
result_base_short12: 3c02d6c020e3
lead_observed_short12: 345b997470f7
master_observed_short12: e8e60cd19540
contract61_short12: 345b997470f7
review36_short12: 345b997470f7
pr_execution: 17
pr_mainline: 16
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-inode-init-37/
allowed_files: [MANIFEST.md, inode-init.md, facts-and-boundaries.yaml, inode-init-paths.dot, deferred-findings.yaml, evidence.md]
prior36_verdict: RETURN
prior36_returned_scope: [R36-01, R36-02]
prior36_return_kind: DOCUMENTATION_ONLY
prior36_correction_status: CORRECTION_SUBMITTED_PENDING_MAINLINE
prior36_closure: PENDING_C04_REVIEW
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  options_flags_cmake: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  panic_c: "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 80
  quotes_reconfirmed: 80
  downgraded_to_inferred: 0
  correction_N36C_direct_quotes: 21
  inode_NI37_direct_quotes: 59
  quote_source_files: 10
  quote_width_1_to_5: true
  legacy36_original_quote_IDs: 56
  legacy36_historical_retained_not_rechecked: 53
  legacy36_replaced_old_IDs: 2
  legacy36_context_only_old_IDs: 1
  legacy36_old_dot_nodes: 45
  legacy36_old_dot_edges: 65
  correction_replacement_dot_nodes: 53
  correction_replacement_dot_edges: 81
  correction_replacement_dot_undefined_endpoints: 0
  new_inode_dot_nodes: 50
  new_inode_dot_edges: 60
  new_inode_dot_undefined_endpoints: 0
  correction_old_node_index: 45
  correction_old_edge_index: 65
  inode_field_source_matrix_rows: 14
  inode_object_identity_records: 10
  inode_finite_cases: 12
  new_deferred_findings: 4
  historical_qualified_links: 12
  full_40hex_SHA_in_artifacts: 0
  delivery_files_expected: 6
  full_local_P9_gate: NOT_RUN
  source_C_ASM_or_repo_scripts_executed: false
  preprocessor_compilation_QEMU_hardware: NOT_RUN
inputs_read_current_turn:
  - "agent-workspace/lead/MYOS2-LEAD-002/61-user-inode-init-contract.md @ 345b997470f7 [full]"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FAT-NAME-PARSE-36-review.md @ 345b997470f7 [full; R36-01/R36-02 and §5]"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fat-name-parse-36/{MANIFEST.md,fat-name-parse.md,facts-and-boundaries.yaml,fat-name-parse-paths.dot,deferred-findings.yaml,evidence.md} @ 3c02d6c020e3 [actual fixed files, correction target only]"
  - "PR17 own batch36 comment 6624419067 [actual body retrieved]"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FAT-DIR-ITERATOR-35-review.md @ d5b6bf140e03 [R35-S01/R35-S02 consumed]"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LOOKUP-CALLBACK-34-review.md @ 98608f6c58df [R34-S01/S02 consumed]"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-lookup-callback-34/deferred-findings.yaml @ bce8404efe6a [CB34-F05 targeted]"
  - "mykernel/scripts/options_flags.cmake @ time a039d9803ade and master [exact canary]"
  - "mykernel/debug/panic.c @ time a039d9803ade and master [exact canary]"
  - "mykernel/fs/vfs/inode.c @ time a039d9803ade [full new_inode, alloc_inode, inode_init_always, inode_init_once, address_space_init_once, __address_space_init_once; inode_init cache args]"
  - "mykernel/fs/fat/inode.c @ time a039d9803ade [fat_alloc_inode, fat_sops, fat_fill_super bound call, fat_build_inode, fat_fill_inode]"
  - "mykernel/include/linux/fs/fs.h @ time a039d9803ade [inode_s, addr_spc_s, super_ops_s, super_block_s fields]"
  - "mykernel/include/linux/fs/fat.h @ time a039d9803ade [msdos_inode_info_s and MSDOS_I]"
  - "mykernel/fs/vfs/namei.c @ time a039d9803ade [__lookup_slow selected direct consumer]"
  - "mykernel/fs/fat/myos_fat32_namei.c @ time a039d9803ade [FAT32_lookup selected direct consumer]"
  - "mykernel/include/linux/fs/fat32.h @ time a039d9803ade [LOWERCASE_BASE/EXT single macro correction]"
  - "mykernel/include/uapi/linux/msdos_fs.h @ time a039d9803ade [MSDOS_NAME macro and true msdos_dirslot_s fields]"
  - "mykernel/fs/fat/myos_dir.c @ time a039d9803ade [selected two parser definitions needed by N00]"
  - "mykernel/fs/fat/myos_fat32_iobuf.c @ time a039d9803ade [readent negative offset and return]"
  - "GitHub repo/PR16/PR17/open PR list/master/time/lead/executor branches, PR16 changed-path intersection, target directory absence"
inputs_inherited_not_reloaded:
  - "MyOS2@bf856f2ddb2d: agent-workspace/README.md,conventions.md,tasks/00-gpt-task-protocol-v2.md, lead/MYOS2-LEAD-002/{00-work-order.md,18-deferred-findings-and-resume.md,24-chatgpt-specialist-conversation-separation.md,25-chatgpt-executor-startup.md,28-review-and-next-batch-same-turn.md}"
  - "Mnemosyne@master: README.md,current/human-approved-spec.md,commands/receive-mnemosyne-handoff.md,commands/load-mnemosyne-guidance.md and previously loaded applicable guards. No current-turn blanket reload or Mnemosyne-maintenance authorization."
  - "Older R33/R32/R31, 30 DOC-FIX, UV-04/OF30 restrictions remain as previously consumed, not reaudited"
open_questions:
  - "R36-01 and R36-02 cannot be marked CLOSED without independent C04 mainline review."
  - "Actual sb binding to fat_sops on any given user path is not observed at runtime."
  - "Actual execution of inode_init_once/address_space_init_once for selected FAT kmalloc-allocated objects is not established; no allocator internals audited."
  - "First value and future uses of mapping.nrpages/page_array, private_idx_info and inode.i_devices remain conditional."
---

# 第37批 MANIFEST：N00追加勘误 + N01–N03独立静态取证 + N04交付

## N00｜第36批文档RETURN的纠正回件

`R36-01`和`R36-02`的本批作者侧状态均为`CORRECTION_SUBMITTED_PENDING_MAINLINE`；旧36总体判定仍然是**RETURN**，`closure=PENDING_C04_REVIEW`。第36批原六件、本人旧交付评论以及主线回执不修改。新`inode-init.md`首章给出证据ID限定、F02/F07新正确长槽来源、原56项索引引用和完整替代DOT代码块；`facts-and-boundaries.yaml`逐条列出旧56个ID的处置、45节点及65边的迁移映射、C18/C19分类、F03互斥前缀与NUL/初始计数例。新evidence将N36C-A001..A021作为**本轮21段直接证据**，不把旧56标签重新包装成已核证；旧A043仅条件编译上下文。37的DOT文件完全没有装36的替代图，防止错误合包。

## N01–N03｜inode路径的新发现与限定

读取`new_inode/alloc_inode/inode_init_always`的真实活动体和FAT的嵌入对象及回调来源，区别`sb->s_op`静态绑定、真实函数指针调用、NULL门、分配内/外层对象、每次字段赋值、一次性初始化函数的定义与未证实际调用。有效new_inode路径可明确建立`i_size=0`、`i_mapping=&inode->i_data`、mapping.host与default empty_aops；FAT目录分支没有重新写size/a_ops不等于缺少上游赋值。page_array/nrpages及private_idx_info在当前always与FAT alloc函数中仍缺直接首次值来源，需留待更深分配/消费证据。对CB34-F05只收窄，绝不自动关闭。

新`inode-init-paths.dot`只画37链，50个节点60条边；新NI37-A001..A059为59段固定time直接证据，不依赖36旧图。新NI37-F01..F04为普通有条件候选，并非四项已证独立漏洞。fat_fill_inode的完整所选正常返回为0，inode_init_always亦为0；周边非零检查属于防御分支，不能虚构正常非零生产者。指针无效或分配/回收未能正常返回时止于最早未证前缀，不能声称物理故障。

## N04｜六件清单、P9实际子集与STOP

- `MANIFEST.md`：精确输入、两线状态、双金丝雀、自查、STOP。
- `inode-init.md`：N00原36纠正专节（含完整替代36 DOT）及N01–N03独立分析。
- `facts-and-boundaries.yaml`：JSON兼容YAML1.2，R36两项待闭合状态、56旧引文ID、45/65旧图映射、14行字段来源矩阵与有限实例。
- `inode-init-paths.dot`：仅第37批新inode初始化链，绝不混装36纠正图。
- `deferred-findings.yaml`：新NI37-F01至F04及旧qualified_id，文档问题与源码候选分别存。
- `evidence.md`：N36C=21、NI37=59，两源计数隔离合计80段新直接源码摘录，全部固定time单定义体1–5行。

P9-1：六件不含完整40位Git SHA，仅列实际来源短号；最终提交值只能在真实远端返回后记于交付评论。P9-2/4/13：**80/80**新引文经第二次固定time回源对照逐字一致，历史56标签不是本次80的一部分。P9-3：本轮新读与继承规则精确分栏，未读范围和权限保留。P9-5：80个新增直接证据标签仅位于`evidence.md`，三数80/80/0；历史索引与DOC正文不再使用新VERIFIED标签。P9-6：无外部研究句柄。P9-7/8：双固定time金丝雀与master精确区别，工作区公约逐字句、自报模型与未知后台身份分离。P9-9/10/12：无新增脚本/内核补丁/include改写，NOT_APPLICABLE。P9-11：**NOT_RUN**仓库C/ASM/脚本、预处理/编译、QEMU/硬件、真实用户程序/文件路径；完整本地P9仍`NOT_RUN`。DOT端点及结构化数据检查不等于控制流或运行正确性。

唯一写区是原`claude/dazzling-cori-q0dnyt`/PR17下`user-inode-init-37/`六件及本人交付评论。全部既存文件、PR16/lead、master/time、内核/用户程序、PR标题正文/Draft/他人评论不动，不创建竞争分支/PR、不合并关闭强推删分支、不访问Owner机器/外部libc/镜像/凭据，不进入阶段3。正式提交前重新检查expected-head `3c02d6c020e3`、所有PR及目标目录不存在；只在租约一致时非强推快进。完成后必须固定提交及活动分支双全文/blob回读，并compare只新增六件；本人PR17评论分别申请`R36-01/R36-02`**待主线闭合**与37独立`READY_FOR_MAINLINE_REVIEW`验收，状态仍`PENDING`。交付后`STOP_WRITING`，不自行启动38。
