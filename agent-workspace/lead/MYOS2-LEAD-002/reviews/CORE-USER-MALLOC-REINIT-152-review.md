---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
conversation_display_name: "MYOS2-A-C11 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（本轮系统身份）"
model_per_owner: "Pro独立验收并同轮安排后继"
actual_backend_model: UNATTESTED
actual_reasoning_effort: UNATTESTED
date: 2026-11-26
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: 7ed4e9279b7d
execution_pin_short12: 6994f6b1aaec
read_channel: "GitHub connector；自制文本/整数/符号链核验"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
self_check_scope: "仅本文件新增正式源码引文；本轮对作者160窗的独立复核另账。"
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
mainline_design_count: {conversation: C11, before_release: 1, after_successful_153_release: 2, thresholds: [8, 10], reached: false}
previous_mainline_design_count: {conversation: C10, distinct_batches: 11}
evidence_class: "SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / OBSERVATION / UNKNOWN"
record_type: independent_mainline_review
reviewed_batch: CORE-USER-MALLOC-REINIT-152
reviewed_author: MYOS2-LEAD-002-CHAT-EXEC-25
mainline_acceptance: RETURN_DOCUMENTATION
technical_and_evidence_acceptance: PASS_PENDING_LOCAL
documentation_item: R152-01
documentation_scope: "新执行席接收记账；不影响已核技术模型和160源码窗"
next_batch_D00: [R152-01]
status: PREPARED_FOR_PUBLICATION_G04_PENDING
open_questions: ["152当时到达后胜任/容量判断及明确刷新完成状态未留完整可消费记录；实际后台/Extra与真实运行未认证。"]
---

# 第152批独立验收 · 技术及文证通过，接收记账转153补正

## 1. 裁定与可以消费的范围

**152的A/B/C技术模型、160正式源码窗及图通过独立核验，技术结论为PASS_PENDING_LOCAL；整批保留RETURN_DOCUMENTATION，仅余R152-01接收记账项。** 完整本地P9未执行，不升级为运行通过。按照[28同轮后继](../28-review-and-next-batch-same-turn.md)和[18问题先入库](../18-deferred-findings-and-resume.md)，本轮将该小项作为[214号153合同](../214-user-free-reinit-contract.md)的D00，随后连续做不受其影响的free消费分析，不要求Owner作技术决定。

本轮审查者为C11，未参与EXEC-25的152作者实现。主线对固定合同制定与本轮作者成果验收职责分列；先前合同就绪模型不充当本轮交付审查。本轮从固定源码和213 GIVEN另建预期，作者delta/状态只作比较目标；另一只读分工独立核文证和图，C11裁定和发布。

来源：[作者正式回执#7185898830](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7185898830)、[作者固定提交6994f6b1aaec](https://github.com/08822407d/MyOS2/commit/6994f6b1aaec)、[固定213合同](https://github.com/08822407d/MyOS2/blob/7ed4e9279b7d/agent-workspace/lead/MYOS2-LEAD-002/213-user-malloc-reinit-contract.md)、[212恢复包](https://github.com/08822407d/MyOS2/blob/7ed4e9279b7d/agent-workspace/lead/MYOS2-LEAD-002/212-executor-continuation/00-C10c-to-C11a-recovery-package.md)与[启动词](https://github.com/08822407d/MyOS2/blob/7ed4e9279b7d/agent-workspace/lead/MYOS2-LEAD-002/212-executor-continuation/01-MYOS2-A-C11a-receiver-startup-prompt.md)。六件结果基目录为[固定user-malloc-reinit-152](https://github.com/08822407d/MyOS2/tree/6994f6b1aaec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-malloc-reinit-152)。

作者回传实际G04=PASS、READY_FOR_MAINLINE_REVIEW、STOP_WRITING，未进入153。本轮独立核当前公开状态，不认证作者私有工具时序、实际Sol/Extra或后台型号；正式作者评论的PENDING_POST_PUBLICATION是发表当时的准备状态，不能据此误认其后回传自相矛盾。连接器上限中断由作者报告已恢复，本轮实际六件及回执可达，未见重复发布或残留缺件。

## 2. 固定成果、净差异与入场门

唯一父07d636217117；实际compare ahead1/behind0、1提交、6新增、零修改/删除，合计1868 LF、606992 UTF-8字节。固定提交、活动exec、实际API Git Blob逐件全文相等：

| 文件 | Blob短12 | LF | UTF-8字节 |
|---|---|---:|---:|
| MANIFEST.md | d31a7ec28677 | 59 | 14961 |
| malloc-reinit.md | b249c6618f9e | 93 | 10373 |
| facts-and-boundaries.yaml | b64fce61ad2b | 1 | 520078 |
| malloc-reinit-paths.dot | dd8ced991434 | 46 | 6661 |
| deferred-findings.yaml | 15274374ae9d | 106 | 4370 |
| evidence.md | fd6e80602995 | 1563 | 50549 |
| 合计 | | 1868 | 606992 |

当前exec完整树2632项、父树2625项，均未截断；原2235个非tree对象逐项SHA/type/mode相同，只添上述六件。旧151/150/149及原RETURN、原作者归属冻结。当前lead=7ed4e9279b7d，time=a039d9803ade，直接master=e8e60cd19540；不用PR.base_sha代替master。

原PR16 Open/Ready、PR17 Open/Draft，原head/base master且未合并；开放PR分页2/0。PR17无since全分页100/100/99/0，共299唯一ID；50枚精确标记逐条唯一且实际ID正确。152作者及上轮正式发令两次直接ID与分页全文一致；历史EXEC-23/149、EXEC-22/145、旧EXEC-24/152及EXEC-25/153作者均0。两time金丝雀在41/79行命中、直接master反验不命中；master公约原句命中。完整门禁表见[本轮检查点](../checkpoints/2026-11-26-reinit152-reviewed-free153.md)。

## 3. 本轮独立技术核验

固定time五个allocator根为malloc.c、_malloc.h、utils.h、list.c、bitmap.c；按213的活对象、无未说明别名、整数投影、逐brk frame建立独立状态。没有把作者给出的delta或旧设计模型直接当预期。自制符号模型共3549组比较、零失败：

| 实际核验 | REINIT_SPARSE | REINIT_FULL |
|---|---:|---:|
| 有序字段写（含同值写） | 1150 | 1146 |
| 其中init / malloc前缀 | 1131 / 19 | 1131 / 15 |
| 调用记录（展开实际助手） | 499 | 498 |
| 其中A调用 / brk / upage复用 | 488 / 17 / 16 | 488 / 17 / 16 |
| 完整循环记录 / 命名快照 | 16 / 8 | 16 / 8 |
| 已定义谓词时点 / header比较 | 6 / 306 | 6 / 306 |
| scan源码求值 / 独立search探针 | 2 / 3 | 128 / 3 |
| A唯一读字段 / 写字段 | 200 / 459 | 200 / 459 |
| C唯一读字段 / 写字段 | 33 / 12 | 30 / 8 |

每项有序写的sequence、phase、iteration、source、field、before/after与全部调用的参数、结果、性质核同；完整入口亦由独立生成器建立后核同。四组实际读/写/未写字段集合、32轮记录、16快照及12组完整谓词一致。两个阶段含链式赋值的内层值流→外层值流顺序已声明，不认证真实机器存储时序。

- **A根重建时点。** 三全局header先重置，Qd已从全局页根不可达，而C9内部旧根仍在；i=9的185重新登记C9与187–189子根重置分开。D到full头189重置后才脱离该子根。静态节点先INIT后插入，未沿旧残余指针摘旧邻居；D/Qd所有字段未写，邻居重写后其残余链不保证互逆。16次upage复用均取回刚放free的同一P_i；124/125动态分支未达。对应E152-035–038、043–052、089–101。
- **B不同观察集合。** REINIT_RETURN实际根集合为{S9}，ROOT求和128/0与字段相合；持续活owner给定集合仍{S9,D}，求和128/128，CU少128。normal_slab_total=1对根相合、对owner集合不合。S9位图不合、D位图自身仍合；脱根不是释放、泄漏或UAF。17个brk逐调用frame保护bitmap载荷和base_colormaps绑定，实际init/助手没有载荷写，不以简单文本未命中替代证明。
- **六个search观察。** 每例旧D与旧P9地址均16比较/16推进后NULL；新地址为P15…P9，7比较/6推进后P9，成功后不读P9.next。search不读owner；P9.owner是快照字段。观察不插入执行控制序列，见E152-003–006、039–042。
- **C边界。** 两例10比较/9推进选C9；free S9迁used后尚free128，CLASS假。SPARSE取idx1、map(3,0)、CF/CU127/1、局部ret_val=0x61404000；FULL取idx128、296假、298–304未执行，局部ret_val仍NULL。310使free.count=1而实际根空，唯一LENGTH失败为C9.normal_slab_free；均止于311 uslab_alloc(S9)调用边界，不消费callee、new_uslp或327。见E152-056–071、102–105。

152两项新增观察保留为条件性静态观察；146无清零、149临时count和151 scan-limit未被重复申报独立新缺陷。153可以明确引用已核REINIT_RETURN（prefix1131）和SPARSE OBJECT_UPDATE_DONE（prefix1149）构造独立GIVEN，不能误接成152的malloc已经正常返回或311已经执行。

## 4. 文证、构建边界和图

160/160正式窗均按真实源Blob逐字节核同，1–5连续物理行，source_index、evidence标签、实际定义/命令实例、行尾与展示声明一致。419条引用物理行分为221 CRLF、193 LF、5 NONE；NONE为E152-105、135、138、157、160，额外展示LF均明确隔离，不写回原源字节。

独立识别109个实际定义/命令实例、77种path::symbol；五CMake共51个有效命令实例、53正式窗覆盖，无相邻命令错归。myinitramfs根未直接列mylib，文本候选不证明真实sh链接或ABI/标准/DEBUG/编译器配置。213强制源码锚全覆盖；三处P6均160/160/0，160标签仅在evidence；六件无连续40位十六进制串或内部检索句柄。十二源根和六件均另算Git Blob并与连接器身份相同。文证自制核验2221项、零失败，计数与技术3549分账，不相加宣称相同类型测试。

图22节点20边全部属性与facts.graph相同，2个11节点分量；12 source_order、2 controlled_next_top_level_call、6 observation_only。无悬空、重复边、孤立节点；六观察叶及两个311停点无后继。deferred两观察及三组继承项的source_ref、场景和证据selector可定位。

本文件未增新正式源码窄窗，P6=0/0/0；本节160窗是对作者证据的独立复核，不合并旧151的138或其他历史证据账。

## 5. R152-01 · 接收记账的有界退回

**性质：报告记账不完整；不是技术结论错误，也不据此推定作者越权、实际未读规范或未作任何能力判断。** 依据固定212恢复包§4及启动词第4条，MANIFEST应登记包ID/非执行源性质、到达后胜任与容量判断，以及实际读完后的明确刷新完成状态。

实件MANIFEST已分开execution_handoff_receive和guidance_refresh，列入口7185473439、固定合同7ed4e9279b7d、身份/日期/时刻UNKNOWN、152-only、missing=[]及输入版本；但未明确记录上述四项。task_local_only可由Mnemosyne_scope的任务局部/不引入外仓写权等效，current_task_preserved可由task/followup/phase、152-only及禁止越界的正文等效；不因缺同名字面字段另立退回。

**状态：R152-01=OPEN_DOCUMENTATION，整批RETURN_DOCUMENTATION；技术及文证允许按§3–4的已核范围消费。** 修正在153规定的六件内完成：

1. MANIFEST追加152接收记账补注，固定来源为212包/启动词@7ed4e9279b7d及正式入口7185473439；明确包ID MYOS2-A-C10c-TO-C11a-152-20261126、non_execution_source_transfer_artifact性质，实际执行授权来自Owner、本项目工作令18与213，任务与原分支/PR及152原六件写区保持。
2. 旧到达后胜任/容量判断及历史刷新完成记录，只能按原C11a本人可恢复的真实记录补述；不能恢复的标NOT_RECORDED/UNATTESTED。不由C11或新检查替作者倒签旧true，不能从六件已交付推定当时完整预检已留证。
3. 153开工前另做当次胜任、容量、材料/权限/接收状态判断；需读或刷新的内容实际完成后，明确当次refresh_completed及缺项/下一安全步。历史未知与本次结果分列。
4. 在153 deferred登记R152-01补注位置、真实材料和SUBMITTED_FOR_MAINLINE_REVIEW状态，交C11下一轮判闭。原152六件、原作者回执与原P6不动，不加第七文件、不重做技术、不另发152作者评论。没有真实技术依赖阻断，D00后连续完成153。

## 6. 规范、未知与后继

本C11同会话在210已独立读完整项目规范、MA采用/全文及同版十五条，按79复用自身未变已读；本轮重读213/212、00、18/24/28/63/79/124/198与当前断点。MA master仍b234c6c91a2e，入口881fd8c1076e；建设新头5cac46110877、检查点bb2a6df4d9b3，已核新增§321仅收存Alaya七项正式意见、等待另一方及合议，当前有效v1.4-r10.266未变。Mne master仍e581eb9df6c4；命名/接收格式仅任务局部，未扩大跨仓权限。

新包由原MYOS2-A-C11a／EXEC-25以GPT-6.2 Sol＋Extra／高推理执行，接收与执行无需Pro；C11正式验收仍Pro。153为D00接收记账＋A旧地址失配＋B新地址前门＋C独立快照的两次free序列。C11成功首次发布153后设计计数1→2，8/10未达；C10历史11保持。

完整本地P9、预处理、编译链接、项目程序/脚本、Graphviz、QEMU、实机NOT_RUN。真实C对象/指针、映射/生命周期、ABI/标准/DEBUG/编译器/链接、生产可达、311 callee、malloc外返、N1547外部标准独立外证及实际后台/effort仍未知。本文G04在发布前为PENDING；正式评论、导航与实际读回完成后才向Owner报本轮G04及STOP_WRITING。
