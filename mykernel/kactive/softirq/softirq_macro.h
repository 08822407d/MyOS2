#ifndef _SOFTIRQ_MACRO_H_
#define _SOFTIRQ_MACRO_H_

	#define local_softirq_pending_ref   pcpu_hot.softirq_pending

	#define local_softirq_pending()		(__this_cpu_read(local_softirq_pending_ref))
	#define set_softirq_pending(x)		(__this_cpu_write(local_softirq_pending_ref, (x)))
	#define or_softirq_pending(x)		(__this_cpu_or(local_softirq_pending_ref, (x)))

#endif /* _SOFTIRQ_MACRO_H_ */