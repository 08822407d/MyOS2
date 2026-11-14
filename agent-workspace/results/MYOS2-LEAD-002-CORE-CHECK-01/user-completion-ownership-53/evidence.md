---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-COMPLETION-OWNERSHIP-53
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
produced_by: unknown
produced_by_reason: "无法直接读取实际后台作者；Owner选定GPT-6/Extra不等于已认证后台身份"
model_per_owner: GPT-6
effort_per_owner: Extra
read_channel: GitHub_connector
date: 2026-11-07
base_snapshot: "time@a039d9803ade"
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
record_type: evidence_and_R52_correction
---
# CO53：R52-01校正短引文与A/B/C独立取证
引文均从固定time@a039d9803ade的单一定义体/单宏/单结构连续1–5个原字节行截取。R52六条只修旧C14/C16定义范围，53新证按A/B/C分开；未重验旧52其余65条。extern/注释不作为活动函数证据。各条definition_range、真实符号、claim、consumer保留如下。
## CO53-R5201 · kthread定义头及create指针
[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
int kthread(void *_create)
{
	// static const struct sched_param param = { .sched_priority = 0 };
	/* Copy data: it's on kthread's stack */
	kthd_create_info_s *create = _create;
```
quote_metadata: {"evidence_id":"CO53-R5201","actual_definition_kind":"function","enclosing_symbol":"kthread","definition_range":"164-215","quote_range":"164-168","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["C14.function_start"]}

## CO53-R5202 · 原WW52-C14通知和后继schedule片段仍在kthread函数
[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	preempt_disable();
	complete(done);
	schedule_preempt_disabled();
```
quote_metadata: {"evidence_id":"CO53-R5202","actual_definition_kind":"function","enclosing_symbol":"kthread","definition_range":"164-215","quote_range":"203-205","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["C14.original_quote"]}

## CO53-R5203 · kthread函数在215行闭合
[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	}
	kthread_exit(ret);
}
```
quote_metadata: {"evidence_id":"CO53-R5203","actual_definition_kind":"function","enclosing_symbol":"kthread","definition_range":"164-215","quote_range":"213-215","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["C14.function_end"]}

## CO53-R5204 · kernel_init真实函数头
[VERIFIED mykernel/init/main.c::kernel_init]
```c
int kernel_init(void *unused)
{
	int ret;
```
quote_metadata: {"evidence_id":"CO53-R5204","actual_definition_kind":"function","enclosing_symbol":"kernel_init","definition_range":"270-309","quote_range":"270-272","quote_supports_claim":true,"source_blob_short12":"06a900f8d509","consumer_positions":["C16.function_start"]}

## CO53-R5205 · 原WW52-C16等待片段仍在kernel_init函数
[VERIFIED mykernel/init/main.c::kernel_init]
```c
	 * Wait until kthreadd is all set-up.
	 */
	wait_for_completion(&kthreadd_done);
```
quote_metadata: {"evidence_id":"CO53-R5205","actual_definition_kind":"function","enclosing_symbol":"kernel_init","definition_range":"270-309","quote_range":"275-277","quote_supports_claim":true,"source_blob_short12":"06a900f8d509","consumer_positions":["C16.original_quote"]}

## CO53-R5206 · kernel_init真实函数在309行闭合
[VERIFIED mykernel/init/main.c::kernel_init]
```c
	while (true);
	
	return 1;
}
```
quote_metadata: {"evidence_id":"CO53-R5206","actual_definition_kind":"function","enclosing_symbol":"kernel_init","definition_range":"270-309","quote_range":"306-309","quote_supports_claim":true,"source_blob_short12":"06a900f8d509","consumer_positions":["C16.function_end"]}

## CO53-A01 · req包括next、内嵌done及err
[VERIFIED mykernel/drivers/base/devtmpfs.c::req_s]
```c
typedef struct req {
	req_s			*next;
	completion_s	done;
	int				err;
```
quote_metadata: {"evidence_id":"CO53-A01","actual_definition_kind":"typedef_struct","enclosing_symbol":"req_s","definition_range":"47-56","quote_range":"47-50","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.objects"]}

## CO53-A02 · req包括name、mode、uid、gid及dev
[VERIFIED mykernel/drivers/base/devtmpfs.c::req_s]
```c
	const char		*name;
	umode_t			mode;	/* 0 => delete */
	kuid_t			uid;
	kgid_t			gid;
	device_s		*dev;
```
quote_metadata: {"evidence_id":"CO53-A02","actual_definition_kind":"typedef_struct","enclosing_symbol":"req_s","definition_range":"47-56","quote_range":"51-55","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.objects"]}

## CO53-A03 · create在kzalloc后保存堆req指针
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_create_node]
```c
int devtmpfs_create_node(device_s *dev)
{
	const char *tmp = NULL;
	req_s *req = kzalloc(sizeof(req_s), GFP_KERNEL);
```
quote_metadata: {"evidence_id":"CO53-A03","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_create_node","definition_range":"109-131","quote_range":"109-112","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.heap_origin"]}

## CO53-A04 · create立刻写mode/uid/gid/name
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_create_node]
```c
	req->mode = 0;
	req->uid = GLOBAL_ROOT_UID;
	req->gid = GLOBAL_ROOT_GID;
	req->name = dev_name(dev);
```
quote_metadata: {"evidence_id":"CO53-A04","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_create_node","definition_range":"109-131","quote_range":"114-117","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.first_dereference"]}

## CO53-A05 · create遇无name直接返回错误
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_create_node]
```c
	if (!req->name)
		return -ENOMEM;
```
quote_metadata: {"evidence_id":"CO53-A05","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_create_node","definition_range":"109-131","quote_range":"118-119","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.name_failure"]}

## CO53-A06 · create设置非零mode与block分支
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_create_node]
```c
	if (req->mode == 0)
		req->mode = 0600;
	if (is_blockdev(dev))
		req->mode |= S_IFBLK;
	else
```
quote_metadata: {"evidence_id":"CO53-A06","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_create_node","definition_range":"109-131","quote_range":"121-125","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.mode"]}

## CO53-A07 · create赋dev并传同一req指针
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_create_node]
```c
	req->dev = dev;

	return devtmpfs_submit_req(req, tmp);
```
quote_metadata: {"evidence_id":"CO53-A07","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_create_node","definition_range":"109-131","quote_range":"128-130","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.submit"]}

## CO53-A08 · delete声明未初始化的自动局部req
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_delete_node]
```c
int devtmpfs_delete_node(device_s *dev)
{
	const char *tmp = NULL;
	struct req req;
```
quote_metadata: {"evidence_id":"CO53-A08","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_delete_node","definition_range":"133-146","quote_range":"133-136","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.stack_origin"]}

## CO53-A09 · delete的name赋值为注释、实际只开始写mode
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_delete_node]
```c
	// req.name = device_get_devnode(dev, NULL, NULL, NULL, &tmp);
	// if (!req.name)
	// 	return -ENOMEM;

	req.mode = 0;
```
quote_metadata: {"evidence_id":"CO53-A09","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_delete_node","definition_range":"133-146","quote_range":"138-142","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.uninitialized"]}

## CO53-A10 · delete只写mode/dev，发布&req
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_delete_node]
```c
	req.mode = 0;
	req.dev = dev;

	return devtmpfs_submit_req(&req, tmp);
```
quote_metadata: {"evidence_id":"CO53-A10","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_delete_node","definition_range":"133-146","quote_range":"142-145","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.publish"]}

## CO53-A11 · submit仅初始化内嵌done
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_submit_req]
```c
static int devtmpfs_submit_req(req_s *req, const char *tmp)
{
	init_completion(&req->done);
```
quote_metadata: {"evidence_id":"CO53-A11","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_submit_req","definition_range":"92-107","quote_range":"92-94","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.done_init"]}

## CO53-A12 · submit在req_lock内发布next和requests
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_submit_req]
```c
	spin_lock(&req_lock);
	req->next = requests;
	requests = req;
	spin_unlock(&req_lock);
```
quote_metadata: {"evidence_id":"CO53-A12","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_submit_req","definition_range":"92-107","quote_range":"96-99","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.queue_publish"]}

## CO53-A13 · submit请求wake后等待req.done
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_submit_req]
```c
	wake_up_process(thread);
	wait_for_completion(&req->done);
```
quote_metadata: {"evidence_id":"CO53-A13","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_submit_req","definition_range":"92-107","quote_range":"101-102","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.wait"]}

## CO53-A14 · wait后kfree(tmp)然后读req.err
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_submit_req]
```c
	kfree((void *)tmp);

	return req->err;
```
quote_metadata: {"evidence_id":"CO53-A14","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_submit_req","definition_range":"92-107","quote_range":"104-106","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.reader"]}

## CO53-A15 · worker持锁取批并清requests头
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_work_loop]
```c
		spin_lock(&req_lock);
		while (requests) {
			req_s *req = requests;
			requests = NULL;
			spin_unlock(&req_lock);
```
quote_metadata: {"evidence_id":"CO53-A15","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_work_loop","definition_range":"355-377","quote_range":"358-362","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.worker_take"]}

## CO53-A16 · worker先保存next，随后读req字段调用handle
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_work_loop]
```c
			while (req) {
				req_s *next = req->next;
				req->err = handle(req->name, req->mode,
						req->uid, req->gid, req->dev);
```
quote_metadata: {"evidence_id":"CO53-A16","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_work_loop","definition_range":"355-377","quote_range":"363-366","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.first_invalid_delete"]}

## CO53-A17 · worker写err、complete、kfree(req)并推进next
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_work_loop]
```c
				req->err = handle(req->name, req->mode,
						req->uid, req->gid, req->dev);
				complete(&req->done);
				kfree(req);
				req = next;
```
quote_metadata: {"evidence_id":"CO53-A17","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_work_loop","definition_range":"355-377","quote_range":"365-369","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.notify_free"]}

## CO53-A18 · handle按mode分派create/remove
[VERIFIED mykernel/drivers/base/devtmpfs.c::handle]
```c
	if (mode)
		return handle_create(name, mode, uid, gid, dev);
	else
		return handle_remove(name, dev);
```
quote_metadata: {"evidence_id":"CO53-A18","actual_definition_kind":"function","enclosing_symbol":"handle","definition_range":"346-353","quote_range":"349-352","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.dispatch"]}

## CO53-A19 · handle_remove是int返回型函数
[VERIFIED mykernel/drivers/base/devtmpfs.c::handle_remove]
```c
static int handle_remove(const char *nodename, device_s *dev)
{
	// struct path parent;
```
quote_metadata: {"evidence_id":"CO53-A19","actual_definition_kind":"function","enclosing_symbol":"handle_remove","definition_range":"297-342","quote_range":"297-299","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.remove_contract"]}

## CO53-A20 · handle_remove尾部return仅注释而函数结束
[VERIFIED mykernel/drivers/base/devtmpfs.c::handle_remove]
```c
	// if (deleted && strchr(nodename, '/'))
	// 	delete_path(nodename);
	// return err;
}
```
quote_metadata: {"evidence_id":"CO53-A20","actual_definition_kind":"function","enclosing_symbol":"handle_remove","definition_range":"297-342","quote_range":"339-342","quote_supports_claim":true,"source_blob_short12":"633678e473c0","consumer_positions":["A.remove_missing_return"]}

## CO53-A21 · kzalloc调用kmalloc且附加零填充标志
[VERIFIED mykernel/mm/kmalloc/slub.h::kzalloc]
```c
		PREFIX_STATIC_INLINE
		__alloc_size(1) void
		*kzalloc(size_t size, gfp_t flags) {
			return kmalloc(size, flags | __GFP_ZERO);
		}
```
quote_metadata: {"evidence_id":"CO53-A21","actual_definition_kind":"function","enclosing_symbol":"kzalloc","definition_range":"168-172","quote_range":"168-172","quote_supports_claim":true,"source_blob_short12":"b07c81bfc978","consumer_positions":["A.allocator"]}

## CO53-A22 · kfree空值直接返回
[VERIFIED mykernel/mm/kmalloc/slub.h::kfree]
```c
			page_s *slab;
			kmem_cache_s *s;

			if (unlikely(object == NULL))
				return;
```
quote_metadata: {"evidence_id":"CO53-A22","actual_definition_kind":"function","enclosing_symbol":"kfree","definition_range":"181-196","quote_range":"185-189","quote_supports_claim":true,"source_blob_short12":"b07c81bfc978","consumer_positions":["A.tmp_null"]}

## CO53-A23 · kfree非空执行folio查找及分配器释放动作
[VERIFIED mykernel/mm/kmalloc/slub.h::kfree]
```c
			folio = virt_to_folio(object);
			if (unlikely(!folio_test_slab(folio)))
				free_large_kmalloc(folio, (void *)object);
			else
				kmem_cache_free(folio_slab(folio)->slab_cache, (void *)object);
```
quote_metadata: {"evidence_id":"CO53-A23","actual_definition_kind":"function","enclosing_symbol":"kfree","definition_range":"181-196","quote_range":"191-195","quote_supports_claim":true,"source_blob_short12":"b07c81bfc978","consumer_positions":["A.free_request"]}

## CO53-B01 · create包含full_name、threadfn及data字段
[VERIFIED mykernel/sched/forkexec/kthread.c::kthd_create_info_s]
```c
typedef struct kthread_create_info {
	/* Information passed to kthread() from kthreadd. */
	char			*full_name;
	int				(*threadfn)(void *data);
	void			*data;
```
quote_metadata: {"evidence_id":"CO53-B01","actual_definition_kind":"typedef_struct","enclosing_symbol":"kthd_create_info_s","definition_range":"20-32","quote_range":"20-24","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.object"]}

## CO53-B02 · create包含result、done及list字段
[VERIFIED mykernel/sched/forkexec/kthread.c::kthd_create_info_s]
```c
	/* Result passed back to kthread_create() from kthreadd. */
	task_s			*result;
	completion_s	*done;

	List_s			list;
```
quote_metadata: {"evidence_id":"CO53-B02","actual_definition_kind":"typedef_struct","enclosing_symbol":"kthd_create_info_s","definition_range":"20-32","quote_range":"27-31","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.object"]}

## CO53-B03 · 创建方栈done与独立kmalloc create
[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
	DECLARE_COMPLETION_ONSTACK(done);
	task_s *task;
	kthd_create_info_s *create =
		kmalloc(sizeof(kthd_create_info_s), GFP_KERNEL);
```
quote_metadata: {"evidence_id":"CO53-B03","actual_definition_kind":"function","enclosing_symbol":"__kthread_create_on_node","definition_range":"239-288","quote_range":"242-245","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.origin"]}

## CO53-B04 · 分配失败及正常路径写threadfn/data/node
[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
	if (!create)
		return ERR_PTR(-ENOMEM);
	create->threadfn	= threadfn;
	create->data		= data;
	create->node		= node;
```
quote_metadata: {"evidence_id":"CO53-B04","actual_definition_kind":"function","enclosing_symbol":"__kthread_create_on_node","definition_range":"239-288","quote_range":"247-251","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.failure_alloc"]}

## CO53-B05 · 创建者发布done地址和另分配的full_name
[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
	create->node		= node;
	create->done		= &done;
	create->full_name	= kvasprintf(GFP_KERNEL, namefmt, args);
	if (!create->full_name) {
```
quote_metadata: {"evidence_id":"CO53-B05","actual_definition_kind":"function","enclosing_symbol":"__kthread_create_on_node","definition_range":"239-288","quote_range":"251-254","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.name"]}

## CO53-B06 · full_name失败时goto free_create
[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
	create->full_name	= kvasprintf(GFP_KERNEL, namefmt, args);
	if (!create->full_name) {
		task = ERR_PTR(-ENOMEM);
		goto free_create;
```
quote_metadata: {"evidence_id":"CO53-B06","actual_definition_kind":"function","enclosing_symbol":"__kthread_create_on_node","definition_range":"239-288","quote_range":"253-256","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.name_failure"]}

## CO53-B07 · 创建方在锁下将create放入kthread队列
[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
	INIT_LIST_S(&create->list);

	spin_lock(&kthread_create_lock);
	list_header_add_to_tail(&kthread_create_list, &create->list);
	spin_unlock(&kthread_create_lock);
```
quote_metadata: {"evidence_id":"CO53-B07","actual_definition_kind":"function","enclosing_symbol":"__kthread_create_on_node","definition_range":"239-288","quote_range":"258-262","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.queue"]}

## CO53-B08 · 创建方真实wait_for_completion调用
[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
		 * kthreadd (or new kernel thread) will call complete()
		 * shortly.
		 */
		wait_for_completion(&done);
```
quote_metadata: {"evidence_id":"CO53-B08","actual_definition_kind":"function","enclosing_symbol":"__kthread_create_on_node","definition_range":"239-288","quote_range":"279-282","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.wait"]}

## CO53-B09 · wait后读取result并请求释放create
[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
		wait_for_completion(&done);
	// }
	task = create->result;
free_create:
	kfree(create);
```
quote_metadata: {"evidence_id":"CO53-B09","actual_definition_kind":"function","enclosing_symbol":"__kthread_create_on_node","definition_range":"239-288","quote_range":"282-286","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.reader_free"]}

## CO53-B10 · kthreadd取尾节点并回推create
[VERIFIED mykernel/sched/forkexec/kthread.c::kthreadd]
```c
			kthd_create_info_s *create;

			List_s *lp = list_header_remove_tail(&kthread_create_list);
			create = container_of(lp, kthd_create_info_s, list);
			spin_unlock(&kthread_create_lock);
```
quote_metadata: {"evidence_id":"CO53-B10","actual_definition_kind":"function","enclosing_symbol":"kthreadd","definition_range":"328-363","quote_range":"349-353","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.queue_consumer"]}

## CO53-B11 · kthreadd调用create_kthread后继续用锁
[VERIFIED mykernel/sched/forkexec/kthread.c::kthreadd]
```c
			create_kthread(create);

			spin_lock(&kthread_create_lock);
		}
```
quote_metadata: {"evidence_id":"CO53-B11","actual_definition_kind":"function","enclosing_symbol":"kthreadd","definition_range":"328-363","quote_range":"355-358","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.queue_consumer_last"]}

## CO53-B12 · create_kthread调用kernel_thread并检查pid负值
[VERIFIED mykernel/sched/forkexec/kthread.c::create_kthread]
```c
	/* We want our own signal handler (we take no signals by default). */
	pid = kernel_thread(kthread, create, create->full_name,
			CLONE_FS | CLONE_FILES | SIGCHLD);
	if (pid < 0) {
		/* If user was SIGKILLed, I release the structure. */
```
quote_metadata: {"evidence_id":"CO53-B12","actual_definition_kind":"function","enclosing_symbol":"create_kthread","definition_range":"217-236","quote_range":"221-225","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.producer_failure"]}

## CO53-B13 · 失败生产者xchg done、释放名字及条件释放create
[VERIFIED mykernel/sched/forkexec/kthread.c::create_kthread]
```c
		completion_s *done = xchg(&create->done, NULL);

		kfree(create->full_name);
		if (!done) {
			kfree(create);
```
quote_metadata: {"evidence_id":"CO53-B13","actual_definition_kind":"function","enclosing_symbol":"create_kthread","definition_range":"217-236","quote_range":"226-230","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.failure_release"]}

## CO53-B14 · 失败有done时写ERR_PTR结果并通知
[VERIFIED mykernel/sched/forkexec/kthread.c::create_kthread]
```c
			return;
		}
		create->result = ERR_PTR(pid);
		complete(done);
	}
```
quote_metadata: {"evidence_id":"CO53-B14","actual_definition_kind":"function","enclosing_symbol":"create_kthread","definition_range":"217-236","quote_range":"231-235","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.failure_notify"]}

## CO53-B15 · 新任务早期从create取得threadfn/data
[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	kthd_create_info_s *create = _create;
	int (*threadfn)(void *data) = create->threadfn;
	void *data = create->data;
```
quote_metadata: {"evidence_id":"CO53-B15","actual_definition_kind":"function","enclosing_symbol":"kthread","definition_range":"164-215","quote_range":"168-170","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.newtask_read"]}

## CO53-B16 · 新任务xchg done以及NULL条件释放
[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	done = xchg(&create->done, NULL);
	if (!done) {
		kfree(create->full_name);
		kfree(create);
		kthread_exit(-EINTR);
```
quote_metadata: {"evidence_id":"CO53-B16","actual_definition_kind":"function","enclosing_symbol":"kthread","definition_range":"164-215","quote_range":"178-182","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.exchange"]}

## CO53-B17 · 正常路径转交full_name指针给self
[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	self->full_name = create->full_name;
	self->threadfn = threadfn;
	self->data = data;
```
quote_metadata: {"evidence_id":"CO53-B17","actual_definition_kind":"function","enclosing_symbol":"kthread","definition_range":"164-215","quote_range":"185-187","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.fullname_transfer"]}

## CO53-B18 · 生产者通知前写create.result=current
[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	__set_current_state(TASK_UNINTERRUPTIBLE);
	create->result = current;
```
quote_metadata: {"evidence_id":"CO53-B18","actual_definition_kind":"function","enclosing_symbol":"kthread","definition_range":"164-215","quote_range":"197-198","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.success_result"]}

## CO53-B19 · 正常生产者complete后仍调用schedule_preempt_disabled
[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	preempt_disable();
	complete(done);
	schedule_preempt_disabled();
```
quote_metadata: {"evidence_id":"CO53-B19","actual_definition_kind":"function","enclosing_symbol":"kthread","definition_range":"164-215","quote_range":"203-205","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.producer_after_notify"]}

## CO53-B20 · 后续kthread结构释放时kfree(full_name)及self
[VERIFIED mykernel/sched/forkexec/kthread.c::free_kthread_struct]
```c
	k->worker_private = NULL;
	kfree(kthread->full_name);
	kfree(kthread);
```
quote_metadata: {"evidence_id":"CO53-B20","actual_definition_kind":"function","enclosing_symbol":"free_kthread_struct","definition_range":"107-121","quote_range":"118-120","quote_supports_claim":true,"source_blob_short12":"4782afc6b3f2","consumer_positions":["B.fullname_later_free"]}

## CO53-B21 · xchg宏路由到arch_xchg
[VERIFIED mykernel/lock_IPC/atomic/atomic_macro.h::xchg]
```c
	#define xchg(ptr, ...) ({						\
				typeof(ptr) __ai_ptr = (ptr);		\
				arch_xchg(__ai_ptr, __VA_ARGS__);	\
			})
```
quote_metadata: {"evidence_id":"CO53-B21","actual_definition_kind":"macro","enclosing_symbol":"xchg","definition_range":"519-522","quote_range":"519-522","quote_supports_claim":true,"source_blob_short12":"d170e8799cf9","consumer_positions":["B.xchg"]}

## CO53-B22 · arch_xchg映射__xchg_op
[VERIFIED mykernel/arch/x86_64/insns/cmpxchg_api.h::arch_xchg]
```c
	#define arch_xchg(ptr, v)	__xchg_op((ptr), (v), xchg, "")
```
quote_metadata: {"evidence_id":"CO53-B22","actual_definition_kind":"macro","enclosing_symbol":"arch_xchg","definition_range":"78-78","quote_range":"78-78","quote_supports_claim":true,"source_blob_short12":"6ac3b567bf6e","consumer_positions":["B.xchg"]}

## CO53-B23 · __xchg_op宏初始化ret并按数据宽度执行exchange
[VERIFIED mykernel/arch/x86_64/insns/cmpxchg_api.h::__xchg_op]
```c
	#define __xchg_op(ptr, arg, op, lock) ({							\
				__typeof__ (*(ptr)) __ret = (arg);						\
				switch (sizeof(*(ptr))) {								\
					case __X86_CASE_B:									\
```
quote_metadata: {"evidence_id":"CO53-B23","actual_definition_kind":"macro","enclosing_symbol":"__xchg_op","definition_range":"39-70","quote_range":"39-42","quote_supports_claim":true,"source_blob_short12":"6ac3b567bf6e","consumer_positions":["B.xchg_mechanism"]}

## CO53-B24 · 交换宏返回__ret
[VERIFIED mykernel/arch/x86_64/insns/cmpxchg_api.h::__xchg_op]
```c
					default:											\
						__ ## op ## _wrong_size();						\
				}														\
				__ret;													\
			})
```
quote_metadata: {"evidence_id":"CO53-B24","actual_definition_kind":"macro","enclosing_symbol":"__xchg_op","definition_range":"39-70","quote_range":"66-70","quote_supports_claim":true,"source_blob_short12":"6ac3b567bf6e","consumer_positions":["B.xchg_old_pointer"]}

## CO53-C01 · 静态单实例getcode_done的DECLARE_COMPLETION用点
[VERIFIED mykernel/arch/x86_64/myos/keyboard.c::getcode_done]
```c
DECLARE_COMPLETION(getcode_done);
```
quote_metadata: {"evidence_id":"CO53-C01","actual_definition_kind":"file_scope_object","enclosing_symbol":"getcode_done","definition_range":"18-18","quote_range":"18-18","quote_supports_claim":true,"source_blob_short12":"a40b5c7e56b3","consumer_positions":["C.static"]}

## CO53-C02 · DECLARE_COMPLETION宏直接构造completion_s对象
[VERIFIED mykernel/kactive/completion/completion_macro.h::DECLARE_COMPLETION]
```c
	#define DECLARE_COMPLETION(work)	\
				completion_s work = COMPLETION_INITIALIZER(work)
```
quote_metadata: {"evidence_id":"CO53-C02","actual_definition_kind":"macro","enclosing_symbol":"DECLARE_COMPLETION","definition_range":"15-16","quote_range":"15-16","quote_supports_claim":true,"source_blob_short12":"093a687a05d4","consumer_positions":["C.static_init"]}

## CO53-C03 · completion包含done及独立swait头
[VERIFIED mykernel/kactive/completion/completion_types.h::struct completion]
```c
	 * DECLARE_COMPLETION_ONSTACK().
	 */
	struct completion {
		uint			done;
		swqueue_hdr_s	wait;
```
quote_metadata: {"evidence_id":"CO53-C03","actual_definition_kind":"struct","enclosing_symbol":"struct completion","definition_range":"17-21","quote_range":"17-21","quote_supports_claim":true,"source_blob_short12":"9512b9489959","consumer_positions":["C.count_distinction"]}

## CO53-C04 · kbd缓冲拥有独立int count和buf
[VERIFIED mykernel/include/obsolete/ktypes.h::kbd_inbuf_s]
```c
		unsigned char * p_head;
		unsigned char * p_tail;
		int count;
		unsigned char buf[KB_BUF_SIZE];
	} kbd_inbuf_s;
```
quote_metadata: {"evidence_id":"CO53-C04","actual_definition_kind":"typedef_struct","enclosing_symbol":"kbd_inbuf_s","definition_range":"14-20","quote_range":"16-20","quote_supports_claim":true,"source_blob_short12":"bb240bfc1918","consumer_positions":["C.count_distinction"]}

## CO53-C05 · keyboard初始化p_kb buffer count=0
[VERIFIED mykernel/arch/x86_64/myos/keyboard.c::init_keyboard]
```c
	p_kb = (kbd_inbuf_s *)kzalloc(sizeof(kbd_inbuf_s), GFP_KERNEL);
	p_kb->p_head = p_kb->buf;
	p_kb->p_tail = p_kb->buf;
	p_kb->count  = 0;
```
quote_metadata: {"evidence_id":"CO53-C05","actual_definition_kind":"function","enclosing_symbol":"init_keyboard","definition_range":"41-87","quote_range":"46-49","quote_supports_claim":true,"source_blob_short12":"a40b5c7e56b3","consumer_positions":["C.buffer_init"]}

## CO53-C06 · 键盘handler写buf并buffer.count++
[VERIFIED mykernel/arch/x86_64/myos/keyboard.c::keyboard_handler]
```c
	if(p_kb->p_head == p_kb->buf + KB_BUF_SIZE)
		p_kb->p_head = p_kb->buf;
	*p_kb->p_head = x;
	p_kb->p_head ++;	
	p_kb->count++;
```
quote_metadata: {"evidence_id":"CO53-C06","actual_definition_kind":"function","enclosing_symbol":"keyboard_handler","definition_range":"115-131","quote_range":"123-127","quote_supports_claim":true,"source_blob_short12":"a40b5c7e56b3","consumer_positions":["C.buffer_producer"]}

## CO53-C07 · 键盘handler解锁后complete静态done
[VERIFIED mykernel/arch/x86_64/myos/keyboard.c::keyboard_handler]
```c
	spin_unlock(&kbdbuf_lock);

	complete(&getcode_done);
}
```
quote_metadata: {"evidence_id":"CO53-C07","actual_definition_kind":"function","enclosing_symbol":"keyboard_handler","definition_range":"115-131","quote_range":"128-131","quote_supports_claim":true,"source_blob_short12":"a40b5c7e56b3","consumer_positions":["C.event_notify"]}

## CO53-C08 · 键盘读取do-while重复等待后检查buffer count
[VERIFIED mykernel/arch/x86_64/myos/keyboard.c::kbd_get_scancode]
```c
	do {
		wait_for_completion(&getcode_done);
	}
	while (p_kb->count <= 0);
```
quote_metadata: {"evidence_id":"CO53-C08","actual_definition_kind":"function","enclosing_symbol":"kbd_get_scancode","definition_range":"146-165","quote_range":"150-153","quote_supports_claim":true,"source_blob_short12":"a40b5c7e56b3","consumer_positions":["C.static_wait"]}

## CO53-C09 · 键盘读取buffer字节
[VERIFIED mykernel/arch/x86_64/myos/keyboard.c::kbd_get_scancode]
```c
	spin_lock_irq(&kbdbuf_lock);
	if(p_kb->p_tail == p_kb->buf + KB_BUF_SIZE)	
		p_kb->p_tail = p_kb->buf;
	ret = *p_kb->p_tail;
	*p_kb->p_tail = 0;
```
quote_metadata: {"evidence_id":"CO53-C09","actual_definition_kind":"function","enclosing_symbol":"kbd_get_scancode","definition_range":"146-165","quote_range":"155-159","quote_supports_claim":true,"source_blob_short12":"a40b5c7e56b3","consumer_positions":["C.buffer_read"]}

## CO53-C10 · 缓冲尾递增、count递减、返回byte
[VERIFIED mykernel/arch/x86_64/myos/keyboard.c::kbd_get_scancode]
```c
	p_kb->p_tail++;
	p_kb->count--;
	spin_unlock_irq(&kbdbuf_lock);

	return ret;
```
quote_metadata: {"evidence_id":"CO53-C10","actual_definition_kind":"function","enclosing_symbol":"kbd_get_scancode","definition_range":"146-165","quote_range":"160-164","quote_supports_claim":true,"source_blob_short12":"a40b5c7e56b3","consumer_positions":["C.buffer_consume"]}

## CO53-C11 · 上层直接调用kbd_get_scancode
[VERIFIED mykernel/arch/x86_64/myos/keyboard.c::kbd_parse_scancode]
```c

	x = kbd_get_scancode();
```
quote_metadata: {"evidence_id":"CO53-C11","actual_definition_kind":"function","enclosing_symbol":"kbd_parse_scancode","definition_range":"167-279","quote_range":"173-174","quote_supports_claim":true,"source_blob_short12":"a40b5c7e56b3","consumer_positions":["C.repeat_user"]}

## CO53-C12 · IDE_Params_s保存controller/disk
[VERIFIED mykernel/arch/x86_64/myos/ide.c::IDE_Params_s]
```c
typedef struct IDE_Spec_Params
{
	u8		ATA_controller;
	u8		ATA_disk;
} IDE_Params_s;
```
quote_metadata: {"evidence_id":"CO53-C12","actual_definition_kind":"typedef_struct","enclosing_symbol":"IDE_Params_s","definition_range":"18-22","quote_range":"18-22","quote_supports_claim":true,"source_blob_short12":"39c36c8cc4f9","consumer_positions":["C.ide_params"]}

## CO53-C13 · blkbuf节点保存req_list及done指针
[VERIFIED mykernel/arch/x86_64/myos/myos_block.h::blkbuf_node_s]
```c
typedef struct blkbuf_node
{
	List_s			req_list;

	completion_s	*done;
```
quote_metadata: {"evidence_id":"CO53-C13","actual_definition_kind":"typedef_struct","enclosing_symbol":"blkbuf_node_s","definition_range":"5-18","quote_range":"5-9","quote_supports_claim":true,"source_blob_short12":"bc99d3b01204","consumer_positions":["C.ide_node"]}

## CO53-C14 · blkbuf节点具有task/count/cmd/LBA/buffer字段
[VERIFIED mykernel/arch/x86_64/myos/myos_block.h::blkbuf_node_s]
```c
	task_s			*task;
	unsigned int	count;
	unsigned char	cmd;
	unsigned long	LBA;
	unsigned char	*buffer;
```
quote_metadata: {"evidence_id":"CO53-C14","actual_definition_kind":"typedef_struct","enclosing_symbol":"blkbuf_node_s","definition_range":"5-18","quote_range":"10-14","quote_supports_claim":true,"source_blob_short12":"bc99d3b01204","consumer_positions":["C.ide_node_fields"]}

## CO53-C15 · IDE_make_request分别分配node及IDE参数
[VERIFIED mykernel/arch/x86_64/myos/ide.c::IDE_make_request]
```c
blkbuf_node_s *IDE_make_request(unsigned controller, unsigned disk,
		long cmd, unsigned long blk_idx, long count, unsigned char *buffer)
{
	blkbuf_node_s *node = (blkbuf_node_s *)kzalloc(sizeof(blkbuf_node_s), GFP_KERNEL);
	IDE_Params_s *IdeParam = (IDE_Params_s *)kzalloc(sizeof(IDE_Params_s), GFP_KERNEL);
```
quote_metadata: {"evidence_id":"CO53-C15","actual_definition_kind":"function","enclosing_symbol":"IDE_make_request","definition_range":"214-247","quote_range":"214-218","quote_supports_claim":true,"source_blob_short12":"39c36c8cc4f9","consumer_positions":["C.ide_alloc"]}

## CO53-C16 · IDE请求初始化count、buffer和DevSpecParams
[VERIFIED mykernel/arch/x86_64/myos/ide.c::IDE_make_request]
```c

	node->LBA = blk_idx;
	node->count = count;
	node->buffer = buffer;
	node->DevSpecParams = IdeParam;
```
quote_metadata: {"evidence_id":"CO53-C16","actual_definition_kind":"function","enclosing_symbol":"IDE_make_request","definition_range":"214-247","quote_range":"220-224","quote_supports_claim":true,"source_blob_short12":"39c36c8cc4f9","consumer_positions":["C.ide_init"]}

## CO53-C17 · ATA调用IDE_make_request并发布栈done地址
[VERIFIED mykernel/arch/x86_64/myos/ide.c::ATA_disk_transfer]
```c
		node = IDE_make_request(controller, disk, cmd,
							blk_idx, count, buffer);
		DECLARE_COMPLETION_ONSTACK(done);
		node->done = &done;
		node->task = current;
```
quote_metadata: {"evidence_id":"CO53-C17","actual_definition_kind":"function","enclosing_symbol":"ATA_disk_transfer","definition_range":"267-298","quote_range":"275-279","quote_supports_claim":true,"source_blob_short12":"39c36c8cc4f9","consumer_positions":["C.ide_done_publish"]}

## CO53-C18 · IDE请求队列发布并唤醒worker
[VERIFIED mykernel/arch/x86_64/myos/ide.c::ATA_disk_transfer]
```c
		spin_lock(&req_lock);
		list_header_add_to_tail(&IDEreq_lhdr, &node->req_list);
		spin_unlock(&req_lock);

		wake_up_process(thread);
```
quote_metadata: {"evidence_id":"CO53-C18","actual_definition_kind":"function","enclosing_symbol":"ATA_disk_transfer","definition_range":"267-298","quote_range":"281-285","quote_supports_claim":true,"source_blob_short12":"39c36c8cc4f9","consumer_positions":["C.ide_publish"]}

## CO53-C19 · IDE等待后请求释放DevSpecParams
[VERIFIED mykernel/arch/x86_64/myos/ide.c::ATA_disk_transfer]
```c
		wake_up_process(thread);
		wait_for_completion(&done);

		if (node->DevSpecParams != NULL)
			kfree(node->DevSpecParams);
```
quote_metadata: {"evidence_id":"CO53-C19","actual_definition_kind":"function","enclosing_symbol":"ATA_disk_transfer","definition_range":"267-298","quote_range":"285-289","quote_supports_claim":true,"source_blob_short12":"39c36c8cc4f9","consumer_positions":["C.ide_wait"]}

## CO53-C20 · IDE释放Params、node后返回常量
[VERIFIED mykernel/arch/x86_64/myos/ide.c::ATA_disk_transfer]
```c
		if (node->DevSpecParams != NULL)
			kfree(node->DevSpecParams);
		if (node != NULL)
			kfree(node);
		return -ENOERR;
```
quote_metadata: {"evidence_id":"CO53-C20","actual_definition_kind":"function","enclosing_symbol":"ATA_disk_transfer","definition_range":"267-298","quote_range":"288-292","quote_supports_claim":true,"source_blob_short12":"39c36c8cc4f9","consumer_positions":["C.ide_release"]}

## CO53-C21 · IDE daemon取请求并保存req_in_using
[VERIFIED mykernel/arch/x86_64/myos/ide.c::ATArq_deamon]
```c
			if (req_in_using == NULL)
			{
				List_s *wq_lp = list_header_remove_tail(&IDEreq_lhdr);
				blkbuf_node_s *node = container_of(wq_lp, blkbuf_node_s, req_list);
				req_in_using = node;
```
quote_metadata: {"evidence_id":"CO53-C21","actual_definition_kind":"function","enclosing_symbol":"ATArq_deamon","definition_range":"355-384","quote_range":"368-372","quote_supports_claim":true,"source_blob_short12":"39c36c8cc4f9","consumer_positions":["C.ide_daemon"]}

## CO53-C22 · IDE中断处理调用end_handler后条件end_request
[VERIFIED mykernel/arch/x86_64/myos/ide.c::ATA_disk_handler]
```c
	blkbuf_node_s *node = req_in_using;
	node->end_handler((unsigned long)node);
	
	if (node->count == 0)
		end_request(node);
```
quote_metadata: {"evidence_id":"CO53-C22","actual_definition_kind":"function","enclosing_symbol":"ATA_disk_handler","definition_range":"318-325","quote_range":"320-324","quote_supports_claim":true,"source_blob_short12":"39c36c8cc4f9","consumer_positions":["C.ide_irq"]}

## CO53-C23 · IDE结束函数正常分支读取node.done发complete
[VERIFIED mykernel/arch/x86_64/myos/ide.c::end_request]
```c
	if (node == NULL)
		color_printk(RED, BLACK, "end_request error\n");

	complete(node->done);
```
quote_metadata: {"evidence_id":"CO53-C23","actual_definition_kind":"function","enclosing_symbol":"end_request","definition_range":"150-159","quote_range":"152-155","quote_supports_claim":true,"source_blob_short12":"39c36c8cc4f9","consumer_positions":["C.ide_complete"]}

## CO53-C24 · IDE完成通知后清全局req_in_using
[VERIFIED mykernel/arch/x86_64/myos/ide.c::end_request]
```c
	complete(node->done);
	spin_lock(&req_lock);
	req_in_using = NULL;
	spin_unlock(&req_lock);
}
```
quote_metadata: {"evidence_id":"CO53-C24","actual_definition_kind":"function","enclosing_symbol":"end_request","definition_range":"150-159","quote_range":"155-159","quote_supports_claim":true,"source_blob_short12":"39c36c8cc4f9","consumer_positions":["C.ide_post_complete"]}

## CO53-C25 · 公共completion增加done并请求swake
[VERIFIED mykernel/kactive/completion/completion.c::complete]
```c
	spin_lock_irqsave(&x->wait.lock, flags);
	if (x->done != UINT_MAX)
		x->done++;
	swake_up_locked(&x->wait, 0);
	spin_unlock_irqrestore(&x->wait.lock, flags);
```
quote_metadata: {"evidence_id":"CO53-C25","actual_definition_kind":"function","enclosing_symbol":"complete","definition_range":"32-40","quote_range":"35-39","quote_supports_claim":true,"source_blob_short12":"422ed6eee18b","consumer_positions":["C.completion"]}

## CO53-C26 · 公开wait固定MAX且void
[VERIFIED mykernel/kactive/completion/completion.c::wait_for_completion]
```c
void wait_for_completion(completion_s *x) {
	wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
```
quote_metadata: {"evidence_id":"CO53-C26","actual_definition_kind":"function","enclosing_symbol":"wait_for_completion","definition_range":"53-55","quote_range":"53-55","quote_supports_claim":true,"source_blob_short12":"422ed6eee18b","consumer_positions":["C.wait"]}

## 元数据外置替代范围
`WW52-C14`原definition_range164-222应为164–215（原引文203–205有效）；`WW52-C16`原270-365应为270–309（原275–277有效）。原文与旧评论保留，C04负责独立闭合R52-01。未由证据计数认证任何内核运行、真实线程时序或内存安全。
