---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FAT-ALLOCATION-GEOMETRY-56
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
produced_by: unknown
produced_by_reason: "Owner-selected GPT-6+Extra does not independently attest backend author"
model_per_owner: GPT-6
effort_per_owner: Extra
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
date: 2026-11-07
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
record_type: batch56_manifest
---
# G00–G04 第56批共同合同及自查
同一C04c/CHAT-EXEC-06，Owner选GPT-6+Extra但后台身份不可独立认证，produced_by unknown。沿用76接收、49 MANIFEST中00工作令/公约P1–P13/18/24/28。已完整读取固定84、55主线PASS回执R55-S01/S02、79、63，合读R54-S01/S02与R53C-S01、R35/R38历史限定；55无返工，R53旧空行误判已撤回不重传。
## G00预检
time=a039d9803ade、master=e8e60cd19540、lead/PR16=2e456bfc4d7a、原PR17预期ef22d0500731。PR16/17均Open/unmerged，PR17 Draft，只有两项开放PR。56新目录预期404，两PR无56路径交集。54/55旧评论及第56批任务发布评论已读，原本人56交付评论0条。固定time双金丝雀：
```text
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	this_cpu = smp_processor_id();
```
## 专题独立状态
A completed_in_scope：read/do尾部next与write普通next未更新current cluster，以及EOF/flags/sector轨迹；旧BI55-F02去重。
B completed_in_scope：iobuf_expand size_t nr与int tmp_nr、数组容量/存在、候选不预留、回滚/commit和fat_add_entries caller。
C completed_in_scope：BPB/GPT源字段、fsbi几何赋值、C中间表达式类型、ROOTBLK软件单位/页面容量、FAT getter/index与newdir的容量矩阵。
## 六件与P6
仅MANIFEST.md、fat-allocation-geometry.md、facts-and-boundaries.yaml、fat-allocation-geometry-paths.dot、deferred-findings.yaml、evidence.md，旧件/评论、master/time/PR16/lead、PR元数据不改。证据只来自固定源码真实片段，长范围拆分1–5行、同consumer合读，CRLF/Tab/空行不修改。
```yaml
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes:
  - "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
  - "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 101
  quotes_reconfirmed: 101
  downgraded_to_inferred: 0
  A_anchors: 24
  B_anchors: 35
  C_anchors: 42
  finite_models: 23
  DOT_nodes: 30
  DOT_edges: 32
  undefined_DOT_endpoints: 0
  exact_source_CRLF_tabs_blanks: true
  actual_definition_bounds_checked: true
  claim_support_requires_group: true
  consumer_mapping_checked: true
  JSON_YAML_parse: true
  full_local_P9: NOT_RUN
  Graphviz: NOT_RUN
  kernel_C_ASM_scripts_preprocess_build_QEMU_hardware: NOT_RUN
write_retry_log:
  before_tree: no_GitHub_writes_this_turn
  local_script_faults: two_prewrite_quote_range_and_header_helper_errors
  expected_head: ef22d0500731
  force: false
  actual_remote_attempts_and_readback: formal_PR17_comment
```
只允许原PR17 user-fat-allocation-geometry-56/六件及本人唯一新交付评论；按63安全重试与回读、expected-head/force=false，保留原HEAD下未知并行写入的禁止覆盖规则。预处理/编译/运行/修内核/合并/建分支PR均禁止。固定提交及活动分支6/6全文/blob、compare旧件零净改删、评论查重回读后STOP_WRITING，整包交C04独立验收；不自行启动57。
