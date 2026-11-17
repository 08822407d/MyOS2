---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PRINTK-FORMAT-BUFFER-94
record_type: bounded_printk_format_buffer_and_console_handoff_static_report
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-12
executor_display_name: MYOS2-A-C06c 代码分析执行
produced_by: "GPT-6（当前会话可见模型家族；后台UNATTESTED）"
model_per_owner: GPT-6 + Extra
date: 2026-11-16
base_snapshot: "time（分支名）"
read_channel: connector
status: AUTHOR_STATIC_PENDING_INDEPENDENT_C06_PRO
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions: [PK94-U01, PK94-U02, PK94-U03, PK94-U04, PK94-U05, PK94-U06, PK94-U07, PK94-U08]
---

# 第94批：printk入口、格式化临时缓冲与控制台移交

## 结果和权限

本批按现有`MYOS2-A-C06c / MYOS2-LEAD-002-CHAT-EXEC-12`继续。固定合同`132@3489bb6ec616`，唯一内核源码`time@a039d9803ade`，执行父`2fe1ce176f4d`。G00已独立PASS：PR16/17只有两项Open，PR17为Draft并停在指定执行父，lead/master/time保持固定direct ref；第94批目录及作者正式marker起点不存在。G00以后只允许原执行分支新增94六件及本人一次PR17顶层交付评论。

**重要结论：** 已得到`pr_err_once → printk_once → printk → vprintk → vscnprintf → vsnprintf`的实际源码调用链，但含`%pe`的调用仍须在第93批`CORE-USER-PRINTF-STRING-POINTER-93::SP93-F01`的`pointer()`无活动return、调用方消费该返回值的边界停止。不能由宏入口闭合推断成功格式化、写入环形缓冲、更不能推断设备显示。另用没有`%p`的普通字面和字符消息完成其余独立链路分析。

可读的源码条件分析还显示：`printk_buf[1024]`中保存的格式化字符数，`for(;*p;p++)`实际扫描的非NUL字符数，`emit_log_char`写入环的次数，`con->write`收到的长度，以及`printk/vprintk`返回的`printed_len`是五个**不同的记账域**。数字相等只发生在满足某些特殊前提的样例中。LOG_BUF为单一物理数组，逻辑序号掩码寻址与可供单次回调消费的物理连续区间必须分开；不能拿环形buffer的逻辑可读状态替代跨环线性C对象有效性证明。

历史状态：93已`PASS_PENDING_LOCAL`但必须合读R93-S01/S02/S03；92按R92-S01/S02/S03、91按R91-S01/S02/S03限定接收。90的文档闭合及R90C-S01保持；53主线已撤回的两条空行误判不恢复，也不代表原53源码候选关闭。第94批没有D00，不修改91–93或任何旧回执。

## A｜日志宏、once与真实参数调用

### 格式源到格式化器：宏拼接、取参与返回分层

固定`mykernel/include/linux/debug/printk.h`中的`KERN_ERR`是字符串字面`"<3>"`；在未提前重定义`pr_fmt`的条件分支，默认`pr_fmt(fmt)`为`fmt`。`pr_err(fmt, ...)`展开为`printk(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)`；`pr_err_once`则展开到`printk_once(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)`。（PK94-E001..E004）

例如`create_worker`分配worker失败后使用不带`%p`的`pr_err_once("workqueue: Failed to allocate a worker\n")`。在其失败分支实际被选中、格式和参数有效且编译所选宏一致的前提下，`printk_once`再调用`printk`；`printk`以`va_start(args,fmt)`创建真实`va_list`，调用`vprintk(fmt,args)`，`va_end(args)`，最后返回`vprintk`的整数结果。`vprintk`再以`vscnprintf(printk_buf+printed_len, sizeof(printk_buf)-printed_len,fmt,args)`交给`vsnprintf`。这些是活动调用边，和把函数名写进注释不同。（PK94-E009..E017/E098）

另一个worker失败源`pr_err_once("workqueue: Failed to allocate a worker ID: %pe\n",ERR_PTR(id))`同样经过上述入口，但当`vsnprintf`进入指针格式分派并尝试消费`pointer()`结果时，**必须按SP93-F01停止**；不能用指针格式旧注释里的`case 'e'`修补一条假想`%pe`成功输出路径。无需为此重做93的全套字符串/指针主体。

### 一次标记的真实先后

`printk_once`是statement-expression宏。每一个实际展开点内声明`static bool __print_once`，判断`!__print_once`后，**先**令其为true，**后**调用`printk`。两个不同的词法展开点分别持有局部static，而同一展开点的顺序第二次进入（假设前次已经写true且无并发数据冲突）会跳过打印语句。（PK94-E005/E006）

由此**不能**推定“多核原子恰打印一次”：所读宏内没有活动原子交换/锁契约；相互竞争的真实调用序列与编译器内存效果未审。若首次进入的是含`%pe`且到旧pointer停止门的路径，只能说在进入printk之前有标记赋值；没有证明第一次已定义良好执行完毕，不能从该未证路径直接续写第二次正常轨迹。

`vprintk`局部`printk_delay()`在格式化前被调用；`printk_delay_msec`非零时会用`mdelay`循环。这里不推断具体时长或真实执行，也不把函数名delay当调度或锁证据。`vprintk`中原`spin_lock`、中断保存、抢占控制与递归防护核心分支多处注释；`console_trylock_for_printk`里原`console_trylock`亦注释。源代码只保留实际活动赋值和检查，不宣称“注释说持锁所以真实已持锁”。（PK94-E014/E015/E036/E037/E074/E077..E080）

## B｜1024B临时缓冲、日志前缀、首个NUL与长度

### 临时格式化、递归提示及首NUL

`vprintk`使用`static char printk_buf[1024]`，本次调用局部`printed_len`初始为0，`current_log_level`来自`console_printk[1]`，其固定初值为4。若`recursion_bug`条件成立，会先用`strcpy`放`recursion_bug_msg`并将`printed_len`置为其`strlen`；随后格式化只传`1024-printed_len`剩余容量。这是一个带前提的分支，不是证明正常执行曾把递归标志置1。可见的旧递归标志赋值和日志锁操作多处被注释，不得冒作活动同步。（PK94-E013/E016/E017/E032..E038）

当正常格式化具定义良好的结果，`vscnprintf`保留的字符数至多为传入容量减1，尾部另有NUL。`vprintk`接着`p=printk_buf`，先`log_prefix`，随后以`for(;*p;p++)`逐个读取字符，**遇到首个NUL就停止**，而不是扫描`vscnprintf`宣称的所有字符数量。字符`%c`实参为0且格式`"A%cB\n"`时，在前序格式对象/时钟条件成立的有限模型中，临时有效字符为A、NUL、B、LF，格式化计数4；实际扫描仅消费A，后面的B/LF不会发到环中。这个分流不是第93批`%c`字节写入单元的重报，而是本批日志消费者的独立输入合同。`sprintf`或`%p`若先发生无定义/未证返回，则不能继续本正常模型。（PK94-E017/E021/E029/E031/E098..E100）

若整个格式需求恰为1024个ASCII A且临时对象确实有1024字节，则在源码的size和逻辑指针门成立下，末尾NUL覆盖数组索引1023，`vscnprintf`可返回1023，后续扫描至首NUL最多读取1023个A；不把这种**恰到边界**的有限输入推广到任意更长格式，否则既有`KN91-U01/PF92-U06`的C指针范围停止门仍先于数学输出结论。

### log_prefix的判定式和真实出参

`log_prefix(const char *p,uint *level,char *special)`首先检查是否以`'<'`开始且后面至少一字符；如果`p[2]=='>'`，对单字符`0..7`写数值等级、对`'c'/'d'`置special；若未提供special指针则特殊格式被拒绝。对于多字符分支，活动判断`p[1]<'0' && p[1]>'9'`两个条件不可能同时为真；这条保护无法据此单独拒绝无数字前缀。接着`simple_strtoul(&p[1],&endp,10)`并以`&7`取等级，必须再要求`endp[0]=='>'`才返回len。（PK94-E050..E057）

完整解析器一路取`simple_strtoul→simple_strtoull→simple_strntoull→_parse_integer_fixup_radix/_parse_integer_limit`。固定base10时`fixup_radix`不会按零开头自动改进制；`_parse_integer_limit`初始rv=0，碰到非数字即break；`simple_strntoull`按rv返回的**消耗字节数**更新`endp`，并没有活动“必须至少有一个数字”的本地门。因此在有效连续字节`"<>A\0"`中，`p[2]='A'`进入多字符分支；解析从`'>'`开始消费0个数字且endp仍指向`'>'`，闭合判断通过，返回`plen=2`并写`level=0`。这是本地有条件输入分类现象，是否违反最终消费者约定仍待独立审核，不称已经影响实机。（PK94-E053/E054/E058..E069）

其它对照：`"<12>A"`读到12后&7得到4并返回plen4；`"<x12>A"`虽然坏保护不拒绝，但数字解析0位后endp停在x，不满足闭合判断而返回0；`"<c>"`若提供special则写c并**不**写level，`"<d>"`同理。`vprintk`对c设plen0且不因这个case强制换行；对d设置plen0而后可落入default，在先前不是新行时额外`emit_log_char('\n')`。出参`level`和`special`不应与`size_t plen`或`vprintk`最终返回混同。

### 同一条文本的五组长度

在独立假定`local_clock()=0`、时间戳`sprintf`得到15字符`"[    0.000000] "`、临时对象/类型/指针均有效时，可分别比较：

| 格式输入 | 临时`vscnprintf`长度 | 扫描到的正文非NUL | 环实际`emit`次数 | `vprintk`返回`printed_len` | 控制台回调前数据长度 |
|---|---:|---:|---:|---:|---|
| 空格式 | 0 | 0 | 0 | 0 | 本消息0 |
| `"A\n"` | 2 | 2 | 20 | 20 | 去掉环内`<4>`后17 |
| `"<3>A\n"` | 5 | 2 | 20 | 23 | 去掉环内`<3>`后17 |
| `"A%cB\n"`、实参0 | 4 | 1 | 19 | 22 | 因没有LF，条件模型为0 |
| `"A\nB\n"` | 4 | 4 | 40 | 40 | 两个换行区间各17 |

注意：“扫描正文非NUL”不包括尾部NUL检查次数、重复读取当前字符或源格式字符串的解析；“环写次数”包含日志等级前缀和时间戳；“回调前长度”指`con->write`的入参`count`而不是设备确认写出数量。表中所有非零时间戳格都以**额外假设时间戳格式化正常**成立为前提，并非标准C路径认证。

为何`"<3>A\n"`返回23而环写20？`vscnprintf`的初次长度5已含3字节原`<3>`；`vprintk`解析后跳过它，行首`for(i<plen) emit_log_char(printk_buf[i])`重发原3字节，并且再次`printed_len+=plen`；加上15字节时间戳，得到5+3+15=23。环内实际只有3+15+2=20字节。该差别不能只写成“返回等于输出长度”。（PK94-E017..E031）

### 时间戳与50字节对象的第一证明门

`vprintk`每次活动行首路径都声明`char tbuf[50]`，读取`local_clock()`为`u64`，对`t`使用`do_div(t,1000000000)`——宏返回纳秒余数，同时将t改为整数秒——再以`sprintf(tbuf,"[%5lu.%06lu] ",(unsigned long)t,nanosec_rem/1000)`建立时间戳，得到`unsigned tlen`。在u64/ulong64及数值有界模型中，时间戳最短15字符、秒数最多11位、文本最多21字符，小于50；但`sprintf`实际进入`vsnprintf(buf,INT_MAX,...)`，它的buffer-size形参**并不是50**。数学字节预算不认证`buf+INT_MAX`等表达式相对于真实50字节C对象的合法性，必须在此留下早期停止门。（PK94-E025..E027/E096/E097/E101）

尤其若`sprintf`的有符号返回异常或`unsigned tlen`超过有效实际对象，随后`for(tp=tbuf;tp<tbuf+tlen;tp++)`本身需要合法C指针与可读范围；不能单凭文本短于50便裁定这一阶段在真实构建中安全。这里沿用`CORE-USER-KTHREAD-NAME-STORAGE-91::KN91-U01`及`CORE-USER-PRINTF-INTEGER-FORMAT-92::PF92-U06`的根因边界，不重复造同根因。

## C｜环形字节写入、游标、换行移交与设备回调前区间

### 环长度、单字节写和逻辑游标

固定`generated/conf.h`声明`CONFIG_LOG_BUF_SHIFT=23`，`printk.c`中`__LOG_BUF_LEN=(1<<CONFIG_LOG_BUF_SHIFT)`，因而在这组固定定义被选中且无覆盖时，静态字节环长度为`8388608`，掩码`8388607`。`LOG_BUF(idx)`通过`idx & (len-1)`定位物理字节；`log_start/con_start/log_end`为静态`uint`逻辑游标（默认零），`logged_chars`为饱和计数。小容量N8的例子只是等结构教学模型，不代表真实内核被改成8字节。（PK94-E039..E049）

`emit_log_char(c)`每次先向`LOG_BUF(log_end)`写**一个**字节，然后`log_end++`。若`log_end-log_start>len`，将`log_start`更新为`log_end-len`；随后独立判断`log_end-con_start>len`而可能同样推进`con_start`；最后当`logged_chars<len`时才递增。因此第9次写入8槽教具时（初始游标零、没有消费者移动），第8次结束`end8/start0/con0/logged8`，第9次覆写槽0后`end9/start1/con1/logged8`。不是一次emit触发一次console写，也不等于旧读者一定读到这第9个字符。实际`uint`会按目标位宽取模，不可把逻辑序号当作无限递增的数学整数。

### 两种不同的跨环危险位置

`call_console_drivers(start,end)`的原输入为逻辑区间，先在`msg_level<0且剩余逻辑长度>2`时尝试`log_prefix(&LOG_BUF(cur_index),&msg_level,NULL)`，随后逐字用`LOG_BUF(cur_index)`检测LF；遇到LF才调用`_call_console_drivers(start_print,cur_index,msg_level)`。当区间从物理环尾开始时，`LOG_BUF(cur_index)`仅定位**一个字节**，并不能自动让`&LOG_BUF(cur_index)`作为线性C字符串跨物理数组尾端读`p[1]`与`p[2]`。例如N8、cur_index7、逻辑四字节`"<3>\n"`分别在槽7、0、1、2，条件剩余4>2；若msg_level<0，`log_prefix`读到p0为'<'后继续尝试p1，但物理数组buf[8]超出对象。必须在这个读取门停止，不宣称成功解析了跨环标签。（PK94-E049/E050/E084/E085）

第二个危险位置是`_call_console_drivers`的切分规则：它只在`(start&MASK)>(end&MASK)`时拆成尾段`[start&MASK,len)`和头段`[0,end&MASK)`，否则直接把**未掩码的逻辑start/end**交`__call_console_drivers`。后者用`&LOG_BUF(start)`作为callback数据指针、`end-start`作为连续字节长度。对于N8、逻辑`[6,10)`，掩码6>2，拆成两个各2字节物理连续段，局部模型没有此类跨界问题；但对于恰好占满一圈、起点非零的`[3,11)`，掩码start与end均为3，判断false，不拆分，直接发送`&buf[3]`和`count=8`。物理数组从槽3到尾只剩5字节，**此时提交的8字节范围并不是一个可供C回调直接连续读取的物理区间**；不能假设环自动拼接，也不能未审注册和真实执行就称已发生设备越界读。（PK94-E089..E093）

这两者是两个不同的最早对象边界：前者在`log_prefix`解析环首标签之前，后者在控制台回调输入区间移交处。均须在当前固定`LOG_BUF`及实际对象有效条件下各自分析，而不能互相替换。

### 先推进con_start、只在LF调用与注释的锁

`console_trylock_for_printk`活动体先把`retval=1`，再经`can_use_console(cpu)`只检查`console_list_hdr.count>0`；原`console_trylock`、`spin_unlock`语句处于注释。`vprintk`在retval非零时调用`console_unlock`。`console_unlock`实际循环里原`spin_lock_irqsave`及尾部信号量释放等为注释；它遇到积压时先快照`_con_start=con_start`、`_log_end=log_end`，接着**先把`con_start=log_end`**，再调用`call_console_drivers(_con_start,_log_end)`。（PK94-E073..E084）

`call_console_drivers`只在扫描到`'\n'`后`_call_console_drivers`。函数尾部的`_call_console_drivers(start_print,end,msg_level)`为注释。因此若唯一新增正文只有`"X"`而没有LF、满足console存在并进入`console_unlock`等模型前提，则函数会逐字符扫描到end，**本轮不会调用任何console->write**，但`con_start`已经前移到`log_end`。下一次flush默认从新的con_start继续，不再包含该旧X段；这不是“暂存直到下一次必会打印”的证明。其它syslog读取路径/注册时回放未审，不能断言所有外部出口永远看不到X。（PK94-E081/E082/E084/E086..E088）

真正`__call_console_drivers`通过`for_each_console`循环后，旧`CON_ENABLED`及`con->write`非NULL筛选处于注释，活动代码直接`con->write(con,&LOG_BUF(start),end-start)`。`console_s::write`类型是`void (*)(console_s*,const char*,unsigned int)`，没有可返回的已写字节数供`printk`消费。若某console无有效write，须在调用点停止；本批没有审完整注册器来证明这一输入状态真实出现。`_call_console_drivers`中原日志等级过滤同样为注释，不能通过函数名或注释签“只输出高优先级”。（PK94-E089..E095）

**一次设备回调的源参数不等于设备确认显示。** 例如有效环内一行`"<4>[    0.000000] A\n"`（无跨界且time模型满足），`call_console_drivers`读取3字节标签并从之后的timestamp+正文开始移交，`con->write`的入参长度为17。即便回调被调用，该函数返回void，是否实际显示在串口、显卡或TTY上仍完全属于未审设备实现与运行证据。

## 结构化矩阵、模型、候选与历史身份

`facts-and-boundaries.yaml`实际有**8组44行**条件矩阵：`log_entry_and_argument_chain_matrix`4、`once_state_and_callsite_matrix`5、`temporary_buffer_and_nul_matrix`6、`prefix_and_length_accounting_matrix`7、`timestamp_and_return_matrix`4、`ring_cursor_and_wrap_matrix`7、`console_handoff_interval_matrix`7、`ABC_join_matrix`4。每行具体区分宏入口、参数类型、实写字节、临时/环缓冲对象、逻辑游标、out参数、返回值与首次停止门。

`PK94-M01..M18`共18个有限纯文本模型，覆盖宏同点/异点、%pe停止、空文本、普通/显式prefix/内嵌NUL/多行、1024B边界、数字及空位prefix、clock0时间戳、N8第9次环写、跨环拆分、同mask整圈、环尾prefix、无LF损失、假设NULL回调。它们都不是仓库函数运行或测试计数。图`printk-format-buffer-paths.dot`按`CALL / MACRO_EXPANSION / DATA / RETURN_FLOW / OUT_PARAM / SOURCE_ORDER / CONDITIONAL_STOP`分边，不能用“节点/边存在”替代实际调用方或有效对象。

`deferred-findings.yaml`有6项独立有条件源码候选`PK94-F01..F06`，分别是空数字前缀、原前缀重复计入return、内嵌NUL与日志扫描分流、无LF片段缺回调且游标前进、整圈等mask的回调区间、环尾prefix线性读取。8项新增证据缺口`PK94-U01..U08`涉及旧%p停止、tbuf真实C对象、可选console write、锁/并发、实际环状态、解析异常、设备结果和消费者返回合同。**并非6项已发生的动态运行故障**；一些是具体本快照行为，一些还缺规范需求与真实生产者。旧`CORE-USER-PRINTF-STRING-POINTER-93::SP93-F01/U07`、`CORE-USER-KTHREAD-NAME-STORAGE-91::KN91-U01`和`CORE-USER-PRINTF-INTEGER-FORMAT-92::PF92-U06`保留完整qualified ID，只追加消费证据，不把旧根因换个新编号重报。

## G04发布条件与最终状态

仅允许新目录`user-printk-format-buffer-94/`中`MANIFEST.md`、本`printk-format-buffer.md`、`facts-and-boundaries.yaml`、`printk-format-buffer-paths.dot`、`deferred-findings.yaml`、`evidence.md`六件，及CHAT-EXEC-12本人一次PR17顶层`FORMAL_DELIVERY`评论。新短锚PK94-E001..E101必须在同一真实定义内逐字1–5行，宏各单独锚定；发表前再核六件全部原始证据标签、源字面/定义边界、事实索引与图边类型，并把规范P6三个实测整数直接填入MANIFEST，而非expected或constructor。

本地完整P9、构建/预处理/编译/链接、仓库程序脚本、Graphviz、QEMU、实机或真实并发均`NOT_RUN`；本会话纯文本/有限数学审阅仅在可观察范围记账。不修改旧成果、旧评论、PR元数据、mainline、master/time或内核。不合并、不建分支PR、不启动95。发布只可用当前原执行HEAD、单父commit、`expected_sha`和`force=false`；若暂态先核远端效果再依63安全重试，未知并行状态不覆盖。

最终只有六件commit、固定及活动六blob、compare六added、四direct refs、PR16/17及唯一本人正式评论均已远端完整回读，作者才能报告`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`，随后`STOP_WRITING`。Owner正式验收前选**Pro**交原“MYOS2-A-C06 内核分析主线续建”，由C06独立复核94及同轮安排后继；本执行者不得自行签94通过。

