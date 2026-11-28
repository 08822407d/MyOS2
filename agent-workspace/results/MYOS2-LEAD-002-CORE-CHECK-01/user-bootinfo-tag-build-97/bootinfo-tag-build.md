---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BOOTINFO-TAG-BUILD-97
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
record_type: bounded_static_analysis_D00_and_A_B_C
produced_by: GPT-6（可见模型家族；后台UNATTESTED）
model_per_owner: GPT-6＋Extra（Owner指定次一档）
date: 2026-11-17
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: connector
status: AUTHOR_STATIC_COMPLETE_PENDING_C06_PRO
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第97批｜Bootinfo标签生产、EFI MMAP转换及ACPI/CPU标签构造

**执行许可与限界。** 本批严格以固定 `time@a039d9803ade` 为唯一源码；从138/139恢复的第97批，不沿用已被替代的136/137“寄存器复制/BSS”范围。独立接收并完成Mnemosyne程序、九项约束及P1–P13、110和124，G00检查固定lead、exec、master、time直接refs、PR16/17、两目录和两种正式marker均通过，再转入下列D00、A/B/C。没有运行项目、脚本、预处理、编译、链接、Graphviz、QEMU、实机，没写内核、loader或旧96；源码标签/有限模型不能代表实际平台故障。

## D00｜第96批R96-01/R96-02文档定点纠正（只在本97六件内追加）

1. **R96-01 FM96-E036定义边界：** 原短锚取 `myloader/uefi/multiboot2.h:320–323`，第320–322行在 `struct multiboot_tag_framebuffer_common` 内，末行323是**另一个独立typedef**：`typedef struct multiboot_tag_framebuffer_common mbi_framebuffer_common_s;`。旧标签的引用并非一段单定义内短引文。本批 `BT97-E001` 仅取struct尾部，`BT97-E002` 单取typedef，`BT97-E003` 独立取bpp 8位字段，`BT97-E006` 是loader实际赋mode值，`BT97-E007` 是RGB常量宏。96 producer类型索引、A-P06、FM96-M11、FM96-F04依次按其真正依赖字段、别名、赋值、常量分别关联；结构体末尾和空白不是错误，错误仅在跨入另一个定义。必须撤回旧96 `source_quote_recheck`、MANIFEST、evidence导语及相关“110个都通过单一定义”**语义全称声明**，不能将数量上的110/110/0改写成新时点的历史真相。
2. **R96-02 FM96-F05三个错位字段：** 旧 `distinct_root=[35,41,44]` 将证据后缀误作根因文本，旧 `evidence_ids` 拼成 `FM96-ECORE-USER-CONSOLE-REGISTRATION-OUTPUT-95::CN95-U05`，不是现存E001–E110证据ID；旧 `prior_related_qualified_ids=[]` 漏掉实际历史关联。本批在facts、deferred和矩阵重建相同 **`CORE-USER-FRAMEBUFFER-MAPPING-96::FM96-F05` 身份**：`distinct_root` 是kernel在有效bpp输入0–7时 `common->framebuffer_bpp/8` 形成零除数的独立输入域；旧正确支持链 `FM96-E035`（bpp字段）、`FM96-E041`（内核目标字段）、`FM96-E044`（消费表达式），新对照 `BT97-E003/E004/E005`；只有 `prior_related_qualified_ids` 放 `CORE-USER-CONSOLE-REGISTRATION-OUTPUT-95::CN95-U05`，解释它是另一行距/布局未知，**不是证据ID**。原FM96-F05及其它四候选、九缺口均不改身份，不新签内核已修。
3. **补正影响范围：** `facts-and-boundaries.yaml.D00_R96_01_02`、`D00_R96_01_02_correction_matrix`、`fact_to_evidence_id_index`、`deferred-findings.yaml.D00_R96_01_02`、本正文、证据和DOT里的 `fm97_struct/fm97_typedef/f05_src` 均沿用新关联；不能仅修一个字符串。原96的源引用 `FM96-E035/E041/E044` 本身真实，原错拼串禁止再当有效支持。原96五候选、九缺口和所有旧文件、旧顶层评论、原历史110/110/0保持冻结。该补正在新97提交**不等于主线已关闭R96-01/02**。
4. **审核保留项不重做：** R96-S01必须区分输入阶段：若外给800×600/bpp32/pitch832，则每行3328字节、按width预算1,920,000、按pitch预算1,996,800；但若另给loader的FrameBufferSize=1,996,800，loader公式会先产生bpp33、pitch3432，再进kernel；不能互混。R96-S02当前4K-only mask阻断“掩码允许直接复用旧1G/2M叶”快捷分支；NOENC和NOCACHE源编码包括NX等差别，均不当作实际PAT/MTRR结果。R96-S03两次CR3操作**时间点不同、根是否不同未证**；图中Mode跨函数是数据联系而非同一指针身份。这些仅按96正式回执保留，不在本轮重画96主体。第95批R95-01/02已正式CLOSED_DOCUMENTATION_ONLY，组合PASS_PENDING_LOCAL。

## A｜一块32页MBI区域、标签前进与END/总长

- `UefiMain` 以 `page_count=32` 和loader自身 `PAGE_SHIFT=12`/`PAGE_SIZE=1<<12` 请求一片 `EfiLoaderData` 内存并调用 `SetMem` 清零，再把 `pages` 视为 `mbi_tags_header_s*`，`init_info_tag_addr(pages)` 得 `pages+sizeof(header)` 的首tag指针。**131,072字节**仅是32×4096的数学预算，实际AllocatePages/SetMem状态在本固定调用处未经检查，不能当作真实可访问分配。
- `UefiMain` 将每个函数返回的下一指针顺次交给Framebuffer、MMAP、ACPI_NEW、CPU_TOPO，并在最后 `fill_info_tag_end`、`fill_info_tag_header`。四个标签**共享同一个arena、不断推进单一游标**。标签头为各含两个u32的type/size；每段 `next_info_tag_addr` 用 `((prev->size+7)&~7)` 做8字节步长。没有逐次传入剩余容量或实际区域上限。size零可不前进，uint32 size+7可回绕；遇无效指针前必须停，不承认已经写完。
- END实际写 `type=0,size=sizeof(mbi_tag_s)`，并返回END尾部的下个游标；总头再算 `total_size=(PHYSICAL_ADDRESS)info_tag_end-start_addr`、`reserved=0`。正常C布局下END自身为8字节且总长包含头、已对齐tag负载及END；`total_size` 是u32目的字段，需另审源差值范围，不能用该赋值证明整个32页区域/消费者安全。
- **消费者接缝非重复80：** kernel `parse_tag` 检magic与8对齐，读取 `total_size` 却以 `tag->type==END` 终止，而不是以总长约束，沿旧 `BM80-F01/F02/U02/U03` 停止；不执行137的寄存器或BSS来源专题。矩阵：`arena_and_tag_cursor_matrix` 7行、`header_end_publication_matrix` 5行；图中 `DATA_HANDOFF` 绝不伪装直接CALL。

## B｜EFI描述符→MMAP记录、合并、排序、容量与终止约定

- `fill_mmap_info` 先用空`MemMap`探测 `GetMemoryMap` 大小，额加`DescriptorSize*5`再 `AllocatePool(EfiRuntimeServicesData)` 与 `SetMem`，再次 `GetMemoryMap` 才显式 `EFI_ERROR(status)` 检测。早期探测、分配、清零及后部 `ExitBootServices` 没有对应成功分支；若DescriptorSize为零，`MemMapSize/DescriptorSize` 在循环条件已不可解释。每个EFI条目按传回的 **DescriptorSize字节步长** 获得，而不是假设 `sizeof(EFI_MEMORY_DESCRIPTOR)`；实际EFI响应及源可读范围不可由C表达式认证。
- NumberOfPages零跳过；未知Type输出信息后跳过。分类（按固定源码type值）：
  - Reserved/MemoryMappedIO/MemoryMappedIOPortSpace/PalCode→2；Unusable→5；ACPIReclaim→3；ACPINVS→4。
  - Loader/BootServices/Runtime服务的代码与数据、Conventional和Persistent→1。**这是本loader当前的类别策略**，不证明相关区域在`ExitBootServices`之后安全可分配。`NumberOfPages<<PAGE_SHIFT` 是本地尺寸运算；真实有符号/无符号宽度和溢出/左移安全必须单列。
- 仅当**上一条已输出记录**与当前类型相同，且`MMap->PhysicalStart==last_end`时，才延长上一项的`len`；否则写新的addr/len/type，`mb_mmap++`、`e820_nr++`。先合并**EFI枚举原顺序**，再双循环以addr升序交换整条记录，**没有排序后二次合并**。例如同类三页记录以0x1000、0x3000、0x2000顺序输入，各自单页，先生成三条；排序后虽然相邻，仍是三条。这只是有限输入模型，不是固件实际顺序。
- 排序之后 `FreePool(MemMap)`、调用 `ExitBootServices(ImageHandle,MapKey)`，再写MMAP tag：`type=6`、`size=sizeof(mbi_mmap_s)+sizeof(mbi_mmap_ent_s)*(e820_nr+1)`、`entry_size=sizeof(mbi_mmap_ent_s)`、next游标；未在函数体显式写 `entry_version`，也未直接写额外第`e820_nr`项的 `len/type=0`，只能在先前共享块确实清零、未越界或覆盖时条件推导零哨兵。
- kernel `parse_tag` 仅把 `entries` 给 `ram_map`；`e820__memory_setup`以本地 `mbi_mmap_ent_s` 指针递增，看到`len==0 || type==0`才停，不用`entry_size/version/tag.size`约束。它向固定 `E820_MAX_ENTRIES=128` 的目标数组写入，未前置检查129条；该目标容量与错配旧根已是 `BM80-F03/F04`，本批不重复造根或修复。生产者端的字段、排序和哨兵依赖另见 `BT97-F03/F04`。
- 重点矩阵：`descriptor_conversion_and_coalescing_matrix` 10行、`mmap_record_publication_matrix` 7行，另有B样本。真实DescriptorSize、MapKey、源record count、32页写区、尾槽、目标128条均是不同的首个不成立即停门。

## C｜ACPI与CPU拓扑标签、EFI服务阶段及kernel实际消费

- `fill_acpi_RSDT_info(SystemTable,cursor)` 在所读体内用**全局`gST`** 的ConfigurationTable枚举，检查两个GUID的 `CompareGuid(...) == 0` 条件及Root的Signature/Revision；只有满足条件才对 `&rsdt_new->rsdp` 执行定长ZeroMem/CopyMem。是否找到匹配**并不控制后续标签发布**：最后仍赋 `MULTIBOOT_TAG_TYPE_ACPI_NEW`、`sizeof(mbi_acpi_new_s)+sizeof(EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER)`，再next。结构体的 `rsdp[0]` 是柔性尾部布局约定，不能单凭类型定义推定源RSDP有效、GUID返回等于匹配、实际拷贝成功；若未匹配且先前arena清零成功，仍可能签发零内容的ACPI_NEW标记。
- `fill_cputopo_info` 先通过gBS LocateProtocol取MP服务指针，调用`mpp->GetNumberOfProcessors` 获取`UINTN nCores,nRunning`，再向标签内 **u32** 的core_num/core_available赋值；用**原始nCores**逐项 `GetProcessorInfo`，写入`cpus[i]`的ProcessorId、StatusFlag及Package/Core/Thread。最后size=`sizeof(myos2_tag_smpinfo_s)+nCores*sizeof(myos2_cpudesc_s)`，tag type=1000并next。未见逐次返回状态、目标剩余容量及u32截断校验；假设sizeof(tag)=16、sizeof(desc)=24、nCores=3才可算size88，不能反证真实ABI/处理器数。
- **服务调用阶段的重要静态边界：** `ExitBootServices` 写在`fill_mmap_info`内部，`UefiMain`随后才调用ACPI和CPU标签函数（它们仍访问gST/gBS或MP服务），甚至后续还执行Print、gBS->CloseProtocol。固定代码相位清楚，但无法在本只读研究中核实`ExitBootServices`是否成功，以及规范是否允许每一种后续调用；不能把疑似不当阶段使用直接等同已发生的运行异常，也不能反向保证EFI服务可用。
- kernel `parse_tag`的 `ACPI_NEW` case**只取得一个局部结构体指针后break**；所读解析函数未进一步消费RSDP。 `CPU_TOPO` case调用 `get_smpinfo`，它**只做 `nr_cpu_ids=smp_info->core_num`**；这条入口未消费core_available与各core描述符。不能由此得出全仓库其它ACPI/SMP消费者不存在的全称结论。矩阵 `auxiliary_tag_extent_matrix` 7行、`service_stage_and_handoff_matrix` 6行。 

## 模型、证据、停止门与后继

- `facts-and-boundaries.yaml` 共 **8组51行矩阵、20个分离的有限模型**：D00 5行，A 12行，B 17行，C 13行，ABC 4行；这不是51次源码执行或20次实际测试。模型包括32×4096=131072B、tag对齐及uint32零步长、虚拟尺寸320B样例、描述符未排序的相邻关系、0或129项/128表容量、ACPI未命中、CPU0/3/2^32计数与EFI服务源顺序。每个模型在首次越过真实对象/ABI/状态前停止推导。
- `evidence.md` 85条 `BT97-E001…E085` 新短锚：D00 7，A 16，B 34，C 25，ABC 3；每条源自固定time、单一定义连续1–5行。类型字段、条件编译别名、服务**直接调用**与函数指针**间接调用**、跨loader/kernel的**DATA_HANDOFF**有不同证据类。没有合并原96的110条证据或旧80的92条为同一计数。
- `deferred-findings.yaml` 8个独立有条件的生产者候选 `BT97-F01…F08`、9个未证缺口`BT97-U01…U09`；均为`needs_evidence`、真实运行缺陷确认0。历史 `BM80-F01…F04/U02/U03/U05/U06/U07`、`FM96-F04/F05`、`CN95-U05`按qualified_id原身份链接，不因本批改变旧闭合状态。
- `bootinfo-tag-build-paths.dot` 包含55个已声明节点、87条有语义分类的边、0条悬挂；**Graphviz NOT_RUN**，不冒充全部CFG或真实EFI时序图。 `CALL` 仅真实直接调用，`INDIRECT_CALL` 显示UEFI函数指针，`DATA_HANDOFF` 不标调用，`CONDITIONAL_STOP` 标首个未证门。
- 全部外部EFI/ACPI/MP/ABI、真实MBI存储/32页容量、E820表原始数量/版本、kernel地址搬运、实际EBS服务状态、运行环境、实机、P9本地全套扫描、编译、Graphviz与QEMU：**NOT_RUN/NEEDS_EVIDENCE**。本批只有静态原文回读和算术条件；没有新增内核或loader变更和补丁。R96-01/02补正及第97批是否通过均留给原主线C06 **Pro独立同轮复审**。发布六件和唯一PR17正式交付评论后停止写入，绝不自启第98批。
