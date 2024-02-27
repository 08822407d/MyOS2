#ifndef _AMD64_PC_DEVICE_H_
#define _AMD64_PC_DEVICE_H_

#include <linux/kernel/types.h>
#include <linux/fs/fs.h>

#include <obsolete/ktypes.h>
#include "arch_proto.h"
#include "myos_irq_vectors.h"

	extern myos_irq_desc_s	irq_descriptors[NR_VECTORS];

	/* device.c */
	void myos_devices_init(void);

	/* keyboard.c */
	void init_keyboard(void);
	void keyboard_handler(unsigned long param, pt_regs_s * regs);
	char kbd_parse_scancode(void);

	/* clock.c, HPET.c */
	void get_cmos_time(time_s *time);


	/* disk.c */
	void init_IDE_disk(void);

	typedef struct blkdev_ops
	{
		long (* open)(unsigned controller, unsigned disk);
		long (* close)(unsigned controller, unsigned disk);
		long (* ioctl)(unsigned controller, unsigned disk,
						long cmd, long arg);
		long (* transfer)(unsigned controller, unsigned disk,
						long cmd, unsigned long blk_idx,
						long count, unsigned char * buffer);
	} blkdev_ops_s;
	extern blkdev_ops_s ATA_master_ops;

#endif /* _AMD64_PC_DEVICE_H_ */