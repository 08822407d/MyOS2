接收及执行模型：次一档（默认GPT-6＋Extra）。必须新开MYOS2-A-C06a 代码分析执行，身份CHAT-EXEC-10；独立验收仍为C06的Pro。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-MEMBLOCK-REGION-LIFECYCLE-79
record_type: bounded_memblock_region_mutation_and_query_contract
evidence_class: 主线设计和限定权限；非执行交付或验收
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner明确选择）
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 03519ebb5d86
lead_before_publication_short12: ea5eb636ae89
fixed_contract_commit: OWNER_FORWARDED_113_STARTUP_AND_114_SAME_COMMIT
receiving_model_tier: next_tier_default
executor_display_name: MYOS2-A-C06a 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
executor_rotation: true
old_executor: MYOS2-A-C05c / MYOS2-LEAD-002-CHAT-EXEC-09
old_executor_state: STOP_WRITING_OWNER_RETIRED_CONTEXT
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memblock-region-lifecycle-79/
allowed_files: [MANIFEST.md, memblock-region-lifecycle.md, facts-and-boundaries.yaml, memblock-region-lifecycle-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: [R78-01]
D00: REQUIRED_DOCUMENTATION_ONLY
R77_01: CLOSED_DOCUMENTATION_ONLY
batch77_composite: PASS_PENDING_LOCAL
activation_gate: Owner在新C06a转发固定启动词且独立接收/九规则与110/Mnemosyne指导/G00全部完成
status: READY_FOR_OWNER_NEW_CONVERSATION_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
---

# 第79批：补正文档，追踪memblock区间的登记、改写与查询

Owner已明确旧C05c上下文过长、不能继续；本包只由新C06a/10承接。必须先按113-executor-continuation两件完成接收，不继承任何旧对话已读声明。所有门完成后连续D00、G01–G04，不逐专题等待批准，也不要求旧09另行签确认。C06保留唯一主线写权，10仅在本79目录有条件写入。

## 1. 三专题目标与边界

78依赖memory/reserved有序、区间有效、计数与容量一致，却没有完整闭合这些记录的改写来源。79围绕三个接合面：A新增区间的切分和遍历，B插入/合并/删除的数组和统计变化，C裁齐、查询及预留成功的消费条件。建立实际输入、对象身份、前后状态和最早停止点，不重复78的页描述符初始化或77的buddy取还页分析。

只做阶段2有界静态取证。PB78-U02/U06及F01–F06是上游条件，不自动关闭；既有MC、GAP-M08、MR-02、PA77及SL76/AE75继续原qualified_id。只有真正不同根因才新编号，不能把同一cnt/尾槽问题分裂成多个“新bug”。不扩成固件/E820全流程、引导器、完整页表、NUMA、热插拔、全部memblock调用者、SLUB/buddy/VM、全树别名、锁原子实现或修复方案。

## 2. G00：接收、规则和固定输入

L=agent-workspace/lead/MYOS2-LEAD-002/，取Owner转发113启动词和本114的同一固定发布提交；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/，取03519ebb5d86；内核只取a039d9803ade。

113/00交接包和113/01启动词为本批必读，严格顺序：形成Mnemosyne接收报告；独立全文加载九份MyOS2约束；追加独立全文110；再单独加载交接包指定适用Mnemosyne指导并记录任务未被替换；随后完成本G00。不能把已读包或本114当作已读九规则，不能继承C05c/09、C06或其它项目的加载声明。临时接收和指导记录在获写权后并入新MANIFEST/evidence，不新增第七件。

L下全文读取本114、110、reviews/CORE-USER-PAGE-BOOTSTRAP-HANDOFF-78-review.md、reviews/CORE-USER-PAGE-ALLOCATOR-LIFECYCLE-77-DOC-CLOSURE-review.md、reviews/CORE-USER-PAGE-ALLOCATOR-LIFECYCLE-77-review.md、checkpoints/2026-11-12-bootstrap78-reviewed-memblock79-exec10-ready.md。必须合读R78-01与R78-S01/S02/S03、R77-S01/S02/S03；77已组合PASS_PENDING_LOCAL、R77-01已关闭，78仍RETURN。不要重开73/74及58/63历史已闭合或撤回项目。

R/user-page-bootstrap-handoff-78/六件全文作为交接及D00输入，正式评论6825390768全文；特别核facts的prior_return_disposition、source_ledger、各区间/失败/最早停止矩阵及deferred各qualified_id。原77无需重做121条源码或36模型，按两份主线77回执消费。历史限定定点读：R/mm-baseline-04/facts-and-dependencies.yaml的MC-01–MC-05、LB-01/LB-08、GAP-M08；R/user-vfs-baseline-05/mm-04-disposition.md全文及deferred中MR-02/status_changes；L/reviews/CORE-MM-BASELINE-04-review.md §3、CORE-USER-VFS-BASELINE-05-review.md §2–3。新对话须自己读取这些指定范围；不全量漫游历史成果或旧脚本。

直接核PR16/17、lead/exec/master/time、开放PR、新79目录与10本批正式marker。执行起始头必须03519ebb5d86；time为a039d9803ade，master直接头预期e8e60cd19540；lead以本次114实际发布提交为准，不把父ea5eb636ae89当新头。PR缓存base_sha不代表master现状。未知推进先核归属，不覆盖；已经存在的目标/marker先识别本批是否交过，不重复创建。

实际回读公约§1(2)整句、固定time/master两金丝雀，保留startup_selfcheck_quote、branch_canary_quotes、read_channel和模型来源。只读门全部完成才由10取得本批限定执行写权；09没有本批写权。不得因旧方不能再回话制造额外等待门，也不得把发布文件等同新聊天已启动。

## 3. D00：R78-01追加纠正

直接回读原78 A31和固定源码page_macro_arch.h::__va，原引文的ifndef行与宏定义分离；新证据只取真实宏定义内连续原文，条件编译前提写在锚外。新正文/facts.prior_return_disposition/MANIFEST/正式评论说明原错位、替代锚和原件冻结；不能宣称原包136条原来全部满足P2。

逐项落实78回执S01的索引表及两处PB78-F06→PB78-F05身份纠正；核对应实际原标签，而非仅照抄表。特别处理F04的B06+B11+B12、F05的B13+B19+B44、锁初始化B15、memmap覆盖B19、internal重试缺锚，以及把B03当free_area_init内部赋值的错误。与相关正文/facts/deferred总括建立明确替代映射，不需要重新生成整份旧报告。

确认S02的两种物理失败路径、边界数组先于pgdat清零、跨阶段图边，及S03的尾后一指针/解引用区别、normal首次下界与retry、31源码模型记录＋1文档计数记录、候选前提已被消费。原78文件/评论一个字不改；纠正仅在79六件内追加，执行者仅报DOCUMENTATION_CORRECTION_SUBMITTED_PENDING_C06，不自关R78-01，不重开已关闭R77-01。

## 4. G01/A：登记新范围的切分和游标

核心固定源：mykernel/mm/early/memblock.c的memblock全局、memblock_cap_size、simple_mmblk_add、simple_mmblk_reserve、simple_mmblk_add_range；memblock_macro.h的for_each_memblock_type；memblock_types.h及memblock_const.h。沿实际包含关系补phys_addr_t、size_t、ulong、ENOERR、PHYS_ADDR_MAX及必要min/边界宏，不能凭常见Linux实现替本树。

区分memory与reserved、有效cnt和数组capacity、空静态槽与有语义的哨兵、物理字节base/size/end以及遍历idx/rgn。列零size、范围截断、空表、前插/尾插、相邻、重叠/被包含/跨多段的代表条件模型；不对所有组合凑笛卡尔积。

严格按真实先后处理rgn-1指针形成、idx>0后置条件、尾槽字段读取、rend和base的比较、insert_size生产、base前进、插入后合并参数和循环结束。带符号idx、ulong形参、size_t/phys_addr_t差值与窄化转换分账；不能把未知实现转换统一叫“回绕”，也不能把后置idx判定作为先前指针形成的保护。前置条件不能证明时停止该条实际路径，后续分支表显式标隔离假设，不宣布所有登记都失败或都成功。

输出range_intake_matrix、overlap_slice_matrix、cursor_and_earliest_stop_matrix；强制锚覆盖两个入口真实分派、cap/零门、循环宏、前置指针及后置检查、各片段生产与真实调用实参、返回及容量门。注释声称允许重叠不等于活动代码已证明覆盖所有重叠。

## 5. G02/B：插入、合并、删除的状态账

完整读memblock_insert_region、simple_mmblk_merge_rgn、simple_mmblk_remove_rgn及其直接调用位置；type/region类型和静态容量初始化分开锚定。必要时只沿最近BUG_ON定义/配置和memmove接口确认停止与请求语义，不扩审整个panic或字符串库；未证原语效果须保留返回与存储前提。

插入列idx/cnt/max验证时点、源/目标搬移范围和字节长度、base/size/flags实际赋值、cnt/total_size变化。形参出现flags不等于结构中确实写了flags；静态零初值、已有槽内容、搬移复制和后续写者需分开。指针越界形成、零长度请求是否仍需有效指针、算术溢出、BUG_ON触发均在最早位置记边界，不编造失败返回或回滚。

合并列start/end传参生产和实际转换、i初值/步进、邻区地址关系、flags相等条件、size增加、total_size先加后由remove减、删除后数组前移与cnt/end变化。未走合并与走一次/多次合并分开；同一个槽被更新后不得沿用旧指针对象内容假设。删除只按实际rgn_idx和cnt范围建模，不把“尾部不用清”的注释当迭代器永不访问尾部的证明。

在合法操作与正常返回假设下核coverage、排序、不重叠、cnt/capacity、total_size与有效区域大小和这些不变量；非法前序后的表只作反事实。输出insert_state_matrix、merge_remove_matrix、region_invariant_matrix、partial_write_and_stop_matrix。每个状态变化必须有真实短锚，不以文件行数代替工作量。

## 6. G03/C：裁齐、查询与预留结果消费

核心memblock.c的memblock_trim_memory、memblock_is_region_memory、memblock_alloc_range及simple_mmblk_reserve的返回连接；memblock.h的memblock_search、memblock_is_memory、memblock_is_reserved、memblock_is_map_memory；memblock_macro.h的for_each_memory_region_topdown。对free范围遍历只读与本批区间状态接合所需的__simple_next_mem_range和should_skip_region，不重做78分块交给buddy。

裁齐列orig/new半开区间、向上起点/向下终点、保留/缩小/删除、反向遍历r指针和cnt变化、total_size是否跟随真实活动写入。指针降到数组之前、索引变更和下次循环求值必须按源顺序，不凭“从后向前”名称证明安全。

查询列空cnt、单段、边界命中、gap、重复/重叠或无序输入（反事实）下left/right/mid、首次读取与终止条件。查询到下标不等于请求整个范围都包含；memblock_is_region_memory的cap和end校验、reserved和memory各自集合、NOMAP读取分别取证。宏名/注释不代替真实谓词，查询是否终止必须给有限前提。

把“range找到候选→reserve返回0→返回物理地址”与“区间确实已正确登记、未覆盖保留区、最终可写”分开。只消费本批已核的producer，不把78的失败哨兵或PFN索引问题当已修；外层未检查返回、别名/flags更新不全则保留缺口。不沿这些接口展开全部启动调用者。

输出trim_transition_matrix、search_boundary_matrix、reservation_consumption_matrix和跨A/B/C反例。强制锚包括裁齐/删除与迭代宏、search真实循环边界、包含性/flags谓词、reserve返回消费及最早停止；不执行C或测试程序。

## 7. 六件与验收门

六件严格见头部；facts含source_ledger、prior_return_disposition、上述矩阵、finite_model_cases、prior_findings_links；DOT区分CALL、CALLER_SEQUENCE、DATA、MACRO、CONDITION、ANALYSIS_STOP和隔离HYPOTHETICAL_SEQUENCE。共享节点不能拼接不同调用者的失败出口，不把数据节点当函数，不把停止边界画成内核goto。

核心与最近源码软上限22份（金丝雀另计）、8组定点检索、4次相关目录浏览；必要超限说明具体依赖，不全仓漫游。新短锚软上限130、源码有限模型最多36、新候选/缺口各最多12；都是上限，不是配额。D00文档计数与源码模型分账。新候选MR79-Fxx、缺口MR79-Uxx，保留旧qualified_id，普通代码问题延期记录而非问Owner逐项修复。

本任务适用agent-workspace/tasks/00-gpt-task-protocol-v2.md全部十三条。P9-1六件完整40位hex串0命中；2/13每条强制源码锚为真实单一定义内非空连续1–5物理行，重新打开固定源逐条核，宏条件指令不冒作宏定义本体，空白物理行不自动判假；3路径真实并写后固定/活动回读；4行号仅辅助；5六件原始全文前缀计数包含说明文字，不只数解析标签，MANIFEST三数自洽；6外部事实有真实出处，不以通用Linux知识补本树；7/8金丝雀、公约整句/read_channel齐备；9/10/12无代码/补丁/降级则N/A；11运行边界如实。完整本地P9、编译/链接及内核运行仍NOT_RUN。

文档引用不支持断言时纠正、收窄或删除，强制锚不得静默降级。不得把接收报告、规则读取或指南刷新冒充技术分析工作量，也不能把原作者的回读次数当新10已经完成的检查。

## 8. G04：原分支安全发布与停写

10仅在原claude/dazzling-cori-q0dnyt/PR17新增79目录六件及本人一条正式顶层交付评论。marker：MYOS2-LEAD-002-CHAT-EXEC-10 / CORE-USER-MEMBLOCK-REGION-LIFECYCLE-79 / FORMAL_DELIVERY。旧results/旧评论、PR标题正文/Draft、lead/PR16、master/time冻结。不是重用09的评论身份，也不更新09记录。

首次写前重新核exec=03519ebb5d86及目标/marker；用完整旧HEAD作为expected_sha、force=false及单父提交。未知并行推进不覆盖。GitHub暂态先查真实远端效果，再按63有间隔安全重试，通常首次加最多三次、遵Retry-After，复用成功对象，仅补缺项；不能一次异常就停工或重复创建已成功的提交/评论。工具发现与真实写尝试分别记录。

compare起始头→新头必须恰六件added、旧件零改删；新固定提交和活动执行分支六件全文及blob、四direct refs、PR16/17、唯一正式评论全部回读。真实SHA/评论号返回后才记录。新接收和指南报告纳入六件中的MANIFEST/evidence，不追加第七件。

完成只报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、R78-01纠正待C06；随后STOP_WRITING，通过PR17交回现MYOS2-A-C06以Pro独立验收。不交旧C05/C05c，不自签78/79，不启动80。Owner只需新建对话并转发一次完整启动词，不搬运ZIP或长报告。

禁止内核/loader/initramfs/Owner脚本修改、补丁、预处理/编译/链接、仓库程序/脚本/旧夹具、Graphviz、QEMU/实机、真实并发、外部libc/镜像、本机凭据、额外模型/子代理/API付费、合并关闭/强推/删分支、新分支PR、阶段3及跨项目维护。新建临时纯文本检查和不执行仓库代码的有限模型仅按原许可使用，必须与内核运行分开。
