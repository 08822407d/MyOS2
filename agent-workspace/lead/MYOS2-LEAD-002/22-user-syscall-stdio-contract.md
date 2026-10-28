---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: local_user_syscall_stdio_static_analysis_contract
conversation_display_name: "MYOS2-A-C02 内核分析主线"
produced_by: "newest gpt6"
produced_by_source: "Owner已告知的网页端显示名；非本轮直接观察。"
model_per_owner: gpt6
date: 2026-10-28
base_snapshot: "kernel=time；workspace=master；instructions=agent/MYOS2-LEAD-002；results=claude/dazzling-cori-q0dnyt"
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-SYSCALL-STDIO-08
required_review_record: CORE-USER-BOUNDARIES-07-REVIEW-001
review_ref: agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-BOUNDARIES-07-review.md
findings_policy_ref: agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md
reviewed_execution_short12: ab3b4b86662c
kernel_source_short12: a039d9803ade
execution_branch: claude/dazzling-cori-q0dnyt
execution_session_display_name: "MYOS2-A-CC02 内核分析本地执行"
initial_execution_pr: 17
allowed_write_prefix: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-syscall-stdio-08/
existing_files_frozen: "PR17当前全部旧文件；07与06交接原件不改。"
status: READY_FOR_OWNER_FORWARDING
external_execution_started: false
local_billing_requirement: subscription_only
paid_fallback_authorized: false
kernel_change_authorized: false
kernel_patch_production_authorized: false
host_fixture_execution_authorized: false
full_kernel_build_authorized: false
qemu_authorized: false
phase3_authorized: false
acceptance_ceiling: PASS_PENDING_LOCAL
read_channel: connector
evidence_class: "限定静态源码分析；不预判libc行为、运行时系统调用或内核正确性。"
self_check: {scope: this_file_only, verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
owner_decisions_requested_now: []
open_questions:
  - "initramfs内用户程序与libc源码是否完整随仓库存在，需按实际目录检查。"
  - "当前系统调用号、分发表与入口包装须从time源码重新锚定，不能从Linux ABI推测。"
  - "真实stdio调用序列、FD继承和console运行行为仍为NOT_RUN。"
---

# CORE-USER-SYSCALL-STDIO-08：把用户态文件操作接到系统调用边界

**本轮仍然只分析，不修改内核或用户程序，不生成修复补丁。** 目标是接续07，明确用户程序的文件/标准输入输出请求在本仓库能追踪到哪里，哪些路径有源码证据，哪些依赖仓库外libc或运行时才能确定。普通问题全部记录入库，Owner当前无需逐项决策。

## 1. 启动块（发给现有本地会话）

```text
执行 MYOS2-LEAD-002-CORE-CHECK-01 的 CORE-USER-SYSCALL-STDIO-08。
先完整读取主线22-user-syscall-stdio-contract.md、07审查回执和18号延期约定。
沿用现有MYOS2-A-CC02会话、专用隔离clone、订阅额度与执行分支。
一次完成C00-C04：检查用户态init/stdio实现边界，追踪syscall分发，
核对FD/dup/exec相关静态路径，接回07的console与VFS说明。
不只交清单；给出连续可读的说明、机器可读事实及缺口状态。
不改内核或用户程序，不出补丁，不编译/运行C/ASM，不构建，不运行QEMU，
不运行安装/挂盘脚本，不安装工具，不修改Owner原开发工作树。
只向user-syscall-stdio-08/新增；PR17仍open则保持Draft复用，
不合并、不删分支、不force-push。
只用订阅额度，不转API或额外付费。普通问题入库，不征求修复意见。
所有结果通过GitHub回传，最后只给PR、提交短标识、完成范围与真实阻断。
```

## 2. C00：绑定与边界

先读本任务、CORE-USER-BOUNDARIES-07-REVIEW-001、18号延期约定，再定点读取07的user-entry-console.md、facts-and-boundaries.yaml中console/syscall/FD相关条目、deferred-findings.yaml中的CORE-USER-BOUNDARIES-07::UB-01、CORE-USER-BOUNDARIES-07::UB-05及GAP-U10相关入口。不要全量重读大YAML。

固定源码快照time a039d9803ade。先读mykernel/scripts/options_flags.cmake，仅做只读Git查询、源码定点读取和已有Python/YAML/JSON工具处理。若远端time已前进，将历史固定快照与当前头差异单列，不能把旧结论直接当成新快照事实。

执行分支头必须包含07结果；旧文件冻结。若远端出现无法解释的并行写入，只停止写入，继续安全的只读工作。不要重跑06或07检查器。

## 3. C01：用户态程序和libc的实际边界

检查仓库中实际存在的myinitramfs用户程序源、启动脚本、构建清单及initd.c（若存在），只确认文件与构建输入，不运行构建或脚本。

建立“仓库内已证实 / 仓库外依赖 / 未知”三栏：init如何启动stdio或打开文件；是否能在仓库源码中追踪printf、freopen、read、write、writev等调用；若只看到外部libc符号，必须止于该边界，不猜libc具体选择哪种系统调用；不下载或引入外部libc源代码，不把通用Linux/glibc行为当成MyOS2现状。

## 4. C02：系统调用入口、分发与返回

从entry_SYSCALL_64、do_syscall_64及实际分发表出发，核对系统调用号如何检查、如何索引、目标函数如何调用、返回值如何进入寄存器帧，以及哪些路径回到07中的syscall_return_via_sysret。

必须使用time源码中的实际表与活动代码。区分注释项、配置项、注册表、活动分发和仅由函数名推测的关系。不能以Linux系统调用号表补MyOS2缺失项；若号段/边界无法静态闭合就列出实际未知。

## 5. C03：文件描述符、dup/exec和console的衔接

定点读取与本问题直接相关的file_table.c、fs/syscall.c、read_write.c、open.c及已确认的console回调路径：fd如何分配并映射到struct file；open/read/write/writev的活动路径与错误返回；dup/dup2如何复制引用或fd槽，实际实现是否有返回路径；exec是否保留、重置或重建标准输入输出相关fd，没有源码就标unknown；07中的/dev/console前后两张f_op表如何与系统调用入口连接。

特别保留07已发现的write(2)与writev(2)路径差异，不把它自行修正或推导为真实用户程序必然行为。不追踪全部文件系统、ATA、FAT32或调度器实现；只追接合点。

## 6. C04：交付与延期登记

只新增到user-syscall-stdio-08/：MANIFEST.md、user-syscall-stdio.md、facts-and-boundaries.yaml、deferred-findings.yaml、evidence.md与最小核对记录。仅当本批需要且确有复用价值时，新增一个小型静态检查脚本。

主说明须包含调用路径图、系统调用表、FD生命周期表、console衔接、仓库外依赖与NOT_RUN边界。事实YAML记录有限能力节点、边、源码锚点、实际查询与未闭合条件。延期文件只追加本批确有证据的新条目，使用全限定ID；不复制旧问题全文，不自动升级或关闭07条目。

对GAP-U10给出明确状态：只有当本仓库实际源代码闭合libc调用选择时才能closed_static；否则保持open，精确指出缺失在何处。UB-01/UB-05只更新相交证据，不把它们变成修复授权。

## 7. 禁止事项与验收边界

- 不改任何内核、用户程序或原脚本；不生成补丁、不构建、不运行C/ASM/QEMU，不执行安装/挂盘/分区脚本。
- 不运行此前H00/V/W/M/N、05、06、07检查；不创建通用验证框架。
- 不安装工具、不提权、不输出凭据；不改Owner原开发树；不写master/time。
- 不猜外部libc实现、不把静态控制流当作实际运行，不以机械检查all_ok替代语义边界说明。
- 普通发现仅入库；当前无需Owner决策。任何未覆盖项写清原因，不能假称完成。
- PR17仍open则沿用并保持Draft；如果Owner已合并，按实际状态检查唯一写入入口，不强制回退master。禁止合并、删除分支、force-push。
- 验收上限PASS_PENDING_LOCAL；本主线不会运行命令或宣称已执行内核验证。