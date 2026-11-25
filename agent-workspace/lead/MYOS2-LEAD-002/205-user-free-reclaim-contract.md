---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
conversation_display_name: "MYOS2-A-C10 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前系统标识；后台及effort未独立认证）"
model_per_owner: "Owner明确要求Pro独立验收及同轮后继"
date: 2026-11-25
date_timezone: Asia/Singapore
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: 00a7ead0ddcd
execution_pin_short12: 12b3784893e5
read_channel: connector
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
record_type: phase2_executor_contract
followup_id: CORE-USER-FREE-RECLAIM-148
status: PUBLISHED_PENDING_EXECUTOR_START
executor_display_name: "MYOS2-A-C10b 代码分析执行"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-23
executor_model_per_owner: "GPT-6.2 Sol；Owner界面可用Extra／高推理强度"
actual_148_receiver_state: NOT_OBSERVED
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-free-reclaim-148/
allowed_files: [MANIFEST.md, free-reclaim.md, facts-and-boundaries.yaml, free-reclaim-paths.dot, deferred-findings.yaml, evidence.md]
D00: []
prior147: PASS_PENDING_LOCAL_WITH_MAINLINE_CONSUMPTION_NOTE
R147_N01: CLOSED_BY_THIS_MAINLINE_CONSUMPTION_NOTE
N1547_author_independent_evidence: UNKNOWN_INHERITED
mainline_design_count: {conversation: C10, first_batch: 141, before_release: 7, after_successful_formal_release: 8, latest_batch: 148, threshold_12: NOT_REACHED, threshold_15: NOT_REACHED}
self_check_scope: "本合同不新增正式源码窗；15个给定片段是任务设计，执行者独立回源"
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/204-user-free-slot-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FREE-SLOT-147-review.md"
  - "myinitramfs/mylib/myglibc/malloc/malloc.c"
  - "myinitramfs/mylib/myglibc/malloc/_malloc.h"
  - "myinitramfs/mylib/util/list.c"
open_questions: ["完整回收递归、对象存活/访问、真实页回收、实际链接/编译及运行未证"]
---

# 205号合同 · 第148批 · free回收门、描述符迁移与调用边界

**继续现有MYOS2-A-C10b／EXEC-23，次一档GPT-6.2 Sol及Extra／高推理强度。** CORE-USER-FREE-RECLAIM-148包括A6回收入口、B4页描述符迁移、C5递归边界/独立续段，共三个相关专题、15个有界记录、一套六件。D00为空；[147独审§4](reviews/CORE-USER-FREE-SLOT-147-review.md)只限定C.clear算术子记录的消费含义，不要求重做147。

Owner转发本次真实正式评论即授权连续完成G00、分析、自检、六件提交、唯一作者评论、实际G04及停写。按[198号](198-repository-first-compact-relay.md)，完整材料留GitHub，最终聊天只给一个短复制块和一个正式作者URL。

## G00 · 当前断点与规范复用

1. 本次正式发令按真实ID双读，取得同提交205、147独审及[本轮检查点](checkpoints/2026-11-25-free147-reviewed-reclaim148.md)。147作者[7149259180](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7149259180)双读；147固定12b3784893e5六件与主线§4合读。B的独立helper入口和C的同片段assigned→clear顺序不能混同。
2. 沿[202§3十四份项目规则](https://github.com/08822407d/MyOS2/blob/7f68407aec9f/agent-workspace/lead/MYOS2-LEAD-002/202-executor-continuation/00-C10a-to-C10b-recovery-package.md)和79，只复用本C10b自己此前完整读取且未变的材料；遗漏/截断先补读。MANIFEST记录实际复用与本轮新读，不继承他人的“已读”。保留工作令、README、公约、协议v2及18/24/28/63/79/110/124/193/194/198。
3. 刷新Meta-Agent及Mnemosyne真实refs/入口；本次主线观察仍b234c6c91a2e/50806a75e050、Mnemosyne=e581eb9df6c4，有效通用v1.4-r10.266及共用1.1-r9.260未变。按202§4辨认采用决定/全文；范围是本项目任务局部，不导入跨仓业务或写权。无需重启旧202交接，实际接收、规范刷新、可见模型与UNATTESTED边界如实记录。
4. 直接四refs：lead为本次发布提交；exec预期12b3784893e5，time=a039d9803ade，master=e8e60cd19540。PR16 Open/Ready、PR17原题Open/Draft，原head/base及未合并保持。不能以PR.base_sha替代直接master。
5. PR17顶层评论无since完整分页至空；本轮检查点所列29枚既有标记与本次147独审/148发令两枚，共31枚各唯一。旧EXEC-22的145作者0。开放PR完整分页至空仅16/17。147固定/活动全文、LF/字节/API Blob和唯一父42066de2e04f的一提交六新增相合；前序冻结件/作者保持。
6. 写前确认148目录/作者标记未出现，读真实exec expected_sha。自身相符部分发布按63续缺项；未知并行变化保全定位，不覆盖、强推或重复评论。无真实阻断即继续，不把普通分析问题送回Owner逐项决定。

C10已设计7批；148成功正式发布后为8，12/15提醒均未达到。改派/补正/验收不加数；不推断实际上下文剩余量。147 N1547独立外证UNKNOWN继承，本批不以重新取得它为前置门。

## 共同给定条件和观察方法

只读固定time的仓内myglibc候选实现；actual_linked_to_sh=UNKNOWN及实际标准/编译器/DEBUG/ABI/映射/运行未知继续保留。给定所涉计数可表示、无算术回绕、无并发。除各例明确标UNKNOWN或另设前提者外，给定相关对象存活、字段类型/访问合法、循环链与container_of/列表助手适用；除明确指针关系外，列表anchor、节点、cache字段与page对象不别名。

**这是分段静态模型。** A从free回收门前的给定新入口开始；B独立调用upage_free；C按明确的调用前或返回后新入口取证。147三个正常C例回收门为假，不能把它们接到本批回收主体。某调用“已返回”不自动保证被释放对象存活，也不保证cache/链/计数无变化。

每例记录entry_kind、given_inputs、source_order、read_set、write_set、call_boundaries、counter_values、physical_membership、endpoint及unknowns。区分源码条件、GIVEN续段入口、分析证据不足停点；不得把后两者画成源码新增if/return。每个实际运行条件分别标reached/result/body_entered。只有明确共同前提成立才组合片段，不能只因两个独立输入数值相同就声称它们是同一执行轨迹。

源码定位由执行者自己复核：malloc.c的free回收尾段、upage_free、uslab_free；_malloc.h、utils.h和真实list.c助手。以下模型数值是合同输入，不是主线代替执行者形成的源码证据。

## A · 回收门、尾节点和调用前瞬态（6例）

统一在free的`normal_slab_free.count > 2`判断前进入。S在used链中，total=128、free=65；full链空。其余列入free链的每个slab都是total=free=128。free链顺序、base身份如下，各例独立；A0只有S，S就是base，其余例B是base。

令n为表中free链实长，初始free.count=n、used.count=1、full.count=0、normal_slab_total=n+1、nsobj_free_count=128n+65、nsobj_used_count=63。这些都是该入口的GIVEN，不重放前半段slot释放，也不证明真实初始化可以到达。

| ID | free链从head到tail | n | normal_base_slab_p |
|---|---|---:|---|
| A0 | [] | 0 | S |
| A1 | [B] | 1 | B |
| A2 | [B,R] | 2 | B |
| A3_BASE_HEAD | [B,R,T] | 3 | B |
| A3_BASE_TAIL | [R,T,B] | 3 | B |
| A4 | [B,R,R2,T] | 4 | B |

逐例求判断结果、是否读取anchor.prev、选中slab身份/是否base、进入回收体后的每次显式计数写及链实长。门假例止于该尾段条件性完成；门真例止于`uslab_free(tmp_uslp)`调用边界，返回及后续字段未知。

特别分别记录：调用前count已经减而节点仍在链中；读tail并没有把它移除；此处是if不是循环。base尾节点例只观察源码选择/传参及该回收门的检查集合，不能升级成“真实base已被释放”或具体故障。对base保护的注释、normal_base_slab_p字段、初始化赋值与本调用点实际条件分别取证，不能让注释代替保护分支。

用初始CF+CU与128(n+1)核给定数值；调用前片段没有自动改变total或对象计数。递归调用发生之后的副作用不从此处旧值外推。

## B · upage_free局部迁移（4例）

三个普通例从同一独立初态出发：used_upage=[P0,P1,P2]，free_upage=[Q0,Q1]，counts=3/2。另设单节点/空目的链例。每个节点恰出现一次，两链及anchor互不别名，列表合法。目标page原uslab_p=TargetSlab；其余owner字段给定且保持不同标签，vaddr是给定描述符字段值。

| ID | 初始used | 初始free | 目标 |
|---|---|---|---|
| B_HEAD | [P0,P1,P2] | [Q0,Q1] | P0 |
| B_MIDDLE | [P0,P1,P2] | [Q0,Q1] | P1 |
| B_TAIL | [P0,P1,P2] | [Q0,Q1] | P2 |
| B_SINGLE_EMPTY | [P0] | [] | P0 |

P0/P1/P2/Q0/Q1的vaddr依次给0x60000000/0x60200000/0x60400000/0x60600000/0x60800000；这里只是字段值，不是实际映射证明。

按真实助手顺序列出受影响的prev/next与两个anchor、两count、目标owner：旧链摘除并self-init、used.count--、插入free头及free.count++、owner清NULL。明确每个阶段字段变化与成员顺序；插入助手再次对self-loop节点调用list_del_init并不等于再从旧链移掉一个成员，也不另改header count。

核总描述符数守恒、目标仅在目的链出现一次及目标vaddr保持。区分page描述符owner清空和slab的upage_p反向字段：upage_free是否实际写后者须按写集合核，不凭名称假定双向解绑。全函数及直接列表助手的显式调用/写集合只支持本地迁移，不推出brk缩小、内核页归还、真实解除映射、payload清零或后续分配成功。

## C · uslab_free与caller的有界调用/续段（5记录）

五记录分别命名C0/C1/C2/C3/C4，入口与终点必须明确，不要求把它们拼成一条真实递归路径。

### C0 · 进入uslab_free，止于第一次递归free

以A3_BASE_HEAD的调用前片段为给定入口：目标T，当前cache=C，free链[B,R,T]实长3但count已为2。这个不一致是caller预减后的已知瞬态，不把count重新填成3。T.upage_p=P1、T.uslabcache_p=C、T.colormap=M（给定非NULL指针值），T.total/free=128；对象及字段有效。

page队列采用B_MIDDLE初态，P1.uslab_p=T。回源展开本层upage_free及其直接列表助手，记录page的迁移/清owner、T.upage_p/colormap/链字段、cache free.count等在该段的写与未写。然后读T.colormap并到达`free(M)`调用，停在调用边界；其返回、内部查找、回收及副作用UNKNOWN，不能假设这是纯函数。

### C1 · 第一次递归free已返回后的独立新入口

这是全新GIVEN，不由C0推出：给定free(colormap)已正常返回，T仍存活且后续字段访问/列表操作合法；T.uslabcache_p=C_m。T的uslab_list恰一次处于C_m.normal_slab_free的合法五节点环[B,R0,R1,R2,T]中，count=5；其余四节点与T、anchor互不别名。C_m可以与C0的C不同，不能回填成旧owner或拿旧count代用。

按后置字段继续：list_del_init(T)、通过当前T.uslabcache_p消费对应count并减一，再到达`free(T)`调用边界即停。记录unlink后而count写前的瞬态、两者完成后成员/实长/count、T的self-loop，以及当前cache身份。free(T)内部行为、返回、T之后的生存期及caller后续读取UNKNOWN。

### C2 · uslab_free已返回，但tmp对象可读性未给定

独立从caller中uslab_free返回后的新入口观察：外层cache=C仍有效，normal_slab_total=4、free.count=1、free物理链[B,R]，used=[S]、full=[]；S.total/free=128/65，B/R均128/128；CF=449、CU=63。tmp_uslp只保留指针值T，T所指对象的存活、类型/访问合法性和total值都UNKNOWN。

先核`uscgp->normal_slab_total;`这一表达式的本身读/写效果，不能按意图添上递减。下一条复合赋值需要读取tmp_uslp->total；在缺少对象前提时停于ANALYSIS_STOP_MISSING_OBJECT_ACCESS_PREMISE，后续CF值NOT_DERIVED。不要把分析停点画成实际源码分支，也不因调用返回就声称T访问合法或必然发生某个运行故障。复合赋值各子求值顺序不额外断言。

这些入口GIVEN本身含count与实长、总数与所列成员等不一致；只记录其已存在状态，不把它们归因于C2片段。

### C3 · 另加对象存活/访问前提的caller数值片段

除另明确给定下列条件外，采用C2同一组新入口数值：**tmp_uslp在本独立入口仍指向存活、类型/访问合法的uslab对象T，合法读取T.total得到128。** 不只是“内存地址可读”；此条件不由free(T)或uslab_free返回推出。

据此求表达式语句与随后CF减量的局部结果，分别列normal_slab_total、free.count/成员实长、CF/CU前后值及本片段写集合。C3只证明所给条件下局部CF变化；入口原有不一致继续区分，不宣称总体回收成功、全cache一致或实际对象存活。不得把C3的追加GIVEN用于填补C2。

### C4 · 两次count写与一次摘链的条件代数

先准确列出caller与callee两处count--的身份和源码顺序。给定同一cache=C贯穿两处写，callee在递归free(colormap)返回后读到的owner仍C；两store均已到达，相关对象/链访问合法，F0=3且取值不下溢。初始free成员[B,R,T]，T唯一。

**观察终点固定在callee第二次count--完成之后、free(T)调用之前。** 扣除caller和callee这两次--及callee的一次T摘链后，同一cache的free.count、该链成员/长度及剩余节点的相对顺序没有其他净效应；page队列是另一组对象。该“无其他净效应”是额外GIVEN，不能从递归函数名称或正常返回推出。

求该终点的字段count、物理成员顺序/实长以及两者差异。若允许中间额外净计数变化Δ，则先写清其含义及表达式；若owner变了或某store没到达，不能再把两处减量合成同一count的−2。不要跨过free(T)或自动接到caller读T.total；C4没有给最终返回或T后续存活性。

## 固定来源、正式锚与范围上限

按协议v2全部P1–P13取证。历史消费仅本轮147独审及§4、147正文/边界和deferred；146/145及更早只保留已通过/补注关闭状态，不重核旧72/90/55窗或旧完整模型。

固定time的正式源码需覆盖：

- malloc.c回收门至free尾部全段：count>2、tail/container_of、调用前count--、uslab_free调用、normal_slab_total表达式、tmp->total后的CF减量；if与真实调用/读写分开。
- upage_free全体及其真实助手：list_del_init、INIT_LIST_S、list_insert_next、list_header_add_to_head，链接变动与count时点分别取证。
- uslab_free全体：page处理、free(colormap)、unlink、后置owner/当前count读取与--、free(slp)；两递归调用的参数/次序、返回/副作用未知边界。
- _malloc.h中page/slab/cache字段、base保护注释，utils.h的List/List_hdr/container_of及相关类型；malloc.c静态base数组与init_uslab对normal_base_slab_p赋值仅定位，不展开初始化。
- 活动配置/canary及五份用户构建文本沿147核当前固定源，确认源码候选与实际链接未知；不据内核DEBUG开关推导用户库实际预处理。

每个正式窗1–5连续物理行；声明/定义/宏/配置/注释分别分类，不能以声明代助手体。“函数不调用brk/free”“某字段本层未写”等否定结论须由完整被限定函数/直接助手的读取及显式write/call集合支持，不凭窄窗缺字推全局不存在。

保留原CRLF/NONE与可逆围栏分隔LF说明；物理行不计末尾split空项，Blob短码从实际对象取得。新的正式窗仅算148，旧证据只作有固定来源的消费索引。没有引文数、行数或新问题数配额。

不扩展：完整初始化/分配/回收递归证明，任意坏指针或畸形对象全分类，147高位移位/外证重做，内核free对比，真实缩堆/页归还、对象生命周期实测、修复或运行。普通新条件观察按18进deferred；同一源码观察只用唯一qualified_id/source_ref/selector，不重报继承的链接/载荷/递归UNKNOWN。

## 六件、自检与主线验收

只在user-free-reclaim-148新增MANIFEST.md、free-reclaim.md、facts-and-boundaries.yaml、free-reclaim-paths.dot、deferred-findings.yaml、evidence.md。

facts至少有given_model、reclaim_entries、page_recycle_cases、slab_call_fragments、read_write_sets、counter_identity_and_delta、object_access_boundaries、source_index、required_anchor_coverage、graph及graph_coverage。A6/B4/C5共15记录；可参数化图，但每条case及停点可定位，不能靠复制同形长链增加体量。

正文、机器件、图逐项相合。DOT/facts.graph节点id/kind/label/scenario及边from/to/kind/label全部比对；另核source_call、opaque_call_boundary、given_resume_entry与analysis_stop的语义。C1/C2/C3为独立GIVEN入口，不用未证正常返回边把C0串成完整轨迹；C4是附带前提的代数关系，不冒作真实递归完整路径。B的实际直接助手可按合法给定展开。

MANIFEST标准根必须真实含read_channel、base_snapshot、inputs_read、startup_selfcheck_quote、branch_canary_quotes及self_check三个整数。交付前重开每个正式固定来源逐窗核对，n=m+k；强制锚不得降级或遗漏。P6只计148新正式源码前缀，明确root值/实际前缀一致。六件用途、覆盖、后继消费与实际规范复用/刷新记录齐全；读毕时点未知不追签。

实际可达P9逐项执行：1–8、11、13；9/10无代码/补丁时NA，12非降级时NA。完整本地P9仍NOT_RUN。148不新增外部语义取证要求，147 N1547缺口按原合同边界保留，不能误写已补齐。实际模型后台/effort、实际标准/链接/运行不作无据认证。

## 提交、唯一正式作者回执、实际G04及停写

原exec写前获取真实expected_sha，force=false、单父快进；净差异仅指定六新增，无其他改删。提交后逐件核固定/活动/API Blob全文、LF/字节、唯一父及净差异；发评前无since全分页到空查重。作者正式唯一标记：

MYOS2-LEAD-002-CHAT-EXEC-23 / CORE-USER-FREE-RECLAIM-148 / FORMAL_DELIVERY

正式作者评论包含固定合同/提交/六件、三专题及新入口/调用/对象前提边界、真实自检、模型可核信息、未证和发表后G04待做状态。完整数据留GitHub，不搬进聊天。

发表后真实ID双读；PR17无since全分页到空，31历史/主线标记各唯一、148作者唯一、旧EXEC-22的145作者0；开放PR完整到空仅16/17及原状态、head/base/PR17原题；四refs；六件固定/活动/API Blob与单父六新增；147冻结六件及原作者评论保持。全部实际做完后才报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、G04=PASS与STOP_WRITING，不自行启动149。

暂态读写故障按63先核远端效果、再有限间隔重试，通常首次加三次；不用重复提交/评论补救未知结果，不因一次故障判无权限。未知并行写不覆盖；无故障记not_needed。

最终聊天只给一个通常3–6行原生复制块：148真实交付/主线待审/实际G04/停写状态、一个正式作者URL，请C10 Pro独审并同轮后继。受阻才加必要缺口。阶段2及所有源码/旧件冻结、原分支/PR、模型分工和未来交接必传约定保持。
