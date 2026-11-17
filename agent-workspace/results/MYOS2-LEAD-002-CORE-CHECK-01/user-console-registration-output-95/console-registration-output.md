---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-CONSOLE-REGISTRATION-OUTPUT-95
record_type: bounded_console_registration_builtin_output_video_readiness_static_report
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-13
executor_display_name: MYOS2-A-C06d 代码分析执行
produced_by: "GPT-6（当前可见家族，后台身份UNATTESTED）"
model_per_owner: "GPT-6＋Extra（Owner指定次一档）"
date: 2026-11-17
base_snapshot: "time（a039d9803ade）"
read_channel: connector
status: AUTHOR_STATIC_COMPLETE_PENDING_MAINLINE_INDEPENDENT_PRO
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions: [CN95-U01, CN95-U02, CN95-U03, CN95-U04, CN95-U05, CN95-U06, CN95-U07, CN95-U08]
---

# 第95批：控制台注册、内置输出回调与启动绘字准备

## 结果先行、事实等级与范围

**本次证明了固定源码中的一条有条件的内置控制台链，不证明屏幕已成功显示。** 在 `time@a039d9803ade` 下，`start_kernel → console_init → myos_init_video` 是活动调用；视频准备在注册临时控制台之前进行两次**直接** `color_printk`，另有一次 `pr_info` 进入内核日志机制。视频函数返回后，`console_init` 才初始化临时节点、调用 `register_console`，然后执行另一条 `pr_info`。当列表登记正确、日志格式化和环形区间有效、出现足以触发回调的换行、帧缓冲及几何对象可写时，`__call_console_drivers → myos_tempcon_write → myos_tty_write_color → kputchar` 构成另一条真实源码路径。（CN95-E053–E066）

**主要新认识：** `register_console` 的活动操作仅是按节点地址查重、尾插和列表计数；注释中的 setup、启用、bootconsole交替、日志回放不执行。声明为 `int` 的 `unregister_console` 和 `myos_tempcon_read` 均缺乏活动返回，但没有证据证明某实际调用者已消费它们。内置 `myos_tempcon.write` 在静态对象中明确非空，因此不能把先前“任意console可能NULL write”的假设误称为内置对象的已发生故障。（CN95-E009–E025、E065–E069）

字节消费者方面，`myos_tty_write_color` 的 `length` 是明确传入的字节数而不是 `strlen`；正常字节（包括NUL）调用16×8绘字，LF仅移坐标，BS会绘空白，TAB按列位置绘制1–8个空白字形。**输入字节数、循环迭代数、字形数、128像素乘以字形数、返回值和真实设备显示绝不天然相等。** 颜色输出的 `color_printk` 又有独立的4096B静态缓冲与格式化逻辑返回长度风险，不能混成日志 `printk` 的缓冲系统。（CN95-E027–E051）

结构化材料：`facts-and-boundaries.yaml` 收录 **7组51行条件矩阵**、**22个纯文本/有限算术模型**，并按 `CN95-E001..CN95-E070` 建立索引；图 `console-registration-output-paths.dot` 表达24节点33边的分类关系。候选 `CN95-F01..F06` 共6项，证据缺口 `CN95-U01..U08` 共8项；**动态确认缺陷数为0**。本报告只静态分析，不写补丁、不编译、不运行、不执行Graphviz或实机测试。第94批为 `PASS_PENDING_LOCAL`，仍受R94-S01/S02/S03约束。

## A｜控制台对象、注册行为、节点发布与退出

### 1. 列表头、实际对象和启动注册

固定 `printk.c` 的 `DECLARE_LIST_HDR_S(console_list_hdr)` 使用 `LIST_HEADER_INIT`：`anchor` 前后指向自身，`count=0`。`console_s` 包含32字节名称、`write`（void返回）、`read`（int返回）及真正用于双向链表的 `List_s node`。不是按 `console->next` 的旧Linux代码登记；那段旧代码已注释。实际静态对象 `myos_tempcon` 的 `name="Myos_Temp_Console"`、`write=myos_tempcon_write`、`read=myos_tempcon_read` 先存在；只有 `console_init` 主动调用 `INIT_LIST_S(&myos_tempcon.node)` 后才交给注册器。（E001–E003/E019–E023/E065；此后E编号均指CN95-E）

`register_console(console_s *newcon)` 返回类型为 `void`。活动体先 `list_header_contains(&console_list_hdr,&newcon->node)`，若相同节点指针已在链表，直接 `return`；否则 `list_header_add_to_tail`，沿 `list_add_to_prev → __list_add_between` 更新链表节点，再在链表helper返回后 `count++`。`contains` 比较实际 `List_s*` 地址，不比较名称、index、flags或回调函数是否非空；`__list_add_valid` 对非法前后连接有 `while` 检查，不能无条件认为插入一定顺利返回。（E004–E010/E067–E069）

| 条件模型（均非实际运行） | 列表效果 | 能证明什么 / 不能证明什么 |
|---|---|---|
| 静态空表，合法新节点首次注册 | `count:0→1`、节点尾插 | 源级入链；不是回调已执行 |
| 维持原拓扑，再注册**同一个node地址** | `contains=true`，不再尾插，`count=1` | 同对象去重；不说明同名异对象也去重 |
| 两个已合法初始化的不同node | 顺序添加时 `count:0→1→2` | 可遍历两个对象；**不是**启动实际注册了两个 |
| 一般假设对象 `write=NULL` | 注册器仍未检查write | 假设flush触达会有间接调用停止门；**内置myos_tempcon并非NULL** |
| 人为重复进入 `console_init` 且旧node仍链中 | 会先重新INIT已链接node再contains | 只能标示未证重入寿命风险；不能断言真实启动重复调用 |

原注册函数的 `newcon->setup`、`CON_ENABLED`、`CON_BOOT`、命令行选择、原日志回放`CON_PRINTBUFFER`、`exclusive_console`、`console_unlock`、sysfs通知与登记时日志消息均为 `//` 注释。故 **register返回不会主动执行旧回放、不会报告显示成功**；下一条独立 `printk` 可能刷新早期缓冲，但须另证。另 `for_each_console` 实际宏从 `console_list_hdr.anchor` 经 `list_header_for_each_container` 迭代；`can_use_console` 只测 `count>0`，日志的注释锁并没有变为活动临界区。（E011/E012/E022–E026/E064–E070）

### 2. 注销、读接口和对象寿命的最早门

`unregister_console(console_s *console)` 活动体仅声明 `a,b,res=1`，旧console链移除、列表或标志清理、`return res;` 均被注释，非void函数运行到结尾没有活动return。它不能证明从新的 `List_hdr_s` 删除节点，也不能证明真正返回整数1。**如果**存在实际调用者并消费返回值，就必须先按C语言非void落尾的语义停下；本次范围未找到该消费链，因此不虚构崩溃。相同原则适用于 `myos_tempcon_read`：int接口空函数体，没有活动return；本批实际 `console` 回调链只查到写接口。（E013/E014/E018/E019）

这两个接口问题与列表成员的长期有效性、重复注册、真实并发及未知其它console对象寿命分开记录为 `CN95-F01/U01/U02/U03`。对旧 `CORE-USER-PRINTK-FORMAT-BUFFER-94::PK94-U03`，**只能收窄内置对象write非空的子项**，不能笼统宣布所有外部注册器及可选回调已审结。

## B｜内置输出适配、长度、控制字符与直接彩色输出

### 1. 从console的count到真正的buf消费

内置 `myos_tempcon_write(console_s *co,const char *s,unsigned int count)` 执行 `myos_tty_write_color(s,count,WHITE,BLACK)`，本身 `void`；不会把“已画字节数”回报给 `printk`。被调用者形参为 `const char *buf,size_t length`，局部 `int count=0,line=0`，锁住 `Pos.lock` 后运行 `for(count=0;count<length || line;count++)`。当 `line>0` 时先 `count--`，再跳到 `Label_tab` 继续画空格，因此随后 `count++` 抵消刚才回退，附加空格循环并不消费新的输入字节；同一普通输入字节在判断LF/BS/TAB直至普通分支过程中可能被多次求值读取。（E017/E028–E039/E066）

| 输入（独立正常几何模型） | 输入字节数 | 字形调用数 | 坐标效果与首门 |
|---|---:|---:|---|
| 空串、`length=0`、`line=0` | 0 | 0 | 未进入for；仍发生锁调用 |
| `'A'` | 1 | 1 | `kputchar('A')`，再 `XPosition++` |
| `'\0'` 按显式length传入 | 1 | 1 | `font_ascii[0]`，**不因NUL终止** |
| `'\n'` | 1 | 0 | `YPosition++`、`XPosition=0`，后检查回卷 |
| `'\b'`，原列大于0 | 1 | 1（空格） | 先左移，再在新位置覆盖空白 |
| `'\b'`，原列恰为0 | 1 | 未能正常认证 | 无符号减1先回绕；越界相关转换/绘字早于末尾回卷 |
| `'\t'`，原列5 | 1 | 3（空格） | `((5+8)&~7)-5=3`；后续2次复用同一count |
| `'\t'`，原列0 / 7 | 1 | 8 / 1（空格） | 8列制表位，非输出8个源字节 |
| `A\tB`，起始列0且每一步合法 | 3 | 9 | A一个、TAB七个空格、B一个 |

单个普通字形 `kputchar` 内用 `font_ascii[(unsigned char)byte]`，16行×8列，总共 **128次有条件像素写**，写入前需逐项证实C对象及几何有效；不能从“字体循环有128次”宣称屏幕上实际出现128个可见像素。（E046–E051）

**坐标类型是重点**：`position_t.XPosition,YPosition` 为 `unsigned int`。BS分支先 `Pos.XPosition--`，随后判断 `Pos.XPosition<0`，对无符号类型该比较恒假；若原列0，会产生无符号回绕，并立即用新值乘字符宽度交给 `kputchar`，**然后**才有普通末尾X/Y回卷。这是 `CN95-F02` 的独立条件候选，最早停在运算/参数窄化/指针有效性，不能把屏幕已经出现错误像素或实际崩溃当作已证事实。（E032–E040）

`length` 为 `size_t` 但索引 `count` 为 `int`；当假设合法可读的超长输入令循环超过 `INT_MAX` 次时，后增count有有符号溢出首门。尚无实际调用者提供这种长度的证据，不据此签真实故障。锁函数是活动调用，但不表示已审计所有并发、IRQ及前置锁状态。（E028–E030/E040/E056）

### 2. `color_printk` 是直接绘字入口，不是日志`printk`

`mykernel/klib/printk.c::color_printk` 中静态 `char buf[4096]`；先 `va_start`，调用 **`vsnprintf(buf,PAGE_SIZE,fmt,args)`**，以返回整数 `i` 再调用 `myos_tty_write_color(buf,i,FRcolor,BKcolor)`，最后返回 `i`。固定x86页常量 `PAGE_SHIFT=12`、`PAGE_SIZE=1UL<<PAGE_SHIFT` 的文本值为4096；但是具体构建选择与ABI **没有运行验证**。它既没有调用日志 `printk`，也不依赖临时console节点已入链。（E042–E045）

保留三种不同“长度”意义：`vsnprintf` 报告的格式需求长度、`buf[4096]` 实际存留的非NUL字符数、`myos_tty_write_color` 按 `length` 试图访问的源字节数。以正常的独立文本模型说明（都先假设格式器的C前提成立）：

- 需求2：`i=2`，writer读2个真实字节，通常对应2个字形。
- 需求4095：尾NUL可放在索引4095，writer长度4095；符合大小算术，但不替格式器证明C指针合法性。
- 需求**4096**：格式器只能保留4095个普通内容字节加索引4095的NUL；writer按照 `i=4096` 消费**包含NUL的一共4096个对象内字节**，不能直接误报第4096字节越界。
- 需求**4097**：若隔离假设格式化此前可定义完成且返回4097，writer拟读到 `buf[4096]`，这才越过4096B对象。若格式器自身在此之前已遇未证C指针运算，必须先在旧 `KN91-U01/PF92-U06` 停止，不能伪造同次已成功返回又触发越界的运行轨迹。
- `vsnprintf` 若有负值返回，只是有条件讨论该 `int` 传 `size_t` 的扩大风险；没有来源证明真实负返回发生。

`CN95-F04` 只补新增“逻辑返回值被直接消费者当作可读长度”的不变量，不按新编号复制旧格式器根因。直接彩色绘字与日志 `printk` 的 `vprintk→LOG_BUF→LF→console` 两条路径必须保持分离。（E042–E045，旧 `CORE-USER-KTHREAD-NAME-STORAGE-91::KN91-U01`、`CORE-USER-PRINTF-INTEGER-FORMAT-92::PF92-U06`）

## C｜真实启动先后、视频准备与绘字目标对象

### 1. 启动顺序、Pos赋值与首两次绘字

`start_kernel` 活动顺序包括 `setup_arch(NULL)`、`setup_per_cpu_areas`、中断与时间初始化，然后 `console_init()`，随后才 `late_time_init()`；此处只核实际静态调用顺序，不推定真实BIOS/引导已经正确执行。（E053/E054）

`console_init` **首先**调用 `myos_init_video()`，并不先登记 `myos_tempcon`。视频函数把 `framebuffer.FB_virbase`、`FB_size`、宽和高复制到 `Pos.FB_addr, FB_length, XResolution, YResolution`；然后光标归零、字符框设为 `8×16` 并初始化 `Pos.lock`。接着构造 `char linebuf[4096]={0}`，按 `i < XResolution/XCharSize` 写空格，用 `color_printk(BLACK,GREEN,"%s",linebuf)` 和 `color_printk(BLACK,GREEN,"\n")` 直接绘字，再用 `pr_info` 记录视频初始化消息。**此时尚未执行 `INIT_LIST_S` 和 `register_console`。**（E015/E055–E058/E065）

`console_init` 等视频函数返回后，才 `INIT_LIST_S(&myos_tempcon.node)`、`register_console(&myos_tempcon)`，最后执行 `pr_info("Printk available now.\n")`。视频阶段的 `pr_info` 可能先积存在log ring；注册完成后下一条日志可能经 `console_unlock` 移交已有换行段。**不是**因为 `register_console` 自动回放；真实移交还受到第94批无LF、环尾前缀解析、等mask跨物理段区间、回调前合法对象等旧停止门的限制。（E001/E016/E024/E025/E065/E066/E070）

`linebuf` 必须按真实容量区分：设 `cols=floor(XResolution/8)`，`cols=80` 时原零初始化在索引80仍保留NUL，后续可进入正常格式模型；`cols=4095` 时索引4095仍NUL；`cols=4096` 时零结尾全被空格覆盖，`%s` 已无数组内终止；`cols≥4097` 时更早在填充循环索引4096首次越界。这是 `CN95-F03`，不能把极端分辨率假定为真实硬件配置。（E057/E058）

### 2. Framebuffer对象的一个必要生产入口与最早停止门

源码中 `framebuffer_s framebuffer` 的直接生产者出现在 `mykernel/arch/x86_64/myos/pre_init.c`：`parse_tag` 遇 `MULTIBOOT_TAG_TYPE_FRAMEBUFFER` 才调用 `get_framebuffer_info`，把物理地址、`phys_to_virt` 的虚拟值、`width*height*bpp/8` 的大小、宽高以及 `framebuffer_pitch/(bpp/8)` 存为帧缓冲属性。这只是一跳必要来源；**未**认证FRAMEBUFFER tag必然存在、`bpp/8` 非零、算式不溢出、地址已映射/可写或实际行跨度。（E052/E060–E063）

`tty_io.c::myos_init_VBE_mapping` 确实还有页区间处理、调用 `myos_ioremap` 及刷新TLB的代码，但没有本批可认证的真实调用时序/返回有效性；不能把helper存在或 `Pos.FB_addr` 赋值等同实际可用的显存映射。（E059）

### 3. 字形索引、像素地址算术与屏幕几何

`kputchar(unsigned int *fb,int Xresol,int x,int y,...,unsigned char font)` 先读取 `font_ascii[font]`；字体定义 `font_ascii[256][16]`，每个字形16行。每行用 `addr=fb+Xresol*(y+i)+x`，对8个从0x80到0x01的掩码分别写前景/背景32位颜色，然后 `addr++`；每行字体指针再前移一个字节。（E046–E051）

对单个8×16字形，若数学宽度 `W=Xresol`、左上像素 `(x,y)`，最大的零起算像素下标为

`W*(y+15)+x+7`，

独立的几何必要条件至少包括 `W>0`、`x≥0`、`y≥0`、`x+7<W`、`y+15<H`。例如 **仅作为条件模型**，分辨率80×32、32bpp、pitch=80像素，最末字符起点为(72,16)，末像素下标为 `80*31+79=2559`；4字节像素的最低完整对象预算为 `(2559+1)*4=10240`字节，与该模型 `80*32*4` 对应。这个数学相等**不**证明C语言 `int` 乘法、传参窄化、`fb+index` 指针边界或硬件映射都合法。（E040/E041/E046–E052）

还有一个对象对齐问题：framebuffer生产者另记录 `PixperScanline`，但绘字函数的每行跨度直接使用 `XResolution`，而像素指针类型固定为 `unsigned int*`，没有按 `framebuffer_bpp` 调整。若实际bpp为24或pitch像素数大于宽度，`FB_size`/实际行跨度和绘字目标不能自动一致。例：紧凑的80×32、24bpp模型生产大小为7680字节，而按32位写满80×32像素需10240字节；**没有事实表明实际启动就是24bpp**。因此列为 `CN95-F05/U05/U06` 的条件风险与证据门。（E052/E061/E062）

最后必须区分绘制前的位点与后置状态更新。`myos_tty_write_color` 先调用 `kputchar`，再 `Pos.XPosition++`，再根据列数与行数回卷。对于正常末列，这种顺序可以在合法几何下成立；对于BS在列0产生的无符号回绕或其它非法位置，后置归一化不能追认先前访问安全。即便有有效 `Pos.lock`，也只看到该函数本地spin_lock/spin_unlock，尚未审计并发生产者、IRQ重入和真实显示顺序。（E028–E040/E046–E049）

## 接合矩阵、图和历史去重

完整事实表是 `facts-and-boundaries.yaml` 的 **7组51行**，分别为 `console_object_and_registration_matrix` 7、`list_publication_and_removal_matrix` 7、`builtin_write_and_control_character_matrix` 10、`format_to_count_consumption_matrix` 7、`boot_readiness_and_first_output_matrix` 8、`glyph_address_and_geometry_matrix` 8、`ABC_join_matrix` 4。22个有限模型 `CN95-M01..M22` 覆盖同node查重、NULL回调假设、TAB/BS、NUL和负长假设、4096B边界、linebuf上限、最后字形像素算术和24bpp反例。不是任何仓库函数执行、编译、模拟器或硬件测试。

图 `console-registration-output-paths.dot` 有24节点33边。蓝色CALL只用于实际源码可见的直接函数调用，`INDIRECT_CALL` 仅是给定有效注册对象时的函数指针调用；`MACRO_EXPANSION`、`DATA`、`SOURCE_ORDER`、`MEMBER_BINDING` 不等价CALL，红色STOP表示未证前提。图不宣称是完整CFG，也没有运行Graphviz。

历史保持精确：`CORE-USER-PRINTK-FORMAT-BUFFER-94::PK94-U03` 的实际内置 `write` 已局部补证，但一般注册表非空/未知回调未审；`CORE-USER-PRINTK-FORMAT-BUFFER-94::PK94-U07` 的静态画字链局部补证，但真实像素对象和显示仍未知。`CORE-USER-PRINTF-STRING-POINTER-93::SP93-F01` 在 `%pe` 的 `pointer()` 无活动return处依然先停，`CORE-USER-KTHREAD-NAME-STORAGE-91::KN91-U01` 与 `CORE-USER-PRINTF-INTEGER-FORMAT-92::PF92-U06` 的C对象边界仍在。第94批环区间 `PK94-F05/F06` 以及无LF尾部条件不因本批找到消费者就自动消失。90文档项保持关闭；53主线撤回的两条空行误判不重开，也不等于当时的源码候选已修复。

## 校验、交付状态与禁止越界

本批由独立新接收者完成真实Mnemosyne接收（先记录九规则/指导pending），继而独立加载九份MyOS2规则及P1–P13、110/124，再独立刷新适用Mnemosyne行为约束，最后G00通过。接收及刷新原始记录仅按 `MANIFEST.md` 和facts内嵌说明保全，**没有第七件**。固定94六件、三条交接与94验收评论、R94-S01/S02/S03以及91–93限定独立阅读；不继承C06c已读声明。

本批新短锚 `CN95-E001..CN95-E070` 均从固定time的真实14份主证文件逐字取回核对，单一定义内连续1–5行；另查必要list、TTY声明及双分支canary。整个六件原始文本的来源核验标签只位于 `evidence.md`，按真实标签计数汇入MANIFEST的三整数。图、事实和证据的索引一致性按纯文本检查；**完整本地P9、预处理/构建/链接、仓库程序、Graphviz、QEMU、实机和真实并发一律NOT_RUN**，没有使用额外模型、子代理或付费API。

唯一拟交付为95指定目录六件与 `MYOS2-LEAD-002-CHAT-EXEC-13 / CORE-USER-CONSOLE-REGISTRATION-OUTPUT-95 / FORMAL_DELIVERY` 一条PR17顶层正式评论；不改其它文件、PR元数据、主线或内核。作者只交 `READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`，**不自行签95通过，不启动96**。完成远端内容、单父expected-head提交、严格六件added对比与正式评论唯一性回读后STOP_WRITING；Owner下一次发正式验收消息前选择**Pro**，交原“MYOS2-A-C06 内核分析主线续建”同轮独立验收与安排后继。
