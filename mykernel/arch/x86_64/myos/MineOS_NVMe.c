/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/
#include <linux/kernel/kthread.h>
#include <linux/kernel/stddef.h>
#include <linux/mm/mm.h>

#include "MineOS_NVMe.h"
// #include "block.h"
// #include "lib.h"
// #include "printk.h"
// #include "interrupt.h"
#include "MineOS_PCI.h"
#include <asm/io.h>
#include <asm/apic.h>
#include <asm/tlbflush.h>
// #include "memory.h"

#include <obsolete/myos_irq_vectors.h>
#include <obsolete/printk.h>
#include <obsolete/arch_proto.h>
#include "myos_block.h"


static task_s *thread;
static DEFINE_SPINLOCK(req_lock);
static blkbuf_node_s *req_in_using;
LIST_HDR_S(NVMEreq_lhdr);

struct NVMe_Controller_Registers * NVMe_CTRL_REG = NULL;
struct NVMe_Identify_Controller_Data_Structure * NVMeID = NULL;

// struct request_queue NVMe_request;

struct Submission_Queue_Entry * ADMIN_Submission_Queue = NULL;
struct Completion_Queue_Entry * ADMIN_Completion_Queue = NULL;

unsigned int * ADMIN_SQ_TDBL = NULL;
unsigned int * ADMIN_CQ_HDBL = NULL;

unsigned short ADMIN_SQ_Tail_DoorBell = 0;
unsigned short ADMIN_CQ_Head_DoorBell = 0;

struct Submission_Queue_Entry * IO_Submission_Queue = NULL;
struct Completion_Queue_Entry * IO_Completion_Queue = NULL;

unsigned int * IO_SQ_TDBL = NULL;
unsigned int * IO_CQ_HDBL = NULL;

unsigned short IO_SQ_Tail_DoorBell = 0;
unsigned short IO_CQ_Head_DoorBell = 0;

// long NVMe_cmd_out()
// {
// 	struct Submission_Queue_Entry * sqptr = NULL;
// 	wait_queue_T *wait_queue_tmp = container_of(list_next(&NVMe_request.wait_queue_list.wait_list),wait_queue_T,wait_list);
// 	struct block_buffer_node * node = NVMe_request.in_using = container_of(wait_queue_tmp,struct block_buffer_node,wait_queue);
// 	list_del(&NVMe_request.in_using->wait_queue.wait_list);
// 	NVMe_request.block_request_count--;

// 	switch(node->cmd)
// 	{
// 		case NVM_CMD_WRITE:
// 			sqptr = IO_Submission_Queue + (IO_SQ_Tail_DoorBell & 0x1);
// 			memset(sqptr,0,sizeof(struct Submission_Queue_Entry));
// 			IO_SQ_Tail_DoorBell++;

// 			sqptr->CID = IO_SQ_Tail_DoorBell;
// 			sqptr->OPC = NVM_CMD_WRITE;	//Write command 0x01;
// 			sqptr->NSID = 1;		//NSID=1
// 			sqptr->PRP_SGL_Entry1 = (unsigned long)Virt_To_Phy(node->buffer);
// 			sqptr->Dword10 = node->LBA & 0xffffffff;		//SLBA-lower = 0
// 			sqptr->Dword11 = (node->LBA >> 32) & 0xffffffff;	//SLBA-upper = 0
// 			sqptr->Dword12 = node->count;	//LR=0,FUA=0,PRINFO=0,DTYPE=0,NLB=1
// 			io_mfence();

// 			*IO_SQ_TDBL = IO_SQ_Tail_DoorBell & 0x1;	////SQ index
// 			io_mfence();
// 			break;

// 		case NVM_CMD_READ:
// 			sqptr = IO_Submission_Queue + (IO_SQ_Tail_DoorBell & 0x1);
// 			memset(sqptr,0,sizeof(struct Submission_Queue_Entry));
// 			IO_SQ_Tail_DoorBell++;

// 			sqptr->CID = IO_SQ_Tail_DoorBell;
// 			sqptr->OPC = NVM_CMD_READ;	//Read command 0x02;
// 			sqptr->NSID = 1;		//NSID=1
// 			sqptr->PRP_SGL_Entry1 = (unsigned long)Virt_To_Phy(node->buffer);
// 			sqptr->Dword10 = node->LBA & 0xffffffff;		//SLBA-lower = 0
// 			sqptr->Dword11 = (node->LBA >> 32) & 0xffffffff;	//SLBA-upper = 0
// 			sqptr->Dword12 = node->count;	//LR=0,FUA=0,PRINFO=0,NLB=1
// 			io_mfence();

// 			*IO_SQ_TDBL = IO_SQ_Tail_DoorBell & 0x1;	////SQ index
// 			io_mfence();
// 			break;

// 		default:
// 			break;
// 	}

// 	return 1;
// }

// void NVMe_end_request(struct block_buffer_node * node)
// {
// 	if(node == NULL)
// 		color_printk(RED,BLACK,"end_request error\n");

// 	node->wait_queue.tsk->state = TASK_RUNNING;
// 	insert_task_queue(node->wait_queue.tsk);
// //	node->wait_queue.tsk->flags |= NEED_SCHEDULE;
// 	current->flags |= NEED_SCHEDULE;

// 	kfree((unsigned long *)NVMe_request.in_using);
// 	NVMe_request.in_using = NULL;

// 	if(NVMe_request.block_request_count)
// 		NVMe_cmd_out();
// }

// void NVMe_add_request(struct block_buffer_node * node)
// {
// 	list_add_to_before(&NVMe_request.wait_queue_list.wait_list,&node->wait_queue.wait_list);
// 	NVMe_request.block_request_count++;
// }

// void NVMe_read_handler(unsigned long nr, unsigned long parameter)
// {
// 	struct block_buffer_node * node = ((struct request_queue *)parameter)->in_using;

// 	color_printk(RED,WHITE,"IO_Completion_Queue Entry READ:%#010x:%#010x,SQHD:%#06x,SQID:%#06x,CID:%#06x,P:%#03x,SC:%#04x,SCT:%#03x,M:%#03x,DNR:%#03x\n",IO_CQ_Head_DoorBell,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->CMD,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->SQHD,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->SQID,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->CID,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->P,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->SC,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->SCT,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->M,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->DNR);

// 	*IO_CQ_HDBL = IO_CQ_Head_DoorBell & 0x1;	////CQ index
// 	IO_CQ_Head_DoorBell++;

// 	NVMe_end_request(node);
// }

// void NVMe_write_handler(unsigned long nr, unsigned long parameter)
// {
// 	struct block_buffer_node * node = ((struct request_queue *)parameter)->in_using;

// 	color_printk(RED,WHITE,"IO_Completion_Queue Entry WRITE:%#010x:%#010x,SQHD:%#06x,SQID:%#06x,CID:%#06x,P:%#03x,SC:%#04x,SCT:%#03x,M:%#03x,DNR:%#03x\n",IO_CQ_Head_DoorBell,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->CMD,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->SQHD,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->SQID,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->CID,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->P,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->SC,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->SCT,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->M,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->DNR);

// 	*IO_CQ_HDBL = IO_CQ_Head_DoorBell & 0x1;	////CQ index
// 	IO_CQ_Head_DoorBell++;

// 	NVMe_end_request(node);
// }

// void NVMe_other_handler(unsigned long nr, unsigned long parameter)
// {
// 	struct block_buffer_node * node = ((struct request_queue *)parameter)->in_using;

// 	color_printk(RED,WHITE,"IO_Completion_Queue Entry OTHER:%#010x:%#010x,SQHD:%#06x,SQID:%#06x,CID:%#06x,P:%#03x,SC:%#04x,SCT:%#03x,M:%#03x,DNR:%#03x\n",IO_CQ_Head_DoorBell,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->CMD,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->SQHD,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->SQID,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->CID,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->P,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->SC,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->SCT,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->M,(IO_Completion_Queue + (IO_CQ_Head_DoorBell & 0x1))->DNR);

// 	*IO_CQ_HDBL = IO_CQ_Head_DoorBell & 0x1;	////CQ index
// 	IO_CQ_Head_DoorBell++;

// 	NVMe_end_request(node);
// }

// struct block_buffer_node * NVMe_make_request(long cmd,unsigned long blocks,long count,unsigned char * buffer)
// {
// 	struct block_buffer_node * node = (struct block_buffer_node *)kmalloc(sizeof(struct block_buffer_node),0);
// 	wait_queue_init(&node->wait_queue,current);

// 	switch(cmd)
// 	{
// 		case NVM_CMD_READ:
// 			node->end_handler = NVMe_read_handler;
// 			node->cmd = NVM_CMD_READ;
// 			break;

// 		case NVM_CMD_WRITE:
// 			node->end_handler = NVMe_write_handler;
// 			node->cmd = NVM_CMD_WRITE;
// 			break;

// 		default:
// 			node->end_handler = NVMe_other_handler;
// 			node->cmd = cmd;
// 			break;
// 	}

// 	node->LBA = blocks;
// 	node->count = count;
// 	node->buffer = buffer;

// 	return node;
// }

// void NVMe_submit(struct block_buffer_node * node)
// {
// 	NVMe_add_request(node);

// 	if(NVMe_request.in_using == NULL)
// 		NVMe_cmd_out();
// }

// void NVMe_wait_for_finish()
// {
// 	current->state = TASK_UNINTERRUPTIBLE;
// 	schedule();
// }

// long NVMe_open()
// {
// 	color_printk(BLACK,WHITE,"NVMe Opened\n");
// 	return 1;
// }

// long NVMe_close()
// {
// 	color_printk(BLACK,WHITE,"NVMe Closed\n");
// 	return 1;
// }

// long NVMe_ioctl(long cmd,long arg)
// {
// 	struct block_buffer_node * node = NULL;
// 	return 0;
// }

// long NVMe_transfer(long cmd,unsigned long blocks,long count,unsigned char * buffer)
// {
// 	struct block_buffer_node * node = NULL;
// 	if(cmd == NVM_CMD_READ || cmd == NVM_CMD_WRITE)
// 	{
// 		node = NVMe_make_request(cmd,blocks,count,buffer);
// 		NVMe_submit(node);
// 		NVMe_wait_for_finish();
// 	}
// 	else
// 	{
// 		return 0;
// 	}

// 	return 1;
// }

// struct block_device_operation NVMe_device_operation =
// {
// 	.open = NVMe_open,
// 	.close = NVMe_close,
// 	.ioctl = NVMe_ioctl,
// 	.transfer = NVMe_transfer,
// };

// unsigned long NVMe_install(unsigned long irq,void * arg)
// {
// 	return 0;
// }

// void NVMe_enable(unsigned long irq)
// {
// }

hw_int_controller_s NVMe_int_controller =
{
	// .enable = NVMe_enable,
	// .disable = NULL,
	// .install = NVMe_install,
	// .uninstall = NULL,
	.ack = IOAPIC_edge_ack,
};

void NVMe_ADMIN_handler(unsigned long parameter, pt_regs_s * regs)
{
	// struct block_buffer_node * node = ((struct request_queue *)parameter)->in_using;
	// node->end_handler(nr,parameter);
	color_printk(RED, BLACK, "NVME Admin Handler\n");
	while (1);
}

void NVMe_IO_handler(unsigned long parameter, pt_regs_s * regs)
{
	// blkbuf_node_s *node = ((blkbuf_node_s *)parameter)->in_using;
	// node->end_handler(nr, parameter);
	color_printk(RED, BLACK, "NVME IO Handler\n");
	while (1);
}


void NVMe_init(struct PCI_Header_00 *NVMe_PCI_HBA)
{
	int bus,device,function;
	unsigned int index = 0;
	unsigned int value = 0;
	unsigned long * tmp = NULL;
	unsigned int * ptr = NULL;

	int BIR_idx;
	u64 TBIR = 0;
	unsigned int * TADDR = NULL;
	u64 PBIR = 0;
	unsigned int * PADDR = NULL;

	int i;

	//// init wait queue
	// wait_queue_init(&NVMe_request.wait_queue_list,NULL);
	// NVMe_request.in_using = NULL;
	// NVMe_request.block_request_count = 0;

	// get NVMe Controller register
	NVMe_CTRL_REG = (struct NVMe_Controller_Registers *)phys_to_virt(NVMe_PCI_HBA->BAR_base_addr[0]);
	u64 map_start = NVMe_PCI_HBA->BAR_base_addr[0];
	u64 map_size = NVMe_PCI_HBA->BAR_space_limit[0];
	myos_ioremap(map_start, map_size);
	flush_tlb_local();
	//
	u64 cap = NVMe_CTRL_REG->CAP;

	void *ASQ = (void *)NVMe_CTRL_REG->ASQ;
	void *ACQ = (void *)NVMe_CTRL_REG->ACQ;
	u64 QSZ = abs(ACQ - ASQ);
	myos_ioremap((phys_addr_t)ASQ, QSZ);
	myos_ioremap((phys_addr_t)ACQ, QSZ);
	flush_tlb_local();

	// set NVMe Controller register
	NVMe_CTRL_REG->CC = 0;
	__mb();

	while(NVMe_CTRL_REG->CSTS & 1)
		barrier();

	NVMe_CTRL_REG->AQA = 0x00030003;	//ACQS=3,ASQS=3
	NVMe_CTRL_REG->CC = 0x00460001;		//IOCQES=4,IOSQES=6,SHN=0,AMS=0,MPS=0,CSS=0,EN=1
	__mb();

	while(!(NVMe_CTRL_REG->CSTS & 1))
		barrier();


	// detect MSI-X capability
	bus = (NVMe_PCI_HBA->BDF >> 16) & 0xff;
	device = (NVMe_PCI_HBA->BDF >> 11) & 0x1f;
	function = (NVMe_PCI_HBA->BDF >> 8) & 0x7;
	index = NVMe_PCI_HBA->CapabilitiesPointer;

	while(index != 0)
	{
		value = Read_PCI_Config(bus, device, function, index);
		if((value & 0xff) == 0x11)
			break;

		index = (value >> 8) & 0xff;
	}

	value = Read_PCI_Config(bus, device, function, index + 4);
	BIR_idx = value & 0x7;
	while (BIR_idx > 5);
	TBIR = NVMe_PCI_HBA->BAR_base_addr[BIR_idx];
	TADDR = (unsigned int *)phys_to_virt(TBIR + (value & (~0x7)));

	value = Read_PCI_Config(bus, device, function, index + 8);
	BIR_idx = value & 0x7;
	while (BIR_idx > 5);
	PBIR = NVMe_PCI_HBA->BAR_base_addr[BIR_idx];
	PADDR = (unsigned int *)phys_to_virt(PBIR + (value & (~0x7)));

	/// Configuration MSI-X
	//MSI-X Table Entry 0	-> Admin Completion_Queue_Entry Interrupt Handler
	*TADDR = 0xfee00000;
	*(TADDR + 1) = 0;
	*(TADDR + 2) = APIC_PIRQA;
	*(TADDR + 3) = 0;

	//MSI-X Table Entry 1 -> I/O Completion_Queue_Entry Interrupt Handler
	*(TADDR + 4) = 0xfee00000;
	*(TADDR + 5) = 0;
	*(TADDR + 6) = APIC_PIRQA;
	*(TADDR + 7) = 0;

	//MSI-X Pending Table Entry 0~127
	*PADDR = 0;
	*(PADDR + 1) = 0;
	*(PADDR + 2) = 0;
	*(PADDR + 3) = 0;

	// enable msi-x : MSI-X MXC.MXE=1,MXC.FM=0
	value = Read_PCI_Config(bus, device, function, index) & 0xbfffffff;
	value = value | 0x80000000;
	Write_PCI_Config(bus, device, function, index, value);

	/// register interrupt
	//Admin Completion_Queue_Entry Interrupt Handler
	register_irq(APIC_PIRQA, NULL , "NVMe_ADMIN", 0, &NVMe_int_controller, &NVMe_ADMIN_handler);
	//I/O Completion_Queue_Entry Interrupt Handler
	register_irq(APIC_PIRQB, NULL, "NVMe0_IO", 0, &NVMe_int_controller, &NVMe_IO_handler);

	//// get struct Submission Queue and Completion Queue
	ADMIN_Submission_Queue = (struct Submission_Queue_Entry *)phys_to_virt(NVMe_CTRL_REG->ASQ);
	ADMIN_Completion_Queue = (struct Completion_Queue_Entry *)phys_to_virt(NVMe_CTRL_REG->ACQ);

	//// clean struct Submission Queue and Completion Queue
	memset(ADMIN_Submission_Queue, 0, sizeof(struct Submission_Queue_Entry) * 4);
	memset(ADMIN_Completion_Queue, 0, sizeof(struct Completion_Queue_Entry) * 4);

	//// Set ADMIN_SQ_TDBL & ADMIN_CQ_HDBL Address
	ADMIN_SQ_TDBL = (unsigned int *)((char *)phys_to_virt((unsigned long)NVMe_PCI_HBA->BAR_base_addr[0]) + 0x1000);
	ADMIN_CQ_HDBL = (unsigned int *)((char *)phys_to_virt((unsigned long)NVMe_PCI_HBA->BAR_base_addr[0]) + 0x1004);

	*ADMIN_CQ_HDBL = ADMIN_CQ_Head_DoorBell & 0x3;	////CQ index
	ADMIN_CQ_Head_DoorBell++;
	__mb();
}

void NVMe_exit()
{
	unregister_irq(APIC_PIRQA);
	unregister_irq(APIC_PIRQB);
}

static int NVMErq_deamon(void *param)
{
	current->flags |= PF_NOFREEZE;

	for (;;) {
		set_current_state(TASK_INTERRUPTIBLE);
		if (NVMEreq_lhdr.count == 0)
			schedule();
		__set_current_state(TASK_RUNNING);

		while (NVMEreq_lhdr.count != 0)
		{		
			spin_lock(&req_lock);
			if (req_in_using == NULL)
			{
				List_s *wq_lp = list_hdr_dequeue(&NVMEreq_lhdr);
				blkbuf_node_s *node = container_of(wq_lp, blkbuf_node_s, req_list);
				req_in_using = node;
				spin_unlock_no_resched(&req_lock);

				// cmd_out(node);

				spin_lock(&req_lock);
			}
			spin_unlock_no_resched(&req_lock);
			break;
		}
	}
	return 1;
}

void init_NVMErqd()
{
	thread = kthread_run(NVMErq_deamon, NULL, "NVMErqd");

	// color_printk(WHITE, BLACK, "ATA disk: initialized\n");
}


void NVMe_IOqueue_init()
{
	struct Submission_Queue_Entry *ASQ_ptr = ADMIN_Submission_Queue;
	//// Set Features command
	ASQ_ptr->CID			= 0xA5A5;
	ASQ_ptr->NSID			= 0;
	ASQ_ptr->OPC			= 0x09;		////Features command
	ASQ_ptr->PRP_SGL_Entry1	= 0;
	ASQ_ptr->Dword10		= 0x07;		////SV=0,FID=7
	ASQ_ptr->Dword11		= 0x50005;	////NCQR=5,NSQR=5

	NVMeID = (struct NVMe_Identify_Controller_Data_Structure *)kzalloc(PAGE_SIZE, GFP_KERNEL);
	ASQ_ptr++;
	//// Send Identify Command
	ASQ_ptr->CID			= 0x5A5A;
	ASQ_ptr->NSID			= 0;
	ASQ_ptr->OPC			= 0x06;		////Identify command
	ASQ_ptr->PRP_SGL_Entry1	= (unsigned long)virt_to_phys((virt_addr_t)NVMeID);
	ASQ_ptr->Dword10		= 0x01;		////CNTID=0,CNS=1
	__mb();

	// //// print struct Submission_Queue_Entry
	// ptr = (unsigned int *)(ADMIN_Submission_Queue + 0);
	// color_printk(RED,WHITE,"ADMIN_Submission_Queue Entry 0:");
	// for(i = 0;i < sizeof(struct Submission_Queue_Entry) / 4;i++,ptr++)
	// 	color_printk(RED,WHITE,"%#010x,",*ptr);
	// color_printk(RED,WHITE,"\n");
	// ptr = (unsigned int *)(ADMIN_Submission_Queue + 1);
	// color_printk(RED,WHITE,"ADMIN_Submission_Queue Entry 1:");
	// for(i = 0;i < sizeof(struct Submission_Queue_Entry) / 4;i++,ptr++)
	// 	color_printk(RED,WHITE,"%#010x,",*ptr);
	// color_printk(RED,WHITE,"\n");

	//// print Complete DoorBell index
	ADMIN_SQ_Tail_DoorBell++;
	*ADMIN_SQ_TDBL = ADMIN_SQ_Tail_DoorBell & 0x3;	////SQ index
	__mb();

	//// print struct Completion_Queue_Entry
	// color_printk(RED,WHITE,"ADMIN_Completion_Queue Entry 0:%#010x,SQHD:%#06x,SQID:%#06x,CID:%#06x,P:%#03x,SC:%#04x,SCT:%#03x,M:%#03x,DNR:%#03x\n",ADMIN_Completion_Queue->CMD,ADMIN_Completion_Queue->SQHD,ADMIN_Completion_Queue->SQID,ADMIN_Completion_Queue->CID,ADMIN_Completion_Queue->P,ADMIN_Completion_Queue->SC,ADMIN_Completion_Queue->SCT,ADMIN_Completion_Queue->M,ADMIN_Completion_Queue->DNR);
	////
	// ptr = (unsigned int *)ADMIN_Completion_Queue;
	// color_printk(RED,WHITE,"ADMIN_Completion_Queue Entry 0:");
	// for(i = 0;i < sizeof(struct Completion_Queue_Entry) / 4;i++,ptr++)
	// 	color_printk(RED,WHITE,"%#010x,",*ptr);
	// color_printk(RED,WHITE,"\n");

	//// print Complete DoorBell index
	*ADMIN_CQ_HDBL = ADMIN_CQ_Head_DoorBell & 0x3;	////CQ index
	ADMIN_CQ_Head_DoorBell++;
	ADMIN_SQ_Tail_DoorBell++;
	*ADMIN_SQ_TDBL = ADMIN_SQ_Tail_DoorBell & 0x3;	////SQ index
	__mb();
	// color_printk(BLUE,WHITE,"ADMIN_CQ_HDBL:%#010x\n",*ADMIN_CQ_HDBL);
	// color_printk(BLACK,WHITE,"ADMIN_SQ_TDBL:%#010x\n",*ADMIN_SQ_TDBL);

// ////	print struct Completion_Queue_Entry
// 	color_printk(RED,WHITE,"ADMIN_Completion_Queue Entry 1:%#010x,SQHD:%#06x,SQID:%#06x,CID:%#06x,P:%#03x,SC:%#04x,SCT:%#03x,M:%#03x,DNR:%#03x\n",(ADMIN_Completion_Queue + 1)->CMD,(ADMIN_Completion_Queue + 1)->SQHD,(ADMIN_Completion_Queue + 1)->SQID,(ADMIN_Completion_Queue + 1)->CID,(ADMIN_Completion_Queue + 1)->P,(ADMIN_Completion_Queue + 1)->SC,(ADMIN_Completion_Queue + 1)->SCT,(ADMIN_Completion_Queue + 1)->M,(ADMIN_Completion_Queue + 1)->DNR);
// ////
// 	ptr = (unsigned int *)(ADMIN_Completion_Queue + 1);
// 	color_printk(RED,WHITE,"ADMIN_Completion_Queue Entry 1:");
// 	for(i = 0;i < sizeof(struct Completion_Queue_Entry) / 4;i++,ptr++)
// 		color_printk(RED,WHITE,"%#010x,",*ptr);
// 	color_printk(RED,WHITE,"\n");

// /////	print The Identify Controller data structure of Identify Command
// 	color_printk(RED,BLACK,"The Identify Controller data structure (size:%d):\n",sizeof(struct NVMe_Identify_Controller_Data_Structure));
// 	color_printk(RED,BLACK,"VID:%#06x\n",NVMeID->VID);
// 	color_printk(RED,BLACK,"SSVID:%#06x\n",NVMeID->SSVID);
// 	color_printk(RED,BLACK,"SN:");
// 	for(i = 0;i < 20;i++)
// 		color_printk(RED,BLACK,"%c",NVMeID->SN[i]);
// 	color_printk(RED,BLACK,"\n");
// 	color_printk(RED,BLACK,"MN:");
// 	for(i = 0;i < 40;i++)
// 		color_printk(RED,BLACK,"%c",NVMeID->MN[i]);
// 	color_printk(RED,BLACK,"\n");
// 	color_printk(RED,BLACK,"FR:");
// 	for(i = 0;i < 8;i++)
// 		color_printk(RED,BLACK,"%c",NVMeID->FR[i]);
// 	color_printk(RED,BLACK,"\n");
// 	color_printk(RED,BLACK,"MDTS:%#04x\n",NVMeID->MDTS);
// 	color_printk(RED,BLACK,"CNTLID:%#06x\n",NVMeID->CNTLID);
// 	color_printk(RED,BLACK,"VER:%#010x\n",NVMeID->VER);
// 	color_printk(RED,BLACK,"CTRATT:%#010x\n",NVMeID->CTRATT);
// 	color_printk(RED,BLACK,"FGUID:%#010x,%08x,%08x,%08x\n",NVMeID->FGUID[3],NVMeID->FGUID[2],NVMeID->FGUID[1],NVMeID->FGUID[0]);
// 	color_printk(RED,BLACK,"OACS:%#06x\n",NVMeID->OACS);
// 	color_printk(RED,BLACK,"HMPRE:%#010x\n",NVMeID->HMPRE);
// 	color_printk(RED,BLACK,"HMMIN:%#010x\n",NVMeID->HMMIN);
// 	color_printk(RED,BLACK,"DSTO:%#04x\n",NVMeID->DSTO);
// 	color_printk(RED,BLACK,"SANICAP:%#010x\n",NVMeID->SANICAP);
// 	color_printk(RED,BLACK,"SQES:%#04x\n",NVMeID->SQES);
// 	color_printk(RED,BLACK,"CQES:%#04x\n",NVMeID->CQES);
// 	color_printk(RED,BLACK,"NN:%#010x\n",NVMeID->NN);
// 	color_printk(RED,BLACK,"SGLS:%#010x\n",NVMeID->SGLS);

// 	IO_Completion_Queue = (struct Completion_Queue_Entry *)kmalloc(PAGE_4K_SIZE,0);
// 	memset(IO_Completion_Queue,0x00,4096);
// 	///Create Completion
// 	(ADMIN_Submission_Queue + 2)->OPC = 0x05;	////Create I/O Completion Queue command
// 	(ADMIN_Submission_Queue + 2)->CID = 0x55AA;
// 	(ADMIN_Submission_Queue + 2)->PRP_SGL_Entry1 = (unsigned long)Virt_To_Phy(IO_Completion_Queue);
// 	(ADMIN_Submission_Queue + 2)->Dword10 = 0x10001;		////QSIZE=2,QID=1
// 	(ADMIN_Submission_Queue + 2)->Dword11 = 0x10003;		////IV=1,IEN=1,PC=1
// 	color_printk(BLACK,WHITE,"IO_Completion_Queue:%#018lx\n",IO_Completion_Queue);

// ////	print struct Submission_Queue_Entry
// 	ptr = (unsigned int *)(ADMIN_Submission_Queue + 2);
// 	color_printk(RED,WHITE,"ADMIN_Submission_Queue Entry 2:");
// 	for(i = 0;i < sizeof(struct Submission_Queue_Entry) / 4;i++,ptr++)
// 		color_printk(RED,WHITE,"%#010x,",*ptr);
// 	color_printk(RED,WHITE,"\n");

// ////	print Complete DoorBell index
// 	*ADMIN_CQ_HDBL = ADMIN_CQ_Head_DoorBell & 0x3;	////CQ index
// 	ADMIN_CQ_Head_DoorBell++;
// 	ADMIN_SQ_Tail_DoorBell++;
// 	*ADMIN_SQ_TDBL = ADMIN_SQ_Tail_DoorBell & 0x3;	////SQ index
// 	io_mfence();
// 	color_printk(BLUE,WHITE,"ADMIN_CQ_HDBL:%#010x\n",*ADMIN_CQ_HDBL);
// 	color_printk(BLACK,WHITE,"ADMIN_SQ_TDBL:%#010x\n",*ADMIN_SQ_TDBL);

// ////	print struct Completion_Queue_Entry
// 	color_printk(RED,WHITE,"ADMIN_Completion_Queue Entry 2:%#010x,SQHD:%#06x,SQID:%#06x,CID:%#06x,P:%#03x,SC:%#04x,SCT:%#03x,M:%#03x,DNR:%#03x\n",(ADMIN_Completion_Queue + 2)->CMD,(ADMIN_Completion_Queue + 2)->SQHD,(ADMIN_Completion_Queue + 2)->SQID,(ADMIN_Completion_Queue + 2)->CID,(ADMIN_Completion_Queue + 2)->P,(ADMIN_Completion_Queue + 2)->SC,(ADMIN_Completion_Queue + 2)->SCT,(ADMIN_Completion_Queue + 2)->M,(ADMIN_Completion_Queue + 2)->DNR);
// ////
// 	ptr = (unsigned int *)(ADMIN_Completion_Queue + 2);
// 	color_printk(RED,WHITE,"ADMIN_Completion_Queue Entry 2:");
// 	for(i = 0;i < sizeof(struct Completion_Queue_Entry) / 4;i++,ptr++)
// 		color_printk(RED,WHITE,"%#010x,",*ptr);
// 	color_printk(RED,WHITE,"\n");

// 	///Create Submission
// 	(ADMIN_Submission_Queue + 3)->OPC = 0x01;	//Create I/O Submission Queue command
// 	(ADMIN_Submission_Queue + 3)->CID = 0xAA55;
// 	IO_Submission_Queue = (struct Submission_Queue_Entry *)kmalloc(PAGE_4K_SIZE,0);
// 	memset(IO_Submission_Queue,0x00,4096);
// 	(ADMIN_Submission_Queue + 3)->PRP_SGL_Entry1 = (unsigned long)Virt_To_Phy(IO_Submission_Queue);
// 	(ADMIN_Submission_Queue + 3)->Dword10 = 0x10001;	////QSIZE=2,QID=1
// 	(ADMIN_Submission_Queue + 3)->Dword11 = 0x10001;	////CQID=1,QPRIO=0,PC=1
// 	color_printk(BLACK,WHITE,"IO_Submission_Queue:%#018lx\n",IO_Submission_Queue);

// ////	print struct Submission_Queue_Entry
// 	ptr = (unsigned int *)(ADMIN_Submission_Queue + 3);
// 	color_printk(RED,WHITE,"ADMIN_Submission_Queue Entry 3:");
// 	for(i = 0;i < sizeof(struct Submission_Queue_Entry) / 4;i++,ptr++)
// 		color_printk(RED,WHITE,"%#010x,",*ptr);
// 	color_printk(RED,WHITE,"\n");

// ////	print Complete DoorBell index
// 	*ADMIN_CQ_HDBL = ADMIN_CQ_Head_DoorBell & 0x3;	////CQ index
// 	ADMIN_CQ_Head_DoorBell++;
// 	ADMIN_SQ_Tail_DoorBell++;
// 	*ADMIN_SQ_TDBL = ADMIN_SQ_Tail_DoorBell & 0x3;	////SQ index
// 	io_mfence();
// 	color_printk(BLUE,WHITE,"ADMIN_CQ_HDBL:%#010x\n",*ADMIN_CQ_HDBL);
// 	color_printk(BLACK,WHITE,"ADMIN_SQ_TDBL:%#010x\n",*ADMIN_SQ_TDBL);

// ////	print struct Completion_Queue_Entry
// 	color_printk(RED,WHITE,"ADMIN_Completion_Queue Entry 3:%#010x,SQHD:%#06x,SQID:%#06x,CID:%#06x,P:%#03x,SC:%#04x,SCT:%#03x,M:%#03x,DNR:%#03x\n",(ADMIN_Completion_Queue + 3)->CMD,(ADMIN_Completion_Queue + 3)->SQHD,(ADMIN_Completion_Queue + 3)->SQID,(ADMIN_Completion_Queue + 3)->CID,(ADMIN_Completion_Queue + 3)->P,(ADMIN_Completion_Queue + 3)->SC,(ADMIN_Completion_Queue + 3)->SCT,(ADMIN_Completion_Queue + 3)->M,(ADMIN_Completion_Queue + 3)->DNR);
// ////
// 	ptr = (unsigned int *)(ADMIN_Completion_Queue + 3);
// 	color_printk(RED,WHITE,"ADMIN_Completion_Queue Entry 3:");
// 	for(i = 0;i < sizeof(struct Completion_Queue_Entry) / 4;i++,ptr++)
// 		color_printk(RED,WHITE,"%#010x,",*ptr);
// 	color_printk(RED,WHITE,"\n");
// 	*ADMIN_CQ_HDBL = ADMIN_CQ_Head_DoorBell & 0x3;	////CQ index
// 	ADMIN_CQ_Head_DoorBell++;
// 	color_printk(BLUE,WHITE,"ADMIN_CQ_HDBL:%#010x\n",*ADMIN_CQ_HDBL);

// ////	Set IO_SQ_TDBL & IO_CQ_HDBL Address
// 	IO_SQ_TDBL = (unsigned int *)((char *)Phy_To_Virt((unsigned long)NVMe_PCI_HBA.Base32Address0) + 0x1008);
// 	IO_CQ_HDBL = (unsigned int *)((char *)Phy_To_Virt((unsigned long)NVMe_PCI_HBA.Base32Address0) + 0x100c);

// 	*IO_CQ_HDBL = IO_CQ_Head_DoorBell & 0x1;	////CQ index
// 	IO_CQ_Head_DoorBell++;

// 	io_mfence();
}