#include <string.h>

#include <include/proto.h>
#include <include/printk.h>
#include <include/block_dev.h>
#include <linux/kernel/sched/sched.h>

#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "include/apic.h"
#include "include/device.h"
#include "include/ide.h"

struct Identify_Device_data disk_id;
bdev_req_queue_T IDE_req_queue;
char read_test[512];

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
static void ATA_set_LBA(unsigned controller,
				unsigned short count, unsigned long lba)
{
	outb(IDE_PIO_ERR_STAT(controller), 0);
	outb(IDE_PIO_LBA_COUNT(controller), (count >> 0) & 0xFF);
	outb(IDE_PIO_LBA_LOW(controller), (lba >> 0) & 0xFF);
	outb(IDE_PIO_LBA_MID(controller), (lba >> 8) & 0xFF);
	outb(IDE_PIO_LBA_HIGH(controller), (lba >> 16) & 0xFF);
}

static unsigned char ATA_read_LBA28(unsigned controller, unsigned disk,
				unsigned long lba, unsigned short count)
{
	ATA_set_LBA(controller, count, lba);
	outb(IDE_PIO_DEV_OPT(controller), 0xE0 | (disk << 4) | (lba >> 24) & 0x0F);
	outb(IDE_PIO_CMD_STAT(controller), 0x20);
	return inb(IDE_PIO_CMD_STAT(controller));
}

static void ATA_write_LBA28(unsigned controller, unsigned disk,
				unsigned long lba, unsigned short count)
{
	ATA_set_LBA(controller, count, lba);
	outb(IDE_PIO_DEV_OPT(controller), 0xE0 | (disk << 4) | (lba >> 24) & 0x0F);
	outb(IDE_PIO_CMD_STAT(controller), 0x30);

	while(!(inb(IDE_PIO_CMD_STAT(controller)) & DISK_STATUS_READY))
		nop();
	outb(IDE_PIO_CMD_STAT(controller), ATA_WRITE_CMD);

	while(!(inb(IDE_PIO_CMD_STAT(controller)) & DISK_STATUS_REQ))
		nop();
}

static unsigned char ATA_read_LBA48(unsigned controller, unsigned disk,
				unsigned long lba, unsigned short count)
{
	ATA_set_LBA(controller, count >> 8, lba >> 24);
	ATA_set_LBA(controller, count, lba);
	outb(IDE_PIO_DEV_OPT(controller), 0xE0 | (disk << 4));
	outb(IDE_PIO_CMD_STAT(controller), 0x24);
	return inb(IDE_PIO_CMD_STAT(controller));
}

static void ATA_write_LBA48(unsigned controller, unsigned disk,
				unsigned long lba, unsigned short count)
{
	ATA_set_LBA(controller, count >> 8, lba >> 24);
	ATA_set_LBA(controller, count, lba);
	outb(IDE_PIO_DEV_OPT(controller), 0xE0 | (disk << 4));
	outb(IDE_PIO_CMD_STAT(controller), 0x34);
}

static void ATA_dev_info(unsigned controller, unsigned disk,
				unsigned long lba, unsigned short count)
{
	outb(IDE_PIO_DEV_OPT(controller), 0xE0 | disk << 4);
	ATA_set_LBA(controller, count, lba);
}

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
long cmd_out(unsigned controller, unsigned disk)
{
	List_s * wq_lp = list_hdr_pop(&IDE_req_queue.bdev_wqhdr);
	while (!wq_lp);
	
	blkbuf_node_s * node = container_of(wq_lp->owner_p, blkbuf_node_s, wq);
	IDE_req_queue.in_using = node;

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
			
		case GET_IDENTIFY_DISK_CMD:
			ATA_dev_info(controller, disk, node->LBA, node->count);

			while(!(inb(IDE_PIO_CMD_STAT(controller)) & DISK_STATUS_READY))
				nop();			
			outb(IDE_PIO_CMD_STAT(controller), node->cmd);
			
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

	if (node->wq.task->state == PS_UNINTERRUPTIBLE)
	{
		node->wq.task->state = PS_RUNNING;
		wakeup_task(node->wq.task);
		curr_tsk->flags |= PF_NEED_SCHEDULE;
	}

	kfree(node);
	IDE_req_queue.in_using = NULL;

	if(IDE_req_queue.bdev_wqhdr.count != 0)
		cmd_out(node->ATA_controller, node->ATA_disk);
}

void add_request(blkbuf_node_s * node)
{
	// per_cpudata_s * cpudata_p = curr_cpu;
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
	blkbuf_node_s * node = (blkbuf_node_s *)kmalloc(sizeof(blkbuf_node_s));
	wq_init(&node->wq, curr_tsk);
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
	
	if(IDE_req_queue.in_using == NULL)
		cmd_out(node->ATA_controller, node->ATA_disk);
}

void wait_for_finish()
{
	if (IDE_req_queue.in_using != NULL)
	{
		// per_cpudata_s *	cpudata_p = curr_cpu;
		curr_tsk->state = PS_UNINTERRUPTIBLE;
		schedule();
	}
}

long ATA_disk_open(unsigned controller, unsigned disk)
{
	color_printk(BLACK,WHITE,"DISK0 Opened\n");
	return 1;
}

long ATA_disk_close(unsigned controller, unsigned disk)
{
	color_printk(BLACK,WHITE,"DISK0 Closed\n");
	return 1;
}

long ATA_disk_ioctl(unsigned controller, unsigned disk,
				long cmd, long arg)
{
	blkbuf_node_s * node = NULL;
	if(cmd == GET_IDENTIFY_DISK_CMD)
	{
		node = make_request(controller, disk, cmd,
						0, 0, (unsigned char *)arg);
		submit(node);
		wait_for_finish();
		return 1;
	}
	else
	{
		return 0;
	}
}

long ATA_disk_transfer(unsigned controller, unsigned disk, long cmd,
				unsigned long blk_idx, long count, unsigned char * buffer)
{
	blkbuf_node_s * node = NULL;
	if(cmd == ATA_READ_CMD ||
		cmd == ATA_WRITE_CMD ||
		cmd == GET_IDENTIFY_DISK_CMD)
	{
		node = make_request(controller, disk, cmd,
						blk_idx, count, buffer);
		submit(node);
		wait_for_finish();
		return 1;
	}
	else
	{
		return 0;
	}
}

blkdev_ops_s ATA_master_ops = 
{
	.open	= ATA_disk_open,
	.close	= ATA_disk_close,
	.ioctl	= ATA_disk_ioctl,
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

void ATA_disk_handler(unsigned long parameter, stack_frame_s * sf_regs)
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

	outb(PORT_DISK0_ALT_STA_CTL, 0);
	IDE_req_queue.in_using = NULL;
	list_hdr_init(&IDE_req_queue.bdev_wqhdr);
}

void get_ata_info()
{
	IDE_id_dev_data_s ide_disk_info[4];
	ATA_disk_transfer(MASTER, MASTER, GET_IDENTIFY_DISK_CMD,
					0, 0, (unsigned char *)&ide_disk_info[0]);
	ATA_disk_transfer(MASTER, SLAVE, GET_IDENTIFY_DISK_CMD,
					0, 0, (unsigned char *)&ide_disk_info[1]);
	ATA_disk_transfer(SLAVE, MASTER, GET_IDENTIFY_DISK_CMD,
					0, 0, (unsigned char *)&ide_disk_info[2]);
	ATA_disk_transfer(SLAVE, SLAVE, GET_IDENTIFY_DISK_CMD,
					0, 0, (unsigned char *)&ide_disk_info[3]);
}

void disk_exit()
{
	unregister_irq(SATA_MAST_IRQ);
}