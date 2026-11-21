---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-DIR-STREAM-LS-116"
record_type: "independent_115_correction_review"
status: "PARTIALLY_CLOSED_DOCUMENTATION_ONLY"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro（Owner选择）"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
phase: 2
read_channel: "connector"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "e96b1079ff0d"
execution_pin_short12: "145d6781c75e"
batch115: "RETURN"
R115_01: "CLOSED_DOCUMENTATION_ONLY"
R115_02: "CLOSED_DOCUMENTATION_ONLY"
R115_03: "OPEN"
batch116: "RETURN"
R116_01: "OPEN"
R116_02: "OPEN"
batch114_composite: "PASS_PENDING_LOCAL"
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MEMDEV-SCALAR-IO-115-review.md","agent-workspace/lead/MYOS2-LEAD-002/165-user-dir-stream-ls-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/dir-stream-ls.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/dir-stream-ls-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io.md","mykernel/scripts/options_flags.cmake","mykernel/include/linux/kernel/limits.h","mykernel/arch/x86_64/mm/page/page_const_arch.h","mykernel/include/linux/kernel/asm-generic/bug.h","mykernel/fs/vfs/read_write.c","mykernel/drivers/char/mem.c"]
---

# 第115批 D00 独立裁定

R115-01、R115-02 文档闭合。R115-03 仍为 `OPEN`，第115批整体仍为 `RETURN`。裁定适用于固定115原件与固定116追加差量的合读。[115原裁定](https://github.com/08822407d/MyOS2/blob/e96b1079ff0d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MEMDEV-SCALAR-IO-115-review.md) · [115固定六件](https://github.com/08822407d/MyOS2/tree/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115) · [116固定六件](https://github.com/08822407d/MyOS2/tree/145d6781c75e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116) · [116作者交付 #7056533291](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7056533291)

## R115-01 · 三张表已补全

复核替代表的8、7、9列，按行对照旧 facts 和新 D00。原23个缺值位置均已填实。按请求 0、1、4095、4096、4097、8192、8193 排列，末块漏计依次为 0、1、4095、4096、1、4096、1，七个哨兵均保持。单页及跨页的三步余量依次为0、1、0。六格均写明回调或缺方法，模式位表达式可在一个表格单元格内读取。九个位置例、七个长度例、六格设备结果继续按原前提消费。[116替代表](https://github.com/08822407d/MyOS2/blob/145d6781c75e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/dir-stream-ls.md#L11-L42)

## R115-02 · 标签、计数与字段映射已纠正

旧 evidence 有70个逐字窗口，69个原标签的定义归属正确。旧六件共有72次正式前缀，两次在 evidence 之外。新 E001/E002 指向 `set(CMAKE_C_FLAGS)` 的开始和包装器定义。在32位 int、4 KiB页的模型下，`INT_MAX & PAGE_MASK` 为2147479552。复核 A.read_VFS、A.write_VFS、A.MAX_RW_COUNT、C.operations.zero 与 C.installation 五处，新映射对应模式与访问门、验证和限长、方法选择、zero 表和实际安装链。宏定义、表项、调用和位置写回分开索引。[D00证据映射](https://github.com/08822407d/MyOS2/blob/145d6781c75e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/facts-and-boundaries.yaml#L266-L389)

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```
[options_flags.cmake L64–66](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L64-L66)

## R115-03 · 迭代返回条件已补，图仍缺边

追加文字正确记录同步迭代返回后的 `BUG_ON(ret == -EIOCBQUEUED)`，并区分随后的读位置写回与写正返回写回。facts 所列六组修改覆盖115图83、84–89、97、107、109–120、132行。实际116图53–55行仅把旧链指向 `ksys_read`、`ksys_write` 和保护节点，两处 ksys 节点均无出边。图中缺分派/截断、外层位置写回、fops 绑定和 read_zero 循环，无法从入口追踪零次、一次或跨页返回。[拟修改的六组边](https://github.com/08822407d/MyOS2/blob/145d6781c75e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/facts-and-boundaries.yaml#L390-L436) · [已提交的图](https://github.com/08822407d/MyOS2/blob/145d6781c75e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/dir-stream-ls-paths.dot#L51-L55) · [115原图](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io-paths.dot#L83-L132)

[VERIFIED mykernel/fs/vfs/read_write.c::new_sync_read]
```c
	ret = filp->f_op->read_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ppos)
		*ppos = kiocb.ki_pos;
	return ret;
```
[read_write.c L88–92](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L88-L92)

[VERIFIED mykernel/drivers/char/mem.c::read_zero]
```c
		memset(buf + cleared, 0, chunk);
		count -= chunk;
		if (count == 0)
			break;
```
[mem.c L90–93](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/mem.c#L90-L93)

**关闭条件**　在117实际 DOT 中追加115子图，每组给出旧行、现节点与边、边类型和源ID。标清 ksys 调用者、各方向验证和限长、局部位置与外层写回、迭代特殊返回门，以及 zero_fops 指向唯一 read_zero 实现。用 0、P、P+1（P=4096）从入口遍历至返回 0、0、4096，并对照实际清零 0、4096、4097。C08 回读实际边及证据后裁定。

完整本地P9、预处理、编译和运行均 `NOT_RUN`。
