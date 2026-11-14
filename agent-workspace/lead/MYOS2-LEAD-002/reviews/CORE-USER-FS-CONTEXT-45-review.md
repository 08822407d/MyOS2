---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FS-CONTEXT-45-REVIEW-001
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
reviewed_base_short12: 2d2bcab1348e
reviewed_head_short12: 207c87cda0e6
reviewed_delivery_comment: 6675892248
contract71_short12: 1a33d35f634c
lead_before_publication_short12: 1a33d35f634c
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R45-S01, R45-S02]
original_artifacts: PRESERVED_UNMODIFIED
runtime_evidence: NOT_RUN
full_local_P9_gate: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_original_55_quote_automatic_byte_scan: NOT_RUN
reviewer_original_facts_deferred_JSON_parse: NOT_RUN
reviewer_DOT_identity: connector_full_text_reconstruction_matches_remote_Git_blob
reviewer_DOT_blob_sha: 750f5938e6bf14098c9189c3ee02babb8238a75b
reviewer_DOT_bytes: 9538
reviewer_graph_structure: {explicit_nodes: 64, unique_nodes: 64, edges: 76, undefined_endpoints: 0}
reviewer_Graphviz: NOT_RUN
reviewer_independent_finite_models: 37
reviewer_integer_model: explicit_signed_32_bit_not_compiled_ABI_attestation
self_check: {scope: three_single_definition_quotes_in_this_receipt, verified_claims: 3, quotes_reconfirmed: 3, downgraded_to_inferred: 0, method: 固定连接器文本人工核对，不是55条原引文自动重放}
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
next_followup: CORE-USER-FS-PATH-ACCESS-46
next_task_ref: 72-user-fs-path-access-contract.md
executor_rotation: false
execution_branch_modified_by_mainline: false
owner_decisions_requested_now: []
phase: 2
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
---

# 第45批独立验收：fs上下文复制、共享与退出

## 1. 裁定、身份及净变更

依据固定71号、44主线回执R44-S01/S02和45完整六件，裁定PASS_PENDING_LOCAL，returned_scope=[]。
原六件及交付评论6675892248保留，后续必须合读R45-S01/S02；无另行返工项，不认证完整fork、路径/挂载生命周期、实际物理回收或内核运行。
执行者为现有C04b/CHAT-EXEC-05，申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING和STOP_WRITING；其申报不是本主线重放了旧执行会话全部工具动作的证明。

独立compare 2d2bcab1348e→207c87cda0e6：ahead=1、behind=0、恰一提交，仅限定新目录六件A新增1963行。
MANIFEST124、正文87、facts908、DOT145、deferred188、evidence511；旧件零净修改/删除、无写区外路径。
现场主线1a33d35f634c、执行207c87cda0e6；PR16/17 Open/unmerged，PR17 Draft。direct refs核time a039d9803ade、master e8e60cd19540；已知两PR开放且开放列表第三项为空。
44及更早裁定不变；FS45-F01/F02保持needs_evidence，历史PI31/CL42/UV/FT等不自动关闭。

## 2. 独立回源支持的核心结论

任务fs上下文、files文件表和file打开对象是不同身份；taskfs_s.users为普通int，不是另两类原子计数。
init_task.fs静态绑定init_fs；init_fs显式users=1、umask=0022。静态初值不证明运行中最后持有者、实际退出或缓存回收路径。

copy_fs共享分支先访问fs->in_exec；非零返回-EAGAIN且不增加users，为零才users++，本体不赋tsk->fs。
共享结果同址依赖有效的前序任务结构复制、orig/current相符及无中途指针改写，不是users++单独保证。
非共享分支在copy_fs_struct正常返回后先把返回指针写入tsk->fs，再判NULL；正常分配失败因此写NULL并返回-ENOMEM，不同于copy_files失败可能保留借用files指针。

copy_fs_struct外层分配NULL时不进入初始化块、未在函数体读取old成员；成功后依序写users/in_exec、初始化新锁，在旧锁之前读取umask，然后旧锁内复制root并path_get、复制pwd并path_get、解锁、返回。
root/pwd是内嵌path_s，复制其值不深复制mnt/dentry；新旧fs对象和路径中被指向对象必须分开。

exit_fs先保存tsk->fs，非NULL时才加任务锁和fs锁，清tsk->fs，执行kill=!--fs->users，解锁，再按kill决定是否free_fs_struct。
free_fs_struct依次请求path_put(root)、path_put(pwd)、kmem_cache_free；到达调用不等于已证明物理释放。
copy_fs正常失败goto cleanup_files跳过exit_fs；成功后更晚失败才可能经过exit_fs，再到exit_files。较早sighand等边界仍有效。

FS45-F01的静态init_fs来源与fs_cachep释放请求条件可以保留，但真实最后一次users归还未证。
FS45-F02在选定path_get/path_put中缺少活动mntget/mntput调用的事实成立；两端均缺席，不能据此直接宣布只增不减、泄漏或UAF。

## 3. R45-S01：对象、计数及路径实际效果的限定

### S01-1 分支、发布与参数前提

copy_fs的in_exec非零分支不读取或递增users；不能把正users条件放在该分支之前当成源码门。
非共享分支对tsk->fs的赋值发生在callee正常返回之后；callee内部旧对象访问失效时不能补出NULL存储、-ENOMEM或caller回滚。
M13的old invalid须收窄为参数值本身可被定义良好地传递、但所指对象不可读取；不能推广成读取未初始化/陷阱指针值也必定安全。
old=NULL且分配先返回NULL不读取old成员，与分配成功后访问old->umask失效，是两种不同路径。

### S01-2 users算术与所有权不能混同

exit_fs的int kill先被声明，首次赋值是kill=!--fs->users；正文“初始化局部kill”应据此理解，不存在更早的源码初始化语句。
在显式有符号整数模型中，0→-1、-1→-2本身不必溢出，但不提供合法正持有者语义。
M26的near minimum不能泛化成必然溢出：恰INT_MIN再减1越界，INT_MIN+1减1仍可表示。共享users++的溢出边界是INT_MAX；in_exec非零早退不执行该递增。
合法共享配平n→n+1→n要求增量可表示、同一对象、无其它写者、实际到达并正常完成清理，不是每个file或每条path的计数配平。
锁前取得fs指针、锁内修改和锁后free分别记录；锁名称不证明全部写者遵循同一协议。

静态init_fs未显式初始化的in_exec及root/pwd指针成员，在静态初始化语义下分别为0/NULL；不把这解释为所有指针表示的字节必然全零，更不推断之后运行状态。
0022是八进制umask值；所有运行中的路径有效性和最后持有者仍须另证。

### S01-3 path请求不等于引用效果；补充一层实际定义

45按任务边界把dget/dput作为未展开callee，这种取证边界可保留。为下一批划界，本主线额外回读同一固定time的两个直接实现：
include/linux/fs/dcache.h::dget只有活动return；例如以下同一函数内片段：

[VERIFIED mykernel/include/linux/fs/dcache.h::dget]
```c
	dget(dentry_s *dentry) {
		// if (dentry)
		// 	lockref_get(&dentry->d_lockref);
		return dentry;
	}
```

fs/vfs/dcache.c::dput完整体中的循环、计数及销毁逻辑全部为注释，没有活动操作。
因此在选定定义、合法求值的调用条件下，path_get/put→dget/dput并未实施目录项计数增减；结合已读包装层，挂载引用操作也未实施。
这些是局部活动体事实，不证明整个系统没有其它持有方式、目录项永久存活或实际已经泄漏。

path_s参数本体必须可读取；path=NULL与“有效path对象的dentry成员为NULL”不能混同。
后一种情形在这些选定dget/dput体中没有对dentry的解引用，不应仅因dentry为NULL就判定必然失效；这不是对任意未初始化、失效指针表达式或所有构建配置的安全认证。
原M15/M16/M28/M29以及图中的dget/dput不返回边，只保留为先前opaque边界的假设，不能当作已由当前空壳实现产生的正常错误出口。
path_get/path_put是void，不产生可被copy_fs_struct检查的errno；若参数求值失效则仍不得补出错误处理和机器副作用。
root/pwd同址仍有两次调用请求，但不能按函数名称把它们算成两次实际+1/-1或双重物理释放。

### S01-4 历史条目合读和候选状态

05::UV-10中“打开时path_get已取得路径引用、dentry与挂载点引用只增不减”的附带效果叙述，不能在该固定pin仅凭path_get名字成立；上述实际辅助体要求对此收窄。
这不撤销UV-10关于close绕过file.f_count的另一个已记录问题，不关闭其状态，也不覆盖旧报告。
31::PI31-F05讨论nameidata局部空清理/路径输出，不能与本批mount包装层缺口直接合并；FS45-F01也不是CL42-F01的同一对象。
FS45-F01/F02仍为needs_evidence；新补的局部辅助体事实不认证实际最后归还、完整挂载所有权或分配器物理反应。

## 4. R45-S02：证据与图示的规范消费

FS45-A004把前置struct声明与typedef混在同一直接定义片段；单独类型别名应消费以下一行，前置声明另列上下文：

[VERIFIED mykernel/sched/sched_type_declaration.h::taskfs_s]
```c
	typedef struct fs_struct taskfs_s;
```

FS45-A042的定义外注释不算变量定义体；直接定义收窄为：

[VERIFIED mykernel/sched/forkexec/fork.c::fs_cachep]
```c
kmem_cache_s *fs_cachep;
```

两个符号及所需核心语句真实存在，本回执归一化而不另派返工。55/55仍是执行者文本自查申报，不追认为55个独立单定义语义证明；其它重叠片段不另计根因。

图P1应合读copy_files返回0，而不仅是正常返回任意值；非零早已跳过copy_fs。
P4的skipping exit_fs仅描述P3错误直接跳入的路径；从P6经exit_fs、P7贯穿到P4时，不能再说从未执行exit_fs。
F2正users只是后续合法增量模型的前提，不应排除in_exec非零的源码早退。A1→A3的实际门仅非NULL，对象有效须另加。
F7代表正常callee返回后的单次赋值，不重复调用copy_fs_struct；F8非NULL同样不验证对象活性。
A5→A15应表示在尝试old->umask读取时停止，不能先认证已完成赋值再报告该读取非法。
E7是赋值表达式，E9才是解锁后的实际if(kill)分流；E19的所有权停止不是源码检查或错误返回。
A9/A12/E13/E15关于无效dentry和不返回的解释必须服从R45-S01-3的实际辅助体限定。
E20/E21是来源数据关系，不能绕过NULL门、计数和解锁直接执行free；E3/E10/E17回P7只适用于copy_process对应调用者，不是所有exit_fs的通用返回目标。
SOURCE、CALL/RETURN、条件数据和分析STOP仍须分开；STOP不是CPU停机指令或源码errno。
所有补充统一约束原正文、facts、DOT、deferred和evidence，不修改执行成果或旧评论。

## 5. 实际方法、限制与后继

完整读取71号、44回执全部R44-S01/S02、63号、45六件及评论6675892248，长正文末尾补读。
MyOS2工作令、公约、18/24/28及适用Mnemosyne指导沿用本主线已加载记录，不冒称全面刷新，不导入其它项目维护。
本轮主要直接回源：sched/misc/fs_struct.c全文；fs_struct_types.h全文；sched_type_declaration.h16–28；init_task.c113–123；fork.c702–747、54–61、1648–1673、1122–1145；namei.c255–290。
node前序、错误标签后半段、path_s定义和errno数值沿用此前同一固定源码已读材料；本轮没有重审完整fork。
历史去重定点回读05::UV-09/10/14/15附近及31::PI31-F05。为46划界另读dcache.h中的结构和dget、dcache.c中的dput、fs_struct.h的get_fs_root/get_fs_pwd，以及fs_struct.c的setter；只核所述局部，不预签46成果。
两项time金丝雀options_flags.cmake38–44及panic.c77–81已重新核对；搜索默认分支仅定位，语义回固定time。

完整DOT由连接器全文重建9538字节，完整Git blob SHA匹配远端750f5938e6bf14098c9189c3ee02babb8238a75b；正则结构检查64唯一节点、76边、0未定义端点，未运行Graphviz。
本地实际执行37个独立纯Python有限模型：12个copy分支、9个exit计数、3个共享配平、8个选定path辅助体事件、4个fork消费和1个释放请求门。
它们是人工转写的控制/整数模型，不是作者44例全集重放，不是原仓库C/编译器/ABI/运行认证。原facts/deferred未机械解析，55原引文未自动全量字节扫描，完整P9仍NOT_RUN。
一次可选raw下载在下载工具的URL前置条件处被拒，未取得该通道文件；未绕过或据此判断GitHub写入失败。正式证据始终为连接器，DOT以远端Git对象身份另核。
无内核/用户程序修改、补丁、仓库C/ASM/脚本、预处理、编译、QEMU、硬件、真实并发或额外付费API；AI角色复核不是人工或已认证异构模型验收。

后继72号CORE-USER-FS-PATH-ACCESS-46继续交现有C04b：独立核root/pwd读取和替换接口，以及直接路径/目录项引用辅助体的实际效果，不扩大为完整挂载生命周期。
固定成果：https://github.com/08822407d/MyOS2/tree/207c87cda0e6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fs-context-45
固定任务：https://github.com/08822407d/MyOS2/blob/1a33d35f634c/agent-workspace/lead/MYOS2-LEAD-002/71-user-fs-context-contract.md
交付评论：https://github.com/08822407d/MyOS2/pull/17#issuecomment-6675892248
C04只写主线新增回执/任务/检查点与既有PR导航，不写PR17成果；任务发布不等于自动投递或启动。
