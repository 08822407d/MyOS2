---
task_id: MYOS2-DR-006
title: Testing and CI research deliverables
base_snapshot:
  branch: time
  commit: a039d9803893e0ce54164413aa8a0c29eacf5c78
  tree: fa8d26ffa2cf2b827c183452039a0b780f82eab2
workspace_source:
  branch: master
write_target:
  branch: agent/MYOS2-DR-006
  directory: agent-workspace/results/MYOS2-DR-006/
delivery_mode: chat-fallback
status: research-complete-proposal-not-executed
---

# MYOS2-DR-006 Manifest

## Scope

本任务研究 MyOS2 应如何建立从“能启动”到“bug 修复必须留下回归测试”的分层内核测试体系，并给出可映射到 GitHub Actions 的 CI 方案。

内核源码事实统一以：

`time@a039d9803893e0ce54164413aa8a0c29eacf5c78`

为基准。

工作区公约、repo map 和任务书来自 `master/agent-workspace/`。

本次读取时 GitHub compare(master...time) 显示 `time` 相对当前 `master` 为 ahead 44 / behind 3；因此 `time` 和 `master` 已存在分叉，不能把当前关系理解成单纯的 fast-forward 差 44 个提交。

对 `time` 快照 tree
`fa8d26ffa2cf2b827c183452039a0b780f82eab2`
的递归检查没有发现 `.github/`，所以方案不依赖已有 GitHub Actions workflow。

## Deliverables

| Path | Purpose | Acceptance coverage |
|---|---|---|
| `MANIFEST.md` | 快照、范围、交付物和限制登记 | provenance |
| `01-survey.md` | 外部成熟测试实践调查，并映射到 MyOS2 | L1–L4 rationale |
| `02-adoption-plan.md` | 分阶段落地、CI 门禁、bug→regression 流程 | L1–L4 |
| `proposed/ci.yml` | GitHub Actions workflow 草案 | L4；编排 L1/L2/L3 |
| `proposed/run-qemu-smoke.sh` | 无头 QEMU 串口日志判定 harness | L1；供 L3 复用 |
| `proposed/test-contract.md` | host unit、guest kernel、syscall 测试协议与回归规则 | L2/L3 |

## Recommended integration model

不要一开始把所有测试塞入单一 QEMU job。

推荐四层模型：

1. L1：QEMU headless boot smoke
2. L2：host-side pure unit tests / 极小型 kernel unit tests
3. L3：真实 guest user-space → syscall → kernel 功能测试
4. L4：GitHub Actions orchestration + reproducible logs + regression gate

其中 L1 是最先建立的“生命体征”；L2 提供最快反馈；L3 验证真正的 ABI 和权限边界；L4 把前三层变成不能被遗忘的合并门禁。

## Key design decisions

### Machine-readable output, not VGA scraping

CI 不应依赖屏幕截图、VGA framebuffer 或人工观察“看起来已经启动”。

内核/测试运行器应把最小测试协议送往 QEMU 可在 headless 模式捕获的字符通道，首选 guest serial → host stdio。

至少定义：

- `MYOS2_BOOT_OK`
- `MYOS2_TEST_BEGIN <suite>`
- `MYOS2_TEST_PASS <case>`
- `MYOS2_TEST_FAIL <case> ...`
- `MYOS2_TEST_END pass=<n> fail=<n>`
- 一个稳定的 panic/fatal 前缀

CI 以“成功标记 + 无 fatal + 有界超时”三条件判断，而不是仅依赖 QEMU process exit code。

### Unit tests and syscall tests are different layers

host unit test 只能用于没有特权指令、没有 MMIO/PIO、副作用已隔离的纯逻辑。

syscall 功能测试必须从 guest 用户态通过真实 syscall ABI 进入内核，不得为了方便而直接链接/调用 syscall implementation。

否则只能证明 C 函数本身工作，不能证明 trap entry、参数传递、用户/内核边界、dispatch 和返回路径工作。

### Every reproduced bug should become a test

bug 修复流程建议固定为：

`reproducer -> test fails -> fix -> test passes -> permanent regression test`

选择最低且足以覆盖 bug 的层级：

- 纯算法问题 → L2
- 启动/中断/早期初始化问题 → guest/kernel test 或 L1 specialization
- syscall/用户态可见语义 → L3
- 多组件启动回归 → L1/L3

## External primary sources

- Linux KUnit:
  `https://docs.kernel.org/dev-tools/kunit/`
- Linux kselftest:
  `https://docs.kernel.org/dev-tools/kselftest.html`
- Linux KTAP:
  `https://docs.kernel.org/dev-tools/ktap.html`
- QEMU system invocation:
  `https://www.qemu.org/docs/master/system/invocation.html`
- GitHub Actions workflow syntax:
  `https://docs.github.com/actions/writing-workflows/workflow-syntax-for-github-actions`

这些来源用于设计方法论；MyOS2-specific 结论只以仓库快照为准。

## Open questions

1. `time@a039d980...` 的最终标准 build command、boot image 路径以及当前人工 QEMU 启动命令，需要在正式接线时从同一快照的 build 文件锁定，并替换 `proposed/ci.yml` 中显式保留的 repository variables。
2. 需要在 MyOS2 当前输出设施中选定一个 CI 稳定通道，并确定现有 panic 文本的稳定前缀；在此之前不应让 CI 猜测 VGA 文本。
3. 需要决定 `time` 当前 behind master 的 3 个提交是否应该进入未来内核开发基线。该问题不属于 MYOS2-DR-006 的修改范围。
4. 本任务只产出 workspace proposal，不应借 MYOS2-DR-006 直接修改内核实现来增加串口日志或测试入口；真正的内核改动应另开实现任务。
5. `proposed/ci.yml` 使用 repository variables 表达尚未冻结的项目 build/QEMU 命令，以避免在没有证据时虚构 MyOS2 路径。正式启用 workflow 前这些变量必须固定为仓库内脚本，而不是长期保留成可漂移的 UI 配置。
