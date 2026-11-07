---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ELF-INTERPRETER-28
record_type: bounded_interpreter_acquisition_mapping_and_lifetime_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
conversation_display_name: MYOS2-A-C03c 代码分析执行续建
date: 2026-11-02
read_channel: connector
source_pin_short12: a039d9803ade
instruction_short12: 5e5fb5e4e460
review27_short12: d2f148ad2d74
result_base_short12: f98ee06bbce8
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
---

# ELF解释器：从PT_INTERP到load_elf_interp和最终入口的有界静态账

## I00｜固定来源、历史接受约束

本报告独立核对`time@a039d9803ade`，范围是`load_elf_binary`的两轮头表扫描、`PT_INTERP`路径/文件取得、解释器头和phdr读取、`load_elf_interp`偏移/返回，以及到`AT_BASE/AT_ENTRY/START_THREAD`的数值传递和所选清理标签。48号任务书`5e5fb5e4e460`、27主线验收回执`d2f148ad2d74`已全文读取；本对话继续原C03c身份，不重放45号接收。I00金丝雀与PR预检见MANIFEST。

第27批按PASS_PENDING_LOCAL限定接收，必须合读R27-S01/S02/S03：读取返回值不等于真实字节或迭代器推进；算术需遵循实际有符号/无符号操作和分配成功门；图中caller/赋值/对象方向必须有直接证据。第26批的`elf_load/elf_map`、尾部清零及`vm_munmap`未消费结果只在解释器**实际调用点**复用，不重审整批；第25批`AT_ENTRY`和最终解释器入口区分及`create_elf_tables`的局部`vma`未初始化门继续约束实际完整exec结论。旧05::UV-02及15所选mm替换/不可返回点限制同样保持。

正文断言引用同目录`evidence.md`的`EI28-A001`等真实path::symbol、最多5行的固定time逐字引文，事实/错误/状态矩阵另见`facts-and-boundaries.yaml`。每个确定性陈述均限所选定义、有效对象、被调用返回和无回绕前提。文本/纸面值表不代表ELF样本执行。

## I01｜PT_INTERP选择、路径字节与文件取得

### 两轮头表不能混为一次

`load_elf_binary`最初将主程序`bprm->buf`作为`elf_ex`头，调用`load_elf_phdrs(elf_ex,bprm->file)`取得主头表`elf_phdata`；返回NULL则从初始`-ENOEXEC`出口退出（27号）。随后第一轮按`elf_ex->e_phnum`遍历程序头，`PT_GNU_PROPERTY`仅记录并`continue`，非`PT_INTERP`也`continue`。遇一个PT_INTERP后从路径校验到完整解释器ELF头读取成功，实际执行`break`；**不会继续打开第二个PT_INTERP**。若第一项途中报错直接`goto out_free_ph`或通过`out_free_interp`跳转，同样不尝试后续PT_INTERP。完全没有PT_INTERP时`interpreter`保持NULL，后续走无解释器入口路径。其后第二轮头遍历处理GNU_STACK及处理器范围的少数选择，不能把它误当再次寻找/打开PT_INTERP。证据：EI28-A001/A002/A004–A008/A015/A016。

### 路径p_filesz、读取与字符串消费

路径长度取当前`Elf64_Phdr.p_filesz`（__u64，字节）。`p_filesz<2`或`>PATH_MAX`时先设置`retval=-ENOEXEC`，并进入主表释放；所选`PATH_MAX=4096`（含NUL），所以活动范围2..4096字节。合格后`kmalloc(p_filesz,GFP_KERNEL)`，NULL时以`-ENOMEM`退出。若分配成功，以**主程序文件`bprm->file`**和`elf_ppnt->p_offset`调用`elf_read`读取恰好`p_filesz`字节；其返回负错（包括正常短读分流的`-EIO`）时经`out_free_interp`释放临时路径，再走主表释放。这里的文件长度与数据已完整有效仍受27号读取接口限制。证据：EI28-A008–A010/A053/A074/A078。

读取成功之后，代码只测试`elf_interpreter[p_filesz-1]=='\0'`，否则`retval=-ENOEXEC`并清理；它**没有在此测试内部是否存在更早NUL**、是否以`/`开头、路径访问权限或对应文件是ELF。若一个9字节纸面数组是`/ld\0junk\0`，尾字节检查通过，随后`getname_kernel`使用`strlen`得到前三字节路径，因而消费的是`/ld`。这是独立字符数组模型，不是已证攻击样本或绕过校验；其它实际打开检查没有在本批穷尽。证据：EI28-A011/A012/A067–A069。

随后调用`interpreter=open_exec(elf_interpreter)`并**立刻`kfree(elf_interpreter)`**，无论该打开返回文件或编码错误，临时路径缓冲都不由解释器文件继续持有。本地`retval=PTR_ERR(interpreter)`写在`IS_ERR(interpreter)`之前：PTR_ERR是数值转换，不是已经执行了错误判定；只有随后的IS_ERR为真才将其作为打开错误走`out_free_ph`，不对错误指针调用`fput`。当返回真实文件指针，当前retval可能暂时含地址截取后的值，但接下来的解释器头读取/检查重新赋值；不能凭这句报告发生内存泄漏或一次打开成功被错判。证据：EI28-A012/A063/A064/A080–A082。

`open_exec`自己的调用顺序是`getname_kernel(name)`，先设置`file_s *f=ERR_CAST(filename)`，只有`!IS_ERR(filename)`时调用`do_open_execat(AT_FDCWD,filename,0)`，随后`putname(filename)`，返回f。后者用`O_LARGEFILE|O_RDONLY|__FMODE_EXEC`和`MAY_EXEC`、`LOOKUP_OPEN/LOOKUP_FOLLOW`构造请求，并拒绝此调用未授权的open flags，最终调`do_filp_open`；文件类型/noexec的**第二次**检查在本固定包装内为注释。不能据包装缺少二次检查就断言更深`do_filp_open`没有其它限制。只在上述`open_exec`和必要`getname_kernel`接口止步，不重新做路径解析与权限全树审计。证据：EI28-A059–A064。

定点展开`getname_kernel`另有一条条件性风险：`__myos_getname`分配失败时返回`ERR_PTR(-ENOMEM)`，而`getname_kernel`未先检查该值便继续访问`name->len`/分配`name->name`。若该失败实际发生，不能保证`getname_kernel`正常向`open_exec`返回错误指针；这只是所选有效/失败对象的源码路径记录，不作为已复现机器错误。对于本PT_INTERP上限和尾NUL，`strlen`的超过PAGE_SIZE门不能直接当成可达的本任务失败支路：路径最多4096且末尾NUL，正常字符串长度最多4095。证据：EI28-A066–A070；详见EI28-F01。

### 八种对象不能因名称接近合并

`bprm->file`是主程序；`interpreter`是本函数的本地`file_s*`，不是`linux_bprm_s.interpreter`成员；`elf_interpreter`是可短期释放的路径字符缓冲；`filename_s`是`open_exec`包装中的另一个对象，`putname`只针对它；`interp_elf_ex`是独立新分配64字节头，`interp_elf_phdata`又是一组独立程序头数组，`elf_phdata`仍是主程序数组。源码中该本地`interpreter`从不直接存给`bprm->interpreter`（限完整已读`load_elf_binary`）；普通指针复制不是file引用计数变化，两个字段的独立结构体定义见EI28-A089。完整对象身份、先行作用、释放标签见facts的O01–O18与M01–M19。证据：EI28-A001–A004/A009/A012/A013/A014/A019/A030/A035/A036/A063/A071。

## I02｜解释器头校验先于mm切换，类型/mmap检查晚于切换

解释器文件取得成功后，`interp_elf_ex=kmalloc(sizeof(*interp_elf_ex),GFP_KERNEL)`；空结果设置`-ENOMEM`并从`out_free_file`释放文件，正常地址则用`elf_read(interpreter,interp_elf_ex,sizeof(*interp_elf_ex),0)`完整读取头，返回负值时`goto out_free_dentry`。在当前ELF64结构布局模型下请求64字节；`elf_read`必须得到等于请求的返回计数才返回0，负错和短读有第27批既有契约，不能把完整计数自动解释为正确磁盘内容。证据：EI28-A013/A014/A053/A076/A079。

首轮break后，解释器非NULL才进入`retval=-ELIBBAD`门：验证`ELFMAG`四字节、`elf_check_arch`及`elf_check_fdpic`，随后调用`load_elf_phdrs(interp_elf_ex,interpreter)`。它的入口宽度/数量与最长一页等约束沿用27；本调用者收到NULL即走`out_free_dentry`，**保留调用前的`-ELIBBAD`，而不是复用主程序的`-ENOEXEC`**。因此相同helper对主程序/解释器失败的外层errno分类不同。证据：EI28-A017–A019/A054–A056。

此处没有独立在解释器头预检里要求`e_type`是`ET_EXEC/ET_DYN`，也没有在此先检查解释器文件的`f_op->mmap`；它们的活动检查存在于**稍后**的`load_elf_interp`内（EI28-A039/A084）。准确顺序为：解释器路径和头/程序头读→magic/arch等检查→部分GNU_PROPERTY/arch属性路径已注释→`begin_new_exec(bprm)`（内含`point_of_no_return=true`及mm替换）→`setup_arg_pages`→主程序PT_LOAD映射→`load_elf_interp`自己的晚到类型/mmap门。这里不能将后两个门当成此前mm和主程序映射成功的前提；实际抵达仍受上游VMA/GUP/参数页问题、文件读数据与有效对象约束。证据：EI28-A020–A024/A026/A039/A065。

解释器头读取/结构/magic失败时，代码在进入`begin_new_exec`之前触发局部清理；晚到`load_elf_interp`校验失败，可能已在不可返回点之后、且主程序映射发生过。错误处理应分别记errno与已发生的对象/VMA前缀，不能将`goto out_free_dentry`当作内存映射事务的全量撤销。旧05::UV-02和26::EL26-F02已经记录这类不可返回点/先行作用风险；本批仅添加解释器调用的新位置证据，不重报为多个已复现独立漏洞。

## I03｜load_elf_interp偏移算法、区分MAP_FIXED和最终入口

### 局部变量及错误值来源

`load_elf_interp(interp_elf_ex, interpreter, no_base, interp_elf_phdata, arch_state)`声明`ulong load_addr=0`，`int load_addr_set=0`，`ulong error=~0UL`，另有`total_size`。**注意：初始`~0UL`在64位无符号模型是0xffffffffffffffff（编码-1），不是已调用某个子函数得到的-ELIBBAD。** 如果最初类型不在ET_EXEC/ET_DYN、架构/fdpic不符或`interpreter->f_op->mmap`为空而goto out，error仍为该初值；调用者随后先看`IS_ERR_VALUE`，再看`BAD_ADDR`并以`(int)elf_entry`赋retval，可形成-1的所选条件返回。这个路径能否从已通过前置magic/arch检查的实际输入达到，须按每一个晚到门证明，不能因代码有相同检查就宣称已发生。证据：EI28-A038/A039/A080/A083/A084/A086/A027/A029。

`total_mapping_size(interp_elf_phdata,e_phnum)`仅以所有PT_LOAD的页下界最小和`p_vaddr+p_memsz`尾界最大值求跨度；无PT_LOAD或跨度为0则`error=-EINVAL`并提前返回。对地址加减及页对齐的回绕、程序头来源与长度门仍需前提。成功数值并不保证实际文件页或VMA已经建立。证据：EI28-A040/A047/A048。

### ET_EXEC、ET_DYN首段及后续段

每个`PT_LOAD`开始`elf_type=MAP_PRIVATE`，按`make_prot(p_flags,arch_state,true,true)`建立请求权限；这与主程序调用`is_interp=false`的实参不同，不认证最终CPU权限。若解释器类型为ET_EXEC**或**`load_addr_set`已经为1，则加`MAP_FIXED`；否则首个ET_DYN段在`no_base!=0`时设置`load_addr=-vaddr`，在64位无符号中是模2^64减法。随后调用`elf_load(interpreter,load_addr+vaddr,eppnt,elf_prot,elf_type,total_size)`，**立刻把total_size置0，再把map_addr存到error并查BAD_ADDR**。因此第一个PT_LOAD如果返回错误也已将本局部total_size清零；只有控制流正常进入下一次迭代才会使用这个0。与主程序首ET_DYN特定路径使用MAP_FIXED_NOREPLACE的方式不同，解释器首个ET_DYN以MAP_PRIVATE非固定请求映射，成功后才令后续段MAP_FIXED；不能把两者的冲突保证混合。证据：EI28-A041–A044/A085及历史26限定。

首个ET_DYN段`map_addr`通过BAD_ADDR数值门且`!load_addr_set`时，源码计算`load_addr=map_addr-ELF_PAGESTART(vaddr)`，并令`load_addr_set=1`。以后其他PT_LOAD的地址为`load_addr+vaddr`。该减法若map_addr小于页下取整vaddr，可能在ulong中回绕；应保留整数范围和调用者IS_ERR_VALUE/BAD_ADDR最终检验，不因为C整数模型算出了值就证明它是合法用户地址。对ET_EXEC，`load_addr`仍可维持0，所有被处理PT_LOAD走MAP_FIXED，若全部局部门通过，函数最后写`error=load_addr`后返回。**返回load_addr是重定位调整量，不是最终PC。** 证据：EI28-A042–A046/A086。

`elf_load`和`elf_map`各自的文件长度、页尾清零、匿名扩展与解除映射副作用仍按26限定。解释器新事实仅为实际调其函数的实参：`file=interpreter`，`addr=load_addr+vaddr`，`prot=make_prot(...,true,true)`，`type=MAP_PRIVATE或MAP_FIXED`，第一PT_LOAD的`total_size`（可能跨空洞）与后续0。`map_addr`先经BAD_ADDR，接下来才计算`k=load_addr+p_vaddr`并验证`p_filesz<=p_memsz`、`p_memsz<=TASK_SIZE`、`TASK_SIZE-p_memsz>=k`；失败时是解释器特有的`error=-ENOMEM`，而主程序相应后置门采用`-EINVAL`。若映射已产生前缀，本层没有活动的全量撤销。证据：EI28-A043/A045/A049–A052。

**零文件字节段的条件分支：** 在`p_filesz=0`时共享`elf_load`跳过`elf_map`，转为`map_addr=zero_start=ELF_PAGESTART(addr)`并可能调用匿名`vm_brk_flags`。解释器无论ET_EXEC还是ET_DYN都会给首次`elf_load`传total_size，但这条路线不一定进行整体**文件**预留；只要数值返回通过BAD_ADDR，首ET_DYN仍可能设`load_addr_set=1`并使后续段走MAP_FIXED。该场景必须附加`total_mapping_size>0`、有效页范围和前述anon映射正常返回等前提；这是26共享帮助函数在解释器侧的新增消费，**不是一个已实测冲突/泄漏**。证据：EI28-A040/A043/A044/A049–A051。

### 两个入口和三个交接值不能合并

主程序段扫描后，`e_entry=elf_ex->e_entry+load_bias`代表**主程序入口**。若有解释器，`elf_entry=load_elf_interp(...,no_base=load_bias,...)`，仅在`!IS_ERR_VALUE(elf_entry)`时将其保存成`interp_load_addr`，然后`elf_entry += interp_elf_ex->e_entry`构成**最终入口候选**。之后`BAD_ADDR(elf_entry)`若成立，错误编码范围由`(int)elf_entry`传递，其余不合法地址映射到`-EINVAL`。若没有解释器则`elf_entry=e_entry`并独立作BAD_ADDR。通过数值门并释放解释器局部对象后，`create_elf_tables(bprm,elf_ex,interp_load_addr,e_entry,phdr_addr)`内向auxv写`AT_BASE=interp_load_addr`、`AT_ENTRY=e_entry`；最后`START_THREAD(...,elf_entry,bprm->p)`使用的是**最终入口**，而不是AT_ENTRY或原始偏移。证明写入栈表和CPU进入用户态仍受第25批未初始化vma及后续门限制。证据：EI28-A025–A034/A037/A088。

纸面例：假定main`load_bias=0x55550000`，主`e_entry`原值0x2340，解释器ET_DYN首段`vaddr=0x12000`、第一次`elf_load`的有效返回`map_addr=0x7f000000`，解释器原始`e_entry=0x12340`。若no_base非零则第一次计算`-vaddr+vaddr=0`（模2^64），接着`load_addr=0x7f000000-0x12000=0x7efee000`；最后入口`elf_entry=0x7efee000+0x12340=0x7f000340`，`AT_BASE=0x7efee000`而`AT_ENTRY=0x55552340`。这是有前提的规则算术，不证明该地址已经映射为合法可执行页。另一个ET_EXEC解释器的条件算例返回load_addr=0并以其自己的e_entry作为PC，见facts::EI28-X02。

## I04｜正常/失败路径、资源账与延期

### 按标签定位释放，不推断物理回收

| 失败或完成点 | 返回来源 | 本地清理调用及其边界 |
|---|---|---|
| 无PT_INTERP | 后续main分支确定 | 不产生解释器file/头/路径buffer；main表按其后继正常/错误出口处理 |
| p_filesz非法、path kmalloc失败 | -ENOEXEC、-ENOMEM | `out_free_ph`对主elf_phdata调kfree |
| path elf_read负错或尾NUL失败 | 原elf_read错误或-ENOEXEC | `out_free_interp`先kfree路径，再`out_free_ph` |
| open_exec返回ERR_PTR | PTR_ERR转换后经IS_ERR门 | 路径已kfree，走`out_free_ph`；不对错误指针fput |
| interp头kmalloc失败 | -ENOMEM | `out_free_file`条件fput(interpreter)，主表kfree |
| interp头短读/负错，magic/arch失败，phdr helper NULL | 原读负错或-ELIBBAD | `out_free_dentry` kfree解释器头及表，再落至`out_free_file`和`out_free_ph` |
| begin_new_exec、setup_arg_pages或主程序映射失败 | 当前retval、先前接口值 | 同一局部释放标签；可能已触发不可返回点或映射前缀，未证完整撤销 |
| load_elf_interp早期-1、跨度0、段失败或最终入口BAD_ADDR | 编码值经IS_ERR_VALUE/BAD_ADDR按所选分支消费 | 同一局部释放标签；不把局部对象释放等同主/解释器VMA全量清理 |
| 解释器装载和最终入口检查通过 | 数值调整量及入口正常 | 调fput(interpreter)、kfree(interp_elf_ex)、kfree(interp_elf_phdata)，再kfree主elf_phdata |
| 后续create_elf_tables失败 | 本地retval<0 | 已经过前述局部释放，`goto out`不会再进入该组释放标签；仍不证明完整exec正常运行 |

本矩阵每项的完整前提、对象和对应1–5行源码引文由facts::EI28-M01..M19给出。源调用`kfree`不自动证明所有实际资源被回收；`fput`经引用归零时才可能调用`kmem_cache_free`，不能对未核持有者及全局路径引用做断言。正常成功分支`fput(interpreter)`及两次`kfree`后，局部指针未被改写为NULL，但后续已读所选代码没有再次**解引用**这些已经归还的解释器对象；后续`create_elf_tables`等用的是数值`interp_load_addr`。非NULL与仍拥有引用不是同一事实，不能因为变量名未清空便武断宣布发生UAF或双重释放。证据：EI28-A030/A032–A036/A072/A073/A087。

还有一项独立的路径分配错误条件：`getname_kernel`直接对`__myos_getname`返回对象写字段，没有先处理ERR_PTR(-ENOMEM)。此项在普通成功路径并不触发，见EI28-F01。剩余延期EI28-F02–F05分别为迟到解释器校验、零文件字节首段的文件预留前提、helper初始-1错误分类，以及尾NUL并不排除内部NUL；其中有明确历史重叠和证据缺口，不能从五个编号宣称五个新独立漏洞。旧`CORE-USER-VFS-BASELINE-05::UV-02`、`CORE-USER-ELF-SEGMENT-LOAD-26::EL26-F01/F02`、`CORE-USER-ELF-HEADER-READ-27::EH27-F02/F03`及第25批VMA局部门只做带ID追加，没有修改旧成果。

**交付和停止：** 本批六件的完成证明、图和自查清单分别在`MANIFEST.md`、`elf-interpreter-paths.dot`及`evidence.md`。仅阶段2静态`READY_FOR_MAINLINE_REVIEW`，主线验收`PENDING`且最高`PASS_PENDING_LOCAL`。没有修改内核、用户程序或历史成果，也没有生成补丁、预处理、编译、运行仓库C/ASM、QEMU、硬件、真实ELF/动态链接器、外部libc或安装任何东西。所有资源/地址/错误结果仍为源码前缀与有效输入条件。最终Git提交、两种远端回读和compare的实际结果只由本人PR17交付评论登记；正式交付即停写，不自发启动29批。
