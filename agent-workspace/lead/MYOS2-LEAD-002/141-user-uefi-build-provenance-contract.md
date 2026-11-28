执行模型：次一档，沿用GPT-6＋Extra。执行位置：现有“MYOS2-A-C06e 代码分析执行”，身份MYOS2-LEAD-002-CHAT-EXEC-14。完成后交原C06主线，Owner发送正式独立验收消息前选择Pro。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-UEFI-BUILD-PROVENANCE-99
record_type: bounded_uefi_module_dependencies_artifact_and_install_provenance_contract
evidence_class: 后继静态调查合同；非构建、安装或运行许可
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（正式独立验收及后继设计）
date: 2026-11-17
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 39b28e36e8f6
lead_before_publication_short12: 280b7b49a521
fixed_contract_commit: OWNER_FORWARDED_141_ACTUAL_FIXED_PUBLICATION
executor_display_name: MYOS2-A-C06e 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
executor_rotation: false
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-uefi-build-provenance-99/
allowed_files: [MANIFEST.md, uefi-build-provenance.md, facts-and-boundaries.yaml, uefi-build-provenance-paths.dot, deferred-findings.yaml, evidence.md]
D00: NOT_REQUIRED
prior_return_items: []
batch98: PASS_PENDING_LOCAL_WITH_R98_S01_S02_S03
activation_gate: OWNER_FORWARD_141_TO_EXISTING_C06E_AND_G00_PASS
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
inputs_read: [98六件及评论, 98审核, 85正式回执, 固定UEFI模块及安装文件的有界划界预读]
owner_decisions_requested_now: []
open_questions: []
---

# 第99批：UEFI模块、库依赖与启动制品交付来源

## 1. 身份、增量和非目标

继续同一有效C06e/14，不新开、不轮换，不向退休13/12/11/10/09索取确认。97的138接收资格只能按本14本人实际读过且对象一致明确复用；不重做接收仪式、不继承别人已读声明、不导入其它项目维护。

98已按R98-S01/S02/S03限定接收，没有新RETURN；99无D00，不重做98、97或96。三个关联专题：A模块清单到源码及入口的选择关系；B外部头/库、类型/宏/ABI与编译参数的来源边界；C已有EFI文件、安装脚本、安装目标及内核文件路径的交付链。重点补98的外部依赖和真实制品来源缺口，不以脚本文本或二进制文件名认证实际构建成功。

不是重做85的内核源筛选、asm-offsets生成或链接算法，不展开98的ELF解析/段装入，不恢复已被138/139替代的旧136/137的MBI寄存器复制/BSS专题。排除完整EDK II/固件实现、完整CMake/bash兼容性、反汇编或执行EFI二进制、全盘/全仓递归搜寻、真实安装/启动、全部initramfs构建。无需修源码，也不要求一定产出缺陷。

## 2. 固定必读和G00

L=agent-workspace/lead/MYOS2-LEAD-002/；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。

本141、L/reviews/CORE-USER-BOOT-ELF-LOAD-98-review.md、L/checkpoints/2026-11-17-bootelf98-reviewed-uefibuild99-ready.md及110/124均取Owner转发本141固定URL中的真实新发布提交，全文读至EOF；读取98作者评论6951092059和本轮主线正式发布评论（实际编号由启动词给出）。不要去父280b7b49a521寻找本轮新文件。

98原六件固定39b28e36e8f6的R/user-boot-elf-load-98/；本14同会话真实已读且对象一致可以逐件明确复用。本轮重开定位BE98-F01/F02/F05/F07/F09及U01/U02/U05/U06/U09与R98-S01/S02/S03，候选和实际消费关系分别记账。97回执及96文档闭合状态继续有效，不重开旧项。

独立全文读取同一主线发布提交的L/reviews/CORE-USER-BOOT-BUILD-CONTRACT-85-review.md，合读R85-S01/S02/S03。85原MANIFEST和deferred固定2489ea8e6963的R/user-boot-build-contract-85/；只在触发相同根因或引用旧模型时定点读取其facts/evidence相关范围，不无差别加载所有旧包。BB85-F01–F05/U01–U08等沿原完整qualified_id消费，已存在问题不重新编号。旧核构建结论不能当作UEFI模块构建实证。

重新全文读取63号、79号及agent-workspace/tasks/00-gpt-task-protocol-v2.md全部P1–P13。九规则和适用Mnemosyne指导只可按本14真实已读、相同对象明确复用；记录本轮新读/本人复用/未读，不倒造日志。110/124全文刷新，普通问题由AI分类记录，不要求Owner逐项审查或批准。

G00前只读/STOP_WRITING。核开放PR、PR16/17、四direct refs、99六目标与所有99正式marker；exec预期39b28e36e8f6、master e8e60cd19540、time a039d9803ade、lead为本141真实发布头。PR17原title/base/Open/Draft不变，缓存base_sha不代替master直接引用。已有99成果或未知推进先核归属，停冲突写、不覆盖、不自动变基或重复交付。核完整公约原句、time/master双canary及connector读取通道；涉及配置或否定判断前全文读取time options_flags.cmake。

全部门通过后连续G01–G04，不逐专题等待批准，不在G00后再等Owner发令。主线预读仅划界，不是99的预定技术答案或已执行成果。

## 3. A：模块清单、源码选择和入口来源

完整读time的myloader/uefi/bootloader.inf、myloader/CMakeLists.txt、scripts/make_install_bootloader.sh、make_install.sh及bootloader.c的UefiMain入口相关范围。逐项建Defines/Sources/Packages/LibraryClasses/Protocols/Ppis/Pcd/Guids到实际本地文件或外部依赖的对应；字段名、路径字符串、声明类别和调用实现不混同。

将INF的ENTRY_POINT、实际UefiMain定义、UefiApplicationEntryPoint库类和最终PE入口分层；不能从ENTRY_POINT字面推断包装入口、固件调用约定或已生成机器码。检查列出的C/头文件、重复列项、相对目录及Elf32/64并列选择，重复项只记录事实，未知构建器去重规则不得当编译故障。头文件被列入Sources不等于独立生成可执行对象。

区分两条路线：模块清单的源码构建意图，与myloader/CMakeLists.txt安装现有BOOTX64.EFI的命令。按实际读取范围核是否存在把本地C源接入该CMake目标的关系；只作所读文件/有界搜索的结论，不断言全仓或外部工作区没有编译入口。若需查平台DSC或外部包路径，先用少量目标检索/目录读取确认；未找到记出处与范围，不能虚构路径、下载整套SDK或把外部文件默认为在本仓。

交module_source_and_entry_matrix、recipe_to_artifact_matrix，列出每一关系的证据类别与仍缺的生成步骤。现有EFI文件最多读元数据与已提供的Git blob身份，不取二进制反汇编/运行，不宣称从源码重建后已字节一致。

## 4. B：外部类型、宏、库和ABI边界

完整读myloader/uefi/utils.h、bootloader.h、ElfLib.h，按实际依赖定点读ElfLib/ElfLibInternal.h、ElfLib/Elf64.h及multiboot2.h。98已读同对象可以明确复用；本轮是来源闭合，不重复解析算法。选择至多四组直接影响98结论的依赖：UINTN/UINT16/UINT32与EFI_STATUS；EFI_PAGE_SIZE/ALIGN_VALUE/MAX_UINT32；ASSERT/ASSERT_EFI_ERROR与DebugLib；EFIAPI及UefiApplicationEntryPoint/BaseMemoryLib/服务表接口。记录包含文件、宏条件、声明、库类以及缺失实现/版本，不能把LibraryClasses条目当已选具体库实例。

必须分开kernel options_flags.cmake所声明的-mabi=sysv等参数、UEFI模块的外部工具链/平台参数、实际编译命令与机器码。当前kernel参数不能自动证明UEFI ABI，外部GCC5路径字符串也不证明实际编译器版本、DEBUG宏或断言行为。__PI_UEFI_H__等条件按真实定义有无解释，不以文件命名补造选择结果。

沿R98-S02将数值、实际C类型和运算阶段分开：外部宏只知名称或数值时不能自动认证取反掩码高位、整数提升、状态错误位或指针表示。缺少固定SDK/库实现时明确停在未证边界。需要外部规则时只采用可核验的一手具体来源并标明版本及适用条件；找不到就保留未证，不从记忆补出返回类型、编译器保证或实际固件行为。不得为此接管外部项目、安装工具或启用付费API。

交type_macro_and_library_provenance_matrix、configuration_and_abi_separation_matrix。编译规则源证据、假设模型和真实构建证明分别列出；没有更多可核证来源时完成缺口表，不反复扩大搜索凑工作量。

## 5. C：制品身份、安装顺序与启动文件交接

完整读myloader/uefi/install_loader.sh、scripts/make_install_bootloader.sh、make_install.sh；合读scripts/make_install_kernel.sh及mykernel/scripts/target_kernel.cmake中的实际install目标、bootloader.c的内核文件名调用。make_install_initranfs等只在共享shell变量/工作目录或前置顺序确有必要时读取相关完整函数，不扩成用户程序构建审计。脚本中的全部mount、rm、cmake、make、cp、sync、umount、objdump均只能阅读，绝不执行。

至少区别：仓库跟踪的myloader/BOOTX64.EFI；EDK输出路径中的bootloader.efi；install_loader.sh写到的启动文件及另一份elfbinary_loader.efi；CMake安装现有EFI文件的来源；内核安装到boot/kernel与UEFI的反斜杠文件名请求。分别列生产者、输入、输出、所依赖工作目录、可变路径、是否同一字节对象及缺失的身份检查。相同目标文件名不证明内容相同，路径看似对应不证明两个安装入口使用相同磁盘/挂载点。

建立命令顺序和失败前缀表：shell函数是否改变当前目录和共享变量、哪个命令受&&控制、哪个后缀无显式状态检查、原文件删除/复制与挂载失败各自怎样影响后续证明。只能在明确shell/工具语义与假设退出状态下做静态推导；不声称实际宿主已写坏文件、挂载成功或部署完成。路径引用/空格等只按真实输入条件分类，不能仅因未加引号就判所有输入必失败。读取到主机磁盘路径仅作为脚本原文，不探测或访问本机设备。

从安装定义到实际Read、再到98的ELF上下文是跨阶段数据关系，不是脚本直接调用固件服务。保留BE98-U02/U06/U09与BT97服务阶段未证前提，不能用制品文件存在关闭装入/运行缺口。

交installation_command_and_failure_prefix_matrix、artifact_identity_and_boot_path_matrix及ABC接合表。输出最小可证关系与缺口，不生成部署脚本、修改建议补丁或要求Owner现在执行安装。

## 6. 成果、证据类别与质量门

严格六件：MANIFEST.md、uefi-build-provenance.md、facts-and-boundaries.yaml、uefi-build-provenance-paths.dot、deferred-findings.yaml、evidence.md。新候选UB99-Fxx、新缺口UB99-Uxx；已存同根因沿完整qualified_id追加证据，不用换名字增加发现数。正文先结果/范围，再三专题和边界；facts含上述矩阵、少量必要条件模型、实际源读取账、事实/证据/图索引。

源码锚仍为真实单一定义内连续1–5行，一个宏一锚。沿85已明确的声明式文件规则：INF只在实际单个命名节或字段内取锚；CMake用实际唯一命令定位；shell用真实函数或唯一顶层赋值/调用语句定位，注明类别，不能伪造C函数名、把相邻独立节拼成一个定义或把注释当执行命令。二进制目录元数据属于制品元数据，不伪装为源码定义锚。引用编号存在还必须实际支持所述字段/依赖/时序。

软上限24份主辅文本源码（金丝雀另计）、8组目标检索、4次必要目录浏览、100个新短锚、24个模型、候选/缺口各10。都是上限不是配额；超限必要直接依赖说明用途，不漫游仓库/SDK，不用重复矩阵、加长文档替代技术增量。

MANIFEST十四字段齐备：task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。self_check直接包含verified_claims、quotes_reconfirmed、downgraded_to_inferred三个真实整数及规范等式；全六件全部原始标签计数，说明文字和字符串不遗漏，不用expected或构造器预估替代。

适用P1–P13和P9：无完整40位十六进制SHA混入成果；逐字短锚、真实定义/类别、事实与模型和图引用、语义支持、必备字段、公约原句和双canary分别核对。完整本地P9及运行未做仍NOT_RUN；独立纯文本/有限算式不能调用、翻译执行仓库实现或冒充构建测试。已知错误必须在本批发布前纠正。

## 7. 写入、恢复与终态

14仅在Owner转发141并通过G00后，于原执行分支user-uefi-build-provenance-99/新增六件，及本人一次PR17顶层正式交付评论。marker：MYOS2-LEAD-002-CHAT-EXEC-14 / CORE-USER-UEFI-BUILD-PROVENANCE-99 / FORMAL_DELIVERY。主线不并写results，退休执行者不恢复写权。

写前核完整exec HEAD为预期39b28e36e8f6对应对象、六目标不存在及全部99marker。一个单父提交、完整expected_sha、force=false。GitHub暂态异常不因首次失败直接停工：按63先核远端效果，再间隔安全重试，通常首次加三次并遵Retry-After；成功blob/tree/commit/评论复用。未知推进不覆盖、不回退、不自动变基；恢复只补缺项，write_retry_log据实记，工具缺失不等于Owner未授权。

发布后compare严格六件added、旧件零净改删；逐件固定全文与活动内容或blob身份、四direct refs、PR16/17、正式评论真实编号/全文/唯一性全部回读。终态READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，随后STOP_WRITING，经PR17交原MYOS2-A-C06；提醒Owner发正式验收消息前选择Pro，同轮审核99及后继，不自签99通过、不启动100。

禁止改旧成果/回执/评论、PR元数据/title/base/Draft、lead/PR16、master/time、内核/loader/initramfs/Owner脚本；禁止补丁、安装、挂载、清理、配置/预处理/汇编/编译/链接、执行任何仓库程序/脚本/夹具、Graphviz/QEMU/实机/真实并发、读取本机凭据或设备、外部libc镜像、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3和跨项目维护。110/124继续显式进入未来主线交接。
