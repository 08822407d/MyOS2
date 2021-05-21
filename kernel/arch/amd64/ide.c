
#include <lib/string.h>

#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "include/apic.h"
#include "include/device.h"
#include "include/ide.h"

#include "../../include/printk.h"

struct Disk_Identify_Info disk_id;

void get_info()
{
	outb(PORT_DISK0_ALT_STA_CTL,0);	

	outb(PORT_DISK0_ERR_FEATURE,0);
	outb(PORT_DISK0_SECTOR_CNT,0);
	outb(PORT_DISK0_SECTOR_LOW,0);
	outb(PORT_DISK0_SECTOR_MID,0);
	outb(PORT_DISK0_SECTOR_HIGH,0);
	outb(PORT_DISK0_DEVICE,0xe0);
	outb(PORT_DISK0_STATUS_CMD,0xec);	//identify
}

void read_test()
{
	outb(PORT_DISK0_ALT_STA_CTL,0);

	while(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_BUSY);
	color_printk(ORANGE,BLACK,"Read One Sector Starting:%02x\n",inb(PORT_DISK0_STATUS_CMD));

	outb(PORT_DISK0_DEVICE,0xe0);
	
	outb(PORT_DISK0_ERR_FEATURE,0);
	outb(PORT_DISK0_SECTOR_CNT,1);
	outb(PORT_DISK0_SECTOR_LOW,0);
	outb(PORT_DISK0_SECTOR_MID,0);
	outb(PORT_DISK0_SECTOR_HIGH,0);
	
	while(!(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_READY));
	color_printk(ORANGE,BLACK,"Send CMD:%02x\n",inb(PORT_DISK0_STATUS_CMD));
	
	outb(PORT_DISK0_STATUS_CMD,0x20);	////read
}

void write_test()
{
	unsigned char a[512];

	outb(PORT_DISK0_ALT_STA_CTL,0);
	
	while(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_BUSY);
	color_printk(ORANGE,BLACK,"Write One Sector Starting:%02x\n",inb(PORT_DISK0_STATUS_CMD));

	outb(PORT_DISK0_ERR_FEATURE,0);
	// outb(PORT_DISK0_SECTOR_CNT,0);
	// outb(PORT_DISK0_SECTOR_LOW,0x12);
	// outb(PORT_DISK0_SECTOR_MID,0);
	// outb(PORT_DISK0_SECTOR_HIGH,0);

	outb(PORT_DISK0_ERR_FEATURE,0);
	outb(PORT_DISK0_SECTOR_CNT,1);
	outb(PORT_DISK0_SECTOR_LOW,0x30);
	outb(PORT_DISK0_SECTOR_MID,0x00);
	outb(PORT_DISK0_SECTOR_HIGH,0x00);
	outb(PORT_DISK0_DEVICE,0x40);

	while(!(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_READY));
	color_printk(ORANGE,BLACK,"Send CMD:%02x\n",inb(PORT_DISK0_STATUS_CMD));

	outb(PORT_DISK0_STATUS_CMD,0x30);	////write

	color_printk(ORANGE,BLACK,"Send Write:%02x\n",inb(PORT_DISK0_STATUS_CMD));
	unsigned long disk_status = 0;
	unsigned long err_status = 0;
	do
	{
		disk_status = inb(PORT_DISK0_STATUS_CMD);
		err_status = inb(PORT_DISK0_ERR_FEATURE);
	} while(!(disk_status & DISK_STATUS_REQ));
	memset(&a,0xA5,512);
	outsw(PORT_DISK0_DATA,(uint16_t *)&a,256);
}

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
// 	struct block_buffer_node * node = ((struct request_queue *)parameter)->in_using;
// //	color_printk(BLACK,BLACK,"disk_handler,preempt:%d,pid:%d,cRSP:%#018lx,RSP:%#018lx,RIP:%#018lx\n",current->preempt_count,current->pid,get_rsp(),regs->rsp,regs->rip);
// 	node->end_handler(nr,parameter);
	// int i = 0;
	// unsigned char a[512];
	// insw(PORT_DISK0_DATA, (uint16_t *)&a, 256);
	// color_printk(ORANGE,BLACK,"Read One Sector Finished:%02x\n",inb(PORT_DISK0_STATUS_CMD));
	// for(i = 0;i<512;i++)
	// 	color_printk(ORANGE,BLACK,"%02x ",a[i]);
	color_printk(GREEN,BLACK,"Write One Sector Finished:%02x, %02x\n",inb(PORT_DISK0_STATUS_CMD),inb(PORT_DISK0_ERR_FEATURE));
	// insw(PORT_DISK0_DATA,(uint16_t *)&disk_id,256);
	// unsigned short * p = (unsigned short *)&disk_id;
	// for(int i = 0;i<256;i++)
	// 	color_printk(ORANGE,WHITE,"%04x ",*(p+i));
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
					0, &disk_int_controller,
					&disk_handler);

	// register_irq(SATA_MAST_IRQ, &entry , "disk0",
	// 				(unsigned long)&disk_request, &disk_int_controller,
	// 				&disk_handler)

	// outb(PORT_DISK0_ALT_STA_CTL, 0);
	
	// wait_queue_init(&disk_request.wait_queue_list,NULL);
	// disk_request.in_using = NULL;
	// disk_request.block_request_count = 0;

	// read_test();
	write_test();
	// get_info();
}

void disk_exit()
{
	unregister_irq(SATA_MAST_IRQ);
}