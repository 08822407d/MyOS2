执行模型：次一档，沿用GPT-6＋Extra。执行位置：现有“MYOS2-A-C06c 代码分析执行”，身份MYOS2-LEAD-002-CHAT-EXEC-12。完成后交原C06主线，Owner发送正式验收消息前选择Pro，同轮验收及安排后继。

---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PRINTF-INTEGER-FORMAT-92
record_type: bounded_printf_parser_integer_conversion_and_output_contract
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（正式验收与后继设计安排）
date: 2026-11-16
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
result_base_short12: 474a2c0363f0
lead_before_publication_short12: cc8bd9c768fd
fixed_contract_commit: OWNER_FORWARDED_130_FIXED_URL
executor_display_name: MYOS2-A-C06c 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-12
executor_rotation: false
mainline_owner: MYOS2-A-C06
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-printf-integer-format-92/
allowed_files: [MANIFEST.md, printf-integer-format.md, facts-and-boundaries.yaml, printf-integer-format-paths.dot, deferred-findings.yaml, evidence.md]
prior_return_items: []
D00: NOT_REQUIRED
batch90_composite: PASS_PENDING_LOCAL_R90_01_AND_02_CLOSED
batch91_acceptance: PASS_PENDING_LOCAL_WITH_R91_S01_S02_S03
activation_gate: OWNER_FORWARD_130_TO_EXISTING_C06C_AND_G00_PASS
status: READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
phase: 2
inputs_read: [91六件及正式评论, 90闭合与91审查, 固定vsprintf解析与数字生成及div64预读，详见检查点]
owner_decisions_requested_now: []
open_questions: []
---

# 第92批：格式状态、整数数字生成与定宽输出

第91批已按R91-S01/S02/S03限定接收，无新增RETURN；R90-01/02文档问题已独立关闭，90组合PASS_PENDING_LOCAL。本92无D00，不重做90/91，不重开89/88/86/53已闭合事项。继续现C06c/12，不新开或轮换执行者。

## 1. 增量与边界

91核了名称的容量、两遍分配及全名/短名关系，但KN91-U02保留整数转换和更广格式行为。92以vsprintf.c的实际实现为中心，合包三个可分别取证的专题：A格式解析状态与参数类型；B整数数字生成；C符号/前缀/宽度/精度及输出消费。不是把91正文换名重交，也不是审计全printf或证明符合整个外部标准。

只追普通字面、百分号、整数d/i/u/o/x/X及必要hh/h/l/ll/z/t资格、宽度/精度。字符串分支仅用于共享解析状态和已核名字消费者的接合，不重复字符串所有权；排除全部%p扩展、浮点、binary printf、scanf、整套printk/日志驱动、完整fork/调度/退出。默认次一档执行；局部疑难携证据和前提交回，不自动启额外模型/API。

## 2. 固定材料、历史消费与G00

L=agent-workspace/lead/MYOS2-LEAD-002/，R=agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/。主线材料均固定取Owner转发本130链接中的真实发布提交，不以父cc8bd9c768fd代替新发布头。

必须完整读取本130、L/110-owner-communication-and-delivery-continuity.md、L/124-model-routing-and-pro-message-budget.md、L/reviews/CORE-USER-KTHREAD-NAME-STORAGE-91-review.md、L/reviews/CORE-USER-ID-ALLOCATION-LIFECYCLE-90-DOC-CLOSURE-review.md、L/checkpoints/2026-11-16-name91-reviewed-printf92-ready.md，以及91正式交付评论6905875456。合读R91-S01/S02/S03、R90C-S01及原R90-S01/S02/S03；旧90及91六件分别固定034cd9c52dbf、474a2c0363f0。

本C06c已完整读过的同一固定91/90原件及历史53回执与闭合、68回执，可明确复用本会话自己的记录。需要借用某具体旧事实时，按固定对象/qualified_id定点补齐，不重新抄整个53/68包。53闭合撤回的两条主线空行误判不能恢复；旧source候选和文档关闭状态分开。

重新全文读取63号、79号、协议v2全部P1–P13。九规则其余、128接收和适用Mnemosyne指导仅可复用12在本有效会话真实读取且版本一致的范围；缺段补齐，逐项记本轮新读/同会话复用/未读，不继承C06或旧C06b读账，不倒造接收/刷新历史，不导入其它项目维护路线。

G00核PR16/17、四direct refs、开放PR、92目录和所有92正式marker。exec预期474a2c0363f0，master e8e60cd19540，time a039d9803ade；lead为本130真实发布提交。PR17仍Open/Draft、原head/base；缓存base_sha不作master现场。未知HEAD推进、已有同批成果或晚到写入先核归属，不覆盖、不自动变基、不重复发布。

先完整读取固定time的mykernel/scripts/options_flags.cmake，再作配置或否定判断；核公约整句、双分支金丝雀与实际通道。G00前STOP_WRITING/只读，通过后连续G01–G04，不逐专题等待批准，不在G00完成后再次等Owner发令。

## 3. A：格式解析状态、位域与参数提取

主源mykernel/lib/printf/vsprintf.c：format_type、printf_spec、SIGN/LEFT/PLUS/SPACE/ZEROPAD/SMALL/SPECIAL各自宏、skip_atoi、format_decode、set_field_width、set_precision，以及vsnprintf中的状态分派和整数va_arg。读取每个相关完整定义；mykernel/include/linux/lib/stdarg.h全文及类型、ctype依赖沿实际include定位。

建立输入格式位置、spec.type、flags、base、width、precision、qualifier与已消费实参的逐步关系。区分字面扫描、读到星号后的提前返回、外层取int实参、再次进入解析器的恢复点；负宽度与负精度分别核，不把两个恢复状态合并。没有星号时的skip_atoi和位域赋值，与set_field_width/set_precision路径分别判断。

核默认值、重复标志、d/i/u/o/x/X和资格到实际va_arg类型的映射；hh/h小类型经哪种实际取参及转换获得，不凭C库习惯补造。零精度、零值、缺失转换字母、非支持格式及解析停点要有有界案例。对过长数字、位域范围、负值取反及整数溢出，在最早未证门停下，不用无限精度数学冒充C结果。告警宏是否返回、拒绝还是仅通知，必须看实际定义或明确保留依赖，不能只按名字解释。

交parser_state_transition_matrix、format_argument_type_matrix、width_precision_input_matrix。A只给当前实现的行为；要与外部标准比较须另有真实具体出处，不取得来源就不作兼容性断言。

## 4. B：整数数字生成及临时空间

主源vsprintf.c的number、decpair、put_dec_trunc8、put_dec_full8、put_dec、num_to_str；do_div的真实include路由、mykernel/include/linux/kernel/asm-generic/div64.h定义，以及cpu_to_le16、hex_asc_upper、相关类型/常量的实际定义。默认分支检索只定位，全部证据回到固定time；未取得链路就留缺口。

核number中的有符号解释、负数幅度、base8/16移位与base10分段，分清输入类型转换和取负哪个先发生。int32模型的INT_MIN与long long最小值不是同一难度；不能把对前者成立的说明推广给所有资格。给出tmp大小、对齐、逻辑字符长度、实际写入宽度和倒序消费范围，特别检查成对写入可能多写一个临时字节的情况。

do_div同时返回余数、更新被除数，必须追真实变量改变，不能当作只返回商的纯函数。分别核1e8分段及两次分段后残值、固定乘法/移位近似的适用输入范围、decpair下标；注释不直接当证明。允许独立有限整数演算和必要的数学区间论证，不运行仓库函数、不暴力重跑整个值域、不为凑模型数量重复同类例。

交integer_sign_magnitude_matrix、digit_chunk_and_temporary_storage_matrix。案例优先0/9/10/99/100、分段阈值前后、明确目标宽度的极值及必要反例，具体数量由取证需要决定；未证明所有整数范围时如实保留未覆盖域。

## 5. C：符号、前缀、宽度/精度与输出消费者

完整读取number的输出后半、vsnprintf整个分派与结尾、vscnprintf/scnprintf/snprintf及num_to_str；必要时复用91已经取得的相同固定对象，但新增结论仍须真实回源。format_worker_id只作为一个有界%d实际消费者接合，不再重做名字分配和所有权。

追sign、0x/0前缀、空格或零补齐、精度和LEFT的实际源序及field_width扣减；区分精度足够/不足、数值零、输出容量不足和逻辑需求长度。展示最终字节和中间写入/指针推进，不能将最终可见字符串当完整访存轨迹。NUL属于哪个函数、返回长度是否包含NUL、num_to_str是否写NUL须分别确认。

合读R91-S01：size拒绝分支可返回0而不写NUL；需求长度、已写字符及有效C指针轨迹是不同承诺。检查int/size_t比较、返回可表示性、容量0/1/刚好/不足和局部允许上界，遇目标对象/指针算术缺口不得继续宣布真实安全执行。上层忽略返回值只是性质，是否违反需求须有额外证据；不把截断本身计成漏洞。

交prefix_padding_and_emission_matrix、return_capacity_and_consumer_matrix及A/B/C接合表。旧KN91-F01/F02/F03/F04、KN91-U01/U02等同根因沿原qualified_id追加范围，不换名字制造重复缺陷；新增问题必须说明不同不变量。候选PF92-Fxx、缺口PF92-Uxx分类记录，无需Owner逐项决定修复。

## 6. 六件与质量检查

严格六件：MANIFEST.md、printf-integer-format.md、facts-and-boundaries.yaml、printf-integer-format-paths.dot、deferred-findings.yaml、evidence.md。主件通俗中文说明核心结论、具体反例及停止边界；facts含结构化矩阵/模型、源账、证据索引和真实读账；图给边类型并在facts有可定位对应。无第七件、补丁或可执行测试脚本。

软限源码24份（金丝雀另计）、8组定点检索、4次相关目录浏览。新锚最多120、有限模型最多24、候选/缺口各最多10；都是上限，不是配额。真实依赖需要超限时说明文件/作用及取舍，不全树漫游，不靠长文档代替技术增量。优先A状态→B数字→C组合；不足如实说明覆盖，不编造结论。

MANIFEST十四字段必须为task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check。规范self_check直接含verified_claims、quotes_reconfirmed、downgraded_to_inferred三个真实整数；expected/constructor统计只能另列。模型只按Owner/可见信息，后台UNATTESTED，不重复向Owner索取确认。

本任务适用协议v2全部P1–P13。发布前逐项P9：1全包40hex；2/13所有引文连续1–5行、真实单一定义、事实语义确受支持；3固定路径和证据/模型/图节点存在；4附带行号不过界；5六件全部原始标签前缀含说明/字符串计数及三整数等式；6外部结论有可解析来源，否则保留未证；7/8逐字公约/双canary/connector通道；9/10/12无代码/补丁/降级时N/A；11所有未运行项明示。

分别检查字面命中、单一定义归属、支持断言、类型/参数/模型前提和规范字段，不把编号存在等同证据支持。一个宏一锚，匿名枚举用真实唯一成员定位；保留原缩进空行，不能为压数量拼接多个定义。允许既有许可内自写临时纯文本与数学检查，不执行仓库实现；未做的完整P9等如实NOT_RUN。已知错误在发布前纠正，不带错交付再等主线补写。

## 7. G04发布、回读与终态

唯一执行写者12，通过G00后仅新增92目录六件及本人一次PR17顶层正式评论。marker：MYOS2-LEAD-002-CHAT-EXEC-12 / CORE-USER-PRINTF-INTEGER-FORMAT-92 / FORMAL_DELIVERY。原分支claude/dazzling-cori-q0dnyt、PR17不换；C06不并写results，旧11/10/09停用。

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。复用成功blob/tree/commit/评论，写前核真实完整exec HEAD=本批起点、目录和marker；单父提交，不自动变基。

compare严格六件added、旧件零净改删；新固定六件全文、活动六件内容或blob身份、四direct refs、PR16/17和评论全文/真实编号/唯一性全部回读。恢复只补缺项，不重复成功提交或评论，不预造对象编号。

全部完成报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，随后STOP_WRITING，经PR17交原“MYOS2-A-C06 内核分析主线续建”。提醒Owner发正式验收消息前选择Pro，由主线同轮验收92及后继。不得自签92通过，不启动93，不将任务发布当作其它对话已收到或自动启动。

禁止改旧成果/回执/评论、PR元数据/Draft/base、lead/PR16、master/time、内核/loader/initramfs/Owner脚本；禁止补丁、配置/预处理/汇编/编译/链接、仓库程序/脚本/旧夹具/Graphviz/QEMU/实机/真实并发、外部libc镜像、本机凭据、额外模型/子代理/付费API、新分支PR、合并关闭强推删除、阶段3及跨项目维护。110与124继续显式进入以后每次主线交接包、启动词和固定必读清单。
