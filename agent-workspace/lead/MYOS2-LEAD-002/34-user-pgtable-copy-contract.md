---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PGTABLE-COPY-16
record_type: chat_fork_pgtable_copy_static_analysis_contract
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-10-31
read_channel: connector
evidence_class: 下一工作包授权及验收条件，不是第16批已完成的分析
authorization_ref: Owner报告15完成；28号验收后同轮派发持续授权
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: MYOS2-A-C03a 代码分析执行
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
receiving_model_tier: next_tier_default
instruction_base_short12: f791212e5bf9
required_review_record: CORE-USER-MM-CONTEXT-15-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MM-CONTEXT-15-review.md
source_branch: time
source_pin_short12: a039d9803ade
result_base_short12: 6ea1003c2ae1
default_branch: master
default_branch_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-pgtable-copy-16/
allowed_files: [MANIFEST.md, pgtable-copy.md, facts-and-boundaries.yaml, pgtable-copy-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 6ea1003c2ae1及之前全部文件，包括15及历史勘误
writer_assignment: C03a仅本批六件及自己的PR17评论；正式交付后停写待C03验收
previous_executor_delegation: CC02旧委派不恢复；旧本机进程及未推送内容unknown
activation_gate: Owner向已有C03a转发入口，绑定及写前核对后P00–P04连续推进
external_execution_started: false
prior_status: mm_context15_PASS_PENDING_LOCAL_WITH_R15_S01_S02
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
  - 第15批六件全文、交付评论和本轮主线回执；既有18/24/25/28/33规则。
  - 固定time pgtable_arch.c 1–100、fork.c的dup_mm/copy_mm和前轮同源dup_mmap，作为根对象与调用边界。
  - copy_page_range默认分支搜索仅导航；固定time fault.c 1–485预检复制层级与叶子调用，未展开所有helper、故障或引用路径。
  - 两PR、直接refs、开放PR第三项为空、PR16变更路径与15增量compare。
open_questions:
  - 页表复制是否改变父子权限、增加数据页引用或实际复制数据，以及部分失败后留下什么，待本批有界核验。
  - 不预判COW和用户隔离成功，不认证完整页表/分配器/TLB/并发或实际CPU状态。
---

# 第16批：fork复制了哪些页表，哪些页面仍然共享

## 1. 目标和启动

15已按限定静态范围接收，R15-S01纠正图的数据来源/字段措辞，R15-S02限定实际切换收尾。先合读即可，不另造勘误文件，不重做15的110/88检查或14区间任务。

本批从dup_mmap已知的copy_page_range接口向下，核父子下级页表的建立、叶子PTE复制、写保护、数据页引用以及失败回传。必须区分四件事：新建mm/根PGD；分配下级页表页；复制页表项及权限；复制或共享真实数据页。任何一项都不能凭“fork成功”“函数名包含copy”替代其他项的证据。

Owner向已有C03a发送：

> 执行CORE-USER-PGTABLE-COPY-16。完整读取34号固定任务书及15主线回执，沿用已有接收和GPT-6＋Extra。按P00–P04连续完成fork页表复制、父子页权限/引用及失败副作用的静态分析、自查、原PR17提交和远端回读。只在指定新目录交付六件，旧件保留；普通问题入库，不修内核、不重做14/15。完成后停写交C03验收，不自行开始下一批。

内部步骤无需逐项确认；GitHub发布不自动唤醒聊天。只有真实权限、读取、输入冲突或不明并行写入才突出阻断；其余安全项继续。范围不足明确PARTIAL，不能用形式齐全替代必要证据。

## 2. P00：固定对象与增量输入

先完整读本任务与required_review_record，区分指令f791212e5bf9、旧结果6ea1003c2ae1及源码a039d9803ade。先读options_flags.cmake；DEBUG、页表级数及外部构建选项保留未预处理限制。18号普通问题延期、24/25执行验收分工及28号同轮派发继续适用，不重做25号接收，不沿用15已结束的写权。

15只取dup_mm/dup_mmap、PGD根、失败mmput的端点和R15-S01/S02；14只取VMA.vm_mm/链与失败副作用；12只按需要取现有PTE写保护缺页的入口，并合读R12补充。13源缓存页与私有COW页的对象区别按R13-S03，不整包重读旧报告。04/05相关既有问题需要时用qualified_id和最新回执定点取证，旧状态不自行更改。

已知导航：mykernel/mm/fault/fault.c及memfault_api.h、sched/forkexec/fork.c、arch/x86_64/mm/pgtable/与实际include的页表帮助函数。默认分支搜索仅导航，真实结论回固定time；主线预检不是本批执行证据。

## 3. P01：实际调用门和多级页表遍历

核dup_mmap对VM_DONTCOPY、VM_WIPEONFORK及copy_page_range返回值的实际处理；区分复制VMA与复制其页表，区分CLONE_VM共享旧mm与非共享dup_mm入口。沿15已有调用边定点补证，不重做全部fork或任务发布。

完整读copy_page_range→copy_p4d_range→copy_pud_range→copy_pmd_range→copy_pte_range的活动层级、参数和返回。逐层记录src/dst对象、addr/end/next推进、none_or_clear_bad的效果、目标层级分配及空返回。缺少源条目时是否已分配目标结构、源坏条目是否被改写，也要按实际顺序说明。

页表层数与P4D折叠只按本树真实宏/类型/配置定义，不凭Linux版本或当前宿主机器推断。对pgd_offset、各级alloc/offset/addr_end等，核实际对应函数或宏，并追到第一次明确分配、安装、锁/解锁或失败端点；不扩展整个buddy/slab或所有架构。

范围要求为源VMA的[start,end)实际字段与上层前提。记录页对齐、空范围、跨层级边界、推进终止及溢出检查由谁提供；无效对象或无法证明终止时不继续推定确定返回。do/while的continue仍会执行哪些尾部更新必须按C控制流核，不把“跳过条目”说成提前退出整个复制。

## 4. P02：叶子PTE、父子权限和数据页身份

完整读copy_present_pte及其直接PTE帮助函数。分清源PTE内存写入、局部pte变量变换与目标PTE安装，尤其is_cow_mapping、pte_write、arch_ptep_set_wrprotect、pte_wrprotect、pte_mkclean/pte_mkold、set_pte_at的真实定义和条件。父PTE、子PTE、物理页号、写/访问/脏位必须分列，不能把本地变量变化当父表已改。

列表区分私有可写、私有只读、共享及本树确有处理的特殊状态。函数名或注释里的“COW”不能证明写时复制已完整工作；依据相应位和分支说明复制时到底是否新分配数据页、复制字节或只共享页号。

从当前复制路径核有无get_page/folio_get、mapcount、rmap或其他真实引用动作；名字相似但未调用者只列潜伏端点。数据页与页表页分别记所有权，不能从目标页表分配成功推断数据页已经加引用；负向结论需所读函数/宏上下文，禁止扩大全树“永不加引用”。

仅把复制后产生的写保护PTE接到12已分析的相应写缺页入口或已知断点，核必要的pte_present/write及fault flags分派即可。**不在本批重新展开完整wp_page_copy/do_cow_fault、文件填页、全部COW并发和释放算法。** 不把13的无PTE文件私有写缺页与本批已有PTE写保护自动当成同一入口。

## 5. P03：失败、非present及返回消费

对每层分配失败及pte_alloc_map_lock/pte_offset_map_lock/pte_unmap_unlock实际实现核完整前后文。尤其失败分支中指针/锁输出参数是否已初始化、清理函数接受何种对象；若在return之前已有无效实参/未初始化状态，不只看末尾return -ENOMEM就宣称可靠失败返回。

分别处理pte_none与非空但!pte_present。是否忙等、支持swap/huge、返回何值须由活动实现证明；注释写“不支持”不等于所有相关输入已被安全拒绝。追copy_present_pte的当前实际返回集合及copy_pte_range如何消费ret；注释提到EAGAIN不等于当前callee真的能产生它。上层将错误归为ENOMEM、忽略或继续时准确记录，不凭接口注释补出传播。

构造精简的代表矩阵：跳过VMA/跳过复制、空页表层级、present叶子私有可写/只读/共享、非present、各级目标分配失败、复制若干页后失败、跨边界、无效范围。每行写父/子页表与权限、页表页/数据页引用、已发生副作用和实际返回/unknown；不凑笛卡尔积或固定数量。

失败后父PTE此前若已写保护、子页表已部分建立、数据页引用已变化，分别核是否回滚；只用15已验收的dup_mm→mmput/__mmput断点说明上层，不能重复说“所有失败无清理”或“必然全部恢复”。TLB失效只核所选复制链及直接调用者是否有活动接口，不展开PCID/shootdown，不从未见调用断言硬件一定保留错误转换。

## 6. P04：六件交付、证据与验收

严格仅allowed_files六件。MANIFEST记录P00–P04完成/partial；pgtable-copy.md讲清层级、父子对象和权限/引用/失败表；facts集中精简节点、边、状态/所有权与锚点；DOT对应facts；deferred只增量问题和旧项证据；evidence写实际方法、读取范围和NOT_RUN。不增加第七份工具、日志或单独勘误。

关键事实附真实路径、符号、1–5行逐字引文；全部强制引用回源核对。调用点、门条件、失败标签分别有证据，负向结论需完整所选主体。引用数量无目标，避免把长函数机械切成大量重复片段。引用相等、ID存在与语义正确分别记录；不自称外部模型独立审查。

call为真实调用方→被调方，宏别名/展开与中间帮助函数明确；data为消费方→真正提供对象，形参对象、task字段、父/子实体不能互换。写表项、改权限、增减计数放effects；active仅表示源语句活动，不保证每次调用抵达。返回API、内部指针和资源持有分字段。

主动反证：新mm是否等于新数据页；复制PTE是否自动增加引用；写保护源PTE是否等于父CPU TLB已同步；返回0是否意味着每页复制；上层失败是否恢复父表及子表；注释里的功能是否真的调用。证据不足标unknown或PARTIAL，普通问题按18号入库，不要求Owner选择修复。

允许在自己的临时沙箱用已有文本工具做引用、YAML/JSON、图ID或算术核对；不安装、不执行仓库脚本/C/ASM、不操作Owner工作树。人工/机械/未执行分开。无Deep Research、额外模型/API或充值要求。只回READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING，上限PASS_PENDING_LOCAL。

## 7. 写前、写后及恢复

沿用PR17/claude/dazzling-cori-q0dnyt，首次写前期望6ea1003c2ae1。C03a唯一获本批六件及自己的评论写权，C03独占lead、PR导航与验收；导览/教学对话仍只读。不得另建或改名分支/PR、改PR标题正文状态或他人评论，CC02旧委派不恢复。

写前复查直接master/time/执行头、开放PR和路径。主线准备时只有16/17，PR16无本批新路径，旧执行增量仅15六件；执行方仍须检查目标目录及当前头。已有同名交付先核是否完成，不重复执行或覆盖；不明漂移先核来源，不重置/强推。time前进保留历史pin并登记新头，不静默换源；lead合法前进不算执行冲突。

六件可顺序提交；按此前自己已核实提交衔接，交付前只可用当前blob SHA更新本批自己的新件。旧件冻结，正式交付后本批亦冻结。Contents API无全局原子分支锁，保留竞态限制。调用超时/结果不明先读远端路径与历史，禁止盲重试。进度保全在现有六件适当位置或自己的评论，明确PARTIAL/下一安全动作；不增加第七件，不声称回合结束后后台继续。

写后按最终固定提交完整读回六件，再按当前分支核blob和头；compare 6ea1003c2ae1到结果只能是本批六路径A新增、旧件无净修改/删除、无未知提交。后置结果记PR17自己的交付评论，给实际短提交和六件入口，不为自引用反复改文件。交付后停写，Owner只通知完成，C03从GitHub验收并同轮安排后继。

## 8. 禁止动作

不修内核/用户程序/旧脚本/旧成果，不生成内核补丁，不预处理/编译/运行C/ASM/内核/QEMU，不安装、挂盘、读凭据或仓库外libc/镜像，不直写master/time/lead，不合并关闭PR、删分支、force-push或进入阶段3。静态接收不等于父子内存隔离或COW在硬件上通过。
