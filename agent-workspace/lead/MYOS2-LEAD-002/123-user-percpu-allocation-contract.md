接收及执行模型：次一档，沿用GPT-6＋Extra。执行位置：现有“MYOS2-A-C06b 代码分析执行”，身份MYOS2-LEAD-002-CHAT-EXEC-11；独立验收仍交MYOS2-A-C06使用Pro。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PERCPU-ALLOCATION-87
record_type: bounded_percpu_fit_chunk_routing_and_concrete_consumer_contract
evidence_class: 主线设计与限定写入授权；不是87执行或运行成果
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner明确选择）
date: 2026-11-14
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 596fb3d7e1a5
lead_before_publication_short12: d2edb61330d9
fixed_contract_commit: OWNER_FORWARDED_123_FIXED_URL
inputs_read: [122合同与86六件及正式评论, 86独立回执, 00/18/24/28/63/79/110及协议v2, 固定percpu分配器与workqueue实际调用者预读]
receiving_model_tier: next_tier_default
executor_display_name: MYOS2-A-C06b 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-11
executor_rotation: false
mainline_owner: MYOS2-A-C06
old_EXEC10_and_EXEC09: STOP_WRITING_OWNER_RETIRED_CONTEXT
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-allocation-87/
allowed_files: [MANIFEST.md, percpu-allocation.md, facts-and-boundaries.yaml, percpu-allocation-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R86-01]
D00: REQUIRED_DOCUMENTATION_ONLY
batch86_acceptance: RETURN_DOCUMENTATION_ONLY_WITH_RETAINED_TECHNICAL_CORE
activation_gate: Owner向现C06b转发本123固定合同并要求执行且G00通过
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第87批：每CPU空间的分配记账、扩块与实际消费者

86已经独立审核，当前RETURN仅R86-01自检文档补正；技术主干按R86-S01/S02/S03保留，不重做整包。87先在新六件内追加D00，再完成三个关联专题。继续现C06b/11，不轮换，不向旧C06a/10或C05c/09索取确认；G00通过后连续D00、G01–G04，不逐专题等待批准。

## 1. 增量范围和真实入口

86保留了PC86-F05及U09的动态使用缺口。87核A请求与对齐/容量记账，B保留/普通路径与扩块/失败，C返回指针如何被一个真实工作队列调用链消费。共用同一perCPU分配器与六件，不重复first chunk启动分析，也不扩成完整工作队列或多核运行研究。

核心固定源码均为time@a039d9803ade：mykernel/mm/misc/percpu.c中的PCPU_GFP、simple_pcpu_find_block_fit、simple_pcpu_alloc_area、simple_pcpu_create_chunk、simple_pcpu_alloc_noprof及相关全局；mykernel/include/linux/smp/percpu.h、percpu_chunk.h；mykernel/arch/x86_64/include/asm/percpu.h中真实per_cpu_ptr/this_cpu_ptr/SHIFT_PERCPU_PTR链；mykernel/include/linux/compiler/compiler.h::RELOC_HIDE。

已定位的真实消费者是mykernel/kactive/workqueue/workqueue.c::simple_alloc_and_link_pwqs与simple_alloc_workqueue。主线预读740–920及900–1120范围取得这些完整定义及workqueue_init_early的源调用点，未证明实际启动到达或工作项已经运行。执行者应读上述实际函数及所需workqueue_s、pool_workqueue_s、cpu_pwq字段/声明和宏，而非用默认分支搜索片段代替固定time。

最近依赖包括ALIGN/round_up、ENOMEM、GFP位、kzalloc及kmem_cache_alloc/free的实际包装、list_header_for_each_container和list_header_add_to_tail、CPU遍历和类型定义。只沿真实include/调用补最近层，不重审整个slab/buddy/memblock、锁库、调度器或全部workqueue生命周期。遇到旧已登记根因，保留原qualified_id并补新证据，不重复计数。

## 2. G00：增量读取及现场

L=agent-workspace/lead/MYOS2-LEAD-002/，R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。工作材料取Owner转发123 URL中的真实固定新发布提交，不能用父d2edb61330d9代替。执行起点596fb3d7e1a5；原86六件固定在该提交，正式评论6870949847；源码仍time固定a039d9803ade。

完整读取本123、L/110-owner-communication-and-delivery-continuity.md、L/reviews/CORE-USER-PERCPU-BOOTSTRAP-86-review.md、L/checkpoints/2026-11-14-percpu86-reviewed-allocation87-ready.md及86正式评论。合读R86-01和R86-S01/S02/S03，保留R85/R84及实际依赖的R83/R82/R81历史限定；消费具体旧结论时须合读其正式回执。84的R84-01保持已闭合，85保持限定PASS_PENDING_LOCAL，不重开。

重新全文读取63号、79号、协议v2全部P1–P13。其它九规则、119接收和Mnemosyne指导、86原六件仅可按本有效C06b自己实际已读且固定对象一致的范围明确复用，缺段补齐。逐项区分本轮新读、同会话复用、未读；不继承C06或旧执行者的资格，不倒造加载日志，不导入其它项目维护任务。

核PR16/17、lead/exec/master/time四direct refs、开放PR、87目标目录及本人正式marker。exec预期596fb3d7e1a5、master直接e8e60cd19540、time a039d9803ade，lead以123真实固定发布头为准；不拿PR缓存base_sha当master真值。已有本批结果先核实归属和完成情况，不重复发布；未知并行推进不得覆盖。

独立核公约§1(2)整句、两分支金丝雀、read_channel；options_flags.cmake先全文读取再作配置范围判断。G00前只读/STOP_WRITING，门通过后取得87限定写权，无需再次等待Owner或旧对话确认。

## 3. D00：R86-01追加自检纠正

严格按86-review§2核原六件：evidence中77个定义锚，facts中1个说明文字裸标签，其他四件0，总原始前缀78；原清单77/77/0是冻结作者记录。逐项独立回查真实原件，记录文件、字段路径和性质，不能仅复制本合同数字当作已验证。

在87新正文/facts及必要MANIFEST字段内追加纠正，撤回86原“全包自检计数一致”总括；不改86六件、旧评论或原77/77/0，不伪造第78条源码事实，不倒造曾经运行的机械扫描。三种数量——真实定义锚、解释字面量、原始前缀总数——分别记录。追加说明自身若出现保留前缀，也必须进入87全包计数；更宜用不会制造额外标签的说明方式。

87本身的self_check三整数按本批真实原文重新核算，含D00新增锚而不重复计旧77为新证明。补正状态仅CORRECTION_SUBMITTED_PENDING_C06_REVIEW；R86-01只能由本C06独立关闭。D00不是第四个新技术专题，完成后继续G01–G04，不等待中途批准。

## 4. G01/A：请求、对齐、容量与提交记账

完整读simple_pcpu_find_block_fit和simple_pcpu_alloc_area及真实ALIGN依赖，沿size_t size/align进入int size、int alloc_offset/need_size/start和int元数据的转换逐步列式。分开合法值模型、实现相关转换、可能的有符号算术越界、移位/对齐前提，不能用宿主任意精度或擅自二进制截断替代真实C语义。

跟踪free_offset、free_bytes、alloc_offset、need_size及start+size的前后值；同时计算对齐填充、请求长度、真实数据块范围和单CPU单位范围。检查适配判据与提交时扣账是否一致，但不预设具体bug或修复方式。区分静态单调用、顺序多调用及依赖外部并发的假设；while条件不返回、错误返回、成功返回和仅停止证明分别记账。

交allocation_request_and_fit_matrix、alignment_and_capacity_matrix、area_commit_and_failure_matrix。模型选0/小请求、对齐1或合法2次幂、非2次幂/0的明确前提、恰好容纳/不足、存在填充、连续两次分配、size_t转int边界。对于固定链接/分配上下界排除的巨大值，仅作隔离算术，不冒称可达启动输入。强制锚包括真实函数、类型、对齐宏及每个条件和记账赋值。

## 5. G02/B：保留路径、普通列表、扩块和失败

完整读simple_pcpu_alloc_noprof和simple_pcpu_create_chunk，连接PCPU_GFP及调用者传参。核reserved且有保留块、reserved但保留块为空、普通块成功/都不合适、扩块成功/失败及restart的分支，不用共享图节点拼出不存在的fallback。精确区分申请元数据、申请data、base_addr计算、入链和返回时机；data失败不能只因元数据非空而当成功。

核当前活动锁调用和已注释锁行，实际列表初始化/迭代/插入包装须定位定义；注释中有锁不是同步已经生效。仅说明所读路径的同步前提与未审调用者条件，不进行真实并发实验或全系统竞态认证。循环不进展的候选必须说明每次重试的状态、大小/对齐是否变化、扩块结果及返回门，不以函数名推断终止。

将first、reserved及后续新块各自的data、base_addr、free_offset/free_bytes初值与86的记录接上，只读必要字段，不重做模板复制/GDT。分开早期memblock清零、后期kzalloc清零、area_found中显式逐CPU清零是否活动，禁止把其中一个替代全部生命周期。

交reserved_normal_routing_matrix、chunk_creation_publication_matrix、restart_lock_and_failure_matrix。模型覆盖保留块不足、无保留块、第一/后继普通块命中、空列表、元数据失败/data失败、持续无法容纳请求等；给出精确返回或止证位置。内存供应器正确性只按最近包装和历史回执保留，不重跑旧slab/memblock分析。

## 6. G03/C：返回编码与一个实际工作队列消费者

将area_found返回表达式与per_cpu_ptr/this_cpu_ptr的真实解码并列。每个量标清是合法对象指针、存入指针类型的相对编码、链接符号地址、unsigned整数偏移还是CPU索引。按所选ABI逐步代入first/reserved/new chunk，区分整数模运算、GNU void指针扩展及有效对象访问。参照R86-S01，不把合法无符号回绕误判为算术异常，也不把其数值一致当作地址合法证明。

只核workqueue.c::simple_alloc_and_link_pwqs内alloc_percpu(pool_workqueue_s *)得到的cpu_pwq、per_cpu_ptr计算、逐CPU槽写入/读取、分配失败出口与现有清理动作，以及simple_alloc_workqueue对其返回值的实际处理和wq发布时机。WQ_UNBOUND/高优先级/普通/BH等只追影响这段调用和返回的分支；不扩展工作项入队、worker调度、完整销毁或rescuer算法。

读真实cpu_pwq类型、有关结构字段、alloc_percpu宏到实际分配函数的完整链及所需列表包装；对free_percpu注释、未取得的释放实现或外部caller条件如实记录，不能声称完成回收。workqueue_init_early至simple_alloc_workqueue仅作源码调用关系，不由函数名宣称启动已到达。已有旧workqueue相关记录若与本次重合，定点合读回执并保留原ID。

交returned_pointer_decode_matrix、workqueue_alloc_use_cleanup_matrix、cross_stage_proof_gates_matrix及A/B/C接合表。代表例选CPU0/CPU1、单CPU/多CPU、首块/新块/保留块、分配NULL与部分槽已写后失败、局部返回未被调用者采用；逐例列存储范围、前序返回与映射条件。未取得真正合法写入地址时停止该路径证明，不继续模拟真实破坏后果。新候选PA87-Fxx与缺口PA87-Uxx分账，旧PC86/BM80/PT81/DM82/TI83/ER84/BB85不换号冒充新根因。

## 7. 六件与发布前质量

facts含source_ledger、D00逐件纠正、上述三专题矩阵、条件模型、事实键→证据ID、prior_findings_links及新读/复用账；DOT区分真实调用、宏/数据关系、同调用者源序和假设，不把重复调用的共享节点变成同一个运行对象。

源码软上限24份（金丝雀另计）、8组定点检索、4次相关目录浏览；必要超限具体说明最近依赖，不全仓漫游。含D00的新短锚软上限120，新专题模型最多30，候选和缺口各最多12；均为上限非配额，不靠材料长度充当实际工作量。

协议v2全部P1–P13适用。MANIFEST十四字段必须齐备：task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。模型来源仅据可见/Owner信息，后台不可独立认证。

P9-1六件40hex零命中；2/13每个短锚在真实单一定义内部连续1–5行并支持论点；3路径与真实固定/活动回读；4行号辅助且不越界；5核六件全部原始前缀而非只数编号标题，说明文字/字符串/D00同样在范围内；6外部工具/硬件语义用实际读取的官方具体出处和适用版本，否则保留缺口；7/8公约整句、金丝雀及read_channel齐备；9/10/12无代码/补丁/降级则N/A；11未运行项明确NOT_RUN。完整本地P9仍待本地，不自签PASS。

先冻结事实和模型中间值，再逐项核正文、机器表、deferred和图的语义支持。任何已发现错误须在本批发布前纠正，不仅保证证据ID存在或字面命中，不把源内调用/注释当观测日志。

## 8. G04：原分支发布、回读及停写

唯一新增区为头部87目录，严格六件，另加11本人一次PR17正式顶层评论。marker为MYOS2-LEAD-002-CHAT-EXEC-11 / CORE-USER-PERCPU-ALLOCATION-87 / FORMAL_DELIVERY。旧成果/评论、PR元数据/Draft/base、lead/PR16、master/time、内核及脚本冻结。

写前再核exec=596fb3d7e1a5、目录和marker，以真实完整旧HEAD作expected_sha，force=false、单父提交。暂态按63先核远端实效再间隔安全重试，通常首次加三次并遵Retry-After；已成功对象/评论复用，不重复创建，未知并行推进不得覆盖。工具发现与真正请求分账。

compare必须严格六件added、旧件零净改删；新固定提交六件全文、活动分支六件内容/blob、四direct refs、PR16/17与正式评论完整内容/编号/唯一性均远端回读。真实提交与评论号由后置正式交付记录承接，不预填、不另交第七件。

完成仅报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、R86-01追加纠正待C06复核，然后STOP_WRITING，经原PR17交回“MYOS2-A-C06 内核分析主线续建”作Pro独立验收。不自行签86闭合或87通过，不启动88，不交旧执行对话。

禁止内核/loader/initramfs/Owner脚本修改和补丁、CMake配置、预处理/汇编/编译/链接、仓库程序/脚本/旧夹具/Graphviz/QEMU/实机运行、真实并发/硬件实验、本机凭据、外部libc镜像、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3及跨项目维护。仅依既有许可可另建独立临时纯文本检查或有限整数模型，不复制运行仓库程序，并准确标记非内核运行。

下一步写入仅11通过G00后的87六件和本人正式评论；C06不并写执行results，旧10/09停用。110号必须继续显式进入以后每次主线交接包、接收启动词和固定必读清单。
