---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
produced_by: "GPT-6 Astra Pro（当前会话标识；不认证后台路由）"
model_per_owner: "Pro（主线）"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
lead_before_publication_short12: "7564e79a9b02"
exec_pin_short12: "4d3183678a64"
master_direct_short12: "e8e60cd19540"
phase: 2
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
record_type: "bounded_rtc_calendar_to_boot_wall_clock_contract"
followup_id: "CORE-USER-RTC-WALL-CLOCK-104"
status: "READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED"
fixed_contract_commit: "RESOLVE_FROM_ACTUAL_148_FIXED_URL"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-15"
executor_display_name: "MYOS2-A-C07a 代码分析执行"
mainline_owner: "MYOS2-A-C07"
canonical_write_branch: "claude/dazzling-cori-q0dnyt"
canonical_pr: 17
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/"
allowed_files: ["MANIFEST.md","rtc-wall-clock.md","facts-and-boundaries.yaml","rtc-wall-clock-paths.dot","deferred-findings.yaml","evidence.md"]
activation_gate: "OWNER_FORWARD_AND_FRESH_G00"
R102_01: "CLOSED_DOCUMENTATION_ONLY"
R102_02: "OPEN"
R102_03: "OPEN"
R103_01: "OPEN"
R103_02: "OPEN"
inputs_read: ["本轮102部分闭合、103审核、固定103成果及U02","28/63/79/110/124和协议v2","固定time RTC/BCD/历法与timekeeper直达路径定界；见本轮检查点"]
open_questions: []
owner_decisions_requested_now: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0,"scope":"本主线文件新增的短锚；不是作者包的自检或本地P9"}
---

# 第104批：RTC采样、年月日解码与启动墙钟

**执行者：现 MYOS2-A-C07a 代码分析执行 / MYOS2-LEAD-002-CHAT-EXEC-15；发送前选次一档 GPT-6＋Extra。** Owner转发本合同实际固定入口并通过新的G00后，连续完成D00及A/B/C、严格六件、自检、原PR17本人正式评论和远端回读，不等第二次批准。发合同不代表执行已启动。

## 1. 为什么接这个入口

第103批在 TK103-U02 将 persistent RTC 与 local_clock 两种输入并列作 opaque。本批只打开前者：从启动墙钟由谁填起，追到六个历法字段如何取值、如何解释、怎样变成秒并由 timekeeper 消费。后者只核调用前类型转换，不展开时间戳计数器或校准。不视为全局关闭 U02，不引入新的启动/IRQ/HPET根因。

本任务适用 `agent-workspace/tasks/00-gpt-task-protocol-v2.md` 全部十三条，保持阶段2。每项给固定源码、真实定义、依赖的条件及第一个未证步骤。源码发出请求、数据返回、一次一致采样、日期有效、真实设备时间正确是五种不同结论。无新问题可如实交零，不设引文、问题或行数下限。

## 2. 固定输入、规则和唯一写区

L=agent-workspace/lead/MYOS2-LEAD-002/；R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。源码固定 time@a039d9803ade；历史102结果@1a92f9839e53、103结果@4d3183678a64；direct master=e8e60cd19540。本合同的固定版本取自 Owner 转发的真实发布URL，不用发布前的 lead 当作104合同头。

本轮**新读全文**：本148、同一发布提交下的 [102部分闭合](reviews/CORE-USER-HPET-ENABLE-CLOCKSOURCE-102-DOC-PARTIAL-CLOSURE-review.md)、[103独立审核](reviews/CORE-USER-TIMEKEEPING-READOUT-103-review.md)、[本轮检查点](checkpoints/2026-11-19-timekeeping103-reviewed-rtc104.md)、PR17 [103作者评论](https://github.com/08822407d/MyOS2/pull/17#issuecomment-6983342388)和本次104主线正式评论（ID见转发块）。原147及102审核在 lead@7564e79a9b02。固定103严格六件见[结果目录](https://github.com/08822407d/MyOS2/tree/4d3183678a64/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timekeeping-readout-103)。现15可复用本对话亲自完整读过并核身份一致的103/102原件，但须回读本轮点名的原段、图、消费者；新裁定不能用上一轮读账替代。

规则沿用：L/00-work-order.md；agent-workspace/README.md、conventions.md、tasks/00-gpt-task-protocol-v2.md；L/18-deferred-findings-and-resume.md、24-chatgpt-specialist-conversation-separation.md、28-review-and-next-batch-same-turn.md、63-github-write-retry-and-readback.md、79-work-package-sizing-and-pro-review.md；另有 L/110-owner-communication-and-delivery-continuity.md、124-model-routing-and-pro-message-budget.md。正式版本以本次固定lead为准。完整读63，P1–P13和新增规则必须当前可用；同一对话已亲读且固定对象相同的其余规则按真实账复用。上下文缺失就回读；不得继承别的对话的已读声明。既有适用指导限此MyOS2项目。

执行仅写 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/` 的下列六个**新**文件及原PR17一条本人顶层交付评论：
1. MANIFEST.md；
2. rtc-wall-clock.md；
3. facts-and-boundaries.yaml；
4. rtc-wall-clock-paths.dot；
5. deferred-findings.yaml；
6. evidence.md。

分支 claude/dazzling-cori-q0dnyt；原PR17的title/base/open/draft不改。不触及旧六件、旧评论、lead、master、time、内核源码、脚本/loader/initramfs。C07继续原lead/PR16及主线导航唯一写者，旧C06/C06e不恢复写权。

## 3. G00：为104重新做，不复用103现场

- 四条 direct refs：lead=实际148发布头；exec=4d3183678a64；time=a039d9803ade；master=e8e60cd19540。PR16 Open/Ready，PR17 Open/Draft、均未合并，开放PR与基线相符。PR缓存base_sha不代替直接引用。
- 新104执行目录、六目标及本人/其它作者同任务正式交付均不存在；分页覆盖PR17**顶层评论**至末页，注明是否since和实际范围。主线合同发布评论不是执行交付；不能按包含任务号就计作者marker。
- 核实103固定六件与活动六个身份；旧102按D00目标核固定文件及其现行blob。明确103作者已STOP_WRITING，收到新任务仅开放104。
- 独立取direct master的公约§1第2条整句，time与master上的 options_flags.cmake / panic.c 双金丝雀。先完整读固定time配置、独立引用单条真正命令，再作有范围的否定；选入与实际构建分开。
- 实际头变动或同名对象时先判归属；本人已成功的动作仅补读回，未知并行写停对应写入，不覆盖、不回退、不自动变基。写ref前再核完整头与目标未被创建。

## 4. D00：四个尚开项，一张差量账

原件冻结。每条填“原固定文件+ID/段/边 → 旧命题 → 保留/收窄/撤回 → 新证据或可复用旧合格ID → 实际直接消费者 → 第一止证门”。同一纠正服务多个R号时用同一条替代，不双算。R102-01已闭合，不再派它；100/101既有文档闭合保持。各R须分别给主线可判定的完成条件。

| 项 | 本批必须完成 | 合格边界 |
|---|---|---|
| R102-02 | 修正102部分闭合回执列的真实函数名/配置命令；补 ___define_initcall、__unique_initcall 与所选声明宏及真实必要分支；A-M03换指 TK103-E027/HC102-E049、B-P09消费真正周期函数；补指定clocksource及time头的真实读账 | 声称实际展开、连接和运行需分别有证据，否则停在源码请求。已有合格的mask、链表和字段锚按固定身份复用，不整包重抄 |
| R102-03 | 以原图/103 D00确认41行基线及8个已接受改判；逐边自查支持ID，差量重点 G102-20 改指 HC102-E095，分清 G102-22 入队、G102-31 调用处＋宏展开 | 旧图/旧表不改；列检查范围、错误行及归属，没变的边可列一次清单。禁止创造41个“新问题” |
| R103-01 | E001、E007–011、E012–013、E030的九个定位，另补E089真正宏体；MANIFEST、read_ledger与全源/引用源/窗口口径；真实字段类型和读范围 | 用协议P2检单元、字面、命题三关。ABC-06、D00定义/图表、A-F04/M01等按实际直接索引并补间接数值依赖；不数到十就假定其它结论合格。无40位串，base_snapshot只写分支名 |
| R103-02 | old_clock 为空/非空及位移量0分开，mono.base另列；M15类型及取负域；M20内外负号、u64商、外层模域、秒存储及rem参数接口；local_clock入参转换；默认mask与人工helper模型区分 | 修 A-F09/F04、A-I05/F02、C-N07/F06/M20 等真实消费者。对给定输入重写条件结论；不声称硬件出错、链接选择或通用C方言结论 |

D00只在新六件追加。能够复用旧合格锚时给固定批号、路径、局部ID；新补定义仍要自己取固定time原文。D00的完成状态只能是“提交待C07独立核收”，作者不可关闭任何OPEN。

## 5. A：一个墙钟样本的来源及一致性边界

主读：mykernel/arch/x86_64/kernel/rtc.c 中 read_persistent_clock64、rtc_cmos_read；mykernel/drivers/rtc/rtc-mc146818.c 中 simple_mc146818_gettime、mc146818_get_time；mykernel/arch/x86_64/include/asm/mc146818rtc.h 的 CMOS_READ、RTC_PORT 与保护定义；mykernel/include/linux/kernel/mc146818rtc.h 的六个实际寄存器符号、控制/更新定义只限所消费项；mykernel/include/uapi/linux/rtc.h 的 rtc_time_s。若说明I/O包装类型，再沿 asm/insns.h→insns/insns_arch_api.h→实际 io 包装定义定点阅读；无证则停在调用请求。

1. 逐行列 sec/min/hour/mday/mon/year 六个字段的取值顺序、请求和目标对象，未更新的 wday/yday/isdst 单列，查后续究竟消费哪些。不能把未写且未读字段直接叫运行错误。
2. 区分选择索引的请求和读取值的请求、一次字段读与六字段同一瞬间；锁宏、被注释的调用与真实执行不要混用。画源码关系，不进行任何端口操作。
3. 对更新进行中、格式/世纪、12/24小时等只说明哪些数据本路径实际取得、哪些判断只在注释或定义中。不能由宏名、注释、总是返回0、或没有外部观测直接推导设备模式/错误率。
4. 明确 rc 谁读取、字段成功赋值需哪些前提；一次逻辑快照一致/时钟正确另列门。硬件规范不作为必读；若无真实一手版本及节号，硬件语义保持未证。

交两张表：字段来源与消费者；活动采样步骤/注释或外部条件。

## 6. B：BCD解码、年份窗口与有效性

主读：上述mc146818实现；mykernel/include/linux/kernel/bcd.h 与 mykernel/lib/bcd.c 的 bcd2bin、const_bcd2bin、_bcd2bin；mykernel/drivers/rtc/lib.c 的 rtc_valid_tm / rtc_month_days 和实际月份表；mykernel/include/linux/kernel/rtc.h 的 is_leap_year；rtc_time的整数字段。比较有效性函数与正常转换调用链，声明不是调用点。

1. 拆开宏的两个候选分支：__builtin_constant_p 选择与实际构建分开；有范围地比较两公式在8位原始输入上的结果。变换是数值变换，验证BCD各半字节是另一件事；不得把可解码等同于合法日历。
2. 对已解码year 69/70处、tm_mon递减及后面加1分别列出字段含义与传参。合法BCD的两位年窗口与不满足该前提的原始值必须分开。
3. 按字段值域检查 rtc_valid_tm 的月份、月内日、时分秒、闰年条件；它定义存在，不代表 read_persistent_clock64→rtc_tm_to_time64 调用它。只对已读直达链作有限否定。区分“可被一个独立校验器拒绝”“调用链是否执行它”“是否读到了这样的样本”。
4. 少量普通纸面样例即可：69/70、合法闰日和非闰年同月同日，可加一项格式假设不成立的抽象样本。不要把样例扩成输入枚举、实际故障重现或设备操作。

交两张表：字节/字段表示转移；日期合法性与校验调用关系。

## 7. C：历法秒值怎样进入timekeeper

主读：mykernel/drivers/rtc/lib.c 的 rtc_tm_to_time64；mykernel/time/misc/time_misc.c 的 mktime64；相关真实 time64_t、timespec64_s、uint/u64 类型定义；rtc.c 的 read_persistent_clock64；mykernel/time/timekeeping/timekeeping.c 的 timekeeping_init / tk_set_xtime / tk_set_wall_to_mono；其类型与短锚可消费已核103。D00只需 linux/sched/clock.h 的 local_clock 返回类型及 ns_to_timespec64 入口等，native_sched_clock 保持 opaque。

1. 从六字段到 mktime64 六实参列类型、年月基准和范围。详细分Jan/Feb与其它月份：mon0/mon/year的无符号域、减2后再转int的测试、调整年和闰日项、何处才转换为time64_t。采用特定方言/表示时写为前提，不能把无限整数的历法恒等式替代源表达式中间类型。
2. 采用有界例（例如1970-01-01、2000-02-29与一个三月边界）给预期的历法关系；列每个例需要的表示及正常返回前提。独立比较值时只用显式日历/普通整数关系，不翻译或执行仓库实现或作者模型。rtc_time64_to_tm 是反向接口，不是本路径调用；没有需求不证明全域往返。
3. 明确 ts->tv_sec 与显式 tv_nsec=0 的来源、wall_time 在 timekeeping_init 内的两个直接消费者，及最终写入的字段。算出 wall_to_mono、调用保存函数、结构字段真正保存是不同动作。不得从 RTC 秒值推出单调时钟、真实精度、boot_offset合法性或ELF实际路径。
4. 结果只缩小 TK103-U02 的 persistent-clock 源码缺口。其它 local_clock、后继写者、运行一致性留旧ID关联；历史HPET/IRQ、RTC中断/闹钟、设备校时/写CMOS、系统调用、NTP、驱动探测均不开展。

交三张表：历法算术与类型；墙钟输入到timekeeper字段；A/B/C连接和首个止证门。七表无规定行数。图的边按 DIRECT_CALL、MACRO_EXPANSION、DATA_FLOW、CONDITIONAL_STEP、UNPROVEN_JOIN 等真实关系标注；图节点若是步骤摘要不要使其成为函数调用者。

## 8. 结构、规模及验收

正文先D00/ABC结论，facts内含差量账、七表、读账、有限模型、实际证据索引；deferred保留旧ID、注明新增或跟进，来源与未证前提分开。使用 RTC104-E/M/F/U 系列及 followup_id::local_id，有必要才产生条目。列表、正文及图指向同一事实，不以重复文字增加计数。

强制锚：单条真实配置命令；D00所缺真实定义/宏链/关键调用点；RTC六字段读、CMOS宏与实际读取函数、被注释的保护/格式检查与受限否定；两种BCD变换真实定义、年份/月处理、rtc_time字段、被消费的校验器和月份/闰年定义；rtc_tm_to_time64、mktime64真正算式/条件、两个ts写入、timekeeping_init调用/消费者及tk_set_xtime。D00和C可共享真实旧锚。以1–5行连续逐字引文、真实路径及**完整定义名**定位；函数取体内，独立宏/类型/对象取本单元。不得用宏旁注释、近似别名、原型、只给调用者或偶然同名段冒充定义。

输入软上限：D00+ABC合计40份time主辅文件（双金丝雀另计），8组有界符号检索、4次必要目录浏览；104新增短锚软上限120、模型上限20、候选/缺口各8，无最低数量。必要直接定义超软限时先复用真实合格ID，仍需超限则逐项说明；优先四项D00和三专题直达路径，不为读取旁路牺牲强制定义。完整文件/段读、仅导航和未审分别记录。

MANIFEST至少列 task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check；另列作者、唯一主线、历史与新状态、实际读账、write_retry_log。模型只按界面与Owner申报，不推断后台。三整数和等式按最终六件原始文本计，说明或示例也计入词头；独立给源码引文数。位置命中不等于命题支持。

P9逐项：1全六件40hex=0；2逐条字面/定义范围/真实符号/命题（必要时一原ID多证据）；3路径及supersedes/消费者确实存在；4辅助位置在源内；5原始词头计数与三整数独立复算；6外部结论有一手URL与版本；7双金丝雀；8公约整句、读取通道和元字段；9/10本批无代码/改动块，N/A；11运行状态准确；12仅降级适用；13合同强制定义＋各项消费者逐项有证。完整本地P9/编译/硬件NOT_RUN；纯文本与结构核查如实记，不代替正式本地闸门。

## 9. 唯一正式发布与终态

六件在 exec@4d3183678a64 上一次单父纯新增（完整SHA用于API，短SHA只取真实返回前12位）。沿父tree构造，更新前再确认HEAD，expected_sha=实际父、force=false；不自动变基、不动任何旧文件。不另开分支、PR或第二交付评论。

本人PR17正式顶层marker精确一处：
MYOS2-LEAD-002-CHAT-EXEC-15 / CORE-USER-RTC-WALL-CLOCK-104 / FORMAL_DELIVERY

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次（约10/20/40秒，尊重Retry-After）。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。区分工具发现和真实写入；已有对象复用，评论响应丢失先查重。

后验收据须有：真实提交/唯一父、compare严格六added及总行/旧修改删除0；固定与活动六件全文及blob身份；四direct refs；PR16/17 title/base/状态；PR17实际评论ID与本人正文完整回读；无since或明确窗口、分页至末及marker唯一。不可在尚未发布的不可变文件里预填未来评论号/回验成功；实际后验写正式评论及最终回复。

完成后 **READY_FOR_MAINLINE_REVIEW / mainline_acceptance=PENDING / D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW / STOP_WRITING**。R102-01仍CLOSED_DOCUMENTATION_ONLY；R102-02/03、R103-01/02仍OPEN等C07独立核收。交回 **MYOS2-A-C07 内核分析主线续建**，发送正式验收消息前选Pro，同轮核收剩余D00、104并安排后继；不启动105。

禁止源码补丁、执行仓库程序/模型/脚本/夹具、编译汇编链接、Graphviz、QEMU、真实端口/设备/时间/并发测试，额外模型/子代理/付费API，新增或合并关闭PR、强推删除、阶段3、跨项目维护。下一步仓库写入：Owner转发实际固定入口且现15 fresh G00通过后，是，仅104严格六件与本人PR17交付评论。
