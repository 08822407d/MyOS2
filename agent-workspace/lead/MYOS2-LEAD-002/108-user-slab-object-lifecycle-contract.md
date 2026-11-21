---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SLAB-OBJECT-LIFECYCLE-76
record_type: bounded_slab_object_initialization_list_transition_and_discard_contract
evidence_class: 主线任务设计与权限；非执行完成
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；未直接读取客户端或认证后台）
model_per_owner: gpt pro（Owner此前指定）
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 0681d7186406
lead_before_publication_short12: 1ba6acf132a0
fixed_contract_commit: PINNED_BY_OWNER_FORWARDING_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_display_name: MYOS2-A-C05c 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-09
executor_model_per_owner: 次一档 GPT-6＋Extra
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-slab-object-lifecycle-76/
allowed_files: [MANIFEST.md, slab-object-lifecycle.md, facts-and-boundaries.yaml, slab-object-lifecycle-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: []
D00: NOT_APPLICABLE
mandatory_supplements: [R74C-S01, R75-S01, R75-S02, R75-S03]
activation_gate: Owner转发现有C05c并要求执行本固定合同且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
inputs_read: [75固定交付与本轮审查, 固定slub.c与slub_types.h设计预读, 04和05历史限定]
open_questions: []
---

# 第76批：SLAB对象初始化、分配归还与整块下线

继续现有C05c/CHAT-EXEC-09，次一档GPT-6＋Extra，不新开对话。G00通过后连续G01–G04，无D00，不逐专题等待批准。三个相关专题共用六件、一次主线验收；只做阶段2有界静态分析。

## 1. 增量及排除

75只把外部申请/清零/释放接到分配器入口，76补内部状态：A槽布局和freelist/ctor初始化，B分配取出及partial/full迁移，C对象归还、整块下线条件和锁间窗口。目标是把cache、slab、单个object、链表计数和页归还请求分账，形成可检验的状态前后表，而不是再抄一遍入口链。
04的MC-07已记录OR回收条件与锁外判断，MC-08已记录首对象ctor遗漏及空闲指针覆盖；不得把再次找到它们登记为新根因。须增加实际生产者、位宽/容量、对象次序、列表迁移及反例前提。只按原qualified_id追加；未证明的真实可达性/运行后果继续needs_evidence，不要求Owner逐项决定修复。
不重做整个SLUB、buddy合并、锁实现、cache自举、所有ctor调用者、task/exit/PID、用户libc或镜像；不修内核、不写运行触发程序。外部Linux惯例不得替代本树活动代码。

## 2. G00固定输入与范围门

完整读本108号及同一固定发布提交的：reviews/CORE-USER-TASK-STORAGE-LIFETIME-74-DOC-CLOSURE-review.md、reviews/CORE-USER-ALLOCATOR-ENTRY-75-review.md、checkpoints/2026-11-12-allocator75-reviewed-slab-lifecycle76-ready.md。R74-01已CLOSED_DOCUMENTATION_ONLY，74组合及75均PASS_PENDING_LOCAL；不重复验收，不重新打开73/74或58/63历史项。
完整刷新63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md、agent-workspace/tasks/00-gpt-task-protocol-v2.md全部P1–P13。其它九份MyOS2规则和适用Mnemosyne指导仅复用本有效C05c实际独立已读正文，缺项补读；不继承其它聊天已读状态、不重放105接收或建分支PR仪式、不导入跨项目维护。
固定执行0681d7186406完整读75的MANIFEST、allocator-entry.md、deferred-findings.yaml；facts定点读allocation_dispatch_matrix、zeroing_failure_matrix、free_provenance_matrix、earliest_stop_matrix和prior_findings_links；证据C10–C14/C24/C25及相关B组按需回读。核正式评论6813648992。合读R75-S01/S02/S03；本批不是75返工。
同执行固定提交的mm-baseline-04/facts-and-dependencies.yaml定点读MC-07/08/09、A-KM03/04/06/07/08/09及DR-04/RF-02/RF-05；memory-baseline.md只读mm.kmalloc和对应反证。主线CORE-MM-BASELINE-04-review.md §3–4、CORE-USER-VFS-BASELINE-05-review.md §2–3和05/deferred-findings.yaml的errata/status_changes/cross_links合读，旧全局不可达/必然物理后果不能恢复为真值。若讨论sighand消费者，只消费72/73/74已经限定的对象发布与清理，不扩审全部信号。
核PR16/17、直接lead/exec/master/time、开放PR、目标76目录及09本批正式marker。首次写前执行头0681d7186406、源码time@a039d9803ade、新76目录不存在。未知执行头变化先核归属，不覆盖；主线发布108不是执行分支竞争。实际回读公约§1(2)及两分支金丝雀，记录三个必需结构字段和真实模型来源。

## 3. G01/A：槽布局、位宽与初始化顺序

固定mykernel/mm/kmalloc/slub.c完整读order_objects、oo_make、simple_calculate_order、simple_calculate_sizes、simple_kmem_cache_open、alloc_slab、alloc_init_new_slab、setup_object、get_freepointer/set_freepointer；slub_types.h取kmem_cache、kmem_cache_order_objects、slab、kmem_cache_node真实定义。slab_common.c只取create_cache/create_boot_cache传入object_size/align/ctor及初始化来源；不要重审整个自举。
区分object_size、对齐后的size、offset、cache.inuse、slab.inuse、oo.obj_nr和slab.objects。按实际类型和位域记录乘法/移位/截断/除零前提；没有生产证明的零对象、溢出或非法size不能直接宣布真实可达。需用get_order/ALIGN/ilog2时只沿最近真实定义；不运行预处理或sizeof探测。
对首槽、后续槽、最后一个freepointer分别列setup_object/ctor及写入次序。slot数量为1及大于1、没有ctor与存在ctor、有效offset与未证来源各自建表；offset为0必须回到所读cache初始化/赋值范围，不从一次负搜索推出全树恒零。CTOR被调用、对象内容被freelist覆盖、分配后是否仍依赖该内容是不同事实；MC-08原身份及RF-02/RF-05反证保持，不能推出所有使用者必然失败。
强制锚：字段及位宽、size/order/objects生产、槽迭代与setup_object真实参数、set_freepointer写入点和初始化来源。只记录源码请求，不认证真实地址可写或物理容量。

## 4. G02/B：从partial取对象及迁移full

固定slub.c完整读online_new_empty_slab、alloc_single_from_partial、get_from_partial_list、slab_alloc；add/remove partial/full宏分别锚定。沿mykernel/lib/list/double_list.h及实际包含的最近宏/结构核这些操作的计数、节点改写、contains与安全迭代，不凭名称推断。
逐步列cache/slab/object身份、freelist前后、inuse/objects、partial/full成员资格及nr_slabs/total_objects。重点区分：取freelist、get_freepointer参数求值、inuse递增、调用者收到object后判断NULL、达到满值时迁移、解锁和post-hook。后置NULL判断不能倒置为对前面读取的保护。
partial为空时新建slab、上线后重新取对象；不将新slab非NULL直接视为已交付object。空闲链NULL/自环、列表损坏、objects/inuse不一致和无效元数据必须先写前提及最早停止点，不构造未证的运行故障。有限正常模型和违反不变量的反事实模型分栏，不混成可达笛卡尔全集。
强制锚：取出与计数更新、满值迁移、列表实际成员/计数变化、锁请求范围、重试取对象与hook调用门。锁API出现不等于互斥效果已经运行验证。

## 5. G03/C：归还、阈值判断与整块下线

固定slub.c完整读free_single_to_partial、free_to_partial_list、slab_free、offline_exceeded_empty_slab、free_slab；simple_kmem_cache_open和slub_const.h仅用于min_partial实际生产。R75-S02的virt_to_slab实参求值前置继续生效，不越过对象转换失败。
先分“归还前已满需full→partial”“原本partial”“本次减量后空/仍有存活对象”，再把新的partial.count与min_partial的比较放到实际时点。列解锁→条件读取→再次加锁摘链→页归还请求的顺序；锁外窗口只是条件性风险，不据此认证真实并发已发生。
为MC-07建立带合法生产前提的有限状态见证与反例：partial阈值以下/等于/高于、归还后inuse为零/正值、逐slab清空与跨多个full slab交错归还分别考察。不得把“跨slab交错的单线程调用顺序”混为“必须多CPU并发”，也不得把历史“顺序释放不会触发”扩为任意顺序免疫。不用测试程序或执行仓库代码。
核remove_from_partial的实际返回值是否被消费、统计减量是否依赖成功摘链、free_slab的页头条件/flag清除及__free_pages调用门。缺失的调用者不变量列缺口；只到页归还接口，不重做buddy或宣布已经发生物理UAF。MC-07原身份保持，只有新的独立根因才另编号。
强制锚：归还前后inuse/freelist、full→partial变化、真实OR表达式、阈值生产、锁边界、下线统计及页归还请求。

## 6. 六件与验收要求

严格六件见头部；facts至少含source_ledger、slot_initialization_matrix、allocation_transition_matrix、free_transition_matrix、discard_gate_matrix、lock_window_matrix、earliest_stop_matrix、finite_model_cases、prior_findings_links。新条件项SL76-Fxx、缺口SL76-Uxx；旧MC等用原qualified_id，不设新问题数量配额。
核心及最近依赖软上限14份源码、8组定点检索、4次相关目录浏览；必要超限说明，不全仓漫游。证据软上限120、有限模型不超过40、新候选与缺口各不超过12；上限不是目标。图区分活动CALL、调用者顺序、字段数据、宏、注释和ANALYSIS_STOP，不当完整CPU CFG。
本任务适用agent-workspace/tasks/00-gpt-task-protocol-v2.md全部十三条。每条锚非空、逐字连续1–5物理行、单一真实定义内；宏/结构/别名分别定位，真实代码与论点对应。P9-1全40位串禁入；2/13强制锚及定义逐条回源；3固定源/路径与写后回读；4行号仅辅助；5六件原始全文前缀计数和MANIFEST三数一致；6外部事实真实可核出处；7/8 branch_canary_quotes、startup_selfcheck_quote、read_channel齐备；9/10/12无代码/补丁/降级则N/A；11不冒称编译运行。完整本地P9仍NOT_RUN，署名、模型来源、实际覆盖、复用和write_retry_log如实记录。

## 7. G04写入与停止

只新增原claude/dazzling-cori-q0dnyt/PR17下user-slab-object-lifecycle-76/六件及09本人唯一正式顶层评论。旧文件/旧评论、PR元数据、lead/PR16、master/time冻结；C05不代写执行成果，旧08不恢复写权。
首次写前再核exec=0681d7186406。GitHub暂态异常不得一次失败即停：按63号先回读远端是否成功，再间隔安全重试，通常首次加最多三次、约10/20/40秒或相应独立核查，遵守Retry-After；expected-head/force=false，未知并行变化不覆盖。复用已有Git对象，评论marker查重，工具发现与真实写请求分账；中断只补实际缺失步骤。
compare自0681d7186406必须严格六件added、旧件零净改删；新固定提交和活动分支六件全文/blob、HEAD、PR状态与唯一评论全部回读。完成STOP_WRITING，返回真实短提交、评论及A/B/C实际范围，交MYOS2-A-C05独立验收；只报READY_FOR_MAINLINE_REVIEW/mainline_acceptance=PENDING，不自签76、不自行开始77。
禁止内核/旧件修改、修复补丁、预处理/编译/链接、仓库程序/脚本/旧夹具/QEMU/实机、另建分支PR、强推/合并关闭/删分支、本机凭据访问、外部libc/镜像、阶段3、跨项目维护及额外付费模型/子代理。临时纯文本检查和自建有限模型不当内核运行。发布不等于已创建、投递或唤醒另一聊天。
