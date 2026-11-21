---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PATHNAME-COPY-29
record_type: bounded_pathname_acquisition_copy_and_lifetime_contract
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-02
read_channel: connector
evidence_class: 后继静态分析授权与验收标准；不是29执行成果
authorization_ref: Owner要求28独立验收；28-review-and-next-batch-same-turn持续约定
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
executor_display_name: MYOS2-A-C03c 代码分析执行续建
execution_model_tier: next_tier_default
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
instruction_base_short12: 0a94f32ad00e
required_review_record: CORE-USER-ELF-INTERPRETER-28-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ELF-INTERPRETER-28-review.md
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；lead=agent/MYOS2-LEAD-002
source_pin_short12: a039d9803ade
result_base_short12: 427604c818a5
master_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pathname-copy-29/
allowed_files: [MANIFEST.md, pathname-copy.md, facts-and-boundaries.yaml, pathname-copy-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 427604c818a5及此前全部文件
writer_assignment: C03c仅本批六件及本人PR17进度/交付评论；正式交付后停写
activation_gate: Owner向现有C03c发送本任务；固定输入与现场写前核对通过
old_executors_reactivated: false
external_execution_started: false
phase: 2
runtime_evidence: NOT_RUN
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_change_authorized: false
kernel_patch_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
inputs_read:
  - 28固定六件、48任务及28主线回执；18延期、28号同轮约定、45角色分离持续适用。
  - 主线预检固定time namei.c1–180、arch/x86_64/myos/utils.c全文、open.c1–285、exec.c550–620和1290–1480至末尾；不当成29已经完成。
open_questions:
  - 路径测长、分配、复制及有符号结果的真实消费须独立取证，不将函数名当Linux契约。
  - filename返回/释放和更深VFS打开、权限及硬件异常行为分别有界，不能扩成完整系统调用审计。
---

# 第29批：用户/内核路径名怎样成为filename对象，何时归还

## 1. 目标和现有对话操作

28按PASS_PENDING_LOCAL限定接收，必须合读R28-S01/S02/S03，无单独返工任务。继续现有C03c / CHAT-EXEC-03、次一档GPT-6＋Extra。本批是该执行对话第5包，不新建聊天、不重放45号接收。

本批从28的getname_kernel分配失败边界扩到与之成对的getname、__myos_getname和putname，核测长、缓冲区大小、NUL、复制返回、错误指针与所选调用者释放。覆盖用户sys_open/sys_execve与内核open_exec的代表使用；kernel_execve/filp_open只作同接口的有限对照。不重新分析ELF、参数栈、全部open flags/文件引用、路径逐分量遍历、权限、符号链接或整个uaccess系统。

Owner转发本49号后，P00–P04连续推进至自查、提交和远端回读。常规取证/内部步骤不逐项等待确认；权限、关键输入身份、不明并行写入等真实阻断才停相应动作。普通缺陷延期，缺证据标partial，不把六件齐全当语义完成。

## 2. P00：绑定输入、配置与历史限定

完整读取本任务及0a94f32ad00e中的28主线验收回执，特别是R28-S02路径长度/对象前提和S03符号/引文匹配。固定源码time a039d9803ade，结果基线427604c818a5；先核mykernel/scripts/options_flags.cmake，沿用适用项目规则、18/28/45。默认分支搜索只定位，所有源码结论回固定ref。

定点读取28::EI28-F01/F05及相关对象/调用、05::UV-04及最新主线限定；05/08的打开缺return、用户库边界等只作为已有约束，不重做原批。对28已记录的__myos_getname错误指针直接使用，追加原qualified_id证据，不因扩到另一个调用者就重复计一个独立根因。未做全仓检索时不声称完成全仓去重。

主要导航为mykernel/fs/vfs/namei.c、arch/x86_64/myos/utils.c、fs/vfs/open.c、sched/forkexec/exec.c；必要定义在filename_s、types.h、err.h、limits.h、字符串/用户访问头与实际实现。先确定选定配置下绑定的符号，勿把旧声明、注释和另一份同名接口混成实际实现。

## 3. P01：getname/getname_kernel的对象、长度和顺序

完整读__myos_getname、getname、getname_kernel、putname。核filename_s本体字段的实际类型/初始化与name字符区的分配；这两个分配是两个对象，不把同名变量/指针强转当作引用转移。记录返回正常对象、NULL、ERR_PTR等实际生产者，区分对错误指针取成员、长度拒绝、字符区分配失败和正常返回。

两入口顺序分别跟踪：测长何时发生、filename对象何时分配、长度门在什么位置、写name->len的来源、字符区请求len+16或len+1及相应溢出域、实际复制长度与NUL来源、每个错误出口归还了什么。不能以kzalloc名字或成功指针直接认证整个内存/分配器；只核必要调用契约并保留运行限制。

length必须明确是否含终止NUL、是请求长度还是实际复制数、是size_t还是long。逐次赋值处理有符号返回到无符号变量、随后len<0或len>PAGE_SIZE的实际意义；不根据变量名把负错默认保留。若检查恒假，给真实类型和语句，而不是仅凭lint习惯。超大非法输入与实际所选入口可达域分开。

对getname_kernel特有的“先分配本体再测长”分支，仅记录所选函数内部资源账；没有更深所有权证据不声称永久泄漏。与PT_INTERP比较时记住28的有效尾NUL、p_filesz<=4096限制，不把通用超长输入硬塞进该受限生产路径。

## 4. P02：直接测长/复制帮助函数与字节稳定性

核实际strlen_user、strncpy_from_user及它们依赖的strlen/strncpy和access_ok最小定义/绑定。必须用MyOS2固定函数体，不补入Linux同名接口的标准返回约定、异常表、SMAP或自动故障恢复。

分开“扫描已经读取字符”“地址范围检查”“目标复制动作”“返回字节数/错误”和“终止NUL已经保证”。明确检查是在访问前还是访问后，边界检查是否包含NUL字节；不把access_ok数值通过当成页已映射、权限正确或异常可恢复。只追当前直接包装/条件选择，不展开整个CPU异常和页表审计。

核strncpy的真实返回生产者：getname_kernel中dest!=name->name分支是否能由所选正常实现生产。没有活动非目标指针返回来源时，不虚构其为可达的独立失败，也不能从一个理论分支ERR_PTR(len)推断真实错误流程。相关Linux声明与本仓函数实现不同必须说明。

用户字符串“测长后再复制”涉及两次观察，分别记录复制前后的长度、name->len和内容。只有给定其他写者/时间条件时才能讨论字节变化；没有实测并发，不能声称已经发生竞态或稳定快照。不编写攻击样本，不运行任何真实用户指针测试。

## 5. P03：所选调用者怎样消费/释放filename

至少核两个用户入口与一个内核入口：
- fs/syscall.c中sys_open入口→do_sys_open→do_sys_openat2对getname结果的IS_ERR/PTR_ERR及putname；到do_filp_open传入filename的接口为止，不重审其深层路径逻辑。
- sys_execve→do_execve→do_execveat_common对getname结果的消费，正常/alloc_bprm失败/后续错误出口到putname的顺序。只为filename生命期定向合读alloc_bprm/free_bprm的相关名称字段，参数复制和ELF本身不重做。
- open_exec→getname_kernel→do_open_execat→putname的用户/内核边界，继承28具体PT_INTERP输入约束。

kernel_execve和filp_open可作同类型内核字符串的直接调用者对照；只核取得、判断、使用和归还，不把这一名单称为全仓所有调用者。内核字符串与用户指针的来源须回到实际调用语句，不能因const char*类型相同就混为一类。

明确filename对象、name->name、调用者借用的字符串与文件对象的身份。指向filename内部字节的字段在putname前后如何使用，区分复制字节、保存指针和真正所有权。下层调用若不返回或已有未定义返回缺口，只给有效前缀/独立局部模型，不能假设跨过缺口推导全链成功或双重释放。

形成有条件的返回/资源矩阵：空字符串、正常有终止符、PAGE_SIZE边界及含/不含NUL区别、超长、第一次/第二次分配失败、用户检查拒绝、复制结果负错/正常、getname_kernel复制返回不等于目标的可达性、调用者返回错误或继续。每行标明生产者是否已确立；真实生产路径、helper模型和未证情形分开。

## 6. P04：六件交付及自查

严格仅allowed_files。MANIFEST逐P00–P04列完成/partial；pathname-copy.md给顺序与对象账；facts给类型/长度/容量、调用者矩阵和必要纸面例；DOT分调用、数据、赋值、返回与释放尝试；deferred使用PN29-*候选和已有全限定ID；evidence记录实际固定读源与检查方法。不设问题数、引文数或报告长度指标，不加第七件或通用脚本。

每个关键结论有真实path::symbol及1–5行原样引文，空行计数；片段必须包含所声称的关键赋值/调用/比较/返回，不能只靠标题、邻接行号或正确的字节匹配。特别自查ERR_PTR与PTR_ERR、strlen与strlen_user、正常数值与错误编码、分配本体与字符区、实际写入者和数据来源。若代码片段属于另一个符号，纠正或降级，不保留错误VERIFIED标签。

允许现成临时沙箱进行文本、JSON/YAML、图ID、有限类型/算术核对；准确记录实际使用的工具，不安装、不运行仓库脚本/C/ASM、不调用外部付费模型/API。历史作者检查不冒充本轮复现，自查通过不等于语义或运行通过。仅申请READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING。

## 7. 写前、回读及停止线

继续C03c唯一执行、原PR17和claude/dazzling-cori-q0dnyt。首次写前期望427604c818a5；现场核直接master/time/执行头、开放PR和路径交集、新目录是否存在。若同名成果已正式交付，先报告状态，不重复执行；未知头变化或并行写入停写核归属，不覆盖/回退/强推。lead合法前进不是冲突，time变化记录但不静默换源。

所有旧件冻结，正式交付前仅可按当前blob修订本批自己新件。正常快进；工具支持expected-head时用它，没有则承认竞态限制，不声称全局锁。不新建竞争分支/PR，不改PR标题/正文/Draft及他人评论。写调用超时先核远端是否已成功，再补余项。

最终提交全文回读六件，再按活动分支核正文/blob/head；compare 427604c818a5到结果头须恰本目录六件A新增、旧件无净修改/删除且无未知提交。把实际短提交、回读和compare结果写本人PR17交付评论，不为在文件自引用最终SHA反复提交。正式交付后停写交C03验收，不自行开始30。中断记录只留本六件或本人PR17评论，不承诺后台继续；Owner仅转发49号入口并通知完成。

## 8. 权限不扩展

阶段2、18延期、28号同轮安排和45身份分离不变。禁止改内核/用户程序/旧成果/脚本、生成修复补丁，预处理/编译/运行C/ASM、内核/QEMU/硬件或真实ELF/用户指针样本，安装/挂盘/读取凭据、操作Owner本机或仓库外libc/镜像。无master/time/lead写入、合并关闭PR、删改名/强推或阶段3权限。C03独占规划、lead、PR导航和最终验收；旧执行者不恢复，教学/导览只读。普通问题登记延期，Owner当前无需修复决定。
