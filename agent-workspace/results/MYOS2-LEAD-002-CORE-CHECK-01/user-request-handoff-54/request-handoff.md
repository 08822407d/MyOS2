---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-REQUEST-HANDOFF-54
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
produced_by: unknown
produced_by_reason: "Owner选定模型不等于可认证后台作者；本对话无法读取后台身份"
model_per_owner: GPT-6
effort_per_owner: Extra
read_channel: GitHub_connector
date: 2026-11-07
source_pin_short12: a039d9803ade
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
record_type: bounded_request_handoff_A_B_C_report
---
# 第54批：设备请求结果与活动指针的有界静态分析
## H00｜第53批三个证据点的外置文档补充
R53-01仍为OPEN，53仍为RETURN；本批只申请R53-01_DOCUMENTATION_CLOSURE_REQUESTED，不自签CLOSED/PASS。新RH54-R5301取devtmpfs.c 109–113、RH54-R5302取kthread.c 242–246，各为5行，**保留源末尾空行**。固定Git blob证实：两条原CO53-A03/B03的109–112及242–245四行其实已经逐字连续，真实空行是kzalloc下一行113、kmalloc下一行246；原回执中空行位于声明之间的叙述与本固定blob不一致，不能伪造其示例。结构锚点CO53-C03确实含定义外注释，替换为struct completion的19–22行（RH54-R5303），17–18行降CONTEXT。消费者映射分别A.heap_origin、B.origin、C.count_distinction，详facts/证据/MANIFEST/本评论；旧79/79仍历史作者申报，另外76条未重验。
R52-01由C04已CLOSED_DOCUMENTATION_ONLY不重开，CA-03/CA-04规范状态均SUPPORTED_IN_SCOPE，UV-01保留deferred_owner_not_ready；R53-S02中kthread成功通知后的调度与失败通知后的返回不混为同一流程，原53 DOT不覆盖，54图只画54。

## H01｜A device_add的devtmpfs返回值是否上传
device_add初始化error=-EINVAL。仅当dev及parent有效时讨论后继：若dev.init_name非空，复制其指针给dev.kobj.name并清init_name；然后做parent=dev.parent、`kobj=&parent->kobj`，**在IS_ERR(kobj)前已经要求parent为有效对象**。因此myos_device_create即使kzalloc成功，当前所读写入只有devt/class/kobj.name，没有给zeroed dev.parent赋值，不能越过早期无效对象门直接声称真正调用到devtmpfs。
在另有合法parent的条件模型中，MAJOR(devt)非零时device_add实际调用devtmpfs_create_node(dev)，但不保存/检查该int值，仍在函数末返回初始error=-EINVAL。MAJOR为零时无节点创建仍返回- EINVAL（固定源未有活动成功赋值）。不能把错误码归因于devtmpfs的返回，更不能由函数名或注释推断设备创建成功。myos_device_create把device_add的int放入retval，非零goto error并ERR_PTR(retval)返回，其put_device是注释，不能认证已经安全释放设备。device_del里的devtmpfs_delete_node整段是注释；不推出全仓无其他删除。
固定源码直接调用者tty_init与chr_dev_init在cdev_add正常返回0后调用myos_device_create，但局部表达式没消费返回指针；这不证明这些函数已经实际运行。dev_name取init_name或kobj.name，内层devtmpfs_create_node还有req分配、name取得等CO53前提。A completed_in_scope；RH54-F01是有有效parent条件的结果传播候选，不是实机故障结论。

## H02｜B IDE队列、活动指针与完成值
blkbuf_node_s含req_list、指向栈completion的done、task、unsigned int count、cmd/LBA/buffer及end_handler/DevSpecParams。**IDEreq_lhdr.count（排队项数）、node.count（传输计数）、completion.done（通知数）不能混同。** ATA_disk_transfer只收READ/WRITE/INFO，建立每次栈done后在req_lock内排队并唤醒daemon。daemon在队列count非零且req_in_using为NULL的锁内条件下list_header_remove_tail，将node写到全局req_in_using，随后解锁调用IDE_cmd_out并丢弃其返回1。该活动指针在IRQ入口被直接读出，所读ATA_disk_handler没有持同一个req_lock保护快照。
IRQ在node非NULL/有效前提下先调用end_handler，之后仅当node.count==0调用end_request；若req_in_using为NULL，首个node->end_handler已失去有效对象前提，后续中断/完成不可推。IDE_read_handler在DISK_STATUS_ERROR时只打印日志而不insw，但仍无条件node.count--并可能推进buffer；IDE_write_handler也记录错误后继续count--。IDE_other_handler没有count递减。对软件计数1的读请求，即使ERROR日志被记录，计数仍可变0并请求complete，但此事不证明硬件读成功。
end_request对NULL也只是打印，随后继续complete(node->done)，没有有效防护；正常路径在complete之后才持req_lock把req_in_using清NULL。caller若wait正常返回，会释放Params和node并return -ENOERR；ENOERR=0，故软件int结果为0，**不是实际I/O状态经IRQ传入**。当第二请求已在队列而第一活动指针不为NULL时，daemon不能在同一条件门下立刻激活第二个；第一清指针后下一轮才可能继续。没有可证公平/单核IRQ串行/全部旧指针回收或真实进展。B completed_in_scope，保留IRG/指针生命期和内存序停止线。

## H03｜C NVMe与XHCI的非等价完成条件
NVMe_transfer仅READ/WRITE进入排队，NVMe_make_request用kzalloc(...,0)给node分配，在当前完整体不另分配DevSpecParams；分派读写end_handler并存node.count。NVMe_cmd_out写CID/NSID/PRP与SQ门铃，cmd_out返回值由daemon丢弃。NVMe_read/write_handler只是应答CQ头索引，没有在当前体读取完成状态码或递减node.count；NVMe_IO_handler直接调node.end_handler并NVMe_end_request，**不采用IDE的node.count==0门**。后者complete再清活动指针。caller正常wait后释放node并return -ENOERR=0。CQ状态/CID与请求一一对应是否真实成立完全未证，不能以通知宣称NVMe成功。
XHCI_transfer没有IDE/NVMe那样的cmd白名单，先XHCI_make_request，分配node和XHCI_Params_s，保存slot/context并从Slot_Params索引得到EP_Param；选定active switch为default赋other_handler。XHCI_cmd_out针对Slot_ID=0、DevCtx_Idx非零有无更新语句的while门，因此这种稳定局部输入下不能保证进入doorbell；NOP分支构造TRB并写DoorBell不等于控制器完成。XHCI IRQ先读活动node及Params、事件TRB，并在命令TRB地址与EP ring dequeue指针不匹配时停在while门；匹配则保存EvtTRB并调node.end_handler/结束函数。
XHCI_end_request在complete前写IMAN/ERDP，complete之后锁下清req_in_using；外层XHCI_handler在end_request返回后还调用AdvanceRingDeqPtr与ToggleCycleBit，使用事前缓存的EP_Param指针。这是与IDE/NVMe的实质区别。不能把XHCIparam释放直接等同EP_Param释放（其来源可能是另一Slot对象），也不能凭该指针在源码中存在证明寿命安全。XHCI_transfer正常wait后释放Params/node并固定return -ENOERR=0，却未把EvtTRB/设备错误转换给调用方。
三类设备各有最低正常条件序列（合法node/Params/IRQ、相应通知、正常wait返回）和无法推出硬件成功的反例（IDE错误日志仍计数归零、NVMe未核CQ状态、XHCI无有效Slot/无匹配事件）。请求回调、软件通知、结果数值和全部旧引用消失须分开。C completed_in_scope，核心结论只限命名函数，不扩全磁盘/USB正确性。

## H04｜来源、证据层级和未执行
正文/facts/evidence按H00与A/B/C分栏，图中源码顺序、跨上下文条件虚线、不可证STOP分开。两项新RH54-F为有界源码候选，CA-03/CA-04/CO53旧身份/状态不自动升级；真机是否死锁、I/O失败、IRQ重入、资源泄漏或释放后读未得到证明。本批不运行仓库C/ASM/脚本、预处理/编译/链接、旧夹具、QEMU/硬件，也不全审GPIO/PIO/MMIO设备规范、完整本地P9或Graphviz；由C04独立裁定53文档闭合与54接收。
