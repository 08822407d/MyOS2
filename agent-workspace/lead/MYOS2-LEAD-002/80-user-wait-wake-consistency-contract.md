执行模型：次一档，沿用现有「MYOS2-A-C04c 代码分析执行」、GPT-6＋Extra。
一个组合工作包，包内三个相关技术专题连续完成后一次交回C04；不新建对话、分支或PR，不逐专题等待Pro再派发。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-WAIT-WAKE-CONSISTENCY-52
record_type: bounded_combined_wait_queue_timer_and_caller_contract
evidence_class: 主线阶段2授权内的组合静态任务
produced_by: ChatGPT主线规划
model_per_owner: gpt pro（Owner本轮确认）
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-07
base_snapshot: time
read_channel: GitHub_connector
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
executor_display_name: MYOS2-A-C04c 代码分析执行
mainline_owner: MYOS2-A-C04
source_pin_short12: a039d9803ade
result_base_short12: 1e128a33a014
lead_before_publication_short12: 72ef20aeff03
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
required_review: CORE-USER-SEMAPHORE-TIMEOUT-51-REVIEW-001
mandatory_supplements: [R51-S01, R51-S02, R50-S01, R50-S02, R49C-S01]
planning_record: 79-work-package-sizing-and-pro-review.md
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-wait-wake-consistency-52/
allowed_files: [MANIFEST.md, wait-wake-consistency.md, facts-and-boundaries.yaml, wait-wake-paths.dot, deferred-findings.yaml, evidence.md]
executor_rotation: false
activation_gate: Owner转发固定80号且G00预检通过
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
open_questions: []
---

# 第52批：等待链局部一致性组合包

## 1. 工作量与问题边界

不再把一个短接口逐个发成独立批次。本包集中回答：A两类等待队列的结构和摘除不变量；B有限timeout的timer登记/取消如何作用于同一对象；C真实直接调用与49–51接口衔接的证据缺口。三专题共用固定源和一次交付，分别标结果，不用同一结论重复凑三份工作。
这是阶段2局部调用契约核查，不是全项目阶段3交叉综合，更不是完整调度器、timer wheel或内存序审计。已确认源码不靠重复报告增加工作量，历史同根因不重新编号。
核心主体范围预期覆盖swait/普通list局部辅助体与timer登记/删除函数群，约相当于此前三个小包；不以引文、模型、文件行数或缺陷数量作为完成配额。

## 2. G00：一次共同预检和输入台账

完整读取本80号、79号、同提交51回执R51-S01/S02、63号。按引用消费同提交50回执R50-S01/S02及49闭合R49C-S01、48限定；不重做历史验收。
沿用同一C04c的76接收和49 MANIFEST项目/指导加载。仍遵守00工作令、公约、P1–P13、18/24/28；禁止导入其它项目维护或自动付费模型。
固定time a039d9803ade；核金丝雀、实际refs与PR16/17、开放PR和写区。首次执行写前期望1e128a33a014；52新目录应不存在，未知后继/并行写需核归属。主线正常前进不等于执行冲突，不静默改变固定合同/源码。
建立一次source ledger：文件路径、固定ref、blob身份、实际读取范围和定义符号。后续同包复用明确标注的已读源，不每个小阶段重新加载整套规则或重复远端抓相同内容。关键写前/写后检查仍执行。
定点读core/results.yaml的V09–V11和对应ca_rulings，保留历史宿主模型身份；未完整读的历史范围不得声称已读，禁止运行旧夹具。

## 3. G01／专题A：两种等待队列及容器一致性

完整回核固定mykernel/kactive/swait/中__prepare_to_swait、__finish_swait、swake_up_locked及其结构/初始化；只为必要对照消费51的普通semaphore wait_list及up/失败摘队。completion调用关系用49闭合后的正确证据。
定点进入mykernel/lib/list/double_list.h与double_list_macro.h：List_s/List_hdr_s、count/anchor、list_header_is_empty、list_header_add_to_tail、list_header_delete_node、list_del_init、list_is_empty_entry及取首项宏，沿实际定义补必要小助手。
分别列“链环实际节点”“表头count”“节点自指/毒化”“持有同一锁”的不变量，区分普通List_s与带计数头。不要仅凭函数名认定count和链同时更新。
至少覆盖空队列、一等待者、唤醒后finish、第二次通知/重用、两个等待者、未被唤醒而错误摘队的有界序列。停止于最早无合法对象/不变量可证处，不对anchor容器继续编造实际CPU效果。
历史V10/V11已有同类观察时，记录它已证明什么以及本次新增的接口消费关系；不重跑宿主夹具、不用一个旧根因新立多个WW52缺陷。无需扩大整个链表库或证明SMP公平/无丢唤醒。

## 4. G02／专题B：有限timeout的timer登记与同步取消

从50已读的schedule_timeout有限分支开始，独立核simple_init_timer_key、__mod_timer、timer_delete_sync→__timer_delete_sync→__try_to_del_timer_sync，并定点读取timer_pending、detach_if_pending和其直接摘链助手的真实定义。
列局部proc_timer_s中的timer/task身份，entry.pprev/function/flags/expires的实际写入点；未写字段不能凭初始化函数名补齐。__mod_timer中MOD_TIMER_NOTPENDING、pending/function门、ret、base选择/迁移以及登记返回分开。
允许读取lock_timer_base/get_timer相关包装及必要标志定义来解释条件，但更深架构锁、时钟推进、轮盘索引算法、全CPU迁移正确性保持opaque。internal_add_timer/enqueue_timer可作为明确写入边界；仅在必须证明本题对象状态时定点展开，不全面展开timer wheel。
取消侧核running_timer比较、ret<0重试、shutdown=false/true对function的影响，以及调用者实际使用false、丢弃int结果的事实。不得把注释“同步”直接升格为回调永远不会再发生或对象可安全离栈。
有限模型涵盖非pending、pending、running_timer相同/不同、function空、一次/多次负返回、未正常返回、同对象被再次登记。真实时钟到期派发和并发重用来源未审就保留条件；不是实机失效注入。
A与B输入独立，A出现候选问题不自动阻止B取证，反之亦然；跨题依赖未经验证不可当真值。

## 5. G03／专题C：直接调用点与局部接口衔接

对down_timeout、msleep、wait_for_completion/complete、schedule_timeout在固定time作有界直接调用定位。可用GitHub固定tree和读回文本；默认分支搜索只用于导航，每个命中必须回固定pin。未获完整固定树覆盖只能报告已查范围内结果，不声称全仓无调用者。
只展开足以确认实际参数、调用返回消费及对象身份的直接调用者。优先衔接48的setup_done与51信号量，不扩任意VFS、全启动或整个调度器。
形成按实际入口区分的局部契约矩阵：谁生产计数/授予、谁入队和摘队、实际timeout是否有限、哪里请求调度、哪里读取返回值、哪些错误会传播、哪些证据仍未知。
输出“既有结论可直接复用／本次新证／矛盾或需降级／真实运行仍未证”四类，不将缺少新缺陷理解为任务无效，也不以小片段证明整体内核正确。
C依赖A/B的部分若被真实证据冲突阻断，明确标blocked-by并完成独立部分；不为普通内部完成逐段向Owner申请Pro批准，不隐式更换研究方向。

## 6. G04：一次六件交付、机械与语义检查分离

六件固定不变，各按A/B/C分区；MANIFEST只写一次预检/约束、固定源台账、本批新增/继承范围、真实检查与重试。正文集中解释新增结果；facts不逐字复制整篇正文；evidence供事实与图共同引用。
每条VERIFIED仍须固定源一个真实定义/单宏/单typedef中连续1–5行，标题命题落在引文内。记录实际定义范围、标签/消费者，extern与条件编译单列CONTEXT；否定断言必须读完整有关体。
可用现有临时工具作纯文本/JSON/图端点/引文匹配及有限状态模型，不安装工具，不运行仓库C/ASM/脚本、不预处理/编译/链接/QEMU/硬件。源码文字命中不能替代函数归属和语义支持检查。
不为凑产量重复验证同一个timeout恒等式几十遍；针对新的反例/边界设计必要模型。自查申报与主线验收分开，未做的项准确NOT_RUN，最高PASS_PENDING_LOCAL。
每个专题均给completed_in_scope／blocked及证据，缺口不是私自删掉任务；总体有阻断如实申请部分验收。全部完成后一次交C04，不自行签PASS或开始53。

## 7. 写入、重试和用户交付

唯一写者C04c/CHAT-EXEC-06，仅原PR17 user-wait-wake-consistency-52/六件及本人评论；旧件/旧评论、lead/PR16/master/time和PR元数据冻结。C04不并行写执行成果。不建分支/PR、不合并关闭/强推删分支、不修改内核。
完整执行63号：暂态异常先确认远端是否已写成，再通常首次加最多三次、约10/20/40秒或安全只读间隔重试；遵守Retry-After。工具暂缺也多轮正规发现，不把工具缺席或预期404解释为无权限。
复用已得对象，未知ref变动不覆盖，expected-head及force=false；评论响应不明先查重，不绕过真实安全/认证限制。实际重试与工具发现分记，不虚构尝试。
优先六件一次提交；若需要保全中间进度，只在已授权六件中新增已完成文件并声明未完整交付，后续只能补未存在文件、不得覆盖冻结原件。最终净差异必须只有规定六件A新增；无法维持该约束时停相应写入、保全并说明，而非擅加第七件。
写前再核预期执行头及自己已确认的后继，写后固定提交与活动分支六件全文/blob/净差异回读，交付评论回读后STOP_WRITING。
最终回复首行：“第52批组合包已交付；PR17提交〔真实短提交〕；交付评论〔真实ID〕；A/B/C均完成；READY_FOR_MAINLINE_REVIEW。”若有未完成项，必须替换为真实部分/阻断状态，不能照抄完成模板。
其余短段落说明边界，不用长表格或接收日志要求Owner搬运。Owner转发一次启动；包内G00–G04连续执行，只有真实冲突/授权/安全阻断才停相应动作。
