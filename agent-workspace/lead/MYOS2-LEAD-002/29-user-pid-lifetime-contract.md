---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PID-LIFETIME-11
record_type: chat_pid_identity_lifetime_static_analysis_contract
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线模型选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-10-31
read_channel: connector
evidence_class: 执行任务与验收条件设计；不是第11批结果
authorization_ref: Owner本轮要求验收10并从本次起验收后同轮给出下一工作；28号与既有阶段2权限
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: MYOS2-A-C03a 代码分析执行
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
receiving_model_tier: next_tier_default
instruction_base_short12: 69523af0507e
required_review_record: CORE-USER-SIGNAL-SEND-WAKE-10-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SIGNAL-SEND-WAKE-10-review.md
same_turn_rule_ref: agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md
source_branch: time
source_pin_short12: a039d9803ade
result_base_short12: 848e2acdf7e1
default_branch: master
default_branch_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pid-lifetime-11/
allowed_files: [MANIFEST.md, signal10-graph-disposition.md, pid-lifetime.md, facts-and-boundaries.yaml, pid-lifecycle.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 848e2acdf7e1中的全部文件，包括10及08/09/勘误
writer_assignment: C03a仅为本工作包七件及自己的PR17评论单写者；交付后停写待C03验收
previous_executor_delegation: CC02旧委派不恢复；旧本机进程和未推送内容unknown
activation_gate: Owner将本固定任务入口发给已有C03a；绑定核对通过后P00至P04连续推进
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
inputs_read:
  - 第10批六件、最新交付评论和本轮主线回执；27号任务与18/24/25/28号规则。
  - 固定time的PID查找/绑定/分配/释放函数、sched/syscall.c、forkexec/exit.c第1-220行、copy_signal局部及start_kernel初始化局部；仅任务设计预检。
  - 05的MANIFEST及主说明C2和前部能力范围，用于避开既有mm/VFS资源分析，不继承未核全局断言。
open_questions:
  - PID分配、发布、引用和退出摘除的实际连接需要执行方本批取证，不能把设计预检当成完成。
  - SR10-01仍待P00处置及C03再核；普通内核问题继续延期。
---

# 第11批：PID何时可查，查到的对象能存活多久

## 1. 一个完整工作包

第10批已建立信号发送到目标指针的静态链，但目标PID与任务对象的发布、引用和退出后可见性仍未闭合。05已有fork/exit资源概览，本批只增量展开身份和生命周期边界，不重做mm、VFS、信号建帧、调度策略或全PID命名空间研究。

先处理第10批的一项小型图/引文关联修正SR10-01，再继续独立的PID取证。P00完成后不为小修单独停一轮；有证据异议保留并继续不依赖争议图的工作，以实际源码为准。主线最终裁定仍由C03承担。

Owner启动句：

> 执行CORE-USER-PID-LIFETIME-11。完整读取29号固定任务及10主线回执，沿用已有接收和GPT-6＋Extra。先完成P00的SR10-01定点图/证据处置，再同轮完成P01–P04的PID创建发布、查询引用和退出摘除静态分析、自查与原PR17回传。只写本批七件，旧件冻结；普通问题入库，不修内核、不重做08/09/10。完成后停写交C03验收。

这是工作包内连续执行，不是逐项审批；无需重做25号接收、新建对话或等待另一个启动仪式。任务发布不自动唤醒聊天。

## 2. P00：绑定与SR10-01追加处置

读取本任务全文、本轮10回执全文及28号；18号普通问题延期、24/25号角色和证据规则继续有效。核对required_review_record、result_base、time、PR17和自己的身份。新任务重新授予七件写区，不沿用已经结束的10写权。指令在lead、结果在执行分支、源码在固定time，不混用master上的同名文件。

旧结果固定848e2acdf7e1，基目录agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。只读10的facts/DOT与回执指出位置，必要正文用于语境；05只读C2/C6、对应能力及UV-14/UV-15/相关缺口，07/08/09仅在需要其既有接口ID时定点读取。旧检查器和大型查询库不重跑。

新增signal10-graph-disposition.md。对回执SR10-01逐项给原位置、接受/部分接受/有证据异议、实际依据及替代内容：E01补真正调用A03b；E12区分A18分支条件与A19调用；E16补A35宏绑定；E21显式恢复dequeue_signal→__dequeue_signal→collect_signal及sig!=0条件，不能把省略层次写成直接call；E24及头部统一data依赖方向，写入副作用放明确effects/说明而不是自造方向例外。

提供可直接消费的局部结构化记录和DOT替代片段或同等明确的替代说明；新增中间节点/边须给本批限定ID和supersedes/target映射，旧SW10编号不改、不覆写。对原26条边做一次有界的同类检查：真实调用、宏展开、经中间函数路径、数据依赖和副作用分别标明；检查范围不是全部内核调用图。引用文字命中与确实支持该边分开核验。无需复制原六文件或重建通用验证器。

该处置只解决文档问题，不恢复complete_signal、修override_rlimit或改内核。C03a的accept只是接受修正要求，不等于主线已关闭SR10-01。

## 3. P01：PID对象创建、分配与发布

先读mykernel/scripts/options_flags.cmake。以实际sys_fork→kernel_clone→copy_process为普通用户入口，只展开PID/任务身份部分；内部其他flags分支与用户入口实参分开，不把内部接口所有配置当用户可达。

定位alloc_pid、PID缓存/namespace初始化、task_pid_ptr、attach_pid以及copy_process中的thread_pid、signal->pids、pid_links、pid/tgid赋值和返回用户编号。入口路径已定位在mykernel/sched/syscall.c、sched/forkexec/fork.c、sched/task/pid.c及直接pid头中；其他定义沿实际include/调用在固定对象定位，不照Linux目录猜。

重点区分三个时点：IDR占号/空指针预留、idr_replace发布pid_s、任务链关联使pid_task能够返回task_s。源码若确有不同阶段，分别给证据，不把“分配出数字”“find_pid_ns非空”“pid_task非空”“任务可运行”合成一个动作。初始静态PID、普通fork、内部共享/线程条件分开；已有05资源复制结论只引用，不重新全面分析。

对失败阶段画有限状态表：分配前、占部分号后、pid对象发布前后、任务链接前后，以及copy_process失败清理。标明实际调用的回退、引用变化、删除/释放对象和未覆盖的返回；源码可能读未初始化值或走无定义路径时，后续仅作有前提说明。IDR/slab/原子原语作为依赖端点，仅检查支撑本批断言必需的直接定义，不认证全树分配器或计数原语正确性。

## 4. P02：查找、namespace和引用所有权

沿10的find_vpid→find_pid_ns→pid_task接口，增量核task_active_pid_ns、ns_of_pid、task_pid、task_tgid_vnr、pid_nr_ns/pid_vnr及直接消费者。区分调用者当前namespace、目标对象中numbers/level，以及PIDTYPE_PID/TGID/PGID/SID对不同字段/链的选择；只追本批实际使用的类型与条件，不宣称完整namespace隔离实现。

给出接口所有权表：输入类型、成功/失败表示、返回对象、是否增加pid引用、是否增加task引用、锁/RCU要求是否实际满足、对应释放及未知。至少比较find_vpid、pid_task、get_pid/put_pid、get_task_pid、find_get_pid、get_pid_task、find_get_task_by_vpid在固定源中的实际行为；名字含get不等于获得task生存保证。ERR_PTR、NULL和有效指针分别对待，不只看调用者是否写了非空判断。

把结果接回10：kill保留的是哪类指针、查找和访问shared_pending之间有无实际引用/锁保护、哪些事实只能表示“尚未证明生命周期安全”。不能从缺少一个锁推断一定已经发生UAF，也不能从未观察到回收推断指针永远安全。getpid/getppid只作编号来源的必要交叉核对。

## 5. P03：退出、摘链与可查询性边界

定点检查mykernel/sched/forkexec/exit.c中的do_exit/exit_notify/do_group_exit及do_task_dead直接边界，结合sched/syscall.c的exit/exit_group/wait4实际入口。已有05::UV-15和退出资源缺口作为旧条目，不重复登记为新根因。

分别追证：停止运行/状态写入；父子链调整/父任务通知；task与pid_links摘除；IDR删除；pid引用归零和内存释放。detach_pid、__change_pid、free_pid、put_pid或release_task等函数的定义存在不等于从当前exit路径调用；若连接缺失，给实际函数范围和注释/调用证据，标为断点或潜伏接口，不画成活动退出链。

核对exit_notify传给PID查询函数的参数类型/namespace来源，以及group_dead等局部参数是否具有已赋值来源；这只是必须检查的问题，不预填缺陷结论。遇未定义前提、声明noreturn而实际正文未闭合、空wait4等情况，不套用Linux僵尸/回收语义，不给固定errno或父进程成功等待的结论。

形成状态矩阵：仅占号、pid对象已发布但未挂任务、正常关联/可查询、创建失败清理、退出处理中、走到任务死亡边界、假设完成摘除/最终释放。对“退出后kill还会查到什么”“PID是否可能重用”的回答必须带具体可达路径和生命周期前提；无法证明就unknown，不模拟运行或触发真实信号。

当前活动入口与定义中潜伏的完整机制分层。旧03调度、04内存、05资源回收、09/10信号只用必要端点，不扩成全并发证明、namespace全实现、FPU/ISA或外部libc研究。普通问题登记后继续不依赖它的安全分析。

## 6. P04：七件交付与验收

只在allowed_write_prefix内形成本任务列明七件：MANIFEST.md列P00–P04实际完成和限制；signal10-graph-disposition.md处置唯一退回；pid-lifetime.md给连续可读说明与阶段/引用矩阵；facts-and-boundaries.yaml保存精简能力、状态、边与锚点；pid-lifecycle.dot与其关系一致；deferred-findings.yaml只登记新增/旧项证据追加；evidence.md记录实际方法和未运行项。

采用本批限定ID与旧qualified_id引用，旧条目不重编号。call只表示真实调用，宏展开给调用和绑定证据，经中间函数的路径必须显式展开或另作路径说明；data仍为消费方→提供方，init_order为后可用者→前置；写入效果独立描述。不能只比较ID集合就宣布图语义正确。每条关键源码事实给真实路径、符号、1–5行逐字引文；所有本批强制引文回源核对并检查上下文、注释与配置。

自查主动检验：预留号是否等于对象可见；对象可见是否等于已有task关联；find/get是否真的持引用；exit是否真的移除PID；释放函数存在是否等于当前路径执行；未初始化或无return能否支持固定结果。否定结论必须限定实际文件/关键词/活动代码判定和截断范围，不能冒称全树grep。

允许在自己的临时沙箱使用已有文本工具处理已取得固定文本、检查YAML/JSON、引用/图/ID；只记真正运行的方法，不安装工具、不执行仓库脚本、不克隆或触碰Owner工作树。人工核对、机械文本检查、条件推断、实际运行分层。无需Deep Research、额外模型/API或大规模子代理。技术细节进文件，Owner回复只给结果入口、状态及真正阻断。

主线验收检查P00逐项处理、PID三时点与所有权表、失败/退出路径的实际连接、图与源码一致、旧问题未越权改判、七件齐全及写区。许可范围外或全局问题保持unknown不阻止有界交付；关键必读缺失须PARTIAL，不能只列TODO就报完成。执行者仅回传READY_FOR_MAINLINE_REVIEW，不签主线PASS或关闭10退回。

## 7. 唯一写区、保存与回传

复用PR17/claude/dazzling-cori-q0dnyt，首次写前执行头应为848e2acdf7e1。C03a仅写本批七件和自己的进度/交付评论；C03继续独占lead及PR导航。不得改PR标题、正文、Draft状态或他人评论，不新建/重命名分支或PR，CC02旧委派不恢复。

写前复查default/master、time、执行头、所有可见开放PR及路径。主线本轮已核仅16/17开放；主线同9af对象的既有路径与10新增六件都和本批前缀不交集。执行方仍要复查。time变化时保留本固定对象作历史分析并单列新头，不能悄悄换基线；未知执行头变化或已存在本任务结果先查来源，不覆盖、不重置、不得重复派同名工作。

七件允许分次顺序提交；以后一次自己已核提交为期望头，交付前可按当前blob SHA修订自己本批文件。848e2acdf7e1全部旧件冻结，正式交付后本批原件也冻结。工具无原子分支头条件时如实记竞态限制，不宣称锁库。调用超时先读远端查成功与否，不盲目重试。

写后按返回固定提交完整读回七件，再按分支核blob和头；compare基线到结果必须只含这七路径A新增，无旧件修改/删除和不明额外提交。后置验证写自己的PR17评论，给工具复制的短提交和短号链接，避免自引用循环修改。真实进度需要保全时用这七件中的适当文件/自己的评论记录PARTIAL、已读/未读和下一动作，不造第八件或新的验证框架。

交付后停写，Owner只需向C03通知完成；C03按28号在同一验收回合发布后继工作，不要求Owner再额外请求派发。该流程不等于两个聊天后台自动互相唤醒。

## 8. 不变的禁止动作

不修内核/用户程序/旧脚本/旧成果，不生成内核补丁、不预处理/编译/运行C/ASM/内核/QEMU，不安装工具、挂盘、读凭据或仓库外libc/镜像，不直写master/time/lead，不合并/关闭PR、删除分支、force-push或进入阶段3。普通缺陷与策略选择仍按18号延期，不向Owner索取修复选择。
