执行模型：次一档，沿用GPT-6＋Extra。执行位置：现有“MYOS2-A-C06d 代码分析执行”，身份MYOS2-LEAD-002-CHAT-EXEC-13。交回原C06正式验收前，Owner选择Pro；主线同轮审核补正、96及后继。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FRAMEBUFFER-MAPPING-96
record_type: bounded_framebuffer_metadata_mapping_and_return_contract
evidence_class: 固定任务输入及新增调查范围，不是技术裁定或运行许可
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（正式验收与后继设计安排）
date: 2026-11-17
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: be83c6095eb1
lead_before_publication_short12: 1c7352f6a06f
fixed_contract_commit: OWNER_FORWARDED_135_FIXED_URL
executor_display_name: MYOS2-A-C06d 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-13
executor_rotation: false
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-framebuffer-mapping-96/
allowed_files: [MANIFEST.md, framebuffer-mapping.md, facts-and-boundaries.yaml, framebuffer-mapping-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R95-01, R95-02]
D00: REQUIRED_APPEND_ONLY
batch95_acceptance: RETURN_TARGETED_R95_01_R95_02_WITH_RETAINED_CORE
activation_gate: OWNER_FORWARD_135_TO_EXISTING_C06D_AND_G00_PASS
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
inputs_read: [95六件与评论, 95主线独立审查, 80至83限定回执, 帧缓冲生产与映射入口划界预读]
owner_decisions_requested_now: []
open_questions: []
---

# 第96批：帧缓冲信息、映射区间与接口衔接

继续当前C06d/13，不新开、不轮换，不向退休C06c/12、C06b/11、C06a/10或C05c/09索取确认。134接收已经由13执行过的真实记录可按相同对象明确复用，不重做新会话接收仪式；不能借用任何其它对话的读取资格。

95已交付，当前仅R95-01/02定点RETURN。96先追加两项纠正，再连续完成三个新专题，不为补正单独消耗一个主线Pro回合。旧95六件、原70/70/0及评论冻结，执行者不得自签关闭。

## 1. 增量与范围

A核帧缓冲信息的实际字段类型、大小/行距单位和有界生产消费；B核启动中的映射入口、物理请求区间与页对齐；C核myos_ioremap的实际下级、返回值、保护属性及刷新衔接。补CN95-F05/U05/U06的相关子项，不重复95的完整注册、字符和绘字分析，不重做80–83页表主体。

主要源：mykernel/arch/x86_64/myos/pre_init.c、myloader/uefi/multiboot2.h、myloader/uefi/bootloader.c的fill_framebuffer_info和必要直接入口、mykernel/drivers/char/tty_io.c::myos_init_VBE_mapping、mykernel/arch/x86_64/mm/init.c的myos_memory_map/init_mem_mapping/myos_ioremap及必要接口，另按真实include定位PFN、地址转换、保护位、刷新与声明定义。loader仅只读静态输入来源，不是新增引导实现/测试任务。

主线预读只用于划界，不是预定缺陷清单。不得以函数名、注释、默认分支搜索或Linux/UEFI惯例替代固定time实现。没有必须发现多少问题、写多少行的配额；边界充分即可，不扩大完整引导协议、全体设备映射、GPU驱动、全部页表/分配器或真实多核TLB审计。

## 2. 固定材料、读取账和G00

L=agent-workspace/lead/MYOS2-LEAD-002/，R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。新135、95回执和本轮检查点取Owner转发本合同链接中的真实发布提交，不把父1c7352f6a06f误当新发布头。

全文读取本135、L/110-owner-communication-and-delivery-continuity.md、L/124-model-routing-and-pro-message-budget.md、L/reviews/CORE-USER-CONSOLE-REGISTRATION-OUTPUT-95-review.md、L/checkpoints/2026-11-17-console95-reviewed-framebuffer96-ready.md，及95作者正式评论6937736735和本轮C06正式发布评论（实际编号由转发启动词给出）。合读R95-01/02和R95-S01/S02/S03。

95原件统一固定be83c6095eb1的R/user-console-registration-output-95/六件。D00必须按回执重新核受影响正文、矩阵、模型、证据、MANIFEST、deferred和图，不只改一句摘要。同一有效13已有实际全文读取可记本人复用，补正涉及的源定义和原条目仍实际重开核对。

历史主线文件固定1c7352f6a06f：
- L/reviews/CORE-USER-BOOT-MEMORY-MAP-80-review.md；CORE-USER-EARLY-PGTABLE-STORAGE-81-review.md；CORE-USER-DIRECT-MAP-RANGE-82-review.md；CORE-USER-DIRECT-MAP-TABLE-INSTALL-83-review.md（后面三件也在同一L/reviews目录）。独立全文读取，合读各R80/R81/R82/R83-S01/S02/S03，旧阶段/作者安排只作历史，不恢复旧批执行权。
- L/reviews/CORE-USER-PRINTK-FORMAT-BUFFER-94-review.md及R94-S01/S02/S03；91、92、93审查和90文档闭合/53撤回误判，按134指定固定对象及本13真实读账合读。未读的适用限定补齐，不把本次RETURN传播成94返工。
- 80原件固定0e9e9e89c4be、81固定05fe580c12a3、82固定3f41eb242040、83固定18047d54b0a2；分别位于R/user-boot-memory-map-80/、user-early-pgtable-storage-81/、user-direct-map-range-82/、user-direct-map-table-install-83/。先读各MANIFEST和deferred-findings.yaml，facts/evidence仅按实际依赖的qualified_id及函数定点读取并登记，不要求重做全部历史模型。

重新全文读取L/63-github-write-retry-and-readback.md、L/79-work-package-sizing-and-pro-review.md和agent-workspace/tasks/00-gpt-task-protocol-v2.md全部P1–P13。其它九规则、110/124之外的旧适用指导，只可按本有效C06d自己实际完整读过且对象一致的范围明确复用；记录本轮新读/本人复用/未读，补齐截断，不倒造刷新，不导入Mnemosyne/Alaya维护路线。

G00前只读/STOP_WRITING。核PR16/17、开放PR、四direct refs、96目录及所有96正式marker；exec预期be83c6095eb1，master e8e60cd19540，time a039d9803ade，lead为135真实发布头。PR17保持Open/Draft、原head/base/标题。缓存base_sha不能替代master直接头。配置与否定判断前全文读固定time options_flags.cmake，核公约原句、time/master双金丝雀和connector通道。未知推进或同批已存在先核归属，不覆盖、不自动变基、不重复交付。

全部门通过后连续D00、G01–G04，不逐专题等批准，不在G00后再次停等发令。

## 3. D00：R95-01/02定点追加纠正

R95-01：完整重读固定time register_console、unregister_console、console_init、console_unlock和实际回调调用边。正确区分新节点尾插后的活动console_unlock与重复节点提前return；区分当前积压排空与被注释的CON_PRINTBUFFER/con_start重置/exclusive策略。补空积压、含LF、无LF、重复节点、假设NULL write五类最小模型，保留实际myos_tempcon非空write和上游停止门。纠正文中“注册不调用回调”“console_unlock被注释”及相关首输出时机；不能反向改成注册必然显示成功。

按95回执列出的A-REG/C-BOOT/ABC、M01/M03/M04、U02/U07及图等做完整消费者映射。保留M02重复节点正确分支；登记原结论保留/修正/撤回及替代证据。真实调用者和间接调用层级不得压成虚构直接CALL。

R95-02：拆开E045的PAGE_SHIFT/PAGE_SIZE（PAGE_MASK仅确需时）；把E064宏前说明移出定义内认证，以实际for_each_console及必要下级作锚；把E050 extern声明与klib/font.c真实对象定义分开，允许明确复用E051定义支持。分别核字面、符号、范围和语义，不以短引文字数合格替代定义检查。

追加quote_index、source_ledger、source_quote_recheck和MANIFEST原全通过总括的纠正，映射每条实际消费者；原70/70/0不改、不倒造历史复核。新D00标签纳入本96六件真实总数。E023尾部空白不作为失败理由；不复活53的空行误判。不另交第七件或errata独立文件，纠正并入本六件。只报CORRECTION_SUBMITTED_PENDING_C06_REVIEW，不自签95闭合。

## 4. A：帧缓冲信息的字段、单位和数值传递

完整读取pre_init.c::get_framebuffer_info、parse_tag的FRAMEBUFFER分支及其入口前提，obsolete/ktypes.h::framebuffer_s；读取myloader/uefi/multiboot2.h的标量别名和multiboot_tag_framebuffer_common/framebuffer实际结构。total_size、tag.size、底层可读存储和真实结构布局分开，不重做80标签遍历或e820审计；出现旧根因沿BM80原ID补证。

完整读取myloader/uefi/bootloader.c::fill_framebuffer_info；UefiMain只追其直接调用和必要mode准备边，set_video_mode仅按实际来源需要有界取证。不得证明全部UEFI服务/模式选择/ExitBootServices。外部类型或常量语义若仓库没有充分定义，记外部前提/缺口，不凭记忆补固定ABI。

建立来源→中间表达式类型→赋值截断/扩展→消费者单位表，覆盖width/height/bpp/pitch/address/FB_size/PixperScanline及framebuffer_type的用途。宏注释“scanline”、字段名pitch与实际赋值不必一致，须核算式；不直接假定所有入口采用相同单位。分别检查中间乘法、先除/后除、bpp/8、零分母、窄字段和目标64位是否真的扩宽中间计算。无符号回绕不等于CPU自动异常，结果范围失配也不自动等于实机越界。

对齐/紧凑行距/含padding、有界窄化和零值输入只作必要隔离模型；先保留真实标签/服务返回/对象/构建选择前提。95的像素布局候选CN95-F05及U05/U06保留原身份，本批补上游来源，不重复命名同根因，不预设当前设备是24或32位模式。

交framebuffer_producer_type_and_unit_matrix、metadata_to_kernel_extent_matrix及必要模型。

## 5. B：启动映射入口、页偏移与实际请求范围

完整读取tty_io.c::myos_init_VBE_mapping，arch/x86_64/mm/init.c::myos_memory_map、init_mem_mapping及直接激活关系；按实际源码定位setup_arch中的调用与init/main.c的console_init先后。只追与本映射有关的启动边，区分函数存在、活动调用、前序正常完成与真实启动观测。记录after_bootmem/活动根/局部刷新前提，不用函数名猜阶段。

读取PFN_DOWN、PFN_UP、PFN_PHYS和PAGE_SHIFT/PAGE_SIZE等真实单宏定义，必要地址转换声明/实现按include定位。把原物理基址A、长度L、页内偏移o、向下对齐起点B、实际传入size与向上取整终点逐项列出。比较实际请求与希望覆盖的[A,A+L)，不能把“起点向下取整且长度向上取整”自动当完整覆盖。分别处理对齐、非对齐、长度恰页倍数、边界附近、零长度和可表示域端点；必须使用实际类型/运算次序，禁止凭无限整数公式覆盖C回绕。

请求覆盖、真实底层PTE写入集合、返回端点和有效映射不是一件事。输入落入已映射RAM、旧大页、空表的差异只按83已有分析及本调用的真实参数补接，不重建整个页表审计。若已遇先行分配/对象停止门，下游只作明确隔离前提，不签同次完整成功。

交boot_mapping_activation_matrix、framebuffer_page_offset_and_extent_matrix。

## 6. C：myos_ioremap返回、保护属性和刷新责任

完整读取固定time myos_ioremap及mm/misc/mm_misc_api.h实际声明、iounmap活动体；合读__kernel_physical_mapping_init与实际返回来源。默认分支搜索仅作导航，必须回time原文，尤其不能把旧注释或其它快照的NOCACHE调用当当前实现。

记录传入page_size_mask、prot、init值及其实际定义链；单独区分请求4K候选、最终叶形态、已有项复用、属性继承/过滤、父表发布和TLB请求。保护位只核源码编码/选择，不凭PAGE_KERNEL_NOENC或NOCACHE名称宣称硬件缓存模式、PAT/MTRR状态或MMIO安全。R83-S01/S02的互斥分支与保护条件、R81分配前提继续适用。

逐层解释返回值的单位和含义：原物理端点、PFN、虚拟地址、成功长度是否被混用；实际VBE调用是否消费返回值、另用哪个FB虚拟地址。R83-S03说明的混合新旧PTE端点不可当覆盖并集或统一成功码，本批只补myos_ioremap消费链。必要时最多补一个有实际固定源码的其他调用者作“消费/忽略返回”对照，不扩展该设备驱动，不把未读调用者说成已验证。

沿实际定义定位flush_tlb_local、相关加载根/刷新包装，至多追必要直接helper；区分发出本CPU刷新动作、其发生时的根和全CPU硬件可见性，后者保持未证。iounmap不因名称自动具有释放/解除映射功能；无返回值消费与无真实调用者分别记。不得运行汇编、读硬件寄存器或创建映射。

交ioremap_parameter_return_and_attribute_matrix、mapping_publication_and_flush_matrix，以及A/B/C接合表。

## 7. 六件、强制证据与规模

严格六件：MANIFEST.md、framebuffer-mapping.md、facts-and-boundaries.yaml、framebuffer-mapping-paths.dot、deferred-findings.yaml、evidence.md。正文先说结果、限制和R95追加纠正状态；facts含D00消费者映射、上述矩阵、有限模型、实际读取账及事实/证据/图对应。新候选FM96-Fxx、缺口FM96-Uxx；D00可以使用独立命名的纠正证据序列，但所有标签纳入整包计数。

强制短锚覆盖：D00真实活动调用/提前返回/三个边界事项；A字段真实类型、生产和消费算式；B活动启动边及PFN实际宏；C真实映射参数、返回和被调用者支持、所选属性及刷新定义。声明只能作声明证据，不冒充定义；注释只证明注释内容/状态，不替代实现或硬件保证。一个宏一个锚；共用完整函数不是跨定义拼接许可。

软限：24份源码（金丝雀另计）、8组定点检索、4次相关目录浏览；全包新短锚含D00最多120、有限模型含D00最多24、新候选/缺口各最多10。均为上限非配额。必要直接依赖超限时说明用途和取舍，不全树漫游；普通问题分类延期，受阻仅停止依赖子项，继续安全独立项，不要求Owner逐项决定。

MANIFEST十四字段必须直接齐备：task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。self_check直接包含verified_claims、quotes_reconfirmed、downgraded_to_inferred三个真实整数，满足规范等式；expected/constructor统计不能替代。模型按Owner/可见信息记账，后端UNATTESTED，不反复确认。

本任务适用协议v2全部P1–P13。P9逐项：1全包40hex；2/13逐字连续1–5行、真实单一定义并支持实际结论；3路径、旧新引用、事实/模型/图索引；4附加行号有效；5六件全部原始标签含D00/说明/字符串实际计数；6外部断言有具体来源否则未证；7/8公约整句、双canary、connector；9/10/12无代码/补丁/降级时N/A；11所有未运行项目明示。已有错误须发布前纠正，不能只留待主线查错。

发布前反查：register与unregister同名调用不串体；声明/注释/定义不串类；单位、目标宽度和中间表达式宽度不混同；先行停止门/有效对象、实际分支和状态前缀完整；CALL、间接调用、宏展开、内部状态、数据流、返回值消费及刷新请求分开。独立数学和文本检查不得调用、翻译执行仓库实现，不冒称完整本地P9或运行测试。

## 8. 写区、查重、回读与终态

13通过G00后仅在原claude/dazzling-cori-q0dnyt新增本96六件及本人一次PR17顶层正式评论。marker：MYOS2-LEAD-002-CHAT-EXEC-13 / CORE-USER-FRAMEBUFFER-MAPPING-96 / FORMAL_DELIVERY。C06不并写results；退休12/11/10/09继续停用。

写前核实际完整exec HEAD对应be83c6095eb1、目标六件及全部96marker；单父提交、完整expected_sha和force=false。GitHub暂态异常不因一次失败停工，先查是否已写成，再按63间隔安全重试，通常首次加三次，遵Retry-After；复用成功blob/tree/commit/评论。未知并行推进停冲突写，不覆盖、不自动变基；本人成功动作恢复只补缺项。

发布后compare严格六件added、旧件零净改删；固定六件全文、活动内容或blob身份、四direct refs、PR16/17、正式评论实际编号/全文/唯一性全部远端回读。未完成不预造提交或评论。完成报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、R95-01/02纠正待C06复核，随后STOP_WRITING。交原“MYOS2-A-C06 内核分析主线续建”，提醒Owner发送正式验收消息前选Pro；主线同轮审补正、96及后继。不自签95闭合或96通过，不启动97。

禁止改旧成果/旧回执/旧评论、PR元数据/Draft/base、lead/PR16、master/time、内核/loader/initramfs/Owner脚本；禁止补丁、配置/预处理/汇编/编译/链接、仓库程序/脚本/夹具、Graphviz/QEMU/实机/真实并发、外部libc镜像、本机凭据、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3及跨项目维护。110和124继续显式进入以后主线交接材料。
