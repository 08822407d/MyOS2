---
task_id: "CORE-USER-PAGE-REF-LIFETIME-18"
track_id: "MYOS2-LEAD-002"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-02"
conversation_display_name: "MYOS2-A-C03b 代码分析执行续建"
produced_by: "GPT-6"
model_per_owner: "GPT-6"
model_effort_per_owner: "Extra（Owner申报；非后台认证）"
date: "2026-11-01"
source_branch: "time"
source_pin_short12: "a039d9803ade"
results_previous_head_short12: "b7f89a7bc5c0"
contract_pin_short12: "3f1cefc573ac"
read_channel: "connector"
status: "READY_FOR_MAINLINE_REVIEW"
mainline_acceptance: "PENDING"
runtime_evidence: "NOT_RUN"
record_type: "bounded_order0_page_lifetime_narrative"
---

# CORE-USER-PAGE-REF-LIFETIME-18 · 普通order-0页的持有、归零与再利用

## 结论和适用范围

固定的`time @ a039d9803ade`具有“初始化页描述符→第一次入buddy空闲结构→分配出队→page/folio引用操作→条件性直接释放再入链”的源码链，但**引用归零并不等于物理页重新成为可分配对象**。普通`folio_put`递减到0后只会调用当前主体为空的`__folio_put`；`__free_pages`则根本不先递减_refcount，而是直接通往`__free_pages_ok`及伙伴空闲链（PL18-A67/A74–A77/A22–A28）。

本批只做order0的条件性静态分析。第17批已由C03按`PASS_PENDING_LOCAL`限定接收，须合读R17-S01/S02/S03；它关于权限增位、图边方向及普通deferred真实性的限定仍有效。不重做17的COW/Folio账，也不重做16的fork PTE。04号MR-02与05号后继回执已收窄“PageBuddy恒假”“全局永不合并”的绝对文字，不能把历史陈述当本轮事实。

## L01 · page、folio、union与初始投放

`struct page`是物理页的**描述符**，不是实际PAGE_SIZE数据。`lru`、`buddy_list`、`pcp_list`共用union位置，首字与`compound_head`表示重叠；`page_type`和`_mapcount`也处于同一个4字节union，而独立`_refcount`是另一原子字段（PL18-A01–A06）。`page_folio`会用`_compound_head`读取page字段，必须要求合法非NULL的order0描述符，不把无效页解释成有效folio（A59/A61/A62）。

实际首次初始化链中，`alloc_node_mem_map`把新取得的mem_map页描述符区域清零；`memmap_init`逐页执行`__init_single_page`，后者`init_page_count→set_page_count(...,1)`，而`page_mapcount_reset`仅为注释（PL18-A07–A15）。随后`mem_init→memblock_free_all`按筛选内存范围选择order，将块送`memblock_free_pages`：清Reserved、把块内页的_refcount设0，再经`__free_pages_ok`入池（A16–A24/A81/A82）。所以**1→0只对真实经过这些调用的页成立**。memblock选择的order可能大于0，本批只取其中满足order0前提的单页作代表，而非宣称每个PFN都按相同路径初始化。

空闲链成员身份和`PageBuddy`判定相互独立。定义`PAGE_TYPE_BASE=0xf0000000`和`PG_buddy=0x80`，`PageBuddy`要求与BASE一致；`__SetPageBuddy`却只清掉PG_buddy一位而未设置BASE（A34–A38）。对“mem_map原来整体为零、期间无其它写者设置BASE高位”的**特定页**，即使已插入free_area，其page_type仍可能为0、PageBuddy判假。这不能扩成整树所有页永远判假：其余初始化/复用/union别名状态未全量核过，且MR-02已经作出正式限定。

## L02 · 分配、出队、计数不是同一动作

通用`alloc_pages(gfp,order)`向`__myos_alloc_pages`传入原实参；COW17所用`folio_alloc_noprof`则忽略自己的gfp与order，固定`alloc_page(GFP_HIGHUSER_MOVABLE)`即order0后执行`page_folio`（A53–A62）。不能用包装的形参来推断这个COW入口的真实分配策略。底层若返回NULL，包装未先判断就转换page，可靠的OOM回退仍未证明；这与17原报告同一事项，按evidence_append引用而非新问题。

出队的真实顺序是`__rmqueue_smallest→get_page_from_free_area→list_header_remove_head`：后者已执行`list_del_init`并把header count减少一次；接下来`del_page_from_free_list`又调用`list_header_delete_node`，但此接口有`list_header_contains`门，只有节点**仍在当前链**才第二次删除/减count（A40–A47/A50/A51）。在链表本来一致、没有并发插回的代表场景中，第二次contains是假，**并不构成已证实的双删**。清Buddy标志和private归零仍是后续活动动作，不能因列表删除未执行就省略这些效果。

从更高阶块取得页时，`expand`把剩余的拆分块插入各自小阶空闲表；分配成功后，对目标块每一个page执行`atomic_inc`，order0恰好+1（A48–A52）。因此普遍关系是**n→n+1**，不是“分配总把_refcount设置成1”。唯有证明当前页首次投放时为0且期间没有其它操作，才可得到0→1。数据内容的`post_alloc_hook`仅在GFP含__GFP_ZERO时清零，与引用计数必须分列（A78）。

锁也只能限局部：`__free_pages_ok`有`spin_lock_irqsave(&zone->lock,...)`及对应unlock；所读分配函数没有zone.lock获取语句，而`zone_init_internals`内`spin_lock_init`是注释（A23/A24/A50/A51/A56/A80）。这还不证明全体调用者无外层同步，也不能预言多CPU竞态必然复现；作为需证据事项入库。

## L03 · 引用归零、直接free、伙伴再分配的区别

对合法持有的order0页，`get_page→page_folio→folio_get→folio_ref_inc→page_ref_inc`使_refcount增加1；`put_page→page_folio→folio_put`调用`atomic_dec_and_test`减少1，**仅新计数为0时**进入`__folio_put`（PL18-A63–A75）。`page_ref_dec`是只递减、不带归零回收钩子的另一接口。当前`__folio_put`没有活动的`__free_pages`调用，所以在“页原先已从free_area出队”的前提下，最后一次put并不会借这一链把页重新入链；不能从函数名或注释补充物理归还。

另一条**直接free**路径是`free_pages(addr,order)`在addr非0时调用`__free_pages`，后者无_refcount归零检查地进入`__free_pages_ok`；只要zone、page和order有效，free准备和加锁后`__myos_free_one_page`会将块投入指定free_area（A22–A28/A76/A77/A79）。这使“调用方若在仍有人持有时直接free”成为条件性重用风险，但没有实际调用者违反前提的证据，也没有运行观测到重复分配。反向也不能说引用为0就已加入空闲链。

伙伴合并检查先根据PFN异或找另一半，要求那个buddy的PageBuddy为真、阶数相同且zone相同（A29–A33）。命中时`__myos_free_one_page`升级order和合并页头，但**所读循环中没有先从原free_area摘除buddy节点**，结束时还添加合并页头（A26–A28/A83）。若PageBuddy等门真的满足、原buddy在列表中，这提供“多个空闲条目可能覆盖同一物理范围”的条件性风险；它与前述zeroed page_type导致PageBuddy为假的场景是**不同前提**，不得同时当作已经发生的实机事实。

## 代表对象/状态账

| 代表场景（都有有效页与无竞态前提） | _refcount | free_area状态 | 严格界限 |
|---|---|---|---|
| 新页初始化→首次入池 | 1→0 | 进入空闲链 | PageBuddy真假另由page_type决定 |
| 首次池中页出队 | 0→1 | 移除头节点 | 第二次删除帮助接口不一定实施删除 |
| 有效get、put非末次 | n→n+1→n | 不因此变化 | n必须是合法先前持有量 |
| 最后一次put | 1→0 | 不经当前__folio_put入链 | 不能认证物理回收 |
| 直接free且引用仍为正（条件性错误调用） | n仍正 | 可能进入free_area | 不断言存在实际违约调用者 |
| 空闲链资源耗尽 | 未取得新page | 无出队 | 内层NULL，不认证folio包装错误出口 |
| 找到符合条件的同阶buddy | 所读合并不改引用 | 合并页头入链，原buddy未见摘除 | 不认证真实重叠分配 |

`facts-and-boundaries.yaml`另列13场景，分别记录_refcount、mapcount/page_type同union、空闲链、锁、实参有效性及实际返回，另有16项effects。两个有限算术例子：① 初投页`1(初始)→0(入池)→1(出队)→2(get)→1(put)→0(put)`，最终仍不能借当前folio_put进入free_area；② 17成功COW的旧folio若原计数为n，临时get一次与尾部两次put是`n→n+1→n→n−1`，但历史fork映射引用没有闭合，**不能推断实际释放、引用安全或下溢**。17的复核失败路径旧folio临时持有可按条件作`n→n+1→n`，新分配folio是不同对象，不能混算。

## 消费限制和退出

`facts-and-boundaries.yaml`的call只画直接调用或宏绑定，间接的`get_page→page_folio→folio_get`写作call_path并指明中间层；data按R17-S02使用消费方→提供方，修改字段/链表另列effect；DOT对应同ID。新增条件性风险、历史MR-02及17 F05/F09的追加证据在deferred中分开，旧件不变。

本批没有进行编译、C/ASM运行、QEMU、真实内存或硬件验证，也没有穷尽所有page_type写入者、锁上下文和全树free调用者。静态空后端、条件性合并与初值场景不能转化为“全局永不释放”“全局永不合并”“必然重复分配”“实际下溢”“已测得泄漏率”等结论。完成交付后本执行者停写，交由C03独立验收。
