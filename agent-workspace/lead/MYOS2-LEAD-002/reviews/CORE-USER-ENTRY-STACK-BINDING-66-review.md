---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-ENTRY-STACK-BINDING-66-REVIEW-001
record_type: bounded_static_review_with_empty_evidence_return
evidence_class: 固定六件、回源和增量核对；外部C规则仅作明示模型条件
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；非界面或后台认证）
model_per_owner: gpt pro（沿用Owner此前明确选择）
backend_identity: unknown_or_not_attestable
date: 2026-11-10
base_snapshot: time
read_channel: mixed
read_channel_detail: GitHub连接器读项目与源码；官方GNU文档核对C类型规则
source_pin_short12: a039d9803ade
contract97_short12: b603f4f09970
reviewed_base_short12: 4b7b0b9642c3
reviewed_head_short12: 6cffc97057c7
reviewed_delivery_comment: 6756756742
acceptance_verdict: RETURN_DOCUMENTATION
returned_scope: [R66-01]
subjects: {D00: ACCEPTED_WITH_R65C_S01_AND_R65_01_CLOSED, A: CORE_RETAINED_PENDING_EMPTY_ANCHOR_REPAIR, B: CORE_RETAINED_WITH_LIMITS, C: CORE_RETAINED_WITH_LIMITS}
mandatory_supplements: [R65C-S01, R66-S01, R66-S02]
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original101_full_automatic_byte_definition_scan: NOT_RUN
original_facts_deferred_full_JSON_parse: NOT_RUN
self_check: {verified_claims: 2, quotes_reconfirmed: 2, downgraded_to_inferred: 0, scope: 本回执两段直接源码，不是原101全量认证}
next_followup: CORE-USER-FIRST-RUN-FRAME-67
next_task_ref: ../98-user-first-run-frame-contract.md
executor: MYOS2-A-C05b / MYOS2-LEAD-002-CHAT-EXEC-08
inputs_read: [97号, 65回执, 66完整六件与正式评论, 固定task与栈/TSS/切换源, 同pin已读原文, 官方GNU的Common-Type及Integers-implementation]
open_questions: []
---

# 第66批：保留主干，仅退空引文及其自检传播

## 1. 真实交付与独立裁定

compare 4b7b0b9642c3→6cffc97057c7为ahead1/behind0、一个提交，仅新增授权六件2322行：MANIFEST117、正文69、facts969、evidence926、DOT80、deferred161。旧件零净改删。PR16/直接lead为b603f4f09970，PR17/直接执行头为6cffc97057c7；两PR开放未合并，PR17 Draft；开放第三项为空。direct time仍a039d9803ade、master仍e8e60cd19540。
六件与6756756742全文已读，长件补齐截断。执行者声明STOP_WRITING，未自行签验收；其隐藏后台、临时文件与全部历史工具调用不由本主线追认。
R65-01的13组重锚、索引与元数据处置结合本轮R65C-S01补锚，已单独文档闭合，65组合材料PASS_PENDING_LOCAL。66自身A10为空，P2及强制命题证据门失败，故66为RETURN_DOCUMENTATION，唯一R66-01。不要求重做整批，不把新退回与旧65闭合捆绑。

## 2. R66-01：A10没有成员声明

固定66 evidence对象e9fecef67367的A10标题为task结构栈指针成员，标签指向task_types.h::task_struct，但c代码围栏内只有一个空行。全文分段读取后，又单独回读304–322行确认同一对象、同一空片段。不是传输未读完，也不是指控执行者改变了源码空行。
空行虽可在许多文件中命中，却不能证明stack成员、类型或定义归属；非空支持证据是本任务强制要求。固定time的真实成员确实存在：

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		void			*stack;
```

主线读取task_types.h定义开头至120行，确认该行位于task_struct。任务栈的直接消费也存在：
[VERIFIED mykernel/sched/misc/sched_misc.h::task_stack_page]
```c
		*task_stack_page(const task_s *task) {
			return task->stack;
		}
```

最小返工：67 D00独立回源，在新evidence给A10的有效单定义短锚；正文与facts.prior_return_disposition映射旧A10，并纠正66正文A、stack_address_matrix中将空A10当证据的读法。原101是标签数量，不因一条空锚自动改成100；但MANIFEST、facts.p9_text_subset和正式评论中的101/101全部有支持、强制锚齐全等声明不能继续作为证据门通过依据。新自检只数新包实际合格锚，其余旧100条未全量重验不得自动签为通过。
全部追加到67六件，不改66或旧评论。只提交documentation_disposition_submitted，关闭权在C05。主线给出的正确例子不替代执行者交付责任；下次发布前必须同时检查围栏非空、真实定义归属及命题支持，不能仅以字符串命中或行数合格判通过。

## 3. R66-S01：技术主干与整数类型补充

A主干成立的范围：task_stack_page返回task.stack，init_task初值指向init_stack，alloc_thread_stack_node请求kmalloc(THREAD_SIZE)并在非空分支写tsk->stack；dup_task_struct在复制父task之后申请新栈。THREAD_SIZE按当前宏为32768、padding0；176来自现存生成头，不是本轮编译sizeof。地址投影base+32592和base+32768只对有效、足够大且满足所用指针规则的存储成立。
链接脚本不只是给init_stack命名：它显式按32768对齐，设置__start_init_stack/init_stack，推进到起点加32768，并令__top_init_kernel_stack为结束地址减176。这是脚本的布局意图，不是最终链接、映射和生命周期证明；extern数组声明仍须与实际链接对象分层。

必须修正原正文、facts和SB66-F03里把__ptr加法直接称作有符号加法的概括。本轮补读uapi/linux/const.h，C分支的_AC(X,Y)使用X##Y；PAGE_SIZE由1UL构造，THREAD_SIZE也带unsigned long。loff_t的当前源码链为long long。若采用unsigned long与long long均64位的LP64模型，则THREAD_SIZE-padding为unsigned long，__ptr与它做加法的公共类型是unsigned long long，而非signed long long；复合赋值最后才转回loff_t。因此不能仅凭临时变量为signed就认定此加法发生signed-overflow UB。
[EXTERNAL https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Common-Type.html] GNU C Language Manual §24.5明确公共类型规则也适用于修改式赋值中的算术。本条用于上述明示LP64模型，不认证项目实际编译器版本、预处理结果或ABI。
[EXTERNAL https://gcc.gnu.org/onlinedocs/gcc/Integers-implementation.html] GCC文档§4.5给出整数转为不可表示的有符号类型时的实现选择。该文档不能替代指针与整数往返的实现约定、对齐、对象范围或生命周期证明；未运行GCC。原有限低地址算例仍可保留，SB66-F03按转换和对象有效性缺口消费，不称已证的有符号加法故障。本主线直接给出此限定，不另派该项返工。

B保留：TSS.sp0、sp2、pcpu_hot.top_of_stack和独立current_task对象不是一物。__switch_to只有在先前save_fsgs/段操作等正常返回后，才依源序写current_task、top_of_stack并经update_task_stack/load_sp0写sp0。多次store既不证明原子发布，也不证明竞态已发生；实际CPU对象、关中断/锁协议和消费者均需独立证据。TSS/GDT构造、load_TR_desc及IDT加载只是源级请求。
C保留：六个寄存器保存/恢复、thread.sp保存点、尾jmp __switch_to与其条件正常return形成已构造inactive帧的契约。8字节槽模型下ret_addr在48偏移，inactive帧56字节；不是编译布局。SYSCALL前缀用sp2暂存用户RSP，软件内核栈来源是pcpu_hot.top_of_stack，不是直接读sp0；异常公共入口使用已有RSP，不能偷换成同一种软件切栈。
原4项SB66均为条件记录，8项缺口及所有EE65/UR64/UC63保持原状态。特别是新fork帧生产者和首次切入仍未由66完成；67负责有界补证，不预签真实首次用户态运行。

## 4. R66-S02：图的精确读法

原DOT3279字节、80行的完整Git blob身份与远端1014c5425d20所对应对象一致。独立文本统计35节点40边、0未定义端点，未运行Graphviz。结构匹配不保证每条边是直接调用。
update_task_stack是真实函数，原节点的macro call称谓不准确；它内部展开task_top_of_stack并调用load_sp0。tssDesc→tr的CALL不可解释为set_tss_desc调用load_TR_desc，两者由cpu_init_exception_handling先后调用；tr→idtr同样是调用者中的后续顺序。top→pcpuInit只可当符号关系，实际静态初始化使用独立TOP_OF_INIT_STACK宏，不调用task_top_of_stack。
switch_to宏展开含真实__switch_to_asm调用；__switch_to_asm到__switch_to是尾跳转。ANALYSIS_STOP是证据停止，不是硬件停机。原C08标题称前五个恢复寄存器，但片段实际展示4个pop和一行注释；完整原汇编恢复6个寄存器的结论不变。以上由本回执追加纠正，无需单独勘误包。

## 5. 实际方法与限度

完整六件阅读：facts按1–330/331–650/651至EOF969；evidence按1–290/291–610/611至EOF926，并定点重读A10；主件末尾60至EOF补齐；其余全文。D00的28条按真实同pin定义作定点比对；主线补锚的desc_arch.h 248–275本轮再次读取。原101全部逐字/定义自动扫描与原facts/deferred全文JSON机械解析未运行；不以作者101/101代替这些未完成项。
新读源码包括task_types.h1–120、sched_misc.h全文、page_const_arch.h全文、fork.c1–230及235–520（覆盖alloc_thread_stack_node和dup_task_struct）、thread_info_const_arch.h、switch_to_types_arch.h、processor_types_arch.h、current.h、asm-offsets.c、kernel.lds、posix_types.h、task_api.h全文，types.h相关loff_t窗口、init_task.c1–110及const.h全文。主线已有同pin的desc/processor函数、common初始化函数、__switch_to、entry_64.S、percpu对象、栈宏与D00其余直接源真实内容复用；不宣称本轮重新下载它们或全量重载所有指导。
本轮Python仅运行自写文本/数学检查：DOT身份与结构、四个作者地址投影重算、6寄存器/ret_addr槽位、4组调度布尔门及4个显式64位无符号公共类型算例。临时文件/mnt/data/myos2_review66/independent_checks.json确实存在，未写仓库；这些不是作者14例脚本重放或真实C执行。可选public raw下载一次DNS失败，连接器正常；Git commit GET用短号404后用真实完整号读取成功，均属只读通道情况，不是写权限失败。
为67划界另读process_arch.c完整copy_thread/ret_from_fork，历史09回执及DOC-FIX只定点导航，不称09全量复审。外部只核官方GNU两页C规则，未核Intel/AMD硬件手册。完整P9、仓库代码/脚本/旧夹具、预处理/编译/链接、偏移生成、Graphviz、QEMU和实机均NOT_RUN。按28/79同轮准备98号，不新开执行对话或扩展为全调度器/信号/MM审计。
