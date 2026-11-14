---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: static_memory_baseline_and_small_spec_disposition_taskbook
conversation_display_name: "MYOS2-A-C02 内核分析主线"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-10-02
base_snapshot: "kernel=time；workspace=master；instructions=agent/MYOS2-LEAD-002（分支名）"
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-MM-BASELINE-04
phase: phase2_bounded_static_baseline
required_review_record: CORE-SCHED-INTEGRATION-03-REVIEW-001
review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-SCHED-INTEGRATION-03-review.md
reviewed_execution_short12: ee9e6a738224
integration_results_frozen_short12: b7fa83583e35
kernel_short12: a039d9803ade
execution_branch: claude/dazzling-cori-q0dnyt
initial_execution_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/mm-baseline-04/
existing_files_frozen: "ee9e6a738224中的全部文件；新目录内追加交付，不改scheduler-integration-03等原件。"
status: READY_FOR_OWNER_LAUNCH
execution_disposition: RUN_WHEN_OWNER_FORWARDS
external_execution_started: false
kernel_change_authorized: false
kernel_patch_production_authorized: false
host_fixture_execution_authorized: false
full_kernel_build_authorized: false
candidate_A_adopted: false
candidate_B_adopted: false
phase3_authorized: false
acceptance_ceiling: PASS_PENDING_LOCAL
read_channel: connector
evidence_class: "可在不修改调度器的情况下执行的静态盘点规格；不是已完成的内存评审"
inputs_read:
  - "本轮CORE-SCHED-INTEGRATION-03审查回执所列已读范围，包括NR-6与A/B字段。"
  - "time:mykernel/mm目录直接子项；mykernel/mm/mm_api.h与options_flags.cmake全文。只据此确认起点，不声称已经分析内存算法。"
  - "本会话已取得的002R/003R课题和主线目的；旧研究只提供ID/结构起点，不作新源码证据。"
supersedes: null
continues_from: agent-workspace/lead/MYOS2-LEAD-002/16-scheduler-integration-contract.md
self_check: {scope: this_file_only, verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
local_validation: "本任务待云端；主线未运行任何命令。"
open_questions:
  - "A/B及键下限暂不采用，相关内核修复无授权；这不阻止四个内存子系统的静态盘点。"
  - "新增内存基线仅覆盖本次挑选的代表路径，不替代002R/003R的完整任务验收。"
---

# 先更正两处AI规格，然后连续补齐四个内存子系统的事实基线

**本次不是继续追调度bug，也不是要求Owner先修代码。** 一次完成M00–M04：短小的规格勘误后，转向物理页分配、内核堆分配、虚拟映射和缺页处理，回答“已有实现是什么、接上了什么、哪些地方还不能当作可靠基础”。不选学习路线，不实施A/B，不新增深度研究。

## 1. Owner只需在原Claude Code Cloud会话转发

```text
执行 MYOS2-LEAD-002-CORE-CHECK-01 的 CORE-MM-BASELINE-04。
先完整读取：
https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/17-memory-baseline-contract.md
再读其中指定的INTEGRATION-03审查回执。
沿用原云会话与claude/dazzling-cori-q0dnyt分支，一次完成M00–M04。
先以值表处理IR-01/IR-02，再直接完成四个内存子系统的静态实现与依赖盘点；不要只交勘误就停止。
A/B与键下限都不采用；不改内核、不出补丁、不编译或运行C/ASM、内核或QEMU，不重跑W/V/M/N，不重造验证器。
仅在任务书的mm-baseline-04/目录新增，ee9e6a738224全部原件保持不变。
PR17仍开放就保持Draft并复用；已合并则按任务书确认原件已入master后，为新内容开唯一Draft PR。
不合并、不删分支、不安装工具、不提权、不输出凭据。
单项受阻保留原因并完成其他安全项；保留反证。通过GitHub交回全部正文和依据，不让我下载上传文件。
完成后只返回真实PR链接、提交短标识和实质结论。
```

沿用现有执行面、模型、effort与费用配置；无须另开同题会话。若原会话/分支不可恢复、PR关闭未合并、不明并行修改或源身份不符，保存工作并报告具体阻断；不索取token、不自动换收费产品、不另建竞争分支。发布本任务或PR评论没有启动外部执行。

## 2. 读取顺序、固定对象和允许动作

本任务 → 同目录reviews/CORE-SCHED-INTEGRATION-03-review.md → ee9e执行对象中scheduler-integration-03/MANIFEST.md及审查指向的IR-01/02部分。确认record_id、ee9e被审头、b7fa结果和next_followup。本回执是局部RETURN；**本任务明确授权先追加处置再继续独立的内存静态分析，不要求先把整个旧任务改成通过**。

开工固定主线实际取得的任务对象，用实际复制的短号记录。之后主线预期追加文档不是异常，不要求它永远停在40bc4faa4202。内核仍固定time a039d9803ade；工作区规则从master读取。读公约与协议时沿用主线适用边界，不导入其他项目任务。

允许Git只读对象查询、定点文本检索，现有Python对文本/YAML/JSON/简单值表的处理，以及本执行分支新写区的提交、推送、PR维护与远端回读。禁止编译或运行C/ASM、仓库原脚本、cmake、内核、QEMU、硬件探针、网络扫描、工具安装、提权或凭据输出。所有中间文件放全新隔离目录，不清空不明路径。

写前和推送前核：执行分支包含ee9e；ee9e全部既有文件不变；新增仅本前缀；time仍对应固定源；无相交并行写入。不要把旧任务的保护脚本前缀直接放宽，也不要重跑旧probe/元测试；本次绑定与检查保留必要命令输出即可，不另建通用框架。

## 3. M00：两处规格处置，不是内核修复

按本轮审查IR-01、IR-02，在新目录交integration-03-disposition.md，引用旧原件，声明supersedes_scope仅对应有问题的规格文字，不替换整包事实。

- IR-01：用NR-6明确输入和原pick做六步值表，分别列真实idle最大键与idle键0变体；验证普通任务时间加idle区间才等于总跨度。主线推导idle0原函数为N2=50、N1=40、idle=10；最大键原函数为60/40/0。独立复核，不迎合；有反证写清每步。可用Python作算术，不运行原函数或重写算法冒充原函数执行。A/B仍仅为未执行预期。未来缺观测不当作无失败。
- IR-02：将B保持项显式列出，排除“保留普通唤醒头插”这一与B-R2相反的要求；键下限的两个分支分别写明，不代Owner选取。
- 同一短文登记：沉默不授权实施A；151引文/词法分类不等于编译与运行证明；10个名字未定位；gap记录12条中11条open、1条收窄；不得说所有调用者都不读唤醒返回值。DV-19的立即返回叙述还需下层调用能够返回，不将忙等和立即返回写成同时必然。

M00只修解释和未来预期，不改kernel或旧交付，不新增调度实验。若某一推导无法闭合，登记未解，继续M01–M04；不因这个小项再次把整个自动工作停给Owner。

## 4. M01–M04：四个子系统，同一轮完成

### M01 输入索引与能力对象

子系统固定为mm.page_alloc、mm.kmalloc、mm.vm_map、mm.fault。主线已确认对应目录与mm/mm_api.h中的接口include。从各目录实际文件与API头定位，不根据Linux惯例猜文件名。

只读取以下旧资料的结构、ID、符号名列：master的results/MYOS2-DR-002/completeness.yaml中这四个子系统及其能力节点；results/MYOS2-DR-003/deps.yaml中相交节点/边端点。不得复制其证据路径、行号、溯源或可靠性判断。需要的新源码路径以time目录/查询为准；旧ID必须保留，新节点标proposed_id，不悄悄修改公共词汇表。旧输入缺失时记清，不以此阻塞真实源码盘点。

范围是四行子系统汇总，加至多24个代表能力节点（每个子系统优先3–6个，有多少依据写多少）；其他旧能力单列未覆盖，不伪装全量。代表入口覆盖分配/释放，建立/解除映射，缺页分派/实际处理和必要初始化；如果功能未定位，交检索范围与缺口，不为凑节点编造。

### M02 实现程度与正确性分轴

每个已选能力至少有：稳定ID、用户能理解的用途、实际入口/关键helper、正常路径、失败/边界路径、配置条件、直接调用或数据依赖、已读证据、未覆盖处。至少核正文而不只是函数名或声明。单一入口不足证明整项功能完整。

使用本批局部描述字段，不擅自把结果折算成旧002的0–4评分：

- implementation_evidence：declaration_only / partial_body / connected_body / not_located_in_scope / not_assessed，并给理由；这是本批证据状态，不是性能或成熟度分数。
- correctness_evidence：not_assessed / static_concern / bounded_prior_observation；新静态疑点必须有路径、前提、反证与unknown，不冒称运行确认。
- runtime_evidence：本批统一NOT_RUN；如引用旧实验另列prior_evidence及其原范围，不能变成本轮运行。
- concurrency_evidence：not_assessed / source_only；没有SMP实测，不因per-CPU名字存在而宣称支持SMP。
- source_condition：记录实际条件及已解析的真值依据；只知道宏名不代表该分支生效，未知保留unknown。

### M03 接口之间的真实依赖

至少分别回答：物理页从哪里准备并分配/归还；堆接口是否及如何消费页分配；虚拟映射怎样管理地址区间与页表；缺页入口怎样到实际页获取/映射。命中有缺口就断开标未知，不能以典型Linux设计把链补齐。

读取起点：mykernel/mm/mm_api.h、四个目录的实际API/正文；必要时追到mm/early、mm/misc、arch/x86_64/mm的实际对应定义，以及init/main.c中的相关初始化调用。外部直接调用者用定点查询核对，涉及已知调度/锁/等待风险时标dependency_risk，不重做调度扫描。

每条边区分call / data / init_order / config / build；给实际引文与作用方向。include关系不是调用关系，源码列入构建不等于能链接运行。仅当要判断build边时读对应CMake及实际引用脚本的文本，不执行。能力边上限40，关键源码锚点上限80；优先完整覆盖四类主路径而非一个子系统无限下钻。跨出范围的宏/回调/间接访问保留端点与原因。

### M04 对后续工作的可用性

生成一张短表：哪些机制已能用于源码讲解；哪些有实现但正确性未证；哪些依赖当前有缺陷的调度、锁、等待机制；哪些需要运行后才能下结论。可读、可单独做宿主验证、需要真实内核前置三者分开。静态分析发现依赖风险不等于整模块全部不可用，也不是忽略风险。

最多列六个下一步验证候选，每项写目的、最小观察量、是否触及当前待修项、执行前提，状态统一NOT_RUN；不实现夹具、不直接启动。不要继续细化A/B，不给Owner布置修复作业，不重发九项研究，不进入正式学习路线与裁剪方案决策。

## 5. 实质交付与最小核对

新写区交五份主件：MANIFEST.md、integration-03-disposition.md、memory-baseline.md、facts-and-dependencies.yaml、evidence.md。正文先给当前效果与未决；YAML覆盖四个汇总与实际选择的能力、依赖、风险、范围。不能只交完成清单，也不要求逐回复催交。

另外只保留从固定仓库重建索引/核对引文所需的最少记录，新增脚本最多两个；可复用已经读懂的只读原语，但要说明边界，不复制上一包的大量代码或测试。源码每条锚点按path::symbol加1–5行逐字in-body引文；纯include、构建顶层、声明和注释明确类型，不混进函数体确认。负面搜索保留命令、范围、返回码，工具失败不是零命中。

对实际交付核：ID唯一且引用不悬空；四个子系统都被回应；计数由实际数据产生；引文定位与声明范围；unknown未被默认成false/正确；NOT_RUN未升级；每文件真实署名与来源；新旧文件边界。旧引文检查器对符号范围的宽松分支不能直接充当严格P2认证，按本批每条实际种类处理，不重造解析器项目。

文件YAML填写执行者真实产品/可知模型；未知就unknown_or_not_attestable，不能冒用主线gpt6。只写实际短提交标识，必要哈希分段，禁止40位提交号、二进制和ZIP。

## 6. 一次交回，不把合并当中间步骤

PR17仍open就在同一分支保持Draft并复用；若Owner已合并，先确认ee9e全部旧文件在master且同执行分支无其他open PR，再为新增内容开唯一Draft。PR关闭未合并时停报，不自建竞争流程。禁止合并、删除分支、force-push或写入master/time/主线分支。

先提交结果固定，再按实际对象远端读回核对正文身份，回读记录后补以免自引用。分支推进引起的新增文档是预期，不要求Owner重复批准。结束条件是M00有明确处置、M01–M04各有实质交付或具体限界、未实施未运行层保持原状；不是把全部内存功能判为通过。

两个分支继续保留到主线回收和明确解除。最终只回真实PR链接、短提交、四个子系统的主要结论与阻断。Owner无须搬运或摘要正文，本主线直接从GitHub读取。
