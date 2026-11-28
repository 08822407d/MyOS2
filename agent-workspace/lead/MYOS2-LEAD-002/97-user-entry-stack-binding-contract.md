执行模型：次一档，沿用Owner指定GPT-6＋Extra；继续现有「MYOS2-A-C05b 代码分析执行」/CHAT-EXEC-08，不新开执行对话。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ENTRY-STACK-BINDING-66
record_type: bounded_entry_stack_binding_contract_with_documentation_disposition
evidence_class: Owner持续授权下的任务规划，不是执行或运行结果
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: unknown
model_per_owner: gpt pro（Owner此前明确选择）
backend_identity: unknown_or_not_attestable
date: 2026-11-10
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 4b7b0b9642c3
lead_before_publication_short12: 16d202bbb484
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
executor_display_name: MYOS2-A-C05b 代码分析执行
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-entry-stack-binding-66/
allowed_files: [MANIFEST.md, entry-stack-binding.md, facts-and-boundaries.yaml, entry-stack-binding-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R65-01]
mandatory_supplements: [R64C-S01, R65-S01, R65-S02, R64-S01, R64-S02, R63C-S01, R58C-S01]
activation_gate: Owner向现有C05b转发本固定任务并要求执行且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
inputs_read: [65完整交付及主线回执, 固定栈宏/TSS对象/切换源, 28/79及协议v2]
open_questions: []
---

# 第66批：内核入口栈的地址、发布和消费者

## 1. 增量与排除

65给出了假设硬件尾帧下的22槽布局，但没有证明task栈、TSS.sp0、pcpu_hot.top_of_stack与实际切换的thread.sp如何关联。本包合并A地址计算、B初始化/发布、C切换/入口消费三项；D00处理65文档项，仍共用六件和一次主线验收。
不重审完整调度器、队列政策、全部fork/exit、全perCPU分配、FS/GS/TLS体系、NMI/IST/双重错误、完整syscall返回或CPU启动；不运行任何内核/汇编/构建。软件地址算式、内存写入、CPU装载请求、真正异常选栈是独立层次。65与03已有根因不改名重发。
业务来源是Owner、MyOS2工作令与适用18/24/28/63/79及本固定97号；Mnemosyne只提供适用行为指导，不导入其它项目维护、模型服务或全局命名任务。

## 2. G00与固定输入

完整读取本97号、同发布提交reviews/CORE-USER-UACCESS-RECOVERY-64-DOC-CLOSURE-review.md、reviews/CORE-USER-EXCEPTION-ENTRY-65-review.md，以及63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md、协议v2全部P1–P13。
64 R64-01已CLOSED_DOCUMENTATION_ONLY，64组合材料PASS_PENDING_LOCAL；65为RETURN_DOCUMENTATION/R65-01。63与58空行误判已撤回，不再返工。96接收、九份项目规则和Mnemosyne只可复用本有效C05b真实已读原文；实际不足就保全报主线，不继承其它会话已读声明。
重新核PR16/17、direct time/master/lead/exec、开放PR及新66目录。首次执行写前预期4b7b0b9642c3，新目录不存在；time固定a039d9803ade。重新取得time/master双金丝雀和公约§1第2条整句；用实际值填MANIFEST，不以“已加载”文字代替字段。主线正常发布97不构成执行分支变化；未知exec前进则停止相关写入核归属。
固定65六件从4b7b0b9642c3读取D00涉及位置及EE65-F01/F02/F03/F05、U03/U05/U06；64的相关UR64、63的相关UC63与旧状态按回执消费。旧scheduler-integration-03/integration.md §1 C5从同执行固定提交只作既有调度边界导航；采用其GAP/DV或其它历史断言前先核原facts及真实主线回执，不冒称已全历史去重。不得执行该目录旧脚本/夹具。

## 3. D00：合并处置R65-01，不重做65

甲：独立核65原evidence的A01、A16、A17、A18、A20、A21、A22、A23、B11、B12、B13、B14、C04共13个指定标签。按65回执拆开独立定义，去除定义外上下文，B14归到真实main。保留字面与原有正确值，不自行整理空行；各单宏/单定义用独立标签。新evidence中给合法替代，facts.prior_return_disposition逐旧标签映射。未重核的其它50条标为保留未全量复验，不能随手加进本次全部通过数。
乙：追加重建facts.idt_binding_matrix的gate_data/gate_pack/table_write/load_request/stubs引用关系和正文错位范围，明确原A27/A30不存在。init_idt_data的present赋值、idt_init_desc的offset_low须有真正支持它们的新锚；只检查编号存在不足以证明命题支持。原有效锚可固定引用，新增锚统一计入新P6。图的宏CALL/聚合CALL及条件返回按R65-S02给追加替代说明。
丙：新MANIFEST必须显式包含read_channel、startup_selfcheck_quote、branch_canary_quotes、produced_by/model_per_owner/backend_identity、self_check三个数、P9逐项结果和write_retry_log；prior_manifest_disposition登记旧65缺前三字段，补录值来自本次真实读取，不假称旧件本来有。原65的63/63只保留为历史作者自报，不继续认证原全部定义合格。正式补充评论只写12位真实短提交，不重复旧评论中的40位号。
正文列原位置、原说法、修正/保留、影响和新证据；三组D00仍放本六件，不新建第七件或修改65文件/旧评论。只提交documentation_disposition_submitted，不自关R65-01；64文档闭合不因此撤销。D00具体证据遇不一致应提交可核异议，不能为迎合主线复制错误文本。

## 4. G01／A：task栈与寄存器帧地址

固定核心源：mykernel/arch/x86_64/sched/context/switch_to_macro_arch.h中TOP_OF_INIT_STACK、task_pt_regs、task_top_of_stack各自完整定义；按实际include或正规搜索定位task_stack_page、THREAD_SIZE、TOP_OF_KERNEL_STACK_PADDING、init_stack及所需类型。每个宏独立锚定，不把相邻宏或注释串成一个定义。
建立base、THREAD_SIZE、padding、sizeof(pt_regs_s)、task_pt_regs、top_of_stack的符号账本。区分loff_t转换/加法、指针算术与纯整数投影；任何溢出、对齐、无效对象或不足容量前提先停，不凭THREAD_SIZE常量证明分配对象实际存在。现存generated/asm-offsets.h和65帧模型只是对照，不运行生成器或编译器。
阅读真实task.stack来源所需的最近直接赋值窗口；分配器及释放全生命周期可标opaque，不为闭合而扩成完整fork/exit审计。若确有必要扩大，先记录必要性和停止界限，不声称已经证明全部栈安全。

## 5. G02／B：TSS与每CPU指针初始化、发布

完整读取固定mykernel/arch/x86_64/smp/percpu_area.c中pcpu_hot和cpu_tss_rw各自对象定义；common.c::cpu_init_exception_handling与cpu_init完整体；processor/desc_arch.h中get_cpu_gdt_rw、set_tssldt_descriptor、set_tss_desc、write_gdt_entry、load_TR_desc直接定义及必要类型窗口。区分TSS初始sp0值、描述符指向哪个对象、TR装载请求及真实CPU状态，不将注释中的用途保证当硬件事实。
完整读取process64_arch.c::__switch_to、switch_to_arch.h::update_task_stack及processor_arch.h::load_sp0。逐项列current_task、top_of_stack、TSS.sp0的实际写入来源和顺序，跟踪所用next_p与当前CPU对象；前置save_fsgs/段操作等未展开callee保留正常返回前提，不越过它们宣称必发布。
不能从相邻源码store推断原子发布、所有中断窗口安全或别的CPU可见顺序；也不能仅因多次store就声称并发损坏已经发生。源配置中被注释的Xen条件和历史trampoline注释需与活动调用分栏。perCPU寻址只读必要直接定义，不扩为整套初始化与GS机制。

## 6. G03／C：实际保存栈指针与入口消费

完整读取entry_64.S::__switch_to_asm，必要时读取switch_to宏和实际inactive/task-kframe类型及TASK_threadsp生成源。将保存的thread.sp、task栈顶、TSS.sp0、pcpu_hot.top_of_stack、用户rsp区分为不同对象/时刻；明确push/pop数、保存点、载入点及尾部jmp __switch_to对有效恢复帧的要求。不是看到相同数值就认定对象身份与生命周期相同。
仅将entry_SYSCALL_64的栈选择前缀及65已审sa_entintr_retp作为限定消费者：读取所用完整入口定义以核源控制，但不展开sysret全链。区分SYSCALL软件选栈与假设硬件异常选栈；cpu_tss_rw.sp2的临时用途不得误认成sp0。依95/65限制，实际CPU选择、GS和硬件尾帧未核则保留假设/缺口。
关联65的可能schedule及同帧条件，但schedule/__schedule、runqueue和MM保持opaque，不重做旧03。已有任务恢复、新任务首次切入和首次用户态进入分栏；未读copy_thread/首次入口生产者时不得宣称三者都成立。缺口可保存，不要求无限延伸取证。

## 7. 六件、证据质量和预算

严格六件：MANIFEST.md、entry-stack-binding.md、facts-and-boundaries.yaml、entry-stack-binding-paths.dot、deferred-findings.yaml、evidence.md。facts至少含prior_return_disposition、prior_manifest_disposition、source_ledger、stack_address_matrix、tss_percpu_publication_matrix、switch_entry_consumption_matrix、finite_model_cases、prior_findings_links。新条件SB66-Fxx/缺口SB66-Uxx，无数量配额，不把一个根因按入口重复编号。
核心源为上列栈宏、TSS对象、common相关函数、desc/processor相关定义、__switch_to及汇编。新增专题必要直接依赖软上限14文件，D00旧源与规则另计；超过须解释必要性，不截断强制定义。正规定位最多8组定点查询及4个相关固定目录；未找到可选外围定义时记范围明确的缺口，不猜标准Linux路径。
强制短锚：D00正确单定义替代及present/offset_low；A三个栈宏及真实直接地址来源；B两种对象初始化与三处发布函数及TSS描述符/加载；C保存/载入栈宏/标签与限定入口使用点。单段1–5物理行，字面、长度、定义归属、命题支持分别核，定义外注释/声明/配置单列source_context。遇空白疑点先核完整对象身份，无法闭合记不确定，不凭重排文本指控。
P6只数本包实际标签，包含D00；对所有引用ID进行存在性与命题匹配检查，不得有悬空编号或错位范围。P9按1–13逐项给实际文本子检查、partial/pending_local/NOT_RUN；字段缺失不能被总括成功声明覆盖。硬件外部结论须按P5给原始规范卷/章/修订及URL，否则只保留明示源级假设，不装作已核硬件。
图中宏展开、源选择、数据依赖、真实CALL/RETURN和ANALYSIS_STOP分开；不得把聚合名称变成虚构函数调用。模型仅本次自建纯数学/文本，不运行仓库C/ASM/脚本/旧夹具或替代本地P9。

## 8. G04发布与停止

唯一新增区为原PR17的user-entry-stack-binding-66/授权六件及本人唯一正式评论。所有旧文件/旧评论、lead/PR16、PR标题正文Draft、master/time对执行者冻结；C05不并行写执行成果。只在Owner转发本固定入口并要求执行、G00通过后启动；发布合同不等于自动跨聊天投递。
GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次，10/20/40秒或等效安全只读间隔并遵守Retry-After。expected-head/force=false，未知并行变化不覆盖；复用已建对象，评论marker查重；工具发现次数与真实写请求分开记录。
写后compare自4b7b0b9642c3严格六件A新增、旧件零净改删；固定新提交与活动分支六件全文/blob/HEAD及正式评论回读。只报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、D00/A/B/C实际状态，然后STOP_WRITING交C05独立验收。不自关R65-01、不自签66通过、不自行开始67。
禁止修内核/出补丁、预处理/编译/链接、运行仓库代码/脚本/旧夹具/QEMU/实机、安装工具、访问Owner本机/凭据或外部libc/映像、额外付费API/子代理、跨仓维护、新建分支PR、合并关闭/强推删分支；阶段3门不变。
