#ifndef _AMD64_PC_DEVICE_H_
#define _AMD64_PC_DEVICE_H_

#include <linux/kernel/types.h>

#include <include/ktypes.h>
#include <linux/fs/fs.h>
#include <include/block_dev.h>

#include "arch_proto.h"
#include "interrupt.h"

	extern irq_desc_s	irq_descriptors[NR_IRQ_VECS];

	// #ifdef dev_t
	// 	#undef dev_t
	// #endif
	// typedef struct dev_nr
	// {
	// 	unsigned short	main	: 16;
	// 	unsigned long	sub		: 48;
	// } dev_t;
	
	typedef struct char_dev
	{
		List_s			cdev_list;
		char *			dev_name;
		dev_t			dev_nr;
		file_ops_s *	f_ops;
	} cdev_s;

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
	extern blkdev_ops_s IDE_device_operation;

#endif /* _AMD64_PC_DEVICE_H_ */