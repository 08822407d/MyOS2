---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: batch146_colormap_analysis_with_D00
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-23
conversation_display_name: "MYOS2-A-C10b 代码分析执行"
produced_by: GPT-6.2
model_per_owner: "GPT-6.2 Sol、Extra／高推理强度；后台及实际effort UNATTESTED"
date: 2026-11-24
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
read_channel: connector
status: AUTHOR_DELIVERY_PENDING_MAINLINE_REVIEW
open_questions: ["145三项补正待主线判闭", "实际链接、载荷和完整递归未知"]
---

# 第146批 · colormap容量、动态申请和字段绑定

**结论：** D00三项处置材料已并入本批，仍须C10判闭，原145保持RETURN；本批独立从固定time源码建立A16/B16/C3共35项有界记录。16档的静态数组容量均覆盖该给定槽数及末槽所在word；所给动态bitmap请求均不少于完整word访问空间，但不是实际malloc成功或载荷为零的证据。初始化和递归、实际链接/堆运行仍未推导。

## D00 · 145定点补正（旧原件冻结）

### R145-01 — 标准根、接收与刷新

原145 MANIFEST头第1–22行缺`base_snapshot`、`inputs_read`、`startup_selfcheck_quote`、`branch_canary_quotes`、`self_check`标准根，原正文出现的值不足以代替根。146 MANIFEST真正落地这些根键；facts.D00["R145-01"]包含明确的145替代消费副本和55条按原ID本轮重核记录。原作者申报55/55/0、本轮旧窗重核55/55、146新90窗分别计量，不能追认过去发布前时序。旧55窗52个原围栏相同、3个只有分隔LF，去除该分隔后55/55源字符相同。实际执行接收与规范刷新是两块独立记录；145旧时点UNKNOWN，146补读是真实本轮动作，详见MANIFEST。

原`malloc-slot.md`第78行含P9-11第三项精确禁串一次；旧零命中和全通过的声明不成立。替代消费句：**初始化及错误恢复不在本批已经取得的证据范围。** 旧否定句不构成运行伪报。旧145原六件及评论不改；两套六件用途/覆盖和消费索引见MANIFEST。R145-01=disposition_submitted，mainline_closure=PENDING。

### R145-02 — 缺失强制锚及NONE

活动内核C配置E146-001–006（区别于实际预处理）；用户H的数值和宏连接E146-018/019；List成员E146-073、INIT_LIST_S及实际插入助手E146-077–082；CMake的C GLOB到target_sources E146-016/017，分别补足旧E145-002、E145-036/040–045、E145-054/055。新锚逐窗原字符/原行尾可回源复核，记入146新P6。旧E145-051/054/055均为原末行NONE，围栏前仅增加一枚分隔LF；还原只移除该一枚LF，不归一化CRLF或其它字符。每个旧ID的路径、物理行、行尾和还原结果见facts.D00["R145-02"]。R145-02=disposition_submitted，mainline_closure=PENDING。

### R145-03 — 门到达、图与显式字段

旧汇总字段`refill_boundary.reached_by_any_C`、`nonreach_reason`及图`REFILL_BOUNDARY_G`不再按旧义消费。三个返回C均到达并求值308条件，均因free.count=1为假，只有主体不进入；C_EMPTY在更早的显式while(1)停止。采用以下完整映射：
| 原C模型 | gate_reached | condition_result | refill_body_entered |
| --- | --- | --- | --- |
| C_USED_MORE | true | FALSE | false |
| C_USED_LAST | true | FALSE | false |
| C_FREE_FIRST | true | FALSE | false |
| C_EMPTY | false | NOT_REACHED | false |

原四个SELECT节点已按原始前态改写标签：USED_MORE旧S.free=65，used摘首再加回，count 1→0→1，扫描前完成；USED_LAST旧S.free=1，先used移full再扫描及free减量；FREE_FIRST从free摘S、count 2→1再入used，R载荷保持、链指针可随摘除更新；EMPTY的local uslp=NULL，无列表迁移、扫描、对象更新及refill门均未到。旧完整77节点/59边复制到本批facts.graph和同一DOT的R145_独立命名空间，所有节点ID、scenario及边端点统一加前缀，原kind、边标签保留，5个更正标签逐项在facts.D00记录。

| 原B模型 | scan前后word同值 | 独立set_word | set_shift | 置位 |
| --- | --- | --- | --- | --- |
| B_ZERO | true | 0 | 0 | bm_set_bit |
| B_63 | true | 0 | 63 | bm_set_bit |
| B_64 | true | 1 | 0 | bm_set_bit |
| B_FULL | true | SKIPPED | SKIPPED | SKIPPED |
| B_EMPTY | true | SKIPPED | SKIPPED | SKIPPED |

A_OVER的选档阶段NOT_REACHED，实际链推进活动次数0；C三返回例slab总数2保持、C_EMPTY为1保持；R的total/free均128、两word零、地址载荷不变、非所选目标，链指针可能随列表调整。上述仅把旧合法数值显式化，不添加新145场景。N01维持条件观察；N02新增仅指post-slot refill顺序和前提、递归及动态初始化未知为继承背景；N03实际链接未知按144及201归INHERITED_UNCHANGED，不计新发现。R145-03=disposition_submitted，mainline_closure=PENDING。

## 新分析共用模型

选固定time中仓内myglibc候选实现；8位byte、bitmap_t=64位、sizeof=8字节；size_t/unsigned long/指针为给定64位模型，所有运算可表示，无回绕、无并发。H=0x200000取本地malloc.c CONST_2M/PAGE_SIZE，不等于内核页尺寸。代码事实与给定假设/整数推算分开，不作真实对象C ABI、指针、页映射或CPU指令测量。

## A · 16个实际静态数组及局部整数

从malloc.c 72–87实际声明解析名称和word数，从89–106指针表读取16项升序映射，而不是将合同参照表抄作证据（E146-026–045）。16档、起档32、H、bitmap类型和word位宽来自E146-018/019/022–025；E146-050给O=32×2^i、N=H/O和local_cm_size_raw=floor((N+7)/8)。末槽索引为N−1，末word=floor((N−1)/64)，末bit=(N−1)%64；源扫描word表达式见E146-089。单位严格区分：
| ID | i | 实际数组 | O字节 | N槽 | W word | 8W字节 | 64W位 | 需完整word数 | 末槽idx | 末word | 末bit | 未用位 | local_cm_size_raw |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| A00 | 0 | cm_32B | 32 | 65536 | 1024 | 8192 | 65536 | 1024 | 65535 | 1023 | 63 | 0 | 8192 |
| A01 | 1 | cm_64B | 64 | 32768 | 512 | 4096 | 32768 | 512 | 32767 | 511 | 63 | 0 | 4096 |
| A02 | 2 | cm_128B | 128 | 16384 | 256 | 2048 | 16384 | 256 | 16383 | 255 | 63 | 0 | 2048 |
| A03 | 3 | cm_256B | 256 | 8192 | 128 | 1024 | 8192 | 128 | 8191 | 127 | 63 | 0 | 1024 |
| A04 | 4 | cm_512B | 512 | 4096 | 64 | 512 | 4096 | 64 | 4095 | 63 | 63 | 0 | 512 |
| A05 | 5 | cm_1KB | 1024 | 2048 | 32 | 256 | 2048 | 32 | 2047 | 31 | 63 | 0 | 256 |
| A06 | 6 | cm_2KB | 2048 | 1024 | 16 | 128 | 1024 | 16 | 1023 | 15 | 63 | 0 | 128 |
| A07 | 7 | cm_4KB | 4096 | 512 | 8 | 64 | 512 | 8 | 511 | 7 | 63 | 0 | 64 |
| A08 | 8 | cm_8KB | 8192 | 256 | 4 | 32 | 256 | 4 | 255 | 3 | 63 | 0 | 32 |
| A09 | 9 | cm_16KB | 16384 | 128 | 2 | 16 | 128 | 2 | 127 | 1 | 63 | 0 | 16 |
| A10 | 10 | cm_32KB | 32768 | 64 | 1 | 8 | 64 | 1 | 63 | 0 | 63 | 0 | 8 |
| A11 | 11 | cm_64KB | 65536 | 32 | 1 | 8 | 64 | 1 | 31 | 0 | 31 | 32 | 4 |
| A12 | 12 | cm_128KB | 131072 | 16 | 1 | 8 | 64 | 1 | 15 | 0 | 15 | 48 | 2 |
| A13 | 13 | cm_256KB | 262144 | 8 | 1 | 8 | 64 | 1 | 7 | 0 | 7 | 56 | 1 |
| A14 | 14 | cm_512KB | 524288 | 4 | 1 | 8 | 64 | 1 | 3 | 0 | 3 | 60 | 1 |
| A15 | 15 | cm_1MB | 1048576 | 2 | 1 | 8 | 64 | 1 | 1 | 0 | 1 | 62 | 1 |

给定16档，W=ceil(N/64)每档相等；因此所有末word合法落在声明的元素数以内，i=11–15分别有32/48/56/60/62个未用位。这仅证明给定容量关系，不能证明内存实际装载或真实对象有效。完整读取`init_uslab`物理174–221行并对标识符`cm_size`做精确词边界搜索，出现一次（196行声明赋值）；未见该函数中后续读取，单位意图UNKNOWN，不能把raw整数充作实际word数、分配量或动态请求参数。

## B · 16个动态bitmap malloc请求与条件选档

同一N来自所属slab的i，但另一个malloc请求所选择的cache索引为j。直接读E146-056的表达式`cslp->total / sizeof(bitmap_t) + sizeof(bitmap_t)`，在给定模型是B_req=floor(N/8)+8字节；M=8×ceil(N/64)是访问完整word的最小字节空间（不是仅用ceil(N/8)的紧凑位数）。给定initiated=true且16档有效升序链，size上界和while按E146-020、058–060：从组0推进直到请求不超过组obj_size。观察止于选档，不进入slab选择或函数外返。
| ID | 所属i | N槽 | B_req字节 | M字节 | B_req−M字节 | 所选j | S字节 | S−B_req字节 | 推进 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| B00 | 0 | 65536 | 8200 | 8192 | 8 | 9 | 16384 | 8184 | 9 |
| B01 | 1 | 32768 | 4104 | 4096 | 8 | 8 | 8192 | 4088 | 8 |
| B02 | 2 | 16384 | 2056 | 2048 | 8 | 7 | 4096 | 2040 | 7 |
| B03 | 3 | 8192 | 1032 | 1024 | 8 | 6 | 2048 | 1016 | 6 |
| B04 | 4 | 4096 | 520 | 512 | 8 | 5 | 1024 | 504 | 5 |
| B05 | 5 | 2048 | 264 | 256 | 8 | 4 | 512 | 248 | 4 |
| B06 | 6 | 1024 | 136 | 128 | 8 | 3 | 256 | 120 | 3 |
| B07 | 7 | 512 | 72 | 64 | 8 | 2 | 128 | 56 | 2 |
| B08 | 8 | 256 | 40 | 32 | 8 | 1 | 64 | 24 | 1 |
| B09 | 9 | 128 | 24 | 16 | 8 | 0 | 32 | 8 | 0 |
| B10 | 10 | 64 | 16 | 8 | 8 | 0 | 32 | 16 | 0 |
| B11 | 11 | 32 | 12 | 8 | 4 | 0 | 32 | 20 | 0 |
| B12 | 12 | 16 | 10 | 8 | 2 | 0 | 32 | 22 | 0 |
| B13 | 13 | 8 | 9 | 8 | 1 | 0 | 32 | 23 | 0 |
| B14 | 14 | 4 | 8 | 8 | 0 | 0 | 32 | 24 | 0 |
| B15 | 15 | 2 | 8 | 8 | 0 | 0 | 32 | 24 | 0 |

B_req−M与S−B_req意义不同：前者相对于完整word访问需要，后者来自条件档位取整。最大B_req=8200，全部不超过CONST_1M；i=0–10的B_req−M=8，i=11–15为4/2/1/0/0。在给定档位下均≥0且S≥B_req。不能据此说动态分配一定成功、可访问对象/映射存在、位图已初始化，或推广到任意total/其他位宽。

## C · 三个独立片段，不拼接真实调用链

### C_BASE

给定一次i与有效base slab/cache元数据；init_uslab的O/N生产、列表初始化、其他字段与调用见E146-046–054。bitmap相关的直接写为：colormap=base_colormaps[i]，uslabcache_p=scgp，free/total由链式赋值获得N。完整init_uslab体174–221未见对bitmap数组元素载荷的直接显式store；该函数仍有其他字段写入及被调函数，不能外推到全部callee。16个全局数组声明没有显式初始化器（E146-026–041）；不将语言静态存储期初值模型当真实加载状态，本模型不赋给数组实际零值。真实payload NOT_DERIVED，被调函数影响UNKNOWN。

### C_DYNAMIC

独立给定合法cslp、两次读取cslp.total均N，两个malloc及upage_alloc均正常返回有效、足够大、互异对象，相关list/page操作合法。只看callee直接写（E146-055–057、065–073、077）：
| 观察点 | 条件性源级写入 |
| --- | --- |
| 1 | nslp local pointer receives metadata malloc result |
| 2 | INIT_LIST_S initializes nslp.uslab_list.prev and next to node itself; helper normal return given |
| 3 | nslp.colormap receives bitmap malloc result with request floor(N/8)+8 |
| 4 | nslp.total and nslp.free receive N via the source chain assignment from the second cslp.total read; no independently observed machine-store order |
| 5 | nslp.upage_p receives normal upage_alloc result |
| 6 | nslp.virt_addr=nslp.upage_p.vaddr and page.uslab_p=nslp |
| 7 | return nslp (callee only) |

元数据malloc的量保持sizeof(uslab_s)符号，不求结构布局。callee没有给uslabcache_p赋值，不能借caller的318行反填callee终点；写colormap指针不等于写bitmap载荷。新bitmap初始payload UNKNOWN；free=total=N不能推出所有bits为0。完整uslab_alloc正文的显式直接写集合限此，callee效果和递归可达性不作推论。

### C_PUBLISH

与前两例独立：入口在uslab_alloc正常返回后、malloc第312行之前，给new_uslp非NULL有效且total=N，列表助手合法、F_r>=1。F_r、T_r、C_r分别是在callee返回时的free.count、normal_slab_total和nsobj_free_count；不默认嵌套调用无副作用，不以310之前的计数替代。
| 观察点 | 源码动作/判断 | 局部变化 |
| --- | --- | --- |
| 1 | line312 free.count-- | F_r→F_r−1 |
| 2 | line313 new_uslp==NULL | FALSE |
| 3 | line318 new_uslp.uslabcache_p=uscgp | 仅字段绑定 |
| 4 | line319 add_to_head free; normal helper insertion then count++ | F_r−1→F_r |
| 5 | line320 total++ | T_r→T_r+1 |
| 6 | line321 nsobj_free_count += new_uslp.total | C_r→C_r+N |

止于321行后，F=F_r、T=T_r+1、C=C_r+N；尚未推到malloc函数return，不能把此增量当整个refill净变化。该片段没有直接显式清bitmap载荷。列表count++是在list_insert_next正常返回之后（E146-061–063、073–084）。

## 配置、构建与后继

E146-001–007取固定time活动内核配置及canary，用户顶层独立构建文本E146-008–012只显式进入myinit/myshell，配置仓库外musl工具链；E146-013–017为另一棵mylibc静态目标及GLOB变量来源。实际sh/init链接符号UNKNOWN（继承，不计新增发现）；预处理、编译链接、外部库读取、实际运行NOT_RUN。R145_有效旧图与146 A/B参数化图、C三独立入口之间无跨批实际调用边；覆盖映射见facts.graph_coverage。全部新源锚、旧55窗重核、D00三项处置及六件消费在MANIFEST／facts／evidence中。D00主线判闭、146独审均PENDING；完整本地P9仍NOT_RUN。仅由C10 Pro裁决及同轮后继，不自行开始147。
