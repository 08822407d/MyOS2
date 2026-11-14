---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FORK-FILES-ROLLBACK-44-REVIEW-001
record_type: independent_bounded_static_acceptance_with_mandatory_supplements
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-06
produced_by: ChatGPT本主线独立复核
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（沿用Owner报告）
backend_identity: unknown_or_not_attestable
read_channel: GitHub连接器；本地仅纯文本与自建有限模型
source_pin_short12: a039d9803ade
reviewed_pr: 17
reviewed_base_short12: 9cd9e0ab4d03
reviewed_head_short12: 2d2bcab1348e
reviewed_delivery_comment: 6660650694
contract70_short12: 2cabad52880a
lead_before_publication_short12: 2cabad52880a
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R44-S01, R44-S02]
original_artifacts: PRESERVED_UNMODIFIED
runtime_evidence: NOT_RUN
full_local_P9_gate: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_original_57_quote_automatic_byte_scan: NOT_RUN
reviewer_original_facts_deferred_JSON_parse: NOT_RUN
reviewer_DOT_identity: connector_full_text_reconstruction_matches_remote_Git_blob
reviewer_DOT_blob_sha: f7ab93b5d81f9e4cc7c775de6ffbce50aadf141f
reviewer_DOT_bytes: 9430
reviewer_graph_structure: {explicit_nodes: 60, unique_nodes: 60, edges: 74, undefined_endpoints: 0}
reviewer_Graphviz: NOT_RUN
reviewer_independent_finite_models: 30
self_check: {scope: two_supplementary_definition_quotes_in_this_receipt, verified_claims: 2, quotes_reconfirmed: 2, downgraded_to_inferred: 0, method: 固定源码连接器文本人工对照，不是57条原引文自动重放}
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
next_followup: CORE-USER-FS-CONTEXT-45
next_task_ref: 71-user-fs-context-contract.md
executor_rotation: false
execution_branch_modified_by_mainline: false
owner_decisions_requested_now: []
phase: 2
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
---

# 第44批独立验收：fork文件表取得状态与失败清理

## 1. 裁定、交付身份及增量

依据固定70号、43主线回执R43-S01/S02及第44批完整六件，裁定PASS_PENDING_LOCAL，无另行返工项。
原六件及评论6660650694保留，后继必须合读本回执R44-S01/S02；不签完整fork、资源生命周期、物理释放或内核运行正确性。
执行身份为现有C04b/CHAT-EXEC-05，交付申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及STOP_WRITING。
停写申报不等于主线证明另一聊天物理终止；独立验收依据固定工件、远端差异与回源。

独立compare 9cd9e0ab4d03→2d2bcab1348e：ahead=1、behind=0、恰一提交，仅本批限定目录六件A新增1795行。
MANIFEST125、正文83、facts724、DOT139、deferred157、evidence567；旧文件零净修改/删除，无写区外路径。
主线现场2cabad52880a、执行2d2bcab1348e，PR16/17均Open/unmerged且PR17 Draft；direct refs核time a039d9803ade、master e8e60cd19540。
已知两PR开放且开放列表第三项为空。主线43既有裁定及更早限定保持，UV/US/FT/CL源码候选不自动关闭。

## 2. 独立回源支持的主要结论

copy_process先以dup_task_struct取得p；在node允许分配且前序正常的路径中，*tsk=*orig预置files指针，不为表增加引用。
copy_files旧表NULL时正常返回0；共享分支增加旧表refcount；非共享正常成功才发布新表。
外层表分配失败时dup_fd以NULL和int输出-ENOMEM返回，copy_files将错误交回；内层file分配NULL失效不是正常错误返回。

copy_files返回非零后goto bad_fork_cleanup_semundo。该标签在bad_fork_cleanup_files之后，因此不执行这个显式exit_files调用：

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_files:
	exit_files(p); /* blocking */
bad_fork_cleanup_semundo:
```

copy_fs返回非零则直接到files标签；copy_sighand失败先到fs；copy_signal到sighand；copy_mm到signal；copy_namespaces到mm；copy_thread到io；alloc_pid经IS_ERR/PTR_ERR后到thread。
从这些入口按实际标签贯穿，条件mmput、__cleanup_sighand、exit_fs、exit_thread等先行操作必须有效且正常完成，才可到达后续files归还。
copy_thread失败不先调用exit_thread；alloc_pid错误跳到cleanup_thread，不倒行经过free_pid标签。
成功return p阻断随后错误标签的自然落入；cancel_cgroup/put_pidfd/free_pid的存在不等于这些错误路径实际进入它们。

取得共享表引用后一次有效回滚，在稳定对象、正初值和无其它计数操作模型中为n→n+1→n。
新表初值1的归还则进入另一对象的1→0及有条件遍历，不能借共享模型掩盖旧UV-09的槽初始化、file副本和缓存来源缺口。
copy_files自己的正常失败没有新表持有；保留借来的指针值不要求本次额外put，也不单独证明泄漏或重复归还。

fork_out在当前sighand锁操作正常完成后返回ERR_PTR(retval)，kernel_clone仅在IS_ERR真分支用PTR_ERR消费。
本次还回读固定单宏以补充数值生产者：

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOMEM]
```c
	#define	ENOMEM		12	/* Out of memory */
```

deferred新增根因0项、FR44-O01..O03为三组有界观察，可以保留；这不是全仓没有新问题的认证。

## 3. R44-S01：持有、错误生产者与最早有效前缀

1. p非NULL不是有效task、有效files或已取得引用的检查。共享身份同时依赖先行结构赋值、orig/current一致、其间未改写files，以及后续copy_files正常完成。
   copy_files失败时必须区分外层正常NULL与内层失效；不可把后者补为-ENOMEM、goto或成功回滚。
   无效对象/索引/未定义行为的分析停止不担保优化后的机器程序必然保留之前每个源码副作用。

2. 前序dup_task_struct返回NULL可来自其自己的不同失败点。不得断言它一定没有执行过结构复制；可证明的是copy_process没有获得正常非NULL结果，也未调用copy_files。
   node不匹配时局部tsk未初始化的边界继续有效；kernel_clone所见NUMA_NO_NODE生产者不代表所有入口穷尽。
   copy_process分配前直接ERR_PTR(-EINVAL)的标志门不经过fork_out，也不发生文件表取得/清理；M32只是这个受有效args/current等前提约束的早退。

3. 每个后序非零检查是真实控制门，不自动证明callee实际生产某个错误值。copy_fs等的条件例不等于已经触发；先行opaque清理可更早失效或不返回。
   UV-14仍可能阻断sighand之后的fs/files路径。本轮未重审其全链，不把“标签相连”提升为“运行中已经归还”。
   本轮为45划界另读copy_fs和fs_struct相关定义，但不由这次预读追认所有callee的失败模型或预签45成果。

4. 无其它赋值且执行定义良好时，cleanup区没有对retval的活动重新赋值；set_kthread_struct失败分支保留此前-ENOMEM，sched_fork等分支取其实际返回。
   所谓opaque不表示任意函数可无依据改写未传址的局部retval；未知的是实际返回值、对象副作用及是否继续执行。
   PTR_ERR返回long，copy_process的retval是int，再交ERR_PTR(long)，kernel_clone返回pid_t；跨类型可表示性与目标指针编码模型须分开。
   在显式64位位模式模型中-4095..-1可识别，-4096、0、正数不能因此自动成为该区间的错误。不是所有C实现或完整syscall ABI的认证。

5. 局部n→n+1→n还要求增量可表示、计数操作正常和中间无额外计数/身份变化；不是每个file.f_count加减，也不是全局所有权证明。
   facts M14的2→1应明确为“进入回滚时表计数为2”；若2是共享前原值，完整模型应为2→3→2，不能混用初值时点。
   新表1→0不证明全部槽/file有效或物理回收完成。exit_files保存表指针在锁前；锁名称不证明整段并发协议。
   CL42-F01的静态表共享失败局部1→2→1只构成这一条条件路径的反证，不排除其它实际最后归还路径。

6. 只证明所选路径上显式exit_files是否被调用及其局部事件；未展开的task释放或其它callee不据此认定完全无额外资源动作。
   return p的正常路径没有落入错误标签，但不能把它表述成另一次自动增加引用的“所有权转移操作”。

## 4. R44-S02：图示纠正及证据消费

原图的P2同时概括dup_task_struct和成功结构复制，而P3/P4之后又画dup_task_struct NULL出口；不能按该图串行解释为先成功取得p、执行sched_fork后再报告p为NULL。
前序部分以后述顺序替代消费，后续标签区保持原图并合读限定：

```text
有效入口及早期标志门
  -> 在已限定node路径调用dup_task_struct
  -> if (!p): 直接fork_out（不先执行p字段/调度操作）
  -> p非NULL且有效: 完成所需前序
       -> 若kthread设置失败: cleanup_delayacct
       -> 否则调用sched_fork；非零: cleanup_policy
       -> 正常继续到copy_files
```

这不是要求修改原DOT，而是本回执对其聚合前序的规范解释；正文和facts中的正确分流继续有效。
P8→P9/P10/P11是已完成copy_files后的状态说明，不是返回后再执行一次计数增量/发布。
S0..S5表示对应函数调用及返回检查的聚合，非零并不等于已证某个errno；S6必须合读pid_struct!=&init_struct_pid的调用门。
alloc_pid未被调用或返回非编码错误不自动证明得到有效PID对象；后面的成功路径仍有条件。
L7→O4→X0只表示同一次exit_files调用的展开，不应计作两次。L12→O5前须合读真实WRITE_ONCE(p->__state,TASK_DEAD)，不是只调栈/任务释放。
O1是if(p->mm)条件块，条件假时不调mmput而继续；X5的源码判断是减后是否零，“最后引用”须另有合法计数模型。
K0→K1应按RETURN理解，不是copy_process主动CALL kernel_clone；错误消费者仅在对应调用返回后执行。
X9/K3等是可证性停止/未认证节点，不是源码return、异常处理或CPU停机；P9等虚线是条件数据关系，不绕过真实错误入口及中途调用。

A054标题“int错误码编码成指针”应精确读为ERR_PTR的long形参；int来自此调用者retval，不是函数签名。
标签/注释片段都须合读完整活动体，不能用标签名字证明资源清理或正常错误生产者。
原57/57仅保留作者自查申报；本主线完整阅读证据并定点回源，没有自动逐条重放57引文，不追认57个独立语义证明。
所有补充统一约束原正文、facts、DOT、deferred和evidence，不覆盖旧件。

## 5. 实际方法及未做事项

完整读取固定70号、43回执含R43-S01/S02、44六件及评论6660650694，长正文和回执尾部补读。
MyOS2工作令、公约、18/24/28、63号及适用Mnemosyne指导沿用本主线已加载记录，不冒称本轮全面重新刷新，不导入跨项目维护。
本轮直接回源：fork.c855–1433的完整copy_process及delayed_free_task、273–365的dup_task_struct、682–747中的copy_files、1444–1494的kernel_clone定点；file.c21–124；err.h全文；errno-base.h1–30。
fdtable/init_task及旧05/08的边界沿用此前同一固定源码/成果记录，不冒称本轮重放执行者全部历史工具调用。
time金丝雀options_flags.cmake38–44、panic.c77–81本轮重新读取。为45划界另读copy_fs、sched/misc/fs_struct.c全文、fs_struct.h与fs_struct_types.h；搜索默认分支只定位，语义回固定time。

连接器完整DOT重建9430字节，Git blob SHA与远端f7ab93b5d81f9e4cc7c775de6ffbce50aadf141f匹配；结构60唯一显式节点、74边、0未定义端点。
另执行30个独立纯Python有限模型：11个错误入口/标签贯穿、6个最早停止、4个共享计数、1个新表计数、1个NULL表、1个成功不贯穿、6个64位错误编码。
这不是作者35例全集重放，不是C/编译器/内核运行；未运行Graphviz、未机械解析原facts/deferred、未自动重扫57引文、完整本地P9仍NOT_RUN。
可选public raw下载DNS失败未取得该通道原件，连接器证据正常；DOT已由完整文本及Git对象哈希核验。此读取故障不是仓库写入或权限失败。
AI角色分离不是人工或已认证异构模型验收；无修复、编译、运行、额外付费API或阶段3授权。

后继71号CORE-USER-FS-CONTEXT-45继续交现有C04b，补copy_fs/exit_fs所操作的taskfs_s、root/pwd和局部用户计数，不重做整个fork或文件表分析。
固定成果：https://github.com/08822407d/MyOS2/tree/2d2bcab1348e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-files-rollback-44
固定任务：https://github.com/08822407d/MyOS2/blob/2cabad52880a/agent-workspace/lead/MYOS2-LEAD-002/70-user-fork-files-rollback-contract.md
交付评论：https://github.com/08822407d/MyOS2/pull/17#issuecomment-6660650694
C04仅新增主线回执/任务/检查点及维护既有PR导航，不写PR17执行成果；发布不等于自动投递或启动。
