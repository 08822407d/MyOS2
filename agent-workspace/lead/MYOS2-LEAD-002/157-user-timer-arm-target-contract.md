---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-TIMER-ARM-TARGET-111"
record_type: "bounded_timer_initialization_and_arm_targets_contract"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "c06abde41e91"
execution_pin_short12: "d7ebfb1d5e40"
status: "READY_FOR_OWNER_FORWARDING_EXECUTION_NOT_STARTED"
R108_01: "CLOSED_DOCUMENTATION_ONLY"
R108_02: "CLOSED_DOCUMENTATION_ONLY"
batch108_composite: "PASS_PENDING_LOCAL"
R109_01: "OPEN"
R109_02: "CLOSED_DOCUMENTATION_ONLY"
batch109: "RETURN"
R110_01: "OPEN"
R110_02: "OPEN"
batch110: "RETURN"
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/156-user-timer-base-forwarding-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMER-WHEEL-PLACEMENT-109-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMER-WHEEL-PLACEMENT-109-D00-108-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/timer-base-forwarding.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/timer-base-forwarding-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/evidence.md","mykernel/time/timer/timer.c","mykernel/time/timer/timer.h","mykernel/time/timer/timer_const.h","mykernel/time/timer/timer_macro.h","mykernel/time/timer/timer_types.h","mykernel/arch/x86_64/include/asm/percpu.h","mykernel/lib/list/hash_list.h","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-WAIT-WAKE-CONSISTENCY-52-review.md"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0,"scope":"本件主线独立源码短引文"}
executor_display_name: "MYOS2-A-C07b 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-16"
canonical_write_branch: "claude/dazzling-cori-q0dnyt"
canonical_pr: 17
allowed_write_prefix: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/"
allowed_files: ["MANIFEST.md","timer-arm-target.md","facts-and-boundaries.yaml","timer-arm-target-paths.dot","deferred-findings.yaml","evidence.md"]
activation_gate: "OWNER_FORWARD_ACTUAL_157_RELEASE_AND_FRESH_G00"
---

# 第111批任务书　定时器初始化与登记目标

**待 Owner 转发现 C07b，执行尚未启动。** 110从已有 timer 的 flags 和期限开始。111向前追踪 flags 的生成，比较四个实际登记入口，解释何时按当前 CPU 选择基准、何时使用显式 CPU，以及入口早退时已经写了哪些字段。D00定点勘误与下列 A/B/C 同包连续执行。阶段2，次一档 GPT-6＋Extra 执行，C07用Pro独立验收。

## 一　身份、材料和写区

1. 完整读取本157、同一发布提交的[110独立审核](reviews/CORE-USER-TIMER-BASE-FORWARDING-110-review.md)、[D00裁定](reviews/CORE-USER-TIMER-BASE-FORWARDING-110-D00-108-109-review.md)、[检查点](checkpoints/2026-11-19-timer-base110-reviewed-timer-arm111.md)及本轮C07正式评论。[110固定六件](https://github.com/08822407d/MyOS2/tree/d7ebfb1d5e40/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110)及[110作者评论](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7011963153)为冻结输入。旧表消费者与109、108证据按两份回执内的固定件定位。[旧156合同](https://github.com/08822407d/MyOS2/blob/c06abde41e91/agent-workspace/lead/MYOS2-LEAD-002/156-user-timer-base-forwarding-contract.md)和[第52批登记/取消边界](https://github.com/08822407d/MyOS2/blob/c06abde41e91/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-WAIT-WAKE-CONSISTENCY-52-review.md)用于避免重做已核内容。
2. 本任务适用 `agent-workspace/tasks/00-gpt-task-protocol-v2.md` 全部十三条。同一有效C07b仅可复用本人已完整读取且版本相同的原九份MyOS2约束和适用的Mnemosyne指导。完整重读[63号写入与回读](https://github.com/08822407d/MyOS2/blob/c06abde41e91/agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md)。执行与主线按[151接收记录](https://github.com/08822407d/MyOS2/blob/c06abde41e91/agent-workspace/lead/MYOS2-LEAD-002/151-executor-continuation/01-MYOS2-A-C07b-receiver-startup-prompt.md)、[28同轮](https://github.com/08822407d/MyOS2/blob/c06abde41e91/agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md)、[79合包](https://github.com/08822407d/MyOS2/blob/c06abde41e91/agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md)、[110沟通](https://github.com/08822407d/MyOS2/blob/c06abde41e91/agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md)和[124模型](https://github.com/08822407d/MyOS2/blob/c06abde41e91/agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md)分工。若需新对话接收，按151先完成独立加载。
3. 唯一执行者 MYOS2-A-C07b / MYOS2-LEAD-002-CHAT-EXEC-16。原 PR17 执行分支 `claude/dazzling-cori-q0dnyt`，唯一执行父 d7ebfb1d5e40。仅新增 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/` 的指定六件，以及本人一次PR17正式顶层评论。C07保有 lead 与PR导航唯一写权。固定 time=a039d9803ade、direct master=e8e60cd19540、lead为本157实际发布头。编译、本地完整P9、运行和旧件改写均不在本包。

## 二　fresh G00 与恢复

接收 Owner 转发的157固定链接和 C07 实际正式回执。独立核四条 direct refs、仅开放16/17，均 base master、未合并，16 Ready、17 原题 Draft。110固定与执行活动六件逐件全文/blob一致；111目标目录和六目标在执行父应缺席。PR17无since全分页至空；本批主线发布标记一次，作者正式标记零次。逐字读 direct master 公约§1第2条和 time/master 的 options_flags.cmake、panic.c 两对金丝雀。正文短SHA仅从连接器实际全值截取，ref租约用完整SHA。在同一批恢复时，先核已有对象和评论的身份，只补未完成部分；未知并行推进暂停相应写操作。

## 三　D00　合并修正 R109-01、R110-01、R110-02

**R109-01 与 R110-01共用一份定位表。** 完整读固定110的 facts 和 evidence。按“原记录ID、字段名、字段内序号”把每个错误位置配回同位证据ID，再用固定 time 的路径、符号、原始1–5行和首尾行回算。主线审核列有72个ID的正确字典。223次出现包括 D00的23、A/B/C/D/X五表的26/38/31/35/33、模型37，合71条记录。给每条受影响记录一份有序ID或“字段位置→字典键”的可展开映射，并核完整性；同一路径/起点296的E017与E051须分别映射到296–296及296–298。选取首次出现、末次出现和碰撞项展示实际定位。可把映射与字典放在同一份新 facts 中，避免重复抄223段源码。旧错误原文可用单独历史说明，引证字段内的每条新位置须为真实文件及整数窗口。记录旧 MANIFEST 的P9-4断言和本轮实际核对结果。

在同一映射中点名原109纠正的六行109A/B/E/H/I/M1，并给109M1–M7的几何依赖追加可复核说明，包括 TW109-E024、E027 与按需使用的 E023–E032、E035。已闭合的 R108-02、R109-02 保留历史状态。原始74条引文、各批模型与旧评论冻结。

**R110-02** 按110审核的三个条目逐消费者补正。图中 per_cpu_ptr / this_cpu_ptr 归为宏，候选从各选择函数返回。直接的迁移后推进调用从 __mod_timer 发出，赋值节点仅表先后；两个原始分支、迁移后第三处与 add_timer_on 的独立调用分开。三个公开包装是 options 的来源，初值证据与最终测试各有位置。正文按“先锁→过非空回调门→forward→锁后REDUCE判定”的实际顺序写。样例注明旧 flags 解码出的既存桶号、当前基准、相关写入的对象和返回门。允许复用 C07 的 RV110 与旧 E/P 号并链接真实固定文件。所有补正由 C07 独立关闭。

## 四　A　从初始化输入到 timer->flags 与 pending 状态

主读 timer_macro.h 的 timer_setup 和 timer_setup_on_stack，timer.c 的 simple_init_timer_key，timer_const.h 的 TIMER_INIT_FLAGS 及其三个独立成员宏，timer_types.h 的 timer_list，timer.h 的 timer_pending，hash_list.h 的 hlist_unhashed_lockless。按源次序串联 entry.pprev=NULL、function赋值、合法标志筛选、与 smp_processor_id() 的结果做按位或。区分两个宏的展开、flags 形参的变化和最终写回对象。列明 expires、entry.next 在这段初始化体内是否赋值，后继登记须由哪里取得一个有效期限。只消费已核 CONFIG_BUG 选支的条件语义，CPU值是给定样本，可沿用有效CPU号5；若需要其真实机器来源，登记缺口。

小模型用普通合法对象和 CPU5，分别给 flags=0、PINNED、PINNED|IRQSAFE。另用一个含未获 INIT_FLAGS 接纳位的纯标量，计算掩码前后与最终写回值。结合 timer_pending 两层表达式解释 pprev=NULL 对新初始化对象的局部含义。[初始化](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L713-L721) · [标志宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer_const.h#L183-L196) · [两个 setup 宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer_macro.h#L19-L22)

## 五　B　当前 CPU 的三个登记入口

主读 timer.c 的 add_timer、add_timer_local、add_timer_global。三者共用的 pending 检查必须放在各自第一次改 flags 和调用 __mod_timer 之前。在初始 PINNED 置/未置的普通对象上比较：保留原位、主动置位、主动清位分别如何影响符号候选；源调用使用哪个 expires、传哪个 options、是否消费 __mod_timer 的 int 返回。保留 caller 自行提供有效 expires 的前提。以同一初始化对象和期限比较三条路径，再取一个合法已登记对象，列早退经由哪一门、该入口内哪些写入尚未发生。联到110已核的回调门、基准选择与 NOTPENDING 重算，不重写整个轮盘或锁系统。[三个入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L441-L479) · [调用方所用基准](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L114-L146)

## 六　C　add_timer_on 的显式 CPU 与交回路径

读 timer.c 的 add_timer_on 完整体。依次区分 pending 检查、PINNED置位、按参数 cpu 得到 new_base、锁旧候选、function门、指针比较、必要的锁变更和 CPU 位写回，以及推进、internal_add_timer、公共解锁。比较“基准指针相等”和“不等”两条正常控制路径，同时处理合法但回调为空时的提前出口。区分源码表达式中的实参 cpu、current CPU、flags所含旧CPU 和基准对象的 cpu 字段。明确 timer->expires 是既有输入还是本函数赋值，旧flags已含的位在掩码后如何处理。

设模型中旧 CPU5、参数CPU7均有效，给定数值期限。先写明符号化的指针关系，再以有效数值确认 TIMER_BASEMASK 相关位的清除和 CPU7 写回；另用目标CPU5和相等基准作对照。基准对象的真实地址、实际CPU归属及运行锁状态只列前提。并列当前CPU三个包装和显式CPU入口的返回类型、已调用的最后一层与停止点。所读 add_timer_on 自己调用 forward_timer_base 和 internal_add_timer；__mod_timer 中迁移后写回的是 base->cpu。分别取证，不因写法相似而替换操作数。[显式CPU入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L495-L539) · [__mod_timer 对照片段](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L306-L315)

## 七　成果、预检及证据界限

唯一六件为 `MANIFEST.md`、`timer-arm-target.md`、`facts-and-boundaries.yaml`、`timer-arm-target-paths.dot`、`deferred-findings.yaml`、`evidence.md`。正文分 D00 和 A/B/C；事实表含可还原的旧错位映射与新路径模型，图标明实际宏、调用者、条件、时序和数据来源；候选与缺口各自保留旧根号及首个未证事实。强制短锚覆盖配置门、setup宏和初始化体、筛选/字段与pending定义、三个包装的共同门与差异、add_timer_on早退和同/异基准分支、实际CPU位和有效期限的消费。不同定义独立取证；签名、extern、include、预处理位置、注释与1–5行定义内引文分账。

本包建议源码不超18份、定点检索8组、目录定位4次、新短锚65、纸面模型12、候选及缺口各8。软上限不设最低数。优先D00、实际分支、字段和可复算样例；外围按18号延期。全程阶段2静态读取与纯文本/有限整数核查；完整本地P9、编译预处理、仓库脚本、Graphviz、内核、QEMU、实机、额外模型/代理及付费API均不在本包。

MANIFEST 要有 task_id、track_id、followup_id、produced_by、model_per_owner、date、base_snapshot、read_channel、inputs_read、status、open_questions、startup_selfcheck_quote、branch_canary_quotes、self_check、实际 write_retry_log。按 P9 编号：1 全六件40hex零；2 每段逐字且位于本符号定义中；3 原件、字段目标和图边可定位；4 **逐条走完最终固定 facts 的所有源引用**，每一条实际解析出整数起止，源窗与对应证据ID的原文全等；P类离散行分开定位。5 全包原始词头、三数、窗口与定位数分账；6 外部断言一手出处；7 双金丝雀；8 公约与元数据；9/10 纯文档；11 真实核查边界；12 只记录实际降级；13 D00及三专题全部强制项逐一核对。实际发布前也对最终六个文本做相同核查。若发现表内引用错误，应先在本轮未发布的六件中纠正。

## 八　发布与交回

刷新执行头、目标目录和作者 marker，以 d7ebfb1d5e40 对应完整执行提交为唯一父，沿原树一次严格六新增，expected_sha=真实父、force=false 正常前移。本人在原PR17只发一次正式顶层交付评论，其标记为

MYOS2-LEAD-002-CHAT-EXEC-16 / CORE-USER-TIMER-ARM-TARGET-111 / FORMAL_DELIVERY

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。执行实际10/20/40秒或服务端 Retry-After；如首次已写成就复用，评论响应不确定先查重。无故障记录 not_needed。

远端核唯一父、六新增、旧文件零改删、固定及活动六件全文/blob、四direct refs、两PR形态，及按ID和无since全分页的正式评论正文/标记唯一性。作者终态 READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、D00=CORRECTION_SUBMITTED_PENDING_C07_REVIEW。R109-01、R110-01/02 保持 OPEN 待C07核收。全部完成即 STOP_WRITING，交回「MYOS2-A-C07 内核分析主线续建」使用 Pro 独立验收并同轮安排后继。
