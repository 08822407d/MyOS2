接收及执行模型：次一档，沿用GPT-6＋Extra。执行位置：现有“MYOS2-A-C06b 代码分析执行”，身份MYOS2-LEAD-002-CHAT-EXEC-11；独立验收仍交MYOS2-A-C06使用Pro。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PERCPU-BOOTSTRAP-86
record_type: bounded_percpu_template_first_chunk_offsets_and_boot_cpu_switch_contract
evidence_class: 主线任务设计与条件写权；不是86执行成果
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner明确选择）
date: 2026-11-14
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 2489ea8e6963
lead_before_publication_short12: dd1e329a035c
fixed_contract_commit: OWNER_FORWARDED_122_FIXED_URL
inputs_read: [121合同与85六件及正式评论, 84原回执与D00, 00/18/24/28/63/79/110及协议v2的本有效C06固定对象, percpu.c与setup_percpu.c等设计预读]
receiving_model_tier: next_tier_default
executor_display_name: MYOS2-A-C06b 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-11
executor_rotation: false
mainline_owner: MYOS2-A-C06
old_EXEC10_and_EXEC09: STOP_WRITING_OWNER_RETIRED_CONTEXT
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-bootstrap-86/
allowed_files: [MANIFEST.md, percpu-bootstrap.md, facts-and-boundaries.yaml, percpu-bootstrap-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: []
D00: NOT_REQUIRED
batch84_composite: PASS_PENDING_LOCAL_R84_01_CLOSED_DOCUMENTATION_ONLY
batch85_acceptance: PASS_PENDING_LOCAL_WITH_MANDATORY_R85_S01_S02_S03
activation_gate: Owner向现C06b转发本122固定合同并要求执行且G00通过
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第86批：每CPU模板、首块空间与启动CPU切换

84的R84-01已由本C06独立关闭；85按R85-S01/S02/S03接收，无新增RETURN或D00。继续现C06b/11，不新开、不轮换、不再向旧10/09索取确认。先G00，通过后连续G01–G04，不在每个专题后等待批准。

## 1. 三个专题与不做的事情

85连接了构建输入、偏移生产和启动消费者，但真实运行尚未证明。本包转向源码中的下一接合面：A链接per-CPU模板到早期first chunk的分配/复制；B单位偏移与CPU索引的发布及访问公式；C启动CPU从旧状态切到新GDT/GS基础的局部源顺序。三个专题共用一套固定证据和六件，不重算85构建链，不重做81–84页表或旧任务生命周期。

核心固定入口：
- mykernel/mm/misc/percpu.c：相关全局、early_pcpu_create_chunk、simple_pcpu_setup_first_chunk；simple_pcpu_create_chunk/simple_pcpu_alloc_noprof仅在需要辨别字段消费者时定点读，不扩成全部动态per-CPU分配器审计。
- mykernel/arch/x86_64/smp/setup_percpu.c全文：初始this_cpu_off与__per_cpu_offset、setup_per_cpu_areas中的偏移登记和CPU0分支。
- mykernel/include/linux/smp/percpu.h、percpu_chunk.h；mykernel/arch/x86_64/include/asm/percpu.h中实际用到的声明/定义、per_cpu_offset/per_cpu/指针转换及per-CPU访问包装，读取完整相关宏链。
- mykernel/arch/x86_64/kernel.lds的__per_cpu_load/start/end及相关输入节；mykernel/init/main.c::start_kernel中setup_arch→setup_per_cpu_areas及后续调用者；mykernel/arch/x86_64/kernel/head_64.S中早期__per_cpu_offset/current_task/GS路径只作先行接口。
- mykernel/arch/x86_64/kernel/cpu/common.c::load_direct_gdt及其直接依赖；get_cpu_gdt_rw、load_gdt、refresh_segment_registers、wrmsrl、MSR_GS_BASE、pcpu_hot/current_task按实际include/引用定位真实定义，不拿声明或Linux惯例替代。

最近依赖包括for_each_possible_cpu、nr_cpu_ids/NR_CPUS、roundup_pow_of_two、ALIGN/PAGE_SIZE、被调用的memblock分配包装和数据/指针类型。只追实际使用的定义与初始化者。nr_cpu_ids的启动来源若涉及80已有parse_tag链，只读本次必要生产/消费接口并合读80回执限定，不重新分析整个启动标签解析器。

禁止完整SMP/AP启动验证、所有CPU热插拔/中断/NMI路径、全部动态per-CPU分配与回收、完整GDT/TSS/调度上下文切换、硬件MSR实验和全树写者穷尽证明。没有配置、构建、运行或修复授权。未取得callee实现或硬件语义时保留接口缺口；不以缺口为由把已授权安全子项全部停掉。

## 2. G00：同一执行对话的增量接续

L=agent-workspace/lead/MYOS2-LEAD-002/，工作材料取Owner转发本122 URL中的同一真实固定提交；不能拿父dd1e329a035c当新发布头。源码只取time@a039d9803ade；85原六件固定2489ea8e6963，正式评论6869453153。

完整读取本122、L/110-owner-communication-and-delivery-continuity.md、L/reviews/CORE-USER-BOOT-BUILD-CONTRACT-85-review.md、L/reviews/CORE-USER-EARLY-ROOT-HANDOFF-84-DOC-CLOSURE-review.md、L/checkpoints/2026-11-14-boot-build85-reviewed-percpu86-ready.md和85正式评论。合读R85-S01/S02/S03与原R84-S01/S02/S03、R83/R82/R81的实际依赖限定。

重新全文读取63号、79号、协议v2全部P1–P13。其它九规则、119接收、Mnemosyne指导、84/85原件只可按本有效C06b自己实际读到且固定对象一致的范围明确复用；缺段补齐。85的not_inherited_from_mainline字段矛盾已由主线限制消费，不能用它伪造历史加载证明；本轮记录逐项写“新读/同会话已读复用/未读”，不能再以含糊二选一字段替代。无需重演接收仪式，不继承C06或旧执行者已读声明，不导入其它项目维护。

核PR16/17、lead/exec/master/time四direct refs、开放PR、目标目录及本人86正式marker。exec预期2489ea8e6963、master直接e8e60cd19540、time a039d9803ade，lead为本122真实发布提交；PR缓存base_sha不是master真值。未知推进先查归属，已有成果先核是否本批已完成，不覆盖或重复发布。

独立核公约§1(2)整句、time/master两项金丝雀和read_channel；先完整读options_flags.cmake再作未启用/未见定义判断。接收/G00前保持STOP_WRITING；通过后按本合同取得86限定写权，继续G01–G04，不要求Owner再次发令。

## 3. G01/A：模板、容量、分配与复制

完整读simple_pcpu_setup_first_chunk、early_pcpu_create_chunk及相关全局/结构定义。把__per_cpu_load、__per_cpu_start/end的链接符号值、C取地址表达、模板长度、实际可读存储分开；输入节规则不等于已经产生ELF布局。给出static_size、reserved_size、dyn_size、size_sum、chunk_size的类型及变换，明确roundup_pow_of_two的有效输入范围，不用数学任意精度悄悄替代C宽度/转换。

追踪元数据与data两次分配的大小/对齐、返回指针、入链时机、first/reserved两块字段的初始化。分别核失败前缀：元数据失败、data失败、offset数组失败；未检查的返回值不能自动当成功，证明停止与源码真实错误处理分开。必要时读取memblock实际包装定义及其正常返回条件，不重做旧memblock整包。

模板memcpy的源、目的、长度、调用次数及所针对CPU范围须按实际源码登记，不能因注释说“perCPU已运行”便声称每个CPU的模板都已复制。size_t与int类型的free_offset/free_bytes转换、乘法溢出、零长度、对齐以及存储可访问性分别列条件。

交template_and_storage_matrix、first_chunk_allocation_and_publication_matrix、initialization_and_failure_matrix。代表模型选择static_size为零/小值/跨页或跨2次幂边界、CPU数量0/1/多值、各分配失败及正常返回，前提必须包括链接差值可表示、类型宽度和分配存储合法性；不要凑满模型上限。强制锚含实际链接符号区、chunk结构相关字段、尺寸计算、分配调用及每处发布/复制。

## 4. G02/B：CPU索引、偏移数组与访问公式

完整核pcpu_unit_offsets生成、setup_per_cpu_areas的delta与__per_cpu_offset发布、this_cpu_off和pcpu_hot.cpu_number写入，以及实际per_cpu宏/指针转换链。明确每个量是字节数、相对链接地址偏移、相对first chunk偏移还是实际地址；不能把数值差自动当有效指针。

逐一比较分配总跨度、unit步长、模板复制范围、reserved布局和后续访问公式，不能默认static_size、size_sum、chunk_size是同一跨度。核nr_cpu_ids与NR_CPUS及for_each_possible_cpu的真实关系；宏究竟遍历连续范围还是集合由定义决定，不能套用上游Linux名称含义。每次数组下标、目标对象所属空间及cpu号写入须与对应容量同时评价。

对CPU0/末CPU、稀疏或超界候选编号、nr_cpu_ids=0/1及小规模多CPU的条件模型，先说明它们是否符合实际遍历定义；不可达模型单列，不拿它们作成功路径故障证据。offset数组分配的sizeof(void*)与存入ulong类型需核所选ABI前提，不凭x86名字写成实际测得值。未穷尽nr_cpu_ids写者时只保留所读生产者/路径结论。

交cpu_index_and_capacity_matrix、offset_address_equation_matrix、publication_and_template_coverage_matrix。强制锚含循环/下标、偏移赋值、真实宏定义、CPU数量来源和代表变量定义；源名相同不等于同一存储对象。若与旧条目同根，只引用原qualified_id并增加本次证据，不重复登记。

## 5. G03/C：启动CPU的GDT/GS交接与消费者边界

从start_kernel实际调用者接到setup_per_cpu_areas，核CPU0分支的load_direct_gdt与wrmsrl顺序；读load_direct_gdt完整定义及get_cpu_gdt_rw、load_gdt、refresh_segment_registers的必要实现。不能按注释“无需写gs”就认证中间callee不改段寄存器或GS基础；未取得实现则标清停证点。

将新GDT地址形成、描述符提交、段寄存器刷新请求、GS_BASE写入请求、per-CPU偏移内存已发布、current/pcpu_hot消费者六类事件分开。读取代表current_task/pcpu_hot及访问包装的实际定义，只核启动CPU接续和一个代表消费者，不把head_64.S共用secondary符号当AP全链已证明。

结合84根身份与85偏移新鲜度限定，说明哪一步依赖内存映射、先行callee返回及正确ABI。旧GS值、内存中的this_cpu_off与MSR值相等与否需独立条件，内存赋值本身不证明处理器寄存器改变。硬件成功、异步中断安全、跨CPU可见性和完整启动可达性均不从源顺序自动推出。

交boot_cpu_switch_order_matrix、GDT_GS_and_current_view_matrix、cross_stage_gates_matrix及A/B/C接合表。条件例覆盖CPU0是否被遍历、load_direct_gdt或其callee不返回、GS写入前/后视图、旧/新模板内容相同或不同；不把假定失败说成实测。强制锚含实际调用者、CPU0分支、GDT辅助函数、必要包装和代表消费者定义。

## 6. 六件及发布前质量

严格六件。facts含source_ledger、三专题矩阵与条件模型、事实键→真实证据ID、prior_findings_links，以及本轮新读/复用账。候选用PC86-Fxx、缺口PC86-Uxx；普通代码问题按18分类收存，Owner无需逐项决定修复。新来源最多24份为软上限（金丝雀另计），8组定点检索/4次相关目录浏览；必要超限具体解释，不全仓漫游。新短锚软上限120、模型最多30、候选/缺口各最多12；都是上限非配额，不靠拉长材料替代工作。

适用协议v2全部P1–P13。MANIFEST必有task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。模型仅据可见/Owner来源，不能认证后台。程序执行、模型手推和作者读取声明分开记账。

P9-1六件40hex零命中；2/13全部短锚为真实单一定义或合法独立命令内连续1–5行，且支持其论点；3路径及固定/活动回读真实；4附行号仅辅助且不越界；5三整数与六件全部原始前缀相符，含说明文字；6外部结论有实际官方出处及适用版本，硬件结论按P5，未核就留缺口；7/8公约整句、双分支金丝雀、read_channel齐备；9/10/12无代码/补丁/降级则N/A；11未执行项明确NOT_RUN。完整本地P9仍待本地，不自签PASS。

逐项审正文、facts、模型、deferred和图的语义支持，不能只查编号存在或逐字命中。宏外条件、相邻声明和另一宏不得拼入一个短锚。图须分真实调用、同一调用者内源序、数据/地址关系及假设；实际模型中间值要算到输出，不跳过先行赋值。发布前纠正已知错误，不把它们留给主线代修。

## 7. G04：限定发布、远端回读与停写

只在原claude/dazzling-cori-q0dnyt/PR17新增头部指定86目录六件，另加11本人一次正式PR17顶层评论。唯一marker为MYOS2-LEAD-002-CHAT-EXEC-11 / CORE-USER-PERCPU-BOOTSTRAP-86 / FORMAL_DELIVERY。旧成果/评论、PR元数据/Draft/base、lead/PR16、master/time、内核和脚本全部冻结。

写前核exec=2489ea8e6963、目标目录及marker；以真实完整旧HEAD作expected_sha，force=false、单父提交。GitHub暂态不因第一次失败停工，按63号先核远端实效，再间隔安全重试，通常首次加三次并遵Retry-After；复用成功对象/评论，未知并行推进不覆盖，工具发现与实际请求分账。

compare严格六件added、旧件零净改删；新固定提交六件全文、活动分支六件内容/blob、四direct refs、PR16/17及正式评论内容/编号/唯一性均远端回读。发布前文件不预造后置提交号；真实提交/评论及后验由正式交付记录承接，不额外交第七件。

完成仅报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，随后STOP_WRITING，经PR17交回“MYOS2-A-C06 内核分析主线续建”作Pro独立验收。不自行签86、不启动87、不重开R84-01或已闭合历史文档项。

禁止内核/loader/initramfs/Owner脚本修改、补丁、CMake配置、预处理/汇编/编译/链接、仓库程序/脚本/旧夹具/Graphviz/QEMU/实机运行、真实并发或硬件实验、本机凭据、外部libc镜像、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3及跨项目维护。仅按既有许可可新建独立临时纯文本检查或有限整数模型，不执行仓库代码，并如实标明非内核运行。

下一步仓库写入：仅11通过G00后的上述86六件和本人交付评论；C06不并写results，旧10/09停用。110号以后每次主线交接的交接包、接收启动词和固定必读清单均须继续显式引用。
