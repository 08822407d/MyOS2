---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PAGE-ALLOCATOR-LIFECYCLE-77
record_type: bounded_page_allocator_lifecycle_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_display_name: MYOS2-A-C05c 代码分析执行
produced_by: GPT-6 (self-reported; backend not independently certified)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract_pin_short12: 89f84109b470
initial_execution_head_short12: 59733f24b6d3
D00: NOT_APPLICABLE
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
review_receiver: MYOS2-A-C06 内核分析主线续建
runtime_evidence: NOT_RUN
---

# 第77批｜页分配、清零和归还之间的真实源码状态

**总体结论。** 在冻结的`time@a039d9803ade`内，`alloc_pages`实际上转到`__myos_alloc_pages`，选区后从各阶`free_area`取块、必要时拆分，然后请求清零及复合页元数据处理；直接释放则通过`__free_pages_ok`申请锁内伙伴合并并重新挂链。这个“从源码能够接上”不等于编译后分支正确，也不证明引用计数、伙伴表、物理归还或并发安全。所选局部存在多处已登记的MC-01至MC-05风险，另新增两项有明确先决条件的研究候选PA77-F01/F02；**没有任何真实运行重现**。

本轮是MyOS2阶段2、有界静态只读取证。固定111合同、110号Owner协作续记、C05→C06确认、77-ready检查点和第76/75审查及74文档闭合均已完整接收；R76-S01/S02/S03、R75-S01/S02/S03和历史MR-02仍是结论前提。第76批独立验收`PASS_PENDING_LOCAL`，第77批无D00返工；不改变74至76的文件或旧评论。固定time核心及最近依赖源码共22份，其中121条新短锚分布于19份（A40/B38/C43），独立第二次固定GitHub对象回读均原文命中121/121；非本地全量P9。补充的36例为**纯整数/布尔/源码条件模型**，并非执行内核、仓库脚本或C函数。

## G01／A：选区、取块与拆分的对象状态

`mempolicy.c::alloc_pages`中Linux式NUMA policy分流在注释里，选定活动调用是`page=__myos_alloc_pages(gfp,order)`（A01）。其第一道门对`order>MAX_ORDER`返回NULL（A02）。按`mmzone_const.h`的未设置`CONFIG_FORCE_MAX_ZONEORDER`分支，`MAX_ORDER=10`，`NR_PAGE_ORDERS=11`（A09/A10），故有效模型允许order10进入后续流程、order11在该门结束；真正构建时的宏分支未经预处理认证，不能把10冒充已测ABI。

`__myos_alloc_pages`有固定的三个`prefered_zone_list`元素：NORMAL、DMA32、DMA，而`max_prefered_idx`取自`NODE_DATA(0)->nr_zones`（A03）。同固定源的`calculate_node_totalpages`写`nr_zones=MAX_NR_ZONES`，枚举中包含DMA、DMA32、NORMAL、MOVABLE、DEVICE共五区（A06–A08/A13），但该初始化语句不保证任意调用时live值一定为5。有效的`nr_zones=5`模型里，普通请求从数组索引0尝试NORMAL→DMA32→DMA；有`GFP_DMA32`则从1尝试DMA32→DMA；有`GFP_DMA`则从2尝试DMA，且与DMA32同设时DMA分支优先（A04/A05/A36/A37）。每个合法索引下先取`node_zones[prefered_zone_list[i]]`，调用`__rmqueue_smallest`；非NULL才执行`prep_new_page`并返回（A14/A15），失败则尝试下一项。当三个合法索引都失败，源循环的下一个`i=3`已超过**三元素数组**，因此在**对`prefered_zone_list[3]`求值**处停止推演，不把尾部`return NULL`当作已证的合法失败出口。这是既有`CORE-MM-BASELINE-04::MC-04`的进一步定点见证，并非新根因，也没有报告某种必然机器越界值或崩溃。

选定zone内，`__rmqueue_smallest`从请求阶逐阶递增至`current_order<NR_PAGE_ORDERS`（A17）。`get_page_from_free_area`实际上先调用`list_header_remove_head`，它在有效非空链上`list_del_init`并减少`List_hdr_s.count`，再由`__list_null_or_page`转成页对象（A20–A23）。随后`__rmqueue_smallest`再调用`del_page_from_free_list`；该函数又尝试`list_header_delete_node`，**它先检查contains，只有节点仍在目标链上时才第二次减少count**，然后无论该删除是否命中都请求清PageBuddy和`private`（A24–A26）。因此在**合法单线程、第一次移除成功且节点已脱链**的限定模型中，链表count只减一次，而不能凭两个“delete”字样凭空断言重复减量；也不能据此证明坏链表或并发情形安全。

若找到的块阶高于请求阶，`expand`逐阶递减high、将剩余另一半`page[size]`初始化为独立链节点，按对应阶挂入free_area、写private阶和Buddy标记（A27–A30）；最后对**本次交付的`1<<order`个page**逐一执行`atomic_inc(&_refcount)`并返回页头（A18/A31）。在合法zone、连续PFN、有效list与可表示移位模型下，举例：一个起始PFN64、阶3的8页块，向阶1的2页申请者交付PFN64–65；原阶3链表减1，新增阶2伙伴PFN68–71和阶1伙伴PFN66–67各一块；交付的两个`_refcount`分别请求增1。这里的`free_area.count`计算的是**块节点数量**，不是空闲物理页总数，也不是页引用数。分配段未见与释放段对称的`zone.lock`请求（释放见C），这是历史`MC-02`锁协议风险；实际并发与其它锁/中断前提没有认证。

## G02／B：清零覆盖、复合页准备及不同消费者

`__myos_alloc_pages`从链上取得有效page后才请求`prep_new_page(page,order,gfp)`，后者**先调用**`post_alloc_hook`，随后只在`order>0 && (gfp&__GFP_COMP)`条件成立时调用`prep_compound_page`（B08）。这并不表示`alloc_pages`已保证取到合法可访问页，也不表示所有高阶分配都是compound。

对`__GFP_ZERO`，`post_alloc_hook`真实循环调用`clear_page((void*)page_to_virt(page)+i)`，`i=0..(1<<order)-1`（B01）。最近定义`clear_page(p)`实际展开为`memset(p,0,PAGE_SIZE)`（B02），在这份x86-64源码的C模型中`PAGE_SHIFT=12`且`PAGE_SIZE=4096`（B05–B07）。**循环的地址是基址每次增加1个字节，而非每次增加4096字节**；在GNU C对`void*`字节增量的声明模型与所有存储合法、callee返回前提下，order0仅请求清区间[0,4096)，order1的两次请求覆盖[0,4096)及[1,4097)，合并区间[0,4097)，尚有4095字节不在**本hook请求范围**；order3的八次请求从偏移0至7开始，合并区间[0,4103)，相对于32768字节块有28665字节不在这组请求范围。**未被此hook覆盖不等于运行时字节必然非零**：原存储可能已经是零，此外还可能存在调用者的后续清零。这是旧`MC-03`的有界覆盖见证，不复制旧根因编号。

当有效高阶分配明确传`__GFP_COMP`且`post_alloc_hook`正常返回时，`prep_compound_page`先设置页头PG_head，循环把第1至第`2^order-1`页标记为尾页、把tail的`compound_head`写成头地址+1并清其`private`，再在`prep_compound_head`检查head标志并设置folio的`_flags_1`低8位order、`_folio_nr_pages`及映射/pin计数（B09–B16）。阶3模型将有7个tail，head中页数写8；order0或没有COMP时，此复合元数据路径不会由**这次调用**执行。`folio_order`取值依`folio_test_large`（实际指向`folio_test_head`）和`_flags_1`低8位（B17–B20）。这与buddy自由块的`page.private`阶号不是一个字段，不能把二者互相替代。

两个下游消费者的清零身份也不能混用：`slub::alloc_slab`先经`prepare_alloc_flags`过滤原GFP位，再拿`oo.order`调用`alloc_pages`；所读`GFP_RECLAIM_MASK/GFP_CONSTRAINT_MASK`没有ZERO位，`simple_calculate_sizes`所读`s.allocflags`生产者通常加COMP/DMA等，而非ZERO（B25–B31）。但这不认证任何未知后续cache writer。`kmalloc_large`则直接把`flags|__GFP_COMP`传给`alloc_pages`，在page有效且原flags带ZERO时又**单独按请求size**进行局部`memset(ptr,0,size)`（B32）。故“底层ZERO覆盖”与“large调用者再清原size”是两个条件相异的操作，不能推出所有Slab/Large调用必然同样处理整块容量。

## G03／C：直接释放、伙伴门与引用释放的分流

直接`__free_pages(page,order)`当前活动本体只调用`__free_pages_ok`，没有自身的引用数减量判断（C01）；`free_pages(addr,order)`仅对非零addr请求`virt_to_page`后转它（C02），非零地址不代表指针有效。进入`__free_pages_ok`后先求PFN和`myos_page_zone`，再调用`free_pages_prepare`（C03）；它在所读活动体中执行部分flag清理，返回类型虽是bool却没有活动返回语句，**调用者没有依据该返回值做条件分支**（C04/C05），不能假造“准备失败会安全返回”的门。随后向`zone.lock`请求加锁，在锁内调用`__myos_free_one_page`，再解锁（C06/C07）。`myos_page_zone`依据`zone_start_pfn/spanned_pages`遍历分区，若未找到覆盖PFN的zone则函数末尾缺return（C08/C09）；在这种不满足前提的对象上先停止，不能继续认证zone解引用或锁动作。

`__myos_free_one_page`按阶寻找伙伴，未找到即break；找到且校验通过才算`combined_pfn`、调整page/PFN、提升order，循环后调用`set_buddy_order`及`add_to_free_list`（C12–C14）。`__find_buddy_pfn`按`pfn^(1<<order)`计算伙伴（C15）；`find_buddy_page_pfn`却在有效性校验之前就计算`page+(__buddy_pfn-pfn)`（C16/C17）。`pfn`和`__buddy_pfn`都是unsigned long：如果初始PFN9、order0，伙伴是8，差值`8-9`在64位无符号模型中变为`2^64-1`，不能在标准C的数组指针前提下直接认证它就是前一页。后续`page_is_buddy`的PageBuddy/order/zone检查在**指针表达式之后**，不能保护前一个地址形成问题。这是新登记的`PA77-F01`，只属待复核的条件源码风险；不声称实际机器一定崩溃，也不把它和既有MC-01合并。

伙伴身份的另一个关键条件是`PageBuddy`实际使用`page_type`和`PAGE_TYPE_BASE|PG_buddy`掩码比较（C24/C27/C28）；`__SetPageBuddy`是清`PG_buddy`位，`__ClearPageBuddy`是置该位（C25/C26）。`page_type`与`_mapcount`在page结构中是**同一union存储**，但`_refcount`是不同成员（C29–C31）。从已读启动`alloc_node_mem_map`先对map整体memset零、`__init_single_page`仅有注释的`page_mapcount_reset`和实际init_page_count，再到free路径调用`__SetPageBuddy`的局部前提，page_type的高类型基值并未因清PG_buddy而产生，因此在**排除其它writer**的有限初值模型下`PageBuddy`判false（C32/C33）。反过来，对假设高位本来具有`PAGE_TYPE_BASE`的有效page_type，PageBuddy可判true。这是旧MR-02/GAP-M08要求的双向限定：没有全树writer认证，既不能宣称Buddy永远不合并，也不能宣称已发生合并。

当`page_is_buddy`的所有前提确实成立而进入合并分支时，所读`__myos_free_one_page`里**没有在合并前摘掉旧伙伴的活动动作**；依次计算合并块、最终在高阶重新入链。这是历史`MC-01`的局部条件缺口，不重复产生新候选，也不声称实际出现两个有效的同一页块或物理重复分配。`MC-02`则保持为分配与释放锁范围不同的源码风险，并不等于已经看到并发写。

本批新`PA77-F02`与旧MC-05的关系需要严格区分。所读`memblock_free_pages`在引导释放前设参与页引用数为0；`__rmqueue_smallest`取出一个有效页面时对其`_refcount`执行`atomic_inc`，但如果一个拥有正确来源的页由**直接`__free_pages`**归还，所读释放体不执行对应的引用减量（C36/A31/C01–C04）。在同一页被再次取得、没有其他持有者操作且所有callee合法返回的假设模型中，计数可表现为`0→1→1→2`。这是**条件性的引用计数漂移候选PA77-F02**，并没有证明真实调用者不在释放前调节计数，也没有观察实机重用。反之`put_page→folio_put`是在引用降为零时转到`__folio_put`，而后者在这个固定源体中只有注释，没有活动的`__free_pages`请求（C39–C41）；它对应旧`MC-05/LB-01`的选定接口断点，不能据此宣告所有内存最终必泄漏。直接释放、boot释放、Slab释放、大块释放、引用归零后处理，是不同前提的路径（C38/C42/C43）。

## 结构成果、延期和明确停止点

`facts-and-boundaries.yaml`保存所读22份核心/最近依赖源码台账，以及zone尝试、分裂迁移、ZERO区间、复合元数据、allocator消费者、释放链、伙伴合并、引用分流、锁边界与最早停止矩阵；其中36个纯条件/算术模型不能转写为真实内核运行结果。`page-allocator-lifecycle-paths.dot`有71个显式节点、79条分型边、无悬空端点。图中的`CALLER_SEQUENCE`、`HISTORICAL_LIMIT`及`ANALYSIS_STOP`不当作无条件callee调用或完整CPU CFG，Graphviz未运行。

`deferred-findings.yaml`新增PA77-F01、PA77-F02**两个条件候选**及PA77-U01–U08八项证据缺口；历史MC-01至MC-05、RF-01、GAP-M08、LB-01/LB-08、AE75-U07、SL76-U09与旧MR-02均保留原qualified_id及主线限定。未证明的对象有效性、区间/页表合法性、无符号算术落点、计数所有者、锁实效、真实物理归还和实际构建产物，都不得用通常Linux习惯或函数名补齐。Owner当前不必自行审PR或逐条决定修复。

**交付状态：** A/B/C均`completed_in_scope`，D00=`NOT_APPLICABLE`；当前最高只报`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`，待原主线新接收会话**MYOS2-A-C06 内核分析主线续建**对正式PR17回件独立验收。所有源码读取、图和模型都不涉及内核/旧目录修改、预处理编译、仓库脚本、Graphviz、QEMU/实机、真实并发或物理释放实测；本地完整P9为`NOT_RUN`。不自签77通过，不启动78，不进行阶段3。
