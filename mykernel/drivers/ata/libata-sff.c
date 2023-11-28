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
#include <linux/device/pci.h>
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
	.sff_check_status	= ata_sff_check_status,
	.sff_tf_load		= ata_sff_tf_load,
	.sff_tf_read		= ata_sff_tf_read,
	.sff_exec_command	= ata_sff_exec_command,
	.sff_data_xfer		= ata_sff_data_xfer,
	// .sff_drain_fifo		= ata_sff_drain_fifo,

	// .lost_interrupt		= ata_sff_lost_interrupt,
};

/**
 *	ata_sff_check_status - Read device status reg & clear interrupt
 *	@ap: port where the device is
 *
 *	Reads ATA taskfile status register for currently-selected device
 *	and return its value. This also clears pending interrupts
 *      from this device
 *
 *	LOCKING:
 *	Inherited from caller.
 */
u8 ata_sff_check_status(ata_port_s *ap) {
	return inb(ap->ioaddr.status_addr);
}

/**
 *	ata_sff_altstatus - Read device alternate status reg
 *	@ap: port where the device is
 *
 *	Reads ATA taskfile alternate status register for
 *	currently-selected device and return its value.
 *
 *	Note: may NOT be used as the check_altstatus() entry in
 *	ata_port_operations.
 *
 *	LOCKING:
 *	Inherited from caller.
 */
static u8 ata_sff_altstatus(ata_port_s *ap)
{
	if (ap->ops->sff_check_altstatus)
		return ap->ops->sff_check_altstatus(ap);
	return inb(ap->ioaddr.altstatus_addr);
}

/**
 *	ata_sff_irq_status - Check if the device is busy
 *	@ap: port where the device is
 *
 *	Determine if the port is currently busy. Uses altstatus
 *	if available in order to avoid clearing shared IRQ status
 *	when finding an IRQ source. Non ctl capable devices don't
 *	share interrupt lines fortunately for us.
 *
 *	LOCKING:
 *	Inherited from caller.
 */
static u8 ata_sff_irq_status(ata_port_s *ap)
{
	u8 status;
	if (ap->ops->sff_check_altstatus || ap->ioaddr.altstatus_addr) {
		status = ata_sff_altstatus(ap);
		/* Not us: We are busy */
		if (status & ATA_BUSY) return status;
	}
	/* Clear INTRQ latch */
	status = ap->ops->sff_check_status(ap);
	return status;
}

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
 *	ata_sff_irq_on - Enable interrupts on a port.
 *	@ap: Port on which interrupts are enabled.
 *
 *	Enable interrupts on a legacy IDE device using MMIO or PIO,
 *	wait for idle, clear any pending interrupts.
 *
 *	Note: may NOT be used as the sff_irq_on() entry in
 *	ata_port_operations.
 *
 *	LOCKING:
 *	Inherited from caller.
 */
void ata_sff_irq_on(ata_port_s *ap)
{
	ata_iops_s *ioaddr = &ap->ioaddr;

	ap->ctl &= ~ATA_NIEN;
	ap->last_ctl = ap->ctl;

	// if (ap->ops->sff_set_devctl || ioaddr->ctl_addr)
	// 	ata_sff_set_devctl(ap, ap->ctl);
	// ata_wait_idle(ap);

	// if (ap->ops->sff_irq_clear)
	// 	ap->ops->sff_irq_clear(ap);
}

/**
 *	ata_sff_tf_load - send taskfile registers to host controller
 *	@ap: Port to which output is sent
 *	@tf: ATA taskfile register set
 *
 *	Outputs ATA taskfile to standard ATA host controller.
 *
 *	LOCKING:
 *	Inherited from caller.
 */
void ata_sff_tf_load(ata_port_s *ap, const ata_tf_s *tf)
{
	ata_iops_s *ioaddr = &ap->ioaddr;
	unsigned int is_addr = tf->flags & ATA_TFLAG_ISADDR;

	if (tf->ctl != ap->last_ctl) {
		if (ioaddr->ctl_addr)
			outb(tf->ctl, ioaddr->ctl_addr);
		ap->last_ctl = tf->ctl;
		// ata_wait_idle(ap);
	}

	if (is_addr && (tf->flags & ATA_TFLAG_LBA48)) {
		// WARN_ON_ONCE(!ioaddr->ctl_addr);
		outb(tf->hob_feature, ioaddr->feature_addr);
		outb(tf->hob_nsect, ioaddr->nsect_addr);
		outb(tf->hob_lbal, ioaddr->lbal_addr);
		outb(tf->hob_lbam, ioaddr->lbam_addr);
		outb(tf->hob_lbah, ioaddr->lbah_addr);
	}

	if (is_addr) {
		outb(tf->feature, ioaddr->feature_addr);
		outb(tf->nsect, ioaddr->nsect_addr);
		outb(tf->lbal, ioaddr->lbal_addr);
		outb(tf->lbam, ioaddr->lbam_addr);
		outb(tf->lbah, ioaddr->lbah_addr);
	}

	if (tf->flags & ATA_TFLAG_DEVICE)
		outb(tf->device, ioaddr->device_addr);

	// ata_wait_idle(ap);
}

/**
 *	ata_sff_tf_read - input device's ATA taskfile shadow registers
 *	@ap: Port from which input is read
 *	@tf: ATA taskfile register set for storing input
 *
 *	Reads ATA taskfile registers for currently-selected device
 *	into @tf. Assumes the device has a fully SFF compliant task file
 *	layout and behaviour. If you device does not (eg has a different
 *	status method) then you will need to provide a replacement tf_read
 *
 *	LOCKING:
 *	Inherited from caller.
 */
void ata_sff_tf_read(ata_port_s *ap, ata_tf_s *tf)
{
	ata_iops_s *ioaddr = &ap->ioaddr;

	tf->command	= ata_sff_check_status(ap);
	tf->feature	= inb(ioaddr->error_addr);
	tf->nsect	= inb(ioaddr->nsect_addr);
	tf->lbal	= inb(ioaddr->lbal_addr);
	tf->lbam	= inb(ioaddr->lbam_addr);
	tf->lbah	= inb(ioaddr->lbah_addr);
	tf->device	= inb(ioaddr->device_addr);

	if (tf->flags & ATA_TFLAG_LBA48) {
		if (likely(ioaddr->ctl_addr)) {
			outb(tf->ctl | ATA_HOB, ioaddr->ctl_addr);
			tf->hob_feature	= inb(ioaddr->error_addr);
			tf->hob_nsect	= inb(ioaddr->nsect_addr);
			tf->hob_lbal	= inb(ioaddr->lbal_addr);
			tf->hob_lbam	= inb(ioaddr->lbam_addr);
			tf->hob_lbah	= inb(ioaddr->lbah_addr);
			outb(tf->ctl, ioaddr->ctl_addr);
			ap->last_ctl = tf->ctl;
		}
		// } else
		// 	WARN_ON_ONCE(1);
	}
}

/**
 *	ata_sff_exec_command - issue ATA command to host controller
 *	@ap: port to which command is being issued
 *	@tf: ATA taskfile register set
 *
 *	Issues ATA command, with proper synchronization with interrupt
 *	handler / other threads.
 *
 *	LOCKING:
 *	spin_lock_irqsave(host lock)
 */
void ata_sff_exec_command(ata_port_s *ap, const ata_tf_s *tf)
{
	outb(tf->command, ap->ioaddr.command_addr);
	ata_sff_pause(ap);
}

/**
 *	ata_tf_to_host - issue ATA taskfile to host controller
 *	@ap: port to which command is being issued
 *	@tf: ATA taskfile register set
 *	@tag: tag of the associated command
 *
 *	Issues ATA taskfile register set to ATA host controller,
 *	with proper synchronization with interrupt handler and
 *	other threads.
 *
 *	LOCKING:
 *	spin_lock_irqsave(host lock)
 */
static inline void
ata_tf_to_host(ata_port_s *ap, const ata_tf_s *tf)
{
	// trace_ata_tf_load(ap, tf);
	ap->ops->sff_tf_load(ap, tf);
	// trace_ata_exec_command(ap, tf, tag);
	ap->ops->sff_exec_command(ap, tf);
}

/**
 *	ata_sff_data_xfer - Transfer data by PIO
 *	@qc: queued command
 *	@buf: data buffer
 *	@buflen: buffer length
 *	@rw: read/write
 *
 *	Transfer data from/to the device data register by PIO.
 *
 *	LOCKING:
 *	Inherited from caller.
 *
 *	RETURNS:
 *	Bytes consumed.
 */
unsigned int
ata_sff_data_xfer(ata_q_cmd_s *qc, unsigned char *buf, unsigned int buflen, int rw)
{
	// struct ata_port *ap = qc->dev->link->ap;
	// void __iomem *data_addr = ap->ioaddr.data_addr;
	// unsigned int words = buflen >> 1;

	// /* Transfer multiple of 2 bytes */
	// if (rw == READ)
	// 	ioread16_rep(data_addr, buf, words);
	// else
	// 	iowrite16_rep(data_addr, buf, words);

	// /* Transfer trailing byte, if any. */
	// if (unlikely(buflen & 0x01)) {
	// 	unsigned char pad[2] = { };

	// 	/* Point buf to the tail of buffer */
	// 	buf += buflen - 1;

	// 	/*
	// 	 * Use io*16_rep() accessors here as well to avoid pointlessly
	// 	 * swapping bytes to and from on the big endian machines...
	// 	 */
	// 	if (rw == READ) {
	// 		ioread16_rep(data_addr, pad, 1);
	// 		*buf = pad[0];
	// 	} else {
	// 		pad[0] = *buf;
	// 		iowrite16_rep(data_addr, pad, 1);
	// 	}
	// 	words++;
	// }

	// return words << 1;
}

/**
 *	ata_sff_data_xfer32 - Transfer data by PIO
 *	@qc: queued command
 *	@buf: data buffer
 *	@buflen: buffer length
 *	@rw: read/write
 *
 *	Transfer data from/to the device data register by PIO using 32bit
 *	I/O operations.
 *
 *	LOCKING:
 *	Inherited from caller.
 *
 *	RETURNS:
 *	Bytes consumed.
 */

unsigned int
ata_sff_data_xfer32(ata_q_cmd_s *qc, unsigned char *buf, unsigned int buflen, int rw)
{
	// ata_dev_s *dev = qc->dev;
	// struct ata_port *ap = dev->link->ap;
	// void __iomem *data_addr = ap->ioaddr.data_addr;
	// unsigned int words = buflen >> 2;
	// int slop = buflen & 3;

	// if (!(ap->pflags & ATA_PFLAG_PIO32))
	// 	return ata_sff_data_xfer(qc, buf, buflen, rw);

	// /* Transfer multiple of 4 bytes */
	// if (rw == READ)
	// 	ioread32_rep(data_addr, buf, words);
	// else
	// 	iowrite32_rep(data_addr, buf, words);

	// /* Transfer trailing bytes, if any */
	// if (unlikely(slop)) {
	// 	unsigned char pad[4] = { };

	// 	/* Point buf to the tail of buffer */
	// 	buf += buflen - slop;

	// 	/*
	// 	 * Use io*_rep() accessors here as well to avoid pointlessly
	// 	 * swapping bytes to and from on the big endian machines...
	// 	 */
	// 	if (rw == READ) {
	// 		if (slop < 3)
	// 			ioread16_rep(data_addr, pad, 1);
	// 		else
	// 			ioread32_rep(data_addr, pad, 1);
	// 		memcpy(buf, pad, slop);
	// 	} else {
	// 		memcpy(pad, buf, slop);
	// 		if (slop < 3)
	// 			iowrite16_rep(data_addr, pad, 1);
	// 		else
	// 			iowrite32_rep(data_addr, pad, 1);
	// 	}
	// }
	// return (buflen + 1) & ~1;
}

static void ata_pio_xfer(ata_q_cmd_s *qc, page_s *page,
		unsigned int offset, size_t xfer_size)
{
	// bool do_write = (qc->tf.flags & ATA_TFLAG_WRITE);
	// unsigned char *buf;

	// buf = kmap_atomic(page);
	// qc->ap->ops->sff_data_xfer(qc, buf + offset, xfer_size, do_write);
	// kunmap_atomic(buf);

	// if (!do_write && !PageSlab(page))
	// 	flush_dcache_page(page);
}

/**
 *	ata_pio_sector - Transfer a sector of data.
 *	@qc: Command on going
 *
 *	Transfer qc->sect_size bytes of data from/to the ATA device.
 *
 *	LOCKING:
 *	Inherited from caller.
 */
static void ata_pio_sector(ata_q_cmd_s *qc)
{
	// struct ata_port *ap = qc->ap;
	// page_s *page;
	// unsigned int offset;

	// if (!qc->cursg) {
	// 	qc->curbytes = qc->nbytes;
	// 	return;
	// }
	// if (qc->curbytes == qc->nbytes - qc->sect_size)
	// 	ap->hsm_task_state = HSM_ST_LAST;

	// page = sg_page(qc->cursg);
	// offset = qc->cursg->offset + qc->cursg_ofs;

	// /* get the current page and offset */
	// page = nth_page(page, (offset >> PAGE_SHIFT));
	// offset %= PAGE_SIZE;

	// trace_ata_sff_pio_transfer_data(qc, offset, qc->sect_size);

	// /*
	//  * Split the transfer when it splits a page boundary.  Note that the
	//  * split still has to be dword aligned like all ATA data transfers.
	//  */
	// WARN_ON_ONCE(offset % 4);
	// if (offset + qc->sect_size > PAGE_SIZE) {
	// 	unsigned int split_len = PAGE_SIZE - offset;

	// 	ata_pio_xfer(qc, page, offset, split_len);
	// 	ata_pio_xfer(qc, nth_page(page, 1), 0,
	// 		     qc->sect_size - split_len);
	// } else {
	// 	ata_pio_xfer(qc, page, offset, qc->sect_size);
	// }

	// qc->curbytes += qc->sect_size;
	// qc->cursg_ofs += qc->sect_size;

	// if (qc->cursg_ofs == qc->cursg->length) {
	// 	qc->cursg = sg_next(qc->cursg);
	// 	if (!qc->cursg)
	// 		ap->hsm_task_state = HSM_ST_LAST;
	// 	qc->cursg_ofs = 0;
	// }
}

/**
 *	ata_pio_sectors - Transfer one or many sectors.
 *	@qc: Command on going
 *
 *	Transfer one or many sectors of data from/to the
 *	ATA device for the DRQ request.
 *
 *	LOCKING:
 *	Inherited from caller.
 */
static void ata_pio_sectors(ata_q_cmd_s *qc)
{
	// if (is_multi_taskfile(&qc->tf)) {
	// 	/* READ/WRITE MULTIPLE */
	// 	unsigned int nsect;

	// 	WARN_ON_ONCE(qc->dev->multi_count == 0);

	// 	nsect = min((qc->nbytes - qc->curbytes) / qc->sect_size,
	// 		    qc->dev->multi_count);
	// 	while (nsect--)
	// 		ata_pio_sector(qc);
	// } else
	// 	ata_pio_sector(qc);

	// ata_sff_sync(qc->ap); /* flush */
}


unsigned int myos_ata_pio_qc_issue(ata_q_cmd_s *qc)
{
	ata_port_s *ap = qc->ap;
	ata_dev_s *ad = qc->dev;
	ata_sff_dev_select(ap, ad->devno);

	ata_tf_to_host(ap, &qc->tf);
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
	ata_iops_s *ioaddr = &ap->ioaddr;
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


unsigned int myos_ata_dma_qc_issue(ata_q_cmd_s *qc)
{
	while (1);
}
