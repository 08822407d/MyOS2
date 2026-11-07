执行模型：次一档，沿用现有「MYOS2-A-C04b 代码分析执行」的GPT-6＋Extra。
不新建对话、分支或PR，不上传附件；独立验收交MYOS2-A-C04。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FILES-COPY-SHARE-43
record_type: bounded_files_table_provenance_copy_share_and_unshare_contract
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
result_base_short12: 6303ec8e9899
lead_before_publication_short12: d7d7477c3a15
fixed_contract_commit: CONTAINING_COMMIT_PINNED_BY_STARTUP_URL
required_review: CORE-USER-FD-CLOSE-42-REVIEW-001
mandatory_supplements: [R42-S01, R42-S02]
required_write_guidance: 63-github-write-retry-and-readback.md
prior_acceptance: PASS_PENDING_LOCAL
prior_returned_scope: []
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-files-copy-share-43/
allowed_files: [MANIFEST.md, files-copy-share.md, facts-and-boundaries.yaml, files-copy-share-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 6303ec8e9899及此前全部文件
executor_rotation: false
external_execution_started: false
activation_gate: Owner转发固定69号给原C04b且T00预检通过
phase: 2
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 第43批：文件表从何而来，何时共享、复制与解除共享

## 1. 增量目标与明确边界

40–42已分别覆盖fd选择/安装、取回/局部引用、关闭/表归还。本批补这些模型所依赖的files_struct和file对象来源，而非重复审查它们。
核静态init_files绑定、copy_files共享/复制分流、dup_fd的表和逐槽file构造，以及unshare_fd/unshare_files的结果发布与旧表归还。
核心区分：任务对象、files表对象、表内槽、file对象及其指针成员；复制一个指针、复制整个file、为原file增加引用是不同事件。

CL42-F01已知init_task静态绑定到init_files；未证实际最后引用路径。UV-09已记录按值file副本及缓存来源问题，不能同义重立新号。
不以证明缺陷为预设结论；正常共享和不需复制的反证同样登记。无法在本批局部范围证明最终归零时保持needs_evidence。
不审完整启动、kernel_clone/copy_process、fork/exec/exit、信号/调度/锁RCU、slab、VMA、权限或文件系统IO；不作POSIX/Linux全兼容判定。
普通源码问题依18号延期；不修内核、不出补丁、不预处理/编译运行、不进入阶段3。

## 2. T00：固定输入、既有指导与现场核对

完整读本69号、同固定提交reviews/CORE-USER-FD-CLOSE-42-review.md及63-github-write-retry-and-readback.md，必须合读R42-S01/S02。
沿用C04b已完成的67接收、MyOS2工作令/公约/协议v2 P1–P13和18/24/28及适用Mnemosyne指导；不新建接收会话，不导入跨项目维护。
本次新读/继承分栏；MyOS2 Owner指令、工作令及本任务是执行权威，Mnemosyne只作行为约束。

固定time a039d9803ade，先核options_flags.cmake及panic.c金丝雀；再核direct master/time/lead/执行头、PR16/17、开放PR、写区交集与43目录缺席。
读42交付评论6654442468及后续主线裁定。首次执行写前预期6303ec8e9899；未知执行后继或同名成果先核归属，不覆盖、不静默换源码。
主线合法前进不是执行冲突，PR正文中的历史执行头不能替代当前ref。

定点读取固定执行6303ec8e9899的42正文/CL42-F01、05::UV-09/UV-10/UV-11、08::US-01/US-03及其已存在的主线限定。
40/41对引用转移、非NULL与有效对象、局部计数与opaque副作用的边界继续有效。新表/复制file来源与旧缓存问题分开，不重做旧分配器分析。

## 3. T01：静态来源与copy_files入口分流

完整读取file.c::init_files定义、init/init_task.c::init_task中files绑定及最小必要上下文、fork.c::copy_files完整活动体。
分别记全局静态对象地址、初值、current->files取得的实际输入、目标tsk->files在调用前的值和调用后的值。
只在直接相关位置定位copy_process调用copy_files，以及此前任务结构复制中提供tsk->files初值的必要赋值/最小helper。
若依赖arch_dup_task_struct等helper，应定位真实定义后回固定time；不从函数名或注释补出整结构复制，不展开完整copy_process。

对oldf为NULL、带CLONE_FILES、无CLONE_FILES三路逐句列：error初值、是否分配、对哪一个表refcount操作、是否写tsk->files、实际返回值。
共享分支若未显式赋tsk->files，须追到有效先行来源才能认定它与oldf同址；单独的atomic_inc不能证明目标任务真的持有该表。
源码if(oldf)或clone位测试不是对象活性、计数合法性或全局并发检查。类型与CLONE_FILES宏只取必要定义，不展开调度或系统调用全ABI。

对于init_files候选，分别列“已证静态绑定”“有界传递/替换路径”“未证运行中最后归还”。不能由init_task名字、静态refcount=1或CLONE_FILES分支单独宣布非法free已可达。
本批可收窄CL42-F01的证据缺口，不自行CLOSED、不实施修复；完整生命周期仍在范围外。

## 4. T02：dup_fd的表对象、逐槽复制与失败出口

完整读mykernel/fs/vfs/file.c::dup_fd及fdtable.h中容量、files_struct_s类型；必要时定点核fork.c files_cachep/proc_caches_init与fs.h file_s字段。
逐语句核errorp写入、外层kmem_cache_alloc及NULL门、refcount/fd_count/锁初始化、max_fds参数及旧新数组。
区分max_fds、oldf.fd_count、固定数组256三个范围；循环条件是否真实限制物理索引以活动体为准。

每个旧槽为NULL或非NULL分别列新槽实际写入；没有写入不等于新槽必为NULL，也不等于真实残留值已知。
分配调用缺少显式清零请求可记录，但不从名字断言实际返回脏内存；仅需要时读最小flags/包装器，不追完整分配器。
核内层kzalloc失败是否被检查、*new_fp=*old_fp的最早有效对象前提、新槽发布及return newf顺序。
外层分配失败与内层失败分开；不可跨过NULL解引用补出正常NULL返回、确定errorp或已完成清理。

按值复制后的file对象地址、f_count数值、f_pos数值、f_path/f_mapping/f_op等指针成员分栏。
“新file对象”不代表其所有指向对象也被复制；复制计数值不等于为父file取得引用；不把两份独立标量与共享指针混同。
只登记与UV-09的增量边界。out_release标签、注释及活动跳转分别列；未读路径不得宣称不存在所有回收。

至少覆盖零/正常max_fds、物理边界、全空旧表、单个非空槽、旧槽别名同一file、外层分配NULL、内层分配NULL、正常复制及errorp与返回指针的配合。
这些是局部条件模型；无效输入停止于最早失效点，不编造实际fork或缓存损坏。

## 5. T03：unshare_fd与unshare_files的输出和发布

完整读fork.c::unshare_fd/unshare_files。定位一个实际调用者（如exec.c中已定位的unshare_files调用）仅读周边门和返回消费，不展开完整exec。
分别跟踪输入unshare_flags、current->files、refcount测试、new_fdp输出槽、dup_fd返回与error、调用者copy=NULL初值。
当不满足复制条件时，helper是否写输出、caller为何可正确解释!copy必须靠真实初始化证明；不要把未写任意输出指针说成已写NULL。

正常得到copy后，按顺序列保存old、task_lock、task->files=copy、task_unlock、put_files_struct(old)与return。
发布新表与归还旧表是两个不同事件；保持旧表身份与被测试对象相同、计数合法及无中途写者的前提，不能用锁名称证明全局原子性。
在无其它写者的模型中，复制条件要求旧表refcount>1时，后续一次put的局部计数结果如何，独立列出；不得因此证明任意并发或其它exit路径不会最后归还。
发生dup_fd失败、无须复制、无files、或旧表归还不正常返回时，分别给最后可证输出/发布状态与错误返回，不跨opaque边界。

形成“task/files/file身份→输入位/计数→真实门→分配/逐槽赋值→输出指针→发布→旧表put→结果/停止点”矩阵。
新非同义候选使用FT43-Fxx，无数量配额；UV/US/CL42原qualified_id及状态保留，实际修复和关闭仍不授权。

## 6. T04：六件、自查与证据

仅新增allowed_files六件；MANIFEST保留produced_by、product_surface、model_per_owner、backend未知、read_channel、base_snapshot、新读/继承、金丝雀、公约句、自查三数及真实write_retry_log。
正文突出增量；facts分表身份、file身份、指针别名与计数/返回矩阵；DOT明确调用、返回、条件数据关系及分析停止点；deferred写前提、反证、旧号关联与恢复门。
非NULL门不得画为有效对象验证；共享表分支不虚构tsk赋值，输出未写不得画为写NULL；释放请求不画为已证物理回收。

每条VERIFIED只取固定源一个函数/结构/对象表/单宏或单独typedef的1–5行。函数外注释、#if/#endif、extern、EXPORT另列上下文，不能混入定义体计数。
字节对应、符号身份、片段支持强度分别检查；完整活动体支撑否定断言，重复片段不是独立根因。
完整blob身份、结构解析、图结构/语义、有限模型分栏；未执行写NOT_RUN，不把转录检查或作者自述冒充完整原件认证。
仅用现有临时环境的纯文本/有限整数模型，不安装，不运行仓库C/ASM/脚本、预处理/编译/QEMU/硬件或额外付费模型/API。

## 7. 63号重试、唯一写区与停止线

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔安全重试，通常首次加三次、约10/20/40秒或穿插安全只读工作，遵守Retry-After。
写动作临时缺席也多轮正规发现，不据此判断Owner没有仓库权限；工具发现次数与实际写请求次数分开记录。
保持expected-head、force=false；复用已有blob/tree/commit，评论响应不明先查重；未知并行写不覆盖，真实认证/策略/安全限制不绕过。
多轮仍不可用才准确报告阻断和已保存/未完成步骤，不默认要求Owner下载上传长文件，不虚报重试日志。

唯一执行者仍C04b/CHAT-EXEC-05，仅原claude/dazzling-cori-q0dnyt/PR17的user-files-copy-share-43/六件及本人评论可写。
旧成果、旧评论、lead/PR16、master/time和PR标题/body/Draft冻结；不新建分支/PR、不合并关闭、强推或删分支。
写前核预期执行头6303ec8e9899；写后新固定提交与活动分支六件全文回读、核blob/head，compare仅本六件A新增，旧件零净改删。
本人正式评论只申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及真实回读/重试；之后STOP_WRITING交本C04，不自行开始44。
Owner转发且T00预检通过后T00–T04同轮连续完成，不为普通内部步骤另等确认；发布不等于自动跨对话投递或后台启动。
阶段2不变；不修内核/用户程序、不出补丁、不构建运行、不操作Owner本机/仓库外libc/镜像/凭据或跨项目维护。
