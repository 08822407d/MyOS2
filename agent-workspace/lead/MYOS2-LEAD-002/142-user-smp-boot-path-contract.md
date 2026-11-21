执行模型：次一档，沿用GPT-6＋Extra。执行位置：现有“MYOS2-A-C06e 代码分析执行”，身份MYOS2-LEAD-002-CHAT-EXEC-14。交回原C06主线时Owner选择Pro，同轮独立验收及后继安排。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SMP-BOOT-PATH-100
record_type: bounded_cpu_topology_bsp_preparation_and_ap_entry_contract
evidence_class: D00文档纠正与三专题静态分析；非SMP运行或修复许可
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（正式独立验收及后继设计）
date: 2026-11-18
base_snapshot: time（分支名）
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 3df2ee58f100
lead_before_publication_short12: 896f19a08088
fixed_contract_commit: OWNER_FORWARDED_142_ACTUAL_FIXED_PUBLICATION
executor_display_name: MYOS2-A-C06e 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
executor_rotation: false
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-smp-boot-path-100/
allowed_files: [MANIFEST.md, smp-boot-path.md, facts-and-boundaries.yaml, smp-boot-paths.dot, deferred-findings.yaml, evidence.md]
D00: REQUIRED_R99_01_APPEND_ONLY_DOCUMENTATION
batch99: RETURN_DOCUMENTATION_ONLY
R99_01: OPEN_UNTIL_SEPARATE_C06_REVIEW
activation_gate: OWNER_FORWARD_142_TO_EXISTING_C06E_AND_G00_PASS
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
inputs_read: [99六件与正式评论及99审核, 97正式回执, 86及闭合与87正式回执, 固定源SMP入口的有界划界预读]
owner_decisions_requested_now: []
open_questions: []
---

# 第100批：99证据类别纠正，CPU拓扑、BSP准备与AP入口

## 1. 本包目标和非目标

保持现C06e/14有效身份，不新开、不轮换，不联系退休13/12/11/10/09。138接收资格只按本14本人真实已读且相同固定对象明确复用，不重做接收仪式、不借用他人声明。

先追加R99-01的14条证据类别纠正，再做三个关联专题：A固件CPU拓扑记录到内核CPU计数/映射；B BSP启动顺序、AP准备及启动请求的实际连接；C AP汇编入口的地址、索引、栈及终止/后继门。目的在于分开“源码含有实现”“实现被选入构建”“调用可达”“处理器已经上线”。不预设SMP已经能运行，也不预设三条路径必然连成完整链。

不重做86/87 perCPU分配、97标签构造、98 ELF装入、99构建交付整包；不恢复被138/139替代的旧136/137 MBI寄存器复制/BSS专题。排除完整调度器/热插拔/ACPI枚举/IPI子系统、完整链接器或固件审计、CPU硬件实验、修源码及补丁。有限缺口也可构成完整交付，无需强行产出新故障或闭合实际运行。

## 2. 固定输入与G00

L=agent-workspace/lead/MYOS2-LEAD-002/；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。

全文新读本142、L/reviews/CORE-USER-UEFI-BUILD-PROVENANCE-99-review.md、L/checkpoints/2026-11-18-uefibuild99-reviewed-smp100-ready.md及110/124，固定到Owner转发本142 URL中的真实新发布提交。读取99作者正式评论6967898004和本轮主线正式评论（实际ID随启动词给出）。不得到父896f19a08088寻找本轮新文件，不能把准备字段当作已发布证明。

99原六件固定3df2ee58f100的R/user-uefi-build-provenance-99/；本14本人同会话真实已读且逐件对象一致可明确复用。D00必须重新定位E019–E031/E041及对应facts索引、引用消费者和MANIFEST总括；真实旧87/87/0保持原样。R99-S01/S02/S03为强制消费限定，不自签99接收或R99-01闭合。

同一主线新发布提交全文读取以下正式回执：CORE-USER-BOOTINFO-TAG-BUILD-97-review.md、CORE-USER-PERCPU-BOOTSTRAP-86-review.md、CORE-USER-PERCPU-BOOTSTRAP-86-DOC-CLOSURE-review.md、CORE-USER-PERCPU-ALLOCATION-87-review.md。本人已读且Git对象一致可以逐件明确复用，不能继承原11的已读资格。重点消费R97-S03的CPU记录/服务阶段，R86-S01/S02/S03与R87-S01/S02/S03的计数、模板、GS、索引和真实启动前提；不重开已关闭的R86-01或R96-01/02。

历史原件仅按实际关系定点：97原包固定f2df1e04cea8；86原包固定596fb3d7e1a5；87原包固定bc611665b38f。先读各MANIFEST与deferred，若引用具体模型/同根因再读facts/evidence相应节，保留完整qualified_id及审查限定，不无差别加载旧六件。85/98回执按99强制限定复用，触发构建/ELF结论才定点展开，不以旧原件未审表述覆盖正式回执。

完整刷新63、79及agent-workspace/tasks/00-gpt-task-protocol-v2.md全部P1–P13；九规则和适用Mnemosyne指导只按本14实际读过且对象相同明确复用，记清新读/本人复用/未读，110/124全文刷新。未获授权启用其它模型、子代理、付费API或跨项目维护。

G00前只读/STOP_WRITING。核四direct refs、PR16/17、100六目标、99作者交付与100正式marker：exec预期3df2ee58f100，time a039d9803ade，master e8e60cd19540，lead为本142真实发布头。PR17原title/base/Open/Draft不变；缓存PR base_sha不能代替master直接头。记录评论查询时间/分页范围，不把局部查询写成全历史扫描。已有100文件/评论或未知分支推进先核归属，停冲突写，不覆盖、不自动变基、不重复交付。

公约§1(2)完整原句、time/master的options_flags及panic双canary须回源；任何配置或否定判断之前全文读取固定time options_flags.cmake。搜索默认分支可能是master：搜索仅定位，所有技术结论回到固定time原文。主线已见time的apic/apic.c与master搜索出现的apic/lapic.c不同；不得照抄master路径充当time证据。

全部门通过后连续D00及G01–G04，不在G00后再次等待Owner逐项批准。主线划界预读不替代100本人执行。

## 3. D00：R99-01追加纠正及消费约束

在100六件内部设置D00节/结构字段，不新添第七件，不改99六件或旧评论。以旧E019–E031与E041为14行纠正台账，包含旧固定路径/选择符、真实原文类别、保留/修正/撤回、旧消费者、替代支持、仍缺前提和纠正状态。

13条include只可保留为声明/包含上下文，不使用定义验证标签，不把它们硬降为“猜测存在”；也不把include(Uefi.h)等自造描述选择符当实际函数定义。E041原型失去定义资格，需要实现的消费回到bootloader.c::load_elf_kernel真实体内连续1–5行。撤回99“87条全部满足定义规则”的总括，保留原始标签87及原文字面命中事实，两者与合格定义资格分别记账；禁止直接把旧账重写为73/73/0。

D00同时确认R99-S01/S02/S03的消费映射：安装prefix/DESTDIR与shell继续执行前提；M04/节表乘法支持、FileInfo->FileSize及条件类型门；图中“字节不同”纠正为“同一性未证”，节点标签与真实调用关系分开。它们是主线已追加的限定，不要求复做99三专题或重绘旧图。若新证据发现额外实质冲突，准确列出，不掩盖、不替主线签通过。

D00交付状态仅CORRECTION_SUBMITTED_PENDING_C06_REVIEW；本100的mainline_acceptance仍PENDING，R99-01保持OPEN等待C06独立关闭。纠正原定义资格不自动修复任何源码问题或消除旧UB99/BE98/BB85缺口。

## 4. A：CPU拓扑记录与内核计数、映射边界

固定time先读myloader/uefi/bootloader.c::fill_cputopo_info和multiboot2.h的MYOS2_TAG_TYPE_CPU_TOPO、CPU_DESCRIPTOR/myos2_tag_smpinfo实际定义，消费97已有服务/容量限定，不重做builder。完整读mykernel/arch/x86_64/myos/pre_init.c的get_smpinfo/parse_tag以及必要全局定义，核core_num、core_available、ProcessorId/status/拓扑字段实际被谁消费；不能把固件“可用数”直接等同内核在线数。

沿直接消费者定点读mykernel/arch/x86_64/smp/setup_percpu.c、percpu_area.c及mykernel/include/linux/kernel/cpumask.h及实际定义nr_cpu_ids/for_each_possible_cpu/NR_CPUS/CONFIG_NR_CPUS的文件。若路径或定义不在已知文件，少量搜索只定位，回time取得原文。把配置容量、运行计数、循环类型、初始化时间和可能CPU集合分开；数字N不自动证明0..N-1合法，也不自动证明CPU ID连续或BSP ID为0。

读time mykernel/arch/x86_64/kernel/apic/apic.c中的apicid_to_cpunr初始化及实际写入/消费位置；结合asm/apic.h中的接口上下文和AP汇编使用，核数组方向、元素宽度、索引起点与终止条件。extern仅作声明上下文，不能作为对象定义锚。未发现写入的结论必须列实际所读范围/检索结果，不扩大为全仓不存在或实机全部未初始化。

交topology_to_kernel_matrix、cpu_count_and_map_matrix；每行有真实来源、数据域/类型、消费点、先行门、止证点。构造少量N=0/1、恰达容量/超过容量、APIC ID不在表/重复/位于首项等隔离模型；按实际代码适用性选择，不强制把所有模型说成真实可达。

## 5. B：BSP顺序、AP准备与启动请求

完整读mykernel/arch/x86_64/myos/smp.c两个实际函数、mykernel/arch/x86_64/kernel/setup.c、init/main.c及kernel/head64.c的x86_64_start_kernel相关完整定义，定点读myos/interrupt.c::myos_init_percpu_intr。区分活跃调用、注释、原型、独立函数定义和最终可达性；注释中的smp_init/smp_prepare_cpus不当执行。不得因函数名含smp就补造调用者或在线握手。

分析myos_early_init_smp复制区间、来源/目的符号、长度计算、物理/虚拟映射与cr3_paddr写入；取myos_startup_smp中真实wrmsrl请求，先列常量、顺序及已读包装，再谈硬件解释。查实际调用关系、准备与发送先后、错误/等待/完成发布；缺连接即记录有界缺口，不假设隐藏调用弥补。

定点合读apic.c::myos_try_enable_x2apic/check_x2apic及直接依赖定义。BSP的x2APIC准备、AP自己的寄存器请求、MSR写入返回、处理器上线各自成列；不能由日志文字或源码注释直接证明实际硬件模式。

构建可达性只读取mykernel/CMakeLists.txt、scripts/options_flags.cmake、scripts/target_kernel.cmake及arch/x86_64/kernel.lds的相关实际定义，必要直接依赖另记用途。应用85正式回执，分开GLOB/target_sources文本、实际对象选择、链接符号/节布局、执行调用。_APboot_text/_APboot_etext/_APboot_phys_start、cr3_paddr/kernel_cr3等按真实定义或限定搜索记录，不能用声明补成链接实值。只核AP相关选择，不重做完整85或旧136/137。

交bsp_call_and_stage_matrix、ap_prepare_and_launch_matrix、source_selection_and_link_boundary_matrix。至少区分“有实现但未证到达”“调用请求但callee正常返回未证”“数值/布局假设”“实际硬件结果未知”。源码中的复制、wrmsr均不执行。

## 6. C：AP入口、表查找、栈和后继停止点

全文读time mykernel/arch/x86_64/kernel/myos_APboot.S，包括局部标签与数据定义；只按真实上下文定位_APboot_start/_AP_code32/_AP_code64/AP_DEBUG、l1及cr3_paddr/GDT/IDT/临时栈。真实宏名/标签与外部符号分开，不跨独立定义拼接短锚，未成对的SYMBOL宏不凭想象补结束标记。

建立16/32/64位阶段的寄存器/输入/地址域账，检查源码如何取得CR3、APIC ID、逻辑CPU索引及栈值；数值算式必须说明位宽、截断、地址基准和先行对象可访问性。重点区分注释声称的BSP假设、实际循环递增起点、表查找无匹配时的局部继续条件、AP_DEBUG时各寄存器真实来源、被注释的idle_tasks/后继入口，以及末尾控制流。不要从名字猜已经分配了idle栈、设置GS/current或进入调度。

x86硬件规则如需引用，必须遵P5给实际取得的一手Intel/AMD手册URL、卷章表/图及修订号；未取得则只报告汇编文本和明确假设，将模式切换、MSR值含义、SIPI地址/送达、异常和处理器状态列为未证，不凭记忆解码成硬件结论。无需为了凑结论下载全套手册或运行反汇编。

交ap_stage_and_address_matrix、ap_lookup_stack_and_exit_matrix及ABC接合表/图。有限模型只计算已经明确的域、合法索引/对象和最早止证；不得把无界循环之后的状态、越界之后的后续步骤或裸jmp .当成实机观测。真实在线完成标记/等待关系若未取得，明确留空，不创造协议。

## 7. 六件、强制锚和质量门

严格六件：MANIFEST.md、smp-boot-path.md、facts-and-boundaries.yaml、smp-boot-paths.dot、deferred-findings.yaml、evidence.md。正文先结论/D00/三专题及未证边界；facts含纠正台账、上述矩阵、必要有限模型、实际读取账与事实/证据/图索引。新候选SP100-Fxx、新缺口SP100-Uxx；旧qualified_id原样消费，已有同根因不换号凑数。

强制定义锚至少覆盖：D00所需load_elf_kernel实现体；CPU标签实际宏和两个结构定义分别取锚；get_smpinfo、parse_tag CPU case；实际CPU枚举宏和容量定义各自单锚；apicid_to_cpunr真实初始化；myos_early_init_smp、myos_startup_smp；实际BSP调用点及x2APIC准备函数；AP入口阶段、l1/AP_DEBUG及相关独立数据定义。每条真实定义内连续1–5行，宏/typedef/结构分锚，不用extern/include/原型或相邻注释冒充定义。找不到强制项的真实定义则明示缺口和阻断，不伪造符号。

声明式配置只用真实唯一CMake命令/链接赋值/输出节定位，保留命令或节类别；ASM局部标签注明所属真实入口与局部基本块，不仅靠辅助行号。外部实现、接口声明、目录/二进制元数据分别分类，不能加入定义验证数。

软上限28份主辅文本源码（含D00有关源，金丝雀另计）、8组目标检索、4次必要目录浏览、110个新短锚、24个有限模型、候选/缺口各10。上限不是配额；必要直接依赖超出时说明用途，不漫游全仓或SDK。第一波DR材料只可按P11取名称/ID作定位，不能继承其断言、行号或溯源。

MANIFEST十四字段齐备：task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。self_check直接列verified_claims、quotes_reconfirmed、downgraded_to_inferred三个真实整数及规范等式；按全六件原始全文计数，D00说明文字/字符串同样不能漏计，不以expected或构造器预测替代。

适用P1–P13。P9-1核六件无完整40位十六进制；P9-2逐字和单定义边界分别核；P9-3/4核实际路径与辅助行号；P9-5全六件原始计数和三整数；P9-6真实外部URL与原文；P9-7/8双canary、公约与通道；P9-9/10无代码件/补丁则N/A；P9-11准确未运行；P9-12只在真实降级交付适用；P9-13强制锚及缺口门。完整本地P9仍NOT_RUN；纯文本检查或隔离算式不能冒充构建/运行，也不能执行或翻译仓库实现。已知文档错误必须在本包发布前纠正。

## 8. 写入、暂态恢复及交回

Owner转发142且G00通过后，14仅在原执行分支的100目录新增六件，及本人一次PR17顶层正式交付评论。marker：MYOS2-LEAD-002-CHAT-EXEC-14 / CORE-USER-SMP-BOOT-PATH-100 / FORMAL_DELIVERY。主线不并写results，退休执行者无写权。

写前核完整实际exec HEAD为3df2ee58f100对应对象、六目标未存在及100 marker；一个单父提交，使用真实完整expected_sha、force=false。GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次并遵Retry-After。成功blob/tree/commit/评论复用，未知推进不覆盖、不回退、不自动变基。write_retry_log据实记；工具发现失败不等于已发写请求或Owner未授权。

发布后compare严格六件added、旧件零净改删；逐件固定全文与活动内容/blob身份、四direct refs、PR16/17、评论实际编号/全文/唯一性全部回读。终态READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、R99-01=CORRECTION_SUBMITTED_PENDING_C06_REVIEW，然后STOP_WRITING，经PR17交原C06。提醒Owner正式交回前选择Pro，同轮独立核99纠正/100并安排后继；不自签99/100通过、不启动101。

禁止改旧成果/回执/评论、PR元数据/title/base/Draft、lead/PR16、master/time、内核/loader/Owner脚本；禁止修复补丁、配置/预处理/汇编/编译/链接、安装挂载清理/复制、执行仓库程序/脚本/夹具、Graphviz/反汇编/QEMU/实机/真实并发、读取本机凭据或设备/外部libc镜像、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3或跨项目维护。110/124继续显式进入以后主线交接。
