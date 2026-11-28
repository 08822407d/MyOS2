接收及执行模型：次一档，沿用GPT-6＋Extra。继续现有MYOS2-A-C06a 代码分析执行／CHAT-EXEC-10，不新开对话；独立验收仍由C06使用Pro完成。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-DIRECT-MAP-RANGE-82
record_type: bounded_direct_map_page_size_range_partition_and_request_contract
evidence_class: 主线设计和限定写入授权；不是执行成果或硬件证明
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（Owner交本主线独立审核）
date: 2026-11-13
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 05fe580c12a3
lead_before_publication_short12: df960bf5c5e9
fixed_contract_commit: OWNER_FORWARDED_117_FIXED_URL
inputs_read: [116固定合同及81六件, 81独立审核限定, 110及00/18/24/28/63/79, 固定init.c及最近定义的有界预读]
receiving_model_tier: next_tier_default
executor_display_name: MYOS2-A-C06a 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
executor_rotation: false
old_C05c_EXEC09: STOP_WRITING_OWNER_RETIRED_CONTEXT
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-range-82/
allowed_files: [MANIFEST.md, direct-map-range.md, facts-and-boundaries.yaml, direct-map-range-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: []
D00: NOT_REQUIRED
batch81_acceptance: PASS_PENDING_LOCAL_WITH_MANDATORY_R81_S01_S02_S03
activation_gate: Owner向现C06a转发117固定合同并要求执行且G00通过
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第82批：大页选择、映射区间整理与请求结果的边界

第81批已由C06按R81-S01/S02/S03接收，没有新增执行返工。本批先G00，通过后连续G01–G04，无D00、不逐专题等待批准。不重做81、79/80，也不把接收旧限定变成新一轮交接仪式。

## 1. 三个技术接合面与不做的工作

81分析了页表存储从哪里来；82检查映射请求如何决定页大小、拆成有限区间、再由上层发出和记账。A能力位与页大小候选，B区间拆分/提升/合并，C请求顺序与返回值消费，共用init.c和一套固定定义证据。这些是新问题，不以增加引文、模型或说明字数替代实际分析。

核心固定源为mykernel/arch/x86_64/mm/init.c中的map_range、NR_RANGE_MR、page_size_mask、direct_gbpages、simple_probe_page_size_mask、probe_page_size_mask、save_mr、adjust_range_page_size_mask、split_mem_range、page_size_string、init_memory_mapping、init_range_memory_mapping、memory_map_bottom_up、init_mem_mapping。只按实际引用补最近定义：mm/early/memblock.c及memblock.h、memblock_macro.h，mm/highmem/highmem_macro.h，include/linux/kernel/math.h及实际clamp定义，arch/x86_64/mm/pgtable/pgtable_const_arch.h、page/page_const_arch.h、page/page_macro_arch.h、early/e820_const.h；CPU能力字段、PG_LEVEL枚举和__pa_symbol沿当前包含关系定点定位，未读到不猜路径或数值。

kernel_physical_mapping_init宏和__kernel_physical_mapping_init只取真实签名、调用参数及必要返回接口，递归建立页表的内部算法留在边界。本包不验证全部PTE/PMD/PUD递归、TLB硬件效果、CPUID初始化全链、引导汇编、loader/固件、设备MMIO、用户VMA/缺页、全局锁与并发或整个buddy/slab。myos_memory_map中的设备函数只记为先行callee，不深入设备初始化。

不修改内核、不生成补丁、不运行仓库代码或旧脚本。不从源码注释、Linux惯例或函数名称推断实际内存布局、硬件能力与映射成功。普通问题按18号保存，不请Owner逐项决定修复；真实访问/授权/未知并行冲突只暂停受影响动作。

## 2. G00：固定输入、限定继承和现场

L=agent-workspace/lead/MYOS2-LEAD-002/，取Owner转发117 URL的同一真实固定发布提交；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/，最新执行输入取05fe580c12a3；源码只用time@a039d9803ade。短SHA只从实际连接器输出复制，产物不写完整40位串。

完整读取本117、L/110-owner-communication-and-delivery-continuity.md、L/reviews/CORE-USER-EARLY-PGTABLE-STORAGE-81-review.md、L/checkpoints/2026-11-13-early-pgtable81-reviewed-directmap82-ready.md和81正式评论6842499415。完整合读R81-S01/S02/S03：证据错位按事实定位表消费，直接页分配与slab过滤分开，模型、地址与图边不跨层套用。原81六件可按本有效C06a自己实际完整读取且固定对象一致的范围复用；不继承C06或旧C05c的已读声明，不能只看交付摘要。

重新全文读取63号、79号、协议v2全部P1–P13和110。其余九份项目规则、113接收约定与适用Mnemosyne指导可按本有效C06a的真实已读范围复用，缺项补齐。旧临时报告不可恢复的历史限制保持，不倒填旧接收/刷新日志、不导入Mnemosyne自身维护。

涉及memblock查询及区间输入，须合读L/reviews/CORE-USER-MEMBLOCK-REGION-LIFECYCLE-79-review.md的R79-S01/S02/S03及79-DOC-CLOSURE、80-review的R80-S01/S02/S03。涉及物理页、清零或启动移交时，按依赖合读77-review及77-DOC-CLOSURE、78-review及78-DOC-CLOSURE。旧MR79/PT81/PB78/PA77/BM80和MM-04的MC/LB/GAP、05/MR-02收窄限定保留原qualified_id；同对象已读可明确复用，不全量重开已结批次。

核PR16/17、四条direct ref、开放PR、82目标目录及本人本批正式marker。exec起点必须05fe580c12a3，time a039d9803ade，master直接头预期e8e60cd19540；lead取117的真实发布头，不用父df960bf5c5e9替代。PR缓存base_sha不作master真值。目标已存在或头变化先查归属/是否本批已完成，禁止覆盖未知推进或重复交付。

实际回读公约§1(2)完整原句与time/master两条金丝雀，保存原文、读取通道和固定对象；凡做未启用/不存在等判断，先读配置面并限制断言范围。G00全部通过后才对82解除STOP_WRITING，无需Owner再次批准或旧C05c确认。

## 3. G01/A：全局能力、候选位与范围提升

完整读simple_probe_page_size_mask及其别名、page_size_mask/direct_gbpages定义和直接调用者。明确PageSize_Ext、Page_1GB作为本函数读取的能力输入，2MiB与1GiB候选位的独立条件、OR写入及direct_gbpages清零；实际硬件是否具有该能力由生产者另证，不能拿字段名当CPUID实测。

区分全局page_size_mask、map_range每项掩码、pgprot权限与最终硬件页大小。枚举值/位移/常量须取最近定义。初始调用和重复调用的状态前提分开：是否重置、旧位是否保留必须按真实赋值判断，不凭init命名排除第二次调用。

adjust_range_page_size_mask对不具备某候选位的区间，按PMD/PUD向外取整并调用memblock_is_region_memory。核何时可提升位、after_bootmem如何控制调用、区域上下界是否改变；“整个取整跨度被一个memory记录覆盖”与“区间空闲、未保留、NOMAP允许、真正已映射”不是同一判断。memblock查询须按79空表/尾槽/单区覆盖限定消费，不重做其变形算法。

交capability_and_mask_matrix、range_promotion_matrix；代表模型包含能力组合、初始/重复状态、已带位/未带位、取整跨度位于一个有效区域/跨洞/仅多个相邻记录联合覆盖、after_bootmem不同值、端点回绕。强制锚覆盖真实能力条件、位更新、尺寸/层级定义、提升判断和查询实际callee。先行无效求值后仅保留隔离模型，不认证实际提升或映射成功。

## 4. G02/B：五段拆分、容量与相邻合并

完整读map_range、NR_RANGE_MR、save_mr、split_mem_range和必要page_size_string；PFN_DOWN、round_up/down和PAGE/PMD/PUD单位定义实际回源。逐段核非大页对齐头部、2MiB段、1GiB段、1GiB尾部、2MiB尾部的起终点、pfn推进、掩码及省略空段条件。

建立字节区间、PFN区间和保存回字节字段三套单位关系；非页对齐start/end的向下截取、空区间、反向输入、首尾恰在PMD/PUD边界、多个边界跨越和整数回绕分别列出。正常覆盖性、间隙/重叠和端点守恒都要有明确输入前提，不把日志打印的end-1或函数返回当覆盖证明。

save_mr只在start_pfn<end_pfn时写项，nr_range与五元素数组的物理容量分别取证；panic调用是检查后的控制流条件，不运行它，也不擅自增加未见return。实际调用init_memory_mapping从nr=0开始，与任意外部nr输入分开。数组尾后一地址与实际元素访问区分，遇容量前提失败不能继续虚构正常表格。

按实际顺序核after_bootmem控制的提升发生在合并前，以及相邻端点/相同掩码门、memmove长度、old_start保留、i--/nr_range--和再次检查的状态。合并只改区间表，不等于撤销或重写实际页表。page_size_string只是掩码显示，不能拿显示1G证明机器用了1G叶。

交partition_coverage_matrix、range_capacity_matrix、merge_transition_matrix；代表模型按必要数量设计，不生产每个数值的重复表。强制锚覆盖五段关键边界、save写入与容量门、调整调用、合并条件/移动/索引和真实返回。不要复写整份81容量分析。

## 5. G03/C：上层请求、统计值和返回端点

完整读init_memory_mapping、init_range_memory_mapping、memory_map_bottom_up别名与init_mem_mapping。init_memory_mapping的局部数组、清零、split调用和逐项kernel_physical_mapping_init参数分开；取真实宏转发与callee接口，核ret是否逐次覆盖、最后如何转PFN返回。不能把最后一段返回值说成全体区间已映射的事务证明；未深入的callee内部保持未认证。

init_range_memory_mapping按for_each_mem_pfn_range消费memory区间，先把PFN转物理字节、clamp到请求范围、跳过空交集，再请求映射和增加mapped_ram_size。核返回端点是否参与统计、callee未正常返回时哪些后续步骤不能成立；以源码实际更新为准区分“请求字节累计”和“实证映射字节”。定点读memblock_macro.h及memblock.c::__next_mem_pfn_range的迭代、终止和PFN取整，不把memory枚举说成memory减reserved或固件表的所有类型。

init_mem_mapping核probe在前、max_pfn移位、ISA范围无条件请求、bottom_up时kernel_end前后两次调用、else中的活动行为、设备映射入口、max_low_pfn更新及load_cr3/flush的后置顺序。__pa_symbol(_end)只给表达式，不能猜实际镜像地址。反向上下界、端点为0、memory洞、能力位全关、没有合法区间、callee返回部分端点等只作明确隔离模型，不调用真实内核。

被注释的add_pfn_range_mapped不是活动生产者；实际max/min映射窗口的全局写者没有穷尽就不能断言不存在。与81的fallback依赖建立来源关系即可，不自动关闭PT81-U06，也不重画81整包。设备callee、storage分配、根发布与TLB真实生效分别保留先行门。

交mapping_request_matrix、return_and_accounting_matrix、boot_order_and_boundary_matrix及A/B/C关联表。强制锚覆盖真实调用层级、参数、ret覆盖/转换、迭代和clamp、累计更新、方向分支、ISA/两段请求及后置根切换入口；CALL与CALLER_SEQUENCE、MACRO、DATA和ANALYSIS_STOP必须分清。

## 6. 六件、证据导航与发布前检查

严格六件见头部。facts包含source_ledger、各矩阵、finite_model_cases、prior_findings_links和R81-S01/S02/S03消费入口；deferred区分新增DM82-Fxx条件候选及DM82-Uxx缺口，旧问题只追加关系不另造独立根因。没有D00，不生成旧81更正包，不改旧99标签。

核心与最近源码软上限24份（金丝雀另计）、8组定点检索、4次相关目录浏览；必要超限说明具体依赖，不全树漫游。新短锚软上限130、源码条件模型最多36、新候选/缺口各最多12。全部是上限而非产出配额，矩阵/模型/引文有重叠时不能相加夸大工作量。

本任务适用agent-workspace/tasks/00-gpt-task-protocol-v2.md全部十三条。MANIFEST必须有task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check；模型按可见/Owner来源记录，后台未知不认证。公约原句和time/master金丝雀逐字保存，自检三个数字为真实整数并与六件原始全文计数相符。

本批特别落实既有P2支持关系检查：先固定证据ID，再逐项检查正文/facts/model/deferred/DOT引用所指的实际内容。存在编号不代表支持断言；以“事实键→路径::符号→证据ID”对照，防止插入一个锚后整段编号错位。条件门必须有条件本身的支持，不能只引参数或对象定义。准备期pending与交付完成字段分开，更新本批未发布文本中的过期状态，不倒造历史操作。

P9-1全文40hex零命中；2/13全部强制锚和真实单一定义内连续1–5行逐字回源；3路径真实、固定/活动成果回读；4行号仅辅助且不越文件；5原始前缀计数包括说明文字，三数一致；6外部事实须真实出处，本包不靠无来源惯例补缺；7/8公约、金丝雀、read_channel确实存在；9/10/12无代码/补丁/降级则N/A；11所有未运行项标NOT_RUN。已知错引或缺字段必须在本批发布前纠正，不能仅凭主干可用跳过硬门。完整本地P9未执行，不自行签PASS。

## 7. G04：限定发布、远端回读和停写

仅在原claude/dazzling-cori-q0dnyt/PR17新增82目录六件，另加本人一条正式顶层交付评论。唯一marker：MYOS2-LEAD-002-CHAT-EXEC-10 / CORE-USER-DIRECT-MAP-RANGE-82 / FORMAL_DELIVERY。旧成果/旧评论、PR标题正文/Draft/base、lead/PR16、master/time不动；主线不代写执行results。

写前再核exec=05fe580c12a3、目标/marker，以真实完整旧HEAD为expected_sha、force=false、单父提交。GitHub暂态异常不因一次失败停工；先核远端实际是否写成，再按63号间隔安全重试，通常首次加三次，遵Retry-After；复用已成功blob/tree/commit/comment，只补缺项，未知并行推进不得覆盖。记录真实尝试和效果，不把工具发现次数当写请求。

compare起点至新头必须严格六件added、旧件零净改删。新固定提交和活动执行分支六件全文/blob、四direct refs、PR16/17、本人正式评论全文及唯一性全部回读；得到真实SHA/编号后才报告，不能把blob创建当正式交付。内部检查记录并入六件，不新增第七件。

完成仅报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，随后STOP_WRITING，通过PR17交回现MYOS2-A-C06 Pro独立审核。不自签82、不启动83、不交旧C05/C05c。Owner只转发固定启动词，无需下载ZIP或逐项审查问题。

禁止内核/loader/initramfs/Owner脚本修改、补丁、预处理/编译/链接、仓库代码/脚本/旧夹具执行、Graphviz、QEMU/实机/真实并发、硬件TLB验证、外部libc/镜像、本机凭据、额外模型/子代理/API付费、新分支PR、合并关闭强推删除、阶段3及跨项目维护。只可按既有许可做新建临时纯文本检查和不执行仓库代码的有限整数模型，明确非内核运行。110号以后每次主线交接须显式进入固定必读清单。
