执行模型：次一档，沿用GPT-6 Sol／GPT-6＋Extra。执行位置：现有“MYOS2-A-C06b 代码分析执行”，身份MYOS2-LEAD-002-CHAT-EXEC-11。交回本C06正式验收时选择Pro，同轮复核纠正、新成果及后继安排。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ID-ALLOCATION-LIFECYCLE-90
record_type: bounded_ida_range_idr_registry_and_consumer_contract
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner正式验收和设计安排）
date: 2026-11-15
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 42897a06feb1
lead_before_publication_short12: ed9cd00830d9
fixed_contract_commit: OWNER_FORWARDED_127_FIXED_URL
executor_display_name: MYOS2-A-C06b 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-11
executor_rotation: false
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-id-allocation-lifecycle-90/
allowed_files: [MANIFEST.md, id-allocation-lifecycle.md, facts-and-boundaries.yaml, id-allocation-lifecycle-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R89-01, R89-02]
D00: REQUIRED_TARGETED_EVIDENCE_AND_LOCAL_ANALYSIS_CORRECTION
batch88_composite: PASS_PENDING_LOCAL_R88_01_CLOSED_DOCUMENTATION_ONLY
batch89_acceptance: RETURN_WITH_RETAINED_CORE_AND_R89_S01_S02_S03
activation_gate: Owner向现C06b转发本127固定合同并要求执行且G00通过
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
inputs_read: [126及89六件全文和正式评论, 89独立审核与88闭合, 110/124及适用规则, 固定idr实现及类型常量, pid.c有界消费者, 11历史回执及固定延期原件]
owner_decisions_requested_now: []
open_questions: []
---

# 第90批：编号范围、对象登记和真实消费者的接口边界

89已审核，RETURN仅R89-01/02；主体按R89-S01/S02/S03限定保留。R88-01已由主线另件关闭，88组合PASS_PENDING_LOCAL。90先D00定点追加，再连续完成三个新专题；不重做89工作队列或11 PID整包。继续现C06b/11，不轮换，不向旧10/09索取确认。

## 1. 增量、真实入口与排除范围

A核IDA请求范围、容量单位、扫描/占用/释放的对应；B核IDR范围、base、NULL槽、循环游标和发布；C核查找/替换/移除/遍历与一个有界PID消费者的接合。89只查到若干最近包装和局部例，11已有NULL占号/查询风险；本90不将已有根因重新包装成新发现。

核心源码只取time@a039d9803ade：
- mykernel/lib/idr/idr.c：完整读取idr_alloc_u32、idr_alloc、idr_alloc_cyclic、idr_find、idr_replace、idr_remove、idr_for_each、idr_get_next_ul、idr_get_next、ida_alloc_range、ida_free及本包实际用到的近邻定义。区分活动实现和大段历史注释，未活动的树算法不当成本版本实现。
- mykernel/lib/idr/idr.h、idr_const.h、idr_types.h：完整读取相应初始化、cursor和IDA包装、数组/位图容量、字段类型及实现配置门。API/include缺口沿真实路径定位，不凭函数名补出上游接口。
- mykernel/kactive/workqueue/workqueue.c：只取worker_pool_assign_id、create_worker中IDA申请/归还实参及相关错误消费；89已有pool/worker/manager主体直接合读回执，不重新审整段。
- mykernel/sched/task/pid.c：完整读取alloc_pid、find_pid_ns、free_pid中编号申请/NULL预留、idr_replace发布、查找及失败回退/移除的相关函数。只分析IDR消费者接口，不扩大为copy_process、task引用、kill、exit、namespace创建/释放全审计。必要类型、容量和常量沿真实include补齐。
- D00近邻固定源：mykernel/include/linux/kernel/cpumask.h、mykernel/sched/runqueue/runqueue.c::set_user_nice、workqueue.c::create_worker、mykernel/include/linux/kernel/err.h。必须实际读取完整相关定义，不能因主线已读就继承资格。

主线已预读上述idr四件、pid.c相关函数、workqueue相关定义和set_user_nice实际实现。pid和IDR的NULL预留与查询上界在11已有记录，不证明本90已经执行。禁止全树漫游、上游替换、完整slab/调度/kthread/namespace重审、修复、解除注释或运行测试。

## 2. G00：读取、历史和现场

L=agent-workspace/lead/MYOS2-LEAD-002/，R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。工作材料取Owner转发本127 URL的真实新发布提交，不能用父ed9cd00830d9替代；原89六件固定42897a06feb1，正式评论6893704757。

完整读取本127、L/110-owner-communication-and-delivery-continuity.md、L/124-model-routing-and-pro-message-budget.md、L/reviews/CORE-USER-WORKQUEUE-POOL-BOOTSTRAP-89-review.md、L/reviews/CORE-USER-WORKQUEUE-WORK-LIFECYCLE-88-DOC-CLOSURE-review.md、L/checkpoints/2026-11-15-workqueue89-reviewed-id90-ready.md及89正式评论。合读R89-01/02、R89-S01/S02/S03、R88-S01/S02/S03及实际依赖的R87/R86与更早限定。

历史去重必读：L/reviews/CORE-USER-PID-LIFETIME-11-review.md；MyOS2@2c410132e42d的R/user-pid-lifetime-11/MANIFEST.md、deferred-findings.yaml全文，以及facts-and-boundaries.yaml中PID11-A13/A66/A67/A70/A72/A73/A74对应锚与必要上下文。保持CORE-USER-PID-LIFETIME-11::PID11-F01/F02身份；旧回执含历史文档退回不等于允许重开该项。需要其它旧结论时只定点追其正式限定，不把整份旧11作为未经核验的源码事实。

重新全文读取63、79和协议v2全部P1–P13。其余九规则、119接收与适用Mnemosyne指导、旧固定成果，仅可按本有效C06b自己实际已读且对象一致范围明确复用；缺段补齐，区分新读/同会话复用/未读。不继承主线或旧执行者读账，不倒造历史加载，不导入其它项目任务。

核PR16/17、lead/exec/master/time四direct refs、开放PR、90目录和本人marker。exec预期42897a06feb1，master直接e8e60cd19540、time a039d9803ade；lead以本127真实固定发布为准，PR缓存base_sha不是master现场。已有结果先核归属，不重复提交；未知并行推进不覆盖。

先全文读options_flags.cmake再作配置或否定判断，独立核公约§1(2)整句、双分支金丝雀与通道。G00前只读/STOP_WRITING；门通过即取得90限定写权，连续D00、G01–G04，不逐专题等批准，不再要求Owner确认模型或开工。

## 3. D00：89两项定点纠正

R89-01：回查WQP89-E010，分别建立for_each_possible_cpu、for_each_online_cpu单宏锚及必要for_each_cpu下级锚。逐项映射旧锚表、枚举矩阵和模型；撤回全部120锚均满足所标定义边界的总括。原120/120/0和字面匹配历史冻结，新90标签含D00按全文另计。

R89-02：实际读取set_user_nice完整定义及create_worker相邻控制流。当前函数只有未使用局部声明、调度/任务操作被注释，不能把NULL任务在此当作已定位的解引用失败点；调用实参pool->attrs->nice仍须合法。修正M15及同义矩阵、F05、U08和图/正文消费说明：在明确假设NULL返回、有效对象及其余callee正常返回的隔离轨迹，可继续到挂接/计数/idle，并因task为空跳过wake；这不证明真实kthread可返回NULL或线程已启动。

两项都需旧位置→新增锚/说明→受影响消费者映射，不只贴本主线意见。F05保留旧ID和NULL来源未证边界，不另造同根因，不扩大成真实运行故障。R89-S01/S02/S03直接作为主线消费限定合读，不冒称作者原本已完成；ID失败直接出口与后续清理、WARN源序也不能沿用错误图示。

只在90新六件追加，不改89/88原件、旧回执或评论，不倒造历史检查。补正只报CORRECTION_SUBMITTED_PENDING_C06_REVIEW，由本C06独立关闭；D00不是第四个新技术专题，完成后直接继续。

## 4. G01/A：IDA范围、容量单位和占用状态

从ida_init及ida_alloc/min/max/range的真实包装入手，分别登记参数类型、编号上下限、word下标、word位数、sizeof单位、返回表达式、位图长度。核min/max的实际消费和循环边界、slot跳过条件、bit扫描及移位操作，不凭接口注释补齐inclusive语义或上游算法。

区分读到空位、返回编号、登记占用和归还后的位图变化。89的F01/F02/F03保留原身份，只追加输入边界证据；需要不同根因时给出差异依据。算例可声明LP64等模型，但不能声称实测类型尺寸；超出可表示范围、无有效bitmap或非法索引时停止正常推导。

交ida_request_and_units_matrix、ida_scan_and_occupancy_matrix、ida_release_and_failure_matrix。代表例覆盖min=0/非零、max小于/等于候选、不同空位位置、边界word、全占位图、重复请求、不同归还编号和初始化失败。例数按有效区分选择，不凑上限，不运行仓库分配器。

## 5. G02/B：IDR存储、范围、NULL槽与循环游标

从idr_init_base的分配尺寸、base/cursor/initiated写入，追idr_alloc_u32及普通/循环包装的参数变换、扫描上限和成功/失败返回。分别核start/end/max、base是否实际参与索引、游标读取/更新、回绕重试条件、有符号与无符号转换；采用明确类型宽度与可表示条件，不任意精度外推。

记录空槽测试、ptr写入、nextid写回及idr_next更新的顺序，区分NULL值与已占用标记。对NULL预留只向PID11-F01追加支持/限定；不能用cursor推进自动证明排他，也不能忽略cursor就断言每次立即重号。

交idr_storage_and_range_matrix、idr_slot_publication_matrix、idr_cursor_and_retry_matrix。覆盖非零base、相邻/反向边界、start靠近容量、范围内满而范围外空、cursor在区间内外、一次重试成功/失败及NULL/非NULL payload；每例明确最早非法访问/转换或callee不返回的停止点。只核局部实现，不证明全局并发分配安全。

## 6. G03/C：映射接口、遍历和有界消费者

完整核idr_find/replace/remove对编号、旧值/新值和范围的实际处理；核idr_for_each、idr_get_next_ul/get_next的活动循环、callback调用、输出参数及返回值，注释算法不算实现。不把“返回0”直接当已遍历全部对象，不把NULL返回当可证明所有非法输入都安全。

将上述表与两个实际消费角色接合：worker_pool_assign_id存pool指针并消费整数状态；alloc_pid使用NULL申请、后置idr_replace发布pid对象、find_pid_ns查询及free_pid/失败路径调用移除。只跟这组接口的参数、发布时点和资源前缀，不研究完整任务生命周期。PID11-F01/F02原身份与回执限制必须同行；当前新证据是接口差异/范围/游标/遍历组合，不重新登记老问题。

交idr_lookup_replace_remove_matrix、idr_iteration_and_output_matrix、consumer_reservation_publication_matrix及A/B/C接合表。模型覆盖不存在/有效映射、替换/移除旧值、重复移除、合法容量边缘与越界前止证、空/非空迭代、回调是否被调用、数字取得到对象发布之间的条件状态。竞态只在明确交错假设下推导，不实际运行线程/硬件，不把锁名当全局证明。

## 7. 六件、质量与规模

严格六件；facts含D00映射、source_ledger、矩阵/模型、事实键到证据ID、历史去重及新读/复用账。正文中文给关键结论与反例，矩阵双格式；模型导航有真实场景名，不整段重复JSON凑篇幅。新候选IDA90-Fxx、缺口IDA90-Uxx分账；与WQP89/PID11同根因则追加原qualified_id，不改旧状态。

源码软限24份（金丝雀另计）、8组定点检索、4次相关目录浏览；必要超限说明具体依赖，不全仓漫游。含D00新短锚软限120、模型最多30、候选及缺口各最多12，均为上限不是配额。不能为压锚数量把两个独立宏/typedef拼成一条。

MANIFEST十四字段：task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。模型按可见/Owner信息记账，不认证后台。

协议v2 P1–P13全部适用。P9-1六件40hex零命中；2/13每锚真实符号、定义内部连续1–5行且支持断言；3路径和固定/活动回读；4辅助行号不越界；5六件全部原始前缀包含D00/说明/字符串，三整数一致；6外部语义需实际官方出处/版本，否则保留未证；7/8公约、双canary及通道；9/10/12无代码/补丁/降级N/A；11未运行明确NOT_RUN。

发布前分开核字面、定义边界、类型/调用帧和语义，不只核ID存在。对NULL参数结论须先读最近callee，不能凭名字宣布解引用失败；依赖未知时如实记未知，不把尚未查明冒充已经定位的非法访问。既有许可内可做独立临时纯文本检查或有限整数模型，禁止复制运行仓库程序；未做不得自报机械PASS。

## 8. G04：发布、回读和停写

仅新增头部90目录六件及11本人一次PR17顶层正式评论。marker：MYOS2-LEAD-002-CHAT-EXEC-11 / CORE-USER-ID-ALLOCATION-LIFECYCLE-90 / FORMAL_DELIVERY。旧成果/评论、PR元数据/Draft/base、lead/PR16、master/time、内核和脚本冻结。

写前再次核exec=42897a06feb1、目标目录和marker；真实完整旧HEAD为expected_sha，force=false、单父提交。暂态按63先核远端效果再间隔安全重试，通常首次加三次，遵Retry-After；复用成功对象/评论，未知并行变化不覆盖。工具发现次数与实际写请求分账。

compare严格六件added、旧件零净改删；新固定六件全文、活动六件内容/blob、四direct refs、PR16/17、正式评论内容/编号/唯一性均回读。真实SHA和评论号由后置交付承接，不预造、不加第七件；恢复只补缺项，不重复已成功提交或评论。

完成报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、R89-01/02纠正待C06复核，随后STOP_WRITING。经PR17交回本C06，提醒Owner发正式验收消息前选择Pro，同轮复核纠正、新成果及后继；不自签89闭合或90通过，不启动91。

禁止内核/loader/initramfs/Owner脚本修改、补丁、CMake配置、预处理/汇编/编译/链接、仓库程序/脚本/旧夹具/Graphviz/QEMU/实机、真实并发/硬件实验、本机凭据/外部libc镜像、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3或跨项目维护。C06不并写results，旧10/09停用；110及124继续显式进入每次主线交接包、启动词和固定必读清单。
