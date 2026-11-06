---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-INIT-PATH-SWITCH-47
record_type: bounded_executor_delivery_manifest_with_R46_document_errata
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
produced_by: ChatGPT普通对话（Owner声明GPT-6＋Extra；实际后台身份未独立认证）
product_surface: ChatGPT普通对话／GitHub连接器
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-06
read_channel: connector
base_snapshot: time（源码分支）
source_pin_short12: a039d9803ade
contract_short12: df273b3b9519
result_base_short12: c176130f0473
write_branch: claude/dazzling-cori-q0dnyt
write_pr: 17
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
prior_batch46_verdict: RETURN
prior_batch46_returned_scope: [R46-01]
R46_01_state: OPEN_PENDING_INDEPENDENT_C04_REVIEW
R46_01_executor_request: DOCUMENTATION_CLOSURE_REQUESTED
batch47_acceptance: PENDING
runtime_evidence: NOT_RUN
phase: 2
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  options_flags_cmake: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  panic_c: "\tthis_cpu = smp_processor_id();"
self_check:
  R46_errata_verified_claims: 6
  R46_errata_quotes_reconfirmed: 6
  R46_errata_downgraded_to_inferred: 0
  IP47_new_verified_claims: 49
  IP47_new_quotes_reconfirmed: 49
  IP47_new_downgraded_to_inferred: 0
  old46_historical_selfcheck_59_whole_revalidated: false
write_retry_log:
  policy: 63-github-write-retry-and-readback.md
  status_before_final_publish: not_needed
  observed_transient_GitHub_write_errors_before_final_publish: 0
  local_prewrite_source_evidence_qa_corrections: true
  actual_remote_commit_ref_comment_and_retry_results: document_only_after_execution_in_PR17_formal_delivery_comment
---

# 第47批｜旧46文档定点闭合申请 + 初始化路径切换新取证

本对话沿用原MYOS2-A-C04b/CHAT-EXEC-05身份及已完成的67交接，不新建执行对话、PR、分支或跨项目工作。业务权威仍为Owner本轮明确指令、MyOS2 00工作令/公约、固定73号任务书和主线46回执。Mnemosyne只继承此前适用行为指导，不用于扩大仓库写区。GPT-6＋Extra为Owner所选执行配置，不将实际后台模型身份猜测为已验证事实。

## N00：输入、旧RETURN和本轮读证据

**本轮从固定主线提交df273b3b9519全文读取：**
- `agent-workspace/lead/MYOS2-LEAD-002/73-user-init-path-switch-contract.md`；
- `agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FS-PATH-ACCESS-46-review.md`，完整消费`R46-01`的明确关闭条件及`R46-S01/R46-S02`；
- `agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md`；
- 为46补充限定又全文回读`reviews/CORE-USER-FS-CONTEXT-45-review.md`中的R45-S01/R45-S02。46独立主线当前裁定仍**RETURN，仅R46-01文档退回，OPEN**；本批提交修订不等于执行者自行关闭。

**固定PR17旧证据定点读取：**第46批全部六件，特别是`evidence.md`旧A050–A053、`facts-and-boundaries.yaml`旧G14/C01/C04/D09、`fs-path-access-paths.dot`旧T4/T5/C3、原MANIFEST自查与报告；第46批正式交付评论6676575935、C04发布评论6676999392均全文核。已定位第29批`PN29-F02`、第31批`PI31-F05`、第32批`LC32-F04`及第05批UV-01/UV-10等历史编号，用于去重，旧问题状态均不由本批更改。

**继承且本轮未再次全文读取的材料：**本C04b最初已完成的67接收、MyOS2 00工作令、agent-workspace/README、conventions.md、tasks/00-gpt-task-protocol-v2.md P1–P13、18/24/28和适用Mnemosyne指南；本轮仅为P6精确原句及双金丝雀单独再读conventions/相关源码，不冒称全套规则已重新载入。

**新增固定time直接源：**`mykernel/fs/vfs/init.c`的完整init_chdir/init_chroot与init_mount定点，`mykernel/fs/vfs/namei.c`的kern_path、filename_lookup、path_lookupat输出门及getname_kernel必要定点，`mykernel/drivers/base/devtmpfs.c`的devtmpfsd全体、`include/linux/fs/namei.h`两个LOOKUP单宏、`include/uapi/linux/fcntl.h`的AT_FDCWD。为46文档勘误另读`include/linux/compiler/myos_debug_option.h`、`sched/misc/fs_struct.h`及`fs/vfs/namespace.c`和`include/linux/fs/dcache.h`受影响真实定义。取证总计9个源文件，均两轮连接器固定time全文读取对照。第47批DOT绝不重新画第46批路径访问图。

## N00：双金丝雀、分支及单写者现场

- 固定内核`time@a039d9803ade`；direct master=e8e60cd19540。主线PR16/agent/MYOS2-LEAD-002在任务发布后为df273b3b9519；原PR17/claude/dazzling-cori-q0dnyt首次执行写前预期`c176130f0473`。PR16/17均Open/unmerged、PR17 Draft，开放PR只有16/17；PR16本轮188个变化文件与47新目录零交集；47目录在原父提交尚不存在（预期404非拒写）。本连接器push权限已读为true。
- 固定time的两条逐字分支金丝雀：

```text
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	this_cpu = smp_processor_id();
```

master同名行的`-mcmodel=large/-fPIE`与`raw_smp_processor_id()`不同；该对照不代表真实编译选项已认证。
- MyOS2公约§1第2条本轮原句：

> 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。

## N00勘误与N01–N04新分析分区

| 本批只新增的文件 | 46追加文档勘误 | 47新分析及边界 |
|---|---|---|
| `init-path-switch.md` | 开首独立R46-01旧位置→不足→修正锚→跨件/图/MANIFEST解释及DEBUG三种条件真值表 | 完整报告内核init_chdir/chroot、kern_path局部输出、devtmpfsd返回消费 |
| `facts-and-boundaries.yaml` | 9组归一化支持映射，独立单宏/返回类型/检索方法与旧59数字限制 | 40个有限输入/结果/stop模型，来源、flags和错误消费字段 |
| `init-path-switch-paths.dot` | **不承载46勘误控制图** | 仅47新链，58个显式节点、70条边、零未定义端点；SOURCE/GOTO/CALL/RETURN/ANALYTIC分层 |
| `deferred-findings.yaml` | 原46仍RETURN、R46-01 OPEN，单独申请C04文档闭合 | 唯一新增候选IP47-F01：devtmpfsd忽略两个目录切换int结果；非同义历史保留 |
| `evidence.md` | 独立`R46-C001..C006`6段新纠正性短引文，原46旧59标签不重核 | 独立`IP47-A001..A049`49段新取证；两组分别二轮回源 |
| `MANIFEST.md` | 本入口分别声明勘误范围和仍未CLOSED的状态 | 记录实际读入、预检、P6自查、NOT_RUN及STOP门 |

**R46-01定点处理重点：**A050/A051分别以单宏R46-C001/C002取代混引，#ifndef/#if条件另列CONTEXT；A052/A053的void由R46-C003/C004两行真实签名补证；G14使用宏定义+条件上下文而非getter函数体片段，C01引用R46-C005的打印与while实际语句；C04只使用真实GitHub默认分支代码搜索的**方法记录**，不能靠namespace引文证明全仓无getter调用者；D09由R46-C006及原结构定义定位d_lockref注释，不把它偷换成dget/dput的全部语义。旧46正文/facts/图/MANIFEST/评论统一以新增矩阵及R46-S01/S02读取；旧59/59保留历史申报但不追认全部合规，未重核原余项。

**第47批新增结论限定：**内核init_chdir/chroot对kern_path非零先直接return，0且path有效时分别设置pwd/root并请求path_put。devtmpfsd仅检init_mount结果，不捕获init_chdir/init_chroot的返回；/..与.字面值不证明实际遍历成功。pathname_get/lookup输出与int错误、有效path及引用寿命分开；已读dput空体不执行真正释放，完整路径链及completion/工作循环保持opaque。

## 自查、实际门与禁止项

```yaml
R46_01_document_errata_self_check:
  verified_claims: 6
  quotes_reconfirmed: 6
  downgraded_to_inferred: 0
  original_46_59_quotes_full_replay: NOT_RUN
  original_46_verdict: RETURN
  executor_cannot_close_R46_01: true
IP47_new_source_self_check:
  verified_claims: 49
  quotes_reconfirmed: 49
  downgraded_to_inferred: 0
  source_pin: time@a039d9803ade
  method: 9份固定time源文件两轮连接器读取，单定义1–5行逐字/符号身份和支持力核对
additional_checks:
  facts_JSON_parse: PASS
  deferred_JSON_parse: PASS
  finite_models: 40
  DOT_nodes: 58
  DOT_edges: 70
  DOT_undefined_endpoints: 0
  Graphviz_render: NOT_RUN
  external_YAML_parser: NOT_RUN
  full_local_P9_gate: NOT_RUN
  original_source_file_byte_materialization: NOT_RUN
  preprocessing_compilation_kernel_scripts_QEMU_hardware: NOT_RUN
  real_init_mount_chdir_chroot_devtmpfsd_runtime: NOT_RUN
  full_mount_permission_thread_scheduler_allocator: NOT_RUN
  extra_paid_API_or_model: NOT_USED
```

两组源标签和旧46历史59分开，不相加声称整体源码安全。验证一个文本片段或条件模型不证明相关函数在机器上正常返回、目录/挂载/权限语义生效、引用已经获取或物理回收。成功路径每一步须合法对象、下游正常返回；STOP只是分析有效前缀，不是源码返回或CPU停机。

## 63号安全写入及交付后的状态

仅授权原PR17执行分支`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-path-switch-47/`中六件新文件及本人新顶层PR17交付评论。旧46六件、此前其它成果和评论、lead/PR16、master/time、PR标题/body/Draft不修改，不开竞争分支/PR、不合并关闭/强推/删分支。使用expected-head=`c176130f0473`、force=false；对未知并行写入停当前写动作不覆盖。GitHub暂态故障先回读确认远端是否已生效，再依63号间隔安全重试复用已有blob/tree/commit，评论响应不明先读评论查重；无故障记not_needed。本文是**发布前**文件，最终SHA、净diff和本人交付评论ID、实际重试细节只在实际成功后用本人PR17正式评论记录，不提前预言成功。

本执行方只申报`R46-01`文档闭合申请（旧46仍RETURN）以及第47批独立`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`；正式评论及远端核验成功后STOP_WRITING，交MYOS2-A-C04主线分别验收，不自行启动48；不修内核、不生成补丁、不编译运行或实施阶段3。
