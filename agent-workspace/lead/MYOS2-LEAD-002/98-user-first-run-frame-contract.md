执行模型：次一档，沿用Owner指定GPT-6＋Extra；继续现有「MYOS2-A-C05b 代码分析执行」/CHAT-EXEC-08，不新开对话。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FIRST-RUN-FRAME-67
record_type: bounded_first_run_frame_contract_with_documentation_disposition
evidence_class: 持续授权下的任务规划；非执行成果或运行认证
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；非界面或后台认证）
model_per_owner: gpt pro（发送方Owner此前选择）
backend_identity: unknown_or_not_attestable
date: 2026-11-10
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 6cffc97057c7
lead_before_publication_short12: b603f4f09970
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
executor_display_name: MYOS2-A-C05b 代码分析执行
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-first-run-frame-67/
allowed_files: [MANIFEST.md, first-run-frame.md, facts-and-boundaries.yaml, first-run-frame-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R66-01]
prior65_status: PASS_PENDING_LOCAL_WITH_R65C_S01
mandatory_supplements: [R65C-S01, R66-S01, R66-S02, R65-S01, R65-S02, R64C-S01, R63C-S01, R58C-S01]
activation_gate: Owner向现有C05b转发本固定任务并要求执行，且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
inputs_read: [66完整交付与本轮主线回执, 固定process_arch.c与既有入口/切换原文, 项目28/63/79及协议v2]
open_questions: []
---

# 第67批：新任务首帧的生产、首次切入与分流

## 1. 增量及禁止扩域

66审的是已经有inactive帧时的恢复契约，并明确copy_thread/首次入场未闭合。本包合并A新帧生产、B首次切入与参数交接、C内核函数/用户返回分流三个专题；D00处理66的空引文及自检传播，共六件、一次主线验收。
不重复完整66栈/TSS链，不重做全部fork/exit、调度队列、信号、TLS/FPU或exec/ELF；不展开完整CPU启动、NMI/IST、所有sysret安全策略，不做内核修复或运行。SB66-U04启动时序和真实硬件选择不因本包自动关闭。
业务权威仍为Owner、MyOS2工作令与适用18/24/28/63/79及本固定98号；Mnemosyne只借行为指导，不导入维护或其它项目任务。

## 2. G00、规则与历史输入

完整读取本98号、同发布提交reviews/CORE-USER-EXCEPTION-ENTRY-65-DOC-CLOSURE-review.md、reviews/CORE-USER-ENTRY-STACK-BINDING-66-review.md及检查点2026-11-10-stack66-reviewed-first-run67-ready.md；全文读取63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md和协议v2全部P1–P13。其他规则仅可复用本有效C05b实际已读原文；不足则补读，不继承其它会话的已读声明。
65 R65-01已CLOSED_DOCUMENTATION_ONLY，组合材料PASS_PENDING_LOCAL；R65C-S01已由主线直接补vector/bits锚，不再派这两项D00。66为RETURN_DOCUMENTATION/R66-01。64闭合和63/58误判撤回保持，不重验旧批次或恢复已撤回指控。
核PR16/17、direct time/master/lead/exec、开放PR、新67目录；首次执行写前预期6cffc97057c7，新目录不存在，固定time仍a039d9803ade。重新取公约原句和time/master双金丝雀，将实际值写入MANIFEST。主线发布98的正常前进不等于执行分支冲突；未知exec变化须停相关写入核归属。
原66从6cffc97057c7读D00涉及位置和SB66-F03/F04、U04/U06/U08及相关前提；原65/64按新回执限定消费。涉及syscall_exit_to_user_mode时，合读同主线固定提交的CORE-USER-RETURN-SIGNAL-09-review.md与CORE-USER-RETURN-SIGNAL-09-DOC-FIX-01-review.md相关消费限制；旧RETURN快照以其闭合记录为准，不重新执行09。采用旧RS/EE/UR/SB或调度GAP/DV前必须读取对应原条目与真实状态，不从第一波摘要或记忆重编号。

## 3. D00：R66-01最小追加处置

独立回读66 evidence A10及time task_types.h的task_struct，确认围栏空行不能证明stack成员。在新evidence提供真正成员声明的连续1–5行锚；facts.prior_return_disposition映射旧A10、新锚及影响，正文逐原位置解释修正读法。66正文A和stack_address_matrix原来引用空A10的地方一并列入追加覆盖。
对原MANIFEST、facts.p9_text_subset和评论6756756742的101/101全部支持声明追加限制：101仍是标签数，不能简单改100来掩盖缺证；未重验的其余100条不自动成为本次通过。只统计新67实际标签和重新核过的锚。旧六件、旧评论一个字节不改；不另交第七件，不自关R66-01。
R66-S01整数类型、R66-S02图的解释已经由主线修正，合读即可，不要求复做66。发现主线证据不符应提出可核异议，不为满足退回而复制错误文本。

## 4. G01／A：copy_thread的新任务帧生产

完整读取mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread，以及switch_to_types_arch.h中的inactive_task_frame和fork_frame、switch_to_arch.h::kthread_frame_init，确认task_kframe_s/fork_frame_s的真实typedef来源。task_pt_regs/current_pt_regs及container_of只读所需直接定义，不凭上游Linux名字补实现。
建立task栈、childregs、fork_frame、frame、thread.sp与ret_addr的对象/偏移账本。单独核frame->bp=(reg_t)childregs+1的运算类型与单位，不把转换后的整数加1解释成指针加一个pt_regs。56/176/232只可作为明示槽宽与布局前提的数学对照，不运行sizeof/offsetof或生成器，不把同值当对象身份。
按PF_KTHREAD、普通用户分支、args->fn非空、args->stack为零/非零分别列字段来源、清零、继承和覆盖顺序，包括bx/r12/ret_addr/bp、childregs.ax/ip/sp。未显式写入的字段只能按已读分配/复制前提解释，不擅称自动清零或必然垃圾。save_fsgs等前置callee以正常返回为门，不越过最早无效对象。
限定追fork.c中copy_thread的直接调用、返回值消费及必要参数来源窗口；上下游大函数只读相关完整控制段，其余资源复制和清理保持opaque。CLONE_SETTLS等注释路径分开登记，不因此扩展整套TLS。

## 5. G02／B：从已构造首帧到ret_from_fork_asm

完整读取entry_64.S::__switch_to_asm和ret_from_fork_asm真实标签区间，合读process64_arch.c::__switch_to的正常返回出口及66限定；不得只凭ret_addr名字认定首次入场必达。
追六个pop后的ret_addr槽、尾jmp至C及C正常返回的条件；分别登记老任务恢复和新fork首帧，不能把老任务已保存的返回地址当新任务生产证据。建立rax/rsp/rbx/r12到ret_from_fork的rdi/rsi/rdx/rcx来源表，并确认regs指向哪个帧。
包括帧容量、寄存器初始化、callee保存约定、合法返回目标、前置不返回路径和编译ABI未认证边界；必要外部CPU/ABI结论按P5给原始规范，否则只保留明示假设。禁止用编译/模拟运行来代替本轮静态证据。

## 6. G03／C：内核函数与用户返回分流

完整读process_arch.c::ret_from_fork和同一ret_from_fork_asm剩余控制路径；按实际源顺序追schedule_tail、可选fn(fn_arg)、regs->ax赋值、syscall_exit_to_user_mode，以及汇编随后基于rbx的分流、间接call和停止路径。
对fn为空、非空且不返回、非空且正常返回、返回工作改变帧或不返回分别画条件路径；逐个call site计数，不预设入口函数一定只调用一次，也不能仅因存在两处调用就声称所有内核线程都会执行两次。必须核到达后一次调用所需的C返回、保存寄存器和帧前提。
schedule_tail通过固定源码正规定位读取其直接定义，调度器其余部分保持opaque；syscall_exit_to_user_mode仅读mykernel/entry/common.c对应函数与必要直接窗口，信号处理按09既有回执分层，不再审整套信号。syscall_return_via_sysret只是限定后继接口，不把跳到该标签等同CPU已进入用户态。
分别记录源码可达、C正常返回、汇编目标、CPU接受帧和用户指令执行；64/65/66运行缺口不自行关闭。完整引导到首次CPL3的全局时序若未建立，应继续needs_evidence，不强行扩成启动工程。

## 7. 六件、强制证据与实际覆盖

严格六件：MANIFEST.md、first-run-frame.md、facts-and-boundaries.yaml、first-run-frame-paths.dot、deferred-findings.yaml、evidence.md。facts至少含prior_return_disposition、source_ledger、frame_production_matrix、first_dispatch_register_matrix、entry_branch_call_matrix、finite_model_cases、prior_findings_links。新候选FR67-Fxx/缺口FR67-Uxx，无引文/模型/缺陷数量配额，旧根因按真实qualified_id关联。
核心为process_arch.c两个完整函数、fork/inactive类型、kthread_frame_init、入口两个汇编标签；新增直接依赖软上限12文件，D00/规则/历史另计。最多8组定点查询和4个相关固定目录，超过须解释必要性，不做全仓扫描或截断强制定义来凑预算。
强制锚：D00真实stack成员；A帧地址及各分支关键赋值；B保存/恢复、ret_addr目标和C参数传递；C两层实际调用/分支及正常返回门。每段原样连续1–5物理行，空行保留计数，但全空白围栏不得支撑事实；宏/类型/对象分别锚定真实定义。
发布前检查每个证据ID存在且支持相应命题，拒绝空围栏、悬空编号和把一条锚扩大成其未包含的字段。逐字、行数、定义归属、命题支持是四项不同检查；文本自动命中不能替代后两项。疑似空白差异先核源对象，无法核定则记不确定，不重演63/58误判。
MANIFEST必须有read_channel、startup_selfcheck_quote、branch_canary_quotes原样值、署名与后台未知、self_check三数、P9逐项实际结果、write_retry_log。P6只数本包新标签（含D00）；不得把标签总数当独立结论数。图区分宏展开、真实CALL、尾跳转/LOCAL_CONTROL、RETURN、DATA和ANALYSIS_STOP，不画聚合伪调用。
本任务适用协议v2全部P1–P13。完整本地P9和硬件行为仍NOT_RUN；自建纯数学/文本检查必须与仓库代码执行、编译和实际CPU运行分开记录。外部C规则可消费R66-S01，但真实工具链版本未核时不得替项目认证实现。

## 8. G04与停止

仅原PR17新user-first-run-frame-67/授权六件及本人唯一正式交付评论可新增；旧文件、旧评论、PR标题/正文/Draft、lead/PR16、master/time对执行者冻结，主线不并行写执行成果。Owner转发本固定入口并要求执行、G00通过后同轮连续D00、G01–G04，无需逐专题再批准。
GitHub暂态异常先查远端是否已成功，再按63号间隔安全重试，通常首次加三次，遵守Retry-After；expected-head/force=false，未知并行写入不覆盖，复用已建对象、评论marker查重，工具发现与真实写请求分别记。
写后compare自6cffc97057c7只能六件A新增，旧件零净改删；固定新提交与活动执行分支六件全文/blob/HEAD和正式评论均须回读，尤其检查A10替代是否真实进入发布对象，而非仅本地草稿存在。正式状态只报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及D00/A/B/C实际范围。
完成后STOP_WRITING，通过GitHub交MYOS2-A-C05独立验收，给Owner真实短提交和唯一正式评论的短通知；不自关R66-01、不自签67通过、不自行开始68。禁止修内核/出补丁、预处理/编译/链接、运行仓库C/ASM/脚本/旧夹具/QEMU/实机、访问Owner本机凭据或外部libc映像、安装工具、额外付费API/子代理、跨仓维护、新建分支PR、强推删分支、合并关闭；阶段3门不变。
