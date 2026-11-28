---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: phase2_executor_contract
followup_id: CORE-USER-BRK-ALLOCATOR-144
conversation_display_name: "MYOS2-A-C10 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前系统标识；实际后台与effort未独立认证）"
model_per_owner: "Owner本轮明确要求Pro独审及同轮后继设计"
date: 2026-11-24
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: c09ad0b19c2e
execution_pin_short12: 0e70d0a148b7
read_channel: connector
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
status: PUBLISHED_PENDING_EXECUTOR_START
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
self_check_scope: "本记录不新增正式源码引文标记；被审143的91窗独立计量"
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-22
executor_display_name: "MYOS2-A-C10a 代码分析执行"
executor_model_per_owner: "次一档GPT-6.2 Sol；沿用Owner界面可用Extra/高推理强度"
actual_batch144_receiver_state: NOT_OBSERVED
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-brk-allocator-144/
allowed_files: [MANIFEST.md, brk-allocator.md, facts-and-boundaries.yaml, brk-allocator-paths.dot, deferred-findings.yaml, evidence.md]
D00: []
mainline_design_count: {conversation: C10, before_release: 3, after_successful_formal_release: 4, first_batch: 141, latest_batch: 144, threshold_12: NOT_REACHED, threshold_15: NOT_REACHED}
inputs_read:
  - "143固定六件与同次独审、199/198；原项目规则及Owner当前回件"
  - "旧14 R14-S01、26 R26-S02、75内核分配范围，121构建/有效对象前提和122对121的文档判闭"
  - "固定time的内核brk、页/对齐/字段、系统调用生成/分发、仓内候选包装与malloc/list及构建文本"
open_questions: ["144执行未观察；当前sh真实链接、C指针/对象有效性、VMA/页与实际堆操作继续未证"]
---

# 200号合同 · 第144批 · brk返回值与仓内用户分配器消费

发给现有「MYOS2-A-C10a 代码分析执行」／EXEC-22，沿用次一档GPT-6.2 Sol及Owner可用Extra/高推理强度。本批为CORE-USER-BRK-ALLOCATOR-144。Owner转发主线简短正式发令，即执行本合同G00、A/B/C、自检、正式交付、实际G04和停写；之后交C10 Pro独审与同轮后继。

[143独审](reviews/CORE-USER-EXEC-ARGUMENT-LIMITS-143-review.md)为PASS_PENDING_LOCAL，无新RETURN。R143-N01已由主线补既有finding来源导航，不要求本批重交旧材料；D00为空。142、141合并消费及140状态、R141-01与五项更早文档关闭保持。

**双向聊天继续按[198号](198-repository-first-compact-relay.md)：完整正文、证据和正式作者回执放GitHub，最终只给一个简短取件通知。** 不复制整份合同、六件统计或检查日志到聊天。

## G00 · 同一执行身份续接

1. 取得同次固定发布的200、143独审及[检查点](checkpoints/2026-11-24-argument-limits143-reviewed-brk-allocator144.md)，按Owner短通知中的真实评论ID双读正式发令；本次两个主线标记为143 INDEPENDENT_REVIEW及144 TASK_RELEASE。不要猜评论ID。
2. 按79准确复用同一有效C10a自己已完整读取且版本未变的原十三份项目约束及198，缺失或失效者补读，不继承其他身份的“已读”。[196原十三份清单](https://github.com/08822407d/MyOS2/blob/5ab41ade828b/agent-workspace/lead/MYOS2-LEAD-002/196-executor-continuation/00-C09a-to-C10a-recovery-package.md)和[194来源导航](194-owner-continuity-and-copyable-return.md)保留；今后交接必须加198。Meta-Agent有效采用/同版本共用全文及Mnemosyne接收命名核当前入口，有生效变化再刷新，未变则如实记复用，不导入跨项目写权。
3. 直接读四refs：lead应为本合同同次正式发布提交，exec=0e70d0a148b7、time=a039d9803ade、master=e8e60cd19540。PR16 Open/Ready；PR17原题Open/Draft，原head/base及未合并状态保持。直接master不能用PR base_sha替代。
4. 开放PR及PR17顶层评论无since完整分页至空；核143独审所列十四个历史/作者标记，加本次两个主线标记各唯一。143作者为[7129263893](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7129263893)，真实ID双读。核其提交唯一父a911aa471305、ahead=1/behind=0、六新增零改删，固定/活动六件全文、LF、UTF-8字节、API Blob相同，5675 LF/219350字节。
5. 首次写前核本批目录及本身份144正式作者标记未存在、exec真实expected_sha。发现相符交付按63只续缺项；未知并行写先定位保全，不覆盖或强推。C10本次正式发布后累计4批，12/15未达到。
6. MANIFEST记实际读取/复用、模型可核信息、标准P3/P6、当前六件消费索引及G00。G00完成后连续做本包；普通源码问题先入库，不要求Owner逐项处理或另行开工。

## 共同模型、实质增量及未证边界

14已经纠正brk字段失败恢复，26已有匿名扩展帮助函数，75主要是内核SLUB；116/121把用户malloc结果作为前提，未闭合这里的仓内分配器生产过程。本包的新量是**内核brk返回值被候选用户包装、16轮初始化及取页函数怎样消费**，不是重做整个VMA/页表、malloc/free或bitmap。

先读固定time的内核options_flags及本包用户构建文件。显式选择myinitramfs/mylib/myglibc候选实现作为模型；顶层myinitramfs/CMakeLists.txt只列myinit/myshell并指向仓外musl，独立mylib子目录另有构建描述。这不证明当前sh实际使用该库；actual_linked_to_sh=UNKNOWN、actual_compilation=NOT_RUN。不读取仓外musl或借其实现填补本仓行为。

给定x86-64数值模型：int为32位有符号，ulong/long/size_t/指针为8字节，地址及算术可表示、对齐无回绕。**K=4096为所选内核PAGE_SIZE；H=0x200000为malloc.c本地PAGE_SIZE。** 同名宏属于不同源语境，不混成内核使用2MiB页。核活动PAGE_ALIGN→ALIGN→__ALIGN_KERNEL及K定义、用户CONST_2M/H定义。

用户brk_end是void *，式brk_end+H仅作合同给定的字节地址投影，不认证C指针运算、GNU扩展、对象范围或实际编译。所选用户声明/包装与内核局部返回之间只在“所选路径正常到达、指定数值经返回尾段传出”的条件下衔接；不证明真实寄存器绑定、CPU返回或系统调用运行。

所有元数据对象和列表节点有效、可写、互不混淆；初态列表计数与结构相符、无并发修改，所用列表操作及所需转换按给定模型正常完成。B/C的指针字段存值不证明该地址映射、可访问或有真实堆对象。指定helper返回只供上层片段消费，不反推内部完整执行或副作用为零。

## A · 内核brk返回接口及候选用户包装（6组）

独立重置入口：mm.start_brk=0x400000，mm.brk=origbrk=0x401234。逐例记录min门、对齐是否执行、old/newbrk、有序实参、调用前mm.brk、正常返回后字段和局部返回：

| ID | 请求brk | 给定被调返回 | 要核的分支与结果 |
|---|---|---|---|
| A_QUERY | 0 | 无 | 低于min直接out；对齐未执行，字段/局部返回均0x401234 |
| A_SAME | 0x401ff0 | 无 | old/new均0x402000；赋请求值并返回，两个映射helper未调用 |
| A_SHRINK_OK | 0x400ff0 | simple_do_vma_munmap返回0 | new=0x401000、old=0x402000；先改mm.brk，再调用(mm,new,old)，返回请求 |
| A_SHRINK_FAIL | 0x400ff0 | 同helper返回-ENOMEM | 调用前字段已是请求值；out恢复并返回origbrk，不外传该负errno |
| A_GROW_OK | 0x403001 | do_brk_flags返回0 | old=0x402000、new=0x404000，调用(0x402000,0x2000,0)；正常返回后才赋请求 |
| A_GROW_FAIL | 0x403001 | 同helper返回-ENOMEM | 调用前字段仍origbrk；out字段/局部返回仍0x401234 |

simple_do_vma_munmap的第三参是end，do_brk_flags的第二参是len。不要把两接口都记成(start,length)，也不能将未到达的对齐计算补成实际执行结果。A失败只复用14 R14-S01的字段恢复限定；不重报旧问题，不把字段恢复升级为VMA/引用事务回滚。26的simple_vma_link正常只返回0限定保持；负返回采用上表明确给定callee边界，不虚构来自它的非零错误。

另建立同一组输入的候选包装消费列，不再新增场景：

- 活动声明为include/unistd.h的void *brk(void *)；附近int声明是注释。
- syscall/syscall.c的SYSFUNC_DEF(brk)、三层生成宏及共用LABEL_SYSCALL/返回尾段分别取证；errno和-1转换为注释。保留局部标签/生成宏的实际归属，不把宏实例当作整个执行函数体。
- sysdeps/unix/sysv_linux/brk.c的__brk转换主体为注释，不能用其语义解释所选活动包装。
- 编号__NR_brk=12、表绑定、x64_sys_call活动switch及MYOS_SYSCALL_DEFINE1链只用于C绑定；不重做08的汇编/硬件返回链。包装中收到的给定原始返回数值保持，不能把非NULL旧break称为分配成功或转换成libc式0/-1。

## B · 完整16轮init_uslab（2组）

给初始内核mm.start_brk=mm.brk=B0=0x40000000，initiated=false；初始brk(0)按A查询返回B0。全局数组及本轮所需字段合法，初始化列表函数正常完成。两组都完整展开i=0…15，不把16轮重复计算成16个独立场景。

每行必须列：i、obj_size=32<<i、obj_nr=H/obj_size、调用前内核/用户break、请求、原始返回、调用后内核mm.brk/用户brk_end、base_upages[i]身份及vaddr、base_uslabs[i].virt_addr、相关列表计数和upage_alloc所选分支。16档obj_size从32到1048576，obj_nr从65536到2；不展开bitmap位算法或cm_size设计。

先从源码闭合这条局部顺序：每轮先把本轮base_upage放free尾，再调用upage_alloc；在给定合法列表模型中，该函数走非空分支，取同一节点并加used头。free计数每轮0→1→0，used由i→i+1；不会到达其malloc/brk动态分支。不要靠“初始化一般不会递归”的习惯代替证据。

### B_OK

16次扩展均给定do_brk_flags正常返回0，brk返回请求值。

- 第i轮请求/返回/两处保存值=B0+(i+1)H；首轮0x40200000，末轮0x42000000。
- 最终mm.brk=brk_end=0x42000000；请求跨度[B0,0x42000000)，数值长度32MiB。
- 每轮保存的upage.vaddr和slab.virt_addr等于**本次请求区间的排他上界**；最后值等于总请求跨度排他上界。这是整数地址关系，不证明已写越界、该地址未映射或堆对象实际存在。
- 最终uslabcache列表16、free_upages 0、used_upages 16，used节点顺序15…0；每档normal_free=1、normal_used/full=0、normal_slab_total=1。
- 所有给定调用和元数据操作正常完成之后，才记录initiated=true。

### B_REFUSE_I1

仅i=1（第二次扩展）给定do_brk_flags正常返回-ENOMEM，其余15次正常返回0。该次brk返回非零旧break=0x40200000。

- i=1请求0x40400000，内核mm.brk未增长；链式赋值使用户brk_end也保留0x40200000。i=0与i=1两个不同元数据节点保存同一vaddr。
- i=2重试请求0x40400000，后续继续增长；最终mm.brk=brk_end=0x41e00000，16个元数据节点只含15种保存地址。
- 列表计数仍到free 0、used 16；后续给定正常完成时initiated仍为true。
- “重复保存地址”只是限定数值/元数据记录，不等于真实堆对象已别名、页重复占用或已发生破坏；失败helper内部副作用不能据此记为零。

两组各有查询1次＋扩展16次，共17个给定brk调用；16次upage_alloc均不另调用brk。明确分别统计元数据节点数、不同地址数、成功请求数和请求跨度，避免把失败轮也算成一次真实扩展。

## C · free页复用与动态申请的不同消费（3组）

### C_REUSE

给free列表只有有效节点Q，used含两个其他有效节点；brk_end=mm.brk=U0=0x50000000，Q.vaddr=0x4fe00000。Q.uslab_p入口未指定。

upage_alloc返回同一Q，free 1→0、used 2→3，Q加入used头；malloc与brk均未调用，两个break和Q.vaddr保持。Q.uslab_p在该函数未读写，仍记入口未指定，不凭“free页”名称把它写成NULL。

### C_GROW_OK / C_GROW_REFUSED

为隔离递归分配未证链，观察入口定在**malloc(sizeof(uPage_s))已经按给定条件正常返回有效对象M之后、brk调用之前**：free计数0、used含两个其他有效节点，brk_end=mm.brk=U0=0x50000000，mm.start_brk<=U0，M尚未入列表。M.uslab_p入口未指定。不从此前malloc行为反推这些初态，不认证其真实可达性，也不展开递归malloc。

请求为U0+H=0x50200000；C_GROW_OK给定内核扩展helper正常返回0，C_GROW_REFUSED给定正常返回-ENOMEM：

| ID | brk原始返回/pg_vaddr | 非NULL门 | 用户brk_end | 内核mm.brk | M.vaddr |
|---|---|---|---|---|---|
| C_GROW_OK | 0x50200000 | 真 | 0x50200000 | 0x50200000 | 0x50200000 |
| C_GROW_REFUSED | 0x50000000 | 真 | 0x50200000 | 0x50000000 | 0x50000000 |

两组都在条件内初始化M链节点并加used尾，used 2→3，返回同一M；free(M)分支未到达。M.uslab_p在该函数没有赋值，仍为未指定；不能由malloc名称或对象有效性推成已清零。

拒绝例的实质差异是：旧break非NULL仍通过门，随后独立brk_end+=H使用户值与内核字段数值分离。成功例保存值也为本次请求区间排他上界。两者都不证明真实映射/复制/堆对象或实际内存破坏。不要新增一个与非零U0拒绝语义矛盾的“拒绝返回NULL”场景；也不扩入uslab_alloc/upage_free/upage_search/bitmap全链。

## 有界输入与强制证据

历史仅取指定范围：[14独审§3 R14-S01](reviews/CORE-USER-VMA-LIFETIME-14-review.md)、[26独审§3 R26-S02](reviews/CORE-USER-ELF-SEGMENT-LOAD-26-review.md)、[75独审§2/5](reviews/CORE-USER-ALLOCATOR-ENTRY-75-review.md)的内核分配器范围，[121独审A与构建限定](reviews/CORE-USER-STDIO-FILE-OPEN-121-review.md)及[122对121的文档判闭](reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-D00-120-121-review.md)。不重审历史全包、不重开已关闭文档项；116仅为主线选题排重，不额外要求本批重新加载。

只读固定time=a039d9803ade以下相关定义和必要上下文：

- mykernel/arch/x86_64/myos/syscall.c::brk主体；mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap、do_brk_flags的签名/返回接口，内部VMA副作用沿旧边界，不展开全函数调查。
- mykernel/mm/misc/mm_misc_types.h::mm_struct的start_brk/brk；mykernel/mm/page_alloc/buddy_const.h::PAGE_ALIGN；mykernel/include/linux/kernel/align.h；mykernel/include/uapi/linux/const.h；mykernel/arch/x86_64/mm/page/page_const_arch.h；必要ulong/long类型入口。
- mykernel/arch/x86_64/include/asm/unistd_64.h、syscalls_64.h；mykernel/arch/x86_64/entry/syscall_64.c；mykernel/include/linux/kernel/syscalls.h::MYOS_SYSCALL_DEFINE1及必要映射宏；mykernel/arch/x86_64/include/asm/syscall_wrapper.h的活动MYOS生成链。只核本包brk绑定。
- myinitramfs/mylib/myglibc/include/unistd.h；myinitramfs/mylib/myglibc/syscall/syscall.c::SYSFUNC_DEF/_SYSFUNC_DEF_/__SYSFUNC_DEF__与brk实例、共用返回段；myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/brk.c的注释性质。
- myinitramfs/mylib/myglibc/malloc/malloc.c：本地常量/全局、upage_alloc、init_uslab；同目录_malloc.h的档数/基数/结构；myinitramfs/mylib/util/list.c中所用初始化/插入/取首及直接助手；myinitramfs/mylib/include/utils.h的List/计数/container_of。
- 构建只读myinitramfs/CMakeLists.txt、mylib/CMakeLists.txt、mylib/myglibc/CMakeLists.txt及其malloc/CMakeLists.txt、syscall/CMakeLists.txt、mylib/util/CMakeLists.txt；内核mykernel/scripts/options_flags.cmake和mykernel/debug/panic.c及直接master反验。仅为构建文本边界，不读仓外库或真实产物。

强制正式窄窗必须覆盖：活动配置/构建范围；两个PAGE_SIZE及对齐宏链/字段类型；brk min/对齐/同页/缩小先赋值/增大后赋值/成功及out返回；两helper参数语义；编号/绑定/活动生成；用户声明/实例/生成宏/返回尾/注释转换；init查询、16轮与链式赋值、obj_size/obj_nr、free→used路径、initiated；upage_alloc非空与动态两路、非NULL门、独立+=、vaddr赋值、列表插尾/返回/失败free分支；实际列表计数和所需成员定义。定义、使用、宏实例、生成文本、注释性质各自标明，不能用一个定义头替代活动证据。

malloc.c的固定原件为CRLF（390个CRLF）；引文按P2保留原始字符/换行身份，或明确记录可逆展示和原对象对应。逐字重核不能只比较悄悄LF归一后的文本；LF统计与物理行、实际Git Blob分别登记。其余文件也按实际换行核，不凭编辑器视觉替换。

## 六件、规模与验收

本任务适用协议v2全部十三条。仅指定六件新增，共A6/B2/C3十一组；B每组16行轨迹，C每组最多六个观察阶段，不按字节/每次列表指针赋值制造大量节点。图可用一份16轮表对应参数化循环，不必重复铺32份同构调用链。无字数、引文数或新缺陷数量配额。

正文/facts/DOT场景ID一致；明确区分SOURCE_FACT、给定题设、整数投影、独立片段、条件caller、NOT_DERIVED/NOT_REACHED/SKIPPED。机器件至少有A接口矩阵、B两条轨迹、C状态矩阵、binding/build_scope、source_index和graph；图与facts.graph按节点id/kind/label/scenario、边from/to/kind/label一致，不允许把数据绑定画成直接调用或把条件片段画成已运行全链。

MANIFEST根使用read_channel、startup_selfcheck_quote、branch_canary_quotes、self_check三个整数及当前六件用途/覆盖/后继消费。全包新正式标记计数，交付前重开固定源逐条核，引文n=m+k，强制锚不降级。D00空，143旧91/142旧73/141旧129不重贴或混算。

deferred只登记本批新增及旧项状态变化；新条目逐项给标准source_ref（分支、固定短码、真实路径/符号、原ID/场景与证据selector），或在条目明确继承共享头并给唯一selector。不靠缺少关系的输入路径清单代替导航。R14-S01/26旧边界不重复报新漏洞，数值反例不写成已发生内存破坏；普通问题先入库，owner_decisions_requested_now为空。

P9按编号核连接器可达项1–8、11、13；9/10无实现代码/补丁、12非降级时NA。P1长标识扫描0，P2原文和支持关系、P6真实计数、配置窄窗及双canary/master startup必须实做；完整本地P9与实际预处理/编译/运行NOT_RUN。

## 发布、实际G04、停写与短回件

原exec以真实expected_sha、force=false、唯一父提交快进；恰指定六件新增，零其他修改/删除。提交后核固定/活动全文、LF、UTF-8字节、API Blob、唯一父和净差异。发评前无since全分页查重，原Draft PR17只发一条本批正式作者顶层评论，标记：

MYOS2-LEAD-002-CHAT-EXEC-22 / CORE-USER-BRK-ALLOCATOR-144 / FORMAL_DELIVERY

正式作者评论在GitHub包含完整身份/合同/固定提交/六件/模型信息、实际检查、成果及未证边界。发表后按真实ID双读、完整分页至空核本批唯一及十六个历史/主线标记；核四refs、开放PR集合、两PR原状态/元数据、固定/活动六件及净差异、143冻结六件和原作者评论。不得预签发表后G04。

“GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。”未发生写故障记not_needed，不能虚报重试。

全部实际完成后才登记READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、实测G04及STOP_WRITING，不自行启动145。最终按198给一个原生短复制块，通常3–6行：

第144批已交付；主线验收PENDING；G04=<实测结果>；<真实停写状态>。
正式作者回执：<完整GitHub评论URL，内含全部固定成果和详细记录>。
请MYOS2-A-C10用Pro独立验收，并同轮安排后继工作包。

部分交付或阻断加一句真实最小状态与必要动作；不把统计、A/B/C长摘要或检查日志搬到聊天。块外不留接收方必须拼接的条件/入口。

持续继承结果先行、人工动作集中、授权内连续推进、问题先入库、三专题六件、次一档执行/Pro独审、同轮后继、63/193/194/198及未来交接必传。阶段2；不改源码、旧成果/旧评论、主线目录或PR元数据，不建分支/PR、不合并、强推或删分支。完整本地P9、MyOS2程序/脚本、预处理、编译链接、Graphviz、QEMU、实机及真实操作均NOT_RUN。
