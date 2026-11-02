---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ARG-PAGES-24
record_type: bounded_exec_argument_page_static_analysis_contract
evidence_class: 后续静态执行授权及验收条件；不是已完成的代码分析
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-11-02
read_channel: connector
authorization_ref: Owner报告23完成；28号同轮验收和后继派发持续约定
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-02
executor_display_name: MYOS2-A-C03b 代码分析执行续建
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
instruction_base_short12: 1d5d6b8a52af
required_review_record: CORE-USER-EXEC-STACK-23-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-STACK-23-review.md
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；lead=agent/MYOS2-LEAD-002
source_pin_short12: a039d9803ade
result_base_short12: 299a960562a9
master_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-arg-pages-24/
allowed_files: [MANIFEST.md, arg-pages.md, facts-and-boundaries.yaml, arg-pages-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 299a960562a9及此前全部文件
writer_assignment: C03b仅本批六件及自己的PR17进度或交付评论；正式交付后停写
activation_gate: Owner向已有C03b转发本任务；固定输入及现场写前检查通过
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
  - 23固定六件及23主线回执；42号与18/24/25/28/36既有分工约束。
  - 固定time exec.c的get_arg_page、put_arg_page、copy_strings/copy_string_kernel及所选上游；gup.c全文；mm/fault/fault.c两个单页映射帮助函数，作为验收及范围预检。
  - 18页引用、12缺页、15地址空间与23栈状态的已有边界作为定向输入，不宣称本轮重做全部旧批或完成24。
open_questions:
  - 已有page和新分配page的持有差异、映射失败消费与参数复制交接由本批系统核实，主线预检不是交付。
---

# 第24批：exec参数页的取得、持有、映射与复制交接

## 1. 目标与分工

23按PASS_PENDING_LOCAL限定接收，必须合读R23-S01/S02/S03；不另交23勘误，不重做栈VMA创建、EXSTACK策略或expand_stack。继续现有C03b、GPT-6＋Extra和25/36已完成接收，不新建聊天，不恢复C03a/CC02。

本批只追get_arg_page→myos_get_user_pages→单页查询/创建映射→参数复制与put_arg_page的接口、状态和对象链。回答：返回状态与输出page是否一致，已有page和新page各获得什么引用，装表失败如何被消费，复制函数在哪些前提下触及哪些字节，失败之前已经发生哪些副作用。不是修复、完整GUP实现、整个内存管理审计或用户态ABI认证。

Owner启动句：

> 执行CORE-USER-ARG-PAGES-24。完整读取43号固定任务及23主线回执，沿用C03b接收和GPT-6＋Extra。按A00–A04连续完成参数页取得、单页映射、引用及复制交接的静态分析、自查、原PR17提交和远端回读。仅交指定六件，旧件保留；普通问题入库，不修内核、不重做22/23。完成后停写交C03验收。

工作包内自主连续推进，不逐小步等待确认；真实权限、输入冲突或不明并行写入阻断才停止相应动作。证据不足如实记partial，不用文档齐全代替分析完成。发布任务不会自动唤醒执行对话。

## 2. A00：固定输入与历史根因

完整读取本任务及1d5d6b8a52af中的23主线回执。源码time固定a039d9803ade、结果299a960562a9与指令基线分开。先核options_flags.cmake，默认分支搜索仅用于导航，判断必须回固定对象。18号延期、24/25执行验收分离、28同轮派发、36的C03b身份继续有效。

23的ES23-F02（GUP缺返回值/输出槽缺写）、ES23-F03（新参数PTE固定模板）已经登记，本批只追加必要分支和调用者证据，不能重复计为新根因；R23-S02明确get_arg_page局部ret是int、GUP声明long。23的栈VMA/mm时序、12的点包含/缺页返回、21初始NULL迭代均为强制限定，不能凭“列表有效”自动消除。

定点合读18主线回执与所需page/folio引用结论，必要时补读对应源码而不重审整个伙伴分配器。15的mm释放/退出、14/22的VMA摘链只作已有生命周期边界，不展开全局资源回收。任何声称“新问题”须先与所读历史根因按来源/条件对照；不能冒称已完成全仓去重。

主要路径为mykernel/sched/forkexec/exec.c、mm/misc/gup.c、mm/fault/fault.c及直接引用、页表、页访问帮助函数。允许为确定实际对象/宏/调用返回而定点读取必要依赖；不扩大完整ELF装载、argv/envp表构建、用户寄存器、通用uaccess、所有fault/COW或硬件MMU分析。

## 3. A01：返回值、输出槽与现有页/新页分流

完整核get_arg_page和myos_get_user_pages及必要声明，逐阶段记录形参类型、局部类型、传入mm、start/pos、nr_pages、pages[i]与调用者page。区分虚拟字节地址、页内偏移、对齐地址、页帧号与page对象；不把同名值在不同阶段混为一个快照。

覆盖实际nr_pages=1调用以及帮助函数局部0/多页条件模型；只有存在真实调用者才将多页模型升为生产路径。核每个循环分支对输出槽究竟是赋有效指针、赋NULL、保留调用者原值还是从未写入；不得把continue理解为清NULL或已返回成功页数。

GUP缺return是已证文本事实；其结果被使用后的确定性推演在缺口处停止，不补写隐含return、不假设随机数或ABI寄存器残值。仍可描述在有效前缀及实际抵达前提下的源码写入，但不能绕过未定义行为去认证真实运行。get_arg_page将结果存入int再比较，输出page的定义和值必须独立检查，不能把ret>0当作page自动有效的证明。

## 4. A02：单页查询与创建映射接口

完整核myos_get_one_page_from_mapping和myos_creat_one_page_mapping及最小必要页表依赖。前者的各级指针/none谓词、物理地址提取、phys_to_page转换和返回对象，与持有引用是分开的事实；软件存在判据不能替代硬件有效映射。

后者逐层记录pgd/p4d/pud/pmd/pte的来源、分配返回和实际检查对象。每次失败门要对照刚得到的变量，分别登记NULL、ERR_PTR、有效页表指针及page参数；不因名字是分配函数就假设必然成功。保留page->_mapcount更新、PTE写入和返回值的先后，区分计数与引用，出错之前的分配/发布与完整回滚。

只核本次单页链，不扩展邻近myos_map_range全流程。PAGE_SHARED_EXEC和PAT等按固定宏及直接构造动作取证，23已有模板问题只追加本路径证据；不签发RWX实机结果。若发现错误返回没有被上层检查，说明真实消费语句与输出对象，不把返回控制流当成映射成功。

## 5. A03：页引用与复制交接的有界状态账

分别建立已有映射page、新分配page、安装前失败page、输出槽未写/NULL的身份和持有账。追选定get_page/put_arg_page→put_page及其必要直接定义，区分分配初始引用、映射持有、临时使用引用和_mapcount；某调用是否确实加引用必须有源码，不按Linux GUP惯例补全。

逐分支说明何时调用put_arg_page、映射帮助函数失败后是否仍get_page/写pages[i]、已有page是否取得独立引用、重复访问同页的调用是否会重复取得/释放。引用归零和物理回收继续受18限定，不因少见一条get就直接宣布已运行双重释放或永久泄漏；需记录前置引用来源与未闭合边界。

对copy_strings及copy_string_kernel只追取得page到复制/归还的最小链：整个字符串预先推进bprm->p，与每个分块pos、offset、bytes_to_copy及真实目标区间分别记账。定点核memcpy_to_page及其页访问依赖；读取原字符串、实际复制字节、剩余页面内容和页面清零是不同事实，不按GFP名称推断零填充。不展开完整字符串安全、通用uaccess或ELF参数表算法。

失败分支记录已发生的指针推进、页分配、引用和PTE副作用及是否有局部撤销；free_arg_pages/free_bprm仅核必要交接，mm完整回收沿用15边界。跨出初始栈VMA要以当前查询地址且不存在其他覆盖VMA为前提，不能从bprm->p整体下降推出所有分块都失败。未定义/无效对象之后不预言确定机器状态。

## 6. A04：六件交付、自查和验收门

严格只交allowed_files。MANIFEST按阶段记completed/partial；arg-pages.md给调用→分流→对象/引用→映射→复制/归还的连贯说明；facts给返回/输出槽及对象状态矩阵；DOT对应真实关系；deferred区分新增独立条件问题和ES23-F02/F03等历史追加；evidence保留真实读源、自查与未做事项。不另加脚本、日志或第七件。

新ID建议AG24-*。矩阵覆盖VMA未命中、已有page、新分配成功/失败、各层映射失败、输出NULL/未写、返回值缺口、不同分块/重复页、复制前后和归还边界。分清实际调用者路径、直接函数条件模型和待证生产者；不设问题数、引文数或报告长度指标。

每个关键断言给实际path::symbol和1–5行逐字引文，包含真实调用句、赋值或门；callee定义不替代caller调用，未展示的清零、锁或return不得由标题声称证明。宏展开、函数指针绑定、函数调用、数据依赖和effect分开，字段不得画为被调用函数；类型与不同阶段对象必须明确。

检查全部计数和引用ID与最终文本一致；没有满足前提的场景不写确定结果。可用已有临时文本工具作字符串、JSON/YAML、图ID、有限算术核对，不安装、不执行仓库脚本/C/ASM、不启用额外付费API或模型。机械一致性不证明语义正确，只申请READY_FOR_MAINLINE_REVIEW和mainline_acceptance=PENDING。

## 7. 写前、交付与中断恢复

继续PR17和claude/dazzling-cori-q0dnyt，首次写前期望299a960562a9。现场核master/time/执行头、开放PR与路径交集、新目录；已有同名内容先核归属和交付状态，不重复起批或覆盖未知写入。time变化记录但不静默换源，主线lead合法前进不是执行冲突。

所有旧件冻结。正式交付前可按当前blob身份修订本批自己新文件；只普通快进，不建竞争分支/PR，不改PR标题正文/Draft及他人评论。接口支持expected-head时使用，但不称全局锁；调用结果不明先读远端再补余项。

最后从固定结果提交全文读回六件，再按活动分支核正文/blob/头；compare 299a960562a9到交付头应仅本目录六件A新增，无历史净修改/删除或不明提交。真实后置核查写自己的PR17交付评论和短提交，不为自引用最终提交反复修改成果。

正式交付后停写，由C03独立验收并同轮安排后继。中断材料仅放约定六件或自己的PR17评论；不加第七件、不承诺后台继续。Owner只转发固定任务入口及通知完成，不搬运正文。

## 8. 权限不扩展

保持阶段2静态分析、18延期、28同轮派发和36角色分工。禁止修改内核/用户程序/旧成果和脚本、生成修复补丁、预处理/编译/运行C/ASM、内核/QEMU/硬件测试、安装/挂盘/读取凭据、操作Owner本机或读取仓库外libc/镜像。不得写master/time/lead、合并关闭PR、删除/重命名/强推分支或进入阶段3。C03独占主线与最终验收，导览/教学只读；普通问题入库，当前不向Owner索取修复决定。
