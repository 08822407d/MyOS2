执行模型：次一档，沿用现有「MYOS2-A-C04c 代码分析执行」的GPT-6＋Extra。
不新建对话、不重做76号接收；本批不含49返工，不修内核。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SEMAPHORE-TIMEOUT-51
record_type: bounded_semaphore_timeout_and_grant_contract
evidence_class: 主线授权范围内的后继静态任务书
produced_by: ChatGPT主线规划
model_per_owner: gpt pro（沿用Owner报告）
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-06
base_snapshot: time
read_channel: GitHub_connector
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
executor_display_name: MYOS2-A-C04c 代码分析执行
executor_rotation: false
source_pin_short12: a039d9803ade
result_base_short12: b79feb4097e2
lead_before_publication_short12: cadbc98f18dc
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
required_review: CORE-USER-SCHEDULE-TIMEOUT-50-REVIEW-001
required_document_closure: CORE-USER-COMPLETION-WAIT-49-DOC-CLOSURE-REVIEW-001
mandatory_supplements: [R49C-S01, R50-S01, R50-S02]
required_write_guidance: 63-github-write-retry-and-readback.md
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-semaphore-timeout-51/
allowed_files: [MANIFEST.md, semaphore-timeout.md, facts-and-boundaries.yaml, semaphore-timeout-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: b79feb4097e2及此前全部文件和旧评论
activation_gate: Owner转发本固定78号且S00预检通过
phase: 2
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
inputs_read: [50交付与独立回执, 49文档闭合回执, CA-03/V09历史缺口, 固定semaphore.c/h规划预读]
owner_decisions_requested_now: []
open_questions: []
---

# 第51批：信号量等待如何消费超时，如何获得资源或退出

## 1. 增量目标与设计理由

50已核schedule_timeout各分支，但CA-03/V09还保留有限timeout实际消费者影响面的缺口。本批只打开内核信号量down_timeout的直接等待链与对应up授予链。
重点区分sem->count、等待队列、栈上waiter.up、任务状态、timeout和返回errno；这些不是completion.done的同一个对象或协议。
选择直接消费者取证，而非扩审timer wheel、完整调度器、锁/中断或所有调用者：在冻结范围内补足“返回值怎样改变后继控制”的证据。
本次不修CA-03，也不为同一有限timeout根因重复编号；新增非同义候选用SM51-Fxx，无数量配额。

## 2. S00：输入、现场与旧裁定

完整读本78号、同固定提交的50验收回执及49文档闭合回执、63号。R49-01已由C04文档闭合，49组合材料和50均为限定接收，不重做接收、闭合或已完成验收。
必须合读R49C-S01、R50-S01/S02；用到旧completion或公开信号状态时合读原R49/R48相关限定。
沿用本人76接收和49 MANIFEST的项目/指导加载，不将其它项目维护计划导入本任务。保持00工作令、公约、P1–P13、18/24/28及63号。
固定time a039d9803ade；核两项金丝雀、direct master/time/lead/执行refs、PR16/17、开放PR和路径交集。首次执行写前预期PR17 b79feb4097e2，本批新目录应不存在。
读50交付评论6682411980及本轮C04回执；未知后继、同名正式成果或竞争写入先核归属，不强推、自动变基或静默换pin。
定点读core/results.yaml::CA-03与V09及50的对应事实/模型；历史宿主夹具只读记录，禁止执行。只关联已有身份，不全仓重审。

## 3. S01：对象、入口及快路径

完整读取固定mykernel/lock_IPC/semaphore/semaphore.c中的down_timeout、__down_timeout，以及semaphore.h中的__down_common/___down_common活动定义；extern声明单列CONTEXT。
按实际包含链定位sema_t、semaphore_waiter、sema_init和初始化宏的真实类型/定义，分别列count、lock、wait_list和waiter.list/task/up；不套用Linux标准实现或swait的List_hdr_s计数模型。
逐语句列down_timeout的初始result、irqsave锁、count>0门、count递减或慢路调用、解锁和返回。
必须分别模型化count有资源/无资源及timeout负值、0、1、一般正有限、MAX；不能只按timeout数值跳过入口count快路径。
只为差异对照定点读down及down_interruptible的直接包装、所传state/timeout；不扩整个信号量API族或用户态信号量标准。

## 4. S02：慢路径、超时与信号返回

完整读___down_common，从栈上waiter、入队、task/up写入起，核for循环中signal_pending_state、timeout<=0、状态设置、解锁、schedule_timeout返回赋回、重锁、waiter.up和退出标签的真实顺序。
记录调用/返回、队列操作、count变化和int错误码分别由谁生产。核timeout与信号错误出口是否摘除同一waiter，成功路径的节点由哪一侧摘除；不由函数名或注释补出资源所有权保证。
将50实际返回域接入：有限正数在正常返回条件下是否能够自然递减到0；MAX是否走不同动作；初始0/负数是否在调用schedule_timeout之前已有真实门。
信号helper的state从long到uint及必要状态位按实际类型说明；对down_timeout所选TASK_UNINTERRUPTIBLE和其它helper参数模型分开，不把-EINTR分支无条件套到不可中断入口。
回源ETIME、EINTR单宏，片段只含一个定义；不猜错误码数值、不合并多个宏为一条证据。
至少覆盖初始signal门、零/负timeout、有限正数重复但无授予、MAX正常恢复、被调函数不正常返回、timeout仍正却waiter.up已经为真。最后一种必须独立判断，不能把“超时不递减”扩成“永远不能成功”。

## 5. S03：up授予、有限事件序与调用者边界

完整读up和__up：空队列时count操作，非空时选哪个waiter、摘队、写up、唤醒请求及解锁顺序。分清“授予标志已写”“发出wake请求”“waiter实际运行”和“down_timeout返回0”。
允许定点读直接list别名及小型add/delete/first辅助定义，确认用的是普通链节点还是带计数表头；不扩完整容器库、调度器或架构锁实现。
有限事件模型需明确同一锁/有效对象/正常返回前提；跨上下文用有条件事件关系，不把up画成waiter的同步callee。
涵盖先有count、先入队后up、等待方已经重新持锁、超时先摘队后up、没有up、至少两个waiter的局部顺序。不得由有限序列认证全局FIFO、公平、无丢唤醒或栈寿命安全。

为收窄CA-03影响面，允许对down_timeout在固定time的直接调用点作有界定位：默认分支搜索仅导航，候选逐个回固定pin；只展开足以证明实际实参与错误消费的直接调用者。
找不到调用者只能写“在已查路径/查询范围内未定位”，不能声称全仓不存在或真实运行必然可达。未做完整固定树索引时标检索覆盖限制；该缺口不阻止本批有界交付。
不运行仓库脚本作检索，不读取Owner本机或外部libc/镜像；仅现有工具的只读树/文本查询。

## 6. S04：六件、证据与验收条件

只新增allowed_files六件。MANIFEST分列本轮新读/继承、模型声明/后台未知、金丝雀/P6、固定输入、实际自查与write_retry_log。
正文/facts按输入门、资源计数、waiter身份、timeout值、信号门、队列与授予、返回传播和未知分层。原CA-03及DS48/CW49/ST50等状态不自行关闭。
每条VERIFIED为固定源一个真实函数/结构/对象/单宏/单typedef的连续1–5行；标题关键语句在片段内，记录definition_kind、enclosing_symbol、支持范围和消费者位置，不能用extern或首次同名命中替代函数体。
全文定义支持否定断言；分开字节匹配、定义身份、命题支持、自查计数、结构解析和图语义。没有做的机械项写NOT_RUN，不假称完整P9。
图须补齐成功、超时、信号及count快路的实际返回；共用节点按调用点匹配，注释不是活动指令。跨线程事件、源码门和分析有效性前提不得混用。
允许现有临时环境纯文本和有限状态模型；不安装工具，不执行仓库C/ASM/脚本、预处理/编译/QEMU/硬件，不新增付费API或隐藏模型委派。

## 7. 重试、发布与停止

完整执行63号：暂态异常先核远端是否已经写成，再通常首次加最多三次、约10/20/40秒或穿插安全只读工作重试；遵守Retry-After。
写动作暂缺也多轮正规发现，发现次数和实际写请求分开；预期404、本地编排/解析错误与真实GitHub暂态拒绝分别记录，不直接认定Owner没有权限。
复用已得blob/tree/commit，评论响应不明先查重；expected-head、force=false，未知并行写不覆盖，不绕过真实认证/策略/安全限制。
只向原PR17 user-semaphore-timeout-51/六件及本人新评论写入；C04不并行写执行结果。全部旧件/他人评论、lead/PR16/master/time、PR标题/body/Draft冻结，不新建分支或PR，不合并关闭/强推删分支。
写前重核b79feb4097e2；写后固定新提交及活动分支六件全文/blob/HEAD回读，compare只有六件A新增、旧件零净改删。

最终回复首先给可直接转发的纯文本一行：
“第51批已交付；PR17提交〔实际短提交〕；交付评论〔实际ID〕；READY_FOR_MAINLINE_REVIEW。”
随后用短段落说明STOP_WRITING、mainline_acceptance=PENDING、未编译运行和重要限制；不要用表格承载交付标识或统计，也不要重新展开接收日志。详表和加载记录留MANIFEST/评论。
未发布则首行报告准确阻断与已保全/待补，不填占位符冒称成功。
正式交付后STOP_WRITING交MYOS2-A-C04独立验收，不自行开始52。Owner转发且S00通过后同轮连续S00–S04，不为普通内部步骤另等确认；任务发布不是自动跨对话投递。
