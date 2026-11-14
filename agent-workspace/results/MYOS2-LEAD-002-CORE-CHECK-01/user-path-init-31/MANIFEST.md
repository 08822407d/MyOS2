---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PATH-INIT-31
record_type: bounded_executor_delivery_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
conversation_display_name: MYOS2-A-C03c 代码分析执行续建
mainline_reviewer: MYOS2-A-C04 内核分析主线续建
produced_by: ChatGPT普通聊天
product_surface: "ChatGPT普通聊天；实际界面精确模型后端不可独立认证"
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-03
read_channel: connector
base_snapshot:
  source_branch: time
  source_pin_short12: a039d9803ade
  execution_branch: claude/dazzling-cori-q0dnyt
  previous_execution_short12: e33103ca7d9e
  lead_branch: agent/MYOS2-LEAD-002
  lead_observed_short12: 867eba5aaff9
  master_observed_short12: e8e60cd19540
  canonical_pr: 17
  mainline_pr: 16
contract54_short12: 867eba5aaff9
review30_docfix_short12: 867eba5aaff9
review30_docfix_disposition: PASS_PENDING_LOCAL
review30_documentation_closures: [R30-01, R30-02]
historic_RETURN_document_retained: true
prior_errata_required_short12: e33103ca7d9e
write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-path-init-31/
allowed_files: [MANIFEST.md, path-init.md, facts-and-boundaries.yaml, path-init-paths.dot, deferred-findings.yaml, evidence.md]
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
owner_decisions_requested_now: []
kernel_code_or_patch_modified: false
old_files_modified: false
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  options_flags_cmake: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  panic_c: "\tthis_cpu = smp_processor_id();"
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/54-user-path-init-contract.md @ 867eba5aaff9"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-OPEN-FLAGS-30-DOC-FIX-01-review.md @ 867eba5aaff9"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-open-flags-30-doc-fix-01/errata.md @ e33103ca7d9e"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PATHNAME-COPY-29-review.md @ 1412a1991aee"
  - "agent-workspace/tasks/00-gpt-task-protocol-v2.md @ 867eba5aaff9"
  - "agent-workspace/conventions.md @ master"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-vfs-baseline-05/user-vfs-baseline.md @ e33103ca7d9e"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-vfs-baseline-05/deferred-findings.yaml @ e33103ca7d9e"
  - "mykernel/scripts/options_flags.cmake @ a039d9803ade"
  - "mykernel/debug/panic.c @ a039d9803ade"
  - "mykernel/fs/vfs/namei.c @ a039d9803ade"
  - "mykernel/include/linux/fs/path.h @ a039d9803ade"
  - "mykernel/sched/misc/fs_struct_types.h @ a039d9803ade"
  - "mykernel/include/linux/fs/namei.h @ a039d9803ade"
  - "mykernel/include/linux/fs/fs.h @ a039d9803ade"
  - "mykernel/include/uapi/linux/fcntl.h @ a039d9803ade"
  - "mykernel/fs/vfs/open.c @ a039d9803ade"
  - "mykernel/fs/syscall.c @ a039d9803ade"
  - "mykernel/sched/forkexec/exec.c @ a039d9803ade"
  - "mykernel/include/linux/kernel/err.h @ a039d9803ade"
self_check:
  verified_claims: 80
  quotes_reconfirmed: 80
  downgraded_to_inferred: 0
  claim_scope: "80 fixed-time direct source quotations from this new batch; main report conditional deductions not independently runtime-proved"
  source_files_for_direct_quotes: 10
  second_connector_fixed_source_read: true
  definition_owner_and_quote_width_checked: true
  max_source_excerpt_lines: 5
  objects: 20
  conditional_input_return_models: 18
  paper_examples: 8
  stage_flow: 7
  graph_nodes: 65
  graph_edges: 88
  graph_endpoints_checked: true
  actual_function_call_source_checked: true
  new_condition_candidates: 5
  historical_bounded_links: 8
  JSON_facts_and_deferred_parsed_by: Code_Mode_JavaScript
  YAML_1_2_compatibility: "JSON syntax accepted as YAML1.2 mapping; no separate YAML parser"
  direct_C_ASM_or_repository_scripts_executed: false
  preprocessing_compilation_QEMU_hardware: NOT_RUN
  new_delivery_files: 6
  full_40hex_commit_SHA_in_new_files: 0
  remote_commit_compare_and_double_ref_readback: "pending at document creation; real results to be documented in own PR17 delivery comment"
---

# 第31批 MANIFEST｜nameidata路径起点与有限退出

## 输入、恢复、单写者及冻结范围（N00）

原C03c执行者`MYOS2-LEAD-002-CHAT-EXEC-03`沿用前期45号接收及Owner选择GPT-6＋Extra，继续原PR17，不另开执行对话或替换模型。最新实际后台模型身份与界面显示细节无法从连接器证明。54号合同及同一固定启动提交中C04的30-DOC-FIX-01复核已经全文读取；30原RETURN历史保留，R30-01/R30-02的文档闭合使30组合材料处于`PASS_PENDING_LOCAL`，**不等于内核运行或源码安全验收**。旧30证据消费必须合读固定勘误`e33103ca7d9e`，不重新把80条旧标记看成全部定义体合格；不重做29/30。

固定源码time`a039d9803ade`。本轮实际先通过GitHub连接器复读time两句配置金丝雀，并核master公约原句。源只在namei.c的nameidata/set_nameidata/path_init、两个直接调用者及link_path_walk前缀、有限退出函数，配套读取path_s/fs_struct/filename/LOOKUP常量与真实sys_open/open_exec参数；VFS05旧文件仅定点消费路径与UV-04历史问题，不重做整个子系统。协议v2 P1–P13已读，特别P2/P6/P9严格要求单定义体、短引文和真实自查。

写前所见master`e8e60cd19540`、time`a039d9803ade`、lead`867eba5aaff9`、执行头`e33103ca7d9e`；PR16与17开放，PR17仍Draft/unmerged，唯一目标`user-path-init-31/`不存在，PR16路径交集为空，GitHub权限push=true。现场时点不是全局排他锁，Git写前仍须使用expected-head检测竞争修改；遇未知实际头变化拒绝覆盖/强推。

## 恰六件（N01–N04）

| 文件 | 实际内容 | 创建阶段状态 |
|---|---|---|
| `MANIFEST.md` | 固定身份、输入清单、P9局部自查、主线验收和STOP线 | 已形成blob，Git提交及远端回读待实际核 |
| `path-init.md` | nameidata清零和起点、dfd/root/flags、两条调用链、prefix与资源边界 | 已形成blob |
| `facts-and-boundaries.yaml` | YAML1.2兼容JSON：20对象、7步流、18类输入返回矩阵、8个纸面例 | 已形成blob |
| `path-init-paths.dot` | 65节点、88条类型边，真实call/write、借入指针与条件结果区别 | 已形成blob |
| `deferred-findings.yaml` | PI31-F01–F05条件候选及8项历史证据/限制链接 | 已形成blob |
| `evidence.md` | 固定time的80段1–5行逐字引文，10个实际源码文件、归属回核 | 已形成blob |

| 子步骤 | 当前结果 | 必留边界 |
|---|---|---|
| N00 | COMPLETE | 固定54及30-DOC-FIX复核、原30勘误、金丝雀/写区、历史限制 |
| N01 | COMPLETE_STATIC | nameidata_s/path_s/fs_struct/filename_s身份，set_nameidata先清零后借入name/dfd、root条件赋值 |
| N02 | COMPLETE_WITH_CONDITIONS | path_init无条件current.fs.root、按s[0]选root/pwd并返回s；dfd/LOOKUP位不在此分支消费；Slash由link_path_walk后跳 |
| N03 | COMPLETE_WITH_UPSTREAM_LIMITS | do_filp_open和filename_lookup参数；path_openat先alloc_file再path_init；path_lookupat按条件输出path及清局部；restore/terminate/complete实际正文 |
| N04 | DOCUMENTS_QA_READY_PENDING_GIT_WRITE | 六件内容/证据/图/结构自查；真实提交、双ref回读和compare将在后续动作执行，不预签 |

## 可消费证据、自查和禁止边界

`evidence.md`的80个固定path::symbol直接证据标签是**本批直接源码引文**，每个有1–5行逐字片段，第二次打开同一固定time定义并按行段、符号实际体定位；原C04验收中的10个代表引文不算入本批80条。原batch30历史标签及勘误也不混计。对齐P9：检查40位提交串零命中、引文属于同一真正函数/结构体/宏定义、路径与行号有效、两条金丝雀及公约、实际标签计数80与复核80一致、强制对象和调用参数可定位；不声称图全部因机械端点检查就具备独立运行语义验证。

有限算术和八进制LOOKUP/dfd值由Code Mode JS检查；YAML文字采用合法JSON语法并由JSON.parse检查，未运行另一种YAML解析器。NOT_RUN：仓库程序/C/ASM、预处理/编译、QEMU/真实内核、用户实际路径、权限/符号链接/挂载、底层file/page引用和锁、Owner本机、外部libc/镜像、额外付费API或模型。未修复源码、未执行文件系统操作、未全仓去重或生成补丁。既有UV-04及OF30错误传播限制不取消。

## 后置提交、验收与停写

只允许在原`claude/dazzling-cori-q0dnyt`分支PR17，向`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-path-init-31/`新增指定六件和本人PR17交付评论。禁止修改原30六件、DOC-FIX纠正件或其它历史材料；禁止写lead/PR16、master/time、PR标题/正文/Draft或他人评论。首次写前预期`e33103ca7d9e`，使用非强推expected-head，冲突或不明写入停写，不另建竞争分支。

提交后须从**实际新固定提交和活动分支**逐件全文读取六件、比对正文/blob/head；compare起点至新提交仅一后继commit、六件A新增、零旧件净改/删除及未知文件。实际完成后短号与证据以本人PR17交付评论登记；文件正文在提交前不伪造结果。正式交付后`STOP_WRITING`，仅交**MYOS2-A-C04 内核分析主线续建**独立验收，`mainline_acceptance=PENDING`、上限`PASS_PENDING_LOCAL`，不得自行启动第32批。
