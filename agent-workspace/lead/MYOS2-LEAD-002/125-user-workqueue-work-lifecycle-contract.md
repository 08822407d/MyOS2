执行模型：次一档，沿用Owner的GPT-6 Sol／GPT-6＋Extra选择。执行位置：现有“MYOS2-A-C06b 代码分析执行”，身份MYOS2-LEAD-002-CHAT-EXEC-11。交回本C06正式独立验收时使用Pro，并同轮安排后继。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-WORKQUEUE-WORK-LIFECYCLE-88
record_type: bounded_work_submission_pool_handoff_and_callback_lifecycle_contract
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（Owner对正式验收和后继设计的选择）
date: 2026-11-15
base_snapshot: time
read_channel: connector
evidence_class: 主线设计与条件写权，不是88执行成果
source_pin_short12: a039d9803ade
result_base_short12: bc611665b38f
lead_before_publication_short12: fc14432f2a56
fixed_contract_commit: OWNER_FORWARDED_125_FIXED_URL
executor_display_name: MYOS2-A-C06b 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-11
executor_rotation: false
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-workqueue-work-lifecycle-88/
allowed_files: [MANIFEST.md, workqueue-work-lifecycle.md, facts-and-boundaries.yaml, workqueue-work-lifecycle-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: []
D00: NOT_REQUIRED
batch86_composite: PASS_PENDING_LOCAL_R86_01_CLOSED_DOCUMENTATION_ONLY
batch87_acceptance: PASS_PENDING_LOCAL_WITH_R87_S01_S02_S03
activation_gate: Owner向现C06b转发本125固定合同并要求执行且G00通过
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
inputs_read: [123与87六件及正式评论, 86回执及87追加纠正, 00/18/24/28/63/79/110及协议v2, workqueue.c相关完整定义与实际头文件]
owner_decisions_requested_now: []
open_questions: []
---

# 第88批：工作项提交、worker接续和回调生命周期

87已按R87-S01/S02/S03限定接收，R86-01另件关闭。88无D00、不重做86/87，继续现C06b/11，不新开、不轮换、不向旧10/09索取确认。G00通过后连续G01–G04，不在专题之间再次等待Owner发令。

## 1. 增量与固定入口

87核到工作队列的每CPU槽分配及一个读取点，未证明整个工作队列可运行。88以有效对象与必要初始化为明确条件，研究三个接合面：A提交入口、工作项状态与返回语义；B队列选择、链表发布及worker通知；C移交到scheduled、取得PWQ及回调前后清理。既查源码中真实边界，也保留87尚未解决的地址/初始化门，不默认构造成功。

核心源仅time@a039d9803ade：
- mykernel/kactive/workqueue/workqueue.c：queue_work_on、__simple_queue_work、kick_pool、simple_assign_work、process_scheduled_works、process_one_work、simple_worker_thread完整定义。alloc_worker/create_worker、manage_workers/maybe_create_worker只读本包需要的字段初值或调用/返回接口，不扩大为线程创建、资源管理全审计。
- mykernel/kactive/workqueue/workqueue.h：queue_work、get_work_pwq/work_struct_pwq、need_more_worker、may_start_working、keep_working、first_idle_worker、worker_enter_idle/leave_idle、worker_set_flags/clr_flags的实际实现与配置门。
- mykernel/kactive/workqueue/workqueue_types.h：work_struct、worker、worker_pool及本包使用的PWQ/WQ字段；workqueue_const.h中的工作位、PWQ掩码、CPU选择、pool/worker标志；workqueue_macro.h与workqueue_api.h的实际宏/接口。
- mykernel/kactive/kactive_type_declaration.h中的相关别名和work_func_t；mykernel/lib/list/double_list.h、double_list_macro.h、实际container_of定义及List_s/List_hdr_s，读完整必要链，不以first名字假定空表返回NULL。
- 直接依赖的atomic_long_read、WARN_ON/WARN_ON_ONCE、IRQ保存恢复、spin_lock及irq变体、smp_processor_id按真实include定位必要定义和配置。wake_up_process、schedule、kthread入口只作既有有界接口与返回门，不重做整个调度器、唤醒或kthread专题。

主线预读取得workqueue.c的1–740及此前同对象740–1120、workqueue.h全文、宏/API及相关结构。INIT_WORK的一次默认分支定位只命中注释，不能据此断言全树没有初始化，也不能凭上游惯例写出本仓库不存在的初始化链。执行者若需要真实work->data/entry/func生产者，应在本范围内定点定位固定源；未取得就保留未证前提，不伪造函数、调用者或自动“按Linux补齐”。

排除延迟工作/timer、完整取消/flush/drain/destroy、rescuer/unbound全实现、完整AP/CPU热插拔、全树竞态/硬件内存序认证和所有工作队列用户。普通源码问题只收存，不修复。不得用scope外缺口把可独立开展的局部分析全部停掉。

## 2. G00：同一执行对话接续

L=agent-workspace/lead/MYOS2-LEAD-002/，R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。工作材料取Owner转发本125 URL中的真实固定发布提交，不把父fc14432f2a56当作新发布头；原87六件固定bc611665b38f，正式评论6872044008。

完整读取本125、L/110-owner-communication-and-delivery-continuity.md、L/124-model-routing-and-pro-message-budget.md、L/reviews/CORE-USER-PERCPU-ALLOCATION-87-review.md、L/reviews/CORE-USER-PERCPU-BOOTSTRAP-86-DOC-CLOSURE-review.md、L/checkpoints/2026-11-15-percpu87-reviewed-workqueue88-ready.md及87正式交付评论。合读R87-S01/S02/S03、R86-S01/S02/S03及实际消费的R85/R84/更早正式回执限定，不重开R86-01或R84-01。

重新全文读取63、79和协议v2全部P1–P13。其它九规则、119接收与Mnemosyne指导、旧成果仅在本有效C06b自己实际读过且固定对象一致的范围明确复用；缺段补齐，逐项记新读/同会话复用/未读。不继承主线或旧执行者读账，不倒造历史接收，不导入其它项目维护。

核PR16/17、lead/exec/master/time四direct refs、开放PR、88目录及本人正式marker。exec预期bc611665b38f、master直接e8e60cd19540、time a039d9803ade，lead取本125真实发布头；PR缓存base_sha不是master真值。已有结果先核归属和完成情况，未知并行推进不覆盖。

先完整读options_flags.cmake再作配置/否定判断；独立核公约§1(2)整句、time/master两项金丝雀和读取通道。G00前保持只读/STOP_WRITING；门通过后获得88限定写权并直接继续，无需再要求Owner确认模型、旧执行者或开工。

## 3. G01/A：提交接口、工作状态与返回语义

从queue_work到queue_work_on再到__simple_queue_work，完整核真实参数和返回流程。区分WORK_CPU_UNBOUND这个CPU选择值、WQ_UNBOUND队列标志和当前CPU编号，不把它们当同一概念。分别检查work->entry、work->func、work->data和PWQ关联；常量/初始化宏存在不证明真实对象已经采用它们。

核工作pending测试/置位是否活动、重复提交的判断发生在哪层、内部函数是否返回可供外层使用的状态，以及外层true/false到底由哪条语句产生。WARN的实际宏及正常返回前提必须取证，不凭名字认为一定记录后继续。返回true、实际入链、通知worker、实际执行回调四种状态不得合并。

交submission_and_return_matrix、work_state_and_data_matrix、cpu_selection_and_preconditions_matrix。条件例覆盖默认/显式CPU、空entry/已链接entry、pending位不同但其它条件相同、data无PWQ标志/有合法编码、NULL或不可证WQ/PWQ的最早止证点。必须写前提和原始对象类型，不把非法输入假想成实际调用。

强制锚：完整实际入口的调用/返回关键语句、work_struct字段、CPU选择值/标志各自真实定义、活动与注释分支的界限、必要WARN包装；否定结论限定所读固定路径。

## 4. G02/B：pool选择、队列发布与worker通知

追__simple_queue_work的per_cpu_ptr读取、PWQ到pool、锁、entry检查、链表插入、kick_pool以及解锁顺序。87的PA87-F05/U06等仍是地址前提；可以分析声明有效对象的后续局部契约，但不能据此宣布原构造缺口已经关闭。

完整读kick_pool、need_more_worker、first_idle_worker及必要计数/空表包装，区分无待处理工作、nr_running变化、有/无idle worker及POOL_BH分支。所读源中的忙等、注释调用、wake_up_process请求和bool返回分别登记；返回true不自动等于睡眠任务已被唤醒或获得CPU。其下级实现未核到的条件保持接口缺口。

核pool->worklist和idle_list的anchor/count一致性及引用对象，区分插入前、已入链但尚未通知、通知未返回和通知返回四个前缀。锁调用不是全系统并发安全证明；不得运行真实并发实验。必要的worker字段初值只读最近生产者，不扩成全部线程管理。

交pool_queue_publication_matrix、idle_worker_and_kick_matrix、locking_and_nonreturn_prefix_matrix。代表模型给出列表/计数、标志、调用返回的前后状态；不跨过非法访问、未返回循环或未知callee继续编造成功轨迹。强制锚为pool选择、实际列表操作及校验依赖、各分支/返回和worker通知调用。

## 5. G03/C：scheduled移交、PWQ解码及回调前后

完整读simple_assign_work、process_scheduled_works、process_one_work和simple_worker_thread的相关外层循环。核从pool工作表到worker scheduled表的删除/插入、返回值消费、空表终止测试和计数更新；读list_headr_first_container到container_of及空表判定的必要实现，不能用“取第一个”这个名字推定有NULL保护。

将get_work_pwq需要的标志与PWQ编码，和提交路径实际写了哪些字段并列。找不到生产者时保留缺口，不把注释的pending处理当活动实现。区分关联PWQ、选择回调函数、current_work/current_func/current_pwq赋值、描述字符串访问、scheduled删除、锁释放、回调调用、重新加锁及字段清理；任何先行解引用或callee返回未证时，后段只能作为独立条件模型。

回调可不返回、可改变工作项或重新入队，只按明确假设分析局部后果；不能默认回调结束后work对象仍存活，也不借此扩大成任意回调安全认证。worker flags与nr_running增减核实际掩码/分支；管理分支、schedule和唤醒只保留最近接口及已确认历史限制，不重审整套调度。

交scheduled_transfer_and_empty_matrix、pwq_decode_and_callback_matrix、completion_and_requeue_boundary_matrix及A/B/C接合表。模型选零/一/多工作项、错误/缺失PWQ标志、已关联/未关联数据、回调正常返回/不返回、声明的重新入队、worker标志组合和外层休眠门。强制锚包含真实获取与解码、callback调用点、前后状态写入及结束条件。

## 6. 六件、证据和规模

严格六件。facts含source_ledger、矩阵、条件模型、事实键到证据ID、prior_findings_links、新读/复用账；候选WQ88-Fxx和缺口WQ88-Uxx分账。遇到具体既有workqueue或调度条目时定点合读正式回执并保留原qualified_id，不宣称历史从未研究过，不重复登记根因。

源码软上限24份（金丝雀另计）、8组定点检索、4次相关目录浏览；必要超限说明实际近邻依赖，不全仓漫游。新锚软上限120、模型最多30、候选/缺口各最多12，都是上限而不是数量目标。保留公约要求的矩阵双格式；结构化字段完整但避免不必要重复展开，正文突出关键结论/反例，不把所有模型原JSON再逐字复制一次。文档长度不充当技术工作量。

协议v2全部P1–P13适用。MANIFEST十四字段：task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。实际模型只按可见/Owner来源记账，不能认证后台。

P9-1六件40hex零命中；2/13短锚在真实单一定义内连续1–5行且支持论点，不混宏外条件或另一函数；3实际路径和固定/活动回读；4附行号辅助且不越界；5按六件全部原始前缀计数，说明文字和字符串也计入；6外部语义有实际官方具体出处/版本，未取到就保留缺口；7/8公约整句、双分支金丝雀、read_channel；9/10/12无代码/补丁/降级则N/A；11未运行明确NOT_RUN。完整本地P9仍待本地，不自签PASS。

在既有许可范围内可做独立临时纯文本检查或有限整数模型，不运行/复制运行仓库程序；如实说明检查实现、输入、输出及局限，不能倒造运行记录。先核事实与模型，再核正文/矩阵/deferred/DOT的实际支持，不只核ID存在；已知错误发布前纠正。源码调用、callee返回后接续、数据关系和假设用不同图示，不把不同调用帧合并。

## 7. G04：发布、回读、停写

唯一新增区为头部88目录，六件之外仅11本人一次正式PR17顶层评论，marker为MYOS2-LEAD-002-CHAT-EXEC-11 / CORE-USER-WORKQUEUE-WORK-LIFECYCLE-88 / FORMAL_DELIVERY。旧成果/评论、PR元数据/Draft/base、lead/PR16、master/time、内核和脚本冻结。

写前再次核exec=bc611665b38f、目标目录和marker；用真实完整旧HEAD作expected_sha，force=false、单父提交。GitHub暂态先核远端是否已成功再按63号间隔安全重试，通常首次加三次并遵Retry-After；成功对象/评论复用，不重复创建，未知并行推进不覆盖。工具发现次数与真实写请求分账。

compare严格六件added、旧件零净改删；新固定六件全文、活动分支六件内容/blob、四direct refs、PR16/17及正式评论内容/编号/唯一性均回读。真实提交与评论号由后置交付记录承接，不预造、不加第七件。发布任务不等于另一对话自动启动。

完成报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，然后STOP_WRITING，经原PR17交回“MYOS2-A-C06 内核分析主线续建”，提醒Owner发送正式验收消息前选Pro；由主线一轮完成独立验收和后继安排。不自签88，不启动89，不重开86已闭合文档项。

禁止内核/loader/initramfs/Owner脚本修改、补丁、CMake配置、预处理/汇编/编译/链接、仓库程序/脚本/旧夹具/Graphviz/QEMU/实机、真实并发或硬件实验、本机凭据、外部libc镜像、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3及跨项目维护。

下一步写入仅11通过G00后的88六件和本人评论；C06不并写results，旧10/09停用。110及124须继续显式进入以后每次主线交接包、接收启动词和固定必读清单。
