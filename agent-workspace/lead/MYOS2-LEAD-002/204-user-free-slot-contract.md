---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
conversation_display_name: "MYOS2-A-C10 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前系统标识；后台及effort未独立认证）"
model_per_owner: "沿用正式独审使用Pro的持续要求；本轮Owner要求核实并恢复工作"
date: 2026-11-25
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: f9de786c34d4
execution_pin_short12: 42066de2e04f
read_channel: connector
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
record_type: phase2_executor_contract
followup_id: CORE-USER-FREE-SLOT-147
status: PUBLISHED_PENDING_EXECUTOR_START
executor_display_name: "MYOS2-A-C10b 代码分析执行"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-23
executor_model_per_owner: "GPT-6.2 Sol；Owner界面可用Extra／高推理强度"
actual_147_receiver_state: NOT_OBSERVED
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-free-slot-147/
allowed_files: [MANIFEST.md, free-slot.md, facts-and-boundaries.yaml, free-slot-paths.dot, deferred-findings.yaml, evidence.md]
D00: []
prior146: PASS_PENDING_LOCAL_WITH_MAINLINE_CONSUMPTION_NOTE
combined145: PASS_PENDING_LOCAL
R145_01: CLOSED_DOCUMENTATION_ONLY
R145_02: CLOSED_DOCUMENTATION_ONLY
R145_03: CLOSED_BY_CORRECTED_CONSUMPTION
R146_N01: CLOSED_BY_THIS_MAINLINE_METADATA_CORRECTION
mainline_design_count: {conversation: C10, first_batch: 141, before_release: 6, after_successful_formal_release: 7, latest_batch: 147, threshold_12: NOT_REACHED, threshold_15: NOT_REACHED}
external_read_channel: web_primary_sources
self_check_scope: "本合同不新增正式源码窗；外部条款与合同给定模型分列，执行者自行回源"
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/203-user-malloc-colormap-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MALLOC-COLORMAP-146-review.md"
  - "myinitramfs/mylib/myglibc/malloc/malloc.c"
  - "myinitramfs/mylib/myglibc/malloc/_malloc.h"
  - "myinitramfs/mylib/include/utils.h"
  - "myinitramfs/mylib/util/bitmap.c"
  - "myinitramfs/mylib/util/list.c"
  - "myinitramfs/CMakeLists.txt"
open_questions: ["实际标准/编译器/DEBUG/链接、真实C对象/载荷与运行未证；不展开回收递归"]
---

# 204号合同 · 第147批 · free前半段、位图宽度和释放计数

**继续现有MYOS2-A-C10b／EXEC-23，次一档GPT-6.2 Sol及Extra／高推理强度。** 本批CORE-USER-FREE-SLOT-147：A7对齐键查找、B11独立位图helper模型、C6条件性free路径，约三个相关专题，共用六件交付。D00为空；[146独审及§4补注](reviews/CORE-USER-MALLOC-COLORMAP-146-review.md)已关闭145三项补正，旧成果、旧RETURN记录和评论全部保留。

Owner转发本轮正式发令即授权连续完成G00、A/B/C、自检、六件提交、唯一作者评论、实际G04及停写；不再等待第二次开工。按[198号](198-repository-first-compact-relay.md)，完整任务/证据/回执留GitHub，最终聊天只给一个短复制块。

## G00 · 复用身份，核准当前断点

1. 本次真实发令评论ID双读，取得同提交204、146独审和[本轮检查点](checkpoints/2026-11-25-colormap146-reviewed-free147.md)。合读146原件与主线§4：旧145 facts Blob有效值387b9eb01d02；规范物理行口径按主线补注，不重抄错码、不重做已闭D00。
2. 继续[202§3列明的十四份规则](https://github.com/08822407d/MyOS2/blob/7f68407aec9f/agent-workspace/lead/MYOS2-LEAD-002/202-executor-continuation/00-C10a-to-C10b-recovery-package.md)：工作令、README、公约、协议v2、18/24/28/63/79/110/124/193/194/198。79仅允许复用本C10b自己实际完整读取且未变的材料；实际遗漏或截断须先补完。MANIFEST记真实复用/重读范围，不用前任或主线的已读代签。
3. 刷新Meta-Agent及Mnemosyne真实refs和有效入口。主线本轮核master/建设仍b234c6c91a2e/50806a75e050，Mnemosyne仍e581eb9df6c4；有效规范仍v1.4-r10.266及共用1.1-r9.260。来源与采用顺序沿202§4，缺少全文先补；接收格式/命名仅任务局部，业务始终147，不导入其他项目写权。无需新接收身份或重复旧交接，刷新记录如实留证。
4. 直接四refs：lead为本次发布提交，exec预期42066de2e04f、time=a039d9803ade、master=e8e60cd19540。PR16 Open/Ready、PR17原题Open/Draft，原head/base和未合并保持；不能用PR.base_sha替代直接master。
5. PR17顶层评论无since完整分页至空、开放PR完整分页至空。25个既有标记加本次146独审、D00_145_CLOSED与147发令三枚，共28个各唯一；146作者[7137058632](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7137058632)双读。旧EXEC-22的145作者0。146固定/活动六件、实际Blob及单父b094af6bb8f3的一提交六新增相合；145冻结件及原作者评论保持。
6. 写前确认147目录/作者标记未出现，获取真实exec expected_sha。自身相符部分交付按63续缺项，未知并行变化保全定位，不覆盖、强推或重复发评。无真实阻断即继续；普通问题先入库，不向Owner逐项提问。

C10已设计6批，147成功正式发布后为7；恢复核查与同批补正不加数，12/15提醒未到。不得据此宣称已测得上下文容量。

## 共同范围与给定模型

适用协议v2全部十三条。仍读固定time的仓内myglibc候选实现；其存在不证明sh实际链接它，actual_linked_to_sh=UNKNOWN继续继承。

给定8位byte；size_t、unsigned long、bitmap_t、unsigned long long及地址数值按64位模型；bitmap_t无符号，sizeof=8、BITMAP_UNITSIZE=64。int及unsigned为32位，INT_MAX=2147483647；所有指定计数/差值可表示，无回绕、并发、异常对象或实际运行。

A使用64位无符号地址投影，不证明真实指针转换/对象有效。B只采用下节明确的ISO草案条款作为有限文义模型，不认证本项目实际标准/编译器。C进一步给定相关对象、循环列表、container_of及助手合法，void指针差按给定字节差投影理解；不是标准C对象合法性证明。

C中DEBUG未定义为GIVEN。实际宏选择/预处理UNKNOWN/NOT_RUN，不能从内核配置或顶层条件构建文本推断该用户库的实际选择。先指出DEBUG门位于NULL判断之前，再分析给定排除该门的六例。

146新bitmap真实初值仍UNKNOWN；本批指定word只是独立模型输入，不关闭初始化缺口。正常C例只取低位、安全已赋值槽并隔离回收体，不拼接首次初始化、递归或真实堆运行。

## 有界外部语义依据

执行方读取以下三个官方来源的指定部分，登记URL、标题、版本/访问日期、所读条款及支持范围；不要求全文读取整份标准，也不扩展为编译器评测：

- [EXTERNAL WG14-N1547] [ISO/IEC 9899:201x Committee Draft，2010-12-02，N1547](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1547.pdf)：§6.4.4.1¶5及表（整数常量选型）；§6.5.7¶3–4（分别整数提升、左类型、计数/可表示边界）；§6.2.5¶19及§6.5.6¶3、9（void与指针相减条件）。采用所列历史条款作合同模型，不声称它就是本项目选中的标准。
- [EXTERNAL GCC-POINTER] [GCC Arithmetic on void- and Function-Pointers](https://gcc.gnu.org/onlinedocs/gcc/Pointer-Arith.html)：只取GNU C对void指针加减按大小1处理的扩展说明，支持C中“字节差投影/扩展”与标准对象条件的区分。
- [EXTERNAL GCC-SHIFT] [GCC Integers implementation](https://gcc.gnu.org/onlinedocs/gcc/Integers-implementation.html)：只取signed左移扩展说明，防止把ISO模型结论写成实际GCC行为；本批不推导该仓某GCC版本、选项、指令或sanitizer结果。

主线实际读通N1547上述段落；N1570 PDF曾超时，未以未读原稿作完成声明。执行方若某官方入口暂态失败，可用官方检索定位到相关条款后读入；不要把镜像摘要冒作原文，也不要把后续修改建议当已采用规则。确实无法取得时，保留本合同给定边界、将自身外部取证标UNKNOWN，A/C及源码事实继续完成；B11也按合同明确的GIVEN边界完成并标明依据是本合同，外部独立取证仍UNKNOWN。准确交回外证缺口，不能漏交B、虚签外证完成或把GIVEN升级为自行回源结论。

## A · upage_search的2MiB对齐键匹配（7例）

H=0x200000取本地CONST_2M/PAGE_SIZE。非空例给合法used_upage循环链，遍历顺序为[D2,D0,D1]，三描述符vaddr分别为D0=0x60000000、D1=0x60200000、D2=0x60600000；count=3。空例anchor自环、count=0。地址只是给定数值与描述符对应关系。

| ID | 输入地址x | 链 |
|---|---|---|
| A_FIRST | 0x60601234 | [D2,D0,D1] |
| A_BASE | 0x60000000 | [D2,D0,D1] |
| A_LASTBYTE | 0x601fffff | [D2,D0,D1] |
| A_NEXT | 0x60200000 | [D2,D0,D1] |
| A_NEXTLAST | 0x603fffff | [D2,D0,D1] |
| A_MISS | 0x60400000 | [D2,D0,D1] |
| A_EMPTY | 0x60000000 | [] |

逐例列round mask、取整键、已比较描述符序列与次数、相等分支、返回描述符或NULL、break/anchor终止原因及元数据不变范围。不要把uPage指针返回写成直接返回对齐地址。比较的是描述符vaddr与取整键相等，不是区间包含检查；循环以anchor为界，不以count决定遍历。未命中例止于search返回NULL，不继续解引用；记录free下一句直接消费返回值的接口边界即可，不造NULL访问结果或实测异常。

A只重读H/取整宏、upage_search和相关类型；不重做144的brk32轮或真实页申请。

## B · assigned-bit与clear-bit独立模型（11例）

每例给合法、足够的两个64位word。除B_UNSET输入为[3,0]外，其余均[0xffffffffffffffff,0xffffffffffffffff]。idx依次如下：

| ID | idx | 初始word例外 |
|---|---:|---|
| B_0 | 0 | 无 |
| B_UNSET | 2 | [3,0] |
| B_30 | 30 | 无 |
| B_31 | 31 | 无 |
| B_32 | 32 | 无 |
| B_63 | 63 | 无 |
| B_64 | 64 | 无 |
| B_94 | 94 | 无 |
| B_95 | 95 | 无 |
| B_96 | 96 | 无 |
| B_127 | 127 | 无 |

分别分析两个helper，不能把clear接在已停止推导的assigned路径之后当作同一次实际执行：

1. assigned：源中无后缀1为int；先算unit_idx和余数t，区分左操作数类型、表达式类型、赋给bitmap_t的转换及unsigned返回变量。移位对两侧分别提升，右侧宽类型不自动把左边1提升成64位。本合同ISO/int32模型中t=0–30可继续求mask、unit&mask、返回数值和非零判定；t=31为有符号结果不可表示边界；t=32–63为计数达到/超过左类型宽度边界。
2. 危险组写ANALYSIS_STOP，mask、后续读取/返回值NOT_DERIVED。不填猜测CPU值，不写“函数返回0”，也不把分析停止伪装成源码分支或NOT_REACHED运行事实；实际GCC扩展行为不在本批推导内。
3. clear：从同例原始word重新独立出发，按1ULL及64位模型计算word索引、shift、mask、有限64位补码掩码、前后两个word与改动位数；函数返回类型void，不编造返回值。t均0–63，不读第三word。B_UNSET明确本次清除不改变原word，不由此证明整体free允许重复释放。

把余数31和32以上两类分开记录；类型赋值发生在表达式求值之后，不能以目标bitmap_t宽度替前面的int移位补合法性。只做这11组，不推广所有配置、编译选项或机器行为，不修改helper。

## C · free的低位条件释放、计数与列表（6例）

独立于A/B实际执行。除C_NULL外，给定upage_search正常返回有效page、page.uslab_p=S、S.uslabcache_p为指定cache；S.V=0x60000000，O=0x4000，N=128。另有R，total=free=128、两word为0，正常空闲备用slab；全cache只有S和R，normal_slab_total=2。used_upage链及助手合法。以给定字节差O×idx形成obj_p，实际对象/指针差合法性不另作证明。

列表顺序和计数由表给出；所有其他列表空，count等于所列节点数，缓存对象计数与bitmap/空闲数一致。F/U/L分别为normal_slab_free/used/full的成员列表；CF/CU为nsobj_free_count/nsobj_used_count，避免混淆slab数与对象数。

| ID | obj_p或idx | S.free | S.word0 / word1 | F / U / L | CF / CU |
|---|---|---:|---|---|---|
| C_NULL | NULL | 不消费 | 不消费 | 不消费 | 不消费 |
| C_FULL | idx0 | 0 | 全1 / 全1 | [R] / [] / [S] | 128 / 128 |
| C_USED | idx30 | 64 | 全1 / 0 | [R] / [S] / [] | 192 / 64 |
| C_LAST | idx0 | 127 | 1 / 0 | [R] / [S] / [] | 255 / 1 |
| C_EMPTY | idx0 | 128 | 0 / 0 | [S,R] / [] / [] | 256 / 0 |
| C_UNASSIGNED | idx2 | 126 | 3 / 0 | [R] / [S] / [] | 254 / 2 |

逐例依源级次序记：DEBUG给定门、NULL门、lookup及两级归属读取、空slab检查、字节差与obj_idx、assigned非零门、clear、S.free/CF/CU更新、使用更新后free的两个列表条件、回收条件及观察终点。每例最多10个有意义阶段，允许合并同组；每个运行条件分别记reached/result/body_entered，不能混写“未到达”与“到达但为假”。DEBUG属于给定的预处理排除，该项运行reached/result/body_entered记N/A，不画成实际运行分支。

- C_NULL在给定排除DEBUG门后正常void早返；lookup及其后不达，不写成返回NULL指针。
- C_EMPTY在空slab显式while(1)处停止源级路径，不求obj_idx、bitmap或后续变化；C_UNASSIGNED到assigned结果为0的显式while(1)，之后clear/更新/迁移不达。这里是源码循环边界，不与B的ANALYSIS_STOP混为一类。
- 三正常例的bit均为1且余数0或30，先clear再加free/CF、减CU；列表判断消费更新后的S.free。单列S与R载荷/链接、三个列表成员和count、normal_slab_total及CF/CU。
- 对C_LAST特别核更新后free=N时，源else-if检查的是0。记录实际条件结果和S最终归属，不按注释改写代码或自动迁入free。只在本批无回绕、原free<N的范围论证该0条件不可满足，不宣称所有畸形前态都不可进入。
- 三正常例的free-list count≤2，均到达回收门且为假，回收主体不进入；正常到函数末端的条件性void完成与实际运行分开。不展开uslab_free、upage_free、回收递归或后续use-after-free等专题。

核三正常例对象数守恒、bitmap已占位计数与S.free关系；C_LAST的计数与列表分类分别记录，不能用计数相合掩盖列表归属结果。普通观察归deferred，不征求Owner现在修复。

## 固定输入、正式锚与范围上限

只读历史：本轮146独审全部及§4补注；146固定正文、MANIFEST、facts中相关边界/类型、deferred；145原独审与146 D00仅确认关闭及消费关系，不重新审全部17组/55窗。原144及更早只保留既有状态。

固定time源码：malloc.c的H/round宏、全upage_search与全free；_malloc.h所用uPage/uslab/cache字段；utils.h的bitmap/单位、container_of及List/List_hdr；bitmap.c assigned/clear全体；list.c实际删除/初始化、头/尾插入与计数包装。malloc/init/uslab_alloc只定位与本批接口相连的既有边界，不展开完整递归。

配置/构建：mykernel/scripts/options_flags.cmake、mykernel/debug/panic.c与直接master反验；myinitramfs/CMakeLists.txt及mylib、myglibc、malloc、util五份构建文本，明确DEBUG条件文本与真正用户库选中状态不同。外部输入仅上列三来源的限定条款。

强制正式源码窄窗必须支持：H及round宏链；search的取整/anchor循环/container_of/比较/break/NULL返回；free的DEBUG在先、NULL、归属读取、两个拒绝循环、obj_idx、clear及三个对象计数、更新后free两条件、列表动作与count、回收门；assigned完整类型/移位/读取/返回，clear的1ULL表达式；相关类型成员及真实列表助手体；活动配置/构建及实际链接未知。定义/宏/声明/配置指令分性质，每窗1–5连续物理行，不以声明代替实现体。

主线仅给模型输入和取证要求，执行者独立回源、重算并保留反例；不把合同中的计算方向当源码证据。原始CRLF与NONE按可逆方式保存，NONE的围栏分隔LF独立记录；物理行不把末尾split空项算新行。引用Blob从实际对象复制，尤其旧消费表不得手拼短码。

不扩展：完整初始化/分配/回收递归、144/145/146旧正确模型重跑、全部任意指针或越界/失配对象、并发、实际ABI/预处理/编译/链接/指令结果、真实分配器行为、修复补丁、内核free对比。回收体仅标为当前正常例未进入；不把本批称为完整free安全性结论。

## 六件、自检与验收

仅user-free-slot-147六新增。facts至少含given_model、external_basis、page_lookup、bit_helpers、free_cases、build_scope、source_index、graph及graph_coverage。A7/B11/C6共24个有界记录；可参数化图，但必须覆盖每行且区分源码路径、独立helper与分析停止，不强迫复制24个同形链。

正文/facts数值及终点一致；DOT/facts.graph按节点id/kind/label/scenario、边from/to/kind/label逐字段一致，关键条件和计数时点也实核，不只检查同构。B危险移位分析边界不画成真实return/循环；独立clear没有来自该边界的实际调用边。C的循环与NULL早返、三正常例各自可定位。

MANIFEST标准根实际存在read_channel、base_snapshot、inputs_read、startup_selfcheck_quote、branch_canary_quotes、self_check三个整数；交付前重新打开每个正式固定源窗逐条复核，n=m+k，强制点不得降级或遗漏。新P6只计147正式前缀；不重贴145/146旧窗当新证据。六件用途/覆盖/后继消费齐，来源/读取时点未知保留UNKNOWN，不追签此前行为。

P9可达1–8、11、13逐项实际核；9/10无代码/补丁、12非降级时NA。外证用可解析URL及条款/版本，不写内部检索句柄，源码formal与外证分账。完整本地P9仍NOT_RUN。deferred仅本批新增观察或真实旧项状态变化，具source_ref及唯一selector；不重复计旧链接、载荷和递归UNKNOWN。没有行数/引文数/新缺陷数配额，Owner当前无待决策项。

## 发布、实际G04与停写

原exec写前读真实expected_sha，force=false，单父快进恰六新增零其他改删；写后固定/活动全文、LF/字节/API Blob、唯一父及净差异。发评前无since完整分页至空查重；唯一正式作者标记：

MYOS2-LEAD-002-CHAT-EXEC-23 / CORE-USER-FREE-SLOT-147 / FORMAL_DELIVERY

正式评论含固定合同/提交/六件、A/B/C范围、外证与GIVEN边界、实际自检及模型可核信息。发表后真实ID双读、完整分页至空；核28个历史/主线标记各唯一及147作者唯一、旧EXEC-22的145作者0；四refs、开放PR2/0与两PR原状态/head/base/PR17原题；六件固定/活动/API Blob、单父净差异；146冻结六件及作者评论保持。实际G04完成才READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、STOP_WRITING，不自行启动148。

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected_sha/force=false；未知并行写不覆盖。记录实际重试/回读，没故障记not_needed，不把工具缺失推断成无仓库权限，不重复提交或发评。

最终聊天按198只给一个通常3–6行的原生短复制块：147真实交付/主线待审/G04/停写状态、一个正式作者URL，请C10 Pro独审及同轮后继。完整数据、证据和说明留GitHub；受阻仅简述必要缺口。阶段2，原件冻结、运行禁令、次一档执行/Pro独审及未来交接必传约定继续有效。
