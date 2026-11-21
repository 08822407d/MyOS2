# 第118批 shell cd/pwd/提示符与D00五项追加补正

执行者MYOS2-A-C07c / CHAT-EXEC-17，固定time=a039d9803ade，主线167=b2f563a67814的后继2b170b8072ed，执行唯一父52c51a3752d3。阶段2静态源码和有界纸面模型；未运行编译、内核或终端。

## D00｜五项待C08裁定的追加纠正

### R115-03：read/write映射、实际入口、位置

新实际DOT的cluster_R115从syscalls_64.h第4/5行read/write映射实例（CW118-P01）经活动__SYSCALL宏定义和switch、生成x64桩、__se/__do内核体，进入真正的ksys_read/ksys_write。引用：CW118-E001、CW118-E002、CW118-E004、CW118-E006、CW118-E007、CW118-E008。读侧由ksys调用myos_fdget_pos，写侧fdget_pos，二者各自调用file_ppos；非STREAM才复制f_pos到局部pos，再以&pos传VFS。VFS核模式/访问/验证/限长和方法选择，非负结果且ppos非NULL时，ksys才实际将局部位置写回file->f_pos；不得把写回归于fd助手。引用：CW118-E009、CW118-E010、CW118-E011、CW118-E012、CW118-E013、CW118-E014、CW118-E015、CW118-E018、CW118-E021。

zero_fops.read是read_zero的表绑定而非自调用，n=0没有循环，正长度先memset后扣减，余0便break而不累计末块（CW118-E026、CW118-E027、CW118-E028、CW118-E029、CW118-E030）。同步read_iter/write_iter各先过BUG_ON(-EIOCBQUEUED)，正常返回才讨论ppos写回，标量zero不走此iter支路（CW118-E023、CW118-E024）。初始非流f_pos=9模型：

| n | chunk顺序 | 减后余量 | 实际清零 | read返回 | 最终f_pos |
| --- | --- | --- | --- | --- | --- |
| 0 | 零轮 | 无 | 0 | 0 | 9 |
| 4096 | 4096 | 0 | 4096 | 0 | 9 |
| 4097 | 4096,1 | 1,0 | 4097 | 4096 | 9 |

### R116-01：尾项不重算，错引文逐项修复

已核单条记录首个actor因prev_reclen=0写自己的d_off=0；两条时两次写第1条；三条时依次写第1、第1、第2条。最后一条只有单条模型具有自身写入来源，多条尾值D2/D3需独立初始化前提（CW118-E041、CW118-E042、CW118-E044、CW118-E045、CW118-E046）：

| 模型 | 记录数 | 写d_off目标 | 尾值 | DIR.filepos |
| --- | --- | --- | --- | --- |
| ONE | 1 | record1 by actor1 (self) | 0 | 0 |
| TWO | 2 | record1 by actor1 (self)；record1 by actor2 (previous) | D2 only if independently initialized | D2 under initialized premise |
| THREE | 3 | record1 by actor1；record1 by actor2；record2 by actor3 | D3 only if independently initialized | D3 under initialized premise |

旧CT117-E040仅判空，true实际是opendir.c47（CW118-E032），false在49（CW118-E033）。旧E046跨汇编标签，本轮分LABEL_SYSCALL(71–73)、sysexit_ret_addr(74–76)、LABEL_SYSCALL_RET(79–81)，分别CW118-E038、CW118-E039、CW118-E040；77–78的errno转换是注释。用户SYSFUNC_DEF三层宏由CW118-E034、CW118-E035、CW118-E036说明。旧116三处不存在的DS116-P29回指CW118-P22 closedir完整体、CW118-P21 closedir(DIR*)公开声明和历史DS116-E058 ls调用；close(int)不是closedir。

### R117-01：首NUL的七行八列替代表

在已清零i+1缓冲且read仅写模型前缀、B07负返回不改缓冲条件下，从真正字节数组寻找首个0，与旧117 facts.B.first_NUL_index及C.first_NUL_at逐项一致。输出请求由%s到首NUL再附格式LF，不能拿read的len直接当写长度（CW118-E082、CW118-E083、CW118-E080）：

| 模型 | 文件长 | read返回 | 缓冲字节 | 首NUL(0起) | stdout请求 | 请求字节数 | printf返回 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| B01 | 3 | 3 | 65,66,67,0 | 3 | ABC\n | 4 | 4 |
| B02 | 3 | 3 | 65,0,66,0 | 1 | A\n | 2 | 2 |
| B03 | 4 | 2 | 65,66,0,0,0 | 2 | AB\n | 3 | 3 |
| B04 | 2 | 2 | 65,10,0 | 2 | A\n\n | 3 | 3 |
| B05 | 0 | 0 | 0 | 0 | \n | 1 | 1 |
| B06 | 4 | 0 | 0,0,0,0,0 | 0 | \n | 1 | 1 |
| B07 | 3 | -5 | 0,0,0,0 | 0 | \n | 1 | 1 |

### R117-02：88个旧引文和89次旧前缀

旧117六件按MANIFEST、报告、facts、DOT、deferred、evidence的前缀出现数分别1/0/0/0/0/88，总数89。多出一次在MANIFEST第113行的说明文字，不是源码短引文。本轮单独统计新六件P2。旧CT117-P05的syscalls_64.h:8–17无法证明read/write/open/close实例；它们在第4/5/6/7行（CW118-P01），lseek实例在第11行（CW118-P02），getcwd/chdir实例在41/42行（CW118-P03）。活动映射宏定义及switch则由CW118-E001、CW118-E002证明。

### R117-03：CAT实际调用者与顺序

新cluster_CAT117由cat_command直接拥有malloc、memset、strcpy、strcat、open、两次lseek、一次read、printf和close的调用边；malloc→memset及memset→strcpy只是执行顺序/数据关系，不是这两个被调用函数互相调用。读由用户read宏实例→内核read映射→x64 switch→生成桩→SYSCALL_DEFINE3(read)→ksys_read→vfs_read。vfs_read直接调用new_sync_read，再由后者通过filp->f_op->read_iter间接调用FAT32_file_ops绑定的generic_file_read_iter。零count直接返回0，正count止步simple_filemap_read入口，实际页缓存和读结果未证（CW118-E007、CW118-E019、CW118-E023、CW118-E047、CW118-E048、CW118-E049）。沿实际图核正常、短读、open失败和零计数模型。

五项R115-03、R116-01、R117-01/02/03仍OPEN。R115-01/02与R116-02已经文档闭合，不由作者重开。历史115–117整体RETURN。

## A｜启动、对象存储期与提示符

本仓myshell.c中PATH_MAX=4096，文件作用域char current_dir[PATH_MAX]为静态存储期，homedir指向字面/（CW118-E050、CW118-E052、CW118-E051）。main先memset(current_dir,0,4096)后getcwd(current_dir,4096)，不检查返回；每次循环开始先独立printf换行，后printf提示符MyOS2:%s # _，再fflush（CW118-E056、CW118-E057）。命令表只是cd/pwd函数绑定，run_cmd才间接调用并不接收返回（CW118-E053、CW118-E059）。

| 模型 | getcwd size | 返回消费? | 写入前提 | 缓存 | pwd请求 | 提示符两次请求 |
| --- | --- | --- | --- | --- | --- | --- |
| A01 | 4096 | 否 | copies '/' plus NUL | / | /\n | \nMyOS2:/ # _ |
| A02 | 4096 | 否 | no bytes written |  | \n | \nMyOS2: # _ |

仅在有效拷贝含NUL的/时可确认缓存/；若getcwd无写入则仍全零，是空C串。只知道返回值但不知写入的字节，不能推定缓存。

## B｜cd选路、内核pwd与缓存刷新

cd无附加参数选homedir=/，否则argv[1]；strcmp(".",path)==0直接返回1，不chdir也不刷新。否则i=chdir(path)，i==0清零current_dir并请求getcwd但忽略其返回；i非0以cd: %s: %s\n格式和strerror(errno)、path请求错误输出，最终仍返回1（CW118-E063、CW118-E064、CW118-E065、CW118-E066）。

内核chdir先user_path_at(AT_FDCWD,filename,LOOKUP_FOLLOW|LOOKUP_DIRECTORY,&path)，仅成功后执行curr->fs->pwd=path，失败goto out直接返回错误而不执行此赋值（CW118-E071、CW118-E072、CW118-E073）。用户系统调用编号chdir=80、getcwd=79（CW118-E061、CW118-E060），宏调用实例不等于定义。失败分支不证明路径查找内部绝无副作用，沿第60批R60-S01。

| 模型 | 选定参数 | chdir返回 | 清零? | getcwd? | 内核pwd | shell缓存 | 错误/正常请求 | cd返回 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| B01 | . | 未调用 | 否 | 否 | /old | /old | 无 | 1 |
| B02 | / | 0 | 是 | 是 | / | / | 无 | 1 |
| B03 | docs | 0 | 是 | 是 | /old/docs | /old/docs | 无 | 1 |
| B04 | missing | -2 | 否 | 否 | /old (no assignment by chdir function on error) | /old | cd: <strerror(errno)>: missing\n | 1 |
| B05 | . | 未调用 | 否 | 否 | /new | /old | 无 | 1 |

B01点名早退；B02无参回家成功、getcwd复制/；B03相对docs成功并复制/old/docs；B04普通失败保留/old，strerror(errno)内容未知；B05内核已变/new而缓存仍/old，点名同样跳过刷新。

## C｜pwd、提示符、getcwd复制和cat复用

pwd_command的if(current_dir)检查的是固定存在数组的非NULL地址，不是检查第一个字节。即使数组为空字符串也调用printf("%s\n",current_dir)并返回1，而run_cmd不接收该返回（CW118-E074、CW118-E059）。main此前单独printf换行，提示符MyOS2:%s # _自身没有尾LF；以下只是stdout写请求，未证实际终端效果。

| 缓存 | 首字节 | pwd请求 | pwd字节数 | 单独LF | 提示符请求 | 提示符字节数 |
| --- | --- | --- | --- | --- | --- | --- |
| / | 47 | /\n | 2 | \n | MyOS2:/ # _ | 11 |
| /old/docs | 47 | /old/docs\n | 10 | \n | MyOS2:/old/docs # _ | 19 |
|  | 0 | \n | 1 | \n | MyOS2: # _ | 10 |

C04给定内核chdir成功已将pwd更新为/new，shell清零后getcwd没有向缓存写任何字节：内核对象仍为/new，shell缓存为空，pwd只请求一个LF，下一提示符先独立请求LF，再请求MyOS2: # _。这不是已运行的故障；如果只知道getcwd返回值或未知部分复制，则缓冲内容无法确定，不应推断pwd文字。

内核getcwd读取curr->fs->pwd/root，kzalloc内部路径缓冲并逆向拼接，检查len与用户size，成功copy_to_user后赋值int error=(long)buf，释放内部bufhead并返回int（CW118-E085、CW118-E087、CW118-E088、CW118-E089、CW118-E090、CW118-E091）。实际复制字节、这个int返回、shell自己的char* getcwd声明及shell忽略返回是四种不同事实，R60-S02禁止把指针转换结果当字符内容证明。

同一shell current_dir缓存还被117批cat用来拼filename，不直接读取内核pwd：

| 缓存 | cat参数 | len | malloc容量 | filename |
| --- | --- | --- | --- | --- |
| / | note | 1 | 7 | /note |
| /old/docs | note | 9 | 15 | /old/docs/note |
|  | note | 0 | 6 | note |

空缓存加note的字面结果是相对路径note，是否能打开取决于之后的内核解析，不是这里的已运行证据。

## 自检、运行范围及交还

本批新P2短窗91条、来源固定time19份文件、普通位置32项；此前115–117原引文只按ID引用。新DOT五个具名子图R115、CAT117、A、B、C，163节点/220带出处边，并静态走通27条关键路径。完整本地P9、预处理、编译、链接、仓库脚本、Graphviz、QEMU、实机与真实shell/stdout均NOT_RUN。作者只报告READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、D00=CORRECTION_SUBMITTED_PENDING_C08_REVIEW，五项仍OPEN，待C08 Pro独立验收。
