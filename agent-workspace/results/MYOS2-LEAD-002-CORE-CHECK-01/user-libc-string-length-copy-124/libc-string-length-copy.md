---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LIBC-STRING-LENGTH-COPY-124
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-19
produced_by: "GPT-6（当前会话自报；后台effort UNATTESTED）"
date: 2026-11-22
base_snapshot: "time（分支名）"
read_channel: connector
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/175-user-libc-string-length-copy-restart-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/175-executor-continuation/00-C08a-to-C08b-handoff-package.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COMPARE-123-D00-122-review.md"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/facts-and-boundaries.yaml"
  - "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot"
  - "agent-workspace/conventions.md"
  - "agent-workspace/tasks/00-gpt-task-protocol-v2.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md"
  - "myinitramfs/mylib/myglibc/string/strlen.c"
  - "myinitramfs/mylib/myglibc/string/strnlen.c"
  - "myinitramfs/mylib/myglibc/string/strncpy.c"
  - "myinitramfs/mylib/myglibc/string/strcmp.c"
  - "myinitramfs/mylib/myglibc/string/memcpy.c"
  - "myinitramfs/mylib/myglibc/string/memset.c"
  - "myinitramfs/myshell/myshell.c"
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_C08_REVIEW
---

# 第124批：源内字符串长度与复制；R123-01定点纠正

只选择固定time@a039d9803ade（短12）的仓内函数体，在足够大、连续、合法的普通可读对象上作条件性纸面模型。分析中未运行任何程序、内核或真实文件操作。实际sh所链接libc与预处理宏仍为UNKNOWN。正文、机器事实、DOT共用case_id；来源短窗见evidence.md。

## D00 · R123-01（原件不修改）

123主线已审：26个原案例可使用，但原图第497–513行把case入口、strcmp结果或前一个输出与另一函数之间的顺序/条件**误标成DIRECT_CALL_INSTANCE**。本批用真实函数实例作源：find_cmd在149行调用每一个strcmp；cd_command在200行调用strcmp，非零才在203行由cd自身请求chdir；ls_command在236/237行调用两个strcmp、在238/240行分别调用名字/换行printf。整数是RETURN_DATA，非零和&&短路是CONTROL，同实例内发生次序是SEQUENCE；不能由两个被调用者之间的序列推导出直接调用。

| 原case_id | cmd | 实际strcmp次数 | 末次strcmp整数 | find_cmd自身返回 |
|---|---|---:|---:|---:|
| C123-F01 | pwd | 3 | 0 | 2 |
| C123-F02 | pw | 10 | -2 | -1 |
| C123-F03 | rm | 6 | 0 | 5 |
| C123-F04 | rmdir | 8 | 0 | 7 |
| C123-F05 | rmd | 10 | 8 | -1 |
| C123-F06 | cd | 1 | 0 | 0 |

find_cmd逐表项比较，遇0返回当时的索引，耗尽返回-1。例如C123-F01最后比较返回0而find_cmd返回2；C123-F02最后比较返回-2，耗尽后find_cmd返回-1。6例共38次比较，原123机器件已有逐次字节对，此处仅补末次整数、条件和本函数返回。F02、F05的末次非零不能当作find_cmd最终返回的数据。

| case_id | path | strcmp(".",path) | 真实控制与调用 |
|---|---|---:|---|
| C123-D01 | . | 0 | 201 return 1，chdir=NULL（未调用） |
| C123-D02 | ./ | -47 | chdir 203 请求；结果未知 |

| case_id | d_name | 第一次strcmp | 第二次strcmp | 名称printf | 尾部printf |
|---|---|---:|---|---|---|
| C123-L01 | . | 0 | NULL（未调用） | NULL（未调用） | 240换行（必须先在233–235循环退出） |
| C123-L02 | .. | 46 | 0 | NULL（未调用） | 240换行（必须先在233–235循环退出） |
| C123-L03 | .config | 99 | 53 | 238名称输出 | 240换行（必须先在233–235循环退出） |

D01/D02各自有cd实例，L01/L02/L03各自有ls实例。L01第二次比较与名称输出不可达；L02名称输出不可达；L03由ls分别发起名称和换行输出，二者不是互相调用。L01–L03给定opendir成功、一次有效记录及下一次readdir为NULL。**240行仅在while循环退出后请求输出**；记录过滤结束不代表循环结束。机器表中未调用字段为null，DOT保留潜在真实调用点节点并测算从该case入口不可达，未用虚构不存在节点冒充排他证据。

图静态文本检查：205节点、262边、62项正向case到调用点、5项真实潜在但未调用节点不可达、5组禁止CALL边不存在、0项错误调用所有者。R123-01仅`CORRECTION_SUBMITTED_PENDING_C08_REVIEW`；历史R122-01由C08文档关闭，旧123文件保持冻结。

## A · STRLEN/strlen：前导扫描、整字筛选、首次NUL

strlen.c:21–25先undef旧名，未被外部重定义的STRLEN默认为strlen；公共声明在string/string.h:405。函数先按指针模8逐字节扫描直至自然对齐，若遇NUL直接返回与源起点的差；后按8字节unsigned long读取。A候选表达式是`((word-lomagic)&~word&himagic)!=0`，其三操作数依次为减去每字节1的结果、原词取反、每字节最高位掩码。候选命中必须从cp[0]起按字节次序逐个确认，真正的首个NUL决定长度。

固定条件为64位unsigned long、小端字节序、8字节对齐、全部真实整字读取均位于同一预先充分可读的数组对象内；起点模8=5仅是地址示意，非实测运行地址。整字栏为`读取偏移:64位小端值/A-mask`。

| case_id | 首址模8 | 前导实际读下标 | 整字取值/候选mask | 候选确认的下标 | 返回长度 |
|---|---:|---|---|---|---:|
| A124-01 | 0 | 无 | 0:0x4141414141414100/0x0000000000000080 | 0 | 0 |
| A124-02 | 5 | 0,1 | 无 | 前导扫描 | 1 |
| A124-03 | 0 | 无 | 0:0x5858585858004241/0x0000000000800000 | 0,1,2 | 2 |
| A124-04 | 0 | 无 | 0:0x6867666564636261/0x0000000000000000; 8:0x5858585858006a69/0x0000000000800000 | 8,9,10 | 10 |

A124-01空串在第一个整字cp[0]确认0，返回0；A124-02未对齐，先读'A'再读下标1的0，尚未整字读取；A124-03对齐首整字的cp[2]首先为0，返回2；A124-04跳过完整无NUL的`abcdefgh`整字，再在下一个`ij\0XXXXX`整字中检查到总下标10，返回10。长度均**不包含**首个终止NUL。

cat_command固定`current_dir="/bin"`、`argc=2`、`argv[1]="hi"`：260行第一次strlen实参current_dir得4写入len；261行第二次实参argv[1]得2，i=len+2=6；262行请求malloc(i+2)=8字节，263行请求memset(filename,0,8)。到请求值为止；不判malloc成功、路径构建、文件操作及显示（117已有独立范围）。

## B · __strnlen：预算上限与加载宽度不同

strnlen.c:28–30仅在存在STRNLEN预处理定义时把源码符号__strnlen改名；同库内部include/string.h:13声明__strnlen，公共string/string.h:412声明strnlen。两份声明不是二进制符号同一性证明，真实符号表和sh链接为UNKNOWN。

函数先形成`end_ptr=str+maxlen`，maxlen=0直接返回0（有效指针且零解引用）。未对齐先逐字节扫、遇0后按end_ptr夹取；对齐后由`longword_ptr<(unsigned long*)end_ptr`控制整字扫描。B候选表达式`((word-lomagic)&himagic)`，和A相比**缺少~word**，可能误报，仍须依次核查cp[0..7]。循环末按`char_ptr>end_ptr`截到端点后返回偏移。所给端点始终在同一充分可读对象内部，无地址溢出路径；不推论任意短对象的越界安全。

| case_id | maxlen | 首NUL下标 | 整字取值/候选B-mask | 实际检查的下标 | 完整整字读取字节数 | 返回 |
|---|---:|---:|---|---|---:|---:|
| B124-01 | 0 | 3 | 无 | 零预算无读取 | 0 | 0 |
| B124-02 | 2 | 3 | 0:0x6766656400636261/0x0000000080000000 | 0,1,2,3 | 8 | 2 |
| B124-03 | 3 | 3 | 0:0x6766656400636261/0x0000000080000000 | 0,1,2,3 | 8 | 3 |
| B124-04 | 5 | 3 | 0:0x6766656400636261/0x0000000080000000 | 0,1,2,3 | 8 | 3 |
| B124-05 | 9 | 12 | 0:0x6867666564636261/0x0000000000000000; 8:0x585858006c6b6a69/0x0000008000000000 | 8,9,10,11,12 | 16 | 9 |
| B124-06 | 10 | 9 | 0:0x8181818181818181/0x8080808080808080; 8:0x585858585858004b/0x0000000000008000 | 0,1,2,3,4,5,6,7,8,9 | 16 | 9 |

B124-01预算0不读取；B124-02/03/04使用**同一数组**，首NUL固定下标3，预算依次2（NUL在预算后，返回2）、3（恰在预算端点，返回3）、5（预算内，返回3）。B124-05首整字无零而跳过，第二词内下标12为0、预算9，最后截到9。B124-06首词八个0x81使B-mask命中而逐字节检查并无零，A-mask不命中；第二词下标9的NUL才是真结果，预算10返回9。因此返回上限maxlen**不限制一次8字节加载的宽度**；正预算2的模型实际也取了8字节，全部由事先保留的大对象满足读取前提。

## C · STRNCPY/strncpy：复制、补零与返回

strncpy.c:20–24默认STRNCPY为strncpy。完整函数先`size=__strnlen(s2,n)`，若`size!=n`执行`memset(s1+size,'\0',n-size)`，最后`return memcpy(s1,s2,size)`。因而三个真实直接调用点都属于STRNCPY函数实例；__strnlen返回的size为数据，memset早于memcpy只表示SEQUENCE。memcpy.c:59返回dstpp，因此本体返回原始s1而非末地址。memset.c:83也返回自己的dstpp。

固定目标8字节初始`23 23 23 23 23 23 23 23`，源为"cat"或空字符串，源首NUL后仍有充足同对象可读空间；源/目标互不重叠且每例目的容量足够。结果列为目的下标0..7的十六进制字节；索引7始终0x23、界外未改。

| case_id | 源 | n | __strnlen size | 先调用memset的位置/长度 | 后调用memcpy参数（即使size0） | 目标最终0..7字节 | 前n字节有NUL |
|---|---|---:|---:|---|---|---|---|
| C124-01 | strncpy.c:27-33 | 0 | 0 | 未调用 | memcpy(dest,src,0) | 23 23 23 23 23 23 23 23 | 否 |
| C124-02 | strncpy.c:27-33 | 2 | 2 | 未调用 | memcpy(dest,src,2) | 63 61 23 23 23 23 23 23 | 否 |
| C124-03 | strncpy.c:27-33 | 3 | 3 | 未调用 | memcpy(dest,src,3) | 63 61 74 23 23 23 23 23 | 否 |
| C124-04 | strncpy.c:27-33 | 5 | 3 | dest+3,2字节 | memcpy(dest,src,3) | 63 61 74 00 00 23 23 23 | 是 |
| C124-05 | strncpy.c:27-33 | 4 | 0 | dest+0,4字节 | memcpy(dest,src,0) | 00 00 00 00 23 23 23 23 | 是 |

n=0时size0，不执行memset，源码仍有0字节memcpy语句，目的不变。n=2小于源长，只复制'ca'不补0。n=3等于源长，只复制'cat'不补0；不能保证NUL终止。n=5大于源长：先把目的[3,4]设0，再复制'cat'至[0,2]。空源n=4：size0，先补目的[0..3]四个0，再调用memcpy长度0。每例返回指针身份为`s1`。

## 构建边界及后续

仓内myglibc是STATIC目标、string/CMakeLists收集*.c/*.S；initramfs/CMakeLists文本中写musl-gcc路径，并增加myinit、myshell两个子目录；myshell定义sh可执行目标，所读内容**不证明真实sh已链接以上仓内三函数**，也不证明宏重命名是否生效。仅静态文本及普通输入纸面复算，完整本地P9、预处理、编译链接、Graphviz、QEMU、实机和真实文件操作均NOT_RUN。作者成果交C08 Pro独立验收，不执行主线裁决。
