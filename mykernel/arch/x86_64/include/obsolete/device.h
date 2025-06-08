#ifndef _AMD64_PC_DEVICE_H_
#define _AMD64_PC_DEVICE_H_

#include <linux/kernel/types.h>
#include <linux/fs/fs.h>

#include <obsolete/ktypes.h>
#include <arch/x86_64/myos/MineOS_NVMe.h>
#include <obsolete/ide.h>

#include "arch_proto.h"
#include "myos_irq_vectors.h"

#define ROOTBLK_NVME 1


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
	extern blkdev_ops_s NVMe_ops;
	extern int bootdisk_LBAsize;

#ifdef ROOTBLK_NVME
	#define ROOTBLK_TRANSFER(cmd, blk_idx, count, buffer)	\
				NVMe_ops.transfer(0, 0, cmd, blk_idx, count, buffer)
	#define CMD_READ		NVM_CMD_READ
	#define CMD_WRITE		NVM_CMD_WRITE
	#define SECT_PER_PG		(SZ_4K / bootdisk_LBAsize)
	// #define SECTOR_SIZE		SZ_4K
#else
	#define ROOTBLK_TRANSFER(cmd, blk_idx, count, buffer)	\
				ATA_master_ops.transfer(MASTER, SLAVE, cmd, blk_idx, count, buffer)
	#define CMD_READ		ATA_READ_CMD
	#define CMD_WRITE		ATA_WRITE_CMD
	// #define SECTOR_SIZE		SZ_512
#endif


#endif /* _AMD64_PC_DEVICE_H_ */