# 关于 006 / 007 / 009 的降级交付与拆分状态

```yaml
record_type: extraction_note
date: 2026-09-01
by: claude-fable-5 主导会话
```

## 情况

MYOS2-DR-006 / 007 / 009 是深度研究任务。深度研究模式下 GitHub 连接器只读（三份报告各自独立确认了这一点），因此它们按公约走**降级路径**：把应写入仓库的文件逐份内嵌在对话报告正文里输出。

本次回收时，对话报告原件已完整归档在各任务的 `received/` 下（这是权威原件，未经任何改动）。我另外把其中**能可靠机械拆分**的部分还原成了独立文件：

| 任务 | 已还原为独立文件 | 未还原（正文未包在代码围栏内，机械拆分会截断） |
|---|---|---|
| 006 | MANIFEST.md、01-survey.md、proposed/{run-qemu-smoke.sh, test-contract.md, ci.yml} | 02-adoption-plan.md |
| 007 | MANIFEST.md、01-evidence-survey.md、importance.yaml、roadmap.md | — |
| 009 | — | MANIFEST.md、01-precedents.md、02-llm-teaching-evidence.md、03-own-kernel-specifics.md、04-curriculum-skeleton.md |

**未还原的部分请直接读 `received/` 下的报告原件**，内容完整，只是没有拆成单独文件。宁可留在原件里，也不放半截文件误导后续读者。

## 使用前必读

这三份产出的可信度裁定见 `agent-workspace/WAVE-1-REVIEW.md`：

- **006**：抽查 23 条中 12 条被证伪，可靠性 low。其 40 位 commit SHA 是伪造的（`a039d9803893e0ce…` 在仓库中不存在，真值 `a039d9803ade2a16…`），且该假值被写进了 `proposed/ci.yml` 的 `BASE_SNAPSHOT` 环境变量与 harness 头注释——**采用前必须替换**。其外部综述未覆盖任务书点名的 xv6/SerenityOS/Redox/syzkaller/KernelCI/osdev（已另开 MYOS2-DR-010 补做）。`ci.yml` 实测跑不起来（无工具链/QEMU/OVMF 安装步骤）。但 `run-qemu-smoke.sh` 经独立核查员实跑五场景通过，是本波次唯一真正可用的工程件（需先修 CRLF 与进程组 kill，详见评审报告）。
- **007**：可靠性 medium，是本波质量较好的一份。MANIFEST 的 tier 计数有误（T1 实为 23、T2 实为 11）。
- **009**：可靠性 medium。因任务书要求其零源码接触，它把 entry（74 行）、device（0 源码）、debug（1 文件）都排进了教学单元——使用其课程映射表前须与 002 的成熟度做交叉过滤。
- **三者共同缺陷**：引用全部是内部检索句柄（共 175 处），无一可解析 URL/DOI，外部结论无法独立复核。第二波已对此立硬性纪律。
