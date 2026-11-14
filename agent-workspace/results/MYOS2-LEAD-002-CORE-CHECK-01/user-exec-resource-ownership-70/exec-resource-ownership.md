---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-EXEC-RESOURCE-OWNERSHIP-70
record_type: bounded_resource_and_registry_source_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
produced_by: GPT-6 (model self-reported; backend not attested)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00: documentation_disposition_submitted_pending_C05
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
runtime_evidence: NOT_RUN
---

# 第70批：exec格式注册、bprm资源转交与归还契约

本件仅分析固定`time@a039d9803ade`内有限定义。引用、条件返回、调用请求、最后引用释放、物理缓存回收、MM激活与真正用户态执行相互独立；所有模型须满足对象有效、正引用数、各前置callee正常返回和未建模并发不存在等前提。完整本地P9、预处理、编译、链接、仓库脚本、旧夹具、Graphviz、QEMU、实机和CPU真实切换均NOT_RUN。

## D00：R69-01最小单定义重锚

主线69回执仅退两个复合宏引文。固定69 evidence的C39虽标`X86_EFLAGS_FIXED`，实际围栏把`X86_EFLAGS_FIXED_BIT`与`X86_EFLAGS_FIXED`两个各自独立的define连写；C40则把`X86_EFLAGS_IF_BIT`和`X86_EFLAGS_IF`连写。这些行原文确实存在，行数也未超限；失败原因专是P2单一定义归属，不是bit1/bit9数值错误，也不是所谓空行问题。

本70新evidence分别以固定processor_const_arch.h的**D01=FIXED_BIT一行、D02=FIXED一行、D03=IF_BIT一行、D04=IF一行**重锚（每行真实宏定义，不混搭注释或其它宏）。消费旧69时：旧C39改用D01/D02，旧C40改用D03/D04。旧69的facts.register_handoff_matrix的start_thread_common项、正文C37–C40段的bit与flags说明需要以这四个锚并用旧69的regs.flags赋值锚合读。旧69 MANIFEST三数105/105/0、facts.P9第2/5/13项和PR17旧正式交付评论#6764279513中“105条都满足单定义”只能当历史作者申报；105是旧标签数量，至少C39/C40不满足单定义，旧其余103条没有由本D00全量再核。不能把旧计数静默改为103全部通过、或把4个新锚算成旧包新补到107条通过。原69六件/旧评论/PR元数据一字节不改。R69-01仅`documentation_disposition_submitted`，等待C05独立闭合；69技术主干按R69-S01/S02限制继续保留。R67文档闭合及68限定接收保持，23/42旧静态回执只按其限定消费。

## G01/A：实际格式对象从初始化到分派

固定`exec.c`把`formats`定义成静态列表头（A01），声明/初始化宏提供指向自身的anchor与count=0（A15/A16）；`register_binfmt(fmt)`直接调用`list_header_add_to_tail(&formats,&fmt->lh)`，`unregister_binfmt`则通过`list_header_delete_node`处理相同对象（A02/A03/A20/A21）。这些都是源码局部操作，不等于外界任务实际完成注册/注销，也没有认证完整list数据结构在所有内存状态下可迭代。

真实注册对象是`linux_binfmt`型的静态`elf_format`，其`load_binary`字段绑定`load_elf_binary`（A04–A06）；**不是**每次exec的`linux_binprm`。入口`do_basic_setup→do_initcalls→init_elf_binfmt`有直接调用关系，`init_elf_binfmt`中初始化`elf_format.lh`后调用register_binfmt，后者把该节点加入formats（A07/A09/A10）。源码另有`exit_elf_binfmt→unregister_binfmt`（A08），没有因为看见该函数就宣称退出时必曾调用。定点搜索只能确认在检索覆盖范围内找到这一具体ELF注册调用；不能据此全局断言永远只有一个格式处理器，更不能把旧69用来说明“两个handler的假设数据”写成实际运行次数。

`search_binary_handler`对formats用`list_header_for_each_container`，它展开到`list_for_each_entry`，并逐个调用`fmt->load_binary(bprm)`（A11/A12/A17–A19）。原根据point_of_no_return或retval提前退出的块当前为注释，最终返回的是最后一次活动处理器retval（A13/A14）。列表头的初始count及自环不自动保证空表时迭代宏在所有编译/地址条件下定义良好；若讨论空表，需要额外的列表消费者/地址前提。注册与遍历本地锁操作均被注释（A23、A11–A13）；**只有真的出现重叠注册/注销、缺少其它同步并破坏有效对象时**才讨论EO70-F02的并发候选，不能凭注释代码就宣布实机竞态。此前IE69-F01关于最后返回覆盖先前返回的条件候选保持原身份，当前只补充真实ELF对象和注册链，不另建重复问题。

## G02/B：bprm取得、借用、mm与exe_file交接

`do_open_execat`按当前参数门调用`do_filp_open`并取得`file_s*`或错误指针（B01–B04）。`alloc_bprm`先拿到file，再kzalloc bprm；若bprm分配失败，走`do_close_execat(file)`；成功则`bprm->file=file`，相应的filename按是否绝对路径/AT_FDCWD选择直接借用`filename->name`或单独申请fdpath（B05–B11）。`bprm->interp`在本分支先直接指向`bprm->filename`，这是**地址别名而不是新增引用**。后续bprm_mm_init取得暂存MM并写bprm.mm；若初始化失败，直接定义可见将字段设NULL及mmdrop，再由alloc_bprm走free_bprm错误清理（B12–B16）。这些涉及mm与arg页的真实构造/清理细节仍遵循23号回执的条件边界，不复审其VMA或页拷贝子系统。

`begin_new_exec`设置`point_of_no_return=true`在前；继而依次经过de_thread、unshare_files、`set_mm_exe_file(bprm->mm,bprm->file)`和`exec_mmap(bprm->mm)`的返回门，真正正常完成后才`bprm->mm=NULL`（B23–B28）。这个NULL标记表示此字段不再由free_bprm的`if(bprm->mm)`支路归还，**不是对该MM物理释放的证明**。exec_mmap的直接体将task.mm和task.active_mm指向新mm、发出activate_mm调用，再按old_mm是否存在走mmput或mmdrop条件路径（B31–B36）；地址空间实际生效、old_mm是否有效及计数平衡未经CPU/完整MM审计。

`set_mm_exe_file`先取得mm.exe_file旧值，若new_exe_file非空，`get_file`会增加该file对象的f_count；随后把mm.exe_file设为new并有条件对old执行fput（B37–B40/C31–C33）。这是**额外引用事件**，不同于bprm.file原先从do_open_execat得到的那一次；不能因为两个字段恰好指向同一对象，就认为出现了一次隐式再get。若`bprm->have_execfd`成立，begin_new_exec把`bprm->executable`交`fd_install`后置NULL（B29/B30）；未证其全部生产者、安装成功的设备/表对象与引用语义，置NULL只表示此清理字段退出所有权路径。

`linux_binprm`分别定义`file`、`executable`、`interpreter`三个file指针字段与`filename`、`interp`、`fdpath`字符串字段（B17–B22）。不要把ELF loader局部`file_s *interpreter`与bprm.interpreter同名字段混为同一对象。所读load_elf_binary在PT_INTERP分支申请名称缓冲、用open_exec取得局部解释器file，并在成功尾部fput或相应较早错误出口中fput（C16–C23/C27）；不能只数两个fput源位置，就断言它们在同一路径执行两次。完整解释器映射与多处失败转移仍有限，相关历史28结论本批未借用未核条目。

## G03/C：按可能存活的对象计算清理事件

在`kernel_execve`的所读分支中，`bprm_execve`正常返回后进入`out_free: free_bprm(bprm)`，再在`out_ret`执行`putname(filename)`并返回retval（C10/C35）。`bprm_execve`依据search_binary_handler负值到out、非负到正常路径，两个出口都只给出源码控制，retval与资源所有权平衡不能等价（C11–C13）。

**关键局部事件：** `free_bprm`先在`bprm->file`非空时调用`fput(bprm->file)`，之后**无条件**调用`do_close_execat(bprm->file)`；后一个帮助函数在file非空时再次调用`fput(file)`（C01/C04）。两处使用的是同一未被写NULL的字段，中间没有本地`get_file`。`fput`按file.f_count减一后是否为零进入归还请求，归零后调用`kmem_cache_free`（C28/C29）；`get_file`另以atomic_long_inc增加同一计数（C31）。故这是有实际调用点支持的**重复引用归还请求**，但绝不能直接宣告实测二次free或必然UAF：初始计数1时，第一次1→0即可能失去对象有效性，须立刻停止模型，不能继续算第二次0→负值；初始2且每次访问仍合法时，则可能2→1→0。若这第二次减去的是mm.exe_file仍期望持有的独立引用，下一次使用该字段有潜在悬空风险；这仍依赖真实引用来源、调用可达性和对象存活证明。相应EO70-F01为`needs_evidence`，而不是运行错误判决。

其余资源事件亦各有单独门：free_bprm中非空bprm.mm才调用mmput（C02/C34），`free_arg_pages`有直接调用但当前定义是空函数（C03/C07），`bprm.executable`非空再fput，`interp!=filename`才释放interp字符串，最后归还fdpath与bprm结构（C05/C06）。空函数不能直接证明所有参数页永不归还，它们可能在其它MM清理路径中处理；但也不能据此将所有引用/页面寿命签成平衡。若任一前置callee或对象无效，就在最早失败点停止，不补写后续伪引用数。

有限引用事件模型在facts中分三种账：一是初始正f_count为1至4的连续两次本地fput，仅在对象仍有效时计算后一步；二是set_mm_exe_file一次get_file加两次put的净值模型，仅对确有同一file、正计数、无其它写者有效；三是MM是否取得/移交与格式列表假定0/1/2个处理器的纯布尔控制情形。它们不是运行的真实引用数或空表CPU遍历证明。文件/页分配器、锁、MM地址空间、动态解释器、fd_install和QEMU/硬件均NOT_RUN。

## 交付范围和停止

新facts的必备字段为prior_return_disposition、source_ledger、format_registration_matrix、bprm_ownership_matrix、cleanup_event_matrix、finite_model_cases、prior_findings_links；另有新的EO70-F01/F02两项条件候选及八项EO70-Uxx证据缺口，零实机复现。DOT分真实CALL、宏展开、数据/别名、局部顺序、条件返回和ANALYSIS_STOP，节点与边只是有界静态图，非真实运行轨迹。
新引文仅在evidence中，D4+A24+B42+C35合计105个真实单定义短锚；按固定time逐字二次回源，完整新六件必须在G04写前/写后做P6原始计数。D00只提交`documentation_disposition_submitted`，A/B/C=`completed_in_scope`，总体`READY_FOR_MAINLINE_REVIEW`且`mainline_acceptance=PENDING`。本次只在PR17新user-exec-resource-ownership-70目录新增授权六件与本人唯一正式评论，旧件/旧评论/PR元数据冻结；发布后STOP_WRITING，不自关R69-01、不自签70通过、不开始71、不修改或运行内核。
