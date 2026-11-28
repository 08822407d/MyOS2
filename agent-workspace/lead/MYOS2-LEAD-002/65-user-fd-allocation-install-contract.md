执行模型：次一档，沿用现有C04a的GPT-6＋Extra；不新建执行对话、不上传附件。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FD-ALLOCATION-INSTALL-40
record_type: bounded_fd_allocation_installation_with_prior_documentation_correction
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-05
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（主线沿用Owner报告）
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
read_channel: connector
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
mainline_owner: MYOS2-A-C04
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
executor_display_name: MYOS2-A-C04a 代码分析执行
source_ref: time
source_pin_short12: a039d9803ade
result_base_short12: 4ea455865a47
lead_before_publication_short12: 08d383bb2b2c
fixed_contract_commit: CONTAINING_COMMIT_PINNED_BY_STARTUP_URL
required_review: CORE-USER-OPEN-COMPLETION-39-REVIEW-001
required_consumption_notes: [R39-S01, R39-S02]
required_write_guidance: 63-github-write-retry-and-readback.md
prior_acceptance: RETURN
prior_return_items: [R39-01]
prior_return_kind: DOCUMENTATION_ONLY
prior_closure_authority: MYOS2-A-C04
prior_return_may_remain_open_during_independent_fd_analysis: true
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-fd-allocation-install-40/
allowed_files: [MANIFEST.md, fd-allocation-install.md, facts-and-boundaries.yaml, fd-allocation-install-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 4ea455865a47及此前全部文件
executor_rotation: false
external_execution_started: false
activation_gate: Owner转发本65号给原C04a且F00预检通过
phase: 2
runtime_evidence: NOT_RUN
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_change_authorized: false
kernel_patch_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
phase3_authorized: false
owner_decisions_requested_now: []
authorization_ref: Owner第39批验收指令；18/28号持续授权与63号写入恢复要求
---

# 第40批：先追加39证据勘误，再核fd查找、预留、安装及失败消费

## 1. 两条线与范围

第39批RETURN仅R39-01文档项，核心观察按回执限定保留；执行者不能自行改为PASS或CLOSED。
F00先追加定点纠正，F01–F03独立研究get_unused_fd_flags/alloc_fd/fd_install/put_unused_fd及do_sys_openat2局部消费者，F04一起交付。
F00完成后不再等待一条Owner指令；新分析不依赖错误A069，不要求再做39整批。

目标是区分“查到空闲整数”“在共享表中预留”“安装file指针”“放弃未使用槽位”四种操作，
核它们是否真实执行、谁产生返回值、谁改变状态，以及错误路径上能证明什么。
第39批UV-04和第38批根mapping/子inode包含关系缺口仍保留；不得假设完整open已成功后推出真实fd结果。
可用独立有效file/helper模型，但必须标明非实际用户路径可达证明。

不扩完整fd生命周期、dup/close/exec继承、fcntl、全局锁/内存模型、调度、权限、分配器或页缓存。
不修内核、不出补丁、不预处理/编译运行；普通源码问题入延期账。

## 2. F00：固定输入、规则、预检与39定点勘误

完整读本65号、同固定提交reviews/CORE-USER-OPEN-COMPLETION-39-review.md及63-github-write-retry-and-readback.md。
必须合读R39-01关闭条件和R39-S01/S02。原39成果固定4ea455865a47；64号及38回执固定08d383bb2b2c。
定点读39正文/返回矩阵/相关evidence，不把69/69原总括作证据。05::UV-04、30-DOC-FIX、R13-S01及38边界按已加载限定继承。
为避免重复计数，定点查询既有05/08涉及fd的条目及主线回执，记录实际qualified_id；找不到不能编造历史编号或声称全仓不存在。

沿用现有C04a的MyOS2工作令/公约/协议v2 P1–P13、18/24/28及适用Mnemosyne指导；当轮新读与继承分栏。
固定time a039d9803ade，先核options_flags.cmake及panic.c金丝雀，再核master/time/lead/执行HEAD、PR16/17、开放PR、写区交集、本人39评论6648914733及新目录缺席。
首次执行写前预期4ea455865a47；主线正常推进不算执行冲突，未知执行后继或同名文件先核归属，不能换源或覆盖。

在新fd-allocation-install.md首部设“第39批追加勘误”专节，原六件与旧交付评论全部保留。
逐项映射R39-01，准确替代A069为真实ENFILE单宏，不再以EXDEV片段证明ENFILE。
对旧69标签说明“1项错配待替代、其余68项历史保留不等于本轮重新验证”；不强制制作69行复制清单或重做全量源码分析。
给39 MANIFEST/evidence/正文和旧交付评论总括的追加消费说明，区分字节匹配与符号身份核查。
按R39-S01补明__FMODE_NONOTIFY桥接及进制表述；其引用应是实际单宏，不把导出、声明或条件编译门当定义。
新evidence分R39C-A纠正来源与FD40-A新fd来源，新核对数按实际分别统计。
MANIFEST、facts.prior_return_items及交付评论均记CORRECTION_SUBMITTED_PENDING_MAINLINE、closure=PENDING_C04_REVIEW。
不自行关闭39 RETURN；本主线补充已定义的其它消费条件不要求再重画整张39图或新增第七件。

## 3. F01：查找空闲fd的生产者、范围与共享状态

完整读固定mykernel/fs/vfs/file.c的get_unused_fd_flags与alloc_fd活动体，及mykernel/include/linux/kernel/fdtable.h的files_struct_s、NR_OPEN_DEFAULT/NR_OPEN_MAX真实定义。
核current->files、fd_count、fd_array固定容量、start/end/flags类型及传参转换、循环变量、锁范围、命中赋值、未命中返回。
逐语句判断查找时是否写槽位/位图/预留标记或仅返回整数；注释“mark it busy”不能代替活动代码。

对零范围、至少一个空槽、全部非空、边界最后槽等给有限模型；界外end/类型转换只到最早不合法访问或无可用返回处。
若本地变量在某路径没有初始化，不补出确定负errno或随机整数，也不能默认满表返回-EMFILE/-ENFILE。
选定数组容量与合法索引、fd_count值来源分别列条件，不把静态init_files.fd_count初值自动推广为所有任务实际值。
仅为确认字段生产者定点读取init_files及必要的一层初始化/复制调用，禁止重做完整fork/exec/close轨道。

flags是否被所读活动体消费、O_CLOEXEC有无真实存储位置只按本地证据陈述；不由函数名补上Linux标准实现。
只需定点读所用宏/结构及必要helper，不递归完整位图或锁算法。

## 4. F02：安装、空槽门、预留撤销与对象引用

完整读fd_install与put_unused_fd，列实际函数返回类型、指针/下标访问、BUG_ON的条件及最终赋值。
先区分实际检查与为了推演要求的合法fd、files、数组、file指针、先前空槽前提。
无索引门时不把BUG_ON(fps[fd]!=NULL)误称为先做边界检查；宏的后果只核必要定义，不执行或推断实际机器停机。

区分smp_rmb、锁和状态占用；不能由一个内存屏障推出互斥、预留或所有权完整。
可以给同一有效files表上两个先查找、后安装的抽象交错，明确共享对象与调用条件；不认证真实并发漏洞已发生。
若两次选择之间槽状态未改变，解释这一状态事实；不得在未证实际共享/调度路径时升格为实机竞态。

put_unused_fd的注释与实际写入分开；有lock/unlock不等于已归还一个槽位。
fd_install的file引用注释不等于函数真的增减f_count；只核本地写入和必要调用，不签完整引用配平或物理回收。
不要重做dup_fd、fget、close/exit全链；发现相关边界仅登记后继条件。

## 5. F03：回接do_sys_openat2，建立状态/错误矩阵

定点回读open.c::do_sys_openat2：先build_open_flags/getname，再fd选取，随后do_filp_open，最后IS_ERR/PTR_ERR或fd_install及putname/return。
保留OF30-F01的校验结果被覆盖和39缺return；不能从这些缺口之后补出确定fd、成功安装或errno。

分别研究可证的getname早退、有效负fd返回模型、已知编码错误file、合法普通file的独立模型。
“fd>=0且!IS_ERR(f)”是实际门，“f是有效普通对象”是额外前提；NULL通过IS_ERR不等于有真实NULL生产者。
原fd选取值与后来被PTR_ERR覆盖的局部fd变量分开，跟踪失败后表状态。
若未调用put_unused_fd，必须先证明此前究竟有没有占用/预留，不能仅照搬Linux契约宣布泄漏；同理不能因没预留就宣布全局资源必定无泄漏。

形成“输入/范围→查找结果→共享表变化→锁释放→file生产→安装/错误→最终槽/整数/引用→最早停止点”矩阵。
至少含选择与占用的差别、范围为空/满表、有效索引的安装、原槽非空、正常错误回传及两个选择的条件交错。
有限数值模型不要运行仓库代码；源码事实、条件推导、helper-only、未知运行均分栏。
普通新候选用FD40-Fxx，不设数量配额，不把旧UV/OF/OC/SB条目重新计作新缺陷或自行CLOSED。

## 6. F04：六件、自查与分别交付

只新增allowed_files六件：正文先39勘误再40分析；facts独立prior_return_items和fd状态矩阵；DOT只画40，39补充用正文替代消费说明；evidence两个来源分区。
每个VERIFIED标签对应真实固定源的一个函数/结构/对象表/单宏，1–5行逐字摘录，符号与引文实际定义必须相符。
符号身份、字节/行匹配、片段能证明的结论分别核；完整函数支撑否定断言，重复片段不算独立事实。

MANIFEST保留produced_by、product_surface、model_per_owner、read_channel、base_snapshot、输入新读/继承分栏、金丝雀、公约句、自查三数和write_retry_log。
分别检查文本/结构解析、DOT端点/计数/关键语义与原字节身份；未执行标NOT_RUN/unchecked，不把转录比对当远端原字节下载认证。
只许现有临时环境的文本/有限整数模型，不安装，不运行仓库C/ASM/脚本、预处理/编译/QEMU/硬件或额外付费模型/API。

## 7. GitHub重试、写区及停止线

完整执行63号：暂态异常不因一次失败直接停工；先回读确认是否已写成，再按默认首次加三次、约10/20/40秒或穿插安全只读工作间隔重试，遵守Retry-After。
写动作暂时缺席也多轮重新发现，不推断Owner没有仓库权限；发现次数与真实写请求分开记录。
保持expected-head、force=false；已有blob/tree/commit复用，未知后继不覆盖，评论响应不明先查重，真实认证/安全限制不绕过。
多轮仍不可用才准确报告受影响步骤并保全安全成果；不要求Owner手工搬运长文件代替正常固定入口。

CHAT-EXEC-04只向原PR17分支user-fd-allocation-install-40/新增六件及本人评论；不写lead/PR16/master/time，不改旧件/PR标题/body/Draft/他人评论，不建分支PR。
最终固定新提交和活动分支分别全文回读六件、核正文/blob/head；compare 4ea455865a47到新头仅本六件A新增、旧件零净改删。
本人评论分别申报“R39-01纠正已提交、PENDING_C04_REVIEW”和“40分析READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING”，随后STOP_WRITING交C04，不自行开始41。
发布不等于自动投递或启动；Owner转发并通过预检后F00–F04连续执行，不为普通步骤重复等待确认。
仍处阶段2；不改内核/用户程序、不出修复补丁、不构建运行、不访问Owner本机/仓库外libc/镜像/凭据、不合并关闭/强推删分支/跨仓维护或进入阶段3。
