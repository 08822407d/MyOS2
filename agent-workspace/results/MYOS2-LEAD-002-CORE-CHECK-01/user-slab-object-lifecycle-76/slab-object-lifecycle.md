---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SLAB-OBJECT-LIFECYCLE-76
record_type: bounded_slab_object_slot_allocation_and_discard_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_display_name: MYOS2-A-C05c 代码分析执行
produced_by: GPT-6 (self-reported; backend not independently attested)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-12
source_pin_short12: a039d9803ade
contract_pin_short12: bfb6578ca8c2
base_snapshot: time
read_channel: connector
D00: NOT_APPLICABLE
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第76批｜SLAB对象槽、分配迁移与整slab下线的有界静态分析

**范围与结论。** 对固定`time@a039d9803ade`，本轮追踪的是`kmem_cache`（缓存布局、阈值）、`slab`（单页组和对象计数）、`object`（每个槽的内容和自由指针）、`List_hdr_s`（partial/full各自的节点数）以及node统计（`nr_slabs/total_objects`）五类不同对象。已有`CORE-MM-BASELINE-04::MC-07/MC-08`所描述的两个机制，从真实布局、状态生产者和有条件的单线程跨slab顺序得到更精确的静态证据；**不把它们重复登记为新根因**，不宣称已发生物理UAF或某个真实ctor消费者受损。

固定输入状态：C05已独立裁定`R74-01=CLOSED_DOCUMENTATION_ONLY`，第74批组合及第75批均`PASS_PENDING_LOCAL`；75的R75-S01/S02/S03与74的R74C-S01继续限定本批。D00=`NOT_APPLICABLE`，不改旧文件、旧comment、PR元数据，不重开先前73/74文档问题。全部证据是新`evidence.md`中A47/B33/C27=**107条**同一真实定义内连续1–5行；二次以固定time连接器回读逐字确认107/107。14份直接/最近依赖源码（其中12份被107条短锚引用）、40个纯整数/布尔状态模型，不是预处理器、编译器、CPU或内核程序执行。非法指针、未初始化字段、越界、类型转换不能证明或不返回的callee出现时，先记`ANALYSIS_STOP`。

## G01 / A｜cache布局、阶与对象槽初始化

`slub_types.h::kmem_cache`保存`object_size`（用户对象字节数）、`size`（按对齐后每个槽的步长）、`inuse`（对齐后对象占用的字节布局值）、`align`、`offset`（freelist指针存储偏移）、`oo`、`ctor`和`min_partial`等独立成员（A01–A04）；**cache.inuse绝不是slab当前已经分配对象个数**。后者是`slub_types.h::slab`内16位`inuse`字段，另有15位`objects`和1位`frozen`，以及`freelist`与`slab_cache`（A05–A07）。`kmem_cache_order_objects`的`obj_nr`和`order`都是16位字段（A08）；`MAX_OBJS_PER_PAGE=32767`为常量（A26），**不是**当前`order_objects`自动执行的数值截断检查。

实际尺寸生产者为`slab_common.c::create_cache/create_boot_cache`写`object_size/size/align/ctor`（A35/A36）；之后`simple_kmem_cache_open`经`calculate_sizes`宏别名调`simple_calculate_sizes`（A21/A44/A45）。该函数以原`object_size`开始，先`ALIGN(...,sizeof(void*))`并写`cache.inuse`，再`ALIGN(...,cache.align)`写`cache.size`（A16–A19/A37/A38）。`simple_calculate_order`以`slub_min_objects=4`、余数占比`fraction=64`、`get_order(4*size)`为候选阶起点；循环计算`slab_size=(uint)PAGE_SIZE<<order`和`slab_size%size`，余数不大于`slab_size/64`时停（A12–A15/A43）。`oo_make`用`order_objects=((uint)PAGE_SIZE<<order)/size`写`oo.order/obj_nr`；`simple_calculate_sizes`再返回`!!oo_objects`（A10/A11/A20/A46/A47）。必须保持非零size、可表示乘/移位、足够大阶空间和位域不溢出的前提；若失败则不能声称slab必定生成正常正对象数。

`simple_kmem_cache_open`在尺寸生成成功后，把`min_partial`设为`max(4,min(8,ilog2(cache.size)/2))`（A22/A24/A25/A39–A41），并调用`INIT_LIST_HEADER_S`初始化node的partial/full空链（A23/B26/B27）。选定假设`sizeof(void*)=8`、`s.align=16`且`object_size=96`的数值模型：最终`size=96`，`get_order(4*96)=0`、`4096%96=64<=64`，所以`order0`、`oo.obj_nr=42`、`min_partial=4`。若`size=4096`，则最小4槽所需order2、对象数4、`min_partial=6`。这些只是有效宏/对齐/类型模型，不是运行时所有cache的实际布局；若alignment不同，比如对象65字节而要求64字节对齐，`cache.inuse=72`而`cache.size=128`，两数字不能混写。

`alloc_init_new_slab`只有在`alloc_slab`有效返回后才写`slab.objects=oo_objects(s->oo)`、`slab.inuse=0`、`slab.frozen=0`、`slab_cache=s`，再取页首地址设`slab.freelist=start`（A30/A31）。真实循环从`p=start,idx=0`开始，直到`idx<slab.objects-1`：求`next=p+s->size`，**对next而不是对p调用**`setup_object(s,next)`，条件存在`s->ctor`时执行ctor，再调用`set_freepointer(s,p,next)`，最后对最终p写NULL（A27–A29/A32–A34）。因此在有效slot数n≥1条件下：slot0**不**经过这个循环的ctor；slot1到slot(n−1)才会收到ctor调用；各槽的自由链指针随后按`object+s->offset`存入，最后槽写NULL。`n=1`的循环完全不进，只有末槽NULL写；`n=4`的有效模型是`slot0→slot1→slot2→slot3→NULL`，有3次后继槽setup、4次freelist写。

这把已有`MC-08`细化为**先给下一槽做ctor，然后在该槽后来成为前一槽时写入其freepointer**。在经选定cache生产者取得`offset=0`且ctor确实写过槽起始指针宽度范围的前提下，后面的freepointer会覆盖这部分构造状态；对偏移外的其它ctor状态却不能下相同结论。直接源码中没有显示`simple_calculate_sizes`重新设offset，但仅凭这些函数与旧Q10负检索**不能认证全树offset恒零**。原`RF-02`已驳回fork首次siglock必坏的说法，`RF-05`已驳回只有sighand带ctor的说法；DR-04的对象分配而未安装限定继续保留，不扩大研究信号消费者。

## G02 / B｜partial链取出、计数递增与full迁移

`online_new_empty_slab`实际在node的`list_lock`请求期间`add_to_partial`，使链表`count`依真实`list_header_add_to_head`增量；还请求`atomic_long_inc(n.nr_slabs)`和`atomic_long_add(slab.objects,n.total_objects)`，然后解锁（B01–B03/B19/B21）。此时新slab本身还不是已经返回给申请者的object。

`slab_alloc`先使用`n=&s->node`，再循环调用`get_from_partial_list`（B11/B12）。`get_from_partial_list`请求node锁，在`iter_partial_list`（实际宏转`list_header_for_each_container_safe`，再转`list_for_each_entry_safe`）中对每个有效slab调用`alloc_single_from_partial`（B06/B07/B24/B25/B33）。后者**先取`object=slab.freelist`，再调用`get_freepointer(s,object)`来更新freelist，然后无条件`slab.inuse++`，最后才返回object**（B04/B05/B32）。因此调用者`if ((object=...)!=NULL)`是在这些内存访问之后，不能反过来保护空freelist。有效partial链成员、非NULL首槽、指针/offset、inuse<objects与容器头都必须是实际先行成立的条件；若不成立，最早在get_freepointer或列表迭代处停止，不把它写成已发生的运行故障。

在有效数据模型中，对一个`objects=4`的slab，首次取出令inuse0→1、freelist指向槽1，第二次1→2；当取最后一槽使inuse3→4等于objects时，`get_from_partial_list`请求`remove_from_partial`再`add_to_full`（B08/B17–B20），最终partial头计数减1、full头计数加1（前提是删除确实找到了成员且新增节点操作成功）。这与`cache.inuse`无关。最近真实链表实现`list_header_delete_node`会先做`list_header_contains`遍历，成功才`list_del_init`、`count--`并返回节点，否则返回NULL（B22/B23/B30）；`list_header_add_to_head`请求指针连接后`count++`（B21/B28），异常链接状态下不能从函数名保证实际入链。列表的anchor和count为`List_hdr_s`独立字段（B27），并非`nr_slabs`或`total_objects`统计同一值。

**partial未命中≠申请失败。** `get_from_partial_list`解锁后返回NULL时，`slab_alloc`调用`alloc_init_new_slab`；只有这个新建callee实际返回NULL才在`slab_alloc`返回NULL（B12/B15/B16）。成功返回新slab后仍先`online_new_empty_slab`再回到while顶端**重新从partial取object**，而不是把slab结构指针返回为object（B13）。取得有效object之后才调用post-alloc hook并返回（B14）。严格保持R75-S03关于部分未命中、下层失败和再取对象的区分；R75-S02中`virt_to_slab(x)`的实参求值屏障也继续约束释放入口。

## G03 / C｜full→partial、OR下线与锁间窗口

`slab_free`先在满足R75-S02的`virt_to_slab(x)`实参转换前提下，通过`free_to_partial_list`归还object（C15/C05）。该helper首先请求node`list_lock`；**归还前**若`slab.inuse==slab.objects`（满slab），就请求`remove_from_full`和`add_to_partial`（C02/C03/C20），随后无论此前满或未满，都调`free_single_to_partial`：`slab.inuse--`、`set_freepointer(s,object,old_freelist)`、`slab.freelist=object`，然后解锁（C01/C04/C27）。按有效数据模型，满的4/4先迁移到partial，再变成3/4；已经partial的3/4则可变成2/4而不移动链表。对象真实性、没有重复释放和有效freelist写入仍是强前提。

**真正整slab下线条件**写在`free_to_partial_list`解锁之后的`slab_free`中：

`slab.inuse == 0 || n.partial.count >= cache.min_partial`（C06/C21）。

它不是`(inuse==0 && count>=threshold)`，也不是“有存活对象时一定不得下线”。条件为真就请求`offline_exceeded_empty_slab`、然后`free_slab`（C07）。在`offline_exceeded_empty_slab`中再度加锁，调用`remove_from_partial`，接着**不检查其返回值**就对`nr_slabs`和`total_objects`做减量，最后解锁（C08–C10/C25，链表删除语义B22/C16/C24）。如果节点已不在partial，helper可返回NULL但统计仍递减；当前并没有证明这样的异常调用来自合法前序，因此记为证据缺口而非新实测BUG。

**MC-07的具体、有限、单线程见证。** 选择有效`size=96`、`objects=42`、`min_partial=4`的条件cache，先在同一cache得到4个各自**完全分配**且位于full链的slab。之后由单一线程依次从不同full slab各归还**一个**object：前3次使partial.count从0增长为1、2、3，目标slab各剩41个存活对象；第4次先把另一满slab移入partial并减其inuse为41，再读到partial.count=4满足OR阈值，因而请求对该仍有41个存活对象的slab下线并向`free_slab`交接。同理，条件`size=4096`、`objects=4`、`min_partial=6`，需6个不同full slab各归还一个，第6次阈值命中时目标inuse仍为3。**交错归还次序可以是单线程连续调用，不要求同时发生多CPU并发；先把一个slab全数归还再处理下一个则通常由inuse归零分支处理，不具备相同的partial增长过程。** 这只证明在完整合法前提下的源码分支算术与状态迁移，不证明实机已产生UAF、页已物理复用，亦不撤销MC-07旧ID或声称所有调用者必触发。

锁窗口分账：`free_to_partial_list`的锁区覆盖链表迁移和freelist/inuse更新；随后`slab_free`**不持这把锁**读取`slab.inuse`与`partial.count`；`offline_exceeded_empty_slab`才重新上锁尝试删除并修改统计；`free_slab`在其返回后执行（C04/C06/C08/C10）。这有真实的源级锁间窗口，但**实际是否有另一个执行者在窗口内改动、节点是否被并发取走或失效**均未证实；单线程的MC-07门本身不依赖该并发条件。

`free_slab`的最早门还有`while(oo_order(s->oo)==0 && folio->page.flag_defs.PG_head!=0);`：如果该状态持续为真，则不能认证其后必达（C11）。若前提允许继续，才请求`folio_order`、`folio->mapping=NULL`、`smp_wmb`、`__folio_clear_slab`，最终调用`__free_pages(&folio->page,order)`（C12–C14/C26）。本批止于这个**页归还接口请求**，不做buddy合并、物理释放、地址再用或并发运行认证。

### 三专题联合审计表

| 对象/条件 | 活动操作 | 仅能保留的限定 |
|---|---|---|
| cache | object_size→两个ALIGN→cache.inuse/cache.size；min_partial与oo | 对齐、32/16位运算、编译分支和对象来源有效 |
| slab | objects、inuse、frozen、freelist写入；分配inuse++、归还inuse-- | 不同于cache.inuse，15位objects上限与有效计数条件 |
| object | next槽ctor；前一槽freelist指针写入；分配取首，归还头插 | 首槽没ctor；offset0覆盖只限对应字节；不推断所有使用者坏 |
| partial/full | 以实际成员检查/节点写入/链表count变化条件性迁移 | count不是nr_slabs；异常链接/NULL或未成功摘链不自动成立 |
| discard | inuse归零或partial阈值命中；offline、统计减量、free_slab | MC-07历史根；分离锁区；只到__free_pages请求 |

## 成果、延期与验收上限

本批六件中`facts-and-boundaries.yaml`保存14份直接/最近来源台账，按要求列`slot_initialization_matrix`、`allocation_transition_matrix`、`free_transition_matrix`、`discard_gate_matrix`、`lock_window_matrix`、`earliest_stop_matrix`与40个有限模型，均为有前提的整数/布尔及顺序代数而非运行代码。新`slab-object-lifecycle-paths.dot`60个显式节点、81条分型边、0悬空，区分宏别名、活动CALL、调用者后续、数据、条件、历史候选与分析停止点。新`deferred-findings.yaml`登记9项SL76-U01–U09证据缺口，没有另立已知MC-07/MC-08的同根findings；旧MC-09、DR-04/RF-02/RF-05、R74C-S01、R75-S01/S02/S03及05历史纠偏保留状态。

自检：新证据A47+B33+C27合计107段，全部在所标真实定义内取非空连续1–5物理行，二次GitHub固定time来源逐字回核107/107，0降级。完整本地P9、Graphviz、预处理、编译、运行仓库脚本/夹具、QEMU/实机、真正并发或物理归还均NOT_RUN。P6标签数只对**本批新六件**计算，不挪用旧04/75标签数。没有任何内核源码或原目录文件变更，也不要求Owner逐项批准代码修复。

最终仅报A/B/C=`completed_in_scope`、`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`，D00不适用；六件和唯一PR17正式评论完成后由原MYOS2-A-C05独立验收。不自签76通过、不启动77、不创建分支PR、不合并、阶段2权限不变。
