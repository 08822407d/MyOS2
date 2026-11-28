---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-INIT-PATH-SWITCH-47-REVIEW-001
record_type: independent_bounded_static_acceptance_with_mandatory_supplements
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: ChatGPT本主线独立复核
model_per_owner: gpt pro（沿用Owner报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-06
read_channel: GitHub连接器；本地仅纯文本与自建有限模型
base_snapshot: agent/MYOS2-LEAD-002
source_pin_short12: a039d9803ade
contract73_short12: df273b3b9519
reviewed_pr: 17
reviewed_base_short12: c176130f0473
reviewed_head_short12: 6f8408772ea7
reviewed_delivery_comment: 6679130532
lead_before_publication_short12: df273b3b9519
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R47-S01, R47-S02]
prior_R46_01: CLOSED_DOCUMENTATION_ONLY_BY_SEPARATE_RECEIPT
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
original49_automatic_byte_scan: NOT_RUN
original_facts_deferred_JSON_parse: NOT_RUN
reviewer_DOT_identity: connector_full_text_reconstruction_matches_complete_remote_Git_blob
reviewer_DOT_blob_short12: 3748e7ee0980
reviewer_DOT_bytes: 9385
reviewer_graph_structure: {explicit_nodes: 58, unique_nodes: 58, edges: 70, undefined_endpoints: 0}
reviewer_Graphviz: NOT_RUN
reviewer_models: {joint_session_total: 37, batch47_event_and_bit_models: 31, separate_closure46_macro_models: 6}
self_check: {verified_claims: 3, quotes_reconfirmed: 3, downgraded_to_inferred: 0, scope: 本回执三段直接源码引文, method: 固定连接器文本人工对照}
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
next_followup: CORE-USER-DEVTMPFS-SETUP-48
next_task_ref: 74-user-devtmpfs-setup-contract.md
executor_rotation: false
phase: 2
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
---

# 第47批限定接收：初始化路径切换与返回值消费

## 1. 裁定、身份和净差异

依据73号、原R46-S01/S02和R45限定，完整读47六件与评论6679130532，并独立回源，裁定PASS_PENDING_LOCAL，无另行返工项。
原六件及评论冻结，后续必须合读R47-S01/S02。R46-01的文档闭合另有回执，不用它代替本批验收。
本裁定不是用户态chdir/chroot、完整挂载、启动同步、路径安全、物理释放或内核运行通过。

compare c176130f0473→6f8408772ea7：ahead1/behind0，一提交，仅47新目录六件A新增1989行。
MANIFEST143、正文110、facts877、DOT133、deferred172、evidence554；旧件零净改删，无写区外路径。
PR16/17均Open/unmerged，PR17 Draft；主线df273b3b9519、执行6f8408772ea7；direct time a039d9803ade、master e8e60cd19540。
执行者READY_FOR_MAINLINE_REVIEW和STOP_WRITING是交付申报，不认证另一聊天物理终止；现场未见未知并行成果。

## 2. 核心静态事实获得支持

init_chdir与init_chroot是内核初始化内部函数，两者声明局部path后以相同FOLLOW|DIRECTORY位交给kern_path。
真实if(error)检查所有非零值；失败直接返回，不调用本层setter/path_put。例如：

[VERIFIED mykernel/fs/vfs/init.c::init_chdir]
```c
	error = kern_path(filename, LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &path);
	if (error)
		return error;
```

正常0路径中，chdir的第二个!error门之间没有活动重新赋值；chroot的权限/capability检查和goto均为注释，dput_and_out由正常顺序贯穿。
分别调用set_fs_pwd或set_fs_root，不由chroot名字推导同时修改pwd或实现完整用户隔离。void setter不生产本层errno。

kern_path把getname_kernel结果、AT_FDCWD、flags和输出地址交filename_lookup；后者先IS_ERR门，再set_nameidata/path_lookupat，正常返回后putname，再返回retval。
path_lookupat只有实际走到最终err==0门才复制path输出并清本地指针：

[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
		*path = nd->path;
		nd->path.mnt = NULL;
		nd->path.dentry = NULL;
```

这是路径结构值输出，不是新取得目录项/挂载持有，也不是所有下游语义已生效。
devtmpfsd检查init_mount的返回，但对下列两个int结果既不保存也不分支：

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
		init_chdir("/.."); /* will traverse into overmounted root */
		init_chroot(".");
```

所以在各调用正常返回的条件下，第一个非零不阻止第二个调用，第二个非零不因该值而阻止后继complete调用。
IP47-F01保留needs_evidence：局部丢弃返回事实成立，实际启动故障、非零路径可达性及用户影响未认证。
本轮定点读取05::UV-01：其已有根切换/exec返回问题不等于这里devtmpfsd两次目录切换结果消费；保留关联，不宣称全仓去重完成。

## 3. R47-S01：参数、输出、错误生产者和后继边界

1. init函数的局部path有存储但没有显式成员初值。&path不是NULL path参数；问题是输出成员是否已写出且可合法求值，以及所指对象是否可用于后续操作，三者须分开。
   setter调用前先要合法求值current->fs等实参。M03/M09不能笼统写成“任何无效current都先进入setter执行path_get”；实际失效可能更早发生在caller实参求值处。
   有效的path值中dentry=NULL与path本体无效不同，继续合读R45/R46空辅助体限定。分析STOP不担保未定义行为后的实际机器副作用顺序。

2. filename_lookup的IS_ERR为假不是NULL或活性检查；PTR_ERR到int的转换及编码范围须按已读固定err.h/R44限定解释。
   getname_kernel已有正常错误出口代码，例如合法name对象而名字缓冲分配正常返回NULL时可返回编码-ENOMEM；“未证明实际触发”不应被改写成“源码完全没有错误生产者”。
   对devtmpfsd两个固定短字面串，不可直接用超长名字模型证明其实际失败。上游对象分配失效也不能补成正常errno。
   正值返回只作接口控制条件模型，未据此声称当前callee产生正errno。

3. path_lookupat中err由link_path_walk赋值，complete_walk只在此前err==0时调用，最终输出门再检查err；lookup_last返回的是指针，不是同一个整数错误变量。
   正文“整数输出时点”应读为“path结构输出与整数返回的不同时间点”。输出写入先于terminate_walk和外层putname；输出已写不保证所有后继正常返回。
   flags传递、路径值复制和本层无path_put错误出口不单独证明目录验证、引用取得、泄漏或路径切换成功。

4. devtmpfsd的err是自己的局部整数，没有被两次切换结果重新赋值；不把未传址的局部值说成可被任意正常callee无依据改写。
   调用complete不等于已经证明等待者被唤醒或系统初始化完成；要进入work_loop还须complete正常返回。
   为48划界另读所选devtmpfs_work_loop完整活动体：外层while(1)，未见活动break/return。在所选源级、定义良好且内层调用正常返回的模型下循环继续，无已证正常函数退出。
   因而M36和原图D7→D8只是原opaque边界的反事实返回假设，不是当前实现已提供的正常出口；不推断实际调度、永远运行或机器停机效果。

5. 原IP47-F01和PN29/PI31/LC32/FS45/UV等历史状态不关闭；本次只接收静态材料。源码中注释的权限检查不是用户态可利用性证据。

## 4. R47-S02：一条补充返回边、图注与证据范围

原DOT缺少init_mount早退I4返回devtmpfsd的边。正文、facts M26/M29及源码的分流正确，后继必须追加以下规范消费关系，不修改原DOT：

```text
I4 --RETURN（仅匹配devtmpfsd中的init_mount调用，将非零ret交给err）--> D2
D2 --err != 0--> D3
```

原图结构计数仍为58节点/70边，不能把语义补边冒充已经写回了原图。图的字节或端点检查不能验证调用链完整性。
T5的“C01 source false”应统一替换为“C2的if(error)为假；其间无赋值；C4的if(!error)为真”。C01不是本图节点，第二个条件不能读成假。
C1/R1中的FOLLOW or DIRECTORY是位或LOOKUP_FOLLOW | LOOKUP_DIRECTORY，值为3，不是任选其一或逻辑||。
K6/K7合读path_lookupat入口先调用path_init；K8之前complete_walk有err==0调用门，不能由聚合节点补成无条件调用。
K13和各返回边只回对应调用点；C7/R7的正常结果为0，ANY int标注应结合C3/R3非零返回端点理解，不扩展0出口的值域。
I3/R2/K3/K8等只有源码谓词是真实门，valid path/name/可写输出是额外前提，不伪造参数验证；失效节点是尝试操作，不先认证同一赋值成功。

facts M11关于CAP/security注释须补读完整init_chroot的67–75行，A008单段不独立证明所有这些注释；原短引文本身仍属于正确函数。
M14合读A018/A020与实际getname_kernel正常返回条件，M19合读既有err.h及set_nameidata/path_init，不能只用一条历史字符串当全链证明。
M40是流程/文档状态记录，支持来源应是73号、MANIFEST、N00矩阵和独立闭合回执，不是C001–C006单独证明审批状态。
原40个模型记录中M40不属于内核输入执行模型；作者计数保留为记录条数，不宣传为40次内核运行或独立根因。
以上有界补充统一约束正文、facts、DOT、deferred及evidence；不另派执行返工，不覆盖原件。

## 5. 实际方法、未执行项及后继

本轮完整读取73号、47六件、6679130532评论；长正文和facts/evidence补读至尾；重新读取28号。R46/R45全部限定、63号和C04项目/Mnemosyne既有接管/加载记录沿用，不冒称本轮全部重新刷新。
直接回源：init.c1–82；namei.c100–165及547–654；devtmpfs.c379–405；namei.h32–46；fcntl.h91–101；另四份纠正源见独立闭合回执。
强制调用/返回、输出门和6段替代引文逐项回核。49条新证全文读完并在所列源段交叉核对，但未做原49条自动字节扫描或原facts/deferred机械解析；不冒充完整本地P9。
两项time金丝雀重新读取；默认分支搜索只供48定位，实际语义仍回固定time。05::UV-01定点回读，不做全仓去重。

完整DOT从连接器文本重建9385字节，完整Git blob SHA与远端3748e7ee0980所指对象一致；正则检查58唯一节点、70边、0未定义端点。Graphviz未运行。
实际执行37个独立纯Python有限模型：12个init切换、8个路径输出、10个工作入口消费、1个位或，以及闭合回执所属6个宏条件；不是执行者原40例重放。
模型中的opaque/反事实返回不认证callee的实际出口。没有C/ASM/仓库脚本、预处理/编译、QEMU/硬件、真实并发或额外付费API。
一次可选raw下载被下载工具前置条件拒绝，未取得文件、未绕过；不影响连接器证据，也不是GitHub写入失败。
为48预读devtmpfs.c的setup_done、devtmpfs_init和work_loop、forkexec_macro.h及部分包含导航；默认分支找到的completion.h在固定time未找到，不把默认分支实现当固定源码，也不以此否定本批结果。

后继74号CORE-USER-DEVTMPFS-SETUP-48继续交现有C04b，研究初始化调用者与工作线程之间的错误值、完成通知及返回消费；不展开完整completion/调度或挂载实现。
固定成果：https://github.com/08822407d/MyOS2/tree/6f8408772ea7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-path-switch-47
固定任务：https://github.com/08822407d/MyOS2/blob/df273b3b9519/agent-workspace/lead/MYOS2-LEAD-002/73-user-init-path-switch-contract.md
交付评论：https://github.com/08822407d/MyOS2/pull/17#issuecomment-6679130532
C04只新增主线回执/任务/检查点并维护既有PR导航，不写PR17成果；发布不等于自动启动下一聊天。
