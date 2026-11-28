---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
conversation_display_name: MYOS2-A-C11 内核分析主线续建
produced_by: GPT-6 Astra Pro（本轮系统身份）
model_per_owner: Pro独立验收并同轮安排后继
actual_backend_model: UNATTESTED
actual_reasoning_effort: UNATTESTED
date: 2026-11-26
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: 55488278ab63
execution_pin_short12: c9f4ba8d3226
read_channel: GitHub connector；自制文本/整数/符号链核验
startup_selfcheck_quote: 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。
branch_canary_quotes:
- "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
- "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
self_check_scope: 仅本文件新增正式源码引文；本轮对作者114窗的独立复核另账。
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
mainline_design_count:
  conversation: C11
  before_release: 2
  after_successful_154_release: 3
  thresholds:
  - 8
  - 10
  reached: false
previous_mainline_design_count:
  conversation: C10
  distinct_batches: 11
evidence_class: SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / OBSERVATION / UNKNOWN
record_type: independent_mainline_review
reviewed_batch: CORE-USER-FREE-REINIT-153
reviewed_author: MYOS2-LEAD-002-CHAT-EXEC-25
mainline_acceptance: PASS_PENDING_LOCAL
R152_01: CLOSED_BY_153_RECEIPT_ADDENDUM
batch152_current_acceptance: PASS_PENDING_LOCAL_WHEN_READ_WITH_THIS_ADDENDUM
batch152_original_record: RETURN_DOCUMENTATION_PRESERVED
next_batch_D00: []
status: PREPARED_FOR_PUBLICATION_G04_PENDING
open_questions:
- 152历史胜任/容量/刷新完成的缺失记录不倒签；完整本地P9、真实运行及实际模型/effort未认证。
---

# 第153批独立验收 · 通过静态核收，判闭R152-01

## 1. 裁定与消费方式

**153整体裁定PASS_PENDING_LOCAL；R152-01=CLOSED_BY_153_RECEIPT_ADDENDUM。** 完整本地P9未执行，结论限于固定源码、明确GIVEN、整数/符号模型与当前公开交付。152原独审的RETURN_DOCUMENTATION和原件保持历史原貌；将[152原独审](CORE-USER-MALLOC-REINIT-152-review.md)与本文件§5合读后，152唯一记账退回项已经闭合，当前整体消费状态为PASS_PENDING_LOCAL。没有补签152当时未记录的胜任/容量判断或刷新完成状态。

固定来源：[153作者回执#7191089498](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7191089498)、[唯一提交c9f4ba8d3226](https://github.com/08822407d/MyOS2/commit/c9f4ba8d3226)、[214合同@55488278ab63](https://github.com/08822407d/MyOS2/blob/55488278ab63/agent-workspace/lead/MYOS2-LEAD-002/214-user-free-reinit-contract.md)及[固定六件](https://github.com/08822407d/MyOS2/tree/c9f4ba8d3226/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-free-reinit-153)。本次C11主线没有参与EXEC-25的153作者实现；技术分工从实际源码/214 GIVEN独建模型，文证分工另核原字节/定义实例/图，主线复查范围、R152-01并统一裁定。先前153合同就绪检查没有代替本次验收。

作者回传实际G04=PASS、READY_FOR_MAINLINE_REVIEW、STOP_WRITING，未进入154；本轮当前公开状态与该回执相符。作者正式文件/评论内PENDING_POST_PUBLICATION保留其发表前时点，不能用后来回传倒改原记录。终检调用上限由作者报告恢复；本轮六件和回执实际可读，未见缺件、重复作者发布或超范围写入。当前检查不认证作者私有工具时序、真实剩余上下文、实际模型或Extra档位。

## 2. 六件、净差异和入场核验

唯一父6994f6b1aaec；实际compare为ahead1/behind0、一个提交、六新增、零其他修改/删除，合计19971 LF、648723 UTF-8字节。逐件固定提交、活动exec、实际API Git Blob全文一致：

| 文件 | Blob短12 | LF | UTF-8字节 |
|---|---|---:|---:|
| MANIFEST.md | cd1008d002ce | 54 | 28919 |
| free-reinit.md | ce581a3b09ba | 102 | 10980 |
| facts-and-boundaries.yaml | f4c2876fcd81 | 18395 | 546162 |
| free-reinit-paths.dot | 198fdc79dbf1 | 70 | 8571 |
| deferred-findings.yaml | 1cddd19114a5 | 131 | 5989 |
| evidence.md | f8c5a8a9959d | 1219 | 48102 |
| 合计 | | 19971 | 648723 |

exec完整树2639项、父树2632项，均未截断；原2241个非tree对象SHA/type/mode逐项不变。152/151/150/149原件、作者归属、旧RETURN和既有主线补注冻结。lead=55488278ab63，time=a039d9803ade，直接master=e8e60cd19540；未以PR.base_sha替代直接master。

原PR16 Open/Ready、PR17 Open/Draft，原head/base master且未合并；开放PR分页2/0。PR17无since全分页100/100/100/1/0，共301唯一ID；53枚精确标记逐条出现一次且实际ID正确。153作者与前次正式发令7186791511直接ID双读并与分页全文相同；154目录及新旧执行身份的154作者标记为0。time两canary在实际41/79行命中，直接master反验均不命中，master公约原句核同。G00为2323项比较、0失败；与技术、文证检查分别记账，详见[本轮检查点](../checkpoints/2026-11-26-free153-reviewed-brk154.md)。

## 3. 独立技术复算

从固定time的malloc.c、_malloc.h、utils.h、list.c、bitmap.c及214 GIVEN建立独立状态和执行模型，不导入作者生成器，也不复用上轮合同预检生成器作预期。冻结152数据只用于明确授权的快照导入：shared_entry＋variant.entry_override＋有序delta；每条先核sequence和before，再应用after。另按合同对象、地址、anchor环、位图与D/Qd字段独建全状态后比较。

| 核验对象 | 实际覆盖 |
|---|---|
| 三个进口 | R_S/R_F各prefix1131，U_S prefix1149；各506个受跟踪非局部字段 |
| 旧前缀消费 | 3411条delta，sequence与before共6822项检查；未重做旧160窗 |
| 本批轨迹 | 5独立入口、6次顶层free、28阶段；3进口＋28阶段共31次谓词观察 |
| 根头观察 | 每次51个header按anchor实际遍历；1581次count/实长比较，核链互逆 |
| 实际搜索 | 60次vaddr比较、56次next推进；成功匹配不读P9.next |
| 实读和写入 | 150条非局部读日志；6条DEBUG条件读另列；恰4条有序非局部写 |
| 独立模型检查 | 8761个分组或标量比较、0失败；全对象比较包含locals、读值/顺序、未写集合、谓词和图 |

检查数量按实际比较口径报告，不把字典比较、字段数和header观察再次相加。首轮独立模型有一处重复呈现的搜索退出说明标签与作者用词同义，核源码157 anchor条件和168 NULL返回后只规范显示标签，未修改作者数据或放宽结构值比较。

| 调用 | search比较/推进 | 阶段 | 元数据读日志 | 写数 | free终点 |
|---|---:|---:|---:|---:|---|
| A_OLD_DYNAMIC | 16/16 | 3 | 33 | 0 | 342访问前缺page对象前提，ANALYSIS_STOP |
| A_OLD_STATIC | 16/16 | 3 | 33 | 0 | 342访问前缺page对象前提，ANALYSIS_STOP |
| B_NEW_SPARSE | 7/6 | 5 | 18 | 0 | 345真，348 SOURCE_LOOP |
| B_NEW_FULL | 7/6 | 5 | 18 | 0 | 345真，348 SOURCE_LOOP |
| C_CLONED_UPDATE_C1 | 7/6 | 7 | 30 | 4 | 391条件性VOID_FALLTHROUGH |
| C_CLONED_UPDATE_C2 | 7/6 | 5 | 18 | 0 | 345真，348 SOURCE_LOOP |

**A的对象边界正确。** 两个旧地址分别搜索0x54000000/0x51400000，遍历P15…P0后返回NULL；search不读owner或header.count，Qd不参与根搜索。free下一句342的成员读取缺少page对象前提，分析停在读取前，未伪造源码NULL拒绝分支、正常外返或真实崩溃。uslp/uscgp、idx/assigned及后继保持NOT_DERIVED。零写只指截至该边界，不能外推继续运行无副作用。定位作者E153-001–011、015–019。

**B先被空slab门阻断。** 两例free(0x61400000)均搜到P9并读到S9/C9，345的128==128为真，348进入源码循环。SPARSE/FULL残留位图只是状态观察；本次未到351/352，不求槽商、不读assigned或clear。成功search只推进6次，不读P9.next。定位E153-018–022。

**C没有偷接旧malloc控制栈。** U_S明确为prefix1149，不带310的临时count=1、旧ret_val、原malloc栈或311后继。输入0x61404000为独立GIVEN；C1中idx1/余0，assigned以int低位mask2从word3返回unsigned数值2。恰四写：bitmap word0的3→1、S9.free的127→128、CF的127→128、CU的1→0。364、370及381皆假，无链表、回收、brk或新opaque helper调用。C1在模型内到void尾后，C2直接消费同一副本；其128==128再次在345/348停止，未清残留bit0。定位E153-012–029、057–059；没有证成原malloc正常外返或真实重入。

全部31次观察CACHE_ROOT为真、CACHE_OWNER_GIVEN为假；owner给定域{S9,D}的used和比根域多128，D实际根登记次数0。S9的free−空位在R_S/R_F/U_S分别为1/128/1，C1后仍1；D自身位图相合，ROOT/OWNER位图合取为假。slab_total=1对根数1相合，对owner数2不合。三个入口CLASS真，C1四写后S9仍在used根而free128，CLASS转假并持续至C2；所有51头长度仍相合。其他15cache、所有页和D/Qd字段未写，D/Qd残余prev/next不被当作重新入根。

facts采用固定152 ref/selector/prefix＋本批四条delta重建，28阶段没有重复铺16cache全值；较大的重复部分是未写字段名称、读集合和谓词。未因文件行数提出新退回。FR153-O01/O02接纳为条件性组合观察，承继147/151/152既有问题，不再登记旧NULL门、分类差异或无清零为新源码缺陷。

## 4. 文证、构建与图

114/114正式窄窗逐字节核同，281条引用物理行=96 CRLF＋180 LF＋5 NONE；NONE为E153-029/089/092/111/114，展示额外LF已明确隔离且可逆。每窗1–5连续物理行，source_index、真实定义/命令实例范围及相邻断言均核。独立识别82个实际定义/命令实例、50种path::symbol；五CMake51个有效命令实例由53窗覆盖，没有把相邻命令归到同名错误实例。十二源根及六件另算Git Blob身份与连接器一致。

E153-012–014只合引定位旧malloc局部与调用边界，未声称153实际执行旧malloc。候选构建文本仅显示myinitramfs根直接myinit/myshell，mylib→myglibc/csu/util，myglibc静态目标及malloc/util的GLOB/target_sources关系；真实sh链接、ABI/标准/DEBUG/编译器保持未知。

独立文证/结构/补注核验1674项、0失败，P6三处均114/114/0；标签仅在evidence，六件无连续40位十六进制串或内部检索句柄。图31节点/28边全属性与facts一致，22 source_order、5 snapshot_import、1 controlled_next_top_level_call。五条导入为数据关系，唯一受控顶层后继C1→C2；两分析停止及三源码循环叶无执行后继，未画出旧malloc到新free的控制边。deferred两项新观察及继承项均可定位。

本文件没有新增正式源码窄窗，P6=0/0/0；114是本轮对作者证据的独立复核，152的160和其他旧批证据账保持分列。

## 5. R152-01的精确判闭

依据[152原独审§5](CORE-USER-MALLOC-REINIT-152-review.md)、214 D00和固定[212恢复包](https://github.com/08822407d/MyOS2/blob/7ed4e9279b7d/agent-workspace/lead/MYOS2-LEAD-002/212-executor-continuation/00-C10c-to-C11a-recovery-package.md)/[启动词](https://github.com/08822407d/MyOS2/blob/7ed4e9279b7d/agent-workspace/lead/MYOS2-LEAD-002/212-executor-continuation/01-MYOS2-A-C11a-receiver-startup-prompt.md)，逐项消费153 MANIFEST的R152_01_receipt_addendum、execution_handoff_receive、guidance_refresh以及deferred.D00.R152-01：

| 原要求 | 本轮实际材料与判定 |
|---|---|
| 包ID、性质、来源、授权及原任务边界 | MYOS2-A-C10c-TO-C11a-152-20261126、non_execution_source_transfer_artifact，固定7ed4e9279b7d两源及入口7185473439相合；权限来自Owner/工作令18/213，原C11a/EXEC-25、阶段2、exec/PR17与152六件写区明确。212实际Blob短12 ac3593b4ed4d、37f62882191b核同。 |
| 历史缺失不可倒签 | arrival_task_adequacy、arrival_capacity_judgment、refresh_completed_at_152分别保留NOT_RECORDED；实际模型/effort与历史容量UNATTESTED，旧C10c发表后G04 UNKNOWN；旧公开加载记录保留，未由交付结果推导旧true。 |
| 153当次真实判断和刷新记账 | 当前胜任ADEQUATE_FOR_BOUNDED_NEXT_TIER_EXECUTION，容量ADEQUATE_TO_PROCEED_QUALITATIVE并保留实时容量未证；当前材料、权限、缺项[]、下一步、task_local_only/current_task_preserved明确；refresh_completed=true明确只属于153，列本人未变已读复用及新读版本。 |
| 补注定位并留给主线判闭 | 153 deferred准确指向补注、旧材料与本次记录，作者仍SUBMITTED_FOR_MAINLINE_REVIEW/PENDING_MAINLINE_CLOSURE，未自行宣布关闭。 |

固定152原MANIFEST和facts已重新取得并与原Blob全文核同；本轮exec净差异亦证明整个旧结果树未改。主线判闭的是上轮要求的有界记账补正，不是认证历史缺失判断已经发生，也不保证作者真实后台容量。153作者时点MA建设5cac46110877/§321的记录按当时公开事实消费；本C11后来读取的新§322不倒推成作者已读。

**最终处置：R152-01闭合；152与本节合读后的当前整体状态PASS_PENDING_LOCAL；原152 RETURN_DOCUMENTATION、六件、160/160/0与作者7185898830全部冻结。** 不新增第二个152作者回执、不要求复做旧技术、不留下该项给154。R151-01及149/150既有补注和历史处置保持。

## 6. 规范、后继与实际限制

同一C11会话此前已独立完整读项目规范、MA采用/全文及同版共用十五条，按79复用本人未变已读；本轮实核00/18/24/28/63/79/124/198/210等固定Blob未变，重开当前合同、前次独审/检查点及212/152补注来源。MA master b234c6c91a2e/入口881fd8c1076e未变；建设现06b783861368、检查点54ac6e5fc66c，新增§322讨论Alaya反馈原始背景与收件流程，属于待合议分析，不是正式新采用。有效v1.4-r10.266及共用1.1-r9.260十五条不变。Mne master e581eb9df6c4，命名/接收仅任务局部。

同轮发布[215号154合同](../215-user-brk-range-accounting-contract.md)：三相关专题为brk边界请求到固定地址门的传值、增长尾部页数账、配对缩小/失败恢复时字段和账本消费；旧14/21/26/144根因承继排重。原C11a/EXEC-25以GPT-6.2 Sol＋Extra／高推理继续，D00=[]，接收/执行无需Pro，正式验收回C11 Pro；154终检后停写，不进155。154合同由设计分工提出，另一未参与候选构造的分工先从固定源码独建两A前缀、三B后返回帧及两C独立数据克隆模型，再与候选输出比较；97项针对核验、0失败。已核64位对齐/错误码投影、明确helper保持、四旧节点与新N身份、插入/摘除两个计数的不同写序及caller恢复；七例均在其GIVEN边界内成立。此项只证明合同设计就绪，不预签154未来交付、真实可达、运行或实际G04。

C11此前成功首次设计152/153共2批，本次154成功首次正式发布后计3，8/10均未达；C10历史11保持。实际模型/客户端/推理档位未独立认证，不操作对话UI。完整本地P9、预处理、编译链接、项目程序/脚本、Graphviz、QEMU、实机NOT_RUN；真实C对象/指针、映射/生命周期、生产可达、原malloc外返/311 callee、N1547独立外证继续UNKNOWN。发布前G04为PENDING；正式评论、导航和实际终检完成后，才向Owner报告本轮实际G04与STOP_WRITING。
