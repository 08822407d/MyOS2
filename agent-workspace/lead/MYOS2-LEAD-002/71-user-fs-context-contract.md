执行模型：次一档，沿用现有「MYOS2-A-C04b 代码分析执行」的GPT-6＋Extra。
不新建对话、分支或PR，不上传附件；独立验收仍交MYOS2-A-C04。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FS-CONTEXT-45
record_type: bounded_task_fs_context_copy_share_and_exit_contract
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-06
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
result_base_short12: 2d2bcab1348e
lead_before_publication_short12: 2cabad52880a
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
required_review: CORE-USER-FORK-FILES-ROLLBACK-44-REVIEW-001
mandatory_supplements: [R44-S01, R44-S02]
required_write_guidance: 63-github-write-retry-and-readback.md
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fs-context-45/
allowed_files: [MANIFEST.md, fs-context.md, facts-and-boundaries.yaml, fs-context-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 2d2bcab1348e及此前全部文件
executor_rotation: false
external_execution_started: false
activation_gate: Owner转发固定71号且S00预检通过
phase: 2
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 第45批：任务文件系统上下文的共享、复制与退出

## 1. 增量及边界

44把copy_fs和exit_fs作为文件表回滚周边边界；本批只打开这组taskfs_s局部契约。
任务的fs是根目录、当前目录、umask等上下文；files是fd表，file是打开文件对象，三者及其users/refcount/f_count不得混用。
目标为静态init_fs来源、CLONE_FS共享、copy_fs_struct复制、copy_fs错误生产与exit_fs/free_fs_struct归还顺序，以及root/pwd的路径持有请求。

不重做40–44文件表/文件引用链，不完整展开fork/exec/exit、chroot/chdir、挂载/命名空间、权限、锁/RCU、slab或路径解析。
set_fs_root/set_fs_pwd/get_fs_root/get_fs_pwd、unshare_fs、in_exec完整读写生命周期不是本批主审对象；只有为解释必要类型或直接调用关系才定点读取并登记范围。
普通问题按18号延期，无内核修改、补丁、预处理/编译/运行、额外付费模型或阶段3授权。

## 2. S00：固定输入与现场

完整读取本71号、同固定提交44主线回执及63号；必须合读R44-S01/S02，尤其前序失败不能穿过成功操作、源码控制与分析前提分离。
沿用C04b的67接收、MyOS2工作令/公约/P1–P13和18/24/28、适用Mnemosyne指导；新读和继承分栏，不导入其它项目维护。
源码固定time@a039d9803ade，先核options_flags.cmake/panic.c两项金丝雀，再核direct master/time/lead/执行头、PR16/17、开放PR和写区交集。
首次执行写前预期PR17 2d2bcab1348e；读取44交付评论6660650694及本次主线裁定。45目录应不存在；未知后继/同名成果先核归属，不覆盖、不静默换pin。

定点消费44关于copy_fs失败goto files、copy_sighand失败经exit_fs的矩阵，43的任务结构预置指针限定以及42的files归还界限。
在固定执行成果中定位05及31等已存在的taskfs/root/pwd/path引用相关条目，仅查相关定义与历史裁定；实际没有找到则明确记录，不编造编号。
UV-09/10/14等是旧边界，不把不同对象的相似计数问题自动合并或改号；不以发现缺陷数量为目标。

## 3. S01：对象来源与copy_fs真实分支

完整读取fork.c::copy_fs、sched/misc/fs_struct_types.h::fs_struct、sched_type_declaration.h的taskfs_s单独别名，以及init_fs定义和init_task的fs绑定。
引用真实结构定义，不用前置声明代替。区分普通int users、files_struct的atomic_t refcount和file的atomic_long_t f_count。

定点合读dup_task_struct的实际结构复制及44补充；只在有效前序且同一身份稳定时解释tsk->fs的调用前值。
分别列CLONE_FS分支的fs来源、in_exec门、users改变、是否本地写tsk->fs和返回值；不把注释中的锁当作执行，也不由未加锁直接宣判实机竞态。
对非共享分支，跟踪copy_fs_struct返回指针被赋到tsk->fs的时点及后置判空；与copy_files失败仍可能留下旧指针的情形区分。

模型至少包括有效fs的共享/非共享、in_exec为零/非零、分配正常NULL、源fs为NULL/无效、前序身份变化。
源码没有判空不能在图中补门；具体哪个分支先使用fs、是否可能先返回分配NULL须按实际语句顺序区分，不能一概认定相同结果。
只核EAGAIN/ENOMEM等必要单宏和真实返回，不以Linux惯例补规范或推断全部用户可达性。

## 4. S02：copy_fs_struct的字段、路径身份及取得请求

完整读取sched/misc/fs_struct.c::copy_fs_struct及必要的fs_cachep/proc_caches_init；明确分配入口、失败返回、成功写入字段、读取旧对象及锁范围。
逐项列users、in_exec、lock、umask、root、pwd的初始化/读取/赋值顺序；不要假定整个对象来自清零分配。
root/pwd是内嵌path_s，其mnt/dentry为指针成员；复制path值不是深复制目录项/挂载，也不等于已经取得相应持有。

定位固定time中path_get/path_put的真实活动定义，仅读这两个直接包装层及识别其参数/条件必需的最小类型或宏。
其更深dentry/mount增减、销毁、锁和分配器作为opaque，不扩完整路径生命周期。
把源码请求path_get/path_put、包装层实际活动操作、条件性的引用增减和最终物理回收分栏；空壳/注释不能当作实现。

分析第一次与第二次路径取得请求之间的有效性和正常返回前提；root与pwd同址时也要按两次持有事件解释，不能仅凭同指针就称重复释放。
分配成功而后续旧对象访问/path_get失效时，不跨越失效点补成正常NULL/-ENOMEM或完成回滚。

## 5. S03：exit_fs/free_fs_struct与fork错误消费

完整读取exit_fs、free_fs_struct。列保存fs指针、NULL门、task_lock/fs.lock、tsk->fs清NULL、users递减与kill赋值、解锁、条件free的真实顺序。
保存指针发生时点与锁范围分开；非NULL不是对象活性验证，users是否代表全部持有者不能由名称认证。
有效正初值1/2、无其它写者模型分别推演；零/负值/溢出、无效对象或锁/被调不正常返回止于最早失效边界。

free_fs_struct中root和pwd归还请求、缓存free请求的先后顺序必须分别记录；前一步不返回或失效时不能宣称后一步发生。
static init_fs来源与动态fs_cachep来源分开；静态初值与绑定不证明实际最后归还。只允许局部条件模型，不扩大完整启动/退出追证。

回到44已审的copy_process调用周边：copy_fs正常失败为什么跳过exit_fs而进入exit_files；copy_fs成功后更晚失败怎样条件性经exit_fs再到exit_files。
共享成功后的users +1/-1与非共享新对象1→0用不同身份模型。copy_fs内部失效不能伪造caller正常错误路径；中间更早sighand清理边界仍有效。
形成“fs/files身份→取得状态→真实门→字段/路径操作→失败goto或归还→最终可证前缀”矩阵，不重做完整错误标签链。

## 6. S04：六件与验收条件

仅新增allowed_files六件；MANIFEST登记身份、Owner模型选择/后台未知、read_channel、source_pin/base_snapshot、新读/继承、金丝雀、公约句、self_check三数及write_retry_log。
正文突出fs上下文增量；facts分字段/对象/路径事件与错误矩阵；DOT分CALL/RETURN、真实门、字段数据、分析前提；deferred以FS45-Fxx登记真正非同义候选，无数量配额。
旧候选保留qualified_id和状态，来源与反证完整；不能以本批静态接收关闭内核问题。

VERIFIED只取固定源单函数/结构/对象/单宏/单typedef的1–5行，真实符号身份与关键语句对应；extern、定义外注释、#if/#endif单列上下文。
否定断言合读完整活动体；源片段真实、符号正确、支持强度、原blob身份、结构解析、图语义及有限模型分别记录。
错误出口必须在实际检查位置分支；不要先画成功复制或先行callee成功，再沿串行边表示更早的失败。
未执行的P9/预处理/编译/Graphviz/运行写NOT_RUN。仅使用现有临时环境作纯文本/有限整数模型，不执行仓库C/ASM/脚本或安装工具。

## 7. 63号重试与唯一写区

GitHub暂态异常不得因一次失败直接停工：先核远端是否已经写成，再按63号通常首次加三次、约10/20/40秒或穿插安全只读工作间隔重试，遵守Retry-After。
写动作暂缺也多轮正规发现，不据此判定Owner无权限；工具发现和实际写请求次数分开，未做不得编造重试记录。
保持expected-head、force=false，复用已得blob/tree/commit；评论响应不明先回读去重，不覆盖未知并行写，不绕过真实认证/策略/安全限制。
多轮仍不可用才准确报告已完成与剩余阻断，不默认要求Owner下载上传长附件。

唯一执行者C04b/CHAT-EXEC-05，仅原claude/dazzling-cori-q0dnyt/PR17的user-fs-context-45/六件及本人新评论可写。
旧件/他人评论、lead/PR16、master/time、PR标题/body/Draft冻结。不新建分支/PR，不合并/关闭/强推/删除。
写前再次核预期2d2bcab1348e；写后固定新提交和活动分支六件全文回读并核blob/head，compare只本六件A新增，旧件零净改删。
正式交付只申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及实际回读/重试；随后STOP_WRITING交C04，不自行开始46。
Owner转发且S00通过后S00–S04同轮连续完成，不为普通内部步骤另等确认；任务发布不等于自动跨对话投递或后台启动。
