---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: bounded_verifier_repair_taskbook
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-CHECK-01-RECHECK-01
phase: core_recheck_01
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-09-27
base_snapshot: "kernel=time；workspace=master；taskbook=agent/MYOS2-LEAD-002（分支名）"
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-core-review.md
  - "PR17 core 的三份正文与全部二十个 fixtures 文本，冻结头 a1e7c2277705"
  - "本会话已完整读取的 09、10、11、pilot-review；不重新调用旧研究"
evidence_class: "已读程序上的静态反例与有界修复/验证设计；本主线未执行"
status: READY_FOR_OWNER_LAUNCH
execution_disposition: RUN_NOW_OPTIONAL
actual_external_execution_started: false
execution_task_id: MYOS2-LEAD-002-CORE-CHECK-01
execution_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
reviewed_core_short12: a1e7c2277705
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/
required_review_record: CORE-CHECK-01-CORE-REVIEW-001
supersedes: agent-workspace/lead/MYOS2-LEAD-002/11-core-verification-cloud.md
supersedes_scope: "本次 core 后续：允许修验证器并新增 recheck-01；不改 V00-V14 技术题目、源码或原交付。"
acceptance_ceiling: PASS_PENDING_LOCAL
local_validation: "待云端执行；本主线没有运行本任务中的任何命令。"
open_questions:
  - "R01-R04 静态反例需先对冻结旧程序实跑；有反证就保留，不迎合主线。"
  - "真实内核/ELF/IRQ/SMP 未在本任务解禁。"
---

# 一次补好验证器，再对同一源码重核；不是重做内核研究

**本次只修自动核验链的四项问题，不修 MyOS2 内核。** 先实跑主线给出的旧程序反例，再修执行入口、观测完整性与结果生成，随后在同一轮完成有界负测及同源 V00–V14。旧 pilot、旧 core 文件与红结果保留。任务建立在已经读到的实质成果上，不撤销它们的局部价值。

## 1. Owner 操作流程（现在可执行）

打开创建 PR #17 的原 Claude Code Cloud 会话（会话链接在 PR 正文），沿用当前可用模型、effort、费用与仓库连接；不另开 Research、并行代理群、API 计费或另一同题会话。无需合并 #16/#17，也无需上传文件。发送：

```text
执行 MYOS2-LEAD-002-CORE-CHECK-01 的 CORE-CHECK-01-RECHECK-01。
先完整读取：
https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/12-core-check-recheck-contract.md
再按其顺序读取主线 core-review 回执。
在原云会话、分支 claude/dazzling-cori-q0dnyt、Draft PR #17 内续做。
先对冻结旧验证器实跑反例，再按任务书修验证器并完成 M01-M12 与同源 V00-V14；不要只修一个点就停下来。
仅在 agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/ 新增修订代码和真实证据。旧 pilot/core 原件不改，内核不改；不跑原仓库脚本、完整构建或 QEMU，不安装工具、不提权、不输出凭据，不合并、不删分支。
遇到主线推导错误要给反证；单项受阻保留原因并完成其他独立安全项目。结果直接推回同一个 PR，最后只给 PR 链接、提交短标识和简短结论，不让我下载上传报告。
```

执行完后 Owner 回当前主线发“PR17 已更新”或同一 PR 链接即可。原会话无法恢复、不能推回原分支、出现不明相交修改时，保留已交结果，回报具体限制，不新建竞争 PR、不索取 token、不自动迁移付费产品。

本启动块被 Owner 转发才启动外部任务和本次窄范围写入；主线发布文件/评论不是自动开跑。核心“RETURN”表示要求修验证器，不是禁止 Owner 发射本次已准备的重核。

## 2. 固定输入、写区和保护范围

按顺序读取本文件 → 主线分支 `reviews/CORE-CHECK-01-core-review.md` → 必要的旧 11/09 与 pilot-review → 冻结 PR17 core 三文档和 fixtures。主线文档目录统一为 `agent-workspace/lead/MYOS2-LEAD-002/`。

实际对象如下（短标识来自已读连接器/执行记录，不补全）：

| 用途 | 分支/固定短标识 |
|---|---|
| 待修验证器和旧 core 结果 | 执行分支的 a1e7c2277705 |
| 旧 core results.yaml 首次提交 | 7e2fa84a9823 |
| 旧 pilot | 0851af4fc08b |
| 六份原技术输入 | agent/MYOS2-LEAD-002 的 57a7c3e0eebf |
| pilot ALLOW_CORE 回执 | agent/MYOS2-LEAD-002 的 f79b3a281616 |
| 本轮审查与修订任务 | 启动时从 agent/MYOS2-LEAD-002 取一次，固定本次读到的对象 |
| 内核源码 | time，基线 a039d9803ade |
| 工作区/公共规则 | master；本次主线读取时 de3bb1df906a |

开始先核 core-review 的 record_id、packet_id、被审 PR17/分支/core 头与本任务一致。比较六份原技术输入是否仍与冻结点相同；主线只追加回执、勘误和检查点属于预期前进，不是技术输入改变。源码若改变，列出影响，暂停受影响实测，不把不同版本混成一次见证。

工作分支始终为 `claude/dazzling-cori-q0dnyt`。先查当前 HEAD/远端/open PR/相交路径：原 pilot 和 core 必须与 a1e7c2277705 中相应文件逐字相同；新增 recheck 文件不能覆盖或改变它们。若当前头是合法续做提交，核祖先与路径，不能要求头永远停在旧 pilot。主线分支只读，不 merge 主线或 time 到执行分支。

唯一新写区是 `core/recheck-01/`（完整前缀见 YAML）。可以保存修订后的 fixtures/ 驱动和辅助代码，必要时复制旧模板并逐文件说明是否修改；也可只读复用旧文件，但所有依赖必须绑定冻结对象并可重建。不要依赖 scratch 中未交的秘密副本。这里明确许可辅助文本，不局限把全部代码塞进三个 Markdown 围栏。

试验一律在新建的独立临时目录中完成，记录真实路径；拒绝非空的既有工作目录，不使用文档中的占位路径直接执行 rm -rf，不清理用户文件或旧试验输出。允许进程内部处理 Git 对象，但公开文件/复制日志不写完整提交标识；SHA-256 仍用四段十六字符表示。缺工具就记录，不安装。

禁止修改内核、原脚本、公约/协议、旧任务、主线文件、.github/.claude/CLAUDE.md、已有工作树配置；禁止完整内核构建、QEMU、真实磁盘/固件、提权、凭据输出、合并/删除/force-push。只使用既有授权连接，不读其他私仓或十八段旧聊天。

## 3. 修复要求：保证“没有证据”不能被认作“符合预测”

### R01：先判运行与事件完整性，再判行为

先实际调用冻结旧版 `evaluate.v05({"cases": {}})`，保存真实返回。主线预测为 OBSERVED_AS_PREDICTED，属于空观测假成功；这只是待复现推导，若结果不同，保留真实程序与输入并解释调用链。

然后核对 v05 对五种固定 label 的覆盖与每种 mask_case/q 事件关系；不能只遍历“有多少就验多少”。对其他 evaluator 采用与其案例相称的完整性检查：必需字段/类型、事件集合及顺序、重复或冲突事件、案例运行状态、超时/收集失败、退出码和重复运行差异。不要求引入通用验证框架或依赖。

先分开 `NOT_RUN / BLOCKED / ERROR / INCOMPLETE_EVIDENCE` 等执行/证据状态，再对有效观察给 `OBSERVED_AS_PREDICTED / COUNTEREVIDENCE / NO_FAILURE_IN_SCOPE`。这是细化基础设施状态，不改旧内核预测。非零退出不一律失败：42/43 是有明确事件和前提的受控停止，应与相应用例合同一起校验；没有必要事件时不能仅凭该退出码接受。

### R02：准入和安全必须实际阻止不安全继续

入口先完成可信输入/只读保护/有效授权核对，失败或解析异常时动态夹具调用次数必须为零。H00 中影响动态安全的失败/超时必须阻止相应动态执行；独立且来源有效的只读检查可以继续。身份/授权失败时不擅自继续整个任务。

单案例异常不能吞掉后续独立安全案例，但也不能被转换为内核反证。总体至少分开 `execution_complete`、`verification_findings` 与 `infrastructure_errors`；约定并记录退出码。程序成功完成检查但发现旧 A46 或内核预测问题时，可以正常完成并报告 finding；不要求将“发现问题”伪装成基础设施异常。

进程组收尾必须有界；快速退出、组已消失、收集再次超时均保留准确状态，不能标没有确认过的“已回收”。只终止本试验创建的进程/组，不触及旁观进程。不为这种小型加固引入容器服务或系统安装。

### R03：结构化结果必须由本次证据决定

去掉 V00/V01/V08、host 执行状态及 CA 汇总中会掩盖证据变化的固定成功/支持状态。固定说明可保留为明确标注的解释模板；不能作为程序计算结果。语义裁定须另外列判据与实际观察引用，缺少依赖或有相反观察时不能自动保留旧 SUPPORTED。

生成器必须处理缺阶段、blocked、异常、部分执行，不因缺键在生成报告前崩溃。核对报告/YAML/CA 汇总和实际覆盖一致；顶层条目“处理过”与真实上下文切换/ELF 等层次的 NOT_RUN 分列。保留旧异常发现和新结果差异，不把目标数当成必须凑齐的成功数。

### R04：可重复执行不依赖远端 HEAD 永远不动

分离以下对象：冻结源码/任务文件、被冻结的 pilot/core 原件、本次执行分支当前头、本次回读目标提交及 blob。合理续提交与旧原件不变可以成立；错误分支、非祖先、旧原件被改或语义输入漂移不能被接受。元测试可注入小型本地结构或函数返回；不得为测试错误分支真的创建/改写受保护远端。

明确支持“当前分支已前进，但读取指定冻结对象”的核验；回读完整比较 curl 成功、无超时、HTTP 200、目标对象与返回字节、解析字段。修正 readback.py 的 --out 参数解析，给出带/不带可选 field=value 的可执行入口。试跑不重做；所需负控纳入本次 M 项。

## 4. M01–M12：同一轮完成的有限验收矩阵

每行可以含少量变体，但不得把行数当作所有子例数。先在旧版完整相关入口演示失败，再测试修订版；对无旧入口/原本已正确的控制说明 not_applicable/control_met，不为了“红→绿”制造旧失败。所有变造输入标为 HARNESS_META_TEST，不是 MyOS2 原函数结果。

| ID | 输入与检查 | 修订版必须满足 |
|---|---|---|
| M01 | 冻结旧 v05 的 cases 缺失/空 events；再经实际调度 evaluator 的入口 | 不存在有效观察时，不能返回符合预测或无失败；旧版真实返回原样保存。 |
| M02 | 从已交 V05 真正的五组事件中去掉一组或截断关联 q 事件 | 检出缺失 label/关联，不能只验剩余部分即成功。 |
| M03 | 重复冲突事件、错误必需字段类型、无法解析事件 | 不由 one/字典覆盖静默选一个；给明确无效证据状态。 |
| M04 | 保留看似正确的输出，但运行标 timed_out、collect_timed_out、ERROR 或重复结果不一致 | 不能作为可靠行为证据接受；异常与内核反例分开。 |
| M05 | 有效记录配错误退出码；另保留合法 0、守卫42、步数43 正控 | 退出码与事件/案例合同一起核对，不全吞非零也不全拒受控停止。 |
| M06 | 入口 identity 返回 gate=false 或抛异常；用可计数的安全替身见证是否调用动态入口 | 动态调用数为零，返回准确阻断/错误记录。不要真的越权运行。 |
| M07 | H00 动态安全探针失败/超时，同时保留一个独立静态检查 | 不执行受影响动态夹具；静态项按有效来源及权限继续，部分报告可生成。 |
| M08 | 从真实 V01 记录派生“引用路径不存在/解析失败”；V00 改变一个机械发现 | YAML/正文/CA 不保留写死的 NO_FAILURE 或旧固定结论。 |
| M09 | 缺一个阶段、一个夹具编译失败、一个案例异常；再给完整正控 | 不崩溃丢整份报告；完整列出未执行层次、受影响 CA 与仍有效项。 |
| M10 | 合法 HEAD 前进且旧原件不变；对照错误分支/非祖先/旧原件变化 | 前者可重放，后三者拒绝或按影响阻断；不移除身份约束。 |
| M11 | readback CLI 有/无 field=value 搭配 --out；成功读取与故意不存在路径 | 参数正确解析；HTTP/超时/对象/字节/字段全纳入状态；保存真实非敏感输出。 |
| M12 | 完整冻结旧源码、原预测、完整有效数据的一次同源 V00–V14；另核主线 A46-C/ASM 两条 | 仍可产生真实 scoped 结论；旧47条中的 A46 仍失败，新两条单列，不偷改原文使旧测全绿。 |

M12 每个原宿主案例沿用既有上限并可重复一次；不反复做多次全树扫描。单个编译最多60秒、单次夹具最多5秒、有界序列最多100步；元负例另用更小限时。驱动错误修复后一次完整复跑，只有失败导致结果不可用时才重跑受影响部分。

保持旧源码抽取方式可核对，不修改任何待测内核算法，不先修错再称旧代码正确。针对原 invalid-container 分支，仍须在真实危险解引用前由守卫停止，并准确描述守卫前已经执行了什么，不声称消除了所有 C 未定义行为。V14 仍是源文/模型层，无可信 ELF 不升级。

## 5. GitHub 固定交付与收尾

只新增到本次 recheck-01/：

- `MANIFEST.md`：R01–R04 处理、M01–M12 与 V00–V14 覆盖、输入/模型归属、文件清单、限制；含最短可直接执行的重建命令及参数。不要只写“全部完成”。
- `results.yaml`：程序生成的真实分层结果、与冻结旧 core 的差异、每项观察和证据引用。旧顶层15/11/2/2不得硬填。
- `evidence.md`：旧反例/新负测/复跑与回传说明，附必要命令、输出、失败记录及变换说明。
- `fixtures/` 与 `observations/`：实际修订代码、元测试、可重建入口及本轮原始结构化运行输出。只保留复核必要材料，不交二进制、磁盘、整仓副本、凭据或整段聊天。原始记录如有脱敏/换行变换须声明；不虚构前轮缺失的原始日志。

不要要求 Owner 转发最终整段回复，也不另造 complete-response/ZIP。最终回复只是同一个 PR #17 的导航，主线从 GitHub 读实际成果。

所有说明文档有 YAML 头；代码采用不破坏语言语法的元数据注释，记录实际执行者/未知模型、来源与本次变更。不要抄主线 produced_by；保留主线 model_per_owner 时明确 originating_lead_only_not_executor，不认证 Claude 后端。

推送前核对 PR 当前 master 的差异范围、pilot/旧 core 原件未改、路径白名单、无完整提交标识/凭据/二进制、数据与报告一致。结果先冻结提交，再从远端读回目标对象；回读回执另文件记录以免自引用。若远端头又前进，只能接受能解释的同线后继并绑定实际对象，不能读到别版后称相同。

更新同一个 Draft PR #17，标题可注明 `core verification / recheck-01`，正文以最新状态和新入口为先，保留旧产物身份及发现。不要合并、删除或把 PR16 当自己的写入目标。执行分支保留至本次回收审查完成并明确解除。

## 6. 主线检查与停止条件

主线优先检查：冻结旧 v05 的真实负例；是否假准入/安全失败真正阻止动态调用；报告是否接受空/坏/失败证据；旧原件保全及新结果与实际输出对应。之后才看全部原案例的变化，不以“测试数更多”替代正确对象与正确判据。

本次主线回执并未运行这些负例，执行者必须验证其推导。发现反证，给精确输入、原程序结果和理由；不为迎合主线改输出。缺工具、无法保护原件、输入冲突或写入越界时停止受影响动作；能独立继续的安全项同轮做完。

验收上限仍为 PASS_PENDING_LOCAL；真正内核执行、真实 ELF、IRQ、SMP、完整正确性均不由此放行。机械修订不要求新增 ChatGPT Pro/Deep Research；仍用原 Claude 云执行会话，新的架构/语义冲突返回主线。下一步仓库写入：是，独立 recheck-01 子树与同一 PR17；主线另在 lead 目录审查，双方提交前核对相交路径。
