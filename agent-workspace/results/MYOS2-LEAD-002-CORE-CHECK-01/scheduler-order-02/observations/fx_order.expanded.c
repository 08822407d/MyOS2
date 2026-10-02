/* ---- provenance (metadata comment) ----------------------------------------------------------
 * packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-ORDER-02 (scheduler-order-02)
 * executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
 * source: new template. Type choices follow core/fixtures/fx_sched.c @ 0d62c4d19711 (frozen,
 *   unchanged); every //@@ORIG line is replaced verbatim from time a039d9803ade by the frozen
 *   recheck-01 build2.Builder.expand. pick_next_task_myos is copied unchanged (comparison, requeue,
 *   accounting order and return are the original text).
 * hand-written here (NOT MyOS2 code): reduced task_struct / runqueue holding only the members the
 *   function touches (__state, se.vruntime, rt.run_list, rt.time_slice, rq->idle, rq->curr, rq->myos)
 *   plus a fixture id; current, need_resched() and jiffies as explicit fixture inputs; BUG_ON records
 *   and stops (exit 42). Observers run only before and after each call; the queue walk compares each
 *   link with the anchor and with the known task nodes by address, never converting an unknown link,
 *   and follows at most 16 links.
 * not taken over from fx_sched.c: its pick() helper that set rq->myos.last_jiffies = jiffies before
 *   every call. Here last_jiffies is set once per scenario; between calls only the original function
 *   updates it.
 * sequence model: in W07/W08 the returned task is made current before the next call. This is not a
 *   context switch, a clock, an interrupt or preemption.
 * --------------------------------------------------------------------------------------------- */
#include "fx_common.h"

#line 1 "fx_list.inc.c"
/* Original MyOS2 list types, macros and primitives, copied verbatim from time by the builder.
 * Hand-written here: the two typedef lines (same text as lib/lib_type_declaration.h). */
typedef struct list_head List_s;
typedef struct list_hdr List_hdr_s;
/* ORIG time:mykernel/lib/list/double_list_types.h struct list_head lines 8-11 (copied verbatim) */
#line 8 "mykernel/lib/list/double_list_types.h"
	struct list_head {
		List_s		*next;
		List_s		*prev;
	};
#line 6 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list_types.h struct list_hdr lines 13-16 (copied verbatim) */
#line 13 "mykernel/lib/list/double_list_types.h"
	struct list_hdr {
		List_s		anchor;
		ulong		count;
	};
#line 7 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list_const.h macro LIST_POISON1 lines 5-5 (copied verbatim) */
#line 5 "mykernel/lib/list/double_list_const.h"
	#define LIST_POISON1  ((void *) 0x100)
#line 8 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list_const.h macro LIST_POISON2 lines 6-6 (copied verbatim) */
#line 6 "mykernel/lib/list/double_list_const.h"
	#define LIST_POISON2  ((void *) 0x122)
#line 9 "fx_list.inc.c"
/* ORIG time:mykernel/include/uapi/linux/stddef.h macro container_of lines 9-13 (copied verbatim) */
#line 9 "mykernel/include/uapi/linux/stddef.h"
	#	define container_of(member_ptr, container_type, member_name) ({				\
					typeof(((container_type *)0)->member_name) * p = (member_ptr);	\
					(container_type *)((unsigned long)p -							\
						(unsigned long)&(((container_type *)0)->member_name));		\
				})
#line 10 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h macro INIT_LIST_HEAD lines 125-125 (copied verbatim) */
#line 125 "mykernel/lib/list/double_list.h"
		#define INIT_LIST_HEAD(list) INIT_LIST_S(list)
#line 11 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h macro list_is_head lines 132-132 (copied verbatim) */
#line 132 "mykernel/lib/list/double_list.h"
		#define list_is_head list_is_head_anchor
#line 12 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list_macro.h macro LIST_INIT lines 5-8 (copied verbatim) */
#line 5 "mykernel/lib/list/double_list_macro.h"
	#define LIST_INIT(name)	{	\
				&(name),		\
				&(name),		\
			}
#line 13 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list_macro.h macro LIST_HEADER_INIT lines 9-12 (copied verbatim) */
#line 9 "mykernel/lib/list/double_list_macro.h"
	#define LIST_HEADER_INIT(name) {				\
				.anchor	= LIST_INIT(name.anchor),	\
				.count	= 0,						\
			}
#line 14 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list_macro.h macro list_container lines 25-26 (copied verbatim) */
#line 25 "mykernel/lib/list/double_list_macro.h"
	#define list_container(ptr, type, member) \
				container_of(ptr, type, member)
#line 15 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list_macro.h macro list_entry lines 27-27 (copied verbatim) */
#line 27 "mykernel/lib/list/double_list_macro.h"
	#define list_entry list_container
#line 16 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list_macro.h macro list_first_entry lines 37-38 (copied verbatim) */
#line 37 "mykernel/lib/list/double_list_macro.h"
	#define list_first_entry(ptr, type, member) \
				list_entry((ptr)->next, type, member)
#line 17 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list_macro.h macro list_for_each lines 88-91 (copied verbatim) */
#line 88 "mykernel/lib/list/double_list_macro.h"
	#define list_for_each(pos, head)			\
				for (pos = (head)->next;		\
					!list_is_head(pos, (head));	\
					pos = pos->next)
#line 18 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list_macro.h macro list_headr_first_container lines 334-335 (copied verbatim) */
#line 334 "mykernel/lib/list/double_list_macro.h"
	#define list_headr_first_container(ptr, type, member) \
				list_first_entry(&((ptr)->anchor), type, member)
#line 19 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list_macro.h macro list_header_foreach lines 340-341 (copied verbatim) */
#line 340 "mykernel/lib/list/double_list_macro.h"
	#define list_header_foreach(pos, lhdr)	\
				list_for_each(pos, &((lhdr)->anchor))
#line 20 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func INIT_LIST_S lines 147-152 (copied verbatim) */
#line 147 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		void
		INIT_LIST_S(List_s *list) {
			WRITE_ONCE(list->next, list);
			WRITE_ONCE(list->prev, list);
		}
#line 21 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func INIT_LIST_HEADER_S lines 153-158 (copied verbatim) */
#line 153 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		void
		INIT_LIST_HEADER_S(List_hdr_s *lhdr) {
			WRITE_ONCE(lhdr->count, 0);
			INIT_LIST_S(&lhdr->anchor);
		}
#line 22 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func __list_add_valid lines 160-168 (copied verbatim) */
#line 160 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		bool
		__list_add_valid(List_s *new, List_s *prev, List_s *next) {
			while ((prev == NULL) || (next == NULL) ||
					(next->prev != prev) || (prev->next != next) ||
					(new == prev || new == next));

			return true;
		}
#line 23 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func __list_del_entry_valid lines 169-179 (copied verbatim) */
#line 169 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		bool
		__list_del_entry_valid(List_s *entry) {
			List_s	*prev = entry->prev,
					*next = entry->next;
			while ((next == NULL) || (prev == NULL) ||
					(next == LIST_POISON1) || (prev == LIST_POISON2) ||
					(prev->next != entry) || (next->prev != entry));

			return true;
		}
#line 24 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func __list_add_between lines 187-197 (copied verbatim) */
#line 187 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		void
		__list_add_between(List_s *new, List_s *prev, List_s *next) {
			if (!__list_add_valid(new, prev, next))
				return;

			next->prev = new;
			new->next = next;
			new->prev = prev;
			WRITE_ONCE(prev->next, new);
		}
#line 25 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func list_add_to_next lines 206-210 (copied verbatim) */
#line 206 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		void
		list_add_to_next(List_s *new, List_s *head) {
			__list_add_between(new, head, head->next);
		}
#line 26 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func list_add_to_prev lines 219-223 (copied verbatim) */
#line 219 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		void
		list_add_to_prev(List_s *new, List_s *head) {
			__list_add_between(new, head->prev, head);
		}
#line 27 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func __list_del lines 233-238 (copied verbatim) */
#line 233 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		void
		__list_del(List_s * prev, List_s * next) {
			next->prev = prev;
			WRITE_ONCE(prev->next, next);
		}
#line 28 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func __list_del_entry lines 253-260 (copied verbatim) */
#line 253 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		void
		__list_del_entry(List_s *entry) {
			if (!__list_del_entry_valid(entry))
				return;

			__list_del(entry->prev, entry->next);
		}
#line 29 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func list_del_init lines 278-283 (copied verbatim) */
#line 278 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		void
		list_del_init(List_s *entry) {
			__list_del_entry(entry);
			INIT_LIST_HEAD(entry);
		}
#line 30 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func list_is_head_anchor lines 419-423 (copied verbatim) */
#line 419 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		bool
		list_is_head_anchor(const List_s *list, const List_s *head) {
			return list == head;
		}
#line 31 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func list_is_empty_entry lines 428-432 (copied verbatim) */
#line 428 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		bool
		list_is_empty_entry(const List_s *head) {
			return READ_ONCE(head->next) == head;
		}
#line 32 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func list_header_is_empty lines 628-632 (copied verbatim) */
#line 628 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		bool
		list_header_is_empty(const List_hdr_s *lhdr_p) {
			return READ_ONCE(lhdr_p->count) == 0;
		}
#line 33 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func list_header_contains lines 633-642 (copied verbatim) */
#line 633 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		bool
		list_header_contains(List_hdr_s *lhdr_p, List_s *l_p) {
			List_s *tmp = NULL;
			list_header_foreach(tmp, lhdr_p) {
				if (tmp == l_p)
					return true;
			}
			return false;
		}
#line 34 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func list_header_add_to_head lines 644-649 (copied verbatim) */
#line 644 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		void
		list_header_add_to_head(List_hdr_s *lhdr_p, List_s *l_p) {
			list_add_to_next(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
		}
#line 35 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func list_header_remove_head lines 650-660 (copied verbatim) */
#line 650 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		List_s
		*list_header_remove_head(List_hdr_s *lhdr_p) {
			if (lhdr_p->count > 0) {
				List_s *ret_val = lhdr_p->anchor.next;
				list_del_init(ret_val);
				lhdr_p->count--;
				return ret_val;
			}
			return NULL;
		}
#line 36 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func list_header_add_to_tail lines 662-667 (copied verbatim) */
#line 662 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		void
		list_header_add_to_tail(List_hdr_s *lhdr_p, List_s *l_p) {
			list_add_to_prev(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
		}
#line 37 "fx_list.inc.c"
/* ORIG time:mykernel/lib/list/double_list.h func list_header_delete_node lines 681-691 (copied verbatim) */
#line 681 "mykernel/lib/list/double_list.h"
		PREFIX_STATIC_INLINE
		List_s
		*list_header_delete_node(List_hdr_s *lhdr_p, List_s *l_p) {
			if (list_header_contains(lhdr_p, l_p)) {
				list_del_init(l_p);
				lhdr_p->count--;
				return l_p;
			}

			return NULL;
		}
#line 38 "fx_list.inc.c"

/* fixture-side observers (hand-written): bounded walk, never follows more than 16 links */
static int fx_walk_len(List_hdr_s *h)
{
	int n = 0;
	List_s *p = h->anchor.next;
	while (p != &h->anchor && n < 16) { p = p->next; n++; }
	return p == &h->anchor ? n : -1;
}
static int fx_occurrences(List_hdr_s *h, List_s *node)
{
	int n = 0, k = 0;
	List_s *p = h->anchor.next;
	while (p != &h->anchor && k < 16) { if (p == node) n++; p = p->next; k++; }
	return n;
}

#line 23 "fx_order.c"

typedef struct task_struct task_s;
typedef struct runqueue rq_s;
typedef struct myos_rq myos_rq_s;
typedef struct sched_entity sched_entity_s;
typedef struct sched_rt_entity sched_rt_entity_s;
/* ORIG time:mykernel/sched/scheduler/scheduler_types.h struct sched_entity lines 8-40 (copied verbatim) */
#line 8 "mykernel/sched/scheduler/scheduler_types.h"
	struct sched_entity {
		// /* For load-balancing: */
		// struct load_weight load;
		// struct rb_node run_node;
		// List_s group_node;
		// unsigned int on_rq;

		// u64				exec_start;
		// u64				sum_exec_runtime;
		u64				vruntime;
		// u64				prev_sum_exec_runtime;
	
		// u64				nr_migrations;

	// #ifdef CONFIG_FAIR_GROUP_SCHED
		// int depth;
		// struct sched_entity *parent;
		// /* rq on which this entity is (to be) queued: */
		// struct cfs_rq *cfs_rq;
		// /* rq "owned" by this entity/group: */
		// struct cfs_rq *my_q;
		// /* cached value of my_q->h_nr_running */
		// unsigned long runnable_weight;
	// #endif

		/*
		 * Per entity load average tracking.
		 *
		 * Put into separate cache line so it does not
		 * collide with read-mostly values above.
		 */
		// struct sched_avg avg;
	};
#line 30 "fx_order.c"
/* ORIG time:mykernel/sched/scheduler/scheduler_types.h struct sched_rt_entity lines 42-58 (copied verbatim) */
#line 42 "mykernel/sched/scheduler/scheduler_types.h"
	struct sched_rt_entity {
		List_s		run_list;
		// unsigned long timeout;
		// unsigned long watchdog_stamp;
		uint		time_slice;
		// unsigned short on_rq;
		// unsigned short on_list;

		// struct sched_rt_entity *back;
	// #ifdef CONFIG_RT_GROUP_SCHED
		// struct sched_rt_entity *parent;
		// /* rq on which this entity is (to be) queued: */
		// struct rt_rq *rt_rq;
		// /* rq "owned" by this entity/group: */
		// struct rt_rq *my_q;
	// #endif
	};
#line 31 "fx_order.c"
/* ORIG time:mykernel/arch/x86_64/sched/context/thread_info_types_arch.h struct thread_info lines 19-24 (copied verbatim) */
#line 19 "mykernel/arch/x86_64/sched/context/thread_info_types_arch.h"
	struct thread_info {
		ulong	flags;		/* low level flags */
		ulong	syscall_work;	/* SYSCALL_WORK_ flags */
		u32		status;		/* thread synchronous flags */
		u32		cpu;		/* current CPU */
	};
#line 32 "fx_order.c"
/* ORIG time:mykernel/sched/runqueue/runqueue_types.h struct myos_rq lines 193-197 (copied verbatim) */
#line 193 "mykernel/sched/runqueue/runqueue_types.h"
	struct myos_rq {
		// MyOS2 variables
		List_hdr_s		running_lhdr;
		ulong			last_jiffies;	// abs jiffies when curr-task loaded
	};
#line 33 "fx_order.c"
struct task_struct {
	struct thread_info thread_info;
	uint __state;
	int on_cpu;
	sched_entity_s se;
	sched_rt_entity_s rt;
	int id;
};
struct runqueue {
	task_s *curr;
	task_s *idle;
	myos_rq_s myos;
};

/* ORIG time:mykernel/sched/task/task_const.h macro TASK_RUNNING lines 26-26 (copied verbatim) */
#line 26 "mykernel/sched/task/task_const.h"
	#define TASK_RUNNING			0x00000000
#line 48 "fx_order.c"
/* ORIG time:mykernel/sched/task/task_const.h macro TASK_UNINTERRUPTIBLE lines 28-28 (copied verbatim) */
#line 28 "mykernel/sched/task/task_const.h"
	#define TASK_UNINTERRUPTIBLE	0x00000002
#line 49 "fx_order.c"

static rq_s rq0;
static task_s *g_current;
#define current g_current
static int g_need_resched;
#define need_resched() (g_need_resched)
static ulong jiffies;
#define BUG_ON(cond) do { if (cond) STOP(EXIT_STOP_INVARIANT, "\"ev\":\"BUG_ON\",\"line\":%d", __LINE__); } while (0)

/* ORIG time:mykernel/sched/scheduler/myos_rt.c func pick_next_task_myos lines 5-58 (copied verbatim) */
#line 5 "mykernel/sched/scheduler/myos_rt.c"
static task_s *pick_next_task_myos(rq_s *rq, task_s *prev)
{
	myos_rq_s	*myos_rq = &rq->myos;
	task_s		*retval, *curr_task;
	retval = curr_task = current;
	sched_rt_entity_s *rt = &curr_task->rt;

	ulong used_jiffies = jiffies - myos_rq->last_jiffies;

	if ((need_resched() ||
		curr_task == rq->idle ||
		used_jiffies >= rt->time_slice ||
		curr_task->__state != TASK_RUNNING) &&
		myos_rq->running_lhdr.count > 0)
	{
		BUG_ON(curr_task->__state != TASK_RUNNING &&
				myos_rq->running_lhdr.count <= 0);

		// fetch a task from running_list
		List_s * next_lp = list_header_remove_head(&myos_rq->running_lhdr);
		while (next_lp == NULL);

		// and insert curr_task back to running_list
		if (curr_task->__state == TASK_RUNNING)
		{
			if (curr_task == rq->idle)
			{
				// insert idle task to cpu's running-list tail
				list_header_add_to_tail(&myos_rq->running_lhdr, &rt->run_list);
			}
			else
			{
				List_s * tmp_list = myos_rq->running_lhdr.anchor.next;
				sched_rt_entity_s *tmp_rt = container_of(tmp_list, sched_rt_entity_s, run_list);
				while ((curr_task->se.vruntime > container_of(tmp_rt, task_s, rt)->se.vruntime) &&
						tmp_list != &myos_rq->running_lhdr.anchor)
				{
					tmp_list = tmp_list->next;
				}
				list_add_to_prev(&rt->run_list, tmp_list);
				myos_rq->running_lhdr.count++;
			}
		}

		if (curr_task != rq->idle)
			curr_task->se.vruntime += used_jiffies;

		sched_rt_entity_s *next_rt = container_of(next_lp, sched_rt_entity_s, run_list);
		retval = container_of(next_rt, task_s, rt);
		myos_rq->last_jiffies = jiffies;
	}
	rq->curr = retval;
	return retval;
}
#line 59 "fx_order.c"

/* ---------------- fixture inputs and observers (hand-written) */
#define TIME_SLICE 100
static task_s TI, TA, TB, TC, TD;                 /* ids I=0, A=2, B=3, C=4, D=5 */
static task_s *const ALL[] = {&TI, &TA, &TB, &TC, &TD};
#define NALL ((int)(sizeof(ALL) / sizeof(ALL[0])))

static int id_of(const task_s *t)
{
	for (int k = 0; k < NALL; k++)
		if (t == ALL[k])
			return ALL[k]->id;
	return t ? -2 : -1;   /* -2: pointer that is not a fixture task, -1: NULL */
}
static void mk(task_s *t, int id, uint state, u64 vr)
{
	memset(t, 0, sizeof(*t));
	t->id = id; t->__state = state; t->se.vruntime = vr; t->rt.time_slice = TIME_SLICE;
	INIT_LIST_S(&t->rt.run_list);
}
static void enqueue_tail(task_s *t) { list_header_add_to_tail(&rq0.myos.running_lhdr, &t->rt.run_list); }

/* queue snapshot: compare with the anchor first, then identify the link by address among the five
 * fixture nodes; an unknown link is reported and the walk stops without dereferencing it */
static void print_queue(void)
{
	List_hdr_s *h = &rq0.myos.running_lhdr;
	int ids[16], n = 0, unknown = 0, complete = 0;
	List_s *lp = h->anchor.next;
	while (n < 16) {
		if (lp == &h->anchor) { complete = 1; break; }
		int id = -2;
		for (int k = 0; k < NALL; k++)
			if (lp == &ALL[k]->rt.run_list)
				id = ALL[k]->id;
		if (id == -2) { unknown = 1; break; }
		ids[n++] = id;
		lp = lp->next;
	}
	int dup = 0, idle_occ = 0, cur_occ = 0;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++)
			if (ids[i] == ids[j])
				dup = 1;
		if (ids[i] == TI.id) idle_occ++;
		if (current && ids[i] == current->id) cur_occ++;
	}
	printf("\"queue\":[");
	for (int i = 0; i < n; i++) {
		task_s *t = NULL;
		for (int k = 0; k < NALL; k++)
			if (ALL[k]->id == ids[i])
				t = ALL[k];
		printf("%s[%d,%llu]", i ? "," : "", ids[i], (unsigned long long)t->se.vruntime);
	}
	printf("],\"count\":%lu,\"walk_len\":%d,\"walk_complete\":%s,\"unknown_link\":%s,\"duplicate_nodes\":%s,"
	       "\"idle_occurrences\":%d,\"current_occurrences\":%d",
	       h->count, n, complete ? "true" : "false", unknown ? "true" : "false", dup ? "true" : "false", idle_occ, cur_occ);
}

static void snap(const char *where, int step)
{
	printf("{\"case\":\"%s\",\"ev\":\"snap\",\"where\":\"%s\",\"step\":%d,\"jiffies\":%lu,\"last_jiffies\":%lu,"
	       "\"used_external\":%ld,\"need_resched\":%d,\"current\":{\"id\":%d,\"state\":%u,\"vruntime\":%llu,\"time_slice\":%u},"
	       "\"rq_curr\":%d,\"rq_idle\":%d,\"tasks\":[",
	       g_case, where, step, jiffies, rq0.myos.last_jiffies, (long)(jiffies - rq0.myos.last_jiffies), g_need_resched,
	       id_of(current), current->__state, (unsigned long long)current->se.vruntime, current->rt.time_slice,
	       id_of(rq0.curr), id_of(rq0.idle));
	for (int k = 0; k < NALL; k++)
		printf("%s[%d,%llu,%u]", k ? "," : "", ALL[k]->id, (unsigned long long)ALL[k]->se.vruntime, ALL[k]->__state);
	printf("],");
	print_queue();
	printf("}\n");
	fflush(stdout);
}

static void call(int step)
{
	snap("before", step);
	task_s *r = pick_next_task_myos(&rq0, current);
	ev("\"ev\":\"call\",\"step\":%d,\"returned\":%d,\"rq_curr\":%d", step, id_of(r), id_of(rq0.curr));
	snap("after", step);
	if (id_of(r) < 0)
		STOP(EXIT_STOP_INVARIANT, "\"ev\":\"returned_not_a_fixture_task\",\"step\":%d", step);
}

/* one scenario: tasks, queue, current, clock and flag are all explicit inputs */
static void setup(u64 a_vr, uint a_state, ulong jf, int resched, int current_is_idle, u64 idle_vr)
{
	mk(&TI, 0, TASK_RUNNING, idle_vr);
	mk(&TA, 2, a_state, a_vr);
	mk(&TB, 3, TASK_RUNNING, 10);
	mk(&TC, 4, TASK_RUNNING, 20);
	mk(&TD, 5, TASK_RUNNING, 30);
	memset(&rq0, 0, sizeof(rq0));
	INIT_LIST_HEADER_S(&rq0.myos.running_lhdr);
	rq0.idle = &TI;
	rq0.myos.last_jiffies = 100;
	enqueue_tail(&TB);
	enqueue_tail(&TC);
	enqueue_tail(&TD);
	if (current_is_idle) {
		current = &TI;           /* idle is current: not queued a second time */
	} else {
		enqueue_tail(&TI);       /* idle always exists; queued when it is not current */
		current = &TA;
	}
	rq0.curr = current;
	jiffies = jf;
	g_need_resched = resched;
	ev("\"ev\":\"scenario\",\"time_slice\":%d,\"last_jiffies_initial\":%lu,\"jiffies\":%lu,\"need_resched\":%d,"
	   "\"current\":%d,\"a_state\":%u,\"a_vruntime\":%llu,\"idle_vruntime\":%llu",
	   TIME_SLICE, rq0.myos.last_jiffies, jiffies, g_need_resched, id_of(current), a_state,
	   (unsigned long long)a_vr, (unsigned long long)idle_vr);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		return 2;
	g_case = argv[1];
	int steps = 1;
	if (!strcmp(g_case, "W01")) {
		setup(25, TASK_RUNNING, 100, 1, 0, 0); call(1);
	} else if (!strcmp(g_case, "W02")) {
		setup(15, TASK_RUNNING, 110, 1, 0, 0); call(1);
	} else if (!strcmp(g_case, "W03")) {
		setup(15, TASK_RUNNING, 100, 1, 0, 0); call(1);
	} else if (!strcmp(g_case, "W04")) {
		setup(15, TASK_RUNNING, 105, 1, 0, 0); call(1);
	} else if (!strcmp(g_case, "W05")) {
		setup(15, TASK_UNINTERRUPTIBLE, 110, 1, 0, 0); call(1);
	} else if (!strcmp(g_case, "W06")) {
		setup(15, TASK_RUNNING, 110, 1, 1, 7); call(1);
	} else if (!strcmp(g_case, "W07")) {
		setup(15, TASK_RUNNING, 110, 1, 0, 0); call(1);
		current = rq0.curr;   /* sequence model: the returned task becomes current; jiffies and last_jiffies untouched */
		g_need_resched = 1;
		ev("\"ev\":\"sequence\",\"before_step\":2,\"current_set_to_returned\":%d,\"jiffies\":%lu,\"need_resched\":%d",
		   id_of(current), jiffies, g_need_resched);
		call(2); steps = 2;
	} else if (!strcmp(g_case, "W08")) {
		setup(15, TASK_RUNNING, 105, 0, 0, 0); call(1);
		current = rq0.curr;
		g_need_resched = 1;   /* same jiffies (105); last_jiffies is whatever the original function left */
		ev("\"ev\":\"sequence\",\"before_step\":2,\"current_set_to_returned\":%d,\"jiffies\":%lu,\"need_resched\":%d",
		   id_of(current), jiffies, g_need_resched);
		call(2); steps = 2;
	} else {
		return 2;
	}
	ev("\"ev\":\"end\",\"steps\":%d", steps);
	return 0;
}
