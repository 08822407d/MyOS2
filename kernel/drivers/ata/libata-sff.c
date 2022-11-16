// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  libata-sff.c - helper library for PCI IDE BMDMA
 *
 *  Copyright 2003-2006 Red Hat, Inc.  All rights reserved.
 *  Copyright 2003-2006 Jeff Garzik
 *
 *  libata documentation is available via 'make {ps|pdf}docs',
 *  as Documentation/driver-api/libata.rst
 *
 *  Hardware documentation available from http://www.t13.org/ and
 *  http://www.sata-io.org/
 */

#include <linux/kernel/kernel.h>
#include <linux/mm/gfp.h>
// #include <linux/pci.h>
// #include <linux/module.h>
#include <linux/drivers/libata.h>
// #include <linux/highmem.h>
// #include <trace/events/libata.h>
#include "libata.h"


const struct ata_port_operations ata_sff_port_ops = {
	// .inherits			= &ata_base_port_ops,

	// .qc_prep			= ata_noop_qc_prep,
	// .qc_issue			= ata_sff_qc_issue,
	// .qc_fill_rtf		= ata_sff_qc_fill_rtf,

	// .freeze				= ata_sff_freeze,
	// .thaw				= ata_sff_thaw,
	// .prereset			= ata_sff_prereset,
	// .softreset			= ata_sff_softreset,
	// .hardreset			= sata_sff_hardreset,
	// .postreset			= ata_sff_postreset,
	// .error_handler		= ata_sff_error_handler,

	.sff_dev_select		= ata_sff_dev_select,
	// .sff_check_status	= ata_sff_check_status,
	// .sff_tf_load		= ata_sff_tf_load,
	// .sff_tf_read		= ata_sff_tf_read,
	// .sff_exec_command	= ata_sff_exec_command,
	// .sff_data_xfer		= ata_sff_data_xfer,
	// .sff_drain_fifo		= ata_sff_drain_fifo,

	// .lost_interrupt		= ata_sff_lost_interrupt,
};

/**
 *	ata_sff_sync - Flush writes
 *	@ap: Port to wait for.
 *
 *	CAUTION:
 *	If we have an mmio device with no ctl and no altstatus
 *	method this will fail. No such devices are known to exist.
 *
 *	LOCKING:
 *	Inherited from caller.
 */
static void ata_sff_sync(ata_port_s *ap)
{
	if (ap->ops->sff_check_altstatus)
		ap->ops->sff_check_altstatus(ap);
	else if (ap->ioaddr.altstatus_addr)
		inb(ap->ioaddr.altstatus_addr);
}

/**
 *	ata_sff_pause		-	Flush writes and wait 400nS
 *	@ap: Port to pause for.
 *
 *	CAUTION:
 *	If we have an mmio device with no ctl and no altstatus
 *	method this will fail. No such devices are known to exist.
 *
 *	LOCKING:
 *	Inherited from caller.
 */

void ata_sff_pause(ata_port_s *ap)
{
	ata_sff_sync(ap);
	ndelay(400);
}

/**
 *	ata_sff_dev_select - Select device 0/1 on ATA bus
 *	@ap: ATA channel to manipulate
 *	@device: ATA device (numbered from zero) to select
 *
 *	Use the method defined in the ATA specification to
 *	make either device 0, or device 1, active on the
 *	ATA channel.  Works with both PIO and MMIO.
 *
 *	May be used as the dev_select() entry in ata_port_operations.
 *
 *	LOCKING:
 *	caller.
 */
void ata_sff_dev_select(ata_port_s *ap, unsigned int device)
{
	u8 tmp;
	if (device == 0)
		tmp = ATA_DEVICE_OBS;
	else
		tmp = ATA_DEVICE_OBS | ATA_DEV1;
	outb(tmp, ap->ioaddr.device_addr);
	ata_sff_pause(ap);	/* needed; also flushes, for mmio */
}


/**
 *	ata_devchk - PATA device presence detection
 *	@ap: ATA channel to examine
 *	@device: Device to examine (starting at zero)
 *
 *	This technique was originally described in
 *	Hale Landis's ATADRVR (www.ata-atapi.com), and
 *	later found its way into the ATA/ATAPI spec.
 *
 *	Write a pattern to the ATA shadow registers,
 *	and if a device is present, it will respond by
 *	correctly storing and echoing back the
 *	ATA shadow register contents.
 *
 *	LOCKING:
 *	caller.
 */
unsigned int ata_devchk(ata_port_s *ap, unsigned int device)
{
	ata_ioports_s *ioaddr = &ap->ioaddr;
	u8 nsect, lbal;

	ap->ops->sff_dev_select(ap, device);

	outb(0x55, ioaddr->nsect_addr);
	outb(0xaa, ioaddr->lbal_addr);

	outb(0xaa, ioaddr->nsect_addr);
	outb(0x55, ioaddr->lbal_addr);

	outb(0x55, ioaddr->nsect_addr);
	outb(0xaa, ioaddr->lbal_addr);

	nsect = inb(ioaddr->nsect_addr);
	lbal = inb(ioaddr->lbal_addr);

	if ((nsect == 0x55) && (lbal == 0xaa))
		return 1;	/* we found a device */

	return 0;		/* nothing found */
}