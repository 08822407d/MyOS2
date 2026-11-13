接收及执行模型：次一档，GPT-6＋Extra。执行位置：新普通对话“MYOS2-A-C06b 代码分析执行”，身份MYOS2-LEAD-002-CHAT-EXEC-11；验收仍交本C06使用Pro。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-EARLY-ROOT-HANDOFF-84
record_type: bounded_early_root_storage_fixup_and_C_entry_handoff_contract
evidence_class: 主线工作设计与条件写入授权；不是执行成果
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（Owner指定主线验收及后续安排）
date: 2026-11-13
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 18047d54b0a2
lead_before_publication_short12: b129d706e350
fixed_contract_commit: OWNER_FORWARDED_120_FIXED_URL
receiving_model_tier: next_tier_default
executor_display_name: MYOS2-A-C06b 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-11
old_C06a_EXEC10: STOP_WRITING_OWNER_RETIRED_CONTEXT
old_C05c_EXEC09: STOP_WRITING_OWNER_RETIRED_CONTEXT
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-root-handoff-84/
allowed_files: [MANIFEST.md, early-root-handoff.md, facts-and-boundaries.yaml, early-root-handoff-paths.dot, deferred-findings.yaml, evidence.md]
receiver_package: 119-executor-continuation/00-C06a-to-C06b-handoff-package.md
receiver_startup: 119-executor-continuation/01-MYOS2-A-C06b-receiver-startup-prompt.md
prior_return_items: []
D00: NOT_REQUIRED
batch83_acceptance: PASS_PENDING_LOCAL_WITH_MANDATORY_R83_S01_S02_S03
activation_gate: Owner将119固定启动词发到新C06b要求接收执行，独立加载并通过G00
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第84批：启动页表的来源、修正与C入口交接

83已按R83-S01/S02/S03接收，无新增RETURN或D00。Owner明确C06a上下文过大，84在新对话开展；不是给旧10继续执行，也不是主线交接。新11先独立接收并通过G00，再连续G01–G04，无需旧C06a或Owner再次批准内部步骤。

## 1. 三个新接合面与范围

83分析了direct-map如何逐层写表，但当前根表、初始映射和早期可访问性仍是先行条件。本包合并三个关联专题：A汇编静态页表及存储/覆盖，B启动C辅助函数的地址修正和临时恒等映射，C进入高地址C代码后的清零、根项复制及后续交接。共用一套源索引和六件，不重做81缓冲区、82分段、83递归建表，也不以规则读取或文档长度替代技术增量。

核心固定输入：
- mykernel/arch/x86_64/kernel/head_64.S：startup_64、common_startup_64所在实际符号范围、initial_code、early_top_pgt、early_dynamic_pgts、level3_kernel_pgt、level2_kernel_pgt、level2_fixmap_pgt、phys_base、相关section与数据宏。
- mykernel/arch/x86_64/kernel/head64.c：__startup_64、next_early_pgt、early_pmd_flags、clear_bss、x86_64_start_kernel；startup_64_setup_gdt_idt只取先行调用/地址依赖。
- mykernel/arch/x86_64/kernel.lds的入口、相关输出节、_text/_end/__bss_start/__bss_stop定义；mykernel/mm/misc/init-mm.c的init_top_pgt/init_mm；mykernel/mm/fault/memfault_api.h的swapper_pg_dir。
- mykernel/arch/x86_64/kernel/setup.c::setup_arch与mykernel/arch/x86_64/mm/init.c中81已审的early_alloc_pgt_buf、myos_early_map_pgt_buf_into_directmap及init_mem_mapping，只核根/调用交接接口，不重做旧算法。

最近定义沿实际include定位：arch/x86_64/mm/mm_const_arch.h及page/pgtable常量、page_arch.h/page_macro_arch.h、mm/fault/pgtable.h与宏别名、include/linux/init/init.h、include/linux/kernel/linkage.h、asm/sections.h及rip_rel_ptr/PMDS/EARLY_DYNAMIC_PAGE_TABLES/KERNEL_IMAGE_SIZE真实定义。需要证明start_kernel到setup_arch的路径时，读取mykernel/init/main.c相关完整调用者定义段；myos_early_init_system等未取到实现的先行callee必须保留边界，不能默认不改页表或必返回。

不进入整个loader/GRUB/固件、32位切换全链、AP启动正确性、CPU特性探测全链、异常处理、完整GDT/IDT/GS栈验证、设备映射、所有TLB/PCID/跨CPU同步或完整链接构建。assembly/C只作文本分析，不预处理、不汇编/编译/链接/运行。源码注释称PIC、8KiB、物理地址或“仅启动一次”，均需实际定义/构建上下文支持，不能按注释直接认证。

## 2. G00：新对话接收、独立规则加载与现场

L=agent-workspace/lead/MYOS2-LEAD-002/，以下工作材料取Owner转发119/120 URL中的同一真实固定发布提交；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。源码仅time@a039d9803ade，83输入固定18047d54b0a2，短SHA从实际连接器输出复制。

完整读取119的00交接包/01启动词、本120、L/110-owner-communication-and-delivery-continuity.md、L/reviews/CORE-USER-DIRECT-MAP-TABLE-INSTALL-83-review.md、L/checkpoints/2026-11-13-table-install83-reviewed-early-root84-exec11-ready.md；读取83正式评论6847954250及固定83目录六件全文。截断必须补齐，不用摘要或文件存在代替读取。

严格依119顺序：先形成真实Mnemosyne接收报告；再独立全文九份MyOS2约束和110；随后单独完整加载当前适用Mnemosyne指导并保存真实刷新记录。新11没有C06a/C06/旧09的读取资格；全部声明只对应本新对话实际动作。临时记录不可落盘就如实说明，不造路径，G00后把实际记录并入84六件，不增加第七件。不导入Mnemosyne维护路线或其它项目。

独立全文合读L/reviews/CORE-USER-EARLY-PGTABLE-STORAGE-81-review.md及L/reviews/CORE-USER-DIRECT-MAP-RANGE-82-review.md的全部补充限定，继承的是限定而非已读声明。83的R83-S01/S02/S03同样全部适用：互斥分支、真实写包装、present/反转条件、同层索引和请求/计数/返回分账。80及更早条目只在实际消费其具体结论时按原记录与对应review/DOC-CLOSURE定点回查，未回查部分只保留ID和状态，不把摘要升级为证据，不全量重开旧批。

核PR16/17、四direct refs、开放PR、84目标目录和本人正式marker。exec须18047d54b0a2，master直接头预期e8e60cd19540，time a039d9803ade；lead须为119/120真实发布头，不拿父b129d706e350替代。PR缓存base_sha不是master真值。已有结果/marker或HEAD变化先查归属，若本批已完成只核交付，未知推进不覆盖。

独立取得公约§1(2)完整原句及time/master金丝雀原文，保留read_channel和固定来源；先全文读options_flags.cmake再作任何未启用/未调用/定义缺失判断。G00全部通过后11才取得本84限定写权，旧10/09继续停写。不能仅做G00便正常结束，后续G01–G04已经授权；真实阻断只停依赖该项的动作。

## 3. G01/A：静态表存储、地址表达与名义覆盖

完整读取head_64.S相关数据块和定义它们的宏；将early_top_pgt、early_dynamic_pgts、level3_kernel_pgt、level2_kernel_pgt、level2_fixmap_pgt、init_top_pgt分别登记。按.fill/.quad/PMDS及真实宏计算名义槽数、字节数、对齐、section和初始非零项，不凭注释认证8KiB根表或默认每种对象均512项。

把标签链接地址、减__START_KERNEL_map后的表达、phys_base初值、运行时RIP相对地址和最终物理装载地址分开。读取KERNEL_IMAGE_SIZE、PMD_SIZE和PMDS真实循环/表达；末级表覆盖范围须以所选输入和具体表项为前提。level3的510/511槽与early根511槽是不同层级，不把它们合为一个索引。

结合kernel.lds和section宏判断哪些对象落入可被clear_bss影响的候选范围，哪些在.data/.init.data等段；没有实际link map就明确只到链接脚本/输入节模式推导。函数/数组声明、定义和最终对象布局是不同证据层，不因存在extern就声称存储已分配。

交static_table_inventory、link_address_and_storage_matrix、initial_mapping_coverage_matrix。代表模型取空槽/末槽、PMDS计数与容量、不同名义镜像尺寸、两个表层边界；强制锚含真实数据标签块、PMDS/尺寸定义、section映射、init根数组和swapper别名。对汇编引文使用实际SYM_DATA_START/END或代码标签所在范围，不把邻接块拼成同一定义，不预先展开虚构最终机器码。

## 4. G02/B：启动地址修正与临时恒等映射

完整读__startup_64及startup_64中实参形成/调用/CR3写入序列。追踪p2v_offset来源、rip_rel_ptr得到的physaddr、phys_base/load_delta赋值、MAX_PHYSMEM_BITS与PMD对齐检查。load_delta==0的注释不等于已由代码保证；CFLAGS/PIC注释与实际构建选择只在所读范围内判断。

核early根高地址项及level3最后两项的修正、early_pgts前两页的角色和next_early_pgt写2。根索引i后i+1与PUD/PMD使用取模的差异需按实际数组长度评价；不能把后一层取模当作前一层已防越界。对真实表页地址与普通标签地址、加flags与按位OR、页框对齐、整数/指针宽度分别说明。

PMD临时填充的循环次数、i/idx、物理基址、步长和%PTRS_PER_PMD决定是否覆盖或复用槽。选取合法对齐小范围、跨PUD/PGD界、靠近末槽、名义镜像跨单表容量、非零load_delta和检查未通过的代表条件模型；每例写明地址域、表容量、算术及先行callee前提。需要停止的是非法路径证明，不能在源代码未拒绝处虚构拒绝分支。

启动信息memcpy只记为返回前的先行调用和目的区依赖，不重新分析80的整个表解析。记录返回值怎样参与CR3值构造，再到高地址跳转；源中的mov cr3/间接跳转不是实际硬件成功证据。

交startup_argument_and_delta_matrix、temporary_identity_mapping_matrix、root_fixup_and_switch_matrix。强制锚覆盖参数形成、检查/赋值、实际修正点、两页选择、根与下级索引、PMD循环、返回值和汇编CR3构造/跳转。ASM符号范围与C定义边界均须真实定位。

## 5. G03/C：C入口清零、根项复制与后续交接

核initial_code指向x86_64_start_kernel的定义以及common_startup_64实际到该间接调用的序列；仅处理启动CPU相关路径，共用secondary符号不等于本批已证明AP启动。原始启动寄存器、RIP相对/高地址执行和堆栈/先行callee必须保留条件，不凭符号名自动通过这些门。

完整读clear_bss、x86_64_start_kernel；依次区分BSS清零、clear_page(init_top_pgt)、早期异常入口、myos_early_init_system、复制init_top_pgt[511]=early_top_pgt[511]、start_kernel调用。复制单根项不是深复制所有页表；两根项可指向同一子树，后续写下级表可能影响共享视图，但实际地址/活动根仍另证。

沿实际调用证据把后续setup_arch中的81提前映射及init_mem_mapping末尾load_cr3(swapper_pg_dir)接上。不能把“数组已清零/复制”“当前CR3已切换”“后续TLB刷新成功”写成同一事件；只读到某个callee入口时须停在其未证返回/可能副作用门。未穷尽活动CR3写者或mapped窗口生产者，不宣称它们不存在，也不自动关闭PT81-U03/U06、DM82-U08或TI83相关根身份缺口。

交C_entry_root_lifetime_matrix、root_alias_and_copy_matrix、handoff_order_and_unproven_gates_matrix及三专题接合表。代表模型覆盖仅511槽复制、两个根共享子页、清零前后对象身份、先行callee不返回、早期与最终根不同/相同的条件；强制锚含真实入口指针、BSS边界/清零、copy语句、实际调用者和后续根写入口。局部读到的新证据可以缩小缺口，但任何旧项关闭必须交主线判断。

## 6. 交付与发布前完整性

严格六件。facts含source_ledger、矩阵、finite_model_cases、prior_findings_links、R83/R82/R81的消费入口，并嵌入本新对话实际接收/九规则/110/指导刷新记录。deferred用ER84-Fxx/ER84-Uxx登记本批条件候选与缺口；旧问题只增补关联，不改ID或重复统计独立根因。无D00，不另做旧83纠正包。

源码及最近依赖软上限24份，8组定点检索、4次相关目录浏览；必需超限说明具体依赖，不能全仓漫游。新短锚软上限130、条件模型最多36、新候选和缺口各最多12；均为上限非配额。规则接收成本与技术增量分别记账。

适用协议v2全部P1–P13。MANIFEST必有task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。模型仅按可见/Owner来源记录，不认证后台。每例明确阶段、地址域、所选类型/宽度、load_delta/p2v_offset、表容量、相关状态及callee返回；不适用写N/A。

P9-1六件40hex零命中；2/13每条强制锚为真实单一定义/数据标签块内连续1–5行原文且支持论点；3路径、固定/活动回读真实；4行号只辅助且不越文件；5三整数自检与六件全文保留前缀相符，包括说明文字；6外部事实需真实可解析出处，硬件手册按P5，不以源码注释替代；7/8逐字公约、金丝雀及read_channel齐备；9/10/12无代码/补丁/降级则N/A；11未运行项明确NOT_RUN。完整本地P9未做，不自签PASS。

先冻结事实键与ID，再核正文、矩阵、模型、deferred及DOT的实际支持内容；只匹配字面或标签存在不算语义通过。准备期pending与完成记录分开，已知错误在发布前纠正，不倒造旧对话接收事实。

## 7. G04：限定发布、回读与停写

只在原claude/dazzling-cori-q0dnyt/PR17新增84目录六件，另加11本人一次正式顶层交付评论。唯一marker：MYOS2-LEAD-002-CHAT-EXEC-11 / CORE-USER-EARLY-ROOT-HANDOFF-84 / FORMAL_DELIVERY。旧成果/评论、PR元数据/Draft/base、lead/PR16、master/time和内核冻结，主线不并写results。

写前再核exec=18047d54b0a2及目标/marker，以真实完整旧HEAD作expected_sha、force=false、单父提交。GitHub暂态先核远端实效再按63号间隔安全重试，通常首次加三次并遵Retry-After；成功对象/评论复用，未知并行推进不覆盖，工具发现与实际写请求分账。

compare须严格六件added、旧件零净改删；新固定提交及活动执行分支六件全文/blob、PR16/17、四direct refs、正式评论全文及唯一性全部回读。真实SHA与编号取得后才报告，检查记录并入六件，不增加第七件。

完成报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，随后STOP_WRITING，通过PR17交回现MYOS2-A-C06 Pro独立验收。不自签84、不启动85，不交旧C06a/C05c，不恢复旧10/09写权。Owner仅新建对话并转发启动词，无需技术审PR、下载ZIP或处理每项缺陷。

禁止内核/loader/initramfs/Owner脚本修改、补丁、预处理/汇编/编译/链接、仓库程序脚本及旧夹具执行、Graphviz、QEMU/实机/真实并发/硬件实验、外部libc镜像、本机凭据、额外模型/子代理/API付费、新分支PR、合并关闭强推删除、阶段3及跨项目维护。仅可按既有许可新建临时纯文本检查和不执行仓库代码的有限整数模型，并明确非内核运行。110号以后每次主线交接继续显式固定引用。
