---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-WORKQUEUE-POOL-BOOTSTRAP-89
record_type: fixed_time_source_definition_and_macro_evidence
produced_by: GPT-6（本对话显示的模型家族；后台不可独立认证）
model_per_owner: GPT-6＋Extra
date: 2026-11-15
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
status: STATIC_AUTHOR_EVIDENCE_TO_BE_INDEPENDENTLY_REVIEWED
runtime_evidence: NOT_RUN
open_questions: [WQP89-U01, WQP89-U02, WQP89-U03]
---

# 第89批固定time源码短锚

本批新短锚120项（D00新替代3项，A49项，B50项，C18项）。每一项是固定time下实际单一定义、宏、枚举成员或静态对象声明内连续1至5行；仅建立固定文本与局部源关系，真实构建、内核执行、CPU/线程运行、完整本地P9均NOT_RUN。

D00不复制旧88的两个失格锚：首个替代只引用work_func_t自身一行，work_s typedef另起独立锚；pool旗标采用真实enum worker_pool_flags。旧88的103个标签出现与旧作者103/103/0保持冻结，不混作89新增数量。

### WQP89-E001 | D00 original E011 work_s typedef itself

[VERIFIED mykernel/kactive/kactive_type_declaration.h::work_s]

```c
	typedef struct work_struct work_s;
```

固定time单一定义或数据声明内连续1行；仅源码静态引文。

### WQP89-E002 | D00 replacement for WQ88-E011, one typedef only

[VERIFIED mykernel/kactive/kactive_type_declaration.h::work_func_t]

```c
    typedef void (*work_func_t)(work_s *work);
```

固定time单一定义或数据声明内连续1行；仅源码静态引文。

### WQP89-E003 | D00 replacement WQ88-E039: real enum identity and four flags

[VERIFIED mykernel/kactive/workqueue/workqueue_const.h::worker_pool_flags]

```c
		POOL_BH					= 1 << 0,		/* is a BH pool */
		POOL_MANAGER_ACTIVE		= 1 << 1,		/* being managed */
		POOL_DISASSOCIATED		= 1 << 2,		/* cpu can't serve workers */
		POOL_BH_DRAINING		= 1 << 3,		/* draining after CPU offline */
```

固定time单一定义或数据声明内连续4行；仅源码静态引文。

### WQP89-E004 | percpu static BH pool array declaration

[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c::bh_worker_pools]

```c
DEFINE_PER_CPU_SHARED_ALIGNED(worker_pool_s
		[NR_STD_WORKER_POOLS], bh_worker_pools);
```

固定time单一定义或数据声明内连续2行；仅源码静态引文。

### WQP89-E005 | percpu static normal pool array declaration

[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c::cpu_worker_pools]

```c
DEFINE_PER_CPU_SHARED_ALIGNED(worker_pool_s
		[NR_STD_WORKER_POOLS], cpu_worker_pools);
```

固定time单一定义或数据声明内连续2行；仅源码静态引文。

### WQP89-E006 | BH perCPU pool array enumeration

[VERIFIED mykernel/kactive/workqueue/workqueue_macro.h::for_each_bh_worker_pool]

```c
	#define for_each_bh_worker_pool(pool, cpu)										\
			for ((pool) = &per_cpu(bh_worker_pools, cpu)[0];						\
					(pool) < &per_cpu(bh_worker_pools, cpu)[NR_STD_WORKER_POOLS];	\
					(pool)++)
```

固定time单一定义或数据声明内连续4行；仅源码静态引文。

### WQP89-E007 | normal perCPU pool enumeration

[VERIFIED mykernel/kactive/workqueue/workqueue_macro.h::for_each_cpu_worker_pool]

```c
	#define for_each_cpu_worker_pool(pool, cpu)					\
			for ((pool) = &per_cpu(cpu_worker_pools, cpu)[0];	\
					(pool) < &per_cpu(cpu_worker_pools,			\
								cpu)[NR_STD_WORKER_POOLS];		\
					(pool)++)
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E008 | two standard pools per CPU

[VERIFIED mykernel/kactive/workqueue/workqueue_const.h::wq_internal_consts]

```c
	enum wq_internal_consts {
		NR_STD_WORKER_POOLS		= 2,			/* # standard pools per cpu */
```

固定time单一定义或数据声明内连续2行；仅源码静态引文。

### WQP89-E009 | HIGHPRI nice/worker ID buffer constants

[VERIFIED mykernel/kactive/workqueue/workqueue_const.h::wq_internal_consts]

```c
		RESCUER_NICE_LEVEL		= MIN_NICE,
		HIGHPRI_NICE_LEVEL		= MIN_NICE,

		WQ_NAME_LEN				= 32,
		WORKER_ID_LEN			= 10 + WQ_NAME_LEN,		/* "kworker/R-" + WQ_NAME_LEN */
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E010 | possible and online loops same dense implementation

[VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_possible_cpu]

```c
	#define for_each_possible_cpu(cpu)	for_each_cpu((cpu))
	#define for_each_online_cpu(cpu)	for_each_cpu((cpu))
```

固定time单一定义或数据声明内连续2行；仅源码静态引文。

### WQP89-E011 | pool scalar and lock layout

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::worker_pool]

```c
	struct worker_pool {
		spinlock_t		lock;			/* the pool lock */
		int				cpu;			/* I: the associated cpu */
		int				node;			/* I: the associated node ID */
		int				id;				/* I: pool ID */
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E012 | pool scalar and lock layout continuation

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::worker_pool]

```c
		uint			flags;			/* L: flags */
```

固定time单一定义或数据声明内连续1行；仅源码静态引文。

### WQP89-E013 | nr_running/nr_workers and list counts

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::worker_pool]

```c
		int				nr_running;

		List_hdr_s		worklist;	/* L: list of pending works */

		int				nr_workers;	/* L: total number of workers */
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E014 | nr_running/nr_workers and list counts continuation

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::worker_pool]

```c
		// int				nr_idle;	/* L: currently idle workers */

		List_hdr_s		idle_list;	/* L: list of idle workers */
```

固定time单一定义或数据声明内连续3行；仅源码静态引文。

### WQP89-E015 | manager, workers list, IDA, attributes storage

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::worker_pool]

```c
		worker_s		*manager;	/* L: purely informational */
		List_hdr_s		workers;	/* A: attached workers */

		ida_s			worker_ida;	/* worker IDs for task name */

```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E016 | manager, workers list, IDA, attributes storage continuation

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::worker_pool]

```c
		wq_attrs_s		*attrs;		/* I: worker attributes */
```

固定time单一定义或数据声明内连续1行；仅源码静态引文。

### WQP89-E017 | nice metadata field

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::workqueue_attrs]

```c
	struct workqueue_attrs {
		/**
		 * @nice: nice level
		 */
		int nice;
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E018 | worker scheduled/task/pool/node

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::worker]

```c
		List_hdr_s			scheduled;		/* L: scheduled works */

		task_s				*task;			/* I: worker task */
		worker_pool_s		*pool;			/* A: the associated pool */
											/* L: for rescuers */
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E019 | worker scheduled/task/pool/node continuation

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::worker]

```c
		List_s				node;			/* A: anchored at pool->workers */
```

固定time单一定义或数据声明内连续1行；仅源码静态引文。

### WQP89-E020 | IDR request range and identity

[VERIFIED mykernel/kactive/workqueue/workqueue.c::worker_pool_assign_id]

```c
worker_pool_assign_id(worker_pool_s *pool) {
	// lockdep_assert_held(&wq_pool_mutex);

	int ret = idr_alloc(&worker_pool_idr, pool,
				0, WORK_OFFQ_POOL_NONE, GFP_KERNEL);
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E021 | IDR success writes pool ID and returns0

[VERIFIED mykernel/kactive/workqueue/workqueue.c::worker_pool_assign_id]

```c
	if (ret >= 0) {
		pool->id = ret;
		return 0;
	}
	return ret;
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E022 | IDR backing allocation request

[VERIFIED mykernel/lib/idr/idr.h::idr_init_base]

```c
		idr_init_base(idr_s *idr, int base) {
			// INIT_RADIX_TREE(&idr->idr_rt, IDR_RT_MARKER);
			idr->idr_rt = kzalloc(sizeof(void *) * MYOS_IDR_BUF_SIZE, GFP_KERNEL);
```

固定time单一定义或数据声明内连续3行；仅源码静态引文。

### WQP89-E023 | IDR fields set without result test

[VERIFIED mykernel/lib/idr/idr.h::idr_init_base]

```c
			idr->idr_base = base;
			idr->idr_next = 0;

			idr->initiated = true;
```

固定time单一定义或数据声明内连续4行；仅源码静态引文。

### WQP89-E024 | IDR init wrapper

[VERIFIED mykernel/lib/idr/idr.h::idr_init]

```c
		idr_init(idr_s *idr) {
			idr_init_base(idr, 0);
		}
```

固定time单一定义或数据声明内连续3行；仅源码静态引文。

### WQP89-E025 | IDR input validation and range

[VERIFIED mykernel/lib/idr/idr.c::idr_alloc]

```c
	if (WARN_ON_ONCE(start < 0))
		return -EINVAL;

	ret = idr_alloc_u32(idr, ptr, &id, end > 0 ?
				end - 1 : MYOS_IDR_BUF_SIZE, gfp);
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E026 | IDR returns allocated id or failure

[VERIFIED mykernel/lib/idr/idr.c::idr_alloc]

```c
	ret = idr_alloc_u32(idr, ptr, &id, end > 0 ?
				end - 1 : MYOS_IDR_BUF_SIZE, gfp);
	if (ret)
		return ret;

```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E027 | IDR returns allocated id or failure continuation

[VERIFIED mykernel/lib/idr/idr.c::idr_alloc]

```c
	return id;
```

固定time单一定义或数据声明内连续1行；仅源码静态引文。

### WQP89-E028 | BUG_ON evaluates and may invoke BUG

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::BUG_ON]

```c
	#			define BUG_ON(condition) do { if (unlikely(condition)) BUG(); } while (0)
```

固定time单一定义或数据声明内连续1行；仅源码静态引文。

### WQP89-E029 | list anchor and count initialization

[VERIFIED mykernel/lib/list/double_list.h::INIT_LIST_HEADER_S]

```c
		PREFIX_STATIC_INLINE
		void
		INIT_LIST_HEADER_S(List_hdr_s *lhdr) {
			WRITE_ONCE(lhdr->count, 0);
			INIT_LIST_S(&lhdr->anchor);
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E030 | header emptiness is count==0, not an anchor-only check

[VERIFIED mykernel/lib/list/double_list.h::list_header_is_empty]

```c
		list_header_is_empty(const List_hdr_s *lhdr_p) {
			return READ_ONCE(lhdr_p->count) == 0;
		}
```

固定time单一定义内连续3行；仅源码静态引文。

### WQP89-E031 | pool lock/IDs and flag OR

[VERIFIED mykernel/kactive/workqueue/workqueue.c::init_worker_pool]

```c
init_worker_pool(worker_pool_s *pool) {
	spin_lock_init(&pool->lock);
	pool->id = -1;
	pool->cpu = -1;
	pool->node = NUMA_NO_NODE;
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E032 | pool lock/IDs and flag OR continuation

[VERIFIED mykernel/kactive/workqueue/workqueue.c::init_worker_pool]

```c
	pool->flags |= POOL_DISASSOCIATED;
```

固定time单一定义或数据声明内连续1行；仅源码静态引文。

### WQP89-E033 | pool work and idle list init

[VERIFIED mykernel/kactive/workqueue/workqueue.c::init_worker_pool]

```c
	// pool->watchdog_ts = jiffies;
	INIT_LIST_HEADER_S(&pool->worklist);
	INIT_LIST_HEADER_S(&pool->idle_list);
	// hash_init(pool->busy_hash);
```

固定time单一定义或数据声明内连续4行；仅源码静态引文。

### WQP89-E034 | pool workers list and IDA init

[VERIFIED mykernel/kactive/workqueue/workqueue.c::init_worker_pool]

```c
	// timer_setup(&pool->mayday_timer, pool_mayday_timeout, 0);

	INIT_LIST_HEADER_S(&pool->workers);

	ida_init(&pool->worker_ida);
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E035 | pool attrs allocation and -ENOMEM check

[VERIFIED mykernel/kactive/workqueue/workqueue.c::init_worker_pool]

```c
	// /* shouldn't fail above this point */
	// pool->attrs = alloc_workqueue_attrs();
	pool->attrs = kzalloc(sizeof(*(pool->attrs)), GFP_KERNEL);
	if (!pool->attrs)
		return -ENOMEM;
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E036 | pool normal return

[VERIFIED mykernel/kactive/workqueue/workqueue.c::init_worker_pool]

```c
	// wqattrs_clear_for_pool(pool->attrs);

	return 0;
```

固定time单一定义或数据声明内连续3行；仅源码静态引文。

### WQP89-E037 | init helper BUG_ON and CPU ID assignment

[VERIFIED mykernel/kactive/workqueue/workqueue.c::init_cpu_worker_pool]

```c
static void __init
init_cpu_worker_pool(worker_pool_s *pool, int cpu, int nice) {
	BUG_ON(init_worker_pool(pool));
	pool->cpu = cpu;
```

固定time单一定义或数据声明内连续4行；仅源码静态引文。

### WQP89-E038 | perCPU attributes nice and node assignment

[VERIFIED mykernel/kactive/workqueue/workqueue.c::init_cpu_worker_pool]

```c
	// cpumask_copy(pool->attrs->cpumask, cpumask_of(cpu));
	// cpumask_copy(pool->attrs->__pod_cpumask, cpumask_of(cpu));
	pool->attrs->nice = nice;
	// pool->attrs->affn_strict = true;
	pool->node = 0;
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E039 | pool ID assignment under BUG_ON

[VERIFIED mykernel/kactive/workqueue/workqueue.c::init_cpu_worker_pool]

```c
	/* alloc pool ID */
	// mutex_lock(&wq_pool_mutex);
	BUG_ON(worker_pool_assign_id(pool));
	// mutex_unlock(&wq_pool_mutex);
```

固定time单一定义或数据声明内连续4行；仅源码静态引文。

### WQP89-E040 | cache and IDR init source order

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueue_init_early]

```c
void __init workqueue_init_early(void)
{
	int std_nice[NR_STD_WORKER_POOLS] = { 0, HIGHPRI_NICE_LEVEL };
	int i, cpu;

```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E041 | cache and IDR init source order continuation

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueue_init_early]

```c
	pwq_cache = KMEM_CACHE(pool_workqueue, SLAB_PANIC);
	idr_init(&worker_pool_idr);
```

固定time单一定义或数据声明内连续2行；仅源码静态引文。

### WQP89-E042 | early BH perCPU outer + init and pool BH flag

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueue_init_early]

```c
	/* initialize BH and CPU pools */
	for_each_possible_cpu(cpu) {
		worker_pool_s *pool;

		i = 0;
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E043 | early BH perCPU outer + init and pool BH flag continuation

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueue_init_early]

```c
		for_each_bh_worker_pool(pool, cpu) {
			init_cpu_worker_pool(pool, cpu, std_nice[i]);
			pool->flags |= POOL_BH;
```

固定time单一定义或数据声明内连续3行；仅源码静态引文。

### WQP89-E044 | BH array init and active BH mark

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueue_init_early]

```c
		for_each_bh_worker_pool(pool, cpu) {
			init_cpu_worker_pool(pool, cpu, std_nice[i]);
			pool->flags |= POOL_BH;
			// init_irq_work(bh_pool_irq_work(pool), irq_work_fns[i]);
			i++;
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E045 | normal pool iterator init

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueue_init_early]

```c
		i = 0;
		for_each_cpu_worker_pool(pool, cpu) {
			init_cpu_worker_pool(pool, cpu, std_nice[i]);
			i++;
		}
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E046 | ordinary events workqueue construction

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueue_init_early]

```c
	system_wq =
			simple_alloc_workqueue("events", 0, 0);
```

固定time单一定义或数据声明内连续2行；仅源码静态引文。

### WQP89-E047 | BH events workqueue construction

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueue_init_early]

```c
	system_bh_wq =
			simple_alloc_workqueue("events_bh", WQ_BH, 0);
```

固定time单一定义或数据声明内连续2行；仅源码静态引文。

### WQP89-E048 | later BH pool worker creation and BUG_ON

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueue_init]

```c
	for_each_possible_cpu(cpu)
		for_each_bh_worker_pool(pool, cpu)
			BUG_ON(!create_worker(pool));
```

固定time单一定义或数据声明内连续3行；仅源码静态引文。

### WQP89-E049 | online alias loop worker creation

[VERIFIED mykernel/kactive/workqueue/workqueue.c::workqueue_init]

```c
	for_each_online_cpu(cpu) {
		for_each_cpu_worker_pool(pool, cpu) {
			pool->flags &= ~POOL_DISASSOCIATED;
			BUG_ON(!create_worker(pool));
		}
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。

### WQP89-E050 | boot early call commented at fixed-time site

[VERIFIED mykernel/init/main.c::start_kernel]

```c
	// workqueue_init_early();
```

固定time单一定义或数据声明内连续1行；仅源码静态引文。

### WQP89-E051 | boot late call commented at fixed-time site

[VERIFIED mykernel/init/main.c::kernel_init_freeable]

```c
	// workqueue_init();
```

固定time单一定义或数据声明内连续1行；仅源码静态引文。

### WQP89-E052 | IDA bitmap allocation and initiated flag without NULL guard

[VERIFIED mykernel/lib/idr/idr.h::ida_init]

```c
		ida_init(ida_s *ida) {
			ida->bitmap = kzalloc(IDA_BITMAP_LONGS * sizeof(long), GFP_KERNEL);

			ida->initiated = true;
		}
```

固定time单一定义或数据声明内连续5行；仅源码静态引文。


### WQP89-E053 | worker allocation request and conditional init

[VERIFIED mykernel/kactive/workqueue/workqueue.c::alloc_worker]

```c
static worker_s *alloc_worker(int node) {
	worker_s *worker;

	worker = kzalloc_node(sizeof(*worker), GFP_KERNEL, 0);
	if (worker) {
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E054 | entry/scheduled/node and PREP initialization

[VERIFIED mykernel/kactive/workqueue/workqueue.c::alloc_worker]

```c
		INIT_LIST_HEAD(&worker->entry);
		INIT_LIST_HEADER_S(&worker->scheduled);
		INIT_LIST_HEAD(&worker->node);
		/* on creation a worker is in !idle && prep state */
		worker->flags = WORKER_PREP;
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E055 | conditional resource return

[VERIFIED mykernel/kactive/workqueue/workqueue.c::alloc_worker]

```c
	}
	return worker;
}
```

固定time单一函数、结构或宏定义内连续3行；只做源代码分析。

### WQP89-E056 | formatter pool/cpu branch

[VERIFIED mykernel/kactive/workqueue/workqueue.c::format_worker_id]

```c
format_worker_id(char *buf, size_t size,
		worker_s *worker, worker_pool_s *pool) {

	if (pool) {
		if (pool->cpu >= 0)
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E057 | formatter cpu and unbound names

[VERIFIED mykernel/kactive/workqueue/workqueue.c::format_worker_id]

```c
		if (pool->cpu >= 0)
			return scnprintf(buf, size, "kworker/%d:%d%s",
					pool->cpu, worker->id,
					pool->attrs->nice < 0  ? "H" : "");
		else
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E058 | formatter cpu and unbound names continuation

[VERIFIED mykernel/kactive/workqueue/workqueue.c::format_worker_id]

```c
			return scnprintf(buf, size, "kworker/u%d:%d",
					pool->id, worker->id);
```

固定time单一函数、结构或宏定义内连续2行；只做源代码分析。

### WQP89-E059 | formatter fallback

[VERIFIED mykernel/kactive/workqueue/workqueue.c::format_worker_id]

```c
	} else {
		return scnprintf(buf, size, "kworker/dying");
	}
}
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E060 | worker IDA entry

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
*create_worker(worker_pool_s *pool) {
	worker_s *worker;
	int id;

	/* ID is needed to determine kthread name */
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E061 | worker IDA entry continuation

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
	id = ida_alloc(&pool->worker_ida, GFP_KERNEL);
```

固定time单一函数、结构或宏定义内连续1行；只做源代码分析。

### WQP89-E062 | negative ID error before object

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
	if (id < 0) {
		pr_err_once("workqueue: Failed to allocate a worker ID: %pe\n",
			    ERR_PTR(id));
		return NULL;
	}
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E063 | worker object failure jumps cleanup

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
	worker = alloc_worker(pool->node);
	if (!worker) {
		pr_err_once("workqueue: Failed to allocate a worker\n");
		goto fail;
	}
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E064 | ID assignment and BH conditional

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
	worker->id = id;

	if (!(pool->flags & POOL_BH)) {
		char id_buf[WORKER_ID_LEN];
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E065 | thread creation request and immediate error test

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
		format_worker_id(id_buf, sizeof(id_buf), worker, pool);
		worker->task = kthread_create_on_node(simple_worker_thread,
							worker, pool->node, "%s", id_buf);
		if (IS_ERR(worker->task)) {
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E066 | ERR_PTR and EINTR subtype

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
		if (IS_ERR(worker->task)) {
			if (PTR_ERR(worker->task) == -EINTR) {
				pr_err("workqueue: Interrupted when creating "
					"a worker thread \"%s\"\n", id_buf);
			} else {
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E067 | non-EINTR report and cleanup goto

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
				pr_err_once("workqueue: Failed to create a "
					"worker thread: %pe", worker->task);
			}
			goto fail;
		}
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E068 | nice-setting and commented CPU binding

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
		set_user_nice(worker->task, pool->attrs->nice);
		// kthread_bind_mask(worker->task, pool_allowed_cpus(pool));
	}
```

固定time单一函数、结构或宏定义内连续3行；只做源代码分析。

### WQP89-E069 | attach then take pool lock

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
	/* successful, attach the worker to the pool */
	worker_attach_to_pool(worker, pool);

	/* start the newly created worker */
	spin_lock_irq(&pool->lock);
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E070 | worker count and idle entry

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
	worker->pool->nr_workers++;
	worker_enter_idle(worker);
```

固定time单一函数、结构或宏定义内连续2行；只做源代码分析。

### WQP89-E071 | conditional worker wake and normal return

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
	if (worker->task)
		wake_up_process(worker->task);
	spin_unlock_irq(&pool->lock);

	return worker;
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E072 | failure release ID/object

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]

```c
fail:
	ida_free(&pool->worker_ida, id);
	kfree(worker);
	return NULL;
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E073 | attach guard/commented mutex

[VERIFIED mykernel/kactive/workqueue/workqueue.c::worker_attach_to_pool]

```c
worker_attach_to_pool(worker_s *worker, worker_pool_s *pool) {
	// mutex_lock(&wq_pool_attach_mutex);

```

固定time单一函数、结构或宏定义内连续3行；只做源代码分析。

### WQP89-E074 | DISASSOCIATED-to-UNBOUND worker flag

[VERIFIED mykernel/kactive/workqueue/workqueue.c::worker_attach_to_pool]

```c
	if (pool->flags & POOL_DISASSOCIATED) {
		worker->flags |= WORKER_UNBOUND;
	} else {
		WARN_ON_ONCE(pool->flags & POOL_BH);
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E075 | associated branch WARN and commented affinity

[VERIFIED mykernel/kactive/workqueue/workqueue.c::worker_attach_to_pool]

```c
		WARN_ON_ONCE(pool->flags & POOL_BH);
		// kthread_set_per_cpu(worker->task, pool->cpu);
	}
```

固定time单一函数、结构或宏定义内连续3行；只做源代码分析。

### WQP89-E076 | pool workers list insert before pool assignment

[VERIFIED mykernel/kactive/workqueue/workqueue.c::worker_attach_to_pool]

```c
	list_header_add_to_tail(&pool->workers, &worker->node);
	worker->pool = pool;

	// mutex_unlock(&wq_pool_attach_mutex);
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E077 | IDA allocation macro forwards to range

[VERIFIED mykernel/lib/idr/idr.h::ida_alloc]

```c
		PREFIX_STATIC_INLINE
		int
		ida_alloc(ida_s *ida, gfp_t gfp) {
			return ida_alloc_range(ida, 0, ~0, gfp);
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E078 | IDA bitmap scan start and read

[VERIFIED mykernel/lib/idr/idr.c::ida_alloc_range]

```c
int ida_alloc_range(ida_s *ida, uint min, uint max, gfp_t gfp)
{
	for (uint slot_idx = min; slot_idx < IDA_BITMAP_LONGS; slot_idx++)
	{
		ulong slot = ida->bitmap[slot_idx];
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E079 | IDA zero-bit selection returns ID without set

[VERIFIED mykernel/lib/idr/idr.c::ida_alloc_range]

```c
		if (slot != ~1ULL)
		for (uint bit_idx = 0; bit_idx < sizeof(long); bit_idx++)
			if ((slot & (1 << bit_idx)) == 0)
				return slot_idx * sizeof(long) + bit_idx;
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E080 | IDA exhaustion return -1

[VERIFIED mykernel/lib/idr/idr.c::ida_alloc_range]

```c
	return -1;
}
```

固定time单一函数、结构或宏定义内连续2行；只做源代码分析。

### WQP89-E081 | IDA release uses AND bit mask

[VERIFIED mykernel/lib/idr/idr.c::ida_free]

```c
void ida_free(ida_s *ida, uint id)
{
	ida->bitmap[id / sizeof(long)] &= (1 << (id % sizeof(long)));
}
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E082 | IDA chunk backing size constant

[VERIFIED mykernel/lib/idr/idr_const.h::IDA_CHUNK_SIZE]

```c
	#define IDA_CHUNK_SIZE			MYOS_IDR_BUF_SIZE	/* 128 bytes per chunk */
```

固定time单一函数、结构或宏定义内连续1行；只做源代码分析。

### WQP89-E083 | IDA number of long slots derived from chunk bytes

[VERIFIED mykernel/lib/idr/idr_const.h::IDA_BITMAP_LONGS]

```c
	#define IDA_BITMAP_LONGS		(IDA_CHUNK_SIZE / sizeof(long))
```

固定time单一函数、结构或宏定义内连续1行；只做源代码分析。

### WQP89-E084 | full ida type fields bitmap pointer and initiated bool

[VERIFIED mykernel/lib/idr/idr_types.h::ida]

```c
	struct ida {
		ulong		*bitmap;

		bool		initiated;
	};
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E085 | node parameter ignored in kmalloc_node macro

[VERIFIED mykernel/mm/kmalloc/slub_macro.h::kmalloc_node]

```c
	#define kmalloc_node(_size, _flags, _node)	\
				kmalloc(_size, _flags)
```

固定time单一函数、结构或宏定义内连续2行；只做源代码分析。

### WQP89-E086 | kzalloc_node adds ZERO and forwards to kmalloc_node

[VERIFIED mykernel/mm/kmalloc/slub_macro.h::kzalloc_node]

```c
	#define kzalloc_node(_size, _flags, _node)	\
				kmalloc_node(_size, (_flags)|__GFP_ZERO, _node)
```

固定time单一函数、结构或宏定义内连续2行；只做源代码分析。

### WQP89-E087 | error-to-pointer cast

[VERIFIED mykernel/include/linux/kernel/err.h::ERR_PTR]

```c
		static inline void *
		ERR_PTR(long error) {
			return (void *) error;
		}
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E088 | pointer-to-error cast

[VERIFIED mykernel/include/linux/kernel/err.h::PTR_ERR]

```c
		static inline long
		PTR_ERR(const void *ptr) {
			return (long) ptr;
		}
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E089 | pointer error classification

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]

```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E090 | thread public wrapper and varargs

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_create_on_node]

```c
task_s *kthread_create_on_node(int (*threadfn)(void *data),
		void *data, int node, const char namefmt[], ...)
{
	task_s *task;
	va_list args;
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E091 | thread public wrapper and varargs continuation

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_create_on_node]

```c

	va_start(args, namefmt);
	task = __kthread_create_on_node(threadfn, data, node, namefmt, args);
```

固定time单一函数、结构或宏定义内连续3行；只做源代码分析。

### WQP89-E092 | thread return after delegated creation

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_create_on_node]

```c
	task = __kthread_create_on_node(threadfn, data, node, namefmt, args);
	va_end(args);

	return task;
}
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E093 | thread create info allocation error

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]

```c
	task_s *task;
	kthd_create_info_s *create =
		kmalloc(sizeof(kthd_create_info_s), GFP_KERNEL);

	if (!create)
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E094 | thread create info allocation error continuation

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]

```c
		return ERR_PTR(-ENOMEM);
```

固定time单一函数、结构或宏定义内连续1行；只做源代码分析。

### WQP89-E095 | kthreadd queue and wake request

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]

```c
	spin_lock(&kthread_create_lock);
	list_header_add_to_tail(&kthread_create_list, &create->list);
	spin_unlock(&kthread_create_lock);

	wake_up_process(kthreadd_task);
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E096 | completion and result/error handoff

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]

```c
		wait_for_completion(&done);
	// }
	task = create->result;
free_create:
	kfree(create);
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E097 | completion and result/error handoff continuation

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]

```c
	return task;
```

固定time单一函数、结构或宏定义内连续1行；只做源代码分析。

### WQP89-E098 | wake request delegates to scheduler

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_process]

```c
int wake_up_process(task_s *p)
{
	return try_to_wake_up(p, TASK_NORMAL, 0);
}
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E099 | idle flag/time/list append

[VERIFIED mykernel/kactive/workqueue/workqueue.h::worker_enter_idle]

```c
			/* can't use worker_set_flags(), also called from create_worker() */
			worker->flags |= WORKER_IDLE;
			worker->last_active = jiffies;

			/* idle_list is LIFO */
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。

### WQP89-E100 | idle flag/time/list append continuation

[VERIFIED mykernel/kactive/workqueue/workqueue.h::worker_enter_idle]

```c
			list_header_add_to_tail(&pool->idle_list, &worker->entry);
```

固定time单一函数、结构或宏定义内连续1行；只做源代码分析。

### WQP89-E101 | idle count and nr_running warning

[VERIFIED mykernel/kactive/workqueue/workqueue.h::worker_enter_idle]

```c
			/* Sanity check nr_running. */
			WARN_ON_ONCE(pool->nr_workers ==
				pool->idle_list.count && pool->nr_running);
		}
```

固定time单一函数、结构或宏定义内连续4行；只做源代码分析。

### WQP89-E102 | list header publication/count increment

[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_tail]

```c
		void
		list_header_add_to_tail(List_hdr_s *lhdr_p, List_s *l_p) {
			list_add_to_prev(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
		}
```

固定time单一函数、结构或宏定义内连续5行；只做源代码分析。


### WQP89-E103 | policy requires nonempty list and nr_running=0

[VERIFIED mykernel/kactive/workqueue/workqueue.h::need_more_worker]

```c
		need_more_worker(worker_pool_s *pool) {
			return !list_header_is_empty(&pool->worklist)
						&& pool->nr_running == 0;
		}
```

固定time真实单一函数或宏定义内部连续4行；非线程运行证明。

### WQP89-E104 | idle count is readiness gate

[VERIFIED mykernel/kactive/workqueue/workqueue.h::may_start_working]

```c
		bool
		may_start_working(worker_pool_s *pool) {
			return pool->idle_list.count != 0;
		}
```

固定time真实单一函数或宏定义内部连续4行；非线程运行证明。

### WQP89-E105 | create need is pending and no idle

[VERIFIED mykernel/kactive/workqueue/workqueue.h::need_to_create_worker]

```c
		bool
		need_to_create_worker(worker_pool_s *pool) {
			return need_more_worker(pool) &&
					!may_start_working(pool);
		}
```

固定time真实单一函数或宏定义内部连续5行；非线程运行证明。

### WQP89-E106 | manager already active short-circuit

[VERIFIED mykernel/kactive/workqueue/workqueue.c::manage_workers]

```c
manage_workers(worker_s *worker) {
	worker_pool_s *pool = worker->pool;

	if (pool->flags & POOL_MANAGER_ACTIVE)
		return false;
```

固定time真实单一函数或宏定义内部连续5行；非线程运行证明。

### WQP89-E107 | claim manager bit and pointer, then call creation

[VERIFIED mykernel/kactive/workqueue/workqueue.c::manage_workers]

```c
	pool->flags |= POOL_MANAGER_ACTIVE;
	pool->manager = worker;

	maybe_create_worker(pool);
```

固定time真实单一函数或宏定义内部连续4行；非线程运行证明。

### WQP89-E108 | manager returns then clears ownership

[VERIFIED mykernel/kactive/workqueue/workqueue.c::manage_workers]

```c
	maybe_create_worker(pool);

	pool->manager = NULL;
	pool->flags &= ~POOL_MANAGER_ACTIVE;
```

固定time真实单一函数或宏定义内部连续4行；非线程运行证明。

### WQP89-E109 | manager returns true after cleanup

[VERIFIED mykernel/kactive/workqueue/workqueue.c::manage_workers]

```c
	// rcuwait_wake_up(&manager_wait);
	return true;
}
```

固定time真实单一函数或宏定义内部连续3行；非线程运行证明。

### WQP89-E110 | outer do releases lock before attempting new worker

[VERIFIED mykernel/kactive/workqueue/workqueue.c::maybe_create_worker]

```c
	do {
		spin_unlock_irq(&pool->lock);

		// /* if we don't make progress in MAYDAY_INITIAL_TIMEOUT, call for help */
		// mod_timer(&pool->mayday_timer, jiffies + MAYDAY_INITIAL_TIMEOUT);
```

固定time真实单一函数或宏定义内部连续5行；非线程运行证明。

### WQP89-E111 | inner retry accepts create success or no more demand

[VERIFIED mykernel/kactive/workqueue/workqueue.c::maybe_create_worker]

```c
		while (true) {
			if (create_worker(pool) || !need_to_create_worker(pool))
				break;
```

固定time真实单一函数或宏定义内部连续3行；非线程运行证明。

### WQP89-E112 | cooldown commented and second demand check

[VERIFIED mykernel/kactive/workqueue/workqueue.c::maybe_create_worker]

```c
			// schedule_timeout_interruptible(CREATE_COOLDOWN);

			if (!need_to_create_worker(pool))
				break;
```

固定time真实单一函数或宏定义内部连续4行；非线程运行证明。

### WQP89-E113 | timer cleanup comment and lock reacquire

[VERIFIED mykernel/kactive/workqueue/workqueue.c::maybe_create_worker]

```c
		// del_timer_sync(&pool->mayday_timer);
		spin_lock_irq(&pool->lock);
```

固定time真实单一函数或宏定义内部连续2行；非线程运行证明。

### WQP89-E114 | outer loop repeats on renewed demand

[VERIFIED mykernel/kactive/workqueue/workqueue.c::maybe_create_worker]

```c
	} while (need_to_create_worker(pool));
}
```

固定time真实单一函数或宏定义内部连续2行；非线程运行证明。

### WQP89-E115 | worker thread checks pending demand

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_worker_thread]

```c
	worker_leave_idle(worker);
recheck:
	/* no more worker necessary? */
	if (!need_more_worker(pool))
		goto sleep;
```

固定time真实单一函数或宏定义内部连续5行；非线程运行证明。

### WQP89-E116 | worker thread manager result directs recheck

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_worker_thread]

```c
	/* do we need to manage? */
	if (unlikely(!may_start_working(pool)) && manage_workers(worker))
		goto recheck;
```

固定time真实单一函数或宏定义内部连续3行；非线程运行证明。

### WQP89-E117 | spin_lock_irq maps to raw implementation

[VERIFIED mykernel/lock_IPC/spinlock/spinlock_smp_macro.h::spin_lock_irq]

```c
	#define spin_lock_irq(lock)	\
				raw_spin_lock_irq(lock)
```

固定time真实单一函数或宏定义内部连续2行；非线程运行证明。

### WQP89-E118 | spin_unlock_irq maps to raw implementation

[VERIFIED mykernel/lock_IPC/spinlock/spinlock_smp_macro.h::spin_unlock_irq]

```c
	#define spin_unlock_irq(lock)	\
				raw_spin_unlock_irq(lock)
```

固定time真实单一函数或宏定义内部连续2行；非线程运行证明。

### WQP89-E119 | nr_running increased on last NOT_RUNNING clear

[VERIFIED mykernel/kactive/workqueue/workqueue.h::worker_clr_flags]

```c
			if ((flags & WORKER_NOT_RUNNING) && (oflags & WORKER_NOT_RUNNING))
				if (!(worker->flags & WORKER_NOT_RUNNING))
					pool->nr_running++;
```

固定time真实单一函数或宏定义内部连续3行；非线程运行证明。

### WQP89-E120 | nr_running decreased on first NOT_RUNNING enter

[VERIFIED mykernel/kactive/workqueue/workqueue.h::worker_set_flags]

```c
			if ((flags & WORKER_NOT_RUNNING) &&
					!(worker->flags & WORKER_NOT_RUNNING)) {
				pool->nr_running--;
			}
```

固定time真实单一函数或宏定义内部连续4行；非线程运行证明。

