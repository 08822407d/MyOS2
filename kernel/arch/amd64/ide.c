
#include <lib/string.h>

#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "include/apic.h"
#include "include/device.h"
#include "include/ide.h"

#include "../../include/proto.h"
#include "../../include/printk.h"
#include "../../include/block.h"
#include "../../include/task.h"

#define IDE_IDX		0
#define DISK_IDX	1

struct Disk_Identify_Info disk_id;

diskrq_queue_s	disk_require_queue;

char read_test[512];

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
inline __always_inline void IDE_set_LBA28(unsigned long lba, unsigned short count)
{
	outb(PORT_DISK0_ERR_FEATURE, 0);
	outb(PORT_DISK0_SECTOR_CNT, (count >> 0) & 0xFF);
	outb(PORT_DISK0_SECTOR_LOW, (lba >> 0) & 0xFF);
	outb(PORT_DISK0_SECTOR_MID, (lba >> 8) & 0xFF);
	outb(PORT_DISK0_SECTOR_HIGH, (lba >> 16) & 0xFF);
}

inline __always_inline void IDE_set_LBA48(unsigned long lba, unsigned short count)
{
	outb(PORT_DISK0_ERR_FEATURE, 0);
	outb(PORT_DISK0_SECTOR_CNT, (count >> 8) & 0xFF);
	outb(PORT_DISK0_SECTOR_LOW, (lba >> 24) & 0xFF);
	outb(PORT_DISK0_SECTOR_MID, (lba >> 32) & 0xFF);
	outb(PORT_DISK0_SECTOR_HIGH, (lba >> 40) & 0xFF);

	outb(PORT_DISK0_ERR_FEATURE, 0);
	outb(PORT_DISK0_SECTOR_CNT, (count >> 0) & 0xFF);
	outb(PORT_DISK0_SECTOR_LOW, (lba >> 0) & 0xFF);
	outb(PORT_DISK0_SECTOR_MID, (lba >> 8) & 0xFF);
	outb(PORT_DISK0_SECTOR_HIGH, (lba >> 16) & 0xFF);
}

unsigned char IDE_read_LBA28(unsigned long lba, unsigned short count, unsigned drv_idx)
{
	IDE_set_LBA28(lba, count);
	outb(PORT_DISK0_DEVICE, 0xE0 | (drv_idx << 4) | (lba >> 24) & 0x0F);
	outb(PORT_DISK0_STATUS_CMD,0x20);
	return inb(PORT_DISK0_STATUS_CMD);
}

unsigned char IDE_write_LBA28(unsigned long lba, unsigned short count, unsigned drv_idx)
{
	IDE_set_LBA28(lba, count);
	outb(PORT_DISK0_DEVICE, 0xE0 | (drv_idx << 4) | (lba >> 24) & 0x0F);
	outb(PORT_DISK0_STATUS_CMD,0x30);
	return inb(PORT_DISK0_STATUS_CMD);
}

unsigned char IDE_read_LBA48(unsigned long lba, unsigned short count, unsigned drv_idx)
{
	IDE_set_LBA48(lba, count);
	outb(PORT_DISK0_DEVICE, 0x40 | (drv_idx << 4));
	outb(PORT_DISK0_STATUS_CMD,0x24);
	return inb(PORT_DISK0_STATUS_CMD);
}

unsigned char IDE_write_LBA48(unsigned long lba, unsigned short count, unsigned drv_idx)
{
	IDE_set_LBA48(lba, count);
	outb(PORT_DISK0_DEVICE, 0x40 | (drv_idx << 4));
	outb(PORT_DISK0_STATUS_CMD,0x34);
	return inb(PORT_DISK0_STATUS_CMD);
}

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
long cmd_out()
{
	blkbuf_node_s * node = NULL;
	m_dequeue_list(node, &disk_require_queue.waiting_list);
	disk_require_queue.curr_user = node;

	while(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_BUSY)
		nop();

	unsigned char status = 0;
	switch(node->cmd)
	{
		case ATA_WRITE_CMD:	
			status = IDE_write_LBA28(node->LBA, node->count, DISK_IDX);

			while(status & DISK_STATUS_REQ)
				nop();
			outsw(PORT_DISK0_DATA, (uint16_t *)node->buffer,256);
			break;

		case ATA_READ_CMD:
			status = IDE_read_LBA28(node->LBA, node->count, DISK_IDX);

			break;
			
		case GET_IDENTIFY_DISK_CMD:

			outb(PORT_DISK0_DEVICE,0xe0 | DISK_IDX);
			
			outb(PORT_DISK0_ERR_FEATURE,0);
			outb(PORT_DISK0_SECTOR_CNT,node->count & 0xff);
			outb(PORT_DISK0_SECTOR_LOW,node->LBA & 0xff);
			outb(PORT_DISK0_SECTOR_MID,(node->LBA >> 8) & 0xff);
			outb(PORT_DISK0_SECTOR_HIGH,(node->LBA >> 16) & 0xff);

			while(!(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_READY))
				nop();			
			outb(PORT_DISK0_STATUS_CMD,node->cmd);
			
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

	node->requestor->state = PS_RUNNING;
	wakeup_task(node->requestor);
	//	node->wait_queue.tsk->flags |= NEED_SCHEDULE;
	curr_tsk->flags |= PF_NEED_SCHEDULE;

	kfree(disk_require_queue.curr_user);
	disk_require_queue.curr_user = NULL;

	if(disk_require_queue.waiting_list.count)
		cmd_out();
}

void add_request(blkbuf_node_s * node)
{
	per_cpudata_s * cpudata_p = curr_cpu;
	m_enqueue_list(node, &disk_require_queue.waiting_list);
}

void read_handler(unsigned long parameter)
{
	blkbuf_node_s * node = ((diskrq_queue_s *)parameter)->curr_user;
	unsigned char status = inb(PORT_DISK0_STATUS_CMD);
	
	if(status & DISK_STATUS_ERROR)
		color_printk(RED,BLACK,"read_handler:%#010x\n",inb(PORT_DISK0_ERR_FEATURE));
	else
		insw(PORT_DISK0_DATA, (uint16_t *)node->buffer, 256);

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
	blkbuf_node_s * node = ((diskrq_queue_s *)parameter)->curr_user;

	if(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_ERROR)
		color_printk(RED,BLACK,"write_handler:%#010x\n",inb(PORT_DISK0_ERR_FEATURE));

	node->count--;
	if(node->count)
	{
		node->buffer += 512;
		while(!(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_REQ))
			nop();
		outsw(PORT_DISK0_DATA, (uint16_t *)node->buffer, 256);
		return;
	}

	end_request(node);
}

void other_handler(unsigned long parameter)
{
	blkbuf_node_s * node = ((diskrq_queue_s *)parameter)->curr_user;

	if(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_ERROR)
		color_printk(RED,BLACK,"other_handler:%#010x\n",inb(PORT_DISK0_ERR_FEATURE));
	else
		insw(PORT_DISK0_DATA, (uint16_t *)node->buffer, 256);

	end_request(node);
}

blkbuf_node_s * make_request(long cmd, unsigned long blk_idx, long count, unsigned char * buffer)
{
	blkbuf_node_s * node = (blkbuf_node_s *)kmalloc(sizeof(blkbuf_node_s));
	m_list_init(node);
	node->requestor = curr_tsk;
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
	
	node->LBA = blk_idx;
	node->count = count;
	node->buffer = buffer;

	return node;
}

void submit(blkbuf_node_s * node)
{	
	add_request(node);
	
	if(disk_require_queue.curr_user == NULL)
		cmd_out();
}

void wait_for_finish()
{
	if (disk_require_queue.curr_user != NULL)
	{
		task_s * curr = curr_tsk;
		curr->state = PS_UNINTERRUPTIBLE;
		schedule();
	}
}

long IDE_open()
{
	color_printk(BLACK,WHITE,"DISK0 Opened\n");
	return 1;
}

long IDE_close()
{
	color_printk(BLACK,WHITE,"DISK0 Closed\n");
	return 1;
}

long IDE_ioctl(long cmd,long arg)
{
	blkbuf_node_s * node = NULL;
	if(cmd == GET_IDENTIFY_DISK_CMD)
	{
		node = make_request(cmd,0,0,(unsigned char *)arg);
		submit(node);
		wait_for_finish();
		return 1;
	}
	else
	{
		return 0;
	}
}

long IDE_transfer(long cmd,unsigned long blk_idx,long count,unsigned char * buffer)
{
	blkbuf_node_s * node = NULL;
	if(cmd == ATA_READ_CMD || cmd == ATA_WRITE_CMD)
	{
		node = make_request(cmd, blk_idx, count, buffer);
		submit(node);
		wait_for_finish();
		return 1;
	}
	else
	{
		return 0;
	}
}

blkdev_ops_s IDE_device_operation = 
{
	.open	= IDE_open,
	.close	= IDE_close,
	.ioctl	= IDE_ioctl,
	.transfer = IDE_transfer,
};

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
hw_int_controller_s disk_int_controller = 
{
	.enable		= IOAPIC_enable,
	.disable	= IOAPIC_disable,
	.install	= IOAPIC_install,
	.uninstall	= IOAPIC_uninstall,
	.ack		= IOAPIC_edge_ack,
};

void disk_handler(unsigned long parameter, stack_frame_s * sf_regs)
{
	blkbuf_node_s * node = ((diskrq_queue_s *)parameter)->curr_user;
	node->end_handler(parameter);	
}

void init_disk()
{
	ioapic_retentry_T entry;

	entry.vector = VECTOR(SATA_MAST_IRQ);
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

	register_irq(SATA_MAST_IRQ, &entry , "disk0",
				(unsigned long)&disk_require_queue, &disk_int_controller,
				&disk_handler);

	outb(PORT_DISK0_ALT_STA_CTL, 0);
	
	disk_require_queue.curr_user = NULL;
	m_init_list_header(&disk_require_queue.waiting_list);
}

void disk_exit()
{
	unregister_irq(SATA_MAST_IRQ);
}