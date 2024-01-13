#include <linux/kernel/kthread.h>
#include <linux/kernel/stddef.h>
#include <linux/mm/mm.h>
#include <linux/kernel/delay.h>

#include "myos_XHCI.h"
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
#include <obsolete/device.h>
#include "myos_block.h"


static task_s *thread;
static DEFINE_SPINLOCK(req_lock);
static blkbuf_node_s *req_in_using;
LIST_HDR_S(XHCIreq_lhdr);

u16 RingSegMinSize = 16;
u16 RingSegMaxSize = SZ_64K / sizeof(XHCI_TRB_s);
#define RING_SIZE	16

// 主控寄存器组
u64				XHCI_BAR0_base = 0;
XHCI_HCCR_s		*XHCI_HostCtrl_Cap_Regs_ptr = NULL;
XHCI_HCOR_s		*XHCI_HostCtrl_Ops_Regs_ptr = NULL;
XHCI_HCRTR_s	*XHCI_HostCtrl_RunTime_Regs_ptr = NULL;
u32				*XHCI_DoorBell_Regptr = NULL;
XHCI_DevCtx_s	**DCBAAP = NULL;
u8				MaxSlotEnts = 0;
	
// 命令环
XHCI_CmdTRB_s	*Host_CmdRing_Base = NULL;
XHCI_CmdTRB_s	*Host_CmdRing_Curr = NULL;
u16				Host_CmdRing_Size = 0;

// 主中断
XHCI_IRS_s		*Main_Intr_RegSet_ptr = NULL;
XHCI_EvtTRB_s	*MainEventRing_Base = NULL;
XHCI_EvtTRB_s	*MainEventRing_Curr = NULL;
u16				MainEventRing_Size = 0;

// 测试用
XHCI_HCCR_s XHCI_HCCR_val;
XHCI_HCOR_s XHCI_HCOR_val;
XHCI_HCRTR_s XHCI_HCRTR_val;
u64				EventData_Test[4];

void XHCI_test_getRegVals()
{
	XHCI_HCCR_val = *XHCI_HostCtrl_Cap_Regs_ptr;
	XHCI_HCOR_val = *XHCI_HostCtrl_Ops_Regs_ptr;
	XHCI_HCRTR_val = *XHCI_HostCtrl_RunTime_Regs_ptr;
}

long XHCI_cmd_out(blkbuf_node_s *node)
{
	memset(Host_CmdRing_Curr, 0, sizeof(XHCI_CmdTRB_s));
	Host_CmdRing_Curr->Cycle_Bit = 1;
	Host_CmdRing_Curr->TRB_Type = node->cmd;

	switch(node->cmd)
	{
		case EVENT_DATA:
			XHCI_EvtDataTRB_s *EvtData_TRB = (XHCI_EvtDataTRB_s *)Host_CmdRing_Curr;
			EvtData_TRB->Chain_Bit = 0;
			EvtData_TRB->IntrOnComp = 1;
			EvtData_TRB->CmdTRB_ptr = (u64)&EventData_Test;
			break;
		case NO_OP:
			break;

		default:
			break;
	}

	Host_CmdRing_Curr++;
	if (Host_CmdRing_Curr->TRB_Type == LINK)
	{
		XHCI_LinkTRB_s *LinkTRB_ptr = (XHCI_LinkTRB_s *)Host_CmdRing_Curr;
		Host_CmdRing_Curr = (XHCI_CmdTRB_s *)phys_to_virt(LinkTRB_ptr->CmdTRB_ptr);
	}
	*(XHCI_DoorBell_Regptr + 0) = 0;
	__mb();

	return 1;
}

void XHCI_end_request(blkbuf_node_s *node)
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

void XHCI_read_handler(unsigned long parameter)
{
	// *IO_CQ_HDBL = IOCQ_Head_Idx & 0x1;	////CQ index
	// IOCQ_Head_Idx++;
}

void XHCI_write_handler(unsigned long parameter)
{
	// *IO_CQ_HDBL = IOCQ_Head_Idx & 0x1;	////CQ index
	// IOCQ_Head_Idx++;
}

void XHCI_other_handler(unsigned long parameter)
{
	// *IO_CQ_HDBL = IOCQ_Head_Idx & 0x1;	////CQ index
	// IOCQ_Head_Idx++;
}

blkbuf_node_s *XHCI_make_request(long cmd, unsigned long blk_idx, long count, unsigned char *buffer)
{
	blkbuf_node_s *node = kzalloc(sizeof(blkbuf_node_s),0);
	list_init(&node->req_list, node);
	node->buffer = buffer;

	switch(cmd)
	{
		// case NVM_CMD_READ:
		// 	node->end_handler = XHCI_read_handler;
		// 	node->cmd = NVM_CMD_READ;
		// 	break;

		// case NVM_CMD_WRITE:
		// 	node->end_handler = XHCI_write_handler;
		// 	node->cmd = NVM_CMD_WRITE;
		// 	break;

		default:
			node->end_handler = XHCI_other_handler;
			node->cmd = cmd;
			break;
	}

	node->LBA = blk_idx;
	node->count = count;
	node->buffer = buffer;

	return node;
}


long XHCI_ioctl(unsigned controller, unsigned disk, long cmd, long arg)
{
	blkbuf_node_s *node = NULL;

	node = XHCI_make_request(cmd, 0, 0, 0);
	// DECLARE_COMPLETION_ONSTACK(done);
	// node->done = &done;
	node->task = current;

	spin_lock(&req_lock);
	list_hdr_enqueue(&XHCIreq_lhdr, &node->req_list);
	spin_unlock_no_resched(&req_lock);

	__set_current_state(TASK_UNINTERRUPTIBLE);
	wake_up_process(thread);
	// wait_for_completion(&done);
	schedule();

	if (node != NULL)
		kfree(node);
	return -ENOERR;

	return 1;
}

long XHCI_transfer(unsigned controller, unsigned disk, long cmd, unsigned long blk_idx, long count, unsigned char *buffer)
{
	// blkbuf_node_s *node = NULL;
	// if(cmd == NVM_CMD_READ || cmd == NVM_CMD_WRITE)
	// {
	// 	// node = NVMe_make_request(cmd,blocks,count,buffer);
	// 	// NVMe_submit(node);
	// 	// NVMe_wait_for_finish();

	// 	node = NVMe_make_request(cmd, blk_idx, count, buffer);
	// 	// DECLARE_COMPLETION_ONSTACK(done);
	// 	// node->done = &done;
	// 	node->task = current;

	// 	spin_lock(&req_lock);
	// 	list_hdr_enqueue(&NVMEreq_lhdr, &node->req_list);
	// 	spin_unlock_no_resched(&req_lock);

	// 	__set_current_state(TASK_UNINTERRUPTIBLE);
	// 	wake_up_process(thread);
	// 	// wait_for_completion(&done);
	// 	schedule();

	// 	if (node != NULL)
	// 		kfree(node);
	// 	return -ENOERR;
	// }
	// else
	// {
	// 	return -EINVAL;
	// }

	// return 1;
}

blkdev_ops_s XHCI_ops =
{
	.transfer = XHCI_transfer,
};

unsigned long XHCI_install(unsigned long irq,void * arg)
{
	return 0;
}

void XHCI_enable(unsigned long irq)
{
}

hw_int_controller_s XHCI_int_controller =
{
	.enable = XHCI_enable,
	.disable = NULL,
	.install = XHCI_install,
	.uninstall = NULL,
	.ack = IOAPIC_edge_ack,
};

void XHCI_handler(unsigned long parameter, pt_regs_s * regs)
{
	XHCI_HostCtrl_Ops_Regs_ptr->USBSTS |= 1 << 3;	// 该位是RW1C
	__mb();

	XHCI_test_getRegVals();
	blkbuf_node_s *node = req_in_using;
	node->end_handler(parameter);
	XHCI_end_request(node);

	u32 regval_32;
	u64 regval_64;

	Main_Intr_RegSet_ptr->IMAN |= 1 << 0;	// 该位是RW1C
	__mb();

	regval_64 = Main_Intr_RegSet_ptr->ERDP;
	regval_64 += sizeof(XHCI_TRB_s);
	regval_64 = phys_to_virt(regval_64);
	if (regval_64 >= (u64)(MainEventRing_Base + MainEventRing_Size))
		regval_64 = virt_to_phys((phys_addr_t)MainEventRing_Base);
	regval_64 |= 1 << 3;	// 该位是RW1C
	Main_Intr_RegSet_ptr->ERDP = regval_64;
	__mb();

	XHCI_test_getRegVals();

	color_printk(RED, BLACK, "XHCI Handler\n");
	// while (1);
}

void XHCI_init(struct PCI_Header_00 *XHCI_PCI_HBA)
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


	// get NVMe Controller register
	XHCI_BAR0_base = XHCI_PCI_HBA->BAR_base_addr[0];
	u64 map_start = XHCI_BAR0_base;
	u64 map_size = XHCI_PCI_HBA->BAR_space_limit[0];
	myos_ioremap(map_start, map_size);
	flush_tlb_local();


	// detect MSI-X capability
	bus = (XHCI_PCI_HBA->BDF >> 16) & 0xff;
	device = (XHCI_PCI_HBA->BDF >> 11) & 0x1f;
	function = (XHCI_PCI_HBA->BDF >> 8) & 0x7;
	index = XHCI_PCI_HBA->CapabilitiesPointer;
	while(index != 0)
	{
		value = Read_PCI_Config(bus, device, function, index);
		if((value & 0xff) == 0x11)
			break;

		index = (value >> 8) & 0xff;
	}
	// enable msi-x : MSI-X MXC.MXE=1,MXC.FM=0
	value = Read_PCI_Config(bus, device, function, index) & 0xbfffffff;
	value = value | 0x80000000;
	Write_PCI_Config(bus, device, function, index, value);
	//
	value = Read_PCI_Config(bus, device, function, index + 4);
	BIR_idx = value & 0x7;
	while (BIR_idx > 5);
	TBIR = XHCI_PCI_HBA->BAR_base_addr[BIR_idx];
	TADDR = (unsigned int *)phys_to_virt(TBIR + (value & (~0x7)));
	//
	value = Read_PCI_Config(bus, device, function, index + 8);
	BIR_idx = value & 0x7;
	while (BIR_idx > 5);
	PBIR = XHCI_PCI_HBA->BAR_base_addr[BIR_idx];
	PADDR = (unsigned int *)phys_to_virt(PBIR + (value & (~0x7)));

	/// Configuration MSI-X
	//MSI-X Table Entry 0	-> Admin Completion_Queue_Entry Interrupt Handler
	*TADDR = 0xfee00000;
	*(TADDR + 1) = 0;
	*(TADDR + 2) = APIC_PIRQC;
	*(TADDR + 3) = 0;
	__mb();
	// //MSI-X Table Entry End
	// *(TADDR + 4) = 0;
	// *(TADDR + 5) = 0;
	// *(TADDR + 6) = 0;
	// *(TADDR + 7) = 0;
	// __mb();

	// enable msi-x : MSI-X MXC.MXE=1,MXC.FM=0
	value = Read_PCI_Config(bus, device, function, index) & 0xb7ffffff;
	value = value | 0x80000000;
	Write_PCI_Config(bus, device, function, index, value);

	/// register interrupt
	//Admin Completion_Queue_Entry Interrupt Handler
	register_irq(APIC_PIRQC, NULL, "XHCI", 0, &XHCI_int_controller, &XHCI_handler);


	/// configure XHCI
	XHCI_HostCtrl_Cap_Regs_ptr = (XHCI_HCCR_s *)phys_to_virt(XHCI_BAR0_base);
	u8 CAPLENGTH = XHCI_HostCtrl_Cap_Regs_ptr->CAPLENGTH;
	XHCI_HostCtrl_Ops_Regs_ptr = (XHCI_HCOR_s *)phys_to_virt(XHCI_BAR0_base + CAPLENGTH);
	XHCI_DoorBell_Regptr = (u32 *)phys_to_virt(XHCI_BAR0_base + (XHCI_HostCtrl_Cap_Regs_ptr->DBOFF & ~0x3));
	XHCI_HostCtrl_RunTime_Regs_ptr = (XHCI_HCRTR_s *)phys_to_virt(XHCI_BAR0_base + (XHCI_HostCtrl_Cap_Regs_ptr->RTSOFF & ~0x1F));
	DCBAAP = (XHCI_DevCtx_s	**)phys_to_virt(XHCI_HostCtrl_Ops_Regs_ptr->DCBAAP);
	MaxSlotEnts = XHCI_HostCtrl_Ops_Regs_ptr->CONFIG.MaxSlotsEn;

	/// 重设主控命令环
	Host_CmdRing_Size = RING_SIZE;
	Host_CmdRing_Base = kzalloc(sizeof(XHCI_EvtTRB_s) *
		((Host_CmdRing_Size + RingSegMaxSize - 1) / RingSegMaxSize) * RingSegMaxSize, GFP_KERNEL);
	XHCI_HostCtrl_Ops_Regs_ptr->CRCR = virt_to_phys((virt_addr_t)Host_CmdRing_Base) | 0x1;
	XHCI_LinkTRB_s *LinkTRB_ptr = (XHCI_LinkTRB_s *)(Host_CmdRing_Base + Host_CmdRing_Size - 1);
	LinkTRB_ptr->CmdTRB_ptr = virt_to_phys((virt_addr_t)Host_CmdRing_Base);
	LinkTRB_ptr->Cycle_Bit = 1;
	LinkTRB_ptr->TRB_Type = LINK;
	Host_CmdRing_Curr = Host_CmdRing_Base;

	/// 重设主中断事件环
	Main_Intr_RegSet_ptr = &(XHCI_HostCtrl_RunTime_Regs_ptr->IRS_Arr[0]);
	// 创建环
	MainEventRing_Size = RING_SIZE;
	MainEventRing_Base = kzalloc(sizeof(XHCI_EvtTRB_s) *
		((MainEventRing_Size + RingSegMaxSize - 1) / RingSegMaxSize) * RingSegMaxSize, GFP_KERNEL);
	// 创建并设置段表
	XHCI_ERSegTblEnt_s *MainEventRing_SegTable_Entp = (XHCI_ERSegTblEnt_s *)kzalloc(sizeof(XHCI_ERSegTblEnt_s), GFP_KERNEL);
	MainEventRing_SegTable_Entp->RingSeg_Base = virt_to_phys((virt_addr_t)MainEventRing_Base);
	MainEventRing_SegTable_Entp->RingSeg_Size = MainEventRing_Size;
	// 设置主中断表项
	Main_Intr_RegSet_ptr->ERSTBA = (XHCI_ERSegTblEnt_s *)virt_to_phys((virt_addr_t)MainEventRing_SegTable_Entp);		//段表基址
	Main_Intr_RegSet_ptr->ERSTSZ = 1;		//段表大小
	Main_Intr_RegSet_ptr->ERDP = (virt_to_phys((virt_addr_t)MainEventRing_SegTable_Entp) | (1 << 3));		//事件环出队指针
	// 使能主中断
	Main_Intr_RegSet_ptr->IMAN |= 1 << 1;

	MainEventRing_Curr = MainEventRing_Base;


	// 测试
	XHCI_test_getRegVals();
	memset(&EventData_Test, 0, sizeof(EventData_Test));
}

void XHCI_exit()
{
	unregister_irq(APIC_PIRQC);
}

static int XHCIrq_deamon(void *param)
{
	current->flags |= PF_NOFREEZE;

	for (;;) {
		set_current_state(TASK_INTERRUPTIBLE);
		if (XHCIreq_lhdr.count == 0)
			schedule();
		__set_current_state(TASK_RUNNING);

		while (XHCIreq_lhdr.count != 0)
		{		
			spin_lock(&req_lock);
			if (req_in_using == NULL)
			{
				List_s *wq_lp = list_hdr_dequeue(&XHCIreq_lhdr);
				blkbuf_node_s *node = container_of(wq_lp, blkbuf_node_s, req_list);
				req_in_using = node;
				spin_unlock_no_resched(&req_lock);

				XHCI_cmd_out(node);

				spin_lock(&req_lock);
			}
			spin_unlock_no_resched(&req_lock);
			break;
		}
	}
	return 1;
}


void init_XHCIrqd()
{
	thread = kthread_run(XHCIrq_deamon, NULL, "XHCIrqd");

	// color_printk(WHITE, BLACK, "ATA disk: initialized\n");
}

void scan_XHCI_devices()
{
	for (int i = 1; i <= MaxSlotEnts; i++)
	{
		if (DCBAAP[i] == NULL)
			continue;
		
		XHCI_DevCtx_s *DCBA = (XHCI_DevCtx_s *)phys_to_virt((phys_addr_t)DCBAAP[i]);
		u8 EP_count = DCBA->Slot_Context.Ctx_Ents;
		while (EP_count < 2);
		
		XHCI_SlotCtx_s *Slot_Context = &DCBA->Slot_Context;
		XHCI_EPCtx_s *Bidir_EP_Context = &DCBA->BiDir;
		XHCI_TRB_s *TR_DequeuePtr = (XHCI_TRB_s *)phys_to_virt(Bidir_EP_Context->TR_Dequeue_Ptr & ~0xF);
		u16 TransferRing_Size = 0;
	}
}

void USB_Keyborad_init()
{
	for (int i = 0; i < 2; i++)
	{
		XHCI_ioctl(0, 0, NO_OP, 0);
		mdelay(100);
	}
	XHCI_ioctl(0, 0, EVENT_DATA, 0);
}