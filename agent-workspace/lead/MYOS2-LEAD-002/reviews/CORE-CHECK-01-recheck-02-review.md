---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-CHECK-01-RECHECK-02-REVIEW-001
record_type: scoped_consumer_recheck_closeout
conversation_display_name: "MYOS2-A-C02 内核分析主线"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-10-02
base_snapshot: "workspace=master；execution=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002（分支名）"
read_channel: connector
evidence_class: "远端代码、实验记录与提交差异的可读复核；不是主线重新执行"
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-CHECK-01-RECHECK-02
reviewed_pr: 17
reviewed_branch: claude/dazzling-cori-q0dnyt
reviewed_commit_short12: 0d62c4d19711
results_frozen_short12: 77faf51e9a43
previous_input_short12: b843d475367a
previous_results_short12: efb9846b88ec
supersedes: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-recheck-01-review.md
supersedes_scope: "关闭该回执的 S01-S03 定点退回；保留全部旧原件和先前已关闭的 R02/R04。"
status: BOUNDED_RECHECK_CLOSED
acceptance_verdict: PASS_PENDING_LOCAL
acceptance_scope: "13号合同的读取层修订、N01-N06、所交冻结观测的有限范围消费；不含内核验收。"
remaining_return_items_in_contract13: []
consumer_use: FROZEN_SCHEMA_AND_SCOPED_REVIEW
unattended_kernel_acceptance_authorized: false
kernel_acceptance_verdict: NOT_ISSUED
reviewer_execution: false
reviewer_hash_or_parser_run: false
executor_model: unknown_or_not_attestable
local_validation: "真实 ELF/IRQ/SMP/上下文切换仍待执行面；不要求把本次同一批 Python 检查再搬回本机重复执行。"
pr17_merge_recommendation: READY_AS_SCOPED_RECORDS_AT_REVIEWED_HEAD
branch_release: false
branch_retention_reason: "后续限定调度实验及复现仍使用当前执行分支；合并不授权删除。"
inputs_read:
  - "0d62c4d19711: consumer-fix-02/MANIFEST.md、evidence.md 全文，截断部分已补读。"
  - "consumer-fix-02/fixtures: consumer3.py、make_results3.py、consumer_tests.py、common3.py、frozen_b843.py、guard3.py、old_side.py、expected_objects3.py、make_summary3.py、final_check3.py 全文。"
  - "consumer-fix-02/fixtures/expected_objects.json 全文。"
  - "consumer-fix-02/observations/consumer_tests.json: N01开头、N05末段、N06及S04/汇总；old_side_s01_s03.json: S02尾部及S03；n06_revalidation.yaml: 头部/当前消费字段。"
  - "consumer-fix-02/results.yaml: 末段复判、CA、计数、差异和汇总；observations/after_results/readback_results.object.json 全文。"
  - "PR16/17元数据；b843到0d62、77fa到0d62的远端比较；执行头的combined statuses及PR触发workflow查询。"
  - "13号合同和前次回执沿用本会话已全文读取内容，主线写前仍在b0aa54db1b70。"
self_check: {scope: this_file_only, verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
open_questions:
  - "CA-02全局可达性、CORE-SCHED-ORDER-02排序候选及真实内核运行层继续开放，不属于本次读取器退回项。"
  - "未保存的第二次stdout不能补造；将来新的实验直接保存两次完整运行记录。"
---

# 读取层补修可以收口；下一步回到内核机制本身

**本轮按13号合同接收 RECHECK-02，结论为 PASS_PENDING_LOCAL。S01-S03已在限定范围内闭合，不再要求第三轮通用验证器加固。** PR17 的被审头可以作为包含历史失败、修订与限定实验的记录合入；这不是内核功能通过，也不是全套第二波研究完成。

当前源运行已经有可消费的受限反例。下一项建议是对14号文件中的调度回插与计费时点做小规模宿主实验，不是继续修改报告格式。其独立任务书为 `15-scheduler-ordering-witness-contract.md`；执行仍需 Owner 在原云会话转发，不把本回执或PR评论当成外部会话已经启动。

## 1. 为什么本次可以结束返工

以下“实跑”指执行方交付了相应程序、返回值和记录；主线没有运行命令。

| 项 | 实际检查到的实现及记录 | 本轮处置 |
|---|---|---|
| S01 缺运行字段默认成功 | consumer3的REQ_CASE/REQ_RUN核字段存在、精确类型、output_lost与重复终态；包装原validate_case追加错误并清除索引。N01逐键/组合删除变成INCOMPLETE_EVIDENCE；完整正控仍有效，显式失败仍拒绝。 | 关闭13号合同中的该项。 |
| S02 检查对象被漏空 | expected_objects3从冻结07/map/MANIFEST取预期集合，不从待验空列表反推；v01_strict核成员身份、集合、重复和所用类型。N02漏项/空集被拒绝，完整清单的空错误列表保持合法，exists=False保留为真实发现。 | 关闭该项，不扩展为任意schema形式化证明。 |
| S03 缓存遮蔽原件缺失、坏JSON导致整包丢失 | make_results3完整CLI经read_json和当前stage可用性重算；原run.evaluation只用于差异展示。N03缺static/v01、N04语法错/空文件/错误顶层均形成部分报告且退出2；原件缺失影响沿依赖传播，不丢无关结果；导入的冻结程序被改时退出4。 | 关闭该项。源执行COMPLETE与现在可复核性分列。 |
| N05 相关回归 | 旧缺label/重复/超时/退出码错仍拒绝；正常0、受控42、步数界43正控可用；真正的CLI覆盖M08/M09与N02变换。 | 接受这些具体回归，不声称所有输入分支穷尽。 |
| N06 既有观测复判 | 七个输入可读；重新判定15项、A46更正、CA、计数、R/M，与efb冻结结果对应项一致；源运行日期和此次消费日期分开。 | 接收为RECORDED_OBSERVATION_REVALIDATION，不算新增一次内核/宿主实跑。 |

旧側 S01-S03 的实测记录与前轮主线推导一致。相关原始记录的抽样与evidence逐项表相符，审查并非只读取all_met；同时也没有对全部大JSON逐字节重新核验。

## 2. 接受S04：反例不能被负控逻辑吞掉

执行者在N02/N05发现：有效观测恰好等于旧负控错误值时，旧judge返回COMPARATOR_INVALID，例如V04 ret=1和V01 hex40命中数1。新包装在该错误值确实不同于预测值时，将其判为VALID/COUNTEREVIDENCE；负控本身与预测同值的无意义情形仍不放行。两组旧/新返回在consumer_tests原始记录尾部可见。

这正符合13号合同“有效且与预测不同的项也应被接受”的正控要求，不属于执行者越界改内核预测。它修的是判定器，不证明真实MyOS2的唤醒返回值已变为1；测试用的ret=1是明示变造的HARNESS_META_TEST。

## 3. 主线直接处理的保留意见

**run.json缺失时连带保留静态项不完整，是本包可接受的保守选择。** 此格式把阶段完成记录放在run.json中；不必为此再让Owner选择另一种格式或要求补做独立阶段认证。其他项目格式不能自动继承该决定。

**第二次stdout缺失不是新数据。** 当前只接受已有第一次原始输出，以及执行方保存的重复一致摘要、第二次退出码/终态；不宣称独立重算了重复输出相同。下一批新实验保存两次完整记录即可，不为补造历史重跑整包。

**预期对象集合沿用producer的选择范围。** 它解决消费者漏对象，并不证明原producer扫描覆盖全仓；尤其不恢复已经撤回的swait无调用者断言。msleep仍只保留原文本扫描边界。

**旧错误原件必须留存。** V00的旧A46仍是失败记录，正确的A46-C/ASM另列。未执行的ELF、IRQ、SMP、计时器到期和真实上下文切换不升级；CA-02仍分函数层、有限idle模型和未知全局可达性。V08排序候选也未因读取器复判而被动态验证。

## 4. 原件、传输和PR处置

连接器比较显示：b843到0d62共26件新增，全部在consumer-fix-02；77fa到0d62只有六件新增的文档/回读文件，冻结results.yaml没有变化。PR17累计84个新增文件，未改内核；和PR16主线写区不相交。object回读记录包含两个通道HTTP/退出码/字节一致字段；主线未自行计算哈希。

接收执行者披露的缓存文件越界事件：一次只读查看生成了写区外pyc，保护门关门；删除本会话所生缓存后重过。它没有被包装成从未出错，远端净差异也没有该缓存。此事不推导为任意本地文件已被安全检查。

当前查询未返回该执行头的combined statuses或PR触发workflow记录；不把“无冲突”写成“CI通过”。执行者模型依然unknown_or_not_attestable，不用主线gpt6署名替换它，也不把Owner合并视为全文审阅。

**合入条件按被审头固定，PR17可解除本轮质量退回；分支保留要求不解除。** 可将其标为Ready以保存已审记录，主线不执行合并。若Owner先转发下一项实验，执行者在同一open PR转回Draft再新增新结果；若Owner已合并，执行者只在确认旧内容已进入master后，为下一项新结果开唯一的新Draft PR。两条路径均不要求Owner先合并，也不得同时开竞争PR。

## 5. 覆盖和后续边界

本轮全文审查十个程序及expected_objects.json、MANIFEST/evidence。build_evidence3.py没有全文审查；其生成的evidence已读。大JSON和results采用上述定点读取，未进行主线解析、哈希或测试，不宣称独立验证了所有26件的每个字节。源数据的可信度仍受执行方记录与宿主夹具保真范围限制。

下一项只验证已提出的两个调度候选及必要对照，保留idle和合法节点，不能用删掉系统前提制造故障。继续禁止直接改内核、QEMU、真实盘操作和未授权阶段3。原核验包返工已收口；后续具体内核反例和学习实验另行成文，不借此建立无限扩展的测试平台。

来源入口（均为本轮已读版本）：
- [本次清单](https://github.com/08822407d/MyOS2/blob/0d62c4d19711/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/consumer-fix-02/MANIFEST.md)
- [读取程序](https://github.com/08822407d/MyOS2/blob/0d62c4d19711/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/consumer-fix-02/fixtures/consumer3.py)
- [完整入口测试](https://github.com/08822407d/MyOS2/blob/0d62c4d19711/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/consumer-fix-02/fixtures/consumer_tests.py)
- [执行证据](https://github.com/08822407d/MyOS2/blob/0d62c4d19711/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/recheck-01/consumer-fix-02/evidence.md)
