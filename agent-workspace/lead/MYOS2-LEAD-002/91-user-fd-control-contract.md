执行模型：次一档，继续现有「MYOS2-A-C05a 代码分析执行」，沿用Owner指定GPT-6＋Extra。
第60批限定接收，无D00返工；第61批连续完成A/B/C，六件一次交付。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FD-CONTROL-61
record_type: bounded_combined_fd_control_contract
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近明确报告）
backend_identity: unknown_or_not_attestable
conversation_display_name: MYOS2-A-C05 内核分析主线续建
date: 2026-11-08
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: 1a6ef1c7a5aa
lead_before_publication_short12: 94777b770c43
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
executor_display_name: MYOS2-A-C05a 代码分析执行
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fd-control-61/
allowed_files: [MANIFEST.md, fd-control.md, facts-and-boundaries.yaml, fd-control-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: []
mandatory_supplements: [R60-S01, R60-S02, R58C-S01]
activation_gate: Owner向现有C05a转发固定任务并要求执行且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
---

# 第61批：fcntl分派、fd复制与文件位置控制

## 1. 新增价值与边界

三个专题为A fcntl命令准入/活动实现/退出，B F_DUPFD路径的槽位安装、别名及引用交接，C lseek入口与FAT位置更新。共用fd/file身份，但不重做40–43的完整分配、引用、关闭或文件表复制，也不等于完整POSIX规范或用户ABI审计。
业务权威为Owner本轮交付处理、00工作令及18/24/28/63/79；Mnemosyne仅作适用指导，不导入其维护任务。继续同一执行者和原PR17，不创建会话、分支或PR，不自动调用其它付费模型。本任务发布不等于已经投递或执行。
普通源码问题按18号延期，保留可反驳前提、反例和最早停止点；本包无D00返工项，不重开58/59/60验收。

## 2. G00：固定输入及写权

完整读取本91号、同发布提交reviews/CORE-USER-FS-CONTEXT-CWD-60-review.md和63/79号。60为PASS_PENDING_LOCAL，合读R60-S01/S02；58 B25主线误判撤回和既有闭合保持。
同一C05a可复用87接收、九份项目规则及适用Mnemosyne指导的实际已读固定原文，记录新读/复用，不拿从未读取的旧声明代替。若真实上下文无法保持授权/证据身份，先保全并报告主线安排独立接收，不能恢复旧C04c或自行转移写权。
固定time@a039d9803ade，回核options_flags.cmake和panic.c双金丝雀；核direct time/master/lead/执行refs、PR16/17及开放PR、新61目录与写区交集。首次执行写前预期1a6ef1c7a5aa；未知HEAD/竞争写入先核归属，不强推或静默换pin。
强制历史回执：CORE-USER-FD-ALLOCATION-INSTALL-40-review.md、CORE-USER-FD-LOOKUP-REF-41-review.md、CORE-USER-FD-CLOSE-42-review.md、CORE-USER-FILES-COPY-SHARE-43-review.md的全部消费限定，均从本主线reviews定位。同一有效会话确已全文读过者可标明固定身份后复用，否则实际完整读。定点取08::US-01、05::UV-10和实际依赖原条目，勿臆造文件名/状态；先列目录定位，保留真实source_field/source_value及qualified_id。
59仅在比较目录f_pos时消费R59-S01/S02，55/56仅在实际读到FAT深层依赖时沿原限定使用，不重启旧批或审全文件系统。

## 3. G01／A：fcntl命令准入、类型与退出

完整读取mykernel/fs/syscall.c的MYOS_SYSCALL_DEFINE3(fcntl)调用体、fs/vfs/fcntl.c的do_fcntl与check_fcntl_cmd，并定点读fdget_raw/__to_fd/fdput、file字段及真实命令常量。
按实际顺序追原始fd取槽、NULL检查、FMODE_PATH门、check_fcntl_cmd、do_fcntl和out1/out。区分原始借用与fget取引用；空槽、无效物理索引、普通有效file、FMODE_PATH及被拒命令各自的最早访问/返回/归还。
必须分别建立“准入名单”和“活动switch实现”矩阵；准入函数返回1不证明do_fcntl实现了该cmd，注释case不作活动代码。覆盖F_GETFL、F_GETFD、F_SETFD、F_DUPFD、F_DUPFD_CLOEXEC及未列命令，实际常量不同来源分别锚定，不按Linux记忆补实现。
追ulong arg→int argi及fd参数转换，区分命令没有使用argi与仍发生初始化表达式；超范围转换只给显式目标模型，不凭低位示例认证真实ABI。F_GETFL的f_flags值来源与文件有效性另列，不能把file状态标志等同每fd的FD_CLOEXEC。
正常返回、早退和fdput事件分别记录；若错误指针/槽值使早先对象消费无效，不补安全EBADF或清理。既有US-01等引用根因只新增该消费者关系，不重复编号。

## 4. G02／B：F_DUPFD安装、别名和引用交接

主干mykernel/fs/vfs/file.c::f_dupfd、alloc_fd、fd_install及get_file/fput直接定义；rlimit/RLIMIT_NOFILE和files_struct物理/逻辑范围只展开直接依赖，沿用40/41/42回执而不拿注释当引用转移证明。
追from与nofile比较、nofile传alloc_fd的形参转换、start/end/flags使用、找到槽与无槽的不同路径、err>=0后get_file→fd_install的顺序。遇alloc_fd未建立返回值或越界/无效file时，停止依赖推断，不自行补EMFILE、保证新fd或回滚。
必须以同一有效file的计数事件链核fcntl外层fdget_raw/fdput和f_dupfd内部get_file；分开“单函数局部配平”“新槽仍持有指针”“对象是否仍活着”，不能只数inc/dec就认证整个寿命。用初始计数1/多持有、失败分支、同file两槽等有限例，并沿close旧事实说明跨接口消费边界，不运行关闭或推定实际UAF。
核O_CLOEXEC如何被传递、所读alloc/install是否真正记录每fd状态；仅有flag实参不认证close-on-exec实现。不得扩展全exec关闭流程。
对syscall.c::dup/dup2只核真实活动宏调用及所依赖ALERT_DUMMY_SYSCALL/控制常量：完整读取实际定义后再说明当前行为，未预处理不虚构展开函数或确定返回码。与fcntl F_DUPFD路径分开；不要求实现stub，不将dup_fd文件表复制混为dup描述符别名。

## 5. G03／C：lseek入口、FAT位置与返回类型

主干syscall.c::MYOS_SYSCALL_DEFINE3(lseek)、myos_fat32.c::FAT32_lseek及FAT32_file_ops真实llseek绑定。先核fd/whence范围门、unsigned负值判断、NULL槽与f_op/method缺席、ret初值和最终返回类型。
SEEK_SET/CUR/END及其它实际常量分别追offset、f_pos、i_size、long/loff_t/off_t/ulong转换；有符号加法在范围检查之前发生时不能跨溢出推EOVERFLOW，非负和不超过i_size门须以有效对象/算术为前提。
核回调失败是否保留f_pos、成功写何处；没有回调时返回初值与“真的seek到了0”不是同一语义。外层ulong暂存负回调结果后的返回必须按所读宏/类型界限陈述，不将机器寄存器或libc errno当已证。
有限例覆盖三种origin、恰零/恰EOF、负目标/超过EOF、加法边界、NULL槽/缺方法、两个fd指向同一file时位置共享。若与59目录枚举位置比较，明确FAT枚举的ctx.pos规则独立，不重做59或宣称所有文件支持相同定位语义。

## 6. 取证预算、六件与验收门

核心输入为syscall.c选定fcntl/dup/dup2/lseek完整调用体、fcntl.c完整、file.c上述完整函数、myos_fat32.c仅lseek/操作表；其它文件仅直接类型、fd/引用、limit和dummy宏定义，额外依赖软上限10个文件。没有找到定义时按P3正规回源/定位留缺口，不因软预算截断必读主干或把默认master搜索摘要当time证据。
六件：MANIFEST.md、fd-control.md、facts-and-boundaries.yaml、fd-control-paths.dot、deferred-findings.yaml、evidence.md。facts至少含source_ledger、prior_review_disposition、fcntl_dispatch_matrix、dup_alias_reference_traces、seek_position_return_matrix、finite_model_cases、prior_findings_links。新条件候选DC61-Fxx、缺口DC61-Uxx，无行数/引文/模型/缺陷配额。
适用协议v2全部P1–P13。强制锚点分A实际fcntl入口/两命令函数/fd归还；B f_dupfd/alloc/install/get_file/fput及实际dummy定义；C lseek入口/FAT方法/操作表/相关类型。唯一宏调用体可定位，但必须注明未预处理；关键缺证如实blocked，不用INFERRED替代强制锚。
每条VERIFIED为单一定义内原样连续1–5物理行，空行计入，跨独立宏拆分。分别核字面连续、行数、定义归属和命题支持；疑似差异必须同pin原成果与源片段实际比对，不继承AI旧误判。MANIFEST有P6原句、双金丝雀、三数及新/历史计数、实际新读/复用、NOT_RUN。
图的CALL/RETURN/DATA/LOCAL_CONTROL/ANALYSIS_STOP须注明调用归属与条件，不能从无效访问后连出安全返回。仅允许临时自写文本/结构与有限数学模型，不执行仓库代码、脚本或旧夹具。

## 7. G04及停止

仅原PR17新user-fd-control-61/六件与本人唯一正式交付评论可新增；全部旧件、旧评论、lead/PR16、PR元数据、master/time对执行者冻结。C05不并行写该成果区。
GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。
默认10/20/40秒或安全只读间隔，遵守Retry-After；复用已创建对象、ref结果未知先查HEAD、评论marker查重，真实认证/策略限制不绕过。写后固定提交和活动分支六件全文/blob/HEAD回读，compare自1a6ef1c7a5aa须仅六件A新增、旧件零净改删。
本人仅申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及A/B/C状态；完成STOP_WRITING交C05，返回真实短提交和正式评论，不自签61通过、不自启62。
不修内核、不出补丁、不预处理/编译/链接、运行QEMU或硬件、不安装工具、不读取Owner本机/凭据/外部libc镜像、不自动付费API/子代理、不跨仓维护、不建分支/PR、不合并关闭/强推删分支；阶段3门不变。
