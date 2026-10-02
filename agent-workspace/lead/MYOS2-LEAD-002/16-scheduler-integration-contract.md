---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: scheduler_integration_fact_and_repair_spec_taskbook
conversation_display_name: "MYOS2-A-C02 内核分析主线"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-10-02
base_snapshot: "kernel=time；workspace=master；instructions=agent/MYOS2-LEAD-002（分支名）"
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-SCHED-INTEGRATION-03
phase: scoped_source_integration
required_review_record: CORE-SCHED-ORDER-02-REVIEW-001
review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-SCHED-ORDER-02-review.md
reviewed_execution_short12: f36b89b8a53a
witness_results_frozen_short12: 5078686e8267
kernel_short12: a039d9803ade
execution_branch: claude/dazzling-cori-q0dnyt
initial_execution_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-integration-03/
existing_files_frozen: "f36b89b8a53a中的全部已有文件；仅在新前缀新增"
status: READY_FOR_OWNER_LAUNCH
execution_disposition: RUN_WHEN_OWNER_FORWARDS
external_execution_started: false
kernel_change_authorized: false
kernel_patch_production_authorized: false
host_fixture_execution_authorized: false
full_kernel_build_authorized: false
phase3_authorized: false
acceptance_ceiling: PASS_PENDING_LOCAL
read_channel: connector
evidence_class: "已核原函数见证与源码局部阅读所导出的有界静态任务，不是已执行的扫描结果"
inputs_read:
  - "15-scheduler-ordering-witness-contract.md全文及本轮CORE-SCHED-ORDER-02审查回执列明的实际程序/记录。"
  - "time:options_flags.cmake、myos_rt.c全文；scheduler_core.c的set_task_cpu、try_to_wake_up、__sched_fork、sched_fork、wake_up_new_task、init_idle等区段；double_list.h的header操作区段。"
supersedes: null
continues_from: agent-workspace/lead/MYOS2-LEAD-002/15-scheduler-ordering-witness-contract.md
self_check: {scope: this_file_only, verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
local_validation: "待云端静态扫描与证据整理；主线未执行命令。本任务不重新运行原函数实验。"
open_questions:
  - "头插优先与全局升序是不同政策，先给出兼容性影响和建议，不冒称Owner已采用其一。"
  - "直接字段搜索不构成完整调用图/运行可达性证明，别名和条件分支未解项须保留。"
---

# 从已确认的局部反例，走到可以实施的最小改进规格

**本任务不再复跑选择器、修验证器或收集更多同类反例。一次查清谁创建/修改运行队列与排序键、现有调用者依赖什么，再交一份有明确修改范围和回归条件的候选方案。** 不改内核、不生成补丁，不替Owner开启正式学习路线或阶段3。

已有W01/W02/W07足以支持当前限定发现。本次需要补的是接入关系：只修pick不能自动保证所有新任务、唤醒任务和idle的行为正确。核心入口已由主线回源确认，见当前审查回执§3；本任务不能仅重新复述那七条引文作为完成件。

## 1. Owner只发这一段，交接仍走GitHub

在创建PR17的原Claude Code Cloud会话，沿用现有模型、effort、费用和GitHub配置，发送：

```text
执行 MYOS2-LEAD-002-CORE-CHECK-01 的 CORE-SCHED-INTEGRATION-03。
先完整读取：
https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/16-scheduler-integration-contract.md
再读其指定的 CORE-SCHED-ORDER-02 审查回执。
在原云会话和 claude/dazzling-cori-q0dnyt 分支，一次完成 D01-D05：
定点静态扫描、生命周期/计费与调用关系、最小改进规格及回归对应表。
复用已交W/V证据，不再编译或运行C夹具、不重做验证器；不改内核、不出补丁。
只在任务书的 scheduler-integration-03/ 新目录新增，f36b89b8a53a全部旧文件不改。
PR17仍开放就转回Draft并复用；已合并则先核原件已入master，再为新内容开唯一Draft PR。
不合并、不删分支、不安装工具、不提权、不运行原仓库脚本。
保留反证和未解项，结果直接通过GitHub返回，不让我下载上传文件。
完成后只给实际PR链接、提交短标识、建议和真正需要人工决定的事项。
```

本任务发布或评论不等于云会话已经启动。合并不是前置，Owner不用先整理资料、复述技术细节或管理扫描步骤。原会话不可恢复、不能推回原分支、不明并行改动或PR关闭未合并时，保留工作并报告具体阻断；不索取token，不自动换收费产品，不创建竞争PR。

## 2. 固定输入与权限

读取本任务 → 同目录 `reviews/CORE-SCHED-ORDER-02-review.md` → 执行头f36b89b8a53a的 `scheduler-order-02/MANIFEST.md`。确认回执record_id、packet、followup、PR17、分支、f36被审头、5078冻结结果、PASS_PENDING_LOCAL和15号合同无剩余退回项。14/15号仅为理解候选约束与输入参照，不重新执行。

开工时从主线分支固定本次实际任务对象并记录短号；新回执/检查点追加属于预期，不要求主线永远停在6706013a079a。源码固定time a039d9803ade，读取工作区规则用master。现行公约/协议与本主线工作令沿用；本任务专项允许的静态扫描和新写区仅在Owner转发后生效。

只允许：Git只读对象查询/定点文本检索；使用已有Python进行文本提取、结果计数与YAML/JSON核对；新增结果分支提交、推送、PR维护和远端回读。不编译或运行C/ASM，不跑H00、原V/M/N/W整批，不运行仓库安装/构建/启动脚本、QEMU、内核、网络扫描或硬件探针，不安装工具、不提权、不输出凭据。

结果目录仅为YAML中的allowed_write_prefix。f36已有文件全部冻结，包括pilot/core/scheduler-order-02与consumer修订。中间文件使用新建隔离目录，不覆盖或清理不明工作树。复用历史扫描/读取助手前读其入口，只取必要原语，不触发旧整批执行链。

## 3. 一次完成D01-D05

### D01：生成可追溯的直接字段/入口索引

在固定time的 `mykernel/` 内，仅围绕 `running_lhdr`、`last_jiffies`、`vruntime`、`rt.run_list`/等价成员访问、`myos_rt_sched_class` 和以下已知入口做定点文本检索。保留实际命令、文件范围、命中数、错误退出/未读取项；不为了这些词执行全源码解释、通用调用图工程或配置穷举。

已知种子：`scheduler_core.c`中的 `set_task_cpu`、`try_to_wake_up`、`wake_up_process`、`wake_up_new_task`、`__sched_fork`、`sched_fork`、`init_idle`、`sched_init`、`__schedule`；`myos_rt.c::pick_next_task_myos`；`double_list.h` 的list_header增删与相邻原语。只按实际命中打开所需定义、宏和直接调用者，不猜文件名，不搬第一波失实断言。

每个相关命中给路径/所在符号/逐字短引文，分为：声明或注释、可见活动文本、条件依赖未解、未能定位。跟踪局部别名到header/list原语时记录别名关系；未能跟踪不伪装成覆盖。源码引用遵循1–5行in-body锚点，跨定义分开。不得将“直接字段检索没有其他命中”改写成“没有其他写入者/调用者”或“全局不可达”。

### D02：只把五条生命周期链补齐

按初始化/idle → 新任务首次入队 → 睡眠后重新唤醒入队 → 运行后回插 → 下一任务选取组织事实表。每行至少给：实际入口和直接调用关系、任务state与键值的输入/输出、队列成员与count操作、是否特殊处理idle、调用者可见返回值、证据与覆盖限制。

必须区分 `set_task_cpu` 的MyOS2实际入队副作用与上游同名职责；`wake_up_new_task`首次入队与普通唤醒不能合并成一条；`__sched_fork`的零键初始化不能外推为睡眠后每次醒来都清零。追踪实际 `pick_next_task` 分派到MyOS2类的路径和该类的设置处，最多补至局部直接调用链；不宣称因此已证明从真实开机可达所有状态。

必要时读取 `init/main.c`、`sched/forkexec/fork.c` 和定点命中所在的实际文件。若链条遇未解宏/回调或活动条件，停止该推导、保留缺口，完成其他独立链；不能用Linux通用知识填空。

### D03：计费与队列不变量分别列清

确定本局部中谁初始化/更新 `last_jiffies`、谁写 `se.vruntime`，是否有其他实际活动记账点；每处给源码证据，不因字段名相似混入未活动的上游路径。保留已有W05/W06/W07/W08的正常行为：阻塞任务已经用掉的时间仍可计入、idle不计费、同时间连续调用不重复加、尚未切出时累计时间未必丢失。

对“高键睡眠任务头插到低键队列”给一份**源码状态推导**，注明前提和政策含义；不生成新的宿主执行次数。表中分开：结构安全（节点/anchor/计数/当前任务）、记账守恒、头插优先、按更新键排序。不得将某一排序政策当作结构安全本身。

### D04：交付候选改进规格，而不是直接实现

给两种有明确差异的候选：

- **兼容现有入口语义的局部修复**：保留已识别的首次入队/唤醒头插行为，针对回插比较对象、anchor判定顺序、记账时点形成最小修改要求；明确它不能承诺全局始终升序，说明其成立前提和未覆盖影响。
- **统一队列顺序契约**：若决定把按更新后vruntime排序作为共同目标，哪些入队、回插、初始化与选择规则必须一起调整；失去或改变哪些唤醒优先行为，涉及哪些调用者。不要把完整CFS/EEVDF、SMP平衡或新调度类别引入本次规格。

每种列：改变的可观察行为、精确受影响符号、必须保持的行为、风险、现有证据、剩余验证、建议先后。能给出基于项目学习目标的建议就直接给出及理由；重要政策采用仍标待Owner/主线明确决定。不得用“可能是作者意图”把事实判定取消，也不得把推测的作者意图当成修改授权。

规格用文字、状态表和接口契约，不交C实现、after代码块或diff，不修改被测函数。最多提出三个真正影响结果的Owner取舍，每项附建议、实际影响和不答时的安全处理；不得把选扫描工具、如何拆文件等内部安排推给Owner。

### D05：把规格接到已有回归，并给出停止点

给出映射：W01/W02/W07的失序证据分别支持哪个修复要求；W03/W04/W05/W06/W07/W08哪些正常行为必须保持；旧V04–V08只在相应已审范围内引用，不重新跑或扩大结论。建议新增回归**至多六项**，只补生命周期接合处、anchor边界和政策差异真正缺少的内容，写清输入/观测/预期与当前状态NOT_RUN。

回归规格要区分“完整记录且违反候选策略”与“记录缺失”，以及“按队首选择符合代码”与“选到了目标策略所要求的任务”。保留两次完整输出要求作为未来执行要求，不能声称本任务做了动态测试。

结束时明确：可以准备实施哪些改进、哪些需要先作政策决定、哪些因为真实运行层仍未知不能承诺。不要把结论写成第二波全量完成或新一轮研究发射清单。

## 4. 紧凑交付

本次交付以实质内容为先，只要求四份主件：

1. `MANIFEST.md`：限制、D01-D05逐项完成/未完、实际文件列表、唯一重建入口、下一步建议。
2. `integration.md`：五条链、计费责任、两种候选方案、修改边界及最多三个重要取舍。
3. `facts-and-regressions.yaml`：直接写入点/调用边/条件、已有见证引用、候选修改点、最多六个未来回归规格。事实、推导、政策提案与NOT_RUN分列。
4. `evidence.md`：实际命令与输出、检索范围、必要引文/计数、未解宏和别名、构建与运行未执行声明。

另附最少的静态扫描/提取脚本和命中记录，仅为从仓库重建上述材料；最多三个小脚本，不复制整个旧包、不放二进制/ZIP、不重新建设闸门框架。主线审查回执中七条G锚点可做定点机械核对并计数，结果单列，不改原47条分母或重跑整批核验。

source facts使用路径/符号和逐字引文，MANIFEST计数与本批实际标签相符。文件署名写执行者真实执行面、可知模型或unknown_or_not_attestable；不冒用主线gpt6，不改旧产出的署名。记录来源短号与分段摘要，不输出40位提交号。反证优先保留，无证据用unknown，不以省事写为false。

## 5. 回传与收口条件

开工、推送前核：分支包含f36，所有f36旧文件不变；新内容仅本次前缀；time与固定源一致；PR唯一且无不明相交写入。主线追加文档是预期，合并到master可能前进不意味着源码变更，但须核对旧原件仍在。

PR17仍open则先转Draft后复用，新内容不继承f36的接收；PR17已合并则先核f36全部原件已入master、同执行分支无其他open PR，才为新内容创建唯一Draft PR。PR关闭未合并时停报，不自创分叉流程。禁止合并、删除分支和force-push。

结果先提交固定，远端按对象回读；回读记录后追加，避免自引用。本次完成标准：D01-D05各有实质回答或具体限界、事实可回源、两种改进范围可比较、未来回归未冒称执行、旧原件未改。不是运行测试通过，也不是所有宏/别名/全局可达性的形式化证明。

不要在已经做到这些以后自行追加另一轮实验、补丁或QEMU任务。最终只回真实PR链接、短提交、建议和阻断。主线从GitHub取回并检查；Owner不逐项搬运、摘要或代为技术审阅。验收上限为PASS_PENDING_LOCAL，任何内核实施及阶段转换仍需其本来的授权。
