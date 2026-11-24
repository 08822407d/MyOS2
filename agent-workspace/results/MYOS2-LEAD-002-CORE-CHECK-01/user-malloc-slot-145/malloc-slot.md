---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: batch145_malloc_slot_analysis
conversation_display_name: MYOS2-A-C10b 代码分析执行
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-23
produced_by: "GPT-6（本会话系统标识；后台未认证）"
model_per_owner: "GPT-6.2 Sol，Extra／高推理强度（Owner指定；实际后台档不可独立认证）"
date: 2026-11-24
base_snapshot: "time@a039d9803ade (short SHA)"
read_channel: connector
status: AUTHOR_PENDING_MAINLINE_REVIEW
open_questions: ["实际运行和实际链接未知；见deferred-findings.yaml"]
---

# 第145批：用户malloc档位、bitmap找槽与slab对象消费

## 范围、来源和结论

针对固定time源码的**17组给定初态、条件性静态模型**，不是一次内核编译、musl链接、真实malloc调用、内存写入、C指针有效性验证或CPU运行轨迹。只覆盖A八种缓存档位选择、B五种64位位图找槽与另给定的置位动作、C四种已初始化有效元数据下的列表及对象分配结果。首次init_uslab、144的brk初始化、完整malloc/free回收、并发与其他下层失败路径都不重复。第144批及主线R144-N01别名证据补正保持原样，D00为空；145新增条件观察不撤销144主线裁决。

源：`myinitramfs/mylib/myglibc/malloc/malloc.c`（44db9ea9d7f7）、`_malloc.h`（2dea1f8fc05c）、`util/bitmap.c`（6a86523c41ba）、`include/utils.h`（4e3e8910ac00）、`util/list.c`（24f7a214a288）。独立构建文本和全部55个固定物理行短锚见evidence.md；条件记录的机器权威副本见facts-and-boundaries.yaml。

## A · 8个独立size入口，只推到选档

前提：`initiated=true`，16个已存在的cache按`obj_size=32<<i`从小到大排好，组链及成员有效。入口先检查初始化，再检查`size>CONST_1M`，非超界从组0开始，条件`size>uscgp->obj_size`为真才沿`next`做一次`container_of`。第15组即1048576；并没有无限自增的组索引或任意size对齐函数。若超界则绕开选档，`ret_val`原值为NULL，来到函数末尾返回。（E145-001、003、010–012、031、035）

| 模型 | size | 尺寸门 | 选组i | 组obj_size | 链前进 | 模型终点 |
|---|---:|---|---:|---:|---:|---|
| A_ZERO | 0 | 通过 | 0 | 32 | 0 | 仅选档 |
| A_ONE | 1 | 通过 | 0 | 32 | 0 | 仅选档 |
| A_32 | 32 | 通过 | 0 | 32 | 0 | 仅选档 |
| A_33 | 33 | 通过 | 1 | 64 | 1 | 仅选档 |
| A_64 | 64 | 通过 | 1 | 64 | 1 | 仅选档 |
| A_65 | 65 | 通过 | 2 | 128 | 2 | 仅选档 |
| A_MAX | 1048576 | 通过 | 15 | 1048576 | 15 | 仅选档 |
| A_OVER | 1048577 | 拒绝 | — | — | 0 | 返回NULL |

各通过例都仅停于cache成员，**不能**接着写成已找出bitmap槽、已返回某个虚址。A_OVER的NULL只属于本例正常函数返回，不是空slab表条件的替代出口。

## B · 5个独立位图扫描与可选置位动作

本地`bitmap_t=__UINT64_TYPE__`、`BITMAP_UNITSIZE=64`，给定两个可访问的64位word、扫描上界128（排他）。`bm_get_freebit_idx`逐个测试i，从`start`到严格小于`limit`，每次按`i/64`读word，用`1ULL<<(i%64)`的掩码寻找零位；扫描函数**不置位**。`bm_set_bit`由题设作为返回后另一动作执行，不能画成扫描helper内部调用。（E145-034、037–039）

| 场景 | 初值word0 / word1 | start | 扫描返回i | bit检查次数 | 读过的word索引 | 独立置位结果 |
|---|---|---:|---:|---:|---|---|
| B_ZERO | 0 / 0 | 0 | 0 | 1 | 0 | set(0)：word0=1 |
| B_63 | 0x7fffffffffffffff / 0 | 0 | 63 | 64 | 0 | set(63)：word0=0xffffffffffffffff |
| B_64 | 0xffffffffffffffff / 0 | 0 | 64 | 65 | 0、1 | set(64)：word1=1 |
| B_FULL | 全1 / 全1 | 0 | 128 | 128 | 0、1 | 跳过置位 |
| B_EMPTY | 0x0123456789abcdef / 0xfedcba9876543210 | 128 | 128 | 0 | 无 | 跳过置位 |

`B_FULL`到i=128不执行循环体，所以没有第129次读取，更不会访问word2；`B_EMPTY`从边界起步，完全不读任何word。表中“读过的索引”是去重集合，不能替代内层连续bit检查总次数。两个跳过场景的返回128是`limit`哨兵，不是可写的第128号槽。两个helper的按位表达式不能和`bm_get_assigned_bit`中的其他移位形式混同。

## C · 4个互相独立的有效初态

共同前提：size=0x4000=16384，用户allocator内部宏`PAGE_SIZE=CONST_2M`即H=0x200000；组i=9，槽宽16384、每slab128槽，S的虚址只作为整数V=0x60000000，bit map分两枚64位word。所有给定列表成员、链表操作、页对象元数据、指针算术所需的合法性和调用正常返回均为**条件**；无外部写者。R是在三项返回例中已在free列表中的预留空slab。计算`(size_t)V + obj_size * idx`只是数值投影，不声明真实C对象存在。（E145-002、003、006–009、013–020、028–045）

| C场景 | 取slab来源 | 扫到槽idx | 偏移offset | 投影返回地址 | S.free 旧→新 | cache free/used 旧→新 |
|---|---|---:|---|---|---:|---|
| C_USED_MORE | used[S] | 63 | 0xfc000 | 0x600fc000 | 65→64 | 193/63→192/64 |
| C_USED_LAST | used[S] | 127 | 0x1fc000 | 0x601fc000 | 1→0 | 129/127→128/128 |
| C_FREE_FIRST | free[S,R] | 0 | 0 | 0x60000000 | 128→127 | 256/0→255/1 |
| C_EMPTY | used/free均空，full[S] | 未执行 | 未执行 | **不返回** | 0→0 | 0/128→0/128 |

**C_USED_MORE：** `used`摘S导致used.count 1→0。原`S.free=65`不等于1，**扫描前**重新将S放入used（count 0→1）。当原word0低63位均1、word1为零时，返回槽63；`bm_set_bit`后word0成全1。R仍在free，count=1，后续`<1`补充门为假；返回数值0x600fc000。

**C_USED_LAST：** 原`S.free=1`，从used摘S后，代码在扫描/实际减free前便把S放入full（full.count 0→1）。bit0–126已占，槽127得以选择，随后`S.free=0`、两word全1。free[R]未动，补充门仍不触发；投影0x601fc000。**列表迁移先于bitmap槽写入**，这一局部中间态不是宣称其他线程可见或实际运行安全。

**C_FREE_FIRST：** used为空，free的头S被取出，free.count 2→1，紧接着加入used使used.count 0→1。S的bitmap最初两个word均零，所以选槽0，最终word0=1，S.free=127，返回0x60000000。留在free的R保障补充门为假。

**C_EMPTY：** used.count和free.count都是0，不选择full中的S作为分配来源。源码走`else`，在`malloc.c:291`遇到显式`while(1);`；把静态推演停在那里。槽扫描、计数更新、refill和函数末尾return均未到达。不能把局部`ret_val=NULL`直接写成“函数返回NULL”；也不能把无限循环的源级存在表述为观察到真实CPU永久不返回。

每一次list`count`变化依赖相应`list_header_remove_head`/add正常完成；失败、畸形节点、访问越界等场景均非本次给定模型的证明范围。机器表保存每例S双word前后位图、used/free/full成员与count的有序操作。（E145-040–045）

## 结尾补充路径与构建边界

对象槽更新的后续`normal_slab_free.count<1`门在三个有返回的C场景都为假（始终有R，free.count=1），在C_EMPTY中则根本未到达。**另一个条件为真的假想入口**按源码先暂时加free.count，再`uslab_alloc(uslp)`，仅在它正常返回时回减；NULL结果转入另一处显式无限循环，非NULL后才绑定所属cache、加入free并增加slab总数及free对象数。`uslab_alloc`又请求两次动态malloc和`upage_alloc`，其递归可达性、内存与page成功、初始化/错误恢复均不属145已验证项。（E145-021–027）

顶层`myinitramfs/CMakeLists.txt`只显式进入`myinit`及`myshell`，且将编译器配置到仓库外musl路径。独立的`mylib`CMake链确实定义静态mylibc并从malloc/util目录加入C文件，但**无法从这些文本推出sh或init实际链接的是这份自定义malloc**；actual_linked_to_sh=UNKNOWN，实际用户程序堆行为NOT_RUN。（E145-046–055）

## 审查接口

六件中的`facts-and-boundaries.yaml`是结构化结果，含A8/B5/C4、`refill_boundary`、`build_scope`、`source_index`和`graph`；其`graph`与`malloc-slot-paths.dot`节点、边、属性同源；`evidence.md`包含55处独立短锚。`deferred-findings.yaml`记录本批出现的条件性风险和证据缺口；既有历史事实不得由本交付自动关闭。请C10在原144主线证据补注仍有效的基础上独立审核，不能将本交付转换为运行、补丁或主线验收授权。
