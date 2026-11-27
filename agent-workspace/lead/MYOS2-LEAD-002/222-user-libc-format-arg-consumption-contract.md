---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
conversation_display_name: MYOS2-A-C11 内核分析主线续建
produced_by: GPT-6 Astra Pro（本轮系统身份）
actual_backend_model: UNATTESTED
actual_reasoning_effort: UNATTESTED
date: '2026-11-27'
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: c2bb3f55604f
execution_pin_short12: bf0a7ea26dcc
read_channel: GitHub connector；自写有限文本/字节/整数核验
startup_selfcheck_quote: 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。
branch_canary_quotes:
- "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
- "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
self_check_scope: 主线本轮不新增正式源码引文窗；复核作者122窗、图补注及设计READY分别另账。
full_local_P9: NOT_RUN
actual_G04: PENDING_POST_PUBLICATION
D00: []
R158_01: CLOSED_BY_MAINLINE_GRAPH_CONSUMPTION_ADDENDUM
R155_01: CLOSED_BY_156_CHECKPOINT_READ_SUPPLEMENT (retained)
R152_01: 153主线既有闭合保持；152历史RETURN/未证不倒签
owner_decisions_requested_now: []
mainline_design_count:
  conversation: C11
  first_batch: 152
  latest_batch: 159
  before_publication: 7
  after_successful_publication: 8
  thresholds:
  - 8
  - 10
  reminder_at_this_release: 8
  automatic_handoff: false
previous_mainline_design_count:
  conversation: C10
  distinct_batches: 11
record_type: phase2_executor_contract
followup_id: CORE-USER-LIBC-FORMAT-ARG-CONSUMPTION-159
status: READY_FOR_EXECUTION_AFTER_FORMAL_RELEASE
executor_display_name: MYOS2-A-C11c 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-27
executor_model_per_owner: GPT-6.2 Sol＋Extra／高推理；普通执行无需Pro
executor_start_state: NOT_OBSERVED_FOR_159
model_per_owner: 本轮Pro设计/独审；C11c次一档执行，交回C11 Pro独立验收
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
execution_parent_short12: bf0a7ea26dcc
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-arg-consumption-159/
allowed_files:
- MANIFEST.md
- libc-format-arg-consumption.md
- facts-and-boundaries.yaml
- libc-format-arg-consumption-paths.dot
- deferred-findings.yaml
- evidence.md
model_scenarios: 3
independent_input_rows: 10
receiver_guidance_load:
  project_guidance: required
  meta_agent_guidance: required
  mnemosyne_guidance: 'no'
  mnemosyne_receive_format: task_local_only
  mnemosyne_naming: required
  refresh_completed: PENDING_CURRENT_RECEIVER_CHECK
  task_local_only: true
---

# 222号合同 · 第159批 · 用户库格式精度、动态宽度与字符消费

**原MYOS2-A-C11c／EXEC-27继续，GPT-6.2 Sol＋Extra／高推理；本批普通执行无需Pro。** 以本轮真实PR17 TASK_RELEASE评论为入口，读取同提交[158独审](reviews/CORE-USER-LIBC-FORMAT-CAPACITY-158-review.md)、本合同和[检查点](checkpoints/2026-11-27-format158-reviewed-args159.md)，据实刷新规范并fresh G00后，连续完成三个相关专题、三个场景十个独立输入行、恰六件、自检、唯一作者回执及实际G04；通过即STOP_WRITING，交C11 Pro独审及同轮后继，不另等开工、不进入160。

158已裁定PASS_PENDING_LOCAL_WITH_MAINLINE_ADDENDUM，R158-01由主线图义补注当轮闭合，后继D00=[]。原六件bf0a7ea26dcc与[作者7208914225](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7208914225)冻结；只按158独审§4合读，不重算其十二行/122窗，不倒签或修改原记录。R155/R152既有闭合及155历史RETURN/未证保持。本批继续已接收的C11c身份，没有换席、改派或新恢复包。

固定源码time=a039d9803ade、直接master=e8e60cd19540，执行唯一父bf0a7ea26dcc，lead取本次正式评论真实发布提交。本任务适用`agent-workspace/tasks/00-gpt-task-protocol-v2.md`全部十三条；技术范围、输入、强制锚、六件与P9判据分别见§1–8及§9–12。SOURCE_FACT/GIVEN/条件推导/UNKNOWN分列；每行重置，首个无法证明合法的操作之前停止。

## 1. 新增价值与精确排重

本批继续用户程序高频使用的仓内printf/vsnprintf接口，研究**输出精度之前的源读取、动态宽度的取参与算术门、字符NUL进入写请求的方式**。A/B的输出容量一律充足，不重做158的n=0/1/恰满、数字游标或4096长提示符。C采用接口调用的给定模型；所读myshell.c的活动printf点只证明项目使用格式输出，不证明它使用本批的星号/%c模板，更不能证明实际sh链接仓内实现。

设计时fresh读取time、lead和exec完整树，分别1280、2077和2674项，均未截断；在lead=c2bb3f55604f未见222前缀占用。下表旧成果固定于exec=bf0a7ea26dcc，旧主线材料固定于lead=c2bb3f55604f；短码均自连接器输出原样截取。这里只声明下列定点排重，不声称逐字重审全部历史。

| 已读取的旧原件与精确selector | 既有覆盖及159增量 |
|---|---|
| user-libc-string-compare-123/libc-string-compare.md §A/§B/§C | strcmp/strncmp比较、预算、shell比较返回消费；159不再建比较案例。 |
| user-libc-string-length-copy-124/libc-string-length-copy.md §A L66–81、§B L83–98、§C L100–114 | 已有strlen前导/整字/首NUL、strnlen预算与读取宽度、strncpy复制补零。A159_04只新增“精度0的格式调用仍进入strlen”的调用者消费，复用原读取纪律，不重做算法族。 |
| 同124 deferred-findings.yaml::items[id=SC124-READ-BEYOND-BUDGET] | 原条目的源是strnlen.c及maxlen边界，不能改写成strlen缺陷或把A159_04冒记为此条已关闭；它仅用于比较另一个候选方向。SC124-ABI-ASSUMPTIONS、SC124-SH-LINK继续保留。 |
| user-libc-string-copy-append-125/libc-string-copy-append.md §A/§B/§C | strcpy/strcat返回指针及cat路径构造；159不重复首NUL复制、追加或路径容量。 |
| user-libc-memory-copy-fill-126/libc-memory-copy-fill.md §A/§B/§C；user-libc-aligned-word-copy-127/libc-aligned-word-copy.md §A/§B/§C；user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy.md §A/§B/§C | 已有字节/整字复制、对齐、暂存变量与写账。159不进入memcpy/wordcopy，不导入这些原件待合读的旧误记为真值。 |
| 180-user-shell-datetime-format-contract.md §B；user-shell-datetime-format-129/shell-datetime-format.md §B L80–115及§C L117–138 | 旧四例是%s、%06ld、字面宽度6、非负signed long与prec=-1；已覆盖转换重置、p回退、逆向数字写和41字节请求。159不把重置或忽略write返回当新发现；新增星号的有序取参、精度0/正/负、负宽度和字符NUL。 |
| 129 deferred-findings.yaml::items[id=DT129-VSNPRINTF-BOUNDARY] | 原limitation明确四例NUL源、缓冲充足且排除invalid pointers/negative/overflow。159作为该qualified ID的限定证据追加；有效ABI、真实实现与全部边界不因此关闭。 |
| user-shell-error-diagnostics-130/shell-error-diagnostics.md §C L41–53、D00 R129-01；181合同及129/130主审相应消费限定 | 五条双%s短诊断、正确时间序/最终布局和write返回忽略已覆盖。159不增加另一条短错误行，不重开R129-01。 |
| user-libc-format-capacity-158/libc-format-capacity.md §A/§B/§C；其deferred::qualified_evidence_additions[id=CORE-USER-SHELL-DATETIME-FORMAT-129::DT129-VSNPRINTF-BOUNDARY] | 158已区分n与物理对象、strlen/源字节读取与存储门、helper/外层q、提示符请求范围。159让precision、取参顺序和%c字节决定新分支，A/B不再扫n，C不再增长字符串长度。 |

另一个高频接口候选是stdio流的后续读写：fresh核iofopen.c和fileops.c，已读后续读写本体为注释，活动打开路径又已由121正文§A/§B/§C及122的R121-01、ABI122-BUILD-LINK等限定覆盖，不用同名注释补出可执行链。另一候选是124留下的strnlen/strncpy不足对象门；它有价值，但需要另一个以源读取预算为中心的包，本批不混入对齐与复制全族。普通问题按18号追加收存，无新缺陷数量要求。

## 2. 强制源码范围与定义链

固定源码time=a039d9803ade。以下用户库简写均相对myinitramfs/mylib/myglibc/；先读取完整被分析定义，再取连续1–5物理行短窗，保留CRLF、制表符和宏边界。

| 文件与必须定位的定义/结构 | blob短12 | 作用 |
|---|---|---|
| stdio/vsnprintf.c::EMIT、enum flags/ranks、完整vsnprintf | ef7e61a278cb | st_normal重置、st_width/st_prec、c/s分支、is_string、收尾和返回。format_int不进入本批输入。 |
| stdio/printf.c::SZ_4K、printf | e8c0430ad375 | 实际4096字节局部对象、va_start/va_end、rv、write和return。 |
| string/strlen.c::STRLEN默认路由与完整定义 | 7416e487fb71 | A/B源读取前缀。A159_04只核逐字节对齐前导；正常源只需核实际首整字与首NUL。 |
| include/string.h、string/string.h::strlen声明 | 962bf46c48d6／23305a96a57a | 包含定位及size_t返回；声明与函数定义分开。 |
| include/stdio.h、libio/stdio.h::printf/stdout/vsnprintf声明定位 | 76b24e00c502／396bfb0df778 | libio/stdio.h L338的printf原型是注释，include/stdio.h L70另有活动printf声明，libio/stdio.h L367有活动vsnprintf声明；声明定位不等于实际链接证明。 |
| bits/types/FILE.h、bits/types/struct_FILE.h::_IO_FILE | 74f62d695aa3／77633f6b639b | FILE typedef和int _fileno的真实定义。 |
| include/unistd.h、posix/unistd.h::write活动声明及ssize_t | bdb6ffa91612／43f11162433e | int fd、const void *、size_t长度、ssize_t返回；不展开write实现。 |
| include/bits/types.h、posix/bits/types.h、bits/typesizes.h、sysdeps/unix/sysv_linux/x86/bits/wordsize.h | 1c479106072c／7ac0a3f83b2a／d72a1e8c7b9e／94ff7c423930 | __ssize_t、__SSIZE_T_TYPE、__SWORD_TYPE与字宽条件；具体目标类型仍依下面GIVEN。 |
| sys/types.h、posix/sys/types.h | 716732f4d413／bfdcead553a9 | 用户库ssize_t/size_t包含定位。 |
| myinitramfs/CMakeLists.txt、myinitramfs/myshell/CMakeLists.txt | 6757d21bc64c／dcdac0564c0e | -m64文本、外部musl路径和sh目标；不认证实际构建。 |
| myinitramfs/mylib/CMakeLists.txt；myglibc的CMakeLists.txt、stdio/CMakeLists.txt、string/CMakeLists.txt | 8f6abc24095a／b956957a602a／73ca342146a7／3b2073525ead | 独立mylibc及源收集、include目录。 |
| myinitramfs/myshell/myshell.c::main及活动printf点 | 5e177066dc16 | 仅核项目消费背景和实际模板边界，不给159虚构调用方。 |
| mykernel/scripts/options_flags.cmake；mykernel/debug/panic.c::panic | 6019436417ad／2e9d5b195559 | P4配置和双金丝雀；内核配置不能代替用户库实际构建证据。 |

声明路由和源码符号不完成编译器stddef/stdarg、va_list宏、有效include顺序或二进制ABI认证。仓内printf定义到所选vsnprintf是本模型选择；把入口接到本仓实现的边标CONDITIONAL_MODEL_BINDING。没有实际shell调用者，图中不建立main→159模板的真实调用边。

## 3. SOURCE_FACT、GIVEN、条件绑定与UNKNOWN分开

SOURCE_FACT只记录固定源中实际声明、字段、控制语句和调用位置；下面输入、类型与外部返回均单列GIVEN；模型结果标CONDITIONAL_SOURCE_DERIVATION。实际链接、实际内存现场、系统调用/设备效果和屏显为UNKNOWN。

共同GIVEN为ASCII、8位字节、32位int/unsigned int、64位long/unsigned long/size_t/ssize_t/指针、little-endian；int范围[-2147483648,2147483647]。指针到unsigned long的所选语义保留下面低地址位。正常源的对象内8字节读取语义成立，但不推广为通用C有效类型规则或真实编译器保证。所有计数正常至多9，无需计数溢出模型。

每行的fmt是完整有效NUL终止数组；所有列出对象在分析前缀存活、彼此不别名、无并发修改。va_list有效，并按实际va_arg顺序提供所列类型和值；不是ABI取证结果。C的va_start/va_end正常完成。A/B的D逐行重置为0x7e；C的printf局部buf未写区域为未指定，不补造初始化值。

源对象完整字节如下，名称每行重置：

| 对象 | 物理容量 | 地址mod8 | 完整hex |
|---|---:|---:|---|
| S_regular | 8 | 0 | 61 62 63 00 7e 7e 7e 7e |
| S_short | 1 | 1 | 41 |
| S_suffix | 8 | 0 | 5a 00 7e 7e 7e 7e 7e 7e |

每次源读、目标写、指针形成及带符号算术分别过门。允许形成同对象one-past，禁止解引用它；**在首个无法证明合法的操作之前STOP**。不把该操作写入已发生事件，不继续取参、访问或返回；只报告停止前状态和未到达部分。停止行没有“最终缓冲”或“正常返回”数值。未覆盖无效va_list、浮点、%n/%p、超长十进制宽度解析、数字格式器或其它errno/设备路径。

## 4. A · 精度裁剪发生在何种源读取之后

A入口统一为vsnprintf(D,16,"[%.*s]",ap)，D是物理16字节数组。

| case_id | 依次给定的变参 |
|---|---|
| A159_01 | int 0；const char * 指向S_regular[0] |
| A159_02 | int 2；const char * 指向S_regular[0] |
| A159_03 | int -1；const char * 指向S_regular[0] |
| A159_04 | int 0；const char * 指向S_short[0] |

必须按源顺序核动态precision的读取/负值规范化、%s取指针、strlen返回赋给int slen、精度比较、输出循环和收尾。分别记录strlen的实际源读范围、输出循环额外读取的字节与真正写入；precision0不能直接当作“字符串参数未消费”。

供规格比对的条件结果如下，执行者仍须从源字节独立重算：

| 行 | 条件结果／最早停止 | 只在正常返回时的完整已写前缀（含收尾NUL） | q偏移／o／return |
|---|---|---|---|
| A159_01 | strlen后slen被裁到0 | 5b 5d 00 | 2／2／2 |
| A159_02 | 字符循环读取a、b | 5b 61 62 5d 00 | 4／4／4 |
| A159_03 | 负精度变为-1；循环读取a、b、c | 5b 61 62 63 5d 00 | 5／5／5 |
| A159_04 | strlen前导先读S_short[0]，再合法形成one-past；下一次前导的*char_ptr将读S_short[1]，在该读前STOP | 不适用；停止前只有D[0]=5b，其余D内字节仍为7e | 仅停止前q=1、o=1；正常返回未建立 |

A159_01–03的strlen读取首个完整8字节对象并按源码确认首NUL；输出循环长度与这次读取宽度不同。A159_04不进入整字加载，且尚未得到strlen返回和slen赋值，不能补后面的precision比较、右括号、收尾NUL或printf请求。它只给124读取规则增加一个precision0调用者的最早停止实例。

## 5. B · 动态宽度先取参、后取反，再消费精度和字符串

B入口统一为vsnprintf(D,32,"[%*.*s]|%s",ap)，D是物理32字节数组。

| case_id | 依次给定的变参 |
|---|---|
| B159_01 | int 5；int 2；const char * S_regular；const char * S_suffix |
| B159_02 | int -5；int 2；const char * S_regular；const char * S_suffix |
| B159_03 | int -2147483648；int 2；const char * S_regular；const char * S_suffix |

建立有序的va_arg消费账：宽度int、精度int、第一指针、第二指针，逐项写实际到达与未到达。普通正宽度在首字符串前填空格；可表示的负宽度先取反再置FL_MINUS，在首字符串后填空格。第二个%s重新建立flags/rank/width/prec；这条旧事实仅用于检验此次动态参数是否串入后续转换，不重复报新发现。

| 行 | 条件结果 | 已写前缀（末尾另含00） | q／o／return |
|---|---|---|---|
| B159_01 | 首字段右对齐；后一个%s取S_suffix | "[   ab]\|Z"；hex 5b 20 20 20 61 62 5d 7c 5a 00 | 9／9／9 |
| B159_02 | 首字段左对齐；后一个%s取S_suffix | "[ab   ]\|Z"；hex 5b 61 62 20 20 20 5d 7c 5a 00 | 9／9／9 |
| B159_03 | 宽度int已取到；-width的数学结果2147483648不能用所选int32表示，在该取反前STOP | 不给最终前缀；停止前只有D[0]=5b，其余D内字节仍为7e | 仅停止前q=1、o=1；正常返回未建立 |

B159_03停止时width保留输入值，FL_MINUS尚未设置，prec尚未由第二个星号读取。后面的三项参数虽已作为输入给出，并不等于被取用；不产生strlen、字符串写入、收尾NUL或返回。不能把宿主语言的无界整数结果或二补数回绕当作该C操作的返回。

## 6. C · %c的NUL是数据，收尾NUL另有来源

本组每行直接给定一次接口模型调用；选择仓内printf及vsnprintf，不添加实际shell调用者。stdout指向有效FILE，int _fileno给定7。write只作为函数调用请求，额外给定它正常返回ssize_t 0；没有实际写入字节、内核或终端效果的GIVEN。

| case_id | 模型调用 |
|---|---|
| C159_01 | printf("A%cB", (int)0) |
| C159_02 | printf("A%.0cB", (int)0) |
| C159_03 | printf("A%3cB", (int)0) |

必须核%c用int取参再转char，carg是单独的一字节局部对象，sarg=&carg、slen=1并进入共用is_string。字符复制时的读取与sarg++允许形成该对象one-past；不读carg之后的内存，不调用strlen(carg)。C159_02仍发生va_arg和carg赋值，但精度门决定后续复制次数；不能说precision0取消了取参。

| 行 | printf内write请求的有效字节（不含最终终止符） | 转换产生的NUL位置 | 最终收尾NUL位置 | vsnprintf返回／write长度／printf返回 |
|---|---|---:|---:|---|
| C159_01 | [0,3)：41 00 42 | 1 | 3 | 3／3／3 |
| C159_02 | [0,2)：41 42 | 无 | 2 | 2／2／2 |
| C159_03 | [0,5)：41 20 20 00 42 | 3 | 5 | 5／5／5 |

三行的请求fd都来自stdout->_fileno，在本GIVEN取7，不能把7推广为真实stdout。源代码按rv传size_t长度，不重新strlen(buf)，因此转换产生的NUL不取消其后B字节的请求；最终收尾NUL则在请求区间之外。输出图须保留字符NUL与收尾NUL的不同写入者/时间点。write给定返回0只用于允许抵达末尾return rv，不能反推成功显示；忽略write返回是129/130/158既有事实，本批只连接新的字节布局。

## 7. 证据与机器表示

所有条件表从源代码和GIVEN重算；不能把本合同的结果列抄入检查器当独立证明。可以使用自写有限文本/字节/整数助手，但不编译、预处理、运行项目C/ASM/脚本、Graphviz、QEMU或真实I/O。独立READY只消费GIVEN输入和固定源，独立结果须与正文/facts/图实际逐行比对，不能以助手退出码代替语义核查。

facts至少分开source_facts、given、conditional_bindings、unknowns以及十行case记录。每行保留完整输入、取参序号/类型、相关解析状态、物理对象、源读取序列、目标存储时间序、局部游标/计数、正常返回或停止前状态、未到达操作；C另列printf局部buf、rv到write长度的转换、write请求及返回值的实际使用。正常行只需给已写前缀和未写区间，STOP行禁填正常末态。不要复制大型状态机或把整张数组逐字节反复展开。

DOT每行拥有自己的入口和停止/返回可达性，可复用有守卫的模板，但不能跨行借节点绕过STOP。**所有连接相邻seq事件的边统一relation_type=LOCAL_CONTROL，与节点/目标的event_kind分列**，不能用目标事件类型冒充关系。DIRECT_CALL、MACRO_EXPANSION、ARGUMENT_DATA、RETURN_DATA、CONDITIONAL_MODEL_BINDING等另有依据的关系独立登记，不混入这条相邻控制链。vsnprintf拥有strlen调用和EMIT宏位置；printf拥有vsnprintf及write调用。C从不透明write请求到printf-return的控制边须显式guard=write正常返回，rv的返回数据源仍是vsnprintf，不能画成write返回数据。C无strlen调用；B159_03不越过取反门；A159_04不从尚未返回的strlen借到slen。

正式P2强制锚覆盖：EMIT定义；vsnprintf局部类型与状态初始化；百分号重置；width星号取参/负值取反/置旗的真实顺序；precision星号及负值规范化；s分支的strlen与c分支的局部字符；is_string精度门、前后填充及读后EMIT；strlen默认路由、前导循环、所用整字/确认路径；格式收尾及o返回；printf局部buf/rv/va_start-va_end/write/rv返回；FILE._fileno及write活动声明的定义链；用户构建定位和P4活动配置。声明/包含定位不冒充函数体；每个直接定义按自己的符号归属取窗，不用一窗覆盖远处多段，也不把123–158旧引文数计入本席P6。

## 8. 恰六件与消费限界

唯一结果目录为agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-arg-consumption-159/，只新增MANIFEST.md、libc-format-arg-consumption.md、facts-and-boundaries.yaml、libc-format-arg-consumption-paths.dot、deferred-findings.yaml、evidence.md。沿用C11c／EXEC-27、原exec分支及Draft PR17；执行父与lead固定入口以主线正式发布治理段为准。本次无D00；当前接收/刷新记录只写入该六件，不另增第七件或修改158原件。

适用协议v2全部P1–P13。MANIFEST显式记录本次receiver_guidance_load的实际profile/status，说明沿用221已接收的C11c持续执行身份、本次fresh刷新与按固定身份有效复用的材料、适用性/冲突核对；不倒改158或声称旧件曾有该同名键，也不为这个补记另开新席、全面重读未变且本席确已读过的材料。并据实记逐件P6字面计数和源窗回核、三场景十行覆盖、条件绑定、真实写入/恢复及G04；完整本地P9和实际运行继续NOT_RUN。没有后台模型/effort认证，不把作者交付写成主线通过。普通源码边界和有效ABI/真实写结果按旧qualified ID追加限定证据，保留UNKNOWN，不因本批小模型而关闭129、124或08的真实链接/运行缺口。

主线同轮实际发布159成功时，C11首次设计发布计数由7变8，最终回复须醒目提示达到8批提醒线；该提醒不等于上下文测量、自动移权或停工。


## 9. 同席接收、规范刷新与权限

业务授权来自Owner本轮明确“独立验收并同轮安排后继”、既有00工作令/28同轮连续安排及本合同；仍是阶段2、任务MYOS2-LEAD-002-CORE-CHECK-01、原exec分支和Draft PR17，只新增本159结果目录的六件。本人已经接收的[221包](221-executor-continuation/00-C11b-to-C11c-recovery-package.md)与[启动词](221-executor-continuation/01-MYOS2-A-C11c-receiver-startup-prompt.md)只作身份/加载来源，不重新声称158尚未交付，不复用前任C11b的已读。

下面十五件是完整必需入口。本次主线fresh核其当前身份未变；C11c应fresh核ref/Blob与适用性，只能按79复用本席自己在158确已读过的同一版本全文，变化或缺项实际读完，不能复用他席读账。当前00含§12第20条，正文411LF；范围/冲突与当前胜任/容量据实判断，不把取得文件等同阅读全文。

| 规范文件 | 当前Blob短12 | LF |
|---|---|---:|
| [00-work-order.md](00-work-order.md) | c357eeebcfe5 | 411 |
| [README.md](../../README.md) | 1deef3891013 | 52 |
| [conventions.md](../../conventions.md) | ca21465d1319 | 63 |
| [00-gpt-task-protocol-v2.md](../../tasks/00-gpt-task-protocol-v2.md) | ebb0e304cf48 | 117 |
| [18-deferred-findings-and-resume.md](18-deferred-findings-and-resume.md) | 4d22d565ab9e | 108 |
| [24-chatgpt-specialist-conversation-separation.md](24-chatgpt-specialist-conversation-separation.md) | a0156f00594d | 41 |
| [28-review-and-next-batch-same-turn.md](28-review-and-next-batch-same-turn.md) | 9a8b0d039cef | 45 |
| [63-github-write-retry-and-readback.md](63-github-write-retry-and-readback.md) | 2dd70d822b3b | 79 |
| [79-work-package-sizing-and-pro-review.md](79-work-package-sizing-and-pro-review.md) | 3b92828411e5 | 68 |
| [110-owner-communication-and-delivery-continuity.md](110-owner-communication-and-delivery-continuity.md) | ae8f04daa248 | 58 |
| [124-model-routing-and-pro-message-budget.md](124-model-routing-and-pro-message-budget.md) | c84b0e04c524 | 65 |
| [193-mainline-context-thresholds-and-model-availability.md](193-mainline-context-thresholds-and-model-availability.md) | a66d24912f6f | 68 |
| [194-owner-continuity-and-copyable-return.md](194-owner-continuity-and-copyable-return.md) | 9ad1f8667620 | 140 |
| [198-repository-first-compact-relay.md](198-repository-first-compact-relay.md) | 5f2b12f75534 | 59 |
| [210-mainline-c11-recovery-and-context-thresholds.md](210-mainline-c11-recovery-and-context-thresholds.md) | 56dddd4d796d | 67 |

MA fresh核直接master及建设分支、现行入口，当前master=b234c6c91a2e、建设=06b783861368、入口Blob881fd8c1076e、检查点54ac6e5fc66c。有效采用仍48→47 v1.4-r10.266（119597a75517，47 Blob e3c7e284a6c4），45共用归档3492039a99ba内01–14/16十五条同版全文及现行model registry0924de22a54d。建设检查点核当前现状和新增适用采用段，真实登记范围；不把§322待议当已采用，也不要求为同席续作重读全部无关施工史。入口：[MA现行规则](https://github.com/08822407d/Meta-Agent/blob/b234c6c91a2e/current/behavior-guidance-entry.md)。

Mne直接master=e581eb9df6c4；仅借用本席已读README、receive命令、人工批准规范和命名§7，Blob分别04e450fc22f7、173f2ea983fd、058bdbbe3ccb、2d930f07381e；[命名原件](https://github.com/08822407d/Mnemosyne/blob/e581eb9df6c4/notes/registries/project-research-display-name-registry-v0.1.md)实际584LF，§7 L445–584。不导入Mne业务、任务或写权；登记显示名不等于操作UI。

在本159 MANIFEST显式写`receiver_guidance_load`：profile列project_guidance=required、meta_agent_guidance=required、mnemosyne_guidance=no、mnemosyne_receive_format=task_local_only、mnemosyne_naming=required；status列本轮真实refresh_completed和task_local_only/current_task_preserved，并逐件列本席full-read来源或本轮新增实际读取/缺项/冲突/适用判断。尚未完成不能填true；缺字面键的158旧件保持原样，本次字段不是历史补签或新增D00。真实后端及effort未认证如实UNATTESTED，不能据此重复索要已给授权。

保留18普通问题追加收存、24职责、28同轮后继、63先核远端效果再限次重试、79合包/复用、110连续协作、124模型分工、198仓库正文/短入口、210的8/10提醒。无实际阻断则连续完成授权终点。

## 10. fresh G00与冻结基线

1. 四refs：lead等于本轮真实发布提交，exec=bf0a7ea26dcc、time=a039d9803ade、直接master=e8e60cd19540；不能用PR.base_sha代替master。原PR16 Open/Ready、17原标题Open/Draft、原head/base master且未合并；开放PR全分页至空。
2. PR17评论无since全分页至空。本轮写前312条／73既有精确标记；本次正式评论两新标记后，入场预计313评论／75标记，全部按[检查点](checkpoints/2026-11-27-format158-reviewed-args159.md)逐字一次并核真实ID。当前159发令、158作者7208914225及158改派7208211734各直接ID双读，与分页正文和时间一致。
3. 检查点完整19负标记均0，特别是旧EXEC-24/25/26不得成为159作者。正确EXEC-27/158只1；正确EXEC-27/159入场0。完整tree及全部评论检查159目录、任意作者159回执、未知并行变更；出现真实占用先保全，不覆盖、不改署。不能仅核指定身份而漏其它身份。
4. 158六件固定bf0a7ea26dcc、活动exec及API Git Blob全文一致；321481字节/12156LF/258CRLF，唯一父abbf15de6fbb、1提交6新增零其它改删；2271旧非tree冻结。只核身份和本次158主线消费限定，不重算已验数值/旧引文。159写前完整exec为2674项/2277非tree，全作冻结基线；lead入场预计2080项/1845非tree，全作只读基线。
5. 两条time金丝雀、直接master不命中、公约完整原句实际核；MANIFEST有read_channel及self_check三整数、D00=[]、R158/R155/R152闭合来源、历史不倒签、真实当前刷新/权限/容量/下一步。fresh G00完成后连续A/B/C，不把本主线或设计READY当自己的G00。

## 11. P1–P13、自检、唯一发表与实际G04

恰六件文件名见头部allowed_files；接收/规范/空D00进MANIFEST，历史qualified索引和限制进deferred，不新增第七件或单独接收评论。新增正式证据窗1–5连续物理行，定义/宏/条件实例归属真实，原TAB/CRLF/LF/NONE逐字保留，P6满足n=m+k。旧158作者122、主线0及159设计READY均不得混入本席159P6。

P9可达验收按编号列账：1全文无40位十六进制；2正式窗原字节/真实定义；3输入/引用路径实存；4行号不超真实物理行；5P6字面计数及等式；6外部URL可解析/无内部检索句柄；7两time金丝雀和master反验；8公约原句及read_channel；9无代码件为NA；10无补丁/改动块为NA；11无未运行却宣称构建运行完成的禁用字样；12非降级为NA；13§7强制锚全部实际有证。通过这些可达项也不把完整本地P9改为已运行。

写前fresh四refs、原PR、目录/任意作者占用和全评论。只以bf0a7ea26dcc为唯一父，expected_sha匹配、force=false，原exec一次提交六新增零其它改删。先回读六件固定/活动/API Blob三渠道全文、独算Blob/LF/字节/行尾、唯一父/compare、所有2277旧非tree及主线冻结，再发表一条唯一作者评论。若已有局部成功，按63先查实际效果再续缺项，禁止重复正式写入。

唯一作者标记：

MYOS2-LEAD-002-CHAT-EXEC-27 / CORE-USER-LIBC-FORMAT-ARG-CONSUMPTION-159 / FORMAL_DELIVERY

作者回执含真实合同/父/提交/六件、3场景10行、独立回源/P6/当前规范实际状态、D00=[]、R158图义闭合和R155/R152保持、模型可核与未证边界；发表时actual_G04=PENDING_POST_PUBLICATION、mainline_acceptance=PENDING，不预签。

发表后实际直接读新ID两次，与全分页正文/时间相同；75旧标记+新作者=76，各字面一次/真实ID正确，19负标记0；预计314评论，仍实际读至空，入场313旧评论正文及created_at/updated_at逐条冻结。四refs（exec为真实159提交）、原两PR和开放页、六件三渠道、唯一父、一提交六新增、旧2277非tree与完整lead冻结全部实核。成功才actual_G04=PASS、READY_FOR_MAINLINE_REVIEW、STOP_WRITING；不进入160。

63恢复先核远端效果，通常首次加三次10/20/40秒或实质工作间隔限次重试，参数拒绝/调用上限/真实写重试/读回暂态分账；未知并行写不覆盖。最终按198仅一个通常3–6行原生短复制块、一个真实作者评论URL，报实际159交付/G04/停写与必要未知，请MYOS2-A-C11使用Pro独审并同轮后继；不把完整长任务书搬回聊天。

## 12. 独立READY、计数和限制

159独立READY：先从GIVEN与固定源封存十行模型，195项（176有限断言、13源锚、6身份）0失败；后读合同158项（96语义、53源表、2声明、6封存、1候选身份）0失败。覆盖核心输入/源表/对象及图契约，不代签历史检索过程、远端G00/G04或未来作者P6。独算先只读GIVEN与固定源、封存模型后才读候选合同；未用设计答案当oracle。主线另核源码的st_width/st_prec、%s/%c、is_string、strlen前导/整字、EMIT/收尾及printf的rv/write/return，设计分工27份源身份与time树匹配。最终合同与就绪模型逐行比对；READY仅证明此包在指定有限条件下可执行，不代签未来作者成果。

159首次成功发布使C11设计152–159共8批，按210在本轮最终回复开头醒目提示达到8批线；不重置、不计158验收/图补注/同批改派为新批。10批线尚未到，C10历史11保留。未来交接必须带十五项目规则、有效MA全文和同版共用全文、Mne任务局部命名/接收、真实断点/身份/计数及口述约定；提醒本身不自动移权或停工。

阶段2，仅静态源读与自写有限文本/字节/整数核验；不执行项目程序/脚本、预处理、C/ASM、编译链接、Graphviz、QEMU/实机/真实I/O。不得借本任务改源码、旧件/旧评论、主线、PR元数据、新建分支/PR、合并/强推/删除或跨仓写。完整本地P9 NOT_RUN；实际ABI/编译器头/链接、调用者现场/真实I/O及后台模型/推理档仍未认证。
