---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FILES-COPY-SHARE-43-REVIEW-001
record_type: independent_bounded_static_acceptance_with_mandatory_supplements
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-05
produced_by: ChatGPT本主线独立复核
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（沿用Owner报告）
backend_identity: unknown_or_not_attestable
read_channel: GitHub连接器；本地仅纯文本和有限事件模型
publication_status: GITHUB_PUBLISHED_FROM_PRESERVED_RECEIPT
source_pin_short12: a039d9803ade
reviewed_pr: 17
reviewed_base_short12: 6303ec8e9899
reviewed_head_short12: 9cd9e0ab4d03
reviewed_delivery_comment: 6655478015
contract69_short12: 47bfd0c93b85
lead_observed_short12: 47bfd0c93b85
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R43-S01, R43-S02]
original_artifacts: PRESERVED_UNMODIFIED
runtime_evidence: NOT_RUN
full_local_P9_gate: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_original_49_quote_automatic_byte_scan: NOT_RUN
reviewer_original_facts_deferred_JSON_parse: NOT_RUN
reviewer_DOT_identity: connector_full_text_reconstruction_matches_remote_Git_blob
reviewer_DOT_blob_sha: 4d1da9e6fda9fa0fc5d3f18c97cae419d1a20c53
reviewer_DOT_bytes: 8267
reviewer_graph_structure: {explicit_nodes: 59, unique_nodes: 59, edges: 68, undefined_endpoints: 0}
reviewer_Graphviz: NOT_RUN
reviewer_independent_finite_models: 30
self_check: {scope: four_supplementary_single_definition_quotes_in_this_receipt, verified_claims: 4, quotes_reconfirmed: 4, downgraded_to_inferred: 0, method: 固定连接器文本人工对照，不是原49条自动重放}
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
next_followup: CORE-USER-FORK-FILES-ROLLBACK-44
next_task_ref: 70-user-fork-files-rollback-contract.md
next_task_publication: PUBLISHED_AWAIT_OWNER_FORWARD
executor_rotation: false
execution_branch_modified_by_mainline: false
owner_decisions_requested_now: []
phase: 2
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
---

# 第43批独立验收：文件表来源、共享、复制和解除共享

## 0. 发布状态

本回执先以保全件完成独立复核；本次恢复GitHub写入后以同一审核结论正式保存。第43批验收结论与R43-S01/S02不变，没有重新执行源码审查。发布不等于第44批已在其它对话自动启动；固定提交与PR导航由实际GitHub结果确认。

## 1. 交付、净差异及裁定

已实读PR17与指定评论6655478015；实际执行HEAD为9cd9e0ab4d03，身份C04b/CHAT-EXEC-05，声明READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及STOP_WRITING。
依据固定69号、R42-S01/S02和43全部六件，独立裁定PASS_PENDING_LOCAL，returned_scope=[]。
原六件与评论保留，后继须合读R43-S01/S02，无另行返工项。不签真实fork/exec、完整引用生命周期、分配器或内核运行正确性。

GitHub compare 6303ec8e9899→9cd9e0ab4d03：ahead=1、behind=0、一提交，仅本批目录六件A新增1752行。
MANIFEST120、正文68、facts793、DOT132、deferred149、evidence490；无旧件净修改/删除，无写区外路径。
PR16/17仍为原分支、Open/unmerged，PR17 Draft；主线观察到47bfd0c93b85。
作者自查/停写申报不等于主线回放了执行聊天全部工具操作或证明其物理进程终止。验收依据为固定工件、实际差异和独立回源。
39文档闭合、40–42限定接收不变，UV/US/FG/CL/FT源码候选不因文档接收而修复或关闭。

## 2. 核心观察得到固定源码支持

init_task静态.files指向init_files；init_files的表计数初值1、fd_count初值0。
在所选有效前序中，dup_task_struct实际执行*tsk=*orig，copy_process将所得p传给copy_files。
copy_files的oldf=NULL和CLONE_FILES分支都不本地写tsk->files；后者只增加oldf表refcount。
目标指针同址必须合读前序复制、orig/current相符及无中途改写，不能由atomic_inc单独推出持有关系。

dup_fd外层先写*errorp=-ENOMEM，kmem_cache_alloc失败正常返回NULL；新表成功时设refcount=1、fd_count=256并初始化锁。
循环按max_fds而非oldf.fd_count；旧槽NULL时不写新槽，非NULL槽逐个分配新file对象并按值复制。
成功返回指针时errorp没有被重置，但接口注释仅约定NULL返回时该输出有效；copy_files成功后主动error=0，unshare_fd成功路径也返回0。不能据此新报错误传播缺陷。

file的f_count/f_pos是独立存储位置中的复制值；f_path、f_op、f_mapping等只复制成员值，不深复制其所指对象或自动为原file调用get_file。
旧两槽别名在正常、互不重叠的逐槽分配模型中可变为两个不同的新file；这是旧UV-09的增量身份说明，不另报同义根因。
内层kzalloc返回NULL后仍进入*new_fp=*old_fp，没有正常NULL错误出口；FT43-F01的限定失败观察成立，仍是条件候选，不是实测分配失败。

unshare_fd只有CLONE_FILES、非NULL表及refcount>1全部成立才写输出；不复制时输出保持原值。
unshare_files先把自己的copy设NULL；正常得到copy后，锁内替换task->files，解锁，再put旧表。
稳定对象/正计数模型中旧表n>1再put一次得到n-1>0；这不证明任意并发、其它退出路径或静态表最终归还安全。
begin_new_exec实际调用与非零返回门得到回核；前序de_thread和其余exec链仍保持边界。

## 3. R43-S01：前序门、所有权和输出模型

### S01-1 任务复制不是对任意node都已证安全

dup_task_struct中，if之后的原计算语句被注释，紧随的分配赋值仍受该if控制：

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	if (node == NUMA_NO_NODE)
		// node = tsk_fork_get_node(orig);
	tsk = alloc_task_struct_node(node);
```

局部tsk没有初始化；node不匹配时不能越过后面的if(!tsk)认证分配失败门或结构复制。
FT43-A006/M33及图P0/P1必须带“node进入实际分配分支且其它前序正常”的前提，而非所有node输入的正常NULL出口。
已定位的kernel_clone实际传递以下实参，说明所选正常分支的生产者，不是全部入口穷尽：

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	p = copy_process(NULL, trace, NUMA_NO_NODE, args);
```

dup_task_struct后续栈分配等调用、copy_process在copy_files前的步骤仍须正常完成；本批没有认证它们全部不改files或全局调度安全。
此处补输入边界，不借机修内核，不将任务复制的相邻问题新签为已复现缺陷。

### S01-2 指针预置、取得持有与失败是不同阶段

*p=*current预置指针值，不自行增加files表引用；共享成功分支增表计数，不逐file增加计数。
非共享外层分配正常失败时，p->files仍可保留借来的旧值，但copy_files未因此取得一个新增表持有。
不能把这个失败状态送进假想的统一exit_files清理；copy_process实际失败标签和返回消费留第44批独立核查。
内层NULL失效不能跨成正常-ENOMEM、自动goto out_release或已完成回滚。

### S01-3 复制是值模型，不是资源生命周期证明

仅在源/目标有效、成功分配对象互不重叠、读取成员可用、无并发修改条件下解释整结构赋值。
复制计数数值不等于持有者已配平；复制指针值不等于被指向对象取得新的独立引用。
f_path本身是内嵌结构，不是file中的裸指针；其内部定义为：

[VERIFIED mykernel/include/linux/fs/path.h::path_s]
```c
	typedef struct path {
		vfsmount_s	*mnt;
		dentry_s	*dentry;
	} path_s;
```

因此浅复制也复制这些指针值；挂载/dentry引用与实际物理寿命不在本轮认证范围。
max_fds=0仍要求errorp有效、新表初始化合法、oldf成员地址表达式有效；零次循环仅免除了循环槽读取。
无新槽赋值不代表实际槽必零或必非零。已读直接调用传256不等于helper内部有容量检查。

### S01-4 输出槽、参数地址及返回值

facts M34与正文的“未初始化指针”应明确为：调用者有一个有效可写变量out，传入&out，但out的值未初始化。
helper不复制时不写out，不能随后读出一个有保证的NULL。若new_fdp参数值本身未初始化/非法，则不能认证这次调用已定义良好成立。
当前unshare_files使用copy=NULL及&copy，不能与上述任意调用者混用。
refcount<=1使复制条件为假是源码事实，不证明零/负计数具有合法所有权语义。
FT43-A021的“错误指针预设”应读作“向int输出参数写-ENOMEM”；这里不是ERR_PTR编码指针。

### S01-5 发布与旧表归还

旧表身份必须在测试、复制和稍后读取old时一致；task_lock不是覆盖整个流程的锁证明。
n>1时一次put不为零，仅限无其它计数写者、所有所需对象有效；opaque函数正常返回也不能自动证明所有资源仍有效。
新表赋值先于put是源码顺序，没有本地回滚语句；若后续含未定义行为，不能把这个前缀担保为优化机器程序必然已完成的副作用。
CL42-F01只增加静态绑定和局部传递证据，没有证明实际最后一次归还；不能改为CLOSED或已触发非法free。

## 4. R43-S02：引文规范化与图的消费方式

FT43-A002最后的#ifdef属于选择上下文，不应混入单定义证据核数；绑定单独消费以下一行：

[VERIFIED mykernel/init/init_task.c::init_task]
```c
	.files				= &init_files,
```

被标符号及核心语句真实存在，本回执直接归一化，不另派返工。原49/49仍为作者方法申报，不被主线追认为49个独立单定义语义证明。
A007中的arch_dup_task_struct是函数内注释，不是活动helper；A026/A027等重叠切片不计作独立根因。

图P10→P12、D3→D5仅以非NULL为源码门；U8→U10实际是error==0且copy!=NULL，对象有效是分析前提。
D15→D17的valid和U16→U14的stable也是分析条件；STOP不是源码return、异常处理或CPU停机。
P0/P1是调用与内部展开，不重复复制任务；P1→P2须先正常完成省略前序。D7须合读i=0，每轮D19只递增一次。
D4/D8连P10和U7表示不同调用点的匹配返回，不能由一次dup_fd同时走两个调用者。
U6聚合调用及正常返回后的输出存储，不能把callee失效时的*new_fdp赋值当成已发生。
D21→U12是条件数据关联，不是发布门；全空旧表也可能正常返回并发布新表，未写槽的初值仍未证。

完整连接器DOT文本重建8267字节，Git blob SHA与远端4d1da9e6fda9fa0fc5d3f18c97cae419d1a20c53匹配。
结构检查59唯一显式节点、68边、0未定义端点；未运行Graphviz。哈希和计数不能替代逐边语义限定。
以上补充统一约束正文、facts、DOT、deferred和evidence，不回写执行成果或旧评论。

## 5. 方法、来源与未做事项

完整读取69号、43六件及指定交付评论；长正文尾部补读，重新读取R42-S01/S02和28号。
MyOS2工作令、公约、18/24、63号及已有Mnemosyne指导沿用已加载内容；没有导入跨项目维护或宣称全量刷新。
本轮固定源关键回读：fork.c273–400（仅dup_task_struct）、947–1148（仅前序指针与copy_files调用）、722–747、1580–1667；file.c21–124；init_task.c73–123；exec.c865–901；fdtable.h19–70；fs.h860–908；path.h1–70；uapi/linux/sched.h1–18；slub.h160–173；types.h101–106。
files_cachep变量定义等同一固定源已有上下文继承。第44批划界另读fork.c1149–1500，只取错误边、标签与kernel_clone调用端，不预签整个fork/PID/信号清理。

本地仅30个自建有限模型：前序3、copy_files6、dup_fd12、unshare9。不是作者34模型全集重放，不是仓库C执行。
原facts/deferred未机械解析，原49引文未全量自动字节扫描；完整本地P9、预处理/编译、内核/QEMU/硬件/真实并发均NOT_RUN。
可选public raw读取DNS失败，未取得该通道原字节；正式证据仍为固定GitHub连接器，此读故障不是仓库写入/权限失败。
不同对话AI角色分离不等于人工或已认证异构模型验收。

固定任务：https://github.com/08822407d/MyOS2/blob/47bfd0c93b85/agent-workspace/lead/MYOS2-LEAD-002/69-user-files-copy-share-contract.md
固定成果：https://github.com/08822407d/MyOS2/tree/9cd9e0ab4d03/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-files-copy-share-43
交付评论：https://github.com/08822407d/MyOS2/pull/17#issuecomment-6655478015

下一项CORE-USER-FORK-FILES-ROLLBACK-44以70号任务书随本回执正式发布，尚未由Owner转发或启动；C04b继续STOP_WRITING直到收到固定GitHub启动指令且预检通过。无需手工附件或重新验收43。