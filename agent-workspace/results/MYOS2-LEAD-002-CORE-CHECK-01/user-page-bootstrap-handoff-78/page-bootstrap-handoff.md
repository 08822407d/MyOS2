---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-PAGE-BOOTSTRAP-HANDOFF-78
record_type: bounded_page_bootstrap_and_free_handoff_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_display_name: MYOS2-A-C05c 代码分析执行
produced_by: GPT-6 (self-reported)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-12
base_snapshot: time
source_pin_short12: a039d9803ade
contract_pin_short12: ea5eb636ae89
read_channel: connector
D00: DOCUMENTATION_CORRECTION_SUBMITTED_PENDING_C06
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
review_receiver: MYOS2-A-C06
runtime_evidence: NOT_RUN
---

# 第78批｜页元数据分配、初始化与第一次空闲页移交

**执行边界与结论。** 本批完成了固定time源码中从启动内存区记录到`NODE_DATA`、`mem_map`、zone/page对象，再到“memory扣reserved→按页对齐→分块→请求buddy”的有界静态追踪。发现多处先行合法性要求并未由当前调用者保证，尤其是页描述符数组的绝对PFN索引、物理零失败哨兵的转换、区统计覆写与空闲页累计量；均只记录条件候选，不宣称编译/实际启动故障。第77批文档退回R77-01在本批D00仅**提交追加纠正，仍待C06独立裁定关闭**。

## D00｜R77-01原六件保留、在本包追加纠正

独立从固定`5a8c5dc1cd38`完整读取第77批六件（以及其正式评论6821135838），以GitHub连接器返回的UTF-8完整文本按**每行完全相同的字符子串**计数：计数字符是U+005B左方括号直接接ASCII单词`VERIFIED`，包括源码标签和说明文字，不按解析出的标签过滤。没有运行仓库脚本或完整本地P9。

| 第77批原文件 | 定义体源码标签 | 六件原始前缀命中 |
|---|---:|---:|
| evidence.md | 121 | 121 |
| MANIFEST.md | 0 | 1 |
| page-allocator-lifecycle.md | 0 | 0 |
| facts-and-boundaries.yaml | 0 | 0 |
| page-allocator-lifecycle-paths.dot | 0 | 0 |
| deferred-findings.yaml | 0 | 0 |
| **合计** | **121** | **122** |

唯一额外命中位于旧MANIFEST第111行“P9-1至P9-13实账”第5条，它是在解释如何数前缀，而非新增源码定义。故旧`MANIFEST.self_check.verified_claims=121`及P9-5“六件原始前缀总数121”的全包口径**不成立**：正确的是**原始命中122、其中带真实定义体标识的标签121、说明文字额外1**。这不是第122条源码证据，不能偷偷抬高`quotes_reconfirmed`到122，也不把历史121条源码引文说成不存在。

本包`facts.prior_return_disposition`分别记录旧MANIFEST、自检总括、原77正文/evidence导语、facts.methods与正式评论6821135838的**替代消费边界**。原77有121个定义标签仍可按主线审核过的范围使用；本次没有重新校核其121条源码本体或重做36个旧模型，也不改旧六件/旧评论。R77-01状态严格`DOCUMENTATION_CORRECTION_SUBMITTED_PENDING_C06`，不由执行者自关。

主线R77-S01补充应按两个不同层级理解：SLAB对象层后置hook获得原`gfpflags`，buddy页层hook获得`prepare_alloc_flags`过滤后的`alloc_gfp`，不能拿旧M20描述的对象层参数冒充页层原参数。R77-S02原图中的**超阶立即NULL退出**与**有效zone未命中继续下一个zone**不同；`expand`是“余块先入链、后标阶”，而free尾部是“先标阶、后入链”，不拼成统一CALL序列。R77-S03由C06提供的PFN16→24多级有界合并例仅为源级条件补充、不是77原36模型已经运行；folio字段位置及PA77-F01/F02的限定继续有效。本批没有重做77，三个补充和旧MC/GAP均保留原主线身份。

## G01/A｜节点表、mem_map容量与PFN索引

启动`setup_arch`后可见`zone_sizes_init`请求`myos_memblock_alloc_normal(MAX_NUMNODES*sizeof(pg_data_t),SMP_CACHE_BYTES)`，继而直接对`i<MAX_NUMNODES`赋值`NODE_DATA(i)=nodes+i`（A01–A05）。`NODE_DATA`实际上索引`node_data`数组；`MAX_NUMNODES=1<<NODES_SHIFT`且`NODES_SHIFT`是否为0受配置分支限制，`sizeof(pg_data_t)`未编译求值。**分配返回NULL没有在这个调用者中检查**，因此不能单凭循环写入就证明node数据可用。正常包装器若有有效虚拟地址会请求按size清零（A09/A40），但其有效性依赖内层物理空间/保留成功及实际直映射。

具体分配链为`memblock_alloc_internal→memblock_alloc_range→__memblock_find_range_bottom_up`，在free区间中以字节单位夹紧候选端点、按align取整、确认容纳size，只有`found!=0`且`simple_mmblk_reserve(found,size)`返回0时才得到非零物理地址（A10–A18）。`memblock_alloc_internal`在第一轮失败且min_addr非零时尝试**去掉最低地址约束**重试；它对物理0作失败检查，然后才调用`phys_to_virt`。对照`alloc_node_mem_map`：该函数却把`memblock_alloc_range`返回值直接送进`phys_to_virt`，**之后**才用`if(!map)`检测虚拟指针（A30–A34）。按这份x86-64`__va=PAGE_OFFSET+x`和非零高端PAGE_OFFSET的条件源模型，物理0可转成**非NULL高端地址**，此检查不足以认证分配成功（条件候选PB78-F02）；但真实映射、实际分配失败和CPU后果均未运行。

另有**PFN/物理字节单位差异**：架构`MAX_DMA_PFN=((16MiB)>>PAGE_SHIFT)`在`PAGE_SHIFT=12`模型等于4096个PFN。正常包装器将其`<<PAGE_SHIFT`转成16MiB字节下界；但`alloc_node_mem_map`传给`memblock_alloc_range`的是未移位的`MAX_DMA_PFN`，按其`phys_addr_t`实参应读作4096**字节**下界（A07/A09/A12/A41）。两个调用者的下界不同；是否实际放置在不期望的低区间尚不确定（PB78-F03）。

`alloc_node_mem_map`中：若`node_spanned_pages==0`立即返回；否则`start=floor(node_start_pfn,MAX_ORDER_NR_PAGES)`，`offset=node_start_pfn-start`，`end=ALIGN(node_start_pfn+node_spanned_pages,MAX_ORDER_NR_PAGES)`，请求`(end-start)*sizeof(page_s)`字节；若合法、可写则`memset(map,0,size)`再公布**`node_mem_map=mem_map=map+offset`**（A27–A35）。另一侧`pfn_to_page(pfn)=mem_map+pfn`，直接把**绝对PFN**加到该指针上，而`alloc_node_mem_map`在局部循环使用`node_mem_map[i]`。这两个原点需要同时正确：例如`node_start_pfn=1`、span16，局部0是`map[1]`而`pfn_to_page(1)`是`map[2]`；若起始PFN1024、span16、对齐块1024页，则实际分配1024个page描述符，而`pfn_to_page(1024)`形成`map+1024`的尾后一位置。**PB78-F01**记录该条件性索引原点/容量风险；它不认证正常启动使用上述非零PFN区间，也不认证任何实际越界内存访问。

本包`node_storage_matrix`、`memmap_index_matrix`和`address_unit_and_failure_matrix`逐项给出物理地址0、start0、start1、start1024/1025、空节点和正常/失败两类API的前提及首个停止点。物理字节、PFN数量、`page_s*`指针偏移与结构体字节长度相互独立，不能凭`pfn_to_page`、`page_to_pfn`形式互逆就证明对象真实指向已分配的数组。

## G02/B｜zone范围覆盖、空洞与页初态

`zone_sizes_init`对五区上界数组先清零，只明确填写DMA、DMA32和NORMAL三个PFN端点（A06/B01）；`free_area_init`按其上界产生各zone的lowest/highest表，清零pgdat并通过`get_pfn_range`取得node的PFN范围，先调用`calculate_node_totalpages`，再申请`mem_map`、调用`free_area_init_core`，最后`memmap_init`（A24/A25/B03/B04/B44）。直接启动层`setup_arch`包含`zone_sizes_init`调用，`start_kernel`在`setup_arch`之后调用`mm_core_init`（B02/B03/B40/B41）；只证明所选C源码顺序，不证明引导镜像已经走完。

`__myos_calculate_node_pages`先对实际node/zone边界做clamp，按`for_each_mem_pfn_range`扣除memory覆盖获得`absent`，然后写`spanned`（B05–B09）。这里的**present**是物理memory记录覆盖的页，不等于空闲页；保留区和memory洞不能直接合并统计。在`node_start==node_end==0`时该callee立即return，未给输出指针赋值；上游`calculate_node_totalpages`使用未预置的`spanned`、`absent`来计算`real_size`（B06/B10/B11），PB78-F04仅记录空节点条件下的未定义值依赖，不推定实际e820一定产出空节点。

对非空node，`calculate_node_totalpages`先向zone写`zone_start/spanned/present`，再总和到`node_spanned/node_present`并写`nr_zones=MAX_NR_ZONES`（B12/B13）。随后`free_area_init_core`使用这些字段，`zone_init_internals`中`spin_lock_init`是**注释**，不能把pgdat的`memset`等同于已验证锁初始化（B14–B17）。再之后`memmap_init`从PFN1开始，**重新赋值zone起点和跨度，且明确将zone.present_pages写为0**，随后初始化每个zone的`free_area[j]`头并为其范围内各PFN调用`__init_single_page`（B18–B20）。若之前确有present=12，这次覆写为0是一个有条件数据流问题PB78-F05；并非证明实际内核功能一定失效。

`__init_single_page`活动写入是`init_page_count`设`_refcount=1`、初始化lru和buddy_list，`page_mapcount_reset`仅是注释（B20/B21/B27）。`flags/flag_defs`、`page_type/_mapcount`各有union复用，`_refcount`单独存在（B23–B26）；`mem_map`之前的清零请求与这些局部写入是不同来源，不能直接宣布PageBuddy恒真或恒假，原GAP-M08及MR-02仍约束。对reserved字节区间，`reserve_bootmem_region`采用`PFN_DOWN(start)`到`PFN_UP(end)`，先以`pfn_valid(pfn)`判断（它所读宏只查`pfn<max_pfn`）、然后再次初始化page并设置PG_reserved（B28–B33）。对于内存洞或描述符越界，**上界检验不是RAM实在性证明**。另列`zone_initialization_matrix`、`page_metadata_initial_state`及`span_hole_and_reserved_matrix`，空区/洞/保留区都有独立停止条件。

## G03/C｜保留集合排除、chunk划分和交给buddy

`memblock.memory`和`memblock.reserved`为不同`memblock_type`，每区`base/size/flags`分别来自region（C01–C03）。`memmap_init_reserved_pages`按`for_each_memblock_type`遍历reserved，宏实写`idx<type->cnt+1`（C04/C05）。这个上界多访问一个槽：初始静态`cnt=0,max=256`时`regions[0]`在容量内；若实际`cnt==max`则最后的`regions[max]`已越界。是否能由有效的插入/合并路径达到满容量未被全量验证，不能只凭“cnt+1”认定它必然越界或必然有合法哨兵。之后`reserve_bootmem_region`标记相应页为Reserved，仍受真实mem_map、PFN和尾槽有效性门（C04–C07/C45）。

`for_each_free_mem_range`由`__for_each_mem_range`把memory与reserved传给`__simple_next_mem_range`，其`u64`游标低32位用于memory索引，高32位用于reserved gap索引；两内存区间分别是`[m.base,m.base+m.size)`与两个reserved之间的gap，输出`[max(m_start,gap_start),min(m_end,gap_end))`，按谁先结束推进索引；耗尽写`ULLONG_MAX`供宏终止（C12–C22）。`should_skip_region`实际只对memory套HOTPLUG/NOMAP/DRIVER_MANAGED等flag规则；名字“free”不意味着所有非本调用者影响均已审计。内部reserved扫描`idx_b<cnt+1`，在尾部先形成`&regions[cnt]`，但`idx_b==cnt`时`r_gap_end`取`PHYS_ADDR_MAX`而不是无条件读取该尾槽的`base`。**地址形成、是否解引用与容量合法性必须分开**。在排序不重叠、游标和容量均合法条件下，本包给出无保留、保留开头/中间/结尾、两个memory段及全部保留的半开区间模型；不认证真实固件布局或遍历器一定在所有错误输入上终止。

`mem_init`先写`after_bootmem=1`然后调用`memblock_free_all`，`mm_core_init`在它返回后才调用`kmem_cache_init`（C36/C37）。`memblock_free_all`先调用reserved标记器，再迭代free内存区间并做`start_pfn=PFN_UP(start)`、`end_pfn=min(PFN_DOWN(end),max_low_pfn)`，空区直接continue（C24/C25/C30/C31）。但其局部`ulong pages;`**没有初始化**，而在任何有效非空区间内，首先执行`pages += end_pfn-start_pfn`（C26），读取了没有已知值的自动对象；不能因这个累计量后续貌似没被使用，就把后面的chunk循环认证为正常可达。PB78-F06记录该条件下的最早断点，下面chunk表只作为**把该阻断隔离后的数值假设**，绝不表示真实源码已成功交出全部页块。

隔离模型里，正的`start_pfn`用`order=min(MAX_ORDER,__ffs(start_pfn))`得到按2幂对齐的最高可能阶，再按`start+(1<<order)<=end`缩阶，逐块请求`memblock_free_pages(pfn_to_page(start),start,order)`并推进start（C27–C29/C32）。例如PFN半开区间[1,9)在各前提下拆成PFN1阶0、PFN2阶1、PFN4阶2、PFN8阶0，合计8页；[16,24)可取PFN16阶3共8页。若`start_pfn=0`，`__ffs(0)`无合法输入保证，必须在此再设一个独立停止点；不可用想象的返回order冒充真实分块。

若有效block和zone确实已准备并正常调到`memblock_free_pages`，它逐页请求清PG_reserved、将refcount设为0，**先**请求`managed_pages += 1<<order`，**然后**调用`__free_pages_ok`（C33–C35/C47–C49）。统计先加不是后续挂链已经成功，物理页可复用、PAGE_TYPE其他writer、buddy合并和引用所有者问题继续由旧PA77-F01/F02、MC-01/05、SL76-U09和AE75-U07限制；也不能把`after_bootmem=1`误称整个移交完成。

## 模型、延期与验收

新`facts-and-boundaries.yaml`保存26份固定time核心/最近依赖台账，其中超过合同24份软上限的2份来自证明虚实地址/bit-scan最近定义所需，不扩为全树审查；保存十类必需状态矩阵、32个纯算术/布尔有限模型，区分正常前提、非法输入和最早停止。`page-bootstrap-handoff-paths.dot`有68节点76类型边，明确`ANALYSIS_STOP`与其后的`HYPOTHETICAL_SEQUENCE`不代表实际执行；不以图节点当真实内核callee。新增六项条件研究候选PB78-F01–F06、八项证据缺口PB78-U01–U08；没有编译运行确认的bug，旧qualified_id一个也不改或自关。

本次新`evidence.md`的A43/B44/C49共136段均为固定time单一定义内的1–5个非空连续原始物理行，分别独立重新读回其源码并比对136/136，0降级。本批新六件的原始保留前缀仅出现在新证据标签136处，其他五件用字符名称描述计数，以免重复旧77的说明文字错误。完整本地P9、预处理、编译、链接、内核/旧夹具、脚本、Graphviz、QEMU、物理内存和并发运行全部NOT_RUN。

**交付状态：** D00=`DOCUMENTATION_CORRECTION_SUBMITTED_PENDING_C06`；A/B/C=`completed_in_scope`；`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`。原R77-01是否闭合、78是否PASS由MYOS2-A-C06在Pro会话独立裁定；执行09不自签77/78、不自动启动79、不修改内核或旧结果、旧评论、PR元数据。
