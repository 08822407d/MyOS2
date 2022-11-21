#include <linux/kernel/slab.h>
#include <linux/drivers/libata.h>
#include <linux/block/genhd.h>
#include <linux/block/blkdev.h>
#include <uapi/kernel/major.h>

#include "libata.h"


#define MAX_IDE_DEV_NR (MYOS_ATA_MAX_PORTS * ATA_MAX_DEVICES)
#define MASTER	0
#define SLAVE	1

// ata_iops_s ioaddr_master =
// {
// 	.cmd_addr		= (void *)0x1f0,
// 	.data_addr		= (void *)0x1f0,
// 	.error_addr		= (void *)0x1f1,
// 	.feature_addr	= (void *)0x1f1,
// 	.nsect_addr		= (void *)0x1f2,
// 	.lbal_addr		= (void *)0x1f3,
// 	.lbam_addr		= (void *)0x1f4,
// 	.lbah_addr		= (void *)0x1f5,
// 	.device_addr	= (void *)0x1f6,
// 	.status_addr	= (void *)0x1f7,
// 	.command_addr	= (void *)0x1f7,
// 	.altstatus_addr	= (void *)0x3f6,
// 	.ctl_addr		= (void *)0x3f6,
// };

// ata_iops_s ioaddr_slave =
// {
// 	.cmd_addr		= (void *)0x170,
// 	.data_addr		= (void *)0x170,
// 	.error_addr		= (void *)0x171,
// 	.feature_addr	= (void *)0x171,
// 	.nsect_addr		= (void *)0x172,
// 	.lbal_addr		= (void *)0x173,
// 	.lbam_addr		= (void *)0x174,
// 	.lbah_addr		= (void *)0x175,
// 	.device_addr	= (void *)0x176,
// 	.status_addr	= (void *)0x177,
// 	.command_addr	= (void *)0x177,
// 	.altstatus_addr	= (void *)0x376,
// 	.ctl_addr		= (void *)0x376,
// };

#define atasff_pops (ata_port_ops_s *)&ata_sff_port_ops
ata_port_s	ide_ports[MYOS_ATA_MAX_PORTS] = {
	//	MASTER CONTROLLER
	{
		.port_no	= MASTER,
		.ops		= atasff_pops,
		.ioaddr		=
		{
			.cmd_addr		= 0x1f0,
			.data_addr		= 0x1f0,
			.error_addr		= 0x1f1,
			.feature_addr	= 0x1f1,
			.nsect_addr		= 0x1f2,
			.lbal_addr		= 0x1f3,
			.lbam_addr		= 0x1f4,
			.lbah_addr		= 0x1f5,
			.device_addr	= 0x1f6,
			.status_addr	= 0x1f7,
			.command_addr	= 0x1f7,
			.altstatus_addr	= 0x3f6,
			.ctl_addr		= 0x3f6,
		},
	},
	//	SLAVE CONTROLLER
	{
		.port_no	= SLAVE,
		.ops		= atasff_pops,
		.ioaddr		=
		{
			.cmd_addr		= 0x170,
			.data_addr		= 0x170,
			.error_addr		= 0x171,
			.feature_addr	= 0x171,
			.nsect_addr		= 0x172,
			.lbal_addr		= 0x173,
			.lbam_addr		= 0x174,
			.lbah_addr		= 0x175,
			.device_addr	= 0x176,
			.status_addr	= 0x177,
			.command_addr	= 0x177,
			.altstatus_addr	= 0x376,
			.ctl_addr		= 0x376,
		},
	},
};

ata_dev_s	ide_devs[MYOS_ATA_MAX_PORTS][ATA_MAX_DEVICES] = {
	//	MASTER CONTROLLER
	{
		// MASTER DISK
		{
			.devno	= MASTER,
			.name	= "hda",
			.ap		= &(ide_ports[MASTER]),
		},
		// SLAVE DISK
		{
			.devno	= SLAVE,
			.name	= "hdb",
			.ap		= &(ide_ports[MASTER]),
		}
	},
	//	SLAVE CONTROLLER
	{
		// MASTER DISK
		{
			.devno	= MASTER,
			.name	= "hdc",
			.ap		= &(ide_ports[SLAVE]),
		},
		// SLAVE DISK
		{
			.devno	= SLAVE,
			.name	= "hdd",
			.ap		= &(ide_ports[SLAVE]),
		}
	}
};


static const blk_dev_ops_s ide_fops = {

};

long ATA_disk_transfer(unsigned controller, unsigned disk, long cmd,
		unsigned long blk_idx, long count, unsigned char * buffer);
void myos_ata_port_probe(ata_dev_s *hdd)
{
	gendisk_s *gd;
	u16 *ataid = hdd->id;
	ata_port_s *ap = hdd->ap;
	ata_iops_s *ioaddr = &(ap->ioaddr);
	outb(0, ioaddr->ctl_addr);

	gd = kzalloc(sizeof(gendisk_s), GFP_KERNEL);
	gd->part0 = kzalloc(sizeof(blk_dev_s), GFP_KERNEL);
	memcpy(gd->disk_name, hdd->name, strlen(hdd->name));
	if (ap->port_no == MASTER)
		gd->major = IDE0_MAJOR;
	else if (ap->port_no == SLAVE)
		gd->major = IDE1_MAJOR;
	else
		while (1);
	gd->first_minor = 0;
	gd->minors = MAX_IDE_DEV_NR;	
	gd->fops = &ide_fops;

	// 	ATA_disk_transfer(MASTER, MASTER, ATA_INFO_CMD, 0, 0,
	// 					(unsigned char *)&ide_disk_info[0]);
	ATA_disk_transfer(ap->port_no, hdd->devno, ATA_CMD_STANDBYNOW1,
			0, 0, (unsigned char *)ataid);
	ata_dev_configure(hdd);
}

void myos_ata_probe()
{
	for (int i = 0; i < MYOS_ATA_MAX_PORTS; i++)
	{
		ata_port_s *ap = &(ide_ports[i]);
		for (int j = 0; j < ATA_MAX_DEVICES; j++)
		{
			if (ata_devchk(ap, ap->port_no))
			{
				ata_dev_s *hdd = &(ide_devs[i][j]);
				myos_ata_port_probe(hdd);
			}
		}
	}
}