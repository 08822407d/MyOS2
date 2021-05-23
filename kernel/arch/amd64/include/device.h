#ifndef _AMD64_PC_DEVICE_H_
#define _AMD64_PC_DEVICE_H_

#include <sys/types.h>

#include "arch_proto.h"
#include "interrupt.h"

#include "../../../include/param.h"
#include "../../../include/block.h"

	extern irq_desc_s	irq_descriptors[NR_IRQ_VECS];

	/* device.c */
	void devices_init(void);

	/* keyboard.c */
	void init_keyboard(void);
	void keyboard_handler(unsigned long param, stack_frame_s * sf_regs);
	void analysis_keycode(void);

	/* clock.c, HPET.c */
	void get_cmos_time(time_s *time);
	void HPET_init(void);

	/* disk.c */
	void init_disk(void);
	extern blkdev_ops_s IDE_device_operation;
#endif /* _AMD64_PC_DEVICE_H_ */