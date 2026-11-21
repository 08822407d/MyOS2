---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: bounded_scheduler_host_witness_taskbook
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-10-02
base_snapshot: "kernel=time；workspace=master；instructions=agent/MYOS2-LEAD-002（分支名）"
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-SCHED-ORDER-02
supersedes: null
continues_from: agent-workspace/lead/MYOS2-LEAD-002/14-scheduler-ordering-followup.md
required_review_record: CORE-CHECK-01-RECHECK-02-REVIEW-001
review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-recheck-02-review.md
reviewed_execution_short12: 0d62c4d19711
consumer_results_frozen_short12: 77faf51e9a43
kernel_short12: a039d9803ade
execution_branch: claude/dazzling-cori-q0dnyt
initial_execution_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-order-02/
existing_files_frozen: "0d62c4d19711 中所有已提交文件；新实验不得改 pilot/core 或 consumer-fix-02 原件。"
status: READY_FOR_OWNER_LAUNCH
external_execution_started: false
execution_disposition: RUN_WHEN_OWNER_FORWARDS
kernel_fixture_execution_authorized: "仅 Owner 转发后，本任务的原函数宿主夹具；不是内核运行。"
kernel_change_authorized: false
phase3_authorized: false
acceptance_ceiling: PASS_PENDING_LOCAL
read_channel: connector
evidence_class: "已有源码推导的有界验证规格；新见证尚未执行"
inputs_read:
  - "14-scheduler-ordering-followup.md：本会话前轮全文；其源码本轮再次打开。"
  - "time:mykernel/scripts/options_flags.cmake、mykernel/sched/scheduler/myos_rt.c 全文；连接器比较 time 与 a039d9803ade identical。"
  - "0d62c4d19711:core/fixtures/fx_sched.c 开头至pick辅助函数的计时赋值与输入打印；不申报本轮重读全部旧C夹具。"
  - "本轮 RECHECK-02 审查回执所列读取层程序与记录。"
local_validation: "待原Claude云会话；主线未编译、未运行本文见证。"
self_check: {scope: this_file_only, verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
open_questions:
  - "按更新后vruntime排序是本实验明确给出的候选策略，不冒称Owner已选择。"
  - "所有新见证的实际输出待执行；全局可达性、真实IRQ/SMP/上下文切换仍不在范围内。"
---

# 下一步：核清调度回插与时间记账，不再修通用验证器

**本次只回答：当前任务放回运行队列时，比较对象与遍历游标是否一致；本轮已运行时间在什么时候记入排序键；由此是否破坏明确声明的候选顺序。** 保留现有MyOS2调度实现，不抄Linux整套调度器，不修改内核，不为此扩展通用测试平台。

RECHECK-02已经在主线限定范围内收口。这里是新的一组内核机制见证，不是其第三轮返工；原N/M/V结果不重算，不重做H00或全树扫描。Owner在原云会话转发后，一次完成W01-W08及本文件要求的轻量结果核对，单项受阻仍交其余独立结果。

## 1. Owner一次启动，AI自行处理PR状态

在创建PR17的原Claude Code Cloud会话沿用现有模型、effort和费用配置，发送：

```text
执行 MYOS2-LEAD-002-CORE-CHECK-01 的 CORE-SCHED-ORDER-02。
先完整读取：
https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/15-scheduler-ordering-witness-contract.md
再读取其中指定的RECHECK-02收口回执和14号分析。
仍在原云会话、claude/dazzling-cori-q0dnyt 分支完成一次有界W01-W08原函数宿主实验；不再返工通用验证器，不改内核。
仅在任务书的scheduler-order-02/新目录新增，0d62c4d19711中的全部旧文件保持不变。
不运行原仓库脚本、完整内核构建或QEMU，不安装工具、不提权、不输出凭据。
原PR17尚开放就转回Draft并复用；已经合并则先核旧结果已入master，再为新实验创建唯一Draft PR，不合并、不删分支。
结果、完整夹具和两次实际运行输出直接通过GitHub回传，不让我搬运文件。
全部完成后只返回实际PR链接、提交短标识、重要发现或阻断。
```

合并不是启动前置，也不需要Owner下载任务包。若原云会话/分支不可恢复、出现不明并行修改或PR已关闭但未合并，保留工作并报告具体阻断，不另开竞争分支、不索取token、不自动换付费产品。

## 2. 输入和权限

读取顺序：本文件 → 同目录 `reviews/CORE-CHECK-01-recheck-02-review.md` → `14-scheduler-ordering-followup.md`。确认回执record_id、packet、PR17、执行分支、被审0d62c4d19711、冻结77faf51e9a43，以及acceptance_verdict为PASS_PENDING_LOCAL且该范围已收口。

开工时固定实际取得的主线对象，用分支加实际复制的短号记录；新回执是预期追加，不要求主线停在b0aa54db1b70。对新写区、旧对象保护和远端状态先核对再编译运行。

内核输入固定time的a039d9803ade。只读 `mykernel/scripts/options_flags.cmake`、`mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos`，以及原夹具实际引用的list操作、类型和宏定义。冻结执行输入为0d62的 `core/fixtures/fx_sched.c`、`fx_common.h`、`fx_list.inc.c` 和需要的定位/展开模块；可复用 `core/recheck-01/fixtures/harness2.py` 的有界进程执行原语。读取新夹具所需原定义不等于启动全树扫描，不使用旧评审的否定性结论当输入。

仅允许在隔离的新临时目录编译运行本任务夹具，使用既有Python/PyYAML、GCC、Git等工具；不安装、不提权。旧会话环境改变或必要工具缺失时，如实说明保真/安全边界，不能假称沿用旧执行环境。不跑整套run_all/run_recheck/meta_tests，不重新核V00-V14、M01-M12、N01-N06，不重做H00探针；已有受限进程超时助手可以复用，但其旧任务身份门不能冒充新任务准入。

## 3. 原函数必须保留；允许改变的只是新夹具

从固定time对象逐字抽取原 `pick_next_task_myos` 及所需list原语；把来源路径、符号、字节数和分段摘要写入抽取清单。原函数体不改，包括比较表达式、回插、计费顺序和返回行为。需要缩减task/runqueue结构时沿用原夹具的类型选择并再次对照实际访问字段，列清缩减与替身；不得重新手写一个“等价算法”代替被测函数。

**不要沿用旧pick辅助函数中无条件 `rq->myos.last_jiffies = jiffies` 的做法。** 本任务需要正时间差。仅在新夹具中显式设置每个场景的初值，旧fx_sched.c不改；连续调用时由原函数维护last_jiffies，不由辅助函数重置。

current、need_resched、jiffies均为显式夹具输入；BUG_ON可以记录并受控终止。它们不是实际时钟、中断、抢占或上下文切换。第二次调用把返回任务作为新current，是明示的序列模型，不宣称执行了真实上下文切换。

观察只能加在新夹具的调用前后。不要为了打印中间值在原函数内插语句；内部控制路径仍属源码解释，输出本身只证明实际记录的边界状态。遍历快照必须先比较链表anchor，再转换节点容器；不得由日志代码额外制造非法读取。

输入节点均真实分配且互不重复，count与链长相符；current不同时在队列中。idle始终存在：current不是idle时，idle在队列中；current是idle时，不重复把idle入队。不得故意删idle或使用空anchor冒充任务，以此论证全局故障。

## 4. 八组有限场景

下面是明确的实验输入和**主线推导**，不是已经观察的结果。任何与推导不同的输出必须保留，不能改函数或输入迎合。统一：单CPU、无并发修改、time_slice=100（覆盖旧mk辅助函数的5）、last_jiffies初值100；强制切换的场景need_resched=true。所有数值很小，不涉及溢出。

普通基准队列为 B(vruntime=10)、C(20)、D(30)、I(idle,0)，依次链接；A为current。I为TASK_RUNNING。除W05外A也为TASK_RUNNING。固定任务ID为I=0、A=2、B=3、C=4、D=5，不复用同一ID指不同对象。

| ID | 输入与调用 | 主线推导及要区分的现象 |
|---|---|---|
| W01 游标见证 | A=25，jiffies=100，基准队列，强制一次选择 | 先选B；比较对象不更新时，A可能被插到D和I后。用于区分游标错误；used=0，不混入计费时点。 |
| W02 计费见证 | A=15，jiffies=110，基准队列，强制一次选择 | 先选B；A按15插在C20前，随后变25，形成A25/C20失序。本次首次比较即为假，不依赖游标循环。 |
| W03 零增量正控 | 与W02相同，但jiffies=100 | 先选B；A15/C20/D30/I的顺序符合下述候选约束。不能把所有输入一律判错。 |
| W04 相等键对照 | 与W02相同，但jiffies=105 | A最终20，与C相等，非递减约束可成立。不能把相等误判为失序；本任务不裁定公平性或同键FIFO政策。 |
| W05 阻塞任务计费 | W02的A改为TASK_UNINTERRUPTIBLE | 先选B；A不回队，但已有运行时间仍按现函数计入A。记录它的25，不把“未回队”误当成“不应计费”的设计结论。 |
| W06 idle对照 | current=I，I的vruntime人为设7，队列仅B/C/D，jiffies=110 | 先选B，idle回队尾；idle的vruntime保持7。7是明确测试输入，不声称真实idle初值如此。 |
| W07 不重复计费 | 从全新W02初态调用一次；令返回值为新current，jiffies仍110，保持原last_jiffies，强制再调用一次 | 第一次原A计入10；第二次无新增时间，被切出的B应增量0，不应再次把10计入任何任务。两次调用的队列、current和last_jiffies都要记录。 |
| W08 累计尚未切出的时间 | A=15，基准队列，jiffies=105，need_resched=false，time_slice=100先调用；同时间105改need_resched=true再调用 | 首次不切换，队列/vruntime/last_jiffies维持原状；第二次才记入从100起累计的5。别让辅助函数在两次之间重置last_jiffies，不能误报“第一次没计费就是丢计费”。 |

W01预期仅写“可能”是因为尚无本任务的实际输出；报告必须给精确实际ID顺序，不把推导原文当结果。若任一合法输入仍触发原函数的未定义行为或守卫/超时，保留原始输出并停止该条后续解引用，不将其转换为确定性的全局内核故障，也不为求输出而偷偷修原函数。

## 5. 判据和输出：先看记录完整，再比较候选约束

候选约束P：被选择任务取出后，回队的非idle可运行任务按**计入本轮消耗之后**的vruntime非递减；idle另行处于尾部。P是本实验显式提出的政策，既不是Owner已经决定，也不是声称MyOS2必须实现完整公平调度。分别报告P的非idle排序与idle尾部子条件，不能把二者混成一个数字。

每次调用至少记录：场景/重复次序/调用步号；jiffies和last_jiffies前后；调用前current ID、state、vruntime、time_slice和need_resched；返回ID与rq.curr；所有已知任务的前后vruntime；完整队列ID+vruntime顺序、count、实际链长、重复节点检查、idle和current的出现次数。记录外部推导的used值，不冒称插入了原函数内部探针。

独立oracle使用普通值列表比较更新后的排序键，不复制原函数的指针遍历和计费次序。按输入计算候选约束P和预期守恒/计费，再与原函数的边界输出对应。记录完整但违反P应是**有效见证**，不能成为COMPARATOR_INVALID；记录完整而推翻主线推导同样保留。链表结构/计数、计费、候选排序分别输出，不以其中一项符合掩盖另外一项。

每个场景独立进程执行两次，保留**两次完整stdout、stderr、退出码、超时及终态记录**，不只留repeat_identical布尔值。单次编译上限60秒、单次场景运行上限5秒；遍历日志上限16节点。编译失败不得执行旧二进制。所有输出根用新建目录，不清空复用不明路径。

只加三项轻量消费正负控：完整W03样本可用；删去必需的一次调用快照后是缺证据而非“排序无失败”；完整但与预测不同的样本仍是有效反证。变造输入单列为元测试，不写回真实运行数据，不复制上一轮全部M/N测试。

## 6. 小型交付与结束条件

在新写区交 `MANIFEST.md`、`results.yaml`、`evidence.md`，以及最少可重建的 `fixtures/` 和 `observations/`。不复制整个旧包、不交二进制/ZIP。MANIFEST把“完成见证”与“内核是否正确”分开；results由实际完整输出计算，按W01-W08逐条列完成/未执行、保真边界、排序/计费/结构检查、是否支持或反驳候选。evidence给实际命令和源函数抽取身份，细节可链接同目录两次原始记录，不靠口头“全部完成”。

复用旧模块时指向0d62中确切路径并在导入前核身份。新模块首部留真实执行面/模型可知性/来源，文档YAML写自身归属；未知模型如实写unknown_or_not_attestable，不冒用主线gpt6。提交仅写实际短号，摘要分段，不出现完整提交号。新测试若需新的记录格式，只定义本八组所需字段，不抽象成通用框架。

目标是将两个候选变成有限的可核反例或反证，并明确只修游标为何可能不够。不得修内核、生成候选内核补丁、扩展为SMP/QEMU试跑或认定Owner学习路线。遇到新的非阻塞问题登记，不不断加用例拖延交付。受阻项如实报告，其余独立安全项继续；结束条件是八组都已有对应结果或具体未执行原因，并完成上述三项结果核对。

## 7. GitHub回传与分支规则

写前、推送前核对：执行分支仍含0d62，所有该提交的文件逐字节不变；新增只在scheduler-order-02；time仍与固定源相符；主线新增文档不作为异常；无不明并行改动。不要把guard3的旧consumer-fix-02前缀直接换成宽泛白名单，另记录本次确切task/目录/源身份的绑定。

若PR17仍open：在同一分支续作，新实验开始时转回Draft（或保持Draft），更新同一个PR；旧通过回执只对0d62已审部分有效，不意味着新实验也已验收。若PR17已被Owner合并：先确认0d62原件已在master、没有同执行分支的其他open PR，再为本次新增内容开一个Draft PR到master。不得要求Owner为了启动先合并；不得同时存在两个指向同一执行分支的open PR；主线/执行者都不代Owner合并。

结果先提交冻结，再按实际对象做远端回读，回读记录后补避免自引用。原执行和主线分支都保留到本轮回收审查完成且明确解除。完成后只回真实PR链接、提交短号和简短结果；Owner把链接发回主线即足够，无需转抄日志。
