接收及执行模型：次一档（默认），Owner指定GPT-6＋Extra。必须在全新「MYOS2-A-C05b 代码分析执行」执行，不再使用C05a。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-EXCEPTION-ENTRY-65
record_type: bounded_exception_entry_contract_with_executor_rotation
evidence_class: Owner持续规划授权下的固定任务，不是执行成果
produced_by: unknown
model_per_owner: gpt pro（Owner此前报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-09
base_snapshot: time
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
result_base_short12: 16817a5be4d1
lead_before_publication_short12: aeefb9025a55
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
executor_display_name: MYOS2-A-C05b 代码分析执行
executor_rotation: true
previous_executor: MYOS2-A-C05a / MYOS2-LEAD-002-CHAT-EXEC-07
previous_executor_state: STOP_WRITING_NO_BATCH65
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exception-entry-65/
allowed_files: [MANIFEST.md, exception-entry.md, facts-and-boundaries.yaml, exception-entry-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R64-01]
prior63_status: PASS_PENDING_LOCAL_WITH_R63C_S01
mandatory_supplements: [R63C-S01, R64-S01, R64-S02, R63-S01, R63-S02, R12-S01, R12-S02, R58C-S01]
activation_gate: Owner在全新C05b明确接收并执行，独立规则加载及G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
inputs_read: [Owner本轮轮换指令, 64六件及主线审查, 63争议原件, 固定入口相关源码]
open_questions: []
---

# 第65批：异常入口、寄存器帧和有条件返回

## 1. 目标与范围

64从copy注记审到架构fault.c，但未建立CPU异常入口、保存帧和返回的源级衔接。本包以#PF与#GP为两个限定分支，合包完成A向量入口/IDT绑定、B保存帧及C参数来源、C正常返回与状态边界；D00并入六件，不另开第七件。
不审整个中断/异常体系、NMI/IST/双重错误、APIC设备、完整CPU初始化、调度器、COW或全部信号返回；不再次取证64的完整fault主体。内核源码只读，任何真实运行、编译和修复仍未授权。
业务执行源是Owner、MyOS2工作令及本固定95号；96是非执行源接收材料。Mnemosyne仅提供适用行为指导，不迁入其维护任务。

## 2. 新会话接收及G00

先完整读取同发布提交96-executor-continuation/00与01、两份本轮主线回执（63-DOC-CLOSURE、64-review）及检查点；按96顺序形成接收报告，独立加载九份MyOS2规则，再单独加载适用Mnemosyne指导。不得继承C05a或C05的“已读”声明。截断必须补齐；接收、项目规则加载、指导刷新分别登记在MANIFEST。
本95号、63号写入规则、79号必须全文读取。64为RETURN_DOCUMENTATION/R64-01，63已由新闭合回执限定接收，不能继续把原63空行当缺陷或自行重开58 B25。固定64六件依本任务D00和历史关联定点读，不要求复验整批。
核PR16/17、direct time/master/lead/执行refs、开放PR及新65目录。首次执行写前预期16817a5be4d1，目录应不存在；time固定a039d9803ade，options_flags/panic双金丝雀须重新核。主线发布95/96的正常前进不等于执行分支冲突，未知变化仍停相关写动作。
旧C05a/07已在正式评论6738378936声明STOP_WRITING；Owner要求新开，本包只把65写入租约给新08。不得重新启动旧07/06。本包不认证旧聊天进程终止，写前仍须核未知并行写入。
必读历史：64回执全部、63新闭合回执R63C-S01、63回执中仍有效的S01/S02、12回执R12-S01/S02及原user-mm-fault-12/deferred中MF12-F01/F02/F03/F04，64原UR64-U03/U04/U08及相关F01/F04/F05。采用09信号/返回或早期中断结论时，先读对应主线回执和真实原条目；不从第一波摘要转抄断言/ID，不臆造历史状态。

## 3. D00：R64-01追加纠正

独立读取固定time的common.c完整cpu_init与arch_cpu_finalize_init定义，以及64原C23。核原引文真正位于后者；在新evidence中给正确函数标签的注释调用锚与定义入口锚，不用extern声明代替。
新exception-entry.md列原位置、原说法、修正读法、影响范围；facts.prior_return_disposition映射C23、MANIFEST/正文/facts台账和矩阵、UR64-F06、DOT cpu节点与评论6738378936的同源误归属。正确两处是check_bugs与arch_cpu_finalize_init；不因此宣布cpu_init的全部行为已审。
纠正66/66及P9全通过的传播口径，新自检数只计新65实际锚，包含D00；未重核其它65条不自动算主线通过。只提交documentation_disposition_submitted，不自关R64-01。63闭合不受本项影响，旧63/64文件和评论全部冻结。

## 4. G01／A：向量入口与IDT绑定

核心固定源：mykernel/arch/x86_64/kernel/idt.c的set_intr_gate、idt_setup_early_handler、idt_setup_apic_and_irq_gates完整定义，以及idt_table/idt_descr声明上下文；entry/entry_64.S的idt_handler_array完整标签区间；processor/desc_const_arch.h的EXCEPTION_ERRCODE_MASK及相关常量。
按实际定义追init_idt_data、idt_init_desc、write_idt_entry和load_idt所选直接实现；先用已读include或正规搜索定位，不凭Linux常见路径编造。区分入口地址计算、门描述符构造、内存表写入、加载请求与真正硬件生效。
以#PF/#GP的常量绑定核“硬件错误码/补零错误码”和vector压栈；mask为本项目源配置，不能直接认证它符合所有CPU异常。其它向量只作必要对照，不展开全表规范审计。数组槽距IDT_ALIGN及.fill不是实际机器码长度已经认证。
需要CPU帧/指令的外部规范才能下硬件结论时，按P5给可靠原始出处；拿不到则以明示假设的源级栈模型和缺口处理，不靠注释、旧报告或模型记忆认证硬件。

## 5. G02／B：保存帧、错误码与分发参数

完整读取entry_64.S的PUSH_REGS、POP_REGS和sa_entintr_retp至iretq，以及mykernel/arch/x86_64/include/asm/ptrace.h的pt_regs_s和user_mode定义。核reg_t的必要类型定义；实际被include的generated/asm-offsets.h及生成来源只读定位，缺生成件记缺口，不运行生成脚本。
核push顺序、软件vector/error两槽和假设硬件尾帧与irq_nr/orig_ax/ip/cs/flags/sp/ss的对应；用明确64位/槽宽及硬件帧假设建立偏移账本。源码结构布局、生成偏移与真实ABI分别登记，不能用C结构声明自动认证汇编栈。
完整读myos/interrupt.c::excep_hwint_context及exception_handler。分别记录regs->irq_nr作为向量、regs->orig_ax作为页错误参数与诊断值；区分vec<FIRST_EXTERNAL_VECTOR与fault.c的地址分类。fault.c只读exc_page_fault/handle_page_fault直接窗口和64既有回执，myos_handle_mm_fault保持opaque。
特别追read_cr2与入口sti、C调用、可能嵌套之间的实际源码顺序；若提出嵌套覆盖风险，必须列触发条件和未证的中断/异常来源，不能据“开中断在前”断言真实已覆盖CR2。不得把诊断读orig_ax与传入error_code无条件当不同值，实际分发实参须先核。

## 6. G03／C：正常返回与状态边界

在同一sa_entintr_retp完整体中跟踪入口/出口两次CS低位判断、条件swapgs、cld、cli/sti、POP_REGS、跳过两槽和iretq。分开进入前CS与callee返回后帧CS，不能不加前提宣称两次swapgs必配平。坏帧、callee不返回、原帧被修改或调度切换时，停止于最早未建立的恢复前提。
excep_hwint_context在处理函数正常返回后仍有!in_atomic()条件及schedule调用；只读in_atomic直接定义，调度器保持opaque，不把本条存在等同必调度、必恢复同一栈或返回安全。正常C返回、到达iretq、CPU接受返回帧和重执行故障指令是四层。
完整读myos/interrupt.c::excep_gen_prot，并与64保留的#PF局部返回/不返回路径对照；其诊断/循环与C层默认分支分别记。不得从地址数值猜#PF/#GP，也不把while或iretq文本当实机结果。
不借此重新审syscall_return_via_sysret、全信号交付、NMI、完整调度/GS安全策略；需要的外部规范未核则明确INFERRED/needs_evidence。

## 7. 输入预算、六件及证据门

核心为idt.c、entry_64.S上述窗口、myos/interrupt.c所列四函数、ptrace.h及desc_const_arch.h相关定义；D00的common.c两个定义另计。必要直接依赖软上限12文件，超过说明必要性；最多8组定点定位及4个相关固定目录，不做全仓扫描。不得为省预算截断强制定义。
严格六件：MANIFEST.md、exception-entry.md、facts-and-boundaries.yaml、exception-entry-paths.dot、deferred-findings.yaml、evidence.md。facts至少含prior_return_disposition、source_ledger、idt_binding_matrix、entry_frame_layout_matrix、dispatch_argument_matrix、return_state_matrix、finite_model_cases、prior_findings_links。候选EE65-Fxx/缺口EE65-Uxx，无行数/模型/缺陷配额。
适用协议v2全部P1–P13。强制锚为D00正确函数定义与注释调用，A三项IDT函数及向量生成宏/标签，B保存/恢复宏、pt_regs_s及实际分发，C sa_entintr_retp返回区和excep_gen_prot/调度门。汇编以真实宏/唯一标签区间作定义锚，局部数字标签和生成文本不能冒充另一个函数。
每条原样连续1–5物理行、空行计入；字面/行数/定义归属/命题支持分别核。遇疑似空白差异先核固定原件和完整对象身份，无法闭合只记不确定，不照抄旧主线指控。P6计数含D00；P13 produced_by与Owner选择、后台未知分开。P9逐项列实际文本子检查和NOT_RUN，不签完整本地P9。
图分SOURCE_SELECTION/METADATA、CALL/RETURN、DATA、LOCAL_CONTROL和ANALYSIS_STOP；宏展开不当CPU调用，返回边不得省略条件。旧根因按真实qualified_id和source_field/source_value关联，不自行关闭历史缺口。

## 8. G04与停止

仅原PR17新user-exception-entry-65/六件及本人唯一正式交付评论可新增。全部旧成果/评论、lead/PR16、PR元数据、master/time对执行者冻结；C05不并行写执行成果。不创建分支或PR、不合并关闭/强推删分支。
GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false，未知并行写入不覆盖；遵守Retry-After，复用已建对象，评论marker查重，工具发现次数与实际写请求分别记。
写后compare自16817a5be4d1只能六件A新增、旧件零净改删；固定新提交与活动分支六件全文/blob/HEAD及正式评论都须回读。正式状态只申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、D00/A/B/C实际范围，然后STOP_WRITING；不自关R64-01、不自签65通过、不自行开始66。
成果和未决事项全部通过GitHub传回C05，最终给Owner一小段可转发消息，含真实短提交、唯一正式评论和本轮状态，不要求搬运长报告。禁止修内核/出补丁、预处理/编译/链接、运行仓库C/ASM/脚本/旧夹具/QEMU/实机、访问Owner本机/凭据或外部libc/映像、安装工具、自动付费API/子代理、跨仓维护；阶段3门不变。
