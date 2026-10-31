---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-MM-CONTEXT-15
record_type: chat_mm_ownership_and_context_static_analysis_contract
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-10-31
read_channel: connector
evidence_class: 下一工作包授权与验收条件；不是第15批分析结果
authorization_ref: Owner报告14完成；28号同轮验收及发布下一批的持续授权
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: MYOS2-A-C03a 代码分析执行
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
receiving_model_tier: next_tier_default
instruction_base_short12: 18ee22673d83
required_review_record: CORE-USER-VMA-LIFETIME-14-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-VMA-LIFETIME-14-review.md
source_branch: time
source_pin_short12: a039d9803ade
result_base_short12: dd8e81ba6fc7
default_branch: master
default_branch_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-mm-context-15/
allowed_files: [MANIFEST.md, mm-context.md, facts-and-boundaries.yaml, mm-context-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: dd8e81ba6fc7及之前全部文件，包括14和历史勘误
writer_assignment: C03a仅本批六件及自己的PR17评论单写者；正式交付后停写待C03验收
previous_executor_delegation: CC02旧委派不恢复；旧本机进程和未推送内容unknown
activation_gate: Owner向已有C03a转发任务入口，绑定及写前核对通过后A00–A04连续执行
external_execution_started: false
prior_status: vma14_PASS_PENDING_LOCAL_WITH_R14_S01_S02
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
  - 14六件全文和本轮主线回执；32号与既有18/24/25/28号规则。
  - 固定time fork.c的mm_init/mm_alloc/mmput/__mmput/mmdrop、get_task_mm/mm_release/dup_mm/copy_mm；exec.c的exec_mmap；exit.c的exit_mm注释段，作为增量设计预检。
  - 默认分支switch_mm搜索导航到scheduler_core.c及arch/x86_64/mm/mmu/；mmu_arch.c 1–180回到固定time读取，未完整核切换链。
  - 两PR、直接refs、PR16完整变更路径、开放PR第三项为空与14增量compare。
open_questions:
  - mm用户引用、结构引用、task指针和实际地址空间切换是否正确配对，仍须本批取证。
  - 不预判调度借用、切换和回收成功；宏/同步原语、硬件及全局并发不作穷尽认证。
---

# 第15批：谁持有地址空间，何时换成另一个，旧对象如何退出使用

## 1. 目标及启动

14已限定接收，R14-S01直接纠正brk字段错误出口、R14-S02补调用证据。先合读回执即可，不另造14勘误文件、不重跑107/73检查、不重新执行VMA split/unlink任务。

本批接续14的mm清理端点，以task.mm、task.active_mm、mm用户/结构引用和页表根为中心，核fork共享或复制、exec安装新mm、调度切换/借用及旧mm释放的有限连接。区分“指针赋值”“引用计数变化”“调用切换接口”“写页表根寄存器”“VMA/PTE/page实际释放”；不能合并成一次成功。

Owner向已有C03a发送：

> 执行CORE-USER-MM-CONTEXT-15。完整读取33号固定任务与14主线回执，沿用已有接收和GPT-6＋Extra。按A00–A04连续完成地址空间持有/复制、exec替换、调度切换接口与释放边界的静态分析、自查、原PR17提交和远端回读。只写指定六件，旧件保留；普通问题入库，不修内核、不重做13/14。完成后停写交C03验收，不自行开始下一批。

内部阶段不逐项等待确认；发布GitHub任务不自动唤醒聊天。遇访问/授权/输入冲突或未知并行写入才突出真正阻断，不依赖阻断的安全项继续。不能把未取到的必要链路写成已完成。

## 2. A00：绑定与最小输入

读取本任务、required_review_record全文，核三个对象：指令18ee22673d83、旧结果dd8e81ba6fc7、源码a039d9803ade。源码先读options_flags.cmake并保留外部选项/未预处理限制。18号普通问题延期、24/25号执行验收分工、28号同轮派发继续有效；不重做25号接收，不沿用14已结束的写权。

14只定点消费mmput/exec/exit端点、对象表和R14-S01/S02；13只取必要的file/cache引用界限及R13-S03。04/05/03的MM及调度旧项需要时按实际qualified_id和最新回执查，不重读全部历史大包，也不恢复旧绝对化结论。源码与原始报告冲突时回源并登记，不以旧PASS_PENDING_LOCAL压过新反证。

已导航到mykernel/sched/forkexec/{fork.c,exec.c,exit.c}、sched/scheduler/scheduler_core.c、arch/x86_64/mm/mmu/{mmu_arch.c,mmu_macro_arch.h,mmu_api_arch.h}和mm/misc/mm_misc_types.h。仅是路径入口，定义、真实字段和调用条件须回固定time核实。默认分支搜索只导航；不把本任务设计预检当执行证据。

## 3. A01：mm对象、计数和最小创建/释放端点

从实际mm_s与task_s定义列对象表：mm/active_mm是归属、借用还是另外持引用；mm_users及本树实际结构计数字段分别初始化为何值、由什么API增减。预检看到mm_refcount，不能未经核对套用Linux的mm_count名称或标准mmdrop契约。字段名、注释、函数名都不是运行证明。

定点读mm_alloc/mm_init、allocate_mm/free_mm、pgd_alloc及必要返回/释放接口；列分配mm、初始化链/计数、分配pgd、发布对象的顺序和失败清理。页表分配器仅追到明确申请/失败/释放端点，不重做04 allocator及全部pgtable内部。

为mmget、mmput、__mmput、mmdrop以及实际存在的mmgrab/结构计数帮助函数分别记录真实签名、返回值、引用变化和副作用。不存在或未找到者需给限定搜索范围，不补造Linux接口。直接free_mm/pgd_free和仅递减计数不是同一行为；__mmput的注释不得变成已执行清理。init_mm等特殊对象约束须按实际门解释。

## 4. A02：fork共享/复制、发布及失败

核copy_mm在current->mm为空、CLONE_VM和非共享复制三类分支。沿dup_mm的结构复制、mm_init、dup_mmap以及copy_process实际调用点/错误标签，判断何时把mm/active_mm赋给新task、什么时候仍只是局部对象、失败后由谁归还。仅追copy_process中与mm相关的直接清理，不重做PID、文件表或完整fork调度。

共享同一个mm与复制一个新mm必须给清楚的引用/别名关系。dup_mmap只作为14已读接口消费：必要时核VMA.vm_mm重绑、file引用和copy_page_range调用门；不扩成完整COW/PTE复制正确性审计。结构memcpy不等于全部字段得到独立初始化或所有引用已增加。

完整读失败标签及调用者如何消费错误，不在goto前停止。把分配失败、pgd失败、复制VMA/页表返回错误、新task发布前后等副作用分开；没有回滚证据就标unknown/断点，不能写“失败必然无影响”，也不能遗漏实际存在的字段恢复。

## 5. A03：exec替换、调度借用与切换接口

以exec_mmap为主，必要时补其直接调用者的传入mm来源和失败处理。分别核old_mm非空与为空的路径：exec_mm_release/mm_release、task.mm与active_mm赋值、activate_mm、旧mm的mmput或mmdrop、临界区/错误返回的实际顺序。只在需要解释引用转移时读取exe_file等直接相关字段，不重做ELF装载和全部exec。

定点读scheduler_core.c中的context_switch及其实际后置清理函数，分别区分用户任务→用户任务、用户→无mm任务、无mm任务→用户、同一mm与不同mm。实际存在的借用active_mm、结构引用增加/延后减少、rq保存旧mm等须有活动源码；注释不算已实现。无需重新分析选择哪个任务、调度排序或时间片。

沿真实activate_mm宏、switch_mm/switch_mm_irqs_off追到本树的页表根装载接口（例如load_new_mm_cr3/write_cr3），先核其是否真被当前路径调用。可以只读相关C/ASM封装文本，不执行。明确prev==next、NULL、pgd有效性等门与提前返回；指针比较相同不自动证明CPU当前硬件状态相同。仅记录源码行为，不证明TLB、PCID/ASID、跨CPU shootdown或屏障协议正确，不扩展安全缓解/全架构审计。

末端连读do_exit的exit_mm注释、__mmput的exit_mmap/mmdrop状态，以及本批所核其他活动释放调用。区分潜伏定义、活动调用、条件终止、当前路径断开；不因一个出口未调用清理就断言所有入口永不释放。

## 6. A04：状态表、证据与六件交付

仅形成allowed_files六件。mm-context.md先给持有/切换/释放三层结论；facts集中精简对象、状态/所有权、调用边和强制引文；DOT对应facts；deferred只增量登记新项/旧证据；evidence写真实方法、范围、反证和未做事项。MANIFEST真实标completed/partial，不能只以六件存在宣称必要链全部闭合。

代表矩阵至少覆盖：无current.mm、共享mm、新复制mm、创建失败、复制失败、exec有/无旧mm、同mm/异mm切换、无mm任务借用及解除、计数非零/归零、特殊init_mm。可合并同源行，不凑数量或做笛卡尔积。每行给调用前后task指针、具体计数、pgd/硬件切换接口、资源清理及前提/unknown。以实际字段为准，计数算术推导不冒称真实并发观察。

关键结论附真实路径、符号和1–5行逐字引文，无引文数量指标。全体强制引文回源；证据必须覆盖所声称的调用、条件与错误出口，不能用相邻声明/清理语句代替调用点。负向结论需完整所选函数/搜索范围，不以一行注释证明全局不存在。

call=直接调用方→被调方，帮助函数与内部间接调用分层；动态目标须有调用点、赋值绑定和目标定义，否则保留dispatch unknown。宏别名单列，未预处理限制保留。data=消费方→提供方，回调槽→目标注明是绑定；写入/计数/释放单列effects。active仅指活动源语句，不免除早退或上游前提。API返回值、内部裸指针、借用/新增引用分字段。

主动反证：mm_users归零是否必释放对象；两个task.mm相同是否对应引用增量；结构复制是否等于独立页；task指针更新是否等于执行了切换；调用write_cr3是否等于全局TLB正确；错误返回是否撤销全部已发生副作用；释放API存在是否意味着该出口调用。普通问题按18号入库，无当前Owner修复选择。

可在自己的临时沙箱使用已有文本工具检查已取得文本的YAML/图ID/引用，不安装、不执行仓库脚本或C/ASM、不操作Owner工作树。人工核查与机械检查分开，未执行即NOT_RUN；不启动新付费模型/API或Deep Research。最终只回READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING；上限PASS_PENDING_LOCAL。

## 7. 写入、保全与交回

复用PR17/claude/dazzling-cori-q0dnyt，首次写前期望dd8e81ba6fc7。C03a仅获本批六件及自己的PR17评论权；C03独占lead、PR导航和验收。禁止另建/改名分支或PR、改PR标题/正文/Draft状态或他人评论，CC02旧委派不恢复。

写前复查master直接ref、time、执行头、开放PR和路径交集。主线预检只有16/17，既有lead与本批结果前缀分离；若目录已有成果或头不明变化，先核来源/是否已完成，不覆盖/重置/重复执行。time前进时保留历史固定对象并登记新头，不静默换源；lead合法前进不算执行冲突。

六件可分次提交，以此前自己实际核实的提交为期望头；交付前只按当前blob SHA修订本批自己的新文件。旧件全部冻结，正式交付后本批亦冻结。Contents接口不提供全局原子分支锁，保留竞态限制；写调用超时先读远端确认，不盲重试。可信进度保存在本批六件适当位置或自己的评论，注明PARTIAL与下一安全动作，不另造第七件，不声称回合结束后后台继续。

最终按固定提交完整读回六件，再按分支核blob/头；compare dd8e81ba6fc7至结果仅允许本批六路径A新增、无旧件净修改/删除或未知提交。后置核验记PR17交付评论，给实际短提交、六件入口、diff/回读结果与NOT_RUN，不循环改文件自引用。交付后停写待C03；Owner只通知完成，主线从GitHub读结果并依28号同轮安排后继。

## 8. 既有禁止动作

不修内核/用户程序/旧脚本/旧成果，不生成内核补丁，不预处理/编译/运行C/ASM/内核/QEMU，不安装、挂盘、读凭据或仓库外libc/镜像，不直写master/time/lead，不合并关闭PR、删除分支、force-push或进入阶段3。用户的评估导览/架构学习对话仍只读，不因此获得执行或主线写权。
