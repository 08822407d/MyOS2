执行模型：次一档，继续现有「MYOS2-A-C05a 代码分析执行」，沿用Owner指定GPT-6＋Extra。
先完成D00的R63-01追加纠正，再连续完成三个恢复衔接专题；不重做第63批、不逐专题等待批准。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-UACCESS-RECOVERY-64
record_type: bounded_combined_uaccess_recovery_contract_with_documentation_disposition
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近明确报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-09
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: 51477ed51ee2
lead_before_publication_short12: 3044d74381f0
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
executor_display_name: MYOS2-A-C05a 代码分析执行
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-uaccess-recovery-64/
allowed_files: [MANIFEST.md, uaccess-recovery.md, facts-and-boundaries.yaml, uaccess-recovery-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R63-01]
mandatory_supplements: [R63-S01, R63-S02, R62-S01, R62-S02, R58C-S01, R12-S01, R12-S02]
activation_gate: Owner向现有C05a转发固定任务并要求执行且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 第64批：用户复制的异常记录、缺页衔接与替换指令启用

## 1. 增量、固定输入和明确排除

63已建立包装层的有条件残量契约，但没有认证异常表被使用或替换指令已启用。64围绕这些未闭合衔接：A异常记录生产及链接声明，B内核访问用户地址时的架构页故障消费，C替换指令元数据及其启用/目标实现。三个专题共用六件和一次主线验收；D00也是这六件内的追加章节，不创建第七件。
12已经审过VMA/PTE及非零fault消费，63已经审过地址谓词、copy包装和标量宏；不重复其主干或把原根因改名。增量是“辅助函数/元数据存在”与“实际恢复消费者存在且绑定”的关系和边界。
不做完整异常/中断/页表/CPU初始化审计，不审完整#GP、COW、文件页缓存或全部syscall；不实际触发fault、替换指令或读取真实用户指针。源中地址范围和执行权限级分开，不能假设所有非法地址都产生#PF。
业务依据仍是Owner持续授权、00及18/24/28/63/79；Mnemosyne只作适用行为指导，不接管其它项目或启用额外付费模型/子代理。

## 2. G00：规则、历史和写入租约

完整读取本94号、同发布提交reviews/CORE-USER-UACCESS-COPY-63-review.md、63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md。63当前为RETURN_DOCUMENTATION，不得写成已PASS；62及更早状态不重开，58 B25原件正确及误判撤回保持。
继续同一C05a/CHAT-EXEC-07；87接收、九份MyOS2规则及Mnemosyne仅能复用本有效会话真实已读固定原文，并分列新读/复用。真实上下文不足则保全并报主线安排独立接收，不自行换身份或恢复C04c。
重新核direct time/master/lead/执行refs、PR16/17及开放PR、新64目录与写区交集。首次执行写前预期51477ed51ee2，新目录不存在；固定time@a039d9803ade，核options_flags.cmake与panic.c双金丝雀。未知HEAD/竞争写入先核归属，停相关写动作，不换pin、不强推。
必读历史：12主线回执的全部技术消费限定、R12-S01/S02及原user-mm-fault-12/deferred-findings.yaml中MF12-F01/F02/F03/F04；63六件相关A/B/缺口UC63-U03/U04；62回执的对象、返回及图限制。已有同pin原文可明确复用，否则实际读取。采用09、55、59、60旧条目时先定点读取对应原件/回执，保留真实qualified_id和source_field/source_value，不臆造ID或全历史去重。

## 3. D00：R63-01追加处置

原63 evidence固定51477ed51ee2，C14/C15/C16三段各有5个物理行，但删除了源码中的1/3/1个空行，原样连续性失败；不是围栏本身超5行。先独立对照固定原件和time readdir.c，不把58 B25历史误判套到本轮。
在新uaccess-recovery.md中列原标签/原说法、保留或修正、影响范围；evidence.md提供合法替代短锚；facts设prior_return_disposition。C14可用真实4行，C15拆成真实5行及2行，C16可用真实5行，具体原文见主线回执但必须自行回源。
追加纠正原MANIFEST、正文及正式评论的“64/64逐字全部通过”等总括口径；不编辑旧文件/评论。新自检数只计本次实际新锚，含D00锚，不继承原64/64或将未重核的其余61条都宣称通过。
补记原63 MANIFEST缺produced_by；新MANIFEST如实填P13署名，Owner指定模型与后台不可认证分开。同时明确消费R63-S01中sizeof(*frame)、dirent非dentry、无符号回绕及真实对象前提，R63-S02图的遗漏错误边不能当完整CFG。
D00只能申报documentation_disposition_submitted；不自行关闭R63-01或给63签PASS。不改内核，不重做63 A/B/C；安全独立专题可继续，不以文档状态阻断所有取证。

## 4. G01／A：异常表记录及链接声明

完整读取固定mykernel/arch/x86_64/include/asm/asm.h中_ASM_EXTABLE_TYPE、_ASM_EXTABLE_UA及选中分支的直接依赖；合读uaccess_64.h::copy_user_generic的标签/操作数/表注记。先区分源宏、可能生成的记录、最终ELF及运行时消费者四层。
每个.long的相对值须以对应字段的当前位置为基点，不擅自把两个相对值共用一个记录起点。类型常量和记录解释者若被采用须读真实定义；只做显式数值/布局模型，不运行汇编/重定位或猜实际地址。
完整读取mykernel/scripts/target_kernel.cmake及其指定arch/x86_64/kernel.lds；options_flags.cmake作为选项上下文。核是否有显式__ex_table/相关边界符号/保留条目及实际消费者引用，但“脚本未显式列出”不等于链接后一定被丢弃，也不证明orphan section的位置或gc最终效果。
允许对异常表结构、搜索/修正入口作有界正规定位。找到定义则读完整目标定义和直接调用窗口；未找到留下查询范围/固定ref/返回信息，不用默认分支无结果签全仓不存在。没有最终映像或实际初始化证据，保持对应缺口。

## 5. G02／B：用户地址范围与内核访问故障的衔接

完整读取mykernel/arch/x86_64/mm/fault.c中exc_page_fault、handle_page_fault、fault_in_kernel_space、do_kern_addr_fault、do_user_addr_fault、myos_bad_area；parse_PF_errcode仅为直接诊断依赖，不扩展printk/slab。
比较access_ok的数值接受域、fault_in_kernel_space的TASK_SIZE_MAX划界、user_mode(regs)和错误位三种不同条件；只给helper-only同地址模型，不假定该地址真实被映射或一定触发#PF。跟踪正常返回、前置对象无效和所选源码循环的不同终点。
myos_handle_mm_fault保持已审12的opaque返回接口，不重开PTE/COW分支；simple_find_vma和扩栈只按R12已有限定消费。非零fault循环不是重新调用handler；零返回不是缺页已经成功修复或异常指令已重试的证明。
核本条活动路径是否消费异常表、改变用于恢复的指令位置或形成明确返回契约；注释中的fixup/OOM/signal分支不能当活动实现。缺少桥接时只判本路径未建立，不外推全部异常或所有入口。
对象/寄存器有效、实际CPU入口、irq与AC状态、汇编返回仍分栏；不把源码while(1)直接写成实测死锁，也不越过诊断分配/字符串访问失败推后继循环。

## 6. G03／C：替换指令的生产与启用链

完整读取alternative.h中ALTERNATIVE、OLDINSTR、ALTINSTR_ENTRY、ALTINSTR_REPLACEMENT、ALT_NOT、alternative及直接类型/标志定义；合读smap.h::stac/clac与copy_user_generic中的两个候选。声明、注释、元数据和运行时修补者必须分栏。
按实际源码追.altinstructions/.altinstr_replacement的记录字段和相对基点；旧指令文本、候选替换文本、CPU条件位与实际选择不同，不用宏名声称FSRM/SMAP已经生效。
有界定位rep_movs_alternative实现、apply_alternatives/alternative_instructions或本项目实际等价入口；默认分支检索仅导航，候选须回固定time核实。找到启动调用者时只读该完整函数与必要直接窗口，未知名字先正规目录/搜索，不编造标准Linux路径或凭空恢复注释。
若未找到，给清楚的已检索范围和未建立的链接，不要求无限全树搜索，也不靠“不存在”凑新发现。asm字符串call不自动适用C声明uint的实参窄化；缺实现时不认证寄存器约定或残量。
最后给三层一致性矩阵：源记录存在、构建/启用链证据、真实运行未认证；将A/B/C汇合到63保留的残量/AC/异常恢复缺口，不能因为新增表格自动关闭旧UC63-U03/U04。

## 7. 输入预算、六件与证据门

核心为asm.h/uaccess_64.h/alternative.h/smap.h、架构fault.c、target_kernel.cmake、kernel.lds及配置上下文；必要直接依赖软上限12文件，超过说明必要性，不截断强制定义。不重复读取整套mm或信号系统；有界符号定位可用最多8组定点查询及4个相关固定目录，仍未定位则记缺口，不扩为全仓扫描。
输出严格六件：MANIFEST.md、uaccess-recovery.md、facts-and-boundaries.yaml、uaccess-recovery-paths.dot、deferred-findings.yaml、evidence.md。facts至少含prior_return_disposition、source_ledger、exception_record_link_matrix、address_fault_route_matrix、alternative_activation_matrix、finite_model_cases、prior_findings_links。新条件UR64-Fxx/缺口UR64-Uxx，无数量配额。
适用agent-workspace/tasks/00-gpt-task-protocol-v2.md全部P1–P13。强制锚：D00四段真实替代；A实际异常表宏与copy标签；B上述页故障分派/错误消费关键定义；C实际替换宏、stac/clac及copy调用点。未定位的可选消费者/rep_movs实现可以留缺口，不能把extern声明冒充定义；缺已指定强制定义则如实blocked。
P2每条单一定义内原样连续1–5物理行，空行计入；不能删除空行凑长度。原件与源码先用相同pin独立回读，疑似差异再核，不继承AI指控。P6总数必须包含D00，并区分作者申报、主线判断及本次实际检查。
P9按1–13逐项记录可做文本子检查与NOT_RUN；P1只用真实短SHA，原件哈希内部可全长比较，公开文件不放40位串。P13 produced_by不得遗漏、不得猜后台。构建/链接脚本上下文与定义内短锚分栏，不伪造函数名。图分SOURCE_SELECTION/METADATA、CALL/RETURN/DATA/LOCAL_CONTROL/ANALYSIS_STOP，不把预处理/宏关系当CPU轨迹。

## 8. G04：安全发布与停止

仅原PR17新user-uaccess-recovery-64/六件及本人唯一正式交付评论可新增。全部旧件/旧评论、lead/PR16、PR元数据、master/time对执行者冻结；C05不并行写执行成果。
GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次，10/20/40秒或安全只读间隔并遵守Retry-After。保持expected-head/force=false，未知并行变化不覆盖，真实认证/策略限制不绕过；记录工具发现与实际写请求的区别。
复用已建Git对象，ref结果未知先核HEAD，评论marker查重。写后固定新提交及活动分支六件全文/blob/HEAD回读，compare自51477ed51ee2严格六件A新增、旧件零净改删。
仅申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、D00/A/B/C状态；完成STOP_WRITING，返回真实短提交和正式评论交C05，不自关R63-01、不自签64PASS、不自行启动65。
不改内核或出修复补丁，不预处理/编译/链接，不运行仓库C/ASM/脚本/旧夹具/QEMU/实机，不安装工具，不访问Owner本机/凭据/外部libc或映像，不自动付费API/子代理，不跨仓维护，不建分支/PR、不合并关闭/强推删分支，阶段3门不变。
