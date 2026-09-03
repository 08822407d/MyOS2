# 前任记录镜像收据（Mnemosyne → MyOS2）

```yaml
record_type: predecessor_mirror_receipt
track_id: MYOS2-LEAD-001
task_id: MYOS2-LEAD-001
produced_by: claude-fable-5.1@claude-code-vscode（主导会话 myos2-62 [c480bd]，UUID 3bfbb342-1b15-4461-a4c3-987f17aad103）
date: 2026-09-03
evidence_class: C（历史/方法参考；只复制不改写）
source_repository: Mnemosyne（GitHub 08822407d/Mnemosyne；本地 /home/cheyh/projs/Mnemosyne，主检出停在工作分支、未切换、未写）
source_ref: origin/master @ c319397eb2fff954259749aafded9e81e90ce1ba（本地远端跟踪引用；镜像前**未 fetch**——工作令只授权 show，Owner 未答是否允许 fetch，故可能落后 GitHub 最新）
mirror_method: git -C /home/cheyh/projs/Mnemosyne show origin/master:<path> > <mirror>，逐字节；双侧 bytes/sha256 互引如下
authorization_ref: 工作令 §12 第 2 条默认（Owner 2026-09-03 "开工" = 接受默认）；§8 阶段 0 交付项 predecessor/
status: final
open_questions: [镜像所据 Mnemosyne origin/master 是否为 GitHub 最新（未 fetch）]
```

| 源路径（Mnemosyne origin/master @ c319397） | bytes | sha256（源） | 镜像路径（本目录） | bytes | sha256（镜像） | 一致 |
|---|---|---|---|---|---|---|
| target-projects/myos2/00-intake-and-wave-001-launch.md | 6329 | `e6e2b6fc39601032b06379c4e187adf1c2e871fd80e40ebbd63ff0c79094839e` | mnemosyne-00-intake-and-wave-001-launch.md | 6329 | `e6e2b6fc39601032b06379c4e187adf1c2e871fd80e40ebbd63ff0c79094839e` | yes |
| target-projects/myos2/01-wave-1-intake-and-review.md | 6448 | `08302580fe4554b7f4d49cfb84f3e858b6e2d006ea21a03322ef244dcf62d654` | mnemosyne-01-wave-1-intake-and-review.md | 6448 | `08302580fe4554b7f4d49cfb84f3e858b6e2d006ea21a03322ef244dcf62d654` | yes |

说明：
- 两份镜像是 Mnemosyne 会话（MYOS2-ANALYSIS-001 轨道）对本计划的立项决策（D1~D6）、留给 meta-agent 的接管清单、基线修正，以及第一波回收评审记录。它们是 C 类材料：可引用其结论的出处，不作为现行规则来源（现行暂定执行源见 03-provisional-execution-sources.md）。
- 复制目的：满足工作令 §1.3 第 6 条"交接自足"——让一个全新会话仅凭 MyOS2 仓库内容即可恢复来龙去脉。
- 若日后 Mnemosyne 侧追加了 02-handover 记录，按同法追加镜像并在本收据新增一行，不改本收据既有行。
