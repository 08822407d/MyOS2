---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-UACCESS-RECOVERY-64
record_type: bounded_exception_fault_alternative_analysis_and_R63_disposition
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
produced_by: GPT-6 (self-reported)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
base_snapshot: time
source_pin_short12: a039d9803ade
execution_base_short12: 51477ed51ee2
fixed_contract_short12: aeefb9025a55
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: DOCUMENTARY_DISPUTE_SUBMITTED_PENDING_MAINLINE
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
runtime_evidence: NOT_RUN
---

# 第64批：R63-01异议、异常表、架构缺页和替换指令衔接

本报告只审固定time源文本。宏的记录生产、目标文件中的记录、最终ELF段布局、运行时消费者、CPU真实异常及恢复，是独立证据层级。所有数值模型只以明示的字长、标签、有效对象和正常被调函数为前提；遇首次非法访问或不返回，不能把后续源码表达式、返回或硬件副作用伪装成已执行。

## D00：R63-01退回项的独立反证

第63批主线回执将旧evidence的C14/C15/C16判作删除1/3/1个原始空行而不符合P2，定为RETURN_DOCUMENTATION。独立对固定51477ed51ee2的原evidence与固定time下readdir.c进行逐字匹配，又分别使用GitHub fetch_file、同pin raw资源和fetch_blob三个通道确认同一源码Blob。**主线的三项空行遗漏指控均不符合实际原文**：

| 原标签 | 原引文与固定源真实位置 | 空行 | 独立结论 |
|---|---|---|---|
| C14 | filldir64第108–112行 | 第112行是末尾空行 | 原样连续5行，定义内 |
| C15 | 第113–117行 | 此范围没有空行 | 原样连续5行，定义内 |
| C16 | 第117–121行 | 第119行是中间空行 | 原样连续5行，定义内 |

回执建议的C14替代片段在第109行后加入实际不存在的空行；C15替代片段在连续赋值语句之间插入了空行；C16替代片段又在copy_to_user之后插入实际不存在的空行，而真正空行位于第119行。若照搬建议内容反而违反P2。此处不是仅根据58 B25的历史误判类推，而是针对实际63源Blob的独立文本对照。

依94号追加证据门，本包仍提供D01＝第108–111行、D02＝第113–115行、D03＝第116–117行、D04＝第117–121行四个真实定义内短锚，分别对应C14、C15拆段、C16。它们是新证据及替代消费入口，不代表承认原三条有误。原63的64条标签中本批只独立重核三条，其余61条不作新64批的全量通过声明；原“64/64”应区分作者历史自报与主线是否实际逐条复查。

原63 MANIFEST确实缺少produced_by，这是P13署名元数据问题。新64 MANIFEST如实填写produced_by为GPT-6（当前模型自报）、Owner指定Extra和backend_identity不可独立认证，不倒改旧63或正式评论6736044109。R63-S01仍要求sizeof(*frame)而非指针大小、用户dirent而非VFS dentry、无符号n-res计算与随后指针偏移区分、x/ptr与raw残量全部有效性前提。R63-S02要求restore_sigcontext还可能失败到badframe，原图不完整，不以一条图边认定唯一入口。58 B25已撤回的主线误判不重开。

因此D00状态是DOCUMENTARY_DISPUTE_SUBMITTED_PENDING_MAINLINE。主线63原裁定仍为RETURN_DOCUMENTATION、R63-01仍待C05独立裁决；本执行者不自关退回或自签63通过。旧六件、旧评论及旧PR元数据均保持原样。

## A／G01：异常表记录生产与链接

在asm.h中，_ASM_EXTABLE_TYPE针对汇编与C内联汇编有不同源宏版本；两者均描述__ex_table中对齐4字节、写三个long宽度为4字节的字段：故障指令标签from相对第一个long字段位置，修复标签to相对第二个long字段位置，再写type值。_ASM_EXTABLE_UA把type传为符号EX_TYPE_UACCESS；当前已读源中与类型定义有关的include被注释，不能擅填该符号的数值或断言编译成功。uaccess_64.h的copy_user_generic使用标号1和2及_ASM_EXTABLE_UA(1b,2b)，只建立产生异常表元数据的源级意图，未认证记录实际进入ELF或被消费者读取。A01–A12为单定义内直接锚。

例如假设表项从0x1000开始，from标号在0x2000、to在0x3000，第一long的位移为4096，第二long的位移为8188，因为其基点是0x1004而非0x1000；第三字段不是相对位移。facts列出10组纯数值表项，均要求符号解析且相对值在s32可表示域内，不是编译重定位实测。

固定target_kernel.cmake的target_link_options以CMake变量组成arch对应的kernel.lds链接脚本路径。选定x86_64/kernel.lds全文件没有显式__ex_table输出节及其边界，也没有显式.altinstructions或.altinstr_replacement的保留/边界。此结论严格限**本脚本没有明文登记**，不能推导最终ELF必然丢弃orphan section、实际GC是否生效或链接后的准确偏移；脚本及CMake引用在facts.source_context，而非伪造VERIFIED函数证据。

按合同8组有界默认分支源码检索、4个固定架构目录加同pin回源，尚未建立__ex_table的活动搜索/修正消费者。在所读fault.c全体指定函数中没有活动查表、修复regs->ip的语句。该负向结论只覆盖这些函数；不能宣称整个MyOS2不存在异常恢复机制。异常表与运行时修复之间的桥仍标为needs_evidence，不关闭UC63-U04。UR64-F01/F02是新增的跨界条件消费者，非已发生的链接故障。

## B／G02：缺页路由与用户地址范围

固定fault.c中exc_page_fault正常进入后先read_cr2，再调用handle_page_fault。后者按fault_in_kernel_space(address)选择分支，而该函数当前只判断address是否不小于TASK_SIZE_MAX；page_const_arch.h给出阈值为(1UL<<47)-4096。这不是x86 __access_ok的低/高符号位阈值，也不等于user_mode(regs)或X86_PF_USER错误位。合法低地址可由内核supervisor指令访问而进入do_user_addr_fault；用户态寄存器、地址值和错误代码属于独立输入。

给定64位整数与二补数符号模型，address恰等于TASK_SIZE_MAX且size16，在x86小常量快捷门和一般sum门下均可能取得数值true；若真实CPU选择产生#PF并进入同一exc_page_fault，fault_in_kernel_space却会分配到do_kern_addr_fault。此不一致只证明数值准入与fault分类边界不同，不能从地址真值推断真实CPU #PF/#GP、VMA、权限或实际内存可达。

do_kern_addr_fault中的fixup/spurious/其它错误门是注释，当前所读活动体最终while(1)。这只是正常到达该源码的控制流结论，不是实测机器卡死。若更早寄存器、异常入口或对象操作无效则不能推到末尾。

do_user_addr_fault从current取mm，user_mode(regs)决定FAULT_FLAG_USER及irq请求，X86_PF_SHSTK/WRITE/INSTR决定另几位。合法mm/VMA的附加前提下调用一次myos_handle_mm_fault；若返回值非0，后续while(fault!=ENOERR)未重新调用处理器或改变fault；若为0，跳过该循环，但不证明PTE已经正确恢复、CPU会安全重执行。找不到VMA时调用myos_bad_area，后者先kmalloc、memset、parse_PF_errcode和打印，再进入while(1)；若其中任何前置访问无效，须停止更早。simple_find_vma稳定性及扩栈限定沿R12-S01，非零结果/图的活动性沿R12-S02；旧MF12-F01/F02/F03/F04保留原status，不重编号。

上述低/高源路径都没有在所读活动体中建立：查询__ex_table→匹配故障指令→改修复PC→让copy_user_generic返回合法残量→异常尾部恢复的完整链。只做所选路径的否定，不覆盖其它异常类别、完整IDT、IRQ、物理页表和真实硬件。B段直接证据见B01–B27。

## C／G03：alternative元数据及启用链

alternative.h的OLDINSTR宏构造771/772旧指令标签及按新旧长度差的NOP填充；ALTINSTR_ENTRY生成.altinstructions节，其中先后两个相对long各自以所在字段为基点，继以4字节feature+flags和两个1字节长度。名义字段宽度4+4+4+1+1共14字节，仅是源级布局模型。ALTINSTR_REPLACEMENT把774/775替代指令文本放入.altinstr_replacement节，ALTERNATIVE顺序组合三套文本，alternative宏以asm_inline使用它们。这些宏是元数据/候选指令生产者，**不是自行运行的CPU热补丁处理器**。C01–C13提供独立定义内原始锚。

copy_user_generic的候选是rep movsb与call rep_movs_alternative，条件参数为ALT_NOT(X86_FEATURE_FSRM)，len通过+c约束，to/from通过+D/+S；smap.h的stac/clac分别选择空指令与__ASM_STAC/__ASM_CLAC，使用X86_FEATURE_SMAP。copy_user_generic的正常C源顺序为stac、内联asm与异常表文本、clac、返回len。存在这些源代码与CPU已替换候选指令是两个不同等级。不能由rep_movs_alternative的C原型uint形参就推定内联汇编call在实际ABI中将寄存器len窄化；本轮限定检索未定位函数实现体，UC63-U03保留needs_evidence。

固定bugs.c::check_bugs虽然实际调用identify_boot_cpu，但alternative_instructions()是注释；common.c::cpu_init中的同名调用亦被注释；alternative.h中的struct alt_instr及extern patcher声明同样是注释。本批按规定边界未发现能证明这两处完成实际替换的消费者，也没有最终ELF、CPU feature选择、补丁写入或执行证据。不能从这些局部事实推断全仓永久没有任何patcher，或强行签定FSRM/SMAP已开启。C14–C23为固定time直接锚。

| 证据层 | 异常表 | Alternative | 结论上限 |
|---|---|---|---|
| 源生产 | _ASM_EXTABLE_UA/__ex_table记录宏 | ALTINSTR_ENTRY与替换节 | 源宏存在 |
| 链接与激活 | 指定lds无显式节/边界，所读fault.c无活动fixup | 指定lds无显式节/边界，两处启动patch调用注释 | 未建立端到端消费者 |
| 真实执行 | CPU异常→查表→恢复PC与残量 | CPU特性→修补目标字节→执行候选 | 全部NOT_RUN |

## 有界结论与交付

D00=DOCUMENTARY_DISPUTE_SUBMITTED_PENDING_MAINLINE；原63的RETURN_DOCUMENTATION仍待主线独立裁定。A/B/C=completed_in_scope，仅说明本合同范围内源链与条件反例形成。新增UR64-F01–F07为7个条件候选，UR64-U01–U08为8个证据缺口，不是实测漏洞。MF12、UC63原ID/status及R12-S01/S02、R63-S01/S02、R62-S01/S02继续生效，58 B25误判撤回不重开。

本批六件包含66段新固定time单定义内1–5行源锚（D4/A12/B27/C23）、13条源台账、55例自建数值/状态模型（D3/A10/B30/C12）和局部DOT。仓库C/ASM/脚本/旧夹具、完整本地P9、预处理/编译/链接、Graphviz、QEMU或实机、真实异常/页表/指令恢复、SMAP/替换指令运行全部NOT_RUN。执行者只签READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，完成后STOP_WRITING，交MYOS2-A-C05独立验收，不自行关闭R63-01或启动65。
