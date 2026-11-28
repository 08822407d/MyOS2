---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-DIR-STREAM-LS-116"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-17"
executor_display_name: "MYOS2-A-C07c 代码分析执行"
record_type: "batch116_executor_formal_manifest"
produced_by: "GPT-6 family; backend effort not independently attested"
model_per_owner: "次一档 GPT-6＋Extra"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_contract_short12: "e96b1079ff0d"
execution_parent_short12: "452444a1c4b4"
canonical_write_branch: "claude/dazzling-cori-q0dnyt"
canonical_pr: 17
status: "READY_FOR_MAINLINE_REVIEW"
mainline_acceptance: "PENDING"
D00: "CORRECTION_SUBMITTED_PENDING_C08_REVIEW"
R115_01: "OPEN"
R115_02: "OPEN"
R115_03: "OPEN"
batch115: "RETURN"
R114_01: "CLOSED_DOCUMENTATION_ONLY"
R114_02: "CLOSED_DOCUMENTATION_ONLY"
batch114: "PASS_PENDING_LOCAL"
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
open_questions: []
self_check: {verified_claims: 62, quotes_reconfirmed: 62, downgraded_to_inferred: 0}
write_retry_log: "not_needed for initial blobs; tree/ref/comment and post-readback logged in sole formal PR17 comment"
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/165-user-dir-stream-ls-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/164-user-memdev-scalar-io-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/159-executor-continuation/01-MYOS2-A-C07c-receiver-startup-prompt.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MEMDEV-SCALAR-IO-115-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MEMDEV-SCALAR-IO-115-D00-114-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIRECTORY-ENUMERATION-59-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-21-memdev115-reviewed-dir-stream116.md"
  - "agent-workspace/conventions.md"
  - "agent-workspace/tasks/00-gpt-task-protocol-v2.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/MANIFEST.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io-paths.dot"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/deferred-findings.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/evidence.md"
  - "mykernel/scripts/options_flags.cmake"
  - "mykernel/include/linux/kernel/limits.h"
  - "mykernel/arch/x86_64/mm/page/page_const_arch.h"
  - "mykernel/include/linux/kernel/asm-generic/bug.h"
  - "mykernel/fs/vfs/read_write.c"
  - "mykernel/drivers/char/mem.c"
  - "myinitramfs/myshell/myshell.c"
  - "myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c"
  - "myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h"
  - "myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c"
  - "myinitramfs/mylib/myglibc/syscall/syscall.c"
  - "mykernel/arch/x86_64/include/asm/unistd_64.h"
  - "mykernel/include/linux/kernel/dirent.h"
  - "mykernel/fs/vfs/readdir.c"
  - "myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent.h"
  - "mykernel/debug/panic.c"
  - "mykernel/arch/x86_64/include/asm/syscalls_64.h"
  - "mykernel/arch/x86_64/entry/syscall_64.c"
  - "mykernel/fs/fat/myos_fat32.c"
  - "myinitramfs/mylib/myglibc/dirent/dirent.h"
  - "myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent_ext.h"
  - "myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/closedir.c"
---

# 第116批接收与验证清单

## 原身份接收与指导刷新
Owner同时转发165合同与C08正式回执7056268192，授权同一原执行会话CHAT-EXEC-17在原PR17完成本批六件。九份MyOS2项目约束和110/124已在本对话首次接收时独立读取；当前C08主线从115发布到165仅新增两份审核、检查点及合同四份，适用原规则未改。此次63规则全文重新核读，协议P1–P13及28/79/110/124定点更新。Mnemosyne直接master本轮e581eb9df6c4，等于上批直接读头；任务局部延续而非另行开始Mnemosyne维护。未虚报本轮重新读全部九项或后台effort。

## fresh G00：实际GitHub直接对象
lead=e96b1079ff0d；exec=452444a1c4b4；time=a039d9803ade；direct master=e8e60cd19540。PR16 Open/Ready、PR17原题Open/Draft，base master，均未合并，开放列表仅此两PR。115固定/活动六件完整文本和Blob均6/6一致；116目标六个新路径最初全部缺席。
PR17顶层无since分页100+100+18+0，共218条。164 C08发布评论7043832344、115作者正式交付7044709177、116 C08正式发布7056268192各出现一次；116全作者正式交付初始0。direct master公约原句在本清单头部。time编译模型kernel/no-pie/no-pic、master模型large/fPIE与panic两分支smp_processor_id/raw_smp_processor_id逐句回源。time C配置wrapper在65行、CONFIG_BUG在66行。
115三项OPEN、批次RETURN；114两项已CLOSED_DOCUMENTATION_ONLY，组合PASS_PENDING_LOCAL。不可由本执行者改变独立审核状态。

## 六件交付与模型范围
MANIFEST.md为接收、P1–P13及G00；dir-stream-ls.md为D00三纠正和A/B/C；facts-and-boundaries.yaml为精确键/有限分支/资源与位置；dir-stream-ls-paths.dot为分类边与缓冲更新；deferred-findings.yaml列首个未证事项；evidence.md含本批62个独立短窗与27个普通位置，旧115的70原文/69符号/72前缀分别冻结。
A四种opendir结果（正常、空路径、负fd但malloc成功、有效fd但malloc失败）。B以三条有效记录24/24/32构成80字节批次，三次读项只做一次getdents64，第四次才返0或负；另核缓存命中。C记录ls过滤、EOF/error汇流、空closedir实现和资源证据边界。

## P1–P13静态预检边界
P1：六件没有40位十六进制串；SHA只截12位写入交付文档。P2/P5/P6：62个新增标签必须从fixed time逐字取1–5行且单一定义归属，新标签仅在evidence一处出现专属短引文；27处普通位置单列，未经编译的布局/条件宏不虚报。
P3/P4：four direct heads、master公约、time/master金丝雀、实际C配置逐回读；否定观察限定所读完整函数与注释行，不扩大到整个运行系统。
P7/P8：原旧件不修改，本批六个新文件必须由一个单父增量提交完成；零旧改删，保留C08主线审核权。
P9文本子集：逐字与路径/符号/真实行界、源ID、各表Markdown列宽、D00旧23缺漏、记录24/24/32和游标0/24/48/80、DOT全部声明端点、62新标签/27普通位置、mainline状态验证。完整本地机械P9仍NOT_RUN。
P10–P13：纯文档，无补丁、编译/链接、仓库脚本、Graphviz、真实shell/getdents64、QEMU/实机或新付费服务。真实运行结果不冒充纸面推演。
63临时写异常应先查远端是否已写，再按服务端Retry-After或10/20/40秒间隔最多3次重试；expected_sha真实父、force=false，未知并行写冲突停该写动作，不重复发评论。实际成功路径写retry not_needed。

## 作者交回状态
READY_FOR_MAINLINE_REVIEW；mainline_acceptance=PENDING；D00=CORRECTION_SUBMITTED_PENDING_C08_REVIEW。R115-01/02/03仍OPEN、第115批RETURN。仅完成六件固定提交和PR17唯一本人正式评论以及全量远端回读后STOP_WRITING，交回C08用Pro独立验收并同轮安排下一批。
