---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-IRQ-REGISTRATION-DISPATCH-101
record_type: bounded_irq_registration_route_dispatch_contract
evidence_class: D00文档纠正与三个关联静态专题；非中断运行或修复许可
conversation_display_name: MYOS2-A-C07 内核分析主线续建
produced_by: GPT-6 Astra Pro（当前会话模型标识；后台不可独立认证）
model_per_owner: gpt pro（Owner本轮明确告知已切换）
date: 2026-11-18
base_snapshot: time（分支名）
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: dd07c3b19fa1
lead_before_publication_short12: 5bfc02244f5d
master_direct_short12: e8e60cd19540
fixed_contract_commit: RESOLVE_FROM_OWNER_FORWARDED_144_ACTUAL_FIXED_URL
executor_display_name: MYOS2-A-C06e 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
executor_rotation: false
mainline_owner: MYOS2-A-C07
mainline_takeover: TAKEOVER_CONFIRMED
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-irq-registration-dispatch-101/
allowed_files: [MANIFEST.md, irq-registration-dispatch.md, facts-and-boundaries.yaml, irq-registration-dispatch-paths.dot, deferred-findings.yaml, evidence.md]
D00: REQUIRED_R100_01_APPEND_ONLY_DOCUMENTATION
batch100: RETURN
R100_01: OPEN_UNTIL_SEPARATE_C07_REVIEW
batch99_composite: PASS_PENDING_LOCAL
R99_01: CLOSED_DOCUMENTATION_ONLY
activation_gate: OWNER_FORWARDS_ACTUAL_FIXED_144_TO_EXISTING_C06E_AND_G00_PASS
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
phase: 2
inputs_read: [C06正式确认及本C07接管回读, Owner本轮设计指令, 100正式回执及142合同, 110及124, 65回执及文档闭合, 03调度集成回执的历史边界, 固定time中断入口的有界划界预读]
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0, scope: 本合同为任务要求；本轮三条划界短锚另计于检查点}
owner_decisions_requested_now: []
open_questions: []
---

# 第101批：100文档纠正；中断注册、路由与分发

**执行：现有C06e / CHAT-EXEC-14，次一档，沿用GPT-6＋Extra。交回：本C07主线，Owner发送验收消息前选择Pro；同轮独立验收并安排后继。** 不新开或轮换执行对话，不向旧C06交回，不联系退休13/12/11/10/09。

## 1. 授权、目标和增量

Owner在C07已完成TAKEOVER_CONFIRMED之后明确告知当前对话已切至gpt pro，并要求开始设计下一批；据此恢复28/79/110/124约定的主线设计与发布。143确认中的101暂停保留为历史，不覆盖旧文件。设计发布不等于另一聊天已经收到或开始；本14在Owner转发本144的真实固定入口且G00通过之前继续STOP_WRITING。

D00先追加第100批R100-01纠正；A核中断描述符的注册、替换、撤销与回调所有权；B核IOAPIC路由索引、描述项表示和访问请求；C核HPET与本地LVT定时器接入分发、应答及返回门。三专题共用一次预检和一套六件；不要求凑新增问题、引文、模型或篇幅。

本批着眼于第100批启动准备之后，处理函数如何被登记、事件如何对应到同一描述符、处理后如何应答。源码调用、注册完成、路由请求、硬件送达、处理正常返回及真实CPU上线分别立账。前序有界缺口不妨碍独立静态取证，也不能被当作已满足的运行前提。

不重做100三专题、65异常帧/GS/iret、86/87 perCPU分配、旧调度/定时器整包；不重开已闭合R99-01/R65-01。排除全部IRQ驱动枚举、ACPI/PCI/MSI域、完整时基/调度器/控制台实现、CPU硬件实验和修复。保留旧HPET/jiffies、CPU0及调度问题的完整qualified_id；不把重复看到旧源码计作新根因。

## 2. 固定输入、规则和G00

L=agent-workspace/lead/MYOS2-LEAD-002/；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。所有短SHA均为连接器实际输出前12位；API写入租约须使用直接查询取得的完整对象。

全文新读Owner转发的本144、同一实际发布提交的checkpoints/2026-11-18-c07-irq101-design-ready.md，以及L/reviews/CORE-USER-SMP-BOOT-PATH-100-review.md。全文读L/143-continuation/03-C06-to-C07-handoff-CONFIRMED.md及正式确认评论6973355370，识别旧C06停写和本C07接管；历史暂停由本144及Owner当前转发解除，不要求回旧C06再次确认。

全文刷新L/63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md、110-owner-communication-and-delivery-continuity.md、124-model-routing-and-pro-message-budget.md，以及agent-workspace/tasks/00-gpt-task-protocol-v2.md全部P1–P13。原九份MyOS2规则及已适用Mnemosyne指导，只可按本14本人此前实际读过、Git对象相同的范围逐件明确复用；缺项补读，不借用C07或退休执行者的已读资格，不重启138接收仪式。110/124以后主线交接继续显式入包。

100原六件固定dd07c3b19fa1的R/user-smp-boot-path-100/：MANIFEST.md、smp-boot-path.md、facts-and-boundaries.yaml、smp-boot-paths.dot、deferred-findings.yaml、evidence.md；本人同一有效14会话已全文读过且逐件blob一致可明确复用。D00涉及的六条旧证据、消费者、总括及读取账须重新定位。读100作者评论6971992040、100正式审核评论6972606488及本101正式发布评论（实际ID由启动词给出），原件中的旧状态按后置正式回执消费。

同一发布提交全文读L/reviews/CORE-USER-EXCEPTION-ENTRY-65-review.md及CORE-USER-EXCEPTION-ENTRY-65-DOC-CLOSURE-review.md，保留R65-S01/S02和R65C-S01；只为边界消费，不重审旧63条。完整读取CORE-SCHED-INTEGRATION-03-review.md的§2、§3、§5及CORE-CHECK-01-recheck-02-review.md中HPET/jiffies/CPU与中断相关条目；这不是旧03状态重裁。若实质引用旧原件，按其回执定位实际固定MANIFEST/deferred和相应条目，保留完整qualified_id及后置纠正，不加载全历史。R100-S01/S02/S03及其明确引用的86/87/97/99限定继续适用；本人同对象已读可明确复用。

G00只读：核四direct refs、PR16/17、开放PR、101新目录及作者/主线marker。预期exec=dd07c3b19fa1、time=a039d9803ade、master=e8e60cd19540，lead为Owner转发本144的真实新发布头；不要去发布前5bfc02244f5d寻找本144。PR16 Open/Ready，PR17原title/base/Open/Draft且均未合并；缓存base_sha不能代替master直接头。新目录/评论已存在或未知推进，先核归属，只停冲突写，不覆盖、不回退、不自动变基。

回源公约§1第2条整句、固定time与direct master的options_flags/panic双金丝雀；任何配置或有限否定前全文读time options_flags.cmake。搜索默认master只用于定位路径，每个技术锚必须回到a039d9803ade。声明、定义、源码中的调用和实际选入/执行分开；本轮不预处理。

记录新读/本人复用/未读和真实范围；每次评论查询记录since、分页及返回范围，不能把局部查询说成全历史穷尽。G00全部通过后连续完成D00、A/B/C、自检、发布与回读，无需再向Owner请求逐项批准。

## 3. D00：R100-01追加纠正

只在101六件内追加D00与supersedes/source_ref字段，不修改100六件、旧评论、原SP100-E编号和原101/101/0；不重做整个100。逐项记录旧ID、固定来源、原类别/选择符、实际定义或命令边界、保留/修正/撤回、替代新锚、所有受影响消费者及仍缺前提。

- SP100-E063：原文是两条独立list命令，撤回将其当成一个CMAKE_UNIQUE_COMMAND_BLOCK的资格。分别为mykernel/CMakeLists.txt中list(REMOVE_ITEM KERNEL_C_SRCS ${KBUILD_FILES})和list(REMOVE_ITEM KERNEL_ASM_SRCS ${KBUILD_FILES})设新单命令短锚；不得合并或只靠旧主线Q1/Q2冒充本人完成。
- E032：回到options_flags.cmake中真实set(CMAKE_C_FLAGS ...)追加配置的那条命令，含CONFIG_NR_CPUS的原文与初始flags命令区分；撤销set(CMAKE_C_FLAGS_CONFIG)为真实源码名称的解释。
- E061/E062：分别回到file(GLOB_RECURSE KERNEL_C_SRCS ...)及file(GLOB_RECURSE KERNEL_ASM_SRCS ...)，不得沿用自造的子命令名。
- E069/E070：回到arch/x86_64/kernel.lds的真实.text输出节；.text_output只能作为作者说明别名，不能冒称实际节名。

撤回MANIFEST、facts、正文/正式评论中“101条全部满足定义/单命令资格”的总括。B-L01/B-L04/B-L05/B-L06/B-P06及实际引用链逐项对齐；六个旧ID是处置集合，不是六项内核故障。原字面命中与合格证据单元分别记账，不把旧账机械改成100或按拆锚机械改成102。新101包按自身全六件原始文本独立计数。

落实R100-S01：重新取得setup_per_cpu_areas内load_direct_gdt调用、fill_cputopo_info末尾游标返回、target_include_directories(kernel)真实包含目录的必要支持；补percpu_area.c本人全文读取及asm/apic.h实际接口/extern上下文的读取账，不能倒写为100交付时已经完成。实现调用成功、头文件实际选中和ABI仍分开；extern/include/原型不能充当定义锚。

落实R100-S02/S03：在D00消费映射中保留计数/分配/合法访问、稳定表项、地址与值、最早止证门；纠正作者评论中压缩的BSP调用箭头，分别表达同一调用者的两个调用。图结构不等于真实执行；不解释未取版本化手册的MSR为已成功硬件发令。无需为复述限制重跑18个模型或重绘旧图。

D00终态只写CORRECTION_SUBMITTED_PENDING_C07_REVIEW。主线第100批仍RETURN、R100-01仍OPEN，待C07以后独立关闭；第99批R99-01已文档闭合，不得从100旧MANIFEST恢复成OPEN。

## 4. A：描述符注册、替换和撤销

主源：mykernel/arch/x86_64/myos/interrupt.c全文；mykernel/arch/x86_64/include/obsolete/arch_proto.h中ioapic_map_s、hw_int_controller_s、myos_irq_desc_s真实定义及接口上下文。单独核irq_descriptors对象、NR_VECTORS及索引类型；函数参数名irq/irq_nr不自动代表同一命名空间。

完整追register_irq、unregister_irq与hwint_irq_handler的直接相关体：字段发布顺序、handler/controller/parameter/irq_name保存还是复制、install/enable与disable/uninstall顺序、回调参数及返回值消费、清除顺序、函数最终返回。分别讨论空controller、部分空回调、重复注册/同槽替换、撤销和处理过程中的可见字段。不得由return 1证明安装成功、由清零证明已经等待在途处理结束，或由静态存储声明证明真实启动时已清零。

callback自行改变描述符、重入及多CPU只可作有明确前提的条件模型；不能把源码先后自动升级为原子发布/互斥或真实竞态。没有并发观察，不借旧锁问题推导本路径必然执行。任何无效索引或对象访问发生后，正常执行证明在最早无效步骤停止。

交descriptor_lifecycle_matrix、callback_ownership_and_return_matrix。每行列前态/输入域、写入/调用顺序、返回门、所有权、前置对象与止证点。有限模型覆盖边界索引、空回调、同槽替换/撤销及install结果处理；只选能区分结论的场景，不排列所有组合。

## 5. B：IOAPIC路由索引、RTE与访问请求

主源：mykernel/arch/x86_64/myos/apic.c全文；mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h、mykernel/arch/x86_64/include/asm/irq_vectors.h、mykernel/arch/x86_64/include/asm/apicdef.h相关定义。完整读IOAPIC_init/enable/disable/install/uninstall、ioapic_rte_read/write、IOAPIC_edge_ack/level_ack；读取ioapic_retentry_T及ioapic_map_s实际字段，不能用函数原型作实现锚。

建立vector→descriptor下标→IOAPIC输入号候选→寄存器索引的域表，分别核FIRST_EXTERNAL_VECTOR、NR_VECTORS、循环初始化范围、无符号减法/乘加和uint8_t参数窄化。IDT可容纳、descriptor可索引、软件初始化过某RTE、设备实际存在该输入，四者分开。不得把LVT/IPI向量套进IOAPIC转换而省略有效域。

RTE读取高低半、写入低高半、index+1及掩码读改写按真实顺序记录；MMIO地址建立、映射有效、访问宽度、设备模式、屏障及udelay实际实现/返回分别列门。mb存在不等于锁、两个写请求不等于原子更新、常量物理地址不证明设备实存。必要直接宏/包装取真实定义；未核的callee保持opaque。

以mykernel/arch/x86_64/kernel/hpet.c的myos_HPET_init、HPET_int_controller作唯一外部路由登记实例：完整核局部entry的各字段赋值与其余字段状态，参数指针进入IOAPIC_install的同步读取及是否保留，类型重解释/对齐/位域布局/大小和目标字节序各自列前提。不默认packed位域等于可移植64位寄存器编码，也不能在合法读取尚未证明时继续推出确定硬件值。

交vector_to_route_domain_matrix、rte_representation_and_access_matrix。边界算例至少区分最低合法输入、初始化范围末项/后一项、表容量末项/后一项、较大向量与窄化碰撞；样例按实际表达式及类型选取，并说明数学算式成立不等于调用可达、合法对象或有效硬件路由。

## 6. C：HPET/LVT接入、分发、应答和返回

全文读mykernel/arch/x86_64/myos/LVT_timer.c；定点完整读mykernel/arch/x86_64/kernel/hpet.c的HPET_int_controller、HPET_handler、myos_HPET_init及mykernel/arch/x86_64/kernel/time.c的hpet_time_init/late_time_init。HPET内部能力/计数/分配实现只在影响注册前提时定点扩展，不审整个clocksource/timekeeping。消费旧HPET/jiffies结论及其限定，不重算旧时基模型或宣称新增根因。

合读mykernel/init/main.c::start_kernel、mykernel/arch/x86_64/kernel/irqinit.c::init_IRQ、mykernel/arch/x86_64/myos/interrupt.c的myos_init_bsp_intr/myos_init_percpu_intr。登记IDT/路由初始化、late_time_init和LVT注册的真实调用关系与先后；正常返回逐层保留。C06已审x2APIC及AP准备边界按R100-S02/S03消费，不能从BSP调用推导AP也执行同样初始化。

只为IRQ接合定点读mykernel/arch/x86_64/kernel/idt.c::idt_setup_apic_and_irq_gates、mykernel/arch/x86_64/entry/entry_64.S的irq_entries_start及实际公共入口、真实IRQ_NR布局依赖，连接到mykernel/arch/x86_64/myos/interrupt.c::excep_hwint_context/hwint_irq_handler。沿65回执区分向量与错误码、汇编宏展开与函数调用；不重做CPU帧/GS/iret整包。未证明的ABI、布局、IDTR加载成功和硬件入口到达保持独立门。

逐项列HPET与LVT的注册向量、controller五回调、handler参数、ack实参及真正消费方式。区分handler返回后重读controller与一次稳定快照；不因局部irq_nr为0就直接宣判所有应答错误——分别核edge、level、LVT的实际参数使用。LVT_timer_init直接请求ack和分发后的ack是两个调用位置，不能合成单次实机事件。

对LVT_timer_ack中的TMICT/LVTT/TDCR/EOI请求只核真实常量、包装与次序；LVT对象位域/指针重解释、寄存器初值及模式、真实频率/周期与投递完成另列。lvt_count、SCALE与显示调用只在计数可表示、必要调用合法并返回的条件下分析；不把显示次数当真实频率，不新审控制台格式器。

末端分别记handler未正常返回、可请求ack、C分发可返回、in_atomic门允许请求schedule及汇编后继未证；不由in_atomic为假推导发生上下文切换或成功iret。缺连接即写有界缺口，不补造在线握手。

交startup_registration_and_dispatch_matrix、handler_ack_and_return_matrix及ABC接合表/图。图边分DIRECT_CALL、CALLBACK_BINDING、DATA_FLOW、CONDITIONAL_STEP、UNPROVEN_JOIN；聚合节点不能冒充真实调用者，证据停止点不能画成CPU实际停止。

硬件语义如需引用，遵P5实际取得一手Intel/AMD手册URL、修订、卷/章/表；无一手材料则只给源码请求与条件，记录未证。不得凭记忆认证APIC/HPET规范、屏障、SIPI、EOI或定时频率；不需要为了凑结论下载全套手册。

## 7. 六件与证据、质量门

严格六件：MANIFEST.md、irq-registration-dispatch.md、facts-and-boundaries.yaml、irq-registration-dispatch-paths.dot、deferred-findings.yaml、evidence.md。正文先D00处置和A/B/C结论/未证边界；facts给D00纠正账、六组矩阵与ABC接合、必要有限模型、读账和证据索引。新候选用IR101-Fxx、新缺口IR101-Uxx；跨包始终使用followup_id::local_id，不沿用旧IR-01的短名混作新项。普通问题登记延期，owner_decisions_requested_now为空。

强制定义锚至少覆盖：D00的两条独立list命令、配置set、两个GLOB命令、真实.text节及S01所需三处支持；irq_descriptors、hw_int_controller_s/myos_irq_desc_s/ioapic_map_s分别取锚；register_irq/unregister_irq/hwint_irq_handler/excep_hwint_context；FIRST_EXTERNAL_VECTOR、NR_VECTORS、实际HPET/LVT向量链中的独立宏；IOAPIC_init/enable/disable/install/uninstall与rte_read/write/两种ack；ioapic_retentry_T与lvt_timer_t分别取锚；HPET/LVT各controller、注册入口、handler和LVT ack；真实启动调用点及IRQ入口到C分发的连接。实际使用的宏/包装/结构字段须有足以支持命题的独立锚。

每条真实定义内连续1–5行；不跨函数/宏/结构/ASM局部块拼接，不以extern、include、原型、前置注释冒充定义。CMake锚用真实唯一命令及输入/输出变量区别同名命令，链接器用真实输出节；不要再造方便阅读的伪源码选择符。强制项未取得就明确缺口及对应交付阻断，不用推断或省略规避闸门。范围只限本合同直接路径，不强制穷尽全树调用者。

输入软上限32份主辅time文本源码（D00计入，双金丝雀另计）、8组目标检索、4次必要源码目录浏览；新短锚上限105、有限模型上限24、新候选/缺口各10。上限不是配额；必要直接依赖超出时逐件说明用途，不漫游SDK/整个驱动树。优先D00与ABC主干，非关键扩展可弃尾并如实说明；强制锚不可弃。第一波DR只按P11取名称/ID定位，不继承断言或溯源。

MANIFEST十四字段：task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check；另写executor_identity、mainline_owner=MYOS2-A-C07、mainline_acceptance=PENDING及D00状态。模型按实际可见信息/Owner报告记，后台不可认证不猜测。

self_check直接列verified_claims、quotes_reconfirmed、downgraded_to_inferred三个真实整数及m+k=n；按全六件最终原始全文计数，D00引文/模板/字符串不漏算。字面命中、单定义资格、命题支持分别检查，不能用计数正确替代证据资格。所有矩阵引用须存在且实际支持对应命题。

本任务适用协议v2全部十三条。按P9编号检查：1全六件无40位十六进制；2逐字命中且单定义/单命令及支持成立；3实际路径与supersedes存在；4辅助行号合法但不作唯一定位；5原始全计数和三整数；6真实外部URL/对应原文；7/8双金丝雀与公约原句/通道；9/10无代码补丁则N/A；11准确未运行；12仅真实降级时适用；13强制锚齐全。完整本地P9仍NOT_RUN；纯文本/JSON/图引用核查不是运行验证器，隔离算式不是仓库实现执行或翻译重放。发现文档错误须在发布本包前纠正。

## 8. 发布、回读和停写

Owner转发144且G00通过后，只有有效14可以在原执行分支新101目录新增六件并发本人一次PR17顶层正式交付评论；marker：MYOS2-LEAD-002-CHAT-EXEC-14 / CORE-USER-IRQ-REGISTRATION-DISPATCH-101 / FORMAL_DELIVERY。C07不写results，C06已停主线业务写入。

写前复核真实完整exec HEAD仍为dd07c3b19fa1对应对象、新六目标与marker不存在；以该头为唯一父、既有tree为base建一个纯追加提交，update_ref带真实完整expected_sha、force=false。已有自己的成功对象复用；未知推进不覆盖/不回退/不自动变基。

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号约10/20/40秒间隔安全重试，通常首次加三次并遵Retry-After。保持expected-head/force=false；未知并行写入不覆盖。记录实际write_retry_log，区分工具发现与真实写请求；不能把工具缺失推断成没有仓库权限。永久认证/策略拒绝不绕过。

写后compare须一个单父提交、严格六added、旧件零净改删；逐件固定全文与活动内容/blob、四direct refs、PR16/17、正式评论真实ID/全文/本查询范围唯一性全部回读。文档内不得预填未发生的后验成功；真实发布和读回结果由正式评论与最终回复承接。终态READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW，然后STOP_WRITING。

向Owner给固定成果提交、PR17正式评论和简短回报；提醒下一条交回“MYOS2-A-C07 内核分析主线续建”前选择Pro，由C07同轮独立核100纠正、101成果及后继安排。不自签100/101通过，不启动102，不向旧C06送验。

禁止改旧成果/回执/评论、PR元数据/title/base/Draft、lead/PR16、master/time、内核/loader/initramfs/Owner脚本；禁止补丁、配置/预处理/汇编/编译/链接、安装/挂载/复制/清理、执行仓库程序/脚本/夹具/作者模型、反汇编、Graphviz、QEMU/实机/真实并发、读取本机凭据或设备/外部libc镜像、额外模型/子代理/付费API、新分支PR、合并关闭、强推删除、阶段3或跨项目维护。静态分析发现的问题入库延期，不要求Owner逐项裁定。
