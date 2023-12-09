#include <linux/kernel/slab.h>
#include <linux/kernel/delay.h>
#include <linux/kernel/sched.h>
#include <linux/kernel/preempt.h>
#include <linux/kernel/kdev_t.h>
#include <linux/kernel/kthread.h>
#include <linux/kernel/completion.h>
#include <linux/device/tty.h>
#include <linux/lib/string.h>
#include <uapi/linux/major.h>
#include <asm/processor.h>
#include <asm/io.h>
#include <asm/apic.h>

#include <obsolete/printk.h>
#include <obsolete/arch_proto.h>
#include <obsolete/myos_irq_vectors.h>
#include <obsolete/device.h>
#include <obsolete/ide.h>
#include "myos_block.h"

static task_s *thread;

static DEFINE_SPINLOCK(req_lock);

static blkbuf_node_s *req_in_using;
LIST_HDR_S(IDEreq_lhdr);

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
static void ATA_set_LBA(unsigned controller,
						unsigned short count, unsigned long lba)
{
	outb((count >> 0) & 0xFF, IDE_PIO_LBA_COUNT(controller));
	outb((lba >> 0) & 0xFF, IDE_PIO_LBA_LOW(controller));
	outb((lba >> 8) & 0xFF, IDE_PIO_LBA_MID(controller));
	outb((lba >> 16) & 0xFF, IDE_PIO_LBA_HIGH(controller));
}

static unsigned char
ATA_read_LBA28(unsigned controller, unsigned disk,
			   unsigned long lba, unsigned short count)
{
	ATA_set_LBA(controller, count, lba);
	outb(0xE0 | (disk << 4) | (lba >> 24) & 0x0F,
		 IDE_PIO_DEV_OPT(controller));
	outb(0x20, IDE_PIO_CMD_STAT(controller));
	return inb(IDE_PIO_CMD_STAT(controller));
}

static void ATA_write_LBA28(unsigned controller,
							unsigned disk, unsigned long lba, unsigned short count)
{
	outb(0, IDE_PIO_ERR_STAT(controller));
	ATA_set_LBA(controller, count, lba);
	outb(0xE0 | (disk << 4) | (lba >> 24) & 0x0F,
		 IDE_PIO_DEV_OPT(controller));
	outb(0x30, IDE_PIO_CMD_STAT(controller));

	int s;
	while (!((s = inb(IDE_PIO_CMD_STAT(controller))) & DISK_STATUS_READY))
		nop();
	outb(ATA_WRITE_CMD, IDE_PIO_CMD_STAT(controller));

	while (!(inb(IDE_PIO_CMD_STAT(controller)) & DISK_STATUS_REQ))
		nop();
}

static unsigned char
ATA_read_LBA48(unsigned controller, unsigned disk,
			   unsigned long lba, unsigned short count)
{
	outb(0, IDE_PIO_ERR_STAT(controller));
	ATA_set_LBA(controller, count >> 8, lba >> 24);
	ATA_set_LBA(controller, count, lba);
	outb(0xE0 | (disk << 4), IDE_PIO_DEV_OPT(controller));
	outb(0x24, IDE_PIO_CMD_STAT(controller));
	return inb(IDE_PIO_CMD_STAT(controller));
}

static void ATA_write_LBA48(unsigned controller,
							unsigned disk, unsigned long lba, unsigned short count)
{
	ATA_set_LBA(controller, count >> 8, lba >> 24);
	ATA_set_LBA(controller, count, lba);
	outb(0xE0 | (disk << 4), IDE_PIO_DEV_OPT(controller));
	outb(0x34, IDE_PIO_CMD_STAT(controller));
}

static void ATA_dev_info(unsigned controller, unsigned disk)
{
	ATA_set_LBA(controller, 0, 0);
	outb(0xE0 | disk << 4, IDE_PIO_DEV_OPT(controller));
	int s;
	while (!((s = inb(IDE_PIO_CMD_STAT(controller))) & DISK_STATUS_READY))
		nop();
	outb(ATA_DISK_IDENTIFY, IDE_PIO_CMD_STAT(controller));
}

static bool myos_ata_devchk(unsigned controller, unsigned disk)
{
	ATA_set_LBA(controller, 0, 0);
	outb(0xA0 | disk << 4, IDE_PIO_CMD_STAT(controller));

	u64 lba_mid = inb(IDE_PIO_LBA_MID(controller));
	u64 lba_high = inb(IDE_PIO_LBA_HIGH(controller));
	u64 cmd_stat = inb(IDE_PIO_CMD_STAT(controller));
	if (lba_mid == 0 && lba_high == 0 && cmd_stat != 0)
		return true;
	else
		return false;
}

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
// long cmd_out(unsigned controller, unsigned disk)
long cmd_out(blkbuf_node_s *node)
{
	unsigned controller = node->ATA_controller;
	unsigned disk = node->ATA_disk;

	while (inb(IDE_PIO_CMD_STAT(controller)) & DISK_STATUS_BUSY)
		nop();

	unsigned char status = 0;
	switch (node->cmd)
	{
	case ATA_WRITE_CMD:
		ATA_write_LBA28(controller, disk, node->LBA, node->count);
		outsw(IDE_PIO_DATA(controller), (uint16_t *)node->buffer, 256);
		break;

	case ATA_READ_CMD:
		status = ATA_read_LBA28(controller, disk, node->LBA, node->count);
		break;

	case ATA_INFO_CMD:
		ATA_dev_info(controller, disk);
		break;

	default:
		color_printk(BLACK, WHITE, "ATA CMD Error\n");
		break;
	}
	return 1;
}

void end_request(blkbuf_node_s *node)
{
	if (node == NULL)
		color_printk(RED, BLACK, "end_request error\n");

	// complete(node->done);
	// spin_lock(&req_lock);
	wake_up_process(node->task);
	req_in_using = NULL;
	current->flags |= PF_NEED_SCHEDULE;
	// spin_unlock_no_resched(&req_lock);
}

void read_handler(unsigned long parameter)
{
	if (inb(IDE_PIO_CMD_STAT(req_in_using->ATA_controller)) & DISK_STATUS_ERROR)
		color_printk(RED, BLACK, "read_handler:%#010x\n",
					 inb(IDE_PIO_ERR_STAT(req_in_using->ATA_controller)));
	else
		insw(IDE_PIO_DATA(req_in_using->ATA_controller), (uint16_t *)req_in_using->buffer, 256);

	req_in_using->count--;
	if (req_in_using->count)
		req_in_using->buffer += 512;
}

void write_handler(unsigned long parameter)
{
	if (inb(IDE_PIO_CMD_STAT(req_in_using->ATA_controller)) & DISK_STATUS_ERROR)
		color_printk(RED, BLACK, "write_handler:%#010x\n",
					 inb(IDE_PIO_ERR_STAT(req_in_using->ATA_controller)));

	req_in_using->count--;
	if (req_in_using->count)
	{
		req_in_using->buffer += 512;
		while (!(inb(IDE_PIO_CMD_STAT(req_in_using->ATA_controller)) & DISK_STATUS_REQ))
			nop();
		outsw(IDE_PIO_DATA(req_in_using->ATA_controller), (uint16_t *)req_in_using->buffer, 256);
	}
}

void other_handler(unsigned long parameter)
{
	if (inb(IDE_PIO_CMD_STAT(req_in_using->ATA_controller)) & DISK_STATUS_ERROR)
		color_printk(RED, BLACK, "other_handler:%#010x\n",
					 inb(IDE_PIO_ERR_STAT(req_in_using->ATA_controller)));
	else
		insw(IDE_PIO_DATA(req_in_using->ATA_controller), (uint16_t *)req_in_using->buffer, 256);
}

blkbuf_node_s *make_request(unsigned controller, unsigned disk, long cmd,
							unsigned long blk_idx, long count, unsigned char *buffer)
{
	blkbuf_node_s *node = (blkbuf_node_s *)kzalloc(sizeof(blkbuf_node_s), GFP_KERNEL);
	list_init(&node->req_list, node);
	node->buffer = buffer;

	switch (cmd)
	{
	case ATA_READ_CMD:
		node->end_handler = read_handler;
		node->cmd = ATA_READ_CMD;
		break;

	case ATA_WRITE_CMD:
		node->end_handler = write_handler;
		node->cmd = ATA_WRITE_CMD;
		break;

	default: ///
		node->end_handler = other_handler;
		node->cmd = cmd;
		break;
	}

	node->ATA_controller = controller;
	node->ATA_disk = disk;
	node->LBA = blk_idx;
	node->count = count;
	node->buffer = buffer;

	return node;
}

// long ATA_disk_open(unsigned controller, unsigned disk)
// {
// 	color_printk(BLACK,WHITE,"DISK0 Opened\n");
// 	return 1;
// }

// long ATA_disk_close(unsigned controller, unsigned disk)
// {
// 	color_printk(BLACK,WHITE,"DISK0 Closed\n");
// 	return 1;
// }

long ATA_disk_transfer(unsigned controller, unsigned disk, long cmd,
					   unsigned long blk_idx, long count, unsigned char *buffer)
{
	blkbuf_node_s *node = NULL;
	if (cmd == ATA_READ_CMD ||
		cmd == ATA_WRITE_CMD ||
		cmd == ATA_INFO_CMD)
	{
		node = make_request(controller, disk, cmd,
							blk_idx, count, buffer);
		// DECLARE_COMPLETION_ONSTACK(done);
		// node->done = &done;
		node->task = current;

		spin_lock(&req_lock);
		list_hdr_enqueue(&IDEreq_lhdr, &node->req_list);
		spin_unlock_no_resched(&req_lock);

		__set_current_state(TASK_UNINTERRUPTIBLE);
		wake_up_process(thread);
		// wait_for_completion(&done);
		schedule();

		if (node != NULL)
			kfree(node);
		return -ENOERR;
	}
	else
	{
		return -EINVAL;
	}
}

// long ATA_disk_ioctl(unsigned controller, unsigned disk,
// 				long cmd, long arg)
// {
// 	ATA_disk_transfer(controller, disk, cmd, 0, 0, NULL);
// }

blkdev_ops_s ATA_master_ops =
	{
		// .open	= ATA_disk_open,
		// .close	= ATA_disk_close,
		// .ioctl	= ATA_disk_ioctl,
		.transfer = ATA_disk_transfer,
};

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
hw_int_controller_s ATA_disk_ioapic_controller =
{
		.enable = IOAPIC_enable,
		.disable = IOAPIC_disable,
		.install = IOAPIC_install,
		.uninstall = IOAPIC_uninstall,
		.ack = IOAPIC_edge_ack,
};

void ATA_disk_handler(unsigned long parameter, pt_regs_s *sf_regs)
{
	blkbuf_node_s *node = req_in_using;
	node->end_handler(parameter);
	
	if (node->count == 0)
		end_request(node);
}

void init_disk()
{
	ioapic_retentry_T entry;

	entry.deliver_mode = APIC_DELIVERY_MODE_FIXED;
	entry.mask = APIC_LVT_MASKED >> 16;
	entry.reserved = 0;

	entry.dst.physical.reserved1 = 0;
	entry.dst.physical.phy_dest = 0;
	entry.dst.physical.reserved2 = 0;

	entry.vector = VECTOR_IRQ(SATA_MAST_IRQ);
	register_irq(SATA_MAST_IRQ, &entry, "ATA-master", 0,
					&ATA_disk_ioapic_controller, ATA_disk_handler);

	entry.vector = VECTOR_IRQ(SATA_SLAV_IRQ);
	register_irq(SATA_SLAV_IRQ, &entry, "ATA-slave", 0,
					&ATA_disk_ioapic_controller, &ATA_disk_handler);

	req_in_using = NULL;
}

void disk_exit()
{
	unregister_irq(SATA_MAST_IRQ);
}

static int ATArq_deamon(void *param)
{
	current->flags |= PF_NOFREEZE;

	for (;;) {
		set_current_state(TASK_INTERRUPTIBLE);
		if (IDEreq_lhdr.count == 0)
			schedule();
		__set_current_state(TASK_RUNNING);

		while (IDEreq_lhdr.count != 0)
		{		
			spin_lock(&req_lock);
			if (req_in_using == NULL)
			{
				List_s *wq_lp = list_hdr_dequeue(&IDEreq_lhdr);
				blkbuf_node_s *node = container_of(wq_lp, blkbuf_node_s, req_list);
				req_in_using = node;
				spin_unlock_no_resched(&req_lock);

				cmd_out(node);

				spin_lock(&req_lock);
			}
			spin_unlock_no_resched(&req_lock);
			break;
		}
	}
	return 1;
}

void init_ATArqd()
{
	thread = kthread_run(ATArq_deamon, NULL, "ATArqd");

	// color_printk(WHITE, BLACK, "ATA disk: initialized\n");
}
