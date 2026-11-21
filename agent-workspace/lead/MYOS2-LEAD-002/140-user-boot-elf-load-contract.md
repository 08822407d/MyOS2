执行模型：次一档，沿用GPT-6＋Extra。执行位置：现有“MYOS2-A-C06e 代码分析执行”，身份MYOS2-LEAD-002-CHAT-EXEC-14。完成后交原C06主线，Owner发送正式独立验收消息前选择Pro。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BOOT-ELF-LOAD-98
record_type: bounded_uefi_file_acquisition_elf64_context_and_load_contract
evidence_class: 后继静态调查合同，不是预定缺陷清单或运行许可
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（本轮正式验收与后继设计）
date: 2026-11-17
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: f2df1e04cea8
lead_before_publication_short12: b878cc3784da
fixed_contract_commit: OWNER_FORWARDED_140_ACTUAL_FIXED_PUBLICATION
executor_display_name: MYOS2-A-C06e 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
executor_rotation: false
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-elf-load-98/
allowed_files: [MANIFEST.md, boot-elf-load.md, facts-and-boundaries.yaml, boot-elf-load-paths.dot, deferred-findings.yaml, evidence.md]
D00: NOT_REQUIRED
prior_return_items: []
batch96: PASS_PENDING_LOCAL_WITH_DOCUMENTARY_CLOSURE
batch97: PASS_PENDING_LOCAL_WITH_R97_S01_S02_S03
activation_gate: OWNER_FORWARD_140_TO_EXISTING_C06E_AND_G00_PASS
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
inputs_read: [97六件与正式评论, 97审核与96闭合, UEFI LoadFile和ELF库入口划界预读]
owner_decisions_requested_now: []
open_questions: []
---

# 第98批：引导器读取内核文件、解析ELF64与装入入口交接

## 1. 身份、技术增量与非目标

继续同一有效C06e/14，不新开、不轮换，不向退休C06d/13、C06c/12、C06b/11、C06a/10或C05c/09索取确认。138独立接收记录只可由本14按真实同会话、相同Git对象明确复用；不重做新会话接收仪式、不继承其它会话已读声明。

96的R96-01/02已由C06关闭，97按R97-S01/S02/S03接收；98无D00，不重做96/97，也不恢复已被138/139取代的旧136/137任务。三个新专题从97之前的真实依赖切入：A文件读取和原始镜像范围；B ELF64校验/上下文与大小来源；C目标分配、段复制清零、返回状态和入口发布。只读源码，既不是实现loader，也不是测试启动。

明确排除：内核早期MBI寄存器保存或BSS交接；用户态exec/ELF解释器与musl动态链接；整个UEFI文件系统、全部ELF32、完整动态链接/重定位、所有处理器架构或真实EFI硬件。共享库可识别32位分支不等于本批已审32位装入；不能仅凭PC架构认定真实输入文件一定为ELF64。主体模型显式条件为到达ELFCLASS64分支，其他分支仅记接口门与缺口。

## 2. 固定必读、历史限定与G00

L=agent-workspace/lead/MYOS2-LEAD-002/；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。

本140、L/reviews/CORE-USER-BOOTINFO-TAG-BUILD-97-review.md、L/reviews/CORE-USER-FRAMEBUFFER-MAPPING-96-DOC-CLOSURE-review.md、L/checkpoints/2026-11-17-bootinfo97-reviewed-bootelf98-ready.md、L/110-owner-communication-and-delivery-continuity.md、L/124-model-routing-and-pro-message-budget.md均取Owner提供本140固定URL中的真实发布提交，全文读取，截断补齐。读取97作者评论6944267731和本轮主线正式发布评论（实际编号由启动词给出）。不去父b878cc3784da寻找本轮新文档。

97原六件统一固定f2df1e04cea8的R/user-bootinfo-tag-build-97/。本有效14已实际全文读取的同对象可逐项记本人复用；本轮依赖的BT97-F01/F05/F08、U01/U02/U04/U07/U09与D00闭合状态重开定位，不把整个旧候选列表冒充均已重新审计。R97-S01及R80-S01的一过尾地址/无效访问区别继续有效，R97-S02的证据支持链及R97-S03的服务返回/图边限定不可省略。80原限定在L/reviews/CORE-USER-BOOT-MEMORY-MAP-80-review.md；其它历史仅按实际依赖读取，不无差别灌入所有前批。

重新全文读取L/63-github-write-retry-and-readback.md、L/79-work-package-sizing-and-pro-review.md、agent-workspace/tasks/00-gpt-task-protocol-v2.md全部P1–P13。九规则和本14已加载的适用Mnemosyne指导只可在对象一致且本14真实读过时明确复用；记录本轮新读/本人复用/未读，不倒造接收/刷新日志，不导入其它项目维护任务。

G00前只读/STOP_WRITING。核PR16/17与开放PR、四条direct refs、98目录六目标和所有98正式marker；exec预期f2df1e04cea8、master e8e60cd19540、time a039d9803ade、lead为140实际发布头。PR17保持原title/base/Open/Draft。未知推进或同批存在先核归属，不能覆盖、自动变基或重复交付。缓存base_sha不替代master直接头。配置/否定判断前全文读time options_flags.cmake；核公约完整原句、time/master双canary和connector读取通道。kernel配置不自动证明UEFI SDK、库宏或实际输入ELF的选择。

全部门通过后连续G01–G04，不逐专题等待批准，不在G00后再次停等Owner发令。新14已接收并完成97，不因此获得主线写权。

## 3. A：文件获取、实际读长与原始镜像对象

全文读取myloader/uefi/utils.c::LoadFile、utils.h及bootloader.c::load_elf_kernel、UefiMain的相关直接调用；读取bootloader.h中的实际接口。以LoadedImage/DeviceHandle→协议/卷/根句柄→文件句柄→FileInfo→内存申请→Read→释放/关闭为边界，不展开整个文件系统或SDK实现。

建立每个实际间接调用的输入、输出参数、所保存返回值、检查分支、已获得资源及失败时后缀能否继续。区分EFI服务函数、库函数和日志；缺少真实声明时明确接口语义未证，不把全部调用臆造为EFI_STATUS返回。不能因为名为Close/Free就签资源成功释放。

区分size形参、FileInfo.FileSize、GetInfo所需/提供bufsize、Read前后的in/out bufsize、页数取整、addr按值形参与调用方固定raw_img_addr。建立“外部实际可读文件长度”和“库稍后从ELF字段算出的FileSize”两个不同来源；未从接口传递的实测长度不得凭空补入解析器。检查零长、显式短size、GetInfo缓冲不足/Read短读或失败的有界分支，条件输入不当作真实服务曾返回。极大整数/指针模型停在首个可表示或对象门，不继续称完整读取成功。

交file_read_status_and_extent_matrix、file_handle_and_buffer_lifecycle_matrix，给出最少必要模型。

## 4. B：ELF64格式门、索引与解析上下文

全文读取myloader/uefi/ElfLib/ElfLib.c中的IsElfFormat、ParseElfImage、GetElfSegmentInfo、CalculateElfFileSize、GetElfSectionPos以及实际必要定义；读取myloader/uefi/ElfLib.h的ELF_IMAGE_CONTEXT、SEGMENT_INFO和接口；按include读取ElfLibInternal.h、Elf64.h及ElfCommon.h中实际依赖的宏/类型，逐宏分锚。Elf64Lib.c的GetElf64SectionByIndex、GetElf64SegmentByIndex须实读完整体。

沿实际代码核魔数各字节、数据编码、class、machine、type、version条件，区分ASSERT与活动返回分支，不能依据注释“valid ELF”认证完整格式或底层对象长度。头访问、表偏移/步长/个数、索引边界、section-name索引和字符串范围各自分账；库未接收文件长度不代表可忽略源extent。

逐字段记录ZeroMem后ParseStatus/FileBase/EiClass/PhNum/ShNum/ShStrOff/ShStrLen/EntryPoint、PreferredImageAddress/ImageSize/ReloadRequired/ImageAddress的生产者。核类型转换发生在运算前或后，不把目的UINTN宽度当中间表达式宽度。按实际PT_LOAD过滤、初始Base/End、对齐算式检查无LOAD段、零MemLen、边界端点和失败返回；每个有限模型必须先通过所选更早IsElfFormat和section访问门。CalculateElfFileSize的来源与状态消费实际回查，不把某个被调用者的成功与ParseStatus全流程成功混同。

交elf64_validation_and_index_matrix、elf_context_extent_and_status_matrix。32位专有下级、全部section名称API和未被实际路径使用的字段作为边界保留，不扩大到完整ELF协议验证或执行样例。

## 5. C：申请地址、PT_LOAD写入与入口发布

完整合读load_elf_kernel、LoadElfImage、Elf64Lib.c::LoadElf64Image，定位ELF_NEXT_ENTRY等实际依赖。用时间顺序表区分raw_img_addr、loaded_addr变量、ElfCtx.ImageAddress、PreferredImageAddress、*entry_addr、ElfCtx.EntryPoint及UefiMain.k_load_addr/func；不能把名称相似的地址当同一分配对象。

按实际语句顺序核AllocatePages的入/出参地址、页数、返回值、解析上下文赋值和LoadElfImage状态；分别判断真正申请的区间、准备复制的区间、上下文声称的范围、最终传给调用者的入口。宏/外部函数未取得实现时保留接口门，不用一般Linux loader惯例填补。日志输出不证明内存申请、段装入或入口可执行。

ELF64逐段只审PT_LOAD选择、p_memsz零分支、p_paddr相对preferred的偏移、源p_offset/p_filesz、目标范围和ZeroMem长度。检查p_filesz与p_memsz关系、中间减法/加法、不同段的重叠及实际地址域；CopyMem/ZeroMem的外部契约不足时只说明请求，不认证重叠拷贝或真实清零效果。不得从最终return EFI_SUCCESS直接推出所有页或权限正确。

重定位只核本调用中ImageAddress与PreferredImageAddress的分支选择、RelocateElf64Sections的接口、EntryPoint改写和返回状态如何被消费。优先证明本实际调用形态能否到达该分支；未到达的重定位算法不扩审。若需作对照，最多读取RelocateElf64Sections入口与直接返回范围，RelocateElf64Dynamic/ProcessRelocation64内部条目算法留缺口，不批量审所有重定位类型。入口发布前后关系与函数实际返回分别记，不把callq后的行为或CPU实际启动当本批认证。

交allocation_destination_and_segment_write_matrix、entry_publication_and_status_consumption_matrix及ABC接合表。该分配/ELF段清零是新对象范围，不是恢复旧137的内核BSS与MBI复制专题。

## 6. 证据门、规模与严格六件

严格六件：MANIFEST.md、boot-elf-load.md、facts-and-boundaries.yaml、boot-elf-load-paths.dot、deferred-findings.yaml、evidence.md。正文先写结果/限界，无D00。facts包含上述矩阵、有限模型、源读取账、事实/证据/图索引；新候选BE98-Fxx、新缺口BE98-Uxx。旧同根因沿完整qualified_id补证，不改身份、不以重复命名增加发现数。

强制短锚至少覆盖A实际文件调用与长度/状态链，B格式谓词/字段生产/索引与大小算式，C地址赋值、实际分配、段写请求、入口与状态消费。一个宏一锚；字段/声明/实际定义分清；正文引用不能只存在ID却缺相应语义。主线划界预读不是预定缺陷：允许反证或未证，不要求一定发现问题。

软限24份主辅源码（金丝雀另计）、8组定点检索、4次相关目录浏览；110新短锚、24有限模型、候选/缺口各最多10。都是上限不是配额，额外必要依赖说明用途，不漫游仓库或EFI SDK。遇到普通问题分类延期，仅停止依赖该前提的子项，继续安全独立项，不要求Owner逐个审查或决定修复。

MANIFEST直接具备14字段：task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。self_check直接包含verified_claims、quotes_reconfirmed、downgraded_to_inferred三个真实整数及规范等式；expected/构造器推算不替代实际全包计数。全部六件的说明、字符串和正文都计原始标签，不复制旧标签凑数。

P9逐项履行：无40hex；1–5行逐字连续单定义并支持结论；实际路径与事实/模型/图引用；有效附注行号；全包真实计数；外部事实有具体来源否则未证；逐字公约与双canary、connector；无代码/补丁/降级项如实N/A；未运行项目明示NOT_RUN。独立纯文本/有限数学检查不能调用或翻译执行仓库实现，不能冒称完整本地P9。已知错误必须发布前纠正，不能交给主线兜底。

## 7. 发布、查重、恢复和终态

14仅在G00后向原claude/dazzling-cori-q0dnyt的user-boot-elf-load-98/新增上述六件，及本人一次PR17顶层正式交付评论。marker：MYOS2-LEAD-002-CHAT-EXEC-14 / CORE-USER-BOOT-ELF-LOAD-98 / FORMAL_DELIVERY。主线不并写results，退休13/12/11/10/09停写。

写前核完整exec HEAD为预期f2df1e04cea8对应对象、六件不存在及全部98marker；一个单父提交、完整expected_sha、force=false。GitHub暂态不因首失败停工，按63先核远端实际效果再间隔安全重试，通常首次加三次并遵Retry-After；成功blob/tree/commit/评论复用。未知推进不覆盖、不回退、不自动变基；恢复只补缺项，记录真实write_retry_log，不把工具缺失说成无授权。

发布后compare严格六件added、旧件零净改删；固定六件全文、活动内容或blob身份、四direct refs、PR16/17、正式评论实际编号/全文/唯一性全部远端回读。未实际完成不预造成功记录或编号。终态READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，随后STOP_WRITING，交原MYOS2-A-C06，提醒Owner发送正式验收消息前选择Pro，同轮审核98及后继。不自签98通过，不启动99。

禁止改旧成果/回执/评论、PR元数据/Draft/base、lead/PR16、master/time、内核/loader/initramfs/Owner脚本；禁止补丁、配置/预处理/汇编/编译/链接、仓库程序/脚本/夹具、Graphviz/QEMU/实机/真实并发、读取本机凭据、外部libc镜像、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3及跨项目维护。110/124继续显式进入未来主线交接。