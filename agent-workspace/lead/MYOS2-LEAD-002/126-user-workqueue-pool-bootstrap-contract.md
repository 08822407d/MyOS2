执行模型：次一档，沿用GPT-6 Sol／GPT-6＋Extra。执行位置：现有“MYOS2-A-C06b 代码分析执行”，身份MYOS2-LEAD-002-CHAT-EXEC-11。交回本C06正式独立验收时选择Pro，同轮审核纠正、新成果并安排后继。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-WORKQUEUE-POOL-BOOTSTRAP-89
record_type: bounded_worker_pool_initialization_worker_creation_and_manager_progress_contract
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner正式验收和设计选择）
date: 2026-11-15
base_snapshot: time
read_channel: connector
evidence_class: 主线有界设计与条件写权，不是89执行或运行成果
source_pin_short12: a039d9803ade
result_base_short12: e9f1ad989ea9
lead_before_publication_short12: cf2377d7c580
fixed_contract_commit: OWNER_FORWARDED_126_FIXED_URL
executor_display_name: MYOS2-A-C06b 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-11
executor_rotation: false
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-workqueue-pool-bootstrap-89/
allowed_files: [MANIFEST.md, workqueue-pool-bootstrap.md, facts-and-boundaries.yaml, workqueue-pool-bootstrap-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R88-01]
D00: REQUIRED_DOCUMENTATION_ONLY
batch88_acceptance: RETURN_DOCUMENTATION_ONLY_RETAIN_R88_S01_S02_S03
activation_gate: Owner向现C06b转发本126固定合同并要求执行且G00通过
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
inputs_read: [125合同与88固定六件及正式评论, 88独立审核, 110及124和适用九规则, 固定workqueue核心与pool存储及main.c实际入口预读]
owner_decisions_requested_now: []
open_questions: []
---

# 第89批：工作线程池初始化、worker创建和管理进展

88已审核，RETURN仅R88-01证据文档补正，技术主干按R88-S01/S02/S03保留。89先追加D00，再完成三个相邻新专题，不重做提交/回调整包。继续现C06b/11，不新开或轮换，不向退休10/09索取确认。G00通过后连续D00、G01–G04，无需再次发令。

## 1. 技术增量与真实固定入口

88假设有效pool/worker输入来分析提交和回调，留下对象生产、初始计数与管理进展的条件门。89查A静态池与两阶段初始化；B worker资源/线程建立、挂接和发布；C manager/retry进展与失败前缀。共用一套证据，不扩大为整个工作队列或调度器实现审计。

核心源均为time@a039d9803ade：
- mykernel/kactive/workqueue/workqueue.c：worker_pool_assign_id、init_worker_pool、init_cpu_worker_pool、workqueue_init_early、workqueue_init；alloc_worker、format_worker_id、create_worker、worker_attach_to_pool；manage_workers、maybe_create_worker。上述函数须完整读取；simple_worker_thread仅取管理调用/返回与既有88限定，不重新逐段审回调。
- mykernel/arch/x86_64/smp/percpu_area.c：bh_worker_pools、cpu_worker_pools的真实DEFINE_PER_CPU_SHARED_ALIGNED定义；mykernel/kactive/workqueue/workqueue_macro.h的BH/CPU池枚举；workqueue_types.h的worker_pool、worker、workqueue_attrs及实际字段；workqueue_const.h的池/线程标志、池数和nice等必要常量；workqueue.h中need_more_worker、may_start_working、need_to_create_worker、worker_enter_idle与相关计数辅助。
- mykernel/init/main.c：start_kernel与kernel_init_freeable的对应调用位置；必须区分活动调用和注释，不能拿master搜索片段或Linux惯例代替固定time。本主线已读取完整main.c，两处workqueue初始化调用均为注释，只能作局部入口边界，不推全树无其它调用。
- 必要近邻：每CPU地址/CPU遍历宏、CONFIG及BUG_ON/BUG/panic接口，IDR/IDA初始化/申请/归还，kzalloc_node/kfree、KMEM_CACHE，IS_ERR/PTR_ERR，kthread_create_on_node、set_user_nice及wake_up_process的最近实际包装。沿真实include定位定义，不凭名称造路径或结论。

主线已取得workqueue.c相关完整定义、workqueue.h、池宏/API、结构、实际perCPU静态存储与启动源位置。IDR/IDA及kthread只补这条消费链的参数、返回类别、资源所有权和最近包装；完整树算法、线程创建/调度/唤醒全链、格式化库、slab/buddy/memblock全审计均排除。未知实现保留接口缺口，不静默假定正确。

既有PA87地址/初始化和WQ88提交/回调风险继续作为前提。不能用“未来可能运行此函数”替代目前启动可达性，也不因入口注释就停止所有独立的局部静态契约分析。不修源码、不解除注释，不运行构建或实验。

## 2. G00：读取、模型、现场和限定写权

L=agent-workspace/lead/MYOS2-LEAD-002/，R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。主线工作材料取Owner转发本126 URL中的真实新发布提交，不能用父cf2377d7c580替代新合同头；原88六件固定e9f1ad989ea9，正式评论6889090097。

完整读取本126、L/110-owner-communication-and-delivery-continuity.md、L/124-model-routing-and-pro-message-budget.md、L/reviews/CORE-USER-WORKQUEUE-WORK-LIFECYCLE-88-review.md、L/checkpoints/2026-11-15-workqueue88-reviewed-pool89-ready.md及88正式评论。合读R88-01、R88-S01/S02/S03、R87-S01/S02/S03、R86-S01/S02/S03及实际消费的更早正式限定。R86-01已闭合、87限定PASS_PENDING_LOCAL保持，不重开旧文档事项。

重新全文读取63、79和协议v2全部P1–P13。其余九规则、119原接收与适用Mnemosyne指导、旧固定成果，仅能在本有效C06b自己实际已读且对象一致的范围明确复用；缺段补齐，逐项分新读/同会话复用/未读。不继承主线或旧执行者的读取资格，不倒造历史加载，不导入其它项目维护。

核PR16/17、lead/exec/master/time四direct refs、开放PR、89目标目录及本人正式marker。exec预期e9f1ad989ea9，master直接e8e60cd19540、time a039d9803ade，lead取本126真实固定发布头；PR缓存base_sha不是master现场。已存在结果先核归属和完成情况，不重复提交，未知并行推进不覆盖。

先全文读取options_flags.cmake，再作配置/否定判断；独立核公约§1(2)整句、双分支金丝雀及读取通道。G00前只读/STOP_WRITING；门过即获本89限定写权，不再让Owner确认模型或开工。默认次一档；局部难点按124带证据交既定Pro回合，不擅自换模型或调用额外服务。

## 3. D00：R88-01追加纠正

独立回查88原evidence及facts锚表，至少纠正WQ88-E011与WQ88-E039：前者替换为work_func_t自身单行定义，必要别名单独取锚；后者以真实worker_pool_flags为符号，并引用其内部连续1–5行。不能只说字面在文件中出现便认定锚合格。

在89新正文/facts和必要清单消费说明中，给出旧锚→新锚→受影响事实、矩阵、模型、候选的映射，撤回原“103条全部位于所标单一定义内部”的总括。原88六件、103/103/0、旧回执和评论全部冻结；103是原始标签出现数，不能倒改成101或伪造过去的边界核验。89新六件连同D00的新增标签按真实全文单独计数，不把旧103作为89新证据。

补正只报CORRECTION_SUBMITTED_PENDING_C06_REVIEW，由C06下一次独立验收关闭；不重做88整包。R88-S01/S02/S03是主线已追加的消费限定，D00说明合读而不冒充作者早已完成。D00不是第四个新技术专题，完成后直接继续。

## 4. G01/A：池存储、早期初始化、身份与上线边界

由bh_worker_pools/cpu_worker_pools的实际定义、数组类型/步长和枚举宏接到init_worker_pool、init_cpu_worker_pool，再到两个workqueue_init入口。区分静态模板中的对象、经perCPU解码的实例和本轮假定合法的输入池；复用86/87限定，不重复模板复制/GDT分析，不从声明直接证明每CPU内存已初始化。

逐项核lock、id/cpu/node、flags、worklist/idle_list/workers、worker_ida、attrs、nr_running/nr_workers/manager的生产者。函数注释“newly zalloc'd”不是所有字段本地赋零的证据；区分显式写入、依赖先前清零、保持旧值和未证来源。计算每CPU普通/BH及优先级索引对应，不把possible与online宏名称当实际硬件集合。

追attrs分配失败和worker_pool_assign_id返回如何被BUG_ON消费；明确对象已部分修改、ID取得/写入/发布时机、正常返回与panic/不可证返回不同。不推定BUG只是日志、不因包装名认证创建成功。workqueue_init_early创建系统WQ处只合读87构造回执，不重审整个PWQ分配器。

交pool_storage_and_initialization_matrix、pool_id_attributes_and_failure_matrix、bootstrap_entry_and_online_matrix。模型覆盖有效清零/非零输入差别、零/一个/两个CPU的声明枚举、普通/BH标志组合、attrs失败、ID失败及两个初始化阶段的条件顺序；所有模拟均标明合法地址/前序返回，不宣称当前启动实际调用。

## 5. G02/B：worker资源、线程创建、挂接与清理

完整读alloc_worker、format_worker_id、create_worker、worker_attach_to_pool。逐阶段记录IDA编号、worker对象、scheduled/entry/node初值、flags、task、pool关联、池workers链、nr_workers、idle_list和唤醒请求；资源编号、对象分配、线程对象建立、挂接、可运行请求和实际进入worker函数是不同门。

普通与BH分支分别追踪：何处调用kthread_create_on_node、何处跳过；task=NULL/错误指针/有效对象不能混作一种失败。IS_ERR/PTR_ERR及所需类型须实际定位。格式化时的attrs/nice前提、set_user_nice返回和被注释的绑定操作保留边界，不扩大为真实CPU亲和性认证。

每个失败出口核到达前已取得哪些资源、ida_free/kfree实参及动作顺序、是否已经入链/计数、先前callee是否可能未返回。不可凭局部kfree就宣布所有线程资源均回收，也不可凭缺少全局销毁研究宣布实测泄漏。前序无效访问处停止该轨迹，后续只能作为独立条件分支。

交worker_resource_acquisition_matrix、worker_attachment_publication_matrix、worker_failure_and_cleanup_matrix。代表例覆盖ID拒绝、元数据NULL、线程错误指针、普通成功、BH无实际线程、挂接/idle/唤醒前缀，给资源与字段前后状态。不运行线程创建、调度或内存实验。

## 6. G03/C：manager状态、扩建重试及进展条件

完整读manage_workers和maybe_create_worker，以及need_to_create_worker到need_more_worker/may_start_working的实际定义。记录MANAGER_ACTIVE、manager指针何时设定/清除，已有manager时返回false的含义，以及simple_worker_thread怎样消费true/false；不是返回true就等于有可运行worker。

逐层画出外do/while、内while、create_worker返回和两次need_to_create_worker检查，锁/IRQ请求的释放与重取、已注释的timer/cooldown。对成功/失败、状态变化/无变化和callee不返回逐项论证是否有进展；不把无限资源假设当实际死循环，也不从理论资源耗尽证明一定安全退出。计数与链状态必须来自明确的输入和生产步骤。

连接A/B与88的待处理工作/idle/运行计数、普通/BH通知及回调门；不重做88提交/回调整段，也不审完整kthread/调度器。普通源码风险按原身份追加，真正新候选WQP89-Fxx与缺口WQP89-Uxx分账。

交manager_ownership_and_return_matrix、retry_lock_and_progress_matrix、bootstrap_to_worker_readiness_matrix及A/B/C接合表。模型覆盖已有manager、创建成功、持续失败且需求不变、需求在重试间消失、锁重取后需求再变、前序callee不返回；明确哪些仅是合法对象条件下的局部推导。

## 7. 质量、规模和六件

严格六件；facts含D00对应表、source_ledger、上述矩阵、条件模型、事实键→证据ID、prior_findings_links、新读/复用账。正文用中文给关键结论和反例，矩阵双格式保留；模型导航给真实场景简名，不再把全部场景写N/A，不逐字重复全部原JSON来凑长度。

源码软限24份（金丝雀另计）、8组定点检索、4次相关目录浏览；必要超限说明真实近邻依赖，不全仓漫游。含D00新短锚软限120，模型最多30，候选及缺口各最多12，均非数量配额。先核实结论再整理记录；少做重复文档不等于少做分析。

MANIFEST十四字段：task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。署名只据可见/Owner信息，不能认证后台。

协议v2 P1–P13全部适用。P9-1六件40hex零命中；2/13每个短锚在真实所标单一定义内部连续1–5行并支持论点，尤其检查typedef边界与枚举名；3真实路径及固定/活动回读；4辅助行号不越界；5按六件全部原始前缀计数，说明、字符串、D00均计；6外部语义用实际官方出处/版本，否则保留未证；7/8公约、canary和通道齐全；9/10/12无代码/补丁/降级则N/A；11未运行明确NOT_RUN。

字面匹配与定义身份/边界必须分开核验，不把“找到103行原文”自动当成“103锚正确”。核正文/矩阵/模型/deferred/DOT的前提、支持链和调用帧；单次未调用不是全局未发生。既有许可内可用独立临时纯文本检查或有限整数模型，不复制运行仓库程序，不冒称完整本地P9完成。

## 8. G04：发布与停写

只新增头部89目录六件及11本人一次正式PR17顶层评论。marker：MYOS2-LEAD-002-CHAT-EXEC-11 / CORE-USER-WORKQUEUE-POOL-BOOTSTRAP-89 / FORMAL_DELIVERY。旧成果/评论、PR正文/元数据/Draft/base、lead/PR16、master/time、内核和脚本冻结。

写前再核exec=e9f1ad989ea9、目录和marker，以真实完整旧HEAD作expected_sha，force=false，单父提交。暂态按63先核远端实效再间隔安全重试，遵Retry-After、通常首次加三次；成功对象/评论复用，未知并行推进不覆盖。工具发现不算实际写请求。

compare严格六件added、旧件零净改删；新固定六件全文、活动分支六件内容/blob、四direct refs、PR16/17、正式评论内容/编号/唯一性均远端回读。真实SHA和评论号由后置正式交付承接，不预造、不额外交第七件。恢复核查只补缺项，不重复已成功提交或评论。

完成报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、R88-01纠正待C06复核，随后STOP_WRITING，经PR17交回本C06。提醒Owner正式验收消息选择Pro，同轮审核纠正、新成果及后继；不自行签88闭合/89通过，不启动90。

禁止内核/loader/initramfs/Owner脚本修改和补丁，CMake配置、预处理/汇编/编译/链接、仓库程序/脚本/旧夹具/Graphviz/QEMU/实机、真实并发或硬件实验，本机凭据和外部libc镜像，额外模型/子代理/付费API，新分支PR、合并关闭强推删除，阶段3及跨项目维护。主线不并写results，旧10/09停用；110及124继续显式进入主线交接包、启动词及固定必读清单。
