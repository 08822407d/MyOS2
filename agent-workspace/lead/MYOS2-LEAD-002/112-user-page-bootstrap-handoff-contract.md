执行模型：次一档（沿用GPT-6＋Extra）；独立验收：本C06的Pro。继续现有C05c／CHAT-EXEC-09，不新开执行对话。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PAGE-BOOTSTRAP-HANDOFF-78
record_type: bounded_page_metadata_bootstrap_and_free_range_handoff_contract
evidence_class: 主线任务设计与权限；非执行交付或验收
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner在本C06明确告知）
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 5a8c5dc1cd38
lead_before_publication_short12: 89f84109b470
fixed_contract_commit: OWNER_FORWARDED_112_FIXED_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_display_name: MYOS2-A-C05c 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_model_per_owner: 次一档 GPT-6＋Extra
executor_rotation: false
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-page-bootstrap-handoff-78/
allowed_files: [MANIFEST.md, page-bootstrap-handoff.md, facts-and-boundaries.yaml, page-bootstrap-handoff-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R77-01]
D00: REQUIRED_DOCUMENTATION_ONLY
mandatory_supplements: [R77-S01, R77-S02, R77-S03, R75-S01, R75-S02, R75-S03, R76-S01, R76-S02, R76-S03]
activation_gate: Owner向原C05c转发本固定合同并要求执行且G00通过
executor_before_activation: STOP_WRITING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第78批：先补正77清单，再核页管理初始化与空闲页移交

保持原C05c/09和PR17，先G00，再连续D00、G01–G04，不逐专题等待批准。D00是短小的文档处置，不重做77，不修改其原六件或旧评论；三个新专题共用六件交付和一次C06独立验收。第77批当前为RETURN（仅R77-01），不是已经通过；执行者无权自行关闭。

## 1. 增量与排除

77检查了取页/清零/归还的局部活动体，却依赖有效的mem_map、zone、空闲链和初值。78追这些生产者：A为节点及页描述符数组分配、地址/PFN索引；B为区间和页元数据初始化；C为保留区/空闲区枚举、对齐分块和初次交给buddy。成果必须说明谁生产前提、在哪一步可能不能继续，不能再抄77调用链或将所有不变量假设为已满足。

不扩成完整固件内存图、引导器、所有页表映射、全memblock变更算法、NUMA、热插拔、所有page_type写者、运行期buddy/SLUB、VM/fault/exit或锁原子实现审计。不得为演示启动而运行程序。旧MC、LB、GAP、PA77、AE75、SL76保留原qualified_id；新增证据不自动修复内核，不把条件模型或一个负检索当全局结论。

## 2. G00：固定输入、规则与写权

L=agent-workspace/lead/MYOS2-LEAD-002/；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。L取Owner转发本112号URL同一真实固定提交，R取5a8c5dc1cd38，源码仅time@a039d9803ade。

完整读取本112号、L/110-owner-communication-and-delivery-continuity.md、L/reviews/CORE-USER-PAGE-ALLOCATOR-LIFECYCLE-77-review.md和L/checkpoints/2026-11-12-page77-reviewed-bootstrap78-ready.md。必须合读R77-01与R77-S01/S02/S03，不以聊天中曾出现的初步接收判断替代正式RETURN。75/76、74闭合回执已在本有效C05c独立读到且对象一致时可明确复用；缺失或有争议就完整补读。R74C-S01、R75/R76限定、73/74既有闭合和58/63误判撤回保持。

完整刷新L/63-github-write-retry-and-readback.md、L/79-work-package-sizing-and-pro-review.md、协议agent-workspace/tasks/00-gpt-task-protocol-v2.md全部P1–P13及110号。其余原九份MyOS2约束和适用Mnemosyne指导仅复用本有效C05c自己实际独立已读正文；缺项补读，不继承C06/旧对话的已读声明，不导入其它项目维护，不重放建分支/PR或主线交接仪式。C06独占lead，09只负责执行。

R/user-page-allocator-lifecycle-77/原六件为D00的固定全文输入，必须读到末尾；正式评论6821135838全文。技术重点消费facts.source_ledger、release_transition_matrix、buddy_merge_matrix、ref_release_comparison、earliest_stop_matrix、prior_findings_links和C32–C38的页初始化证据；其余121源码锚不要求为D00全部重做。原36模型和图只按R77-S01/S02/S03使用。

历史输入限定为R/mm-baseline-04/facts-and-dependencies.yaml中mm.early/mm.page_alloc相关已有能力、MC-01–MC-05、LB-01/LB-08、GAP-M08及相应证据索引；L/reviews/CORE-MM-BASELINE-04-review.md §3与L/reviews/CORE-USER-VFS-BASELINE-05-review.md §2–3；R/user-vfs-baseline-05/mm-04-disposition.md及deferred中MR-02/status_changes。已在同一C05c实际读到的固定材料可标复用；不得恢复全局“PageBuddy恒假/永不合并/全局不可达”。不全量重读历史结果或旧脚本。

直接核PR16/17、lead/exec/master/time及开放PR，预检新78目录与本人正式marker。exec应5a8c5dc1cd38，time应a039d9803ade，master直接头预期e8e60cd19540；lead以本112实际发布头为准，不拿89f84109b470父头当发布后头。未知变化先核归属，不能覆盖或强推。目标或marker已存在先查是否本任务已交，不重复发布。

实际回读公约§1(2)及time/master双金丝雀，保存startup_selfcheck_quote、branch_canary_quotes、read_channel、模型来源和覆盖范围。G00通过才对本78批解除STOP_WRITING；有效上下文不足必须报告真实缺项，不假装加载或自行换身份。

## 3. D00：R77-01原始计数的追加纠正

对固定77六件分别记录：原始保留前缀命中数、带真实定义标识的源码标签数、仅说明文字命中数和具体位置。主线可读证据为evidence121个定义标签、MANIFEST的P9-5说明另1次、其余四件0；独立以实际全文复核，不因任务书给了预期数就照填。纯文本扫描可用新建临时检查，不运行仓库脚本；若只可读计数，如实写方法，不冒充机械结果。

在本78正文D00、facts.prior_return_disposition、MANIFEST和本人新交付评论中明确：原77的121条定义标签与原六件122次原始前缀是两个统计口径；原P9-5的全包121总括不成立，历史self_check不能倒填为原来正确。覆盖原MANIFEST、相关facts.methods/正文/evidence导语的自检总括和评论6821135838对应表述的消费说明，不改变121个历史标签的存在事实，也不宣称本次重新验证了全部源码。

原77六件和旧评论一字不改。纠正只追加在78六件内，不另造第七件、勘误分支或PR。不得为凑数新增假源码锚或把说明文字算作第122项源码事实。说明文字不要再次出现连续保留前缀；可用字符名称或编码表示，避免在本包重犯。78自身P6/P9按其六件原始全文独立计数，不混入77的历史数。

D00同时确认R77-S01的两个hook标志分账、S02图的两个上下文区别、S03主线追加多级模型与候选限定已被正确消费；这些补充由主线直接给出，无需另交整套77图或36模型。执行交付时R77-01只可标DOCUMENTATION_CORRECTION_SUBMITTED_PENDING_C06，闭合须C06下一次独立回执。

## 4. G01/A：节点、mem_map存储与地址索引

核心源：mykernel/mm/page_alloc/mmzone.c::zone_sizes_init；mykernel/mm/misc/mm_init.c::free_area_init、alloc_node_mem_map；mykernel/mm/early/memblock.c::myos_memblock_alloc_normal、memblock_alloc_internal、memblock_alloc_range、__memblock_find_range_bottom_up。沿memblock.h/const/types、实际包含关系补NODE_DATA、MAX_NUMNODES、MAX_DMA_PFN、SMP_CACHE_BYTES、phys_to_virt和内存地址类型的最近定义，先证明单位与接口，再建有限模型。

列nodes分配大小/对齐/返回、NODE_DATA逐项赋值、pgdat清零与node_start/spanned生产的真实先后。分配失败不等于转换后必为NULL；不能把物理地址0的失败哨兵与非零直映射虚址混用。核alloc_node_mem_map中物理分配返回、phys_to_virt、if(!map)、memset的次序；只证明地址表达式不证明页表已映射可写。

建立start对齐、offset、end向上对齐、size=(end-start)*sizeof(page_s)、map与node_mem_map/mem_map关系。sizeof保持符号值，不编译探测。与highmem_macro.h的pfn_to_page/page_to_pfn逐式配对，分node_start为0、非零对齐、非零不对齐及空节点模型；核绝对PFN与局部数组下标是否用同一原点，不能只证明两个宏互逆就认定物理映射正确。关注乘加溢出、int循环与ulong跨度、零/无效指针和实际分配extent的最早停止点。

比较alloc_node_mem_map传给memblock_alloc_range的下界与normal包装器的下界：实参MAX_DMA_PFN及移位的真实单位分别核验，不照名字认定字节或页。size/align/end特殊常量、下界重试、reserve返回门分别登记。simple_mmblk_reserve及其最近变更callee只追到证明此处返回/容量/区间前提所必需，不顺势重审整个插入合并算法；未闭合则保留条件。

强制锚：nodes分配与赋值、pgdat/init顺序、mem_map大小/offset/指针发布、地址转换与失败门、分配范围及reservation门、PFN转换真实定义。交node_storage_matrix、memmap_index_matrix、address_unit_and_failure_matrix。

## 5. G02/B：zone范围、实际内存与页字段初值

核心源：mmzone.c三种上界写入；mm_init.c的__myos_calculate_node_pages、calculate_node_totalpages、free_area_init_core、zone_init_internals、memmap_init、__init_single_page、reserve_bootmem_region；buddy.c::get_pfn_range，memblock.c::__next_mem_pfn_range与memblock_macro.h::for_each_mem_pfn_range。补mmzone_const/types、buddy_types、page-flags相关定义和PFN宏；不把注释中的Linux初始化当活动实现。

分开zone_start/spanned/present/managed、node_start/spanned/present、nr_zones与free_area.count，列先算后覆盖的真实写入顺序。分别检查空区、边界相等、存在内存洞、首个实际PFN非零等模型。只覆盖span不等于证明每个PFN是RAM；pfn_valid的上界判断不能替代洞/保留区/数组来源检查。数值clamp/对齐/加法均须限定可表示和合法输入。

列每页_refcount、page_type/_mapcount共享存储、flags及链表lru/buddy_list的初值与实际写入；重复初始化同一union成员、对范围内未实际存在页的访问必须按证据区分，不自动裁定运行损坏。pgdat被清零与zone.lock的显式初始化请求也是不同事实，不能从全零推导已测试锁机制。PageBuddy局部初值结论仍受GAP-M08/MR-02限定，不做全树writer认证。

仅定点读取arch/x86_64/kernel/setup.c中zone_sizes_init调用及相关上界来源、init/main.c的mm_core_init调用附近，确认本阶段直接顺序；未追完整固件输入/引导汇编就不宣称全启动可达。需要外部输入时用符号前提，不访问Owner机器或启动映像。

强制锚：范围上界与各统计生产者、nr_zones赋值、后续覆盖、PFN迭代边界、free_area与页字段初始化、Reserved位处理、最邻近调用顺序。交zone_initialization_matrix、page_metadata_initial_state、span_hole_and_reserved_matrix。

## 6. G03/C：保留区排除、分块与初次交给buddy

核心源：memblock.c的memmap_init_reserved_pages、memblock_free_all、__simple_next_mem_range、should_skip_region、__next_mem_pfn_range；memblock_macro.h的for_each_memblock_type、__for_each_mem_range、for_each_free_mem_range；memblock_types/const及所用最近flag访问器。接合arch/x86_64/mm/init.c::mem_init和mm_init.c::mm_core_init，末端只到buddy.c::memblock_free_pages及77已限定的__free_pages_ok，不重做整个buddy。

把memory集合、reserved集合、两索引游标、终止哨兵和输出半开区间分开。对无保留区、保留覆盖开头/中间/末尾、多个memory区及空区给代表模型，前提为已证明或明确假设的排序/不重叠/容量。核for_each_memblock_type的实际cnt边界及尾槽访问：count、capacity、dummy/尾槽内容和可能的多一次遍历分别取证，不能见cnt+1就无条件认定越界，也不能凭“哨兵”称呼认定安全。未见flags赋值、跳过条件或变更者不完整时记录缺口。

对free区间先核PFN_UP/PFN_DOWN、max_low_pfn裁切、空区跳过，再核按__ffs/阶上限与终点缩阶进行块切分，列每块PFN/阶/覆盖区间及总量。先处理start_pfn=0、移位/加法、局部累加变量初值等最早可能失去确定性的点；若当前路径前面已有未能证明的求值，后面的块表只能明确标为隔离的条件模型，不能宣称真实函数已到达。不得因局部变量结果似乎未使用就自动假定其读取无影响，也不推断具体编译器必如何处理。

接到memblock_free_pages时核Reserved清除、引用设零、managed_pages加量与__free_pages_ok调用次序，不能把统计先加等同后续挂链成功。after_bootmem置位时点、memblock_free_all正常完成、buddy可供分配是不同断言；no-return/非法数据处停止，不作自动回滚假设。旧PA77-F01/F02、MC-01/05与真实物理回收缺口不因初次handoff被自动关闭。

强制锚：迭代宏/真实callee、游标与排除交集、reservation枚举、PFN裁切与分块、相关初值/先行求值、逐页/统计写入与末端调用、mem_init/mm_core_init顺序。交free_range_iterator_matrix、bootstrap_chunk_matrix、handoff_transition_matrix、earliest_stop_matrix。

## 7. 六件、范围与验收

严格六件见头部。正文含D00与A/B/C；facts含source_ledger、prior_return_disposition、上述矩阵、finite_model_cases、prior_findings_links；deferred区分文档处置、新条件候选、证据缺口与旧项增量。新事实PB78-Fxx、缺口PB78-Uxx，历史问题沿原qualified_id，不设新缺陷数量目标。

源码软上限24份（核心与最近依赖，金丝雀另计）、8组定点检索、4次相关目录浏览；必要超限说明具体依赖，不全树漫游。新短锚软上限140、有限模型最多36、新候选/缺口各最多12；上限不是配额。D00统计不是内核模型，不占用虚构源码证据名额。

本任务适用协议v2全部P1–P13。P9-1原六件完整40位hex串为0；2/13源码锚必须在固定源真实单一定义中、非空连续1–5物理行，发布前逐条再开源核对；3固定路径/原件与新六件回读；4行号仅辅助不过界；5必须扫描或如实逐项核全部原始前缀，包括说明文字，不能只数解析成功标签，MANIFEST三数与真实口径一致；6外部事实有真实出处，不以通用Linux知识填本树；7/8金丝雀/公约原句/read_channel齐备；9/10/12无代码/补丁/降级则N/A；11声明NOT_RUN。完整本地P9、预处理、ABI与内核运行仍未完成。

DOT将CALL、CALLER_SEQUENCE、DATA、MACRO、CONDITION和ANALYSIS_STOP分开；共享节点不能拼接不同上下文的出口或颠倒写入顺序。ANALYSIS_STOP不是内核goto。部分状态写入、callee返回和跨层参数要逐项说明。普通源码问题入库延期，不向Owner逐项问修复；文档错误由AI追加纠正，不覆盖旧成果。

## 8. G04：发布、远端回读与停止

09仅在原claude/dazzling-cori-q0dnyt/PR17新增78目录六件和本人一次正式顶层交付评论。marker：MYOS2-LEAD-002-CHAT-EXEC-09 / CORE-USER-PAGE-BOOTSTRAP-HANDOFF-78 / FORMAL_DELIVERY。旧results/旧评论、PR标题正文/Draft、lead/PR16、master/time冻结。首次写前再核exec5a8c5dc1cd38、目标路径和marker；单父原头提交，完整expected-head、force=false，未知并行变化不覆盖。

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔安全重试，通常首次加最多三次，遵Retry-After或以安全独立读取形成间隔。复用已成功blob/tree/commit/comment，只补缺项；记录真实重试与回读，工具发现与实际写请求分账，不能把一次异常误称Owner未授权。

compare原头→新头须严格六件added、旧件零净改删；固定新提交及活动执行分支六件全文/blob、直接四分支、PR16/17、本人唯一评论都须回读。真实提交/评论号取得后再记录，不提前猜写，不把文件入库等同导航或评论完成。

完成只报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及R77-01待主线闭合，随后STOP_WRITING；通过PR17交回现MYOS2-A-C06以Pro独立验收，不交旧C05、不自签77或78、不启动79。Owner只转发入口，证据经GitHub回传，无需ZIP或手工入库。

禁止内核/loader/initramfs/Owner脚本修改、补丁、预处理/编译/链接、仓库程序/脚本/旧夹具、Graphviz、QEMU/实机、真实并发、外部libc/镜像、本机凭据、额外模型/子代理/API付费、另建分支PR、合并关闭/强推/删分支、阶段3及跨项目维护。新建临时纯文本检查及不执行仓库代码的有限模型可按已有许可使用，须与内核运行区分。110号必须进入以后每次主线交接的固定必读清单。
