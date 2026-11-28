---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
conversation_display_name: MYOS2-A-C11 内核分析主线续建
produced_by: GPT-6 Astra Pro（本轮系统身份）
model_per_owner: Pro独立验收并同轮安排后继
actual_backend_model: UNATTESTED
actual_reasoning_effort: UNATTESTED
date: 2026-11-26
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
read_channel: GitHub connector；自制文本/整数/符号链核验
startup_selfcheck_quote: 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。
branch_canary_quotes:
- "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
- "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
previous_mainline_design_count:
  conversation: C10
  distinct_batches: 11
evidence_class: SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / OBSERVATION / UNKNOWN
lead_before_publication_short12: d02e263970ef
execution_pin_short12: b57bd74931dc
self_check_scope: 仅本文件新增正式源码窄窗；本轮对作者191窗的独立复核另账。
mainline_design_count:
  conversation: C11
  before_release: 3
  after_successful_155_release: 4
  thresholds:
  - 8
  - 10
  reached: false
record_type: new_executor_receiver_startup_prompt
package_id: MYOS2-A-C11a-TO-C11b-155-20261126
receiver_display_name: MYOS2-A-C11b 代码分析执行
receiver_identity: MYOS2-LEAD-002-CHAT-EXEC-26
receiver_actual_state: NOT_OBSERVED
receiving_model_per_owner: GPT-6.2 Sol＋Extra／高推理；接收执行无需Pro
D00: []
status: PREPARED_NOT_RECEIVED
open_questions:
- 新方接收/规范刷新/执行待发生；不认证后台档位。
---

# 新「MYOS2-A-C11b 代码分析执行」完整启动词

本文件是随[216恢复包](00-C11a-to-C11b-recovery-package.md)保存的完整取件/执行说明。Owner实际转发本轮正式入口才表示新方收到；这里不声称已创建新对话或操作UI。新席名称与身份必须保持一致，不能冒用C11a／EXEC-25。

@GitHub 请在MyOS2辅助分析项目中，以「MYOS2-A-C11b 代码分析执行」／MYOS2-LEAD-002-CHAT-EXEC-26接收MYOS2-A-C11a-TO-C11b-155-20261126，连续完成CORE-USER-RW-POSITION-CONSUMPTION-155。Owner报告旧C11a在完成154后上下文耗尽；C11已经独立核收154 PASS_PENDING_LOCAL、D00=[]。你接新155，不重做154、不要求旧方补签、不另等开工。

1. 使用Owner指定的次一档GPT-6.2 Sol＋Extra／高推理，接收与普通执行无需Pro；正式验收交C11 Pro。列可见系统身份、实际客户端/后台/effort未知，真实到达后胜任与容量判断；不自动换档或因不可认证后台反复问Owner。命名为本标题，未操作侧栏即如实登记。
2. 取本轮PR17正式入口的真实ID及固定lead提交，完整读同提交[216包](00-C11a-to-C11b-recovery-package.md)、[217合同](../217-user-rw-position-consumption-contract.md)、[154独审](../reviews/CORE-USER-BRK-RANGE-ACCOUNTING-154-review.md)及[检查点](../checkpoints/2026-11-26-c11a-to-c11b-brk154-reviewed-rw155.md)。原任务MYOS2-LEAD-002-CORE-CHECK-01、阶段2、原exec与Draft PR17，唯一写区为user-rw-position-consumption-155六件；包是non_execution_source_transfer_artifact，业务权来自Owner/工作令19/217。
3. 必须自己完整读恢复包§3十五项目规则（00取本次发布全文含第19条，13项固定1174c16c4540，另210），§4 MA现行入口、48采用→47有效全文和45同版共用01–14/16十五条全文、最新建设检查点/模型登记；Mne README、接收命令、批准规范及命名§7按任务局部。fresh核refs，输出截断续读到尾，不能继承旧C11a或C11已读/摘要；不导入外仓业务或写权。
4. 在本批MANIFEST分开execution_handoff_receive与guidance_refresh：包ID/性质/实际固定入口与来源、当前名称/身份/任务/阶段/原分支/六件写区、真实接收日期时间或UNKNOWN、权限/缺项/下一安全步、当前胜任与容量判断；另记实际读取版本/十五规则及十五共用全文、适用范围、task_local_only/current_task_preserved，读完后才当前refresh_completed=true。不得重演R152-01的缺字段，也不替旧方补签历史记录。接收记录不另发评论或第七件。
5. 实做217 G00：lead等于本轮实际发布提交，exec=b57bd74931dc、time=a039d9803ade、直接master=e8e60cd19540；原PR状态/原题/拓扑、开放PR全分页；PR17无since到空、60标记每枚实际ID/字面唯一、负标记0，当前入口/154作者7192993361/154发令7192512724直接双读并与分页同文；154六件固定/活动/API全文和唯一父六新增，旧文件/评论冻结、155目录与EXEC-26作者0。真实未知并行写不覆盖。
6. 按217 A/B/C三专题七个有限场景连续完成：标量read/write回调直接写file.f_pos与ki_pos后的提交/保留、负返回与stream、kernel_read位置别名/计数消费。前提按固定源码和GIVEN分列，fdput_pos前分析停止不伪造ksys已返回；旧115/27只按精确选择器消费。发现普通问题进入deferred，不请求Owner逐项修复。
7. 恰六新增：MANIFEST.md、rw-position-consumption.md、facts-and-boundaries.yaml、rw-position-consumption-paths.dot、deferred-findings.yaml、evidence.md。自写有限文本/整数核验可用；不得运行MyOS2脚本/程序、预处理、编译链接、Graphviz、QEMU、实机。完整本地P9未做即NOT_RUN；新P6独立逐窗复开源，不合并旧191账。
8. 写前fresh核，以真实b57bd74931dc为唯一父、expected_sha匹配/force=false，在原exec一次提交六新增零其它改删，先三渠道全文/LF/字节/唯一父/净差异回读，再发唯一作者标记MYOS2-LEAD-002-CHAT-EXEC-26 / CORE-USER-RW-POSITION-CONSUMPTION-155 / FORMAL_DELIVERY。正式回执发表时G04=PENDING_POST_PUBLICATION、mainline_acceptance=PENDING；不修改旧作者/主线/PR元数据。
9. 发表后实际核新ID两次和全分页全文，原60＋新作者61标记各唯一/实际ID、负标记0，四refs/两PR/开放页、六件三渠道、单父六新增、旧成果和303条及本轮主线评论冻结。全部通过才回报READY_FOR_MAINLINE_REVIEW、实际G04=PASS及STOP_WRITING，不进入156。63先核效果后限次重试，真实写重试与工具恢复分账。
10. 最终依198只给一个通常3–6行原生短复制块、一个真实作者评论URL，写155实际交付/自检/G04/停写及未证限制，请C11使用Pro独审并同轮后继。C11本轮155首次成功发令后计4，8/10未达，C10历史11保留；未来交接传完整规则/MA同版全文/命名/真实断点与计数。

你没有源码修复、阶段3、合并/强推/删除、新建分支/PR、跨仓业务写或自行进入后继批的权限。没有真实阻断就按已授权范围连续做到上述终点；真正缺项只停受影响部分并保全，不冒签已读、已执行或G04。
