---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ELF-HEADER-READ-27
record_type: bounded_elf_header_and_program_header_read_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
date: 2026-11-02
source_pin_short12: a039d9803ade
result_base_short12: 487936efec09
instruction_short12: e1fc364983c0
review_26_short12: 895acd739f07
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
---
# 第27批：ELF首块读取、格式分派、程序头完整性及kernel_read接口

## H00 · 冻结范围、历史限定

执行对象是固定`time@a039d9803ade`的`prepare_binprm → kernel_read`，`search_binary_handler → fmt->load_binary`，`load_elf_binary → load_elf_phdrs → elf_read → kernel_read`，以及选择性的`__kernel_read → read_iter/read`。47号任务在`e1fc364983c0`，26号主线验收回执按R26-S01/S02/S03合读；结果冻结基线`487936efec09`。本批不重做25栈表、26映射与匿名段，也不把26所核的程序头后置界限改成“已在入口全部检查”。

与旧报告重合之处：`CORE-USER-VFS-BASELINE-05::UV-03`已登记多格式分派早退逻辑被注释，`UV-06`已登记`simple_filemap_read`尾页计数和ki_pos，`UV-07/UV-08`记录缓存下标和底层read完成性；本批只补它们在首块/程序头调用者处的实际消费，不另创同根缺陷。13号文件缓存边界及R26-S02“没有活动非零返回者就不得虚构失败”持续适用。来源与1–5行逐字引文见`evidence.md`的EH27-A001–A086；本报告不将任何接口注释自动补成Linux惯例。

## H01 · 首块请求、清零与格式分派的分层

### 先有bprm.file，再有256字节输入缓冲

`alloc_bprm`先经`do_open_execat(fd,filename,flags)`取得`file`，对错误指针分流，分配`linux_bprm_s`后把`file`保存为`bprm->file`。真正的`bprm->buf`是`linux_binprm`内部固定`char[BINPRM_BUF_SIZE]`，当前`BINPRM_BUF_SIZE=256`，不是另一次`kmalloc`出来的ELF程序头数组（EH27-A003/A004/A037/A038/A040）。

`prepare_binprm`的活动代码定义`loff_t pos=0`，对这256字节**先全量memset为0**，再调用`kernel_read(bprm->file,bprm->buf,256,&pos)`，函数返回`int`，而`kernel_read`返回`ssize_t`。在读取接口遵守正常返回界限`-errno`或`0..256`的条件模型下，本调用的计数转换不会因过大合法值丢失精度。对超请求的正值、大范围异常返回或无效指针，不推断存在真实生产者，也不以此恢复Linux内核读取保证（EH27-A005/A056/A063/A084）。

**字节状态不能混写：** `memset`确已尝试在有效`bprm`对象上清256字节；若后续读取按约定回报`r`且恰将前`r`字节复制进缓冲，则`[0,r)`才可能是此次文件字节，`[r,256)`保持此前写入的零（除其他写者/异常）。`r`本身**不认证**读入的数据来自正确磁盘页，也不保证文件真的含256字节。下层现有pagecache接口与05::UV-06/UV-08相关。负返回发生时，不把缓冲区宣称全为零：读取前已清零，但底层可能在报错前部分写入；同样不能认定已经收到可靠文件字节。

### 分派实际只拒绝负的prepare结果

`search_binary_handler`先`retval=prepare_binprm(bprm)`，只有`retval<0`才直接返回。正常结果0、正短读`1..255`及完整256均**不会仅凭首块字节数被拒绝**，接着本地`retval=-ENOENT`并开始遍历格式链（EH27-A006–A008）。这不等于0/短读被当作合法ELF：下一步`load_elf_binary`要检查magic、类型和架构，且程序头读取另有完整长度门。举例，若正常返回0且缓冲保持全零，则当前ELF magic条件不能成立，所选ELF handler会返回`-ENOEXEC`；此例要求有效文件对象、格式表已注册、实际到达所选handler。

实际绑定链分开三项：`elf_format.load_binary=load_elf_binary`是结构体函数指针赋值；`init_elf_binfmt`初始化链节点并经`register_binfmt(&elf_format)`追加到`formats`，另以`core_initcall`声明初始化入口；真正的动态调用发生于`search_binary_handler`中的`retval=fmt->load_binary(bprm)`。列表容器宏由`list_header_for_each_container`转向`list_for_each_entry`，这两个宏不是再多两层运行函数。当前所选源中实际核到这一ELF注册，不能据此宣告所有可能的模块、并发或非选定构建注册者已经穷尽（EH27-A001/A002/A009/A016–A018/A039/A051–A054）。

源码中“若`point_of_no_return`或返回不是`-ENOEXEC`则立即退出格式循环”的块**已注释**。因此给定两个有效格式节点且能定义良好地依次运行的假想模型：第一个handler返回0，第二个返回`-ENOEXEC`，最终`retval`被覆盖为后者，再由`bprm_execve`按负值进入错误出口。当前只核到ELF格式的真实注册，不把“两处理器”当成正常生产路径；这恰是已有`CORE-USER-VFS-BASELINE-05::UV-03`，本批仅追加调用者/绑定证据。格式表为空时的`-ENOENT`只是“循环在定义良好条件下执行零次”的局部结果；所选`list_first_entry`宏本身声明假定非空链，空链的对象运算与运行可靠性未认证（EH27-A008–A011/A052/A053）。

`bprm_execve`实际保存`search_binary_handler`结果，只有`retval<0`转错误出口；正常返回0才是所选ELF handler的成功值，不能以该门证明任意正数结果也是完整exec成功。旧`point_of_no_return`之后发致命信号的操作亦为注释。H01止于源码返回消费，不重审exec换mm、失败信号与真实用户态交接（EH27-A012–A015/A083，历史UV-02）。

## H02 · ELF字段门、程序头容量与elf_read

### 首块被解释为ELF64_Ehdr，不等于先验证全256字节

在`load_elf_binary`内`elfhdr_s *elf_ex=(elfhdr_s *)bprm->buf`。`retval`初始化为`-ENOEXEC`；第一个检查为`memcmp(e_ident, ELFMAG, SELFMAG)`，其中`ELFMAG`为四字节魔数且`SELFMAG=4`，随后检查`e_type==ET_EXEC || ET_DYN`、`elf_check_arch`及`elf_check_fdpic`，还有`bprm.file.f_op.mmap`是否存在。当前x86-64的`elf_check_arch`活动宏只比较`e_machine==EM_X86_64`（EH27-A019–A023/A041/A042/A049/A050）。

ELF64头在所选基本类型和通常x86-64布局模型下是64字节，`e_ident`占前16、`e_type`是16位、`e_machine`是16位、`e_phoff`是64位、`e_phentsize`与`e_phnum`是16位。在这段实际入口检查中没有针对`EI_CLASS`、`EI_DATA`、`EI_VERSION`、`e_ehsize`、`e_version`的单独比较；本结论仅限完整读到的选定`load_elf_binary`初始门，不代指整个仓库/其他格式处理器全局没有约束（EH27-A020/A021/A043–A045/A048/A085/A086）。短读64字节在符合数据复制前提下能涵盖本模型的64字节头，但缺少256字节首块不等于后续程序头表已存在，仍需独立读验证。

### load_elf_phdrs 的完整入口

已读定义`load_elf_phdrs(const elfhdr_s *elf_ex,file_s *elf_file)`先设`elf_phdata=NULL`、`retval=-1`、`uint size`；只在`e_phentsize==sizeof(elf_phdr_s)`时继续，随后计算`size=sizeof(elf_phdr_s)*e_phnum`，要求size非0、`size<=65536`且`size<=ELF_MIN_ALIGN`。当前`ELF_MIN_ALIGN=PAGE_SIZE=4096`，ELF64程序头在所选结构/ABI条件下56字节一项，e_phnum是16位，因此正常合法范围内乘法不会溢出32位uint（最大65535×56=3,669,960），最强的长度门仅允许**1至73项**：73×56=4088字节可通过长度门，74×56=4144字节会被拒绝。没有执行预处理或sizeof编译实验，此范围是基于固定定义和通常x86-64类型布局的条件算术（EH27-A027–A029/A046–A048/A080/A081）。

长度通过后调用`kmalloc(size,GFP_KERNEL)`；若未得到非NULL指针便直接跳到`out`，不尝试读取。若分配成功，则真实执行语句为`retval=elf_read(elf_file,elf_phdata,size,elf_ex->e_phoff)`，即**独立的文件偏移**、独立的目标缓冲和独立于首块的按值长度；成功时返回该指针给`load_elf_binary`，后者才进入程序头遍历（EH27-A030–A033/A023/A024）。这里没有看到直接检查`e_phoff+size<=file_size`或`e_phoff`落在有符号loff_t非负范围；长度门验证的是请求的分配大小，**不是文件区间的物理存在**。对此仍需结合`elf_read`的实际返回完整性判断以及下层读取输入契约，不能由“缺少预校验”断言必然越界读取。

`elf_read`实际接收`loff_t pos`的**值参数**，将本地`&pos`交给`kernel_read`并保存其`ssize_t rv`。当`rv!=len`，负数按`int`返回原负值，非负的0、正短读或假想超长返回都转`-EIO`；恰等于请求len时返回0。由于此路径len在正整数且不超过4096的约束下，通常负errno及计数的`int`转换不会截断。文件偏移为ELF64无符号`e_phoff`，进入`loff_t`接口时超出正的有符号范围的值仍有转换/下层范围验证问题，不能不加前提认定安全或某个固定errno（EH27-A025/A026/A044/A031）。

`load_elf_phdrs`的后续出口对非零`retval`调用`kfree(elf_phdata)`并设`elf_phdata=NULL`，然后返回该指针。若前述长度门或分配门失败也进入该出口，但不要把调用`kfree`等同已认证物理回收。值得区分的**错误来源丢失**是：选定`load_elf_binary`在这个调用前仍保留初始`retval=-ENOEXEC`；收到NULL便`goto out`，因此helper内部的`-EIO`、读失败或分配NULL都折叠为上层的`-ENOEXEC`，而不是原始读错误或`-ENOMEM`。这不证明任一失败在现实负载中已经发生；只是控制流对返回类型的真实消费（EH27-A020/A023/A025–A033）。

`elf_read`另被解释器头读取复用，`load_elf_phdrs`也用于解释器程序头表；本批仅登记重用接口与相关实参，解释器递归、PT_INTERP、GNU属性、动态加载与实际PT_LOAD映射仍不进入本批验收范围（EH27-A034/A035）。阶段交接止于成功的程序头指针；其包含的各PT_LOAD字段是否合理以及后置段映射限制仍属26回执。

### 简明静态值表

| 纸面条件 | 当前源码分流 | 不可推出的结论 |
|---|---|---|
| 首块读取0字节 | 搜索只检查负值，进入格式循环；在仅ELF且全零头的条件下magic拒绝 | 不能认定实际文件读返回0的生产者已建立 |
| 首块读取64/256字节 | 均通过首块非负门；64可能已包含完整固定ELF头 | 不代表e_phoff表内容真实存在或所有校验通过 |
| e_phentsize不等于56 | phdr helper返回NULL | 不代表全局ELF错误返回类型完整保存 |
| e_phnum=0 | size0拒绝 | 不运行malloc或读 |
| e_phnum=73 | size4088，长度门可通过 | 不代表分配、文件完整读和内容有效 |
| e_phnum=74 | size4144>4096，返回NULL | 不须假设uint乘法溢出 |
| 已分配但elf_read返回0字节 | elf_read非负长度不符→-EIO，helper清理并返回NULL | 不能把未读入字节当作有效程序头 |
| elf_read恰返回len | helper正常返回已分配指针 | 数据完整性需底层按约定填充，页缓存/驱动未认证 |

## H03 · kernel_read直接层、方法分派与文件位置

`kernel_read`调用`rw_verify_area(READ,file,pos,count)`；实际`rw_verify_area`在最开始就`return 0`，原本的count/pos检查及后续权限验证代码不可达。因此此包装的0**不能证明**已经完成访问区间校验。只有该局部门返回非零时才会停，现有活动体正常执行不产生非零；之后调用`__kernel_read`（EH27-A055/A063）。

`__kernel_read`先在栈上建`kvec_s iov={.iov_base=buf,.iov_len=min_t(size_t,count,MAX_RW_COUNT)}`，其中`buf`是内核目标缓冲，而非用户指针。固定`MAX_RW_COUNT=(INT_MAX & PAGE_MASK)`，在本x86-64页尺寸和32位int模型下为`0x7ffff000`，大于256与合法程序头最大4088，所以所选两类请求不会被此min截短。注意只有kvec的iov_len经过该min，**legacy read**分支依然接收原始count；本批两类合法请求相等故无差异，不能推广为所有大请求已被统一截断（EH27-A056/A068/A069/A080）。

`__kernel_read`的`WARN_ON_ONCE(!(file->f_mode & FMODE_READ))`成立时返回`-EINVAL`，没有从函数名自动补出另一个`FMODE_CAN_READ`检查，因为它当前是注释。之后`init_sync_kiocb`建`ki_filp=file`，并将局部`kiocb.ki_pos=pos?*pos:0`，用`iov_iter_kvec`建立`ITER_KVEC`。有`file->f_op->read_iter`时优先`read_iter(&kiocb,&iter)`，否则有`read`时调用`read(file,buf,count,pos)`，两者都没有才`ret=-EINVAL`。只比较方法存在性还需要有效file/f_op对象和相关回调条件；不能把空方法分支扩展成实际FAT32生产返回（EH27-A057–A060/A065–A067/A070/A071）。

**位置所有权：** `read_iter`完成后，`__kernel_read`当前`if(ret>0) *pos=kiocb.ki_pos`部分处在注释中，函数直接return ret；所选FAT32的`read_iter`指向`generic_file_read_iter`，其活动体在非零count路径调用`simple_filemap_read`，后者以`iocb.ki_pos`取得读起点，但在本已读定义中没有同步更新此字段。所选包装因此不能证明`*pos`或`file->f_pos`被推进；`elf_read`又是pos按值进入，纵然legacy `read`更新它，也只是本次elf_read私有位置。不得将`ki_pos`、`*pos`与`file.f_pos`合为一个状态（EH27-A025/A059/A061/A062/A072–A074）。

FAT32的实际方法表在所选源码中包含`.read=FAT32_read`、`.read_iter=generic_file_read_iter`，因而对于确实绑定该file_ops的有效文件对象，内核`__kernel_read`优先`generic_file_read_iter`而不是直接FAT32_read。再沿这一选定调用看，`simple_filemap_read`通过`iter.kvec`取目标，缓存槽为空时申请页并调用`filp->f_op->read(...PAGE_SIZE...)`，此返回**未被消费**，然后按循环页长`memcpy`并返回目标指针位移。只有在这些指针、索引、数据、循环和底层返回均定义良好时，返回计数才可以作为“本层声称复制多少字节”的接口数据，不自动认证真实文件字节的来源或页缓存已完整（EH27-A075–A079及历史UV-06/07/08）。

**对本任务的具体历史调用点追加：** `simple_filemap_read`对末页计算`len=end % PAGE_SIZE - inpage_start`。纸面设`e_phoff=64`、`e_phnum=72`（程序头大小4032，未越4096）且`ki_pos=64`、`iter.kvec.iov_len=4032`；则`end=4096`，循环首个页位置0，`inpage_start=64`，最后分支给`len=-64`，传给`memcpy`的`size_t`可能成为巨大值。这里仍有缓存索引/有效对象/执行抵达前提，不断言真实文件已触发或必然完成某种内存写入。此属于已登记的`CORE-USER-VFS-BASELINE-05::UV-06`的**ELF程序头使用端证据追加**，而非本批全新独立根因（EH27-A074/A076/A077/A082）。

### 返回与内容不能互代

| 选定前提/接口 | 活动返回或分流 | 缓冲/位置状态限定 |
|---|---|---|
| rw_verify_area | 当前活动返回0 | 不等于已经检查文件区间或权限 |
| FMODE_READ门不通过 | __kernel_read返回-EINVAL | 不进入read_iter/read |
| 文件read_iter存在 | 委托并返回ssize_t | `*pos`回写在包装中注释，数据可信度独立 |
| read_iter不存在而read存在 | 直接调用read(file,buf,count,pos) | read可能按自己的实现改`*pos`，不是统一包装保证 |
| 两种方法均不存在 | -EINVAL | 不接触目标数据 |
| 读取返回0、正短读或负值 | prepare只以负值阻断；elf_read要求恰好len | 首块已清零不等于文件字节完整；phdr失败时放弃已分配缓冲 |
| 读取正好请求len | phdr的elf_read返回0 | 返回数量与所读内容、磁盘真实文件范围是不同证明 |

## H04 · 延期项、交接与停止范围

新条件候选`EH27-F01..F04`分别限于首块短读输入边界、phdr错误来源折叠、e_phoff与`rw_verify_area`实际偏移验证边界、以及所选初始ELF头的字段检查覆盖度；是否独立根因/实际漏洞由后续证据评审，不因四项编号就认定四次实测问题。UV-03多处理器返回覆盖、UV-06页尾计数及其FAT32读取链、UV-07/UV-08缓存下标与读结果皆保留旧全限定ID和状态；本批只提交新使用端补证，不更改历史成果。

本批输出六件：本说明、`facts-and-boundaries.yaml`中14对象/条件矩阵/4个纸面例、`elf-header-read-paths.dot`中分离的调用与数据/返回图、`deferred-findings.yaml`、`evidence.md`逐字源码、`MANIFEST.md`完整自查。执行者只声明阶段2静态`READY_FOR_MAINLINE_REVIEW`，不声明最终PASS。没有运行仓库脚本、C/ASM、预处理、编译、QEMU、内核、硬件或真实ELF文件，也未读取仓库外libc与镜像、修改内核/旧文件、产生修复补丁或开始下一批。正式Git提交/远端核对结果只记本人PR17交付评论，不在提交前的报告内预先伪造SHA或验证通过。
