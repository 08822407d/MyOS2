---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
conversation_display_name: "MYOS2-A-C12 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（本轮系统身份；实际后台和effort未独立认证）"
actual_backend_model: "UNATTESTED"
actual_reasoning_effort: "UNATTESTED"
date: "2026-11-28"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "12535d98d47d"
execution_pin_short12: "ff7c39dea3dd"
read_channel: "GitHub connector；自写有限文本/字节/整数核验"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\", "\tthis_cpu = smp_processor_id();"]
self_check: {"verified_claims": 0, "quotes_reconfirmed": 0, "downgraded_to_inferred": 0}
self_check_scope: "本轮三份主线材料不新增正式源码引文；159原85窗回核、160设计就绪与未来作者P6分账。"
full_local_P9: "NOT_RUN"
actual_G04: "PENDING_POST_PUBLICATION"
R159_01: "OPEN_DOCUMENTATION"
R158_01: "CLOSED_BY_MAINLINE_GRAPH_CONSUMPTION_ADDENDUM_RETAINED"
owner_decisions_requested_now: []
record_type: "bounded_execution_contract"
followup_id: "CORE-USER-LIBC-BOUNDED-STRING-OBJECT-GATES-160"
document_number: 224
status: "READY_FOR_EXECUTION_AFTER_FORMAL_RELEASE"
executor_identity: "MYOS2-A-C11c / MYOS2-LEAD-002-CHAT-EXEC-27"
executor_model_per_owner: "GPT-6.2 Sol＋Extra／高推理；普通执行无需Pro"
executor_branch: "claude/dazzling-cori-q0dnyt"
executor_pr: 17
allowed_result_directory: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-bounded-string-object-gates-160/"
allowed_files: ["MANIFEST.md", "libc-bounded-string-object-gates.md", "facts-and-boundaries.yaml", "libc-bounded-string-object-gates-paths.dot", "deferred-findings.yaml", "evidence.md"]
D00: ["R159-01"]
scenarios: 3
case_rows: 11
next_mainline: "MYOS2-A-C12 / MYOS2-LEAD-002"
mainline_design_count_before: 0
mainline_design_count_after_successful_release: 1
---

# 224号合同 · 第160批 · 有界字符串预算、对象范围与复制顺序

**原MYOS2-A-C11c／EXEC-27继续，使用GPT-6.2 Sol＋Extra／高推理；普通执行无需Pro。** 从本轮PR17真实TASK_RELEASE评论读取同提交的[159独审](reviews/CORE-USER-LIBC-FORMAT-ARG-CONSUMPTION-159-review.md)、本合同及[检查点](checkpoints/2026-11-28-args159-reviewed-bounded160.md)；完成本人规范刷新和fresh G00后，连续完成D00、三个相关专题十一行、恰六件、唯一提交/作者评论及实际G04。通过后STOP_WRITING，交MYOS2-A-C12 Pro独审并同轮后继，不另等开工许可、不进入161。C12接任主线不改变C11c执行身份。

## 1. 新增价值与排重

[124主审](https://github.com/08822407d/MyOS2/blob/12535d98d47d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-LENGTH-COPY-124-review.md)“从输入字节重算”给定充分可读同一对象，并明确按有限地址次序解释指针转换/比较；[124正文和deferred](https://github.com/08822407d/MyOS2/blob/ff7c39dea3dd/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/deferred-findings.yaml)中的SC124-READ-BEYOND-BUDGET仍留下insufficiently readable objects。124全部正常结果及shell cat定位不重做。[126正文](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md)明确容量充分、不重叠；其正常字节复制、补零和最后写者解释作为输入，不作为本批新增结论。158/159格式输出与参数消费也未覆盖本包对象不足首停。

本包分别研究：A端点形成和未对齐前导；B读取预算与整字物理窗口；C先补零后复制在不足目标对象上的首停与部分写入。控制行只用于定位边界，不增加新发现计数。不扩展wordcopy全族、所有字符串库、真实shell链或运行实验。普通发现按18追加收存，无新缺陷配额。

## 2. D00：159文档分类补正，冻结旧原件

R159-01依据同提交159独审§2，当前开放；本执行席只能提交补正候选，闭合由C12后续独立裁定。固定159为 `ff7c39dea3dd`、六件原字节/历史评论/85原窗及P6原报85/85/0均保留。

1. 对旧85正式窗建立完整分类映射。将E055–E059、E061–E062、E064、E067及E068–E083共25项转普通定位，保留旧ID、固定源码path/symbol/行界/原字节；其中5 extern、4类型声明、16用户侧CMake。余60正式窗为E001–E054、E060/E063/E065/E066及E084/E085。E084/E085是P4内核活动配置特例，不扩及用户CMake。原23普通与这25合计48。
2. 在160的MANIFEST/正文/facts/evidence内承载逐项勘误和可重建组合阅读视图，注明supersedes范围只限159证据分类与相应统计/描述。给出固定旧文件、唯一字段路径/物理行或章节ID、旧值、替代值及原因；保持未列出部分完全不变。先核实际选择器唯一，不能盲改全部数字85。
3. 必须覆盖旧MANIFEST的self_check、per_file_formal_counts、coverage.formal_windows和P9对应声明；正文末段的源窗/普通定位总结；evidence末段及25个章节的标签分类；facts.evidence_index与ordinary_location_index的组合视图（60/48）。检查source_facts、source_graph、正文和其他字段消费这25个ID的用途，保留引用ID可解析且明确只支持声明/类型/构建定位。函数行为继续由159已有真实函数/宏/结构定义证据支持，实际write实现不在本包补造，C三例仍是条件请求。
4. 对组合视图计算自己的字面分布与正式计数目标60/60/0，普通定位48；历史原件字面85和原报85/85/0另列。禁止把降普通当作INFERRED计入85/60/25。对旧六件的替代视图可用自写文本助手重建并复算，但不写回旧目录。保留原代码窗的TAB/CRLF/LF/NONE，不重排源文本。
5. 160本轮新P6单列，旧60窗的“组合视图重核数”不混入160新正式标签数。为避免复制旧标签污染新P6，替代动作采用字段化分类映射或分段标签表示；若实际把完整正式标签写进160六件，必须按字面计入160 P6并逐窗复核，不能声称它不计数。无需新增第七件、重做159数值、改旧评论或替旧作者补签历史。

159十行技术结论、图关系、其他历史裁定与未知边界保留。补正完成时写CORRECTION_SUBMITTED_PENDING_C12_REVIEW，不能预写R159-01已闭合或159整体PASS。D00是旧P2解释一致化：强制取证链仍完整，声明/用户构建不能省略但不冒充定义体。主线合同原表述混杂已澄清，非责令Owner另做技术决定。

## 3. 强制源码范围

| 固定time源文件 | 强制完整读取/定义链 | Blob短12 / LF |
|---|---|---|
| [myinitramfs/mylib/myglibc/string/strnlen.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strnlen.c) | 完整__strnlen；L35端点、39–43零预算/回退、47–54前导、85门、116–150整字/确认、153–158夹取 | 3525b2bc96b4 / 158 |
| [myinitramfs/mylib/myglibc/string/strncpy.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strncpy.c) | STRNCPY默认路由与L27–33完整定义；size、先补零后复制 | b661abdab724 / 32 |
| [myinitramfs/mylib/myglibc/string/memset.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memset.c) | 完整memset；len>=8门、76–83逐字节写/整数游标及原实参返回 | bd50b22c66bf / 83 |
| [myinitramfs/mylib/myglibc/string/memcpy.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c) | MEMCPY默认路由、完整定义；L30–36游标/阈值、56–59尾宏/返回 | b7c61fb0c0d4 / 59 |
| [myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h) | op_t/OPSIZ、byte定位、BYTE_COPY_FWD完整宏L78–90、OP_T_THRES L199 | 61d668720747 / 206 |


另外读取 `include/string.h` 的内部__strnlen路由及 `string/string.h` 的公共声明，前缀均为myinitramfs/mylib/myglibc/；声明和typedef按普通定位。用户树 `myinitramfs/CMakeLists.txt`、`myinitramfs/myshell/CMakeLists.txt`、mylib与myglibc及string子目录CMakeLists只用于来源/目标定位。D00涉及159原26份源文件及原证据窗，按本人已读同一固定身份与本次分类用途核对，变化/缺读实际补读；不得复用另一会话的已读。P3/P4仍读 `mykernel/scripts/options_flags.cmake` 和 `mykernel/debug/panic.c`，核time命中/master反验。

正式定义锚必须覆盖：__strnlen端点先于零预算判断、前导读取、整字起点门、真实B掩码、各已达确认分支、char_ptr=end与最终夹取；STRNCPY的size取得、补零调用及复制返回；memset短分支守卫/字节写/游标与原实参返回；MEMCPY阈值/字节宏调用/原目的返回；BYTE_COPY_FWD实际宏体顺序、OP_T_THRES和默认路由宏。只对实际消费的条件实例取1–5行定义窗，结构不能拿外部声明充数。P4活动配置按原协议单列；用户CMake、include、typedef及extern一律普通定位。

## 4. SOURCE_FACT、GIVEN与UNKNOWN

以下是本包显式GIVEN，不宣称仓库证明它们普遍成立：8位字节，64位unsigned long/size_t/指针，小端；每行新建独立存活S/D，互不别名、不重叠、无并发。S/D仅表中列出的物理容量存在，不能补造更大底层对象。各对象基址为可表示低地址，指针/整数往返保留给定地址。

沿用124主审的有限地址模型：转换、比较按给定地址次序解释，包括未对齐end_ptr向unsigned long指针转换，以及memset/MEMCPY的整数游标。这个约定不证明通用C有效类型、指针对齐转换、真实ABI或编译器行为。**本包“首停”仅指此有限模型中第一个不满足对象范围的操作**，不能声称发现了真实C执行的唯一最早未定义行为或实际内存故障。

对象范围门逐次检查：同一数组内或one-past指针形成允许，one-past不允许读取/写入；整字读取必须整个区间在对象内，单字节读写也各自过门。`*longword_ptr++`作为完整待执行访问先检查范围，失败则在该操作前停止，不伪造word、mask或后增。短复制宏中的整数加一按其源码顺序记账，不与指针形成混称。

正常行报告返回和最终对象字节；停止行只报告此前实际事件、局部状态及停止前快照，之后不补读取、存储、helper返回或外层返回。真实shell调用者、实际sh所链libc、有效宏/编译器头、预处理、真实指令和所有I/O均UNKNOWN/NOT_RUN。不得用内核配置替用户ABI背书。

## 5. A：端点形成与未对齐前导，四行

入口均为仓内__strnlen的条件模型；S从[0]起，首地址mod8=1。独立逐行重置。

| case_id | S完整hex / 物理容量 | maxlen | 必须独立推导的边界 |
|---|---|---:|---|
| A160_01 | `41` / 1 | 0 | L35先形成S+0，随后零预算返回0；零源读 |
| A160_02 | `41` / 1 | 1 | end=S+1是one-past；先读S[0]并递增，下一L49读S[1]前STOP；无返回 |
| A160_03 | `41 00` / 2 | 1 | 读S[0]与预算外、对象内的S[1]；char_ptr==end，L51的>为假，返回1 |
| A160_04 | `41` / 1 | 2 | L35的S+2形成即超过one-past，在形成前STOP；end未得、无读取，不能引用后续回退挽救 |

分别列end是否已形成、前导每次余数/读字节/递增、预算与物理对象门；A02不能把已读1字节写成正常返回1。

## 6. B：整字起点门不等于整窗合法，三行

S首地址mod8=0。读取B掩码实际表达式，不能偷换成strlen多一个取反项的表达式。整字字节数、确认字节数、返回值分账。

| case_id | S完整hex / 物理容量 | maxlen | 必须独立推导的边界 |
|---|---|---:|---|
| B160_01 | `41 42 43 44 45 46 47` / 7 | 1 | 起点0<end1，L116整次[0,8)不在对象内，读取前STOP；无word/mask/后增 |
| B160_02 | `41 42 43 44 45 46 47 00` / 8 | 1 | word=0x0047464544434241，B-mask=0x8000000000000000；整字读后依次确认0–7，命中零、夹到end1，返回1 |
| B160_03 | 九个`41` / 9 | 9 | 首词0x4141414141414141、mask0；char_ptr=end9，word_ptr=8。下一起点8<9，但[8,16)不在对象内，第二次L116前STOP；无返回9 |

B03保留“char_ptr已到end但下一整字仍将被请求”的独立游标状态。B02允许读取预算外但对象内的字节；不得把该事实等同所有小预算输入都满足对象门。

## 7. C：STRNCPY先补零再复制，四行

共同入口 `STRNCPY(D,S,5)`；S首址mod8=0、物理8字节 `61 62 63 00 7e 7e 7e 7e`。D各物理字节初始7e，低地址且与S不重叠。__strnlen的整字0x7e7e7e7e00636261、B-mask=0x0000000080000000、确认0–3取得size=3；其读取全在S内。memset长度2<8，MEMCPY长度3<OP_T_THRES16，仅消费短字节路径。

| case_id | D实际容量 | 必须独立推导的终点 | 停止前快照/正常最终hex |
|---|---:|---|---|
| C160_01 | 2 | strncpy L31形成D+3超过one-past，形成前STOP；memset未调用、memcpy未到 | `7e 7e` |
| C160_02 | 3 | D+3可形成one-past，进入memset；L78第一次D[3]存储前STOP，无存储/返回/复制 | `7e 7e 7e` |
| C160_03 | 4 | memset先写D[3]=0，整数游标到4、len=1；第二次L78存储前STOP，无memcpy | `7e 7e 7e 00` |
| C160_04 | 5 | memset写3、4并返回其原实参D+3；后MEMCPY字节宏按0、1、2复制，STRNCPY返回D+0 | `61 62 63 00 00` |

每行分开记录size值、D+size形成、memset调用/逐次写/返回、MEMCPY是否到达、BYTE_COPY_FWD源读和目标写，以及外层返回是否建立。C04总写序3、4、0、1、2，最后写者逐字节明确；C03部分补零不能写成正常复制完成。helper各自返回指针不能都概写为D+0。

## 8. 机器表示、图与六件

facts按11个case_id给出完整输入、源/目标物理对象、预算、已发生的有序事件、已合法形成的指针/整数游标、读宽/byte值、mask、存储事件及最新写者、首次停止候选操作及此前状态、未到达helper。正常返回和停止快照用不同字段；不以nullptr/0冒充未建立的返回。D00可独立重建，引用旧ID的分类变化可逐项检查。

DOT每行有可对应facts的seq链，所有相邻顺序边为LOCAL_CONTROL，事件种类在节点属性。STRNCPY拥有__strnlen/memset/MEMCPY直接调用；MEMCPY拥有BYTE_COPY_FWD宏展开；helper之间只在控制或数据关系上相连，不画相互直接调用。返回值来源用RETURN_DATA，正常返回控制条件另列；STOP无通往正常结尾的边。可只用文本解析核图，不运行Graphviz。

六件即头部allowed_files，共用一次G00/规范加载/证据索引。正文解释D00与A/B/C，facts承载可复算账，DOT表达关系，deferred追加限定发现，evidence保留新定义窗/普通定位和D00分类映射，MANIFEST记实际过程/P6/G04。不要复制无关整套定义或将整份历史材料重发。所有表从固定源和GIVEN重算；不把本合同结果表硬编码成检查器oracle。可以使用自写有限文本/字节/整数助手，声明它不是项目执行或真实C语义认证。

deferred仅给 `CORE-USER-LIBC-STRING-LENGTH-COPY-124::SC124-READ-BEYOND-BUDGET` 追加本有限模型证据；旧SC124-ABI-ASSUMPTIONS、SC124-SH-LINK、SC124-STRNLEN-SYMBOL、SC124-STRNCPY-HELPERS保持UNKNOWN。C目标门可新增本批限定观察，不要求缺陷数量、不扩大为实际可利用性结论。R152/R155/R158既有闭合与历史RETURN保持；R159-01补正待主线。

## 9. 同席规范刷新、授权与口述约定

业务授权来自Owner已要求159独立验收并同轮后继、00/28既有连续安排及本合同。仍是阶段2和原任务/exec分支/Draft PR17；唯一执行写区是本160目录六件。主线C12由223接任；不得因为主线接任而新开执行席或复用前任C11b已读。

| 当前必需规范 | Blob短12 | LF |
|---|---|---:|
| [00-work-order.md](00-work-order.md) | 9e18ca4a42d7 | 424 |
| [README.md](../../README.md) | 1deef3891013 | 52 |
| [conventions.md](../../conventions.md) | ca21465d1319 | 63 |
| [00-gpt-task-protocol-v2.md](../../tasks/00-gpt-task-protocol-v2.md) | ebb0e304cf48 | 117 |
| [18-deferred-findings-and-resume.md](18-deferred-findings-and-resume.md) | 4d22d565ab9e | 108 |
| [24-chatgpt-specialist-conversation-separation.md](24-chatgpt-specialist-conversation-separation.md) | a0156f00594d | 41 |
| [28-review-and-next-batch-same-turn.md](28-review-and-next-batch-same-turn.md) | 9a8b0d039cef | 45 |
| [63-github-write-retry-and-readback.md](63-github-write-retry-and-readback.md) | 2dd70d822b3b | 79 |
| [79-work-package-sizing-and-pro-review.md](79-work-package-sizing-and-pro-review.md) | 3b92828411e5 | 68 |
| [110-owner-communication-and-delivery-continuity.md](110-owner-communication-and-delivery-continuity.md) | ae8f04daa248 | 58 |
| [124-model-routing-and-pro-message-budget.md](124-model-routing-and-pro-message-budget.md) | c84b0e04c524 | 65 |
| [193-mainline-context-thresholds-and-model-availability.md](193-mainline-context-thresholds-and-model-availability.md) | a66d24912f6f | 68 |
| [194-owner-continuity-and-copyable-return.md](194-owner-continuity-and-copyable-return.md) | 9ad1f8667620 | 140 |
| [198-repository-first-compact-relay.md](198-repository-first-compact-relay.md) | 5f2b12f75534 | 59 |
| [210-mainline-c11-recovery-and-context-thresholds.md](210-mainline-c11-recovery-and-context-thresholds.md) | 56dddd4d796d | 67 |
| [223-mainline-c12-recovery-and-context-thresholds.md](223-mainline-c12-recovery-and-context-thresholds.md) | 968a986742bb | 122 |


共16份1576 LF。00已新增第21条、现424 LF，223为122 LF；C11c必须实际读取新增/变化内容，并核其余14份当前Blob与适用性。79仅允许同一会话对本人确已完整读过、固定身份相同的全文复用；取得文件或他席读账不能替代本人加载。

Meta-Agent本轮fresh核master=b234c6c91a2e、建设06b783861368，入口881fd8c1076e，检查点54ac6e5fc66c。有效采用仍48→47 **v1.4-r10.266**（固定119597a75517，47 Blob e3c7e284a6c4），45同版共用**1.1-r9.260**归档3492039a99ba含01–14/16十五份全文，model registry0924de22a54d；入口见[现行规则](https://github.com/08822407d/Meta-Agent/blob/b234c6c91a2e/current/behavior-guidance-entry.md)。核建设检查点的新适用采用段，§322待议不作采用。已读同版可据79复用，任何变化/缺读实际补读；不从外仓继承业务/付费/写权。

Mnemosyne master=e581eb9df6c4，仅任务局部读取/命名依据；README、receive、批准规范、命名Blob依次04e450fc22f7/173f2ea983fd/058bdbbe3ccb/2d930f07381e。命名[§7原件](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/notes/registries/project-research-display-name-registry-v0.1.md)L445–584保留C11c显示名，主线为C12，不声明UI已改名。

MANIFEST显式receiver_guidance_load：project_guidance与meta_agent_guidance=required；mnemosyne_guidance=no，receive_format=task_local_only，naming=required。status据实记refresh_completed/task_local_only/current_task_preserved；逐件注明本人全文来源、同版复用或新增实际读取、适用性/冲突/缺项。当前加载不能倒签159历史，模型后台/effort未认证不成为重复索要授权的理由。

口述约定继续：18普通问题先入库，28验收后继同轮，63先查效果再限次重试，79约三个关联专题合包，110人工动作集中，124按整轮选模，198仓库承载完整材料/双向单个3–6行短入口，223传递身份和8/10计数。C12首次成功发布160计0→1；C11历史152–159共8保留。下次交接带16规则、届时有效MA全文/同版十五全文、Mne局部依据、真实断点与各代计数，不自动停工/移权。

## 10. fresh G00与冻结基线

1. 直接四refs：lead为正式发令明示的真实发布提交；exec=ff7c39dea3dd、time=a039d9803ade、直接master=e8e60cd19540。PR.base_sha不能替master。PR16 Open/Ready、PR17原标题Open/Draft，原head/base master、未合并；开放PR全分页至空。
2. PR17无since全分页至空。主线写前314评论/76精确标记，主线本次一评论双标记后预计315/78；按[本检查点](checkpoints/2026-11-28-args159-reviewed-bounded160.md)完整逐字/真实ID核对，不能只核数量。新159主审/160发令、159作者7212991140、旧159发令7212542733各直接ID双读与分页正文/两时间相同。
3. 检查点22负标记全部0；正确EXEC-27/159恰1、正确EXEC-27/160入场0。完整tree/全部评论检查160目录及任意身份160作者占用，未知并行变化先保全、不覆盖。旧158改派/闭合保持，不因恢复重发。
4. 159六件固定ff7、活动exec、API Blob全文一致；535680字节/19893 LF/156 CRLF；唯一父bf0a7ea26dcc，一提交六新增，2277旧非tree冻结。此次只重核身份和159主审消费条件，十行已成立结果不重做；D00分类按本合同实际核。160写前exec2681项/2283非tree全部冻结；主线发布后预期2084项/1849非tree，作为执行者全只读基线，实际计数为准。
5. 两条time金丝雀、直接master反验、公约完整原句实核。MANIFEST包括read_channel、self_check三整数、D00=[R159-01]及待审状态、R158/R155/R152保持、本人规范/权限/容量和真实下一步。G00完成后连续D00+A/B/C，不能拿主线READY替自己的G00。

## 11. P1–P13、发布与实际G04

恰六件；新增正式定义窗1–5连续物理行，真实归属且原TAB/CRLF/LF/NONE逐字保留，P6字面计数与三数等式成立。旧159原85/更正60、主线0和READY分别记账。P9可达项逐项登记：1长哈希零；2定义体/原字节；3输入/引用/supersedes路径实存；4真实行界；5P6字面与等式；6外部URL/无内部检索句柄；7双金丝雀/反验；8公约/read_channel；9无代码件NA；10无补丁NA；11无未运行却声称运行完成措辞；12非降级NA；13本合同强制锚完整。可达项不冒充完整本地P9。

写前fresh四refs、原PR、目录/任意作者占用、全评论。以ff7c39dea3dd为唯一父，expected_sha匹配、force=false，原exec一次提交六新增零其它改删。先实际回读六件固定/活动/API Blob三渠道全文、独算Blob/字节/LF/行尾、唯一父/compare、全部2283旧非tree和完整lead冻结，再发唯一作者评论；局部成功先查效果再补缺项，禁止重复。

唯一作者标记：

MYOS2-LEAD-002-CHAT-EXEC-27 / CORE-USER-LIBC-BOUNDED-STRING-OBJECT-GATES-160 / FORMAL_DELIVERY

作者评论含真实合同/父/提交/六件、三场景十一行、D00更正60/48与当前P6分账、R159-01待主线、规范实际加载、独立回源及限制；发表时actual_G04=PENDING_POST_PUBLICATION、mainline_acceptance=PENDING。发表后新ID直接双读并与完整分页同文同时间；78旧标记+新作者=79，各一次且真实ID正确，22负标记0；预计316评论，仍读至空，315旧评论正文/两时间逐条冻结。再核四refs/原PR/开放页、六件三渠道、一父一提交六新增、旧2283非tree与完整lead冻结。实际通过才G04=PASS、READY_FOR_MAINLINE_REVIEW、STOP_WRITING，不进入161。

63恢复通常首次加三次10/20/40秒或实质工作间隔；先核实际效果，参数错误/读回暂态/真实写重试分账，未知并行写不覆盖。最终按198给一个原生3–6行短复制块，标题“160｜下面一段转发给MYOS2-A-C12”，一个真实作者评论URL，报告实际终点、D00待独审和必要未知，请Pro独审并同轮后继。完整材料留GitHub，不让Owner拼接。

## 12. 设计就绪与阶段2终点

主线读取五份固定源及124/126历史选择器，核出三个64位word/mask与11行终点；辅助候选自写有限模型和另一独立只读源码追踪一致。后者完整读五源、核重算Blob，逐行验证指针形成、整窗读、短helper到达与返回，不使用前一辅助脚本作oracle。主线核定有限地址语义及首次停止范围后形成此合同；该READY不是盲测声明，也不代签未来作者成果、G00/P6/G04或本地P9。

仅静态源读和自写有限文本/字节/整数核验。不执行项目程序/脚本、预处理、C/ASM、编译链接、Graphviz、QEMU/实机/真实I/O；不改源码/旧件/旧评论/主线/PR元数据，不新建分支/PR、不合并/强推/删除、不跨仓写或新增付费API。真实ABI/链接/内存行为与后台模型/推理档未认证，完整本地P9 NOT_RUN。
