
#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "include/apic.h"
#include "include/device.h"
#include "include/disk.h"

#include "../../include/printk.h"

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
hw_int_controller_s disk_int_controller = 
{
	.enable = IOAPIC_enable,
	.disable = IOAPIC_disable,
	.install = IOAPIC_install,
	.uninstall = IOAPIC_uninstall,
	.ack = IOAPIC_edge_ack,
};

void disk_handler(unsigned long parameter, stack_frame_s * sf_regs)
{
// 	struct block_buffer_node * node = ((struct request_queue *)parameter)->in_using;
// //	color_printk(BLACK,WHITE,"disk_handler,preempt:%d,pid:%d,cRSP:%#018lx,RSP:%#018lx,RIP:%#018lx\n",current->preempt_count,current->pid,get_rsp(),regs->rsp,regs->rip);
// 	node->end_handler(nr,parameter);
	int i = 0;
	unsigned char a[512];
	insw(PORT_DISK0_DATA, (uint16_t *)&a, 256);
	color_printk(ORANGE,WHITE,"Read One Sector Finished:%02x\n",inb(PORT_DISK0_STATUS_CMD));
	for(i = 0;i<512;i++)
		color_printk(ORANGE,BLACK,"%02x ",a[i]);
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

	outb(PORT_DISK0_ALT_STA_CTL,0);

	while(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_BUSY);
	color_printk(ORANGE,WHITE,"Read One Sector Starting:%02x\n",inb(PORT_DISK0_STATUS_CMD));

	outb(PORT_DISK0_DEVICE,0xe0);
	
	outb(PORT_DISK0_ERR_FEATURE,0);
	outb(PORT_DISK0_SECTOR_CNT,1);
	outb(PORT_DISK0_SECTOR_LOW,0);
	outb(PORT_DISK0_SECTOR_MID,0);
	outb(PORT_DISK0_SECTOR_HIGH,0);
	
	while(!(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_READY));
	color_printk(ORANGE,WHITE,"Send CMD:%02x\n",inb(PORT_DISK0_STATUS_CMD));
	
	outb(PORT_DISK0_STATUS_CMD,0x20);	////read
}

void disk_exit()
{
	unregister_irq(SATA_MAST_IRQ);
}