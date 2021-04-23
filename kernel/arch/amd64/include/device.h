#ifndef _ARCH_DEVICE_H_
#define _ARCH_DEVICE_H_

#include <sys/types.h>

#include "arch_proto.h"
#include "interrupt.h"

	extern irq_desc_s	irq_descriptors[NR_IRQ_VECS];

	/* device.c */
	void devices_init(void);

	/* keyboard.c */
	void keyboard_init(void);
	void keyboard_handler(unsigned long param, stack_frame_s * sf_regs);


#endif /* _ARCH_DEVICE_H_ */