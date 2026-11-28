---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BOOT-ELF-LOAD-98
record_type: bounded_static_uefi_elf64_load_report
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
executor_display_name: MYOS2-A-C06e 代码分析执行
produced_by: GPT-6（当前可见模型家族；backend UNATTESTED）
model_per_owner: GPT-6＋Extra（Owner指定次一档）
date: 2026-11-17
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
read_channel: connector
status: AUTHOR_STATIC_COMPLETE_PENDING_C06_PRO
mainline_acceptance: PENDING
D00: NOT_REQUIRED
runtime_evidence: NOT_RUN
---

# 第98批｜UEFI内核文件取得、ELF64解析与PT_LOAD装入入口

**结论与限界。** 本轮只针对固定 `time@a039d9803ade` 的UEFI引导路径做静态取证。可以确认文件读取请求、ELF64谓词和字段、段装入与入口发布的真实调用及源码计算顺序，却**不能确认**固件确实返回数据、实际Read长度、分配到可写物理区、内核段已正确复制、或CPU已经从入口启动。第96批两项文档纠正已由C06关闭且组合`PASS_PENDING_LOCAL`；第97批按`R97-S01/S02/S03`限定接收，无新增RETURN。本98不做D00，不重审96/97；完整本地P9、预处理/编译/链接、仓库程序、Graphviz、QEMU或实机均`NOT_RUN`。

本次生成 `facts-and-boundaries.yaml` 的 **7组58行条件矩阵、21个隔离有限模型**，`evidence.md` 的 **103条固定源码单定义短锚**，`deferred-findings.yaml` 的 **9条条件候选、9条缺口**，以及 `boot-elf-load-paths.dot` 的 **64节点92边**分类图（Graphviz未运行）。条目数是报告栏目，不是已确认的实际缺陷数或测试次数。

## A｜从UEFI文件系统取原始内核镜像

### 1. 调用链与实际状态

`UefiMain` 先经`HandleProtocol`尝试取得LoadedImage，再以`L"\\boot\\kernel"`和`&k_load_addr`调用`load_elf_kernel`。此函数把`raw_img_addr`定为`0x2000000`，执行`LoadFile(LoadedImage,filename,raw_img_addr,0)`，**没有检查LoadFile返回**，紧接着在同一固定`raw_img_addr`上调用`IsElfFormat`，然后`ParseElfImage`。（BE98-E001–E003/E015–E017）

`LoadFile`内从`LoadedImage->DeviceHandle`经`gBS->HandleProtocol`、`Vol->OpenVolume`、`RootFs->Open`形成Vol/RootFs/FileHandle三层对象。只将`RootFs->Open`的EFI_STATUS保存并在EFI_ERROR时提前return；本次所读函数**没有对应检查**前两次服务返回。FileInfo缓冲的`AllocatePool`及`GetInfo`同样未保存返回，随后的`FileInfo->FileSize`读取必须先证明FileInfo及其内容有效。这里的“没有本地检查”不等于外部服务一定失败，也不猜不同接口全部具有同一种返回类型。（BE98-E004–E009）

**资源正常后缀**为`FileHandle->Read`后依次`FreePool(FileInfo)`、`FileHandle->Close`、`RootFs->Close`、`return EFI_SUCCESS`。若`RootFs->Open`触发失败return，正常关闭后缀没有进入；若其它服务失败，则在第一处失效指针/返回门停止，不能从文本顺序签后来确实关闭或已经泄漏。外部释放状态、句柄所有权与EFI接口结果均未实测。（BE98-E006–E012）

### 2. 四种不同的“文件长度”

| 量及来源 | 源码实际用法 | 能证明什么、不能证明什么 |
|---|---|---|
| `LoadFile`形参`size` | 上层实参是0；显式非0时优先作为bufsize | 请求策略，不是读回长度 |
| `FileInfo->FileSize` | `size==0` 时赋给`bufsize` | 依赖GetInfo成功及可读FileInfo，是外部文件元信息，不是Read结果 |
| `Read(&bufsize)`调用前后 | 前值用于申请页并传入Read，后值仅留于LoadFile局部变量 | 外部Read可能更新bufsize；当前函数不向Parse传递实际字节数，也未消费Read状态 |
| `ElfCt.FileSize` | Parse内部通过section末端及section-header表末端求MAX | **由ELF头字段推算**，不是FileInfo/Read实测字节数 |

文件信息缓冲`GetInfo`的`bufsize`先以`sizeof(EFI_FILE_INFO)+100*sizeof(CHAR16)`为初始输入，之后又被`bufsize=size`覆盖，不能把GetInfo的输出缓冲字节数当文件实际长度。`LoadFile`页数为`(bufsize+PAGE_SIZE-1)/PAGE_SIZE`，本loader实际PAGE_SHIFT为12、PAGE_SIZE写作`1<<12`；在无溢出且假定4096B页时，文件长度4097的孤立模型请求2页，大小0时会形成0页请求。具体AllocatePages和Read实际效果仍未证。（BE98-E008–E014）

`LoadFile`的addr为**按值形参**，它把`&addr`交给`AllocatePages(AllocateAddress,...)`。即使某服务改变该局部addr，`load_elf_kernel`的`raw_img_addr`并不会因此自动改变；后一格式读始终使用原`0x2000000`。该分支只辨别地址身份，不宣称EFI会在正常成功请求中擅自迁址。申请页的status也未被检查，打印的`File loaded address/size`发生在Read前，不能充当完成凭据。（BE98-E009–E016）

矩阵：`file_read_status_and_extent_matrix`10行与`file_handle_and_buffer_lifecycle_matrix`5行。首个证明停止点通常是协议/根句柄或FileInfo的有效性、物理缓冲实际分配、Read真实字节范围，不能跳到“ELF解析成功”覆盖这些前提。

## B｜ELF64格式、索引、上下文和由头部推算的长度

### 1. 格式函数的真实判定范围

`IsElfFormat`以`ASSERT(ImageBase!=NULL)`后直接访问ELF header；此ASSERT的外部宏具体行为未在本包认证。魔数比较表达式是 **EI_MAG0、EI_MAG1、EI_MAG1、EI_MAG2**，即第2字节被重复检查，第4字节`EI_MAG3`（宏值3、预期`ELFMAG3='F'`）在这组判断中没有被比较。单独假设ELF64 header其余字段满足门、`e_ident`前四字节为`0x7f,'E','L','X'`，这个局部魔数条件不会因X而拒绝；这不是实际固件文件通过整个启动链的观察。（BE98-E019/E028–E031/E100–E103）

随后判`EI_DATA==ELFDATA2LSB`及32/64 class；**本包选择ELFCLASS64有条件分支**：`e_machine==EM_X86_64`、`e_type`为ET_EXEC或ET_DYN、`e_version==EV_CURRENT`，否则FALSE。此谓词没有凭空补上整个ELF协议或所有e_ident其它字段的检查，且类分支不证明本次raw镜像确是64位。ELF32分支仅标为存在，不展开装入。（BE98-E021–E027/E032–E037）

### 2. 索引安全与字符串表先门

`Elf64_Ehdr`的`e_phoff/e_phentsize/e_phnum`与`e_shoff/e_shentsize/e_shnum/e_shstrndx`均来自文件头。两个64位下级`GetElf64SectionByIndex`/`GetElf64SegmentByIndex`的前置检查分别只是`Index>=e_shnum`/`Index>=e_phnum`，通过后按`ImageBase + offset + Index*entsize`形成指针；没有传入Read真实字节数来校验表总跨度。因此**索引小于计数与真实内存足量是两道不同门**，不能把尾后一指针形成、下个成员的实际访问和一切越界情况混成一个判断。（BE98-E039–E045/E051–E055）

`ParseElfImage`清零ELF_IMAGE_CONTEXT并绑定FileBase，先重复IsElfFormat。在ELF64分支还必须用`e_shstrndx`访问`GetElf64SectionByIndex`，返回NULL即UNSUPPORTED；只有该索引和源对象真正有效，才取得`e_entry`→EntryPoint、`e_shnum`→ShNum、`e_phnum`→PhNum；`Elf64_Shdr.sh_size/sh_offset`会**先转UINT32**再存入UINTN的ShStrLen/ShStrOff，故目标字段名/宽度不足以证明源高位保留。外部返回与实际ABI另证。（BE98-E046–E048/E056–E062）

`GetElfSegmentInfo`的ELF64分支逐一取得ProgramHeader的p_type、p_offset、p_filesz、p_memsz、p_paddr和p_align，必要字段显式转UINTN。Parse用`ASSERT_EFI_ERROR(Status)`而非独立显式`if(EFI_ERROR)...return`消费其状态；ASSERT具体行为未证，若段表不合法，必须停在获取或断言，不应继续把未赋值SegInfo当真实源事实。（BE98-E063–E066/E070–E071）

### 3. ImageSize与FileSize是两条计算线

`ParseElfImage`初始化`Base=MAX_UINT32`、`End=0`、`ReloadRequired=FALSE`，只对PT_LOAD计算：`MemLen!=Length`标ReloadRequired；Base可降到`MemAddr & ~(EFI_PAGE_SIZE-1)`，End可更新为`ALIGN_VALUE(MemAddr+MemLen, EFI_PAGE_SIZE)-1`，之后`ImageSize=End-Base+1`、`PreferredImageAddress=(VOID*)Base`。这里的`EFI_PAGE_SIZE`与`ALIGN_VALUE`来自所读外部包含面，本包**没有以loader的PAGE_SIZE宏冒充它们的实现定义**；有限模型若用4096都明确以该假设为前提。（BE98-E063–E069）

对高地址或无PT_LOAD输入，初值`MAX_UINT32`不会自动变成UINTN64最大：一个孤立的合法PT_LOAD、`MemAddr=0x100000000`且假定4096B对齐时，已对齐起点仍大于MAX_UINT32，Base可保留`0xffffffff`。无PT_LOAD时End=0，ImageSize将由具体UINTN宽度下`0-MAX_UINT32+1`决定，不能根据源码注释把它直接当真实零大小图像，更不能越过此前section/段对象门解释分配成功。（BE98-E063/E067–E069）

`CalculateElfFileSize`从ShNum-1的section偏移+大小算`FileSize1`，再以section-header表`e_shoff+e_shentsize*e_shnum`算`FileSize2`，取MAX。这些加法/乘法有中间整数域与溢出先门，且ShNum==0会触及ShNum-1的索引域问题（实际Parse主路可能更早在e_shstrndx停止）。**Parse调用CalculateElfFileSize后未检查返回，就将ParseStatus置SUCCESS返回。** 因此内部FileSize和ParseStatus不构成“文件真实读取完整”的双重证明。（BE98-E069/E072–E078）

矩阵：`elf64_validation_and_index_matrix`11行，`elf_context_extent_and_status_matrix`11行。以上只证明文件格式判定和内部字段生产者的条件关系，不做ELF32、全section名称API、全部ELF规范或者用户态动态链接调查。

## C｜目标页请求、PT_LOAD复制/清零与入口发布时间

### 1. 按发生顺序区分六种地址

| 名称 | 固定源码生产/使用时点 | 非等同关系 |
|---|---|---|
| `raw_img_addr` | load_elf_kernel内常量`0x2000000`；LoadFile按值接收 | 原始镜像缓冲，并非目标装载地址 |
| `ElfCtx.FileBase` | Parse将其赋为`ImageBase`（上层传raw） | 只是一项源码指针赋值，原始Read extent未证 |
| `ElfCtx.PreferredImageAddress` | Parse从PT_LOAD的Base计算 | 期望装入基址，不代表已实际AllocatePages |
| `loaded_addr` | **在**改写ImageAddress前，先从`ElfCtx.ImageAddress`取值 | 作为AllocatePages(AllocateAddress)的**in/out地址**，不自动等于Preferred |
| `ElfCtx.ImageAddress` | 后来被调用者明确赋`PreferredImageAddress` | 是LoadElf64Image使用的**写入目标基址**，不是AllocatePages的out变量 |
| `ElfCtx.EntryPoint`、`*entry_addr`、`k_load_addr` | Parse取e_entry，load_elf_kernel在分配前赋给*entry_addr，UefiMain稍后赋func | 是将要尝试调用的入口地址，不等于成功映射/装入/执行 |

最关键的顺序是：
`loaded_addr=(EFI_PHYSICAL_ADDRESS)ElfCtx.ImageAddress` → `*entry_addr=ElfCtx.EntryPoint` → `ElfCtx.ImageAddress=ElfCtx.PreferredImageAddress` → `AllocatePages(AllocateAddress,...,&loaded_addr)` → `LoadElfImage(&ElfCtx)` → `return status`。
在Parse的ZeroMem成功生效、ImageAddress未另改的**条件模型**中，loaded_addr通常从零开始，但下一段拷贝目标仍使用PreferredImageAddress。**实际是否将相同物理页申请到Preferred需要另核服务请求及返回**，当前代码没有把loaded_addr的out值回填ImageAddress，也没有消费AllocatePages返回状态。此处只能记目标来源错位候选，不能说已发生覆写故障。（BE98-E056/E079–E086）

### 2. PT_LOAD段复制和清零请求

`LoadElfImage`检查ElfCt是否NULL、ParseStatus错误、ImageAddress是否NULL，再按EiClass选择`LoadElf64Image`并返回其status；上层`load_elf_kernel`没有使用该返回。ELF64函数按`e_phoff/e_phnum/e_phentsize`迭代program headers，只处理`p_type==PT_LOAD`且`p_memsz!=0`的条目；每条计算`Delta=(UINTN)p_paddr-(UINTN)PreferredImageAddress`，发出：
`CopyMem(ImageAddress+Delta, FileBase+(UINTN)p_offset, (UINTN)p_filesz)`；
`ZeroMem(ImageAddress+Delta+(UINTN)p_filesz, (UINTN)(p_memsz-p_filesz))`。（BE98-E083–E091）

每次复制请求必须先有**FileBase+p_offset的真实源可读长度**、ImageAddress+Delta的真实目的可写长度，以及源表指针、整数/指针形成与段间地址关系的证明。代码片段未给出`p_filesz<=p_memsz`检验：孤立模型`p_filesz=8192,p_memsz=4096`时，先请求CopyMem 8192字节，而后减法在声明的无符号域将形成很大尺寸；若复制的真实目标早已越界，**首先停于CopyMem的对象门**，不能伪称ZeroMem已正常执行。两段重叠亦只算条件输入，不认证库的重叠拷贝效果。（BE98-E044/E045/E088–E090）

### 3. 重定位选择、返回码和入口交接

`LoadElf64Image`仅当`ImageAddress!=PreferredImageAddress`时调用`RelocateElf64Sections`；本实际load_elf_kernel已经把二者赋为相等，因此在保持上下文不被外部修改、且正常到达判断处的**本调用形态**里不会进入重定位分支，与ELF文件ET_EXEC/ET_DYN无关。作为上限旁证，仅核了RelocateElf64Sections入口：当该接口被其它形态调用时计算Delta并改EntryPoint，DYN另调用RelocateElf64Dynamic。未审动态重定位内部条目算法或全部类型；也不将Relocate入口状态伪装本调用已经消费。（BE98-E081/E082/E091–E094）

`LoadElf64Image`在执行复制/清零调用后返回常量EFI_SUCCESS；`load_elf_kernel`的`status`仅由ParseElfImage更新，后续AllocatePages和LoadElfImage返回未写回status，最终返回的依然是解析状态。`*entry_addr`也**在**这些操作之前写入EntryPoint，所以日志、返回`EFI_SUCCESS`、以及UefiMain把`k_load_addr`转为`func`并发出`callq *func`，都不能证明目标页可读写/可执行、段已正确装入、或者CPU真正进入内核。后续MBI与ExitBootServices历史边界按R97-S03及BT97-F08/U09保留，本98不重做旧137复制/BSS专题。（BE98-E079–E082/E091/E095–E099）

矩阵：`allocation_destination_and_segment_write_matrix`9行、`entry_publication_and_status_consumption_matrix`8行及`ABC_join_matrix`4行。

## 证据、同根因与最终边界

`facts-and-boundaries.yaml`区分所有历史来源、99条首轮新锚加4条宏补证形成的**103条最终引文**、7组58矩阵行和21个独立的隔离计算/分支模型；其`source_quote_index`逐条给出实际path::symbol、类别和短锚行段，graph索引关联到真实节点，**没有把“真正read length”从ELF header中伪造出来**。`deferred-findings.yaml`仅新增`BE98-F01–F09`与`BE98-U01–U09`。已有`BT97-F05/F08`和`BT97-U01/U02/U04/U09`仍作为EFI状态/阶段先门，不因本次另一接口的进一步取证变成已修；旧BM80-F03必须按R80-S01及R97-S01对合法尾后一指针与后继非法成员写分离。

`boot-elf-load-paths.dot`不是完整CFG。`CALL`只表示明确直接函数调用，`INDIRECT_CALL`标EFI协议函数指针，`CALL_REQUEST`标外部库复制/清零请求，`DATA_HANDOFF`不是直接调用，`CONDITIONAL_CALL_NOT_SELECTED`表示有定义但在本调用形态不满足的重定位入口。全部图、矩阵和模型都是未执行的源码阅读与有界数值推导。不存在真实ELF二进制、UEFI服务结果、ABI布局或内核运行证据，程序级复现缺失不能用当前候选数量掩盖。

本次只新增第98批严格六件和EXEC14本人一次PR17正式评论；**提交成功前不能写“已发布”，发布成功也不能写“主线已独立验收”。** 所有固定目录/活动分支、严格六件、PR状态、四direct refs、评论唯一性回读完成后作者STOP_WRITING，交原C06独立Pro审核本98并同轮安排后继；本作者不签98通过、不启动99。
