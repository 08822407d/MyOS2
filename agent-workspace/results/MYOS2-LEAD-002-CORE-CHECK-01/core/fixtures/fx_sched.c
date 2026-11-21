/* fx_sched: host fixture for V04-V08 (try_to_wake_up / set_task_cpu / wake_up_new_task /
 * pick_next_task_myos). Original code is inserted verbatim at //@@ORIG lines by build.py.
 * Hand-written replacements (evidence.md lists them):
 *   struct task_struct and struct runqueue reduced to the members these functions touch (member
 *   types taken from the original sub-structs, which are themselves copied verbatim);
 *   per_cpu(runqueues, cpu) -> array element; current; preempt_disable/enable (counter);
 *   smp_rmb (compiler barrier); need_resched() (fixture flag); jiffies (fixture variable);
 *   BUG_ON (record + stop).
 */
#include "fx_common.h"

//@@INCLUDE fx_list.inc.c

typedef struct task_struct task_s;
typedef struct runqueue rq_s;
typedef struct myos_rq myos_rq_s;
typedef struct sched_entity sched_entity_s;
typedef struct sched_rt_entity sched_rt_entity_s;
//@@ORIG time mykernel/sched/scheduler/scheduler_types.h struct sched_entity
//@@ORIG time mykernel/sched/scheduler/scheduler_types.h struct sched_rt_entity
//@@ORIG time mykernel/arch/x86_64/sched/context/thread_info_types_arch.h struct thread_info
//@@ORIG time mykernel/sched/runqueue/runqueue_types.h struct myos_rq
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

//@@ORIG time mykernel/sched/task/task_const.h macro TASK_RUNNING
//@@ORIG time mykernel/sched/task/task_const.h macro TASK_INTERRUPTIBLE
//@@ORIG time mykernel/sched/task/task_const.h macro TASK_UNINTERRUPTIBLE
//@@ORIG time mykernel/sched/task/task_const.h macro __TASK_STOPPED
//@@ORIG time mykernel/sched/task/task_const.h macro TASK_WAKING
//@@ORIG time mykernel/sched/task/task_const.h macro TASK_NEW
//@@ORIG time mykernel/sched/task/task_const.h macro TASK_NORMAL
//@@ORIG time mykernel/sched/scheduler/scheduler_const.h macro WF_TTWU
//@@ORIG time mykernel/sched/misc/sched_misc_api.h macro task_thread_info

#define NCPU 4
static rq_s runqueues[NCPU];
#define per_cpu(var, cpu) ((var)[(cpu)])
static task_s *g_current;
#define current g_current
static int g_preempt, g_need_resched;
#define preempt_disable() do { g_preempt++; } while (0)
#define preempt_enable() do { g_preempt--; } while (0)
#define smp_rmb() barrier()
#define need_resched() (g_need_resched)
static ulong jiffies;
#define BUG_ON(cond) do { if (cond) STOP(EXIT_STOP_INVARIANT, "\"ev\":\"BUG_ON\",\"line\":%d", __LINE__); } while (0)

//@@ORIG time mykernel/sched/scheduler/scheduler.h func task_cpu
//@@ORIG time mykernel/sched/scheduler/scheduler.h func select_task_rq
//@@ORIG time mykernel/sched/scheduler/scheduler_core.c func set_task_cpu
//@@ORIG time mykernel/sched/scheduler/scheduler_core.c func try_to_wake_up
//@@ORIG time mykernel/sched/scheduler/scheduler_core.c func wake_up_process
//@@ORIG time mykernel/sched/scheduler/scheduler_core.c func wake_up_new_task
//@@ORIG time mykernel/sched/scheduler/myos_rt.c func pick_next_task_myos

/* ---------------- fixture helpers */
static task_s T[8];
static const char *tf(bool b) { return b ? "true" : "false"; }
static void mk(task_s *t, int id, uint state, uint cpu, u64 vr)
{
	memset(t, 0, sizeof(*t));
	t->id = id; t->__state = state; t->thread_info.cpu = cpu; t->se.vruntime = vr;
	t->rt.time_slice = 5;
	INIT_LIST_S(&t->rt.run_list);
}
static void reset(void)
{
	for (int c = 0; c < NCPU; c++) {
		memset(&runqueues[c], 0, sizeof(runqueues[c]));
		INIT_LIST_HEADER_S(&runqueues[c].myos.running_lhdr);
	}
	g_preempt = 0; g_need_resched = 0; jiffies = 100;
}
static void order(char *buf, size_t n, List_hdr_s *h)
{
	size_t k = 0; int guard = 0;
	buf[0] = 0;
	/* loop variable must not be named p: the original container_of declares its own local p */
	for (List_s *lp = h->anchor.next; lp != &h->anchor && guard < 16; lp = lp->next, guard++) {
		sched_rt_entity_s *rt = container_of(lp, sched_rt_entity_s, run_list);
		task_s *t = container_of(rt, task_s, rt);
		k += snprintf(buf + k, n - k, "%s%d", k ? "," : "", t->id);
	}
}
static void qstate(const char *step, int cpu, task_s *t)
{
	List_hdr_s *h = &runqueues[cpu].myos.running_lhdr;
	char ob[128];
	order(ob, sizeof(ob), h);
	ev("\"ev\":\"q\",\"step\":\"%s\",\"cpu\":%d,\"count\":%lu,\"walk_len\":%d,\"order\":[%s],"
	   "\"task\":%d,\"task_state\":%u,\"task_occ\":%d,\"task_cpu_meta\":%u,\"preempt\":%d",
	   step, cpu, h->count, fx_walk_len(h), ob, t ? t->id : -1, t ? t->__state : 0,
	   t ? fx_occurrences(h, &t->rt.run_list) : -1, t ? task_cpu(t) : 0, g_preempt);
}
static void enqueue_tail(int cpu, task_s *t)
{
	list_header_add_to_tail(&runqueues[cpu].myos.running_lhdr, &t->rt.run_list);
}
static void pick(const char *name, task_s *idle)
{
	rq_s *rq = &runqueues[0];
	rq->idle = idle; rq->curr = current;
	rq->myos.last_jiffies = jiffies;
	char before[128];
	order(before, sizeof(before), &rq->myos.running_lhdr);
	ev("\"ev\":\"pick_in\",\"name\":\"%s\",\"current\":%d,\"current_state\":%u,\"current_is_idle\":%s,"
	   "\"need_resched\":%d,\"order_before\":[%s],\"count_before\":%lu", name, current->id,
	   current->__state, tf(current == idle), g_need_resched, before, rq->myos.running_lhdr.count);
	task_s *r = pick_next_task_myos(rq, current);
	char after[128];
	order(after, sizeof(after), &rq->myos.running_lhdr);
	ev("\"ev\":\"pick_out\",\"name\":\"%s\",\"returned\":%d,\"returned_is_current\":%s,\"rq_curr\":%d,"
	   "\"order_after\":[%s],\"count_after\":%lu,\"walk_len_after\":%d,\"idle_in_list\":%s", name, r->id,
	   tf(r == current), rq->curr->id, after, rq->myos.running_lhdr.count,
	   fx_walk_len(&rq->myos.running_lhdr), tf(fx_occurrences(&rq->myos.running_lhdr, &idle->rt.run_list) > 0));
	current = r; /* fixture models the context switch by making the returned task current */
}

int main(int argc, char **argv)
{
	if (argc < 2)
		return 2;
	g_case = argv[1];
	reset();
	task_s *W = &T[0], *A = &T[1], *B = &T[2], *C = &T[3], *IDLE = &T[7];
	mk(W, 1, TASK_RUNNING, 0, 0);
	current = W;
	if (!strcmp(g_case, "v04_noncurrent_wake")) {
		mk(A, 2, TASK_UNINTERRUPTIBLE, 0, 0);
		qstate("before", 0, A);
		int r = wake_up_process(A);
		ev("\"ev\":\"ret\",\"fn\":\"wake_up_process\",\"ret\":%d", r);
		qstate("after", 0, A);
	} else if (!strcmp(g_case, "v05_state_not_in_mask")) {
		struct { const char *label; uint st; uint mask; int use_wup; } v[] = {
			{"stopped_via_wake_up_process", __TASK_STOPPED, TASK_NORMAL, 1},
			{"uninterruptible_mask_interruptible", TASK_UNINTERRUPTIBLE, TASK_INTERRUPTIBLE, 0},
			{"task_new_via_wake_up_process", TASK_NEW, TASK_NORMAL, 1},
			{"running_not_queued_via_wake_up_process", TASK_RUNNING, TASK_NORMAL, 1},
		};
		for (unsigned k = 0; k < sizeof(v) / sizeof(v[0]); k++) {
			reset(); current = W;
			mk(A, 2, v[k].st, 0, 0);
			int r = v[k].use_wup ? wake_up_process(A) : try_to_wake_up(A, v[k].mask, 0);
			ev("\"ev\":\"mask_case\",\"label\":\"%s\",\"state_in\":%u,\"mask\":%u,\"state_in_mask\":%s,\"ret\":%d",
			   v[k].label, v[k].st, v[k].mask, tf(v[k].st & v[k].mask), r);
			qstate(v[k].label, 0, A);
		}
		reset();
		mk(A, 2, TASK_UNINTERRUPTIBLE, 0, 0);
		current = A;
		int r = try_to_wake_up(A, TASK_INTERRUPTIBLE, 0);
		ev("\"ev\":\"mask_case\",\"label\":\"current_uninterruptible_mask_interruptible\",\"state_in\":%u,"
		   "\"mask\":%u,\"state_in_mask\":false,\"ret\":%d", (uint)TASK_UNINTERRUPTIBLE, (uint)TASK_INTERRUPTIBLE, r);
		qstate("current_path", 0, A);
	} else if (!strcmp(g_case, "v06_double_wake")) {
		mk(A, 2, TASK_UNINTERRUPTIBLE, 0, 0);
		int r1 = wake_up_process(A);
		qstate("after_first", 0, A);
		A->__state = TASK_UNINTERRUPTIBLE; /* target blocks again before it ever ran (still queued) */
		int r2 = wake_up_process(A);
		qstate("after_second", 0, A);
		int r3 = wake_up_process(A);   /* third call while queued and TASK_RUNNING */
		qstate("after_third", 0, A);
		ev("\"ev\":\"rets\",\"r1\":%d,\"r2\":%d,\"r3\":%d", r1, r2, r3);
	} else if (!strcmp(g_case, "v07_cpu_metadata")) {
		mk(A, 2, TASK_UNINTERRUPTIBLE, 3, 0);
		set_task_cpu(A, 1);
		qstate("set_task_cpu_A_to_1_rq1", 1, A);
		mk(B, 3, TASK_UNINTERRUPTIBLE, 2, 0);
		int r = wake_up_process(B);
		ev("\"ev\":\"ret\",\"fn\":\"wake_up_process\",\"ret\":%d", r);
		qstate("wake_B_meta2_rq0", 0, B);
		qstate("wake_B_meta2_rq2", 2, B);
		mk(C, 4, TASK_NEW, 3, 0);
		wake_up_new_task(C);
		qstate("new_task_C_meta3_rq0", 0, C);
		qstate("new_task_C_meta3_rq3", 3, C);
	} else if (!strcmp(g_case, "v08_pick_combinations")) {
		mk(IDLE, 0, TASK_RUNNING, 0, 0);
		/* (a) current runnable, empty queue */
		reset(); mk(A, 2, TASK_RUNNING, 0, 0); current = A; g_need_resched = 1; pick("a_running_empty", IDLE);
		/* (b) current blocked, empty queue */
		reset(); mk(A, 2, TASK_UNINTERRUPTIBLE, 0, 0); current = A; pick("b_blocked_empty", IDLE);
		/* (c) current blocked, one runnable replacement */
		reset(); mk(A, 2, TASK_UNINTERRUPTIBLE, 0, 0); mk(B, 3, TASK_RUNNING, 0, 0); enqueue_tail(0, B);
		current = A; pick("c_blocked_one_runnable", IDLE);
		/* (d) idle current, one runnable: idle is re-queued at the tail */
		reset(); mk(IDLE, 0, TASK_RUNNING, 0, 0); mk(B, 3, TASK_RUNNING, 0, 0); enqueue_tail(0, B);
		current = IDLE; pick("d_idle_one_runnable", IDLE);
		/* (e) blocked non-idle current, queue holds only idle */
		reset(); mk(IDLE, 0, TASK_RUNNING, 0, 0); mk(A, 2, TASK_UNINTERRUPTIBLE, 0, 0); enqueue_tail(0, IDLE);
		current = A; pick("e_blocked_queue_only_idle", IDLE);
	} else if (!strcmp(g_case, "v08_sequence_idle_requeue")) {
		/* original pick applied repeatedly; the fixture makes the returned task current */
		mk(IDLE, 0, TASK_RUNNING, 0, 0); mk(A, 2, TASK_RUNNING, 0, 0); mk(B, 3, TASK_RUNNING, 0, 0);
		current = IDLE;
		wake_up_new_task(A); wake_up_new_task(B);
		pick("s1_idle_switches_out", IDLE);
		current->__state = TASK_UNINTERRUPTIBLE; pick("s2_first_blocks", IDLE);
		current->__state = TASK_UNINTERRUPTIBLE; pick("s3_second_blocks", IDLE);
	} else if (!strcmp(g_case, "v08_sequence_idle_switched_out_blocked")) {
		/* precondition that breaks the invariant: idle is switched out while not TASK_RUNNING */
		mk(IDLE, 0, TASK_RUNNING, 0, 0); mk(A, 2, TASK_RUNNING, 0, 0);
		current = IDLE;
		wake_up_new_task(A);
		IDLE->__state = TASK_UNINTERRUPTIBLE;
		pick("t1_blocked_idle_switches_out", IDLE);
		current->__state = TASK_UNINTERRUPTIBLE; pick("t2_only_task_blocks", IDLE);
	} else if (!strcmp(g_case, "v08_vruntime_requeue_order")) {
		mk(A, 2, TASK_RUNNING, 0, 25); mk(B, 3, TASK_RUNNING, 0, 10); mk(C, 4, TASK_RUNNING, 0, 20);
		task_s *D = &T[4]; mk(D, 5, TASK_RUNNING, 0, 30);
		enqueue_tail(0, B); enqueue_tail(0, C); enqueue_tail(0, D);
		mk(IDLE, 0, TASK_RUNNING, 0, 0);
		current = A; g_need_resched = 1;
		pick("f_running_requeue_by_vruntime", IDLE);
	} else {
		return 2;
	}
	return 0;
}
