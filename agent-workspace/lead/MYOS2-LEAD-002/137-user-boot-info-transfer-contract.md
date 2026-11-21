接收与执行模型：次一档（默认），沿用GPT-6＋Extra。执行位置：新对话“MYOS2-A-C06e 代码分析执行”；身份MYOS2-LEAD-002-CHAT-EXEC-14。完成后交原C06主线，以Pro独立验收。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BOOT-INFO-TRANSFER-97
record_type: bounded_boot_information_construction_copy_and_lifetime_contract
evidence_class: Owner授权的后继工作包；不是运行许可或预定缺陷结论
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台未独立认证）
model_per_owner: Pro（正式验收与设计安排）
date: 2026-11-17
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 6a1fa78d0356
lead_before_publication_short12: fda0f3d1cb60
fixed_contract_commit: OWNER_FORWARDED_136_AND_137_FIXED_PUBLICATION
executor_display_name: MYOS2-A-C06e 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
executor_rotation: OWNER_REQUESTED_C06D_PERFORMANCE_DEGRADATION
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-info-transfer-97/
allowed_files: [MANIFEST.md, boot-info-transfer.md, facts-and-boundaries.yaml, boot-info-transfer-paths.dot, deferred-findings.yaml, evidence.md]
D00: REQUIRED_R96_01_R96_02_APPEND_ONLY
batch95_composite: PASS_PENDING_LOCAL
batch96: RETURN_TARGETED_R96_01_R96_02_WITH_RETAINED_CORE
activation_gate: OWNER_FORWARD_136_TO_NEW_C06E_AND_FRESH_RECEIVE_RULES_GUIDANCE_G00_PASS
status: READY_FOR_OWNER_FORWARDING_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
inputs_read: [96六件与评论及审查, 80和84历史限定, 引导信息构造和复制入口预读]
owner_decisions_requested_now: []
open_questions: []
---

# 第97批：引导信息构造、复制交接与存储寿命

Owner指定下一批改在新对话执行，故本合同只授予新C06e/14的条件写权。旧C06d/13完成96后停止后继业务写入；无需旧方确认。C06主线、PR16/17与源码基线不变，134/135不再作为97执行入口，不导入旧95/96任务的重复执行。

## 1. 新增技术范围

A研究loader构造引导信息容器、tag游标及总长度；B研究入口寄存器保存、MBI/boot_params分支和定长复制；C研究复制目标的存储寿命及解析后保留的视图/指针。连接80的输入可信边界和96的framebuffer来源，但不重做e820算法、84页表建立、95绘字或96映射本体。先完成96两项文档补正，不能把新发现的源码问题交Owner逐项决定。

唯一源码time@a039d9803ade。核心文件/节：myloader/uefi/bootloader.c的UefiMain、各fill_*中size/游标/容量相关完整被选函数；myloader/uefi/multiboot2_utils.c的init_info_tag_addr、next_info_tag_addr、fill_info_tag_end、fill_info_tag_header；bootloader.h和multiboot2.h的相关声明/类型/常量；mykernel/arch/x86_64/kernel/head_64.S的startup_64入口保存和进入C的必要边；head64.c的__startup_64复制段及x86_64_start_kernel；setup.c的multiboot_MBI/boot_params存储定义；pre_init.c的实际解析入口和视图发布。实际include所需asm/setup.h、init/linkage宏、kernel.lds及memcpy选择至多追必要直接依赖。

这些入口已由主线预读确认，不是要求按预定答案找错。不得扩成全部UEFI服务、ELF加载器、GRUB实现、ACPI、CPU拓扑、完整SMP/中断/页表或所有memcpy实现审计；禁止运行loader、仓库程序或构建。服务状态、外部ABI和实际内存范围无证据则保留缺口，不凭惯例补实测。

## 2. 固定必读与新会话接收

L=agent-workspace/lead/MYOS2-LEAD-002/，R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。136两件、137、本轮96审查、95闭合及检查点统一取Owner转发136/01链接中的真实发布提交；不得误取父fda0f3d1cb60查新文件。

先完整读取136/00和01、本137、96-review、95-DOC-CLOSURE和本轮检查点，核现场，按136形成真实接收报告；然后独立全文九份MyOS2规则及110/124；再单独完整加载本次适用Mnemosyne指导；最后G00。不得继承13、主线或前任的已读资格，截断必须补齐。接收报告和G00不是正常停点，全部门通过后同轮连续D00、G01–G04。

96六件统一固定6a1fa78d0356的R/user-framebuffer-mapping-96/，新14独立全文读取；作者正式评论6941872312全文读取。原135、95-review固定fda0f3d1cb60，合读R95-S01/S02/S03和本轮95闭合，不重新执行95纠正。96发布评论6938385614是历史，当前97正式发布评论实际编号由Owner启动词和PR导航给出。

历史主线回执均固定fda0f3d1cb60，全文读取：L/reviews/CORE-USER-BOOT-MEMORY-MAP-80-review.md；CORE-USER-EARLY-ROOT-HANDOFF-84-review.md及CORE-USER-EARLY-ROOT-HANDOFF-84-DOC-CLOSURE-review.md（后两件同reviews目录）。合读R80-S01/S02/S03、R84-S01/S02/S03；R84-01已闭合，不传播原RETURN为现状。53空行误判撤回、90文档闭合及91–94限定继续保留；具体使用相关事实时补读对应回执，不要求无关历史全量灌入。

80原件固定0e9e9e89c4be的R/user-boot-memory-map-80/，84原件固定7d53b2c1ca36的R/user-early-root-handoff-84/：各先全文读MANIFEST和deferred-findings，再按实际依赖的qualified_id定点读取facts/evidence，记录真实范围。84闭合所引85/D00仅在需要该替代证据时取2489ea8e6963的R/user-boot-build-contract-85/。不重做旧模型；不把历史“待本地”改成通过。

G00核PR16/17、开放PR、四direct refs、97六件目录及所有作者的97正式marker。预期exec=6a1fa78d0356、master=e8e60cd19540、time=a039d9803ade，lead为本次136/137实际发布头；不以PR缓存base_sha替代direct master。重读固定time options_flags.cmake后作配置或否定判断，核公约整句、双分支canary和connector通道。发现未知推进/同批已存在先核归属，不覆盖、不重复交付。

## 3. D00：只追加R96-01/02，不重开95

R96-01：重开原FM96-E036及multiboot2.h真实结构，拆开结构内type/reserved字段和结构后独立typedef。按96回执映射A-P06/M11/FM96-F04、quote_index、producer索引与全部实际消费者；追加撤回“110条全部单一定义内通过”的旧总括。E030–E032单一定义的typedef、嵌套结构或空白不能机械地误报。

R96-02：重开deferred中FM96-F05，将数字数组错放distinct_root和拼坏evidence_ids逐字段纠正。引用真实FM96-E035/E041/E044及必要定义，相关旧CN95-U05放在历史关系字段，不拼进EID。五候选、九缺口及其索引作有界完整引用检查：ID须真实存在且语义支持，字段类型和角色须相符。保留原FM96-F05身份，不把台账修正称为内核修复。

D00按“旧位置—保留/修正/撤回—替代结论—新证据—消费者”追加，原96六件、110/110/0、评论和回执不改；新D00标签计入97全包真实总数。两项只报CORRECTION_SUBMITTED_PENDING_C06_REVIEW，执行者不自签关闭，不另交第七件。R96-S01/S02/S03直接作为消费限定，不把全部补充变成整批重做。

## 4. G01 / A：构造容器、tag游标与长度记账

沿UefiMain的AllocatePages/SetMem请求、init_info_tag_addr、各fill_*返回值、end标签和总头填充建立实际调用顺序。区分内核镜像header的parse_mb2_header与loader输出MBI信息tag，不混用两套结构/魔数/长度。

核每个被选fill_*如何确定tag.size及下一游标：固定/可变payload、向8字节取整、结构sizeof、尾标签和总长度各自有来源。各fill函数只扩到容量和长度生产所需完整定义；不顺势审全部固件服务或设备内容。分开“申请容量”“已使用长度”“允许写入extent”“返回游标”和“可被接收方读取的长度”，检查状态返回是否被消费、对齐及整型窄化/回绕的最早门。

至少覆盖正常串接、可变tag长度接近容量、缺失/无效来源或未证服务返回、终止tag与total_size边界的必要条件模型；不预设成功分配、不把一个数学长度当真实UEFI页大小或ABI实测。

交boot_info_builder_capacity_matrix、tag_cursor_and_header_length_matrix及必要有限模型。

## 5. G02 / B：入口保存与复制范围

沿UefiMain实际内联汇编操作数、startup_64的rax/rbx和rsi/r15保存、__startup_64参数与mbi_magic分支，核指针/标量的生产者及存储位置。区分源码寄存器赋值意图、编译器约束、链接布局、CPU实际状态；不能用SysV/GRUB名称自动认证未读协议或真实入口。

取得mbi_magic、mbi_base、multiboot_MBI及boot_params的实际定义和相关声明；一个汇编标签、宏调用、导出、对齐、节选择分别取锚，不能把邻近指令拼为一个对象定义。核复制使用sizeof目标、来源total_size还是其它长度；区分源已分配容量、源可读extent、目的容量、已初始化内容和复制后可解析extent。

比较本仓loader路径与其它输入来源时仅以真实证据/明确假设区分：不能把本loader的容量担保外推给所有magic相同的外来引导器。magic不匹配的boot_params分支独立分析，不能把MBI分支的合法性替代它。memcpy及rip_rel_ptr至多补直接实现/宏选择和所需前提，不展开完整库或84页表主体。

交entry_register_and_saved_object_matrix、copy_source_destination_extent_matrix。必要样例覆盖源容量/声明长度/固定拷贝长度彼此不同、分支选择、重叠或地址别名未证；一旦前序门失败，不签后续完整成功。

## 6. G03 / C：目标存储寿命、清零边界与解析视图

核multiboot_MBI与boot_params的.data属性、相关链接输入/输出节及clear_bss范围。静态声明、最终链接布局和实际执行分别记账，不仅凭.data文字认定真实复制必然保留，也不重做84的根页表/启动地址修正。

沿x86_64_start_kernel→myos_early_init_system→parse_tag，定位mbi_tag_header、ram_map及framebuffer/nr_cpu_ids等必要直接消费：哪些保存原字节对象内指针，哪些复制数值；指向原loader存储还是内核副本、是否被重置、是否依赖终止标签与真实存储extent。可补最多两个代表后续消费者以证明视图用途，不扩大为全部e820/CPU拓扑/ACPI审计。

80已记录的tag游标、total_size未形成真实边界及容量问题只追加上游传递证据，沿原BM80 qualified_id去重；新复制/寿命根因若独立才登记BT97-Fxx。保留ER84构建/地址前提和FM96/CN95来源门，不把复制完毕、解析完毕、启动状态标志或打印信息当正确性认证。

交copied_storage_and_clear_boundary_matrix、parser_views_and_lifetime_matrix及A/B/C接合表。

## 7. G04 / 六件、质量与发布

严格六件：MANIFEST.md、boot-info-transfer.md、facts-and-boundaries.yaml、boot-info-transfer-paths.dot、deferred-findings.yaml、evidence.md。接收报告、规则读账、指导刷新、G00和D00并入六件；主报告先说结果和限制。新证据BT97-Exxx、新候选BT97-Fxx、新缺口BT97-Uxx；旧根因不得重复编号。

软限24份源码（金丝雀另计）、8组定点检索、4次必要目录浏览；短锚含D00最多120，有限模型含D00最多24，新候选/缺口各最多10。这些是上限不是配额；直接依赖必要超限须说明用途与弃尾，不靠文档长度充技术量。

MANIFEST十四字段直接齐备：task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。self_check必须有verified_claims、quotes_reconfirmed、downgraded_to_inferred三个真实整数及规范等式；整包原始标签包含D00/说明/字符串，不用expected或构造器统计替代。可见/Owner模型与后台UNATTESTED分开，不反复索取同一模型确认。

协议v2全部P1–P13适用。P9-1查全包40hex；2/13核逐字1–5行、真实单一定义、强制链及实际语义；3/4核路径、旧新ID、附加行号和图索引；5全包实际计数；6外部结论有具体来源否则保留未证；7/8公约、双canary、read_channel；9/10/12无代码/补丁/降级时N/A；11未运行项明示NOT_RUN。全文规则不能以本段摘要代替。

发布前字段级反查：evidence_ids只容纳存在的证据字符串；qualified_id关系不混入EID；distinct_root表达根因而非误放编号数组。类型声明、存储定义、结构后独立typedef、注释、宏条件和真实活动代码分开。图CALL、间接调用、宏展开、源序、数据流及复制视图分别标注。已知错误发布前纠正；允许的独立纯文本/数学检查不执行或翻译仓库实现，不冒称完整P9。

14通过全部门后仅在原执行分支新增97六件及本人一次正式PR17顶层评论。marker：MYOS2-LEAD-002-CHAT-EXEC-14 / CORE-USER-BOOT-INFO-TRANSFER-97 / FORMAL_DELIVERY。主线不并写results；13及旧12/11/10/09停用。发布前兼查任意作者的同批成果/marker，未知推进停冲突写，不覆盖、不自动变基。

单父提交、完整expected_sha、force=false。暂态按63先核实际效果，再间隔安全重试，通常首次加三次并遵Retry-After；复用成功blob/tree/commit/评论，恢复只补缺项。发布后compare严格六件added、旧件零净改删，固定六件全文及活动内容/Blob身份、四refs、PR16/17、评论实际编号/全文/唯一性全部回读。

完成后READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、R96纠正待主线复核，随后STOP_WRITING。交原MYOS2-A-C06，提醒Owner正式验收消息前选Pro，主线同轮审核补正、97及后继。不自签96闭合或97通过，不启动98。

禁止修改旧成果/旧回执/旧评论、PR元数据/Draft/base、主线/PR16/master/time、内核/loader/initramfs/Owner脚本；禁止补丁、配置/预处理/汇编/编译/链接、仓库程序/脚本/夹具、Graphviz/QEMU/实机/真实并发、外部libc镜像、本机凭据、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3及跨项目维护。110和124继续进入以后主线交接。
