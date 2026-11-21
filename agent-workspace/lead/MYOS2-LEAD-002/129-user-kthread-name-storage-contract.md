执行模型：次一档，沿用GPT-6＋Extra。执行位置：现有“MYOS2-A-C06c 代码分析执行”，身份MYOS2-LEAD-002-CHAT-EXEC-12。完成后交原C06主线，Owner发送正式验收消息前选择Pro，同轮核收与安排后继。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-KTHREAD-NAME-STORAGE-91
record_type: bounded_format_count_allocation_and_kthread_name_storage_contract
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（正式验收与后继设计）
date: 2026-11-16
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 034cd9c52dbf
lead_before_publication_short12: ffe6d8b314c8
fixed_contract_commit: OWNER_FORWARDED_129_FIXED_URL
executor_display_name: MYOS2-A-C06c 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-12
executor_rotation: false
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-kthread-name-storage-91/
allowed_files: [MANIFEST.md, kthread-name-storage.md, facts-and-boundaries.yaml, kthread-name-storage-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R90-01, R90-02]
D00: REQUIRED_APPEND_ONLY_DOCUMENTATION_CORRECTION
batch89_composite: PASS_PENDING_LOCAL_R89_01_AND_02_CLOSED
batch90_acceptance: RETURN_WITH_RETAINED_CORE_AND_R90_S01_S02_S03
activation_gate: OWNER_FORWARD_129_TO_EXISTING_C06C_AND_G00_PASS
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
inputs_read: [90六件及正式评论, 89闭合与90回执, 53回执及闭合与68回执, 固定kthread及kasprintf和vsnprintf相关定义，详见检查点]
owner_decisions_requested_now: []
open_questions: []
---

# 第91批：工作线程名称从格式化、分配到保存与读回

90已经独立审核，RETURN仅R90-01/02文档项；主体按R90-S01/S02/S03保留。89两项纠正已另件关闭，组合PASS_PENDING_LOCAL。91先D00再连续三个新专题，不重做IDA/IDR整包，也不重开89/88/86已闭合事项。继续现C06c/12，旧11/10/09停用。

## 1. 为什么选择本增量

89/90用到format_worker_id、栈上id_buf和kthread_create_on_node；53已经分析create请求与done的对象交接，68已经分析线程首入口与业务回调。因此本包不再重写那两份调用链，而补其未展开的名字字节生产、两遍格式化分配、full_name与task.comm存储/读取关系。名字构造成功、创建结果发布、线程运行、调度优先级生效必须分开。

三个专题：A实际格式化长度与容量边界；B动态名称分配及两遍输入/返回值；C名称指针转移、短名/全名读取与释放调用条件。主线只做预读划界，不预签91结论。

## 2. 固定输入、历史和G00

L=agent-workspace/lead/MYOS2-LEAD-002/，R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。工作材料取Owner转发本129链接中的真实固定发布提交，不用父ffe6d8b314c8代替新发布头。

完整读取本129、L/110-owner-communication-and-delivery-continuity.md、L/124-model-routing-and-pro-message-budget.md、L/reviews/CORE-USER-ID-ALLOCATION-LIFECYCLE-90-review.md、L/reviews/CORE-USER-WORKQUEUE-POOL-BOOTSTRAP-89-CORRECTION-CLOSURE-review.md、L/checkpoints/2026-11-16-id90-reviewed-name91-ready.md及90正式交付评论6896688417。90原六件固定034cd9c52dbf；若本C06c确已完整读取同一对象，可明确复用自身记录，不能继承C06或C06b读账。

历史边界必读：L/reviews/CORE-USER-COMPLETION-OWNERSHIP-53-review.md及CORE-USER-COMPLETION-OWNERSHIP-53-DOC-CLOSURE-review.md；L/reviews/CORE-USER-KTHREAD-ENTRY-68-review.md。53闭合明确撤回两项主线空行误判，不得仅摘旧RETURN重新传播；保留R53C-S01、R53-S01/S02与R68-S01/S02/S03。仅当本包某具体断言实际依赖旧原始事实时，按其固定指针定点读原件及对应qualified_id，不全量重做53/68，也不把主线摘要当源码。

重新全文读取63号、79号及协议v2全部P1–P13。九规则其余、128接收及适用Mnemosyne指导只能按12本有效会话自己实际已读、版本一致的范围明确复用；缺段补齐，逐项区分本轮新读/同会话复用/未读。不得倒造接收日志或导入Mnemosyne/Alaya维护路线。

核PR16/17、lead/exec/master/time四direct refs、开放PR、91目标目录和所有91正式marker。exec预期034cd9c52dbf，master e8e60cd19540，time a039d9803ade；lead为本129真实发布头。未知推进先核归属，不覆盖、不自动变基、不重复交付。PR缓存base_sha不是master现场。

先全文读time的mykernel/scripts/options_flags.cmake再作配置/否定判断，核公约整句、双分支金丝雀及真实通道。G00前只读/STOP_WRITING；G00通过即按本发令取得91限定写权，连续D00、G01–G04，不在正常接收或G00后再次等批准。

## 3. D00：90两个文档项的追加处置

R90-01：回读原MANIFEST，列出三个规范self_check键缺失及构造计数不能替代它们的原因；独立回查原六件逐件原始标签数量，记录旧59/59/0的声明域和当前新核范围。原90清单补充按本次真实核对给出字段含义、实际结果及不足，不倒造原发布时已完成检查；原件冻结，失效E013单列撤回/替代，不把旧标签说成已从冻结文件删除。91自身MANIFEST的verified_claims、quotes_reconfirmed、downgraded_to_inferred必须真实齐全，不能再次使用expected或构造器键替代。

R90-02：E013涉及的IDA_CHUNK_SIZE、IDA_BITMAP_LONGS、IDA_BITMAP_BITS分别取单宏定义内短锚，MYOS_IDR_BUF_SIZE另行支持。给旧E013→新锚→实际消费者的映射，涵盖源账、事实键、矩阵、模型、F04/U01等、正文和图。撤回旧全部59条定义边界合格的总括，保留原历史计数/原评论，不制造新的源码根因。

D00所有新增短锚计入91全包；原90历史计数、有效补充视图、新91原始计数分栏。R90-S01/S02/S03直接作为主线消费限定合读，顺带修正后继引用的次序、矩阵前缀与模型前提，不要求重画旧图或重做30模型。只报CORRECTION_SUBMITTED_PENDING_C06_REVIEW，不自签90闭合。

## 4. A：有界格式化返回值、容量与实际worker名字

固定源：mykernel/kactive/workqueue/workqueue.c的format_worker_id与create_worker相关完整定义，workqueue_const.h/types.h中WORKER_ID_LEN及实际字段类型；mykernel/lib/printf/vsprintf.c中vsnprintf、vscnprintf、scnprintf和实际用到的format_decode、string、number近邻定义。

只追本消费者的普通文本、%d、%s及其必要宽度/精度控制；不审全套%p扩展、浮点、binary printf或所有数值转换。读取全部vsnprintf主体及所需分支，不能仅靠其注释声称C99兼容；宏、类型及helper沿真实include定位，默认分支检索仅作定位，证据必须回到time固定源。

区分需求长度、已写字符、包含终止符的容量、截断后的终止位置及实际返回值。核size=0/1/恰好容纳/不足/超过局部允许值的路径、int与size_t比较和中间指针前提，不能用任意精度数学替代C对象边界。源码假设与语言/目标实现未证点分开，不把size=0不写目的区等同所有指针运算已获标准证明。

从pool->cpu、pool->id、worker->id、attrs->nice的真实类型和分支建立实际字符串场景；显式区分nice仅参与H后缀与真实调度效果。format_worker_id返回值在create_worker是否被消费、id_buf的自动存储期、后续%s传递的对象有效性逐一核实。

交format_count_capacity_matrix、worker_name_input_and_truncation_matrix；有效模型覆盖短名、边界名、截断、不同pool分支和前置对象缺口。场景须有具体输入及停止点，不凑字数或把假设故障写成真实复现。

## 5. B：kvasprintf两遍分配与输入稳定性

固定源：mykernel/lib/printf/kasprintf.c全文；mykernel/include/linux/lib/stdarg.h及实际va_list/va_copy/va_start/va_end定义来源；A的vsnprintf实现；kthread.c::__kthread_create_on_node与kthread_create_on_node完整相关定义。

追va_copy后的第一遍计长、first/second的实际类型、first+1表达式及分配实参、NULL分配返回、第二遍输出和最后返回。不能将first==second的注释检查当活动门；分别列格式串/字符串实参稳定与变化时的条件轨迹，非字符串值参数的复制与指针指向对象变化不可混为一件事。

检查第一遍/第二遍错误或截断返回值如何传入无符号变量、分配长度和调用者；只在可表示、对象有效、所调函数正常返回的前提内作有限推导。kmalloc内部不重审，保留供应器返回/对象范围门。kvasprintf_const的注释或空实现不自动加入本活动链；如发现本次实际调用才核其影响，历史第一波摘要只作导航。

交two_pass_format_allocation_matrix、varargs_and_input_stability_matrix、name_failure_result_matrix。区分NULL名字结果、ERR_PTR任务结果、create请求指针、真正线程task指针；不得由名字分配失败推断任务包装必返回NULL。

## 6. C：全名、短名及释放调用的对象关系

固定源：mykernel/sched/forkexec/kthread.c中kthd_create_info_s、kthread_s、to_kthread、get_kthread_comm、set/free_kthread_struct、kthread、create_kthread及创建包装；mykernel/sched/forkexec/exec.c的__get_task_comm与__set_task_comm；mykernel/lib/string/string_helpers.c::strscpy_pad及mykernel/lib/string/string.c::strscpy的实际活动实现。task类型中的comm长度、相应wrapper和锁定义按实际依赖定点补齐。

这些路径中kthread和kasprintf及vsnprintf已由主线预读；exec/string helper仅已定位，尚未由本主线认证语义，执行者须实际完整读相关定义。需要追到task.comm写者时，只在fork.c的kernel_thread/copy_process命名片段及对应包装取证，不扩大整个fork、exec或调度器。

分别画栈id_buf、堆full_name、请求create、私有kthread、task.comm和读回dest六种对象的生产者/借用者/最后访问/释放条件。以53已有交接为边界，重点核字符串指针是转移还是深拷贝、错误分支谁释放名字、请求释放是否等于名字释放、full_name缺失时fallback、读取中的截断/补零/返回值是否被上层保留。void的get_kthread_comm不能被写成返回strscpy_pad整数。

free_kthread_struct存在不等于实际exit一定调用；仅定位直接调用方/回收门，不重新审退出全链。to_kthread中的告警不自动构成拒绝门；task/private/full_name/dest各自有效性、锁和生命周期条件必须区分。名称改变/截断不等于内存越界，也不等于线程创建失败或调度成功。

交name_object_ownership_matrix、name_readback_and_padding_matrix、failure_and_release_prefix_matrix及A/B/C接合表。只向CO53/KT68/WQP89同根因追加；需要新问题时说明不同不变量，不能把旧交接结论换名当新发现。

## 7. 六件与发布前质量门

严格六件。正文通俗中文解释结论与关键反例；facts保存矩阵和具体模型、事实键到锚映射、源账、读账、D00处置；DOT表达有条件调用/源序/对象关系且有facts对照，不把比较关系当调用。候选KN91-Fxx、缺口KN91-Uxx分账；原qualified_id不改状态。

软限：源码24份（金丝雀另计），8组定点检索、4次相关目录浏览；必要超限说明具体依赖，不全树漫游。新短锚含D00最多120，模型最多24，候选和缺口各最多10，均为上限不是配额。三个专题按真实工作量推进，不以扩大文档替代取证。

MANIFEST十四字段：task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。self_check直接含verified_claims、quotes_reconfirmed、downgraded_to_inferred三个整数；需要的expected/constructor统计只能另列，不替代规范键。模型按Owner/可见来源，后台UNATTESTED，不反复索取确认。

适用协议v2全部P1–P13。发布前明确检查：P9-1六件40hex；2/13真实符号与定义内连续1–5行且语义支持；3路径/固定对象/事实引用真实存在；4辅助行号不越界；5六件全部原始前缀计数及三整数关系，包含说明/D00/字符串；6外部语义有实际官方出处/版本，否则保留未证；7/8逐字公约和双canary、通道；9/10/12无代码/补丁/降级为N/A；11所有未运行项明示。

在自检记录分开列“字面命中、单一定义归属、支持哪条断言、有限模型前提、规范字段、原始计数”。一个宏一锚，不能为压数量拼接相邻宏/typedef；保留原空行/缩进，不凭记忆补行。具备工具时用既有许可内独立临时纯文本检查辅助，不能执行仓库实现；未做机械检查如实NOT_RUN。已知错误必须发布前处理，不只报告发现错误后照常交付。

## 8. G04：唯一写者、发布回读和停止

仅12新增本91目录六件及本人一次PR17顶层正式评论。marker为MYOS2-LEAD-002-CHAT-EXEC-12 / CORE-USER-KTHREAD-NAME-STORAGE-91 / FORMAL_DELIVERY。旧成果/回执/评论、PR元数据/Draft/base、lead/PR16、master/time、内核/loader/Owner脚本冻结。C06不并写results，旧11/10/09停用。

写前再核exec=034cd9c52dbf、目标不存在或归属明确、marker不重复；使用完整实际HEAD作expected_sha，force=false、单父提交。GitHub暂态不因一次失败停工，先核远端效果，再按63间隔安全重试，通常首次加三次、遵Retry-After；复用成功blob/tree/commit/评论，未知并行推进不覆盖。工具发现与实际写请求分账。

compare严格六件added、旧件零净改删；新固定六件全文、活动六件内容或blob身份、四direct refs、PR16/17及正式评论全文/编号/唯一性全部回读。恢复只补缺项，不重复已成功提交或评论，不预造SHA或评论号。

完成报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、R90-01/02纠正待C06复核，然后STOP_WRITING。交原“MYOS2-A-C06 内核分析主线续建”，提醒Owner发正式验收消息前选择Pro，同轮复核纠正、新成果及后继。不自签90闭合/91通过，不启动92。

禁止内核/loader/initramfs/脚本修改、补丁、CMake配置、预处理/汇编/编译/链接、仓库程序/旧夹具/Graphviz/QEMU/实机/真实并发、外部libc镜像、本机凭据、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3或跨项目维护。110与124继续显式进入以后每次主线交接包、启动词和固定必读清单。
