#include <linux/drivers/libata.h>

ata_ioports_s ioaddr_master =
{
	.cmd_addr		= (void *)0x1f0,
	.data_addr		= (void *)0x1f0,
	.error_addr		= (void *)0x1f1,
	.feature_addr	= (void *)0x1f1,
	.nsect_addr		= (void *)0x1f2,
	.lbal_addr		= (void *)0x1f3,
	.lbam_addr		= (void *)0x1f4,
	.lbah_addr		= (void *)0x1f5,
	.device_addr	= (void *)0x1f6,
	.status_addr	= (void *)0x1f7,
	.command_addr	= (void *)0x1f7,
	.altstatus_addr	= (void *)0x3f6,
	.ctl_addr		= (void *)0x3f6,
};

ata_ioports_s ioaddr_slave =
{
	.cmd_addr		= (void *)0x170,
	.data_addr		= (void *)0x170,
	.error_addr		= (void *)0x171,
	.feature_addr	= (void *)0x171,
	.nsect_addr		= (void *)0x172,
	.lbal_addr		= (void *)0x173,
	.lbam_addr		= (void *)0x174,
	.lbah_addr		= (void *)0x175,
	.device_addr	= (void *)0x176,
	.status_addr	= (void *)0x177,
	.command_addr	= (void *)0x177,
	.altstatus_addr	= (void *)0x376,
	.ctl_addr		= (void *)0x376,
};

enum {
	IDE_0_0,
	IDE_0_1,
	IDE_1_0,
	IDE_1_1,
};

ata_port_s ide_ports[4];

void myos_ata_port_probe(ata_port_s *ap)
{

}

void myos_ata_probe()
{
	ide_ports[IDE_0_0].port_no = 0;
	ide_ports[IDE_0_1].port_no = 1;
	ide_ports[IDE_1_0].port_no = 0;
	ide_ports[IDE_1_1].port_no = 1;

	ide_ports[IDE_0_0].ioaddr =
	ide_ports[IDE_0_1].ioaddr = ioaddr_master;
	ide_ports[IDE_1_0].ioaddr =
	ide_ports[IDE_1_1].ioaddr = ioaddr_slave;

	for (int i = 0; i < 4; i++)
	{
		ata_port_s *ap = &ide_ports[i];
		ap->ops = (ata_port_ops_s *)&ata_sff_port_ops;
		if (ata_devchk(ap, ap->port_no))
			myos_ata_port_probe(ap);
	}
}