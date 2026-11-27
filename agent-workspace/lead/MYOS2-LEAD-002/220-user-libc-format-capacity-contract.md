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
startup_selfcheck_quote: 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。
branch_canary_quotes:
- "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
- "\tthis_cpu = smp_processor_id();"
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
previous_mainline_design_count:
  conversation: C10
  distinct_batches: 11
model_per_owner: 本轮事务性恢复无需Pro；新席GPT-6.2 Sol＋Extra／高推理，正式独审交C11 Pro
read_channel: GitHub connector；自写文本/字节/元数据核验
lead_before_publication_short12: a74bf1dc357b
execution_pin_short12: abbf15de6fbb
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
self_check_scope: 本轮恢复不新增正式源码窗，0/0/0；原158设计READY与157作者/主线P6分别保留，不计作新席已完成。
mainline_design_count:
  conversation: C11
  first_batch: 152
  latest_batch: 158
  distinct_batches: 7
  before_reassignment: 7
  after_reassignment: 7
  amendment_increments_count: false
  thresholds:
  - 8
  - 10
  reached: false
evidence_class: Owner恢复报告 / 远端观察 / 已有设计继承 / UNKNOWN
status: PREPARED_FOR_PUBLICATION_G04_PENDING
D00: []
R155_01: CLOSED_BY_156_CHECKPOINT_READ_SUPPLEMENT (retained)
receiver_actual_state: NOT_OBSERVED
old_executor_unpublished_work: UNKNOWN
record_type: phase2_executor_contract
followup_id: CORE-USER-LIBC-FORMAT-CAPACITY-158
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-capacity-158/
allowed_files:
- MANIFEST.md
- libc-format-capacity.md
- facts-and-boundaries.yaml
- libc-format-capacity-paths.dot
- deferred-findings.yaml
- evidence.md
execution_parent_short12: abbf15de6fbb
model_scenarios: 4
independent_input_rows: 12
open_questions:
- 实际ABI/构型、sh链接、getcwd生产者、写入效果和后台档位未知；只作限定源级容量消费。
executor_display_name: MYOS2-A-C11c 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-27
executor_model_per_owner: GPT-6.2 Sol＋Extra／高推理；接收执行无需Pro
executor_start_state: NOT_OBSERVED_FOR_158
replaces_executor_for_this_batch: MYOS2-A-C11b / MYOS2-LEAD-002-CHAT-EXEC-26
technical_body_preserved_from_short12: a74bf1dc357b
technical_body_unchanged_sections: 1–8
recovery_package_id: MYOS2-A-C11b-TO-C11c-158-20261127
---

# 220号合同 · 第158批 · 用户库格式容量、指针与长度消费

**改派新「MYOS2-A-C11c 代码分析执行」／MYOS2-LEAD-002-CHAT-EXEC-27，GPT-6.2 Sol＋Extra／高推理；接收执行无需Pro。** Owner报告C11b上下文耗尽、158失败；当前远端158尚未交付。依[221恢复包](221-executor-continuation/00-C11b-to-C11c-recovery-package.md)与[完整启动词](221-executor-continuation/01-MYOS2-A-C11c-receiver-startup-prompt.md)，新席独立接收、完整规范加载及fresh G00后，连续完成三相关专题、四场景十二独立输入行、恰六新增、自检、唯一作者回执与实际G04，通过即STOP_WRITING，回C11 Pro独审及同轮后继，不另等开工、不进入159。

[157主线独审](reviews/CORE-USER-SIGNAL-CONTROL-CONSUMPTION-157-review.md)的PASS_PENDING_LOCAL、D00=[]、R155/R152既有闭合及所有历史未证保持。157作者仍EXEC-26，[7206572285](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7206572285)及[abbf15de6fbb六件](https://github.com/08822407d/MyOS2/tree/abbf15de6fbb/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-signal-control-consumption-157)不改署、不重做。旧C11b的158执行指派由本次替换，其未公开工作UNKNOWN，不要求旧方补签。

本次仅修改身份、接收与发表治理；以下§1–8完整保留[原220固定版](https://github.com/08822407d/MyOS2/blob/a74bf1dc357b/agent-workspace/lead/MYOS2-LEAD-002/220-user-libc-format-capacity-contract.md)原字节，共177 LF／24081字节。段内原设计检索的2070项树、e9f4a788b1aa及“本轮”属于158首次设计来源，不代签本次或C11c已经读取；新席须自己回源、建模与逐窗自检。原191/100 READY仅作设计历史，未运行的158P6、执行与G04不得填完成。

固定源码time=a039d9803ade；执行唯一父abbf15de6fbb；最终lead以本轮真实改派评论为准。批号仍158，C11计数保持7。十二行各自重置、实际数组与名义容量分列、首次无法证明合法的操作前停止。

## 1. 目标、选择与排重

本批研究仓内用户库的 `vsnprintf/format_int/printf`，回答三个相邻问题：**输出预算怎样限制实际存储；数字分支是否在存储门之前已经越过真实对象；格式化返回值怎样成为4096字节局部缓冲的写请求长度。** 采用四个必要场景、十二个独立输入行；每行从自己的入口重置，不串成真实程序。数量只描述下面的必要对照，不是新增场景、引文或缺陷配额。

优先消费当前程序中实际存在的调用：`myshell.c::main` 的 `printf("MyOS2:%s # _", current_dir)`。实际shell对哪个printf实现的链接仍未认证，故仅在明确选择仓内用户库实现的条件下组合这条模板，不能据此说真实sh已经发生截断或越界读取。

### 1.1 本轮三个候选及取舍

在完整lead树2070项的合同/reviews路径中检索 `fd-control|writev|signal|printf|printk|stdio|datetime|diagnostics|string-length-copy` 后，定点读实际旧合同与原件：

| 候选 | 已实际读取的旧对象/selector | 取舍 |
|---|---|---|
| fd复制/向量写后续消费 | 61原正文，91号合同§3–5；92号writev合同§3–5。61已经要求初计数1/多持有、同file两槽与close交接；62已要求有界正/零/负回调序列、位置提交和FAT消费者 | 不再以相似对象数值重做，且不把155/156换函数名重交 |
| 信号接收队列组合 | 09 `user-return-signal.md` D02-b L296–305、L332–349；10 `signal-send-wake.md` §4 L197–233。已有私有/共享优先、同步优先、同号多节点保位、无节点合成info及RS-14“第一次摘节点留位、第二次合成info”的具体组合 | 不把已有组合改一组队列数字作为158；不扩到帧/ABI来制造增量 |
| 用户库格式容量与消费者 | 117/118短串输出；129的41字节混合格式；130的五条充足容量错误行；94实际为另一套内核格式器和日志消费者 | 选择本项。新增域是容量0/1/恰满、数字分支真实指针停止，以及合法cwd对象经现有提示符扩张后的请求区间 |

这只是所列定点范围，不声明全历史逐字读完或全仓绝无同类事实。

### 1.2 强制旧消费范围

旧成果固定取执行 `abbf15de6fbb`；旧主线材料固定取lead `e9f4a788b1aa`。每份以实际path/blob核身份，不用标题替代原条目。

| 旧原件与selector | 已有结论及158使用方式 |
|---|---|
| `user-syscall-stdio-08/user-syscall-stdio.md` §2、§8及GAP-U10 | 当前构建文本和仓内库/仓外libc之间的边界。158不认证sh的真实库绑定，不读仓库外libc，不从源码符号名推已执行系统调用 |
| `user-shell-cat-file-display-117/shell-cat-file-display.md` §C；`user-shell-cwd-display-118/shell-cwd-display.md` §A/§C | 已有%s、首NUL、EMIT计数和printf忽略write返回的短串说明。118已有固定current_dir[4096]及提示符模板。158只新增容量边界与长的给定输入，不重做cat文件位置/文件读取或cwd生产 |
| `user-shell-datetime-format-129/shell-datetime-format.md` §B/§C | 已有%06ld参数解析、零填充、逆向数字写及41字节整行。不能把这些语句当新发现。158 B01新增截断时format_int局部q与外层q的差异 |
| `user-shell-error-diagnostics-130/shell-error-diagnostics.md` D00.R129-01、正文格式化/写请求段；129/130对应主审及 `CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-D00-127-129-review.md` §R129-01 | R129-01已文档闭合；时间顺序与按目的地址排序的最终字节表必须分开。130五条样例均给定缓冲充足；158不复用其旧误归属图，不重开旧裁定 |
| `user-libc-string-length-copy-124/libc-string-length-copy.md` §A L66–81及其主审“从输入字节重算／C08的两项使用补记” | strlen整字读取需充分对象、长度不含NUL、返回长度不等于访存宽度。158只消费给定源对象的长度，保留对齐/读取/别名语义条件，不重新展开strlen算法族 |
| `user-printk-format-buffer-94/printk-format-buffer.md` §B L53–87；130/131/132号内核printf合同的主源范围 | 94是mykernel的1024字节日志缓冲、vscnprintf夹取、环形扫描以及50字节sprintf对象门。158主源是myinitramfs的另一实现；仍沿用“逻辑长度不能替代对象合法性”的证据纪律，不重做内核日志 |
| 155/156的文件位置消费、157的信号设置/flush | 本批不复用其位置末态或把157条件末态当真实进程现场；不产生三者的修复/运行结论 |

同一根因若已有qualified ID，仅追加新消费者/新范围；确需新ID必须说明不同的不变量。没有新缺陷配额，旧问题不自动关闭。

另已定点核117/118/129/130的原deferred：117 `CT117-CAND-07` 明示“no formatting truncation”，118 `CW118-CAND-08` 只到缓存显示/ABI边界；129 `DT129-VSNPRINTF-BOUNDARY` 明示4096缓冲充足并把edge cases留给后继。`CORE-USER-SHELL-DATETIME-FORMAT-129::DT129-STDIO-LINK`、`::DT129-PRINTF-WRITE` 与130 `actual_sh_libc_linkage`、`actual_stdout_and_runtime` 均仍保留真实绑定/输出边界。158的容量模型应作为 `CORE-USER-SHELL-DATETIME-FORMAT-129::DT129-VSNPRINTF-BOUNDARY` 的限定证据追加，不能宣布其“effective ABI”或整个未知项已关闭，也不重报既有“忽略write返回”。


## 2. 强制源码门与构建边界

固定源码为time `a039d9803ade`。全部正式证据必须来自该固定对象；下面的blob短12用于身份核验，不是另一份版本。表内用户库的后续简写路径均相对 `myinitramfs/mylib/myglibc/`；其它路径完整列出。先读完整被分析定义，再取连续1–5物理行短窗，CRLF与制表符原样保留。

| 必读源/实际selector | blob短12 | 作用 |
|---|---|---|
| [myinitramfs/mylib/myglibc/stdio/vsnprintf.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c)：flags、ranks、EMIT、完整format_int、完整vsnprintf | `ef7e61a278cb` | 三专题核心；不得套用内核vsnprintf |
| [myinitramfs/mylib/myglibc/stdio/printf.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/printf.c)：SZ_4K和完整printf | `e8c0430ad375` | 实际局部数组、rv、write和return |
| [myinitramfs/mylib/myglibc/string/strlen.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strlen.c)：STRLEN默认路由及完整定义 | `7416e487fb71` | %s源读取先于输出预算；只核本模型的读取条件 |
| [myinitramfs/myshell/myshell.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c)：PATH_MAX/current_dir、main L66–94、cd刷新段 | `5e177066dc16` | 真实4096字节对象与提示符格式；实际生产者未证 |
| [myinitramfs/myshell/tests.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/tests.c)：test_PrintDateTime | `71e7681d050b` | 区分旧129的实际模板，不给158冒接时间调用 |
| [mykernel/fs/vfs/d_path.c](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/d_path.c)：myos_prepend、getcwd本体及长度门 | `75aae754bb56` | 只说明源码有len/size门，不认证任意长cwd可被实际生产 |
| [mykernel/include/uapi/linux/limits.h](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/linux/limits.h)／[mykernel/include/linux/kernel/limits.h](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/limits.h) | `bddfd09e4be2`／`6cf9b01e4068` | PATH_MAX定义与包含定位；不把未预处理的定位签成编译结果 |
| [myinitramfs/CMakeLists.txt](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/CMakeLists.txt)／[myinitramfs/myshell/CMakeLists.txt](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/CMakeLists.txt) | `6757d21bc64c`／`dcdac0564c0e` | 用户树-m64文本、外部musl路径和sh目标 |
| [myinitramfs/mylib/CMakeLists.txt](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/CMakeLists.txt)／[myinitramfs/mylib/myglibc/CMakeLists.txt](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/CMakeLists.txt)／[myinitramfs/mylib/myglibc/stdio/CMakeLists.txt](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/CMakeLists.txt) | `8f6abc24095a`／`b956957a602a`／`73ca342146a7` | 独立mylibc目标、include目录及stdio源收集；不等于当前sh实际链接 |
| [myinitramfs/mylib/myglibc/include/stdio.h](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/include/stdio.h)／`libio/stdio.h`／`bits/types/FILE.h`／`bits/types/struct_FILE.h`（后三者同myglibc前缀） | `76b24e00c502`／`396bfb0df778`／`74f62d695aa3`／`77633f6b639b` | stdio声明路由、stdout声明、FILE typedef及int _fileno |
| [myinitramfs/mylib/myglibc/include/unistd.h](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/include/unistd.h)／`posix/unistd.h` | `bdb6ffa91612`／`43f11162433e` | write活动声明的int/const void*/size_t、ssize_t返回 |
| [myinitramfs/mylib/myglibc/include/bits/types.h](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/include/bits/types.h)／`posix/bits/types.h`／`bits/typesizes.h`／`sysdeps/unix/sysv_linux/x86/bits/wordsize.h` | `1c479106072c`／`7ac0a3f83b2a`／`d72a1e8c7b9e`／`94ff7c423930` | 所选字宽与__ssize_t定义条件；不得把外部编译器类型认证补成SOURCE_FACT |
| [myinitramfs/mylib/myglibc/sys/types.h](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sys/types.h)／`posix/sys/types.h`／`string/string.h` | `716732f4d413`／`bfdcead553a9`／`23305a96a57a` | ssize_t和strlen声明/包含定位 |
| [mykernel/scripts/options_flags.cmake](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake) | `6019436417ad` | 项目规定配置读取与金丝雀；不是用户库实际编译参数的替代证据 |

`libio/stdio.h`、`posix/unistd.h`等仍包含 `<stddef.h>/<stdarg.h>`。本轮所选用户库目录没有给出这两个编译器头的完整实现；size_t具体定义、va_list宏展开、include实际选择及二进制ABI均不能认证。下面直接声明有限模型的类型和取参前提；若执行方只能证明较弱范围，应明确降为GIVEN/UNKNOWN，不签“可编译／真实可达”。

顶层initramfs配置写外部musl路径并加入myinit/myshell，另一棵mylib配置声明mylibc。源码中同名printf的存在及相同函数原型，不足以证明sh选中了它。图上从真实main的printf符号调用接到本仓内printf实现的桥必须标为 `CONDITIONAL_MODEL_BINDING`，不能伪装已经核实的链接边。

## 3. 共同GIVEN、对象与停止规则

1. 选ASCII、8位字节、32位int/unsigned int、64位long/unsigned long/size_t/ssize_t/指针。所有本批正常返回计数最多4105，处于所选int可表示域；不借无穷精度整数掩盖C溢出。用户树-m64及wordsize条件只支持这一选择的来源，不认证实际构建。
2. 每行独立重置；fmt是完整、有效、NUL终止的数组。源/目的互不别名、在整个被推导前缀存活，没有并发或外部修改。未写字节统一初始为0x7e，记作“~”；所谓保持字节仅指物理对象内的字节。
3. va_list有效建立，按源中实际va_arg类型依次提供所列值；printf内va_start/va_end正常。它们是GIVEN，不是从未取得的编译器宏推导出的ABI事实。B01给signed long 7，B02给unsigned int 12345；不用类型不匹配的变参制造样例。
4. 物理对象容量与参数n必须分栏。每一次q/qq更新、指针差和解引用分别核同一真实对象范围；允许合法one-past指针存在，不允许解引用它。越过one-past的第一次运算即停止，不能因为后面有存储门继续给出返回或最终字节。
5. A和C中的字符串源按8字节对齐，所有实际整字读取及候选逐字节确认均在给定对象内，所选实现的整字访问语义成立。明确GIVEN strlen正常返回所列首NUL下标，另以实际源码和对象范围核前提自洽；不认证通用C可移植性、有效类型规则或真实编译。无NUL、实际指针无效、读取/取参更早失败的路径不进入后续容量表。
6. 无浮点、%p、%n、动态宽度/精度、超大宽度或负极值模型；它们不为本批目标提供必要增量。有关分支源码可定位，但不冒称覆盖。
7. C中stdout指向有效FILE，_fileno是已初始化int F，不默认F=1。只推到所选printf发起的write指针/长度请求；不进入write实现、系统调用、VFS、页映射或设备。若说明printf的末尾return，须额外写明“该write调用正常返回时”；write不返回/更早失败不跨越。
8. 原始调用方参数、格式器消费顺序、局部写入时间顺序、最终字节布局及返回来源分别记录。普通printf返回源为vsnprintf赋给rv的值，不是write返回值；该旧事实只作必要连接，不算新根因。
9. 所有末态均为源码与GIVEN下的条件推导。真实目录树、getcwd成功生产、当前sh链接、屏显、真实用户/内核读取、资源寿命或文件位置均UNKNOWN；完整本地P9及所有编译/运行保持NOT_RUN。

## 4. A：普通字面与%s，容量门到底限制什么

### A01／STRING_CAPACITY_AND_NUL

入口 `vsnprintf(D,n,"A%sZ",ap)`。D为实际8字节数组，入口全部0x7e。S为8字节对齐数组，完整字节为 `62 63 00 7e 7e 7e 7e 7e`；取出的const char*指向S。n分别为0、1、4、5，四行独立重置。

必须按源顺序追：初始q/o；普通A；%s取参和strlen；两个源字符的读取；每次EMIT的o<n；普通Z；收尾条件；size_t o到int返回。n=0仍需读格式、取参和strlen；输出预算为0并不免除源对象要求。

| 行 | n | 收尾前o／q相对D偏移 | EMIT真正写入的字节 | 收尾NUL写入 | 正常返回与最终D |
|---|---:|---|---|---|---|
| A01_N0 | 0 | 4／0 | 无 | 无 | 4；八个~均保留 |
| A01_N1 | 1 | 4／1 | D[0]='A' | D[0]=0，覆盖A | 4；`00 7e 7e 7e 7e 7e 7e 7e` |
| A01_N4 | 4 | 4／4 | D[0..3]="AbcZ" | D[3]=0，覆盖Z | 4；`41 62 63 00 7e 7e 7e 7e` |
| A01_N5 | 5 | 4／4 | D[0..3]="AbcZ" | D[4]=0 | 4；`41 62 63 5a 00 7e 7e 7e` |

这些预期必须由执行者独立回源重算。真实存储次数、不同目标字节数、最终非NUL字符数及return=4不强加相等。D是8字节而不是“n字节对象”；本例没有进入数字writer，不据A的饱和q行为概括所有格式分支。

## 5. B：数字分支的两个游标与真正的对象门

### B01／INTEGER_PADDING_AND_TRUNCATED_CURSOR

入口 `vsnprintf(D,n,"%06ld!",ap)`。D实际16字节，全部0x7e；取参为signed long 7。n=4、7、8三行独立重置。

完整解析该格式，给出FL_ZERO、rank_long、base、width、prec以及到format_int的实参。不能仅复述旧129六位结果：本批必须分别跟踪format_int自己的q/o/qq/oo和外层vsnprintf的q/o。format_int返回的是size_t长度，不返回它自己的q；外层执行自己的 `q += sz`。

本例format_int逻辑长度为6，初始真正数字数为1，五次零填充按增加目标地址请求；随后数字按实际逆向段处理。精确预期如下：

| n | 零填充真正写偏移 | 数字7是否写／偏移 | format_int末尾局部q偏移／返回sz | 外层进入!前q偏移 | !写入与收尾NUL | 正常vsnprintf返回／最终前缀 |
|---:|---|---|---|---:|---|---|
| 4 | 0、1、2、3；第5次只增逻辑o | 不写 | 5／6 | 6 | !不写；D[3]=0 | 7／`000\0`，D[4..15]保持~ |
| 7 | 0、1、2、3、4 | 写D[5]='7' | 6／6 | 6 | 先D[6]='!'，再D[6]=0 | 7／`000007\0` |
| 8 | 0、1、2、3、4 | 写D[5]='7' | 6／6 | 6 | D[6]='!'；D[7]=0 | 7／`000007!\0` |

n=4的format_int局部q=5、外层q=6是两个不同调用帧的游标，不能合并成一个“已写字节位置”。其所有指针仍位于D[16]内；这正是合法截断模型能够继续的额外物理对象前提。

本例只覆盖所列非负小整数。它不证明任意数字需求长度都可在n字节物理对象中安全完成。

### B02／SAME_N_DIFFERENT_OBJECT_EXTENT

入口 `vsnprintf(D,4,"%u",ap)`；取参为unsigned int 12345。两个独立行只改变D的真实物理容量：B02_NARROW为4，B02_WIDE为8；入口字节均0x7e。

要求查到第一次对象范围门，不以“有if(oo<n)”替代此前指针检查：

- **B02_NARROW：** 数字计数为5，未发生符号/前缀/填充写。format_int的 `q += ndigits`（固定源码L167）试图从D+0形成D+5，而D[4]的one-past只有D+4。这是第一停止点；不能继续认证紧随的o更新、逆向写、return、外层q、最终NUL或任何write请求。可记录停止前没有目的写，不提供停止后的“最终D”。
- **B02_WIDE：** 相同n=4而D[8]，D+5在对象内。逆向第一轮偏移4因oo<n为假而不存储；随后真正数字存储依次为D[3]='4'、D[2]='3'、D[1]='2'、D[0]='1'。format_int返回5，外层q=D+5、o=5，收尾D[3]=0，最终 `31 32 33 00 7e 7e 7e 7e`，局部return5。

整数表达式的“候选偏移5”可用有限算术给出，不能把非法C指针的后续行为当数学数组下标继续执行。B02是单独helper条件模型，没有证明当前shell曾发起这个小缓冲请求。

## 6. C：现有提示符模板怎样扩大请求长度

### C01／PROMPT_CAPACITY_TO_WRITE_REQUEST

实际模板来自 `myshell.c::main` 的单次 `printf("MyOS2:%s # _", current_dir)`。仅建这一次调用；main此前独立的 `printf("\n")` 和后续fflush不并入本次字节或调用数。

每行current_dir是独立、8字节对齐的4096字节物理对象。给定首字节'/'，其后直至首NUL之前均为'x'；首NUL之后对象内其余字节仍为0x7e。三个首NUL下标L=4085、4086、4095，全部满足L≤4095；不存在4096字符再加NUL的输入对象。条件地将这个模板交给所选仓内printf，后者建立独立buf[4096]。

这只是格式化入口的GIVEN，不是宣称某个目录树或getcwd实际产生了这些串。必须同时读：

- shell实际PATH_MAX=4096、current_dir数组、main传给getcwd的size和提示符调用；
- d_path.c::getcwd中先为结束NUL预留1、算len、len>PATH_MAX、len>size及copy_to_user的源顺序。长度检查的存在不证明任意模型输入可由当前文件系统产生；也不能反向从getcwd的返回值推出缓冲内容；
- strlen实际完整字读取在给定对齐4096字节对象内：最长行首NUL在4095，包含它的末次完整字范围为[4088,4096)，没有读取对象外字节的GIVEN缺口。

模板前缀 `MyOS2:` 为6字节，后缀 ` # _` 为4字节，逻辑输出长度为L+10。这里的4096/4105是**输出需求或请求长度**，不是current_dir输入长度。只有普通字面/%s，所有输出EMIT饱和后不再增加q，须实际核q最多到buf+4096；不能把B02中的数字游标规则套来提前截断本例。

| 输入L | 逻辑o、vsnprintf正常返回rv | 收尾前q偏移 | buf最后保留内容与NUL | 传给write的长度与对象关系 |
|---:|---:|---:|---|---|
| 4085 | 4095 | 4095 | 完整提示符4095字节，buf[4095]=0 | 请求4095，全部位于buf中，不含末NUL |
| 4086 | 4096 | 4096 | 最后'_'先存buf[4095]，后被NUL覆盖；最终非NUL文本4095字节 | 请求4096，恰覆盖buf全对象，包含最后NUL；“请求未超对象”不等于保留完整提示符 |
| 4095 | 4105 | 4096 | 先存前缀6与cwd前4090字节，最后一字节被NUL覆盖；最终前缀6与cwd前4089字节、buf[4095]=0；4字节后缀均未保存 | 请求4105；以整数偏移表示的请求[0,4105)比4096字节对象多9字节，即4096..4104；不得实际构造或解引用buf+4105 |

L=4095行仍然读取整个给定源串，输出空间用尽只抑制目的存储；逻辑o继续累计。这不证明源码在格式化阶段已写出buf边界。真正不同的消费门是printf把rv直接传给write而未按buf容量夹取。只认证请求的指针、长度、已知对象区间和已存字节；**不认证那9个对象外字节被读取、被输出、属于什么内存，或已经形成信息泄漏。**

若write调用正常返回，无论其具体返回值如何，所读printf返回其原rv；本批不额外发明全写/短写/失败三行，因为该返回独立性已由129/130建立。若write不返回，停在调用，不能签printf末尾return。实际系统调用、内核检查、显示及错误处理均在本批边界之外。

## 7. 四场景共用的机器记录与图

必要输入行是A01四行、B01三行、B02两行、C01三行。每一行至少能从下面的信息独立重算：入口函数、实际源对象及容量、n、fmt、取参类型和值、各局部游标归属、按发生顺序的读写/跳过事件、每个门的真假、最早停止或局部return、最终已知字节、C中的条件写请求及范围。无需把长字符串逐字抄4095次，可以用无损区间和重复计数表示。

必须把 `event_order` 与 `final_bytes` 分开：例如B01_N7先写!再写NUL，B02_WIDE的数字真正写入顺序3→2→1→0，不能按最终0→3地址表冒充执行顺序。同一行同时保留format_int局部和vsnprintf局部字段，严禁覆盖同名q/o。

图至少区分 `SOURCE_CALL`、`CONDITIONAL_MODEL_BINDING`、`LOCAL_CONTROL`、`WRITE_EVENT`、`RETURN_VALUE`、`REQUEST_ARGUMENT`、`ANALYSIS_STOP`。调用拥有者正确：vsnprintf调用format_int与strlen；printf调用vsnprintf和write；main调用外部printf符号。两个被调函数之间的先后不是直接调用，write节点不是printf返回rv的来源。

B02_NARROW停止节点没有通往return/NUL/write的正常边。C01的对象外“请求区间”用整数元数据表示，不画成已经发生的字节读写。构建链接UNKNOWN是跨实现组合边界，不能藏在图注里再把桥画成SOURCE_CALL。

## 8. 恰六件与验收实质

唯一成果目录为 `user-libc-format-capacity-158/`，恰六件：

| 文件 | 必须包含的实质 |
|---|---|
| `MANIFEST.md` | 实际输入、构建/类型GIVEN、四场景覆盖与停止、规范自检及未运行范围；不借旧件计数充本批 |
| `libc-format-capacity.md` | 三专题结论、容量和真实对象的区别、旧消费限定、重要first-stop和当前程序边界 |
| `facts-and-boundaries.yaml` | 固定源账/旧selector、独立输入、局部对象、事件时间序、最终字节、return/request与最早停止；与正文一致 |
| `libc-format-capacity-paths.dot` | 调用归属、门、局部写入、条件绑定及停止边，和facts逐case对应 |
| `deferred-findings.yaml` | 仅独立新候选或已有qualified ID的证据追加；真实链接/生产者/运行保持UNKNOWN，没有缺陷配额 |
| `evidence.md` | 每一关键门的真实定义内短窗与普通定位；CRLF原样、字段/符号/配置归属正确、实际纯文本/整数核查方法 |

强制源码锚必须支持：EMIT守卫内q自增与守卫外o自增；%s取参/strlen/逐字节源读取；收尾两分支及return；B所选parser状态与实际va_arg类型；format_int数字数/零填充/`q += ndigits`/逆写门/return；外层q/o更新；printf真实buf大小、rv赋值、write请求及独立return；shell current_dir/调用模板；用户CMake构建边界；所依赖类型与声明条件。不能用只有签名或case标签、但缺真正更新/取参的短窗支撑强断言。

只允许源级静态、临时自写纯文本/结构核查和有限整数/字节模型。不得运行仓库脚本、旧夹具、预处理、C/ASM、编译链接、Graphviz、QEMU、实机或真实I/O；不读仓库外libc/编译器头。源体/类型/对象门缺失时写PARTIAL或UNKNOWN，不能以更长的假设链补签实际运行。



## 9. 正式入口、新席独立接收与写区

以本轮PR17唯一改派评论为入口，取实际ID及固定lead，完整读同提交本合同、[221恢复包](221-executor-continuation/00-C11b-to-C11c-recovery-package.md)、[完整启动词](221-executor-continuation/01-MYOS2-A-C11c-receiver-startup-prompt.md)、[当前检查点](checkpoints/2026-11-27-c11b-to-c11c-recovery-format158.md)和不变的[157独审](reviews/CORE-USER-SIGNAL-CONTROL-CONSUMPTION-157-review.md)。原158首次发令7207878199作为历史入口保留，不重发TASK_RELEASE；新方执行以本次改派为准。

新席MYOS2-A-C11c／EXEC-27，任务仍MYOS2-LEAD-002-CORE-CHECK-01、阶段2、原claude/dazzling-cori-q0dnyt及Draft PR17，唯一写区为user-libc-format-capacity-158六件。授权来自Owner恢复请求、00工作令§12第20条及本合同；恢复包是non_execution_source_transfer_artifact，不导入外仓业务或写权。不重演建分支/阶段0等待，不操作主线、PR元数据、旧件或源码。

按221§3–4独立全文读十五项目规则（00取本次全文含第20条；其余十四件固定a74bf1dc357b且本次未改）、Meta-Agent现行入口及48采用→47有效全文、45同版共用01–14/16十五条全文、现行模型登记与建设检查点的现状/后续适用采用段，以及Mnemosyne任务局部README、接收命令、批准规范及命名§7。新C11c不能按79继承C11b或主线的已读；完整取得与实际阅读全文分开记录，输出截断续读到尾。

在MANIFEST分列execution_handoff_receive与guidance_refresh，记录包ID=MYOS2-A-C11b-TO-C11c-158-20261127、非执行源性质、真实固定入口/来源/授权、接收日期时刻或UNKNOWN、新名称/EXEC-27、任务/阶段/原分支/六件写区、实际已读与缺项、当前胜任及容量判断、权限边界、下一安全动作和未知；刷新块记录项目十五份、MA有效全文及同版十五条的实际版本/范围、适用与冲突、task_local_only/current_task_preserved，真实读完才refresh_completed=true。新席接收、刷新、启动待自己完成；不得把前任旧记录复制成自己的完成记录。

本轮MA master b234c6c91a2e、建设06b783861368，入口881fd8c1076e/检查点54ac6e5fc66c；Mne master e581eb9df6c4。新席到达仍fresh核，实际适用差异真实读取；§322待议不是新采用。命名§7实际L445–584共140物理行，全文件584LF；旧585是尾空段，不构成新D00。R155既有闭合只作历史裁定，不继承C11b完整读记录，也不重开155历史。

保留18问题先入库、24职责、28同轮后继、63先核远端效果再限次重试、124次一档执行/Pro验收、198仓库正文/短入口和210的8/10提醒。后台/effort未知保持UNATTESTED，当前容量判断不是未来保证；无真实阻断就连续执行到终点，不重复索取开工许可。

## 10. fresh G00与冻结基线

1. 直接lead等于本次改派评论真实发布提交；exec=abbf15de6fbb、time=a039d9803ade、直接master=e8e60cd19540，不用PR.base_sha替代。原PR16 Open/Ready，PR17原标题Open/Draft，原head/base master、未合并；开放PR完整到空。
2. PR17评论无since全分页到空；本次准备310条，改派后预计311条。检查点70既有精确标记加本轮EXECUTOR_REASSIGNED、OWNER_DIRECTED_RECOVERY_PREPARED两枚共72，每枚字面一次且实际ID正确。当前改派入口、原158首次发令7207878199和157作者7206572285各直接ID双读并与分页正文/时间一致；不得只比总数。
3. 检查点完整十六错误作者负标记均0，包含旧EXEC-26/158、新EXEC-27/157；正确EXEC-26/155/156/157及EXEC-25/152/153/154各1，新EXEC-27/158入场0。任意作者158正式回执、整个结果目录及未知并行变更均检查；发生真实占用先保全定位，不覆盖、不改署。
4. 157固定abbf15de6fbb、活动exec、API Git Blob六件全文一致，22588 LF／746032字节；唯一父2d134f987c21，一提交六新增零其它改删，原2265非tree保持。158写前完整exec2667项／2271非tree冻结。只核冻结、主线裁定和补链/说明，不重算157的232事件、205窗或重新判闭R155。
5. 两time canary、直接master反验、公约原句实核，保留read_channel与self_check。MANIFEST明确D00=[]、R155既有主线闭合入口、历史保持、真实接收/权限/独立全文加载/当前容量/下一步；实际完成才登记。G00通过后连续A/B/C。

## 11. 六件、自检、唯一发表与实际G04

恰六件：MANIFEST.md、libc-format-capacity.md、facts-and-boundaries.yaml、libc-format-capacity-paths.dot、deferred-findings.yaml、evidence.md。接收、规范、D00空集在MANIFEST，必要旧项索引在deferred；不加第七件或接收评论。保留read_channel、startup_selfcheck_quote、branch_canary_quotes和self_check三整数。允许共享基态/有序差量，但所有关键对象容量、q/o所属帧、指针运算、实际写序、NUL覆盖、长度、首次停止必须可重建。

P2/P6正式证据每窗1–5连续物理行，按固定源的原字节、LF/CRLF/NONE和真实单一定义/命令实例重开核。vsnprintf原CRLF不得归一化后冒签。P6 n=m+k，普通位置另账；157作者205、157主线补窗3、原158设计READY 191/100与任何未公开旧草稿都不计入新席158P6。连接器可达P9第1–8、11、13实做，9/10无代码变更、12非降级可NA；完整本地P9未做即NOT_RUN。不以降级逃避关键体/定义，不设字数/窗数/新问题配额。

写前fresh四refs、原PR状态、作者/目录占用。仅以真实abbf15de6fbb为唯一父，expected_sha匹配、force=false，原exec一次提交六新增零其它改删。先回读六件固定/活动/API Git Blob全文、LF/字节/行尾、唯一父、compare及全部2271旧非tree冻结，再发表一条唯一作者评论；实际发生局部成功则按63核效果后续缺项，不重复写。

唯一作者标记：

MYOS2-LEAD-002-CHAT-EXEC-27 / CORE-USER-LIBC-FORMAT-CAPACITY-158 / FORMAL_DELIVERY

作者回执列真实新席接收/刷新、固定修订220及221、提交/六件、D00=[]和R155保持、四场景十二行/首次停止、Owner指定/可见身份/未知后台档位、实际P6与可达自检。发表时actual_G04=PENDING_POST_PUBLICATION、mainline_acceptance=PENDING，不预签终检或主线验收。

发表后新ID实际直接读两次，与全分页同文；72原标记加新作者共73，每枚字面一次/ID正确，十六负标记0，评论预计312条且仍读到实际空页。四refs（exec为真实158提交）、原两PR/开放页、六件三渠道、唯一父/一提交六新增、全部旧对象及改派后311条旧评论冻结均实核。通过才READY_FOR_MAINLINE_REVIEW、实际G04=PASS、STOP_WRITING，不进入159。

63先核远端效果，通常首次加三次10/20/40秒或实质工作间隔限次重试；参数校验未发出、调用上限恢复、写重试及读回暂态分账，不覆盖未知并行写，不为补正回改旧评论/成果。最终按198只给一个通常3–6行原生短复制块、一个真实作者评论URL，报158实际交付/自检/G04/停写与必要未知，请C11 Pro独审并同轮后继；完整依据和字段留GitHub。

## 12. 原设计READY、同批改派计数与限制

原158首次发布时由设计分工提出，另一技术分工先读27份固定源与GIVEN-only输入并封存模型，再读取合同比较，191项源码/模型检查和100项合同比较分别0失败。该记录固定在原a74bf1dc357b合同与157独审；本轮只验证§1–8逐字冻结，不重跑、不新增技术READY，不把旧结果代签C11c的回源、P6或实际G04。

名义n与真实数组大小分开；B02 D[4]在q加5之前首次停止，后续o更新、逆写、返回及最终NUL均不能继续认证。C01只到write指针/整数长度请求；4105比对象4096多9仅是请求范围，实际读取/输出/泄漏未证；只有write正常返回才继续printf局部return。旧129 qualified容量项仅作限定证据追加，不关闭effective ABI、真实绑定或输出未知。

C11首次正式设计152–158共7批，本次158换席恢复计7→7，不新发159、不重置；8/10未达，C10历史11保留。未来交接带十五项目规则、有效MA全文及同版共用全文、Mne命名/接收依据、实际断点/身份/计数及口述约定。C11c实际接收尚未观察，登记名称不等于操作聊天UI。

阶段2，仅允许自写有限文本/整数/对象核验；无项目脚本/程序、预处理、编译链接、C/ASM执行、Graphviz、内核/QEMU/实机/真实IO。完整本地P9 NOT_RUN，实际配置/ABI、sh链接归属、输入生产者、系统调用/内核效果及后台模型/Extra未认证。无源码/旧件/主线/PR元数据修改、新分支/PR、合并、强推、删除或跨仓写权。
