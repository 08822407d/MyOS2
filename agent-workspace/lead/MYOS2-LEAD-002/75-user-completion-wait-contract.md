接收模型：次一档（默认），建议GPT-6＋Extra。
执行位置：MyOS2辅助分析Project内新普通对话「MYOS2-A-C04c 代码分析执行」。先接收76号，不继承C04b的规则已读状态。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-COMPLETION-WAIT-49
record_type: bounded_completion_count_wait_and_return_contract
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: ChatGPT主线规划
model_per_owner: gpt pro（沿用Owner报告）
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-06
read_channel: GitHub连接器
base_snapshot: time
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
executor_display_name: MYOS2-A-C04c 代码分析执行
executor_rotation: true
source_pin_short12: a039d9803ade
result_base_short12: d23aa99d2c3a
lead_before_publication_short12: b63fa979e409
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
required_review: CORE-USER-DEVTMPFS-SETUP-48-REVIEW-001
mandatory_supplements: [R48-S01, R48-S02]
required_handoff: 76-executor-continuation/00-C04b-to-C04c-handoff-package.md
required_write_guidance: 63-github-write-retry-and-readback.md
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-completion-wait-49/
allowed_files: [MANIFEST.md, completion-wait.md, facts-and-boundaries.yaml, completion-wait-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: d23aa99d2c3a及此前全部文件和旧评论
activation_gate: Owner转发76固定启动入口；接收报告、项目规则、适用指导及W00现场核对全部通过
phase: 2
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 第49批：completion计数怎样发布、等待、消费并返回

## 1. 增量目标与界限

48已审devtmpfs创建/worker错误通道，且定位固定time的mykernel/kactive/completion/。本批独立打开completion等待核心，而非重复创建方或worker全审。
目标是从init/complete的done状态，经wait_for_completion→wait_for_common→do_wait_for_common，明确真实条件、锁区、等待请求、计数消费与返回类型。
主线预读只提供范围，不代替新执行者独立取证；禁止按Linux常识填补本仓实现。

允许定点读取直接swait辅助体的入队/摘队/唤醒请求及所需类型/宏，不继续追完整调度器、架构锁/中断实现、schedule_timeout内部、信号全链或真实内存序。
不审完整kthread、devtmpfs请求所有权/释放、挂载、启动、完整completion API家族或全局生命周期。不修内核、不生成补丁、不编译运行。
普通源码候选依18号延期，不能因本批编号而强行增加发现数量。

## 2. W00：新接收与固定预检

先完整读同固定发布提交的76/00、76/01、本75号、48回执、63号及本次检查点。
按76号先形成Mnemosyne接收报告，再分别完整加载MyOS2项目规则和适用Mnemosyne指导；不把“C04b已读”当本对话已读。
接收、项目规则、指导刷新和任务预检分别记录于MANIFEST，不增加第七件，不写Mnemosyne或其它仓库。
核Owner所选模型/能力档与实际可知状态；后端不可证写unknown。任务是有界取证，默认次一档；遇超出能力的关键判断明确降级/升级建议，不隐式调用付费模型或自改任务。

固定源码time a039d9803ade；重新核options_flags.cmake、panic.c两项金丝雀及direct master/time/lead/执行头、PR16/17、开放PR、写区交集和GitHub写动作。
首次执行写前预期PR17 d23aa99d2c3a，49目录应不存在。读48交付评论6680077382及本次主线回执；发现未知后继、同名成果或并行写入先核归属，不强推/自动变基/静默换pin。
定点读取48关于complete/wait与DS48-F01/F02/F03的证据、R48-S01/S02；原R47、R46C限定按引用需要消费，不重做历史验收。
若旧基线已有completion/swqueue相关候选，按原qualified_id引用，缺证如实标注，不复制整库或重编号。

## 3. W01：对象与完成计数生产

完整读取固定kactive/completion/completion_types.h、completion_macro.h、completion.h::init_completion、completion.c::complete及completion_s的单独typedef。
分开静态DECLARE_COMPLETION宏使用、COMPLETION_INITIALIZER、动态init_completion，字段顺序、done=0与等待队列初始化表达式；宏条件另列上下文，不执行预处理或认证链接。
核complete的锁取得、UINT_MAX门、done++、swake_up_locked、解锁顺序。必须列0、1、一般正计数、最大值前一项、最大值五类条件，常量数值以实际定义或明确模型假设为准。
最大值不递增但仍发起唤醒请求，不把饱和值等同本批未审的某个complete_all调用，不把一次完成调用当errno负载。
只论局部写入和请求；不能按注释认证FIFO、屏障效果或等待者实际运行。

## 4. W02：等待核心、锁与返回值

完整读wait_for_completion、wait_for_common、do_wait_for_common活动体及必要选择条件。
逐条核：公开入口的MAX_SCHEDULE_TIMEOUT与TASK_UNINTERRUPTIBLE；wrapper加锁、向do_wait传schedule_timeout函数指针、解锁及long返回；最外层void如何处理该long。
对do_wait核初始!done门、DECLARE_SWAITQUEUE、signal_pending_state分支、timeout=-ERESTARTSYS、break、__prepare_to_swait、任务状态写入、解锁、action(timeout)、重加锁、循环条件、__finish_swait及后续计数/return。

分别建立done初值非零的快路径与done为零的等待路径；不要把action函数指针的调用误写为每次入口必调。
核if(!done) return timeout、if(done!=UINT_MAX) done--、return timeout ?: 1的条件与值域，区分0、正值、负值和饱和值模型。
实际公共入口传入的state/timeout和任意helper参数模型分开；负返回生产者不能只凭内部long类型猜测。
为判断公共TASK_UNINTERRUPTIBLE路径是否能进入signal_pending_state分支，允许定点追该helper和必要状态位定义；若定位/绑定不足就明确保留条件，不宣称真实信号中断。
不展开schedule_timeout内部或运行调度模型。action正常返回值只作为有界输入，实际可生产哪些值另列证据/未知。

## 5. W03：直接队列调用和有界时序

按固定time实际树定位__prepare_to_swait、__finish_swait、swake_up_locked及所需swait结构/宏；默认分支搜索只定位，定义必须回固定pin。
可读这三个直接辅助体的入队/摘队/唤醒请求与其直接小型容器操作；进一步任务唤醒/调度/锁实现保持边界，不展开全部等待队列系统。
需要的锁持有前提、对栈上wait对象的保存/摘除、state字段和调用顺序分别记录；宏、extern、函数实现不能混为证据。
未定位部分降级为opaque；不得用注释构造实际队列操作或保证生命周期。

至少覆盖：先complete后wait、先wait再条件性complete、无完成事件、已有多份done的顺序消费、饱和done、action返回0或非零、信号分支可达/未证、先行调用不正常返回。
跨执行上下文只给明确同步前提下的有限事件序，不把图画成同步调用栈，也不从任一有限序列证明完整无丢唤醒、公平性或实机无死锁。
若helper返回值为负而最外层void丢弃，分别说明返回值与计数消费是否发生，不把它解释为worker errno传输。
将48::DS48-F01的等待语义边界补上所能证明的局部事实，但不自动关闭候选；真实失败可达性、所有通知者、重用/对象生存期仍未审。
形成“对象/初值→真实门和锁→入队/状态/等待请求→重检与摘队→计数消费→long值与void消费→未证项”矩阵。

## 6. W04：六件、自查及交付

仅新增allowed_files六件；MANIFEST写接收报告摘要、模型档、项目/指导实际读取范围、金丝雀/P6、固定输入、实际self_check和write_retry_log。
正文、facts、DOT、deferred、evidence分开源码事实/条件模型/未知；新非同义候选使用CW49-Fxx，无数量配额，旧DS48/IP47等保持原ID和状态。
每个VERIFIED仅固定源一个真实函数/结构/对象/单宏/单typedef的1–5行，包含标题声称的关键语句；条件编译、extern、定义外注释另列上下文。完整体支持否定断言。
图需区分调用、正常返回、分析STOP、值传播和跨上下文事件。计数变化、唤醒请求、任务运行与函数返回不得互相代替。
引文逐字匹配、符号身份、支持强度、JSON/YAML结构、图端点/语义、有限模型分项记录；未执行写NOT_RUN，不声称完整P9/编译/运行通过。
仅使用现有临时工具作纯文本与有限状态模型；不安装工具、不执行仓库C/ASM/脚本、不预处理/编译/QEMU/硬件，不访问Owner本机、外部libc/镜像或凭据。

## 7. 写入、重试及停止

完整遵守63号：GitHub暂态异常先核远端是否已写成，再通常首次加最多三次、约10/20/40秒或穿插安全只读工作间隔重试；遵守Retry-After。
写动作暂缺也多轮正规发现；发现次数与真实写请求次数分开，不把临时缺席推断成Owner无权限。复用已得blob/tree/commit，评论响应不明先读回查重。
保持expected-head、force=false，不覆盖未知并行写，不绕过真实认证/策略/安全限制。多轮仍不可用时准确保全并报告待补，不默认让Owner搬运长附件。

只有新C04c/CHAT-EXEC-06可在本批目录写六件及本人新交付评论；C04b不再获49委派。主线C04仍唯一规划/验收者。
沿用claude/dazzling-cori-q0dnyt及PR17，所有旧文件/旧评论、lead/PR16/master/time、PR元数据冻结；不新建分支/PR、不合并/关闭/强推/删分支。
写前再核d23aa99d2c3a；写后固定新提交和活动分支六件全文及blob回读，compare从该基点只新增本六件且旧件零净改删。
正式交付只申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING和真实重试/回读；随后STOP_WRITING交MYOS2-A-C04，不自行开始50。
接收/加载/W00通过后，同轮连续W00–W04，不为普通内部步骤另等Owner确认。任务发布不等于自动投递或启动。
