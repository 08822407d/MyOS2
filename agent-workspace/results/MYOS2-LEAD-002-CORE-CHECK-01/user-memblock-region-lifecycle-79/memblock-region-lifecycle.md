---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-MEMBLOCK-REGION-LIFECYCLE-79
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
model_per_owner: GPT-6 + Extra
backend_identity: unknown_or_not_attestable
source_pin_short12: a039d9803ade
contract_pin_short12: ed8b3ac237f6
initial_execution_head_short12: 03519ebb5d86
read_channel: connector
runtime_evidence: NOT_RUN
review_receiver: MYOS2-A-C06
mainline_acceptance: PENDING
---

# 第79批：memblock memory/reserved 区间的注册、变形、查询与分配

## 0. 方法和资格

**限定对象**：固定 `time@a039d9803ade` 的 `mykernel/mm/early/memblock.c`、`memblock.h`、`memblock_macro.h`、`memblock_types.h`、`memblock_const.h`，以及邻接定义；共11份本批**有新引文**的源文件。不能把Linux同名函数习惯、master现行版本或先前作者的结论当作本批源码。95条本批短锚分为D13、A23、B24、C35；均源于真实、同一作用域定义内的连续1–5行。额外读过的MyOS2规范和历史结果不是新增源码引文。

**证明阶梯**：先验证函数体、宏实际活动行、结构字段、类型与字面常量，再声明 *条件性的局部执行顺序*；遇到无效对象/越界指针、容量未知、整数截断、指向已删槽等，停止证明。后文的数值表只能在另行指定的合法抽象状态下计算，不证明C代码走到了那里。独立审查者不得把预期规范、注释声称或整段函数未执行，提升为运行实证。预处理、编译、链接、旧脚本、本地P9、Graphviz、QEMU、实机、并行竞争、修复补丁均 **NOT_RUN**。

## D00｜第78批R78-01文档纠正（追加，不改原件）

第78批独立回执仍是 `RETURN`，只涉及 `R78-01` 文档问题；**本批提交者只声明 `DOCUMENTATION_CORRECTION_SUBMITTED_PENDING_C06`**，不能自行宣称78 PASS或R78-01闭合。旧77批 `R77-01` 已由C06关闭，77组合 `PASS_PENDING_LOCAL`；不得重新返工。第78批的六件成果与PR17既有交付评论6825390768保持原样，其原136条定义标签（A43、B44、C49）亦保持历史计数。替代阅读时使用以下映射。

| 旧78证据和问题 | 必须采用的纠正口径 | 本批证据 |
|---|---|---|
| A31把 `#ifndef __va` 与真正 `#define` 混成一个「宏定义引文」 | **只把实际定义行** `#  define __va(x) ((void *)(ulong)(x) + PAGE_OFFSET)` 算作定义；`#ifndef __va` 是在引文之外的选择条件，实际预处理路径仍未知 | D01 |
| 图 A_ZERO把普通 `memblock_alloc_internal` 同直接 `alloc_node_mem_map` 混同 | internal在首次物理0且min非零时先降下界重试；仍为0时**返回NULL**，只有非零才 `phys_to_virt`。直接alloc_node_mem_map却是先转物理零再判断虚拟map，此处风险PB78-F02仍是条件性 | D02、D03、D04；旧A30/A31/A33 |
| B03与B04/B05及B40/B41/B44的调用、初始化与后置覆盖混读 | B03仅为setup_arch调用 `zone_sizes_init`；`free_area_init`先清边界数组(B04)，后清pgdat(B05)，计算zone统计并分配mem_map、初始化core、再调用memmap_init(B44)。B40是内核入口先setup_arch，B41稍后mm_core_init；不能以B40证实zone赋值 | D05、D06、D09、D10 |
| B06、B10、B11/B12空node和无交集分支 | B06空node时立即return，B11的caller局部spanned/absent未初始化而B12后续读取。**D11另外标识「无交集」提前return**，该分支同样不足以证明全部出参被写；B10只对应满足正常分支时计算spanned/absent的片段，不是它们的初始定义 | D11，及旧B06/B10/B11/B12 |
| B12/B13和B18/B19的生产者与覆写者 | B12的 `real_size = spanned - absent` 是计算、**B13**才是zone的 `start/spanned/present` 写入，**B19**是后来的 `zone->present_pages=0`，**B18**是memmap_init循环起点。PB78-F05必须交叉引用 **B13+B19+B44**；不再错用B12+B18+B40 | D07、D08、D09、D10 |
| facts两处把zone present overwrite误称PB78-F06 | 两处均应是 **PB78-F05**。**PB78-F06**只指 `memblock_free_all` 未初始化的 `pages` 在首个非空PFN区间自加时被读取（旧C24/C25/C26）；与zone present的覆写无关 | D12、D13 |
| 旧78的数值样本和文档总数 | 原32条模型中 **31条**是源条件/整数模型，**PB78-M32**只核77文档计数121+1=122；不得称作32条内核模型，不增加源码事实 | 旧78 facts 的 model分类；本批只追加此解释 |

**R78-S01/S02/S03 的消歧消费**：B13、B14是计算完成的zone与node统计生产者；B15只是被注释的 `spin_lock_init`，B16是core读取前值，B17是 `zone_init_internals` 与空区分流；B18–B22属于后续 `memmap_init` 与每页 `__init_single_page`，不能用它们来证明前面的统计已正确有效。B40→B41是调用链先后，B44提供 `alloc_node_mem_map→free_area_init_core→memmap_init` 的局部调用次序。PFN/span/空节点/洞/Reserved的逻辑前提仍开放；第78批发现PB78-F01至F06一概不因改引文而变成运行缺陷或验收通过。

## G01｜A：memory/reserved 的初始记录与区间注册入口

两张独立表的计数 `cnt`、容量 `max`、`total_size` 和指针 `regions` 是不同概念（A01–A07）；起初memory最多静态128槽、reserved默认256槽，两个 `cnt=0`。单个记录的 `base` 与 `size` 均为物理**字节**区间 `[base,base+size)`，`flags` 独立（A03/A04）。`simple_mmblk_add` 把请求送到memory；`simple_mmblk_reserve` 送到reserved；二者 `flags=0`（A10/A11），使用**同一** `simple_mmblk_add_range` 变形器，并不直接凭名字保证无重叠。

进入函数先执行 `size=min(size,PHYS_ADDR_MAX-base)`，再计算end；`size=0` 或裁剪成0时直接返回数值0，即 `ENOERR`（A08/A09/A20/A21）。这只是零操作成功，不是插入成功。宏 `for_each_memblock_type` 迭代到 `idx < cnt+1`，所以当 `cnt < max` 时会**访问容量内但不一定逻辑有效的尾槽**；当 `cnt==max` 时，尾槽读可越界（A12/A14）。尤须注意循环第一次 `idx=0` 就在 `idx>0` 检查**之前**构造 `prev=rgn-1`（A13）。严格C对象边界下该形成不应被视为合法，故本批**在这里停止对实际注册链的到达证明**；后续例子皆为隔离的区间代数/假设合法模型，不能倒过来认证真实执行。

在隔离模型中，旧区间完全位于新增base前会跳过；新段在旧区间前或走到尾槽时使用 `end-base` 插入；而 `rbase<end` 分支使用 `rbase-base`（A14–A17）。旧[10,20)与新[15,25)取到数学差 `10-15=-5`，若转成64位无符号 `size_t` 即 `2^64-5`，**不是**应加入的正确区间长度。旧[10,20)与新[12,18)也得 `2^64-2`；不能在此之后宣称正确去重、排序或reserve成功。内层调用 `memblock_insert_region` 后立刻以 `idx-1` 调merge，最后才有 `cnt>=max-2` 检查（A18/A19），这不是事先预检加事务回滚。

## G02｜B：insert / merge / remove 的逐次状态变形

`memblock_insert_region` 先形成 `&regions[idx]`，才检查 `cnt>=max`；随后 `memmove(rgn+1,rgn,(cnt-idx)*sizeof(*rgn))` 向右挪动，写 `base`、`size`，递增 `cnt` 和 `total_size`（B01–B03）。`flags` 虽作为形参传入，却**未在本函数有效正文中赋值给新记录**。这在复用残留槽并含非零flags时可能影响合并或NOMAP等过滤；当前两个公开包装器传0，不能把“已有非零残留”冒称已在真实启动发生（B21、A22）。

`simple_mmblk_merge_rgn` 的 `start_rgn` 是 `ulong`，其实现先令 `int i=0`，在start非零时赋 `i=start_rgn-1`；end则钳制至 `cnt-1`（B04/B16）。idx0传入 `idx-1` 会经历负int到ulong、再到int的跨类型转换，最终 `i` 结果依赖实现，不得简单假设为0或安全退出。有效配对时先检查端点相接且flags相同，不匹配则可能触发重叠检查；相匹配则左块size增加、total_size暂加右块size，然后调用 `simple_mmblk_remove_rgn`（B05–B07）。删除先扣被删块size，`memmove` 左移，再 `cnt--`，尾槽**不清空**（B08–B10）；不能凭 `memmove` 支持重叠，就推导移动范围与计数合法。

**有限桥接反例**：旧表依次[10,20)、[30,40)，新增[20,30)。假设全部边界合法，插入于idx1使数组三项；merge(start0,end1)合并前两项，并缩短其end扫描范围，可能留下 `[10,30), [30,40)` 两个相邻但没有继续合并的项。这里的“可能”描述给定的 **isolated merge-body** 数值，不是按C函数已经合法执行到此。另一个可独立复核的状态转换是：`cnt=1` 时删除唯一项，memmove计数0、`cnt=0`，其 `regions[0]` 可仍保存旧区间。由此引出G03空搜索的同一“cnt/尾槽”问题；不要把同一根因裂变成多个已确认故障。

## G03｜C：裁齐、成员查询与候选分配边界

**倒序裁齐**。宏 `for_each_memory_region_topdown` 初始索引是 `cnt-1`，因此 `cnt=0` 不能合法推出空迭代；`cnt=1` 迭代后仍可能把指针减到数组前并比较（C01）。`memblock_trim_memory` 用 `round_up(orig_start,align)` 和 `round_down(orig_end,align)`；两个宏只在2的幂对齐及整数可表示等前提下才有预期作用（C02/C05/C06）。例如[3,14)按4向内裁齐得[4,12)，旧size11改成8，却不同时调整 `memblock.memory.total_size`：在该分支的存储状态可出现11对8（C03）。若裁齐后start不小于end，会调用删除再于函数体内 `r--`，倒序宏自己的更新又 `r--`；删除原索引2时可能跳过索引1（C04）。这不证明真实内存表已被破坏，只证局部运算存在未闭合的边界。

**查找与成员资格**。`memblock_search` 以 `right=cnt` 初始化，却使用 `do…while`；即使 `cnt=0` 仍可能读取 `regions[0]`，如果尾槽残留[10,20)，查询15在隔离C状态中会返回0（C07–C10）。`memblock_is_memory`、`memblock_is_reserved` 消费此结果，而 `memblock_is_map_memory` 在命中之后再判NOMAP（C11–C14）：**是否在表内**、**是否物理RAM**、**是否mappable**、**是否未被reserved占用**不能混成一个布尔谓词。`memblock_is_region_memory` 以base搜索单个memory槽，裁剪请求size后比较其end（C15/C16），不保证多段覆盖、原本溢出的完整请求或合法直接映射。

**候选与预留**。`for_each_free_mem_range` 从memory减reserved，由双游标与 `should_skip_region` 的HOTPLUG/MIRROR/NOMAP/DRIVER_MANAGED筛选产生半开交集（C17–C25）。其中 `__simple_next_mem_range` 在终端 `idx_b==cnt` 时可以形成尾槽指针，却以条件表达式取 `PHYS_ADDR_MAX`，**不在该分支解引用尾槽base**；这与A组 `for_each_memblock_type` 在尾槽读取rbase/rend有所区别。输入必须有有效数组、排序、区间关系和无越界算术才能推断扫描正确。

`__memblock_find_range_bottom_up` 先把free区间钳到请求[start,end]，随后按align上取整并检验 `cand<this_end && this_end-cand>=size`（C26–C28）。例如free[100,200)、size16、align16，候选112；free[100,110)没有这种候选。这是**整数模型**，不是实际物理布局。 `memblock_alloc_range` 对align0改用 `SMP_CACHE_BYTES`，对end==0或1改用current_limit，强制start至少PAGE_SIZE；随后仅在非零candidate**且** `simple_mmblk_reserve(found,size)` 返回0时输出物理地址，否则返回0（C29–C33）。`size=0` 是独立的条件性缺口：若有非零候选，`simple_mmblk_reserve(found,0)` 立即返回ENOERR但不插入记录，上层可能收到非零地址而没有预留字节。未核实真实调用者是否传0。

最后，`memblock_alloc_internal` 首次失败可放弃min下界重试，但`memblock_alloc_range` 始终施加PAGE_SIZE物理下界；internal第二次仍为0则直接返回NULL，不会把0先转成高端虚拟地址。此前78批直接调用的 `alloc_node_mem_map` 另有不同风险，不在本批把二者合并（D01–D04、C34–C35）。

## 条件模型示例与最早停止门

| 情形 | 隔离后的数值或状态 | 最早不能越过的证明门 |
|---|---|---|
| 首次注册且cnt0 | idx0循环体被选中，prev按代码形成region[-1] | C指针对象界限，不能认证随后的插入 |
| old[10,20)，new[15,25) | `rbase-base=-5`，64位unsigned值 `2^64-5` | 错误大小不能用来证明预留成功 |
| idx0→merge | 参数 `idx-1=-1`，转ulong为最大值，再赋int | C整数转换及循环边界 |
| 删除唯一[10,20) | cnt1→0，slot0可能残留 | 不能再把slot0当active member |
| cnt0且slot0残留[10,20) | `search(15)` 由于do先进入可能返回索引0 | empty-search的逻辑有效数不等于槽可读 |
| trim [3,14) / align4 | 写[4,12)，原total11仍可能为11 | 对齐/计数不变量 |
| trim倒序删末项 | 函数内r--与宏r--导致跳过前一项 | 当前ptr合法性和下一次迭代 |
| free[100,200)、size16 align16 | candidate112 | reserve写入能否保持无重叠/合法表仍未证 |
| alloc size0 | nonzero candidate与零字节reserve返回0可同现 | 返回非零不蕴含有效保留 |
| internal第一次0、min非零 | 放下界再试；仍0→NULL | 不能宣称此分支对物理0直接做__va |

全部30个限定模型的输入、预期、锚点和停止条件见 `facts-and-boundaries.yaml` 的 `finite_model_cases`；不是30次运行，也不冒充经真实配置编译的C重放。

## 范围限制、历史继承及交付

旧 `CORE-MM-BASELINE-04::MC-01/MC-05/LB-01/LB-08/GAP-M08`、`CORE-USER-PAGE-ALLOCATOR-LIFECYCLE-77::PA77-F01/F02`、第78批PB78-F01–F06均保留原qualified_id与历史限定，特别不能从本批memblock区间推导buddy合并、全树PageBuddy值、真实内核启动成功或成功物理回收。新候选 `MR79-F01–F08` 与 `MR79-U01–U10` 均为 `needs_evidence`，不是已复现故障；详见 `deferred-findings.yaml`。本批未引入修复候选选择、用户操作、需要人工代码审查或自动部署；Owner当前无需手动执行技术操作。

仅当六件新增文档严格挂到原PR17执行分支、核对单提交、全文与blob完整回读、本人唯一正式PR17评论发布并核唯一性之后，身份 `MYOS2-LEAD-002-CHAT-EXEC-10` 才进入 **STOP_WRITING**，并由 `MYOS2-A-C06 内核分析主线续建（Pro）` 独立判断R78-01与本批79是否接受。即使文本达到静态取证要求，也只允许 `PASS_PENDING_LOCAL` 上限，不自签验收、不启动80。
