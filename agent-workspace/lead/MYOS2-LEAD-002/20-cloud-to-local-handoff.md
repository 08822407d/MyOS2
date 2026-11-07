---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: cloud_closeout_and_local_takeover_contract
conversation_display_name: "MYOS2-A-C02 内核分析主线"
produced_by: "newest gpt6"
produced_by_source: "Owner此前告知的网页端显示名，非本轮客户端直接观测。"
model_per_owner: gpt6
date: 2026-10-25
base_snapshot: "kernel=time；instructions=agent/MYOS2-LEAD-002；execution=claude/dazzling-cori-q0dnyt（分支名）"
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-EXECUTION-HANDOFF-06
required_review_record: CORE-USER-VFS-BASELINE-05-REVIEW-001
review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-VFS-BASELINE-05-review.md
findings_policy_ref: agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md
reviewed_execution_short12: 293a9e3ef5f4
baseline05_results_short12: 22934124fe92
baseline04_results_short12: eb75b3100601
kernel_source_short12: a039d9803ade
execution_branch: claude/dazzling-cori-q0dnyt
initial_execution_pr: 17
cloud_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/execution-handoff-06/cloud/
local_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/execution-handoff-06/local/
existing_files_frozen: "293a9e3ef5f4全部既有文件；云端新增件在本地阶段也只读。"
status: READY_FOR_OWNER_FORWARDING
external_execution_started: false
cloud_followup_policy: ONE_SMALL_CLOSEOUT_ONLY_NO_NEW_RESEARCH
local_billing_policy: OWNER_SUBSCRIPTION_ONLY_NO_API_OR_PAID_FALLBACK
single_writer: CLOUD_CLOSE_THEN_LOCAL_ACCEPT
kernel_change_authorized: false
kernel_patch_production_authorized: false
host_fixture_execution_authorized: false
full_kernel_build_authorized: false
qemu_authorized: false
phase3_authorized: false
acceptance_ceiling: PASS_PENDING_LOCAL
owner_decisions_requested_now: []
read_channel: connector
evidence_class: "执行面迁移合同；不是本机接收完成或云端scratch已经保全的证明。"
inputs_read:
  - "05主线审查回执所列实际读取；05 evidence明确生成器与探针驱动/副本未交付。"
  - "05 uv_facts_check.py全文；03 scan_index.py开头REPO/PIN与04检查器在本会话的既有读取。"
  - "Owner当前指定改用本地Claude Code订阅额度；18号问题收存约定。"
local_validation: "待执行面；本主线没有运行命令、修改本机配置或访问云容器。"
open_questions:
  - "约8美元余量为Owner报告，无法保证任何特定收尾耗费。"
  - "本机路径、登录与Git写权限须在本地检查，不能继承云端容器路径或授权。"
---

# 先小收尾，再本地接手；中间不需要再开一轮Pro确认

## 1. 目的与固定边界

已交成果主要在GitHub，不需要云端重新总结数万行历史，更不需要ZIP搬运。剩余云额度只用于保全可能独有的少量scratch资料；后续实质工作改用本地Claude Code、Owner订阅内额度。不要充值、开启额外用量、切到API或另开收费产品。

当前05已获限定PASS_PENDING_LOCAL接收，上轮MR-01/MR-02按追加文档关闭；运行层仍NOT_RUN。所有代码缺陷、政策与未知继续按18号记录，不提Owner问卷、不修内核。此任务仅迁移、核对及整理继续入口，不自行启动新研究。

仓库08822407d/MyOS2。主线agent/MYOS2-LEAD-002仍由ChatGPT写；执行分支claude/dazzling-cori-q0dnyt由当前唯一执行者写。PR17仍open时复用、不新建竞争PR。合并不是前置；不合并、不删分支、不force-push、不改remote配置、不直接写master/time。

## 2. CLOUD_CLOSE：发给原云端会话

```text
执行 CORE-EXECUTION-HANDOFF-06 的 CLOUD_CLOSE，只做最小交接收尾。
读取主线分支的20-cloud-to-local-handoff.md §1–§3，按§3完成。
不要继续baseline研究，不重读全部历史，不启动子代理、网络调研、测试或新扫描。
只检查本会话已知目录的未推送/未提交工作及独有scratch文件，保存必要小型文本与复现入口；不复制凭据或整个环境。
写入约定cloud/目录，推回原分支和PR17后停止写入，等待本地接手。
不充值，不自动改用其他额度；额度不足就保留现状停止，不反复重试。
最终只给PR链接、短提交和CLOUD_WRITER_STOPPED或实际阻断。
```

这里的停止指云端不再接下一项任务，不代表删除云会话或停止用户其他项目。不要在未获授权时修改账户设置。Owner若曾开启本会话Auto-fix，应先在界面关闭，避免后继PR评论再次唤醒云端。没有开启就不需要额外操作。

## 3. 云端最小收尾内容（只做这一节，不执行本地阶段）

C01 读取当前Git身份与状态：仓库、分支、PR、HEAD短号、相对远端未推送提交、已跟踪/未跟踪变化。检查实际293冻结文件未被修改。不要全盘搜索用户目录、输出环境变量或凭据；不要清理、stash、reset或删除未知文件。若发现不属于本任务的变化，仅列非敏感路径与状态，不擅自打包或提交。

C02 仅在本会话**已经知道**的scratch位置清点：05生成YAML的脚本、12个探针的驱动/构造规则、其他确实独有且影响复现的文字。生成后的正式YAML已经入库，不重复抄；源码解包副本可由固定Git对象重建，不搬运。探针运行结果已经在05 records/checker_probes.json，不把一份摘要冒充完整驱动。

对各类填：已在GitHub的路径；仅云端存在；可从Git重建；未找到/未检查。可确认安全的独有小文本最多4件、总计不超过64 KiB，新增到cloud/assets/。原文复制时保留原署名，另在HANDOFF中写来源；需去敏则记录删减性质，不能宣称字节一致。不要导出.env、token、登录缓存、.claude会话全量、SSH密钥、全环境变量或系统镜像。不执行这些资料。

C03 只写一个主件cloud/HANDOFF.md，正文以中文约1200字内为目标（资产表可另附），不要重新写研究综述。必要字段：

- task_id/followup_id、真实产品与可知模型（未知则unknown_or_not_attestable）、date、base_snapshot、inputs_read、status。
- 293研究冻结点、当前执行分支、结果229、主线20号路径；实际短提交从Git复制，不写40位提交号。文件内容摘要需要时分段，不写连续长十六进制串。
- 05正式入口；未完成项只引用qualified_id/路径；普通问题无需Owner决策。
- C01结果；C02资产清点与未保全项；本机不可继承的环境依赖（例如/home/user/MyOS2固定路径、git/Python/PyYAML；版本只记实际已知，不安装新工具）。
- `cloud_writer_state: STOPPED_AFTER_THIS_PUSH`，声明本次推送后不继续修改执行分支；未能完成则准确写PARTIAL，不伪报清洁或无遗漏。

先提交HANDOFF及获准资产，再push并从远端读回主件。最终短号放PR评论/聊天，文件不自引用其尚未产生的提交号。沿用任务提交前缀，不改旧05成果、不更新其模型署名。

**没有费用保证。** 达到可用额度/登录限制立即停，不为完成收尾充值；不得为一份总结启动多代理。云端无法再运行时，本地仍可从293开始，登记`CLOUD_SCRATCH_NOT_INVENTORIED`，只把依赖缺失scratch的复现留待以后，不把已入库正文判丢失。

## 4. LOCAL_ACCEPT：发给新的本地Claude Code任务

```text
执行 CORE-EXECUTION-HANDOFF-06 的 LOCAL_ACCEPT，接手MyOS2分析执行，不修内核。
先读仓库08822407d/MyOS2、分支agent/MYOS2-LEAD-002的
agent-workspace/lead/MYOS2-LEAD-002/20-cloud-to-local-handoff.md，
再读其指定的05审查回执、18号问题收存约定和云端HANDOFF（存在时）。
只使用本地Claude Code订阅额度；不自动切API、额外用量或收费替代品。
在全新隔离clone中一次完成L00–L04，不切换、不清理、不改动我原开发工作树。
延续claude/dazzling-cori-q0dnyt，PR17仍开放则复用；不与云端同时写。
旧293文件和cloud收尾原件冻结，仅向约定local/目录新增接收证据。
只运行获准的Python静态检查，禁止旧安装脚本、编译、C/ASM、QEMU、内核修改。
不默认存在/home/user/MyOS2；按合同记录必要的路径适配，不能削弱检查以取得成功。
全部结果经GitHub回传，普通问题只入库，不问我修复意见。
无关单项受阻先记录并完成其他安全项；最后给实际PR、短提交和接手状态。
```

本地任务启动前由Owner检查登录使用订阅账户；本地Agent只记录可观察的认证类型，不索取、打印或上传密钥。只知道用户已选订阅、无法机器证实计费时如实标owner_asserted，不伪造账单验证。怀疑API/第三方路由正在生效时先停模型密集工作，报告一次准确的登录阻断；不修改全局账户配置或删除其他项目的Key。

## 5. L00–L04：一轮完成本地接手和最小验证

### L00 单写者与本机隔离

在Owner给定的新空目录内建立**全新clone**，不复用正在开发的MyOS2目录，不对它执行checkout/pull/stash/reset/clean。新目录若已有文件就换一个新的子目录，不覆盖。不得使用共享worktree去改变原库元数据。使用既有git/凭据，新clone为同一个08822407d/MyOS2，不是fork。

不自动执行项目hooks、setup、安装或构建脚本，不递归初始化submodule；发现项目代理配置要求这些动作时仍以本合同禁止范围为准。clone/fetch本身获准，但只落在新隔离目录。原开发工作树内容不读取、不上传。

云端已停止后才允许写。优先读cloud/HANDOFF.md；没有它时允许从293恢复，但必须确认Owner是把本任务交给本地且不再向云端续派，推送前再次检查远端无并行前进。若HEAD有解释不了的改动，停止写入，继续可做的只读核对；不force或整包覆盖。

### L01 输入身份、全旧件保留与最小上下文

获取执行分支、time、master、主线分支以及293/229等固定对象；不用master目录代替执行成果。记录实际短号与PR状态。检查：293为当前执行头祖先，293的全部已跟踪旧文件字节不变；云端阶段新增只在cloud前缀，无其他相交提交；源码time仍对应a039（若不同，固定旧源做历史复核并单列差异，不把旧事实自动当最新）。

读取顺序：本合同 → 05-review → 18号 → 05 MANIFEST/基线/勘误与deferred头部及恢复索引 → cloud HANDOFF（存在时）→ 本阶段所需检查代码。旧core、W实验与大YAML按索引定点读取，不把七万行历史全塞进上下文。不要重跑H00/V/W/M/N或重建环境试跑。

现状摘要应能回答五件事：目标仍是以MyOS2辅助内核学习；阶段2代表基线到05；代码修复与A/B均未授权；延期问题从18号和本批增量找；当前只做安全静态核验和接手。

### L02 仅路径适配的05静态复核

核验依赖链：05/scripts/uv_facts_check.py → 04/scripts/mm_facts_check.py → 03/scripts/scan_index.py，以及固定Git对象、PyYAML。先静态检查实际调用，确认不执行C/ASM、内核或原脚本。

scan_index的REPO固定为/home/user/MyOS2。**不要直接运行后把找不到该目录当作源码失败；也不要在本机伪造系统路径或修改原件。** 允许在local/写一个很小的适配入口：在独立Python进程中按原相对关系加载模块，将共享S.REPO重绑定到新clone的真实路径，再调用原uv_facts_check.main。PIN、REVIEWED、输入数据、期望值和所有判定逻辑保持原样。若运行前还需其他平台适配，应先记录精确原因，不静默改业务规则。

import之前禁写字节码，保留`PYTHONDONTWRITEBYTECODE=1`；新输出只能放新local/records/目录。传入完整05包及mm-04-disposition.md、user-vfs-baseline.md两个Markdown，不覆盖05内records。不得重跑原scope start/stage/docs来要求master或工作树回到云端旧状态；按L01的新边界单独核。

保存实际命令、返回码、stdout/stderr、工具版本、复核JSON和查询记录。正常结果应解释90引文、31标签、44查询、7词法检查、40旧文字定位以及值表计数是否与冻结记录一致；不能只比一个all_ok。失败、不可用依赖或异常均记录，不为凑成功补默认值。

这是静态文本一致性复核，不是90条语义全审、不是内核重测。缺PyYAML时可使用已安装的其他现成环境，不安装工具；无法执行就标L02 NOT_RUN并继续L03/L04。已交12个负控驱动若未取回，不声称已复跑；本轮不要求重建它们。

### L03 保全检查与可续接工作地图

生成一份短resume-map.md和可选YAML索引，按包列pilot/core/recheck-01/consumer-fix-02、scheduler-order-02、scheduler-integration-03、mm-baseline-04、user-vfs-baseline-05的实际入口、主线回执、已接受范围、未运行层和后继勘误。不要复制问题全文，不把53子系统的全量盘点或007R整项写成已完成。

列cloud仅存资产的接收/缺失状态和重建方法；未知不当作无遗漏。对本轮接触的旧口径加消费提示：公式负长度不等于已观察到必然机器终态，局部无调用不等于全树不可达，Python检查已经执行但内核NOT_RUN。不是修改原报告。

为下一次安全分析列最多3条已有资料缺口，优先引用05 GAP-U03/U04/U05等真实端点；只给证据入口、范围与所需授权，不新开研究、不修缺陷、不向Owner出问卷。当前任务不是阶段3学习路线或裁剪决策。

### L04 一次推回与停止条件

主要交付：local/RECEIPT.md、local/resume-map.md、local/records/（最小实际核对记录）及必要的小型路径适配脚本。RECEIPT区分输入已接收、scratch未穷尽、本地静态复核成功/失败/未运行、推送已完成或阻断。执行者署真实产品和可核模型，未知记unknown_or_not_attestable，不冒用主线模型名。

提交前核旧293文件、cloud收尾件均不变，新增仅local前缀；不存在相交外来修改。推前再次读远端，必要时只快进获取已经理解的cloud追加，不merge未知修改、不force。PR17仍open就维护同一Draft；已由Owner合并则先核旧件在master且无同源open PR，再为本批唯一新PR；关闭未合并则只读保留并报告。

结果先提交、push、按远端对象回读，再在PR/最终回复写实际短提交。不要把云端最后一句“完成”当成本地接收证据。最终只返回PR、短号、接手状态与“问题已入库，当前无需技术决策”。没有真正阻断不要求Owner操作；整个过程不需要人工下载或转发正文。

## 6. 权限与费用止点

本地执行不等于允许运行项目中的安装/分区/挂盘/删除脚本。禁止完整构建、QEMU、C/ASM夹具、内核/用户程序修改、磁盘镜像操作、工具安装、提权或凭据输出。旧测试被允许执行过，不构成本次重跑权限。

订阅额度不足就保留阶段性结果；不得开启extra usage、fast等额外付费配置、调用API备用或自动转回云端。脚本核对失败不阻塞其他安全接手工作；只有不能安全确定仓库、权限、单写者或写区时才停写。保留两个分支和云端会话至接收完成，不删除历史。

## 7. 产品说明与选择依据（2026-10-25查阅，非账户实测）

[EXTERNAL https://code.claude.com/docs/en/authentication] 官方列明本地可用订阅账户登录；环境API凭据、apiKeyHelper等可能优先于订阅OAuth。用/status确认当前认证，必要时/login选择claude.ai订阅；不要仅凭“在本机运行”判断走周额度。Owner账户的实际剩余额度不在本主线可见范围。

[EXTERNAL https://code.claude.com/docs/en/claude-code-on-the-web] 官方提供--teleport迁移云会话历史和Git分支，但要求正确仓库、干净工作树、同账户等，且不等于搬运全部scratch。此任务默认GitHub接手，不要求传送完整历史；若Owner自行使用teleport，也只能在新隔离clone中，接收标准仍以本合同为准。不要在原开发目录接受自动stash提示。

云端简短收尾后，本地可以按本合同直接接收，不必等主线再批准一次相同范围；只有出现新阻断或权限扩展才回主线。云端发布HANDOFF和本地发布RECEIPT都不替代最终主线审查。
