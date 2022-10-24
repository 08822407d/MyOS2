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
#include <linux/kernel/types.h>

#include <obsolete/proto.h>

uint64_t	softirq_status = 0;
softirq_s	softirq_vector[64] = {0};

void set_softirq_status(unsigned sirq)
{
	softirq_status |= (1 << sirq);
}

uint64_t get_softirq_status()
{
	return softirq_status;
}

void register_softirq(unsigned sirq,void (*action)(void * data),void * data)
{
	softirq_vector[sirq].action = action;
	softirq_vector[sirq].data = data;
}

void unregister_softirq(unsigned sirq)
{
	softirq_vector[sirq].action = NULL;
	softirq_vector[sirq].data = NULL;
}

void do_softirq()
{
	unsigned sirq;
	for(sirq = 0; sirq < 64 && softirq_status; sirq++)
	{
		if(softirq_status & (1 << sirq))
		{
			softirq_vector[sirq].action(softirq_vector[sirq].data);
			softirq_status &= ~(1 << sirq);
		}
	}
}

void myos_softirq_init()
{
	softirq_status = 0;
}