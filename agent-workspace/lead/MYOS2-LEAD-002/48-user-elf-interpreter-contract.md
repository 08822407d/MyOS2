---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ELF-INTERPRETER-28
record_type: bounded_elf_interpreter_acquisition_mapping_and_lifetime_contract
evidence_class: 后继静态任务授权与验收条件；不是执行成果
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-02
read_channel: connector
authorization_ref: Owner要求27独立验收；28-review-and-next-batch-same-turn持续约定
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
executor_display_name: MYOS2-A-C03c 代码分析执行续建
execution_model_tier: next_tier_default
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
instruction_base_short12: d2f148ad2d74
required_review_record: CORE-USER-ELF-HEADER-READ-27-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ELF-HEADER-READ-27-review.md
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；lead=agent/MYOS2-LEAD-002
source_pin_short12: a039d9803ade
result_base_short12: f98ee06bbce8
master_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-elf-interpreter-28/
allowed_files: [MANIFEST.md, elf-interpreter.md, facts-and-boundaries.yaml, elf-interpreter-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: f98ee06bbce8及此前全部文件
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
  - 27固定六件、47任务及27主线回执；18延期、28号同轮约定、45身份分离持续适用。
  - 主线本轮固定time binfmt_elf.c539–612、610–780的load_elf_interp和PT_INTERP取得/头部检查，仅作范围预检。
  - exec.c570–730中的open_exec最小包装已预检；25/26已核的最终入口及共享段接口作为后继边界，不冒称本批完成。
open_questions:
  - 解释器文件与头部的所有权、局部错误值及装载偏移由执行者独立取证，不以主线预检代替。
---

# 第28批：ELF解释器从PT_INTERP路径到最终入口的有界静态链

## 1. 目标、分工与操作

第27批按PASS_PENDING_LOCAL限定接收，须合读R27-S01/S02/S03；没有单独文档返工任务，不改27原件。继续已接收的C03c / CHAT-EXEC-03、次一档GPT-6＋Extra；本批是该对话第4包，不需要新建对话或重放45号接收。

本批核内核怎样读取PT_INTERP路径、取得解释器文件和头表、调用load_elf_interp建立其映射参数，最后计算实际入口并处理局部失败与释放。解释器在此是ELF程序指定的装载器文件，不是本对话里的分析执行者。

只分析内核侧所选路径，不读取仓库外动态链接器/libc/镜像，不运行ELF样本，不展开用户态动态重定位、符号解析或全部文件系统权限/路径解析。elf_read/load_elf_phdrs、elf_load/elf_map和栈表使用27/26/25既有限定，只补解释器调用点的新实参、返回及顺序差异。

Owner发送本48号固定入口后，I00–I04内部连续推进至六件自查、提交、远端回读及本人PR17交付评论；不逐步骤索取确认。真正权限、输入身份或并行写入冲突才停相关动作。普通问题入库，证据缺口标partial，不把问题数或引文数当完成指标。任务发布不会自动唤醒对话。

## 2. I00：固定输入和旧结论消费

完整读取本任务与d2f148ad2d74中的27主线回执。源固定time a039d9803ade、结果基线f98ee06bbce8；先核mykernel/scripts/options_flags.cmake，默认分支搜索仅导航，结论返回固定对象。

定点合读25的AT_ENTRY/最终入口与未定义vma限制、26共享段映射/清尾/裁剪和错误生产者限制、27长度/错误折叠/类型/迭代器补充。必要时读05::UV-02/03及15地址空间替换的对应回执，避免把begin_new_exec后的错误当成仍可无条件回到旧地址空间。旧根因追加全限定ID，不重做旧批、不宣称全仓去重。

主要源码为mykernel/fs/vfs/binfmt_elf.c；仅为解释直接调用补sched/forkexec/exec.c中的open_exec/do_open_execat、必要file/filename/ELF定义和fput/kfree接口。路径解析、驱动、分配器、页表释放及CPU异常不是本批新审计范围。

## 3. I01：PT_INTERP选择、路径缓冲与文件取得

从load_elf_binary实际程序头遍历取得elf_ppnt，核PT_INTERP筛选、循环break/continue、同一数组中多个PT_INTERP的所选行为。不能从循环形式推定所有解释器都被加载；未命中、首个命中失败及成功后停止分别登记。上游程序头有效性仍受27限制。

跟踪p_filesz的真实类型、2..PATH_MAX门、kmalloc、elf_read实参p_offset与长度、末字节NUL检查、open_exec和路径buffer的kfree。区分末字节为NUL、内部可能先遇NUL、路径可被使用、打开成功及解释器内容有效；不假定路径必为绝对路径，不把存在尾NUL当权限或目录解析认证。只检查为说明这一调用需要的路径消费接口，不做全文件系统路径审计。

按真实语句核open_exec中getname_kernel、IS_ERR、do_open_execat、putname与返回；必要时核do_open_execat给do_filp_open的flag/模式及活动/注释校验，止于其取得file或错误的直接接口。不得凭函数名断言权限全部检查，也不得因本包装未查便宣称下层无约束。

建立对象表，至少区分主程序bprm->file、本地interpreter、bprm结构里的interpreter字段、路径临时buffer、filename_s、interp_elf_ex、interp_elf_phdata和主程序elf_phdata。名字相似不等于同一对象；注册/调用/复制指针不自动取得或转移引用。

## 4. I02：解释器头/程序头校验和错误来源

核解释器ELF头的独立分配、sizeof读取、magic/架构/fdpic等检查，以及e_type和f_op->mmap在哪一层才检查。区分主程序的-ENOEXEC与解释器的-ELIBBAD及其他来源，不把27主程序错误折叠结论无条件迁移到解释器调用点。

对load_elf_phdrs只复用27的长度和NULL结果契约，补本次参数及调用者retval阶段值。明确读取错误、分配失败、类型/架构不符分别走哪个label，是否保留原错误或改为更粗分类。PTR_ERR仅作值转换时不等于已经判定该对象是错误指针，须合读IS_ERR及后继赋值。

标出上述校验相对begin_new_exec、setup_arg_pages、主程序映射和load_elf_interp的源顺序。迟到的类型或方法检查不能倒写为早前动作已满足的前提。GNU property、PT_LOPROC等只记录必要的外部先决门，不展开完整属性解析；无定义良好的全链前提时继续使用独立局部模型，不假设跨过旧缺口。

## 5. I03：load_elf_interp的偏移、首段状态、返回与入口

完整读取load_elf_interp及其真实调用点，跟踪load_addr、load_addr_set、error、total_size、no_base、map_addr的初值、赋值和使用。特别区分error初始~0UL与实际负errno；helper返回的是何种偏移/编码值，不能把它直接称为最终PC。

核ET_EXEC/ET_DYN、首个/后续PT_LOAD的MAP_FIXED选择、no_base实际来自哪个主程序变量、load_addr=-vaddr的类型与模运算含义、total_size的初次使用和清零时点，以及成功后load_addr如何从map_addr和页对齐vaddr计算。与26主程序首段MAP_FIXED_NOREPLACE/整体预留路线比较时只列差异，不重跑26全部帮助函数。

核BAD_ADDR和后置p_filesz/p_memsz/TASK_SIZE检查的实际顺序，错误生产值和调用者IS_ERR_VALUE/BAD_ADDR转换；total_mapping_size零值、无PT_LOAD或仅零文件字节段等模型均须注明生产前提，避免凭有效地址数值认定全部映射、清尾、裁剪和引用已完成。必要纸面算例明确宽度、unsigned加减、对齐和转换，不忽略R27-S02。

回到load_elf_binary核interp_load_addr、解释器e_entry相加、最终elf_entry与主程序e_entry的不同职责，以及AT_BASE、AT_ENTRY、START_THREAD的交接。只复用25已核边界，不重做栈表/寄存器返回或宣称用户态链接器已运行。

## 6. I04：局部资源账、错误矩阵与六件自查

逐入口/label记录临时路径、解释器file、解释器头/头表、主头表的持有和释放调用：正常读取、路径长度/读取/末NUL失败、打开错误、头分配/读取失败、头校验/程序头失败、映射错误、最终入口越界与正常交接。实际kfree/fput调用、引用减到零、对象确实回收和整段映射已撤销是不同层，不做未经证明的“永久泄漏”“双重释放”或全事务回滚判断。

成功清理后局部指针是否清零、是否被后继代码使用、使用的是数值/条件还是解引用须分开；非NULL不等于仍持有引用，fput调用也不自动等于对象已释放。发现需要更深文件生命周期证明时登记依赖，不扩成整个fput系统重审。

仅交front matter指定六件：MANIFEST逐I00–I04列completed/partial；elf-interpreter.md给连贯路径与资源账；facts给对象/阶段快照、错误矩阵及有限算例；DOT分调用、绑定、数据和效果；deferred以EI28-*记录候选及历史全限定ID；evidence包含实际固定读源、1–5行逐字锚点和已做/未做。空行计入5行上限，不将标题代替源码门；补引用时保留所属调用者与类型。

图中调用点不是新函数，返回数值不是写入动作主体，主程序和解释器buffer/file必须分对象。共享helper接口复用不能画成新调用已运行成功。允许现成临时沙箱的文本/JSON或YAML/图/有限算术检查，记录确切实际工具；不安装、不运行仓库脚本/C/ASM、不调用额外付费模型/API。只申请READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING。

## 7. 写前、写后和停止线

C03c为本批唯一执行者，继续原PR17与claude/dazzling-cori-q0dnyt；首次写前期望f98ee06bbce8。核直接master/time/执行头、开放PR、路径交集和新目录；已存在同名成果先核归属/是否交付，不盲覆盖或重复起批。lead合法前进不是冲突，源变化记录但不静默换源。旧C03a/C03b/CC02不恢复。

所有旧件冻结；仅正式交付前可按当前blob修订自己本批六件。正常快进，支持expected-head就用；否则如实保留竞态限制，不宣称全局锁。不建竞争分支/PR，不改PR标题正文/Draft或他人评论。超时先读远端确定是否成功，再补余项。

写后从最终固定提交完整回读六件，并按活动分支核正文/blob/head；compare f98ee06bbce8到结果须恰本目录六件A新增、旧件零净修改/删除、无未知提交。实际后置核验和短提交写本人PR17交付评论，不为自引用最终提交反复改文件。

正式交付后停写交C03独立验收，不自行启动29。中断记录留本批六件或本人PR17评论，不加第七件，不承诺后台执行。Owner只需转发本48号固定入口及通知完成，不搬运报告。

## 8. 禁令和持续边界

阶段2、18延期、28号同轮安排约定和45角色分离不变。禁止改内核/用户程序/旧成果/脚本、生成修复补丁，预处理/编译/运行C/ASM、内核/QEMU/硬件或真实ELF样本，安装/挂盘/读凭据、操作Owner本机及读仓库外libc/镜像。无master/time/lead写入、合并关闭PR、删改名/强推分支或阶段3权限。C03独占规划、lead、PR导航与最终验收，导览/教学只读；普通问题延期，Owner当前无需修复决定。
