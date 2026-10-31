---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-CHECK-01-RECHECK-01-REVIEW-001
record_type: targeted_recheck_review
conversation_display_name: "MYOS2-A-C02 内核分析主线"
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-10-01
base_snapshot: "workspace=master；execution=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002（分支名）"
read_channel: connector
evidence_class: "远端程序/记录的可读审查；新负例是静态推导，尚未由本主线执行"
status: TARGETED_REVIEW_COMPLETE_CONSUMER_FIX_READY
disposition: RETURN
return_scope: "R01/R03 的证据读取与完整性传播；不是重做环境、全部研究或全部内核实验"
retained_evidence: SCOPED_RECORDED_OBSERVATIONS
reusable_verifier_accepted: false
reviewed_pr: 17
reviewed_branch: claude/dazzling-cori-q0dnyt
reviewed_commit_short12: b843d475367a
results_frozen_short12: efb9846b88ec
first_batch_short12: f9ae2bce9a64
previous_core_short12: a1e7c2277705
supersedes: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-core-review.md
supersedes_scope: "接续 R01-R04 修订状态；接受 M09 第二批更正与无调用者说法撤回；不改旧原件"
inputs_read:
  - "PR17 recheck-01/MANIFEST.md、evidence.md 全文；长响应分段补读"
  - "recheck-01/fixtures 的十二个关键程序全文，详见 §6；不是全部十五个程序"
  - "recheck-01/results.yaml：文件头、运行/准入/阶段字段、差异与 R/M 摘要的指定区段；并非全文重新计数"
  - "recheck-01/observations/v01.json、rerun_vs_batch1.json 全文"
  - "observations/old_counterexamples.json 的 R01/R02/旧回读部分，meta_tests.json 的 M09/变体 B，run.json 的部分 swait/timeout 原始记录"
  - "PR16/17 元数据；a1e7c2277705 到 b843d475367a、efb9846b88ec 到 b843d475367a、主线写前状态的远端比较"
  - "12 号任务书与前次 core-review 使用本会话此前全文读取内容；本轮不宣称重新运行其中任务"
reviewer_execution: false
mechanical_hash_check_by_reviewer: false
executor_model: unknown_or_not_attestable
kernel_acceptance_verdict: NOT_ISSUED
acceptance_ceiling: PASS_PENDING_LOCAL
local_validation: "待执行面；本主线没有运行命令、解析器、测试或新负例。"
followup_id: CORE-CHECK-01-RECHECK-02
followup_taskbook: agent-workspace/lead/MYOS2-LEAD-002/13-core-evidence-consumer-recheck.md
followup_allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/consumer-fix-02/
self_check: {scope: this_file_only, verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
open_questions:
  - "S01-S03 的精确旧程序返回待执行面实跑；若有反证必须保留。"
  - "CA-02 全局可达性、V08 排序候选、真实 ELF/IRQ/SMP/上下文切换仍未闭合。"
---

# 已有执行证据保留；剩余工作缩到读取结果的程序

**R02 准入/安全门和 R04 后继版本/回读在本次检查范围内得到支持；R01/R03 的多项修订也确实起效。但结果读取层仍有明确的缺失数据假有效路径，尚不能把整套验证器作为无人复核的验收依据。** PR17 继续 Draft，不合并、不解除分支保留。

下一步仅需原 Claude 云会话按 13 号文件做一次数据层修订：先用已经入库的观测复现下面三个反例，再修读取/完整性传播并重新判读既有观测。**不重新编译或运行内核片段，不重做 pilot/H00，不重新展开九项研究。** 结果通过同一个 PR 返回；不需要 Owner 搬运文件或先合并 PR16。

## 1. 对本轮修订的逐项裁定

[VRF] 这里的“实跑”指执行者提交了相应程序和运行记录；不是本主线重跑，也不是 CI 或不可伪造执行认证。

| 原问题 | 本次已取得的有效改进 | 主线裁定 |
|---|---|---|
| R01 空/坏观测 | 原空 V05 假成功和空调度入口假 COUNTEREVIDENCE 有冻结旧程序返回；新 M01-M05 对缺案例、缺 label、重复事件、错误类型、已标明超时及退出码不符作了区分。 | 这些具体变体保留；必需运行字段缺失、静态对象清单被清空仍漏检，见 S01/S02。 |
| R02 门控失效 | 新入口在身份失败/异常时动态入口为零；H00 失败时阻止动态夹具且独立静态检查可以继续，M06/M07 有对应记录和代码。 | 在已列范围内关闭；不要求重新发射安全试跑。 |
| R03 固定裁定/部分记录 | 裁定改为按数据计算；M08 的实际不符合项进入结论；M09 编译失败/案例异常/缺静态阶段能输出部分结果。 | 已实现部分保留；生成器独立从归档读取时仍可沿用失去依据的缓存判定，坏 JSON 也会在错误收集前终止，见 S03。 |
| R04 固定旧分支头/CLI | 原件不变的后继被接受，错误分支/非祖先/原件修改被拒绝；object/branch 回读与带/不带字段参数的 CLI 有正负例。 | 在本次绑定和测试范围内关闭；不是所有未来仓库状态的证明。 |

M01-M12 所列具体变体的成功记录不撤回。下面是其中未覆盖、但仍属于 12 号合同“缺失证据不能有效、部分失败须如实成文”的路径；不是把已运行的十二组测试改判为失败，也不是新建无限扩展的测试框架。

## 2. 接受 Owner 特别指出的纠正和边界

**M09 第二批修正是实际修正。** `run_recheck` 对未完成的 static 阶段不再向 V08 交付数据，`evaluate2.v08` 在判据缺失时返回 INCOMPLETE_EVIDENCE；元测试变体 B 保留 static.json，却把阶段退出记录改为 1，V08/V14 源码层随之失去完整性。变体 B 的 H00 与夹具复用方式已明示，不当作另一次真实上下文切换。来源：`fixtures/meta_tests.py::m09`、`observations/meta_tests.json` 的 M09 与 `evidence.md` §6/§8。

**同源结果相同不等于所有文件逐字节相同。** `compare_runs2.py` 和 `rerun_vs_batch1.json` 给出 evaluation 相同、各案例 stdout/退出码相同，v00 的执行头字段改变；元数据及 results.yaml 已重新生成。比较记录分组为 11+8+2+1+1，即 23 个宿主案例；顶层仍是 15 项，不能混用两个分母或继续沿用旧摘要的 24。主线没有重新计算哈希。

**不登记“旧 run_pg 收集超时虚报已回收”为已确认缺陷。** 前次对终态准确性的要求不能倒写成已经证明存在该错误；旧实现由返回码决定 child_reaped。组已消失时异常外抛是另一条有实跑记录的问题，两者分开。

**撤回无依据的无调用者说法。** `swake_up_all_locked/finish_swait 在 swait.c 外无调用者` 不再作为本主线的现状结论或影响面缩小依据。此次交付的静态扫描程序没有覆盖这一证明，撤回不等于已经发现调用者。`msleep` 的未命中也只保留在已交 static_checks 的文本扫描范围内，不能据此作全局不可达或删除决定。

**继续开放的内核问题不因工具修订而消失。** CA-02 仍区分函数层反例、idle 重新入队的限定模型和未建立的全局可达性；V08 的回插顺序仍是后续候选。原子加减、trylock、swait/timeout 的受限输出可以继续用于分析，但不是内核正确性、真实 ELF、IRQ、SMP 或上下文切换验收。

## 3. S01：缺少运行终态字段仍可被当作有效

来源：[evaluate2.py](https://github.com/08822407d/MyOS2/blob/b843d475367a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/fixtures/evaluate2.py)，`validate_case`。

[VRF] 当前代码把缺少的 `terminal_state` 默认为 `exited`，把缺少的 `reaped_confirmed` 默认为 True；`collect_timed_out` 只有显式 True 才拒绝，缺失 `stderr` 也等同于无内容。

[INFERRED] 从本次 canonical 的真实 V04 记录复制一份，只删除其 run 内 `collect_timed_out`、`terminal_state`、`reaped_confirmed`、`stderr`，保留其余字段和事件，再调用 `evaluate2.v04`，会继续得到 VALID/OBSERVED_AS_PREDICTED。该输入尚未由主线运行。修订必须先实际复现，再让“未知”保留为未知，不默认补成成功。

这不表示此次真实 V04 缺了上述字段；问题是读取器不能拒绝缺失版本。M04 只把这些字段改成显式失败值，没有测试删除键。实际 producer 完整写出字段，不能替代 consumer 检查必需字段。

## 4. S02：清空静态检查对象清单仍可得“无失败”

来源：同一 `evaluate2.py::v01`；[实际 v01 记录](https://github.com/08822407d/MyOS2/blob/b843d475367a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/observations/v01.json)。

[INFERRED] 复制实际 v01.json，仅作以下替换，其余内容不动：

```python
record["referenced_paths"] = {}
record["report_inputs_read"] = []
record["hex40_hits_in_scope_files"] = {}
```

通过当前 `evaluate2.v01`，三处检查分别退化为 `all([])`、`all([])` 和空集合求和 0，仍得到 VALID/NO_FAILURE_IN_SCOPE。必需对象清单应与冻结输入的预期集合对应，不能只检查“实际剩下的每个元素都没有报错”。新负例仍待实跑。

注意区分两类空集合：实际应检查的路径/文件清单被清空是缺证据；完整扫描后的“错误列表为空”可以是合法结果。不能以一律拒绝空数组的方式修复。M08 把存在性改成 False 的测试有效，但没有覆盖清空或漏掉被检查对象的情况。

## 5. S03：独立生成报告时没有贯通原始输入状态与缓存判定

来源：[make_results2.py](https://github.com/08822407d/MyOS2/blob/b843d475367a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/fixtures/make_results2.py)，`generate/_load/build_doc`；[meta_tests.py](https://github.com/08822407d/MyOS2/blob/b843d475367a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/fixtures/meta_tests.py)，`m08/_doc_of/m09`。

[INFERRED] 有两个精确的完整入口负例，不需要再运行 C 程序：

1. 把本次已经完成的 observations 复制到全新临时目录，但不复制 static.json；保留原来的 run.json，调用原版 make_results2 CLI。它会把 V14 的源码层列为缺证据，却仍从 run.evaluation 取回 V08 的 VALID/COUNTEREVIDENCE 和原限定模型判定；该份消费报告已没有 static 原件支撑这一部分。源运行“当时完成”和当前交接包“足以复核”是两个事实，应分列。
2. 同样的临时复制，将 static.json 内容改成不完整 JSON，例如只有 `{`，再调用完整 CLI。`_load` 的 json.load 在 `build_doc` 的 section 错误收集之前抛异常，因此不能输出承诺的部分结果；这不是内核反证。

上述是静态推导，未由主线执行。它们没有否定 M09 修正：M09 在 producer 中改变阶段状态后再生成报告；此处改变的是成包之后 consumer 所能取得的原始材料。M08 也先手工更新 run.evaluation，再调用 build_doc，没有覆盖缓存与实际输入不同步的读入路径。

修订须从当前可读原件决定各层可消费性，缺失/损坏时不能照搬旧缓存 VALID；JSON 语法或结构错误要记录到相应文件/案例，并保留不依赖该输入的已有证据。源运行记录保持原样，不把消费检查时间冒称为新的宿主实验。

## 6. 本轮覆盖、保全与续接

完整读过的十二个关键程序：run_recheck.py、evaluate2.py、make_results2.py、identity2.py、harness2.py、h00_2.py、build2.py、readback2.py、meta_tests.py、old_counterexamples.py、frozen.py、compare_runs2.py。a46_check.py、final_check2.py、build_evidence2.py 未在本轮全文逐行审查；其相关结果按 evidence/远端差异使用，不宣称审完全部十五个脚本。

MANIFEST/evidence 全文已分段读取；大 JSON 与 results.yaml 采用前述定点读取，不宣称对全部归档字节、每次重复运行或全部 32 件内容做机械校验。本轮从 GitHub 比较确认：原 core 到 b843 只新增 recheck 子树 32 件；efb 到 b843 只新增六件文档/回读材料，results.yaml 未再改；主线写前仍在 ec62453e76d2。比较结果不替代主线自己执行哈希或测试。

下一任务见 [13-core-evidence-consumer-recheck.md](https://github.com/08822407d/MyOS2/blob/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/13-core-evidence-consumer-recheck.md)。只在 `core/recheck-01/consumer-fix-02/` 新增，冻结 b843 中全部既有文件。采用六组数据层检查和已交观测复判，不要求重编译、全树扫描或重做 R02/R04 安全试验。新的真实执行仍由 Owner 向原云会话发送一次指令启动，不把 GitHub 评论当自动运行。

主线仍在阶段 2；全量完成度、依赖关系、重要度、专项研究及后续正式学习路线未被本核验包替代。此次已经有具体可学习的受限反例，保留其价值；不把修验证器变成修内核前无限扩展的公共平台项目。当前 RETURN 仅对应 S01-S03 已指出的读取缺口，验收上限仍为 PASS_PENDING_LOCAL。
