#include <linux/kernel/slab.h>
#include <linux/drivers/libata.h>

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

ata_iops_s ioaddr_master =
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
};

ata_iops_s ioaddr_slave =
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
};

enum {
	IDE_0_0,
	IDE_0_1,
	IDE_1_0,
	IDE_1_1,
};

ata_dev_s ide_disks[4];

long ATA_disk_transfer(unsigned controller, unsigned disk, long cmd,
		unsigned long blk_idx, long count, unsigned char * buffer);
void myos_ata_port_probe(ata_dev_s *hdd)
{
	ata_port_s *ap = &(hdd->ap);
	ata_iops_s *ioaddr = &(ap->ioaddr);
	outb(0, ioaddr->ctl_addr);

	// 	ATA_disk_transfer(MASTER, MASTER, ATA_INFO_CMD, 0, 0,
	// 					(unsigned char *)&ide_disk_info[0]);
	ATA_disk_transfer(hdd->devno, ap->port_no, ATA_CMD_STANDBYNOW1,
			0, 0, (unsigned char *)hdd->id);
}

void myos_ata_probe()
{
	ide_disks[IDE_0_0].devno = 0;
	ide_disks[IDE_0_1].devno = 0;
	ide_disks[IDE_1_0].devno = 1;
	ide_disks[IDE_1_1].devno = 1;

	ide_disks[IDE_0_0].ap.port_no = 0;
	ide_disks[IDE_0_1].ap.port_no = 1;
	ide_disks[IDE_1_0].ap.port_no = 0;
	ide_disks[IDE_1_1].ap.port_no = 1;

	ide_disks[IDE_0_0].ap.ioaddr =
	ide_disks[IDE_0_1].ap.ioaddr = ioaddr_master;
	ide_disks[IDE_1_0].ap.ioaddr =
	ide_disks[IDE_1_1].ap.ioaddr = ioaddr_slave;

	for (int i = 0; i < 4; i++)
	{
		ata_dev_s *hdd = &ide_disks[i];
		ata_port_s *ap = &(hdd->ap);
		ap->ops = (ata_port_ops_s *)&ata_sff_port_ops;
		if (ata_devchk(ap, ap->port_no))
			myos_ata_port_probe(hdd);
	}
}