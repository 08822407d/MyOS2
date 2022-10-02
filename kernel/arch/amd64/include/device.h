#ifndef _AMD64_PC_DEVICE_H_
#define _AMD64_PC_DEVICE_H_

#include <linux/kernel/types.h>
#include <linux/fs/fs.h>

#include <obsolete/ktypes.h>
#include <obsolete/block_dev.h>
#include "arch_proto.h"
#include "interrupt.h"

	extern irq_desc_s	irq_descriptors[NR_IRQ_VECS];

	/* device.c */
	void devices_init(void);
	void creat_dev_file(void);

	/* keyboard.c */
	void init_keyboard(void);
	void keyboard_handler(unsigned long param, stack_frame_s * sf_regs);
	char kbd_parse_scancode(void);

	/* clock.c, HPET.c */
	void get_cmos_time(time_s *time);
	void HPET_init(void);


	/* disk.c */
	void init_disk(void);
	extern blkdev_ops_s ATA_master_ops;

#endif /* _AMD64_PC_DEVICE_H_ */