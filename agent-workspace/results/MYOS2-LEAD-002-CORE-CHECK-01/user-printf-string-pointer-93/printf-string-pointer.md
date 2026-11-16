---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PRINTF-STRING-POINTER-93
record_type: bounded_printf_string_padding_character_pointer_static_report
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-12
executor_display_name: MYOS2-A-C06c 代码分析执行
produced_by: "GPT-6（当前会话可见家族；后台UNATTESTED）"
model_per_owner: "GPT-6＋Extra"
date: 2026-11-16
base_snapshot: "time（分支名）"
read_channel: connector
status: AUTHOR_STATIC_PENDING_INDEPENDENT_C06_PRO_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions: [SP93-U01, SP93-U02, SP93-U03, SP93-U04, SP93-U05, SP93-U06, SP93-U07, SP93-U08]
---

# 第93批｜字符串读取、字段补齐与指针格式入口的有界静态核查

**先说明最重要的结果：** 本快照 `mykernel/lib/printf/vsprintf.c::pointer` 的活动函数体并没有返回值，原有的switch及所有return均为注释；但外层`vsnprintf`的 `FORMAT_TYPE_PTR` 分支会执行 `str = pointer(...)`，第一个需要使用这一char*返回值的地方就无法建立定义良好的返回契约。因此，本批**不宣称任何%p/%pe实际输出内容，不宣称后缀循环已正常执行，也不把被注释的默认哈希/错误指针格式作为活动行为**。后缀分类只作为假设返回后的独立词法条件图，标记首个未证门。

本批唯一源码基线`time@a039d9803ade`；主线合同`131@2d3517914f13`；原执行父`dbd0ee1684d4`。角色持续为`MYOS2-LEAD-002-CHAT-EXEC-12`、原执行分支和PR17。第92批已按R92-S01/S02/S03限定接收为PASS_PENDING_LOCAL，第91批按R91-S01/S02/S03限定保留，R90C-S01和第90批文档闭合生效；本93**没有D00**、不重做91/92或53/68，不重开任何已闭合项。本文是阶段2作者静态证据，不是内核执行/编译/ABI认证，也不是C06独立验收。

## G00｜接收、固定现场与真实读账

本轮全文新读131、110号Owner续记、124号模型分工、92独立审查与93检查点、92作者正式评论6909800484、C06正式发布评论6910693379、63号GitHub重试、79号包规模，以及协议v2全部P1–P13。九份MyOS2规则其余部分、128接收及本对话之前已加载的Mnemosyne指导仅在**同一有效EXEC12**相同固定对象范围复用；未冒充本轮重做全套指导刷新或接收。91回执、90闭合、92六件和53/68历史限定也仅按本会话既有实际加载与已固定对象复用，未从主线或旧C06b移植私有已读声明。不存在跨项目维护授权。

本轮首先单独全文读取`mykernel/scripts/options_flags.cmake`，然后核`agent-workspace/conventions.md``1(2)逐字句、time/master的options_flags和panic双分支金丝雀。只取固定time源码；默认GitHub检索只用于定位`mykernel/lib/ctype.c`和arch PAGE_SIZE，所有结论回到固定time全文/指定函数。新读的额外依赖包括`mykernel/include/linux/lib/string.h`、`mykernel/include/uapi/linux/string.h`、`mykernel/lib/string/string.c`完整相关memset/memmove体、`err.h`、`ctype.h`、`lib/ctype.c`及x86页面常量。读取旧92回执的数学修正只保留R92-S02算法子域，不扩为实际C对象、编译或运行证明。

G00实核direct lead`2d3517914f13`、exec`dbd0ee1684d4`、master`e8e60cd19540`、time`a039d9803ade`；只有PR16/17开放，17为Draft，执行分支仍原head/base；第93批目录六件及所有93作者正式标记均无已发布成果。通过后本执行者仅可**新增93指定六件和一次本人PR17顶层评论**。没有任何“接收后等待再次授权”的步骤。

## A｜%s参数、精度与错误占位的真实读取

### 从%s取参到判别的边界

`format_decode`遇到`s`设`FORMAT_TYPE_STR`，外层`vsnprintf`以`va_arg(args,char *)`取得源指针，再交`string(str,end,s,spec)`（SP93-E032/E040）。`string`先调用`check_pointer(&buf,end,s,spec)`：

- `s==NULL`：`check_pointer_msg`返回字面`"(null)"`。`check_pointer`调用`error_string`，让它在**输出缓冲**写替代消息并推进`*buf`，内部返回`-EFAULT`；`string`因非零仅返回**已经推进的buf**。这不是`vsnprintf`直接返回`-EFAULT`。
- `s`非空且`(ulong)s<PAGE_SIZE`，或者`IS_ERR_VALUE(s)`成立：替代`"(efault)"`，同样不正常读取原始s对象。在固定x86页宏`PAGE_SHIFT=12`时，容量模型的`PAGE_SIZE=4096`；`MAX_ERRNO=4095`与`IS_ERR_VALUE`只有地址数值模型，非运行地址空间验证。
- `s`未触发上述两种本地分支：`check_pointer_msg`返回NULL，`check_pointer`返回0，`string`继续`string_nocheck`按字节`*s++`读取。**未被这个数值筛查挡下不证明地址可读**；如果实际对象未映射或不具备所需访问范围，第一处实际取字节就须止证。

源码短锚SP93-E001..E007、E012..E015。不能把这个`%s`特有的`check_pointer`机制搬到独立的`%p`分支；%p的源状态在后文另行分析。

### precision限制的是读取循环次数，不是NUL保证

`string_nocheck`先设`int len=0`与`int lim=spec.precision`；循环`while(lim--)`中每次先`char c=*s++`读**一个源字节**，如果读到NUL就break；只有非零字节才可能写入dest、`++buf`并`++len`。输出dest是否真的写入取决于`buf<end`，即使没写也会逻辑推进buf。最后把这个buf、已计len和spec交给`widen_string`；后者还可能进行字段宽度补齐。

| 有效源输入 | 读源字节数 | 逻辑非零字符数 | 最早的正常停止条件 |
|---|---:|---:|---|
| `"AB\0"`、默认precision=-1 | 3 | 2 | 第3次读到NUL |
| `"ABC\0"`、precision=3 | 3 | 3 | 三次读取后lim用尽，**不读取**NUL |
| `"ABC\0"`、precision=4 | 4 | 3 | 第4次读取NUL，break |
| precision=0 | 0 | 0 | `while(0--)`不进入循环；宽度仍可产生空格 |

默认`precision=-1`在循环条件中为真，会继续读取至NUL（或更早对象/有符号递减门），并不是`-1`意味着“读取负一字节”。`%.*s`的精度来自外层`va_arg(int)`；若给`-2`，当前`format_decode`在PRECISION恢复点明确将负值调整为0，不能按外部C库习惯自动解释成“不限精度”。`error_string`仅在spec.precision仍为-1时改为`2*sizeof(void*)`（LP64模型是16）；例如NULL替代字面有6个可见字符，需为检出NUL再读取第7字节，而不是读取16个字符。已指定非负精度则保留给定限制。SP93-E006、E008..E011、E028..E030、E039。

对异常长且没有NUL的源流，`lim--`最终存在有符号int边界；源对象可读性、循环能否达到边界和算术均未认证，不以数学无穷字符串代替有效C对象。

## B｜%s补齐、重叠搬移与%c实际写入

### widen_string的两条分支

如果`n>=spec.field_width`，直接返回此前的逻辑buf；否则计算`spaces=field_width-n`。有LEFT时在当前buf处连续尝试写space，每步先检查`buf<end`却仍`++buf`；没有LEFT时先调用`move_right(buf-n,end,n,spaces)`，然后返回`buf+spaces`。所以**搬移的是已经写进目标缓冲的字符串部分，接着填前导空格**，不是把原始s重新从头读取一遍。这里`buf-n`和`buf+spaces`必须满足真实C对象的有效指针算术前提；若逻辑buf此前因截断已超界，不能凭memmove内部裁剪就宣称整个调用安全。SP93-E016..E022。

`move_right`在当前buf>=end时立即返回；否则`size=end-buf`。若`size<=spaces`，只`memset(buf,' ',size)`并返回，不调用memmove。若仍有可容纳部分，`len>size-spaces`时先裁剪到`size-spaces`，再`memmove(buf+spaces,buf,len)`，最后`memset(buf,' ',spaces)`。所读通用`mykernel/lib/string/string.c::memmove`在`dest>src`时从尾到头按字节复制，适用于该源/目标重叠的条件模型；`memset`线性写目标。但两个通用函数都处在`__HAVE_ARCH_MEMMOVE`/`__HAVE_ARCH_MEMSET`条件门下，本包没有运行预处理、链接或实机，**不签真实二进制一定选择此通用函数**。SP93-E023..E026。

### 有中间覆盖的有限例子

假定有效source字符串、目的对象与通用memmove均成立，`%5s`输入`"AB\0"`且dest有6字节：先读源3字节（包括NUL），写目的前2字节“A、B”；宽度差3导致`memmove`将已写的A、B从目标索引0、1反向搬至3、4，再用`memset`把0..2填空格，最后`vsnprintf`写dest[5]的终止NUL。最终像`"   AB\0"`，需求长度5。目的区经历了**初写2 + 搬移2 + 填空格3 + 末尾NUL1**次字节写入，不能因为最终可见字符串仅5字符就漏记搬移期间的访问。

`%-5s`的`"AB\0"`则先写AB、后写3空格，不走move_right；`%5.2s`输入`"ABC\0"`时源仅读A/B两字节，不读C和NUL，搬移和空格填充后仍形成`"   AB\0"`。如果`%5s`目标size恰为5，流程会先把B搬到最后一个目标位置，再由`vsnprintf`的末尾NUL覆盖它，最终是`"   A\0"`，数学需求5不变。最后这一情景不能误写成“从未写B”或把返回值当最终strlen。

单独分析`move_right(base0,end2,len3,spaces3)`，可见它只把两个可访问位置填space即返回；`move_right(base0,end4,len3,spaces2)`先把len裁为2，若初态ABCD则右移A/B至索引2/3，再填头两个空格，局部末态`"  AB"`。**这些是合法对象假设下的helper模型**，不能无条件拼接为超容量的`widen_string`完整执行，因为其随后逻辑`buf+spaces`可能已越目标对象允许范围。细节在facts的两组搬移矩阵中有前置区间与止证门。

### %c与字符串NUL并非同一返回契约

`format_decode`遇到`c`设CHAR，`vsnprintf`的CHAR分支在没有LEFT时先按`--field_width>0`补前导空格，随后以`(unsigned char)va_arg(args,int)`取字符，尝试写一个字节并递增str，再通过第二个宽度循环补尾部空格。因此`%3c`和实参ASCII Q得到`[space,space,Q,NUL]`，`%-3c`得到`[Q,space,space,NUL]`（均需要有效至少4字节目的对象）。

更关键的是`%3c`、整数实参0：CHAR分支**真的写了一个值为NUL的字符**（在索引2），外层完成时还会写一个格式化终止NUL（索引3），缓冲字节为`[space,space,NUL,NUL]`，数学需求返回3，但对从起点开始的有效C字符串而言，首个NUL在索引2，长度只有2。`"%cX"`配int0同理可形成`[NUL,'X',NUL]`、返回2；不能凭strlen或屏幕可见文字判断格式化总写入数。`%%`则不取va_arg，只条件写一个'%'并推进逻辑str。SP93-E031/E034/E041..E044/E047/E048。

不论是%s还是%c，`vsnprintf`在正常到达out且size>0时才决定NUL写在str或end[-1]，返回`str-buf`；当size>INT_MAX的早退门成立时可能直接返回0而**没有写任何终止NUL**。具体截断的C指针范围问题按R91-S01/R92-S03继续保留，不因最终字符像简单而解除。

## C｜%p真实活动体、后缀分类与首次返回值门

### 第一条无法建立的返回契约

固定源码`format_decode`对`p`设`FORMAT_TYPE_PTR`且推进格式位置；`vsnprintf`的PTR分支实际写的是：

- 以`va_arg(args,void *)`读取一个指针实参作为函数参数；
- 执行`str=pointer(fmt,str,end,ptr,spec)`；
- 源码后面是`while(isalnum(*fmt)) fmt++`的后缀扫描。

然而独立全文读取`pointer`的活动函数体：其原switch、`case 'e'`、`default_pointer`、`return err_ptr`等全部在注释内，最终函数直接闭合，**没有可确认的活动return**。这意味着在外层第一次尝试消费返回值以赋给str时，没有定义良好的char*返回契约；本包**不假定返回原buf、NULL、空串、错误字符串、十六进制地址或哈希值**。在此门后，后缀while、后续普通字面、out的NUL及`str-buf`均不能作为正常完成的实际路径认证。SP93-E033/E045/E046/E049..E051是本批核心强制证据。

这与A专题的字符串指针筛查完全分离。`%pe`位于pointer函数内的`case 'e'`只是注释；虽然`create_worker`中的错误分支确实把`"%pe"`传给`pr_err_once`（SP93-E063/E064），本批未把日志宏/其余下级全部追到vsnprintf，不宣称真实已经输出某种错误文本。也不因为有注释中的`default_pointer`就宣称普通`%p`安全输出哈希。

### 只在额外假设下分析格式后缀词法

`isalnum(c)`实际由`__ismask(c)`从`_ctype[(unsigned char)c]`读类别，再用`(_U|_L|_D)`区分字母和数字。固定`mykernel/lib/ctype.c::_ctype`的ASCII表对A、数字1给相应类别，对下划线和冒号/点号等是标点。因此假定**额外前提**“pointer已经返回一个有效指针且外层继续”：

- `%pe:rest`：fmt进入pointer时指向e，while条件会跳过e，在冒号处停止；
- `%pA1_tail`：会跳过A和1，停在下划线，后继普通字面理论仍可解析；
- `%p-`：第一个后缀字符是标点，条件扫描不会跳过它。

这三者只是被隔离的**词法假设模型**，不是对真实%p运行通路的证明。格式游标被分类并推进，与`va_arg(void*)`消费一次实参、字符串输出buf推进，是三个独立的状态；后缀的字符数量不是指针输出字节数。所读的`_ctype`是固定数组定义、不是把同名宏/注释当成实际映射。SP93-E052..E060、E045/E046。

## 有界矩阵、模型、旧身份与延期

完整机器可读数据在`facts-and-boundaries.yaml`，按真实工作量形成**7组39行**矩阵：`string_input_classification_matrix`6行、`precision_and_read_footprint_matrix`6行、`padding_and_move_interval_matrix`8行、`character_and_embedded_nul_matrix`6行、`pointer_dispatch_and_return_gate_matrix`4行、`format_suffix_consumption_matrix`5行、`ABC_join_matrix`4行。每行都有输入对象、源语句、实际访存范围、条件输出和首次止证门，而不是39次运行测试。

**19个有限静态模型**覆盖默认/边界/超界精度、NULL与低地址替代、%5s/-5s/5.2s、截断后覆盖、通用memmove双方向、%3c与内嵌NUL、%%、%pe和A1下划线的词法、以及%p首个返回门。它们是本轮独立文本/整数/ASCII序列推演，未执行仓库C函数或模拟实际CPU。在证据锚SP93-E001..E064之外，图中的边会将**真实函数调用、数据对象关系、callee返回流、macro展开以及跨调用恢复**分别标注；点和边存在不自动证明运行到达。

`deferred-findings.yaml`登记两项新条件候选：`SP93-F01`（pointer非void活动体缺return且vsnprintf使用其结果）和`SP93-F02`（非NULL地址数值分类不能验证实际可读对象）。另有8个新证据缺口`SP93-U01..U08`，针对该返回门、地址空间/数值转换、极端lim、通用memmove选择、C指针越界、真实vararg/对象、日志路由和后缀词法可达性。并非两项动态复现故障；第二候选还需具体坏指针生产者与首次访问可达证据。

旧`CORE-USER-KTHREAD-NAME-STORAGE-91::KN91-U01`与`CORE-USER-PRINTF-INTEGER-FORMAT-92::PF92-U06`的输出指针根因沿原身份消费；`KN91-F03/F04`、`PF92-F03`、`KN91-U02`和历史53/68限定都不被本批重新登记或关闭。主线R92-S01/S02/S03数学论证仅收窄除100算法的数学适用域，不能延伸为本批内存/ABI/运行认证；R91-S02要求最后像和真实访问足迹分离，R91-S03要求每条短锚及图边的语义支持，不以编号存在代替证据。53已撤回的两条空行误判不得传播。

## G04｜固定目录六件、质量与交回边界

本批只允许新目录`user-printf-string-pointer-93/`中的`MANIFEST.md`、`printf-string-pointer.md`、`facts-and-boundaries.yaml`、`printf-string-pointer-paths.dot`、`deferred-findings.yaml`、`evidence.md`六件，外加本人一次正式PR17顶层交付评论。P6的三个规范整数按**发布前对六件全部源标签的实际统计和独立回源**填写，旧92的110/110/0冻结而不重算替代。64条新锚均在本固定time各自唯一真实定义内部，数组、宏各独立锚，严禁拼邻接注释作活动return证据。

发布前对六件统一做40位十六进制字符串零命中、引用与图节点对应、每引文逐字和1–5行单定义核对、双branch canary、准确公约`1(2)、reader channel、所列无运行项；完整本地机械P9、仓库程序/脚本、预处理、编译链接、Graphviz、QEMU、实机、真实并发均`NOT_RUN`。只用单父Git提交、旧完整真实HEAD做expected_sha、force=false；晚到并行更改须先核归属不覆盖。后置compare严格六件added、旧件零修改删除、六件固定与活动blob/全文及四direct refs、PR16/17和唯一正式评论均须远端回读。

作者仅交`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`，全部发布回读后`STOP_WRITING`；交原“MYOS2-A-C06 内核分析主线续建”由Owner在正式验收消息前选择**Pro**，主线同轮复审93及安排后继。不自行签93通过，不启动94、不修改PR元数据、旧成果、内核或其它分支。

