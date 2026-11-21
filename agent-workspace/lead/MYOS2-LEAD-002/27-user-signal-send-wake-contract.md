---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SIGNAL-SEND-WAKE-10
record_type: chat_signal_sender_static_analysis_contract
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；Owner报告本轮选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-10-31
read_channel: connector
evidence_class: 后续静态分析任务设计；不是10的执行成果或新的内核正确性结论
authorization_ref: Owner本轮要求发布下一批任务；工作令阶段2、18号、24至26号分工及DOC-FIX-01验收
owner_request: 本对话模型仍然是gpt pro，你发布下一批任务。
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: MYOS2-A-C03a 代码分析执行
execution_surface: existing_ordinary_ChatGPT_conversation
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
receiving_model_tier: next_tier_default
model_routing_reason: 有界源码取证与材料生产由C03a执行；开放冲突和最终验收交C03，不启动额外研究或付费API
instruction_base_short12: 0e2a53c3dcc3
source_branch: time
source_pin_short12: a039d9803ade
result_base_short12: 037b880e792b
default_branch: master
default_branch_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
lineage_note: 沿用CORE-CHECK-01既有执行谱系的第10批；C03a是执行身份，不另建平行谱系
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-signal-send-wake-10/
allowed_files: [MANIFEST.md, signal-send-wake.md, facts-and-boundaries.yaml, send-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 037b880e792b中的全部文件；包括08、09及DOC-FIX-01
writer_assignment: C03a为本批唯一执行写入者；交付后停写待C03验收
previous_executor_delegation: CC02旧委派不恢复；旧本机进程及未推送内容仍unknown
activation_gate: Owner将本任务入口发给已有C03a；绑定核对通过后同一工作包连续推进
external_execution_started: false
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
required_review_record: CORE-USER-RETURN-SIGNAL-09-DOC-FIX-01-REVIEW-001
prior_documentation_return: SR09-01_CLOSED_DO_NOT_REEXECUTE
inputs_read:
  - 本轮PR16/17元数据、heads直接引用、仓库元数据、开放PR集合及第三项为空、两PR完整变更路径。
  - 最新sr09-doc-fix-accepted检查点全文；09主说明第718-743行；23号旧任务第1-90行；工作令第211行至末尾。
  - 固定time的options_flags.cmake与lock_IPC/syscall.c全文；lock_IPC/signal/signal.c第1-150、350-802行；sched目录导航。
  - 沿用已读18号、24至26号与09/DOC-FIX-01回执；Mnemosyne单一PR谱系guard第1-128行和写入可见性guard全文为本轮定向核对。
self_check: {scope: task_design_not_execution, new_source_verdicts_issued: 0, runtime_checks_run: 0}
open_questions:
  - 发送侧的输入、目标解析、排队、唤醒与09消费边界尚须由本批实际取证，任务书不预填结论。
  - 本轮未穷尽PID、锁、调度和全部信号生产者；不可把任务设计预检当作它们已通过审查。
---

# CORE-USER-SIGNAL-SEND-WAKE-10：信号发给谁，怎样挂起，是否触发唤醒

## 1. 当前目标与启动

09及其勘误已按有限静态范围验收，SR09-01关闭。09说明的是返回用户态前怎样选取、处理信号；其“覆盖与剩余边界索引”明确把发送端与唤醒完整语义留在范围外。本批补这段上游：以实际kill系统调用为主线，说明从调用参数到目标任务、pending位图/队列，再到唤醒接口和09接收端的连接。没有欠缺必须先修的文档项，不重做08、09或26号。

选择该范围是因为它直接利用现有09接口和仓库内源码，不依赖内核修复、外部libc、构建或硬件实验。这里的“连接”是阶段2源码依赖取证，不是阶段3架构综合、学习路线排序或修复设计。

Owner只需向已有C03a发送：

> 执行CORE-USER-SIGNAL-SEND-WAKE-10。完整读取27号任务书，沿用已有接收和GPT-6＋Extra；按S00至S04连续完成信号发送、目标解析、挂起队列与唤醒连接的静态分析、自查及原PR17回传。只写本批新目录；普通问题入库，不修内核、不重做08/09，完成后交C03验收。

这是一个完整工作包，不是五次审批。内部读取顺序、文本核对和报告修订由执行方自行安排，不为常规发现停下来问Owner。只有真正的输入/权限冲突、未知并行写入或无法继续的访问故障才突出最小阻断；其他不依赖部分继续。任务发布不代表对话自动启动。

## 2. S00：绑定与最小读取

保持MYOS2-A-C03a / MYOS2-LEAD-002-CHAT-EXEC-01身份，不重做25号接收，不新建会话、分支、PR或本机clone。核对本任务、instruction_base中的最新检查点和DOC-FIX-01回执结论，确认SR09-01已关闭；18号延期、24号分工和25号行为规则继续有效。本任务为第10批重新授予明确写区，不延长26号已经结束的单文件权限。

指令从lead读取；旧结果固定037b880e792b；源码固定a039d9803ade。先核实际PR/分支头，先读mykernel/scripts/options_flags.cmake，再判断活动代码和宏分支。配置文本引用外部变量时，不能仅凭未列某个-D就断言所有实际构建均无该宏。time变化时只把本批作为旧固定对象的历史分析并登记新头，不悄悄改证据基线或覆盖新源码事实。

旧结果基目录为agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。仅读09主说明的范围索引、D02-b发送前提、RS-03/04/12/13/14相关段落和被使用的结构化记录；调用返回值的消费边界同时读DOC-FIX-01勘误。07/08/03只在接口依赖实际涉及其既有ID时定点引用。不要全量加载8229行facts、10661行queries，也不要重新运行旧检查器。

源码入口已在任务设计预检中定位：mykernel/lock_IPC/syscall.c的kill；mykernel/lock_IPC/signal/signal.c的prepare_kill_siginfo、kill_something_info、kill_proc_info、group_send_sig_info、send_signal_locked、__send_signal_locked、__sigqueue_alloc/free、complete_signal、signal_wake_up_state。头文件/位操作/PID及唤醒实现应沿这些真实引用定位到固定对象中的实际定义，不按Linux目录习惯猜路径。

允许读取直接依赖的signal头、类型、位图/链表原语、PID查询/任务绑定、调度唤醒接口、初始化调用点及必要构建文本。只为闭合本批断言展开依赖；遇到整个调度器、PID生命周期、分配器或异常恢复内部，标出接口和未知后停在边界，不将其变成新的子项目。

## 3. S01：调用入口、目标解析与输入边界

从活动系统调用表、包装器到sys_kill实际函数，确认号、实参类型、调试宏及返回值传递。不得根据函数名或Linux/POSIX常识预设行为。回读ALERT_DUMMY_SYSCALL等宏对该路径是否有实际控制影响；不要未经核对把调试打印当作不返回，或把stub名称当作无实现。

逐段追踪prepare_kill_siginfo → kill_something_info → kill_proc_info → find_vpid/pid_task → group_send_sig_info。区分用于查找任务的pid_type、用于选择pending容器的pid_type、调用者与目标任务的namespace/身份来源；给出指针来自何处、NULL如何处理、哪些生命周期/并发前提未验证。只展开PID查找和当前绑定所需定义，不审计全PID分配/销毁。

形成输入/结果矩阵，至少覆盖以下类别，可合并同源行但不得漏掉差异：正pid存在、正pid不存在、发送给自身、pid=0、pid=-1、pid<-1；sig=0、合法边界、负值及超过范围的值；SIGKILL与普通信号、普通与实时编号的不同路径。每类都列实际条件、校验位置、是否到目标解析/排队、源文中的局部返回或不返回、未知和证据。类别不是预设测试通过：不适用、缺少检查、不返回或无法推导均须明示。

重点核查信号号检查、权限检查、sig=0的处理、被忽略/被屏蔽动作是否在发送侧过滤，还是交给09接收侧。若需要继续到sigaddset等宏才能解释非法编号，读取真实类型/位移表达式；遇越界、未初始化或未定义行为，只记录文本事实、前提和风险，不推导确定的寄存器值、固定返回码或必然崩溃。

## 4. S02：pending位图、队列与返回值分别意味着什么

明确t->pending与t->signal->shared_pending的选择条件。按实际语句顺序记录：队列节点分配、初始化、信息填充、链表插入、位图更新及局部返回；辨别是否先入链再写info，调用方是否持有被调函数所要求的锁。函数名含locked、注释要求持锁或接收端持锁，都不能替代发送端实际锁证据。

核查普通信号重复发送、实时信号、SIGKILL、PF_KTHREAD、分配成功与失败、特殊info指针分支的条件；区分从本次sys_kill真正可达的分支与其他调用者/条件路径。追踪override_rlimit等传入参数是否赋值及实际消费，不因后续某个操作看似不需要它就跳过语言层未知。对未定义前提下的后续路径仅作有条件源码描述，不承诺运行结果。

siginfo至少列si_signo、si_code、si_pid、si_uid的实际来源或未初始化状态；pending位存在不等于存在完整siginfo节点。只定点衔接09的dequeue_signal/collect_signal摘取和释放接口，说明无节点回退及相同编号多节点的消费前提，不重做整段get_signal、建帧或sigreturn。查明sigqueue_cachep创建位置及能找到的初始化调用入口，无法闭合顺序就记依赖缺口，不展开slab实现。

每条关键分支分开报告“源文局部返回值”“队列/位图副作用”“是否触发通知/唤醒”“对端以后能否取到所需前提”。不要将return 0、置位、入队、目标运行及handler实际执行合成一个成功结论；也不要用某个可见缺口否定所有不依赖它的局部事实。

## 5. S03：唤醒连接、活动调用与09接收端

围绕complete_signal、signal_wake_up、signal_wake_up_state、wake_up_state、kick_process及直接调用点，建立有界调用证据表，分别标注定义、活动调用、注释调用、宏/条件分支及检索未覆盖范围。若当前kill路径在某处缺少活动调用，先指出断点；不要因为被调函数存在，就把这条边画成活动链。

必要时检查唤醒接口怎样设置TIF_SIGPENDING、传入哪些任务状态及调用哪个调度入口；只读直接接口与必要分支，不重新验证调度策略、远端CPU时序或原子/锁全局正确性。没有活动连接的函数体内问题应登记为潜伏/有前提风险，不能混入本次sys_kill必经路径。反过来，有限名字检索也不能证明全仓绝无间接调用。

与09按对象和时序衔接：同一目标的pending在哪里被消费；解除屏蔽、目标已运行/可运行、目标正在睡眠、自发信号分别需要什么后续入口；哪些只是排队而不能保证及时处理。以09已核返回入口为边界，不自动把任意中断/异常返回当作信号处理入口。只讨论源码条件，不给最大延迟、唤醒可靠率或实际调度保证。

以CORE-USER-RETURN-SIGNAL-09::RS-03/RS-04/RS-12/RS-13/RS-14为既有线索，核查本批证据对其“发送方/唤醒方前提”是补充、收窄还是有冲突。同根因优先evidence_appends，不复制旧问题正文或另算独立根因；确有矛盾记录erratum候选和证据，最终旧项状态由主线裁定。RS-01、GAP-U04硬件/运行部分及GAP-U10不在本批关闭范围。

## 6. S04：六件交付、自查与结束条件

只在allowed_write_prefix下形成以下六件，不另造检查器、批量日志库或旧报告副本：

| 文件 | 必需内容 |
|---|---|
| MANIFEST.md | 身份、固定输入、S00-S04完成/部分完成、六文件清单、自查摘要、未覆盖项、runtime_evidence: NOT_RUN、mainline_acceptance: PENDING；建议最后发布 |
| signal-send-wake.md | 连续可读的发送链说明、输入矩阵、排队/返回/唤醒分层、与09衔接及消费限制；先给结论，不堆工具流水账 |
| facts-and-boundaries.yaml | 精简能力/边、关键分支矩阵、源码锚点、必要的有界查询记录和unknown；用ID引用，不重复大段正文 |
| send-paths.dot | 与facts一致的节点/边；活动、条件、缺失/未核连接明确区分，不把注释调用画成活动调用 |
| deferred-findings.yaml | 本批新增、旧项证据追加及确有必要的文档勘误候选；无新增亦显式为空并写覆盖范围 |
| evidence.md | 实际读取/查询范围、方法、检查结果与NOT_RUN、引用/ID核对、写前状态及回传说明；后置远端读回放PR评论 |

本批按05/07/08/09词汇和边方向沿用：call为调用方→被调用方，data为消费方→提供方，init_order为后可用者→先决条件；被注释的建议路径不冒作实际call。新增ID以本followup限定，跨包引用用全限定ID；正文可给简称表。不要重编号旧条目。

关键源码结论必须带路径、实际函数/宏及1–5行逐字引文。至少覆盖：活动入口/包装器、pid分流与实际查找、输入/权限检查状态、pending选择、分配与关键参数、信息/位图/链表写入、唤醒连接或断点、09消费接口。不是规定锚点数量，不能用增加同义查询替代缺失证据；相同源段集中保存、其他位置引用其ID。

全部本批强制引文须在固定源码核对文字与所属符号；区分源码事实、条件推导、外部假设、未核和运行观察。默认分支搜索只作导航；否定结论记录实际文件集合、关键词、注释/宏处理及截断情况，不把“未找到”升级为全局不存在。未完整取到固定检索范围时收窄结论，不伪造全树grep。

可用GitHub连接器完成全批；允许自己的临时沙箱以已有Python做已取得固定文本的字符串、YAML/JSON、ID和图一致性核对。不安装工具、不执行仓库脚本、不建C/ASM宿主夹具、不克隆或操作Owner工作树。实际执行与人工核对分开记；没有运行就写NOT_RUN，脚本/工具可见不等于已调用成功。无需API额度、额外模型、Deep Research或大规模子代理配置。

自查需主动尝试推翻关键概括：成功返回是否必然有节点；有pending是否必然通知/唤醒；对端被唤醒是否必然执行handler；无活动调用是否仅限本路径；未知值是否被错误分类。列出支持条件和反证/未覆盖，而不是机械地全部打勾。语言层未定义/未初始化前提不得被“这是静态分析”掩盖。

主线验收以六件齐全、关键分支有证据、矩阵/正文/图一致、旧项不被越权改判、未覆盖如实、diff只在本批为准。允许某些硬件/外部/全局问题保持unknown且完成有界分析；关键必读路径获取失败则标PARTIAL，不能只列TODO就报READY。最终仅回传READY_FOR_MAINLINE_REVIEW，不自行签发主线PASS。

## 7. 单写者、写前写后和恢复规则

沿用PR17/claude/dazzling-cori-q0dnyt；首次写入前应仍为037b880e792b。本批仅C03a可新增上述六件，并追加自己的PR17进度/交付评论；不改PR标题、正文、状态或他人评论。C03继续独占lead与最终验收；CC02旧委派不恢复。分支名称中的claude只表示历史名称，不表示新成果作者。

写前复查default/master、time、执行分支、开放PR和路径交集；本轮主线已核仅PR16/17开放且分别在lead/results，无本批新增路径。首次目标目录若已存在或执行头不明变化，先查来源和是否本任务已经交付，不覆盖、不重新起同名任务。lead正常追加不算执行源码漂移。

本批六件允许分次顺序提交；每次以上次自己已核的提交为期望头。037b880e792b内的旧件永远冻结。本批未交付前仅可用当前blob SHA修订自己新建的这六件；它们仍须在相对基线的最终diff中表现为A新增。不得删除文件或覆盖他人变动；正式交付后本批原件冻结，后续勘误须新任务。连接器没有原子分支头条件时明确竞态限制，不声称已锁库。

写后以返回的固定提交完整读回本批文件，并按分支核blob身份及当前头；compare 037b880e792b到交付头必须只含上述六路径A新增，无旧件修改/删除和不明额外提交。PR17仍open/Draft；默认/source/lead引用未经执行方修改。字节或哈希未实算就不声称已算，以实际Git对象核对记录。

任务中断但已产生可信部分时，及时保存到本批允许文件/自己的PR17进度评论，明确PARTIAL、已读/未读、当前短提交与下一安全动作；沿同一任务恢复，不重新接收、不另开分支。创建/更新调用超时或结果不明时先读远端确认，再决定是否重试，避免重复或覆盖。可继续不依赖阻断的安全取证。

最终交付评论给实际短提交、六文件入口、完成范围、diff与完整回读结果、仍未做项。所有公开Git对象链接也使用工具复制的短提交；章节符号用正常§或标题，不将§误替换为反引号。不会自动向C03发送消息，Owner只需通知完成，正文由主线读取。

## 8. 不变的禁止动作

普通缺陷、条件风险和策略选择按18号入库，不要求Owner现在修复，不自行实施A/B或恢复被注释机制。不修改内核、用户程序、旧脚本或旧成果，不生成内核补丁，不预处理、编译或运行C/ASM/内核/QEMU，不安装、挂盘或读凭据，不读仓库外libc/镜像，不直写master/time/lead，不合并/关闭PR、不删除/重命名分支、不force-push，不进入阶段3。

本工作包结束即停写待C03验收，不从TODO自行启动11或另一专项。当前任务完成的含义是发送侧静态边界材料可被有限审查，不是MyOS2信号可靠性已经通过运行验证。
