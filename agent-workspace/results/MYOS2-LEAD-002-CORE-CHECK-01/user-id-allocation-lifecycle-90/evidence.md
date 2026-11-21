---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ID-ALLOCATION-LIFECYCLE-90
record_type: fixed_time_qualified_definition_anchors
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-12
model_per_owner: GPT-6 + Extra
backend_identity: UNATTESTED
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: GitHub_connector
status: STATIC_EVIDENCE_PENDING_PRO_REVIEW
runtime_evidence: NOT_RUN
---

# 第90批静态源码证据索引

所有 59 条短锚均由本对话单独取回的 `time@a039d9803ade` 对象生成；D00 独立宏锚 IDA90-E001–E003，NULL条件路径 E004–E010。这里标签是引文身份，不是运行/编译/ABI/底层供应的证明；源码长函数的完整阅读及各处短锚并不替代独立Pro验收。

### IDA90-E001 | for_each_cpu：固定time真实定义内短锚

[VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_cpu]

```c
	#define for_each_cpu(cpu) \
				for (int cpu = 0; cpu < nr_cpu_ids; cpu++)
```

此处仅表示 for_each_cpu 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E002 | for_each_possible_cpu：固定time真实定义内短锚

[VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_possible_cpu]

```c
	#define for_each_possible_cpu(cpu)	for_each_cpu((cpu))
```

此处仅表示 for_each_possible_cpu 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E003 | for_each_online_cpu：固定time真实定义内短锚

[VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_online_cpu]

```c
	#define for_each_online_cpu(cpu)	for_each_cpu((cpu))
```

此处仅表示 for_each_online_cpu 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E004 | set_user_nice：固定time真实定义内短锚

[VERIFIED mykernel/sched/runqueue/runqueue.c::set_user_nice]

```c
void set_user_nice(task_s *p, long nice)
{
	bool queued, running;
	rq_s *rq;
	int old_prio;
```

此处仅表示 set_user_nice 定义内部连续 5 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E005 | set_user_nice：固定time真实定义内短锚

[VERIFIED mykernel/sched/runqueue/runqueue.c::set_user_nice]

```c
	// p->sched_class->prio_changed(rq, p, old_prio);
}
```

此处仅表示 set_user_nice 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E006 | create_worker：固定time真实定义内短锚

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
		set_user_nice(worker->task, pool->attrs->nice);
		// kthread_bind_mask(worker->task, pool_allowed_cpus(pool));
```

此处仅表示 create_worker 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E007 | create_worker：固定time真实定义内短锚

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
		if (IS_ERR(worker->task)) {
			if (PTR_ERR(worker->task) == -EINTR) {
				pr_err("workqueue: Interrupted when creating "
					"a worker thread \"%s\"\n", id_buf);
```

此处仅表示 create_worker 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E008 | create_worker：固定time真实定义内短锚

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
	worker_attach_to_pool(worker, pool);

	/* start the newly created worker */
	spin_lock_irq(&pool->lock);
```

此处仅表示 create_worker 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E009 | create_worker：固定time真实定义内短锚

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
	worker->pool->nr_workers++;
	worker_enter_idle(worker);
```

此处仅表示 create_worker 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E010 | create_worker：固定time真实定义内短锚

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
	if (worker->task)
		wake_up_process(worker->task);
	spin_unlock_irq(&pool->lock);

	return worker;
```

此处仅表示 create_worker 定义内部连续 5 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E011 | ida_init：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.h::ida_init]

```c
			ida->bitmap = kzalloc(IDA_BITMAP_LONGS * sizeof(long), GFP_KERNEL);

			ida->initiated = true;
```

此处仅表示 ida_init 定义内部连续 3 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E012 | MYOS_IDR_BUF_SIZE：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr_const.h::MYOS_IDR_BUF_SIZE]

```c
    #define MYOS_IDR_BUF_SIZE		32768
```

此处仅表示 MYOS_IDR_BUF_SIZE 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E013 | IDA_BITMAP_LONGS：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr_const.h::IDA_BITMAP_LONGS]

```c
	// #define IDA_CHUNK_SIZE		128	/* 128 bytes per chunk */
	#define IDA_CHUNK_SIZE			MYOS_IDR_BUF_SIZE	/* 128 bytes per chunk */
	#define IDA_BITMAP_LONGS		(IDA_CHUNK_SIZE / sizeof(long))
	#define IDA_BITMAP_BITS 		(IDA_BITMAP_LONGS * sizeof(long) * 8)
```

此处仅表示 IDA_BITMAP_LONGS 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E014 | ida：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr_types.h::ida]

```c
struct ida {
		ulong		*bitmap;

		bool		initiated;
```

此处仅表示 ida 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E015 | ida_alloc：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.h::ida_alloc]

```c
			return ida_alloc_range(ida, 0, ~0, gfp);
```

此处仅表示 ida_alloc 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E016 | ida_alloc_min：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.h::ida_alloc_min]

```c
			return ida_alloc_range(ida, min, ~0, gfp);
```

此处仅表示 ida_alloc_min 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E017 | ida_alloc_max：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.h::ida_alloc_max]

```c
			return ida_alloc_range(ida, 0, max, gfp);
```

此处仅表示 ida_alloc_max 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E018 | ida_alloc_range：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::ida_alloc_range]

```c
int ida_alloc_range(ida_s *ida, uint min, uint max, gfp_t gfp)
{
	for (uint slot_idx = min; slot_idx < IDA_BITMAP_LONGS; slot_idx++)
	{
```

此处仅表示 ida_alloc_range 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E019 | ida_alloc_range：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::ida_alloc_range]

```c
		if (slot != ~1ULL)
		for (uint bit_idx = 0; bit_idx < sizeof(long); bit_idx++)
			if ((slot & (1 << bit_idx)) == 0)
				return slot_idx * sizeof(long) + bit_idx;
```

此处仅表示 ida_alloc_range 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E020 | ida_alloc_range：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::ida_alloc_range]

```c
			if ((slot & (1 << bit_idx)) == 0)
				return slot_idx * sizeof(long) + bit_idx;
```

此处仅表示 ida_alloc_range 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E021 | ida_alloc_range：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::ida_alloc_range]

```c
	return -1;
}
```

此处仅表示 ida_alloc_range 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E022 | ida_free：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::ida_free]

```c
	ida->bitmap[id / sizeof(long)] &= (1 << (id % sizeof(long)));
```

此处仅表示 ida_free 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E023 | idr_init_base：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.h::idr_init_base]

```c
			idr->idr_rt = kzalloc(sizeof(void *) * MYOS_IDR_BUF_SIZE, GFP_KERNEL);
			idr->idr_base = base;
			idr->idr_next = 0;

			idr->initiated = true;
```

此处仅表示 idr_init_base 定义内部连续 5 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E024 | idr_init：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.h::idr_init]

```c
			idr_init_base(idr, 0);
```

此处仅表示 idr_init 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E025 | idr：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr_types.h::idr]

```c
struct idr {
		// struct radix_tree_root	idr_rt;
		void		**idr_rt;
		uint		idr_base;
		uint		idr_next;
```

此处仅表示 idr 定义内部连续 5 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E026 | idr_get_cursor：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.h::idr_get_cursor]

```c
			return READ_ONCE(idr->idr_next);
```

此处仅表示 idr_get_cursor 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E027 | idr_set_cursor：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.h::idr_set_cursor]

```c
			WRITE_ONCE(idr->idr_next, val);
```

此处仅表示 idr_set_cursor 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E028 | idr_alloc_u32：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_alloc_u32]

```c
	uint base = idr->idr_base;
	uint id = *nextid;

```

此处仅表示 idr_alloc_u32 定义内部连续 3 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E029 | idr_alloc_u32：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_alloc_u32]

```c
	for (int slot_idx = id; slot_idx < MYOS_IDR_BUF_SIZE; slot_idx++)
	{
		void **tmp_slot = &idr->idr_rt[slot_idx];
		if (*tmp_slot == NULL)
		{
```

此处仅表示 idr_alloc_u32 定义内部连续 5 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E030 | idr_alloc_u32：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_alloc_u32]

```c
			*tmp_slot = ptr;
			*nextid = slot_idx;
			return 0;
```

此处仅表示 idr_alloc_u32 定义内部连续 3 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E031 | idr_alloc_u32：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_alloc_u32]

```c
	return -ENOSPC;
```

此处仅表示 idr_alloc_u32 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E032 | idr_alloc：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_alloc]

```c
	if (WARN_ON_ONCE(start < 0))
		return -EINVAL;
```

此处仅表示 idr_alloc 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E033 | idr_alloc：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_alloc]

```c
	ret = idr_alloc_u32(idr, ptr, &id, end > 0 ?
				end - 1 : MYOS_IDR_BUF_SIZE, gfp);
	if (ret)
		return ret;
```

此处仅表示 idr_alloc 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E034 | idr_alloc：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_alloc]

```c
	if (ret)
		return ret;

	return id;
```

此处仅表示 idr_alloc 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E035 | idr_alloc_cyclic：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_alloc_cyclic]

```c
	u32 id = idr->idr_next;
	int err, max = end > 0 ? end - 1 : MYOS_IDR_BUF_SIZE;

	if ((int)id < start)
		id = start;
```

此处仅表示 idr_alloc_cyclic 定义内部连续 5 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E036 | idr_alloc_cyclic：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_alloc_cyclic]

```c
	err = idr_alloc_u32(idr, ptr, &id, max, gfp);
	if ((err == -ENOSPC) && (id > start)) {
		id = start;
		err = idr_alloc_u32(idr, ptr, &id, max, gfp);
	}
```

此处仅表示 idr_alloc_cyclic 定义内部连续 5 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E037 | idr_alloc_cyclic：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_alloc_cyclic]

```c
	idr->idr_next = id + 1;
	return id;
```

此处仅表示 idr_alloc_cyclic 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E038 | idr_find：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_find]

```c
	return idr->idr_rt[id];
```

此处仅表示 idr_find 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E039 | idr_replace：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_replace]

```c
	entry = idr->idr_rt[id];
	idr->idr_rt[id] = ptr;

```

此处仅表示 idr_replace 定义内部连续 3 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E040 | idr_remove：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_remove]

```c
	void *ptr = idr->idr_rt[id];
	idr->idr_rt[id] = NULL;
	return ptr;
```

此处仅表示 idr_remove 定义内部连续 3 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E041 | idr_for_each：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_for_each]

```c
	return 0;
}
```

此处仅表示 idr_for_each 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E042 | idr_get_next_ul：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_get_next_ul]

```c
	void *entry = NULL;
	ulong base = idr->idr_base;
	ulong id = *nextid;

```

此处仅表示 idr_get_next_ul 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E043 | idr_get_next_ul：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_get_next_ul]

```c
	return entry;
```

此处仅表示 idr_get_next_ul 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E044 | idr_get_next：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_get_next]

```c
	void *entry = idr_get_next_ul(idr, &id);

	if (WARN_ON_ONCE(id > INT_MAX))
		return NULL;
```

此处仅表示 idr_get_next 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E045 | idr_get_next：固定time真实定义内短锚

[VERIFIED mykernel/lib/idr/idr.c::idr_get_next]

```c
	*nextid = id;
	return entry;
```

此处仅表示 idr_get_next 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E046 | worker_pool_assign_id：固定time真实定义内短锚

[VERIFIED mykernel/kactive/workqueue/workqueue.c::worker_pool_assign_id]

```c
	int ret = idr_alloc(&worker_pool_idr, pool,
				0, WORK_OFFQ_POOL_NONE, GFP_KERNEL);
	if (ret >= 0) {
		pool->id = ret;
```

此处仅表示 worker_pool_assign_id 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E047 | worker_pool_assign_id：固定time真实定义内短锚

[VERIFIED mykernel/kactive/workqueue/workqueue.c::worker_pool_assign_id]

```c
	if (ret >= 0) {
		pool->id = ret;
		return 0;
	}
	return ret;
```

此处仅表示 worker_pool_assign_id 定义内部连续 5 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E048 | create_worker：固定time真实定义内短锚

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
	id = ida_alloc(&pool->worker_ida, GFP_KERNEL);
	if (id < 0) {
		pr_err_once("workqueue: Failed to allocate a worker ID: %pe\n",
			    ERR_PTR(id));
```

此处仅表示 create_worker 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E049 | create_worker：固定time真实定义内短锚

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
fail:
	ida_free(&pool->worker_ida, id);
	kfree(worker);
	return NULL;
```

此处仅表示 create_worker 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E050 | alloc_pid：固定time真实定义内短锚

[VERIFIED mykernel/sched/task/pid.c::alloc_pid]

```c
			nr = idr_alloc(&tmp->idr, NULL,
					tid, tid + 1, GFP_ATOMIC);
			/*
			 * If ENOSPC is returned it means that the PID is
```

此处仅表示 alloc_pid 定义内部连续 4 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E051 | alloc_pid：固定time真实定义内短锚

[VERIFIED mykernel/sched/task/pid.c::alloc_pid]

```c
			nr = idr_alloc_cyclic(&tmp->idr, NULL,
					pid_min, pid_max, GFP_ATOMIC);
		}
```

此处仅表示 alloc_pid 定义内部连续 3 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E052 | alloc_pid：固定time真实定义内短锚

[VERIFIED mykernel/sched/task/pid.c::alloc_pid]

```c
		idr_replace(&upid->ns->idr, pid, upid->nr);
		upid->ns->pid_allocated++;
```

此处仅表示 alloc_pid 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E053 | alloc_pid：固定time真实定义内短锚

[VERIFIED mykernel/sched/task/pid.c::alloc_pid]

```c
		idr_remove(&upid->ns->idr, upid->nr);
	}

```

此处仅表示 alloc_pid 定义内部连续 3 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E054 | find_pid_ns：固定time真实定义内短锚

[VERIFIED mykernel/sched/task/pid.c::find_pid_ns]

```c
pid_s *find_pid_ns(int nr, pid_ns_s *ns) {
	return idr_find(&ns->idr, nr);
}
```

此处仅表示 find_pid_ns 定义内部连续 3 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E055 | free_pid：固定time真实定义内短锚

[VERIFIED mykernel/sched/task/pid.c::free_pid]

```c
		idr_remove(&ns->idr, upid->nr);
	}
```

此处仅表示 free_pid 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E056 | pid_idr_init：固定time真实定义内短锚

[VERIFIED mykernel/sched/task/pid.c::pid_idr_init]

```c
	idr_init(&init_pid_ns.idr);

```

此处仅表示 pid_idr_init 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E057 | pid_idr_init：固定time真实定义内短锚

[VERIFIED mykernel/sched/task/pid.c::pid_idr_init]

```c
	init_pid_ns_idr->idr_rt[0] = &init_struct_pid;
	init_pid_ns_idr->idr_next++;
```

此处仅表示 pid_idr_init 定义内部连续 2 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E058 | PID_MAX_DEFAULT：固定time真实定义内短锚

[VERIFIED mykernel/sched/task/pid_const.h::PID_MAX_DEFAULT]

```c
	#define PID_MAX_DEFAULT (CONFIG_BASE_SMALL ? 0x1000 : 0x8000)
```

此处仅表示 PID_MAX_DEFAULT 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

### IDA90-E059 | IS_ERR：固定time真实定义内短锚

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]

```c
			return IS_ERR_VALUE((size_t)ptr);
```

此处仅表示 IS_ERR 定义内部连续 1 行原文；行为结论须按事实表的前置条件与止证门消费。

证据范围：此文件不证明外部 Linux 通用实现；IDR 历史注释的 radix-tree 算法不是本固定代码活动实现。旧WQP89、PID11的源证据只通过固定ID与定点核读引用，未复制到新锚计数。六件完整机械P9未运行，内核和仓库程序均NOT_RUN。
