---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FD-LOOKUP-REF-41-REVIEW-001
record_type: independent_bounded_static_acceptance_with_mandatory_supplements
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-05
produced_by: ChatGPT本主线独立复核
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（沿用Owner报告）
backend_identity: unknown_or_not_attestable
read_channel: GitHub连接器；本地仅纯文本和有限整数模型
source_pin_short12: a039d9803ade
reviewed_pr: 17
reviewed_base_short12: 46f5327a8cdf
reviewed_head_short12: ee0882d46bbf
reviewed_delivery_comment: 6652894020
contract66_short12: b90b8a2c34d4
lead_before_publication_short12: b90b8a2c34d4
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R41-S01, R41-S02]
original_artifacts: PRESERVED_UNMODIFIED
runtime_evidence: NOT_RUN
full_local_P9_gate: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_original_40_quote_automatic_byte_scan: NOT_RUN
reviewer_original_facts_deferred_JSON_parse: NOT_RUN
reviewer_DOT_identity: connector_text_reconstruction_matches_full_remote_Git_blob
reviewer_DOT_blob_short12: 5caeb8483747
reviewer_DOT_bytes: 4719
reviewer_graph_structure: {explicit_nodes: 38, unique_nodes: 38, edges: 37, undefined_endpoints: 0}
reviewer_Graphviz: NOT_RUN
reviewer_independent_finite_models: 26
self_check: {scope: three_normalized_definition_quotes_in_this_receipt, verified_claims: 3, quotes_reconfirmed: 3, downgraded_to_inferred: 0, method: 固定源码连接器文本逐项核对；不是原40条自动字节重放}
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
next_followup: CORE-USER-FD-CLOSE-42
next_task_ref: 68-user-fd-close-contract.md
executor_rotation: false
execution_branch_modified_by_mainline: false
owner_decisions_requested_now: []
phase: 2
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
---

# 第41批独立验收：fd取回及局部引用契约限定接收

## 1. 交付核实与裁定

Owner询问41是否正确完成，未给新提交。本主线先读取实际PR17与最新交付评论，自行定位ee0882d46bbf和6652894020。
交付身份为新C04b/CHAT-EXEC-05，READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，并声明STOP_WRITING；不是旧C04a继续执行。
依据固定66号、40回执R40-S01/S02及41完整六件，裁定PASS_PENDING_LOCAL，无另行返工项。
原六件须与本回执R41-S01/S02组合消费；不将原40个证据标签单独全部重新签为合格，不签真实fd、映射或内核运行通过。
39文档闭合、40限定接收及更早源码候选状态保持不变。

独立compare 46f5327a8cdf→ee0882d46bbf：ahead=1、behind=0、一提交，仅user-fd-lookup-ref-41/六件A新增1546行。
分别MANIFEST213、正文82、facts514、DOT80、deferred194、evidence463；旧件零净修改/删除，无越界路径。
两PR均Open/unmerged，PR17 Draft；主线b90b8a2c34d4、执行ee0882d46bbf。
direct branches核time a039d9803ade、master e8e60cd19540；已知PR16/17开放且开放列表第三项为空。
交付停止声明和MANIFEST加载记录不等于独立认证另一聊天的物理状态或每一步历史工具调用；本次验收主要依固定工件及独立回源。

## 2. 得到固定源码支持的核心观察

__simple_fget仅以fd<files->fd_count作逻辑门；门内先读槽，再调用atomic_long_inc(&retval->f_count)，没有先判retval为NULL。
因此表内空槽与逻辑范围外不是同一路径：前者缺有效对象前提，不能认证正常NULL返回；后者不读槽且保持retval=NULL。
fd_count是逻辑字段，不能自动充当固定256槽的物理边界；mask在所读完整活动体没有消费，fget传入FMODE_PATH并不建立本层过滤。

myos_fdget_pos直接取槽、返回flags=0；__to_fd及fdget/raw/pos直接取槽、返回flags=3，均没有局部增加f_count。
fdput按FDPUT_FPUT调用fput；fdput_pos的实际位置解锁仍为注释。该轻量取回/归还不配对的旧问题保持08::US-01，不新增同义条目。
fget与get_file的有效对象路径确有计数增加；不与fdget混同，也不从fd_install没有计数调用推出不能转移已有引用。

mmap源码包装层的fd声明为unsigned long，向fget(uint)传参。明确LP64假设下可作模2^32窄化模型，但不是完整syscall ABI或实机参数认证。
非匿名fget正常返回NULL才由包装层给出-EBADF；空槽在callee更早的成员访问不能由后置判空补救。
普通匿名路径不调用fget；匿名HUGETLB在所选源码进入while(1)。成功取回file且opaque映射调用正常完成后才到out_fput。
read与write均在取得非NULL结果的分支内执行后续操作和fdput_pos；返回值符号只影响f_pos更新条件，不另控制随后的fdput_pos。

FG41-F01/F02/F03保留各自前提、反证及延期状态；本裁定不是三个独立已复现漏洞或源码问题闭合。

## 3. R41-S01：实际门、类型模型及引用的消费前提

1. fget的范围比较发生在uint fd与unsigned long fd_count之间；物理界限、槽值已初始化、file对象活着、计数操作合法均是额外前提。
   对编码错误、悬空或未初始化槽，不能仅因非NULL就继续推导引用取得；NULL成员表达式处已缺合法对象，不保证机器必然崩溃或正常返回。
   非法下标处停止是分析界限，不能把未定义行为前的源码顺序当作优化后的机器程序必然保留的副作用。

2. 类型别名只说明uint=unsigned int、ulong=unsigned long，不单独证明32/64位宽。
   本批大整数例限定于显式LP64及包装层入口值已到达的模型；编译参数、syscall宏展开和ABI调用者并未全链认证。
   uint→int超出可表示范围时不自行选定实现值；负int→ulong模型另论，不能把两段转换合并成固定-1。

3. n→n+1→n及n→n-1是局部计数事件模型，要求有效对象、有限可表示计数、原子操作正常和无其它计数修改。
   vm_mmap_pgoff、vfs_read、vfs_write保持opaque；它们可能取得、放弃或转移独立引用。
   因而“包装层配平”不是整个调用前后f_count总量恒定，也不是VMA/页缓存/文件对象生命周期完整证明。
   opaque函数正常返回仍不自动证明对象一直有效；到fdput前若发生失效，停止于更早位置。计数到零只确认kmem_cache_free源码调用，不认证实际物理释放。

4. read/write后置条件只是f.file或filp非NULL，不是普通有效file验证。有效对象是进入file_ppos及更深callee的附加前提。
   fdput_pos确在ret正负两类正常返回之后都可执行，但须没有更早失效、前面的f_pos赋值也正常完成；不能越过callee不返回或其它无效访问。
   正常空槽返回的fd_s在caller判空后直接返回-EBADF，不调用fdput；这与直接把NULL、flags3结构交给fdput的helper模型不同。

5. mask未使用仅证明本层没有依据它过滤。未核目标规范、真实FMODE_PATH生产者及opaque下层策略，不把它自动认定为权限绕过或保证映射成功。
   UV-04、OF30、SB38及40的索引/对象来源限制继续有效，不用有效helper输入代替实际用户open成功证明。

## 4. R41-S02：证据归一化、图示与元数据

FG41-A025明确组合标为uint/ulong，原片段实际同时含ushort、uint、ulong三条typedef；不是一个单定义体，也不能由三行推出位宽。
与ENFILE标签实际摘录EXDEV的错误身份不同，本项所称两个类型别名均确实在原片段中；本主线提供两个单独替代摘录，后继必须分开消费，不另派返工批次。
原40/40只保留为作者所申报文本核对口径，不等于40个独立、单定义的语义证明；本主线没有自动重放原40条。

[VERIFIED mykernel/include/linux/kernel/types.h::uint]
```c
		typedef unsigned int		uint;
```

[VERIFIED mykernel/include/linux/kernel/types.h::ulong]
```c
		typedef unsigned long		ulong;
```

FG41-A031首行注释位于函数外；直接定义证据归一化为以下三行，外部注释仅作上下文：

[VERIFIED mykernel/fs/vfs/read_write.c::file_ppos]
```c
static inline loff_t *file_ppos(file_s *file) {
	return file->f_mode & FMODE_STREAM ? NULL : &file->f_pos;
}
```

图M5→M7、R3→R5、W3→W5的SOURCE: file valid应读为源码仅检查非NULL，另加对象有效前提；没有新增有效性检查。
F2→F4是源码正常顺序，valid file_s assumed不是活动分支；F3/R2/W2是分析停止节点，不是源码return、异常处理或CPU停止指令。
M4→F0聚合了fget向__simple_fget转调，不表示源码省略了fget这一层。
R6/W6概括条件f_pos写入，条件为假也继续到R7/W7归还；不能误读为只有ret>=0才fdput。

已把连接器返回的完整DOT文本重建为4719字节，并计算完整Git blob对象哈希，与远端5caeb8483747对应的完整SHA一致。
因此本轮图检查使用字节身份已核的完整副本，而非仅转录节点；正则结构核对38唯一显式节点、37边、0未定义端点。
未运行Graphviz；结构与字节身份核对不代替上述逐边语义检查。

MANIFEST接收/指导JSON中的execution_source=current/human-approved-spec.md仅表示Mnemosyne自身指导权威。
本MyOS2执行权威仍是Owner指令、本项目00工作令、固定66任务及67授权交接，不能被Mnemosyne维护源替代；原MANIFEST的“不改变MyOS2任务”说明继续合读。
加载记录作为执行者申报与身份记录使用，不冒称主线逐次回放了旧聊天。下批继续用C04b，无新交接或跨项目维护。

## 5. 方法、范围、限制与后继

本轮完整读66号、41六件和交付评论6652894020；40回执R40-S01/S02、39闭合与既有MyOS2/Mnemosyne约束沿用已加载固定记录。
63号与28号持续有效；本轮未全量重新刷新项目或Mnemosyne规则，不把继承范围写成重新读取。
固定time直接回读：file.c184–242及28–123；file.h1–142；types.h80–115；fdtable.h19–70；fs.h95–119及860–915；
file_table.c108–138；mmap.c1312–1340；read_write.c175–338（只审本任务包装层；中间函数因窗口返回而可见，不纳入完整读写审计）。
金丝雀options_flags.cmake38–44、panic.c77–81重新核对。旧05 deferred421–700定点核UV-09/10/11；08::US-01及13/14限定沿用此前已读材料。

纯Python执行26个自建有限模型：9个取回/对象状态、6个LP64窄化、2个负int到ulong、6个局部引用事件、3个mmap门顺序。
并非作者20个案例全集机械重放；未运行任何仓库C/ASM/脚本、预处理、编译、QEMU、硬件或真实fd/IO/并发。
未自动扫描40原引文字节、未解析原facts/deferred JSON/YAML、完整本地P9仍NOT_RUN。
一次可选public raw下载遇DNS失败，未获得该通道原件；GitHub连接器证据正常，DOT通过连接器全文和Git对象哈希确认，不将下载失败记作写入/权限失败。
不同对话角色分离不等于人类或异构模型认证，Owner当前无需作修复决定。

后继68号CORE-USER-FD-CLOSE-42交原C04b/CHAT-EXEC-05，独立核close槽位清除、filp_close/fput和exit_files/put_files_struct局部释放顺序，不预签42成果。
为任务划界预读固定fs/syscall.c1–190（只取close入口）、open.c313至末尾、file.c退出包装层；未扩大审计完整调度退出、fork或分配器。
固定任务：https://github.com/08822407d/MyOS2/blob/b90b8a2c34d4/agent-workspace/lead/MYOS2-LEAD-002/66-user-fd-lookup-ref-contract.md
固定成果：https://github.com/08822407d/MyOS2/tree/ee0882d46bbf/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fd-lookup-ref-41
交付评论：https://github.com/08822407d/MyOS2/pull/17#issuecomment-6652894020
C04仅写主线新文件及既有PR导航，不写PR17成果；下一批发布不等于自动跨对话投递或执行。
