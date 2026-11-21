---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ALLOCATOR-ENTRY-75
record_type: bounded_allocator_dispatch_zeroing_and_free_entry_contract
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台配置未独立认证）
model_per_owner: gpt pro
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 6f792023d592
lead_before_publication_short12: d46bab628653
fixed_contract_commit: PINNED_BY_OWNER_FORWARDING_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_display_name: MYOS2-A-C05c 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_model_per_owner: 次一档 GPT-6＋Extra
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-allocator-entry-75/
allowed_files: [MANIFEST.md, allocator-entry.md, facts-and-boundaries.yaml, allocator-entry-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R74-01]
D00: REQUIRED_APPEND_ONLY_DOCUMENTATION_DISPOSITION
mandatory_supplements: [R73C-S01, R74-S01, R74-S02, R74-S03, R73-S01, R73-S02, R73-S03]
activation_gate: Owner转发现有C05c并要求执行本固定合同且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
---

# 第75批：内核分配尺寸分流、清零与释放入口

继续现有C05c/CHAT-EXEC-09、次一档GPT-6＋Extra，无需新开对话。先G00，再D00及连续G01–G04，不逐专题等待批准。文档纠正和三个技术专题共用六件、一次C05验收。普通源码问题延期收存，不要求Owner逐项决定修复。

## 1. 增量与不做事项

74止于task的kzalloc、栈的kmalloc和两类kfree请求；本批只把这些调用与最近分配器入口接起来：A尺寸/缓存/大块分派，B清零请求与失败返回，C释放入口的来源/分类及直接callee交接。不是重做04内存全基线，也不证明完整SLUB或buddy正确。
不审整个空闲链、页合并、锁算法、OOM、任意并发或所有调用者；不重复74 task生命周期、11 PID、15 MM和73退出。04已登记的MC-07/MC-08、DR-04/RF-02及05后续限定须先核原身份，普通旧根因只补调用边/条件，不新号复制、不重开历史文档退回。TSL74-U04是本批接合入口，不因新增源码资料自动关闭其物理/运行部分。
kmalloc_array/kcalloc、realloc、cache自举全流程和用户空间分配器不在本包；只有真实直接依赖或强制锚缺口才有限展开，不能借补证转为全仓漫游。

## 2. G00输入、规则和租约

完整读取本107号、同发布提交reviews/CORE-USER-EXIT-REAP-73-DOC-CLOSURE-review.md、reviews/CORE-USER-TASK-STORAGE-LIFETIME-74-review.md及checkpoints/2026-11-12-task-storage74-reviewed-allocator75-ready.md。R73-01已闭合、73组合材料PASS_PENDING_LOCAL；74为RETURN_DOCUMENTATION，仅R74-01。合读全部R73C-S01/R74-S01/S02/S03，不能把74退回误当73重新打开。
完整刷新63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md及agent-workspace/tasks/00-gpt-task-protocol-v2.md全部P1–P13。其它九份项目约束和适用Mnemosyne指导只复用本有效C05c实际独立已读正文，缺项补读；不重放105接收或原始建分支PR仪式，不导入其它项目维护。
固定执行6f792023d592下完整读74的MANIFEST.md、task-storage-lifetime.md、deferred-findings.yaml；facts定点读prior_return_disposition、task_allocation_matrix、stack_lifetime_matrix、destroy_caller_matrix及TSL74-M14/M23；evidence读B12和本包依赖锚，核正式评论6812545638。
历史定点：同执行固定提交的mm-baseline-04/memory-baseline.md中mm.kmalloc能力、§3.1堆接合问答及相关MC-07/MC-08、DR-04/RF-02；原facts-and-dependencies.yaml按这些ID定位，不要求重读全文件。同主线CORE-MM-BASELINE-04-review.md §3–4与CORE-USER-VFS-BASELINE-05-review.md对应纠正；05/deferred-findings.yaml只读关联旧项status_changes及MR-01/02处置。历史原句并非当前真值，未取得有效状态则明确待核，不臆造已关闭。不得运行这些目录的旧检查脚本。
核PR16/17、直接lead/exec/master/time、开放PR、新75目录和本人marker；首次写前exec=6f792023d592，time=a039d9803ade。新主线发布107不是执行分支竞争，未知exec前进或同名目标须停相应写入核归属。公约§1第2条及time/master双金丝雀实际回读，MANIFEST保留branch_canary_quotes、startup_selfcheck_quote、read_channel及真实模型来源。

## 3. D00：只纠正R74-01

旧74六件、旧评论原样保留。新正文D00、facts.prior_return_disposition、MANIFEST、evidence及本人交付评论追加传播：旧B12引用THREAD_SIZE_ORDER却标为THREAD_SIZE，应分别提供两个独立宏的新锚和旧→新映射；再分别锚定PAGE_SIZE和PAGE_SHIFT，若写C字面量还须读_AC的实际定义/条件。每锚只含自身定义内1–5行，不用一段跨四宏凑齐。
补足正文G02、facts.stack_lifetime_matrix及M14、DOT K48等的尺寸证据；该数值是所选宏的有界计算，非预处理、ABI或实测分配。原74标签数74不变，但74/74全定义合格声明不再整体采用；其它73条本次未全量重新认证。纠正这些声明在原清单、正文、facts、evidence导语和评论6812545638的消费，不修改原件。
新75计数只统计新六件真正原始前缀，含D00新锚；说明文字不制造额外命中。状态仅documentation_disposition_submitted，R74-01关闭交C05。本批不得重做73/74、重开R73-01或58/63空行项。

## 4. G01/A：尺寸到分配入口的真实分派

固定mykernel/mm/kmalloc/slub.h::kmalloc/kzalloc；slab_common.c::__kmalloc/kmalloc_slab/size_to_index及kmalloc_caches、kmalloc_info、create_kmalloc_caches；slub_const.h对应KMALLOC_MAX_CACHE_SIZE/KMALLOC_SHIFT_HIGH和page_const_arch.h所需独立宏。DEBUG/SLUB_DEFINATION与PREFIX宏仅定位真实定义和条件，不运行预处理。
建立0、尺寸类边界、192前后、缓存上界相等/加1、THREAD_SIZE投影的分派表；分别记录入口拒绝、返回NULL、循环、索引和真实callee。表中有8K槽或注释写order-1不等于kmalloc会把8K路由到缓存，必须以实际条件为准。仅对已读边界作判断，不猜sizeof(task_s)的具体数值；task的sizeof请求保持符号值，栈按已补的宏链做有界投影。
对缓存指针是否已初始化，区分定义、创建循环的赋值与实际启动时机；必要时只定位kmem_cache_init最近调用，不扩为启动全链。fls和get_order等若参与算例，须回最近实际定义，或把相应算例明确留作待证，不凭Linux习惯补值。

## 5. G02/B：清零标志、大小与失败传播

完整读slub.h::kzalloc/slab_post_alloc_hook，slab_common.c::kmalloc_large，slub.c::kmem_cache_alloc/slab_alloc；需要时仅展开get_from_partial_list、alloc_init_new_slab和alloc_slab的直接返回门，不重审partial/full算法。沿真实include取得__GFP_ZERO、__GFP_COMP、GFP_KERNEL及必要类型。
分别列申请长度、cache.object_size、按阶容量、实际memset长度、NULL检查和标志转交。kzalloc传入清零位不单独证明所有路径均清零；所选hook、调用门及目标有效性合读。大块请求按size清零和小块按object_size清零分账，不能推断舍入后的尾部一定清零。
将task的kzalloc和stack的kmalloc作为两种调用者输入，不假定后者自动清零，也不把指针存储的零字节等同所有C字段/同步对象已完成合法初始化。分配器NULL、错误编码、忙等、不返回及最早无效对象分开；不得从未返回的callee继续演算正常-ENOMEM或成功清理。

## 6. G03/C：kfree的来源与分类边界

固定slub.h::kfree/virt_to_slab、slab_common.c::free_large_kmalloc及slub.c::kmem_cache_free/slab_free的直接交接；virt_to_folio、folio_test_slab、folio_slab、folio_order的最近定义按真实include定位，只展开分类所需字段/地址前提，止于__free_pages和已知MC-07内层断点，不重做buddy。
区分NULL早退、合法slab对象、合法大块起始指针、内部指针、静态init_stack/无关地址和已失效地址。若当前函数没有检查某种来源，仅报告所读范围缺该检查，不能凭此声称所有非法值都有确定后果。folio标志分流不自动证明对象原由这个分配器取得；kfree和直接kmem_cache_free对NULL/来源的门不能混同。
列参数求值、页/folio访问、分类、cache指针/阶数读取、释放请求及返回；order==0处告警后是否继续必须看活动代码，不能以WARN名字补出return。把74的release_task_stack后续NULL存储与本callee正常返回有条件连接，重复put时task有效性和状态门仍先于kfree(NULL)。不认证物理释放、最后持有者、ABA或并发安全，不给漏洞触发程序或修复补丁。

## 7. 六件、强制证据和验收

严格六件见头部；facts至少含prior_return_disposition、source_ledger、allocation_dispatch_matrix、size_class_boundary_matrix、zeroing_failure_matrix、free_provenance_matrix、caller_handoff_matrix、earliest_stop_matrix、finite_model_cases、prior_findings_links。新条件项AE75-Fxx、缺口AE75-Uxx；旧根因优先原qualified_id，无数量配额。
核心/最近依赖软上限14源码文件、8组定点检索、4次相关目录浏览；必要超限给原因，不全仓漫游。证据软上限120（含D00）、有限模型不超过40、新候选和缺口各不超过12；上限不是目标。单定义宏和有效源码锚优先于凑数量。
本任务适用协议v2全部十三条。强制锚为D00四宏及其真实消费；A实际尺寸比较/索引/调用；B清零位传递、实际memset长度与失败门；CNULL/folio分类及释放callee调用点。每锚非空、逐字连续1–5物理行、在标签真实定义内部；不得声明代替调用、注释代替活动实现或跨宏拼段。
P9-1全40位串禁入；2/13真实定义与强制锚逐项回源；3固定路径及写后回读；4行号仅辅助；5原始六件前缀计数与MANIFEST三数；6外部结论需可核来源，本树任务不靠外部标准填空；7/8三个结构化字段及分支差异；9/10/12无代码/补丁/降级则N/A；11不能冒称构建运行。MANIFEST记录实际覆盖、复用、模型/后台未知、D00和write_retry_log；完整本地P9仍NOT_RUN。

## 8. G04和停止边界

只在原claude/dazzling-cori-q0dnyt/PR17新增user-allocator-entry-75/六件及09本人唯一顶层正式评论；全部旧成果、旧评论、PR元数据、lead/PR16、master/time冻结，C05不代写执行区，旧08不恢复写权。
首次写前再核预期6f792023d592和新目录。GitHub暂态异常不因一次失败停工：按63号先回读实际效果，再有间隔安全重试，通常首次加三次、约10/20/40秒或相应独立核查，遵守Retry-After。expected-head/force=false，未知并行变化不覆盖；复用已创建Git对象，评论查重，发现次数与实际写请求分账。中断只补确实缺失步骤。
compare自6f792023d592必须严格六件added，旧件零净改删；固定新提交及活动分支六件全文/blob、HEAD、PR状态与唯一评论全部回读。完成STOP_WRITING，返回真实短提交、正式评论及D00/A/B/C状态，交C05独立验收；不自关R74-01、不自签75、不自行开始76。
禁止修改内核/旧件/修复补丁、预处理/编译/链接、仓库程序/脚本/旧夹具/QEMU/实机、另建分支PR、强推/合并关闭/删分支、访问本机凭据/外部libc、阶段3、跨项目维护或额外付费模型/子代理。临时纯文本检查和自建有限模型不当内核运行。发布任务不等于已投递或唤醒另一聊天。
