执行模型：次一档，沿用Owner指定GPT-6＋Extra；继续现有「MYOS2-A-C05b 代码分析执行」/CHAT-EXEC-08，不新开对话。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-KTHREAD-ENTRY-68
record_type: bounded_task_entry_producer_and_return_contract
evidence_class: Owner持续授权下的规划；非执行或运行结果
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；非后台认证）
model_per_owner: gpt pro（主线Owner此前选择）
backend_identity: unknown_or_not_attestable
date: 2026-11-10
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 157bf5604754
lead_before_publication_short12: d1a8cee95e42
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
executor_display_name: MYOS2-A-C05b 代码分析执行
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-kthread-entry-68/
allowed_files: [MANIFEST.md, kthread-entry.md, facts-and-boundaries.yaml, kthread-entry-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R67-01]
prior66_status: PASS_PENDING_LOCAL_WITH_R66C_S01
mandatory_supplements: [R66C-S01, R67-S01, R67-S02, R66-S01, R66-S02, R65C-S01]
activation_gate: Owner向现有C05b转发本固定任务并要求执行且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
inputs_read: [67完整交付及主线回执, 固定kernel_clone/kernel_thread/kthread/启动调用者源码, 28/63/79及协议v2]
open_questions: []
---

# 第68批：真实任务入口、包装层与返回条件

## 1. 技术增量与边界

67建立了首帧与两个fn调用点的条件关系，但未回答真实调用者给首帧放入哪个函数、该函数是否会正常返回。68合并A参数生产与首帧入口、B通用kthread包装层、C直接启动任务与终止边界三个专题，D00只纠正67计数记录；共用六件与一次主线验收。
不重做整个67、66栈/TSS链、03调度队列、49/53完成量所有权、全部fork/exit/信号、47根目录切换或ELF加载；不修内核、不构建运行。技术问题按18号延期，普通新发现不要求Owner逐项决策。Mnemosyne只提供适用行为指导，不导入其它仓库维护。

## 2. G00与固定输入

完整读本99号、同发布提交reviews/CORE-USER-ENTRY-STACK-BINDING-66-DOC-CLOSURE-review.md、reviews/CORE-USER-FIRST-RUN-FRAME-67-review.md、checkpoints/2026-11-10-first-run67-reviewed-kthread68-ready.md；全文读63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md及协议v2全部P1–P13。其余九份项目规则和Mnemosyne指导只可复用本有效C05b实际已读原文，缺项则补读，不继承其它会话已读声明。
R66-01已CLOSED_DOCUMENTATION_ONLY，66组合材料PASS_PENDING_LOCAL；67为RETURN_DOCUMENTATION/R67-01，但技术主干保留。65/64文档闭合、63/58误判撤回保持；不重新打开旧空行争议。
核PR16/17、direct time/master/lead/exec、开放PR和新68目录。首次执行写前预期157bf5604754，新目录不存在，time固定a039d9803ade；未知exec前进停相关写入，不覆盖。主线正常发布99不构成执行分支冲突。
重新取得公约§1第2条和time/master双金丝雀原样值，填写MANIFEST。原67从157bf5604754读取D00相关位置、FR67-F01/F02/F03及U03/U04/U05/U06/U07和引用前提；原SB66及09按对应固定回执消费。采用任何旧问题ID前须核原条目与状态，不根据第一波摘要重编号。

## 3. D00：R67-01计数一致性，不重做82条

定点读取67 facts.P9_text_subchecks键5及MANIFEST三数，确认说明文本额外包含P6计数前缀。按原协议规则核实：源码标签数、非标签前缀命中数、六文件原始总命中数三者分别是什么；完整计数未运行时仅报告可证下界，禁止把估计说成程序输出。主线已证82个源码标签加至少一个说明命中，不要求你编造第83条源码锚。
在新facts.prior_return_disposition和正文登记原位置、实际计数方法、影响与限制；旧67 MANIFEST/事实件/评论6757793690中的“82等于全文计数、P6已过”须追加纠正。旧文件与旧评论一个字节不改，不把说明文字作降级断言凑数，不回溯声称旧82个引文全部失效或全部重新通过。
新68所有非证据说明避免写出匹配P6的保留前缀；只在真实源码标签中使用。对新六件全文进行允许的本次自建文本计数，另对真实锚清单核对，保证MANIFEST的三数与真实文件一致；纯文本检查不等于完整本地P9。原计数声明的纠正不需要重取整个67源链或新增第七件。
D00只提交documentation_disposition_submitted，关闭权在C05；R66闭合不受本次纠正影响。若实际远端文字与主线描述不同，应提交可核异议，不迎合错误要求。

## 4. G01／A：kernel_thread到首帧fn的参数链

固定核心源mykernel/sched/forkexec/fork.c：完整kernel_thread与kernel_clone；copy_process仅读args.kthread/PF_KTHREAD及copy_thread必要控制窗口，按67原限定消费。完整相关参数类型mykernel/sched/misc/sched_misc_types.h::kernel_clone_args，必要宏通过真实include定位，不猜Linux路径。
建立输入fn/arg/name/flags到kclone_args.fn/fn_arg/kthread/stack/stack_size/exit_signal的来源表，再接到copy_thread的真实分支。区分存入stack字段的数值与被PF_KTHREAD分支实际消费的用户栈；不因字段名叫stack就认定函数地址成为真实用户栈。
记录可见NULL检查、错误返回及wake_up_new_task前提；仅能说所读函数范围内是否检查，不能推出所有调用者都允许NULL。kernel_clone返回PID不证明子任务已执行到fn；资源复制、PID及调度机制保持opaque，不重做其历史问题。

## 5. G02／B：通用kthread包装层与业务threadfn

固定mykernel/sched/forkexec/kthread.c完整相关定义：kthread_create_info与kthread类型、set_kthread_struct/to_kthread、__kthread_create_on_node、kthread_create_on_node、create_kthread、kthread、kthread_exit；kthreadd的取件到create_kthread窗口。可全文读取该短文件，但只审本范围，不借机重做completion队列/取消所有权。
分别给出请求中的threadfn/data、kernel_thread接收的入口fn/arg、首帧bx/r12、包装器self->threadfn/data以及最终业务回调的身份。必须核实首帧究竟调用kthread包装器还是直接调用业务threadfn，禁止将两层函数都命名为同一个fn后直接宣称重复业务执行。
对done为空、建立完成通知、等待首次唤醒、SHOULD_STOP、业务函数不返回/正常返回、kthread_exit路径分别列最早停止点。complete通知、create->result存在与业务回调开始不是同一时刻；schedule_preempt_disabled正常恢复是前提，不展开完整调度器。
仅读实际kthread_exit→do_exit的最近定义与终止接口；noreturn声明不单独作为完整实现不返回证明。按正规搜索定位do_exit及必要do_task_dead窗口，最多沿一层直接终止边，资源释放主体保持opaque；未闭合就记条件缺口，不执行仓库脚本或运行内核。

## 6. G03／C：直接启动入口与返回界线

固定mykernel/init/main.c完整rest_init、kernel_init；fixed kthread.c完整kthreadd。列出两类直接kernel_thread调用与通用create_kthread路径的区别，明确源码给出的非空符号，不能把这几个调用者的证据外推成全仓NULL禁令。
针对kernel_init中的前置初始化/根目录切换、kjmp_to_doexecve调用及后续循环，按函数正常返回、显式控制转移和本地终止分栏。kjmp_to_doexecve只允许正规定位其最近直接定义/跳转窗口；不深入ELF/MM/整个用户态启动。kthreadd的持续循环、kthread的业务返回后退出路径与67的正常返回前提分别比较。
输出return_condition_matrix：首帧入口、其内部业务回调、是否在所读局部出现正常return、到达67后一次call尚缺的条件。不能把存在return语句当可达，也不能把while/注释/声明当实机终止证明。即使真实所选入口不满足67重复调用前提，也只是收窄FR67-F01的适用范围，不自行关闭旧ID或宣称全内核不存在重复路径。
保持活跃RBX/R12与pt_regs保存字段分离；CPU/SYSRET/GS和首次CPL3运行仍未认证。

## 7. 六件、预算与证据门

严格六件：MANIFEST.md、kthread-entry.md、facts-and-boundaries.yaml、kthread-entry-paths.dot、deferred-findings.yaml、evidence.md。facts至少含prior_return_disposition、source_ledger、entry_argument_matrix、wrapper_callback_matrix、return_condition_matrix、finite_model_cases、prior_findings_links。新项使用KT68-Fxx/KT68-Uxx，无数量配额；属于FR67已有根因则保留其身份，补证不重复造bug。
核心为fork.c两个构造/调用函数、kthread.c限定定义和main.c两个入口，必要直接依赖软上限12文件，D00/规则/历史另计；最多8组定点查询、4个相关固定目录，超过须解释必要性。可选外围找不到则保存范围明确的缺口，不猜实现。
强制短锚：A真实参数初始化与调用/失败门；B包装器入口身份、threadfn读取/调用及退出；C实际启动调用者、正常返回/循环/跳转边界。D00证据是原交付字段和计数记录，不强行冒作内核源码锚。每条源码引文原样连续1–5物理行，非空、单一定义、支持相应命题；不复演空行误判。不得用外部无出处硬件常识补成已核事实。
本任务适用协议v2全部P1–P13。P6按六件原始文本计数，并核全部引用ID存在及命题支持，不能只数标题或过滤掉不方便的说明命中。MANIFEST含read_channel、startup_selfcheck_quote、branch_canary_quotes、署名与后台未知、self_check三数、P9逐项实况、write_retry_log。P9-1至8及13按实际检查给结果；9/10/12无代码/补丁/降级时N/A；11不得伪称运行；完整P9仍NOT_RUN。自建纯数学/文本检查可做，仓库代码/旧夹具不可执行。
图分MACRO_EXPANSION、CALL、TAIL_JUMP、RETURN、DATA、LOCAL_CONTROL及ANALYSIS_STOP；包装层与业务层用独立节点，顺序边不伪装成直接调用。

## 8. G04与停止

唯一新增区为原PR17的user-kthread-entry-68/授权六件及本人唯一正式交付评论。旧文件/旧评论、PR标题正文Draft、lead/PR16、master/time对执行者冻结，C05不并行写成果。Owner转发固定99号并要求执行且G00通过后，连续D00及G01–G04，不逐专题等待批准。
GitHub暂态异常不因一次失败停工；先核远端是否已写成，再按63号间隔安全重试，通常首次加三次并遵守Retry-After。expected-head/force=false，未知并行写入不覆盖；复用已建对象，评论marker查重，发现工具与实际写请求分别记。
写后compare自157bf5604754严格六件added、旧件零净改删；固定新提交及活动执行分支六件全文/blob/HEAD、评论唯一性与正文都须回读。不能把本地正确草稿代替最终远端文件，尤其复核新包自检计数。
完成只报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、D00/A/B/C实际范围，随后STOP_WRITING，通过GitHub交C05独立验收；不自关R67-01、不自签68通过、不自行开始69。禁止内核修改/补丁、预处理/编译/链接、仓库C/ASM/脚本/旧夹具/QEMU/实机运行、访问Owner本机凭据或外部libc映像、安装工具、额外付费API/子代理、跨仓维护、新建分支PR、合并关闭、强推或删分支；阶段3门不变。
