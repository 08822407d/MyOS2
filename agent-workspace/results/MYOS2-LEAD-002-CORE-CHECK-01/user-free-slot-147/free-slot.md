---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: batch147_free_slot_analysis
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-23
produced_by: GPT-6.2
model_per_owner: "GPT-6.2 Sol Extra/high requested; backend and actual effort UNATTESTED"
date: 2026-11-25
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
read_channel: connector
status: AUTHOR_PENDING_MAINLINE_REVIEW
---

# 第147批 · free前半段：页键查找、位图文义与低位释放

**结论范围：** A7/B11/C6共24个独立有界记录；给定宽度与合法对象条件下的静态数值/源级路径，不是实际执行。146按C10独审§4合读PASS_PENDING_LOCAL；145三项补正已关闭，combined145=PASS_PENDING_LOCAL；147 D00空，旧件不动。146载荷、实际链接与完整递归仍未知。

## 共同模型与外部语义边界

8位byte，64位size_t/unsigned long/地址数值/bitmap_t/unsigned long long，bitmap_t unsigned、sizeof=8，int/unsigned为32位，INT_MAX=2147483647。所有模型指定数值可表示，无回绕、并发或异常对象。C的DEBUG未定义为GIVEN，不是实际预处理结论；free的DEBUG等待在NULL门之前（E147-033、E147-035）。实际编译器/标准/DEBUG、sh实际链接均UNKNOWN。

- 指定N1547官方PDF（2010-12-02，https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1547.pdf，§6.4.4.1¶5、6.5.7¶3–4、6.2.5¶19、6.5.6¶3/9）本轮直接两次超时、www9官方变体不可取得，检索未找到可独立读取的指定原文。状态UNKNOWN_INDEPENDENT_SOURCE。按204明确给定的历史ISO/int32模型完成B11，不用其他草案或镜像冒充N1547原文，不签独立ISO外证完成。
- [EXTERNAL https://gcc.gnu.org/onlinedocs/gcc/Pointer-Arith.html] GCC在线手册 Arithmetic on void- and Function-Pointers §6.13.2（2026-11-25读取；具体发行版未独立核定）：GNU C允许void指针加减，按大小1处理。C中只采用合同给定字节差投影，不证明真实ISO对象/指针差合法。
- [EXTERNAL https://gcc.gnu.org/onlinedocs/gcc/Integers-implementation.html] GCC在线手册 Integers implementation §4.5（2026-11-25读取）：文档说明对某些有符号左移采用扩展语义，不能把ISO模型的停止边界等同实际GCC行为。实际版本、选项、指令/诊断结果均不推导。外证两项与源窗计数分开。

## A · 2MiB对齐键与anchor遍历（7例）

H=0x200000取本地CONST_2M/PAGE_SIZE，宏链E147-023/E147-024/E147-025/E147-026/E147-027。64位投影中round_mask=0x00000000001fffff、补掩码=0xffffffffffe00000；`upage_search`先把入参按该宏取整，ret_val初值NULL。以anchor.next开始，直到anchor；container_of恢复描述符，比较其vaddr与对齐键**相等**，不是区间包含检查。相等时保存描述符指针并break（E147-029–E147-032）；count=3或0不是遍历终止条件。

| ID | 输入x | 取整键 | 比较顺序 | 次数 | 返回 | 终止 |
| --- | --- | --- | --- | --- | --- | --- |
| A_FIRST | 0x60601234 | 0x60600000 | D2 | 1 | D2 | BREAK_ON_EQUAL |
| A_BASE | 0x60000000 | 0x60000000 | D2,D0 | 2 | D0 | BREAK_ON_EQUAL |
| A_LASTBYTE | 0x601fffff | 0x60000000 | D2,D0 | 2 | D0 | BREAK_ON_EQUAL |
| A_NEXT | 0x60200000 | 0x60200000 | D2,D0,D1 | 3 | D1 | BREAK_ON_EQUAL |
| A_NEXTLAST | 0x603fffff | 0x60200000 | D2,D0,D1 | 3 | D1 | BREAK_ON_EQUAL |
| A_MISS | 0x60400000 | 0x60400000 | D2,D0,D1 | 3 | NULL | ANCHOR_REACHED |
| A_EMPTY | 0x60000000 | 0x60000000 | — | 0 | NULL | ANCHOR_REACHED |

D0=0x60000000、D1=0x60200000、D2=0x60600000，非空顺序[D2,D0,D1]。命中返回uPage_s指针，不是key数值；A_MISS三次均不等后到anchor，A_EMPTY初始即anchor而零次比较。源函数仅读相关链和vaddr、写局部量；给定无其他写者时描述符、owner、count、链和载荷不变。未命中仅止于search返回NULL；free紧接着直接读取upgp->uslab_p（E147-036），不把这两步拼成NULL访问的实际结果。

## B · 两个独立helper（11组）

### assigned（GIVEN历史ISO模型）

E147-048/E147-049：unit_idx=idx/(8*sizeof(bitmap_t))、t=idx%(8*sizeof(bitmap_t))；无后缀十进制1在本给定模型为int32，移位两侧各自整数提升，结果类型取提升后的左类型；右侧64位余数不会自动拓宽1。只有表达式求值后才转换到bitmap_t mask。t=0…30时表达式可表示，读所给word，再把unit&mask转换赋予unsigned32 ret_val并返回；这里返回掩码数值而非归一化布尔。t=31在该ISO模型为正的有符号结果不可表示，t>=32为计数达到/超过左宽度；均ANALYSIS_STOP，后续mask、读取、返回NOT_DERIVED，不能声称机器返回0或存在源中额外控制分支。GCC扩展实际结果不推导。

| ID | idx | word | t | assigned ISO给定模型 | mask赋值后 | unsigned返回 | 非零 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| B_0 | 0 | 0 | 0 | CONTINUE | 0x0000000000000001 | 1 | true |
| B_UNSET | 2 | 0 | 2 | CONTINUE | 0x0000000000000004 | 0 | false |
| B_30 | 30 | 0 | 30 | CONTINUE | 0x0000000040000000 | 1073741824 | true |
| B_31 | 31 | 0 | 31 | ANALYSIS_STOP_SIGNED_RESULT_NOT_REPRESENTABLE | NOT_DERIVED | NOT_DERIVED | NOT_DERIVED |
| B_32 | 32 | 0 | 32 | ANALYSIS_STOP_SHIFT_COUNT_GE_LEFT_WIDTH | NOT_DERIVED | NOT_DERIVED | NOT_DERIVED |
| B_63 | 63 | 0 | 63 | ANALYSIS_STOP_SHIFT_COUNT_GE_LEFT_WIDTH | NOT_DERIVED | NOT_DERIVED | NOT_DERIVED |
| B_64 | 64 | 1 | 0 | CONTINUE | 0x0000000000000001 | 1 | true |
| B_94 | 94 | 1 | 30 | CONTINUE | 0x0000000040000000 | 1073741824 | true |
| B_95 | 95 | 1 | 31 | ANALYSIS_STOP_SIGNED_RESULT_NOT_REPRESENTABLE | NOT_DERIVED | NOT_DERIVED | NOT_DERIVED |
| B_96 | 96 | 1 | 32 | ANALYSIS_STOP_SHIFT_COUNT_GE_LEFT_WIDTH | NOT_DERIVED | NOT_DERIVED | NOT_DERIVED |
| B_127 | 127 | 1 | 63 | ANALYSIS_STOP_SHIFT_COUNT_GE_LEFT_WIDTH | NOT_DERIVED | NOT_DERIVED | NOT_DERIVED |

安全组的读值、按位AND及赋值后word同值详见facts。危险组不填后续word值，分析停止不等于实际运行的NOT_REACHED。B_UNSET给[3,0]、idx2，返回0。

### clear（从每例原始两个word独立起步）

E147-050：以1ULL为64位无符号左操作数，t=0…63均在宽度内。按word_idx执行一次AND赋值；补掩码按有限64位表示。绝不由assigned的分析停止连接到clear；不读word2；函数void无返回值。

| ID | idx | word | t | 原word0/1 | clear mask | 64位补掩码 | 后word0/1 | 改位数 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| B_0 | 0 | 0 | 0 | 0xffffffffffffffff / 0xffffffffffffffff | 0x0000000000000001 | 0xfffffffffffffffe | 0xfffffffffffffffe / 0xffffffffffffffff | 1 |
| B_UNSET | 2 | 0 | 2 | 0x0000000000000003 / 0x0000000000000000 | 0x0000000000000004 | 0xfffffffffffffffb | 0x0000000000000003 / 0x0000000000000000 | 0 |
| B_30 | 30 | 0 | 30 | 0xffffffffffffffff / 0xffffffffffffffff | 0x0000000040000000 | 0xffffffffbfffffff | 0xffffffffbfffffff / 0xffffffffffffffff | 1 |
| B_31 | 31 | 0 | 31 | 0xffffffffffffffff / 0xffffffffffffffff | 0x0000000080000000 | 0xffffffff7fffffff | 0xffffffff7fffffff / 0xffffffffffffffff | 1 |
| B_32 | 32 | 0 | 32 | 0xffffffffffffffff / 0xffffffffffffffff | 0x0000000100000000 | 0xfffffffeffffffff | 0xfffffffeffffffff / 0xffffffffffffffff | 1 |
| B_63 | 63 | 0 | 63 | 0xffffffffffffffff / 0xffffffffffffffff | 0x8000000000000000 | 0x7fffffffffffffff | 0x7fffffffffffffff / 0xffffffffffffffff | 1 |
| B_64 | 64 | 1 | 0 | 0xffffffffffffffff / 0xffffffffffffffff | 0x0000000000000001 | 0xfffffffffffffffe | 0xffffffffffffffff / 0xfffffffffffffffe | 1 |
| B_94 | 94 | 1 | 30 | 0xffffffffffffffff / 0xffffffffffffffff | 0x0000000040000000 | 0xffffffffbfffffff | 0xffffffffffffffff / 0xffffffffbfffffff | 1 |
| B_95 | 95 | 1 | 31 | 0xffffffffffffffff / 0xffffffffffffffff | 0x0000000080000000 | 0xffffffff7fffffff | 0xffffffffffffffff / 0xffffffff7fffffff | 1 |
| B_96 | 96 | 1 | 32 | 0xffffffffffffffff / 0xffffffffffffffff | 0x0000000100000000 | 0xfffffffeffffffff | 0xffffffffffffffff / 0xfffffffeffffffff | 1 |
| B_127 | 127 | 1 | 63 | 0xffffffffffffffff / 0xffffffffffffffff | 0x8000000000000000 | 0x7fffffffffffffff | 0xffffffffffffffff / 0x7fffffffffffffff | 1 |

B_UNSET清除原本为0的bit2，改动位数0；此局部helper结果不证明整体free允许重复释放。B_31/B_95与B_32/63/96/127的assigned停止原因分别记录，clear仍有自己的独立输入。

## C · 给定低位正常槽及拒绝路径（6例）

除NULL外，给upage_search正常返回有效page，page.uslab_p=S，S.uslabcache_p=cache；S.V=0x60000000、O=16384、N=128，备用R total/free=128、双word0，cache总slab=2。有效对象、循环链、list helper正常返回及给定字节差均为前提，不是实际指针条件证明。DEBUG预处理排除，NULL检查在lookup前，lookup后两级归属读取、空slab门、obj_idx和assigned门依源码次序（E147-036/E147-037/E147-038/E147-039）。

| ID | 给定obj_p | 实际求得idx | S.free前→后 | S.word0/1前 | S.word0/1后 | CF/CU前→后 | F/U/L最终 | 终点 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| C_NULL | NULL | NOT_REACHED | NOT_CONSUMED | NOT_CONSUMED | NOT_CONSUMED | NOT_CONSUMED | NOT_CONSUMED | CONDITIONAL_VOID_EARLY_RETURN |
| C_FULL | 0x60000000 | 0 | 0→1 | 0xffffffffffffffff / 0xffffffffffffffff | 0xfffffffffffffffe / 0xffffffffffffffff | 128/128→129/127 | [R]/[S]/[] | CONDITIONAL_VOID_COMPLETION |
| C_USED | 0x60078000 | 30 | 64→65 | 0xffffffffffffffff / 0x0000000000000000 | 0xffffffffbfffffff / 0x0000000000000000 | 192/64→193/63 | [R]/[S]/[] | CONDITIONAL_VOID_COMPLETION |
| C_LAST | 0x60000000 | 0 | 127→128 | 0x0000000000000001 / 0x0000000000000000 | 0x0000000000000000 / 0x0000000000000000 | 255/1→256/0 | [R]/[S]/[] | CONDITIONAL_VOID_COMPLETION |
| C_EMPTY | 0x60000000 | NOT_REACHED | 128→128 | 0x0000000000000000 / 0x0000000000000000 | 0x0000000000000000 / 0x0000000000000000 | 256/0→256/0 | [S,R]/[]/[] | SOURCE_EXPLICIT_WHILE_1_EMPTY_SLAB |
| C_UNASSIGNED | 0x60008000 | 2 | 126→126 | 0x0000000000000003 / 0x0000000000000000 | 0x0000000000000003 / 0x0000000000000000 | 254/2→254/2 | [R]/[S]/[] | SOURCE_EXPLICIT_WHILE_1_UNASSIGNED_SLOT |

下表每格为 reached/result/body_entered（DEBUG三项N/A；未到达与到达FALSE区分）：

| ID | DEBUG | NULL | empty | !assigned | free==1 | else-if free==0 | base-slab | reclaim |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| C_NULL | N/A/N/A/N/A | true/TRUE/true | false/NOT_REACHED/false | false/NOT_REACHED/false | false/NOT_REACHED/false | false/NOT_REACHED/false | false/NOT_REACHED/false | false/NOT_REACHED/false |
| C_FULL | N/A/N/A/N/A | true/FALSE/false | true/FALSE/false | true/FALSE/false | true/TRUE/true | false/NOT_REACHED/false | false/NOT_REACHED/false | true/FALSE/false |
| C_USED | N/A/N/A/N/A | true/FALSE/false | true/FALSE/false | true/FALSE/false | true/FALSE/false | true/FALSE/false | false/NOT_REACHED/false | true/FALSE/false |
| C_LAST | N/A/N/A/N/A | true/FALSE/false | true/FALSE/false | true/FALSE/false | true/FALSE/false | true/FALSE/false | false/NOT_REACHED/false | true/FALSE/false |
| C_EMPTY | N/A/N/A/N/A | true/FALSE/false | true/TRUE/true | false/NOT_REACHED/false | false/NOT_REACHED/false | false/NOT_REACHED/false | false/NOT_REACHED/false | false/NOT_REACHED/false |
| C_UNASSIGNED | N/A/N/A/N/A | true/FALSE/false | true/FALSE/false | true/TRUE/true | false/NOT_REACHED/false | false/NOT_REACHED/false | false/NOT_REACHED/false | false/NOT_REACHED/false |

### C_NULL

1. DEBUG block excluded by given preprocessing premise; no runtime condition
2. obj_p==NULL TRUE
3. return; no lookup, pointer or slab read

### C_FULL

1. DEBUG block excluded by given preprocessing premise; no runtime condition
2. obj_p==NULL FALSE
3. upage_search normal return GIVEN; read page.uslab_p=S, then S.uslabcache_p=cache
4. S.free==S.total FALSE
5. given byte difference 0, obj_idx=0, assigned returns 1; !assigned FALSE
6. clear(0); S.free 0->1; CF 128->129; CU 128->127
7. updated S.free==1 TRUE
8. list_del_init S from full, full.count 1->0, add used head, used.count 0->1; else-if not evaluated
9. free-list count 1>2 FALSE; reclaim body not entered
10. normal source end conditional void completion; no runtime execution

初始列表F/U/L=[R]/[]/[S]，count=1/0/1；最终count=1/1/0。S/R总数2保持，R双word与total/free保持，UNCHANGED: R is in separate free list for C_FULL; no list mutation in other cases。S占位数128→127，末S.total−free=127；CF+CU=256，容量256。consistent with given membership/conditions。

### C_USED

1. DEBUG block excluded by given preprocessing premise; no runtime condition
2. obj_p==NULL FALSE
3. upage_search normal return GIVEN; read page.uslab_p=S, then S.uslabcache_p=cache
4. S.free==S.total FALSE
5. given byte difference 491520, obj_idx=30, assigned returns 1073741824; !assigned FALSE
6. clear(30); S.free 64->65; CF 192->193; CU 64->63
7. updated S.free==1 FALSE
8. else-if updated free==0 FALSE (updated=65); no migration; S stays used
9. free-list count 1>2 FALSE; reclaim body not entered
10. normal source end conditional void completion; no runtime execution

初始列表F/U/L=[R]/[S]/[]，count=1/1/0；最终count=1/1/0。S/R总数2保持，R双word与total/free保持，UNCHANGED: R is in separate free list for C_FULL; no list mutation in other cases。S占位数64→63，末S.total−free=63；CF+CU=256，容量256。consistent with given membership/conditions。

### C_LAST

1. DEBUG block excluded by given preprocessing premise; no runtime condition
2. obj_p==NULL FALSE
3. upage_search normal return GIVEN; read page.uslab_p=S, then S.uslabcache_p=cache
4. S.free==S.total FALSE
5. given byte difference 0, obj_idx=0, assigned returns 1; !assigned FALSE
6. clear(0); S.free 127->128; CF 255->256; CU 1->0
7. updated S.free==1 FALSE
8. else-if updated free==0 FALSE (updated=128); no migration; S stays used despite free==total
9. free-list count 1>2 FALSE; reclaim body not entered
10. normal source end conditional void completion; no runtime execution

初始列表F/U/L=[R]/[S]/[]，count=1/1/0；最终count=1/1/0。S/R总数2保持，R双word与total/free保持，UNCHANGED: R is in separate free list for C_FULL; no list mutation in other cases。S占位数1→0，末S.total−free=0；CF+CU=256，容量256。MISMATCH: empty by free==total but remains normal_slab_used; not repaired by count conservation。

### C_EMPTY

1. DEBUG block excluded by given preprocessing premise; no runtime condition
2. obj_p==NULL FALSE
3. upage_search normal return GIVEN; read page.uslab_p=S, then S.uslabcache_p=cache
4. S.free==S.total TRUE
5. line348 while(1); no obj_idx or bitmap access; stop source-level path

初始列表F/U/L=[S,R]/[]/[]，count=2/0/0；最终count=2/0/0。S/R总数2保持，R双word与total/free保持，UNCHANGED: R is in separate free list for C_FULL; no list mutation in other cases。S占位数0→0，末S.total−free=0；CF+CU=256，容量256。consistent with given membership/conditions。

### C_UNASSIGNED

1. DEBUG block excluded by given preprocessing premise; no runtime condition
2. obj_p==NULL FALSE
3. upage_search normal return GIVEN; read page.uslab_p=S, then S.uslabcache_p=cache
4. S.free==S.total FALSE
5. given byte difference 32768, obj_idx=2, assigned returns 0; !assigned TRUE
6. line355 while(1); no clear or counter/list mutation

初始列表F/U/L=[R]/[S]/[]，count=1/1/0；最终count=1/1/0。S/R总数2保持，R双word与total/free保持，UNCHANGED: R is in separate free list for C_FULL; no list mutation in other cases。S占位数2→2，末S.total−free=2；CF+CU=256，容量256。consistent with given membership/conditions。

正常三例均先clear再更新对象计数（E147-040），用更新后的free求分支（E147-041/E147-042/E147-043）。C_FULL旧free=0，更新1先unlink/full.count--后插used，head helper内实际插入完成才count++（E147-071）；else-if未到达。C_USED更新65，两个条件均假；C_LAST更新128，else-if实际比较0为假，S仍在used且free=total。给定无回绕、原free<N且正常更新的范围内free+1≥1，因此0分支不可满足；不推广畸形状态或任意配置。三正常例回收门均已求值且free-list count=1≤2，为假（E147-044）；未进入uslab_free或回收递归。C_NULL条件性void早返，C_EMPTY/C_UNASSIGNED是源中显式while(1)处的观察终止，未运行CPU；与B的ANALYSIS_STOP不同。

## 配置/构建及后继消费

固定time活动内核C配置和panic canary见E147-001,E147-002,E147-003,E147-004,E147-005,E147-006,E147-007,E147-008,E147-009；用户顶层DEBUG条件与外部musl/直接myinit,myshell子目录、独立mylibc的malloc/util GLOB和消费见E147-010,E147-011,E147-012,E147-013,E147-014,E147-015,E147-016,E147-017,E147-018,E147-019,E147-020,E147-021,E147-022。这些文本不证明用户库实际宏、预处理、ABI、编译、链接或运行；actual_linked_to_sh=UNKNOWN继承。完整本地P9、编译链接、MyOS2程序/脚本、Graphviz、QEMU、实机与真实操作NOT_RUN。本批仅两新增局部条件观察，见deferred；不要求立即修复。机器表/图与证据逐项可读，主线独审PENDING，后继由C10安排。
