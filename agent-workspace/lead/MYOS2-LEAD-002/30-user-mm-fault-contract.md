---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-MM-FAULT-12
record_type: chat_user_address_fault_static_analysis_contract
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线模型选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-10-31
read_channel: connector
evidence_class: 后续任务与验收条件设计，不是第12批分析结果
authorization_ref: Owner报告11完成并要求继续；28号要求验收后同轮发布下一工作，既有阶段2和18号规则
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: MYOS2-A-C03a 代码分析执行
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
receiving_model_tier: next_tier_default
instruction_base_short12: e7ca3dbbcc06
required_review_record: CORE-USER-PID-LIFETIME-11-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PID-LIFETIME-11-review.md
same_turn_rule_ref: agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md
source_branch: time
source_pin_short12: a039d9803ade
result_base_short12: 2c410132e42d
default_branch: master
default_branch_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-mm-fault-12/
allowed_files: [MANIFEST.md, pid11-disposition.md, user-mm-fault.md, facts-and-boundaries.yaml, fault-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 2c410132e42d中的全部旧文件，包括11、10、08/09及原勘误
writer_assignment: C03a是本批七件与自己的PR17评论唯一执行写入者；交付后停写待C03验收
previous_executor_delegation: CC02旧委派不恢复；旧本机进程及未推送内容unknown
activation_gate: Owner将固定任务入口发给已有C03a，核对绑定与写前条件后连续执行M00至M04
external_execution_started: false
phase: 2
kernel_change_authorized: false
kernel_patch_production_authorized: false
preprocessing_or_build_authorized: false
c_or_asm_execution_authorized: false
qemu_authorized: false
external_libc_read_authorized: false
phase3_authorized: false
merge_authorized: false
runtime_evidence: NOT_RUN
acceptance_ceiling: PASS_PENDING_LOCAL
owner_decisions_requested_now: []
prior_item_disposition: {SR10-01: CLOSED_IN_BOUNDED_REVIEW_WITH_SUPPLEMENT, SR11-01: RETURN_PENDING_M00}
historical_candidate_disposition: PID11-ER01的05主说明C6限定已接受，其他位置须准确回读后处置
inputs_read:
  - 第11批七件、最新交付评论、本轮主线回执及28号同轮规则；范围见回执。
  - 固定time的arch/x86_64/mm/fault.c第1-460行、mm/fault/fault.c第1-240行；后者尚未在本轮主线预检中读到myos_handle_mm_fault函数体。
  - 09此前范围索引明确未展开myos_handle_mm_fault内部及其返回后的消费；04/05只作必要旧接口入口。
  - 两PR状态、直接refs、PR16完整路径与11增量compare；只作写区及任务设计预检。
open_questions:
  - 本批实际缺页分派、映射副作用与返回码的连接尚待执行取证，不能从任务书问题反推结论。
  - SR11-01与历史同义摘要在M00交付后仍由C03裁定；普通内核问题不在本批修复。
---

# 第12批：用户地址缺页怎样进入内存处理，处理结果又怎样被使用

## 1. 目标与连续工作包

第11批PID主体静态成果保留，SR10-01已由主线限定关闭；本批不重做10/11。先以M00完成11的一组接口表/证据关联修正和05历史表述限定，再继续独立的缺页分析。修正不单独停一轮，不要求Owner为内部步骤另作选择。

选择缺页链，是接续09明确未覆盖的myos_handle_mm_fault内部及返回后的消费，而不是扩大到整个MM系统。核心范围固定为：架构侧用户地址缺页入口与VMA判断 → 内存侧页表/缺页分派及代表分支 → 返回状态与架构侧消费。04已有分配器/页表概览、05已有文件映射读链，不重新全盘审计或以旧结论代替本批关键回源。

Owner启动句：

> 执行CORE-USER-MM-FAULT-12。完整读取30号固定任务和11主线回执，沿用已有接收及GPT-6＋Extra。先完成M00的SR11-01与历史表述处置，再连续完成M01–M04用户地址缺页控制流、VMA/PTE分派和返回结果消费的静态取证、自查及原PR17回传。只写本批七件，旧件冻结；普通问题入库，不修内核、不重做旧批。完成后停写交C03验收。

该入口不是重新接收，也不自动唤醒聊天。已有授权内自主安排阅读和文本核对；实际访问/权限冲突或不明并行写入才突出阻断。不依赖争议的安全取证可继续，未完成部分如实标PARTIAL。

## 2. M00：绑定、SR11-01及历史表述处置

先读本任务、required_review_record全文，核验result_base、time、PR17与执行身份。18号普通问题延期、24/25号执行/验收分工、28号同轮安排继续有效；不重跑25号接收，不沿用11已经结束的写权。源码在固定time，旧结果在2c410132e42d，指令在lead，三者不能混读。

新增pid11-disposition.md，一次完成以下定点处置，所有旧件冻结：

1. **SR11-01 / PID11-OWN11：** kill_proc_info的API返回是int状态，不是task_s*。将声明/返回语句、内部借用的pid/task指针、是否增加引用、正常或失败条件分开记录；正文、所有权表和引用该表的摘要同步给追加读法。NULL目标的局部-ESRCH和有目标的发送返回不能越过此前未初始化/生存前提变成全输入保证。
2. **SR11-01 / PID11-E38：** A93仅为声明与for入口；实际find_vpid调用改用已有A93b/A94，保留A93只作上下文。对同一接口的E39/OWN11及正文做一次有界同类核查，不重造全125锚点检查。
3. **回执的定位补足：** ST14涉及do_exit到do_task_dead的调用点补已有A84b；A84/A86/A87分别保留其局部含义。P00引用旧SW10-A20不能单独证明发送方未初始化实参，按主线补足读取A15、赋值注释上下文与A19；不重新打开已关闭的SR10-01。
4. **PID11-ER01历史限定：** 精确回读05主说明C6原句和deferred-findings.yaml中UV-15.claim。对“已改挂到1号”“最后必达do_task_dead”等超出证据的读法给可直接采用的替代文字；若UV-15原句没有该概括，记录无需改动，不为凑位置虚构错误。保留原等待/回收缺口和UV-15编号。将PIDTYPE_PID=0说明为未提供有效namespace、形成空指针前提风险，不声称编译器必拒绝。

每项写清原位置、接受/部分接受/有证据异议、短引文、替代记录、受影响ID和限制。API返回值与函数内部对象不再共用含混字段。旧图无需整体复制，必要时提供精确局部替代说明；本批accept仍不是主线关闭SR11-01。

## 3. M01：缺页入口、访问来源和VMA选择

先读mykernel/scripts/options_flags.cmake，保留其外部CMAKE_C_FLAGS及未预处理的未知。实际源码入口为mykernel/arch/x86_64/mm/fault.c中的exc_page_fault、handle_page_fault、fault_in_kernel_space、do_user_addr_fault及错误处理边界。固定对象完整读所选函数，不凭Linux同名函数预设行为。

区分故障地址所在范围与发生访问时的权限级；核address、error_code、regs、current->mm的来源及user_mode/写/取指标志怎样变成FAULT_FLAG。入口C函数存在不等于已验证IDT/汇编绑定；如引用该绑定，只定点核必要调用点，否则把C入口列为本批边界，不声称完成异常入口或ISA审计。

定位simple_find_vma的实际定义，说明它返回包含地址的VMA、后继VMA还是其他对象；不要凭函数名推断。按无VMA、VMA内、位于起始地址之前、可向下增长、扩栈成功/失败区分。核mm为空及原子/中断上下文保护、访问权限检查、锁操作哪些活动哪些注释；无需展开VMA插入/删除或完整mmap/mprotect实现。

非法地址、错误对象、未初始化值或未定义控制流只给文本事实与条件风险，不能预填固定errno、保证SIGSEGV或断言真实机器必崩溃。内核访问用户地址与用户态访问用户地址也不能合成同一输入类别。

## 4. M02：myos_handle_mm_fault的分派与代表分支

在固定mykernel/mm/fault/fault.c定位并完整读取myos_handle_mm_fault实际函数体，沿真实调用定位页表分配与PTE处理的直接依赖；如函数名、结构或路径与任务预检预期不同，按实际源码记录，不制造同名Linux实现。

说明vma/address/flags怎样进入vm_fault等上下文，页表各级缺失/存在的分支、必要的分配失败和返回路径。只为闭合本批断言核所需页表辅助宏、类型和返回常量，04的allocator与页表整体不重做。

建立实际存在分支的矩阵，覆盖匿名缺页、文件后备缺页和已有PTE的写保护分派等代表情况；某分支仅注释、空体或不可确定时也要明确。若通过vm_ops等函数指针，必须同时给调用点、赋值绑定、目标函数证据，不能画成猜测的直接调用。

各代表分支只追到首次关键页/PTE变更和向上返回状态所需的直接处理层，或清楚的外部/未核边界。区分调用者检查过的输入、被调方所需前提、映射/页数据副作用与返回值。遇文件读后端、块设备、完整COW复制、slab、回收/LRU或全并发语义时引用既有端点并停边界，不把所有依赖变成新子项目。缺少返回或关键参数未定义时，不能以“静态分析”为由继续给确定成功结果。

## 5. M03：返回状态消费与恢复边界

沿代表分支 → myos_handle_mm_fault → do_user_addr_fault → handle_page_fault/exc_page_fault，逐层列返回类型、数值/位集合、实际检查和副作用。特别区分vm_fault_t标志、errno和ENOERR的真实定义，不能因为都叫错误码就混用。

核架构层对0和非0结果的处理，是否重试调用、仅循环检查不变局部量、错误分支被注释，或确实离开当前函数；必须以活动语句证明。返回0只说明走到相应分支，不能自动证明页表已经可用、权限已正确或故障指令会在硬件上成功重执行。非0也不凭标准Linux语义推导信号交付/OOM恢复。

将07/09有关用户地址访问和信号帧的已有接口作必要衔接：缺页处理能否为这些访问提供可读的恢复路径，哪些仅是候选。不得把异常返回自动等同09的系统调用返回尾；汇编、硬件重执行和copy_user_generic/alternatives完整后端不在本批展开范围。

输入/结果表至少区分：地址范围与访问来源；VMA存在/空缺/增长路径；读、写和取指标志；PTE不存在/存在/写保护；必要的分配失败；返回0/非0及不返回。可合并同源行，但必须标明实际覆盖和未知，不能用笛卡尔积凑测试数量。所有表都是静态条件矩阵，不发送真实缺页或运行内核负例。

## 6. M04：七件交付与语义自查

仅形成allowed_files列出的七件：MANIFEST列M00–M04完成/部分完成与范围；pid11-disposition为定点修正；user-mm-fault给连续可读说明和输入/结果矩阵；facts保存精简能力、关键分支/边/证据；DOT与事实一致；deferred只列新增、旧项证据追加和必要勘误候选；evidence记录真实读取与核对及NOT_RUN。

每条关键源码结论附真实路径、符号及1–5行逐字引文，集中保存锚点、其他位置用ID引用。引用文字匹配、所在符号、确实支持该断言分别核验；不得仅凭附近文本或锚点存在宣布调用边正确。无强制锚点数量，不复制旧大型事实库、不把句尾或空行拆成大量独立证明。

call=调用方→实际被调用方，宏展开附绑定；经中间层的路径须显式展开或独立标注，不冒充直接call。data=消费方→提供方，init_order=后使用者→先决条件；写入/删除/状态改变放effects。API实际返回类型和值与内部对象、借用引用分别记录。新ID以本followup限定，旧ID不改名。

主动反证：有VMA是否等于访问合法；调用分配器是否等于已有有效PTE；返回0是否等于异常可恢复；非0是否实际产生信号；检查被注释是否足以证明全树无保护；真实引文是否确实支持图边。负向结论写清文件范围、关键词、活动/注释和截断情况，未做全树搜索不能声称全局不存在。

可在自己的临时沙箱用已有文本工具处理已取得的固定文本、检查YAML/JSON、引用和图；不安装、不执行仓库脚本、不建C/ASM夹具、不操作Owner工作树。实际机械检查、人工复核和未执行项分开；不冒称旧检查器重跑或后台模型独立交叉审查。无需Deep Research、额外模型/API或付费后备。

主线验收检查M00准确处置、VMA/页表分派与结果消费闭合程度、关键条件和图一致性、证据及未覆盖诚实、七件齐全与真实diff。普通缺陷按18号入库，不阻断独立安全取证；必要路径未取得则PARTIAL，不用TODO清单冒充完成。最终只回传READY_FOR_MAINLINE_REVIEW，不自签PASS、不自动关闭SR11-01或旧缺口。

## 7. 写入、恢复与交回

继续PR17和claude/dazzling-cori-q0dnyt，首次写前应仍为2c410132e42d。本批仅C03a获七件新文件及自己的PR17评论权限，C03继续独占lead及PR导航/验收。CC02旧委派不恢复。不修改PR标题、正文、Draft状态或他人评论，不创建/改名分支或PR。

写前复核master直接ref、time、执行头、全部可见开放PR及路径交集；主线本轮核得只有16/17开放，lead路径及11新增路径与本前缀分离。time前进时本批仍是旧固定对象分析并登记新头，不静默换源。目标目录已存在或执行头不明漂移时先读来源和是否已交付，不覆盖、不重新启动同名工作。

允许七件分次顺序提交，以上次自己已核提交为期望头；交付前只可按当前blob SHA修订本批自己的文件。2c410132e42d旧件全冻结，正式交付后本批亦冻结；无原子分支头条件的接口必须保留竞态限制，不宣称锁库。写调用结果不明先远端读回查成功与否，不盲目重试。

可信部分需要保全时使用七件中的适当文件或自己的进度评论，写PARTIAL、已读/未读、当前短提交与下一安全动作，不造第八件或另起验证框架。写后按最终固定提交完整读回七件，并按分支核blob/头；compare 2c410132e42d到交付头只能是本批七路径A新增，无旧件净修改/删除及不明提交。

最终PR17评论给工具复制的短提交、七件入口、实际diff/回读结果、范围和限制；后置核验放评论，避免文件自引用导致循环修改。完成后停写，Owner只需通知C03，报告由主线读GitHub；下次验收依28号仍同轮发布后继，不自动唤醒任何聊天。

## 8. 原禁止动作不变

不修改内核/用户程序/旧脚本/旧成果，不生成内核补丁，不预处理/编译/运行C/ASM/内核/QEMU，不安装、挂盘或读取凭据，不读取仓库外libc/镜像，不直写master/time/lead，不合并/关闭PR、删除分支、force-push或进入阶段3。普通问题只入库，当前没有Owner修复决策。
