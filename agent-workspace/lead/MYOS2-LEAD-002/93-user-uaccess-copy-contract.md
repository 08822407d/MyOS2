执行模型：次一档，继续现有「MYOS2-A-C05a 代码分析执行」，沿用Owner指定GPT-6＋Extra。
第62批限定接收，无D00返工；先准确消费R62-S01/S02，再连续完成本批三个专题与六件交付。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-UACCESS-COPY-63
record_type: bounded_combined_uaccess_copy_contract
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近明确报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-09
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: 54101196eb11
lead_before_publication_short12: 8647a2225055
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
executor_display_name: MYOS2-A-C05a 代码分析执行
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-uaccess-copy-63/
allowed_files: [MANIFEST.md, uaccess-copy.md, facts-and-boundaries.yaml, uaccess-copy-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: []
mandatory_supplements: [R62-S01, R62-S02, R61-S01, R61-S02, R61-S03, R58C-S01]
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

# 第63批：用户地址准入、批量复制和标量访问

## 1. 目标与增量

A建立access_ok的实际定义选择和数值准入条件；B核copy_to/from_user包装、raw层、返回残量与目的缓冲填零；C核get_user/put_user活动宏及两个限定消费者如何解释成功/错误。三专题共用用户内存边界，一套六件、一次主线验收，不按helper逐批回主线。
55/59/60/62已记录用户复制返回被忽略或直接用户store，23–25/29及09可能已有相同宏的消费者证据。不能把旧缺口重说一遍就计作新根因；本批增量是定义选择、数值谓词、返回/填零关系和调用者错误门的可反驳证据。
不审整个页错误处理、异常表安装/链接、SMAP安全性、CPU替换机制、所有用户copy调用点或完整POSIX/ABI。inline asm可只读追直接契约，不执行、预处理或构建。
业务权威是Owner持续主线授权、00工作令及18/24/28/63/79；Mnemosyne只作适用指导，不接管其维护任务、不启用额外付费模型或子代理。此处“第63批”与“63号写入规则”不是同一编号。

## 2. G00：规则、固定输入及写入租约

完整读取本93号、同发布提交reviews/CORE-USER-WRITEV-CONSUMPTION-62-review.md、63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md。62为PASS_PENDING_LOCAL，无D00；R62-S01/S02的循环最终递增、零长度簇早退、局部位置与总值分离、图条件及证据口径必须准确消费，不改旧62。
61三个补充、58 B25指控撤回和更早既有裁定不重开。继续C05a/CHAT-EXEC-07，可复用本有效会话真实已读的87接收、九份项目规则及适用Mnemosyne；分别登记新读/复用。真实上下文不足时保全并报主线安排独立接收，不自行转移写权，不恢复C04c。
核direct time/master/lead/执行refs、PR16/17及开放PR、新63目录和写区交集。执行首次写前预期54101196eb11，新目录应不存在。固定time@a039d9803ade，重新核options_flags.cmake与panic.c双金丝雀；未知HEAD/竞争写入停相关写动作，不换pin或强推。
历史必读：55回执R55-S01/S02的复制残量契约；59回执R59-S01/S02的直接用户store/记录计账；60回执涉及copy_to_user和用户返回的限定；09回执及09-DOC-FIX-01关于rt_sigreturn/get_user和SR08-02的适用消费状态。已真实完整读过的固定原文可明确复用，不能以旧会话已读声明替代。
原条目先用真实目录/原deferred定位：62中WV62及所依赖BI55、DE59、FC60，09实际相关条目；若采用23–25/29的历史结论，先读对应回执和原条目。保留qualified_id与真实source_field/source_value，未找到则留缺口，不臆造ID或状态，不做全历史重验。

## 3. G01／A：地址检查的定义选择与数值域

主干固定源：mykernel/arch/x86_64/include/asm/uaccess.h的include选择上下文，asm/uaccess_64.h::valid_user_address及__access_ok，mykernel/include/uapi/asm-generic/access_ok.h中的完整fallback与access_ok宏。
从实际include先后、同名自定义宏和#ifndef门建立选择账本，分别记“源文本可见”“选定条件下绑定”“目标构建未认证”。不把generic阈值和x86符号位谓词混为同一算法；不可因两者都存在就说运行时逐一执行。
完整追size参数、__builtin_constant_p表达式、PAGE_SIZE分支及ulong加法/溢出比较。编译器是否视某表达式为常量不能仅凭调用参数写法确定；分别给条件分支模型，不假称已预处理。常量和实际类型只展开必要直接定义。
有限数值例覆盖size0/小值/跨界/大值、地址边界、无符号回绕、NULL数值与有效对象区别；显式声明位宽和有符号转换假设。谓词为真不证明映射存在、权限满足、对象存活、页面驻留或后续访问不异常。
允许用小整数抽象地址和显式64位模型，不访问真实指针、不读Owner本机或页表，不把地址数值模型叫实际CPU测试。

## 4. G02／B：批量复制、残量和填零契约

完整读取mykernel/include/linux/kernel/uaccess.h的copy_from_user/copy_to_user、_inline_copy_from_user/_inline_copy_to_user及实际选择分支；asm/uaccess_64.h的raw_copy_from_user/raw_copy_to_user/copy_user_generic完整定义。把注释中的access_ok、instrument、check_copy_size和其它路径与活动调用区分。
追两个方向是否有活动地址门、原n怎样变成返回值、from方向res非零怎样形成to+(n-res)及memset长度。先假设0<=res<=n建立全成功/部分/全部未复制模型，再独立分析该约束未证或违反时哪个表达式最早失去依据；不得把正常返回无条件等同残量合约有效。
分开源/目的对象、原请求长度、实际已复制前缀、未复制残量、填零范围和最终返回。copy_from_user的尾部清零不代表原用户数据已正确取得；copy_to_user返回0也只有在底层合约成立时才能解释为无残量。
在copy_user_generic中追len的输入/输出操作数、两个汇编候选文本、stac/clac和异常表标记的源码关系。仅展开实际直接依赖定义；rep_movs_alternative若能找到定义则限定读该实现，找不到按正规通道留缺口。其uint声明不等于asm字符串中的call发生了C实参窄化，不能用声明偷换寄存器调用约定。
不能仅凭_ASM_EXTABLE_UA字符串、注释“handles exceptions”或函数名断言真实故障必然恢复、完整残量必然正确、AC状态必然恢复。宏绑定、链接/安装、实际异常入口未审处明确STOP/needs_evidence；无需为此扩展整个fault或alternative子系统。
零长度仍按实际门与调用序列检查；不把n0当所有对象/指针表达式都可任意无效。只执行自建有限数学/事件模型，不运行汇编或仓库旧夹具。

## 5. G03／C：标量宏和限定调用者的错误门

完整读asm/uaccess.h中实际get_user和put_user两个独立宏及ENOERR直接定义；区分旧文档注释和当前活动load/store。核get_user的目标x类型参与指针转换、put_user的目的ptr类型参与赋值转换；必要时以显式helper-only类型例说明宽度/对齐前提，不能据任意宏例认定真实调用者有同样错误。
分别追正常求值后的宏结果、加载/存储本身和调用者if(error)门。若先有无效对象访问，不能补出安全-EFAULT、自动清零、错误标签或后续返回；也不能把未读的异常路径说成全仓不存在。
消费者上限两个窗口：signal_64.c的唯一MYOS_SYSCALL_DEFINE0(rt_sigreturn)完整体，核access_ok→两次get_user→调用者条件/后继，恢复信号上下文及完整返回尾段保持opaque；readdir.c::filldir64完整体，比较直接用户字段store、copy_to_user残量及计账。getdents64末尾put_user仅作注释活动性对照，不重审目录枚举。
rt_sigreturn前面的地址算术、帧对象/字段类型和get_user的合法求值前提必须写清；正常宏结果不自动证实整个frame安全。填目录项时也不能因中间copy调用有检查就认为此前/之后直接store均受保护。
09与59已知问题沿原ID和回执消费，不以本次细化重新计作新漏洞；不扩展全信号建帧、ELF栈、所有系统调用或用户ABI审计。

## 6. 输入预算、六件和验收门

核心为上述四份uaccess/access_ok头的指定定义和两个消费者窗口；必要直接依赖软上限10文件，优先类型/常量、smap/alternative/extable直接宏、实际rep_movs_alternative定义和最小frame/dirent结构。超限说明必要性，不为守数截断强制定义，也不展开全内核。
六件固定为MANIFEST.md、uaccess-copy.md、facts-and-boundaries.yaml、uaccess-copy-paths.dot、deferred-findings.yaml、evidence.md。facts至少含source_ledger、prior_review_disposition、definition_selection_matrix、address_predicate_models、copy_residual_padding_traces、scalar_consumer_error_matrix、finite_model_cases、prior_findings_links。新条件记录UC63-Fxx、缺口UC63-Uxx；无条目/引文/模型/缺陷配额。
本任务适用agent-workspace/tasks/00-gpt-task-protocol-v2.md全部P1–P13。强制锚：A两个__access_ok的独立定义及选择宏；B公开/inline/raw/generic-copy活动定义；C两个标量宏与两个消费者实际关键语句。定义选择证据与定义内短摘录分栏；找不到强制源锚如实blocked，不用INFERRED代替。
P1禁止40位十六进制串；P2每条VERIFIED必须单一定义内原样连续1–5物理行、空行计入。分别核字面/行数/定义归属/命题支持，疑似差异用同pin原件和源码真实比较，不继承AI旧误判。P3/P4实际核配置及金丝雀，否定句只限所读活动体。
P6原句、三个自检数、实际新读/复用及NOT_RUN入MANIFEST；P9逐项记可做的文本检查和未做门，不冒签完整本地P9。模型保留全部影响结果的输入，不能在摘要中丢掉加数、位置增量或路径门后仍宣称无条件结果。
图须区分CALL/RETURN/DATA/LOCAL_CONTROL/ANALYSIS_STOP，宏展开关系不假装运行时函数调用，正常错误出口不画成未定义行为，注释不当活动门。结构计数/哈希不替代语义检查。

## 7. G04：安全写入、交付及停止

仅原PR17新user-uaccess-copy-63/六件及本人唯一正式交付评论可新增；全部旧件/旧评论、lead/PR16、PR元数据、master/time对执行者冻结，C05不并行写执行成果。
暂态GitHub失败先回读是否已经生效，再按63号规则安全间隔重试（通常首次加三次，10/20/40秒或安全只读间隔，遵守Retry-After）；复用已建对象，ref结果未知先核HEAD，评论marker查重。保持expected-head/force=false，未知并行变化不覆盖，真实认证/策略限制不绕过。
写后固定新提交与活动分支六件全文/blob/HEAD回读，compare自54101196eb11须仅六件A新增、旧件零净改删；工具不可用不推断为无权限，实际未发请求不算写失败。
本人只申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING和A/B/C状态；完成STOP_WRITING，返回真实短提交及正式评论交C05，不自签63通过、不自行启动64。
不改内核、不出修复补丁、不预处理/编译/链接、不运行仓库C/ASM/脚本/旧夹具/QEMU/实机，不安装工具、不访问Owner本机/凭据/外部libc或镜像，不自动付费API/子代理、不跨仓维护、不新建分支或PR、不合并关闭/强推删分支。阶段3门不变。
