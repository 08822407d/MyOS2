---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BOOT-MEMORY-MAP-80
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
produced_by: GPT-6 (模型自报；后台不可独立认证)
model_per_owner: GPT-6 + Extra (Owner指定)
date: 2026-11-13
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
lead_pin_short12: f4a3b437cb44
execution_parent_short12: c10d0447940e
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第80批｜启动内存图接收、最大PFN及memblock移交（追加79文档纠正）

**交付位置**：原执行分支 `claude/dazzling-cori-q0dnyt`、PR17，仅第80批新六件；固定源码 `time@a039d9803ade`。由 **MYOS2-A-C06 Pro** 独立核查，静态许可上限 `PASS_PENDING_LOCAL`。不改内核、既有结果、评论、PR元数据或主线；未执行内核、构建、QEMU、Graphviz、完整本地P9。正文所有条件链只在前序表达、存储和callee已合法的隔离前提下成立；首次未证门后没有真实执行结论。

## D00｜第79批文档纠正追加；78不重开

78的 `R78-01=CLOSED_DOCUMENTATION_ONLY`，78组合 `PASS_PENDING_LOCAL`，是C06已独立签发的结论，现不重开。79本身仍 `RETURN`，仅 `R79-01/02` 文档范围。原79目录六件及评论 `6828422617` 均保持原样，本80提交不是更新79原件。

**R79-01**：原79/MANIFEST的顶层必备字段缺少11个：`track_id`、`produced_by`、`model_per_owner`、`base_snapshot`、`read_channel`、`inputs_read`、`status`、`open_questions`、`startup_selfcheck_quote`、`branch_canary_quotes`、`self_check`。该事实来自本轮固定原件前置区字段检查；历史作者称读过并95次逐字重核，但95个原始标签中A06/A07把宏外的 `#ifndef` 包进了定义，不能据原字面命中宣布95/95全部P2合格。79六件原始保留前缀数量仍为95（D13/A23/B24/C35），不回写原 `facts.methods` 的旧自报，当前80的92个新短锚也不与95相加宣传。新MANIFEST的 `historical79_manifest_supplement` 已逐字保留当前重新获取的公约原句、time/master两金丝雀与读取通道、原始计数/署名来源和历史缺项。原临时Mnemosyne接收与指导刷新报告在当前运行环境不可取得，明确写 `NOT_AVAILABLE`；仅保留79作者在旧MANIFEST的历史声明，并补记**2026-11-13这次真实读取清单及适用范围**，没有倒造旧报告或旧接收时间。

**R79-02**：79/A06/A07仍是历史标签；80/D01和D02分别以单行 `#  define INIT_MMBLK_RESERVED_REGIONS ...`、`#  define INIT_MMBLK_MEMORY_REGIONS ...` 取代原跨条件指令的引文，`#ifndef` 只在引文外说明。此替代映射覆盖旧MANIFEST、正文/evidence导语、facts.methods和旧PR17评论将“95条全部单一定义内”的过宽说法；旧95数目仅说明标签存在、作者曾字面重读，非全部通过P2。实际宏变体选中条件和旧评论均保持历史状态，不自行关闭R79。

**R79-S01/S02/S03**：直接finder在free[100,200)、align16、size0得到112的79/M23只能是低层整数样本；完整`memblock_alloc_range`先将start提高到PAGE_SIZE。声明free[4096,8192)、size0、align16且全部迭代门合法时，低层candidate4096可以通过外层的非零地址门，而`reserve(4096,0)`因size0返回0且不新增保留字节，此为隔离模型（D03/D07）。79/M25仅首次0并且min_addr非零时仍可重试，最终0才是NULL（D04）；`MEMBLOCK_NONE`下区域的MIRROR位不独立造成排除（D05）。旧30条历史记录拆分为**29个源码条件样本＋1个文档计数记录**。旧DOT中caller内insert→merge及alloc参数准备→finder属于`CALLER_SEQUENCE`，不是跨函数调用；direct alloc_node_mem_map物理0转虚拟路径不得与internal的重试/NULL失败节点共用一个条件；原C25的迭代终点需以80/D06真实 `*idx=ULLONG_MAX` 补证。79/M06/M07中差值在窄化成size_t之前先依操作数类型求值，M11和M19中无符号减法、向int窄化、数组指针形成与解引用逐门区分。原79的8个候选、10个缺口仍全为needs_evidence；对旧图、旧矩阵和旧根因计数不另开整批复审。

D00当前仅 `DOCUMENTATION_CORRECTION_SUBMITTED_PENDING_C06`；第79批两项是否闭合必须由C06下一回执裁定。

## G01/A｜从标签到复制后的E820表

**输入/指针账**：在内核C侧 `__startup_64` 有按 `mbi_magic` 选择的固定 `memcpy(mbi_dst,(*mbi_base_p),sizeof(multiboot_MBI))`；目标 `multiboot_MBI` 在setup.c中定义为32×PAGE_SIZE字节的.data页对齐数组（A08–A10）。这个copy不是按标签头 `total_size` 拷贝，真实源可读extent、最早C函数是否实际进入、外部loader如何填充 `mbi_magic/mbi_base` 都不在已证范围。所读 `x86_64_start_kernel` 调用 `myos_early_init_system`，后者将 `mbi_magic` 和 `&multiboot_MBI` 传入 `parse_tag`（A01/A11）；不追引导汇编或外部固件。文件作用域 `ram_map` 是指针定义，`mbi_tag_header` 是另一独立指针（A02/A03），不能把头地址与entries地址同一化。

**标签入口**：`parse_tag`先验证magic和addr&7，失败直接return，不清ram_map。通过后设置 `mbi_tag_header=(...*)addr`，读取 `total_size` 到局部 `unsigned size`，却没有用它限制标签遍历；循环止于tag.type==END，步进由 `((tag->size+7)&~7)` 计算（A04–A06）。因此即便头部宣称某长度，不等于每个tag位于该长度内；tag.size=0的非END标签可能使指针不前进，缺END或溢出后的下一次读取须在已证extent界停推。MMAP分支只把 `mbi_mmap->entries` 发布给 `ram_map`，虽然共享头有`entry_size`、`entry_version`，消费处并未用它们（A07/A18）。缺MMAP时指针保持原值；多个MMAP标签若遍历均有效则最后一次赋值覆盖早先值，不能据“有MMAP”断言输入唯一或存活期足够。

**复制到目标表**：`e820_table` 指向静态 `e820_table_init`，目标结构是 `nr_entries` 和 `entries[E820_MAX_ENTRIES]`，本树容量128（A21–A24）。`e820__memory_setup`先置 `nr_entries=0`，随后按 `mmap_entp->len!=0 && mmap_entp->type!=0` 继续，指针`mmap_entp++`按本地 `mbi_mmap_ent_s` C对象大小步进；每次先形成 `&entries[i]` 再写addr/size/type，循环结束才发布计数并打印（A25–A29）。零len或零type是“终止”，不是跳过；size不同于C对象步长、没有哨兵、指针无有效extent均不得推到写后状态。128条合法记录后存在合法终止哨兵可在边界内收束；**129条均有效时，在i=128形成entries[128]地址即触及容量门**，不可继续虚构nr_entries=129已正常发布。表名与源码长注释不证明真实固件遵循本地类型/容量规则。

**交付矩阵**：facts内 `tag_intake_matrix`、`ram_map_provenance_matrix`、`e820_copy_capacity_matrix`、`earliest_stop_matrix`；纯条件样本BM80-M01–M10。最早STOP按tag头读取、非前进/越界tag、ram_map源记录读、目标第129槽各自独立登记。没有实际内存损坏或真实启动失败的运行证据。

## G02/B｜类型过滤、PFN上界与直接消费者

固定time的`e820__end_of_ram_pfn()`是宏包装，传入的 `MAX_ARCH_PFN` 在本树写为 `MAXMEM >> PAGE_SHIFT`，且 `MAXMEM=(1UL<<MAX_PHYSMEM_BITS)`、`MAX_PHYSMEM_BITS=46`、`PAGE_SHIFT=12`（B01–B06）。这个宏没有整体括号，必须放到“函数实参”和函数内“`ulong max_arch_pfn=`”的真实消费上下文判断优先级；在声明的x86-64 64位unsigned long整数模型中，两处均得 `2^34` PFN，不是所有宏使用上下文均可无条件类推。

`e820__end_ram_pfn(limit_pfn)`对每个已发布的E820条目，仅 `E820_TYPE_RAM=1` 与 `E820_TYPE_ACPI=3` 进入端点计算（B07–B10）。`start_pfn=addr>>12`，`end_pfn=(addr+size)>>12`，其中 `addr+size` 在u64域先求值，有端点回绕的先行条件；右移对未页对齐尾端作向下取整，不是`PFN_UP`。start>=limit跳过，end>limit把last置limit并break，其余逐条取max，结束再以MAX_ARCH_PFN裁切（B11–B16）。这产出的是**最大物理页号边界**，不是RAM页数之和、更不是实际分配器free页数：例如两个4096字节RAM段[0,4096)、[12288,16384)只有2页覆盖，却得last_pfn4。对合法、非回绕端点的纯最大值，不要求输入按地址排序；这不证明实际输入已排序、无重叠或完成规范化。

`setup_arch`把返回值赋给`max_pfn`、`max_possible_pfn`，再赋给`max_low_pfn`并形成`high_memory=(void *)__va(max_pfn*PAGE_SIZE-1)+1`（B17–B19）。`__va`只是地址表达，未建立实际页表可读/可写证明；max_pfn0时乘法后减1和指针形成须另行处理。后续`zone_sizes_init`基于`max_low_pfn`填DMA/DMA32/NORMAL界限、调用free_area_init（B20/B21），这一步只是直接参数消费，不重审78的zone内部初始化。ACPI参加本函数上界，但G03并不把ACPI加入memblock.memory。矩阵 `pfn_limit_selection_matrix`、`typed_endpoint_matrix`、`upper_bound_consumption_matrix`，样本M11–M20；任何内存读/地址合法性之前置门失败即止。

## G03/C｜早期保留、brk及E820类别移交

`setup_arch`给出的源码顺序是 `early_reserve_memory → e820__memory_setup → (若先行调用成功) PFN/max_low/high_memory → early_alloc_pgt_buf → reserve_brk → myos_early_map_pgt_buf_into_directmap → e820__memblock_setup → init_mem_mapping → zone_sizes_init`（C15–C20/B17–B18）。这是函数内**条件性先后**，不是实际启动成功轨迹；不进入页表callee或外部启动镜像。

`early_reserve_memory`分别调用三次reserve：内核起点`__pa_symbol(_text)`、长度`(ulong)__end_of_kernel_reserve-(ulong)_text`；低端`[0,0x10000)`；BIOS预留`[0x9F000,0x100000)`（长度0x61000）。C01–C06引用真实实参和SZ常量。实际链接符号顺序、物理变换和每个reserve返回均未证；“提出保留请求”不等于“成功登记”。

`_brk_start`和`_brk_end`起始都指向符号`__brk_base`。 `extend_brk(size,align)`先算`mask=align-1`，检查关闭标志与`align&mask`，再对齐end、比较形成的地址与`__brk_limit`，随后返回旧end、增加end、清零指定size（C07–C12）。`align=0`不被`align&mask`直接拒绝，若落入后续算术则需要停在地址门，不能断言已运行BUG_ON或memset；非2幂align会使相应条件非零，具体机器后果未运行。合法size0或空brk只做隔离样本。 `reserve_brk`只在end>start时调用`simple_mmblk_reserve`，忽略int返回，随后无条件`_brk_start=0`（C13/C14）；状态清零不是reserve成功的验证。

`e820__memblock_setup`逐项取表中记录的类型，先做`u64 end=addr+size`，再判断`end!=(resource_size_t)end`（C21/C22）。本树`resource_size_t`是`phys_addr_t`别名，而`phys_addr_t`是u64（C29/C30），所以同宽强转比较不能识别此前u64溢出。通过门后：`SOFT_RESERVED=0xefffffff`提出`simple_mmblk_reserve`；`RAM=1`提出`simple_mmblk_add`；ACPI、RESERVED和其余类型不加入memory，亦不会因参与B的max_pfn就被视为可分配（C23/C24/C31）。两个API分别转给`memblock.reserved`和`memblock.memory`，返回码在这里未消费（C26–C28/C33–C34）。循环后调用`memblock_trim_memory(PAGE_SIZE)`（C25/C32）；如何合并、尾部越界、逆序指针、total_size改变等**仍受79未闭合条件**限制，不能由80的调用入口洗白。类型表可重复、重叠或乱序的真实范围由输入producer证明，超出本包。

交付矩阵 `early_reservation_matrix`、`brk_transition_matrix`、`e820_type_handoff_matrix`、`cross_layer_boundary_matrix`，样本M21–M31。条件候选BM80-F01–F08与缺口BM80-U01–U10只标`needs_evidence`，不提供补丁、不请求Owner逐项选择修复。

## G04范围声明及审核边界

本批新增**92条源码短锚**（D7/A29/B22/C34）、**31组隔离有限模型**（A10/B10/C11）、11份矩阵共61行、8项候选和10项证据缺口；数字相互有重叠，不能相加为更多独立运行见证。来源清单列于facts；全部候选均未在真实内核重现。D00补正的7条占本92条，不与历史79的95标签混账。

完整本地P9、内核/用户程序运行、ABI确认、链接实际值、页表映射测试、Graphviz：**NOT_RUN**。本次完成六件的新文件/引文/元数据一致性闸门后才允许推进PR17原执行头；执行者不得签闭合R79-01/02或本80独立通过。交付后的唯一正式PR17评论及固定提交由真实发布回读记录，完成即 `STOP_WRITING`，不启动81。
