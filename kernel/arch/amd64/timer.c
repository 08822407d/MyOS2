#include <lib/stddef.h>
#include <lib/utils.h>

#include "include/device.h"

#include "../../include/printk.h"
#include "../../include/glo.h"
#include "../../klib/data_structure.h"


unsigned long volatile jiffies = 0;
timer_list_s timer_list_head;

void init_timer(timer_list_s * timer,
				void (* func)(void * data),
				void *data,
				unsigned long expire_jiffies)
{
	m_list_init(timer);
	timer->func = func;
	timer->data = data;
	timer->expire_jiffies = jiffies + expire_jiffies;
}

void add_timer(timer_list_s * timer)
{
	timer_list_s * tmp = timer->next;

	if(timer_list_head.next = timer_list_head.prev = NULL)
	{
		
	}
	else
	{
		while(tmp->expire_jiffies < timer->expire_jiffies)
			tmp = tmp->next;
	}
	m_list_insert_back(timer, tmp);
}

void del_timer(timer_list_s * timer)
{
	m_list_delete(timer);
}

void test_timer(void * data)
{
	color_printk(BLUE,WHITE,"test_timer");
}

void timer_init()
{
	timer_list_s *tmp = NULL;
	jiffies = 0;
	init_timer(&timer_list_head,NULL,NULL,-1UL);
	register_softirq(0,&do_timer,NULL);

	tmp = (struct timer_list_s *)kmalloc(sizeof(timer_list_s),0);
	init_timer(tmp,&test_timer,NULL,5);
	add_timer(tmp);
}

void do_timer(void * data)
{
	timer_list_s * tmp = timer_list_head.next;
	while(timer_list_head.prev != NULL &&
			timer_list_head.next != NULL &&
			(tmp->expire_jiffies <= jiffies))
	{
		del_timer(tmp);
		tmp->func(tmp->data);
		tmp = timer_list_head.next;
	}

	color_printk(RED,WHITE,"(HPET:%ld)",jiffies);
}