---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-WP-COW-17
record_type: chat_existing_pte_write_protection_cow_static_contract
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-11-01
read_channel: connector
evidence_class: 下一工作包授权和验收条件；不是第17批已完成的分析
authorization_ref: Owner报告16完成；28号验收后同轮派发持续授权
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: MYOS2-A-C03a 代码分析执行
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
receiving_model_tier: next_tier_default
instruction_base_short12: 8fcefd53ef79
required_review_record: CORE-USER-PGTABLE-COPY-16-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PGTABLE-COPY-16-review.md
source_branch: time
source_pin_short12: a039d9803ade
result_base_short12: 38a23958a355
default_branch: master
default_branch_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-wp-cow-17/
allowed_files: [MANIFEST.md, wp-cow.md, facts-and-boundaries.yaml, wp-cow-paths.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 38a23958a355及之前全部文件，包括16和历史勘误
writer_assignment: C03a仅本批六件及自己的PR17评论；正式交付后停写待C03验收
previous_executor_delegation: CC02旧委派不恢复；旧本机进程与未推送内容unknown
activation_gate: Owner转发本任务到已有C03a，绑定及写前核对后W00–W04连续执行
external_execution_started: false
prior_status: pgtable_copy16_PASS_PENDING_LOCAL_WITH_R16_S01_S02
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
  - 16六件全文及交付评论、本轮16主线回执；34号及既有18/24/25/28规则。
  - 固定time fault.c 480–770、1210–1310，以及同源先前已读vm_normal_page/folio_prealloc/__wp_page_copy_user入口，作为17范围预检；不是完整后端核验。
  - 本轮固定time页表位变换、分配、映射锁与update_mmu_tlb后备定义；16有关父子PTE/数据页的有限结论不等于17已完成。
  - 两PR、直接refs、开放PR第三项为空、PR16全部变更路径与16增量compare。
open_questions:
  - 写保护后是否真正准备了新数据、安装新PTE并正确转移新旧页引用，仍待本批逐路径取证。
  - 不能依据COW函数名、return 0或注释承诺实际隔离、重试、资源回收或并发正确。
---

# 第17批：写保护触发之后，数据、映射和引用到底发生了什么

## 1. 目标和启动

16已经限定接收，R16-S01/S02直接补足软件谓词、真实入口门和目标初态。合读回执即可，不另造16勘误文件，不重跑162/84检查，不重做fork页表复制和15的mm持有/切换分析。

本批只追已有PTE的写保护缺页：handle_pte_fault→do_wp_page→wp_page_copy及必要直接依赖。分别回答旧数据页如何取得、新folio是否真正分配/初始化/复制、何时安装哪个PTE、引用如何转移及失败留下什么。不得将缺PTE的文件do_cow_fault与本路径混为一谈，也不扩成完整MM、文件缓存或硬件隔离审计。

Owner向已有C03a发送：

> 执行CORE-USER-WP-COW-17。完整读取35号固定任务及16主线验收回执，沿用已有接收和GPT-6＋Extra。按W00–W04连续完成已有PTE写保护缺页、数据页复制、PTE替换与引用/失败边界的静态分析、自查、原PR17提交及远端回读。只在指定新目录交付六件，旧件保留；普通问题入库，不修内核、不重做15/16。完成后停写交C03验收，不自行开始下一批。

工作包内部连续推进，不逐项征求确认。GitHub发布不自动唤醒聊天；访问/授权/输入冲突或不明并行写入才突出真正阻断，不依赖阻断的安全项继续。资料不足准确标PARTIAL，不以六件存在替代必要证据。

## 2. W00：绑定和增量输入

先完整读取本任务及required_review_record，区别指令8fcefd53ef79、旧结果38a23958a355与源码a039d9803ade。先读options_flags.cmake；DEBUG、页表及外部构建选项保留未预处理限制。18号普通问题延期、24/25执行验收分工和28号同轮派发继续适用，不重做25号接收，不沿用16已经结束的写权。

16只取复制后PTE权限/PFN、数据页引用和R16-S01/S02；15只取mm/失败清理端点；12只取相关handle_pte_fault及返回消费记录并合读主线限定；13只取源缓存页与私有folio身份及R13-S03。需要旧发现时用真实qualified_id与最新回执定点引用，不整包重读大型YAML，也不复制历史检查计数。

已知导航为mykernel/mm/fault/fault.c、pgtable.h、pgtable_macro.h、pgtable-generic.c及arch/x86_64/mm/pgtable/下的直接位操作。folio/page、高端映射、数据复制等帮助函数按实际include/调用定位。默认分支搜索只导航，正式结论必须回固定time；本任务的预检不是本批已经完成的证据。

## 3. W01：实际入口和旧页身份

完整读handle_pte_fault、do_wp_page及必要上游调用段，核orig_pte与pte_ptr如何产生、谁实际获取锁、哪些检查/解锁只是注释。对软件pte_none/pte_present、硬件PRESENT位和PROTNONE分开；R16-S01指出的PTE一致性、WRITE与RW门不能遗漏。不要仅凭函数注释的“已锁定”或__releases标注认定存在配对锁。

核do_wp_page的VM_SHARED/VM_MAYSHARE、私有分支、被注释的复用/共享处理以及实际folio_get。沿vm_normal_page到明确PFN校验/转换端点，分别记录合法普通page、零页PFN、没有普通page及非法输入；vmf->page为NULL不自动等于zero_pfn，非NULL也不自动证明获得新增引用。

只补足解释本批所必需的上游fault/VMA门，不重做12整个异常入口与错误处理。选定代表场景可以假定合法用户写缺页，但不能把该场景假设当作源码真实检查了全部权限或所有特殊PTE。

## 4. W02：新folio、数据准备和PTE替换

完整读wp_page_copy、folio_prealloc、__wp_page_copy_user以及直接的folio分配、复制/清零帮助函数。沿真实绑定核到明确分配/字节复制/清零端点，不展开整个buddy/slab。特别核need_zero参数与实际GFP标志及动作之间的关系；参数名、zero页名称或注释不能代替数据已经初始化的证据。

分别列：旧page存在且实际copy、旧page不存在但PFN不是零页、零页PFN、分配失败、copy帮助函数返回错误。__wp_page_copy_user必须读完所有出口；被注释的备用复制和清零不能算成活动处理，返回0也不自动表示数据已复制。帮助函数会失败或不返回时，不继续推定后续安装一定发生。

对wp_page_copy中的pte_ptr非NULL和pte_same复核、局部entry构造、FAULT_FLAG_UNSHARE、VM_WRITE/VM_MAYWRITE、mk_pte/maybe_mkwrite、dirty/young及set_pte_at逐项记录。分开旧PTE、局部entry、实际故障任务的目标PTE、另一任务仍持的PTE。父子共享原数据不代表改一方PTE就自动修改另一方；所有别名关系须由实际对象证明。

PTE发生变化或指针为NULL时走哪条分支、是否有更新/清理、返回何值都独立列出。只记录本树活动的TLB/缓存通知接口及其真实定义/条件；空后备定义和被注释调用分别标明，不认证CPU TLB、PCID、跨核shootdown或屏障协议。

## 5. W03：引用、失败及返回消费

建立以对象身份为中心的引用账本，至少区分旧映射数据page/old_folio、函数临时folio_get、新分配folio、局部变量new_folio和实际安装PTE。完整追所有赋值与folio_put：例如new_folio被改指old_folio时，变量名不再代表原新页。不能只数两个put就宣称双重释放；须说明每次get/put对应哪份已证持有、映射引用是否存在、初值及下层释放前提。

16的复制链未见数据页增引用是相关输入，不是自动证明本路径已经UAF或内存破坏。folio_get/put、page_folio及分配初始引用只追到本批需要的真实原语和明确释放端点；沿用13对__folio_put等已核边界时注明来源，不把到达释放接口当物理回收成功。

错误矩阵覆盖分配失败、复制失败、PTE复核失败/变化、旧page有无、零页PFN、UNSHARE、共享/可共享停点和正常安装。每行给已发生的数据/PTE/引用副作用、锁状态、清理及实际返回。VM_FAULT_OOM/HWPOISON、0及不返回分开，不能把0泛化为安装成功，也不把VM_FAULT位集合当普通errno。

仅沿直接上层消费说明返回结果被如何使用，可复用12的已核入口限定；不因此宣称故障指令已成功重试或进程已经恢复。必要锁获取、退出与重新验证实际抵达前提要读完整，不能在goto前停止，也不从被注释的标准Linux流程补出本树不存在的步骤。

## 6. W04：六件和验收标准

严格只交allowed_files六件。wp-cow.md先解释“数据准备—PTE替换—引用转移—返回”的不同结果；facts集中对象、条件矩阵、状态/所有权、真实调用和锚点；DOT对应facts；deferred只增量新项或旧项证据；evidence列真实读取、自查、反证、未做事项；MANIFEST标各阶段completed/partial。不新增第七件脚本/日志/单独勘误。

强制源码断言给真实路径、符号和1–5行逐字引文，全部回源。引文必须包含被声称的调用、判定掩码、赋值或错误出口，不能只截函数头/相邻声明。没有引文数量目标，优先紧凑的关键证据而非大量重叠切片。对否定结论标明完整所读函数与搜索范围，不能把有限未发现扩成全仓不存在。

call为实际直接调用或显式宏绑定，保留帮助函数中间层；data明确来源对象/参数/目标地址依赖，写入和输出赋值单列effects。源/目的实体、原页/新页与变量重绑定不能互换；图ID匹配不等于语义通过。动态目标须有绑定证据，否则unknown。软件谓词与硬件位、正常条件推论与真实运行仍分开。

主动反证至少处理：分配新页是否等于已复制有效数据；零PFN与page=NULL是否同义；局部entry改变是否等于PTE已经安装；pte_same检查是否真的伴随锁；两个put是否真的消耗同一份引用；return0是否代表COW完成；PTE写入是否代表硬件隔离/TLB同步。条件不足就收窄，不制造新修复决定。

允许在自己的临时沙箱用已有文本工具做字符串、引用、YAML/JSON、图ID和有限算术核对；不安装、不执行仓库脚本或C/ASM，不操作Owner工作树。人工、机械及未执行分别登记；不启动额外付费模型/API或Deep Research。仅回READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING，验收上限PASS_PENDING_LOCAL。

## 7. 写前、写后和中断恢复

沿用PR17/claude/dazzling-cori-q0dnyt，首次写前期望38a23958a355。C03a唯一获本批六件及自己的PR17评论权，C03独占lead、PR导航和验收。CC02旧委派不恢复，导览及架构教学对话仍只读。禁止新建/改名分支或PR、改PR标题/正文/状态或他人评论。

写前复查直接master/time/执行头、开放PR和路径。主线准备时只见16/17开放、PR16不含本批结果路径；执行方仍须核目录是否已有成果。若同名任务已经交付，先读结果，不重复执行或覆盖；不明头变化先核来源，不重置/强推。time前进仍保持历史pin并登记新头，不能静默换源；lead合法更新不算执行冲突。

六件可顺序提交，交付前只按当前blob SHA修订本批自己的新文件；旧件全部冻结，正式交付后本批也冻结。Contents接口无全局原子分支锁，保留竞态限制。写调用超时先查远端路径和历史；已成功则只补后续步骤，不盲重试。进度保全在本批六件合适位置或自己的评论，标明PARTIAL和下一安全动作，不新增第七件，也不声称回合结束后后台继续。

最终固定提交全文读回六件，再按当前分支核blob/头；compare 38a23958a355到结果只可有本批六路径A新增，无旧件净修改/删除或未知提交。实际后置结果写PR17交付评论，不为自引用结果提交反复改文件。完成后停写待C03；Owner只通知完成，主线从GitHub读取并同轮安排后继。

## 8. 禁止动作

不修内核/用户程序/旧脚本/旧成果，不生成内核补丁，不预处理/编译/运行C/ASM/内核/QEMU，不安装、挂盘、读凭据或仓库外libc/镜像，不直写master/time/lead，不合并关闭PR、删分支、force-push或进入阶段3。现状解释、风险记录和设计讨论均不自动成为实施授权。
