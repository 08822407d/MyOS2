---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-SHELL-ERROR-DIAGNOSTICS-130
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
produced_by: GPT-6 (self-reported; backend/effort UNATTESTED)
model_per_owner: GPT-6＋Extra
date: 2026-11-23
base_snapshot: time
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---
# MANIFEST · 130合同限定的六件

六件：MANIFEST.md（接收/G00/P9）、shell-error-diagnostics.md（D00/A/B/C）、facts-and-boundaries.yaml（可解析JSON兼容YAML；选择器与全部事件）、shell-error-diagnostics-paths.dot（六历史投影簇、四日期簇、五条件诊断簇）、deferred-findings.yaml（未知与主线复审）、evidence.md（新增44个短引文窗口）。

## 接收与双指南刷新

mnemosyne_handoff_receive：包MYOS2-A-C08b-TO-C08c-130-20261123，为non_execution_source_transfer_artifact而非执行授权。Owner转发固定01及PR17发布回执激活181号合同。接收显示名MYOS2-A-C08c 代码分析执行，执行身份MYOS2-LEAD-002-CHAT-EXEC-20。接收档次next_tier_default、Owner指定GPT-6＋Extra；模型自述GPT-6，后台型号/推理档UNATTESTED。独立读取Mnemosyne固定e581eb9df6c4的README、current/human-approved-spec、commands/receive-mnemosyne-handoff，状态RECEIVED_READY。只持原MyOS2 exec分支指定六件与PR17唯一作者评论写权。

project_guidance独立读取MyOS2固定父0b52728d0314的11项：00-work-order、agent-workspace/README、conventions、tasks/00-gpt-task-protocol-v2、18-deferred-findings-and-resume、24-chatgpt-specialist-conversation-separation、28-review-and-next-batch-same-turn、63-github-write-retry-and-readback、79-work-package-sizing-and-pro-review、110-owner-communication-and-delivery-continuity、124-model-routing-and-pro-message-budget。公约§1第2条原文：
> 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。

任务合同181进一步限制本次results新目录。P2标题原文：P2 · 引文锚定（每条 [VERIFIED] 的最低要求）。

mnemosyne_guidance_refresh独立读取Mnemosyne固定e581eb9df6c4的commands/load-mnemosyne-guidance及current/guard-registry.yaml，并读取适用guard：user-operation-next-step-capability-and-intent、github-single-active-pr-lineage、run-context-and-pr-provenance、artifact-delivery-and-direct-generation、next-step-repository-write-visibility、cross-conversation-execution-intent-and-operator-flow、model-tier-and-subtask-cost-routing、source-artifact-preservation-and-design-rationale、external-research-display-name。project_guidance=required，mnemosyne_guidance=yes，task_local_only=true，current_task_preserved=true。指导不是MyOS2执行源。

## 独立G00

四直接GitHub refs初核：lead 266aa3a14fdb、exec 824ce6b5607a、time a039d9803ade、直接master e8e60cd19540。PR16 Open/Ready和PR17 Open/Draft至master、未合并，原开放PR仅2，目标130目录第一次检查为不存在。顶层PR17所有无since评论分页100/100/47/0至空，180发布#7080232408、129作者#7081367607、181发布#7082318231分别按ID回读，EXEC-20本130标记原为0。

旧129提交824ce6b5607a唯一父4a8952c6c956，六件纯新增6612行、删除0；本对话逐件比对固定和活动分支全文、SHA及UTF-8字节全部相同，六件合计283929字节；Blob短12顺序994fc24e0f8d、05ef831029cc、a56c7c3a0b55、cc2c5aa8918a、03ae459fe99b、8f67d9549b、8f67d9549b0f（以前一短串为前缀；正式计六项取末一项）。发布266aa3a14fdb单父0b52728d0314，lead八新增。

独立反向金丝雀：time内核C编译含-m64 -mcmodel=kernel -fno-pie -fno-pic，直接master含-m64 -mcmodel=large -fPIE；time panic.c调用smp_processor_id而master调用raw_smp_processor_id。time源码不取master替代。181号合同、00/01、129两份审查、检查点、四源分支及作者评论均直接读取。原R127-01和R128-01已文档关闭，R127-02、R129-01的历史RETURN未消失。

## 静态P6/P9账与交付边界

evidence新增44条形式为路径::符号的短窗口，均源内1–5物理行，保留原制表符及CRLF。self_check：verified_claims=44，quotes_reconfirmed=44，downgraded_to_inferred=0，旧129的76窗不纳入本批。前缀P2标题不计为正式引文锚（缺少路径::符号）。

R127旧边18项准确选择器替代、新增19边和9项B标签，DOT D00投影81节点/91条有类型边；三C簇每簇4个外层宏位置/1个直接helper调用/1条原参返回。43对寄存来源原样保存。R129按四case的27步迭代绑定与41字节结果、独立存储事件在工具内重放；五诊断字节37/25/37/35/28、ASCII hex、NUL及返回独立核对。

P1–P13中可由连接器核对的身份、作用域、P2源码窗、分支/P3金丝雀、P6计数、字节、表与有界DOT语义已检查；**完整本地P9 NOT_RUN**，编译、预处理、Graphviz、QEMU、实际文件操作与shell运行均NOT_RUN，不虚构全闸PASS。GitHub写入限一次单父新树快进(expected_sha、force=false)，出现暂态先按63读远端效果再重试，不擅建PR或修改旧文件。作者状态READY_FOR_MAINLINE_REVIEW；主线PENDING；两项补正CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW，交C09 Pro裁定。
