执行模型：次一档，沿用现有「MYOS2-A-C04b 代码分析执行」的GPT-6＋Extra。
本文件为GitHub主线正式任务书。Owner转发其固定提交并通过R00预检后方可启动；沿用原PR17，不新建对话。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FORK-FILES-ROLLBACK-44
record_type: bounded_fork_files_acquisition_and_error_cleanup_contract
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-05
produced_by: ChatGPT主线规划
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（沿用Owner报告）
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
executor_display_name: MYOS2-A-C04b 代码分析执行
source_pin_short12: a039d9803ade
result_base_short12: 9cd9e0ab4d03
lead_before_publication_short12: 47bfd0c93b85
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
required_review: CORE-USER-FILES-COPY-SHARE-43-REVIEW-001
mandatory_supplements: [R43-S01, R43-S02]
required_write_guidance: 63-github-write-retry-and-readback.md
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fork-files-rollback-44/
allowed_files: [MANIFEST.md, fork-files-rollback.md, facts-and-boundaries.yaml, fork-files-rollback-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 9cd9e0ab4d03及此前全部文件
executor_rotation: false
external_execution_started: false
activation_gate: Owner转发本文件的真实固定GitHub URL且R00预检通过
phase: 2
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 第44批：fork在文件表取得前后失败时，实际清理哪一份持有

## 1. 增量目标和边界

43核了copy_files/dup_fd的局部构造，未核失败返回后的copy_process清理。
本批只补“前序预置指针→copy_files取得/未取得持有→后续错误标签→exit_files→最终错误返回”的局部契约。
关键是正常copy_files失败与成功后其它步骤失败，是否进入不同标签；不以泄漏或重复释放为预设答案。
区分借来的task.files指针、已经增加的共享表引用、新分配表持有，以及表中file对象引用。

读取copy_process相关控制边/标签与必要直接生产者/消费者；其它资源函数作为opaque正常返回/不返回节点。
不审完整fork/exec/exit、信号、MM、PID、调度/唤醒、锁/RCU、slab或完整文件生命周期。
不重做43构造或42释放函数全审；UV-09/10/11、UV-14、US-01、FT43-F01、CL42-F01保持原qualified_id和状态。
普通源码问题按18号延期，不修内核、不生成修复补丁、不预处理/编译/运行，不进入阶段3。

## 2. R00：正式输入和现场门

本文件已经正式发布。启动时必须完整读取本任务固定提交下的70号、同提交43回执与63号。
必须合读R43-S01/S02：node分配门、预置指针不等于取得持有、内层失效不可补成正常错误、输出变量和非法参数的区别。
沿用C04b的67接收、MyOS2工作令/公约/P1–P13、18/24/28及适用Mnemosyne指导。新读/继承分开，不导入其它项目维护。

固定time@a039d9803ade，重新核options_flags.cmake和panic.c金丝雀；核direct refs、PR16/17、开放PR与写区交集。
首次执行写前预期PR17为9cd9e0ab4d03，读取43评论6655478015和后继主线裁定；44目录应未存在。
未知执行后继/同名成果先核归属；主线正常前进不算执行冲突，不能静默替换源码、强推或覆盖。
本任务没有把附件当成正式入库任务的例外授权，也不允许C04b代写lead/PR16。

定点消费43身份/输出矩阵、42的exit_files/put_files_struct限定，以及05::UV-09/10/11/14、08::US-01必要条目。
历史材料只作去重和停止边界，不照搬旧物理释放推测为当前事实。

## 3. R01：文件表取得状态，不由p非NULL推断

定位fork.c::copy_process真实签名、dup_task_struct返回检查、copy_files调用及最近前后控制。
定点复读dup_task_struct赋值和node门，核kernel_clone实际传入NUMA_NO_NODE。栈/调度等前序保持明确有效性和正常返回前提，不扩任务分配全审。

建立四类状态：
1. 尚未调用copy_files：p可能预置旧表指针，但未因此新增表引用。
2. copy_files正常返回0：分别为旧表NULL、CLONE_FILES增旧表引用、非共享新表发布。
3. copy_files外层分配失败正常返回非零：无新表发布、无共享引用增加；目标可能仍保留借来的旧指针值。
4. dup_fd内层失效/不正常返回：没有可供caller消费的正常返回，不伪造retval或清理跳转。

按代码列copy_files后的retval检查和目标标签。指针存在不能证明失败路径已取得可归还引用。
独立task对象、files表对象及file对象的计数与来源分栏；不把共享表增引用写成逐file增引用。

## 4. R02：真实goto、标签贯穿与错误返回

读取copy_process的活动控制结构，只抽取影响files取得与归还的错误边。完整记录实际跳转、标号间贯穿与提前return；注释中的goto不计活动入口。
覆盖copy_files失败、copy_fs失败，以及copy_sighand/copy_signal/copy_mm/copy_namespaces/copy_thread和alloc_pid所见正常错误检查；不递归展开这些资源子系统。

重点比较bad_fork_cleanup_files与bad_fork_cleanup_semundo的位置和到达条件：前者的exit_files(p)是否被后者跳过；为什么不能按标签名字猜测相同清理。
列出到files标签之前的exit_thread、mmput、__cleanup_sighand、exit_fs等实际中间调用，并保留其正常返回/对象有效前提。
已有UV-14可能形成更早停止边界；本批不复审其整个信号对象生命周期，不跨它假定必达exit_files。

bad_fork_cancel_cgroup、pidfd及其它标号，即使含活动语句也须核实际入口，不能因名字存在就判可达；不得把正常return p后的标签当作成功路径继续执行。
区分源码存在一条可连接的条件路径、错误生产者已证、实际运行已触发三种证据等级。

沿相关路径追retval最后赋值、是否被后续操作覆盖、ERR_PTR(retval)，以及kernel_clone的IS_ERR/PTR_ERR消费。
只定点读err.h必要定义；ERR_PTR转换不替代负errno范围证明。无效返回/不返回不能补出确定errno；不扩syscall汇编ABI、PID创建或唤醒执行。

## 5. R03：清理与持有状态配对

定点复读file.c::exit_files/put_files_struct和42回执；只比较本次传入p.files的来源与计数，不重做所有关闭路径。
建立下列条件矩阵：
- copy_files尚未取得持有或外层正常失败：实际清理是否避免归还借来的旧表。
- 旧表NULL且copy_files返回0：后续错误经exit_files时的NULL分支。
- CLONE_FILES成功：有效旧计数n先加1，后续一次归还是否回n；仅限正初值、稳定对象、无其它写者和正常原子操作。
- 非共享新表成功：新表初值1，在后续失败归还时才进入表遍历。原空槽对应新槽未初始化、逐槽file计数复制等旧缺口仍有效，不能把“到了exit_files”当完整清理成功。
- 内层kzalloc失效：不进入伪造的正常caller回滚路径。
- 中间opaque清理先失效/不返回：停止在更早位置；即使已调用exit_files，也须区分摘除指针、减少表计数、遍历file、释放请求和实际物理回收。

task对象销毁与文件表归还不是同一操作。只确认所选路径中的显式exit_files调用次数；没有穷尽其它callee时不声称全局绝无额外释放。
CL42-F01仅可追加局部来源/计数反证：例如共享分支的+1/-1，不足以证明全生命周期没有最后归还。

形成“失败位置→此前取得的files持有→活动goto→必经条件/opaque调用→exit_files是否可到达→局部引用事件→最终返回/停止点”矩阵。
新非同义候选使用FR44-Fxx，无数量配额；已知根因保持旧号，未证机器后果不得升级为已复现漏洞。

## 6. R04：六件、证据和自查

仅新增allowed_files六件。MANIFEST记录身份、Owner模型选择/实际backend未知、read_channel、source_pin、新读/继承、金丝雀、公约句、自查三数与write_retry_log。
正文讲增量；facts含取得状态和错误边矩阵；DOT区分调用/返回/贯穿/分析前提；deferred记录前提、反证、旧号及恢复门。
每条VERIFIED为固定源一个函数、结构、对象、单宏或单独typedef的1–5行。#if/#endif、定义外注释、extern/EXPORT另列上下文。
整数输出参数不得叫ERR_PTR；非NULL门不得叫有效性验证；源码清理请求不得等同实际物理回收。
完整活动体支持否定断言；区分片段字节、符号身份、支持强度、原blob身份、结构解析、图语义和有限模型，未做项写NOT_RUN。
仅用现有临时环境作纯文本/有限状态模型，不安装，不运行仓库C/ASM/脚本、预处理/编译/QEMU/硬件或额外付费模型/API。

## 7. 安全重试、写区和停止线

完整遵守63号：暂态异常不因一次失败直接停工；先核远端是否已写成，再通常首次加三次、约10/20/40秒或穿插安全只读工作间隔重试，遵守Retry-After。
写动作暂缺也多轮正规发现，不据此判断Owner无权限；工具发现与实际写请求次数分别记录。
保持expected-head、force=false；复用已得blob/tree/commit，评论响应不明先查重；未知并行写不覆盖，真实认证/策略/安全限制不绕过。
多轮仍不可用才准确报告已保存与待补步骤，不默认要求Owner下载上传长文件，不编造重试次数。

唯一执行者C04b/CHAT-EXEC-05，仅原PR17分支user-fork-files-rollback-44/六件及本人评论可写。
旧成果/旧评论、lead/PR16、master/time及PR标题/body/Draft冻结。不新建分支/PR，不合并/关闭/强推/删分支。
写前核预期9cd9e0ab4d03；写后固定新提交和活动分支六件全文回读、核blob/head，compare只本六件A新增，旧件零净改删。
正式评论申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及实际回读/重试，随后STOP_WRITING交C04，不自行开始45。
正式任务转发且R00通过后，同轮R00–R04连续完成，不为普通内部步骤另等确认；发布不等于自动投递或后台启动。