---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-WAIT-WAKE-CONSISTENCY-52
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-07
source_pin_short12: a039d9803ade
read_channel: GitHub_connector
record_type: batch52_manifest_and_common_preflight
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# G00–G04 · 第52批A/B/C单次预检与统一交付目录

## 唯一身份、读入口和历史责任
同一C04c/CHAT-EXEC-06，Owner选GPT-6＋Extra，真实后端身份不可独立认证；沿用76既有接收、49 MANIFEST记载的MyOS2 00工作令、公约、README、P1–P13、18/24/28及适用Mnemosyne指导，未重新启用项目外维护或付费模型。该组合按79号合包策略在一轮完成A/B/C，不逐专题请求C04派发。
本轮**完整读取**固定`c7f6cad5316f`的80任务书、79工作量/合包说明、51主线回执R51-S01/S02、63安全重试规则，并读50 R50-S01/S02、49文档闭合R49C-S01及48 R48-S01/S02；定点读core/results.yaml中V09/V10/V11和CA-03/CA-04，不运行历史夹具。50/51已由C04 PASS_PENDING_LOCAL、R49-01 CLOSED_DOCUMENTATION_ONLY，均不重做验收或状态升级。

## G00预检与唯一允许的新写区
固定time@a039d9803ade；预检direct ref `master=e8e60cd19540`, `lead/PR16=c7f6cad5316f`, `PR17=1e128a33a014`（首次写前预期），时间分支`time=a039d9803ade`。PR16/17均Open/unmerged，PR17 Draft；开放列表只有16/17。52新目录预期404且PR16/17既有改动无写区交集；旧51交付评论6688576624及主线评论6700858922均已读，作者52评论原先不存在。一次直接ref只读遇RemoteProtocolError断连，后续同URL回读恢复且HEAD匹配，此为读取暂态，**不是写入失败或认证拒绝**。
固定time双金丝雀（从真实源码读回）：
```text
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	this_cpu = smp_processor_id();
```
源码台账随facts.source_ledger（本轮16件完整源码+blob短ID、具体函数）与evidence的真实定义/行号；若来源引用先前同一pin则在facts.supplementary_source_reads标明。205个固定`.c`全文的只读名称扫描另记C.scan；扫描早期一次编排超过单轮工具上限，**没有任何GitHub写动作**，随后分批完成205/205零失败。全部头文件索引未完成，明列边界。

## 三专题各自状态
- **A completed_in_scope**：List_hdr_s的count与链环/自指关系，prepare、wake、finish、all及普通semaphore对照，已复用既有CA-04/V11，不宣称实机异常。
- **B completed_in_scope**：stack timer初始化、MOD_TIMER_NOTPENDING分流、base/flags、pending摘链、running_timer重试、shutdown=false和回调生存期停止线；不运行timer系统。
- **C completed_in_scope**：固定tree全部205个`.c`源码文字搜索、固定符号身份和典型直接调用消费、49–51矩阵、未扫的头文件/编译可达性明确留缺口。

## 六件成果
| 新文件 | 单独职责 |
|---|---|
| MANIFEST.md | G00共同合同/输入/源台账入口/自查/发布停止线 |
| wait-wake-consistency.md | A/B/C自然语言有界推演，证据和条件分层 |
| facts-and-boundaries.yaml | JSON兼容YAML1.2、统一source ledger/模型/调用者索引/接口矩阵 |
| wait-wake-paths.dot | A/B/C分区结构与跨上下文条件虚线、STOP边 |
| deferred-findings.yaml | CA-03/04身份不重复、四个局部缺口 |
| evidence.md | 按专题编号的固定源单定义1–5行引文，消费者/定义身份精确标记 |

## G04 P6 机械与语义分项
```yaml
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
source_pin_short12: a039d9803ade
self_check:
  verified_claims: 67
  quotes_reconfirmed: 67
  downgraded_to_inferred: 0
  A_evidence: 22
  B_evidence: 29
  C_evidence: 16
  pure_JS_models: 36
  dot_unique_nodes: 47
  dot_edges: 54
  undefined_dot_endpoints: 0
  quote_1to5_actual_definition: true
  source_blob_and_text_match: true
  quote_support_checked_manually: true
  facts_and_deferred_JSON_parse: true
  git_sha40_in_artifacts: 0
  full_local_P9: NOT_RUN
  Graphviz: NOT_RUN
  preprocess_compile_QEMU_hardware: NOT_RUN
write_retry_log:
  at_composition: not_needed
  git_write_attempts_before_tree: 0
  expected_head: 1e128a33a014
  update_strategy: expected_sha_and_force_false
  later_real_attempts_and_readbacks: record_in_official_PR17_delivery_comment
```

## 发版与终止
仅向PR17分支`agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-wait-wake-consistency-52/`新增六件；旧件/旧评论、master/time、lead/PR16、PR17标题/body/Draft冻结。不建新分支/PR，不修内核、不运行仓库C/ASM/脚本、不预处理编译/QEMU/硬件、不合并关闭强推。优先一tree一commit按expected-head非强推更新，固定commit/活动分支6/6全文及blob、net diff与唯一正式评论回读。完成后STOP_WRITING，提交C04进行一次独立组合验收，不自签PASS或启动53。
