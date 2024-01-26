#include <linux/kernel/slab.h>
#include <linux/lib/list.h>

#include <obsolete/printk.h>
#include <obsolete/proto.h>
#include <obsolete/glo.h>
#include <obsolete/myos_irq_vectors.h>

unsigned long volatile jiffies = 0;
List_hdr_s	timer_lhdr;

void init_timer(timer_s * timer,
				void (* func)(void * data),
				void *data,
				unsigned long expire_jiffies)
{
	list_init(&timer->tmr_list, timer);
	timer->func = func;
	timer->data = data;
	timer->expire_jiffies = jiffies + expire_jiffies;
}

int add_timer_chk(List_s * curr_lp, List_s * tartget_lp)
{
	timer_s * ct_p = (timer_s *)curr_lp->owner_p;
	timer_s * pt_p = (timer_s *)curr_lp->prev->owner_p;
	timer_s * tt_p = (timer_s *)tartget_lp->owner_p;

	int rv = 0;
	if ((tt_p->expire_jiffies > pt_p->expire_jiffies || curr_lp->prev == &timer_lhdr.header) &&
		(tt_p->expire_jiffies < ct_p->expire_jiffies))
	{
		rv = 1;
	}
	return rv;
}
int add_timer_do(List_s * curr_lp, List_s * tartget_lp)
{
	list_insert_prev(curr_lp, tartget_lp);
	timer_lhdr.count++;
}
int add_timer_end(List_s * curr_lp, List_s * tartget_lp)
{
	add_timer_do(curr_lp, tartget_lp);
}
void add_timer(timer_s * timer)
{
	list_search_and_do(&timer_lhdr, &timer->tmr_list,
						add_timer_chk, add_timer_do, add_timer_end);
}

void del_timer(timer_s * timer)
{
	if (timer->tmr_list.next != &timer_lhdr.header)
	{
		list_delete(&timer->tmr_list);
		timer_lhdr.count--;
	}
}

void test_timer(void * data)
{
	color_printk(BLUE,WHITE,"test_timer");
}

void myos_timer_init()
{
	jiffies = 0;
	list_hdr_init(&timer_lhdr);
	timer_s * tmr = kzalloc(sizeof(timer_s), GFP_KERNEL);
	init_timer(tmr, NULL, NULL, ~(unsigned long)0);
	add_timer(tmr);
	register_softirq(HPET_TIMER0_IRQ, &myos_do_timer, NULL);

	// timer_s * tmp = (timer_s *)kmalloc(sizeof(timer_s));
	// init_timer(tmp, &test_timer, NULL, 3);
	// add_timer(tmp);
}

void myos_do_timer(void * data)
{
	timer_s * tmp = timer_lhdr.header.next->owner_p;
	while (tmp->tmr_list.next != &timer_lhdr.header &&
			(tmp->expire_jiffies <= jiffies))
	{
		del_timer(tmp);
		tmp->func(tmp->data);
		tmp = timer_lhdr.header.next->owner_p;
	}

	color_printk(RED, WHITE, "(HPET:%ld)", jiffies);
}