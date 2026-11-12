---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-ALLOCATOR-ENTRY-75
record_type: bounded_allocator_entry_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_display_name: MYOS2-A-C05c 代码分析执行
produced_by: GPT-6 (self-reported; backend not independently certified)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-12
base_snapshot: time
source_pin_short12: a039d9803ade
contract_pin_short12: 1ba6acf132a0
read_channel: connector
D00: documentation_disposition_submitted
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第75批｜内核分配器入口、清零请求及释放来源分流

本批只读固定`time@a039d9803ade`，以第74批task结构`kzalloc(sizeof(task_s),GFP_KERNEL)`、新线程栈`kmalloc(THREAD_SIZE,GFP_KERNEL)`及`kfree`两个请求为接合入口。**主要结论：** 正常`kmalloc`在所读宏模型中以4096字节为小缓存/大块分界；新任务栈32768的请求进入`kmalloc_large`，task结构的`sizeof(task_s)`未实际测量，不能代入臆测大小。小块ZERO在`slab_post_alloc_hook`按`cache.object_size`请求清零，大块ZERO在`kmalloc_large`显式按原`size`请求清零；`kfree`先对NULL返回，再依据folio上的slab标志选择两种释放接口，**这个标志不是分配来源或合法指针的证明**。

方法是固定源码逐字短锚、限定分支模型及原始Git对象回读。所有算例均需非空/合法对象、可表示整数、下层callee正常返回等局部前提，遇未初始化对象、非法指针或不返回循环处停止推演。无内核/仓库脚本运行、预处理、编译、链接、QEMU/实机、旧夹具或物理释放观测；完整本地P9、Graphviz均`NOT_RUN`。

## D00｜R74-01追加文档纠正

C05对74唯一文档退回为`R74-01`：74旧证据`B12`标签声称`page_const_arch.h::THREAD_SIZE`，但实际引文属于另一个宏`THREAD_SIZE_ORDER`。本批在独立新证据给出**五个分别单定义的宏锚**，不再把相邻宏拼成一个定义：

| 原74消费 | 本批新锚及实际定义 | 数值角色 |
|---|---|---|
| B12旧引文本身 | D01：`THREAD_SIZE_ORDER` | 3 |
| B12旧标签要求的定义 | D02：`THREAD_SIZE` | `PAGE_SIZE << THREAD_SIZE_ORDER` |
| 隐含页大小宏 | D03：`PAGE_SIZE` | `_AC(1,UL) << PAGE_SHIFT` |
| 隐含页偏移宏 | D04：`PAGE_SHIFT` | 12 |
| C语言整数字面量的生成 | D05：`_AC` C分支 | `X##Y`，令`_AC(1,UL)`投影为`1UL` |

在**仅按所读C源宏分支、unsigned long可表示、无宏重定义**的有限表达式中，`PAGE_SIZE=(1UL<<12)=4096`，`THREAD_SIZE=(4096UL<<3)=32768`。没有运行预处理器或对链接后真实栈布局作承诺。74正文G02段、facts中`stack_lifetime_matrix[storage_size]`与`TSL74-M14`、DOT的`K48`均按D01–D05**替代消费**；新`facts.prior_return_disposition`逐项记录这些旧→新映射，旧事实文件、图及其节点本身不修改。

74旧全部74个引文标签仍然是**历史标签计数**，其中旧B12的定义身份不符合协议；其余73个标签没有在本批重新做完整机械定义复核，不能以“74/74均正确”继续消费。这个限定同时传播到旧74的MANIFEST、正文、facts、evidence导语及正式评论6812545638的总括说法；新75的P6数值只数新六件中87个引文前缀（D5/A31/B26/C25），既不改旧评论也不把新增锚回填为旧文件本身正确。D00仅为`documentation_disposition_submitted`，R74-01由C05独立验收、未由本执行者关闭。

R74-S01限制`dup_task_struct`整体复制后`err=0`及其紧邻恒零分支，保留真实栈申请失败来源；S02要求`TASK_DEAD`**完整等值**、对象有效、`kfree(NULL)`精确前提，S03要求区分静态`init_task`与一般`orig`以及条件性切换恢复。本批仅接合分配器入口，不重做task/pid/exit。R73-01已经由C05独立裁定`CLOSED_DOCUMENTATION_ONLY`、第73批组合`PASS_PENDING_LOCAL`，R73C-S01及R73-S01/S02/S03继续有效，**绝不重新开启R73-01**。

## G01 / A｜尺寸、索引与缓存分派

`slub.h::kmalloc(size,flags)`实际判断`size > KMALLOC_MAX_CACHE_SIZE`，真分支调用`kmalloc_large`，否则调用`__kmalloc`（A01）。`slub_const.h::KMALLOC_MAX_CACHE_SIZE`的**活动定义是PAGE_SIZE**（A15），所以本批条件模型分界为4096。虽然`KMALLOC_SHIFT_HIGH=(PAGE_SHIFT+1)=13`，`kmalloc_info`数组确实有`8192`的第13号槽，初始化循环也尝试创建索引1至13的缓存（A10–A17），但**正常`kmalloc(8192)`不会因为表中有8K槽而绕过实际分派条件去走该cache**。源码注释提及order-1同样不能覆盖`MAX_CACHE_SIZE=PAGE_SIZE`的活动宏。

在小块支路，`__kmalloc`先有`while(unlikely(size>MAX_CACHE_SIZE));`（A02）。这个循环只对**直接错误调用`__kmalloc`且尺寸超过上限**生效；普通`kmalloc`已经把这种尺寸导向`kmalloc_large`。已有`CORE-MM-BASELINE-04::MC-09`为直接调用路径记录风险，本次不重新编号。对于合法不超过4096的参数，`__kmalloc`从NULL初值出发、调用`kmalloc_slab`，仅缓存指针非NULL才继续`kmem_cache_alloc`，否则仍返回NULL（A03/A28）。没有把“cache创建函数存在”偷换成“这个使用时点必然已初始化”。

`kmalloc_slab`首先对`size==0`以及直接越界尺寸返回NULL（A04）。1–192字节经过`size_to_index`真实`switch((bytes-1)/8)`（A05/A08/A09/A29）；193–4096字节则用`fls(size-1)`求索引（A20），所读`fls`近期定义含x86 BSR与`return r+1`（A18/A19）。直接调用`size_to_index`超过0..192时另有循环停止（A07），但合法`kmalloc_slab`只在参数满足1..192时调用它。小缓存尺寸边界的28个例子详列`facts.size_class_boundary_matrix`，包括1、8/9、16/17、32/33、64/65、96/97、128/129、192/193、4096/4097、8192、32768，不让跨类型转换、cache缺失或未回返callee自动升级为成功分配事实。

当前可见类型表包括8、16、32、64、96、128、192、256、512、1024、2048、4096等档；若`size=65`，所读索引算法返回1，表中的`object_size`请求为96；若`size=193`，经fls得到index8，表里的cache class为256（前提是相应cache存在且初始化正常）。高位缓存创建函数`create_kmalloc_caches`按`kmalloc_info[idx]`调用create helper，循环后写`slab_state=UP`（A13/A14/A31）；`slub.c::kmem_cache_init`有调用`create_kmalloc_caches(0)`（A17），这只证明已读函数调用与赋值顺序，不证明目标内核已启动到这里、所有缓存确实可用或实际构建采用指定头文件分支。DEBUG、`SLUB_DEFINATION`及`PREFIX`的预处理/链接形态未执行。

大块支路`kmalloc_large`请求`alloc_pages(flags|__GFP_COMP,get_order(size))`（A23），`get_order`所读非恒量分支与`fls64`来源分别有A21/A22，整数模型中4097对应order1及8192容量，32768对应order3及32768容量；这不是物理页确实返回的观察。老`04`的page allocator、buddy合并及所有缓存自举内部均不重做。Task结构的申请实参是`sizeof(task_s)`（A26），没有测真实sizeof，故**其最终落小块还是大块为未知**；新栈申请实参是`THREAD_SIZE`（A27），在D00的有界C源宏模型下才明确为32768并选择大块页入口。

## G02 / B｜清零请求的范围、标志与失败

`slub.h::kzalloc(size,flags)`直接转为`kmalloc(size,flags|__GFP_ZERO)`（B01）；`__GFP_ZERO`在所读`buddy_const.h`通过typed宏包裹原始位`___GFP_ZERO=0x100u`（B02/B03）；`GFP_KERNEL`来自其独立组合宏（B04）。因此task的`kzalloc(sizeof(task_s),GFP_KERNEL)`确实**要求**ZERO，但不能仅凭名字证明分配成功或所有对象字段完成合法构造。栈的`kmalloc(THREAD_SIZE,GFP_KERNEL)`（B20）**没有由该调用处显式OR清零位**，不能断言其返回字节必为非零、也不能把全零字节自动解释为有效同步/引用计数对象。

对于小块：`kmem_cache_alloc`转`slab_alloc`（B18），先尝试partial链取对象；若无对象则`alloc_init_new_slab`接`alloc_slab`，再按有效缓存字段请求`alloc_pages`（B05–B07/B21）。任何一层返回NULL，`slab_alloc`可直接返回NULL，**没有到达`slab_post_alloc_hook`**（B06/B23）；这不是一定返回`-ENOMEM`。若得到有效非NULL对象并越过前面callee，真实代码先调用`slab_post_alloc_hook(s,gfpflags,&object)`才返回指针（B09/B25）；该hook设`zero_size=s->object_size`（B10/B12），仅当原`gfpflags & __GFP_ZERO`时请求`memset(*obj_p,0,zero_size)`（B11）。所以65字节的cache示例在有效96字节对象上请求清96字节、193字节例请求清256字节，**不能把原申请长度错写成hook的memset长度**。必须独立证明cache对象长度、指针有效且可写，不能据源码名推定对非法对象也安全。`prepare_alloc_flags`在slab新页路径中过滤标志（B08），但hook实参仍是原始`gfpflags`，这两套标志消费不能合并。

对于大块：`kmalloc_large`初值`ptr=NULL`，只有`alloc_pages`返回非空page才请求`page_to_virt`和`if(flags&__GFP_ZERO) memset(ptr,0,size)`（B15/B16/C18）。此处明确的**本地memset长度是原始size**，而不是`get_order`舍入出来的整块页容量。以4097字节、order1、容量8192模型为例，`kmalloc_large`本体显式请求清4097字节；额外尾部是不是也被底层页分配器处理，**本包未审核，不能下结论**。页分配返回NULL则`ptr`保持NULL、跳过`page_to_virt`和本地清零，最后返回NULL，不由普通`kmalloc`本身构造稳定的错误码。

分配器的NULL、直接`__kmalloc`非法尺寸忙等、下层异常、非法object以及可能不返回的callee均逐一列在`facts.earliest_stop_matrix`，不会越过这些前提继续断言“之后确实初始化成功”。

## G03 / C｜kfree来源门、两条释放路线与对象身份

`slub.h::kfree(object)`在`object==NULL`时直接返回（C01）。这一点使**仍有效的task，前次put成功将stack字段设成NULL、第二次仍满足完整`TASK_DEAD`相等门且没有其他写入**的狭窄模型可以接上早退（R74-S02/旧TSL74-M23）。但对**已经被释放的task**，调用者会先在`READ_ONCE(tsk->__state)`的成员访问处停止，不能直接跳过到`kfree(NULL)`来证明重复put安全。更不能推断对任意非NULL、静态`init_stack`或悬空指针都存在同样保护。

非NULL时，`kfree`先调用`virt_to_folio(object)`，它由`virt_to_page(x)`取得page再`page_folio(page)`（C02），之后以`folio_test_slab(folio)`检查folio标志（C03/C04）；若该标志真，取`folio_slab(folio)->slab_cache`调用`kmem_cache_free(cache,object)`；否则调用`free_large_kmalloc(folio,object)`。这里的folio/PG_slab是**分类元数据**，不保证传入地址确实为本分配器最近返回的对象起始地址、对象仍存活、folio/cache/order没有失效，也不保证存在最后引用。对无关地址、无效folioptr、内部偏移指针，按最早无法建立的page/folio或对象前提停止，而不是宣称一定能产生正常return、warning或崩溃。

有充分合法前提的slab支路，`folio_slab`是最近宏中的转换（C06），`kmem_cache_free(s,x)`**没有活动`cache_from_obj`检查/NULL早退**，而是调用`slab_free(s,virt_to_slab(x),x)`（C10/C24）；随后`slab_free`读取`s->node`并请求`free_to_partial_list`，还可能依自己的`inuse==0 || partial.count>=s.min_partial`执行`offline_exceeded_empty_slab`和`free_slab`，进而请求`__free_pages`（C11–C14/C22/C25）。已有`CORE-MM-BASELINE-04::MC-07`是这一释放条件的历史静态疑点，本75只把接口接到它，**不**重新声称其必然产生物理释放或把它当成一个新问题。直接`kmem_cache_free(NULL或者错误cache)`不能借用`kfree(NULL)`的保护：它按所读本体先向`slab_free`交接、在那里读取cache成员，来源不符先停止。

有充分合法前提的大块支路，`free_large_kmalloc`先求`folio_order(folio)`，其最近定义对非large返回0，对large读取`_flags_1`低8位（C07/C09/C21）。若`order==0`，当前活动代码`WARN_ON_ONCE`后调用`pr_warn_once`，**没有**在这条条件之后直接`return`；在告警callee正常返回的前提下，后续仍请求`__free_pages(folio_page(folio,0),order)`（C08/C19）。不能按“WARN”字面自造异常早停，也不能反过来认证物理页已经交还、页头必为原始分配起点、buddy最终管理正确。

与74调用者的严格接口接合：`free_task_struct(tsk)`请求`kfree(tsk)`（C17），`release_task_stack(tsk)`先请求`kfree(tsk->stack)`再设`tsk->stack=NULL`（C16）；每次必须先满足74的task有效性、准确状态门及释放来源。赋NULL只在前面callee正常返回后是已证的源级顺序请求。新task的stack来源与静态`init_task.stack=init_stack`是不同类别，不能只凭同名字段都调`kfree`就认定可合法释放。更不能把本批两个callee的存在推导成正常`do_exit`已经连到task/stack真实回收，原UV-15、TSL74-U04与R74-S02/S03仍生效。

## 对照与延期

| 分层 | 本批活动源能说什么 | 本批不能说什么 |
|---|---|---|
| 分派 | size与4096比较，正常kmalloc分成cache/large两路 | 8192表槽必被普通kmalloc使用、task的sizeof具体数字 |
| 清零 | ZERO位交给下一层，小cache hook按object_size、大块本地按size请求memset | 失败也会清、整个舍入页都清、非ZERO分配字节必非零 |
| 释放 | kfree(NULL)返回；合法folio按PG_slab分派；两支可向__free_pages请求 | 对任意非NULL指针有效、已物理释放、无旧MC-07/并发/ABA风险 |
| task接合 | 结构与stack有各自分配和kfree请求，状态与字段清空有条件 | 正常退出自动接通最后引用释放或所有对象都来自kmalloc |

`facts-and-boundaries.yaml`完整列出14份固定time核心/最近依赖源、10项分派、28个尺寸类边界、10项清零失败、8项来源释放、6项调用者接合、14项最早停止以及39个纯整数/布尔/条件有限模型。`allocator-entry-paths.dot`为65个显式节点、77条有类型边、0悬空；`INFERENCE`和`ANALYSIS_STOP`不代表CPU CFG。新证据D5+A31+B26+C25合计**87段**非空、单一定义内连续1–5行短锚，第二次直接从固定time重新读同14份源码，逐段原样匹配87/87；没有对旧74的其他73个标签重新签整体P2。

本批只登记`AE75-U01–U08`八项证据缺口，**新独立根因候选0项、运行证实BUG 0项**。现有MC-07/08/09、DR-04/RF-02、TSL74-U04以及05的MR-01/02历史限定保持原qualified_id/已有裁定，不追加冗余bug，不要求Owner现在处理修复方案。

执行方的范围状态：**D00=`documentation_disposition_submitted`；A/B/C=`completed_in_scope`；`READY_FOR_MAINLINE_REVIEW`；`mainline_acceptance=PENDING`**。是否关闭R74-01、是否签第75批由MYOS2-A-C05独立评判。本批不改内核、旧结果、旧评论或PR元数据，不编译运行、不合并、不执行阶段3，也不自行启动76。
