执行模型：次一档，沿用现有「MYOS2-A-C04b 代码分析执行」的GPT-6＋Extra。
不新建对话、分支或PR，不上传附件；主线验收仍交MYOS2-A-C04。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FD-CLOSE-42
record_type: bounded_fd_close_and_local_file_table_teardown_contract
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-05
produced_by: ChatGPT主线规划
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（沿用Owner报告）
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
read_channel: GitHub连接器
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
source_pin_short12: a039d9803ade
result_base_short12: ee0882d46bbf
lead_before_publication_short12: b90b8a2c34d4
fixed_contract_commit: CONTAINING_COMMIT_PINNED_BY_STARTUP_URL
required_review: CORE-USER-FD-LOOKUP-REF-41-REVIEW-001
mandatory_supplements: [R41-S01, R41-S02]
required_write_guidance: 63-github-write-retry-and-readback.md
prior_acceptance: PASS_PENDING_LOCAL
prior_returned_scope: []
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fd-close-42/
allowed_files: [MANIFEST.md, fd-close.md, facts-and-boundaries.yaml, fd-close-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: ee0882d46bbf及此前全部文件
executor_rotation: false
external_execution_started: false
activation_gate: Owner转发本固定68号给原C04b且C00预检通过
phase: 2
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 第42批：关闭一个fd、归还file引用与退出文件表的局部契约

## 1. 增量目标与边界

40区分索引选择、预留、安装；41区分取回、借用与归还标签。本批接续“如何结束持有”，但不展开整个fd生命周期。
选择三条局部路径：fs/syscall.c的close入口；open.c::filp_close→file_table.c::fput；file.c::exit_files→put_files_struct。
区分fd槽、file对象及其f_count、files_struct对象及其refcount、路径/操作表别名、分配器入口和真实物理释放。
旧05::UV-10已登记close直接释放及fput局部语义，UV-09已登记复制对象/缓存来源；它们是基线，不以新编号重复包装。

不重做完整fork/dup/exec、fcntl、VMA、页缓存、挂载引用、锁/RCU/调度、slab或权限审计。
不预设getname/open返回缺口、SB38对象来源问题、US-01已经修复，不把辅助有效file/表模型当实际用户路径认证。
普通源码问题依18号延期，无修内核、补丁、预处理/构建/运行或阶段3授权。

## 2. C00：固定输入、既有指导与现场预检

完整读取本68号、同提交reviews/CORE-USER-FD-LOOKUP-REF-41-review.md及63-github-write-retry-and-readback.md。
必须合读R41-S01/S02，尤其实际非NULL门与有效对象前提、局部计数与opaque副作用、单类型别名证据。
沿用C04b已完成的67接收及MyOS2工作令/公约/协议v2 P1–P13、18/24/28与适用Mnemosyne指导；无需再次轮换或重建接收。
新读与继承范围分别记录，MyOS2 Owner规则/工作令/本任务才是本次执行权威；Mnemosyne仅行为指导，不导入其维护任务。

源码固定time@a039d9803ade，先核options_flags.cmake及panic.c金丝雀，再核direct master/time/lead/执行HEAD、PR16/17、开放PR及写区交集。
读取41交付评论6652894020及其后本次主线验收；首次执行写前预期ee0882d46bbf，42目录应未存在。
主线合法推进不算执行冲突；未知执行后继、同名成果或竞争写入先核归属，不静默换源码、不覆盖旧件。

定点读固定执行ee0882d46bbf的user-vfs-baseline-05/deferred-findings.yaml中UV-09/UV-10/UV-11，user-syscall-stdio-08/deferred-findings.yaml中US-01及必要主线限定。
消费41引用矩阵及40的R40-S01/S02；39组合回执和38对象边界按既有记录继承。不重做此前整批，不把旧推测当实际物理释放结论。

## 3. C01：close入口的索引、空槽、清除与返回

完整读mykernel/fs/syscall.c的MYOS_SYSCALL_DEFINE1(close)实际函数体，及fdtable.h所需类型/容量定义。
逐语句列unsigned int fd、fd_count逻辑界限、物理256槽、实际取槽、kfree调用、清NULL和最终return顺序。
辨认fd<0对所选无符号参数的意义；不要将逻辑上界检查自动提升为物理容量检查，或把空槽检查补成源码实际存在的门。

分别推演逻辑范围外、表内NULL、有效file、fd_count超容量且索引界外、后继调用不能正常返回等情况。
NULL输入的处理只允许定点追到固定源码kfree实际入口中必要的NULL/低值门；准确定位后回固定time，禁止递归整个分配器。
若入口依赖宏，可读最小相关定义；仍不能据函数名或Linux惯例断言NULL总是安全、编码错误会被拒绝或内存实际释放。
只在kfree及先行前缀正常完成且files对象仍有效等条件下说明清槽与返回，不跨更早非法下标、失效对象或未证callee补出0/errno。

追踪同一file另有持有者的局部模型时，先说明来源和有效性；close是否读取f_count、是否调用filp_close/fput/操作表回调以完整活动体为准。
不得把注释的close回调或close_fd当成实际调用；不将直接释放请求自动写成已经实测UAF。

## 4. C02：filp_close、fput及两种对象计数

完整读open.c::filp_close和file_table.c::fput，列声明返回类型、活动计数操作、条件缓存free调用、返回值来源和被注释步骤。
区分“调用fput”“计数减到零”“进入kmem_cache_free入口”“物理回收已完成”；最后一项不在本任务认证范围。
只定点核__alloc_file/files_init的filp缓存来源及dup_fd分配file副本的相关赋值，补来源矩阵，不重做UV-09分配器分析。
已有引用转移不一定需要本地get_file；不要求每个安装/关闭都必须增加计数，也不根据缺少某个计数调用证明完整所有权已成立。

对有效file、初值1/2等有限计数、先行步骤正常及无其它计数修改建立事件表。无效/零初值/溢出或重复使用已失效对象止于边界，不猜实际后果。
flush/release/path_put等只判断所读函数内是否存在活动调用；不能由局部缺席推成全仓永不存在，不能以注释还原完整Linux关闭语义。

## 5. C03：exit_files与put_files_struct的局部释放顺序

完整读file.c::exit_files与put_files_struct。分别列tsk->files取值、NULL门、task_lock/unlock、将tsk->files清NULL、随后put_files_struct的顺序。
对象files_struct.refcount与各file.f_count必须分开；它们分别保护什么只能在已读持有/释放关系内说明，不靠名称补齐全局线程模型。

核put_files_struct在何条件下遍历、i的类型与fd_count比较、每槽判空、filp_close调用及files_cachep释放请求。
说明分支不进入时的可证前缀，正常结束时的表对象状态；不是所有exit_files调用都立即遍历或释放表。
fd_count可能大于物理容量时，在最早不合法访问或更早callee不返回处停止；不继续推导已完成整个表的清理。

允许条件模型：无files；有效表refcount大于1；最后持有者且全空表；一个有效槽；两个槽别名同一file且计数分别已配平/未配平。
对别名槽顺序，只记录本地引用事件和最早失效边界，不默认真实fork/dup建立了该模型，不在可能已释放后继续访问并算第二次确定结果。
是否在每个filp_close之前/之后清槽、是否检查其返回值以实际体为准；不把销毁整个表的语境混成正在使用的表上的单fd关闭。
可以定点定位一个直接exit_files调用者以说明入口来源，但只读取调用周边，不展开进程退出全链。

形成“入口与对象来源→真实检查→槽/计数写入→callee与返回→条件释放请求→最后可证状态/停止点”矩阵。
新候选仅在不与UV-09/UV-10/UV-11/US-01/FG41同义时使用CL42-Fxx，无数量配额；已有qualified_id保留状态，不自行CLOSED。

## 6. C04：六件、自查、固定证据

只新增allowed_files六件。MANIFEST登记produced_by、product_surface、model_per_owner、backend未知、read_channel、base_snapshot、输入新读/继承、金丝雀、公约句、自查三数及真实write_retry_log。
正文只写本批增量，facts分别列两种计数/槽状态/函数返回与条件模型，DOT区分真实控制、调用、数据、分析前提及停止点。
不得把非NULL写成有效对象验证，不把free源码调用画成已证物理释放；图中文字摘要与展开不重复执行一次操作。

每条VERIFIED只取固定源一个函数/结构/对象表/单宏或单独类型别名定义的1–5行。不同typedef分别给标签；函数外注释、#if/#endif、extern/EXPORT单列上下文。
字节、符号身份、片段支持强度分别检查，完整活动体支撑否定断言；重复片段不是独立根因或功能通过数。
规范化P9适用项，未执行写NOT_RUN/unchecked；原文件完整blob身份、文本解析、图结构/语义、有限模型分别登记，不把转录检查冒充原件核验。
只用现有临时环境的纯文本/有限整数模型，不安装，不运行仓库C/ASM/脚本、预处理/编译/QEMU/硬件或额外付费模型/API。

## 7. 63号重试、写区与停止线

GitHub暂态异常不得因一次失败直接停工；先核远端是否已写成，再按63号通常首次加三次、约10/20/40秒或穿插独立只读工作间隔安全重试，遵守Retry-After。
工具写动作临时缺席也多轮正规发现，不据此判断Owner没有仓库权限；发现次数与实际写请求次数分开。
保持expected-head、force=false；复用已得blob/tree/commit，评论响应不明先查重，未知并行写入不覆盖，真实认证/策略/安全限制不绕过。
多轮仍不可用才准确说明已完成/未完成部分并保全安全成果，不默认让Owner下载上传长文件；未重试不得编造日志。

唯一执行者仍CHAT-EXEC-05，只向原claude/dazzling-cori-q0dnyt/PR17的user-fd-close-42/写规定六件及本人进度/交付评论。
全部旧件、lead/PR16、master/time、PR标题/body/Draft及他人评论冻结；不新建分支/PR，不合并/关闭/强推/删分支。
写前重核预期执行头ee0882d46bbf；写后固定新提交和活动分支各全文回读六件并核blob/head，compare只本目录六件A新增、旧件零净改删。
正式评论只申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及真实回读/重试；随后STOP_WRITING交本C04，不自行开始43。
Owner转发且C00预检通过后C00–C04同轮连续执行，不为普通内部步骤再次等待确认。发布不等于自动投递或后台启动。
仍处阶段2；不修内核/用户程序、不出补丁、不编译运行、不操作Owner本机/仓库外libc/镜像/凭据，不跨项目维护或进入阶段3。
