# MYOS2-DR-006 内核测试与 CI 深度研究交付物

## 执行状态与冻结快照

本次分析已按任务要求读取 `master` 上的 `agent-workspace/conventions.md`、`agent-workspace/repo-map.md` 和 `agent-workspace/tasks/MYOS2-DR-006-testing-and-ci.md`，并成功读取 `time` 分支。源码分析快照冻结为：

- **源码分支**：`time`
- **源码 commit**：`a039d9803893e0ce54164413aa8a0c29eacf5c78`
- **对应 tree**：`fa8d26ffa2cf2b827c183452039a0b780f82eab2`
- GitHub 对比结果显示，在本次读取时 `time` 相对当前 `master` 是 **ahead 44 / behind 3**；因此当前仓库已经不是简单的“master 严格落后 time 44 个提交”关系，而是两个分支已经发生少量分叉。这不改变任务要求：内核事实仍以 `time@a039d980…` 为分析基准。
- 对上述 `time` tree 的递归目录检查没有发现 `.github/` 路径，因此该源码快照中没有现存 GitHub Actions workflow 可直接继承。

遵照“深度研究期间 GitHub 连接器只读”的约束，本回合没有创建 `agent/MYOS2-DR-006`、没有提交文件、也没有建立 PR。因此执行任务书规定的**降级路径**：下面逐文件完整输出应新增到 `agent-workspace/results/MYOS2-DR-006/` 的交付物。所有文件均明确记录实际使用的 `base_snapshot`。

外部实践的设计依据采用成熟内核/虚拟机/CI 项目的官方一手文档：Linux KUnit 将内核单元测试与结构化测试结果作为一等公民；Linux kselftest 用真实用户空间接口验证内核行为；KTAP 为内核测试结果定义机器可解析协议；QEMU 官方系统仿真接口支持无图形前台运行以及串口标准输入输出；GitHub Actions 官方工作流提供作业超时、最小权限和按 job 分离失败域等机制。对应官方来源为 Linux KUnit 文档 `docs.kernel.org/dev-tools/kunit/`、kselftest 文档 `docs.kernel.org/dev-tools/kselftest.html`、KTAP 文档 `docs.kernel.org/dev-tools/ktap.html`、QEMU System Invocation 文档 `qemu.org/docs/master/system/invocation.html` 与 GitHub Actions workflow syntax 文档 `docs.github.com/actions/writing-workflows/workflow-syntax-for-github-actions`。这些来源被用于**测试体系设计原则**，而不是用来推断尚未从 MyOS2 源码确认的实现事实。

## MANIFEST

**目标路径：`agent-workspace/results/MYOS2-DR-006/MANIFEST.md`**

```markdown
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
```

## 外部实践调查

**目标路径：`agent-workspace/results/MYOS2-DR-006/01-survey.md`**

```markdown
---
task_id: MYOS2-DR-006
deliverable: survey
base_snapshot:
  branch: time
  commit: a039d9803893e0ce54164413aa8a0c29eacf5c78
  tree: fa8d26ffa2cf2b827c183452039a0b780f82eab2
status: proposal
---

# Testing and CI Survey for MyOS2

## Executive conclusion

MyOS2 不应该把“CI 测试内核”理解为只有一个：

`make && qemu-system-*`

的 job。

更稳健的结构是把测试拆成四个责任不同的层次：

| Layer | Question answered | Feedback speed | Failure localization |
|---|---|---:|---|
| L1 boot smoke | 内核镜像能否在标准虚拟机配置下启动并达到确定 milestone？ | 快 | 中 |
| L2 unit | 一个可以隔离的逻辑单元是否正确？ | 最快 | 高 |
| L3 syscall functional | 用户程序能否通过真实 ABI 得到正确 kernel-visible 行为？ | 中 | 高 |
| L4 CI | 前三层能否在每个变化上自动、可重复、超时受控地执行？ | — | — |

核心原则是：

**用最便宜的测试定位局部错误，用真实 guest 测试覆盖不能从 host 模拟的边界。**

这与成熟内核测试体系中的分层方式一致：KUnit 面向可隔离的 kernel unit；kselftest 面向从用户空间可观察的内核接口；KTAP 提供结构化结果；QEMU 提供自动化 guest 执行环境；CI 再负责编排这些测试。

## Survey: Linux KUnit

Linux KUnit 是 Linux 内核中的白盒单元测试框架。官方文档把 unit test 定位为针对相对独立的代码单元，并提供 assertion、suite/case 组织和可机器处理的测试输出。

Source:
`https://docs.kernel.org/dev-tools/kunit/`

### Relevant lesson for MyOS2

MyOS2 不需要复制整个 KUnit 实现，真正值得采用的是三个设计边界：

1. unit test 应当小而确定；
2. 测试结果必须由程序判断而非人工看屏幕；
3. 测试代码与生产代码之间需要明确的 seam。

对于教学/小型内核，最危险的做法是为了“unit test”而模拟整个机器。例如测试一个包含：

- page allocator
- interrupt controller
- scheduler
- serial device
- process table

的对象图，最终会产生一个比被测代码还复杂的 mock 系统。

更合适的做法是先找纯逻辑，例如：

- bitmap/index calculation
- bounded queue/ring bookkeeping
- pathname/token parsing
- flags validation
- size/alignment calculation
- table lookup/dispatch metadata
- time conversion helper

前提是这些具体候选必须从当前 MyOS2 源码逐项确认，而不能仅因为其他 OS 通常具有它们就宣称 MyOS2 已有对应模块。

### MyOS2 mapping

第一阶段不建议“先实现一个 MyKUnit”。

优先顺序应当是：

`可复用纯函数 -> host-side unit harness -> 必要时 guest kernel micro-test`

原因是 host unit tests：

- 不需要启动 QEMU；
- 崩溃可直接由 host process 表达；
- 容易在 CI 并行执行；
- 更适合 sanitizer；
- 失败定位比完整 kernel boot 清楚。

但是任何含 privileged instruction、port I/O、interrupt state、真实页表或用户/内核 privilege transition 的路径，都不应该因测试方便而被假装成 host unit。

## Survey: Linux kselftest

Linux kselftest 从用户空间调用正在运行的内核接口，验证用户可见行为。

Source:
`https://docs.kernel.org/dev-tools/kselftest.html`

它与 unit test 的重要区别不是代码放在哪里，而是**测试观察点在哪里**。

### Relevant lesson for MyOS2

对 syscall 而言，正确的测试不是：

```c
sys_write_impl(...);
```

而应接近：

```text
guest user test
    ↓
userspace syscall wrapper
    ↓
actual trap/syscall entry
    ↓
argument validation / dispatch
    ↓
kernel implementation
    ↓
return-to-user
    ↓
assert result
```

这样一次测试可以同时捕获：

- syscall number 错误；
- trap entry 错误；
- dispatch table 错误；
- register ABI 错误；
- user pointer/argument validation 错误；
- return value 错误；
- kernel implementation 回归。

直接调用 implementation 会绕过其中绝大部分路径。

### MyOS2 mapping

MyOS2 应建立一个专门的 guest syscall test program。

测试程序必须：

- 作为正常 user program 被加载；
- 使用与真实应用相同的 syscall wrapper/ABI；
- 每个 case 打印机器可解析结果；
- 某 case 失败时继续运行其他安全 case，最后输出汇总；
- 对会使 kernel fatal 的 case 单独启动 guest，以避免一个 case 污染整个 suite。

建议第一批只覆盖少量基础 syscall，而不是追求数量。

每个 syscall 至少考虑：

1. nominal success path；
2. boundary/zero-size path；
3. invalid argument path（在 ABI 支持并且安全时）；
4. repeated invocation / state transition。

## Survey: KTAP/TAP-style result protocol

Linux 文档定义了 KTAP 作为 kernel test 使用的结构化结果格式。

Source:
`https://docs.kernel.org/dev-tools/ktap.html`

MyOS2 没有必要立即完整实现 KTAP parser，但应当吸收其最关键的 CI 原则：

**输出应该是协议，而不是 prose。**

例如下面的日志对人很好看，却不适合作为长期 API：

```text
filesystem looks good!
we reached userspace :)
```

因为一句文案修改就可能让 CI 失效。

建议先建立一个极小稳定协议：

```text
MYOS2_TEST_BEGIN syscall
MYOS2_TEST_PASS syscall.write.basic
MYOS2_TEST_PASS syscall.write.zero
MYOS2_TEST_FAIL syscall.bad-number expected=-1 actual=0
MYOS2_TEST_END pass=2 fail=1
```

随后可以逐步迁移为 TAP/KTAP-compatible 输出。

### Why not parse every printk

CI 不应把所有普通 kernel log 都变成稳定接口。

只有以下标记属于测试协议：

- suite begin/end；
- pass/fail；
- boot-ready；
- panic/fatal。

普通 debug log 可任意变化。

## Survey: QEMU as a deterministic headless harness

QEMU 官方 system invocation 文档定义了系统仿真的命令行运行方式和无图形/字符设备配置。

Source:
`https://www.qemu.org/docs/master/system/invocation.html`

### Relevant lesson for MyOS2

“QEMU 能启动”与“QEMU 可用于 CI”是两个不同条件。

CI-ready guest 至少需要：

1. 不要求图形 display；
2. 不要求人工键盘输入；
3. host 能捕获 guest 的关键文本输出；
4. test 有 hard timeout；
5. kernel reboot/triple-fault 不能被误判成成功；
6. 有明确成功 milestone。

因此 smoke test 的判定必须是：

```text
PASS =
    expected success marker observed
    AND no fatal/panic marker observed before success
    AND deadline not exceeded
```

不能使用：

```text
PASS = qemu process started
```

也不能仅使用：

```text
PASS = qemu process exited with 0
```

原因是 OS guest 通常并不会像普通 Unix test process 一样自然退出；相反，成功启动后的 kernel 可能永久 idle。

### Recommended transport

首选：

```text
guest serial
    ↓
QEMU character backend / stdio
    ↓
CI log file
    ↓
small parser
```

而不是：

```text
VGA framebuffer
    ↓
screenshot/OCR
    ↓
CI
```

串口协议成本低、可 diff、可 grep，并且能覆盖非常早的 boot 阶段。

## Survey: GitHub Actions

GitHub Actions workflow syntax 官方文档定义了 jobs、steps、permissions、timeouts、environment 等编排能力。

Source:
`https://docs.github.com/actions/writing-workflows/workflow-syntax-for-github-actions`

### Relevant lesson for MyOS2

内核 CI 最重要的不是 workflow YAML 有多复杂，而是它要把 failure domain 分开。

推荐至少形成：

```text
host-unit
      \
       +--> build-image --> qemu-boot-smoke
                         \-> syscall-functional
```

如果 host unit 失败，应能立刻显示“unit failure”，而不是让开发者打开一个包含几千行 QEMU 输出的 job。

每个 guest job 都必须设置 timeout；QEMU hang 是内核开发的正常故障模式，不能允许一个错误无限占用 runner。

### Artifact/log policy

每次 guest failure 应保留：

- exact source commit；
- QEMU command；
- complete serial log；
- test protocol summary；
- runner/tool version，至少在排查失败时可获得。

不建议默认上传完整 build tree。

真正有诊断价值的是：

- boot/test logs；
- linker map / symbol map（如果项目已有）；
- failing test metadata。

## What the time snapshot tells us

本任务冻结的 kernel snapshot 为：

`time@a039d9803893e0ce54164413aa8a0c29eacf5c78`

tree：

`fa8d26ffa2cf2b827c183452039a0b780f82eab2`

本次递归 tree 检查未发现 `.github/` 路径。因此这套方案应该被视为建立 CI 基线，而不是修改现有 Actions pipeline。

同时，GitHub compare 在本次读取时返回：

```text
time relative to master:
ahead_by  = 44
behind_by = 3
```

这对 CI 有一个实际影响：

**不要让测试工作流隐式 checkout “当前默认分支”，然后声称它测试的是 time。**

在开发/验证阶段应显式记录源码 SHA。

任务产物中的所有方案因此统一使用：

`a039d9803893e0ce54164413aa8a0c29eacf5c78`

作为 base snapshot。

## Recommended MyOS2 test pyramid

### L1: headless boot smoke

Goal:

证明 boot artifact 在 QEMU 中从 reset 前进到一个稳定 milestone。

Minimal oracle:

```text
MYOS2_BOOT_OK
```

Failure:

- panic/fatal；
- QEMU 意外退出；
- success marker 未出现；
- timeout。

不要一开始测试 shell command、filesystem、timer 等所有功能。L1 的价值来自它非常小，因此启动基础设施一坏就能准确报警。

### L2: host/kernel unit tests

Goal:

快速定位可以隔离的纯 kernel logic。

Rule:

只有“生产代码与测试代码真正共享同一实现”才算 unit test。

下面这种测试价值较低：

```c
/* test copy */
static int copied_algorithm(...) { ... }
```

它只证明复制版本正确。

更好的形态：

```text
production pure helper
        ↙       ↘
 kernel caller   host test
```

I/O 和 privileged behavior 应放在边界之外。

### L3: syscall functional

Goal:

从 user mode 穿过实际 ABI 测内核。

Minimal initial suite:

- 一个确定成功 syscall；
- 一个边界输入；
- 一个确定失败输入；
- 一个可以验证返回值的 stateful sequence。

具体 syscall 名称应从当前 MyOS2 ABI 中选择，不能根据 Linux/POSIX 假定。

### L4: CI gate

Goal:

把 L1–L3 变成可重复的 merge signal。

Required properties:

- clean checkout；
- deterministic build command；
- finite timeout；
- machine-readable oracle；
- no manual input；
- failure log preserved；
- layer-specific jobs；
- exact revision visible。

## Anti-patterns to reject

### Sleep-and-pray boot tests

```bash
qemu ... &
sleep 5
kill ...
exit 0
```

这只能证明五秒过去了。

### Grepping generic boot prose

```bash
grep "Welcome to MyOS"
```

普通欢迎语不是稳定测试 API。

### Calling syscall implementation directly

这不覆盖 ABI/privilege transition。

### One giant integration test

如果 unit、boot、syscall、filesystem、timer 都只有一个 QEMU test，任何失败最终都会表现为：

```text
guest timed out
```

定位成本极高。

### Tests that can hang forever

内核 bug 很容易导致 spin/deadlock/interrupt failure。CI 超时是测试 correctness 的组成部分，而不是可选优化。

## Bug-to-regression workflow

每个已确认 bug 应记录：

```text
Bug
 ↓
Smallest reproducer
 ↓
Choose lowest valid test layer
 ↓
Add test and observe FAIL
 ↓
Apply/fix implementation
 ↓
Observe PASS
 ↓
Keep test permanently
```

理想 PR 证据：

```text
before:
  regression test FAILS on known-bad commit

after:
  same test PASSES on fix commit
```

如果问题只能在 QEMU 中复现，就不应该为了追求“unit test”而写一个无法真正复现 bug 的 mock。

## Recommendation

MyOS2 当前最优先的不是高测试覆盖率，而是建立三个稳定协议：

1. **build protocol**：一个无交互命令生成确定 boot artifact；
2. **console protocol**：headless guest 的机器可读输出通道；
3. **test-result protocol**：稳定的 PASS/FAIL/BOOT/FATAL marker。

有这三个协议之后，增加测试只是增量工作。

没有这三个协议时，即使写很多测试，CI 仍会被构建路径、VGA 输出和 QEMU hang 所绑架。

## Open questions

- 当前 snapshot 中哪一个现有输出 primitive 最适合作为 early-boot CI serial sink，需要在实施任务中以源码调用链最终确认。
- MyOS2 当前 user program 的装载/打包方式和 syscall test binary 的最佳注入点，需要与构建系统一起冻结。
- 哪些现存 kernel helper 已足够纯、无需重构即可作为第一批 host unit，需要在单独的 implementation task 中逐文件确认。
- 当前 panic 文本是否已有稳定前缀尚不应被 CI 当作 contract；推荐正式定义一个 marker。
```

## 采用与落地计划

**目标路径：`agent-workspace/results/MYOS2-DR-006/02-adoption-plan.md`**

```markdown
---
task_id: MYOS2-DR-006
deliverable: adoption-plan
base_snapshot:
  branch: time
  commit: a039d9803893e0ce54164413aa8a0c29eacf5c78
  tree: fa8d26ffa2cf2b827c183452039a0b780f82eab2
status: proposed
---

# MyOS2 Testing and CI Adoption Plan

## Objective

把 MyOS2 的测试从人工“启动 QEMU 看一眼”演化为具有以下性质的自动回归体系：

```text
source change
    |
    +-- fast host unit tests
    |
    +-- reproducible kernel image
            |
            +-- headless boot smoke
            |
            +-- syscall functional guest
```

所有 guest 测试：

- 无 GUI；
- 无人工输入；
- 有 hard timeout；
- 有机器可解析 oracle；
- 失败时保存完整文本日志。

## Frozen baseline

Kernel baseline:

```text
branch: time
commit: a039d9803893e0ce54164413aa8a0c29eacf5c78
tree:   fa8d26ffa2cf2b827c183452039a0b780f82eab2
```

本次读取时：

```text
time vs master = ahead 44 / behind 3
```

因此 CI 设计必须显式记录被测 SHA，不能仅写“latest time”。

本次 tree inventory 中没有 `.github/`，所以本计划是 greenfield CI proposal。

## Phase A: establish automation contracts

在增加大量 tests 前先冻结三个接口。

### Build contract

最终应存在一个 repository-owned command：

```bash
./scripts/ci/build.sh
```

或等价单一入口。

它负责：

```text
clean environment
 -> compile
 -> link
 -> construct boot artifact
 -> print artifact path
```

CI 不应在 YAML 中复制几十行编译逻辑。

原因：

同一个 build script 应可被：

- developer shell；
- GitHub Actions；
- future local regression runner

共同调用。

本 proposal 暂时用 `MYOS2_BUILD_CMD` 明确表示这个尚待接线的入口，而不虚构当前仓库的 build command。

### Console contract

至少指定一个 headless output channel。

推荐目标：

```text
kernel/test console
  -> serial
  -> QEMU stdio
```

不要把 framebuffer 内容变成 CI oracle。

### Result contract

稳定 marker：

```text
MYOS2_BOOT_OK
MYOS2_TEST_BEGIN <suite>
MYOS2_TEST_PASS <case>
MYOS2_TEST_FAIL <case> ...
MYOS2_TEST_END pass=<n> fail=<n>
MYOS2_FATAL ...
```

`MYOS2_*` marker 视为 testing ABI。

普通 debug text 不属于 testing ABI。

## Phase B / L1: QEMU headless smoke

### Purpose

尽早回答：

> 当前 kernel image 是否还能启动到预定 milestone？

### Success condition

全部成立：

```text
1. QEMU process can start
2. serial log contains exact boot-success marker
3. fatal marker did not occur first
4. deadline did not expire first
```

### Failure classes

Harness 应至少区分：

```text
FATAL_MARKER
EARLY_QEMU_EXIT
TIMEOUT
NO_SUCCESS_MARKER
```

这样 CI 中“boot failed”不会只剩一个模糊的 exit code。

### Milestone placement

`MYOS2_BOOT_OK` 应放在足够晚、又足够基础的位置。

它应该证明：

- kernel 已真正进入正常初始化路径；
- early fatal path 已经过；
- 系统能够执行到稳定控制点。

它不应该等待：

- 人工 shell input；
- 任意外部设备；
- 长时间 timer event；
- 网络。

### Harness

本任务提供：

`proposed/run-qemu-smoke.sh`

它：

- 启动任意由调用方提供的 QEMU command；
- 捕获 stdout/stderr；
- 轮询 success/fatal regex；
- success 后终止 guest；
- guest 提前退出时失败；
- timeout 时失败；
- 保留 log。

因此 QEMU 本身无需为测试专门“正常退出”。

## Phase C / L2: unit tests

L2 拆为两个类别。

### L2a host-pure

优先。

Eligibility rule:

```text
host testable =
  no privileged instructions
  AND no direct hardware access
  AND deterministic input/output
  AND dependency boundary can be represented cheaply
```

推荐 first-wave 数量：

```text
3–10 cases
```

而不是立即追求 coverage number。

选一个足够小、容易证明的 module，建立工程模板后再扩张。

编译建议：

```text
warnings enabled
warnings treated seriously
debug symbols
optional ASan/UBSan where compatible
```

sanitizer 结果只说明 host execution 的 C-level 问题，不能替代 guest 测试。

### L2b kernel-resident micro-tests

当目标依赖：

- privileged CPU behavior；
- true address-space setup；
- low-level interrupt state；
- kernel allocator/bootstrap state

时，测试必须进入 guest。

这些测试仍应尽量只启动测试所需的最少内核环境，并使用相同 result protocol。

## Phase D / L3: syscall functional tests

### Architecture

```text
QEMU
 └─ MyOS2 kernel
      └─ ordinary user test program
          ├─ syscall A
          ├─ syscall B
          └─ result reporter
```

严格禁止：

```text
test executable -> directly link kernel syscall implementation
```

因为这跳过真正需要验证的 ABI。

### First suite selection

从当前 snapshot 已支持、返回值易验证、依赖少的 syscall 中选择 2–4 个。

每个候选按以下矩阵设计：

| Case | Goal |
|---|---|
| basic valid | nominal path |
| boundary | zero/small/max safe boundary |
| invalid | kernel rejects bad request safely |
| sequence | state survives repeated/ordered use |

不要从 POSIX/Linux 猜测期望值；expected semantics 必须来自 MyOS2 当前 ABI。

### Result

guest user test 输出：

```text
MYOS2_TEST_BEGIN syscall
MYOS2_TEST_PASS syscall.foo.basic
MYOS2_TEST_PASS syscall.foo.boundary
MYOS2_TEST_FAIL syscall.foo.invalid expected=X actual=Y
MYOS2_TEST_END pass=2 fail=1
```

host harness 从同一 serial stream 判定。

## Phase E / L4: GitHub Actions

### Proposed jobs

最终目标：

```text
host-unit
build
qemu-boot-smoke
qemu-syscall
```

其中 image build 可以：

- 由每个 QEMU job 自己构建，优点是简单、隔离；
- 或 build once + artifact fan-out，优点是减少时间。

对 MyOS2 当前规模，第一版优先**简单和可诊断**，避免过早优化 CI runtime。

### Mandatory controls

每个 QEMU job：

```yaml
timeout-minutes: ...
```

workflow：

```yaml
permissions:
  contents: read
```

日志必须包含：

```text
BASE_SNAPSHOT
build command
QEMU command
test result
```

### Dependency installation

不要长期在 workflow 中散落大量版本不固定的安装逻辑。

第一版可以由 runner 安装工具；稳定后推荐：

```text
repository bootstrap script
or
documented toolchain container
```

以缩小 developer/CI 环境差异。

## Bug to regression policy

### Definition of done for kernel bug fix

一个可复现 bug 的修复 PR 应尽量包含：

```text
1. symptom
2. minimal reproducer
3. failing regression test
4. fix
5. passing regression test
```

### Layer selection

用最低有效层：

| Bug type | Preferred layer |
|---|---|
| pure calculation / parser / table logic | L2 host unit |
| privileged internal state | L2 guest kernel |
| syscall ABI / argument / return behavior | L3 |
| boot initialization | L1 specialized smoke |
| whole user→kernel sequence | L3 |

“最低有效层”不等于“永远 host test”。

测试必须真实经过 bug 所在的边界。

### Naming

建议：

```text
regression.<subsystem>.<issue-or-short-description>
```

示例只是格式，不代表 MyOS2 当前 subsystem：

```text
regression.foo.zero-length
```

### Never delete a regression test because the bug is fixed

回归测试的价值正是证明未来不会重新出现。

只有在：

- feature 被正式删除；
- contract 被正式改变；
- test 被等价、更强测试替换

时才删除。

## CI gating policy

建议分三个成熟阶段。

### Initial

Required:

```text
build
L1 smoke
```

Advisory:

```text
L2/L3 experimental
```

### Stabilized

Required:

```text
build
L1
stable L2
stable L3
```

### Mature

在稳定之后再考虑：

```text
multiple QEMU machine/config variants
compiler matrix
sanitizers
long stress tests
scheduled nightly jobs
```

不要在 smoke 稳定之前引入大 matrix。

## Flakiness policy

kernel CI 一旦 flaky，很快就会被团队忽略。

规则：

```text
No blind retry as success policy.
```

测试偶发失败时应该：

1. 保存 seed/command/log；
2. 分类为 deterministic bug、timing assumption 或 infrastructure；
3. 修复 root cause；
4. 必要时暂时 quarantine；
5. quarantine 必须仍然可见。

不要写：

```yaml
continue-on-error: true
```

然后永久不处理。

## Required evidence for L1–L4

### L1 evidence

```text
serial log includes MYOS2_BOOT_OK
harness exits 0
```

以及一个负向验证：

```text
wrong success regex => harness exits non-zero
```

### L2 evidence

至少一个 production pure implementation 被 host test 直接链接/调用。

必须证明测试没有自己的 duplicate implementation。

### L3 evidence

测试程序在 guest user mode 执行，并通过真实 syscall interface。

至少保存一次：

```text
MYOS2_TEST_BEGIN syscall
...
MYOS2_TEST_END pass=N fail=0
```

### L4 evidence

workflow：

```text
parses as valid YAML
has finite job timeout
has minimal read permission
runs L1/L2/L3 via explicit commands
fails on test failure
```

## Proposed CI integration seam

由于本任务不应猜测尚未冻结的 build/image/QEMU 路径，`proposed/ci.yml` 采用三个 repository variables：

```text
MYOS2_BUILD_CMD
MYOS2_QEMU_CMD
MYOS2_BOOT_OK
```

这是**proposal 接缝，不是长期最终架构**。

正式落地后应该把前两个迁入仓库脚本：

```text
scripts/ci/build.sh
scripts/ci/qemu.sh
```

最终 workflow 只调用这些稳定接口。

## Rollout order

最推荐的提交顺序：

```text
PR A:
  serial/test markers + deterministic build interface

PR B:
  L1 smoke harness

PR C:
  first L2 host unit test

PR D:
  syscall guest runner + first L3 cases

PR E:
  GitHub Actions required checks

future bug PRs:
  bug -> regression test -> fix
```

这样每一步都能独立验收。

## Risks

### False green

最大风险。

例如：

```text
QEMU launched
sleep 10
kill QEMU
exit 0
```

永远禁止。

### False red from text changes

解决方法是专用 marker，不 grep 普通 prose。

### Mock overgrowth

一个复杂硬件 mock 很可能产生自己的 bugs。

优先把 pure logic 与 side effect boundary 分开。

### CI-only behavior

必须让开发者在本地能运行与 Actions 同一个测试入口。

## External design sources

Linux KUnit:
`https://docs.kernel.org/dev-tools/kunit/`

Linux kselftest:
`https://docs.kernel.org/dev-tools/kselftest.html`

Linux KTAP:
`https://docs.kernel.org/dev-tools/ktap.html`

QEMU System Invocation:
`https://www.qemu.org/docs/master/system/invocation.html`

GitHub Actions Workflow Syntax:
`https://docs.github.com/actions/writing-workflows/workflow-syntax-for-github-actions`

## Open questions

1. 正式实施前必须把 `MYOS2_BUILD_CMD` 固定成仓库内真实 build script。
2. 正式实施前必须把 `MYOS2_QEMU_CMD` 固定成仓库内真实无头 QEMU command。
3. 必须从当前内核日志实现确定 `MYOS2_FATAL` 应在哪里、由哪个 fatal/panic path 输出。
4. 必须从当前 user ABI 选择 L3 的第一批 syscall；不能以 Linux syscall 语义替代 MyOS2 事实。
5. `time` 当前相对 master behind 3 的分支状态需要项目维护者另行处理；本任务不改分支历史。
```

## Proposed 实现件

**目标路径：`agent-workspace/results/MYOS2-DR-006/proposed/run-qemu-smoke.sh`**

```bash
#!/usr/bin/env bash
# base_snapshot: time@a039d9803893e0ce54164413aa8a0c29eacf5c78
#
# Generic bounded QEMU/headless smoke-test harness for MYOS2-DR-006.
#
# The script intentionally does NOT hard-code a MyOS2 image path or QEMU
# invocation that has not been frozen by this research task.
#
# Usage:
#
#   run-qemu-smoke.sh \
#       --timeout-seconds 45 \
#       --success-regex '^MYOS2_BOOT_OK$' \
#       --fatal-regex '^MYOS2_(FATAL|PANIC)' \
#       --log qemu-serial.log \
#       -- qemu-system-i386 ... -nographic ...
#
# Success:
#   success marker is observed before fatal/timeout/early process exit.
#
# Failure:
#   fatal marker, timeout, or process exit before the success marker.

set -euo pipefail

timeout_seconds=45
success_regex='^MYOS2_BOOT_OK$'
fatal_regex='^MYOS2_(FATAL|PANIC)'
log_path='qemu-smoke.log'
poll_seconds='0.20'

usage() {
    cat <<'EOF'
Usage:
  run-qemu-smoke.sh [options] -- <qemu command> [args...]

Options:
  --timeout-seconds N   Hard deadline in seconds (default: 45)
  --success-regex RX    Extended-regex success oracle
  --fatal-regex RX      Extended-regex fatal oracle
  --log PATH            Complete guest/QEMU output log
  -h, --help            Show this help
EOF
}

die() {
    printf 'MYOS2_HARNESS_ERROR %s\n' "$*" >&2
    exit 2
}

while (($#)); do
    case "$1" in
        --timeout-seconds)
            (($# >= 2)) || die "--timeout-seconds requires a value"
            timeout_seconds="$2"
            shift 2
            ;;
        --success-regex)
            (($# >= 2)) || die "--success-regex requires a value"
            success_regex="$2"
            shift 2
            ;;
        --fatal-regex)
            (($# >= 2)) || die "--fatal-regex requires a value"
            fatal_regex="$2"
            shift 2
            ;;
        --log)
            (($# >= 2)) || die "--log requires a value"
            log_path="$2"
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        --)
            shift
            break
            ;;
        *)
            die "unknown option: $1"
            ;;
    esac
done

(($# > 0)) || die "missing QEMU command after --"

[[ "$timeout_seconds" =~ ^[1-9][0-9]*$ ]] \
    || die "--timeout-seconds must be a positive integer"

mkdir -p "$(dirname "$log_path")"
: >"$log_path"

qemu_pid=''

cleanup() {
    if [[ -n "${qemu_pid:-}" ]] && kill -0 "$qemu_pid" 2>/dev/null; then
        kill "$qemu_pid" 2>/dev/null || true

        # Give QEMU a brief opportunity to leave cleanly.
        for _ in 1 2 3 4 5; do
            if ! kill -0 "$qemu_pid" 2>/dev/null; then
                break
            fi
            sleep 0.1
        done

        if kill -0 "$qemu_pid" 2>/dev/null; then
            kill -KILL "$qemu_pid" 2>/dev/null || true
        fi

        wait "$qemu_pid" 2>/dev/null || true
    fi
}

trap cleanup EXIT INT TERM

printf 'MYOS2_HARNESS_START timeout=%s log=%s\n' \
    "$timeout_seconds" "$log_path"

printf 'MYOS2_HARNESS_COMMAND'
printf ' %q' "$@"
printf '\n'

# Redirect the complete QEMU stream to a persistent log.  CI may print it on
# failure and/or retain it as an artifact.
"$@" >"$log_path" 2>&1 &
qemu_pid=$!

start_seconds=$SECONDS

while true; do
    # Fatal wins if it appears before the success condition is accepted.
    if grep -Eq "$fatal_regex" "$log_path"; then
        printf 'MYOS2_HARNESS_FAIL reason=fatal-marker\n' >&2
        cat "$log_path" >&2
        exit 1
    fi

    if grep -Eq "$success_regex" "$log_path"; then
        printf 'MYOS2_HARNESS_PASS reason=success-marker\n'
        cat "$log_path"
        exit 0
    fi

    if ! kill -0 "$qemu_pid" 2>/dev/null; then
        set +e
        wait "$qemu_pid"
        qemu_rc=$?
        set -e

        qemu_pid=''

        # Close a tiny race where the final bytes were written immediately
        # before process exit.
        if grep -Eq "$fatal_regex" "$log_path"; then
            printf 'MYOS2_HARNESS_FAIL reason=fatal-marker qemu_rc=%s\n' \
                "$qemu_rc" >&2
        elif grep -Eq "$success_regex" "$log_path"; then
            printf 'MYOS2_HARNESS_PASS reason=success-marker qemu_rc=%s\n' \
                "$qemu_rc"
            cat "$log_path"
            exit 0
        else
            printf 'MYOS2_HARNESS_FAIL reason=early-qemu-exit qemu_rc=%s\n' \
                "$qemu_rc" >&2
        fi

        cat "$log_path" >&2
        exit 1
    fi

    elapsed=$((SECONDS - start_seconds))

    if ((elapsed >= timeout_seconds)); then
        printf 'MYOS2_HARNESS_FAIL reason=timeout elapsed=%s\n' \
            "$elapsed" >&2
        cat "$log_path" >&2
        exit 124
    fi

    sleep "$poll_seconds"
done
```

**目标路径：`agent-workspace/results/MYOS2-DR-006/proposed/test-contract.md`**

```markdown
---
task_id: MYOS2-DR-006
deliverable: proposed-test-contract
base_snapshot:
  branch: time
  commit: a039d9803893e0ce54164413aa8a0c29eacf5c78
  tree: fa8d26ffa2cf2b827c183452039a0b780f82eab2
status: proposed
---

# MyOS2 Test Contract

## Purpose

本文件定义 CI、kernel test 和 user-space syscall test 之间的最小稳定协议。

它有意不规定 MyOS2 尚未在本任务中冻结的内部实现路径。

## Output protocol

### Boot

Kernel 到达 CI 认可的启动 milestone：

```text
MYOS2_BOOT_OK
```

该字符串属于 automation ABI。

如果显示文案需要更友好，应另发普通日志，例如：

```text
Welcome to MyOS2
MYOS2_BOOT_OK
```

CI 只依赖第二行。

## Fatal

统一格式：

```text
MYOS2_FATAL <class> [details...]
```

如果现有 panic infrastructure 需要保留：

```text
MYOS2_PANIC [details...]
```

则 harness 可以同时匹配：

```regex
^MYOS2_(FATAL|PANIC)
```

不要让 CI 依赖普通 panic prose 的完整句子。

## Test suite

```text
MYOS2_TEST_BEGIN <suite>
MYOS2_TEST_PASS <case>
MYOS2_TEST_FAIL <case> <diagnostic>
MYOS2_TEST_END pass=<N> fail=<N>
```

Example:

```text
MYOS2_TEST_BEGIN syscall
MYOS2_TEST_PASS syscall.example.basic
MYOS2_TEST_FAIL syscall.example.invalid expected=-1 actual=0
MYOS2_TEST_END pass=1 fail=1
```

## Case identifiers

Case name 应：

- 稳定；
- 不包含空格；
- 描述 behavior 而非代码行号；
- bug regression 有独立名称。

Suggested grammar:

```text
<layer>.<subsystem>.<behavior>
```

Regression:

```text
regression.<subsystem>.<short-description>
```

## Host-unit eligibility contract

一个 kernel component 只有满足以下要求才应进入 host-pure unit：

```text
[ ] no privileged instruction is executed by test
[ ] no real MMIO/PIO is required
[ ] no guest address-space semantics are being claimed
[ ] result is deterministic from explicit inputs/state
[ ] production implementation is shared with the test
```

最后一条非常重要。

禁止：

```text
production algorithm A
test-only rewritten algorithm A'
test passes
=> claim A is tested
```

必须：

```text
same implementation A
        ↙      ↘
 kernel        host test
```

## Hardware boundary rule

对带 side effect 的模块，建议：

```text
pure decision logic
        |
        +--- tested on host

small hardware adapter
        |
        +--- tested in QEMU/guest
```

不要 mock 整个 x86 machine。

## Syscall test contract

syscall functional test 必须：

```text
[ ] execute as a normal guest user program
[ ] call the normal MyOS2 syscall entry/wrapper
[ ] pass through real kernel dispatch
[ ] observe return in user mode
[ ] emit structured result
```

不得：

```text
[ ] directly call kernel implementation and call that an ABI test
```

### Initial syscall test matrix

具体 syscall 名称从 MyOS2 当前 ABI 选择。

对每一个被选 syscall：

```text
basic:
    smallest unambiguously valid request

boundary:
    a documented boundary that does not depend on undefined behavior

invalid:
    a documented invalid request where kernel has a defined response

sequence:
    two or more invocations where state/ordering matters, if applicable
```

expected result 必须以 MyOS2 当前定义为准，不能复制 Linux errno 或 POSIX 行为作为默认答案。

## Test process exit versus kernel state

guest test 的 process exit code 可以作为一个 signal，但不得作为唯一 oracle。

host harness 以：

```text
explicit result protocol
+
fatal detection
+
hard timeout
```

决定测试结果。

这能覆盖：

- guest test process 无法正常 exit；
- kernel 在结果之后 idle；
- kernel 在 test 中 hang；
- emulator 异常退出。

## Timeout hierarchy

建议有两层 timeout：

```text
inner harness:
    30–60 seconds for one smoke/test guest

outer GitHub job:
    several minutes
```

内层 timeout 负责精确诊断 guest hang。

外层 timeout 是 runner 最终安全阀。

## Bug-to-regression contract

每个可复现 bug：

```text
Step 1: construct the smallest valid reproducer
Step 2: choose the lowest layer that still reproduces the real failure
Step 3: commit/add test and demonstrate FAIL on bad behavior
Step 4: apply fix
Step 5: demonstrate PASS
Step 6: retain test
```

### Choosing the layer

```text
pure deterministic helper bug
    => host unit

privileged kernel-only logic
    => guest kernel test

user-visible syscall bug
    => syscall functional

boot regression
    => specialized boot smoke
```

不要为了“测试快”而选择一个绕过实际 bug path 的层级。

## CI log contract

每个 guest invocation 至少输出：

```text
BASE_SNAPSHOT=<branch>@<commit>
QEMU_COMMAND=<command>
RESULT=<pass|fail>
LOG=<path>
```

对 source SHA 的记录应来自实际 checkout：

```bash
git rev-parse HEAD
```

而不是只相信 branch name。

## Test ownership

测试 marker 是稳定接口。

普通 debug output 不是。

因此：

```text
changing:
  "initializing scheduler ..."
to:
  "scheduler init ..."
```

不应破坏 CI。

但是删除：

```text
MYOS2_BOOT_OK
```

应被视为修改 automation contract。

## Relationship to external practice

本协议采用 KUnit/kselftest/KTAP 的核心思想：

- unit 与 end-to-end interface test 分层；
- test result 是结构化协议；
- guest-visible behavior 从真实边界验证。

References:

- `https://docs.kernel.org/dev-tools/kunit/`
- `https://docs.kernel.org/dev-tools/kselftest.html`
- `https://docs.kernel.org/dev-tools/ktap.html`

QEMU headless execution model：

- `https://www.qemu.org/docs/master/system/invocation.html`

## Open questions

- 现有 MyOS2 logging API 与 serial driver 的最小改动点需在实现任务确认。
- syscall guest test 如何加入 boot artifact 取决于当前 user-program build/packaging path。
- 第一批 host-pure target 需要在实现任务对 `time@a039d980...` 做逐函数可测试性审查。
```

**目标路径：`agent-workspace/results/MYOS2-DR-006/proposed/ci.yml`**

```yaml
# base_snapshot: time@a039d9803893e0ce54164413aa8a0c29eacf5c78
#
# Proposed GitHub Actions workflow for MYOS2-DR-006.
#
# This is intentionally an integration proposal.  The research task does not
# invent repository-specific build/image paths that have not been frozen.
#
# Before activation, configure/freeze:
#
#   MYOS2_BUILD_CMD
#   MYOS2_QEMU_CMD
#   MYOS2_BOOT_OK
#
# Better final state:
# move BUILD_CMD and QEMU_CMD into repository-owned scripts and call the
# scripts here, so local developers and CI execute the same entry points.

name: myos2-kernel-ci-proposal

on:
  workflow_dispatch:
  push:
    branches:
      - master
      - time
  pull_request:
    branches:
      - master

permissions:
  contents: read

env:
  BASE_SNAPSHOT: "time@a039d9803893e0ce54164413aa8a0c29eacf5c78"

jobs:
  proposal-sanity:
    name: Proposal syntax and harness sanity
    runs-on: ubuntu-latest
    timeout-minutes: 5

    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Show revision
        shell: bash
        run: |
          set -euo pipefail
          echo "proposal_base=${BASE_SNAPSHOT}"
          echo "checked_out=$(git rev-parse HEAD)"

      - name: Validate smoke harness shell syntax
        shell: bash
        run: |
          set -euo pipefail
          bash -n \
            agent-workspace/results/MYOS2-DR-006/proposed/run-qemu-smoke.sh

  host-unit:
    name: L2 host unit tests
    runs-on: ubuntu-latest
    timeout-minutes: 10

    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Run repository-owned host unit entry point
        shell: bash
        env:
          MYOS2_HOST_TEST_CMD: ${{ vars.MYOS2_HOST_TEST_CMD }}
        run: |
          set -euo pipefail

          if [[ -z "${MYOS2_HOST_TEST_CMD}" ]]; then
            echo "::error::MYOS2_HOST_TEST_CMD is not frozen yet."
            echo "::error::Replace this variable with a repository-owned test script before making the workflow required."
            exit 2
          fi

          echo "BASE_SNAPSHOT=${BASE_SNAPSHOT}"
          echo "checked_out=$(git rev-parse HEAD)"
          bash -lc "${MYOS2_HOST_TEST_CMD}"

  qemu-boot-smoke:
    name: L1 QEMU headless boot smoke
    runs-on: ubuntu-latest
    timeout-minutes: 10

    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Build boot artifact
        shell: bash
        env:
          MYOS2_BUILD_CMD: ${{ vars.MYOS2_BUILD_CMD }}
        run: |
          set -euo pipefail

          if [[ -z "${MYOS2_BUILD_CMD}" ]]; then
            echo "::error::MYOS2_BUILD_CMD is not frozen yet."
            exit 2
          fi

          echo "BASE_SNAPSHOT=${BASE_SNAPSHOT}"
          echo "checked_out=$(git rev-parse HEAD)"
          bash -lc "${MYOS2_BUILD_CMD}"

      - name: Run bounded QEMU smoke
        shell: bash
        env:
          MYOS2_QEMU_CMD: ${{ vars.MYOS2_QEMU_CMD }}
          MYOS2_BOOT_OK: ${{ vars.MYOS2_BOOT_OK }}
        run: |
          set -euo pipefail

          if [[ -z "${MYOS2_QEMU_CMD}" ]]; then
            echo "::error::MYOS2_QEMU_CMD is not frozen yet."
            exit 2
          fi

          if [[ -z "${MYOS2_BOOT_OK}" ]]; then
            echo "::error::MYOS2_BOOT_OK is not frozen yet."
            exit 2
          fi

          mkdir -p ci-logs

          echo "QEMU command:"
          printf '%s\n' "${MYOS2_QEMU_CMD}"

          agent-workspace/results/MYOS2-DR-006/proposed/run-qemu-smoke.sh \
            --timeout-seconds 45 \
            --success-regex "${MYOS2_BOOT_OK}" \
            --fatal-regex '^MYOS2_(FATAL|PANIC)' \
            --log ci-logs/qemu-boot.log \
            -- bash -lc "${MYOS2_QEMU_CMD}"

      - name: Print boot log
        if: always()
        shell: bash
        run: |
          if [[ -f ci-logs/qemu-boot.log ]]; then
            cat ci-logs/qemu-boot.log
          fi

  qemu-syscall:
    name: L3 syscall functional tests
    runs-on: ubuntu-latest
    timeout-minutes: 10

    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Build syscall-test boot artifact
        shell: bash
        env:
          MYOS2_SYSCALL_BUILD_CMD: ${{ vars.MYOS2_SYSCALL_BUILD_CMD }}
        run: |
          set -euo pipefail

          if [[ -z "${MYOS2_SYSCALL_BUILD_CMD}" ]]; then
            echo "::error::MYOS2_SYSCALL_BUILD_CMD is not frozen yet."
            echo "::error::It must build a guest image containing the real user-mode syscall test."
            exit 2
          fi

          echo "BASE_SNAPSHOT=${BASE_SNAPSHOT}"
          echo "checked_out=$(git rev-parse HEAD)"
          bash -lc "${MYOS2_SYSCALL_BUILD_CMD}"

      - name: Run syscall functional guest
        shell: bash
        env:
          MYOS2_SYSCALL_QEMU_CMD: ${{ vars.MYOS2_SYSCALL_QEMU_CMD }}
        run: |
          set -euo pipefail

          if [[ -z "${MYOS2_SYSCALL_QEMU_CMD}" ]]; then
            echo "::error::MYOS2_SYSCALL_QEMU_CMD is not frozen yet."
            exit 2
          fi

          mkdir -p ci-logs

          agent-workspace/results/MYOS2-DR-006/proposed/run-qemu-smoke.sh \
            --timeout-seconds 60 \
            --success-regex '^MYOS2_TEST_END pass=[0-9]+ fail=0$' \
            --fatal-regex '^(MYOS2_(FATAL|PANIC)|MYOS2_TEST_FAIL )' \
            --log ci-logs/qemu-syscall.log \
            -- bash -lc "${MYOS2_SYSCALL_QEMU_CMD}"

      - name: Print syscall log
        if: always()
        shell: bash
        run: |
          if [[ -f ci-logs/qemu-syscall.log ]]; then
            cat ci-logs/qemu-syscall.log
          fi
```

## 验收结论与开放问题

按 `MYOS2-DR-006` 的目标，这套交付物把四层测试责任明确分开：**L1** 由 `run-qemu-smoke.sh` 实现有界、无头、基于机器标记的 QEMU 启动判定；**L2** 规定只把真正可隔离且复用生产实现的逻辑放到 host unit，并为 CI 保留独立 job；**L3** 明确要求测试从 guest user mode 穿过真实 syscall ABI，不允许直接调用内核 implementation 冒充 syscall test；**L4** 由 `ci.yml` 给出合法 GitHub Actions 结构、最小 `contents: read` 权限、独立 timeout 和按层失败域。设计依据分别对应 Linux KUnit、kselftest/KTAP、QEMU system emulation 和 GitHub Actions 官方文档。

最关键的工程结论是：**MyOS2 现在最值得先稳定的不是测试数量，而是 build、headless console 和 result marker 三个自动化接口。** 没有这三个接口，CI 最终只能依赖 sleep、VGA 文本或不可靠的 QEMU 进程状态；有了它们，启动测试、syscall 测试和 bug 回归都可以复用同一套 harness。

此次只读研究没有将尚未确认的 build artifact 路径、QEMU 命令或具体 syscall 名称伪造为仓库事实，因此 `proposed/ci.yml` 有意把这些位置留成显式接线点。正式实现时最重要的 `open_questions` 是：确定 `time@a039d9803893e0ce54164413aa8a0c29eacf5c78` 的唯一 build/QEMU 入口、选择现有 kernel logging 中最合适的 serial CI sink、从实际 MyOS2 ABI 选择首批 syscall case，以及从实际 kernel 文件中挑出第一批真正 host-pure 的 L2 单元；这些都不应通过套用 Linux/POSIX 语义来猜测。

此外，本次读取揭示了一个应写入后续集成记录的分支事实：`time` 在读取时相对当前 `master` **ahead 44，同时 behind 3**。因此成果分支仍应按任务约定从 `master` 创建，但测试研究的 `base_snapshot` 必须继续锁定 `time@a039d9803893e0ce54164413aa8a0c29eacf5c78`；不要在后续 PR 中把“成果文件的父分支”与“被分析内核源码的基线”混为一谈。