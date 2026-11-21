---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FRAMEBUFFER-MAPPING-96
record_type: bounded_framebuffer_metadata_mapping_and_return_contract_static_report_with_D00
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-13
executor_display_name: MYOS2-A-C06d 代码分析执行
produced_by: GPT-6（本会话可见家族，后台UNATTESTED）
model_per_owner: GPT-6＋Extra（Owner指定次一档）
date: 2026-11-17
base_snapshot: time（分支名）
read_channel: connector
status: AUTHOR_STATIC_COMPLETE_PENDING_C06_PRO
D00: CORRECTION_SUBMITTED_PENDING_C06_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions: [FM96-U01, FM96-U02, FM96-U03, FM96-U04, FM96-U05, FM96-U06, FM96-U07, FM96-U08, FM96-U09]
---

# 第96批：帧缓冲生产、物理请求与实际映射接口

## 结论、边界和第95批纠正状态

**本批完成静态源码调查和第95批定点纠正的追加提交准备，但旧95审查仍为RETURN（仅R95-01/02），纠正只有原C06主线有权签收。** 固定源码唯一为 `time@a039d9803ade`；UEFI loader也只读这个固定提交，不把master搜索结果当成当前实现。新分析不修改内核、旧95六件、PR元数据或既往评论。

三个专题的核心区别：

1. loader提供帧缓冲地址、宽高、bpp、pitch及类型字段；kernel把其解析进 `framebuffer_s`。但**中间表达式的整数宽度与赋值目标宽度不同**，`FB_size` 虽是64位字段也不能自动消除先前32位乘法回绕。pitch和每行像素数还必须按真实公式分账。
2. `myos_init_VBE_mapping` 把物理基址向下对齐，又**单独**把长度向上取整；未把原始物理基址的页内偏移计入所传长度。对于不对齐且长度为整页倍数的样例，本次请求的数学区间可以漏掉原帧缓冲末尾。请求有界不证明真实页表覆盖。
3. `myos_ioremap` 固定活动体选 `PAGE_KERNEL_NOENC` 而非注释的 `NOCACHE`；它调用底层映射函数并把其**物理终点整数**强转为 `void __iomem *`。VBE根本不消费该返回值，另一个已读HPET调用者会保存它。保护编码、页表旧项继承、根切换、TLB请求以及真实硬件缓存效果属于不同证明层级。

结构化成果在 `facts-and-boundaries.yaml`：**8组55行事实矩阵、FM96-M01–M23共23个独立有限模型、110条固定源码短锚索引**；`deferred-findings.yaml` 有 **5项有条件候选FM96-F01–F05、9项未证缺口FM96-U01–U09**（不是14项实机故障）；`framebuffer-mapping-paths.dot` 有39节点58分类边，未运行Graphviz。完整本地P9、内核/loader的预处理编译链接、仓库脚本、QEMU、实机、CPU缓存、多CPU同步与真实屏幕显示均为 `NOT_RUN`。

## D00｜R95-01：注册接口的活动flush与消费者纠正

固定 `mykernel/printk/printk.c::register_console` 先查 `list_header_contains`，同一已入链 `node` 会在函数开头直接 `return`；新节点才调用 `list_header_add_to_tail`，之后跨越大段 `//` 注释到达**活动**语句 `console_unlock();`。此前95正文将注册概括为“只入链、不调用flush”，且将该处console_unlock当作注释，是明确错误；旧mainline R95-01成立。（FM96-E001–E006）

正确调用关系是：

`register_console → console_unlock → call_console_drivers → _call_console_drivers → __call_console_drivers → con->write`，最后一个是**有条件间接回调**；只有合法注册且遍历到内置 `myos_tempcon`，`con->write` 才会间接选择 `myos_tempcon_write`。不把 `register_console` 直接画一条“CALL myos_tempcon_write”，也不把旧 `CON_PRINTBUFFER`、`con_start=log_start`、`exclusive_console=newcon` 等注释掉的**指定对象回放**当成当前实现。当前活动flush使用已有 `con_start/log_end`，可能处理此前积压；也可能空转。（FM96-E002–E014）

| 注册/积压的独立有限输入 | 正确源级结果 | 最早限制 |
|---|---|---|
| 新节点，`con_start==log_end` | **进入**console_unlock，然后因无积压退出；无write | 列表及调用前提 |
| 新节点，有完整LF行 | 活动flush可在register返回前移交，触达回调 | 环前缀、线性范围、有效write、像素对象 |
| 新节点，只有X而没有LF | 先推进con_start，扫描后没有尾部write | 旧 `PK94-F04` 仍在 |
| 同一node重复register | contains为真、直接提前return；**不执行本次flush** | 前次链表拓扑保持有效 |
| 另一个假设console.write为NULL且有LF | 入链不筛write，后续间接调用无有效回调前提 | 实际内置tempcon.write静态非NULL，不能假定此对象是NULL |

**跨95消费映射**：`A-REG01/03/04/05/06` 从“入链＝全部活动行为”改为“入链后可能flush”；`A-LIST04` 在“计数影响printk可用性”之外补注册自身flush；`C-BOOT05/06` 正确区分视频函数内 `pr_info`（登记前可能入环）和随后 `register_console` 的**首次可能移交**；`ABC-01/02/04` 补入注册到回调边。有限模型 `CN95-M01/M03/M04` 追加flush条件，`CN95-M02` 同node提前return结论保留；追加了本批 `FM96-M01–M05`。图补 `reg→console_unlock` 活动CALL及逐层间接回调链；旧 `CN95-U02/U07`、`PK94-U03/U07` 的未证设备与其它console对象仍保留。完整见facts的 `D00_R95_01_02_consumer_repair_matrix`。

本纠正**撤回旧95“注册不主动flush/只能等待注册后pr_info”的概括**，不反向声称“注册后一定成功显示”，不替主线关闭R95-01。

## D00｜R95-02：三组定义锚和历史计数纠正

| 95旧锚 | 问题分类 | 本96正确取证 | 实际消费者 |
|---|---|---|---|
| `CN95-E045` | 将宏前说明以及 `PAGE_SHIFT`、`PAGE_SIZE`、`PAGE_MASK` 三个独立宏归作一个PAGE_SIZE定义 | `FM96-E015`、`E016`、`E017`，**各一宏一锚** | `B-FMT02–05`、`CN95-M14/M15`、`CN95-F04/U04` |
| `CN95-E064` | 引的是 `for_each_console` 宏前说明文字，而非活动宏定义 | `FM96-E013` 为真实 `for_each_console`；`E014` 为下级 `list_header_for_each_container` | `A-LIST04/LIST07`、`ABC-02`、`CN95-U03` |
| `CN95-E050` | `font.h` 的 `extern unsigned char font_ascii[256][16]` 只是声明，不是对象定义 | `FM96-E018` 重新锚 `font.c` 中真实数组定义；旧 `CN95-E051` 仍是有效定义旁证 | `B-BYTE02`、`C-GLYPH01`、`CN95-M07`、`CN95-U06` |

旧95 `MANIFEST` 中“70条全部单一定义内均通过”的总括应**撤回**，不能用“70次字符串命中”证明P2的单一定义正确。**原95的70/70/0为冻结历史原始标签/原作者复核声明，保持原样、不覆盖、不冒称当时就检查正确**；第96批自己的110条证据和三整数是另一个新账。`CN95-E023` 尾部空白不计错误；不恢复53曾撤回的空行误判。R95-02只是**已提交纠正、待主线独立验收**，不是本作者自签CLOSED。

## A｜帧缓冲元数据：来源、实际字段类型、单位和计算顺序

### loader写入与kernel消费

`myloader/uefi/bootloader.c::UefiMain` 的活动源码先 `set_video_mode(ImageHandle,-1)`，分配并初始化MBI标签存储后调用 `fill_framebuffer_info`，再写其它标签。set_video_mode对当前/候选Mode、Info->PixelFormat、分辨率与SetMode有条件筛选，但已读范围没有外部UEFI操作结果。不能据此保证屏幕真的选中某一格式或服务返回成功。（FM96-E019–E029）

该固定loader里的FRAMEBUFFER公共结构，物理地址类型为 `multiboot_uint64_t`，pitch/width/height为 `multiboot_uint32_t`，bpp与framebuffer_type为 `multiboot_uint8_t`。别名在UEFI分支分别写为 `UINT64/UINT32/UINT8`，内核最终使用 `framebuffer_s`：`phys_addr_t FB_phybase`、`virt_addr_t FB_virbase`、`uint64_t FB_size`、三个 `uint32_t`（PixperScanline、X/Y_Resolution）。**字段名所称pitch未必是最终消费者所用单位**；必须按两边算式核对。（FM96-E030–E046）

关键生产与消费表（宽度只按明确的固定类型和附加目标ABI前提使用）：

| 信息 | loader真正写入的表达式 | kernel真正保存/解释的表达式 | 首个证明门 |
|---|---|---|---|
| 物理地址 | `Mode->FrameBufferBase`→64位common.addr | `FB_phybase=common.addr`；`FB_virbase=phys_to_virt(FB_phybase)` | 物理/虚拟值转换不证明映射存在 |
| 宽、高 | `HorizontalResolution/VerticalResolution`→32位字段 | `X_Resolution/Y_Resolution`→32位 | 未读真实GOP Mode，不证明像素区域 |
| bpp | `Mode->FrameBufferSize*8/(width*height)`→**8位** | 读 `common.framebuffer_bpp` 参与其它算式 | 分母width*height可能32位回绕或为0；中间类型与窄赋值分开 |
| pitch | `PixelsPerScanLine*common.bpp/8`→32位common.pitch | `PixperScanline=common.pitch/(bpp/8)`→32位 | 假定bpp为8整倍数、所有中间乘法有效且分母非0 |
| FB_size | 没有直接从FrameBufferSize转存为此kernel成员 | `width*height*bpp/8` 再存 **64位FB_size** | width和height已是32位，不能因目的u64就认为前面乘积是u64 |
| framebuffer_type | `Mode->Mode`→8位字段 | 当前 `framebuffer_s` 未储存type | Mode序号不是同一源码变量 `Info->PixelFormat`；实际契约/消费未证 |

**长度的条件性反例：** 给定内核字段width=16384、height=8192、bpp=32，数学可见字节预算是536,870,912（512MiB）。但在**明确uint32中间乘法**模型中，`width*height=134217728`、再乘32得 `4294967296`，在32位域模`2^32`回绕为0，随后`/8`仍0，最后赋值到64位`FB_size`还是0。目标64位字段不会追补丢失的高位。该模型不是已观察到的分辨率/实机错误，且loader的前序计算、有效完整tag和特定编译器类型模型都必须先过门。（FM96-M08；新 `FM96-F01/U03`）

**行距的独立反例：** 800×600，32bpp，`PixelsPerScanLine=832` 时，条件数学上 `common.pitch=3328` **字节/行**，内核 `PixperScanline=832` **像素/行**；但内核 `FB_size=800*600*4=1,920,000` 字节，不等于含padding的 `3328*600=1,996,800` 字节。旧95 `CORE-USER-CONSOLE-REGISTRATION-OUTPUT-95::CN95-F05/U05/U06` 的消费者像素跨度问题继续保留，不重签为本批新根因。（FM96-M07）

同时，`Mode->Mode` 是被写到8位 `framebuffer_type` 的实际值，而本loader的像素格式筛选条件读 `Info->PixelFormat`；这两者在固定源码中不是同一个字段。是否确实违反应用契约还缺UEFI返回、设备和下游解释证据，暂作为 `FM96-F04` 条件候选，不能把RGB宏常量和Mode序号等同。若bpp小于8，kernel `bpp/8=0` 会成为PixperScanline的除数，先在这道算术门停止，不推导后续绘字。（FM96-F05）

`parse_tag` 只有在有效FRAMEBUFFER tag分支内调用 `get_framebuffer_info`，不能用loader里存在填充函数推定同一内核启动已经收到了合法tag。旧 `CORE-USER-BOOT-MEMORY-MAP-80::BM80-U02/U03` 的tag原始范围、total_size与实际可读对象限制继续有效；这不是第80批重做。（FM96-E045/E046）

## B｜启动激活、物理请求区间与原始页内偏移

### 实际call路径与顺序

固定 `start_kernel` 先调用 `setup_arch`；后者通过内存早期初始化执行 `init_mem_mapping`。在 `init_mem_mapping` 里，初始化必要RAM映射后活动调用 `myos_memory_map`，它又活动调用 `myos_init_VBE_mapping`，VBE调用 `myos_ioremap` 及 `flush_tlb_local`。随后 `init_mem_mapping` 才执行 `load_cr3(swapper_pg_dir)`、`__flush_tlb_all()`；返回后 `start_kernel` 后续才调用 `console_init` 及视频绘制。这是**固定源中的实际调用顺序**，不是已观测的启动轨迹、物理内存映射成功或活跃CR3身份。（FM96-E049–E055/E081–E083）

### 数学区间与C语言停止门

设 `P=4096`（固定`PAGE_SHIFT=12`），`A=framebuffer.FB_phybase` 是原始物理**字节**地址，`L=framebuffer.FB_size` 是数学上的原始长度；`o=A mod P`。VBE真正传入的参数为：

[
B=Plfloor A/Pfloor,qquad S=Plceil L/Pceil .
]

`myos_ioremap(B,S)` 的活动实现再次用 `PFN_PHYS(PFN_UP(B+S))` 求端点；在中间加法、PFN_UP里的`+P-1`、`__va`、物理页范围及对象全部合法的**隔离整数模型**中，该端点是 `B+S`，于是请求区间为 `[B,B+S)`。而原先期望覆盖 `[A,A+L)`，正长度时必要条件是 **`o+L<=S`**。将起点向下对齐和长度向上取整，不会自动补偿原始偏移。（FM96-E015–E017/E049/E050/E079/E084–E087）

| 有限输入 | 实际数学请求 | 与目标的关系 |
|---|---|---|
| `A=0x1000,L=4096` | `[0x1000,0x2000)` | 与目标相等 |
| `A=0x1003,L=4096` | `[0x1000,0x2000)` | 目标末端0x2003，漏3字节 |
| `A=0x1003,L=100` | `[0x1000,0x2000)` | 可覆盖目标`[0x1003,0x1067)` |
| `A=0x1fff,L=2` | `[0x1000,0x2000)` | 目标末端0x2001，漏1字节 |
| `A=0x1003,L=0` | `S=0`，原正长度目标为空 | 没有新增正长度覆盖保证 |
| `B=2^64-4096,S=4096` | 若`ulong`为64位，`B+S`回绕为0 | 必须在整数域停止，不继续按无限整数解释页表调用 |

即便某次数学请求完整覆盖目标，也仍缺：loader帧缓冲真实物理大小、页表已有PTE/大页、下级页分配、active root、TLB状态和最终硬件可写性。旧81–83的 `PT81`、`DM82`、`TI83` 合同限制必须继续消费；不把请求区间当真实已构建页表。（FM96-F02/U05）

## C｜myos_ioremap实参、返回、属性与TLB

### 真实调用形状不是通常猜想的通用I/O重映射

固定 `mykernel/arch/x86_64/mm/init.c::myos_ioremap` 接口是 `void __iomem *myos_ioremap(size_t paddr_start, ulong size)`，VBE传入向下取整的物理起点和单独向上取整的长度；**活动体**调用：

`__kernel_physical_mapping_init(paddr_start, PFN_PHYS(PFN_UP(paddr_start + size)), 1 << PG_LEVEL_4K, PAGE_KERNEL_NOENC, true)`

再将返回值强转 `(void *)` 返回。函数上方 `// return __myos_kernel_physical_mapping_init(...,__PAGE_KERNEL_NOCACHE)` 是注释，不是当前执行目标。固定枚举中 `PG_LEVEL_4K` 在NONE后，故掩码仅请求4K候选，而非给新叶同时打开2M/1G。**不据此保证最终所有现存映射都变为4K页或替换保护位**。（FM96-E078/E079/E088）

当前保护宏 `PAGE_KERNEL_NOENC` 经 `__pgprot_mask(__PAGE_KERNEL|0)` 形成，`__PAGE_KERNEL` 含源码给定的present/rw/accessed/nx/dirty/global组合但没有显式 `_PAGE_PWT/_PAGE_PCD`；`PAGE_KERNEL_NOCACHE` 来自另一宏 `__PAGE_KERNEL_NOCACHE` 并包含`__NC`，`_PAGE_NOCACHE=_PAGE_PWT|_PAGE_PCD`。固定`_PAGE_ENC` 在本源码定义成0。**所以当前NOENC不等于NOCACHE的源级属性编码。** 这只能证明所选位构造，不能不读CPU的PAT/MTRR、已有映射属性、PGE/PCID和真实页面硬件状态就宣布实际缓存一定是WB/UC/WC。（FM96-E089–E100；`FM96-U06`）

### 下层页表写入与返回单位

`__kernel_physical_mapping_init` 以物理起点、终点和保护对象进入；先按 `__va(paddr_start/end)` 求direct-map预定虚拟地址，再按PGD/宏 `phys_p4d_init`（实际展开到`phys_pud_init`）等逐层操作。根存在和缺失路径不同；PUD/PMD原有nonleaf向下递归、原有leaf可复用或按掩码拆分、新表从alloc_low_page取得，最终PTE已有值可略过，新值才由 `pfn_pte` 和 `set_pte_init` 写入。这些是第83批互斥分支限制的延续，不是一条所有节点必依次新建的无条件流程。（FM96-E056–E075/E098–E110）

底层返回类型为 **`ulong`**，返回变量 `paddr_last` 初值为传入的物理 `paddr_end`，新4K写入后可能更新为物理页末端，复用已有页项时可以不更新；其他分支也可能形成不相同的末端。根据 `R83-S03`，它既不是所有新旧PTE物理区间的并集，也不是统一成功码。外层 `myos_ioremap` 仅把这个**物理整数终点**强转成 `void __iomem *`，未调用 `__va` 转为实际预定direct-map虚拟首地址。VBE把返回值直接丢弃；本次只补查一个实际固定其他caller：`hpet_set_mapping` 把该返回赋给 `hpet_virt_address`，但此处不能再扩展为HPET驱动运行故障报告。（FM96-E069–E080/E104/E105；`FM96-F03/U08`）

`iounmap` 确有函数定义，但活动体是空的；本包只记“当前所读函数没有解除页项的操作”，**不**据函数名断言有真实unmap，也不声称未读调用者不存在。（FM96-E080）

### 映射发布、局部flush和换根

VBE调用 `flush_tlb_local` 的固定活动体是 `write_cr3(__read_cr3())`。其前述抢占/中断提醒代码已注释。故静态上能够确认“代码发出了**读取并重写当前CPU CR3**的请求”，但不能证明当时的根恰是后来 `swapper_pg_dir`、写入的新页项已落在实际在用根、PAT/MTRR效果符合设计，或者其它CPU全局缓存已同步。`init_mem_mapping` 中的`load_cr3(swapper_pg_dir)`及`__flush_tlb_all`位于VBE之后；后者固定活动体又调用 `flush_tlb_local`，旧global PGE分支为注释，不是“所有CPU全部完成全局flush”凭据。（FM96-E050/E055/E101–E107；`FM96-U07`）

本包继续保留 `CORE-USER-EARLY-PGTABLE-STORAGE-81::PT81-U03`、`CORE-USER-DIRECT-MAP-RANGE-82::DM82-U07/U10`、`CORE-USER-DIRECT-MAP-TABLE-INSTALL-83::TI83-F03/F06/F08` 及对应R81/R82/R83-S01/S02/S03。它们分别限定内存分配、页表生产者/旧项、物理端点/页属性与实际CPU可见性，不能靠本次单个包装函数消除。

## 核查范围、模型和延期状态

`facts-and-boundaries.yaml` 的8组55行是：D00十行、帧缓冲loader八行、kernel消费者六行、启动五行、页偏移七行、ioremap接口八行、映射/刷新七行、A/B/C接合四行。共有 **23个**条件模型：D00五个，A的字段与算术七个，B的对齐与整数域六个，C的下级页/返回/缓存/刷新五个（逐项源于矩阵，不等于仓库程序测试）。实际计数可由facts内 `finite_models` 和 `matrices` 独立复查。

新发现 `FM96-F01..F05` 为5个不同有条件的根因或输入合同候选；`FM96-U01..U09` 为9个独立取证缺口。旧 `CORE-USER-CONSOLE-REGISTRATION-OUTPUT-95::CN95-F05/U05/U06`、`CORE-USER-BOOT-MEMORY-MAP-80::BM80-U02/U03` 与81–83 `PT81/DM82/TI83` 均沿**完整qualified_id**追加，不改旧元组、不合并不同类别。第94批 `PASS_PENDING_LOCAL` 及R94-S01/S02/S03不因95暂时RETURN而被推翻；第95批只退D00两个文档/语义项，其他R95-S01/S02/S03保留。90历史文档已闭合，53旧空行误判撤回并不等于原源码候选被修复。

本作者提交前对新 **110条**固定源码锚进行了两次独立定点回源、分宏/对象/函数定位；数值例只做非仓库实现的有界整数推演，不执行UEFI、内核C/汇编、固件、页表、真实并发或硬件。本批不出补丁，不自行修已发现缺陷，不启用额外代理/模型/API。

## 六件交付与停止边界

本目录仅有 `MANIFEST.md`、本报告、`facts-and-boundaries.yaml`、`framebuffer-mapping-paths.dot`、`deferred-findings.yaml`、`evidence.md` 六件。PR17作者本人只发一次新batch96正式顶层交付评论，不改变任何旧文件/评论/PR元数据/主线或内核。完整本地P9以及预处理、构建、运行、Graphviz、QEMU、实机、设备缓存/多CPU均为 `NOT_RUN`。所有写入需保持旧HEAD为单父、expected_sha、force=false；暂态错误先查远端是否已成功。

第96批作者交付状态 `READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`，R95-01/02仅`CORRECTION_SUBMITTED_PENDING_C06_REVIEW`。在六件固定及活动回读、严格6added/0modified/0deleted、四direct refs、评论全文与唯一性全部核实后 `STOP_WRITING`；将PR17交原“MYOS2-A-C06 内核分析主线续建”，Owner正式验收前选择**Pro**，同轮审核补正与第96批，并安排后继。不自签95闭合或96通过，不启动97。
