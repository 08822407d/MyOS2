/***************************************************
*		版权声明
*
*	本操作系统名为：MINE
*	该操作系统未经授权不得以盈利或非盈利为目的进行开发，
*	只允许个人学习以及公开交流使用
*
*	代码最终所有权及解释权归田宇所有；
*
*	本模块作者：	田宇
*	EMail:		345538255@qq.com
*
*
***************************************************/
#include <lib/string.h>
#include <lib/stddef.h>
#include <sys/types.h>

#include "include/proto.h"

uint64_t	softirq_status = 0;
softirq_s	softirq_vector[64] = {0};

void set_softirq_status(unsigned irq)
{
	softirq_status |= (1 << irq);
}

uint64_t get_softirq_status()
{
	return softirq_status;
}

void register_softirq(unsigned irq,void (*action)(void * data),void * data)
{
	softirq_vector[irq].action = action;
	softirq_vector[irq].data = data;
}

void unregister_softirq(unsigned irq)
{
	softirq_vector[irq].action = NULL;
	softirq_vector[irq].data = NULL;
}

void do_softirq()
{
	unsigned irq;
	for(irq = 0; irq < 64 && softirq_status; irq++)
	{
		if(softirq_status & (1 << irq))
		{
			softirq_vector[irq].action(softirq_vector[irq].data);
			softirq_status &= ~(1 << irq);
		}
	}
}

void softirq_init()
{
	softirq_status = 0;
	memset(softirq_vector, 0, sizeof(softirq_s) * 64);
}