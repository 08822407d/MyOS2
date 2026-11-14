---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PATHNAME-COPY-29
record_type: bounded_executor_delivery_manifest
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
executor_display_name: MYOS2-A-C03c 代码分析执行续建
mainline_reviewer: MYOS2-A-C04 内核分析主线续建
mainline_branch: agent/MYOS2-LEAD-002
mainline_pr: 16
execution_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
owner_model_per_owner: GPT-6
owner_effort_per_owner: Extra
model_backend_identity: unknown_or_not_attestable
date: 2026-11-03
read_channel: GitHub_connector
phase: 2
source_pin_short12: a039d9803ade
resume_51_short12: 2a0a66faa76d
original_49_short12: 324927712a98
review28_short12: 0a94f32ad00e
results_expected_base_short12: 427604c818a5
master_observed_short12: e8e60cd19540
lead_observed_short12: 2a0a66faa76d
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pathname-copy-29/
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_change_authorized: false
old_files_modified: false
owner_decisions_requested_now: []
startup_rule: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
source_canaries:
  options_flags_cmake: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  panic_c: "\tthis_cpu = smp_processor_id();"
self_checks:
  quoted_anchors: 68
  fixed_source_files_for_anchors: 14
  second_fixed_connector_read_and_exact_match: true
  quote_excerpt_lines_1_to_5: true
  verified_tag_count: 68
  facts_objects: 16
  facts_return_and_resource_models: 24
  facts_paper_models: 10
  graph_nodes: 57
  graph_edges: 105
  graph_id_and_callable_endpoints_checked: true
  deferred_new_conditions: 4
  historical_qualified_id_additions: 6
  JSON_documents_parse: true
  YAML_1_2_parser_executed: false
  validation_means: Code Mode JavaScript, GitHub connector, JSON.parse, finite arithmetic, text/graph endpoint checks
  full_40_hex_SHA_in_six_reports: 0
  source_C_ASM_or_repository_scripts_executed: false
  kernel_preprocessing_build_QEMU_hardware: NOT_RUN
  final_git_commit_compare_and_double_branch_readback: "pending at artifact creation; actual results must be in own PR17 delivery comment"
---
# 第29批 MANIFEST｜用户与内核路径名的取得、复制和filename局部生命期

## 起点、暂停解除和单写者核验

本批是C03c/CHAT-EXEC-03已完成25–28后续第5个有界执行包。Owner在同一执行对话中明确发送“继续执行CORE-USER-PATHNAME-COPY-29”、51号固定入口及交付转C04要求；因此由Owner直接释放此前第29批交接暂停，仍须现场预检。模型选择按Owner已报告的GPT-6＋Extra继续；后端真实内部身份不可由该普通聊天独立认证。没有重新执行首次45号接收、没有新建执行对话、没有重新审核28或开启第30批。

已完整读取固定51号恢复说明`2a0a66faa76d`、原49号任务`324927712a98`及原49指定在`0a94f32ad00e`的28号主线验收回执；依R28-S01/S02/S03消费`PASS_PENDING_LOCAL`与`returned_scope=[]`的限定。51号将49的验收对象从已退出的C03改为C04，不改P00–P04技术内容、六件名称和禁令。也回读了C03→C04的固定交接确认，并以PR16/17及实际ref核实C04 lead已续写；只写PR17本批六件及本人交付评论，不接触lead/PR16。

固定源码`time@a039d9803ade`；先完整读`mykernel/scripts/options_flags.cmake`及`mykernel/debug/panic.c`金丝雀和约定唯一写区。写前直接ref：master`e8e60cd19540`、time`a039d9803ade`、lead`2a0a66faa76d`、执行头`427604c818a5`。开放PR只有16与17，17为open/Draft且未合并，PR16改动路径与本批新目录无交集，`user-pathname-copy-29/`在实际执行分支不存在。上述只是已见远端现场，不能证明没有别的会话尚未推送草稿；首次快进写之前仍须使用expected-head租约并复核目录。

## 恰六件交付清单

| 文件 | 所装有界结果 |
|---|---|
| `MANIFEST.md` | 本文件：输入身份、P00–P04、自查与正式交付停止线 |
| `pathname-copy.md` | 两种getname的长度/分配/复制/NUL及所选sys_open、sys_execve、open_exec对象释放账 |
| `facts-and-boundaries.yaml` | YAML1.2兼容JSON映射：16对象、24种带前提的返回/资源模型、10个纸面值例 |
| `pathname-copy-paths.dot` | 57节点105边，独立标示函数调用、宏展开、返回来源、数据、赋值、释放尝试 |
| `deferred-findings.yaml` | PN29-F01..F04四项条件候选、六个历史qualified_id证据补充；旧记录不改 |
| `evidence.md` | 68段固定time逐字1–5行引文、14源文件、实际检查方法及未做范围 |

## P00–P04覆盖状态

| 阶段 | 执行结果 | 未闭合条件 |
|---|---|---|
| P00 | COMPLETED | 固定任务/回执及R28-S01/02/03、GitHub写权/写区/金丝雀和历史同根问题，未重复审28 |
| P01 | COMPLETED_STATIC | `__myos_getname/getname/getname_kernel/putname`本体与字符区、两入口顺序、unsigned转换、错误指针与2级分配；未证物理释放 |
| P02 | COMPLETED_WITH_CONDITIONS | `strlen_user/strncpy_from_user`、选定通用`strlen/strncpy`与x86 access_ok、NUL来源、同一源两次观察；无实际用户指针测试 |
| P03 | COMPLETED_WITH_UPSTREAM_LIMITS | `sys_open/sys_execve/open_exec`和`kernel_execve/filp_open`接口对照、有效filename借用及putname条件；旧UV-04下层缺return未闭合 |
| P04 | DOCUMENTS_QA_PASS_PENDING_GIT_WRITE | 六件Git blob读回、JSON文本/算术/引文/图端点核查已完成；实际提交、远端compare、双ref正文回读须下一步执行并记录PR17交付评论 |

## 实际源与检查工具

本轮固定time的直接引文共14文件：`fs/vfs/namei.c`、`arch/x86_64/myos/utils.c`、`lib/string/string.c`、`include/linux/fs/fs.h`、x86的`asm/uaccess_64.h`及`asm/uaccess.h`、`include/uapi/asm-generic/access_ok.h`、`mm/kmalloc/slub.h`、`include/linux/kernel/err.h`、`fs/vfs/open.c`、`fs/syscall.c`、`sched/forkexec/exec.c`、`include/uapi/asm-generic/posix_types.h`和`arch/x86_64/include/obsolete/arch_proto.h`。有少量导航用仓库默认分支GitHub文本搜索，但**所有正式引文均从固定time再次回读**，逐字68/68一致且每段含空行总计不超过5行。旧28的EI28-F01/F05、05::UV-04通过固定PR17旧交付读取，保留原ID，不冒称重跑全部历史事实/测试。

自查使用本对话代码执行环境里的Code Mode JS、连接器反查Git blobs及固定源码、JSON.parse对YAML1.2兼容JSON文本做结构检查、BigInt作少量64位无符号算术、图ID/函数调用端点/引文引用验证；**没有**调用外部YAML解析器、额外收费模型/API或在内核源码中执行测试工具。机械一致不证明动态调用实际成功，也不表明C04已独立验收。

未做：修改源码/补丁或用户程序、运行C/ASM/仓库脚本、预处理/编译、QEMU/硬件、真实用户指针样本、OS文件挂盘、读取凭据、仓库外libc/镜像、全面path_openat/权限/符号链接/页表/异常恢复分析、全仓延期问题去重。不能凭`kfree`证明对象实际被物理回收，也不能凭`access_ok`证明合法地址已映射。

## 后置验证和STOP_WRITING

仅允许从预期起点`427604c818a5`以正常非强推快进新增六件到原PR17执行分支，能用expected-head即使用，出现真实冲突停止相关写入，不覆盖/回退或新建竞争PR/分支。最终固定提交与活动分支各全文读回六件及blob，再执行compare确保仅本目录六件A新增、恰1提交、零历史净修改/删除及未知路径。实际短提交、回读与状态写本执行者PR17交付评论，避免正文在提交前编造自身SHA。

主线验收对象**「MYOS2-A-C04 内核分析主线续建」**，非旧C03。正式交付评论回读后`STOP_WRITING`，不自行启动30。阶段2上限`PASS_PENDING_LOCAL`，本报告只请求`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`，不得申称内核运行PASS。
