---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-MEMDEV-SCALAR-IO-115"
record_type: "independent_successor_document_correction_review"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro（沿用 Owner 在本主线已明确选择）"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "a5b40470646a"
execution_pin_short12: "452444a1c4b4"
status: "CLOSED_DOCUMENTATION_ONLY"
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
R114_01: "CLOSED_DOCUMENTATION_ONLY"
R114_02: "CLOSED_DOCUMENTATION_ONLY"
batch114_composite: "PASS_PENDING_LOCAL"
batch115: "RETURN"
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FILE-SEEK-POSITION-114-review.md","agent-workspace/lead/MYOS2-LEAD-002/164-user-memdev-scalar-io-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114/file-seek-position.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114/file-seek-position-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io-paths.dot","mykernel/scripts/options_flags.cmake","mykernel/arch/x86_64/include/asm/syscalls_64.h","mykernel/arch/x86_64/entry/syscall_64.c","mykernel/include/linux/kernel/syscalls.h","mykernel/arch/x86_64/include/asm/syscall_wrapper.h","mykernel/drivers/char/char_dev.c","mykernel/fs/syscall.c","mykernel/fs/fat/myos_fat32.c"]
---

# 第114批两项补正的独立裁定

R114-01、R114-02 均 `CLOSED_DOCUMENTATION_ONLY`。固定114原件、115追加差量与本回执合读，第114批为 `PASS_PENDING_LOCAL`。第115批自己的三项退回见同期独立验收。[114原裁定](https://github.com/08822407d/MyOS2/blob/a5b40470646a/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FILE-SEEK-POSITION-114-review.md) · [作者补正](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml#L17) · [115正式交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7044709177)

### R114-01 · 映射实例与实际生成的桩

旧 FS114-E003（syscalls_64.h:11）是映射调用实例，E004 对应活动宏定义，E044 落在完整的 def_chr_fops 聚合定义。作者58个逐字窗口、57个原定义合格、原列28个普通位置及另列的E003重分类均可回查。115报告分阶段表、facts.mapped_steps / consumer_repairs 与图中有类型的边补正了旧报告41/55、旧 facts.A.dispatch_chain 与旧图89–91。[原图](https://github.com/08822407d/MyOS2/blob/a2dd370fcf15/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114/file-seek-position-paths.dot#L89-L91) · [映射实例](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscalls_64.h#L11) · [活动 switch](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/entry/syscall_64.c#L26-L31) · [完整聚合](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/char_dev.c#L95-L98)

固定 C 设置从第58行 `set(CMAKE_C_FLAGS …)` 开始，第65行开包装器，第66行定义 CONFIG_BUG。syscalls.h:93–102选体系头，1386行选生成宏，241–244行调用桩宏。104–109行的 long 桩实际调用并返回 __se，245–249行再调用 __do 并返回。C08独立补录正确大小写的设置名。115作者E001的原文正确，其标签误拼另计R115-02。旧114所缺的配置及包装链已经补足。[配置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L58-L66) · [包含门](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L93-L102) · [入口宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L1386) · [生成与返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241-L251)

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```
[options_flags.cmake L64–66](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L64-L66)

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__SYS_STUBx]
```c
				long __##abi##_##name(const pt_regs_s *regs);	\
				long __##abi##_##name(const pt_regs_s *regs)	\
				{												\
					return __se_##name(regs);					\
```
[syscall_wrapper.h L105–108](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L105-L108)

### R114-02 · 上界判断与最终位置

达到 fd 上界时，比较已经读取 curr->files->fd_count，随后返回 -EBADF。数组槽、操作表、llseek 调用和位置赋值在其后。旧 facts:389 与115的 executed_before_reject / skipped_after_reject 可逐项对应。[原字段](https://github.com/08822407d/MyOS2/blob/a2dd370fcf15/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114/facts-and-boundaries.yaml#L382-L415) · [入口判断](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/syscall.c#L78-L90)

逐项比对冻结 B01–B11 的 origin、offset、返回、写位与最后位置。SET/CUR/END 被识别后，候选在可表示条件下计算，再经闭区间判断。旧位40、文件大小100时，B04/B10写100，B05/B06/B07/B11返回 -EOVERFLOW 且保持40。DATA 被后端拒绝，HOLE 先被入口拒绝，另外三个成功行 B01/B02/B03 分别写0/45/0。旧 A6／B11／C5 的适用前提保留。[补正后的FAT判定](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml#L143) · [后端范围门](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/fat/myos_fat32.c#L283-L308)

[VERIFIED mykernel/fs/syscall.c::MYOS_SYSCALL_DEFINE3(lseek)]
```c
	if(fd < 0 || fd >= curr->files->fd_count)
		return -EBADF;
	if(whence < 0 || whence >= SEEK_MAX)
		return -EINVAL;
```
[syscall.c L82–85](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/syscall.c#L82-L85)

本裁定限于文档与有界源码模型。编译后的包装、运行行为、完整本地P9均待本地验证。
