---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
lead_before_publication_short12: "40725302f9ee"
execution_pin_short12: "51f1f5b5dc08"
master_direct_short12: "e8e60cd19540"
read_channel: "connector"
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
record_type: "bounded_boot_scheduler_clock_provenance_contract"
followup_id: "CORE-USER-SCHED-CLOCK-BOOT-OFFSET-105"
status: "READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED"
executor_display_name: "MYOS2-A-C07a 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-15"
mainline_owner: "MYOS2-A-C07"
canonical_write_branch: "claude/dazzling-cori-q0dnyt"
canonical_pr: 17
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/"
allowed_files: ["MANIFEST.md","sched-clock-boot-offset.md","facts-and-boundaries.yaml","sched-clock-boot-offset-paths.dot","deferred-findings.yaml","evidence.md"]
activation_gate: "OWNER_FORWARD_ACTUAL_149_RELEASE_AND_FRESH_G00"
R103_01: "OPEN_CONSUMER_MAPPING_ONLY"
R104_01: "OPEN"
R104_02: "OPEN"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/148-user-rtc-wall-clock-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/deferred-findings.yaml","本轮D00及104回执、检查点、可定位的固定time启动、校频、cyc2ns、jiffies源码"]
open_questions: []
owner_decisions_requested_now: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","time_panic":"\tthis_cpu = smp_processor_id();","master_options":"\t-m64 -mcmodel=large -fPIE \\","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0,"scope":"本主线文件独立源码短锚"}
---

# 第105批　启动阶段 local_clock 的来源、换算和 boot_offset

继续现 **MYOS2-A-C07a / MYOS2-LEAD-002-CHAT-EXEC-15**，模型按Owner指定次一档 GPT-6＋Extra。收到Owner转发的实际149固定提交与C07发布评论后，独立读本轮裁定并做fresh G00。通过即连续执行D00、A/B/C、六件自检、一次发布、一次本人PR17正式评论及全部回读。

本合同接第103批TK103-U02的第二个输入及104的RTC104-U06。目标是让读者回答：在源码规定的启动顺序中，boot_offset的纳秒输入可能来自哪里；何时以哪个CPU的数据换算；以什么前提能将数值传给ns_to_timespec64。RTC输入使用本轮已核结论，时间戳和频率仅作为源码输入。阶段2，最高PASS_PENDING_LOCAL。

## 1　固定入口与授权

源码time@a039d9803ade，direct master@e8e60cd19540；执行父51f1f5b5dc08；主线头取Owner转发本149的**实际固定URL**。所有材料写出的短SHA只取GitHub正规返回前12位，完整对象仅用于API及租约。

完整新读：本149、[历史四项D00裁定](reviews/CORE-USER-RTC-WALL-CLOCK-104-D00-102-103-review.md)、[104独立审核](reviews/CORE-USER-RTC-WALL-CLOCK-104-review.md)、[本轮检查点](checkpoints/2026-11-19-rtc104-reviewed-sched-clock105.md)、[104作者正式交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-6998446053)和本次105主线PR17正式发布评论。104原六件在[此固定目录](https://github.com/08822407d/MyOS2/tree/51f1f5b5dc08/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104)，148及前次102/103回执固定在40725302f9ee。旧103@4d3183678a64、旧102@1a92f9839e53只读D00点名的表/引文/边。现15本人已在本对话完整读取的同一对象可复用，仍核固定/活动身份与指定段落。所有新裁定必须真实读取。

本任务适用 agent-workspace/tasks/00-gpt-task-protocol-v2.md 全部十三条。复用同一对话亲读的适用MyOS2/Mnemosyne指导并记录真实读账；规则包含 L/00-work-order.md、agent-workspace/README.md、agent-workspace/conventions.md、上述协议、L/18-deferred-findings-and-resume.md、24-chatgpt-specialist-conversation-separation.md、28-review-and-next-batch-same-turn.md、63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md、110-owner-communication-and-delivery-continuity.md、124-model-routing-and-pro-message-budget.md。L=agent-workspace/lead/MYOS2-LEAD-002/。以本次固定lead为版本，完整读63；上下文缺失则补读，已读复用须注明身份和范围。

C07拥有原lead/PR16及主线导航唯一写权。现15仅可在原执行分支 claude/dazzling-cori-q0dnyt 的 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/` 新增六件，并在原PR17添加一次本人顶层交付评论。旧执行者继续停写。

## 2　G00现场

1. 直接取四refs并比实际149主线头、执行父51f1f5b5dc08、time及master；PR16 Open/Ready、PR17原title/base/Open/Draft，均未合并；基线开放PR只有16/17。直接master优先于PR缓存base_sha。
2. 核104固定六件和活动blob；本105目录与六目标尚不存在；PR17分页至末，无since或如实标明范围，检查本人及其它作者的105正式交付标记。主线合同发布与作者交付分开数。
3. 从**direct master**取公约§1第2条整句及来源，独立从time/master各取 options_flags.cmake、panic.c 两个金丝雀。完整读固定time配置，先用真实单条命令的独立短锚支持受配置影响的有限否定。
4. 碰到已存在对象先核归属；本人成功的动作补读回，未知并行写停受影响的写入。最终更新ref前再核完整HEAD和目标，原件、旧评论、既有合同不动。

## 3　D00先行，只处理三项

用一张差量表区分实际原始ID引用、已撤销引用、沿分组或公式间接消费、最终有效引用。每个跨包行给固定批号、真实路径、对象ID。已核准的十个替代、41边、旧时钟条件和历史文档闭合直接复用。

| 项 | 最小交付 | 主线下轮按何种证据核收 |
|---|---|---|
| R103-01余项 | 使用本轮D00回执的十行基线，去掉或重新归类E089→A-M01及其它同名/自由描述行；区分G102-20/21/22历史和现行、D00成组的HC102模型/候选和新包直接消费者 | 每个列作直接消费者的行可实际解引用并支持该命题。A-F04/M01保留，ABC-01如用常量记间接。新证据重复数为零亦可 |
| R104-01 | E047拆两个定义，E060实例化和宏体分账，补持久时钟忽略状态返回的真实调用锚。按104审核修wall字段矩阵、采样/来源行、F05、M12/U06等实际引用。签名/字段作为独立身份配对体内语句。把默认mask/shift绑定到真实旧依据。公约读账用实际direct master | 原104完整词头仍104。替代项各自对应单一定义；普通使用点不冒充宏体。索引ID存在且所引片段支持命题。仅报告有差的行，不复制全部引文 |
| R104-02 | 把偏移图更正为 boot_offset − wall_time，lhs、rhs、注释存储归属一致；未写字段用注记、控制寄存器注释放回实际函数。RTC寄存器编号与返回字节分列。日历样例补零时分秒、校验器的真实tm_year/tm_mon表示 | 新图/表/样例同意；保留正确的午夜数值和日期间隔。只在105六件追加supersedes/限定，冻住104原件 |

四个旧项中，R102-02/03及R103-02已经由C07文档闭合。第102组合PASS_PENDING_LOCAL。第103仅上述一项OPEN，第104有两项OPEN。执行方仅可提交纠正待独立核收。105材料不得宣称已作为C07关闭事由。

## 4　A　谁产生频率，何时允许调度时钟使用TSC

主读固定time的 mykernel/init/main.c 中 start_kernel，mykernel/arch/x86_64/kernel/setup.c 中setup_arch，kernel/time.c 中late_time_init（此处kernel/time.c指mykernel/arch/x86_64/kernel/time.c），mykernel/arch/x86_64/kernel/tsc.c 中 tsc_early_init、tsc_init、tsc_enable_sched_clock、determine_cpu_tsc_frequencies、cpu_khz_from_cpuid、native_calibrate_cpu_early、native_calibrate_tsc、get_loops_per_jiffy；x86_init.c 的x86_platform实际初始化及x86_init.h对应回调字段。

- 画出 setup_arch 内的超管设置和tsc_early_init、返回start_kernel后的setup_per_cpu_areas、timekeeping_init、再后late_time_init→tsc_init。列出可提前返回的门。源码定义、函数指针初值、条件替换、真实启动选择各自有证据。
- 比较原生CPU频率与TSC频率的两个入口。核 processor_const_arch.h 的CPUID_LEAF_TSC/CPUID_LEAF_FREQ常量。cpu_khz_from_cpuid 的能力门、注释调用、活动leaf与EAX数值用途分别登记；不要由变量名推断叶语义。native_calibrate_tsc 主/备叶的分子分母、单位、整除/截断、无符号中间乘积和返回域沿源码表述。CPU兜底DUMMY_TSC_KHZ、tsc_khz==0回退、tsc_early_khz与clocksource_tsc.freq_khz的不同写入点分别记录。必要时回读真实abs及do_div定义；读不到选中语义则条件化。
- 查 asm/hypervisor.h、mykernel/cpu/hypervisor.c 与 vmware.c 中 init_platform 选择及 vmware_platform_setup 更新两回调的条件。仅追到返回值与函数指针赋值，平台查询本身按opaque处理。不得假定这台机器真的用了某供应商回调。
- 源码中“标记clocksource不稳定”、__use_tsc调度时钟门、clocksource注册/选中是不同状态。只追clocksource_mark_unstable及必要字段写入到已声明后果。不展开整个watchdog或虚拟化实现。

交 startup_order_and_gates、frequency_source_and_units 两张表。每条校频结果给来源、单位、类型、失败/回退和第一个未证步骤。

## 5　B　启动CPU换算数据的建立和发布

主读 tsc.c 的 cyc2ns_init_boot_cpu、__set_cyc2ns_scale、__simple_cyc2ns_read、__cycles_2_ns、cycles_2_ns 及实际别名；asm/timer.h的两个结构、asm/percpu_area.h 及 arch/x86_64/smp/percpu_area.c 的cyc2ns模板；asm/percpu.h 的 this_cpu_ptr、per_cpu_ptr 和直接必要辅助。clocks_calc_mult_shift真实体在mykernel/time/clocksource/clocksource.c，两个 mul_u64_u32_shr 条件定义在mykernel/include/linux/kernel/math64.h；与103审查的“加b再移位”函数分开。已有合格源码锚可复用。

- 从模板的mul/shift/offset，到“当前CPU旧表示”读取、求ns_now、新mul/shift、shift==32降至31时mul减半、以同一tsc_now计算offset、写到指定CPU的c2n->data，逐一列出。源模板存在、复制/偏移已生效、this_cpu_ptr与目标CPU身份一致须分别判断。
- 重点核 early 路径与 setup_per_cpu_areas 的先后。对偏移、模板映射和CPU身份，只消费固定lead的[86原审核](https://github.com/08822407d/MyOS2/blob/40725302f9ee/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PERCPU-BOOTSTRAP-86-review.md)与[86文档闭合](https://github.com/08822407d/MyOS2/blob/40725302f9ee/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PERCPU-BOOTSTRAP-86-DOC-CLOSURE-review.md)的适用边界及必要短窗口。已有根ID按原名复用，不重做整个per-CPU分配器。
- 两种编译候选的实际表示分开，包含INT128条件、回退分拆运算、各右/左移位宽、执行高字分支的必要条件。对已有数学输入说明取整、模域、sft==32更改系数和存储宽度。只在合法域推普通数值。
- 给出在同一计数点处“新offset加新换算”和“旧ns_now”相合所需条件。一个CPU的数据模板、一次结构赋值、三个分别READ_ONCE的字段与跨CPU一致性并非同一命题。条件推断与实测分账。

交 per_cpu_scale_provenance、integer_conversion_and_continuity 两张表。禁止为证明对象有效而执行指令、修改模板或构造并发复现。

## 6　C　local_clock选择、jiffies回退及boot_offset

主读 linux/sched/clock.h 的local_clock，tsc.c 的native_sched_clock，arch/x86_64/insns/x86msr.h 中rdtsc定义仅核返回值来源。读取mykernel/time/timer/timer.c 的jiffies_64初始化、time/systick/systick_const.h / systick_macro.h的INITIAL_JIFFIES实际定义及相关include、systick_api.h声明、asm-generic/param.h的HZ、arch/x86_64/kernel.lds的jiffies别名。凡相对路径以固定time源树的mykernel/为前缀，arch文件均在arch/x86_64。边界定位以真实树为准。

- TSC门真时的读取、乘移换算、偏移相加及u64返回，门假时的 (jiffies_64-INITIAL_JIFFIES)*(1000000000/HZ) 分开。指出INITIAL_JIFFIES中的显式转换、减法和乘法的类型、整数除法及回绕。链接脚本的别名声明只签文本，最终选用脚本、加载地址及硬件周期保留未证。
- 采用少量有判别力的显式输入，例如jiffies等于初值/比初值多一、同一tsc_now处两表示、正数落在s64可表示域或刚越界。写全输入、实参宽度、正常返回和计算前提。不复跑旧负纳秒模型，不用宿主时钟代替源码。
- 接到104已审的local_clock u64→ns_to_timespec64 s64、boot_offset−wall_time及限定的timekeeper存储。说清哪些输入来源被收窄，以及频率真实性、多CPU同步、真实链接、时间精度还缺什么。

交 read_path_and_fallback、boot_offset_consumer、ABC_join 三张表。图只画实际调用者、宏/回调和数据关系，注明方向。硬件实际频率、CPUID规范语义、序列化或跨核同步只有取得适用的一手卷节/版本时才可提出外部结论；本包可停在源码报告值。无硬件采样授权。

## 7　六件、规模及证据门

六件名称依次为 MANIFEST.md、sched-clock-boot-offset.md、facts-and-boundaries.yaml、sched-clock-boot-offset-paths.dot、deferred-findings.yaml、evidence.md。正文先给D00与A/B/C结论，facts为差量映射、七表、显式样例、证据索引及真实读账；deferred分已知根、跟进和新增；DOT仅文本文档。新局部ID使用SC105-E/M/F/U，跨包用followup_id::local_id及真实固定路径。read_ledger分完整读、定义窗口、引文源和同身份复用。

强制锚包括单条配置命令；D00明确的缺项；启动调用点、平台回调初值与条件改写、两个leaf定义/实际请求/返回换算、终值回退与启用次序；per-CPU模板/字段、真实读/写体、clocks_calc_mult_shift及实际mul_u64_u32_shr定义分支；native_sched_clock两条返回、INITIAL_JIFFIES、jiffies_64定义、HZ与读时间接口交界。函数体内1–5行，独立宏、对象、CMake和链接符号按各自定义单元。证明实参/返回类型时另给签名来源；调用点与定义身份单列。

软上限为本包time主辅源44份（双金丝雀另计）、8组有界符号检索、4次必要目录浏览、新增短锚120条。模型上限18，候选及缺口各8。无最低数量。超软限按实际直接用途说明，优先D00和三个直达专题，旁路作为未证边界。无代码件、无补丁、无运行。只允许对取回文本/结构和给定输入作有限的普通整数量核，不运行、翻译执行或模拟仓库实现/作者模型。

MANIFEST至少列 task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。另列当前执行人、C07、D00与三个OPEN，及逐项真实write_retry_log。快照字段写分支名，短SHA另列。P9依协议逐项：1全六件40hex零；2全部短锚逐字/单元/真实支持；3paths、supersedes、依赖ID逐项可解引用；4辅助位置合法；5最终原始词头数及三整数复算，单列源码引文数；6外部结论须一手可解析来源；7双金丝雀；8公约、通道和字段；9/10无代码N/A；11运行记录真实；12降级才适用；13合同强制项对照勾核。此处仅做可读子集，full_local_P9、构建、实机均NOT_RUN。

## 8　发布、回读、停写

用实际执行完整HEAD为唯一父与已有tree为base，一次提交严格六件全新增。更新ref时expected_sha=真实父、force=false，未知并写停对应动作。本人PR17顶层正式marker精确一次：

MYOS2-LEAD-002-CHAT-EXEC-15 / CORE-USER-SCHED-CLOCK-BOOT-OFFSET-105 / FORMAL_DELIVERY

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次，约10/20/40秒并尊重Retry-After。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。成功的blob/tree/commit优先复用，评论丢响应先查重。

回读须有真实提交和唯一父、compare严格六added总行/旧修改删除0、固定与活动六件全文/blob、四direct refs及PR16/17原形态、按ID和全顶层分页读正式评论及唯一marker。执行文件勿预填未来评论或回验成功。完成后 READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING，D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW，R103-01 / R104-01 / R104-02 OPEN，STOP_WRITING。交回 **MYOS2-A-C07 内核分析主线续建**，Owner发送正式验收消息前选择Pro，C07同轮核收及安排后继。第106批不在本次授权内。

普通发现依18号延期。禁止写旧件、旧评论、源码、loader/脚本、PR16或PR17元数据、master/time及其它项目；禁止额外模型/子代理/付费API、预处理/编译/链接、仓库程序或模型、Graphviz、QEMU、实际CPU/端口/设备实验、新开或合并关闭PR、强推删除。当前仅合同发布，执行须Owner转发并fresh G00通过。
