---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-EXEC-RESOURCE-OWNERSHIP-70
record_type: bounded_exec_registry_and_resource_ownership_contract
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报，非后台认证）
model_per_owner: gpt pro（主线Owner指定）
backend_identity: unknown_or_not_attestable
date: 2026-11-10
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: cf06a0d7f6da
lead_before_publication_short12: 44ba4f92b2b7
fixed_contract_commit: PINNED_BY_OWNER_STARTUP_URL
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
executor_display_name: MYOS2-A-C05b 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
executor_model_per_owner: 次一档 GPT-6＋Extra
executor_rotation: false
mainline_owner: MYOS2-A-C05
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-resource-ownership-70/
allowed_files: [MANIFEST.md, exec-resource-ownership.md, facts-and-boundaries.yaml, exec-resource-ownership-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R69-01]
mandatory_supplements: [R69-S01, R69-S02, R68-S01, R68-S02, R68-S03]
activation_gate: Owner向现有C05b转发本已发布固定入口并要求执行，且G00通过
acceptance_ceiling: PASS_PENDING_LOCAL
phase: 2
runtime_evidence: NOT_RUN
inputs_read: [69固定交付与主线回执, 固定exec.c注册及转交窗口, binfmts.h, elf_format及init_elf_binfmt, 23及42历史限定范围, 00/18/24/28/63/79与协议v2]
owner_decisions_requested_now: []
open_questions: []
---

# 第70批：exec格式注册、资源转交与清理边界

**继续现有C05b/CHAT-EXEC-08，次一档GPT-6＋Extra。** G00后先完成R69-01最小D00，再连续A/B/C及G04，六件一次交回主线，不逐专题等批准，不新开对话。本任务是静态分析，不是修复或运行授权。

## 1. 增量与不重复范围

69已连接初始exec桥、返回值与寄存器恢复，但仍以有效格式列表、bprm/文件/mm对象及清理正常完成作为前提。70查这些直接资源边界：A实际注册入口与分派对象；B bprm资源取得及移交；C失败/成功出口清理及别名计数。D00只修69的两条宏引文，不重做整个69。
不重验23/24的参数页、25的栈表、26–28的完整ELF段/解释器、41/42全部fd引用或15全部MM上下文；不展开全内核并发、SLUB、PID或调度器。普通疑点按18号记录延期，不要求Owner逐项选择修复，也不执行任何修复。

## 2. G00与固定输入

完整读本101号、同发布提交reviews/CORE-USER-INIT-EXEC-HANDOFF-69-review.md及checkpoints/2026-11-10-init-exec69-reviewed-exec-ownership70-ready.md。完整刷新63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md与协议v2全部P1–P13。其余项目约束及适用Mnemosyne指导只可复用本有效C05b实际已读原文；缺项补读，不借其它聊天的已读状态，不导入其它项目维护。
69为RETURN_DOCUMENTATION/R69-01，A/B/C核心按主线补充保留；67文档项已闭合，68为PASS_PENDING_LOCAL；66/65/64闭合、63/58主线空行误判撤回保持。
核PR16/17、direct lead/exec/master/time、开放PR及目标70目录。首次写前执行头应为cf06a0d7f6da；新目录应不存在，time固定a039d9803ade。主线本次正常发布101不算执行分支竞争；未知执行头变化须停相关写动作核归属。重新读取公约§1第2条和time/master双金丝雀，原样登记MANIFEST。
原69从cf06a0d7f6da读取evidence C39/C40及实际引用位置、MANIFEST检查说明、facts/deferred的IE69-F01/F02/F03及U03/U04等原条目，合读评论6764279513。历史至少定点读同主线固定reviews/CORE-USER-EXEC-STACK-23-review.md的§1–3、CORE-USER-FD-CLOSE-42-review.md的§2–3；涉及MM交接或解释器旧结论时再读CORE-USER-MM-CONTEXT-15-review.md、CORE-USER-ELF-INTERPRETER-28-review.md相应节。采用任何旧qualified_id前必须核原条目与真实状态，不复制第一波未核断言；未取得则明确缺口，不臆造旧ID。

## 3. D00：R69-01单定义重锚及传播

固定processor_const_arch.h中分别读取X86_EFLAGS_FIXED_BIT、X86_EFLAGS_FIXED、X86_EFLAGS_IF_BIT、X86_EFLAGS_IF。各自用真实1行定义形成独立证据，映射旧C39/C40，不把相邻宏合在一个标签下。
在新facts.prior_return_disposition与正文记录原evidence、实际引用这两项的flags矩阵/正文、旧MANIFEST/P9及正式评论的总括声明如何消费。原105是标签计数；至少两条不满足单定义门，不能再声称105条全部通过，也不能把其余103条自动认证或凭拆分改成107条全通过。
不改旧69任何文件/评论，不重取整包源链，不为凑配额增加无关引文。新70所有六件按原始全文统计真实保留前缀；非证据说明避免写出该前缀。D00只报documentation_disposition_submitted，由C05独立关闭。若实物与回执不符，提交可核异议，不迎合错误要求。

## 4. G01/A：注册对象到实际分派

固定mykernel/sched/forkexec/exec.c的formats对象、register_binfmt、unregister_binfmt、prepare_binprm和search_binary_handler；mykernel/include/linux/fs/binfmts.h的linux_binfmt及linux_binprm；mykernel/fs/vfs/binfmt_elf.c的elf_format、init_elf_binfmt、exit_elf_binfmt；mykernel/init/main.c中do_initcalls到init_elf_binfmt的直接调用窗口。
建立“对象初始化→登记调用→链接节点→实际遍历/回调→注销”的矩阵。只按实际include定位必要列表宏/帮助函数，不凭Linux通用实现猜这里的空表/首元素行为。搜索其他register_binfmt调用可作有界覆盖核查，默认分支搜索仅导航，结论回固定time。
分别说明已找到的静态对象/源级调用和未证的运行登记次数/顺序。不能因为只找到elf_format就断言全仓永远单handler；也不能以69的假设多返回序列证明实际有多handler。核被注释的锁/模块引用或提前返回仅在所读范围的影响，不外推全局必有竞态。补充IE69-F01真实前提，不自行关闭或重复编号旧根因。

## 5. G02/B：bprm取得与转交

固定exec.c完整相关定义：alloc_bprm、do_open_execat、begin_new_exec及exec_mmap；kernel_execve/bprm_execve只取衔接所需窗口，按69已审限制消费。必要最近直接被调定义可有界展开，资源深层实现保持opaque。
分别记filename.name、fdpath、interp、bprm.file/executable/interpreter、bprm.mm和current.mm/active_mm的来源、别名和归还义务；字段同值不等于新增引用，置NULL不等于物理释放。不得将linux_bprm对象与linux_binfmt注册对象混为一物。
begin_new_exec中de_thread、unshare_files、set_mm_exe_file、exec_mmap等门及bprm.mm=NULL、have_execfd分支需逐序核对，前置失败应在最早未闭合处停止。set_mm_exe_file真实定义从mykernel/sched/forkexec/fork.c固定对象定位；fput/get_file/mmput等从真实声明/include或正规搜索定位，只读必要直接接口，不重做完整文件/MM子系统。核转交成功与后续失败的区别，不把point_of_no_return字段名或注释视为失败自动终止已实现。
load_elf_binary只取begin_new_exec、解释器文件获取/释放及最终出口中涉及本包资源的窗口，不重做映射/栈/动态链接器。解释器局部file与bprm同名字段必须按真实赋值区分，禁止凭名称推断相同所有权。

## 6. G03/C：清理及引用事件账本

完整核exec.c::free_bprm、do_close_execat，以及alloc_bprm/kernel_execve/bprm_execve的实际调用/失败出口。逐个资源列“未取得、已取得未转交、已转交、错误返回、正常清理、不返回/失效”状态，不将所有失败合成一个平衡的资源释放路径。
特别核同一个bprm.file在free_bprm各语句及do_close_execat中的使用，真实fput语义和已有mm/exe引用对计数的影响。可用独立有限引用事件模型比较不同合法初始引用数/别名；一旦对象有效性不成立即停止，不在已可能失效对象上继续算确定负值、二次free或实测UAF。没有全生命周期证明时只登记条件性问题，不能从“出现两次fput”直接宣告实际释放两次。
free_arg_pages、mmput等若涉及23/24/15旧根因，按旧原件和回执限定；新工作只增加本调用者的取得/转交/清理关系，不升级为旧问题已修。格式列表、共享bprm、解释器与mm/exe之间的引用是否仍有效分别证明，不用返回0替代资源正确性。保留此前桥接忽略错误的IE69-F02身份，补证不是另造相同新bug。

## 7. 交付、证据与预算

严格六件：MANIFEST.md、exec-resource-ownership.md、facts-and-boundaries.yaml、exec-resource-ownership-paths.dot、deferred-findings.yaml、evidence.md。facts至少有prior_return_disposition、source_ledger、format_registration_matrix、bprm_ownership_matrix、cleanup_event_matrix、finite_model_cases、prior_findings_links。新条件项EO70-Fxx、缺口EO70-Uxx，无数量配额；旧根因保持原ID及状态。
强制锚：D00四个独立宏；A真实注册对象/登记/遍历接口；B资源取得与关键转交/失败门；C清理调用及最近引用操作。每条非空、逐字连续1–5物理行且在单一真实定义内。声明、注释、字段名和作者计数不能替代定义或命题支持。图分真实CALL、宏展开、局部顺序、DATA和ANALYSIS_STOP，别名/计数必须标前提。
核心及直接依赖软上限14源码文件，D00/规则/历史另计；最多8组定点搜索与4个相关目录，超限说明必要性，不全仓漫游。可以复用本有效C05b同固定对象已实际取得的原文，但新结论必须指出真实来源和本次覆盖；外围未知登记缺口，不省掉强制定义。
本任务适用协议v2全部十三条。MANIFEST含署名/Owner档位与后台未知、read_channel、公约原句、双金丝雀、self_check三数、实际P9逐项、write_retry_log。P6检查新六件原始全文而不只数evidence标题；ID存在和命题支持另核。P9-9/10/12无代码/补丁/降级则N/A；完整本地P9仍NOT_RUN；本次自建纯数学/文本检查可做，但不运行仓库代码/脚本/旧夹具。

## 8. G04及停止

Owner转发真实固定101入口并要求执行、G00通过后连续D00及G01–G04，不逐项等待批准。唯一写区为原PR17新user-exec-resource-ownership-70/授权六件及08本人唯一正式交付评论；全部旧文件、旧评论、PR元数据、lead/PR16、master/time冻结；C05不代写执行成果。
按63号：暂态异常先核远端是否已成功，再有间隔安全重试，通常首次加三次并遵守Retry-After；expected-head/force=false，不覆盖未知并行写入；复用已建对象，评论查重，发现次数与实际写请求分账。
写后compare自cf06a0d7f6da严格六件新增、旧件零净改删；固定新提交和活动分支全文/blob/HEAD、唯一正式评论均回读。只报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及D00/A/B/C范围；完成STOP_WRITING，返回真实短提交和评论交C05。不自关R69-01、不自签70通过、不自行开始71。
禁止内核改动/修复补丁、预处理/构建/链接/仓库程序或旧夹具/QEMU/实机运行、另开分支PR、强推/合并/关闭/删分支、跨仓维护、外部付费模型/子代理与阶段3推进。连接器权限不替代业务授权。
