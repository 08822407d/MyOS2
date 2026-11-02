---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-EXEC-STACK-23
record_type: bounded_exec_user_stack_static_analysis_contract
evidence_class: 后续静态执行授权及验收条件；不是已完成的代码分析
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-11-02
read_channel: connector
authorization_ref: Owner报告22完成；28号验收与后继同轮持续约定
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-02
executor_display_name: MYOS2-A-C03b 代码分析执行续建
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
instruction_base_short12: c9e23b11fa66
required_review_record: CORE-USER-VMA-MERGE-22-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-VMA-MERGE-22-review.md
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；lead=agent/MYOS2-LEAD-002
source_pin_short12: a039d9803ade
result_base_short12: b32f3ebc9095
master_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-stack-23/
allowed_files: [MANIFEST.md, exec-stack.md, facts-and-boundaries.yaml, exec-stack-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: b32f3ebc9095及此前全部文件
writer_assignment: C03b仅本批六件及自己的PR17进度或交付评论；正式交付后停写
activation_gate: Owner向已有C03b转发本任务；固定输入和现场写前检查通过
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
  - 22固定六件及本轮22主线回执；41号与18/24/25/28/36既有规则。
  - 固定time exec.c 90–230及440–630；mmap.c 976–1065；arch/x86_64/mm/fault.c 1–370作为范围预检。
  - 12 deferred-findings中的MF12-F01/F02/F03及既有12、14、15、19、20、21限定作为导航，不宣称已完成23系统取证。
open_questions:
  - 初始用户栈的生产、设置与扩展的状态/时序链由本批闭合；主线预检不是执行成果。
---

# 第23批：exec建立的用户栈，地址、权限与扩展是否真正衔接

## 1. 目标与分工

22按PASS_PENDING_LOCAL限定接收，须合读R22-S01/S02/S03；不另交22勘误，不重做VMA合并。继续已有C03b及GPT-6＋Extra，沿用25/36接收，不新建聊天，不恢复C03a/CC02。

本批转向exec的用户栈VMA：谁创建、如何插入mm、setup_arg_pages改变哪些地址与权限数据、expand_stack实际改变什么，以及所选缺页入口能否找到待扩展对象。区别用户栈VMA、内核任务栈、参数指针、实际页面和最终用户寄存器，不把它们视作同一对象或一次原子成功事件。

Owner启动句：

> 执行CORE-USER-EXEC-STACK-23。完整读取42号固定任务及22主线回执，沿用C03b接收和GPT-6＋Extra。按S00–S04连续完成exec用户栈VMA创建、地址/权限准备、扩展和缺页入口衔接的静态分析、自查、原PR17提交及远端回读。仅交指定六件，旧件保留；普通问题入库，不修内核、不重做21/22。完成后停写交C03验收。

内部连续推进，不逐小步等待确认；真实访问、授权或不明并行写入阻断才停止相应动作。证据不足准确记partial，不以文档齐全掩盖未完成。文件发布不会自动唤醒聊天。

## 2. S00：固定输入与增量边界

完整读取本任务及c9e23b11fa66中的22回执。源码time固定a039d9803ade、结果b32f3ebc9095、指令基线分别登记；先核options_flags.cmake，默认分支搜索只导航，判断必须回固定对象。18延期、24/25执行验收分离、28同轮派发、36的C03b身份继续生效。

定点合读12主线回执及user-mm-fault-12/deferred-findings.yaml的MF12-F01（点包含查询限制grow-down入口）、MF12-F02（所选mm/权限/同步门）、MF12-F03（缺页返回消费），以及21的初始NULL迭代限定。它们是旧根因，不重记为本批新发现。

15的exec替换mm仅作身份/时序边界，19的匿名判定与页分配、20的局部flags/保护模板区分作为已审输入；必要时按符号/ID定点回读，不加载全部08–22大型事实库。22明确的分阶段副作用、实际调用证据和对象区分适用于本批。

主要导航为mykernel/sched/forkexec/exec.c、mm/vm_map/mmap.c、util.c、vm_map.h及类型/常量，arch/x86_64/mm/fault.c、相关架构栈对齐接口。必要的实际调用者从仓库定位并回固定time，不能根据Linux标准流程假造调用顺序。

## 3. S01：临时用户栈VMA的创建与发布

核bprm_mm_init、__bprm_mm_init以及最小必要上游调用点。区分bprm->mm、current->mm和vma->vm_mm在该阶段的身份；mm替换全过程复用15，不重新展开调度/CR3或fork。说明分配失败、插入失败和清理各自的控制流，不以变量同名认定同一个mm。

完整核初始栈vm_start/end、vm_flags、vm_page_prot、vm_ops、vm_pgoff的赋值来源。读取vm_area_alloc/vma_init、vma_set_anonymous及insert_vm_struct的必要直接依赖，确认谁何时设置页号、建立列表关系以及改变map_count/stack_vm/total_vm。未在生产链上赋值的字段与被初始化为零的字段不能混称为未知随机值。

STACK_TOP_MAX、PAGE_SIZE、VM_STACK_FLAGS、VM_STACK_INCOMPLETE_SETUP、VM_SOFTDIRTY等须回真实宏值和配置分支；初始只建一个VMA、计数写1、页面已分配及参数字节已复制是不同断言。参数拷贝或get_arg_page仅在证明本接口状态确有必要时定点读，不展开完整argv/envp/uaccess算法。

## 4. S02：setup_arg_pages的地址、权限和对象变化

至少追到一个真实调用setup_arg_pages的活动调用点，记录stack_top和executable_stack来源及该调用相对mm替换的时序。只核所选调用者接口，不扩展整个ELF解析、段装载或动态链接。

完整核arch_align_stack的实际实现、stack_shift计算、bprm->p/loader/exec、mm->arg_start和start_stack的写入。分别登记指针数值变化、VMA边界变化、已有页表搬迁和页面字节移动；注释里的shift_arg_pages不能当作执行过的搬迁。关注地址加减、对齐、无符号回绕、对象身份和先写后检查的顺序。

区分局部vm_flags的计算、写回vma->vm_flags的实际动作、vm_page_prot是否刷新及实际PTE权限。EXSTACK_ENABLE_X/DISABLE_X、mm->def_flags、临时flags清除各自按源代码取证；日志输出或局部变量含VM_EXEC不等于VMA/PTE已经拥有该权限。mprotect_fixup的注释和具名syscall都不能替代活动写入。

核mmap锁包装及其返回/解锁边界，不能从接口名推定已取得互斥。分别标明各失败出口之前已经写过什么、是否存在对应恢复；正常返回不是每一条先行写入发生的必要条件，也不能在无效对象/未定义行为后签发确定的机器状态。

## 5. S03：expand_stack及缺页入口的有限衔接

完整读expand_stack：输入地址的掩码和最小值门、address与vm_start的关系、size/grow计算、grow与vm_pgoff比较、目标字段写入、锁动作及error返回。没有扩大VMA的分支是否仍可能返回0，需要按真实控制流说明，不用函数名或0值替代后置条件。

核stack_expand、rlim_stack/stack_size和stack_base的生产及消费，区分活动限额/guard检查与注释；vm_start_gap/vm_end_gap存在不证明本路径使用它们。只对本链读到的同步/检查作有限结论，不展开全局SMP锁或资源限制实现。

区分exec准备时直接调用expand_stack与运行期do_user_addr_fault中的调用。对后者消费12的MF12-F01及21的NULL迭代限制：稳定VMA、实际点包含谓词、空查找和grow-down分支的前提分列，不把“定义存在”当作真实栈外地址能抵达。只衔接到myos_handle_mm_fault或错误出口，缺页内部PTE分派、匿名分配/COW与硬件返回沿用12/17/19，不重做。

特别区分元数据扩大、可查到新地址、为该地址建立PTE、页面内容就绪、初始用户栈指针被CPU使用这五个证据层。最终用户寄存器只需标出已有接口/未核边界，不重开07/08汇编和外部libc验证。

## 6. S04：六件交付与自查

严格仅allowed_files。MANIFEST逐阶段completed/partial；exec-stack.md给创建→准备→扩展→消费的连贯说明；facts给对象身份/字段前后/分支矩阵；DOT对应真实调用与数据/写入；deferred区分新条件问题和历史追加；evidence保留实际读取、1–5行逐字锚点、自查方式与未做事项。不新增脚本、日志或第七件。

新ID建议ES23-*。矩阵按实际分支覆盖分配/插入失败、exec不同阶段mm身份、栈顶对齐/偏移、可执行栈策略、局部flags与写回、扩展/不扩展/不足页偏移/低于最小地址、先写后失败、直接扩展与缺页触发、有效对象和不能继续确定的情形。不设引文数、问题数、报告长度指标；纸面演算不是实测。

每个关键源码断言给实际path::symbol和包含必要门的原文；字段赋值归真正执行函数，调用边须有调用者中的语句，不能只引被调定义。宏、函数指针绑定和运行调用分开，data按消费方到提供方，effect按真实写入；一个变量或展示节点不能掩盖不同对象和时间快照。

逐项检查矩阵前提是否足以推出结果，保留先行早退和短路求值。新增锚点后同步所有最终计数；数量一致与自查通过不等于语义正确。可用已有临时沙箱文本工具作字符串、JSON/YAML、图ID和有限算术检查，不安装、不执行仓库脚本或C/ASM，不调用额外收费模型/API。只申请READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING。

## 7. 单写者、交付与中断恢复

继续PR17及claude/dazzling-cori-q0dnyt，首次写前期望b32f3ebc9095。现场核master/time/执行头、开放PR与路径交集、新目录；存在内容先核归属和完成状态，不重复起批或覆盖未知写入。time变化记录但不静默换源，lead合法前进不是执行冲突。

全部旧件冻结；正式交付前可按当前blob身份修订本批自己新件。只正常快进，不建竞争分支/PR，不改PR标题正文/Draft或他人评论。接口支持expected-head时使用它，但不把比较更新称为全局锁；结果不明先读远端再补余项。

最后从固定结果提交全文回读六件，再与活动分支核正文/blob/头；compare b32f3ebc9095到交付头应恰本目录六件A新增，无旧件净修改/删除或未知提交。真实后置结果写自己的PR17交付评论及短提交，不为自引用最终提交反复修改文件。

正式交付后停写，由C03独立验收并同轮安排下一工作包。中断材料只放约定六件或自己的PR17评论；不加第七件，不承诺后台续跑。Owner只转发固定任务入口和通知完成，不搬运正文。

## 8. 禁止动作不变

仅阶段2静态分析。禁止修改内核、用户程序、旧成果/脚本，生成修复补丁，预处理/编译/运行C/ASM、内核/QEMU/硬件测试，安装/挂盘/读凭据、操作Owner本机或读仓库外libc/镜像。不得写master/time/lead、合并关闭PR、删除/改名/强推分支或进入阶段3。C03独占主线规划、PR导航和验收，教学/导览只读；普通问题按18号延期，当前不向Owner索取修复决定。
