# 续接检查点 · 临时消费口径与核实计划（2026-09-07）

```yaml
task_id: MYOS2-LEAD-001
track_id: MYOS2-LEAD-001
record_type: continuation_checkpoint
evidence_class: B
produced_by: claude-fable-5.1@claude-code-vscode（主导会话；UUID 3bfbb342-1b15-4461-a4c3-987f17aad103）
date: 2026-09-07
authorization_ref: 00-owner-work-order-verbatim.md §13 第 4~6 条
base_snapshot: origin/master @ 22098048（PR #7 合并后）；内核 time @ a039d9803ade2a1613d620bda375e028530d5242
branch: agent/MYOS2-LEAD-001；open PR #8
status: WAITING_WAVE2_RETURNS_PROVISIONAL_MODE
```

## 五件事
1. **现行规则**：不变；新增临时口径——回收件暂按"未编造"使用但标 `provisional_unverified`（第 6 条）；全部 GPT 任务由深度研究执行、署名标 gpt6（第 4 条）。
2. **做到哪一步**：第二波 10 个发射块已交 Owner（WAVE-2-LAUNCH §7）；GPT 主导会话工作令（MYOS2-LEAD-002）与接管快卡已起草（PR #8）；尚未回收任何一份。
3. **哪些门待 Owner**：合并 PR #8；是否启动 LEAD-002；本地闸门是否保留（本地 A 取号 012）；OI-1；12 条决策。
4. **暂定/待追认**：协议 v2、全部任务书、LEAD-002 工作令均为 draft；本轮回收件的"未编造"是假定不是裁定。
5. **下一步安全动作**：额度恢复前只做零成本登记（回收件入库、标记 provisional）；约 2026-09-10 起，Owner 指示后做正式核实：每份回收件跑 P9 十三项（需先取号做本地 A 引文闸门脚本）＋对抗核查，出 WAVE-2-REVIEW 或补充 LEAD-002 的 GATE-REPORT。

## 核实计划（2026-09-10 起，待 Owner 一句话启动）
- 输入：每份回收件（PR 分支或 received/ 原件）＋其任务书 v2 ＋协议 v2。
- 步骤：本地 A 脚本（取号 012）→ 逐份 P9 十三项 → 不过的整份退回 → 过的做三视角对抗核查（反编造 / 事实一致 / 出处支持性）→ 裁定写入 WAVE-2-REVIEW.md（若 LEAD-002 已写则出 -v2 supersedes）→ 标 `verification_status: verified | returned`。
- 期间不启动子代理、不跑工作流。
