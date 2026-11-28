---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
conversation_display_name: MYOS2-A-C11 内核分析主线续建
produced_by: GPT-6 Astra Pro（本轮系统身份）
actual_backend_model: UNATTESTED
actual_reasoning_effort: UNATTESTED
date: '2026-11-27'
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
startup_selfcheck_quote: 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。
branch_canary_quotes:
- "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
- "\tthis_cpu = smp_processor_id();"
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
previous_mainline_design_count:
  conversation: C10
  distinct_batches: 11
model_per_owner: 本轮事务性恢复无需Pro；新席GPT-6.2 Sol＋Extra／高推理，正式独审交C11 Pro
read_channel: GitHub connector；自写文本/字节/元数据核验
lead_before_publication_short12: a74bf1dc357b
execution_pin_short12: abbf15de6fbb
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
self_check_scope: 本轮恢复不新增正式源码窗，0/0/0；原158设计READY与157作者/主线P6分别保留，不计作新席已完成。
mainline_design_count:
  conversation: C11
  first_batch: 152
  latest_batch: 158
  distinct_batches: 7
  before_reassignment: 7
  after_reassignment: 7
  amendment_increments_count: false
  thresholds:
  - 8
  - 10
  reached: false
evidence_class: Owner恢复报告 / 远端观察 / 已有设计继承 / UNKNOWN
status: PREPARED_NOT_RECEIVED
D00: []
R155_01: CLOSED_BY_156_CHECKPOINT_READ_SUPPLEMENT (retained)
receiver_actual_state: NOT_OBSERVED
old_executor_unpublished_work: UNKNOWN
followup_id: CORE-USER-LIBC-FORMAT-CAPACITY-158
package_id: MYOS2-A-C11b-TO-C11c-158-20261127
sender_display_name: MYOS2-A-C11主线代备；不是旧C11b签认
old_executor_display_name: MYOS2-A-C11b 代码分析执行
old_executor_identity: MYOS2-LEAD-002-CHAT-EXEC-26
old_executor_availability: OWNER_REPORTED_CONTEXT_EXHAUSTED
receiver_display_name: MYOS2-A-C11c 代码分析执行
receiver_identity: MYOS2-LEAD-002-CHAT-EXEC-27
receiving_model_per_owner: GPT-6.2 Sol＋Extra／高推理；接收执行无需Pro
record_type: new_executor_receiver_startup_prompt
---

# 新「MYOS2-A-C11c 代码分析执行」完整启动词

本文件随[221恢复包](00-C11b-to-C11c-recovery-package.md)保存。Owner实际把本次正式入口转入新对话，才表示新席收到；本文不声称已经创建聊天、操作侧栏、完成接收或执行。

@GitHub 请以「MYOS2-A-C11c 代码分析执行」／MYOS2-LEAD-002-CHAT-EXEC-27接收`MYOS2-A-C11b-TO-C11c-158-20261127`，继续CORE-USER-LIBC-FORMAT-CAPACITY-158。Owner报告旧C11b上下文耗尽、158失败；主线实际未见158远端交付。你接同158，不改批号、不冒用EXEC-26、不要求旧方补签，连续执行到实际G04通过后停写，不另等开工、不进入159。

1. 使用次一档GPT-6.2 Sol＋Extra／高推理，接收与普通执行无需Pro；完成交C11 Pro独审。记录Owner指定、可见系统身份与实际客户端/后台/effort未知，作当前胜任及容量判断；不自动换档，不因后台未认证重复索要已给授权。名称按本标题登记，未操作UI就如实写未操作。
2. 从本轮PR17正式改派评论取得真实ID和固定lead，完整读同提交[221恢复包](00-C11b-to-C11c-recovery-package.md)、[修订220合同](../220-user-libc-format-capacity-contract.md)、[恢复检查点](../checkpoints/2026-11-27-c11b-to-c11c-recovery-format158.md)，以及[157固定独审](https://github.com/08822407d/MyOS2/blob/a74bf1dc357b/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SIGNAL-CONTROL-CONSUMPTION-157-review.md)。157既有裁定与R155闭合保持，不重做157或倒签155。旧C11b未公开工作UNKNOWN。
3. 必须自己全文读取221§3十五项目规则：00取本次完整发布版含第20条，其余14项按固定a74bf1dc357b；自己读取221§4的MA现行入口、48采用、47有效全文、45同版01–14/16十五条共用全文与模型登记。建设检查点核当前现状/后续适用采用段并真实记范围，不要求为恢复重读无关施工史。Mne README、接收命令、人工批准规范及命名§7按任务局部读取；当前§7是L445–584。fresh核refs，截断续读到尾，不能继承C11b或C11“已读”，不导入外仓任务/写权。
4. 在158 MANIFEST分开记录`execution_handoff_receive`和`guidance_refresh`：包ID/非执行源性质、固定入口/来源/业务授权、包头显式`receiver_guidance_load`及实际加载状态、新名称/EXEC-27、真实接收时间或UNKNOWN、任务/阶段/分支/六件写区、权限、实际已读/缺项、当前胜任与容量、下一安全步；另记15项目规则、MA有效全文/共用十五条和其它实际读取版本/范围、task_local_only/current_task_preserved。未读完的加载状态保持pending，当前要求实际读完后才refresh_completed=true，不继承旧席2032行补读声明，不预签未发生步骤。不另发接收评论或第七件。
5. fresh G00：lead等于本次真实发布提交；exec=abbf15de6fbb、time=a039d9803ade、直接master=e8e60cd19540；原PR16 Open/Ready及原PR17原标题Open/Draft、原head/base、未合并，开放页至空。PR17评论无since至空，预计311条/72标记逐枚字面一次且实际ID正确，16负标记0；本次改派入口、157作者7206572285、原158发令7207878199直接双读并与分页同文。核157六件三渠道全文和2271旧非tree冻结、158目录/任意作者未占用，两time canary/直接master反验/公约原句。未知并行写保全，不覆盖。
6. 按修订220§1–8原样连续完成三相关专题、四场景、十二独立输入行；必须独立回源构建自己的分析和新P6，原设计READY不代替作者工作。分别跟踪名义容量和真实对象；首次指针边界停止，不签未到达的写/返回；C01只到write请求边界，不把整数请求超出对象推成真实读取/输出。真实链接、ABI、getcwd生产者及IO未证保持。普通发现记入deferred，不逐项索要修复决定。
7. 唯一写区为原`claude/dazzling-cori-q0dnyt`／Draft PR17下`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-capacity-158/`，恰六件：MANIFEST.md、libc-format-capacity.md、facts-and-boundaries.yaml、libc-format-capacity-paths.dot、deferred-findings.yaml、evidence.md。按220自检；P6各窗1–5连续物理行、原字节和行尾重开核，vsnprintf的CRLF不得归一化冒签。作者157的205与主线3窗另账；完整本地P9未做即NOT_RUN。
8. 写前fresh复核，以真实abbf15de6fbb为唯一父，expected_sha匹配、force=false，一提交六新增零其它改删；先核固定/活动/API六件全文、LF/字节/行尾、唯一父/compare及旧对象冻结，再发唯一作者标记`MYOS2-LEAD-002-CHAT-EXEC-27 / CORE-USER-LIBC-FORMAT-CAPACITY-158 / FORMAL_DELIVERY`。回执包含真实接收/刷新、固定合同/提交/六件、四场景十二行、D00=[]、实际P6/自检及未知；发表时actual_G04=PENDING_POST_PUBLICATION、mainline_acceptance=PENDING。
9. 发表后实际双读新ID并核全分页同文，预计312评论、72旧标记加唯一作者73逐枚唯一/实际ID正确、16负标记0、311旧评论正文/时间冻结；四refs、两原PR及开放页、六件三渠道、单父六新增和全部旧对象冻结全部通过后，才READY_FOR_MAINLINE_REVIEW、实际G04=PASS、STOP_WRITING。63暂态恢复先查远端效果再限次间隔重试，工具恢复/参数拒绝与真实写重试分账，不能重复正式写入。
10. 最终按198给一个通常3–6行原生短复制块、一个真实作者评论URL，报告158实际交付/自检/G04/停写及必要未证，请MYOS2-A-C11使用Pro独审并同轮后继；不进入159。C11设计计数仍7、8/10未达，C10历史11不变；未来交接传完整规范入口、真实断点/身份/计数和口述约定。

仅允许本批有限静态分析及自写文本/整数/对象核验；不运行MyOS2程序/脚本、预处理、编译链接、Graphviz、QEMU或真实IO，不改源码、旧件/评论、主线或PR元数据，不新建分支/PR、不合并/强推/删除或跨仓写。没有真实阻断就完成已授权终点；确有缺项只停受影响部分并保全，如实报告最小缺口，不冒签已读、已执行或G04。
