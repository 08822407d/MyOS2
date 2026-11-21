接收及执行模型：次一档（沿用GPT-6＋Extra）。继续现有MYOS2-A-C06a 代码分析执行／CHAT-EXEC-10；不新开对话。独立验收由C06使用Pro完成。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-EARLY-PGTABLE-STORAGE-81
record_type: bounded_early_pgtable_buffer_mapping_and_allocation_contract
evidence_class: 主线设计与限定执行授权；非执行成果或运行验证
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（Owner本轮交回指定）
date: 2026-11-13
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 0e9e9e89c4be
lead_before_publication_short12: f4a3b437cb44
fixed_contract_commit: OWNER_FORWARDED_116_FIXED_URL
inputs_read: [115合同与80六件, 79回执及80审查限定, 固定早期页表源码预读, 00/18/24/28/63/79/110]
receiving_model_tier: next_tier_default
executor_display_name: MYOS2-A-C06a 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
executor_rotation: false
old_C05c_EXEC09: STOP_WRITING_OWNER_RETIRED_CONTEXT
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-pgtable-storage-81/
allowed_files: [MANIFEST.md, early-pgtable-storage.md, facts-and-boundaries.yaml, early-pgtable-storage-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: []
D00: NOT_REQUIRED
R79_01_02: CLOSED_DOCUMENTATION_ONLY
batch79_composite: PASS_PENDING_LOCAL
batch80_acceptance: PASS_PENDING_LOCAL_WITH_MANDATORY_R80_S01_S02_S03
activation_gate: Owner向现C06a转发本固定合同并要求执行且G00通过
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第81批：早期页表缓冲区、提前映射与分配来源切换

本批没有D00返工。先G00，通过后连续G01–G04，不逐专题等待批准。79两项文档已闭合，80按R80-S01/S02/S03接收；合读这些限定，不重做79/80，不恢复旧09或重演接收仪式。

## 1. 技术增量与范围

80把启动内存表、物理页号上界和memblock登记分开，本批继续核这些信息被早期页表存储使用的三个接合面：A以brk取得缓冲区并划分两个页表页；B把该缓冲区接入direct-map页表并发布根项；C按阶段和剩余容量在缓冲区、memblock和页分配器之间选择存储来源。三个专题共用固定init.c、地址/页表定义及一套六件，不以引文或文档数量充当工作量。

只做阶段2有界静态取证。核心是实际对象、容量/索引、地址域、写入先后、失败门和返回条件。既有MR79/PB78/PA77/BM80条件与缺口保持原qualified_id；读到新上游或新消费者不自动闭合旧项。约束来自00、18/24/28/63/79及110，不是新增内核操作授权。

不扩成完整物理映射算法、所有页表级递归、用户缺页/VMA、完整MMU/TLB硬件证明、引导汇编、loader/固件、AP启动、所有映射调用者、设备MMIO、锁原子/调度器或全树别名审计。不修内核，不出补丁，不预处理/编译/运行；注释声称“已经直接映射”不能代替实际表项/地址前提。

## 2. G00：固定材料、当前身份和现场

L=agent-workspace/lead/MYOS2-LEAD-002/，取本116启动URL同一真实固定发布提交；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/，本次最新执行输入取0e9e9e89c4be；源码仅time@a039d9803ade。短SHA自真实连接器输出复制，产物不写完整40位串。

全文读取本116、L/110-owner-communication-and-delivery-continuity.md、L/reviews/CORE-USER-BOOT-MEMORY-MAP-80-review.md、L/reviews/CORE-USER-MEMBLOCK-REGION-LIFECYCLE-79-DOC-CLOSURE-review.md、L/checkpoints/2026-11-13-bootmap80-reviewed-early-pgtable81-ready.md及80正式评论6841097460。合读R80-S01/S02/S03；本有效C06a自己已完整读到且固定对象一致的80六件可按身份明确复用，不能只引用交付摘要，更不能继承C06已读声明。

重新全文读取63号、79号、协议v2的全部P1–P13和110。九份项目规则、113接收约定、已适用Mnemosyne指导可按同一有效C06a自己的读取范围复用；缺项补读，不把旧临时记录不可取得说成当前无需守规则，也不再次伪造历史接收报告。发生真实上下文不足时先保全、报告受影响范围，不能擅自沿用09身份。

涉及页分配器/清零/初次移交时，必须合读L/reviews/CORE-USER-PAGE-ALLOCATOR-LIFECYCLE-77-review.md的R77-S01/S02/S03、77-DOC-CLOSURE、78-review的R78-S01/S02/S03及78-DOC-CLOSURE。AE75-U07、SL76-U09与MM-04的MC-01/02/03/04/05、LB-01/LB-08/GAP-M08及05/MR-02历史限定按实际依赖定位读取；已读同对象可复用，不全量重开历史批次。73/74、58/63已闭合或撤回不重开。

直接核PR16/17、lead/exec/master/time、开放PR、81新目录和本人本批正式marker。exec必须0e9e9e89c4be、time a039d9803ade、master直接头预期e8e60cd19540；lead取116真实发布头，不用父f4a3b437cb44代替。PR缓存base_sha不作master现状。已有目标或marker先核是否本批已经交付，未知推进先查归属、不覆盖。

实际回读公约§1(2)整句和time/master两条金丝雀，保存逐字原文、通道和来源，不只记“不同”。G00全部通过才为本81解除STOP_WRITING；无需Owner重复批准或旧C05c确认。仅停真实访问/授权/未知并行冲突影响的动作，普通源码问题入库不转给Owner逐项决定。

## 3. G01/A：brk预留、缓冲区尺寸和地址身份

核心固定源：mykernel/arch/x86_64/mm/init.c中的EARLY_PGTBUF_CHUNK_SIZE/MAX_CHUNKS/SIZE、RESERVE_BRK调用、pgt_buf_start/end/top、early_pgtbuf_pud/pmd、early_alloc_pgt_buf；asm/setup.h的RESERVE_BRK；kernel/setup.c的extend_brk/reserve_brk及直接调用顺序；kernel.lds的brk相关符号和最近sections声明。沿实际包含关系补PAGE/PMD大小、__pa/__START_KERNEL_map、真实类型与section定义。

区分链接时预留长度、运行时extend_brk申请长度和对齐余量。注释或宏调用不证明链接节已放入目标映像；没有实际链接输出就保留符号地址。建立tables字节、base物理地址、start/end/top页号及两个虚拟别名的单位表，不把kernel-image alias与direct-map alias混成同一个地址域。

按真实先后跟踪extend_brk返回、__pa转换、pgt_buf_start发布、pud/pmd指针形成、end跳过前两页和top计算。声明2MiB/4KiB等选中常量后可做8MiB、2048页和首两页的有限算术例；这不是实际分配成功或可写证明。size/align、地址相加、移位、有效extent和callee返回分别作为门，先行非法后的状态只作隔离假设。

列合法基址、对齐余量、首两页占用、剩余容量、物理/虚拟转换前提和失败时已写状态；不重复80整个brk审查。交buffer_capacity_matrix、address_domain_matrix、buffer_publication_order_matrix。强制锚覆盖尺寸宏本体、RESERVE_BRK真实定义、分配实参和所有缓冲区状态写入；extern声明只能当接口来源，不能冒作函数实现。

## 4. G02/B：提前映射缓冲区的表项、边界与发布顺序

完整读init.c::myos_early_map_pgt_buf_into_directmap及必要早期根数组定义/直接消费；读mm/fault/pgtable_macro.h的pgd/pud/pmd索引和populate别名，再沿最近定义核实际callee、set_pmd、__pmd、pgprot_val和PAGE_KERNEL_LARGE相关位。只追本函数用到的层级与写入，不泛化为整个页表子系统已验证。

分别记录phys_start/end、va/vend、early_top_pgt与init_top_pgt根项身份、折叠p4d表达、独立pud/pmd页、清零和populate的先后。单张PUD页和单张PMD页能覆盖的区间由索引与层级定义推导；有关跨PGD/PUD的注释或被注释检查不当作活动拒绝门。

对通常对齐、邻近PUD边界、邻近PGD边界、空/反向/溢出区间分别建代表模型。nr_pmd与pa推进、pmd_index回卷、同槽重复写和单个父项能否指向预期子页分别检查；“缓冲区只有8MiB”本身不保证它不跨1GiB边界。任何地址/数组前提未证则停止该真实路径，不能依据模型宣告真实机器发生覆盖。

明确函数内调用__flush_tlb_all与复制*init_pgd=*early_pgd的真实先后；根项复制、刷新请求与当前活动CR3不同概念。可定点读init_mem_mapping的load_cr3/刷新调用和头部C根项来源作为消费边界，不深入引导汇编、全部flush实现/跨CPU协议或外部硬件规范。拿不到必要当前根/映射证据就保留缺口，不从名字“flush_all”保证全局生效。

交table_identity_matrix、slot_and_boundary_matrix、mapping_write_sequence_matrix、root_publication_and_flush_matrix。强制锚覆盖地址/索引生产、实际清零/populate/set、循环边界和最后刷新/复制；CALL、CALLER_SEQUENCE、DATA分清，不用共享图节点拼接不存在的调用链。

## 5. G03/C：alloc_low_pages的三种来源、数量和清零

完整读init.c::alloc_low_pages与其实际使用的alloc_low_page包装；最近get_order、__get_free_pages返回、memblock_phys_alloc_range、clear_page及GFP定义按真实别名追到必要层级。after_bootmem生产与min_pfn_mapped/max_pfn_mapped来源在已定位文件和直接依赖内定点核查；未穷尽写者不得说全局不存在赋值。init_memory_mapping中被注释的登记调用不是活动生产者。

按after_bootmem真假、缓冲区end+num与top关系、mapped范围是否有效、memblock返回0/非0分开。缓冲区分支先推进end再逐页清零；回退分支在地址有效范围内请求物理字节并判0；晚期分支先算order再直接返回页分配器结果。不能用早期for循环的清零步骤证明晚期分支也执行了同一代码，更不能用GFP_ZERO名字覆盖77的页层hook限定。

代表条件含num=0、1、非2幂、恰好剩余容量、超出一页、end+num溢出、mapped上下界相等/反向、零返回、过大的num和int i循环宽度。num页请求、页阶向上取整后的实际容量、已清零页数与返回地址含义分别记账；移位/乘法发生的类型及顺序必须取证。未证参数有效时先停，不以“内核应只传1”填补调用者义务。

追踪早期失败panic/循环与晚期NULL/零返回的不同出口；语句存在不等于观察到机器停机。对新近callee只能消费已审限定，不能重做buddy/slab全链或把phys非0解释为页表立即可访问。列部分状态写入和无回滚边界，最终成功还依赖存储可写、转换及初始化时序。

交allocation_source_matrix、num_order_extent_matrix、zeroing_and_failure_matrix、mapped_window_provenance_matrix及跨A/B/C接合表。强制锚覆盖阶段门、容量门、回退实参/判0、PFN及游标写入、清零循环、晚期order/flags/返回；所有未知按PT81-Fxx候选或PT81-Uxx缺口登记，旧qualified_id不重复拆根因。

## 6. 六件与发布前完整性门

严格六件见头部；facts包含source_ledger、各矩阵、finite_model_cases、prior_findings_links和R80-S01/S02/S03消费入口。没有D00，不再生成旧80整套矩阵或重画旧图。DOT只表达本批关系，隔离模型用HYPOTHETICAL_SEQUENCE，分析停止不画成实际内核分支。

核心与最近源码软上限24份（金丝雀另计）、8组定点检索、4次相关目录浏览；必要超限说明具体缺失依赖，不全仓漫游。新短锚软上限130、源码条件模型最多36、新候选/缺口各最多12；上限不是配额。重复锚/矩阵/模型不能相加夸大独立工作量，规则加载不算技术成果。

本任务适用agent-workspace/tasks/00-gpt-task-protocol-v2.md全部十三条。MANIFEST须给出原字段task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check；模型按实际可见或Owner来源记录，后台未知不认证。

发布前逐项检查字段存在、类型和来源。公约整句与time/master金丝雀保存逐字原文；self_check含整数verified_claims、quotes_reconfirmed、downgraded_to_inferred，与本六件原始全文前缀总数相符。每个短锚真实单一定义内连续1–5行并支持论点，宏前置条件不混入宏本体，尾后一指针形成与访问分账。已知不合格项在本批发布前纠正，不用“技术主干可用”跳过硬门。

P9-1全文40hex零命中；2/13定义内逐字回源和强制锚全覆盖；3路径及写后固定/活动回读；4行号仅辅助且不越文件；5原始前缀计数含说明文字，三数自洽；6外部事实须真实出处，当前不以外部惯例补源码；7/8金丝雀/公约/read_channel实际存在；9/10/12无代码/补丁/降级则N/A；11所有未运行项如实NOT_RUN。完整本地P9与构建/运行仍未执行，执行者不签独立PASS。

## 7. G04：原执行分支安全发布与停写

仅在claude/dazzling-cori-q0dnyt/PR17新增81目录六件及本人一条正式顶层评论。唯一marker：MYOS2-LEAD-002-CHAT-EXEC-10 / CORE-USER-EARLY-PGTABLE-STORAGE-81 / FORMAL_DELIVERY。旧成果/旧评论、PR元数据/Draft、lead/PR16、master/time冻结；主线不代写执行results。

写前重新核exec=0e9e9e89c4be及目标/marker，使用完整旧HEAD为expected_sha、force=false和单父commit。GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次，遵Retry-After。复用成功blob/tree/commit/comment，只补缺项；未知并行推进不得覆盖。记录实际请求/重试和远端效果，工具发现次数不作写请求。

compare起点至新头必须严格六件added、旧件零改删。新固定提交及活动执行分支六件全文/blob、四direct refs、PR16/17、正式评论全文与唯一性全部回读。真实SHA/评论号取得后才报告完成；发布文件不等于主线验收，不能新增第七件记录。

完成后仅报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，随后STOP_WRITING，通过PR17交回现MYOS2-A-C06 Pro独立验收。不自签81、不启动82、不回旧C05/C05c。Owner只需转发本固定启动词，不搬运ZIP、不逐项审查源码问题。

禁止内核/loader/initramfs/Owner脚本修改、补丁、预处理/编译/链接、仓库程序或旧夹具执行、Graphviz、QEMU/实机、真实并发与硬件验证、外部libc/镜像、本机凭据、额外模型/子代理/API付费、新分支PR、合并关闭强推删除、阶段3及跨项目维护。只可按既有许可做新建临时纯文本检查和不执行仓库代码的有限模型，明确非内核运行。110号以后每次主线交接仍须显式列为固定必读。
