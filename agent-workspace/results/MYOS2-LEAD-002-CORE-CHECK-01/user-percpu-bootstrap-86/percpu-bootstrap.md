---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PERCPU-BOOTSTRAP-86
record_type: bounded_static_percpu_bootstrap_analysis
produced_by: GPT-6 (visible model identity; backend not attestable)
model_per_owner: GPT-6 + Extra
date: 2026-11-14
base_snapshot: time
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW_AFTER_PUBLICATION
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions: [PC86-U01, PC86-U02, PC86-U03, PC86-U04, PC86-U05, PC86-U06, PC86-U07, PC86-U08, PC86-U09]
---

# 第86批：per-CPU首块、单位偏移和启动CPU GDT/GS切换

本批仅取固定time@a039d9803ade的源码；主线固定发布d2edb61330d9、执行原父2489ea8e6963。身份延续MYOS2-LEAD-002-CHAT-EXEC-11，不轮换执行者。第84批R84-01已经C06独立闭合（84组合PASS_PENDING_LOCAL）；第85批按R85-S01/S02/S03限定PASS_PENDING_LOCAL。本批不需要D00、不倒改历史原件或旧评论。

证据细节由evidence.md的77项单一定义短锚支撑；facts-and-boundaries.yaml列出25个实际读取的固定time源码入口（24个含新短锚，另1份仅作为extern声明上下文而不作定义锚）、10组69行矩阵、29个条件模型及明确的新读/同会话复用/未读账；deferred-findings.yaml保留PC86-F01..F09九项条件候选和PC86-U01..U09九项证据缺口。下文只陈述由所读源码与声明前提推出的结论；不是构建、链接、真实物理CPU或者内核运行记录。

## G01/A｜模板、空间、首块分配及复制

链接脚本的三个符号不能混用：L=__per_cpu_load在perCPU输出节前得到赋值，T=__per_cpu_start在.data..percpu输出节起始段定义，E=__per_cpu_end在输出节末段赋值。该节有page_aligned、hot、read_mostly、普通percpu等输入节模式和对齐语句。当前没有ELF/link map，所以不能断定L与T绝对相等，也不能说这些输入节在最终二进制的真实位置或可访问性已经测量。C源码以E-T计算static_size，却以&L作为模板memcpy源，这是两个相关但独立的地址表达；如果链接实际使L与T不同，复制源区间与欲复制的模板范围可能不一致（PC86-U01）。

令S=(size_t)(E-T)为模板字节长度；R=2*PAGE_SIZE，Y=0，sum=S+R+Y，C=roundup_pow_of_two(sum)。仅在PAGE_SIZE=4096、差值可表示、size_t加法及roundup移位范围有效时，R=8192并可把C理解为不小于sum的2的整数次幂。__roundup_pow_of_two使用ulong移位，不能把任意大的数用无限精度数学悄悄圆整；还需要真实编译ABI与链接范围。N=nr_cpu_ids。early_pcpu_create_chunk每调用一次，先申请sizeof(chunk)且对齐sizeof(chunk)的元数据，后请求N*C字节、按C对齐的DMA32数据，若数据返回非空，则myos_memblock_alloc_DMA32内有条件memset零填充。函数在申请data后立即执行链表加入，不在本函数内再次检查data是否为NULL。

simple_pcpu_setup_first_chunk先调用该early helper取得pcpu_first_chunk，然后未经本地判空便取first->data成为pcpu_base_addr。随后将first->free_bytes设为C-sum、free_offset设为sum、base_addr设为0。这两个free字段实为int，而size_sum和chunk_size均为size_t，因此只有在值属于int可表示范围时才能把值当作无损字节计数；大值/实现相关转换不可直接称为已证故障。

接着只出现一次memcpy(first->data,&__per_cpu_load,S)，该代码没有围绕CPU编号循环的模板复制。之后以myos_memblock_alloc_normal(N*sizeof(void*),8)申请offset数组并以int i从0到N-1写入i*S；最后再次调用early_pcpu_create_chunk获得另一块元数据与N*C数据作为pcpu_reserved_chunk，并把reserved->base_addr设为reserved->data-(ulong)pcpu_base_addr。该reserved是另行申请的chunk，不可凭名称认作first chunk内的简单切片。两个early helper调用加一次normal数组申请共五项底层分配请求，只在所有路径到达时成立，申请或复制成功没有实验记录。

**失败前缀独立处理：**首块元数据NULL使调用者下一个first->data解引用缺乏合法对象；首块data NULL虽可能已有链表发布，但S>0时目标memcpy无法证成；offset数组NULL而N>0会在首次((ulong*)pcpu_unit_offsets)[0]写入处停止正常证明；reserved元数据NULL会使后续reserved->base_addr不可证明。源码中的是否判空、实际返回与硬件异常是不同命题。并没有注入失败或运行memblock测试。

**有界算例：**S=4096、R=8192、Y=0得sum=12288，C=16384。N=3时首块和保留块分别请求49152字节，offset数组在LP64前提下请求24字节；U[0..2]=[0,4096,8192]，而每个被申请数据对象名义跨度是N*C。显式复制次数为1，复制4096字节。该计算不证明3个CPU真实初始化，也不把全部49152字节称为已填入模板。更细的S=0/1/4097/8192/8193及0CPU、各级失败和大值转换见模型M01–M12。

## G02/B｜CPU编号、offset发布与per_cpu地址公式

nr_cpu_ids在pre_init.c中声明为unsigned，get_smpinfo将收到的smp_info->core_num赋给它，CPU_TOPO类型的tag case调用get_smpinfo。这里只追必要生产/消费接口；既有第80批R80-S01/S02/S03对解析长度、源存储、索引和执行限制继续适用，不声称完整tag验证或所有nr_cpu_ids写者均已审计。

当前for_each_possible_cpu(cpu)宏直接代理for_each_cpu，后者是从0到nr_cpu_ids-1的连续整数循环，不是内核cpumask中逐位过滤的通用惯例。因此一个假设的稀疏拓扑不能直接当作本循环跳过空洞的证据；也不证明本机必然有稀疏编号。__per_cpu_offset是NR_CPUS个unsigned long的静态数组，NR_CPUS来自CONFIG_NR_CPUS，已读公共CFLAGS文本设-D CONFIG_NR_CPUS=256（原文为无空格的宏定义串）；这仍是源文本层的编译候选，真实配置/其他选项/运行输入不因字符串而被认证为256。N>实际NR_CPUS时，经过边界索引的写入存在条件性数组容量风险，原输入是否可达未知。

设B=(ulong)pcpu_base_addr，T=(ulong)__per_cpu_start，V为某perCPU变量链接地址，则在无回绕、对应内存合法、ABI及数组容量相符的条件下：U_i=i*S（字节），D=B-T，O_i=D+U_i，内存中__per_cpu_offset[i]=O_i，per_cpu(var,i)采用V+O_i定位CPU i的实例。循环还依次对per_cpu(this_cpu_off,i)写入O_i，对per_cpu(pcpu_hot.cpu_number,i)写入i。由此offset数组、perCPU存储中的this_cpu_off、pcpu_hot字段以及GS基址寄存器是不同的四类状态，不是同一个对象。

另一条访问公式要单列：this_cpu_ptr使用arch_raw_cpu_ptr；raw_cpu_read_long(this_cpu_off)宏在所读定义中**直接返回rdgsbase()的值**而未消费参数this_cpu_off，因此get_current()取*this_cpu_ptr(&pcpu_hot.current_task)实际依赖当前处理器可读的GS基址与该变量链接地址。只看到per_cpu(this_cpu_off,i)=O_i的内存赋值不能推导rdgsbase将返回O_i；只有CPU0后续wrmsrl请求，仍未验证真实硬件效果。BOOT_PERCPU_OFFSET则指向__per_cpu_load所代表的链接地址，不能无条件与B-T这个运行delta等同。

在S=4096,N=3的模型中，N*C=49152字节而单位步长仅4096字节；如果后续消费者试图在每单位的[S,C)尾区使用数据，则不同单位的C字节名义窗口会重叠，这是一个**带实际使用范围前提的条件风险**，尚未证明保留区分配器确实进行了冲突写入。相反静态模板各S字节单位窗口在该纯整数模型下可恰好相邻。memblock成功零填充、首份非零初始化模板仅复制一次、其余CPU的pcpu_hot.current_task/GDT初始化需求之间亦存在证据缺口：loop显式修改cpu_number与this_cpu_off，不等于复制了所有非零初始成员；不能据此把AP运行崩溃写为已复现。

**具体地址例：**假设T=L=0x100000、B=0x200000、V=T+0x100、S=0x1000、N=2且无回绕，则D=0x100000，O0=0x100000、O1=0x101000，对应per_cpu指针为0x200100与0x201100。它是隔离的合法数学条件值，不等于实际链接、CPU寄存器或地址映射。若L与T不相等，BOOT_PERCPU_OFFSET=L与D还可能不同；模型M22特别隔离该前提。

## G03/C｜启动CPU的GDT/GS交接

真实上层调用者在start_kernel中先调用setup_arch(NULL)，随后调用setup_per_cpu_areas；前者的实际成功返回、84中页表切换和有效内存访问须独立满足。后者先调用simple_pcpu_setup_first_chunk，再计算delta并进入连续CPU循环，执行O_i、this_cpu_off实例及cpu_number实例的写入。只有该循环实际遇到cpu==0才进入CPU0局部分支；nr_cpu_ids=0则源码循环没有这次切换，不能把函数名说成无条件完成启动CPU初始化。

CPU0分支先调用load_direct_gdt(cpu)。该函数形成GDT描述符，address取get_cpu_gdt_rw(cpu)，size取GDT_SIZE-1；源宏GDT_ENTRIES=16且GDT_SIZE=16*8，因此在该宏模型下提交的长度值为127。get_cpu_gdt_rw从per_cpu(gdt_page,cpu).gdt返回指针，依赖前述O_i与gdt_page模板是否已经具备可读内容。load_gdt包装含lgdt汇编请求，其后同一调用者又请求refresh_segment_registers；后者具有SS与lretq路径。无CPU实证不能认证LGDT成功、描述符有效或段寄存器完成预期变化。DEBUG和ARCH_DESC_DEFINATION对内联/外部定义的条件仍须以真实配置为前提。

CPU0分支在load_direct_gdt返回后才调用wrmsrl(MSR_GS_BASE,per_cpu_offset(cpu))。源码wrmsrl把u64值拆成低/高32位转交__wrmsr，__wrmsr中含wrmsr指令文本；但编译/硬件执行仍未进行。更重要的是段寄存器刷新的源码不能凭周围“无需写gs”的注释推导中间操作不改变相关状态：需要隔离返回前提与真实GS_BASE状态。后续get_current依赖rdgsbase；该指令可用性的硬件/CPU特性门未在本包取得运行或官方硬件手册认证，故仅列PC86-U07，不宣称特定故障码或实机错误。

head_64.S共同secondary_startup_64范围内可见__per_cpu_offset(,ecx,8)、current_task(%rdx)和TASK_threadsp(%rax)等早期汇编消费者，但其先后路径和CPU编号来源并未在本批重新审计为完整AP启动链。独立的perCPU变量current_task与pcpu_hot.current_task是不同的存储入口，汇编读取前者不能直接充当C get_current读取后者的同一状态。代码定义、链接地址、先行callee正常返回、GDT装载请求、GS基址写请求、CPU寄存器真实值和后续current读取各自独立。

## 保留结论、延期问题及交回

新PC86-F01..F09仅为需要进一步证据的条件性源码问题，PC86-U01..U09为非缺陷结论的未证门槛。多个失败/容量/初始化风险有共同前提，不应把九个F和九个U相加为18个已确认独立bug。之前ER84/BB85/PT81/DM82/TI83及更早问题均保留原qualified_id和主线限定，不擅自闭合。R85-S01正确的双空格解析结论与S02/S03的工具/历史来源界限不再被旧作者错误说法覆盖。本批全部索引见facts，结构图须按图例区分真实C调用、同一调用者语句序、数据/地址关系与未证假设；图不是完整控制流图。

本包没有实施CMake、编译、预处理、链接、运行、仓库Python脚本、Graphviz、QEMU、实机、多核实验、硬件寄存器读取、内核或旧成果修改、补丁或额外模型/API。所有纯整数模型仅检查输入前提下表达式输出；没有测试实际C函数，完整本地P9保持NOT_RUN。发布后仅由原PR17交回C06 Pro独立审核86，不自行签PASS、不启动87。


## 十组可核矩阵（九主题矩阵及A/B/C接合表）

### template_and_storage_matrix（8行）

| ID | 对象 | 条件结论 | 证据缺口 | 源锚 |
|---|---|---|---|---|
| PC86-A-T01 | template input | three link symbols are textually distinct; section has alignment/input rules | actual link map and whether load/start addresses coincide | PC86-E001, PC86-E002, PC86-E003, PC86-E004 |
| PC86-A-T02 | static_size | template length is S bytes; source pointer still &__per_cpu_load | linker extent and legal pointer difference not observed | PC86-E016, PC86-E001, PC86-E004 |
| PC86-A-T03 | size_sum | under PAGE_SIZE=4096 and no overflow reserved=8192 bytes | actual configuration/C arithmetic overflow | PC86-E016, PC86-E017, PC86-E029, PC86-E030 |
| PC86-A-T04 | chunk_size | C is a rounded chunk-size value separate from S and sum | size_sum valid range and any shift-width invalid cases | PC86-E017, PC86-E026, PC86-E027, PC86-E028 |
| PC86-A-T05 | memcpy source/dest | only one explicit S-byte copy; not a per-cpu loop | actual source availability, other writers, successful memory | PC86-E020, PC86-E001, PC86-E002 |
| PC86-A-T06 | hot/GDT template | the linked template can contain initialized nonzero bytes | actual emitted .data..percpu ELF and effective GDT configuration | PC86-E055, PC86-E070, PC86-E003 |
| PC86-A-T07 | fields | C size_t metadata is stored into narrower int members | actual ABI sizes and conversion results for concrete linked size | PC86-E011, PC86-E012, PC86-E019 |
| PC86-A-T08 | N*chunk_size vs unit offsets | allocation span, logical unit step and template extent are three distinct quantities | no ABI/linker N/S value, address disjointness not observed | PC86-E014, PC86-E021 |

### first_chunk_allocation_and_publication_matrix（8行）

| ID | 对象 | 条件结论 | 证据缺口 | 源锚 |
|---|---|---|---|---|
| PC86-A-P01 | first call | first metadata has size/alignment sizeof struct; NULL explicitly tested | memblock success & alignment constraints unknown | PC86-E013, PC86-E024 |
| PC86-A-P02 | within each early helper | backing memory requested for N*C and aligned to C; pointer not locally null-checked | size product overflow, zero size and actual allocator return | PC86-E014, PC86-E015, PC86-E024 |
| PC86-A-P03 | early helper | list insertion appears without data pointer success check | list helper implementation/valid pointer not fully audited | PC86-E014, PC86-E015 |
| PC86-A-P04 | setup first chunk | global base stores first chunk's data pointer if helper returned a valid object | first helper may return NULL; no caller guard | PC86-E018, PC86-E007, PC86-E009 |
| PC86-A-P05 | first chunk metadata | metadata stored after successful first create and before memcpy | int conversion, negative free quantity and overall layout proof | PC86-E019, PC86-E011, PC86-E012 |
| PC86-A-P06 | normal zone | requested offset storage is N pointer-size bytes | NULL allocation, ABI equality sizeof(void*) vs ulong not checked | PC86-E021, PC86-E025, PC86-E008 |
| PC86-A-P07 | second helper call | reserved chunk uses separately allocated data, not simply a reserved subrange of first chunk in this helper | second helper failure and pointer arithmetic conditions unresolved | PC86-E022, PC86-E010, PC86-E012 |
| PC86-A-P08 | DMA32/normal | successful wrapper return is conditional zeroed bytes | mapping and physical-to-virtual memory accessibility not proven | PC86-E024, PC86-E025, PC86-E023 |

### initialization_and_failure_matrix（7行）

| ID | 对象 | 条件结论 | 证据缺口 | 源锚 |
|---|---|---|---|---|
| PC86-A-FL01 | first call | proof of subsequent normal setup stops at pcpu_first_chunk->data | no actual allocator failure observed | PC86-E013, PC86-E018 |
| PC86-A-FL02 | first metadata succeeds | later memcpy with S>0 cannot be certified; execution after invalid memory access not inferred | memblock return and memory validity unknown | PC86-E014, PC86-E015, PC86-E020 |
| PC86-A-FL03 | N>0, first chunk valid | normal successful initialization cannot be certified | returned pointer/array storage unknown | PC86-E021, PC86-E025 |
| PC86-A-FL04 | first chunk and offsets valid | completion after a NULL reserved chunk is unproven | second call failure was not executed | PC86-E022, PC86-E013 |
| PC86-A-FL05 | C and sum may exceed INT representable range | narrowed/truncated/sign-changed metadata is a conditional risk, not a tested result | actual ABI and valid linked extent unknown | PC86-E011, PC86-E019 |
| PC86-A-FL06 | sum is 0 or exceeds maximum representable power-of-two | cannot use arbitrary precision rounding without input and shift preconditions | real compiler interpretation / runtime n values unknown | PC86-E017, PC86-E026, PC86-E027, PC86-E028 |
| PC86-A-FL07 | __per_cpu_load differs from __per_cpu_start | exact intended source window may not match entire section without linker equality | actual linked section VMA/LMA unresolved | PC86-E001, PC86-E002, PC86-E004, PC86-E020 |

### cpu_index_and_capacity_matrix（6行）

| ID | 对象 | 条件结论 | 证据缺口 | 源锚 |
|---|---|---|---|---|
| PC86-B-I01 | CPU count object | one scoped producer/consumer link, no exhaustive writers | tag length/validity and all initialization paths not verified | PC86-E032, PC86-E033, PC86-E034 |
| PC86-B-I02 | possible CPU iteration | contiguous 0..N-1 loop, not a sparse cpumask check | N and entry preconditions unknown | PC86-E035, PC86-E036 |
| PC86-B-I03 | fixed array limit | array length is fixed at selected compile-time CONFIG, distinct from runtime N | actual config/other defines and N<=NR_CPUS not verified | PC86-E037, PC86-E038, PC86-E040 |
| PC86-B-I04 | first N offset alloc | storage uses pointer-byte width, later values stored as ulong | sizeof(void*)==sizeof(ulong) must be an ABI assumption | PC86-E021, PC86-E025 |
| PC86-B-I05 | loop index | source macro creates a local int-style declarator; N may exceed signed range | actual preprocessing/C declaration semantics not executed | PC86-E035, PC86-E041 |
| PC86-B-I06 | zero/large count | conditional CPU0 branch absent if N=0; capacity risk if N>NR_CPUS | allocator and parser success not attested | PC86-E035, PC86-E036, PC86-E040, PC86-E042 |

### offset_address_equation_matrix（7行）

| ID | 对象 | 条件结论 | 证据缺口 | 源锚 |
|---|---|---|---|---|
| PC86-B-E01 | unit-step equation | U_i=i*S bytes, not i*C; source of write is allocated ulong array | multiplication overflow, pointer width, valid N | PC86-E021, PC86-E008 |
| PC86-B-E02 | delta equation | D=B-T, difference of C pointer representations cast to unsigned long | actual valid link/runtime addresses, overflow and mapping | PC86-E041, PC86-E002, PC86-E007 |
| PC86-B-E03 | public offset | O_i=D+i*S for each visited i | N<=NR_CPUS, no unsigned wrap, allocation success | PC86-E042, PC86-E043, PC86-E040 |
| PC86-B-E04 | ordinary per_cpu formula | if symbol V belongs to template, computed address V+D+i*S | RELOC_HIDE details and computed virtual address valid | PC86-E048, PC86-E049, PC86-E047, PC86-E050 |
| PC86-B-E05 | boot-time macro | boot variable initialization not identical by definition to later D=B-T | final link symbol values and compiler output unknown | PC86-E051, PC86-E039, PC86-E001, PC86-E002 |
| PC86-B-E06 | current cpu formula | current pointer uses current register GS base + link symbol, not memory this_cpu_off | rdgsbase availability, MSR value and mapping unknown | PC86-E049, PC86-E052, PC86-E053, PC86-E057 |
| PC86-B-E07 | two offset objects | U_i and O_i reside in different storage despite similarly named suffixes | same type ABI assumptions but no memory alias proof | PC86-E008, PC86-E040, PC86-E043 |

### publication_and_template_coverage_matrix（8行）

| ID | 对象 | 条件结论 | 证据缺口 | 源锚 |
|---|---|---|---|---|
| PC86-B-P01 | publish order | source order of three assignments per visited CPU | callee return and memory writes/addresses still conditional | PC86-E041, PC86-E042 |
| PC86-B-P02 | template scope | one CPU unit nominally gets source bytes; no per-CPU template duplication loop in this function | other copies/writers outside audited bounded path unknown | PC86-E020, PC86-E021, PC86-E042 |
| PC86-B-P03 | successful zeroing | other backing units are initially zero on successful zeroed allocation | values later written by loop and other writers must be counted | PC86-E014, PC86-E024, PC86-E055 |
| PC86-B-P04 | hot template vs CPU writes | CPU>0 fields other than cpu_number not initialized from template by that loop | additional writers and actual CPU activity unknown | PC86-E055, PC86-E054, PC86-E042 |
| PC86-B-P05 | perCPU this_cpu_off | stored value is CPU instance data, separate from current GS register write | GS register state only addressed in CPU0 branch | PC86-E039, PC86-E042, PC86-E061 |
| PC86-B-P06 | possible CPU versus CPU0 | CPU0 (not arbitrary boot CPU index) singled out | actual physical BSP number / cpu data initialization unknown | PC86-E035, PC86-E036, PC86-E059 |
| PC86-B-P07 | head assembly early consumers | early consumers depend on a coherent offset/stack and independent code paths | no AP chain or actual reachability shown | PC86-E076, PC86-E077, PC86-E040 |
| PC86-B-P08 | perCPU variable storage | one declared template is used as address basis; output section matches not same as observed object | actual compilation/ELF and per-variable location unobserved | PC86-E044, PC86-E045, PC86-E046, PC86-E001, PC86-E003 |

### boot_cpu_switch_order_matrix（6行）

| ID | 对象 | 条件结论 | 证据缺口 | 源锚 |
|---|---|---|---|---|
| PC86-C-O01 | start_kernel caller | ordered C call sites; not necessarily executed to completion | setup_arch returns successfully and earlier memory mappings valid | PC86-E058 |
| PC86-C-O02 | setup stage order | source has first allocation/copy before offset publishing | allocation failures can stop path before loop | PC86-E041, PC86-E018, PC86-E020, PC86-E021 |
| PC86-C-O03 | per iteration writes | program source sequence before CPU0 switch branch | each computed address valid and earlier call returns | PC86-E042, PC86-E043, PC86-E049 |
| PC86-C-O04 | conditional boot switch | at N>0 CPU0 path contains GDT request | N source valid, loop executed, and load_direct_gdt returns | PC86-E059, PC86-E060, PC86-E035 |
| PC86-C-O05 | GDT helper internals | source requests GDT descriptor load and segment refresh in this order | nested implementations/assembly and hardware consequences not verified | PC86-E062, PC86-E063, PC86-E064, PC86-E065, PC86-E066, PC86-E067 |
| PC86-C-O06 | GS BASE publication | MSR write request appears after GDT refresh for CPU0 in program order | load_direct_gdt and refresh return, MSR instruction succeeds | PC86-E060, PC86-E061, PC86-E071, PC86-E072, PC86-E073 |

### GDT_GS_and_current_view_matrix（7行）

| ID | 对象 | 条件结论 | 证据缺口 | 源锚 |
|---|---|---|---|---|
| PC86-C-G01 | GDT address | GDT address uses O_cpu and linker symbol via per_cpu macro, not GSBASE to locate | GDT bytes initialized, V+D+i*S mapped and valid | PC86-E064, PC86-E048, PC86-E049, PC86-E070 |
| PC86-C-G02 | descriptor size | nominal descriptor limit 127 and address from GDT pointer under macro inputs | actual compiler/layout and CPU validity of descriptors unknown | PC86-E068, PC86-E069, PC86-E062 |
| PC86-C-G03 | register reload | source indicates GDT and segment register update requests, not hardware proof | runtime fault/return and full effects of lower code unknown | PC86-E065, PC86-E066, PC86-E067 |
| PC86-C-G04 | GS register target | GSBASE write intended to use O_0 after GDT helper | CPU permission, MSR behavior, execution unseen | PC86-E061, PC86-E071, PC86-E072, PC86-E073 |
| PC86-C-G05 | current CPU pointer | postswitch current view is conditional on actual GS register, not on this_cpu_off memory assignment alone | FSGSBASE capability/use gate and hardware state unknown | PC86-E052, PC86-E053, PC86-E074, PC86-E075, PC86-E057 |
| PC86-C-G06 | source templates | representative consumer get_current reads pcpu_hot.current_task; GDT uses another template object | whether current CPU copy and memory are valid and synchronized unknown | PC86-E054, PC86-E055, PC86-E057, PC86-E070, PC86-E064 |
| PC86-C-G07 | early secondary path | same named __per_cpu_offset is a source dependency before C handoff; no AP success proof | actual CPU path, saved stack and initial offset states unknown | PC86-E076, PC86-E077, PC86-E040 |

### cross_stage_gates_matrix（6行）

| ID | 对象 | 条件结论 | 证据缺口 | 源锚 |
|---|---|---|---|---|
| PC86-C-X01 | link to first allocation | only after valid S, C and mapped storage can single copy be considered meaningful | link map and earlier memblock stage not run | PC86-E001, PC86-E002, PC86-E004, PC86-E016, PC86-E020, PC86-E024 |
| PC86-C-X02 | first chunk to offsets | no successful offset table may be inferred when helper returned invalid pointer | alloc returns/other writes unknown | PC86-E013, PC86-E015, PC86-E018, PC86-E021, PC86-E041 |
| PC86-C-X03 | CPU indexing | CPU number, array capacity and N must be jointly checked | external N validation beyond scoped producer unknown | PC86-E033, PC86-E035, PC86-E036, PC86-E037, PC86-E040, PC86-E042 |
| PC86-C-X04 | template and GDT | a successful lgdt is not shown by source, and other CPU template coverage remains limited | link input/ABI/address/CPU capability not verified | PC86-E020, PC86-E042, PC86-E064, PC86-E065, PC86-E070 |
| PC86-C-X05 | GS and current | memory this_cpu_off, offset array, GS MSR and current register are different state holders | MSR instruction, rdgsbase feature and active CPU mapping unknown | PC86-E039, PC86-E042, PC86-E052, PC86-E053, PC86-E061, PC86-E074 |
| PC86-C-X06 | historical gates | neither old PASS_PENDING_LOCAL is a real build or startup demonstration | local P9/ELF/CPU execution all NOT_RUN | PC86-E058, PC86-E076, PC86-E077 |

### cross_topic_A_B_C_join_matrix（6行）

| ID | 对象 | 条件结论 | 证据缺口 | 源锚 |
|---|---|---|---|---|
| PC86-ABC-J01 | size lineage | distinct quantities cannot be merged into a generic perCPU size | link size and C-width assumptions required | PC86-E016, PC86-E017, PC86-E014, PC86-E021, PC86-E028 |
| PC86-ABC-J02 | one copy vs index loop | does not prove CPU>0 receives all template initializers | other writers and real CPU count unknown | PC86-E020, PC86-E021, PC86-E035, PC86-E042, PC86-E055 |
| PC86-ABC-J03 | offset link to GDT | GDT pointer source expression originates from published memory array | actual mapped region and GDT contents not proven | PC86-E041, PC86-E042, PC86-E048, PC86-E049, PC86-E064 |
| PC86-ABC-J04 | CPU0 switch to current view | post-switch current pointer relies on actual GS and source template bytes | no CPU instruction/runtime evidence | PC86-E060, PC86-E061, PC86-E052, PC86-E053, PC86-E057, PC86-E070 |
| PC86-ABC-J05 | preexisting raw assembly | source shows early consumers are not automatically tied to successful setup_per_cpu_areas | startup timing/phase and ABI symbol freshness unknown | PC86-E076, PC86-E077, PC86-E040 |
| PC86-ABC-J06 | failure propagation | linear declaration and actual completion must be separated | no injected failures or actual boot measured | PC86-E013, PC86-E015, PC86-E018, PC86-E021, PC86-E022 |

## 二十九个条件模型索引（真实中间值见facts）

| 模型 | 主题/输入 | 已算出的输出摘要 | 止证门 |
|---|---|---|---|
| PC86-M01 | A / S=0B, N=1, PAGE_SIZE=4096B,dyn=0 | {"static":0,"reserved":8192,"dynamic":0,"sum":8192,"chunk":8192,"first_allocation_span_bytes":8192,"second_allocation_span_bytes":8192,"offset_array_bytes_assuming_pointer8":8,"offsets":[0],"copy_calls":1,"copy_bytes":0,"free_bytes_size_t":0,"free_offset_size_t":8192} | ABI and real link/image/data copies not observed |
| PC86-M02 | A / S=1B, N=1, PAGE_SIZE=4096B,dyn=0 | {"static":1,"reserved":8192,"dynamic":0,"sum":8193,"chunk":16384,"first_allocation_span_bytes":16384,"second_allocation_span_bytes":16384,"offset_array_bytes_assuming_pointer8":8,"offsets":[0],"copy_calls":1,"copy_bytes":1,"free_bytes_size_t":8191,"free_offset_size_t":8193} | ABI and real link/image/data copies not observed |
| PC86-M03 | A / S=4096B, N=1, PAGE_SIZE=4096B,dyn=0 | {"static":4096,"reserved":8192,"dynamic":0,"sum":12288,"chunk":16384,"first_allocation_span_bytes":16384,"second_allocation_span_bytes":16384,"offset_array_bytes_assuming_pointer8":8,"offsets":[0],"copy_calls":1,"copy_bytes":4096,"free_bytes_size_t":4096,"free_offset_size_t":12288} | ABI and real link/image/data copies not observed |
| PC86-M04 | A / S=4097B, N=2, PAGE_SIZE=4096B,dyn=0 | {"static":4097,"reserved":8192,"dynamic":0,"sum":12289,"chunk":16384,"first_allocation_span_bytes":32768,"second_allocation_span_bytes":32768,"offset_array_bytes_assuming_pointer8":16,"offsets":[0,4097],"copy_calls":1,"copy_bytes":4097,"free_bytes_size_t":4095,"free_offset_size_t":12289} | ABI and real link/image/data copies not observed |
| PC86-M05 | A / S=8192B, N=2, PAGE_SIZE=4096B,dyn=0 | {"static":8192,"reserved":8192,"dynamic":0,"sum":16384,"chunk":16384,"first_allocation_span_bytes":32768,"second_allocation_span_bytes":32768,"offset_array_bytes_assuming_pointer8":16,"offsets":[0,8192],"copy_calls":1,"copy_bytes":8192,"free_bytes_size_t":0,"free_offset_size_t":16384} | ABI and real link/image/data copies not observed |
| PC86-M06 | A / S=8193B, N=3, PAGE_SIZE=4096B,dyn=0 | {"static":8193,"reserved":8192,"dynamic":0,"sum":16385,"chunk":32768,"first_allocation_span_bytes":98304,"second_allocation_span_bytes":98304,"offset_array_bytes_assuming_pointer8":24,"offsets":[0,8193,16386],"copy_calls":1,"copy_bytes":8193,"free_bytes_size_t":16383,"free_offset_size_t":16385} | ABI and real link/image/data copies not observed |
| PC86-M07 | A / S=4096B, N=0, PAGE_SIZE=4096B,dyn=0 | {"static":4096,"reserved":8192,"dynamic":0,"sum":12288,"chunk":16384,"first_allocation_span_bytes":0,"second_allocation_span_bytes":0,"offset_array_bytes_assuming_pointer8":0,"offsets":[],"copy_calls":1,"copy_bytes":4096,"free_bytes_size_t":4096,"free_offset_size_t":12288} | zero-byte data/offset alloc behavior unknown; no CPU0 iteration |
| PC86-M08 | A / first early_pcpu_create_chunk metadata= NULL | {"first_chunk":"NULL","next_source_action":"pcpu_first_chunk->data; no valid dereference proof"} | execution stops at unguarded dereference, not a measured trap |
| PC86-M09 | A / first metadata non-NULL, data=NULL, S=4096 | {"data":"NULL","list_publication":"attempted","memcpy_bytes":4096} | memcpy with data NULL not normal success; postfault state unknown |
| PC86-M10 | A / first chunk healthy, N=2, pcpu_unit_offsets=NULL | {"allocation":"NULL","first_loop_store":"((ulong*)NULL)[0]"} | valid first offset write cannot be established |
| PC86-M11 | A / first chunk+offsets success, reserved create=NULL | {"pcpu_reserved_chunk":"NULL","next_operation":"pcpu_reserved_chunk->base_addr"} | subsequent valid dereference not established |
| PC86-M12 | A / S=2147483648, reserve=8192, N=1 | {"sum":2147491840,"chunk":4294967296,"int_free_offset_input":2147491840,"int_free_bytes_input":2147475456,"first_data_bytes":4294967296} | C out-of-range unsigned-to-signed conversion implementation-dependent; no runtime witness |
| PC86-M13 | B / L=0x100000,T=0x100000,B=0x200000,V=L+0x100,S=4096,N=2 | {"delta":1048576,"U":[0,4096],"O":[1048576,1052672],"ptrs":[2097408,2101504]} | percpu pointer values not proof of real CPU memory |
| PC86-M14 | B / S=4096,N=3,C=16384 | {"allocation_total":49152,"U":[0,4096,8192],"nominal_C_width_windows":[[0,16384],[4096,20480],[8192,24576]],"windows_overlap":true} | actual perCPU reserved/dynamic allocation may differ; scoped overlap is candidate only |
| PC86-M15 | B / N=1,S=4096 | {"visited":[0],"offsets":[0],"CPU0_GDT_GS_requests":"present_if_callees_return"} | cannot infer actual register state or other CPU existence |
| PC86-M16 | B / N=0,S=4096 | {"visited":[],"CPU0_branch":"not_reached","data_alloc_request_bytes":0,"offset_alloc_request_bytes":0} | actual N=0 path and zero-size alloc outcomes unknown |
| PC86-M17 | B / N=257,NR_CPUS=256,S=4096 | {"fixed_array_valid_last_index":255,"first_excess_index":256,"visited_indices_in_source_loop":"0..256"} | not a proven live input or actual OOB write; stop before postinvalid effects |
| PC86-M18 | B / N=3,sizeof(void*)=8,sizeof(ulong)=8 | {"requested_bytes":24,"ulong_stores":3,"store_bytes":24,"last_start_offset":16} | unknown ABI if not LP64 or if allocator returns insufficient space |
| PC86-M19 | B / expected CPUs {0,2}, nr_cpu_ids=3 | {"visited_actual_from_macro":[0,1,2],"expected_sparse_set":[0,2],"extra_visited":1} | not a confirmed real sparse CPU topology |
| PC86-M20 | B / S=4096,C=16384,N=2 | [{"first_static":[0,4096],"unit1_static":[4096,8192],"hypothetical_reserved_within_unit0":[4096,12288],"intersection_with_unit1_static":[4096,8192]}] | does not prove reserved allocator actually writes overlapping ranges |
| PC86-M21 | B / N=2,S>0 and nonzero pcpu_hot.current_task template | {"template_copies":1,"CPU0_current_task":"copied template","CPU1_before_specific_writes":"zero under successful DMA32 zeroing","CPU1_cpu_number":"written 1","CPU1_current_task":"not assigned by shown loop"} | not real CPU1 invocation/current evidence |
| PC86-M22 | B / L=0x100000,T=0x101000,B=0x200000,S=4096 | {"boot_percpu_offset":1048576,"runtime_delta":1044480,"runtime_O_cpu0":1044480,"difference":4096} | actual linker may make L=T; not a witnessed difference |
| PC86-M23 | C / N=1,all first chunk allocations/copies/offsets succeed | {"ordered":["publish O0","write this_cpu_off[0]","write cpu_number[0]","call load_direct_gdt(0)","inside helper load_gdt","inside helper refresh_segment_registers","request wrmsrl(MSR_GS_BASE,O0)"]} | no CPU instruction success proof |
| PC86-M24 | C / N=1, load_direct_gdt or refresh_segment_registers does not return | {"O0":"published","load_direct_gdt":"called","GS_write":"not reached"} | no assertion about actual failure/exception |
| PC86-M25 | C / N=1, load_direct_gdt returns, MSR write faults | {"GDT_request":"prior","MSR_request":"issued","post_GS_read":"no valid runtime proof"} | CPU state after trap not derived |
| PC86-M26 | C / N=2,perCPU loop reaches cpu1 | {"cpu1_offset":"published","cpu1_cpu_number":"1","cpu1_GDT_GS_calls_from_this_function":0} | does not audit or exclude AP initialization elsewhere |
| PC86-M27 | C / N=1, old_GSbase G_old!=O0, both mapped, new template copied | {"before_expr":"V+G_old","after_requested_expr":"V+O0","memory_this_cpu_off":"O0"} | actual MSR state not observed; no safe early dereference guarantee |
| PC86-M28 | C / rdgsbase execution without feature enabled | {"source_request":"rdgsbase mnemonic","guard_in_local_raw_cpu_read_long":"none in quoted macro","result":"UNKNOWN, no hardware semantics asserted"} | need official hardware manual/CPU evidence for runtime effects |
| PC86-M29 | C / GDT_ENTRIES=16, GDT_SIZE=16*8 | {"GDT_SIZE":128,"gdt_descr_size_input":127} | not actual LGDT completion or safe descriptor contents |

