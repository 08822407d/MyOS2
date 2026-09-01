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
