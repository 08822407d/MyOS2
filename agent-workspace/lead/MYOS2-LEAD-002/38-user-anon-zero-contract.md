---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ANON-ZERO-19
record_type: chat_anonymous_missing_pte_and_zero_page_static_contract
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-11-01
read_channel: connector
evidence_class: 第19批授权和验收条件，不是已完成的分析
status: READY_FOR_OWNER_FORWARDING
authorization_ref: Owner报告18完成；28号同轮验收与后继派发持续授权；36号执行角色接续
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-02
executor_display_name: MYOS2-A-C03b 代码分析执行续建
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
receiving_model_tier: next_tier_default
instruction_base_short12: a6cea80edd0c
required_review_record: CORE-USER-PAGE-REF-LIFETIME-18-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PAGE-REF-LIFETIME-18-review.md
source_branch: time
source_pin_short12: a039d9803ade
result_base_short12: 15a162f13089
default_branch: master
default_branch_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-anon-zero-19/
allowed_files: [MANIFEST.md, anon-zero.md, facts-and-boundaries.yaml, anon-zero-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 15a162f13089及此前全部文件，包括18及旧勘误
writer_assignment: C03b仅本批六件及自己的PR17评论；正式交付后停写待C03验收
old_executor_delegation: C03a及CC02不恢复；旧本机草稿和物理进程状态unknown
activation_gate: Owner向已有C03b转发本任务且固定输入及写前核查通过后Z00–Z04连续执行
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
inputs_read:
  - 18六件及交付评论、本轮18回执；17回执与18/24/25/28/36/37既有规则。
  - 固定time fault.c 1–91和770–960的init_zero_pfn/do_anonymous_page；同源此前已读do_pte_missing/handle_pte_fault和folio_prealloc，作为范围预检。
  - zero_pfn默认分支搜索只导航到fault.c、pgtable.h和init/main.c；尚未核完ZERO_PAGE定义、特殊PTE和匿名入口绑定，不作为19已完成。
  - 直接refs、PR16/17、开放PR第三项为空、PR16路径及18增量compare。
open_questions:
  - 匿名读缺页的零页身份/数据初始化、PTE保护和后续写入接口尚待本批取证。
  - 匿名写缺页的新页准备、发布条件及引用失败出口需要区分，不依函数名或return0签发正确性。
---

# 第19批：匿名地址第一次缺页时，映射了哪一页、哪些权限

## 1. 目标、角色和启动

18已按PASS_PENDING_LOCAL限定接收，合读R18-S01/S02/S03；不另交18勘误、不重跑84条检查。继续已有C03b及GPT-6＋Extra，不重新做25/36首次接收，也不恢复C03a写权。

本批补匿名缺PTE生产侧：真实匿名判定→do_anonymous_page读零页/写新页→保护构造、PTE发布与返回。17研究的是已有PTE写保护，18研究普通物理页引用及空闲链；本批引用其限定端点而不重做完整COW、分配器或文件缓存。不扩大成全MM、完整启动、全部mmap/exec、特殊大页或硬件隔离认证。

Owner启动句：

> 执行CORE-USER-ANON-ZERO-19。完整读取38号固定任务及18主线回执，沿用C03b已有接收和GPT-6＋Extra。按Z00–Z04连续完成匿名缺PTE入口、读零页/写私有页、PTE权限与引用/失败边界的静态分析、自查、原PR17提交及远端回读。只在指定新目录交付六件，旧件保留；普通问题入库，不修内核、不重做17/18。完成后停写交C03验收，不自行开始下一批。

工作包内连续推进，不逐内部步骤向Owner请示。关键访问、授权、输入冲突或不明并行写入才停相应路径，安全且独立的项可继续；证据不足记PARTIAL，不以文件齐全代替内容完整。发布文件/评论不会自动唤醒对话。

## 2. Z00：固定输入及消费限定

先完整读本任务和required_review_record，区分指令a6cea80edd0c、结果15a162f13089和源码time a039d9803ade。先读options_flags.cmake；DEBUG、链接/BSS与外部构建配置都只按实际文本说明，不假装执行过预处理。18号延期、24/25分工、28号同轮派发、36号C03b身份持续适用；18批写权已结束，本批重新限定目录。

必读18回执R18-S01/S02/S03，只取普通页取得/引用/归零端点；17回执R17-S01/S02/S03用于权限增位、对象身份与真实调用；16回执R16-S01/S02只取软件none/present语义和目标初态；12主线回执只取缺页入口/返回消费及既有匿名分支边界。需要旧问题时定点读取真实qualified_id，不全量搬运历史YAML，不把主线预检计入本批VERIFIED。

导航：mykernel/mm/fault/fault.c、pgtable.h和相关宏，mykernel/arch/x86_64/mm/pgtable/，mykernel/init/main.c及ZERO_PAGE/empty_zero_page实际定义位置；文件按真实include/符号定位。默认分支搜索只导航，正式证据回固定time。旧注释或Linux典型实现不可补出当前代码。

## 3. Z01：缺PTE入口和匿名身份

完整读do_pte_missing、vma_is_anonymous的实际定义/绑定、do_anonymous_page，以及解释其输入所需的myos_handle_mm_fault/handle_pte_fault对应段。明确anon由什么谓词决定，不擅自等同vm_file为NULL、VM_ANON标志或某系统调用名称。至少追一个已存在的VMA生产端的实际字段赋值到该判定；如果需要继续追踪会超出有界范围，明确入口条件及未闭合部分，不穷尽所有mmap/exec来源。

区分orig_pte的none谓词、硬件PRESENT、软件PROTNONE、初次缺页和已有PTE写保护；保留有效vma/mm/pte_ptr与实际控制流条件。do_anonymous_page的VM_SHARED拒绝、重复pte_alloc、读写标志来源及注释的锁/重检查分别记录，不把函数注释的“已持锁”算作实际取锁。

本批不重新审查架构异常汇编、完整栈扩张、系统调用返回或全部VMA分割；只读取能说明本入口权限/flags的直接上游。源头未证明的UNSHARE等情形不假设实际可达。

## 4. Z02：读缺页的零页来源与保护

追my_zero_pfn、zero_pfn、init_zero_pfn、ZERO_PAGE及其真实静态对象/初始化入口，区分零页标识、物理PFN数值0、page描述符和PAGE_SIZE数据。核实际初始化调用是否存在、何处设置PFN，清零由何动作或明确段初始化契约提供；静态对象声明/初始化不等于已证明引导加载和BSS在硬件上正确。只追必要定义及直接调用，不审整个boot/memblock。

对读分支的pfn_pte、pte_mkspecial、vm_page_prot和set_pte_at核真实宏/函数：special位是否必然清RW？构造是否继承已有保护？所选VM_READ/WRITE/MAYWRITE/SHARED与PTE标志是怎样关联的？对必要保护来源定点读取vm_page_prot的生产接口；不把名称当成只读保证，不因参数是zero_pfn就保证数据全零或特殊页不计引用。

分别记pte_ptr非NULL、NULL、原PTE在检查后变化、初始化入口尚未证实等路径。新局部entry与真正写入的PTE分开；源码中未执行的重验证、TLB/cache通知及锁都保持注释状态。是否取得零页的新增引用、是否可能被其他路径put，仅说明本路径及与17既有端点的关联，不做全系统零页引用证明。

## 5. Z03：写缺页的新页、安装和失败出口

完整读alloc_anon_folio→folio_prealloc的参数传递与调用点。复用17/18对folio_alloc包装、need_zero、NULL转换及put端点的已核限定，但本批必须核匿名写分支怎样使用返回值、IS_ERR/NULL、错误码生产者和每一个goto出口。不能把IS_ERR检查存在就当作已存在ERR_PTR(-EAGAIN)生产者。

新数据初始化、mk_pte保护构造、VM_WRITE条件增位、dirty/young/special等操作逐项拆开；“没有主动增RW”不等于“主动清RW”。释放前新folio身份、成功安装后映射持有与原有引用分列，不从PTE写入推导自动get/mapcount/rmap已执行。pte_ptr失效、分配失败、共享拒绝、IS_ERR和正常安装各行给已发生副作用、清理和真正返回。

还需说明读零页后再写如何与17接口接合：只读调用门和关键对象身份，不能重做17整套wp_page_copy；零页当前是否真的只读、哪种写入会进入写保护分支，取决于前述实际位与上游条件，不假设运行一定产生预期缺页。

返回位掩码、0和不返回分开；只沿直接上层消费复用12限定，不宣称CPU已重试或应用已恢复。对新的未清零、可写零页或引用问题只给前提/反证/待核，不把条件风险宣传成已复现泄漏或权限绕过。

## 6. Z04：交付和自查标准

严格六件：MANIFEST逐阶段completed/partial；anon-zero.md连贯说明“入口—零页/新页—保护—安装—引用—返回”；facts含对象/状态矩阵、真实调用与effects；DOT对齐；deferred仅新增必要问题或旧条目evidence_append；evidence记录实际读取、逐字锚点、自查方法及未做事项。不额外造脚本、日志或接收文件。

建议新ID AZ19-*。强制源码断言有path::symbol及1–5行逐字引文，包含实际条件、掩码、调用或赋值；完整读控制流后才能作否定断言。引文放evidence、正文/facts精确引用ID即可，不设数量指标、不切大量重叠片段。原始tab与转义保持源码真实内容；未匹配不可标VERIFIED。

call表示实际caller→callee/明确宏绑定；嵌套实参求值不画成转换助手调用外层函数。间接路径可用call_path但必须列真实中间调用层，不混入仅参数依赖。data为消费方→提供方；类型覆盖/结构关系单独标明layout，写入/引用变化用effect。调用发生条件与内部副作用条件分列，图ID相等不代表语义通过。

精简矩阵至少覆盖匿名判定真/假、VM_SHARED、读/写、zero_pfn准备有无证据、输入保护不同、pte_alloc失败、folio非NULL/NULL/错误指针生产未证、pte_ptr为NULL、检查后PTE变化、成功安装及释放出口。没有证据的输入不硬说可达，也不因无运行而省略已核控制流。

主动反证：special是否等于只读；零页名字是否等于数据已清零；need_zero是否被消费；PTE none快照是否保证写入时仍空；VM_WRITE为假是否清RW；return0是否必然安装；folio_put是否实际回收；共享静态零页与新分配页是否可套同一引用账。条件不足即收窄。

可在自己的临时沙箱使用已有文本工具做字符串、JSON/YAML、图ID及有限算术检查；不安装、不执行仓库旧脚本或C/ASM、不操作Owner本地工作树。人工/机械/未运行明确分层，不调用额外付费模型/API/Deep Research，不声称持有全局锁。仅申请READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING；上限PASS_PENDING_LOCAL。

## 7. 单写者、发布与恢复

继续PR17及claude/dazzling-cori-q0dnyt，首次写前期望15a162f13089。C03b / CHAT-EXEC-02只获本批六件及自己的PR17评论权；C03独占lead、PR导航和最终验收；C03a/CC02及导览/教学对话不获本批写权。

写前查实际master/time/执行头、开放PR及路径交集和同名目录。准备时master=e8e60cd19540、time=a039d9803ade、只有PR16/17开放且PR16无结果前缀；现场仍须复查。time前进要记录但不静默换pin，lead合法前进不是执行冲突。已有成果先核归属与状态，未知变化停写，不覆盖或重复派同名任务。

可一次或按清楚的本批自身提交顺序发布；交付前仅用当前blob身份修订本批新件。正式交付后本批也冻结。只普通快进；不另建/改名分支/PR、改PR正文/Draft/标题或他人评论。写入结果不明先读远端路径/历史，已成功则补余项，不盲重试。中断进度保存在六件适当位置或自己的PR评论，不新增第七件，不承诺后台续跑。

最终固定提交全文读回六件，再与活动分支核blob/头；compare 15a162f13089到交付头仅本批六路径A新增，无旧件净修改/删除或未知提交。后置核验记PR17交付评论及实际短提交，不反复修改文件以自引用最终提交。完成后停写；Owner仅通知C03，由主线直接读取并同轮安排下一工作包。

## 8. 不变的禁止动作

不修内核/用户程序/旧脚本/旧成果，不生成修复补丁，不预处理/编译/运行C/ASM/内核/QEMU或硬件实验，不安装/挂盘/读凭据及仓库外libc/镜像，不直写master/time/lead、不合并关闭PR/删分支/force-push或进入阶段3。继续18号普通问题延期；当前无需Owner作修复决定。
