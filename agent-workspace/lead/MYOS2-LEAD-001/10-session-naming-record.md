# 会话命名登记（MYOS2-LEAD-001 · 响应 Owner 补充说明第 9 条）

```yaml
task_id: MYOS2-LEAD-001
track_id: MYOS2-LEAD-001
record_type: session_naming_record
evidence_class: B
produced_by: claude-fable-5.1@claude-code-desktop（会话 UUID 3bfbb342-1b15-4461-a4c3-987f17aad103）
date: 2026-10-15
rule_source: Mnemosyne origin/master @ 2bba5ad notes/registries/project-research-display-name-registry-v0.1.md §7（§7.1 格式、§7.3 类型码、§7.4 三条铁律、§7.5 Claude Code 落实方式、§7.7 默认适用与找回）；设计源 notes/cross-model-review-results/FABLE5-REVIEW2-001/03-independent-design/11-conversation-naming-convention.md
status: final（名字已落 UI；注册表行待 Mnemosyne 侧落表）
```

## 1. 名字

**`MYOS2-L-CC01 内核分析主导`**

| 段 | 值 | 依据 |
|---|---|---|
| 项目 | `MYOS2` | 注册表 §1 尚无 MyOS2 项目码；MyOS2 的全部 canonical 任务号已用 `MYOS2` 作项目前缀（MYOS2-DR-NNN、MYOS2-LEAD-NNN、MYOS2-ANALYSIS-001），沿用可免第二个别名。Owner 若偏好三字母码（如 `MYO`），改登记即可（§7.4 铁律 2）。 |
| 主线 | `L` | 主导线（与 Mnemosyne 的 `L` 同义：Claude Code 主导会话谱系）。本会话是 MyOS2 分析计划的本地主导会话。 |
| 类型 | `CC` | Claude Code 会话（§7.3）。 |
| 序号 | `01` | §7.4 铁律 1：序号＝主线内交接代数，只在**同主线**交接时 +1。前任 f0e60aba 是 Mnemosyne 主导线会话（已登记为 `MNE-L-CC05`，一个对话只有一个名字），MyOS2 的工作是它插入 Mnemosyne 主线的任务；跨主线交接不计代，故本会话为 MyOS2 主导线 CC 第 01 代。谱系真相由仓库记录承载：`predecessor/`、`08-handover-closure-log.md`、Mnemosyne `target-projects/myos2/02-handover-record.md`。 |
| 主题 | `内核分析主导` | 无状态词、无日期（铁律 3）。 |

## 2. 落实

- 桌面端会话标题已由本会话自行设置（§7.5：Claude Code 桌面端会话用会话工具自行改标题，app 自动生成的标题可直接替换）。原标题 "MyOS2 work order startup" 为 app 自动生成。
- 跨会话消息与共享记忆中涉及本会话身份时一律写 "UUID 3bfbb342-1b15-4461-a4c3-987f17aad103 ＋ MYOS2-L-CC01"（§7.7 身份写法），不写"本会话"。
- 机器名（myos2-xx [......]）随进程重启变化，不是名字。

## 3. 待 Mnemosyne 侧落表的注册行（本会话不得写 Mnemosyne 仓，工作令 §4.9；请 Owner 转给任一 Mnemosyne 会话或自行粘贴）

```yaml
# §1 projects 追加
  MyOS2:
    project_id: myos2
    abbreviation: MYOS2          # 沿用 canonical 任务号前缀；Owner 可改为 MYO
    sequence_width: 3
    allocation_owner: MyOS2_lead_session_or_owner
    canonical_series: [MYOS2-DR-NNN（next 012）, MYOS2-LEAD-NNN（next 003）]
# §7.2 mainline_codes 追加
  MYOS2:
    L:
      full_name: MyOS2 分析计划主导线（Claude Code 主导会话谱系；接续 Mnemosyne 主导线 MNE-L-CC05 插入的 MYOS2-ANALYSIS-001）
      status: active
      first_display_name: MYOS2-L-CC01 内核分析主导
      generation: "01"
      type_code: CC
      session_uuid: 3bfbb342-1b15-4461-a4c3-987f17aad103
      canonical_task_id: MYOS2-LEAD-001
      predecessor: f0e60aba-25ea-419b-b350-71be428ec822（MNE-L-CC05；跨主线交接，不计代）
      record: MyOS2 agent-workspace/lead/MYOS2-LEAD-001/10-session-naming-record.md
      named_at: 2026-10-15
      derived: [MYOS2-L-C01 回收初审（ChatGPT 普通对话主导会话 MYOS2-LEAD-002；并行主导，Owner 在 ChatGPT 侧重命名）]
      notes: [UI_alias_only, sidebar_renamed_by_session_via_claude_code_session_tool, named_by_session_itself_on_owner_instruction_2026-10-15]
```

## 4. 顺带：本轨道已创建/起草对象的 §7.1 名（§7.7"新建即命名"的补登，均为导航元数据）

| 对象 | §7.1 名 | 说明 |
|---|---|---|
| ChatGPT 主导会话 MYOS2-LEAD-002 | `MYOS2-L-C01 回收初审` | 类型 C（ChatGPT 普通对话）；由 Owner 在 ChatGPT 侧重命名 |
| 第二波十个深度研究对话 | 沿用 canonical 号：`MYOS2-DR-003R 依赖图重锚` 等 | §7.3：DR 类型沿用 `<项目>-DR-NNN` 序列时可省主线段，现名已合规 |
| 本会话未来派生会话 | `MYOS2-L-CC01a …` | 父名加后缀，不占代数（铁律 1） |
