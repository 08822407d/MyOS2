#include <linux/kernel/slab.h>
#include <linux/kernel/delay.h>
#include <linux/kernel/sched.h>
#include <linux/kernel/preempt.h>
#include <linux/kernel/kdev_t.h>
#include <linux/kernel/kthread.h>
#include <linux/lib/string.h>
#include <uapi/linux/major.h>
#include <asm/processor.h>
#include <asm/io.h>

#include <obsolete/printk.h>
#include <obsolete/block_dev.h>

#include <obsolete/arch_proto.h>
#include <obsolete/interrupt.h>
#include <obsolete/apic.h>
#include <obsolete/device.h>
#include <obsolete/ide.h>

static task_s *thread;
bdev_req_queue_T IDE_req_queue;

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
	while(!((s = inb(IDE_PIO_CMD_STAT(controller))) & DISK_STATUS_READY))
		nop();
	outb(ATA_WRITE_CMD, IDE_PIO_CMD_STAT(controller));

	while(!(inb(IDE_PIO_CMD_STAT(controller)) & DISK_STATUS_REQ))
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
	while(!((s = inb(IDE_PIO_CMD_STAT(controller))) & DISK_STATUS_READY))
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
	// List_s * wq_lp = list_hdr_pop(&IDE_req_queue.bdev_wqhdr);
	// while (!wq_lp);
	
	// blkbuf_node_s * node = container_of(wq_lp->owner_p, blkbuf_node_s, wq);
	IDE_req_queue.in_using = node;
	unsigned controller = node->ATA_controller;
	unsigned disk = node->ATA_disk;

	while(inb(IDE_PIO_CMD_STAT(controller)) & DISK_STATUS_BUSY)
		nop();

	unsigned char status = 0;
	switch(node->cmd)
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
			color_printk(BLACK,WHITE,"ATA CMD Error\n");
			break;
	}
	return 1;
}

void end_request(blkbuf_node_s * node)
{
	if(node == NULL)
		color_printk(RED,BLACK,"end_request error\n");

	wake_up_process(node->wq.task);
	current->flags |= PF_NEED_SCHEDULE;

	IDE_req_queue.in_using = NULL;
}

void add_request(blkbuf_node_s * node)
{
	list_hdr_append(&IDE_req_queue.bdev_wqhdr, &node->wq.wq_list);
}

void read_handler(unsigned long parameter)
{
	blkbuf_node_s * node = ((bdev_req_queue_T *)parameter)->in_using;
	unsigned char status = inb(IDE_PIO_CMD_STAT(node->ATA_controller));
	
	if(status & DISK_STATUS_ERROR)
		color_printk(RED, BLACK, "read_handler:%#010x\n",
						inb(IDE_PIO_ERR_STAT(node->ATA_controller)));
	else
		insw(IDE_PIO_DATA(node->ATA_controller), (uint16_t *)node->buffer, 256);

	node->count--;
	if(node->count)
	{
		node->buffer += 512;
		return;
	}

	end_request(node);
}

void write_handler(unsigned long parameter)
{
	blkbuf_node_s * node = ((bdev_req_queue_T *)parameter)->in_using;

	if(inb(IDE_PIO_CMD_STAT(node->ATA_controller)) & DISK_STATUS_ERROR)
		color_printk(RED, BLACK, "write_handler:%#010x\n",
						inb(IDE_PIO_ERR_STAT(node->ATA_controller)));

	node->count--;
	if(node->count)
	{
		node->buffer += 512;
		while(!(inb(IDE_PIO_CMD_STAT(node->ATA_controller)) & DISK_STATUS_REQ))
			nop();
		outsw(IDE_PIO_DATA(node->ATA_controller), (uint16_t *)node->buffer, 256);
		return;
	}

	end_request(node);
}

void other_handler(unsigned long parameter)
{
	blkbuf_node_s * node = ((bdev_req_queue_T *)parameter)->in_using;

	if(inb(IDE_PIO_CMD_STAT(node->ATA_controller)) & DISK_STATUS_ERROR)
		color_printk(RED, BLACK, "other_handler:%#010x\n",
						inb(IDE_PIO_ERR_STAT(node->ATA_controller)));
	else
		insw(IDE_PIO_DATA(node->ATA_controller), (uint16_t *)node->buffer, 256);

	end_request(node);
}

blkbuf_node_s * make_request(unsigned controller, unsigned disk, long cmd,
				unsigned long blk_idx, long count, unsigned char * buffer)
{
	blkbuf_node_s * node = (blkbuf_node_s *)kzalloc(sizeof(blkbuf_node_s), GFP_KERNEL);
	wq_init(&node->wq, current);
	node->buffer = buffer;

	switch(cmd)
	{
		case ATA_READ_CMD:
			node->end_handler = read_handler;
			node->cmd = ATA_READ_CMD;
			break;

		case ATA_WRITE_CMD:
			node->end_handler = write_handler;
			node->cmd = ATA_WRITE_CMD;
			break;

		default:///
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

void submit(blkbuf_node_s * node)
{	
	add_request(node);
}

void wait_for_finish()
{
	// myos_preempt_disable();
	// myos_wake_up_new_task(thread);

	__set_current_state(TASK_UNINTERRUPTIBLE);
	// myos_preempt_enable_no_resched();
	schedule();
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
				unsigned long blk_idx, long count, unsigned char * buffer)
{
	blkbuf_node_s * node = NULL;
	if(cmd == ATA_READ_CMD ||
		cmd == ATA_WRITE_CMD ||
		cmd == ATA_INFO_CMD)
	{
		node = make_request(controller, disk, cmd,
						blk_idx, count, buffer);
		submit(node);
		wait_for_finish();
		if (node != NULL)
			kfree(node);
		return 1;
	}
	else
	{
		return 0;
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
	.enable		= IOAPIC_enable,
	.disable	= IOAPIC_disable,
	.install	= IOAPIC_install,
	.uninstall	= IOAPIC_uninstall,
	.ack		= IOAPIC_edge_ack,
};

void ATA_disk_handler(unsigned long parameter, pt_regs_s *sf_regs)
{
	blkbuf_node_s * node = ((bdev_req_queue_T *)parameter)->in_using;
	node->end_handler(parameter);	
}

void init_disk()
{
	ioapic_retentry_T entry;

	entry.deliver_mode = APIC_ICR_IOAPIC_Fixed ;
	entry.dest_mode = ICR_IOAPIC_DELV_PHYSICAL;
	entry.deliver_status = APIC_ICR_IOAPIC_Idle;
	entry.polarity = APIC_IOAPIC_POLARITY_HIGH;
	entry.irr = APIC_IOAPIC_IRR_RESET;
	entry.trigger = APIC_ICR_IOAPIC_Edge;
	entry.mask = APIC_ICR_IOAPIC_Masked;
	entry.reserved = 0;

	entry.dst.physical.reserved1 = 0;
	entry.dst.physical.phy_dest = 0;
	entry.dst.physical.reserved2 = 0;

	entry.vector = VECTOR_IRQ(SATA_MAST_IRQ);
	register_irq(SATA_MAST_IRQ, &entry , "ATA-master",
			(unsigned long)&IDE_req_queue, &ATA_disk_ioapic_controller,
			&ATA_disk_handler);

	entry.vector = VECTOR_IRQ(SATA_SLAV_IRQ);
	register_irq(SATA_SLAV_IRQ, &entry , "ATA-slave",
			(unsigned long)&IDE_req_queue, &ATA_disk_ioapic_controller,
			&ATA_disk_handler);

	IDE_req_queue.in_using = NULL;
	list_hdr_init(&IDE_req_queue.bdev_wqhdr);
}

void disk_exit()
{
	unregister_irq(SATA_MAST_IRQ);
}

static int ATArq_deamon(void *param)
{
	while (true)
	{
		if (IDE_req_queue.in_using == NULL &&
			IDE_req_queue.bdev_wqhdr.count > 0)
		{
			List_s * wq_lp = list_hdr_pop(&IDE_req_queue.bdev_wqhdr);
			while (!wq_lp);
			blkbuf_node_s * node = container_of(wq_lp->owner_p, blkbuf_node_s, wq);
			IDE_req_queue.in_using = node;

			cmd_out(node);
		}

		// __set_current_state(TASK_INTERRUPTIBLE);
		schedule();
	}

	return 1;
}

void init_ATArqd()
{
	thread = myos_kthread_create(ATArq_deamon, NULL, "init");
}