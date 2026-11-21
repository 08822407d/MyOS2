---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PAGE-REF-LIFETIME-18
record_type: chat_order_zero_page_reference_lifetime_contract
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-11-01
read_channel: connector
evidence_class: 第18批任务授权及验收条件，不是已完成的源码分析
authorization_ref: Owner报告17交付；28号同轮验收与下一工作包持续授权；36号执行角色接续
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-02
executor_display_name: MYOS2-A-C03b 代码分析执行续建
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
receiving_model_tier: next_tier_default
instruction_base_short12: d542a09075f9
required_review_record: CORE-USER-WP-COW-17-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-WP-COW-17-review.md
source_branch: time
source_pin_short12: a039d9803ade
result_base_short12: b7f89a7bc5c0
default_branch: master
default_branch_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-page-ref-lifetime-18/
allowed_files: [MANIFEST.md, page-ref-lifetime.md, facts-and-boundaries.yaml, page-ref-lifetime-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: b7f89a7bc5c0及之前全部文件，含C03b第17批和所有历史勘误
writer_assignment: C03b仅本批六件及自己的PR17评论；交付后停写待C03验收
previous_executor_delegation: C03a与CC02旧委派不恢复；其本地草稿及物理进程状态unknown
activation_gate: Owner向已有C03b转发37号，绑定及写前核对通过后L00–L04连续执行
external_execution_started: false
prior_status: WP_COW17_PASS_PENDING_LOCAL_WITH_R17_S01_S02_S03
phase: 2
kernel_change_authorized: false
kernel_patch_production_authorized: false
preprocessing_or_build_authorized: false
c_or_asm_execution_authorized: false
qemu_authorized: false
external_libc_read_authorized: false
phase3_authorized: false
merge_authorized: false
runtime_evidence: NOT_RUN
acceptance_ceiling: PASS_PENDING_LOCAL
owner_decisions_requested_now: []
inputs_read:
  - 17六件全文、C03b交付评论和本轮17回执；35/36及18/24/25/28既有规则。
  - 固定time mempolicy.c、buddy.c全文及buddy.h 1–240、highmem_macro.h、page-flags.h和page-flags_macro.h选定段落，作为引用/分配/释放入口预检。
  - 04回执MR-02及相关可见限制定向重读；未将旧报告绝对结论当本批事实，未闭合全树页状态或buddy算法。
  - PR16/17、直接refs、开放PR第三项为空、PR16完整变更路径及17六件增量compare。
open_questions:
  - 普通order-0页的初始及复用计数、空闲结构状态、folio_put与直接free的差异需要本批增量证据。
  - 不预判全局永不释放、永不合并、重复分配、数据泄漏或引用下溢已发生。
---

# 第18批：引用归零是否等于物理页真的重新可用

## 1. 目标、入口和职责

17已按PASS_PENDING_LOCAL接收；先合读R17-S01/S02/S03的权限与图关系修正即可，不重做17、不另交小型勘误。任务仅在现有C03b中执行，使用Owner指定GPT-6＋Extra，不新建会话、不重做完整25号/36号首次接收。你自己的17接收记录可沿用，但不能把前任C03a的读取当本轮完成。

本批以17所用普通order-0数据页为中心，区分四个事实：描述符/引用初值；分配并形成持有；引用递减至零；进入空闲结构并可能再次分配。对照folio_put路径与直接__free_pages路径，不能把同名“释放”、计数变化、链表入队或接口注释当成同一保证。

Owner启动句：

> 执行CORE-USER-PAGE-REF-LIFETIME-18。完整读取37号固定任务书及17主线回执，沿用已有接收和GPT-6＋Extra。按L00–L04连续完成普通order-0页的初始化/分配、page与folio引用、释放接口及空闲链边界的静态分析、自查、原PR17提交和远端回读。只交指定新目录六件，旧件保留；普通问题入库，不修内核、不重做16/17。交付后停写交C03验收，不自行开始下一批。

任务内自主推进，不逐项向Owner请示；真正权限/输入/并行写入阻断才停相应路径。证据不足如实PARTIAL，其他安全项继续。发布文件不自动唤醒对话，也不代表18已开工。

## 2. L00：固定输入与历史去重

完整读本任务及required_review_record，区分指令d542a09075f9、结果基线b7f89a7bc5c0和源码a039d9803ade。先读options_flags.cmake再作配置相关判断，仍不预处理/编译。18号延期、24/25分工、28同轮派发及36的C03b身份有效；17写权已结束，本批重新明确新目录。

17只取A11–A16、A31–A37、A52及局部引用账，结合主线R17补充；13仅取folio_put/源缓存页与私有页不同对象的已核边界；16只作PTE复制未证明数据页新增持有的相关输入，不重审COW或fork。

按主题定点读取04的mm.page_alloc代表基线及主线CORE-MM-BASELINE-04-review.md中MR-02，再看CORE-USER-VFS-BASELINE-05-review.md对旧表述的后继处置。历史“PageBuddy恒假/永不合并/全局不可达”不能直接继承；旧结论受后来回执约束。不重读全部调度/内存检查器、历史大YAML或无关IR/MR调度选择。

导航：mykernel/mm/page_alloc/{buddy.c,buddy.h,buddy_const.h,buddy_types.h,page-flags.h,page-flags_macro.h,mempolicy.c}、mm/highmem/highmem_macro.h、mm/misc/mm_init.c及真正使用的链表帮助函数。路径要以真实树为准；默认分支搜索只导航，结论回固定time。主线预检不计入你的VERIFIED数量。

## 3. L01：页对象、初次入池和可证明初值

只追解释普通order-0页所需的page/folio布局、_refcount、_mapcount/page_type及相关union别名，区分数据页描述符、物理页内容、链表节点和compound_head。读取实际类型及写入，不凭Linux惯例把这几个字段当独立存储或自动同步。

定点核__init_single_page、init_page_count/set_page_count及memblock_free_pages对所选页的初始化/首次归还调用链；只追到该页进入所读空闲结构的明确端点。列出证明初值的调用顺序和前提：例如哪一步设置1、哪一步设置0，是否只适用于首次投放而非所有复用。不能只引注释保证启动时每个PFN都走过路径。

不全面审计E820/memblock/NUMA/保留区布局、整机可用页数或全部启动初始化。无法闭合某页实际来源时保留符号初值n；允许构造明确条件的0→1算术场景，但不得把示例升级为全系统不变量。

## 4. L02：普通页分配、包装和所有权建立

核folio_alloc别名、folio_alloc_noprof、alloc_page/alloc_pages至__myos_alloc_pages的实参传递、正常/NULL出口与page_folio转换条件。COW所用order0路径与更一般gfp/order接口分开，不能从参数名推定它们真的被传递；若包装忽略参数，只记实际范围和后果条件，不修改。

沿__rmqueue_smallest、get_page_from_free_area、del_page_from_free_list和必要expand动作核页出队、PageBuddy/private/计数变化。链表remove和delete是否重叠、重复调用后如何影响链，必须读取真实帮助函数；不能仅凭函数名判定双删或正确幂等。只围绕一页分配与其直接依赖，不扩成通用链表框架审计。

记录_refcount增加的实际对象、次数与初值来源；加一不一定等于设置为1。post_alloc_hook的清零与标志初始化单独列示，17关于数据字节缺清零保证作为已核输入，不重做全部copy/PTE链。分配NULL可能在哪一步被消费或解引用须保留语言前提。

锁只核所选分配路径的实际获取/归还及调用者条件，别把gfp注释当实际回收/睡眠策略；不证明原子原语、SMP或所有调用者安全。

## 5. L03：引用操作、两种释放与再可用边界

分别完整追get_page/put_page、folio_get/folio_put、page_ref_inc/dec、atomic_dec_and_test的调用对象和归零分支；到实际函数体而非接口注释。head/tail转换仅核解释order0前提及特殊输入的最低范围，非完整compound/大页审计。mapcount和_refcount分列，增加PTE不自动代表任一计数已增加。

对照以下出口：folio_put归零→__folio_put；直接free_pages→__free_pages→__free_pages_ok→__myos_free_one_page及空闲链写入；memblock_free_pages首次入池。逐步记录是否读取/递减/清零引用、是否清flags、是否查伙伴、是否入空闲链。源码调用free不等于引用已归零，入链也不自动证明无其他持有者或可安全复用。

对__myos_free_one_page的伙伴选择/阶数推进，只展开判定空闲状态、链表摘挂和本页重新可见所必需的直接帮助函数。高阶路径、页类型写入者全树穷尽和合并正确性不作无限扩展；MR-02要求的未知继续保留。出现新根因候选可登记，但不要为“闭合全部分配器”重做04。

给出精简对象/状态矩阵：已知首次投放页、来源未闭合的复用页、分配失败、有效get/put且结果非零、put降零、直接free时计数仍正或为零、已入链再分配、对象/阶数/别名不满足前提。每行列_refcount、mapcount/page_type、free-list状态、实参有效性、锁、真正返回/副作用和unknown。重复归还或仍有持有者只作条件分析，不执行触发。

用17的成功/复核失败引用账演示这些出口如何衔接即可，不重算全部COW：所选旧映射引用仍未知时，必须保留n而非假定有“正确的两份”。无法证明实际归还就停在已核接口，不宣称泄漏率、UAF、重复分配或无限碎片化已复现。

## 6. L04：六件交付及证据质量

严格仅allowed_files。正文先回答“持有、计数、归零、入空闲链、再分配”的不同状态；facts含精简对象、真实调用/data/effects、条件矩阵与锚点；DOT对应；deferred只登记增量/追加证据；evidence列实际读取、自查及未做项；MANIFEST逐阶段completed/partial。不得为接收、运行记录或勘误增加第七件。

新ID建议PL18-*，引用旧项必须带完整任务前缀；已有问题作evidence_append并保持旧状态，不能把每次再次见到__folio_put空体当一个全新bug。没有新增问题可明确写0，不凑数。

源码强制结论附真实path::symbol及1–5行逐字引文，包含真正调用、判定掩码、赋值或标签；全部强制引用回源。配置文本、静态推论、字符串核验、解析和运行分层。不设引用数量目标，避免机械重复切片。源码引文放evidence时，正文和facts用可追溯ID精确引用即可。

call只画直接调用/明确宏绑定；压缩多步路径标为call_path并写中间层，不能冒充direct call。data统一消费方→提供方；输出赋值/链表修改/加减引用单独effects。函数形参、字段值、局部变量及物理对象保持区别，按R17-S02规则自查，禁止全体箭头反向却宣称图语义通过。

允许自己的临时沙箱现有文本工具做引用、JSON/YAML、图ID及有限计数算术检查，不安装、不执行仓库脚本/C/ASM、不操作Owner工作树。仅有工具可见不算运行成功，人工核对不称机械检查；没有全局锁证据不声称持有全局租约。无需额外模型/API或Deep Research。

只回READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，静态验收上限PASS_PENDING_LOCAL。自查通过不替代C03验收；普通问题按18号延期，不请求Owner当前修复决定。

## 7. 单写者、提交和中断恢复

C03b / CHAT-EXEC-02为本批唯一执行写者，沿用原PR17和claude/dazzling-cori-q0dnyt；首次写前应为b7f89a7bc5c0。本批权限不恢复C03a/CC02、不授予lead或PR导航权限，导览/教学对话继续只读。C03继续独占lead及最终验收，执行方不得改PR标题/正文/Draft状态或他人评论。

写前核直接master/time/执行头、开放PR及路径交集和本批目录。准备时master=e8e60cd19540、time=a039d9803ade、只有PR16/17开放，PR16无本批结果前缀。现场仍须复查；time前进记录新头但不静默换源，lead正常前进不算执行冲突。已有同名成果先查任务及交付，不能覆盖或重复起批；不明变化停写核实。

可一次或按已核自身提交顺序交付六件；交付前仅以当前blob身份修订本批新件。全部历史文件冻结，正式交付后本批也冻结。只正常快进，不新建/改名分支或PR、不重置/强推。接口的比较更新能力与真正全局锁分开；读写竞态仍须如实保留。

写调用结果不明先读远端路径/提交/评论，已经成功则只补余项。中断进度保全在既定六件或自己的PR17评论，不新增通用日志/检查器；准确写PARTIAL及下一安全动作，不承诺后台续跑。

最终从固定结果头完整读回六件，再与活动分支核blob/头；compare b7f89a7bc5c0到最终头只能新增本批六路径，无旧件净修改/删除或未知提交。后置结果记自己的PR17交付评论，给真实短提交和MANIFEST入口，不反复写自引用提交。交付后停止，Owner仅通知C03；本主线直接读取并同轮安排后继。

## 8. 不新增的权限

禁止修改内核/用户程序/旧脚本/旧成果、生成内核修复补丁、预处理/编译/运行C或ASM、内核/QEMU/硬件实验、安装/挂盘/读取凭据和仓库外libc/镜像、直写master/time/lead、合并/关闭PR、删分支、强推或进入阶段3。静态资源路径分析不是修复实施或内存安全的运行认证。
