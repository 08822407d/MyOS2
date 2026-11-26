---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: receiver_startup_prompt
conversation_display_name: "MYOS2-A-C11 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（本轮系统身份）"
model_per_owner: "继承Pro主线独审与同轮后继安排"
actual_backend_model: UNATTESTED
actual_reasoning_effort: UNATTESTED
date: 2026-11-26
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: 974a51a9aa96
execution_pin_short12: 07d636217117
read_channel: "GitHub connector；自制文本/整数辅助核验"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
status: PREPARED_AWAITING_OWNER_START_IN_NEW_CHAT
package_id: MYOS2-A-C10c-TO-C11a-152-20261126
package_status: non_execution_source_transfer_artifact
receiver_display_name: "MYOS2-A-C11a 代码分析执行"
receiver_identity: MYOS2-LEAD-002-CHAT-EXEC-25
actual_receiver_state: NOT_OBSERVED
receiving_model_per_owner: "次一档GPT-6.2 Sol＋Extra／高推理"
receiver_guidance_load: {project_guidance: required, meta_agent_guidance: required, mnemosyne_guidance: no, mnemosyne_receive_format: task_local_only, mnemosyne_naming: required, refresh_completed: PENDING, task_local_only: true}
business_execution_source: "Owner本轮恢复请求、同提交00工作令第18条与213号152合同"
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
self_check_scope: "仅本文件新增正式源码引文；既有151的138窗独立复核另账。"
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
mainline_design_count: {conversation: C11, before_release: 0, after_successful_152_release: 1, thresholds: [8, 10], reached: false}
previous_mainline_design_count: {conversation: C10, distinct_batches: 11}
evidence_class: "SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / OBSERVATION / UNKNOWN"
open_questions: ["真实运行、ABI/标准/DEBUG/链接/对象生命周期与后台档位未证；旧C10c发表后G04历史未证。"]
---

# 新对话接收并连续完成第152批

**本次接收和普通执行无需Pro，使用GPT-6.2 Sol＋Owner可用Extra／高推理。完成交C11 Pro独审与同轮后继。**

@GitHub 请在MyOS2辅助分析项目中，命名并以「MYOS2-A-C11a 代码分析执行」／MYOS2-LEAD-002-CHAT-EXEC-25接收MYOS2-A-C10c-TO-C11a-152-20261126，连续完成CORE-USER-MALLOC-REINIT-152。Owner报告旧C10c上下文耗尽，主线已核151实际交付并验收；你接新152，不重做151，不要求旧方补签，也不另等开工确认。

1. 按Owner本轮真实正式评论ID双读，锁定其实际发布提交；完整读[00恢复包](00-C10c-to-C11a-recovery-package.md)、[213合同](../213-user-malloc-reinit-contract.md)、[151独审](../reviews/CORE-USER-MALLOC-STATE-CONSUMPTION-151-review.md)、[工作令第18条](../00-work-order.md)及[本轮检查点](../checkpoints/2026-11-26-c10c-to-c11a-state151-reviewed-reinit152.md)。本文是完整接收指令，不把外仓包格式当业务权限。
2. 正确认定断点：151=07d636217117六件＋原作者#7184689930，归EXEC-24；主线已在R151-01有界助手摘要补注下PASS_PENDING_LOCAL，152 D00=[]。旧作者发表后G04历史未知，主线新终检只证明当前远端。150及149补正/合读链、旧RETURN和原件全部保持。新身份不继承旧方已读、私有工作或署名。
3. 必须自己完整加载恢复包§3十五份项目规则（00取本次固定版，13固定1174c16c4540，加210），§4的MA 48→47及同版共用十五条全文、现行入口/建设检查点/模型登记；Mnemosyne README、接收命令、批准规范及命名§7按任务局部读取。刷新实际refs；输出截断补读到结尾，不以旧方或主线已读/摘要代替。保持MyOS2任务与权限，不接管外仓业务。
4. 在152 MANIFEST分别写execution_handoff_receive与guidance_refresh：实际包来源/时间/新身份、Owner指定模型与系统可核身份/UNATTESTED后台effort、已读与缺失、到达后胜任及容量判断、当前阶段2/唯一六件写区/下一安全步、task_local_only/current_task_preserved。实际读完才记refresh_completed；不增加第七文件或单独发接收评论。
5. fresh G00：直接lead等于本轮发布提交，exec07d636217117、timea039d9803ade、mastere8e60cd19540；原PR16 Open/Ready和原题PR17 Open/Draft、head/base/未合并；开放PR与PR17无since完整分页至空。46历史＋3本轮标记＝49逐条唯一，真实ID须核同；本轮评论及151作者/前次C11恢复双读并与分页相同。核151固定/活动/API六件、单父与差异、全部旧原件/评论冻结、152目录及EXEC-25/旧EXEC-24本批作者均0；两个历史负标记保持0。
6. 无真实阻断即按213执行两条独立reinit→malloc受控序列：A根重置，B残留bitmap/根成员与owner集合/只读search观察，C后继一次malloc止于311调用边界。技术给定、共享对象及有序delta可紧凑表达，不重复展开旧151604KB facts。不能把局部ret_val写成已返回、临时count当真实节点或旧对象脱离根链写成实际释放。
7. 只在原claude/dazzling-cori-q0dnyt／Draft PR17新增user-malloc-reinit-152/的MANIFEST.md、malloc-reinit.md、facts-and-boundaries.yaml、malloc-reinit-paths.dot、deferred-findings.yaml、evidence.md。标准P3/P6根字段、源实际定义/命令实例、CRLF/NONE可逆处理、强制锚/图全属性及连接器可达P9实际做，交付前重新打开源逐窗核；不凑字数/引文数量，不复制旧错误摘要。
8. 写前再查未知并行写及目录/作者占用；真实expected_sha、force=false、唯一父、一提交六新增零其他改删。固定/活动/API全文、LF/字节和差异核同后，唯一作者标记为MYOS2-LEAD-002-CHAT-EXEC-25 / CORE-USER-MALLOC-REINIT-152 / FORMAL_DELIVERY。评论留完整身份/接收/规范/合同/固定成果/自检及限制。63暂态错误先核效果再限次间隔重试，不重复发评、提交或覆写未知写入。
9. 发表后才做真实G04：新ID双读、全评论分页至空，49原标记＋新作者共50各唯一；旧EXEC-24的152及两个历史负标记0；四ref、开放PR/两PR状态/原题/元数据、六件固定/活动/API和唯一父差异、151冻结及旧评论正文。全部实做后报告READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、实际G04与STOP_WRITING；不启动153。
10. 最终聊天按198只给一个通常3–6行原生短复制块、一个真实作者评论URL，说明152真实完成/待独审/实际G04/停写及请C11 Pro独审并同轮后继。受阻或未完成如实给最小缺口；不把完整技术、统计、规范日志搬回聊天，不要求Owner拼接条件。

继承结果先行、人工动作集中、授权内连续推进、普通问题先入库、次一档执行/Pro独审、同轮后继、63与198、主线8/10提醒。C11首次设计152后计1，C10历史11，交接本身不加数；以后交接须传十五规则与有效MA全文及同版共用全文、命名依据、真实断点与计数。

阶段2；完整本地P9、预处理、编译链接、MyOS2程序/脚本、Graphviz、QEMU、实机NOT_RUN。无源码、旧结果/评论、主线目录、PR元数据写权；不新建分支/PR、合并、强推或删除。只读规范归档解码允许。新方实际接收/执行待发生，不预签。
