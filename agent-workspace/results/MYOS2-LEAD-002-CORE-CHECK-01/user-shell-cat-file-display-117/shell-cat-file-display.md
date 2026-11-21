# 第117批｜shell cat文件路径、读取、格式化输出与D00补正

MYOS2-A-C07c / CHAT-EXEC-17；合同CORE-USER-SHELL-CAT-FILE-DISPLAY-117。固定源码time@a039d9803ade；主线合同发布b2f563a67814；执行父145d6781c75e。**只读源码和给定小整数/字节模型；完整本地P9及任何编译、程序或内核运行NOT_RUN。**

## D00｜三项仍OPEN的历史纠正

### R115-03：新图已有真实调用链，并可走到三个结果
旧115图的实际调用主体不应从sys_read直接连到fd helper。第116批只补了拟修订文字，新图`cluster_R115`把`sys_read→ksys_read→myos_fdget_pos→vfs_read`及`sys_write→ksys_write→fdget_pos→vfs_write`分别接通，且将`rw_verify_area`和`MAX_RW_COUNT`限长归于VFS，将真正`f_pos=pos`归于ksys的非负返回门（CT117-E001、CT117-E002、CT117-E003、CT117-E004、CT117-E006、CT117-E007）。
零设备的`zero_fops.read`只是**方法绑定**到唯一`read_zero`，不是read_zero自调用。源循环先判while(count)，n=0直接返回cleared0；正长度先`memset`、再减count，减到0就break，只有未终结一轮才累计chunk（CT117-E015、CT117-E016、CT117-E017、CT117-E018、CT117-E019）。每条图路径均有回到ksys同值写回f_pos=9的出口，有限输入结果如下：
| 例 | 请求n | 清零块顺序 | 实际清零 | 返回字节 | 最终f_pos |
|---|---:|---|---:|---:|---:|
| Z0 | 0 | 零轮 | 0 | 0 | 9 |
| ZP | 4096 | 4096 | 4096 | 0 | 9 |
| ZP1 | 4097 | 4096、1 | 4097 | 4096 | 9 |

图还区分read_iter和write_iter两条旁支：`BUG_ON(ret == -EIOCBQUEUED)`先于局部位置读回，过门后读只需ppos非NULL，写另需ret>0且ppos非NULL；不能将queued值当成一般负返回继续（CT117-E012、CT117-E013、CT117-E014）。上述0/P/P+1均走标量read_zero而非iter，旧A9/B7/C6不改。**R115-03保留OPEN**。

### R116-01：一条尾项d_off可写到自身，多条尾项则不同
固定`getdents64`的初始化使`prev_reclen=0`，第一条`filldir64`算`prev=current_dir−0`，所以会把**第一条自己的d_off**写成actor提供的offset。FAT actor给该参数固定0，若所有对象、名字和直接写入都有效，则单条批次唯一尾项的d_off=0。第二条成功时更新的是第一条，第三条成功时更新的是第二条；多条批次**最后一条**未由这些赋值初始化，尾补写代码仍是注释（CT117-E032、CT117-E036、CT117-E038、CT117-E039、CT117-E034）。
| 模型 | 成功记录数 | 本函数写入的d_off | 尾项状态 | 用户DIR.filepos |
|---|---:|---|---|---|
| ONE | 1 | 第1条由第1次actor写0 | 0, if actor offset0 and direct store succeeds | 0 |
| TWO | 2 | 第1条由第1次actor写0；第1条由第2次actor写0 | record #2 tail not written by filldir64; requires separately initialized D2 | D2 if readable and known |
| THREE | 3 | 第1条由第1次actor写0；第1条由第2次actor写0；第2条由第3次actor写0 | record #3 tail not written by filldir64; only read with independent initialized D3 | D3 if readable and known |

`readdir`确实读`dp->d_off`到`DIR.filepos`（CT117-E031）；因此两条和三条模型仅在尾记录字段已有独立、确定、可读的D2/D3值时能推出最后用户filepos，不推测malloc缓冲默认零。
同时修复旧116凭空引用的`DS116-P29`：新包用真实仓根路径`myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/closedir.c:26–51`（CT117-P19），旧确有`DS116-P01`可以继续引用。`invalid_name`必须同时核true与false两条返回（CT117-E040、CT117-E041、CT117-P20）；用户syscall宏拆`SYSFUNC_DEF→_SYSFUNC_DEF_→__SYSFUNC_DEF__`（CT117-E042、CT117-E043、CT117-E044），汇编`LABEL_SYSCALL`与`sysexit_ret_addr`的比较/退出段分清（CT117-E046、CT117-E047、CT117-E048、CT117-P24）。**R116-01保留OPEN。**

### R116-02：新图修复了调用者和shell真实控制分支
新图`cluster_R116`将`运行时run_cmd→ls_command`标为间接调用，命令表只是函数指针绑定；`ls_command→opendir`为直接调用，而opendir先做invalid_name，再调用open，open返回后**opendir**调用`__alloc_dir`，后者才调用malloc并判结果（CT117-E020、CT117-E021、CT117-E022、CT117-E023、CT117-E024）。
`readdir`缓存未耗尽则直接取条目；耗尽时由readdir先限制maxread再调用getdents64。内核有记录`prev_reclen!=0`才返回`count−buf.count`字节，无记录则走error返回；有记录时由shell过滤`.`、`..`后**继续下一轮**，普通`alpha`请求输出制表符后继续；只有readdir返回NULL才跳出循环、请求换行、由ls_command调用closedir而后返回1。
旧116 A03的`DIR.fd=-2`仅是存储值，**有效fd持有数0**，不能写成“持有fd−2”。旧A04先成功获得有效fd7，再因DIR分配失败而未在所审函数中关闭，**有效fd持有数1**。旧B01/B02的显示动作按事实修为“跳过并继续”：
| 模型 | 项目 | 新getdents次数（累计） | 记录起点 | offset/size | shell动作 |
|---|---|---|---:|---|---|
| B01 | . | 1（1） | 0 | 24/80 | 跳过并继续 |
| B02 | .. | 0（1） | 24 | 48/80 | 跳过并继续 |
| B03 | alpha | 0（1） | 48 | 80/80 | printf alpha\t 后继续 |
| B04 | NULL | 1（2） | — | 80/80 | NULL 才退出，printf \n，调用 closedir，ls 返回1 |

117的实际DOT R116子图包含有效打开、NULL打开、缓存命中、80字节正返回、0/负返回、过滤、继续、打印及NULL结束，均用CALL/CONTROL/DATA/RETURN区分，旧116原件不改。**R116-02保留OPEN。**

## A｜`cat`怎样构造filename并打开
内建表将`cat`绑定`cat_command`，`run_cmd`间接调用且不消费其返回（CT117-E050、CT117-E020）。限定`current_dir`和`argv[1]`是有效已终止的短字符串，且分配成功。cat先取`len=strlen(current_dir)`、`i=len+strlen(argv[1])`，分配`filename=malloc(i+2)`并将全部容量清零；`strcpy`复制当前目录及其NUL，若len>1就用斜杠覆写该NUL，之后`strcat`从新的第一个NUL位置复制参数字符串及终止NUL（CT117-E052、CT117-E053、CT117-E054、CT117-E058、CT117-E057）。
| 模型 | cwd | argv1 | len | i | malloc字节 | strcat起点 | 最终filename |
|---|---|---|---:|---:|---:|---:|---|
| A01 | `/` | `note` | 1 | 5 | 7 | 1 | `/note` |
| A02 | `/docs` | `note` | 5 | 9 | 11 | 6 | `/docs/note` |
| A03 | `/docs` | `/note` | 5 | 10 | 12 | 6 | `/docs//note` |

其中`/`+`note`时原NUL在第1字节、无额外斜杠，最后为`/note`；`/docs`+`note`时第5字节由NUL改为`/`，第6字节保持预零，最后为`/docs/note`；`/docs`+`/note`不会归一化，形成`/docs//note`。目录解析成功与否不在字符串函数中判断。
随后调用`open(filename,O_RDONLY,0)`（CT117-E055）。若fd<0，错误打印实际是`printf("ls: %s: %s\n", strerror(errno), filename)`，**前缀是ls而非cat**，参数次序为错误文本、filename，函数返回-1；这一分支已分配filename，却既没分配buf、也没调用close或free（CT117-E056）。具体错误文本取决于未在此确定的errno，不虚构字符串内容。

## B｜两次lseek与一次read，不把预计长度当作实际读长
限定有效普通FAT32文件的长度0–32字节且值可表示、两次seek成功、malloc与用户缓冲有效。`i=lseek(fd,0,SEEK_END)`先得到位置（用户原型返回`__off_t`，cat赋入int i），FAT SEEK_END从i_size+0算候选且范围门通过；`lseek(fd,0,SEEK_SET)`随后定位到0，**cat没有检查这次返回值**（CT117-E059、CT117-E060、CT117-E066、CT117-E067、CT117-E068）。
再`malloc(i+1)`，`memset`整块为0，唯一一次`len=read(fd,buf,i)`以**预计长度i**为请求，read返回值覆盖此前当前目录长度变量len，却没有参与随后格式化的字节数决定（CT117-E061、CT117-E062）。
读链在有效file/READ位/access_ok前提下为`ksys_read→vfs_read→read_iter优先`；FAT32_file_ops同时绑定标量`FAT32_read`和`.read_iter=generic_file_read_iter`，因此在所示方法表下优先迭代方法。`generic_file_read_iter`对count=0直接返回0，正count经`simple_filemap_read`，**这里是本批源码核查的停止点**，不把磁盘/页缓存读满或真实f_pos变化当作已证结果（CT117-E071、CT117-E073、CT117-E070、CT117-E075、CT117-E076）。用户lseek/read原型、公开SYSFUNC_DEF调用实例与真正宏定义分列普通位置CT117-P01/P03/P04及新E042–E045。
| 模型 | 预计i | END返回 | SET返回（未检查） | buf分配 | read请求 | 给定read返回 | read提供字节数 | 保留预清零字节 |
|---|---:|---:|---|---:|---:|---:|---:|---:|
| B01 | 3 | 3 | 0（忽略） | 4 | 3 | 3 | 3 | 1 |
| B02 | 3 | 3 | 0（忽略） | 4 | 3 | 3 | 3 | 1 |
| B03 | 4 | 4 | 0（忽略） | 5 | 4 | 2 | 2 | 3 |
| B04 | 2 | 2 | 0（忽略） | 3 | 2 | 2 | 2 | 1 |
| B05 | 0 | 0 | 0（忽略） | 1 | 0 | 0 | 0 | 1 |
| B06 | 4 | 4 | 0（忽略） | 5 | 4 | 0 | 0 | 5 |
| B07 | 3 | 3 | 0（忽略） | 4 | 3 | -5 | 0 | 4 |

尤其短读B03只把前2字节AB写入，后续保持0；非空文件返回0的B06与空文件B05不同；B07普通负返回需要额外前提“读动作未修改缓冲”，才可继续推出空字符串输出。对无依据的失败seek、坏指针、未证文件缓存状态不构造实际运行结果。

## C｜`printf("%s\n")`的首NUL、写请求和资源状态
`printf`先通过`vsnprintf`格式化到4096字节局部数组，`%s`调用`strlen`到**第一个NUL**，按`slen`逐字符EMIT；原格式最后的换行是另一个st_normal字面字符。EMIT只在空间内存放但无论如何累计输出长度，本批模型最长只有4字节，因此格式化结果能放入缓冲（CT117-E079、CT117-E080、CT117-E082、CT117-E084、CT117-E085、CT117-E086）。
`printf`令`rv=vsnprintf(...)`，然后直接`write(stdout->_fileno,buf,rv)`请求rv字节，**不消费底层write返回**，自己返回格式化rv；cat也不检查printf的返回值（CT117-E081、CT117-E078）。下表写的是请求输出字节，不是已显示终端内容：
| 模型 | 文件长 | read返回 | read后缓冲(ASCII/LF/NUL) | 首NUL | 请求输出(转义) | stdout写请求字节 | printf rv |
|---|---:|---:|---|---:|---|---:|---:|
| B01 | 3 | 3 | A,B,C,NUL | undefined | `ABC\n` | 4 | 4 |
| B02 | 3 | 3 | A,NUL,B,NUL | undefined | `A\n` | 2 | 2 |
| B03 | 4 | 2 | A,B,NUL,NUL,NUL | undefined | `AB\n` | 3 | 3 |
| B04 | 2 | 2 | A,LF,NUL | undefined | `A\n\n` | 3 | 3 |
| B05 | 0 | 0 | NUL | undefined | `\n` | 1 | 1 |
| B06 | 4 | 0 | NUL,NUL,NUL,NUL,NUL | undefined | `\n` | 1 | 1 |
| B07 | 3 | -5 | NUL,NUL,NUL,NUL | undefined | `\n` | 1 | 1 |

所以ABC→`ABC\n`共4字节；`A,NUL,B`→`A\n`共2；预计4短读AB→`AB\n`共3；原文件已有`A\n`再加格式换行→`A\n\n`共3；空文件、普通零返回、以及独立假定负返回不改缓冲都只请求一个LF。这些是纸面格式化/写入请求，真实stdout fd、系统调用写成功或终端可见内容均未验证。
资源与返回需分开签名：
| 分支 | filename | buf | 调用close | free(filename)/free(buf) | cat返回 |
|---|---|---|---|---|---:|
| OPEN_FAIL | 已分配 | 未分配 | 否 | 否／否 | -1 |
| NORMAL | 已分配 | 已分配 | 是 | 否／否 | 1 |
| READ_NEGATIVE | 已分配 | 已分配 | 是 | 否／否 | 1 |

正常及read负返回时都调用`close(fd)`但不消费返回；所读cat_command完整体**没有调用free(filename)或free(buf)**。open失败时在buf分配之前返回-1，`run_cmd`不会接收cat的int返回（CT117-E078、CT117-E056、CT117-E020）。该观察只限cat_command本体，不扩大到内核close是否成功、进程最终资源生命周期。

## 交付范围
新增88个1–5行连续源锚、35个普通位置，固定time源来自16份真实文件；旧115/116历史锚保持冻结。117实际图有三个独立子图R115、R116与CAT117，153个节点、193条有类型的边，已在源级检查23条入口到返回路径。完整本地P9、编译/链接、仓库脚本、Graphviz、QEMU/实机、真实文件/终端输出均NOT_RUN。
作者交回状态`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`、`D00=CORRECTION_SUBMITTED_PENDING_C08_REVIEW`。R115-03、R116-01、R116-02仍OPEN，第115/116批仍RETURN；等待C08 Pro独立裁定，旧件和评论均不改。
