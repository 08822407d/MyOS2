---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-VMA-LIFETIME-14
record_type: chat_vma_lifetime_static_analysis_contract
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-10-31
read_channel: connector
evidence_class: 后继工作包授权与验收条件，不是第14批已完成的分析
authorization_ref: Owner报告13完成；28号要求验收后同轮发布下一工作包
status: READY_FOR_OWNER_FORWARDING
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: MYOS2-A-C03a 代码分析执行
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
receiving_model_tier: next_tier_default
instruction_base_short12: bd25e19fd944
required_review_record: CORE-USER-FILE-CACHE-13-REVIEW-001
required_review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FILE-CACHE-13-review.md
source_branch: time
source_pin_short12: a039d9803ade
result_base_short12: 2d67bea2fe59
default_branch: master
default_branch_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-vma-lifetime-14/
allowed_files: [MANIFEST.md, vma-lifetime.md, facts-and-boundaries.yaml, vma-lifecycle.dot, deferred-findings.yaml, evidence.md]
existing_files_frozen: 2d67bea2fe59及之前全部文件，包括13和历史勘误
writer_assignment: C03a仅本批六件和自己的PR17评论单写者；正式交付后停写待C03验收
previous_executor_delegation: CC02旧委派不恢复；旧本地进程及未推送内容unknown
activation_gate: Owner向已有C03a转发固定任务入口，绑定与写前核对通过后L00至L04连续推进
external_execution_started: false
prior_status: file_cache13_PASS_PENDING_LOCAL_with_R13_S01_S02_S03_and_explicit_lifecycle_pending
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
  - 第13批六件全文、交付评论及本轮主线回执；既有18/24/25/28分工与延期规则。
  - 固定time mmap.c 1–235、400–640、650–805、1060–1310和arch/x86_64/myos/syscall.c 1–175，作为范围预检；未完成14的helper、实际用户入口或全部退出清理核验。
  - 默认分支munmap/__vm_munmap搜索只作导航，候选vm_map_api.h宏和系统调用表须回固定time核实。
  - 当前两PR、直接refs、PR16完整路径与13增量compare；本轮未修改执行成果。
open_questions:
  - 实际用户munmap/mmap入口、区间操作与对象释放是否完整连接尚待本批取证；不得把题目中的函数名当作实现保证。
  - 不认证真实硬件映射撤销、TLB一致性或全内存回收；普通发现仍只入库。
---

# 第14批：解除一个映射，实际解除了哪些东西

## 1. 目标与启动

第13批已限定接受，无新退回。其R13-S01/S02/S03和历史ER01/02在主线回执中已给出替代读法，本批合读即可，不另造勘误文件、不重做13的88引文或12的缺页分派。

新目标是补上13明确未闭合的VMA生命周期：建立/持有文件的最小来源 → 区间分割及摘链 → VMA对象、file引用、PTE、页引用和缓存槽是否各自解除。不得把“从VMA列表删掉”与“内存、页表和文件全部释放”合成一个步骤。用有限状态和引用表交付，不扩成完整MM/VFS或运行实验。

Owner向已有C03a发送：

> 执行CORE-USER-VMA-LIFETIME-14。完整读取32号固定任务及13主线回执，沿用已有接收和GPT-6＋Extra。按L00–L04连续完成VMA区间分割/摘链、文件引用和页表/页释放边界的静态分析、自查、原PR17提交及远端回读。仅写指定六件，旧件冻结；普通问题入库，不修内核、不重做12/13。完成后停写交C03验收，不自行开始下一批。

这是一份完整工作包，内部读取、核对、修订和交付不逐项请示。GitHub发布不自动唤醒聊天。遇实际读取失败、权限/输入冲突或不明并行写入，突出最小阻断；不依赖阻断的安全工作继续，完成范围不足就明确PARTIAL。

## 2. L00：绑定和增量输入

先读本任务及required_review_record全文，核任务、执行身份、源码/结果/指令三个不同固定对象。18号普通问题延期、24/25号执行验收分离、28号同轮派发继续有效，不重做25号接收，不沿用13已结束的写权。源码先读options_flags.cmake；仅配置文本，不能否定外部选项或假称已预处理。

旧结果固定2d67bea2fe59。13仅读C03所有权、关联状态及FC13-F05和主线补充；12只取VMA查询/PTE消费边界；05/04/11按实际相关的mm/VMA/file退出旧项定点引用并合读审查。普通问题用qualified_id做证据追加，不重建旧编号。无需整包重读、重跑旧检查器、复制旧巨大锚点库。

已有导航为mykernel/mm/vm_map/mmap.c、vm_map.h、vm_map_api.h及arch/x86_64/myos/syscall.c。其余按真实include/调用定位；默认分支搜索只导航，所有断言回固定time。主线预检只发现内部__vm_munmap与brk收缩等片段，**没有证明用户sys_munmap一定有活动实现或调用它**。

## 3. L01：真实入口与对象初态

定点核用户mmap/munmap相关实际定义、系统调用表绑定与内部vm_mmap/vm_munmap别名；不重做08分发全表。声明、宏、空体、注释和活动实现分别记录。若某用户入口缺失或空体，内部函数仍可按真实内部调用者分析，但不能叫作用户可达路径。搜索未命中不等于全仓无实现。

以simple_mmap_region的新VMA路径为对象初态：vm_area_alloc/vma_init、起止地址与vm_pgoff、vm_file=get_file、call_mmap、入链及失败清理。分清调用前已持引用、VMA新增引用、回调可能修改的对象及合并时是否释放临时VMA。只追影响本批引用/生命周期的分支，不展开所有地址选择算法、映射权限或文件系统。

列最小对象关系：mm.mm_mt链、VMA及其邻居、vm_file/file.f_count、vm_ops回调、PTE/页表、源缓存页/私有页。VMA复制仅复制指针与真实get_file/open操作分开；不能以拷贝一个结构体证明全部资源引用已增加。

## 4. L02：区间分割、摘链及失败副作用

完整读取__vm_munmap、simple_do_vma_munmap、__split_vma及直接依赖：find_vma_intersection、simple_find_vma、vma_prev_vma/vma_next_vma、vm_area_creat_dup、__simple_vma_adjust、__vma_link_to_list/__vma_unlink_from_list的实际定义。如名称是宏，核展开绑定；不能跳过中间helper画成直调。

把[start,end)与地址/长度的类型、页对齐、start+len溢出、0长度、范围上界检查分开；明确哪些由实际调用者检查、哪些未见。重点解释前驱为空、链首/链尾、区间边界正好相等，以及第一次/第二次分割失败时之前的副作用是否已经发生。源码出现无效指针、未赋值、无限循环或未定义前提后，不继续计算确定返回值。

形成有界代表矩阵：无交叠、整段删除、删前缀、删后缀、从中间挖洞、跨多个VMA、操作第一个/最后一个VMA、边界相等、必要的分配失败。不是要求跑例子或穷尽所有组合；同源情况可合并，指出实际覆盖与unknown。对vm_start/vm_end/vm_pgoff及链节点变化分别写前后条件，不把源码注释的Linux意图当作实际效果。

simple_mmap_region的覆盖旧映射、brk收缩和直接内部unmap若共用helper，列真实调用和参数差异；其他ELF调用仅用于必要入口交叉核对，不重做ELF加载或扩展完整brk实现。simple_vma_merge/__simple_vma_adjust只展开影响本批区间和资源处置的直接路径，不启动全映射策略研究。

## 5. L03：对象、文件、页表及退出连接

对每种实际操作分别记录：从mm链摘除、改变map_count/范围、调用vm_ops.close、fput(vm_file)、vm_area_free、清PTE、TLB失效、put_page/folio_put、清除page_array槽。每项标明活动调用/注释/潜伏定义/在限定范围未找到。缺少顶层free字样不能替代下层helper核查，释放函数存在也不能证明当前路径执行。

核simple_unmap_region的实际主体及调用者、所选unlink helper是否释放对象。跟到首次明确页表/引用/清理副作用或断点即止；完整页表遍历、allocator、TLB多核协议、LRU、truncate/writeback和全部回收消费者不展开。file减少一份引用不等于cache页被释放；13回执R13-S03的源缓存页put与私有cow页put必须保持对象区分。

退出只做有界连接核查：在固定do_exit里确认exit_mm是否活动；必要时读mmput/__mmput及所用清理端点，注明谁确实调用它们。不要重做11 PID退出或04内存分配全链，不从潜伏mm释放函数反推出每个进程退出必完成回收。fork的VMA复制仅在说明共享/引用前提确有需要时定点读取。

输出三类结论要分开：函数返回或区间不再被查询到；页表/硬件映射是否真的撤销；各对象引用或内存是否实际释放。返回0只代表源码相应返回路径，不能证明后两类完成。不存在运行证据时保持NOT_RUN；普通风险和旧FC13-F05/相关MM问题按18号入库，不要求Owner选择修复方案。

## 6. L04：六件交付与验收

严格只形成allowed_files六件。MANIFEST写L00–L04真实完成/partial；vma-lifetime.md给可读链路、区间矩阵、对象引用及失败状态；facts集中放精简节点/边/状态/所有权和锚点；DOT与facts语义一致；deferred仅新发现和旧证据增量；evidence给真实方法、负向检索范围和NOT_RUN。不增加第七份勘误、工具或日志库。

关键事实附真实路径、符号和1–5行逐字引文。调用/门条件分别给足证据；负向结论需要完整的所选函数上下文，不能用空行、闭括号或一行注释证明整个函数无释放。无引文数量目标，能复用就复用，不将长函数机械切成大量三行记录凑数。所有本批强制引文回源核对。

call=真实调用方→被调方，动态调用给调用点、绑定和目标；中间helper不能压成直调。data=消费方→提供方，回调表绑定保持表/槽→目标方向且注明非直接call；对象改写放effects。active只表示源语句活动，仍须保留早退与调用者前提。返回类型、内部对象、借用/新增引用分字段。

主动检查：摘链是否等于free；分割是否自动增所有引用；返回0是否等于PTE/TLB已更新；失败是否无副作用；首VMA有无前驱；清理函数存在是否等于退出调用。证据不足标unknown或PARTIAL，不用TODO冒充必要链路完成，不以旧PASS_PENDING_LOCAL压过新反证。

允许自己的临时沙箱使用已有文本工具核YAML/JSON、图/ID、引用；不安装、不执行仓库脚本、不建C/ASM夹具、不操作Owner工作树。人工与机械核对分开，未实际调用就NOT_RUN；不自称外部模型独立复核。无Deep Research、新API/模型或付费服务要求。

验收看真实入口、区间矩阵、失败副作用、对象引用及清理连接、条件证据和图一致、六件与真实diff。最终只回传READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING，不自签PASS、不关闭旧运行缺口。

## 7. 写入、进度保全和交回

复用原PR17/claude/dazzling-cori-q0dnyt；首次写前期望2d67bea2fe59。仅C03a获本批六件及自己的PR17评论权限，C03独占lead、PR导航和验收。禁止另开/改名分支或PR、改PR标题/正文/Draft状态或他人评论，CC02旧委派不恢复。

写前复查master直接ref、time、执行头、开放PR与路径。主线准备时只见16/17，lead旧路径及13增量与本批前缀分离，执行方仍要复查。若结果目录已存在或头不明漂移，先确认来源/是否已交付，不覆盖、不重复执行；time前进时仍以固定对象作历史分析并登记新头，不静默换源。

六件可顺序提交；以上一笔自己已核结果为期望头，交付前只按当前blob SHA修订本批自己的文件。全部旧件冻结，正式交付后本批亦冻结。Contents接口无原子分支头锁，必须保留竞态限制。工具写调用中断或结果不明时，先查远端路径/历史再决定是否补动作，不盲重试；13的恢复经验不意味着以后可假定写调用必成功。

长步骤用已有六件的适当文件或自己的评论保全可信进度，记录已读/未读、当前短头、下一安全动作；不新增第七件，不声称回合结束后后台继续。每轮已有授权内尽量推进到交付，普通发现只入库。

写后以最终固定提交完整读回六件，再按分支核blob/头；compare旧基线到结果必须仅本批六路径A新增、无旧件净修改/删除或未知提交。后置核验放自己的PR17交付评论，给实际短提交、文件入口、diff/回读结果与限制，避免自引用循环修改。完成后停写，Owner通知C03即可；C03依28号同轮验收和发布后继，不自动唤醒对话。

## 8. 原禁止动作不变

不修内核/用户程序/旧脚本/旧成果，不生成内核补丁，不预处理/编译/运行C/ASM/内核/QEMU，不安装、挂盘、读取凭据或仓库外libc/镜像，不直写master/time/lead，不合并关闭PR、删除分支、force-push或进入阶段3。材料静态接收不等于映射解除和资源回收在硬件上通过。
