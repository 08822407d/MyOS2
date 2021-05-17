#include <lib/stddef.h>
#include <lib/utils.h>

#include "arch/amd64/include/interrupt.h"


#include "include/printk.h"
#include "include/proto.h"
#include "include/glo.h"
#include "klib/data_structure.h"


unsigned long volatile jiffies = 0;
timer_s timer_list_head;

void init_timer(timer_s * timer,
				void (* func)(void * data),
				void *data,
				unsigned long expire_jiffies)
{
	m_list_init(timer);
	timer->func = func;
	timer->data = data;
	timer->expire_jiffies = jiffies + expire_jiffies;
}

void add_timer(timer_s * timer)
{
	timer_s * tmp = timer_list_head.next;

	if(timer_list_head.next == NULL &&
		timer_list_head.prev == NULL)
	{
		
	}
	else
	{
		while(tmp->expire_jiffies < timer->expire_jiffies)
			tmp = tmp->next;
	}
	__m_list_insert_back(timer, tmp);
}

void del_timer(timer_s * timer)
{
	m_list_delete(timer);
}

void test_timer(void * data)
{
	color_printk(BLUE,WHITE,"test_timer");
}

void timer_init()
{
	timer_s *tmp = NULL;
	jiffies = 0;
	init_timer(&timer_list_head, NULL, NULL, -1UL);
	register_softirq(HPET_TIMER0_IRQ, &do_timer, NULL);

	// tmp = (timer_s *)kmalloc(sizeof(timer_s));
	// init_timer(tmp, &test_timer, NULL, 5);
	// add_timer(tmp);
}

void do_timer(void * data)
{
	timer_s * tmp = timer_list_head.next;
	while(timer_list_head.prev != NULL &&
			timer_list_head.next != NULL &&
			(tmp->expire_jiffies <= jiffies))
	{
		del_timer(tmp);
		tmp->func(tmp->data);
		tmp = timer_list_head.next;
	}

	color_printk(RED, WHITE, "(HPET:%ld)", jiffies);
}