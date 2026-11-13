---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-EARLY-PGTABLE-STORAGE-81
record_type: bounded_static_analysis_narrative
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
produced_by: GPT-6 (model self-report, backend unknown)
model_per_owner: GPT-6 + Extra (Owner-directed)
date: 2026-11-13
base_snapshot: time
source_pin_short12: a039d9803ade
lead_pin_short12: df960bf5c5e9
execution_start_short12: 0e9e9e89c4be
read_channel: connector
runtime_evidence: NOT_RUN
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---

# 第81批｜早期页表缓冲存储、提前映射和分配来源切换

这是MYOS2-A-C06a/CHAT-EXEC-10在原PR17上的第81批有界静态研究，源码仅来自固定 `time@a039d9803ade`。由现C06 Pro独立验收，最高`PASS_PENDING_LOCAL`。无D00：79文档R79-01/02已由C06独立闭合、79组合PASS_PENDING_LOCAL，80已PASS_PENDING_LOCAL但须合读R80-S01/S02/S03；其历史return和原件均冻结。旧PA77、PB78、MR79、BM80及AE75/SL76/MC/LB/GAP、MR-02所有身份不改，不因本批增读上游自动结案。99条新锚(A32/B37/C30)独立于80的92条，12张矩阵78行、32个隔离模型与同一源码存在重叠，不相加声称更多独立见证。

## G01/A｜页表缓冲区的brk来源、字节容量与地址域

**对象与容量。** 固定`init.c`定义三个`__initdata`游标`pgt_buf_start/end/top`，`EARLY_PGTBUF_CHUNK_SIZE=PMD_SIZE`、`EARLY_PGTBUF_MAX_CHUNKS=4UL`、`EARLY_PGTBUF_SIZE=4*PMD_SIZE`（A01–A06）。对应最近宏定义`PMD_SHIFT=21`，在64位无溢出的声明整数模型下`PMD_SIZE=2MiB`、`EARLY_PGTBUF_SIZE=8MiB`。源里确有`RESERVE_BRK(early_pgt_alloc,EARLY_PGTBUF_SIZE+PMD_SIZE)`全局调用，文本上声称静态预留10MiB，`RESERVE_BRK`定义实际把对象置于`.bss..brk`节（A07）；这不是`extend_brk`实际成功的证明。

**重要的链接节接合点。** 固定`kernel.lds`把`*(.bss* COMMON)`收进`.bss`，而独立`.brk`段先作`. += 64*1024`，只显式匹配`*(.brk*)`，定义`__brk_base/__brk_limit`（A08–A10）。从这些**文本模式**看，`.bss..brk`应匹配`.bss*`而非`.brk*`，所以不能因为调用`RESERVE_BRK`就认定运行时`__brk_limit`自动获得10MiB。这是`PT81-F01`条件候选，而非已经观察到链接失败：真实预处理选项、链接输入、生成映像和link map在本批未读取。即使目标段含其它未见输入，也不能用固定声明字面推断完整布局；不能将`.bss`和`.brk`混成一个地址域。

**运行时申请。** `early_alloc_pgt_buf`以`extend_brk(8MiB,PMD_SIZE)`取回指针、再经`__pa`得`phys_addr_t base`（A16）。`extend_brk`先算`mask=align-1`，对齐`_brk_end`，做`_brk_end+size`与`__brk_limit`比较，然后取`ret`、推进end并`memset(ret,0,size)`（A13–A15）。8MiB申请与2MiB对齐余量是不同数量，静态10MiB的名义表达也不认证运行时`.brk`上界。无实际链接符号值、存储可写性、对齐/偏移合法性时，后续指针与PFN只能进行隔离模型；BUG_ON语句存在不是机器必然停止的证据。

**地址身份账。** `base`是经过`__pa`转换的**物理字节地址**；`pgt_buf_start=base>>PAGE_SHIFT`和`pgt_buf_end=start+2`、`top=start+(tables>>PAGE_SHIFT)`都是**物理页号**。`early_pgtbuf_pud=(void *)base+__START_KERNEL_map`和`early_pgtbuf_pmd=early_pgtbuf_pud+PAGE_SIZE`是**内核镜像别名**，并非`__va(base)`的direct-map别名；`__phys_addr_nodebug`与`__va`是不同地址转换式（A16–A17、A24–A31）。在已合法且物理基址4KiB对齐的隔离模型下8MiB为2048页，前两页是PUD/PMD，活动游标初始`start+2`，剩余2046页或8,380,416字节。物理2MiB对齐须由`__pa`、phys_base和真实布局共同证明，不能仅凭虚拟地址按PMD_SIZE对齐推出。

**发布次序。** `early_alloc_pgt_buf`按源序发布start、PUD别名、PMD别名、end、top；`setup_arch`先调用此函数，随后`reserve_brk`，且保留请求的返回值未消费、最终清零`_brk_start`（A17、A20–A23）。前一个函数如果在中途形成非法地址或未正常返回，不能说这些状态全已成功写入；清零_brk_start也不证明memblock成功保留。G01三矩阵见facts；条件模型PT81-M01–M08。

## G02/B｜提前direct-map的两级子页、槽位范围和发布先后

`myos_early_map_pgt_buf_into_directmap`从buffer页号产生`phys_start/end`，然后形成`va=__va(phys_start)`、`vend=__va(phys_end-1)+1`（B01/B02）。`__va`是地址构造，不是直接映射已经可访问/可写的证明；反向、零端点、溢出或物理与虚拟算术不能成立时在首次未证门停止。源码中跨PGD、跨PUD检查相关的两个`if/panic`处于注释，**不是活跃拒绝条件**（B03）。

**对象与真实调用。** 初始根槽分别取`early_top_pgt[pgd_index(va)]`和`init_top_pgt[pgd_index(va)]`，将early根强转成p4d指针，先清零缓冲PUD页，然后通过`p4d_populate`宏别名进入`fill_pud_to_p4d_ent`，以`__pa(pud_p)|_PAGE_TABLE`构造并写early父槽；随后仅选`pud_base[pud_index(va)]`，清零缓冲PMD页，通过`pud_populate`写入该**一个PUD子表入口**（B04/B05、B10–B21）。`init_top_pgt`有单独数组定义，`swapper_pg_dir`是其宏别名；`early_top_pgt`在本包读到接口消费，未用外部引导汇编证实当时真实根和CR3状态。这些写入的前提是两个内核镜像别名在该时点可写且物理转换正确，不是函数名或注释能担保。

**叶项及容量不变量。** `nr_pmd=(phys_end-phys_start)/PMD_SIZE`；循环每轮`this_va=va+i*PMD_SIZE`、`pmd_slot=pmd_index(this_va)`、`pmd=&pmd_base[pmd_slot]`、`val=pa|pgprot_val(PAGE_KERNEL_LARGE)`，经`__pmd`和`set_pmd`写一条PMD大页叶（B06/B07、B22–B32）。`PAGE_KERNEL_LARGE`从含PSE的位表达并经过`__pgprot_mask`；有效掩码/真实硬件效果不由宏名保证。所选宏给出PGD位移39、PUD位移30、PMD位移21、每PUD/PMD页512入口。静态8MiB、物理/虚拟2MiB对齐且不溢出的正常算术例为4个PMD叶。

**1GiB/512GiB边界不由8MiB容量保证。** 对隔离物理起点`0x3fc00000`（1GiB-4MiB）及8MiB范围，四次PMD索引是`510,511,0,1`，后两次本应属于下一PUD入口；当前函数却只配置起始VA的一个PUD父项，写`pmd_base[0/1]`并不能单凭这个初始父项映射下一1GiB范围。如果跨更高的512GiB PGD边界，初始选中的一个根槽同样不足。见PT81-F03、矩阵B-S04/B-S05/B-S06及模型M10/M11；这只是前提成立时的逻辑覆盖风险，实际buffer物理位置与有效映射尚无运行证据。空、反向、极端端点也需在物理差值与`vend`形成前停推；不能把后续叶循环当实际执行。

**刷新与根发布不能混淆。** 源码先把叶写到early根结构，接着调用`__flush_tlb_all()`，最后才`*init_pgd=*early_pgd`，只复制**一个索引的根表项**（B08）。所读`__flush_tlb_all`函数体调用`flush_tlb_local`，并非硬件多核生效认证。后续`init_mem_mapping`另有`load_cr3(swapper_pg_dir)`及再次刷新（B35–B37）；这与当前函数内的刷新请求和根条目拷贝在时间上分开。需要当前活动CR3/页表映射证据才能把根状态变更提升为“立刻所有CPU可访问”；本包不读完整汇编或硬件手册。G02四矩阵与M09–M17细化槽位、父项、循环和停止点，图的CALL、CALLER_SEQUENCE与数据边不得当完整CFG。

## G03/C｜alloc_low_pages的晚期、缓冲区及memblock三源

`alloc_low_page()`是`alloc_low_pages(1)`的直接包装（C11），但函数本身接受`uint num`，不能未经完整调用者审计宣布所有num恒为1。`after_bootmem`是独立全局标志，`mem_init`中先写1后调用`memblock_free_all`（C01/C09/C10）；标志发布不等于伙伴页已成功归还或高阶分配一定可用，延续PB78/PA77限定。

**晚期分支（after_bootmem非0）。** 首先计算`get_order((ulong)num<<PAGE_SHIFT)`，随即`return (void*)__get_free_pages(GFP_ATOMIC|__GFP_ZERO,order)`（C02）。`get_order`对于零大小在最近定义文档中注明未定义；常量和动态大小分支不同（C12–C14），不能把num0当普通0页申请。合法整数样本num1得到order0，num3对应12288字节、order2，后者的块容量是4页而不是3页。所读`__get_free_pages`内部调用`alloc_pages`，失败返回0、成功转成虚拟地址（C15），这里**直接向调用者传播0**，没有早期的panic/while。

`__GFP_ZERO`是标志请求，不是上述早期逐页清零循环。其页层消费还受R77-S01页层过滤标志与slab对象层原标志不同的历史限制。当前最近`buddy.c::post_alloc_hook`的ZERO条件代码在`i`循环内调用`clear_page((void*)page_to_virt(page)+i)`（C20），文本上`+i`是字节步进而非`PAGE_SIZE*i`。在真实页层ZERO条件确实到达、order>0且GNU void*算术选中时，**不能由这个循环推出每张页都分别清零**；更不能仅凭晚期`__GFP_ZERO`标志声称机器结果已全零。见PT81-F07、R77-S01与证据缺口U08；不是独立已复现缺陷。

**早期缓冲区分支（after_bootmem=0，容量充足）。** 先求`pgt_buf_end+num`并与`pgt_buf_top`比较；满足`<=top`时取旧end为pfn，随后`pgt_buf_end+=num`，再使用`int i`循环，对每张`__va((pfn+i)<<PAGE_SHIFT)`调用`clear_page`，最后返回第一张的VA（C03/C06–C08/C19）。这是真正尝试逐页4096字节的路径，前提仍是每次VA有效、可写、未与数据/保留区冲突；游标提前写入后没有本函数内的失败回滚。num=0在缓冲区剩余时游标不动、循环0次却返回cursor地址，并不能称“分配了0张可用页”。恰好剩余页数可用，超过一页则整个请求转回退；无先行整数溢出证明时`end+num`还可能回绕，使容量门判定失真。极大num与有符号int循环索引的关系要分开条件化，不能因为num为uint就直接认证i++合法。

**早期memblock回退（buffer不足）。** 仅在`min_pfn_mapped<max_pfn_mapped`时请求`memblock_phys_alloc_range(PAGE_SIZE*num,PAGE_SIZE,min_pfn_mapped<<PAGE_SHIFT,max_pfn_mapped<<PAGE_SHIFT)`（C04）。其真实callee转到`memblock_alloc_range`，后者还包含PAGE_SIZE下界、finder和reserve门（C21–C25）；ret为物理地址，0触发源码中的`panic(...);while(1)`，而非“memblock已成功分配0号页”。非0才转PFN并进入同一个早期清零循环，**缓冲区游标在回退分支并不增加**。`min_pfn_mapped`为init.c静态对象、`max_pfn_mapped`为setup.c独立全局，所读`init_memory_mapping`里的`add_pfn_range_mapped`行是注释（C26–C28）；这不证明全树没有其它生产者，更不证明[min,max)真实已直接映射。若窗口无效导致ret仍为0，panic分支是条件推导，不是实测启动失败。

**边界合账。** after_bootmem先置1不证明memblock_free_all已完成；buffer足够不证明其镜像/直接映射已生效；fallback拿到非0物理地址不证明该地址已能用`__va`清零；晚期页分配器要求阶数、位标志与页层hook另行成立。G03五矩阵（含跨A/B/C表）及M18–M32逐一分账：num、get_order阶数、返回地址类型、实际清零页数、失败出口、PFN生产者和首次非法停点；不扩展到buddy/slab全链、NUMA/锁或完整虚拟内存系统。

## 结果、证据等级与交回

新增短锚99（A32/B37/C30），本批技术矩阵12个/78行，32组隔离有限模型（A8/B9/C15），条件候选PT81-F01–F08八项、缺口PT81-U01–U10十项，全部needs_evidence而非运行实证；图63节点、90条带语义边，未运行Graphviz。软范围内实际取源20份含短锚，另读sections.h与highmem_macro.h两份最近定义，共22份，均来自`time@a039d9803ade`。`facts`为逐项矩阵/模型和旧限定，`evidence`为真实定义内1–5行短锚，`deferred`保留后续证据义务。

完整本地P9、宏预处理、编译、链接/真实映像、QEMU、硬件TLB、真正内核/页分配器运行均为NOT_RUN。对链接节匹配和C宏语义的文本推导，不声称实际预处理/链接或编译ABI有实测证据。只在第81批原目录六件新增、原PR17本人一次正式评论，六件全部远端回读及唯一性后STOP_WRITING；交给现MYOS2-A-C06（Pro）独立审核，不自签81通过、不启动82、不要求Owner逐项审核源码问题。
