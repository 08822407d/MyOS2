---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-DIRECT-MAP-RANGE-82
record_type: direct_map_range_static_narrative
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
produced_by: GPT-6 (model self-report, backend unknown)
model_per_owner: GPT-6 + Extra (Owner selected)
date: 2026-11-13
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
lead_pin_short12: e8f950fa4ad8
execution_start_short12: 05fe580c12a3
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第82批：direct-map候选页大小、五段区间整理与调用记账

本批由原MYOS2-A-C06a／CHAT-EXEC-10在固定`time@a039d9803ade`对三个互相关联接合面作有界源码静态分析。没有D00；第81批已由C06判定`PASS_PENDING_LOCAL`，须合读R81-S01/S02/S03；79/80及其文档闭合事项不重开。这里只核页大小候选、临时范围、上层请求与返回类型，不证明真实CPUID值、全部页表层次、物理内存空闲、固件E820正确、已建立direct-map或CR3/TLB硬件效果。

## G01/A｜CPU候选位与取整跨度提升

全局`direct_gbpages=1`，`page_size_mask`是没有显式初值的静态`int`。所读静态初始化语义下mask初为0。`simple_probe_page_size_mask`先测试`boot_cpu_data.x86_capa_bits.PageSize_Ext`，真时`page_size_mask |= 1<<PG_LEVEL_2M`，假时清零`direct_gbpages`；之后必须同时有`direct_gbpages`与`Page_1GB`才OR`1<<PG_LEVEL_1G`，否则再清零开关（证据A01/A02/A03/A04/A05；事实键：gb_default,global_mask,probe_2m,probe_1g,probe_1g_else）。当前`pg_level`枚举从NONE=0、4K=1到2M=2、1G=3，故在声明整数模型中2M位为4、1G位为8；`PMD_SHIFT=21`和`PUD_SHIFT=30`提供2MiB、1GiB单位（证据A07/A08/A09/A10/A11；事实键：level_enum,pmd_shift,pud_shift,pmd_size,pud_size）。两个能力字段在本树`x86_cap_bits`中确实存在，但结构体声明及名字不说明实际机器何时/怎样填入了什么值（证据A12/A13；事实键：cpu_pse,cpu_1g）。

**OR不等于每次重新生成掩码。** 该函数读/OR全局mask，但在两个失败分支只清`direct_gbpages`，不清原已置的2M/1G位。因此隔离模型：初次两能力都真得mask12、开关1；若此后不复位mask而再次以PageSize_Ext为假调用，则开关0而mask仍12。这是重复调用的条件风险，不主张启动过程中确实执行了两次，也不把`page_size_string`显示的1G视为1G叶已经装入（证据A03/A04/A05/A27/A28；事实键：probe_2m,probe_1g,probe_1g_else,display_preference,display_4k）。

`split_mem_range`仅当`!after_bootmem`时，在**合并前**运行`adjust_range_page_size_mask`。对于每条缺2M位且全局允许2M的MR，把start向下、end向上按PMD_SIZE取整，再调用`memblock_is_region_memory`；真则只OR该MR的2M位。缺1G位且全局允许1G时，独立按PUD_SIZE做同样判断和OR（证据A26/A15/A16/A17/A18；事实键：promotion_phase,promote_2m_gate,promote_2m_write,promote_1g_gate,promote_1g_write）。这只更改候选位，不重写start/end，也没有在这个函数内发出实际页表写入。

包含查询首先用`memblock_search(&memblock.memory,base)`取得**包含base的单一记录**，再核该记录end覆盖被`memblock_cap_size`裁切后的查询end（证据A19/A20/A23；事实键：single_search,single_containment,cap_size）。在合法且未合并的两个相邻memory记录联合覆盖范围的隔离模型中，若第一条不足以包住取整end，则查询仍可能false；反之一个memory记录包住全部查询不意味着reserved为空、NOMAP允许或者实际free。查询与物理可用性必须分账。尤其`memblock_search`使用do-while，cnt0时读取记录的条件性风险仍属MR79旧界限，不因本批复用就算已关闭（证据A21/A22/A19；事实键：search_enter,search_access,single_search）。

`round_up/round_down`的掩码表达和`memblock_cap_size`自身裁切是不同操作；ulong高端取整回绕时不能因为后续布尔返回就认证一个合法的物理范围（证据A24/A25/A23；事实键：round_up,round_down,cap_size）。G01两张矩阵分别列能力/掩码及范围提升，有限样本DM82-M01–M09，PT81及旧MR79有关对象边界继续有效。

## G02/B｜五段MR拆分、容量和同掩码合并

`struct map_range`保存**字节**`start/end`和一个`uint page_size_mask`；临时数组容量宏`NR_RANGE_MR=5`。而`split_mem_range`的参数是字节`start/end`，先计算`limit_pfn=PFN_DOWN(end)`和`pfn=PFN_DOWN(start)`，在PFN单位拆成至多五个候选段，再由`save_mr`左移`PAGE_SHIFT`恢复为字节区间（证据B01/B02/B03/B07/B08/B05/B22/B23；事实键：capacity,range_fields,save_args,limit_floor,head_bounds,save_count,pfn_down,shift_4k）。因此输入不是页对齐时并不保持原始字节覆盖：例如`[4097,8193)`在独立整数模型里保存`[4096,8192)`，向低端扩1字节并丢失高端最后1字节；`[4096,8191)`虽有正长度，却可能因为两个PFN_DOWN端点相同而不生成MR。不能将这些局部数值样本说成已真实映射了某段内存。

五段是**按源码固定顺序处理**，不是五种保证出现的输出：

- 第一段：从floor(start)至下一PMD对齐边界，若超过limit则截到limit，mask0（证据B08/B09；事实键：head_bounds,head_save）。
- 第二段：按2MiB对齐，到下一1GiB界或limit的2MiB下界，mask为global mask与2M位交集（证据B10/B11；事实键：two_bounds,two_save）。
- 第三段：PUD对齐的1GiB中段，mask为global的2M/1G候选位交集，**未必真的含1G位**（证据B12/B13；事实键：one_bounds,one_save）。
- 第四段：1GiB之后尚能按PMD对齐的2MiB尾段，mask为2M候选位（证据B14/B15；事实键：tail2_bounds,tail2_save）。
- 第五段：余下的4KiB整页尾段，mask0（证据B16；事实键：tail4_save）。

每个非空段才调用`save_mr`；它仅在`start_pfn<end_pfn`时检查`nr_range>=NR_RANGE_MR`并调用`panic`，随后写MR并自增计数（证据B04/B05；事实键：save_guard,save_count）。从`init_memory_mapping`的真实调用`split(mr,0,start,end)`看，五段各最多产生一项，正常输入时最多五个数组元素；这不是允许任意外部调用者传已满的nr_range，也不能只看到panic调用就假定该调用绝不返回。面对超容量或非法源指针，停止正常状态推导。

**合并发生在提升之后。** 若`mr[i].end==mr[i+1].start`且两项mask相等，源码保存旧start，以`memmove`把尾部MR左移，然后恢复start、`i--`、`nr_range--`，for循环随后继续检查当前位置（证据A26/B17/B18/B19；事实键：promotion_phase,merge_guard,merge_move,merge_count）。它只合并**临时元数据记录**，不撤销或重写任何已建立的页表。整数模型`[4096, 2GiB+2MiB+4096)`且mask12，可得到依次为0/4/12/4/0的五段，正常情况下仍有五段；在相同端点但global mask0的模型中，五段掩码全0、相邻合并后可成为一段。MR日志经`page_size_string`展示优先1G再2M，并不是最后叶页尺寸证明（证据B17/B19/A27/B20；事实键：merge_guard,merge_count,display_preference,print_only）。

对应`partition_coverage_matrix`、`range_capacity_matrix`与`merge_transition_matrix`，数值样本DM82-M10–M21。非有序、反向、空范围和大数回绕必须在首次未证算术/数组门前停，不把打印的end-1或表格段数作为映射成功证据。

## G03/C｜逐段请求、累计量、真实上层调用顺序

`init_memory_mapping(start,end,prot)`先创建五项局部MR及`ret=0`，清零MR后以nr0调用拆分；逐MR调用`kernel_physical_mapping_init(mr[i].start,mr[i].end,mr[i].page_size_mask,prot)`，结果每次**覆盖**ret，最后只返回`ret>>PAGE_SHIFT`（证据C01/C02/C03/C04；事实键：local_state,call_split,each_request,last_return）。这个宏实际展开为`__kernel_physical_mapping_init(...,true)`，真实callee签名接收字节起终点、候选mask、prot与init；它在内部有条件更新`paddr_last`后返回，而不是由调用者检查所有分段是否形成了叶表项（证据C05/C06/C07/C08；事实键：mapping_macro,lower_signature,lower_initial,lower_return）。因此最后一个返回PFN不是全体区间完成的事务证明：若无保存段，for跳过而ret保持0；若有多段，每次返回覆盖前值，不能把最后ret当总和或最大端点的已核结果。注释掉的`add_pfn_range_mapped`不是这段代码的活动登记调用（证据C04；事实键：last_return）。

上层`init_range_memory_mapping(r_start,r_end)`有自己的`mapped_ram_size=0`，采用`for_each_mem_pfn_range`迭代`memblock.memory`可组成整页的区间；实际`__next_mem_pfn_range`用PFN_UP(base)和PFN_DOWN(base+size)输出，结束将idx置-1（证据C09/C17/C19/C20/C21；事实键：range_initial,iterator_macro,iterator_scan,iterator_stop,iterator_publish）。这不是memory减reserved的free列表，也不是E820每一种类型都被加入memory的事实。每条迭代记录用`PFN_PHYS`转物理字节后，分别`clamp_val(...,r_start,r_end)`，若start>=end就跳过；否则调用`init_memory_mapping(start,end,PAGE_KERNEL)`，**不使用**该调用的返回值而直接`mapped_ram_size += end-start`，函数最后返回这个请求字节累计（证据C10/C14/C15/C16/C11/C12；事实键：range_clamp,pfn_phys,clamp_val,clamp_t,range_account,range_return）。例如合法且分离的两条memory`[0,8192)`、`[16384,24576)`，对请求`[0,24576)`累计的是16384个**请求**字节而不是24576的跨度，更不保证真实映射16384字节；下层即使返回部分端点，上层仍按请求长度加账，只要callee正常返回。

顶层`init_mem_mapping`先调用`probe_page_size_mask`，将`max_pfn`左移形成`end`，随后**无条件发起**`init_memory_mapping(0,ISA_END_ADDRESS,PAGE_KERNEL)`，本树ISA上界为1MiB（证据C23/C24/C31/C35；事实键：top_probe,top_isa,isa_end,max_pfn_upstream）。后面`if(memblock_bottom_up())`，其真实访问器读取memblock.bottom_up，所读初始化值为true；若该条件成立则`kernel_end=__pa_symbol(_end)`，**先**经`memory_map_bottom_up`请求`[kernel_end,end)`，**后**请求`[ISA_END_ADDRESS,kernel_end)`。宏`memory_map_bottom_up`实际别名为`init_range_memory_mapping`；else中的top-down调用在所读源码是注释（证据C25/C29/C30/C32/C26/C13；事实键：direction_gate,bottomup_accessor,bottomup_initial,pa_symbol,two_calls,bottomup_alias）。即使静态默认bottom_up=true，也不认证没有其它写者；`kernel_end`真实链接值、范围正序与被枚举memory的合法性均须另证。

无论是否进入这两次请求，之后还调用`myos_memory_map`（当前读取中包含VBE和IOAPIC设备callee）、在条件下更新`max_low_pfn`，最后才调用`load_cr3(swapper_pg_dir)`与`__flush_tlb_all`（证据C27/C33/C34/C28；事实键：devices_and_low,device_vbe,device_apic,load_root）。这是**函数内条件性顺序**，不是实际设备初始化成功、全部direct-map可写或硬件TLB生效。对于`kernel_end>end`或`kernel_end<ISA_END_ADDRESS`，两个上下界可能反向，各自必须停在首个不成立的范围/算术门；不能把`clamp_val`当作保证反向输入安全的总证明。第81批的PT81缓冲/映射界限、第79批memblock合法性与第80批启动内存表输入仍持续限定此路径。

G03交付`mapping_request_matrix`、`return_and_accounting_matrix`、`boot_order_and_boundary_matrix`和`cross_layer_boundary_matrix`。条件样本DM82-M22–M30分别区分没有MR、最后一次返回、memory洞、空区、方向false与反向请求。任何下层callee不返回或触及未证页表/物理地址门时，后续统计、根切换及真实映射结论都不再声称可达。

## 成果、证据级别及交接

本批新短锚87条（A28/B24/C35）来自13份固定time源码。每个事实键绑定`path::symbol`、证据ID、逐字引文及必须在引文内的字面，facts中57个矩阵行与30个隔离模型均按这个事实键导航；不能仅凭编号存在判断引用成立。本批新增条件候选DM82-F01–F08共8项、缺口DM82-U01–U10共10项，均needs_evidence，非已复现故障；DOT46节点/63类型边/0悬空，为静态关系图，未运行Graphviz。矩阵、样本和引文相互支持但重叠，不合计作为多次独立实验。

完整本地P9、内核脚本、宏预处理、编译、链接、硬件CPUID、QEMU/实机、MMU/TLB及实际物理映射均为**NOT_RUN**。本执行者只在原PR17分支新增第82批固定六件与本人一次正式交付评论，执行后须按六件固定/活动全文、四direct refs和唯一评论回读确认。通过PR17交回原MYOS2-A-C06 Pro独立验收，`READY_FOR_MAINLINE_REVIEW`／`mainline_acceptance=PENDING`，之后STOP_WRITING；不自签82、不启动83，不修改旧结果或内核。Owner无需技术手工操作。
