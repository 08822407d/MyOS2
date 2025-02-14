#ifndef _SOFTIRQ_CONSTS_H_
#define _SOFTIRQ_CONSTS_H_

	/*
	 * PLEASE, avoid to allocate new softirqs, if you need not _really_ high
	 * frequency threaded job scheduling. For almost all the purposes
	 * tasklets are more than enough. F.e. all serial device BHs et
	 * al. should be converted to tasklets, not to softirqs.
	*/

	enum {
		HI_SOFTIRQ=0,
		TIMER_SOFTIRQ,
		NET_TX_SOFTIRQ,
		NET_RX_SOFTIRQ,
		BLOCK_SOFTIRQ,
		IRQ_POLL_SOFTIRQ,
		TASKLET_SOFTIRQ,
		SCHED_SOFTIRQ,
		HRTIMER_SOFTIRQ,
		RCU_SOFTIRQ,		/* Preferable RCU should always be the last softirq */

		NR_SOFTIRQS
	};

#endif /* _SOFTIRQ_CONSTS_H_ */