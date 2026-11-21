---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: bounded_evidence_consumer_recheck_taskbook
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-CHECK-01-RECHECK-02
phase: evidence_consumer_recheck
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-10-01
base_snapshot: "workspace=master；executor=claude/dazzling-cori-q0dnyt；instructions=agent/MYOS2-LEAD-002（分支名）"
evidence_class: "实际程序上的静态负例和有界执行合同；主线未执行"
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-recheck-01-review.md
  - "b843d475367a 的 recheck-01 中 run_recheck/evaluate2/make_results2/identity2/harness2/h00_2/build2/readback2/meta_tests/old_counterexamples/frozen/compare_runs2 全文"
  - "recheck-01/MANIFEST.md、evidence.md 与审查回执列明的观测区段"
status: READY_FOR_OWNER_LAUNCH
execution_disposition: RUN_WHEN_OWNER_FORWARDS
external_execution_started: false
required_review_record: CORE-CHECK-01-RECHECK-01-REVIEW-001
execution_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
reviewed_input_short12: b843d475367a
frozen_results_short12: efb9846b88ec
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/consumer-fix-02/
old_files_must_remain_unchanged: true
kernel_fixture_execution_authorized: false
full_tree_scan_authorized: false
acceptance_ceiling: PASS_PENDING_LOCAL
local_validation: "待云端执行；主线未运行本任务中的任何负例。"
supersedes: agent-workspace/lead/MYOS2-LEAD-002/12-core-check-recheck-contract.md
supersedes_scope: "仅本次后续：修 S01-S03 的数据读取层并复判已交观测；不重复原 M12 内核片段实跑，不改变原问题和源码"
open_questions:
  - "S01-S03 的静态推导必须先用冻结程序实际验证；反证不成立就如实返回。"
  - "原内核候选的全局可达性与真实运行层不在本次执行范围。"
---

# 只修数据读取缺口，不再重跑内核实验

**本次目标：读取已经归档的结果时，缺字段、漏对象、缺文件或坏 JSON 不能被悄悄当成有效证据；其他仍完整的观察继续保留。** 固定输入是 PR17 的 b843d475367a。原 R02/R04 已在审查范围内获得支持，本任务不重做 pilot/H00、C 编译、内核片段实跑或全树扫描，也不设计通用验证框架。

13 是主线文件序号，不是取用预留/后续 DR 研究任务号。仍用同一执行任务、同一分支和 PR。

## 1. Owner 一次操作

在创建 PR17 的原 Claude Code Cloud 会话，沿用现有模型、effort、费用和仓库授权。无需合并任何 PR，无需上传下载报告，发送：

```text
执行 MYOS2-LEAD-002-CORE-CHECK-01 的 CORE-CHECK-01-RECHECK-02。
先完整读取：
https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/13-core-evidence-consumer-recheck.md
再读其指定的 recheck-01 审查回执。
在原云会话、claude/dazzling-cori-q0dnyt 分支、Draft PR #17 内续做。
先用冻结 b843d475367a 的程序和已交观测实跑 S01-S03，随后修数据读取与完整性传播，同轮完成 N01-N06。
这次只做数据层检查和既有观测复判；不重新编译/运行 C 夹具，不重做 H00、全树扫描或研究，不改内核。
仅在任务书指定的 consumer-fix-02/ 子目录新增；b843 中的所有旧文件保持不变。
保留反证、错误输入与实际输出，结果直接推回同一个 PR；不合并、不删分支，不让我搬运文件。
最后只返回 PR 链接、提交短标识和简短结果。
```

完成后在 MyOS2 主线回复“PR17 已更新”即可。原会话不能恢复、不能推回原分支或出现不明并行修改时，保留已有工作并报告具体阻断，不开竞争 PR、不更换付费产品、不索取凭据。发布任务书/PR 评论不自动启动会话；Owner 转发本段才启动本次外部执行。

## 2. 固定输入与保护边界

按顺序读本文件 → 主线同目录 `reviews/CORE-CHECK-01-recheck-01-review.md` → b843 的 recheck-01/MANIFEST.md 和实际 S01-S03 对应程序/观测。前次 12 号合同只按需读，不重新执行其中全部步骤。

核对回执：record_id 为 CORE-CHECK-01-RECHECK-01-REVIEW-001，packet、PR17、执行分支、b843d475367a 与 efb9846b88ec 身份相符，followup 为本任务。启动时从主线分支读取一次并固定所读对象；新的主线回执和检查点是预期追加，不能要求主线永远停在 ec62453e76d2。

固定数据根：`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/`。程序与 observations 均从 b843d475367a 取出，不以工作树旧临时文件作基准。原 results.yaml 在 efb9846b88ec 冻结，b843 只追加文档/回读材料；先验证此关系。

新写区仅为 YAML 中的 `allowed_write_prefix`。选择 recheck-01 下的子目录，是为了让原执行分支/原件保护逻辑仍可复用，不是放开整个 recheck-01 的修改权限。**b843 中所有既有文件均冻结**，包括 recheck-01 的程序、JSON、MANIFEST、evidence 和 results；本轮修订以新文件实现，需要复用旧模块时从固定对象读取并列出依赖。

运行前及推送前核对 HEAD/远端后继关系、原件逐字节不变、相交路径、唯一 PR；只允许可解释的同线后继。原 identity2 可以用作原 core/pilot 保护的辅助，但它只保护到自己的范围，不能替代本轮对全部 b843 原件的额外保护，也不能代替新任务绑定。

只允许 Python 数据层实验、必要 Git 只读查询、结果分支提交推送/PR 更新和远端回读。不运行原仓库脚本、不重新调用旧 run_all/run_recheck/meta_tests 的整批主入口（它们会重新运行 C 夹具），不跑 C 编译、内核构建、QEMU、H00 进程实验、全树检索、真实磁盘/固件操作，不安装工具、不提权、不输出凭据。源码仍固定 time a039d9803ade；读取任务所需少量冻结元数据不等于重新扫描源码。

所有变造样本只写到新建的隔离临时目录；不删除/清空旧目录，拒绝用不明或非空路径作输出根。允许复制必要 JSON 作为元测试输入，逐一记录变换；不能把变造输入或历史记录的再次判读称为新的 MyOS2 执行。

## 3. 先复现三个精确问题，再做最小修订

下面全部是主线静态推导，执行者必须先用未改的 b843 程序验证，保存原返回/退出码/输出。若不成立，交实际反证，不修改输入迎合主线。

### S01：必需终态信息被默认补成成功

从本次实际 `observations/run.json` 复制 fixtures 记录，选 `fx_sched/v04_noncurrent_wake`；删除其 run 中 `collect_timed_out`、`terminal_state`、`reaped_confirmed`、`stderr`，其他字段不改。调用冻结的 evaluate2.v04。主线预测仍为 VALID/OBSERVED_AS_PREDICTED。

修订要求：对本格式明确需要的字段先核存在性、类型与取值；缺键不能默认成 exited/True/无错误，不能从退出码自行补齐未记录的证据。显式失败、缺失、完整但与原预测不同须有不同处理。检查 producer 已经保存、consumer 会使用的字段，不通过新造一套庞大通用格式扩大范围。旧摘要不能补出它没有的第二次原始输出；历史正控以实际保存范围为限，必要的合成元测试字段必须明示。

### S02：检查对象清单漏空，空集合仍全真

复制实际 v01.json，分别以及合并清空 `referenced_paths` 为 `{}`、`report_inputs_read` 为 `[]`、`hex40_hits_in_scope_files` 为 `{}`，通过冻结 evaluate2.v01。主线预测仍 VALID/NO_FAILURE_IN_SCOPE。

修订要求：对这些实际应检查的对象集合核完整性、唯一性、成员身份及每项结果类型。预期集合来自冻结 07/map/MANIFEST 等输入，可一次提取固定使用；不能从被验证的空清单自己推导“预期也是零项”。不必重新扫描整个内核或重新产生所有文件哈希。**完整对象清单下的空错误列表是合法正控**，不得用一律拒绝空集合修复。

### S03：生成器的完整读取入口仍可用缓存覆盖缺证据

在隔离目录复制当前 observations，但省略 static.json，保留 complete run.json，通过冻结 make_results2.py 的真实 CLI 生成报告。主线预测：V14 源码层缺证据，而 V08 仍沿用缓存的 VALID 与限定模型判定。再从完整复制中把 static.json 改成只有 `{`，走相同 CLI；主线预测在 section 错误收集之前抛 JSONDecodeError，没有部分报告。

修订要求：完整的“文件读取 → schema/覆盖检查 → 各层消费状态 → YAML/报告”链要按当前实际可读原件工作。不能仅修改测试内存中的 run.evaluation 后声称完整入口已修。源运行当时为 COMPLETE 可以作为历史事实保存，但当前消费结果必须单独反映缺失/损坏；不得把依赖缺失的缓存 VALID 或旧 SUPPORTED 作为当前结论。动态原始观察在 run.json 内，静态原始材料在各 stage JSON 内，按实际依赖传播；能独立保留的函数层结果继续保留。

读取错误要有文件名、错误类别、受影响项，并能输出其余独立结果。语法错、空文件、错误顶层类型不能只在最外层崩溃丢掉整批，也不能默认为空成功。明确约定 CLI 非零失败/部分状态，区分验证发现与读取器自身失败。

## 4. 同轮完成 N01-N06，不重新运行内核

| ID | 本次数据层用例 | 必须观察到 |
|---|---|---|
| N01 | S01 四个键分别删除及组合删除；保留完整真实正控；显式失败值继续拒绝 | 缺失不能得到 VALID 或行为结论；有错误类别；完整正控仍有效。 |
| N02 | S02 三种对象集合分别漏一项、清空及组合清空；完整对象下保留空错误列表；某成员明确 exists=False 的对照 | 漏对象是缺证据而非“无失败”；合法零错误仍可用；真实完成检查得到 False 不得混成读取失败。 |
| N03 | 完整生成入口：保留 complete run.json，分别省略 static.json 与 v01.json；检查缓存、V/CA/层次和顶部消费状态 | 不借旧缓存掩盖当前缺原件；V08 缺 static 时保留可独立支撑的函数层而不保留限定模型有效；不影响不依赖缺件的项。 |
| N04 | 完整 CLI：static/v01 至少各一项坏 JSON、空文件或错误顶层类型；run.json 不可解析的控制 | 如实形成错误/部分报告，退出语义匹配；不能给受影响项行为裁定；无 run 原始数据时不冒称动态证据可用。 |
| N05 | 在已有 M01-M05/M08/M09 变造数据基础上做关联回归；包含有效且与原预测不同的一项；通过真正的读取/成文入口验证 N02/N03 变换 | 不能通过一律拒绝数据规避问题；旧缺 label/重复/超时/错误退出仍拒绝，合法 0/42/43 仍按各自事件合同消费；不调用会运行 C 的旧整批入口。 |
| N06 | 从 b843 的全部已交观测重新判读，覆盖原 V00-V14、A46 更正和 CA，并与 efb 冻结结果比较 | 数据完整时原受限观察保留；差异逐项说明。原 A46 失败保留，两个新锚点分列，未执行层不升级；不称这是新一次宿主实跑。 |

N05 的元测试若沿用已有脚本中的辅助函数，先读其入口，确认不会触发 C/H00/全树扫描；更简单的做法是复制其必要变造逻辑并标注来源。每个旧侧/新侧 Python 实验有有限超时，建议单例 30 秒；不为纯数据检查新引入进程管理框架或多 agent 群。单项问题不丢失其他独立结果；失败必须进入报告。

N06 只叫 `RECORDED_OBSERVATION_REVALIDATION`。保留 source_execution 的原身份/日期、此次 consumer 检查的身份/日期；不改原 run.json，不把反例数据回填到真正观测文件，不改原内核算法或预测。没有新证据时，CA-02 全局可达性、V08 排序候选、真实 ELF/IRQ/SMP/上下文切换继续开放。

## 5. 小型交付与验收范围

只在 consumer-fix-02/ 新增：

- `MANIFEST.md`：S01-S03 的旧侧实际结果、新处理、N01-N06 覆盖、实际文件清单和最短重建命令，限制在前；不再只有“均完成”。
- `results.yaml`：旧负例、新负测、归档观测复判/差异、输入身份、消费者有效性与源运行事实分列，结果来自实际记录。
- `evidence.md`：必要命令、输出、变造说明、负例及反证、未完成项；附对上一轮“无调用者”说法的撤回导航，不重新扫描来凑结论。
- 必需的 `fixtures/` 修订模块/测试与 `observations/` 元测试记录。只交复核必要内容；不复制整仓、所有 C 模板或旧大日志，不交二进制/ZIP/对话转录。复用文件注明 b843 的具体路径并校验身份。

文档 YAML 头写真实执行面、实际可知模型或 unknown_or_not_attestable，代码用不破坏语法的元数据注释；不要抄主线 produced_by 冒称执行者。来源提交只写实际复制的短号，所有公开文本不出现完整提交号；摘要用分段形式。新修订必须能从 GitHub 中的文件和明确冻结依赖重建，不依赖已经消失的 scratch 内容。

推送前对照 b843：全部既有文件不变，新增仅在本次前缀；没有 kernel/public rules/其他项目改动。结果文件先提交冻结；object/branch 回读绑定本次真实对象与 blob，回读记录另行追加，避免自引用。更新同一个 Draft PR17，正文先指向最新 consumer-fix-02，旧版本保留为历史。

原 R02/R04 在已审范围内关闭，不为了本任务重跑环境或修改那条已成立的执行链；若新消费者入口需要调用旧模块，仍按现有准入与保护边界，不绕过它。主线将只核 S01-S03、相关回归、归档数据消费完整性与原件保护；不会把本任务升级为任意损坏/任意恶意输入的通用形式化证明。

PR17 暂不合并；执行分支保留到本次回收审查完成并明确解除。主线仍写 agent/MYOS2-LEAD-002/PR16，执行者仍写原分支/PR17。完成后只返回同一 PR 链接、短提交和简短结果；不要求 Owner 下载上传或逐项审稿。验收上限仍为 PASS_PENDING_LOCAL；本任务不授权修内核、进入正式阶段3或代替 Owner 选择学习路线。
