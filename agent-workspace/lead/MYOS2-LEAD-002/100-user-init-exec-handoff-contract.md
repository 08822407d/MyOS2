---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报，非后台认证）
model_per_owner: gpt pro（沿用Owner此前明确选择）
backend_identity: unknown_or_not_attestable
date: '2026-11-10'
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
publication_status: PREPARED_FOR_FIXED_COMMIT_POSTWRITE_RECEIPT_REQUIRED
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
runtime_evidence: NOT_RUN
open_questions: []
followup_id: CORE-USER-INIT-EXEC-HANDOFF-69
record_type: bounded_initial_exec_and_register_handoff_contract
evidence_class: Owner持续授权下的任务规划；非执行成果或运行认证
result_base_short12: 4d45fa939fa1
lead_before_publication_short12: 09aa0ff5dc48
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
executor_display_name: MYOS2-A-C05b 代码分析执行
executor_rotation: false
executor_model_per_owner: 次一档 GPT-6＋Extra
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-exec-handoff-69/
allowed_files:
- MANIFEST.md
- init-exec-handoff.md
- facts-and-boundaries.yaml
- init-exec-handoff-paths.dot
- deferred-findings.yaml
- evidence.md
prior_return_items: []
activation_gate: C05正式发布本固定100号和两份回执并完成回读；Owner向现有C05b转发真实固定入口要求执行，且G00通过。
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
inputs_read:
- agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md
- agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md
- agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-kthread-entry-68/kthread-entry.md
- mykernel/arch/x86_64/myos/arch_task.c
- mykernel/sched/forkexec/exec.c
- mykernel/arch/x86_64/sched/context/process64_arch.c
- mykernel/arch/x86_64/entry/entry_64.S
---

# 第69批：首次exec桥接、返回条件与寄存器帧消费

**执行面为现有C05b/CHAT-EXEC-08，次一档GPT-6＋Extra；不新开对话，无D00返工。** Owner转发本文件的已回读固定提交入口并要求执行、G00通过后，连续完成G01–G04。原未发布备份不再作为启动依据，不恢复旧C05a/07或C04c/06。

## 1. 技术增量

68区分了首帧入口和业务回调，并将kernel_init的跳转桥留为KT68-U07。69合并三个新专题：A桥接前的任务/地址空间/帧身份；B exec返回值和失败出口与桥接消费者；C成功帧字段的生产与SYSRET前的软件消费。无D00，不重复67/68整包、66栈/TSS链、09全部信号返回或23–28全部ELF工作。
不把调用kernel_execve、它返回非负、帧被写入、跳到汇编标签、CPU接受状态和用户代码运行混为一步。普通发现按18号入库，不要求Owner立即修复，也不自行实施修复。

## 2. G00、规则与固定输入

完整读取本100号、同发布提交的reviews/CORE-USER-FIRST-RUN-FRAME-67-DOC-CLOSURE-review.md、reviews/CORE-USER-KTHREAD-ENTRY-68-review.md及checkpoints/2026-11-10-kthread68-reviewed-init-exec69-ready.md。全文刷新63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md及协议v2 P1–P13。其它项目规则和适用Mnemosyne指导只能复用本有效C05b实际读过的原文；不继承别的对话已读声明，不导入跨仓维护。
按同次正式回执消费：R67-01文档闭合，67/68均PASS_PENDING_LOCAL，R68-S01/S02/S03必须合读；既有66/65/64闭合、63/58误判撤回保持。
核PR16/17、direct time/master/lead/exec、开放PR和新69目录；首次写前预期执行头4d45fa939fa1，固定内核time@a039d9803ade。未知exec变化或同名成果出现须核归属，不覆盖。重新读取公约原句与time/master双金丝雀，记录真实值。
68固定结果从4d45fa939fa1读取主件及facts/deferred中KT68-U07/U06、FR67关联限定。相关历史：同固定主线reviews/CORE-USER-RETURN-SIGNAL-09-review.md及reviews/CORE-USER-RETURN-SIGNAL-09-DOC-FIX-01-review.md用于返回/信号界面。若采用25 ELF栈表或28解释器旧结论，先在同固定reviews目录定位相应原回执，再按所消费的入口/栈限制定点读取；未取得原条目则只记缺口，不阻塞独立新取证，不重验旧批次。采用任何旧qualified_id前须回原条目及真实状态，不从第一波摘要恢复结论。

## 3. G01/A：初始exec桥的任务与帧身份

固定核心为mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve全文，以及mykernel/init/main.c::kernel_init的直接调用与后继循环段。明确curr/current、curr_ptregs/task_pt_regs、PF_KTHREAD、mm/active_mm、argv/envp和hwtss_cpu0各是什么对象及何时赋值。
不能把curr->mm=curr->active_mm当作已完成地址空间所有权/引用交接；不能把hwtss_cpu0指针赋值当作写TSS.sp0。仅追为解释本地操作所必要的真实直接定义；全MM/GS/TSS启动和生命周期仍是边界。
记录curr_ptregs是在exec前求值还是之后、所指task/stack是否有可核保持前提、实际汇编输入操作数和跳转目标。区分源代码改RSP/无条件jmp意图与编译器可见的控制流、clobber、普通函数返回约定；不凭volatile或注释签ABI正确，也不运行编译器来替代取证。

## 4. G02/B：kernel_execve的返回门及桥接消费

完整读mykernel/sched/forkexec/exec.c::kernel_execve、bprm_execve、search_binary_handler；alloc_bprm/free_bprm及getname/计数/复制helper只追必要直接返回接口，不重做字符串/路径/文件生命周期专题。对PF_KTHREAD拒绝、参数/名称/分配/复制失败、binary handler负值/非负值、清理正常返回或不返回分别建表。
把每个返回值接回kjmp_to_doexecve：是否有检查、失败时哪些前置字段已经改动、之后是否仍发出切RSP/跳转请求。没有显式错误检查不等于一定发生了失败；清理或前置调用出错时，不越过最早停止点推演最后指令。
search_binary_handler的循环与被注释的提前返回门须按本仓库文字分析；“任意一次loader成功”“最后一个retval非负”“寄存器帧已经由目标loader完整准备”不是同一命题。只核当前分派和返回契约，不扩审所有binary format或整个ELF装载。
普通用户sys_execve路径仅用于区分直接调用者，不展开另一套完整系统调用分析。涉及旧23–28/09根因，保留原ID与状态，不以桥接消费补证重复登记漏洞。

## 5. G03/C：帧字段生产到SYSRET软件消费

固定mykernel/arch/x86_64/sched/context/process64_arch.c完整start_thread/start_thread_common，mykernel/arch/x86_64/entry/entry_64.S完整syscall_return_via_sysret及所用POP_REGS宏，必要ptrace/生成偏移定义。现存偏移不等于重新生成的ABI。
通过固定mykernel/fs/vfs/binfmt_elf.c定位load_elf_binary的最终START_THREAD调用和最近成功/错误出口；该路径来自正规搜索定位，必须在time固定对象实际读取后才能作事实。START_THREAD实际宏选择从真实include链确认，不能用默认分支搜索片段替代固定源码。只核入口IP/用户SP及regs指针的直接来源窗口，不重做段映射、解释器或栈表全链。
建立ip/sp/flags/cs/ss以及保存的RCX/R11、原syscall号/向量字段到活跃寄存器的关系。按原样xchg顺序、POP_REGS、addq与popq推导条件性软件栈变化；前提须包括对象/偏移、正确GS/入口模式和前置调用。不要用保存帧cs/ss数值直接替代CPU由其它状态形成的选择子规则。
明确初始桥直接跳到syscall_return_via_sysret，与正常ret_from_fork/syscall_exit_to_user_mode路径是否在已读直接控制段等价；有省略的调用只按实际源界面记录，不据此宣称全局信号一定遗漏。CPU指令语义如需外部结论，依协议P5引用具体Intel/AMD原始手册版本/章节，否则保留明确假设与缺口；不执行硬件模拟。

## 6. 六件、证据门及预算

严格六件：MANIFEST.md、init-exec-handoff.md、facts-and-boundaries.yaml、init-exec-handoff-paths.dot、deferred-findings.yaml、evidence.md。facts至少包括source_ledger、bridge_state_matrix、exec_return_consumption_matrix、register_handoff_matrix、finite_model_cases、prior_findings_links。无D00返工；新候选IE69-Fxx、新缺口IE69-Uxx，旧根因不得重复编号为新缺陷。
强制锚：A桥中flags/mm/帧计算与完整asm操作数；B关键返回/错误门及桥如何消费；C START_THREAD实际调用/定义、字段写入及汇编交换/恢复。所有源码锚必须非空、单一定义内原样连续1–5物理行；图分真实CALL、宏展开、条件后继、TAIL_JUMP、DATA、ANALYSIS_STOP。
核心与直接依赖软上限14源码文件，历史/规则另计；最多8组定点检索与4个相关目录，超限说明必要性，不全仓扫描。没有引文、模型、缺陷数量配额。缺少外围证据时登记边界，不编造实现也不截断强制定义来凑预算。
MANIFEST含read_channel、公约原句、双金丝雀、署名/后台未知、self_check三数、P9逐项实际结果、write_retry_log。P6须检查新六件原始全文而非只数evidence标题，非证据说明不用保留前缀；每个引用ID既存在又支持命题。无代码/补丁/降级时P9-9/10/12 N/A；完整本地P9仍NOT_RUN，自建纯数学/文本检查与仓库执行分开。

## 7. G04及停止门

只有C05已正式发布真实固定提交、Owner向现有C05b转发并要求执行、G00通过后，才连续G01–G04。仅使用主线实际回读的新提交URL，不使用旧99号或猜测的100号地址。
唯一写区为原PR17新user-init-exec-handoff-69/授权六件及08本人唯一正式交付评论；旧全部文件、旧评论、PR元数据、lead/PR16、master/time冻结；C05不代写执行成果。没有新分支/PR、强推、合并关闭、内核改动、预处理/编译/链接、仓库脚本/旧夹具/QEMU/实机、阶段3或额外付费API/子代理许可。
暂态写入或动作暂缺先核远端效果，再按63号通常首次加三次、有间隔恢复；遵守Retry-After、expected-head/force=false，未知并行写入不覆盖。记录工具发现次数与实际写请求数，不能把没有动作说成已发出失败请求。
发布后compare只六件A新增、旧件零净改删；固定提交及活动分支六件全文/blob、HEAD与正式评论均回读。交付只报READY_FOR_MAINLINE_REVIEW/mainline_acceptance=PENDING，完成STOP_WRITING，交C05独立验收，不自签69通过或自行开始70。
