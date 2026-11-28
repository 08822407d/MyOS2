---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: phase2_mainline_independent_review
followup_id: CORE-USER-EXEC-ARGV-ENTRY-VIEW-140
evidence_class: "固定交付、固定源码、历史原件、图表及数值的有界独立静态复核"
conversation_display_name: "MYOS2-A-C10 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前系统标识，实际后台与effort未独立认证）"
model_per_owner: "沿用正式独立验收与后继设计使用Pro的持续约定；本轮未另报界面档位"
date: 2026-11-24
date_timezone: Asia/Singapore
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: f897f5ad5abc
execution_pin_short12: cde2d7f994b5
execution_parent_short12: 0a3075f3f861
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-21
author_receipt_id: 7120006816
read_channel: connector
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
status: PASS_PENDING_LOCAL
acceptance_verdict: PASS_PENDING_LOCAL
new_return_items: []
D00_closed: [R138-03, R139-01, R139-02, R139-03, R139-04]
combined_138: PASS_PENDING_LOCAL
combined_139: PASS_PENDING_LOCAL
full_local_P9: NOT_RUN
self_check: {verified_claims: 4, quotes_reconfirmed: 4, downgraded_to_inferred: 0}
self_check_scope: "本记录新写的四条正式引文；被审六件的P6另为98/98/0"
inputs_read:
  - "192固定合同全文；140固定及活动六件取得并全文比对；正式作者评论按ID双读及全分页查唯一"
  - "193/194及C10正式恢复记录；原九份项目约束和110/124延续前轮已完整加载版本，未声称重启全部加载仪式"
  - "139独审、139-D00-138裁定、138/139相关冻结原件：本轮D00逐项比对，范围见同批历史裁定"
  - "140 evidence所涉14个固定源文件由本轮只读复核组取得，98窗逐字及性质/支持范围核对"
  - "主线直接重核binfmt_elf.c:1–311、exec.c计数/复制/平移/清理/入口相关段、strnlen_user全文、crt1_c/initd/myshell与五份initramfs CMake全文"
  - "固定time与直接master的两金丝雀；直接master公约和协议v2"
  - "25/26/28/66/69历史边界及auxv相关类型/宏，用于本轮限界及后继预检"
review_limits:
  - "未执行完整本地P9、YAML/DOT工具、预处理、编译链接、仓库程序/脚本、Graphviz、QEMU或实机"
  - "函数编排内使用纯文本、JSON字段核对及算术；不把这些计作MyOS2运行验证"
  - "Blob身份按API对象与固定/活动文本核对，未声称本轮独立重算Git Blob SHA-1"
  - "未取得原执行者发表作者评论后的私有工具过程及聊天最终回复，不倒推其内部G04全部完成"
owner_decisions_requested_now: []
open_questions: ["真实ELF装载、CRT绑定、入口寄存器和运行行为仍未证"]
---

# 第140批独立验收：PASS_PENDING_LOCAL；五项历史补正关闭

**裁定：** 以PASS_PENDING_LOCAL接收第140批D00及A/B/C限定静态资料，无新增执行退回项。五项历史文档补正分别CLOSED_DOCUMENTATION_ONLY；138与139的固定原件合读其已判闭补正及[本轮历史裁定](CORE-USER-EXEC-ARGV-ENTRY-VIEW-140-D00-138-139-review.md)后，组合消费状态更新为PASS_PENDING_LOCAL。原RETURN文件和旧评论原样保留，不把文档关闭写成内核问题已修复。

本轮Owner直接要求验收140并按惯例给出141；C10按194号恢复记录承接主线裁定职责。后继在同轮发布[195号合同](../195-user-exec-auxv-payload-contract.md)，使用原C09a／CHAT-EXEC-21。主线未参与140作者交付的设计或编写；本轮三个只读辅助核对分别覆盖D00、源码引文、机器与图及数值，主线回源核关键断言并作最终裁定，未将作者自检直接当独审。

## 1. 交付真实性与范围

[正式作者回执#7120006816](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7120006816)按实际ID双读正文一致；发表前无since分页100/100/71/0，共271个不同ID，140完整作者标记唯一，没有既存140独审或141发布标记。直接四refs为lead=f897f5ad5abc、exec=cde2d7f994b5、time=a039d9803ade、master=e8e60cd19540。开放PR分页2/0，只有原PR16 Open/Ready及原题PR17 Open/Draft，均未合并、base为master。提交短码均从连接器输出复制前十二位。

[执行提交](https://github.com/08822407d/MyOS2/commit/cde2d7f994b5)唯一父0a3075f3f861，差异仅192指定目录六件新增、零改删。固定及活动逐件全文、LF、UTF-8字节和API Blob一致：

| 文件 | LF行 | UTF-8字节 | Blob短码 |
|---|---:|---:|---|
| MANIFEST.md | 209 | 13164 | d0c1425a429d |
| exec-argv-entry-view.md | 208 | 20297 | ef8f46304c69 |
| facts-and-boundaries.yaml | 7028 | 206300 | 35f1ec875939 |
| exec-argv-entry-view-paths.dot | 362 | 32520 | 076aa9dc77f1 |
| deferred-findings.yaml | 127 | 5448 | e9b6f3f144f4 |
| evidence.md | 1079 | 29954 | 8ce3e09a71b1 |
| 合计 | 9013 | 307683 | 6/6 |

本轮重构的是可观察远端终态。原作者评论末尾“发评后再做G04”不等于其内部流程已认证；本轮独立完成上述核查，不要求为已存在评论重发。此前C09已确认的193规则和本轮前C10恢复属于授权lead进展，192技术范围未改变。

## 2. 历史五项补正

| 项 | 本轮裁定 | 核心理由 |
|---|---|---|
| R138-03 | CLOSED_DOCUMENTATION_ONLY | 四处冻结原行分别逐字相等；真实73/47键可定位；execve完整三行宏式身份、fork完整身份和活动语句与固定源相符。 |
| R139-01 | CLOSED_DOCUMENTATION_ONLY | 五次复制补齐30个字节与两个次序表，T0改为有定义的T，保存exec与argv[0]位置分别为T−10和T−30。 |
| R139-02 | CLOSED_DOCUMENTATION_ONLY | A0两路径均为未读槽；其余实际终止槽及复制重读分开，三个旧实边四元组与别名明确对应。 |
| R139-03 | CLOSED_DOCUMENTATION_ONLY | 地址与相对偏移分开；U0/U1、U2、K2空串来源准确，K0/K1只读释放前p并保留正常清理返回前提。 |
| R139-04 | CLOSED_DOCUMENTATION_ONLY | 92ID/91区间旧索引逐项一致，使用/定义、赋值/返回、守卫/受守卫赋值分窗并给组合依据。 |

详细实物位置、旧裁定关联与关闭条件见[历史五项裁定](CORE-USER-EXEC-ARGV-ENTRY-VIEW-140-D00-138-139-review.md)。不重开138已判闭的R138-01/02/04，136/137既有组合PASS_PENDING_LOCAL不变。

## 3. A：表槽、游标和字段可接收

[A1/A2报告表](https://github.com/08822407d/MyOS2/blob/cde2d7f994b5/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/exec-argv-entry-view.md#L94)与机器slots、steps、field_writes及图逐项相符。以每槽8字节、独立S和U、给定有效对象及辅助操作正常完成为前提：

| 模型 | 槽值（自S起） | argv／envp／auxv起点 | 最终扫描p |
|---|---|---|---|
| A1 | 2,U−30,U−22,0,U−19,U−15,0 | S+8／S+32／S+56 | U−10 |
| A2 | 1,U−20,0,U−19,U−15,0 | S+8／S+24／S+48 | U−10 |
| C投影 | 3,U−93,U−84,U−69,0,U−54,U−39,U−24,0 | S+8／S+40／S+72 | U−9 |

argc写槽时旧局部p未给，随后才由arg_start复位；作者没有提前赋给它参数扫描起点。四类mm字段的初写与终写已分列，终止槽只写空指针，不扫描空字符串。局部int计数最后失败测试后为−1，不代表bprm内计数被写回。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	p = mm->arg_end = mm->arg_start;
	while (argc-- > 0) {
		size_t len;
		if (put_user((elf_addr_t)p, sp++))
			return -EFAULT;
```

“以旧sp地址写入，正常完成后sp前移一个槽”是本轮采用的语句前后状态读法；不把E140-048短标题解释成对C子表达式或机器写入先后的额外证明。固定put_user活动宏与25号R25-S01只支持所选标量模型的一次自增。

## 4. B：候选C入口模型可接收

[B1/B2/B3](https://github.com/08822407d/MyOS2/blob/cde2d7f994b5/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argv-entry-view-140/exec-argv-entry-view.md#L125)的envs分别为S+32、S+24、V+16。B3的P是指向单字节NUL的非空指针；V+8、V+16槽存空指针，envc=0使首环境槽也是终止槽。固定循环始终检查argv[argc]而不递增；给定该槽为NULL，三个模型均一次条件为假。

[VERIFIED myinitramfs/mylib/csu/crt1_c.c::_start]
```c
	while (argv[argc] != NULL);
	envs = argv + argc + 1;
```

给定fopen三次正常返回F0、NULL、F2，可按源码顺序确定stdin/stdout/stderr赋值；后续main实参保持给定argc/argv，给定main返回7，exit的实参为7。该结论不保证NULL stdout下任意main行为、不证明设备打开或退出已经发生。顶层CMake的musl-gcc声明与独立mycrt STATIC声明分开，不能据候选_start认定真实CRT选择或入口寄存器绑定。

## 5. C：同一源名称、两个副本及shell新向量可接收

固定init定义清楚区分局部prog_name、args与envs；args[0]和execve的filename原本同指prog_name：

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	char prog_name[] = "/boot/sh";
	char *const args[] =
			{ prog_name , "userarg_test_1", "userarg_test_2", NULL };
	char *const envs[] =
			{ "userenv_test_1", "userenv_test_2", "userenv_test_3", NULL };
```

文件名先复制并保存地址，再复制环境、参数；其余辅助复制范围已独立回源核对。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;
```

七次含NUL长度为9、15、15、15、15、15、9，合计93字节；从低地址依次为argv三串、环境三串、保存文件名。两份/boot/sh位于U−93和U−9，各9字节；22个入口槽及22步p/sp变化均逐项复算。shell入口argv与每行parse_command产生的__argv是不同来源；run_cmd只在index非负分支被调用。这里只检查该数据来源和调用点，不执行任意输入解析或推断完整shell运行。

## 6. 证据、图和协议可达核查

本轮对被审六件重计正式标记98处，MANIFEST为98/98/0。全部E140-001至098无缺号或重复，98个不同路径/起止区间，每窗1–5连续物理行；14源文件逐窗原文、完整定位身份、source_index字段及支持范围一致。不同区间可以共享部分行，不把它说成98个完全互不重叠行集。80窗在函数或宏式函数定义内，16窗为明确标明的构建声明，1窗为宏定义，1窗为envs全局对象声明／暂定定义；后两类及配置只按192规定的相应证据性质消费，不当成函数执行证明。

机器和DOT的193个唯一节点id/kind/label、165条唯一from/to/kind/label全部一致，零悬空。C的调用到复制模型、复制到表模型以及计数到后续复制均标为独立假设投影或数据关系，没有连成已发生的完整运行链。旧30字节与新93字节逐字复算，旧92/91和138的47/46不混入本批新P6。

P9连接器可达范围：1长标识扫描0；2/13引文及强制锚点上述有界核实；3/4所用输入路径与附带范围可定位；5为98/98/0；6无外部事实依赖或检索句柄；7两金丝雀time命中且直接master反验无命中；8启动原句与connector声明相符；11禁用确定性运行措辞0。9/10无实现代码、改动块或补丁，12非聊天降级交付，均不适用。完整本地P9仍NOT_RUN，未用内存文本检查冒签本地机械门。

## 7. 保留边界和后继理由

[25号限定裁定](https://github.com/08822407d/MyOS2/blob/f897f5ad5abc/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-ELF-STACK-TABLES-25-review.md)的create_elf_tables未初始化vma／注释赋值门继续保留。本批给定越过该门的局部片段模型不证明实际生产路径可达。25/26/28/66/69关于映射、用户访问、实际解释器入口与CRT绑定的限制不变；140 deferred七项保留，普通源码问题不在本轮修复。

141选择auxv条目载荷与实际复制边界：25已确定的12/13非终止项、208/224字节及46字容量仅作复验基准。新范围是完整有序tag/value、有效零值和地址指向关系，140三类表尾的逐字映射及零尾复制边界，再加调用者对PHDR/ENTRY/BASE的具体算例。它承接140明确未展开的auxv，不重做整个ELF或全系统ABI审计。

阶段2；完整本地P9、预处理、编译链接、MyOS2程序/脚本、Graphviz、QEMU、实机及真实文件/进程/PID/调度/时钟操作均NOT_RUN。无源码修复、执行分支写入、旧件修改、合并或阶段3授权。
