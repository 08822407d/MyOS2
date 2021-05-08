#ifndef _AMD64_PC_DEVICE_H_
#define _AMD64_PC_DEVICE_H_

#include <sys/types.h>

#include "arch_proto.h"
#include "interrupt.h"

#include "../../../include/param.h"

	extern irq_desc_s	irq_descriptors[NR_IRQ_VECS];

	/* device.c */
	void devices_init(void);

	/* keyboard.c */
	void keyboard_init(void);
	void keyboard_handler(unsigned long param, stack_frame_s * sf_regs);
	void analysis_keycode(void);

	/* clock.c, HPET.c */
	void get_cmos_time(time_s *time);
	void HPET_init(void);

#endif /* _AMD64_PC_DEVICE_H_ */