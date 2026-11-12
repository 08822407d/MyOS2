执行模型：次一档（沿用现有GPT-6＋Extra执行选择）；主线独立验收：Pro。使用现有C05c，不新开执行对话。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PAGE-ALLOCATOR-LIFECYCLE-77
record_type: bounded_page_allocation_preparation_and_release_contract
evidence_class: C06主线工作包设计与权限；非执行交付或技术验收
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（Owner本轮明确告知已切换并要求设计发布77）
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 59733f24b6d3
lead_parent_short12: 235c695863e1
rule110_publication_short12: 235c695863e1
handoff_confirmation_short12: ebbc28407ff2
fixed_contract_commit: OWNER_FORWARDED_111_FIXED_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_display_name: MYOS2-A-C05c 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_model_per_owner: 次一档 GPT-6＋Extra
executor_rotation: false
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-page-allocator-lifecycle-77/
allowed_files: [MANIFEST.md, page-allocator-lifecycle.md, facts-and-boundaries.yaml, page-allocator-lifecycle-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: []
D00: NOT_APPLICABLE
activation_gate: Owner向原C05c转发本固定合同并要求执行且G00通过
executor_before_activation: STOP_WRITING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第77批：页分配、初始化与归还的状态衔接

沿用原C05c／CHAT-EXEC-09、执行分支和PR17。Owner转发本合同后，先G00，再连续G01–G04；无D00，不逐专题等批准。约三个关联专题共用六件与一次C06独立验收。发布合同不等于另一聊天已经收到或启动。

## 1. 目标、技术增量与边界

75把kmalloc/kfree接到页接口，76停在slab的取页和归还请求；77向下补三段状态：A选区、取块与拆块；B清零及复合页元数据；C直接归还、伙伴判定及引用归还的区别。必须建立输入前提、活动写入、调用正常返回条件、输出状态与最早停止点，不只重新画入口调用链。

04的MC-01–MC-05已涉及类型位/合并、锁、清零、选区边界与引用归还。不得将再次找到这些文本登记为新根因。增量应是选区失败的准确停止位置、拆分区间与计数、实际清零区间、head/tail字段生产、回收所需前提和两种释放入口的差别；按原qualified_id追加证据。MC-07与SL76-U09、AE75-U07的物理后果仍不能仅凭本包静态图关闭。

不重做74–76或整个04，不重新验收76；不扩审全部memblock/引导、NUMA策略、内存回收/压缩、全部页类型写者、VM/fault/exit、锁或原子实现。只沿明确最近依赖补证据；缺全局不变量就保留缺口。无内核修复、补丁、运行触发程序或真实物理UAF认证。

## 2. G00：固定材料、身份与启动门

L=agent-workspace/lead/MYOS2-LEAD-002/；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。L取Owner转发本111号URL的同一真实固定提交，R取59733f24b6d3，源码取time固定a039d9803ade。

完整读取本111号及L下：
- 110-owner-communication-and-delivery-continuity.md；checkpoints/2026-11-12-c06-owner-rules-recorded-page-allocator77-ready.md。
- reviews/CORE-USER-SLAB-OBJECT-LIFECYCLE-76-review.md，必须合读R76-S01/S02/S03；reviews/CORE-USER-ALLOCATOR-ENTRY-75-review.md的全文及R75-S01/S02/S03；reviews/CORE-USER-TASK-STORAGE-LIFETIME-74-DOC-CLOSURE-review.md全文及R74C-S01。
- 109-continuation/03-C05-to-C06-handoff-CONFIRMED.md：C06已接任原主线，旧C05停止业务写入；这不是执行者轮换，也不是要求重放交接仪式。

完整刷新63号、79号、协议v2全部P1–P13。其余原九份MyOS2约束与适用Mnemosyne指导只可复用本有效C05c此前实际独立已读正文，缺失就补读；110号为追加必读。不得继承C06或其它会话的已读声明，不导入跨项目维护，不重建分支/PR。第76批PASS_PENDING_LOCAL、无返工；73/74文档闭合及58/63误判撤回保持。

R下完整读76的MANIFEST.md、slab-object-lifecycle.md、deferred-findings.yaml；facts定点读source_ledger、discard_gate_matrix、earliest_stop_matrix和prior_findings_links，evidence定位C11–C15/C26。核76唯一正式评论6818039586。75只补读allocator-entry.md有关大块/清零/释放段及deferred-findings.yaml中AE75-U07，不重审尺寸分档。

历史限定：R/mm-baseline-04/facts-and-dependencies.yaml只定点读MC-01–MC-05、MC-07、RF-01、LB-01/LB-08、GAP-M08及相应A-PA证据索引；R/user-vfs-baseline-05/mm-04-disposition.md完整读，deferred-findings.yaml定点读MR-02及相关status_changes。L/reviews/CORE-MM-BASELINE-04-review.md §3和CORE-USER-VFS-BASELINE-05-review.md §2–3合读。MR-02=CLOSED_BY_EXPLICIT_NARROWING_NOT_GLOBAL_PROOF，旧“全局恒假/永不合并/全局不可达”不得复活；旧RF-01也不是合并安全证明。

核PR16/17、直接lead/exec/master/time、开放PR、目标77目录与本人正式marker。执行前头须为59733f24b6d3，time为a039d9803ade，master直接头预期e8e60cd19540；主线以本111号真实发布头为准，不能把235c695863e1父头要求成发布后头。PR缓存base_sha不替代direct master。目标目录/正式marker已存在时先识别是否本批已交，不重复创建；未知并行推进停止受影响写入，不强推。

实际回读公约§1(2)与time/master双金丝雀，MANIFEST保留startup_selfcheck_quote、branch_canary_quotes、read_channel及真实模型来源。G00通过才解除09对本77批的STOP_WRITING；权限仅限本合同六件和本人一次正式PR17顶层评论，不恢复旧批次写权。若有效上下文无法容纳本包，保全已取得证据并报告所缺，不假装已读、不自行换执行身份。

## 3. G01/A：请求、选区、取块和拆分

核心源：mykernel/mm/page_alloc/mempolicy.c的alloc_pages，buddy.c的__myos_alloc_pages、__rmqueue_smallest、get_page_from_free_area、__list_null_or_page、del_page_from_free_list、expand、set_buddy_order、add_to_free_list。先区别page_alloc_api.h中的声明与真实定义；NUMA等注释不当活动实现。

将order合法区间、NR_PAGE_ORDERS/MAX_ORDER、GFP选择起点、prefered_zone_list三项与循环上界分开。沿mmzone_const.h、mmzone_types.h、buddy_const.h及mm/misc/mm_init.c的calculate_node_totalpages读取真实类型、宏和nr_zones生产者；用明确初始化前提列普通/DMA32/DMA及相关位组合、命中/逐区失败路径。潜在数组越界必须在首次不能合法索引的位置停止，不把越界之后的返回NULL或某个机器值当作确定结果。不能把旧MC-04“所有失败”叙述当作新的运行观察。

取块时核list_header_remove_head与后续list_header_delete_node：分别记录链接、contains、count和返回值，不能仅凭出现两个调用就说双减量，也不能从“第二次可能不命中”推出整个链安全。沿mykernel/lib/list/double_list.h及double_list_macro.h、double_list_types.h的实际callee补证据。

拆分用带有效PFN区间/对齐/同区/可表示移位前提的有限模型，列取出高阶块、每级余块范围/阶、最终交付范围、各free_area成员及count差量、private与PageBuddy状态；区分块数与页数。再核每页_refcount增量，不把atomic_inc调用当作已证明的原子或初值正确。比较分配和直接释放所请求的锁边界，但不推导真实并发必发生。

强制锚：alloc_pages真实转调；order门与选区索引/上界及生产者；取块/删除/拆分写入；最邻近链表count与返回；引用增量。A须交zone_attempt_matrix、split_transition_matrix和对应最早停止点，不接受仅抄04结论。

## 4. G02/B：清零范围与复合页状态生产

核心源：buddy.c的prep_new_page、post_alloc_hook、prep_compound_page；buddy.h的prep_compound_head、prep_compound_tail、compound_order、folio_order；buddy_types.h与page-flags.h中实际page/folio共享布局、head/tail编码和相应访问器。只沿明确包含关系补clear_page、page_to_virt及PAGE_SIZE的最近真实定义；声明、注释、宏展开候选与活动定义分开，不执行预处理器。

清零必须按实际地址表达式与clear_page写入长度推导请求区间：order0/正阶、ZERO有/无分开；GNU void*算术或其它编译语义只作明确条件，未经构建不能当实际ABI。列各次区间、并集、重叠和未覆盖部分，不把调用次数乘PAGE_SIZE当作实际不重叠清零长度，也不把未请求清零写成实测非零。下层无效/不返回时保留此前已请求的部分，不作事务回滚假设。

复合页按order与COMP的条件，列head置位、逐tail编码/private、head order及页数/计数字段的时序；区分调用者传入order、private、folio读取的order、slab cache.oo.order，不因数值可能一致就混同来源。核32位/64位移位、低位掩码、字段覆盖范围和页数组容量前提，未知编译布局保留符号化，不用sizeof探测补齐。

与75/76的连接只取mykernel/mm/kmalloc/slub.c和slab_common.c中实际alloc_slab/prepare_alloc_flags、kmalloc_large/相应释放入口及最近宏：逐项证明ZERO/COMP在进入alloc_pages前是否保留、移除或增加，区分页层清零、large的原size清零、slab对象hook的object_size清零。s.allocflags与gfp_allowed_mask只核选定最近生产者，未证初始化或后续写入不能凭字段名补默认值。说明这些生产状态如何约束free_slab的order0/PG_head等待门和folio_order读取，但不重做76槽与partial/full分析。保留MC-03既有身份与调用者限定，不声称所有调用者依赖错误清零。

强制锚：ZERO地址和长度来源、prep_new_page先后及COMP门、head/tail写入与order读取、page/folio相关共享字段、选定slab/large的标志生产。B交zeroing_interval_matrix、compound_metadata_matrix、allocator_consumer_matrix。

## 5. G03/C：直接归还、伙伴合并与引用归还

完整读buddy.c的__free_pages、free_pages、__free_pages_ok、free_pages_prepare、__myos_free_one_page、__folio_put；buddy.h的myos_page_zone、buddy_order、page_is_guard、page_is_buddy、__find_buddy_pfn、find_buddy_page_pfn、folio_put/put_page；page-flags.h及page-flags_const.h的PageBuddy/__SetPageBuddy/__ClearPageBuddy和实际字段/掩码。

直接归还逐步列地址/PFN转换、zone取得、flags清理、锁内合并请求与最终挂链。free_pages_prepare返回类型、活动语句和调用者是否消费返回值分开，不能把注释中的检查当成真实拒绝门，也不能凭签名伪造“返回false即停止”。无效指针/无匹配zone/非法阶或不返回callee出现时先记最早停止，已发生的字段更新不得抹去。区分_refcount、page_type/_mapcount共享存储、flags与private；接口名不证明已核最后持有或正确释放阶。

伙伴模型先核PFN异或、描述符地址形成、类型判定、order匹配、同区检查的真实顺序。跨洞/越界不能等到后置同区检查才停止；unsigned差值与指针计算不一概叫相同的回绕或未定义行为。只在合法描述符/对齐/同区/可表示运算下，分伙伴不命中、单级与多级命中，列原伙伴成员资格/标志、合并后范围、各阶count及新块头。查活动路径是否实际摘旧伙伴，不从Linux惯例补代码；同时保留MC-01的局部条件和历史反证，不把假设命中模型说成真实运行可达。

仅回到mm_init.c的__init_single_page、memmap_init及其紧邻mem_map初始化来源，和buddy.c的memblock_free_pages，建立本范围内已知初值/字段写入表。page_type与_mapcount别名必须纳入；未查全树写者与生命周期，不能宣布PageBuddy全局恒假或永不合并，GAP-M08仍按证据范围处理。不得以填这个缺口为由扩成全树或全部引导专项。

对照folio_put/put_page→__folio_put，只说明所读活动体与引用减量后的调用，不把它和__free_pages合并为一条完整释放链；MC-05/LB-01继续原身份。输出对SL76-U09/AE75-U07的新增证据与剩余缺口：到达挂链请求、源级成员变化、可被后继申请选中的条件模型、实际物理复用/实测UAF分别分级，绝不由前两级自动升级到最后一级。

强制锚：直接释放入口与参数来源、准备/锁/挂链顺序、伙伴判定与合并循环、相关类型位/别名/局部初值、引用归还实际活动体。C交release_transition_matrix、buddy_merge_matrix、ref_release_comparison和earliest_stop_matrix。

## 6. 六件交付、规模与验收门

严格六件见头部。正文说明A/B/C的可保留结论、条件与未完成项；facts含source_ledger、以上各矩阵、lock_boundary_matrix、finite_model_cases和prior_findings_links；DOT区分CALL、CALLER_SEQUENCE、DATA、MACRO、CONDITION、COMMENT与ANALYSIS_STOP，调用方向和入口参数须与真实定义一致。ANALYSIS_STOP不是内核goto，不把图称完整CPU控制流。

源码软上限22份（上述核心与最近依赖合计，双金丝雀另计）、8组定点检索、4次相关目录浏览；必要超限在MANIFEST说明具体依赖，不全仓漫游。证据软上限130条、有限模型最多36例、新候选和缺口各最多12项；均是上限而非配额。选代表边界与反例，不按笛卡尔积凑例。新事实PA77-Fxx、缺口PA77-Uxx；已知MC/AE75/SL76等必须用原qualified_id，只有真正独立根因才新增条目。

本任务适用agent-workspace/tasks/00-gpt-task-protocol-v2.md全部十三条。P9-1六件全文不得有完整40位hex串；2/13每条强制锚必须在固定源真实单一定义内、非空连续1–5物理行，发布前逐条再开源回核；3来源路径与六件远端回读；4行号仅辅助且不过界；5计六件原始全文全部保留标签前缀，MANIFEST三数自洽，不能只数解析出的成功条目；6外部事实须真实可核出处，不以外部常识补本树；7/8金丝雀、公约原句和read_channel齐备；9/10/12无代码/补丁/降级则N/A；11运行边界如实。纯文本/有限算术自检不等于完整本地P9，完整本地P9、预处理和内核运行仍NOT_RUN。

引用失败必须纠正、降级或删除，强制锚不得静默降级。普通源码问题分类记录；文档错误由AI在本批新件准确限定，不能暗改旧件或要求Owner逐项决定修复。已有MR-02、73/74闭合及R75/R76限定继续有效。主线本轮预读只用于设计，不替执行者逐条取证，不提前签77通过。

## 7. G04：安全发布、回读与停止

执行者只新增原执行分支下77目录严格六件及本人一条PR17正式顶层评论。正式marker：MYOS2-LEAD-002-CHAT-EXEC-09 / CORE-USER-PAGE-ALLOCATOR-LIFECYCLE-77 / FORMAL_DELIVERY。旧results、旧评论、PR标题/正文/Draft、lead/PR16、master/time均冻结；不另开分支PR、不代写主线回执。

首次写前再核exec=59733f24b6d3及目标路径/marker。用原头单父提交、expected-head与force=false；未知并行变化不覆盖。GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔安全重试，通常首次加三次，遵Retry-After。复用已成功的blob/tree/commit/comment，只补缺失步骤；工具发现次数与真实写请求次数分开记录，不能把一次异常当作Owner未授权。

compare原头→新头须严格六件added、旧件零净改删；固定新提交与活动执行分支六件全文及blob身份、直接四分支、PR16/17和唯一正式评论全部回读。MANIFEST或正式评论记录真实write_retry_log与尚未完成的步骤，不预猜提交号、不虚报评论成功。

完成只报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，随后STOP_WRITING；交回现MYOS2-A-C06以Pro独立验收，不再交旧C05，不自签77通过、不自行启动78。Owner只需转发入口，技术资料经GitHub回传，不要求搬运ZIP或长报告。

禁止内核/loader/initramfs及Owner脚本修改、补丁、预处理/编译/链接、仓库程序/脚本/旧夹具、Graphviz、QEMU/实机、真实并发、额外模型/子代理/API付费、合并关闭/强推/删分支、本机凭据访问、外部libc/镜像、阶段3或跨项目维护。新建临时纯文本检查和不执行仓库代码的有限模型可按既有权限使用，须明确与真实内核运行分离。
