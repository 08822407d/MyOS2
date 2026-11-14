---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PERCPU-ALLOCATION-87
record_type: bounded_percpu_allocator_and_workqueue_static_report
produced_by: GPT-6 (visible model family; backend not independently attestable)
model_per_owner: GPT-6 + Extra
date: 2026-11-14
base_snapshot: time
read_channel: connector
status: STATIC_AUTHOR_COMPLETE_PENDING_MAINLINE
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions: [PA87-U01, PA87-U02, PA87-U03, PA87-U04, PA87-U05, PA87-U06, PA87-U07, PA87-U08, PA87-U09]
---

# 第87批：per-CPU 分配适配、分块扩容与工作队列调用者

本批沿用 MYOS2-LEAD-002-CHAT-EXEC-11、固定time@a039d9803ade和PR17既定唯一执行分支。固定123合同主线fc14432f2a56、执行父596fb3d7e1a5。没有执行内核程序、编译器、仓库Python/脚本、Graphviz、QEMU、实机或并发实验；以下按源码、整数模型、合法指针访问和真实执行四层分开说明。证据库为本批102项新的固定源码定义短锚（evidence.md）；facts-and-boundaries.yaml 给出10组72行矩阵、28例有输入与中间值的条件模型，以及D00和读取账。

## D00｜第86批 R86-01：仅追加全包原始标签计数纠正

当前主线验收为：86 RETURN，仅R86-01自检文档范围，86的技术主干受R86-S01/S02/S03限定保留。87对固定86原提交的**六件文件逐件独立重新读取并统计字面前缀**，没有重跑历史作者本地P9，也没有把主线回执数字当成本人实际统计。结果如下：

| 第86批原件 | 字面出现次数 | 真实定义锚 | 说明文字 |
|---|---:|---:|---:|
| MANIFEST.md | 0 | 0 | 0 |
| percpu-bootstrap.md | 0 | 0 | 0 |
| facts-and-boundaries.yaml | 1 | 0 | 1 |
| percpu-bootstrap-paths.dot | 0 | 0 | 0 |
| deferred-findings.yaml | 0 | 0 | 0 |
| evidence.md | 77 | 77 | 0 |
| **全包** | **78** | **77** | **1** |

说明文字具体位于 `facts-and-boundaries.yaml` 的 `source_ledger` 中、`path=mykernel/include/linux/kernel/asm-generic/sections.h` 条目的 `extent` 字符串。它表达“extern声明不应充当独立强证据”的意思，却在句中使用了完整的保留标签起始字面；该字面也必须被六件全包的P6/P9-5原始计数纳入，但**不是第78条源码定义锚**。本报告故意不复写那串完整起始字面，避免再在本批制造虚增的原始标签。

因此明确**撤回**第86批原“全包自检77/77/0一致”的总括：其77项定义锚属另一指标，全包实际原始出现数为78；原MANIFEST的77/77/0仍冻结为历史作者自报，不倒改为78/78/0，不改第86批六件、原评论6870949847或原审查回执。87自身的全包计数另行核算，D00不增加旧86的事实锚，也不将78统计为新内核问题。**R86-01：CORRECTION_SUBMITTED_PENDING_C06_REVIEW**；仅主线C06 Pro可以判断关闭。R84-01已由C06文档闭合，85按R85-S01/S02/S03限定PASS_PENDING_LOCAL；两者均不重开。

## G01/A｜请求类型、对齐适配和扣账

`simple_pcpu_alloc_noprof(size_t size,size_t align,bool reserved,gfp_t gfp)`接收无符号`size_t`长度/对齐量，但其调用的`simple_pcpu_find_block_fit(chunk,int size,size_t align)`和`simple_pcpu_alloc_area(chunk,int size,int start)`使用`int`长度。后者的`free_offset`和`free_bytes`也是`int`，而外部`chunk_size`是`size_t`。因此每步先检查编译ABI类型、输入在int范围之内、所有有符号加减有效，不能把超过INT_MAX的size_t值直接按宿主二进制截断成可继续模拟的int。

`ALIGN(x,a)`真实调用`__ALIGN_KERNEL`并使用`(typeof(x))(a)-1`形成位掩码，**只有在a是正的、可表示的2的幂以及相关int求和不溢出的声明前提下**，才能把它解释成常规向上对齐。源中另有`round_up`宏，但本处适配实际调用的是`ALIGN`，不能替换为另一个工具。

令旧`free_offset=x`、旧`free_bytes=F`、有效请求q、正2幂对齐a，则适配中间量是：

`start = ALIGN(x,a); gap = start-x; need = start+q-x = gap+q`。

如果`F<need`，fit返回`-ENOMEM`；反之返回`start`。然而真正提交函数在一条空体`while(start < chunk->free_offset);`之后，只执行`free_offset=start+q`、`free_bytes-=q`、返回`start`。**检查需要gap+q，提交只扣q**。如果先前物理尾部可用量为`C-x=F`，新的账面`F-q`比实际尾部`C-(start+q)`多出`gap`。这是一条不依赖机型的条件性源码记账差异，但不等于已执行非法写入。

具体有限顺序例（M02–M04）：假设合法新块`C=16384`、初始`x=0,F=16384`，每次有效C语言运算并且调用均返回。第一次请求`q=1,a=1`，返回start0、变成x1/F16383；第二次`q=16380,a=4`，start4、gap3、need16383恰好被接收，变成x16384/F3；第三次`q=3,a=1`，start16384、need3通过，提交结果x16387/F0，超出**单CPU名义chunk_size容量3字节**。源码未在这一步直接写入数据，且背后整体申请可能是N*C而非一张独立C大小的对象，所以这里认证的是容量账和被接收offset不一致，不假造真实分配越界事件。单次不带填充的准确可容纳/不可容纳、0长度和预先存在的尾部不足由M01/M05/M06另列。

输入越界模型还包括：`align=0,x=1,q=1`在特定int掩码条件下可以使start=0、need=0而进入没有显式进展的`while(0<1);`；`align=3,x=5`可得start5但5不是3的整数倍；这些**都在正2幂对齐的预设合同之外**，不是实际工作队列调用过的输入。符号有符号溢出或size_t→int不适配时，模型在转换/算术点停止，不模拟虚假的继续状态。

## G02/B｜reserved/normal、扩块与错误前缀

有`reserved=true`且`pcpu_reserved_chunk`非NULL时，函数只对该保留块做fit和提交；fit为负会直达`fail_unlock`，而不是从这里回退普通列表。若`reserved=true`但保留块为NULL，判断为假，实际接入普通`restart`路径；若`reserved=false`也从普通列表起。早期first与reserved两次创建都会调用`list_header_add_to_tail(&pcpu_chunk_lists,&chunk->list)`，因此普通扫描的是**共同链表**，并非先天只含后期普通块。某请求可以在有条件的列表顺序中通过普通路径选择到保留chunk，这与独立的reserved快捷分支不能互相混成一个调用。

普通链以`list_header_for_each_container`展开为带表头哨兵的`list_for_each_entry`，每个候选先fit，成功再提交并`goto area_found`；扫描完仍无命中才调用`simple_pcpu_create_chunk`。新创建元数据使用`kzalloc(sizeof(*chunk),gfp)`，检查元数据NULL；随后初始化链节点、`free_offset=0`、`free_bytes=chunk_size`，再`kzalloc(N*chunk_size,gfp)`作为data。实际源码**没有为data单独判空**，紧接着做`chunk->base_addr = chunk->data - (ulong)pcpu_base_addr`并把元数据入链、返回chunk。若data为NULL，必须在这个GNU void指针算术和对象有效性门停止正常证明，不能宣布后续链表必然安全发布。

新chunk元数据返回非NULL后，外层`goto restart`，但没有缩小请求、修改align或改变新块的chunk_size。若`q>C`且新块恒为C，新的fit仍不足；只有某次创建失败、外部状态变化或满足其它独立条件才能提供退出依据。例M17令`C=16384,q=16385,a=1`：每个新块自由空间初值16384，need16385恒不匹配；如果第k次元数据失败则返回NULL；若资源无限成功则有限终止得不到认证。**不能把这一有条件的不进展描述成已经实测死循环。**

锁方面，`simple_pcpu_alloc_noprof`内`spin_lock_irqsave`与`spin_unlock_irqrestore`的几处调用均为注释，当前源位置没有执行这些锁调用的文本。列表宏及插入实现存在，但跨调用者、并发修改和外部同步规则没有审计；因此既不借注释声称线程安全，也不进行实机并发诊断。失败路径`fail_unlock/fail`只见普通返回NULL，保留路径和扩块路径应按各自不同前缀理解。

## G03/C｜相对指针返回、per_cpu_ptr与工作队列

真实`area_found`执行的是`ptr = chunk->base_addr + off`，随后返回`ptr`；紧邻的`__addr_to_pcpu_ptr`和逐CPU清零是注释。初始first chunk的`base_addr=0`；早期reserved和晚期new chunk则将`data - (ulong)pcpu_base_addr`放入`base_addr`。表达式左侧仍是`void*`，不能擅自写成两个unsigned整数相减；必须把“假设GNU扩展下的数值编码”“C指针对象合法性”和“实际CPU可访问地址”分开。

所读宏链中`per_cpu_ptr(P,i)`经`SHIFT_PERCPU_PTR(P, per_cpu_offset(i))`，最终`RELOC_HIDE`将P转成`unsigned long`并加offset再转回指针。沿86/R86-S01限定，在LP64/无越界索引及w位无符号模计算的声明模型内，令B为首块数据地址、T为链接`__per_cpu_start`、S为模板尺寸、`O_i=(B-T+i*S) mod 2^w`，则`A_i=(P+O_i) mod 2^w`。对first若把源码`base_addr=0`下的`P=off`仅当作数字，算得`A_i=off+B-T+i*S`；对另块data=Q、`P=Q-B+off`，得`A_i=Q+off-T+i*S`。二者在T非零的数值模型里，都不自动等于预期的`B(or Q)+off+i*S`，相差一个-T。这可能是相对编码与解码契约缺失，**但绝不意味着NULL void*算术或跨对象减法已合法、指针已经真的落在某错误对象、或工作队列真的执行了错误写入**。旧PC86-F05/U09关于单位跨度仍保持原qualified_id，不能将两者混写为已证系统故障。

数值例M19：LP64下假定T=0x100000、B=0x200000、S=4096，首块C16384、`free_offset=12288`、`free_bytes=4096`，工作队列`alloc_percpu(pool_workqueue_s *)`若以8字节指针类型请求8字节/对齐8，fit得off12288(0x3000)，提交后x12296/F4088。首块编码P=0x3000，公开O0=0x100000，则`per_cpu_ptr(P,0)`数值为0x103000，目标首块数据数字B+off=0x203000；差-T。CPU1同理A1=0x104000、期望B+S+off=0x204000。M21/M22为reserved/new单独举出Q-B编码的数字结果；M23仅说明若T=0，数值可以匹配，却仍不证明地址表达/真实对象有效。上述所有地址是**独立输入的条件模型**，不是本内核实际链接布局、物理机地址或CPU访存记录。

工作队列真实调用从`simple_alloc_and_link_pwqs`进入。其`wq->cpu_pwq = alloc_percpu(pool_workqueue_s *)`只申请每CPU的一个**PWQ指针槽**，不是一个完整PWQ结构体；若返回NULL，直接`goto enomem`。有`!(wq->flags & WQ_UNBOUND)`时，`WQ_BH`选择BH池，否则CPU普通池，`highpri`用作两种池中0/1下标（在选用的`NR_STD_WORKER_POOLS=2`定义下）。每CPU循环经`per_cpu_ptr`取得槽，执行`*pwq_p=kmem_cache_alloc(pwq_cache,GFP_KERNEL)`，非NULL后填充pwq->pool/wq并添加至wq->pwqs链表，完成全部CPU才返回0。这条链至少需要perCPU地址计算、slot范围、slab分配和链表调用合法返回四组门槛，不能由malloc函数名本身判定成功。

三条异常消费必须拆开。第一，`alloc_percpu`返回NULL时内层返回`-ENOMEM`。第二，已经链接部分PWQ后某CPU的`kmem_cache_alloc`返回NULL，内层遍历槽，对非NULL指针调用`kmem_cache_free`，但所读错误分支没有将先前的pwqs_node从wq->pwqs摘除，且`free_percpu`一行已注释，随后置`wq->cpu_pwq=NULL`并返回错误。这里最多是条件性的遗留链接/资源所有权风险，不能在未证合法slot读取的条件下继续模拟真实UAF。第三，当`WQ_UNBOUND`成立且cpu_pwq申请成功，跳过受约束CPU循环后执行`return ret`，而`ret`在这个已完整读取的函数中只有int局部声明没有赋值；该分支没有已定义的确定返回值。可见的早期events和events_bh两次直接调用分别使用flags0和WQ_BH，不代表本例实际发生。

更关键的是上层`simple_alloc_workqueue`在wq对象kzalloc成功并初始化后，**调用`simple_alloc_and_link_pwqs(wq)`却未读取或检查其返回值**，之后仍执行`list_header_add_to_tail(&workqueues,&wq->list)`并返回wq。因而在明确的“内层失败但所有上层后续调用合法返回”条件下，外层有继续发布不完整workqueue的源分支。仅凭源码的`workqueue_init_early`中存在`simple_alloc_workqueue("events",0,0)`和`("events_bh",WQ_BH,0)`，不证明真实启动已经到达或任何worker/工作项已经运行。所读`__simple_queue_work`随后按CPU读取cpu_pwq槽和pool，进一步说明它是依赖这些地址条件的代表消费者，但本批不进入调度、完整释放或多核现场。

## 结论与交回边界

本包三个新主题的10组72行矩阵、28个有限条件模型、9项PA87-F01..F09条件风险与9项PA87-U01..U09证据缺口，均使用102个新的真实固定time短锚支持。R86-01只是当前文档补正，不算第四个新主题，也不改变旧77/77/0。条件候选可能与旧PC86及更早qualified_id关联，不能把旧新条数相加当作已确认bug总数。源中写入表达式、算术模型通过、最终存储有效、硬件执行/并发可达性均是不同的认证门。

交付为**静态代码分析**：完整本地P9未运行、无内核修改/补丁、不运行构建或仓库脚本、无Graphviz/QEMU/真实内存实验，不调用额外模型/子代理/付费API。最终提交、PR17本人唯一正式评论及后置远端回读的真实对象值只能在发布之后确认。第87批及R86-01追加纠正均须交原MYOS2-A-C06 Pro独立验收；执行者不自行签闭合/通过、不启动88。

## 十组矩阵：逐行前提、结论与止证门

### allocation_request_and_fit_matrix（7行）

| 行ID | 主题 | 源码表达 | 条件结论 | 停止证明的前提 | 新锚ID |
|---|---|---|---|---|---|
| PA87-A-REQ01 | request type | find fit receives int size | request above INT_MAX requires a conversion with implementation-dependent result; cannot assume modulo 2^32 | selected C ABI, exact compiler and actual caller size | PA87-E023, PA87-E024, PA87-E014 |
| PA87-A-REQ02 | GFP selection | pcpu_gfp=gfp&(GFP_KERNEL\|__GFP_NORETRY\|__GFP_NOWARN) | only whitelisted flag bits forwarded to new backing allocator | flag definitions/allocator effect not fully audited | PA87-E001, PA87-E025, PA87-E056 |
| PA87-A-REQ03 | fit input | alloc_offset=ALIGN(free_offset,align) | fit produces a candidate aligned start only under supported positive power-of-two align and valid int operations | type cast to int, signed overflow/zero/nonpower2 align | PA87-E008, PA87-E014, PA87-E018, PA87-E019, PA87-E020 |
| PA87-A-REQ04 | need expression | need_size=alloc_offset+size-free_offset | need=padding+requested bytes, independent of chunk total C | value-range and original free_bytes invariant | PA87-E014, PA87-E015 |
| PA87-A-REQ05 | rejection | return -ENOMEM from fit | no area commit on this rejection path, branch caller treats off<0 as failure | allocation and exit context must be followed | PA87-E015, PA87-E022, PA87-E029, PA87-E031 |
| PA87-A-REQ06 | new chunk baseline | free_offset=0, free_bytes=chunk_size (int) | initial free units are bytes, but size_t to int narrowing possible | chunk_size within int range and zero allocation success | PA87-E010, PA87-E011, PA87-E008 |
| PA87-A-REQ07 | first chunk baseline | free_offset=size_sum, free_bytes=chunk_size-size_sum | first chunk initial free region starts past template and reservation (source metadata) | first chunk success, valid sum, int conversion, real mapped bytes | PA87-E026, PA87-E008 |

### alignment_and_capacity_matrix（6行）

| 行ID | 主题 | 源码表达 | 条件结论 | 停止证明的前提 | 新锚ID |
|---|---|---|---|---|---|
| PA87-A-AL01 | power-of-two align | ALIGN(x,a)=((x+(a-1))&~(a-1)) | candidate is nondecreasing aligned integer within declared range | full C usual arithmetic and cast preconditions | PA87-E018, PA87-E019, PA87-E020 |
| PA87-A-AL02 | padding charge comparison | need_size=gap+q; reject if free_bytes<need | fit correctly includes padding in required capacity under valid int arithmetic | does not imply commit decrements same amount | PA87-E014, PA87-E015, PA87-E017 |
| PA87-A-AL03 | zero alignment condition | ALIGN returns 0 with sourced mask expression in this isolated case | start can be below free_offset; area contains empty while guard | not evidence real alloc_percpu ever supplies a=0, and C overflow/typing assumptions matter | PA87-E016, PA87-E018, PA87-E019, PA87-E020 |
| PA87-A-AL04 | non-power-of-two alignment | mask=2; (5+2)&~2=5 | result5 is not a multiple of3; bit-mask macro is not arbitrary ALIGN | actual caller type alignment generally a valid power of two but other inputs unknown | PA87-E018, PA87-E019, PA87-E020 |
| PA87-A-AL05 | signed arithmetic boundary | int operations and int metadata | do not continue finite model through C undefined signed overflow | actual compile ABI and link values not observed | PA87-E008, PA87-E014, PA87-E015, PA87-E019, PA87-E020 |
| PA87-A-AL06 | capacity bound | new 3-byte request align1 passes free_bytes>=need | source accepts a candidate off=16384 with end16387 beyond C in accumulated-padding model | not a physical memory write; actual caller/states and backing mapping unknown | PA87-E014, PA87-E015, PA87-E017, PA87-E011 |

### area_commit_and_failure_matrix（6行）

| 行ID | 主题 | 源码表达 | 条件结论 | 停止证明的前提 | 新锚ID |
|---|---|---|---|---|---|
| PA87-A-CM01 | empty while | while (start < free_offset); | empty-loop has no visible state mutation; normal fit with valid align and no wrap returns start>=free_offset | optimizer/concurrency semantics and reachability not certified | PA87-E016, PA87-E014, PA87-E018 |
| PA87-A-CM02 | offset advancement | free_offset=start+size | total consumed tail span includes padding+request | valid start and size; no actual memory store by this line | PA87-E017 |
| PA87-A-CM03 | free byte accounting | free_bytes-=size (no subtraction of padding) | new reported free=F-q, rather than F-gap-q; overstates physical tail by gap if initial invariant held | comparison and valid C arithmetic assumptions | PA87-E017, PA87-E015 |
| PA87-A-CM04 | short sequential witness | first returns0 -> off1/free16383; second returns4 -> off16384/free3; third fit returns16384 -> off16387 | conditional acceptance of offset extending 3 bytes beyond nominal C despite finite-accounting success | not a demonstrated invalid store or real workqueue request | PA87-E014, PA87-E015, PA87-E017, PA87-E011 |
| PA87-A-CM05 | fit error boundary | fit returns -ENOMEM and reserved/normal caller branches before area commit | ordinary list can keep scanning, reserved path can fail immediately | caller route and valid chunk are separate conditions | PA87-E015, PA87-E029, PA87-E031 |
| PA87-A-CM06 | return-pointer phase | area_found encodes ptr=chunk->base_addr+off then returns it | value is returned pointer encoding, not a memcopy or verified memory write | GNU void pointer arithmetic assumption, final pointer decoding and object range | PA87-E038, PA87-E039, PA87-E009 |

### reserved_normal_routing_matrix（7行）

| 行ID | 主题 | 源码表达 | 条件结论 | 停止证明的前提 | 新锚ID |
|---|---|---|---|---|---|
| PA87-B-R01 | reserved existing | if(reserved&&pcpu_reserved_chunk) then fit reserved | only reserved chunk considered before goto exits; does not scan normal list in this arm | successful chunk object and valid type conversions | PA87-E028, PA87-E029, PA87-E030, PA87-E006 |
| PA87-B-R02 | reserved insufficient | goto fail_unlock -> fail -> return NULL | no fallback to normal list or new chunk under this branch | source-level not actual failure outcome | PA87-E029, PA87-E040, PA87-E041 |
| PA87-B-R03 | reserved absent | if condition false -> restart label | normal chunk traversal/new chunk path is used, not unconditional NULL | list may be empty/invalid, later failures unknown | PA87-E028, PA87-E031, PA87-E034 |
| PA87-B-R04 | ordinary request | skip reserved if, scan pcpu_chunk_lists | first, reserved and later chunks can all be on shared list if helpers inserted them | actual insertion/list validity and ordering unknown | PA87-E028, PA87-E031, PA87-E007, PA87-E012, PA87-E013 |
| PA87-B-R05 | ordinary first hit | off>=0 -> area_found | does not need new chunk on that successful iteration | pointer encoding and backing access not proved | PA87-E031, PA87-E032, PA87-E033, PA87-E038 |
| PA87-B-R06 | empty/no fit | simple_pcpu_create_chunk(pcpu_gfp) | normal request can grow via newly allocated chunk | new metadata/data and restart progress unknown | PA87-E031, PA87-E034, PA87-E035 |
| PA87-B-R07 | reserved success | goto area_found | returns encoded base+off from that reserved chunk | GNU pointer arithmetic assumption, offset decode may mismatch address | PA87-E030, PA87-E038, PA87-E039, PA87-E097 |

### chunk_creation_publication_matrix（7行）

| 行ID | 主题 | 源码表达 | 条件结论 | 停止证明的前提 | 新锚ID |
|---|---|---|---|---|---|
| PA87-B-CH01 | new metadata | chunk=kzalloc(sizeof(*chunk),gfp); if(!chunk) return NULL | NULL metadata has explicit error return before any list insertion | lower kmalloc/slab behavior not exhausted | PA87-E001, PA87-E010, PA87-E054, PA87-E056 |
| PA87-B-CH02 | new chunk initialization | INIT_LIST_HEAD then free_offset=0, free_bytes=chunk_size | free_bytes int receives size_t chunk_size; conversion is conditional | actual chunk size, int range and object validity | PA87-E011, PA87-E008 |
| PA87-B-CH03 | backing allocation | data=kzalloc(nr_cpu_ids*chunk_size,gfp) | requested zeroing data allocation follows metadata; size multiplication may overflow selected width | real N, allocation/zero success unknown | PA87-E011, PA87-E054 |
| PA87-B-CH04 | data allocation failure | base_addr=data-(ulong)pcpu_base_addr; list insertion then return chunk | helper has no data-null check before pointer arithmetic and list publication | C/GNU extension, NULL pointer math, list insertion and actual effect not observed | PA87-E012, PA87-E009, PA87-E053 |
| PA87-B-CH05 | list storage | wrapper adds to anchor and increments count | new chunks join same list as two earlier early chunks when all insertions completed | list helper, allocation and concurrent validity not certified | PA87-E007, PA87-E012, PA87-E053, PA87-E045, PA87-E046 |
| PA87-B-CH06 | chunk identity | early helper likewise inserts into pcpu_chunk_lists; late initializes free_offset=0 | first/reserved/new are different objects even if sharing list | actual object count depends on prior early allocations | PA87-E013, PA87-E010, PA87-E011, PA87-E007 |
| PA87-B-CH07 | late kzalloc vs early memblock | kzalloc passes __GFP_ZERO to kmalloc | different allocation paths/zeroing preconditions; no per-CPU per-area clear inferred | lower allocator/hardware mapping not audited | PA87-E010, PA87-E011, PA87-E013, PA87-E054, PA87-E037 |

### restart_lock_and_failure_matrix（7行）

| 行ID | 主题 | 源码表达 | 条件结论 | 停止证明的前提 | 新锚ID |
|---|---|---|---|---|---|
| PA87-B-X01 | normal traversal | list_first_entry/list_for_each_entry anchor walk | on valid list, each observed chunk may be considered for fit; no guaranteed suitable chunk | list implementation, empty list and concurrent mutation still gated | PA87-E031, PA87-E047, PA87-E048, PA87-E049, PA87-E050 |
| PA87-B-X02 | new chunk restart | goto restart without resetting size,align or changing test | new chunk may also be unsuitable if request exceeds capacity | resource availability and data validity, not a proof of infinite execution | PA87-E034, PA87-E035, PA87-E014, PA87-E015 |
| PA87-B-X03 | new meta failure | goto fail -> NULL | normal path stops with NULL under that return | actual allocator failure not performed | PA87-E034, PA87-E041 |
| PA87-B-X04 | new data failure | helper still returns metadata nonNULL after list insertion | caller may restart against unusable data; cannot infer safe ptr return | C pointer validity at base_addr construction and subsequent actions unknown | PA87-E010, PA87-E011, PA87-E012, PA87-E035 |
| PA87-B-X05 | locking in this function | no active lock invocation at these locations | static path shows no lock operations here, not whole-system lock absence | other wrappers/external callers and actual concurrency unreviewed | PA87-E042, PA87-E043, PA87-E044 |
| PA87-B-X06 | return failure | fail_unlock/fail flow returns NULL | no replacement object, no automatic fallback after reserved fit failure | pointer/cleanup of previously inserted chunks not audited | PA87-E029, PA87-E034, PA87-E040, PA87-E041 |
| PA87-B-X07 | list publication hazards | list insertion before return even without data success test | cannot equate metadata success with valid perCPU backing data | outer allocator, linked list behavior and lifetime unknown | PA87-E012, PA87-E053 |

### returned_pointer_decode_matrix（8行）

| 行ID | 主题 | 源码表达 | 条件结论 | 停止证明的前提 | 新锚ID |
|---|---|---|---|---|---|
| PA87-C-DEC01 | workqueue type | alloc_percpu(type) forwards sizeof(type),alignof(type),false,GFP_KERNEL | perCPU allocated object is a slot holding a PWQ pointer, not a pool_workqueue_s struct itself | size/align actual ABI and later pointer validity | PA87-E059, PA87-E087, PA87-E088, PA87-E089 |
| PA87-C-DEC02 | returned encoding | ptr=chunk->base_addr+off; return ptr | the returned value is an encoded relative perCPU pointer from chunk, not a raw CPU0 data pointer | GNU void pointer arithmetic and NULL/object provenance must be separately justified | PA87-E038, PA87-E039, PA87-E009 |
| PA87-C-DEC03 | per_cpu_ptr decoding | SHIFT_PERCPU_PTR(P,per_cpu_offset(i)) via RELOC_HIDE unsigned long add | integer address is (P+O_i) mod 2^w under LP64/type assumptions | integer-pointer cast, actual object/mapping/indices unverified | PA87-E091, PA87-E092, PA87-E093, PA87-E094 |
| PA87-C-DEC04 | offset provenance | O_i=(B-T+i*S) mod 2^w in qualified R86 formula | substituting the actual offset source into decoded address yields P+B-T+i*S | 86 S01 modular caveat; memory/cpu index must be valid | PA87-E096, PA87-E027, PA87-E004 |
| PA87-C-DEC05 | first chunk | P=off as numeric encoding; decode=off+B-T+i*S | differs from desired physical backing numeric B+off+i*S by -T unless T=0 mod 2^w | NULL-base pointer arithmetic not certified; T and B are link/runtime values | PA87-E026, PA87-E038, PA87-E039, PA87-E093, PA87-E094 |
| PA87-C-DEC06 | reserved chunk | P=R-B+off; decode=R+off-T+i*S | does not automatically equal R+off+i*S; source lacks explicit +T in encoding | C pointer arithmetic and underlying mapped object unverified | PA87-E097, PA87-E038, PA87-E093, PA87-E094 |
| PA87-C-DEC07 | new ordinary chunk | P=Q-B+off; decode=Q+off-T+i*S | new chunk has same relative-offset discrepancy under the hypothesis | actual Q allocation, initialization and pointer validity unknown | PA87-E012, PA87-E038, PA87-E093, PA87-E094, PA87-E096 |
| PA87-C-DEC08 | pointer value vs object | RELOC_HIDE casts value through unsigned long back to pointer | arithmetic mod result does not prove legal C pointer, allocation ownership, alignment or writable slot | actual compiler, memory map and runtime operation NOT_RUN | PA87-E094, PA87-E093, PA87-E008 |

### workqueue_alloc_use_cleanup_matrix（13行）

| 行ID | 主题 | 源码表达 | 条件结论 | 停止证明的前提 | 新锚ID |
|---|---|---|---|---|---|
| PA87-C-WQ01 | outer field type | typed perCPU pointer to pool_workqueue_s* slots | slot storage and pointed-to pwq heap objects are distinct allocations | pointer-size ABI and actual perCPU address valid | PA87-E083, PA87-E089, PA87-E080 |
| PA87-C-WQ02 | outer allocate failure | if(!wq->cpu_pwq) goto enomem | helper returns -ENOMEM through failure branch without perCPU cleanup if pointer null | caller must check return separately | PA87-E059, PA87-E060, PA87-E071 |
| PA87-C-WQ03 | bound pool selection | WQ_BH chooses bh_worker_pools else cpu_worker_pools; highpri used as index | bounded CPU pool chosen conditionally; flags are enum bits not proof requested by actual caller | perCPU worker pool templates, CPU indices, effective config | PA87-E061, PA87-E062, PA87-E084, PA87-E085, PA87-E101 |
| PA87-C-WQ04 | perCPU slot allocation | pwq_p=per_cpu_ptr(wq->cpu_pwq,cpu); *pwq_p=kmem_cache_alloc(pwq_cache,GFP_KERNEL) | allocates a pwq object per visited CPU and puts its pointer in the CPU slot | decoded slot belongs to backing buffer and slab allocator returns valid object | PA87-E063, PA87-E064, PA87-E065, PA87-E055 |
| PA87-C-WQ05 | PWQ linkage | pwq->pool=pool,pwq->wq=wq, list_header_add_to_tail(wq->pwqs,pwqs_node) | linked PWQ object becomes member of wq's pwqs list | list node and header valid, no concurrency/reentrancy tested | PA87-E066, PA87-E067, PA87-E081, PA87-E082, PA87-E053 |
| PA87-C-WQ06 | bound success | return 0 after CPU loop | helper returns zero even though actual worker operation not started | real call chain/allocator/CPU loop completion unknown | PA87-E067, PA87-E068, PA87-E098, PA87-E099 |
| PA87-C-WQ07 | unbound branch | skip bound loop and execute return ret; ret local int has no assignment in read function | potential indeterminate return read for unbound scope; not immediate named events0/BH caller | actual caller flags and compiler C behavior not executed | PA87-E059, PA87-E061, PA87-E068, PA87-E084 |
| PA87-C-WQ08 | partial slot failure | enomem iterates all slots and kmem_cache_free for nonNULL objects | cleanup has conditional frees but the shown function does not remove earlier pwqs_node links | slot initialization/validity and list lifetime afterward not certified | PA87-E065, PA87-E066, PA87-E067, PA87-E069, PA87-E070, PA87-E100 |
| PA87-C-WQ09 | perCPU storage cleanup | free_percpu line commented, then cpu_pwq=NULL, return -ENOMEM | perCPU slot allocation is not visibly released in this scoped function | other owner reclaim paths not exhausted; no actual leak measured | PA87-E070, PA87-E071 |
| PA87-C-WQ10 | outer ignores helper failure | simple_alloc_and_link_pwqs(wq); no return assignment/test; list_header_add_to_tail(workqueues) | even if inner returns -ENOMEM, outer source proceeds to publish and return wq if all preceding operations return | earlier memory writes/list calls and actual reachability conditional | PA87-E072, PA87-E073, PA87-E074, PA87-E075, PA87-E076 |
| PA87-C-WQ11 | caller first alloc null | if(!wq) return NULL | one earlier failure branch exits before inner perCPU allocation | actual allocated object state/flags not established | PA87-E072, PA87-E054 |
| PA87-C-WQ12 | named early caller | pwq_cache=KMEM_CACHE; simple_alloc_workqueue("events",0,0) and ("events_bh",WQ_BH,0) | source contains two direct calls with ordinary and BH flags; not an actual boot execution log | workqueue_init_early call reachability, needed cache setup and system initialization unknown | PA87-E077, PA87-E078, PA87-E079, PA87-E084 |
| PA87-C-WQ13 | representative work consumer | __simple_queue_work uses *per_cpu_ptr(wq->cpu_pwq,cpu), then pwq->pool | later consumer depends on perCPU pointer validity and nonNULL PWQ; this batch does not audit enqueue/schedule | actual queue work start and synchronization unknown | PA87-E102, PA87-E083, PA87-E093 |

### cross_stage_proof_gates_matrix（6行）

| 行ID | 主题 | 源码表达 | 条件结论 | 停止证明的前提 | 新锚ID |
|---|---|---|---|---|---|
| PA87-C-X01 | 86 static first chunk | S, B, T, N, pcpu_unit_offsets source constraints feed O_i | 86 RETURN doc issue does not invalidate static source facts but is not standalone PASS | R86-01 pending C06 and no real build/startup | PA87-E026, PA87-E027, PA87-E096 |
| PA87-C-X02 | align bookkeeping to pointer | fit may approve off>=C while free_bytes overestimates tail | numerical accepted off not proof legal pointer or actual memory overwrite | selected ABI, caller request and backing region validity | PA87-E014, PA87-E015, PA87-E017, PA87-E038 |
| PA87-C-X03 | new chunk failure to consumer | new chunk inserted; subsequent fit/area may return encoded pointer | non-null metadata does not prove valid perCPU slot backing or PWQ writes | C/GNU pointer arithmetic and real allocation unknown | PA87-E010, PA87-E011, PA87-E012, PA87-E064 |
| PA87-C-X04 | loop and cleanup dependency | partial failure cleanup scans N slots and frees nonNULL values | requires all slots readable and initialized; cannot derive cleanup success solely from branch text | actual ABI, CPU count, earlier template/new chunk zeroing | PA87-E069, PA87-E098, PA87-E099 |
| PA87-C-X05 | publication and caller return | outer workqueue global list receives object and returns wq | one source-level publication path may continue despite inner failure | do not assert real linked workqueue usable or actual worker execution | PA87-E074, PA87-E075, PA87-E076, PA87-E058 |
| PA87-C-X06 | historic qualifications | pointer modular arithmetic, prior source state and boot-CPU assumptions separate | no old root cause reclassified by this analysis | local P9, compiled ABI, CPU memory and full workqueue lifecycle all NOT_RUN | PA87-E094, PA87-E096, PA87-E102 |

### ABC_join_matrix（5行）

| 行ID | 主题 | 源码表达 | 条件结论 | 停止证明的前提 | 新锚ID |
|---|---|---|---|---|---|
| PA87-ABC-J01 | first fit to consumer | fit computes aligned off and need, commit then ptr base+off, per_cpu_ptr adds O_i | complete numeric chain only under checked types/width and valid backing | source does not prove pointer is a legal lvalue | PA87-E014, PA87-E015, PA87-E017, PA87-E059, PA87-E089, PA87-E093 |
| PA87-ABC-J02 | routing into workqueue | normal list scan/extend may use first, early reserved or new chunk | workqueue cannot assume its perCPU slot came exclusively from first chunk | list and allocation success not measured | PA87-E088, PA87-E031, PA87-E034, PA87-E013, PA87-E012 |
| PA87-ABC-J03 | error propagation break | helper returns -ENOMEM while outer ignores call result | the outer global WQ publication decision is independent of inner status | actual memory writes and caller receipt unknown | PA87-E041, PA87-E060, PA87-E065, PA87-E071, PA87-E074, PA87-E075 |
| PA87-ABC-J04 | cleanup not complete ownership | conditional kmem_cache_free, commented perCPU slot free and no list unlink in shown path | allocated PWQ pointers and perCPU slot block have distinct cleanup requirements | other cleanup and actual workqueue lifespan not exhausted | PA87-E067, PA87-E069, PA87-E070, PA87-E071, PA87-E100 |
| PA87-ABC-J05 | encoding for three chunks | all are passed as chunk.base_addr+off into same per_cpu_ptr decode | with T nonzero numeric formulas include -T, not automatically backed address | real compiler/link/runtime layout unknown | PA87-E026, PA87-E097, PA87-E012, PA87-E038, PA87-E093, PA87-E094 |

## 28个有限条件模型（中间值与实际单位见facts）

| 模型 | 输入 | 数值/分支输出 | 首个不能证明的位置 |
|---|---|---|---|
| PA87-M01 | zero-size request, align1 | {"new_free_offset":0,"new_free_bytes":16,"returned_off":0} | zero-byte pointer/actual caller validity untested |
| PA87-M02 | C16384 first small request | {"after":{"free_offset":1,"free_bytes":16383},"returned_off":0} | no real heap allocation made |
| PA87-M03 | same chunk next padded large request | {"after":{"free_offset":16384,"free_bytes":3},"physical_tail_bytes":0,"overstated_free_bytes":3} | no actual memory access or observed caller request |
| PA87-M04 | same chunk third request from overstated free count | {"after":{"free_offset":16387,"free_bytes":0},"overrun_beyond_nominal_C":3,"accepted_offset":16384} | accepted offset is not proof of actual invalid memory write |
| PA87-M05 | exact fit no padding | {"new_free_offset":16,"new_free_bytes":0} | not certified as real boot chunk state |
| PA87-M06 | insufficient fit | {"fit_return":"-ENOMEM=-12","accounting_mutated_by_fit":false} | caller may choose reserved fail or normal continue; not evaluated here |
| PA87-M07 | zero alignment violates power-of-two precondition | {"area_while_body":"empty","normal_return":"not established"} | no real client known to pass align0; C compiler/liveness effects not run |
| PA87-M08 | non-power-of-two align3 | {"new_free_offset":7,"new_free_bytes":9,"returned_start":5,"start_mod3":2} | not a demonstrated caller alignment; no real allocation |
| PA87-M09 | size_t to int bound | {"fit_size_value":"NOT_CERTIFIED","need_size":"NOT_EVALUATED"} | stop at conversion, do not assume two's-complement wrap |
| PA87-M10 | signed int ALIGN overflow | {"fit_return":"NO_DEFINED_NUMERIC_MODEL"} | stop before signed overflow; no continued commit |
| PA87-M11 | reserved chunk fits | {"return":"encoded reserved base+0","new_free_offset":8,"new_free_bytes":8} | GNU pointer expression/real encoded pointer unresolved |
| PA87-M12 | reserved chunk insufficient although normal has room | {"normal_scanned":false,"returned":"NULL"} | not proof real reserved underflow happened |
| PA87-M13 | reserved requested but no reserved chunk | {"normal_can_return_encoded_ptr":true} | no observed initialized reserved pointer in machine |
| PA87-M14 | normal list contains early reserved chunk | {"may_choose_reserved_object":true} | source does not guarantee this runtime list order |
| PA87-M15 | ordinary extension metadata fails | {"returned":"NULL","new_list_entry":false} | not an actual exhaustion experiment |
| PA87-M16 | new metadata success but data fails | {"safe_new_chunk_return":"NOT_CERTIFIED"} | stop at NULL void-pointer arithmetic; no real list insertion certified |
| PA87-M17 | new chunk repeatedly cannot fit request | {"if_kth_metadata_fails":"returns NULL at k","if_every_metadata_success":"no finite termination proven"} | no actual infinite-resource execution or machine infinite loop claimed |
| PA87-M18 | ordinary first matching chunk | {"new_chunk_created":false,"returned":"base_addr+0 encoding"} | not actual decoded pointer validity |
| PA87-M19 | normal workqueue pointer-size slot allocated from first chunk | {"decoded_CPU0":"0x103000","expected_backing_CPU0":"0x203000","difference_bytes":-1048576} | NULL base pointer arithmetic, mapped lvalue and actual template link address unverified |
| PA87-M20 | same encoded slot for CPU1 | {"difference_bytes":-1048576} | does not prove second unit is initialized or mapped |
| PA87-M21 | reserved chunk pointer encoding | {"decoded_CPU0":"0x200008","expected_Rdata_plus_off":"0x300008","difference_bytes":-1048576} | not a proof C cross-object pointer difference legal |
| PA87-M22 | late ordinary chunk pointer encoding | {"decoded_CPU0":"0x300008","expected_Qdata_plus_off":"0x400008","difference_bytes":-1048576} | actual Qdata existence/typed C pointer arithmetic not certified |
| PA87-M23 | special case linker template address zero | {"expected_backing":"0x203000","numeric_match":true} | even numeric agreement not a legal object write certificate |
| PA87-M24 | inner perCPU slot allocation NULL, outer ignores | {"outer_return":"wq nonnull conditional","cpu_pwq":"NULL"} | not an actual published broken workqueue observed |
| PA87-M25 | second CPU PWQ slab failure after first link | {"old_pwqs_node_link_removal_in_scoped_cleanup":false,"outer_may_publish_wq":true} | no runtime UAF/leak result and other cleanup paths not audited |
| PA87-M26 | WQ_UNBOUND set with successful cpu_pwq allocation | {"determinate_return_value":"NOT_CERTIFIED"} | indeterminate read C behavior; no immediate actual named events caller uses unbound |
| PA87-M27 | early BH named call | {"inner_success_return":0,"outer_global_list_append":"conditional"} | not proof actual boot reaches workqueue_init_early |
| PA87-M28 | outer workqueue kzalloc fails before perCPU call | {"outer_return":"NULL"} | no actual slab failure/recovery measured |

全部模型是按固定源码手推/独立小整数公式得到的条件分支样例，不是运行MyOS2源码、Python脚本、GCC、CMake、Graphviz、QEMU或任何实机验证。模型M04的容量差异不是实测非法内存写；模型M19–M23的地址也不是已建立合法C对象指针。所有表中正式源证据采用PA87-Exxx，并且未在说明文字中复写保留标签的原始起始字面。
