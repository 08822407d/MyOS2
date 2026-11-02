---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-VMA-MERGE-22
record_type: bounded_vma_merge_static_analysis_contract
evidence_class: 后续静态执行任务及验收条件；不是已经完成的代码结论
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-11-02
read_channel: connector
authorization_ref: Owner报告21完成；28号同轮验收及后继派发持续约定
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-02
executor_display_name: MYOS2-A-C03b 代码分析执行续建
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
instruction_base_short12: 207d89c40826
required_review_record: CORE-USER-MMAP-PLACEMENT-21-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MMAP-PLACEMENT-21-review.md
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；lead=agent/MYOS2-LEAD-002
source_pin_short12: a039d9803ade
result_base_short12: 16baace7bd3c
master_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-vma-merge-22/
allowed_files: [MANIFEST.md, vma-merge.md, facts-and-boundaries.yaml, vma-merge-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 16baace7bd3c及此前全部文件
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
  - 21固定六件及21主线回执；40号与18/24/25/28/36既有规则。
  - 固定time mmap.c合并及调整主体作为范围预检；vm_map.h查询与vma_pages、vm_map_types.h对象字段；未进行22的完整分支取证。
  - 14主线回执和14延期项VL14-F01/F02/F03/F05，20主线回执关于回调后二次合并及字段归属。
open_questions:
  - 目标对象选择、合并前提与字段/链表副作用的系统性分析由本批完成，不把主线预检当作执行成果。
---

# 第22批：VMA合并到底选中哪个对象、改变哪些状态

## 1. 目标与分工

21按PASS_PENDING_LOCAL限定接收，须合读R21-S01/S02/S03；无需另交21勘误、不重做21。继续现有C03b及GPT-6＋Extra，沿用25/36接收，不新建对话，不恢复C03a或CC02。

只分析simple_mmap_region中的VMA合并调用、simple_vma_merge及其直接前后相容性帮助函数、__simple_vma_adjust与必要的列表/引用接口。回答：候选是否相邻且相容、实际目标是谁、边界与页偏移怎样变化、后继对象怎样被摘除或保留、返回对象与请求区间有什么关系。不是修代码、完整VMA审计或阶段3综合。

Owner启动句：

> 执行CORE-USER-VMA-MERGE-22。完整读取41号固定任务及21主线回执，沿用C03b接收和GPT-6＋Extra。按G00–G04连续完成VMA合并候选、对象选择、边界/页偏移及必要链表与引用副作用分析、自查、原PR17提交和远端回读。仅交指定六件，旧件保留；普通问题入库，不修内核、不重做20/21。完成后停写交C03验收。

工作包内自行连续推进，不逐步等确认。真实访问、权限、证据或不明并行写入阻断时如实停下相应动作；不能以报告齐全冒充分析闭合。任务发布不会自动唤醒执行对话。

## 2. G00：固定输入与历史问题归属

完整读取本任务和207d89c40826中的21主线回执。源码time固定a039d9803ade、结果16baace7bd3c与指令基线分开；先核options_flags.cmake，默认分支搜索仅用于导航，结论回固定对象。

18号延期、24/25执行验收分离、28号验收后同轮派发、36号C03b身份继续适用。定点合读14回执和14 deferred-findings中的VL14-F01（严格前驱/初始NULL）、F02（vm_pgoff表达式）、F03（摘链与清理）、F05（非事务回滚），以及20回执R20-S02/S04的回调后对象变更和函数归属。

21已经证明选址或查询存在条件缺口，不能以“上层已经确保没有重叠”作为无证据前提。14的页偏移公式与摘链不清理属于已登记根因，本批只能追加合并路径证据；未发现新的独立根因时不要重新计数。不得覆盖旧ID、旧文件或把报告接受改成问题已关闭。

## 3. G01：合并调用点与候选来源

以simple_mmap_region中的第一次合并和文件回调后的第二次合并为主线，核实传入的mm、prev、addr/end、vm_flags、file与pgoff。区分旧映射处理前保存的prev、本次调用时仍有效的对象、实际相邻前驱，以及回调之后的vma字段和局部变量。

完整读simple_vma_merge、simple_is_mergeable_vma、simple_can_vma_merge_before/after及必要查询帮助函数。保留VM_SPECIAL、VM_SOFTDIRTY配置、file身份、vm_ops/close与页偏移连续性等真实门，不用注释中Linux八种场景替代MyOS2活动代码。需要其他调用者时仅做有界定位，不展开完整mprotect、brk、exec或mremap流程。

find_vma_intersection、vma_lookup、vma_pages的实际语义须合读21/14限定；包含end的VMA不自动等于起点恰为end的相邻后继。注释保证、函数名和返回非NULL均不能代替邻接和对象有效性证据。

## 4. G02：目标对象与控制流

逐分支记录prev、curr、next、adjust_target、merge_prev、merge_next、vma_start/end及vma_pgoff的定义、赋值和读取。覆盖无候选、只前合并、只后合并、双向合并、prev存在但不相容、curr存在或为空等实际组合；区分仅局部可构造的状态与已证明由调用者产生的状态。

每次读取指针前确认所有抵达路径是否给它赋值；未初始化值、NULL、失效对象或不返回帮助函数出现时，停止推演确定的后续写入/返回，不假定编译器或机器会替代码补齐。不能将21的初始NULL遍历缺口仅用“链表有序有效”消除。

分别说明helper返回NULL的确切门、返回旧对象的前提，以及主调函数如何决定跳过新建或替换局部vma。第二次合并后释放的是新建临时VMA还是被选中的旧对象，需按身份账核实，不能只看变量名vma。

## 5. G03：调整后的字段、链表和资源边界

完整核__simple_vma_adjust中目标vm_start、vm_end、vm_pgoff的更新顺序及实际表达式。页号、字节地址、页数分别记账，保留PAGE_SHIFT、类型与溢出条件；变量vma_pgoff被计算不等于实际写入字段。比较前后对象身份，不能把已覆盖的vm_start当作仍可用的旧起点。

核后继完全覆盖时的循环摘链、部分覆盖时的vm_start与vm_pgoff改变，并定点核__vma_unlink_from_list和直接链表函数对mm->map_count、mm_mt.count及链接的操作。调用帮助函数与其副作用实际完成分开；链表校验/锁/不返回边界不得略去。

分别登记保留目标、被摘后继、新建临时对象及file引用。只追本链直接调用的fput、vm_area_free、vm_ops.close或其缺失，不重跑14整个munmap/退出清理。与14同根时evidence_append，不凭某个函数体未见free宣称全树永久泄漏。

vm_flags、vm_page_prot、vm_ops、vm_file及vm_private_data哪些被保留、哪些被改写，只按活动代码说清；字段相容检查没有覆盖的维度登记为证据缺口，不直接判全部合并非法。返回旧对象、扩大区间、更新PTE和完成硬件映射是不同事件。

## 6. G04：六件交付和自查

仅allowed_files。MANIFEST逐阶段记completed/partial；vma-merge.md给连贯的调用→候选→目标→字段/对象变化→返回链；facts给状态/身份/字段及条件矩阵；DOT保持相同ID；deferred区分独立新问题、旧根因的新路径及纯待证假设；evidence保留实际读源与未做事项。不另造脚本、日志或第七件。

新ID建议VM22-*。矩阵覆盖有实际意义的前/后/双向/均不合并、相邻与仅包含边界、flags/file/close/pgoff不相容、初次和回调后二次合并、未初始化/NULL门、完全摘除和部分裁切、可正常返回与不能继续确定的情形。纸面算例标为条件演算，不是真实运行测试；不设问题数、引文数或报告长度指标。

每条强制源码断言用真实path::symbol及1–5行引文，包含其必要门。宏片段须核展开定义，字段赋值归实际执行函数；纯闭括号或文件尾不能充当独立事实证据。汇总计数以最终六件内容为准，新增锚点后同步所有头部/清单计数，不把计数一致说成语义正确。

call、宏展开、间接绑定、消费方到提供方的数据依赖以及真实effect分开。call_path必须列出真实中间函数，不能把表项/宏当多次运行调用；同一局部变量在不同时间的值分阶段表达。

允许已有临时沙箱文本工具检查字符串、JSON/YAML、图ID和有限算术；不安装、不运行仓库旧脚本或C/ASM、不调用额外收费模型/API。人工、机械、未做分别记录，只申请READY_FOR_MAINLINE_REVIEW和mainline_acceptance=PENDING。

## 7. 写前核查、交付和中断恢复

继续PR17及claude/dazzling-cori-q0dnyt，首次写前期望16baace7bd3c。核实际master/time/执行头、开放PR与路径交集及新目录；已有内容先核归属和状态，不重复起批或覆盖未知提交。time变化必须记录，不静默换源；lead合法前进不是执行冲突。

只新增本批六件，全部旧件冻结。正式交付前可按当前blob身份修订本批自己的新文件；只普通快进，不建竞争分支/PR、不改PR标题正文或Draft、不改他人评论。接口比较更新不是全局锁；结果不明先读远端再补余项。

完成后从最终固定提交全文读回六件，再按活动分支核正文/blob/头；compare 16baace7bd3c到结果头必须仅本批六件A新增，无历史净修改/删除或不明提交。真实后置结果写自己的PR17交付评论与短提交，不为自引用结果提交反复修改成果。

正式交付后停写，由C03验收并同轮安排后继。中断材料仅放约定六件或自己的PR17评论，不增加第七件、不承诺后台工作。Owner只转发固定任务入口及通知完成，不搬运正文。

## 8. 不扩展的权限

保持阶段2静态分析。禁止修改内核、用户程序、旧成果/脚本，生成修复补丁，预处理/编译/运行C或ASM、内核/QEMU/硬件实验，安装/挂盘/读取凭据、操作Owner本机或读取仓库外libc/镜像。不得写master/time/lead、合并关闭PR、删除/重命名或强推分支、进入阶段3。C03独占主线规划验收，导览/教学只读；普通问题按18号延期，当前不向Owner索取修复决定。