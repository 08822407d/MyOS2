---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-EARLY-ROOT-HANDOFF-84
produced_by: GPT-6（当前会话标识；后台不可独立认证）
model_per_owner: GPT-6＋Extra（Owner指定）
date: 2026-11-13
base_snapshot: time
read_channel: connector
status: final
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions: [ER84-U01, ER84-U02, ER84-U03, ER84-U04, ER84-U05, ER84-U06, ER84-U07, ER84-U08]
---

# 第84批：启动根表存储、地址修正与C入口交接

新身份：MYOS2-LEAD-002-CHAT-EXEC-11。唯一源码 time@a039d9803ade；执行父提交18047d54b0a2；合同120@6b5aabc24e2a。九项机器可读矩阵及28例条件模型在facts-and-boundaries.yaml；49条固定源码短锚在evidence.md。所有判断仅为静态分析，CPU/固件/ELF最终链接/内核运行均NOT_RUN。

## G01/A：六种静态页表对象

early_top_pgt的汇编实物是511个8字节零槽和一个非零高地址指针槽，共512项=4096字节，而不是附近注释提到的8KiB。early_dynamic_pgts的64张表页由512×64×8=262144字节静态零填充组成；本轮__startup_64只使用前两页。level3_kernel_pgt有510个零槽，后两项位于PUD层索引510、511，分别通向level2_kernel_pgt和level2_fixmap_pgt。level2_fixmap_pgt共512个零槽。level2_kernel_pgt由PMDS按KERNEL_IMAGE_SIZE/PMD_SIZE计数，在CONFIG_RANDOMIZE_BASE未定义/定义两种情况下分别为256项、2048字节、512MiB名义覆盖与512项、4096字节、1GiB名义覆盖；不能凭extern声明称该表任何配置都填满512槽。

__STARTUP定义.head.text.startup且被kernel.lds的.text/KEEP收集；__INITDATA对应.init.data，安放early_top_pgt与early_dynamic_pgts；level3、level2和phys_base属于.data。C定义的init_top_pgt[ENT_PER_TABLE]依64位pgd_t=p4d_t单值布局、PAGE_SIZE/sizeof(pgd_t)计算为512项/4096字节，属于未初始化全局，最终.bss/COMMON位置还须真实编译/链接map确认。initial_code使用__REFDATA即.ref.data，kernel.lds未按名称显式收集.ref.data，输出orphan的位置与PT_LOAD可访问性不可由源码注释代证。

链接高地址__START_KERNEL_map=0xffffffff80000000，kernel.lds使用phys_startup_64=ABSOLUTE(startup_64-__START_KERNEL_map)作为入口表达，以AT(ADDR-__START_KERNEL_map)定义名义装载地址；这与运行时RIP相对地址、phys_base零初始化和后续load_delta写入分属不同证据层。root[511]不是PUD[510]/[511]：它们位于不同层级。BSS清零范围是[__bss_start,__bss_stop)，不是对所有已声明全局变量的无条件覆盖。

固定time的CMake选项中未看到CONFIG_RANDOMIZE_BASE，但其他配置来源未穷尽。kernel.lds断言只允许_end-__START_KERNEL_map<=1GiB，不能直接证明非随机分支512MiB的PMD叶项覆盖真实链接布局；例如假定链接范围768MiB时可过1GiB断言却超出512MiB静态名义窗口，尚非实际镜像的证明。
## G02/B：地址修正、临时恒等映射与CR3

startup_64用RIP定位common_startup_64运行地址，减去.Lcommon_startup_64存储的链接地址，形成第一个实参p2v_offset；初始rsi被保存到r15再作为第二实参传给__startup_64。C函数先用rip_rel_ptr(&_text)形成physaddr，并检查physaddr>>MAX_PHYSMEM_BITS，其中MAX_PHYSMEM_BITS固定为46，故可继续的起点严格小于64TiB。随后phys_base=load_delta=__START_KERNEL_map+p2v_offset，再判定load_delta & ~PMD_MASK：只有2MiB对齐的delta才能继续，零不是强制条件；失败分支是无限循环，没有后续修正操作。va_text/va_end虽被计算，却没有被余下可见函数体消费。

通过入口检查后，代码只将load_delta加到early_top_pgt的高地址根项以及level3_kernel_pgt的末两个PUD表项。level2_kernel_pgt的原始PMDS高映射叶仍以i×2MiB构造，本函数没有相应的叶项重定位循环。例如以链接_text物理偏移16MiB为前提，delta=2MiB可通过对齐检查，实际物理_text为18MiB，恒等PMD槽9开始映射18MiB；但是同一链接高地址_text仍使用静态高PMD槽8，其原始物理叶为16MiB，形成条件性映射不一致候选ER84-F01。它尚需其他可能修正写者、真实链接地址及机器码验证，不能直接判定硬件故障。

同时，head64.c注释声称C使用PIC，固定time公共CFLAGS实际是-mcmodel=kernel -fno-pie -fno-pic；rip_rel_ptr在asm.h有非PIC内联汇编路径，但phys_base与level3_kernel_pgt等直接全局访问不全是rip_rel_ptr。必须区分源码引用、编译器实发地址和引导身份映射阶段可访问性，ER84-F02仅列风险，不断言未经编译的具体机器指令。

在临时表分支中，early_pgts[0]作PUD页、[1]作PMD页，next_early_pgt赋2，pgtable_flags为_KERNPG_TABLE_NOENC。顶层索引i=(physaddr>>PGDIR_SHIFT)%512，再写pgd[i]和pgd[i+1]同指一个PUD页；后一槽没有取模，但在physaddr<2^46时i仅为0..127，i+1最多128，不能把i=511视为可达的越界实例。下一级PUD[(j+0)%512]、PUD[(j+1)%512]都指向同一PMD页；PMD循环次数N=DIV_ROUND_UP(_end-_text,PMD_SIZE)，第k次写(floor(physaddr/2MiB)+k)%512，其值为physaddr+k×2MiB与大页EXEC权限位相加、去掉GLOBAL。按2MiB页框对齐假设，加法不会把低位flag进位到页框；这不是无条件可推广的数值语义。

以physaddr=1GiB-2MiB、N2为模型，PUD跨槽0/1、PMD跨槽511/0仍可由共用的PMD页描述；physaddr=512GiB-2MiB时PGD槽0/1也需连续可达。只查PMD模512不证明上层可达，只有父项索引与表指针一起成立才是所建临时窗口。N=513的模型展示PMD槽复用，但固定链接起点约16MiB和1GiB上界使该N不属于可证明的真实链接输入。另有物理上界例：起点2^46-2MiB、N2可过起点检查，第二片到2^46，函数没有对末端单独检查；输入是否实际可出现仍未知（ER84-F04）。

返回前还有按mbi_magic选择的memcpy目的区与源指针依赖，成功路径C函数字面return 0。汇编执行leaq early_top_pgt(%rip)加到rax，再写%cr3，后按.Lcommon_startup_64内保存的链接高地址间接跳转。源注释提到SME mask，但代码返回0，不能凭注释填上未存在的mask处理；mov cr3/间接jmp存在也不是CPU成功执行的证据。
## G03/C：C入口与独立根的生存期

head_64.S在__REFDATA区声明initial_code，其初始值为x86_64_start_kernel。common_startup_64是SYM_CODE_START(secondary_startup_64)与SYM_CODE_END(secondary_startup_64)内的中间标签；启动CPU从早期CR3和高地址跳转到该处，之后尚有smpboot_control分支、perCPU索引、栈/GDT/GS、EFER/CR0等条件，直到callq *initial_code(%rip)。.Lread_apicid存在源代码自循环，因此不能把所有控制分支视为已达C，更不能把共用secondary符号当作完成AP启动分析。

若C入口到达，x86_64_start_kernel中的可见语句严格依次为clear_bss()、clear_page(init_top_pgt)、idt_setup_early_handler()、myos_early_init_system()、init_top_pgt[511]=early_top_pgt[511]、start_kernel()。clear_bss依链接__bss_start/__bss_stop执行memset，随后的clear_page又独立清一张init_top根页；两者不是一个内存事件。特别是myos_early_init_system和早期IDT路径只是已读的调用点，没有得到整段callee实现及其副作用/返回保证，若任一callee不返回，后续复制和start_kernel根本不会执行。即使返回，copy也只是将一个PGD槽的值复制到另一张根页；可使两张根指向相同PUD子树，绝非整张页表深复制，也不等于主动写CR3。

后续唯一已读实际调用者是mykernel/init/main.c的start_kernel，其主体开始处调用setup_arch(NULL)。setup_arch相关顺序为early_alloc_pgt_buf、myos_early_map_pgt_buf_into_directmap、init_mem_mapping；它们之间还有内存保留及e820等先行callee门。提前directmap函数按81审查界限将early_top_pgt对应directmap顶层槽写入新子表，调用__flush_tlb_all后再执行*init_pgd=*early_pgd。其直接映射顶层槽的复制与早前C入口高地址槽[511]复制是不同索引与不同时间点的动作，不等于同一事件。init_mem_mapping末尾出现load_cr3(swapper_pg_dir)及后续__flush_tlb_all；memfault_api.h将swapper_pg_dir别名为init_top_pgt，而init_mm.pgd的静态初始化也指向此别名。这只能证明源中有后续CR3切换请求，不能据静态指针/根项写入声称活动CR3早已切换或TLB已真实生效。

**停证边界。** 后续81/82/83涉及的页表缓冲、directmap分段及表安装只沿用其已审结论和限定，不在84重新验证整个算法。特别是PT81-U03/U06、DM82-U08和TI83原有根/窗口/属性缺口不能由单项根复制自动关闭。若先行callee会动当前CR3或共享下级页，则需重新设前提，不能凭入口函数的书写顺序替代运行链证明。

## 第84批有限模型及问题分类

facts-and-boundaries.yaml实际记录A8/B16/C4共28个纯整数模型，并逐例列阶段、地址域、64位整数/指针宽度、delta及p2v假设或N/A、PGD/PUD/PMD容量、callee返回前提。典型B09起始16MiB、delta0、N2对应PMD8/9；B10起始18MiB、delta2MiB对应临时PMD9/10而链接高VA的_text仍处于静态PMD8；B13起点2^46-2MiB、N2暴露尾部越界门；B14/B15各自跨PUD/PGD；B19/B20在N512/513展示槽碰撞并明确后者不获真实链接可达认证；C26/C27区分先行callee不返回与正常返回后仅复制高根槽的条件。

新登记五项条件候选ER84-F01..F05及八项证据缺口ER84-U01..U08详见deferred-findings.yaml，前者只是需要主线判断的潜在问题，后者不是八个已确认程序缺陷；均未获得修改内核授权。固定时间点的源、代码生成、链接映射和真实运行是四种不同证据层。此次未运行任何仓库脚本、编译/预处理、汇编/链接、Graphviz、QEMU/实机或额外模型/API。

## 九矩阵：56条源事实及条件项

### A1 静态表对象清册

| ID | symbol | section_input | slots_nominal | bytes_nominal |
|---|---|---|---|---|
| ER84-A-ST01 | early_top_pgt | .init.data | 512 | 4096 |
| ER84-A-ST02 | early_dynamic_pgts | .init.data | 32768 | 262144 |
| ER84-A-ST03 | level3_kernel_pgt | .data | 512 | 4096 |
| ER84-A-ST04 | level2_kernel_pgt | .data | 256 if !CONFIG_RANDOMIZE_BASE; 512 if defined | 2048 or 4096 |
| ER84-A-ST05 | level2_fixmap_pgt | .data | 512 | 4096 |
| ER84-A-ST06 | init_top_pgt | uninitialized global; .bss/COMMON candidate under linker wildcard, final compiler/link map unverified | ENT_PER_TABLE=PAGE_SIZE/sizeof(pgd_t)=512 under x86-64 one-word ABI | 4096 under stated ABI |

### A2 链接与运行时地址

| ID | object | link_address | runtime | storage |
|---|---|---|---|---|
| ER84-A-LK01 | startup_64 | __START_KERNEL_map + linked offset; entry phys_startup_64 = startup_64 - __START_KERNEL_map | identity-mapped physical code assumption | (.head.text.startup) under .text PT_LOAD |
| ER84-A-LK02 | early_top_pgt | link high VA; .quad pointer stores level3 label minus __START_KERNEL_map plus flags | RIP-relative root label can evaluate current mapped VA | .init.data not .bss; 1 page, not 8KiB |
| ER84-A-LK03 | early_dynamic_pgts | link high VA; runtime rip_rel_ptr selects two pages | physical execution alias requires pre-existing accessibility | .init.data 64 contiguous pages |
| ER84-A-LK04 | level3_kernel_pgt | static .quad physical symbolic address | normal C address direct [510]/[511] writes, distinct from rip_rel_ptr; depends codegen/addressability | .data, padded/aligned |
| ER84-A-LK05 | level2_kernel_pgt | PMDS leaves encode 0-based physical frame values | no corresponding PMD leaf += load_delta in read __startup_64 | .data; 256 or512 elements |
| ER84-A-LK06 | init_top_pgt | C global link VA, not same object as early_top | BSS + clear_page + one root entry copy; later load_cr3 | likely .bss/COMMON, map not produced |
| ER84-A-LK07 | initial_code | .quad x86_64_start_kernel | RIP-relative indirect call | .ref.data input not explicitly listed by kernel.lds; output placement unproven |

### A3 静态初始映射

| ID | layer | index | entry | coverage |
|---|---|---|---|---|
| ER84-A-CV01 | PGD | 511 | level3_kernel_pgt physical+_PAGE_TABLE_NOENC | one PUD page, 512*1GiB theoretical; only populated PUD 510/511 |
| ER84-A-CV02 | PUD | 510 | level2_kernel_pgt physical+_KERNPG_TABLE_NOENC | only PMDS COUNT=256 or512 huge leafs |
| ER84-A-CV03 | PUD | 511 | level2_fixmap_pgt physical+_PAGE_TABLE_NOENC | all 512 PMD entries zero in read initial definition; no leaf assumed |
| ER84-A-CV04 | PMD | 0..255 | 0,2MiB,...510MiB+EXEC flags | nominal 512MiB, not necessarily all final linked kernel if linker only checks 1GiB |
| ER84-A-CV05 | PMD | 0..511 | 0,2MiB,...1022MiB+EXEC flags | nominal 1GiB |
| ER84-A-CV06 | PGD vs PUD | 511 versus 510/511 | different levels and distinct data tables | independent indexing |

### B1 参数与装载偏移

| ID | step | expression | domain | condition |
|---|---|---|---|---|
| ER84-B-AR01 | asm_arg | p2v_offset=RIP(common_startup_64)-link_time_quad(common_startup_64) | 64-bit difference under input identity mapping | startup_64 setup_gdt_idt and lretq return, no divergent path |
| ER84-B-AR02 | runtime_physical | physaddr=(ulong)rip_rel_ptr(&_text) | physical under identity-mapped execution precondition | physaddr >>46 ==0; otherwise infinite loop |
| ER84-B-AR03 | delta | phys_base=load_delta=__START_KERNEL_map+p2v_offset | unsigned ulong arithmetic mod2^64; relocation delta if input model holds | load_delta &~PMD_MASK ==0; zero not required |
| ER84-B-AR04 | computed_but_not_consumed | va_text=physaddr-p2v_offset; va_end=rip_rel_ptr(&_end)-p2v_offset | calculated links under same model | no later read of these locals in __startup_64 body |
| ER84-B-AR05 | return | return 0 (after selected memcpy) | ulong return register, then ASM adds RIP root address | memcpy and all prior callee effects/returns accepted |

### B2 临时恒等映射

| ID | step | index | role | condition |
|---|---|---|---|---|
| ER84-B-ID01 | select_pages | early_pgts[0],[1] | PUD and PMD | RIP-relative early_dynamic_pgts addresses accessible |
| ER84-B-ID02 | identity_PGD | i=(physaddr>>39)%512; i+0 and i+1 | both refer one PUD page | physaddr<2^46 ensures i in[0,127], i+1<=128 |
| ER84-B-ID03 | identity_PUD | (physaddr>>30+0/1)%512 | both refer one PMD page | no evaluation of actual PUD entry collisions with pre-existing non-zero view |
| ER84-B-ID04 | identity_PMD | (floor(physaddr/2MiB)+i)%512 | large page physical frames physaddr+i*2MiB | N=DIV_ROUND_UP(_end-_text,PMD_SIZE), alignment and span fit required |
| ER84-B-ID05 | flags | PMD leaf | (__PAGE_KERNEL_LARGE_EXEC & ~_PAGE_GLOBAL)+physaddr+i*PMD_SIZE | 2MiB aligned physical frame, flags do not collide with frame bits |
| ER84-B-ID06 | boundary | cross 1GiB PUD | same PMD table reused on two consecutive PUDs | PUD/PGD reachability and no PMD slot collision |

### B3 根项和CR3

| ID | step | target | change | source |
|---|---|---|---|---|
| ER84-B-SW01 | early_high_root | early_top_pgt[pgd_index(__START_KERNEL_map)]=slot511 | +=load_delta; conditional validity of static table frame+flags | ER84-E30 |
| ER84-B-SW02 | lower_high_tables | level3_kernel_pgt[510/511] | +=load_delta only on two subordinate pointer entries | ER84-E03, ER84-E30 |
| ER84-B-SW03 | high_leaf_pmd | level2_kernel_pgt static leaf slots | no leaf relocation loop in read __startup_64; nonzero delta remains conditional hole | ER84-E04, ER84-E29, ER84-E34 |
| ER84-B-SW04 | identity_root | early_top_pgt[i],early_top_pgt[i+1] | =pud physical pointer + pgtable_flags | ER84-E32 |
| ER84-B-SW05 | CR3 | movq %rax,%cr3 | rax=0 + RIP-relative early_top_pgt address under successful function return | ER84-E25, ER84-E26, ER84-E36 |
| ER84-B-SW06 | high_jump | jmp *.Lcommon_startup_64(%rip) | source selects indirect target; hardware success and mapped target not attested | ER84-E26, ER84-E40 |

### C1 C根生命周期

| ID | stage | op | root | observed |
|---|---|---|---|---|
| ER84-C-LF01 | C_entry | initial_code RIP indirect call | early_top_pgt intended active | ASM indirect call exists; actual runtime success unknown |
| ER84-C-LF02 | clear_BSS | clear_bss() | early_top_pgt (init.data) distinct from init_top_pgt (BSS candidate) | C call order; linker BSS bounds exact but final map unknown |
| ER84-C-LF03 | clear_init_root | clear_page(init_top_pgt) | init_top_pgt | additional explicit root clearing, no CR3 write |
| ER84-C-LF04 | early_hooks | idt_setup_early_handler();myos_early_init_system() | unknown potential side effects on page tables/CR3 | only call sites within studied code; no return proof |
| ER84-C-LF05 | root_copy | init_top_pgt[511]=early_top_pgt[511] | value-copy single PGD entry, shared target PUD tree | if pre-calls return; no deep clone |
| ER84-C-LF06 | start_kernel | start_kernel();setup_arch(NULL) | init_top not yet guaranteed active | source caller order |
| ER84-C-LF07 | future_root | init_mem_mapping tail load_cr3(swapper_pg_dir) | init_top_pgt alias | later source write; no executed hardware evidence |

### C2 根别名与共享子树

| ID | view | own_pages | shared_child | note |
|---|---|---|---|---|
| ER84-C-AL01 | early_top_pgt | one 4KiB root in init.data | high [511] pointer to level3_kernel_pgt | init_top_pgt not same array |
| ER84-C-AL02 | init_top_pgt before BSS clear | one 4KiB C global, .bss candidate | not established; compiled layout not verified | uninitialized definition and linker wildcard separate layers |
| ER84-C-AL03 | init_top_pgt after clear_page | same root, all bytes written zero if executed | none from this operation | not a CR3 write |
| ER84-C-AL04 | init_top_pgt[511] after copy | same distinct root object | same [511] PUD page as early_top_pgt if copy executed | shallow entry copy, not deep copy; downstream writes may affect both |
| ER84-C-AL05 | direct_map index updated | early and init root entries at given direct-map PGD | value copied from early to init in myos_early_map_pgt_buf_into_directmap | may overwrite direct-map slot; does not generally update every slot |
| ER84-C-AL06 | init_mm.pgd | pointer alias swapper_pg_dir | init_top_pgt root | structural alias not active CR3 attestation |

### C3 交接顺序与停证前提

| ID | order | source | action | gate |
|---|---|---|---|---|
| ER84-C-HO01 | 1 | head_64.S::startup_64 | p2v_offset, __startup_64 and CR3 early_top | prior identity mapping, register inputs, prior GDT/IDT helper returns, valid actual codegen |
| ER84-C-HO02 | 2 | head_64.S::secondary_startup_64 | common_startup_64 indirect initial_code | valid perCPU stack/segment/GS/cpuid preconditions, no AP proof |
| ER84-C-HO03 | 3 | head64.c::x86_64_start_kernel | BSS clear then init_top clear | entry reached; sections accessible and non-overlapping needed |
| ER84-C-HO04 | 4 | head64.c::x86_64_start_kernel | early IDT, myos_early_init_system; high root[511] value copy; start_kernel | unknown callee return/CR3 effects; no automatic pass |
| ER84-C-HO05 | 5 | main.c::start_kernel | setup_arch(NULL) | start_kernel actually called and returns from previous callees |
| ER84-C-HO06 | 6 | setup.c::setup_arch | early_alloc_pgt_buf => myos_early_map_pgt_buf_into_directmap => init_mem_mapping | e820/other early calls and user config branches return; 81/82/83 preserved |
| ER84-C-HO07 | 7 | init.c::init_mem_mapping | load_cr3(swapper_pg_dir); __flush_tlb_all() | init_mem_mapping reached, wrappers/CPU semantics and mapping valid; runtime NOT_RUN |

## 三专题接合总表

| 接合点 | 已有静态证据 | 尚不能推出 |
|---|---|---|
| 静态高映射→启动C修正 | 早期根[511]，PUD[510/511]，PMDS静态叶，root和表层分离 | delta非零下所有实际叶项得到正确修正 |
| startup C修正→早期CR3 | 各守卫、两scratch页、返回0、mov cr3、间接高跳转 | 固件身份映射/CR3硬件操作与高跳转真实成功 |
| 高跳转→C入口 | common嵌在secondary大符号内，initial_code跳C，清BSS/根等次序明确 | AP路径全证、所有先行callee必返回/不碰根 |
| 高根单项复制→后续directmap根/CR3 | 先copy根[511]，后direct-map根单项复制，最后load_cr3(swapper_pg_dir) | 复制就是deep clone、实际活动根/flush成功 |

## 审查限制及交回条件

R83-S01/S02/S03继续约束互斥分支、真实写包装、present/权限位、同层索引、父项可达性与请求/写入/返回的分账；R82-S01/S02/S03保留阶段与整数取整及范围容量边界；R81-S01/S02/S03保留准确定位、直接页分配和地址回译限定。PT81-U03/U06、DM82-U08及TI83所有旧候选状态不能由本执行者关闭。D00不要求，旧81–83不重做。

本执行者仅交可复审静态六件；本批提取五个条件候选ER84-F01..F05、八个证据缺口ER84-U01..U08，独立缺陷是否成立以及旧项能否缩小交主线Pro。无预处理/编译/链接、仓库程序、Graphviz、QEMU、实机、付费API或外加模型。P9全量本地套件未运行，不自签PASS，mainline_acceptance=PENDING。
