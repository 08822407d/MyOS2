接收及执行模型：次一档，沿用GPT-6＋Extra。继续现有MYOS2-A-C06a 代码分析执行／CHAT-EXEC-10，不新开对话；独立验收仍由C06使用Pro完成。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-DIRECT-MAP-TABLE-INSTALL-83
record_type: bounded_direct_map_table_install_and_return_contract
evidence_class: 主线设计与限定写入授权；非执行成果或硬件认证
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（Owner本轮要求独立验收及后续安排）
date: 2026-11-13
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 3f41eb242040
lead_before_publication_short12: e8f950fa4ad8
fixed_contract_commit: OWNER_FORWARDED_118_FIXED_URL
inputs_read: [117及82六件和正式评论, 82审查限定, 110及00/18/24/28/63/79, 固定init.c下层建表及最近定义预读]
receiving_model_tier: next_tier_default
executor_display_name: MYOS2-A-C06a 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
executor_rotation: false
old_C05c_EXEC09: STOP_WRITING_OWNER_RETIRED_CONTEXT
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-table-install-83/
allowed_files: [MANIFEST.md, direct-map-table-install.md, facts-and-boundaries.yaml, direct-map-table-install-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: []
D00: NOT_REQUIRED
batch82_acceptance: PASS_PENDING_LOCAL_WITH_MANDATORY_R82_S01_S02_S03
activation_gate: Owner向现C06a转发118固定合同并要求执行且G00通过
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第83批：直接映射的下层遍历、叶项转换与写入记账

82已按R82-S01/S02/S03接收，无新增RETURN。本83没有D00，不重做82或81，也不重新演一轮接收仪式。先G00，通过后连续G01–G04，不逐专题等待批准。源码问题按18号分类保存，不要求Owner逐项选择修复。

## 1. 增量、共有输入与停止范围

82核上层请求和区间表，83进入它明确留下的下层边界，合包三个专题：A根项和子表遍历、地址身份与发布；B已有大页的复用、新建与降级；C末级PTE填充、写入接口和分层返回/计数。三者共享固定init.c、表项定义及83一套六件，不能用文档长度、引文数量或规则读取替代技术工作。

核心固定源为mykernel/arch/x86_64/mm/init.c中的kernel_physical_mapping_init、kernel_physical_mapping_change、__kernel_physical_mapping_init、phys_p4d_init、phys_pud_init、phys_pmd_init、phys_pte_init、prot_sethuge、update_page_count和direct_pages_count；上层init_memory_mapping只作实参与返回消费入口。alloc_low_page/alloc_low_pages按81审查限定承接，不重做缓冲区、buddy或memblock整包。

最近依赖从实际include/别名定点定位：mykernel/mm/fault/pgtable.h、pgtable_macro.h，arch/x86_64/mm/pgtable/pgtable_arch.h、pgtable_macro_arch.h、pgtable_const_arch.h，以及page/page_arch.h、page_macro_arch.h、page_const_arch.h、mm/highmem/highmem_macro.h。结构类型、none/leaf/present谓词、PFN/权限掩码与init写入包装须沿真实定义补读，不预设Linux同名实现。

主线预读取得真实四层调用体与宏转发，但没有认证所有set_*_init/populate_*_init包装的实际定义。执行者必须在固定time中定位并读取；默认分支搜索无结果不证明固定树无定义。若有界查找后仍缺实现，保存查找范围、配置面和最小接口缺口，只停止依赖它的实现认证，继续其它可证路径；不凭函数名补写实现，不把未找到定义直接宣告构建必失败。

不扩展全部虚拟内存、用户缺页/VMA、设备ioremap、完整引导汇编、固件、AP启动、所有CPU和TLB协议、全局锁原子实现或完整链接验证。邻近spp_getpage/fill_pud/fill_pmd/fill_pte除非证实是本链直接依赖，不因同文件出现而扩成另一专题。源码注释提到Linux、Xen或Intel不是本包外部规范证据。没有内核修改、补丁、构建或运行授权。

## 2. G00：固定材料、继承边界与写前现场

L=agent-workspace/lead/MYOS2-LEAD-002/，取Owner转发118 URL同一真实固定发布提交；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/，82输入固定3f41eb242040；源码仅time@a039d9803ade。只复制真实短SHA，产物不写完整40位串。

完整读取本118、L/110-owner-communication-and-delivery-continuity.md、L/reviews/CORE-USER-DIRECT-MAP-RANGE-82-review.md、L/checkpoints/2026-11-13-directmap82-reviewed-table-install83-ready.md及82正式评论6843923007。合读R82-S01/S02/S03：提升前后模型分开、字节/PFN单位分开、反向clamp与分析停止分开、memory/free/真实映射及图中调用者分开。

重新全文读取63号、79号和协议v2全部P1–P13；110也须全文。原82六件、九份项目约束、113接收约定和适用Mnemosyne指导，只可按本有效C06a自己实际完整读取且固定对象一致的范围明确复用；缺段补齐，不继承C06、旧C05c或其它对话已读声明，不倒造旧临时接收日志，不导入其它项目维护。

本批直接依赖81存储，须完整合读L/reviews/CORE-USER-EARLY-PGTABLE-STORAGE-81-review.md及其R81-S01/S02/S03；涉及页分配清零、初始移交、memblock及PFN来源时，按依赖合读77/78/79/80正式回执及相应DOC-CLOSURE。已读同对象可复用，不全量重开旧批。PT81/DM82/MR79/PB78/PA77/BM80、原MC/LB/GAP、05/MR-02收窄与58/63撤回均保留原qualified_id及限界。

直接核PR16/17、lead/exec/master/time、开放PR、83目标目录及本人本批正式marker。exec须3f41eb242040，time a039d9803ade，master直接头预期e8e60cd19540；lead取118真实发布头，不用父e8f950fa4ad8代替，不把PR缓存base_sha当master真值。已有目标/marker先核是否已交付；未知并行推进不得覆盖或重复发布。

实际读取并保存公约§1(2)整句及time/master双金丝雀原文、固定来源和read_channel；配置面必须在否定“启用/定义/调用”等结论前核对并限定范围。G00全部通过才解除本83的STOP_WRITING，无需Owner或旧C05c重复确认。

## 3. G01/A：根项、地址域与子表取得/发布

完整读__kernel_physical_mapping_init、phys_p4d_init及phys_pud_init中定位子表的部分，沿pgd_offset_k、pgd_val、pgd_page_vaddr、p4d_offset、pud_index、pmd_offset等实际定义取证。区分init_mm.pgd、根槽、下级页基址和各层索引；折叠p4d的类型/宏不当作额外真实分配层。

分别跟踪物理字节paddr_start/end、__va得到的vaddr/vaddr_end、__pa回译和层级next边界。PUD使用虚拟索引而PMD/PTE使用物理索引时，是否等价须取实际PAGE_OFFSET、位移和掩码前提，不能凭通常直映偏移猜结论。空、反向、未对齐、跨PGD/PUD和高端回绕分别给出最早合法性门。

根项非零判断与present/none/bad语义不能混同。已有根项路径怎样取得子表、新根项路径怎样alloc_low_page、先递归填充后populate，必须按真实顺序登记。分配零/非零、可写映射、清零和真实表页容量沿81保留前提，不能从返回指针非零直接认定可解引用。

交table_identity_and_address_matrix、root_and_child_selection_matrix、publication_order_matrix。强制锚覆盖根选择、地址/索引、next、已有/新子表分支、宏转发、实际父项发布及锁调用位置。锁只记录所读区间，不认证跨CPU互斥、TLB生效或回滚；未证节点后只能作显式隔离模型。

## 4. G02/B：PUD/PMD大页的复用、新建与降级

完整读phys_pud_init、phys_pmd_init及prot_sethuge。对每层按空项、已有非叶子项、已有大页叶项分开，再按该层page_size_mask位、after_bootmem和init参数分支；候选位不等于已经存在同尺寸硬件映射。既有非叶子项是否优先继续下钻，按实际源码而非“优先使用大页”的概括判定。

记录允许复用时paddr_last/pages如何变化、是否重写表项，以及不允许大页时如何取得旧保护位、去huge位、分配小一级表并发布。旧叶的物理页框、传入paddr与生成子叶的页框要分别追踪；源码注释声称保留属性，不证明全部页框、PAT/NX/权限等必然一致。pte_t类型转换、实际pte_clrhuge/pte_pgprot、pfn_pud/pfn_pmd和掩码均需最近定义。

新建大页路径核对齐、prot_sethuge、实际写入包装和init选择。空项/已有叶/已有表、2M和1G候选组合、边缘部分区间、跨层终点、分配失败及先行部分写入设计代表模型；不得把未初始化子表或失效指针继续画成正常降级成功。旧页/旧表是否释放、回滚或同步若未见，仅限所读函数说明，不做全局不存在断言。

交existing_entry_transition_matrix、huge_to_child_mapping_matrix、protection_and_frame_matrix。强制锚覆盖none/leaf条件、复用和降级分支、旧prot取得、新叶构造、下钻调用及父项发布。把“已有项被复用”“新写入”“只改候选”三种效果分账；普通条件风险记录为TI83-Fxx或原ID增量，不擅自修复。

## 5. G03/C：PTE填充、写接口与分层返回/计数

完整读phys_pte_init以及set_pte_init、set_pmd_init、set_pud_init、p4d_populate_init、pud_populate_init、pmd_populate_kernel_init的实际实现或所选别名。必须区分init参数与after_bootmem：前者可能选择写接口，后者参与统计/分配阶段；不能靠名称推断二者恒相同。安全写、普通写、WARN/检查、最终store及错误后的继续行为按真实代码，不把“safe”当拒绝覆盖保证。

PTE循环核初始索引、指针形成、paddr_next、范围外continue、已有非空项复用、新项构造和paddr_last更新。循环每次的数组索引与末尾尾后一指针区别于实际访问；空/部分页、恰到512槽边界、已存在全部/部分项和端点回绕分别建条件表。若先形成指针后检查范围，不能把后置条件当先行合法性证明。

跨PTE/PMD/PUD/根层分别记录paddr_last初值、更新分支、下层返回覆盖和最终单位。不要把默认end、复用分支的next边界或最后一次递归返回看成所有请求已完成；返回值可能与精确请求end、实际新写项数量不同，须以所读源码说明。只有可达前序均合法返回才继续统计。

完整读update_page_count及direct_pages_count，分别统计新建/复用、阶段真/假、各层pages的增量和调用先后。统计不是唯一映射页总量，也不自动证明降级旧大页已从另一层扣除；该关系限所读更新点评价。根发布与上层load_cr3/flush仍按81/82分开，不进入硬件刷新全证明。

交pte_loop_and_store_matrix、init_write_interface_matrix、return_endpoint_matrix、page_count_matrix及跨A/B/C接合表。强制锚覆盖PTE分支/store、实际init包装、各层返回/计数更新和上层直接消费。真实定义未取得的接口必须标缺口，不能以extern声明或Linux同名实现替代。

## 6. 六件与发布前内容门

严格六件见头部。facts包含source_ledger、各矩阵、finite_model_cases、prior_findings_links、R82/R81补充的消费入口及事实键到真实证据的对应；DOT只表达本批真实调用者、宏和数据关系。没有D00，不重画82整包或改旧87标签。

核心与最近依赖软上限24份（金丝雀另计）、8组定点检索、4次相关目录浏览；必要超限须解释具体缺失依赖，不全仓漫游。新短锚软上限130、独立条件模型最多36、新候选和缺口各最多12；均为上限而非配额。同一代码被矩阵/模型/引文复用，不相加夸大工作量。

适用协议v2全部十三条。MANIFEST原字段task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check必须齐全，模型来源按实际可见/Owner告知，后台不认证。公约整句及time/master原文金丝雀逐字保存。

P9-1六件40hex零命中；2/13全部强制锚和真实单一定义内连续1–5行逐字回源；3真实路径及固定/活动回读；4行号辅助且不越文件；5三整数自检与六件原始全文前缀相符，含说明文字；6外部事实须真出处，不用无来源硬件惯例；7/8公约、金丝雀、read_channel存在；9/10/12无代码/补丁/降级则N/A；11未运行项明确NOT_RUN。已知不合格项必须在本批未发布文本中先纠正。

先冻结事实键/证据ID，再逐项核正文、矩阵、模型、deferred和图实际引用内容。字面命中不等于论点成立；每个数值样例明确所在阶段、after_bootmem、init、候选mask、已有表项状态、地址单位及需要的callee返回，未用条件可写N/A。禁止把提升前/后、物理/虚拟、请求/成功和不同调用者混算；普通证据缺口不包装为新缺陷根因。发布前清理本批过期pending字段，不倒造历史记录。

## 7. G04：原执行分支安全发布与停写

只向claude/dazzling-cori-q0dnyt/PR17新增83目录六件及本人一条正式顶层评论。唯一marker：MYOS2-LEAD-002-CHAT-EXEC-10 / CORE-USER-DIRECT-MAP-TABLE-INSTALL-83 / FORMAL_DELIVERY。旧成果/旧评论、PR元数据/Draft/base、lead/PR16、master/time冻结，主线不代写执行results。

写前再核exec=3f41eb242040及目标/marker，使用真实完整旧HEAD作expected_sha、force=false、单父commit。GitHub暂态异常先核远端实效再按63号间隔安全重试，通常首次加三次并遵Retry-After；成功blob/tree/commit/comment复用，不重复创建，未知并行推进不覆盖。只记实际写请求与重试，不把工具发现混作写入。

compare起点到新头严格六件added、旧件零净改删；新固定提交与活动执行分支六件全文/blob、四direct refs、PR16/17、正式评论全文及唯一性全部回读，得到真实提交/评论号才报告。检查记录并入六件，不另增第七件。

完成报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING后STOP_WRITING，经PR17交回现MYOS2-A-C06 Pro独立审核。不自签83、不启动84、不交旧C05/C05c。Owner只需转发固定启动词，不下载ZIP、不逐项审查技术问题。

禁止内核/loader/initramfs/Owner脚本修改、补丁、预处理/编译/链接、仓库程序/脚本/旧夹具执行、Graphviz、QEMU/实机/真实并发、硬件实验、外部libc/镜像、本机凭据、额外模型/子代理/API付费、新分支PR、合并关闭强推删除、阶段3和跨项目维护。只可按既有许可做新建临时纯文本检查与不执行仓库代码的有限整数模型，明确非内核运行。110号继续显式进入以后每次主线交接固定必读清单。
