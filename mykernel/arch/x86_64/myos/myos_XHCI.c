#include <linux/kernel/stddef.h>
#include <linux/kernel/mm.h>
#include <linux/kernel/delay.h>
#include <linux/kernel/sched_api.h>

#include "myos_XHCI.h"
#include "myos_USB.h"
#include "MineOS_PCI.h"
#include <asm/insns.h>
#include <asm/apic.h>

#include <obsolete/myos_irq_vectors.h>
#include <obsolete/printk.h>
#include <obsolete/arch_proto.h>
#include <obsolete/device.h>
#include "myos_block.h"


typedef struct XHCI_Spec_Params
{
	u8		Slot_ID;
	u8		DevCtx_Idx;
	EP_Param_s	*EP_Param;
	XHCI_EvtTRB_s	EvtTRB;
} XHCI_Params_s;

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
XHCI_xECP_s		*xECP_Base = NULL;
u32				*XHCI_DoorBell_Regptr = NULL;
XHCI_DevCtx_s	**DCBAAP = NULL;
u8				MaxSlotEnts = 0;
	
// 主中断
XHCI_IRS_s		*Main_Intr_RegSet_ptr = NULL;
XHCI_EvtTRB_s	*MainEventRing_Base = NULL;
u16				MainEventRing_Size = 0;

// 传输环
XHCI_TRB_s		*TR_DeqPtr_Curr = NULL;


// 全局参数缓存
Slot_Param_s *Slot_Params[256];

typedef struct XHCI_Values_for_Test
{
    XHCI_HCCR_s		HostCtrl_Cap_Regs;
    XHCI_HCOR_s		HostCtrl_Ops_Regs;
    XHCI_HCRTR_s	HostCtrl_RunTime_Regs;
} XHCI_ValTest_s;

XHCI_ValTest_s Initial_Values;
XHCI_ValTest_s Reset_Values;

// 测试用
u32			EventData_Test = 0;
u32			Intr_Count = 0;

void XHCI_test_getRegVals(XHCI_ValTest_s *Val)
{
	Val->HostCtrl_Cap_Regs = *XHCI_HostCtrl_Cap_Regs_ptr;
	Val->HostCtrl_Ops_Regs = *XHCI_HostCtrl_Ops_Regs_ptr;
	Val->HostCtrl_RunTime_Regs = *XHCI_HostCtrl_RunTime_Regs_ptr;
}


bool XHCI_RingIsFull(EP_Param_s *EP_Param)
{
	return (EP_Param->Cycle_Toggled == 1) &&
		((EP_Param->Ring_EnqueuePtr + 1) == EP_Param->Ring_DequeuePtr);
}

void XHCI_AdvanceRingEnqPtr(EP_Param_s *EP_Param)
{
	EP_Param->Ring_EnqueuePtr++;
	XHCI_TRB_s *Ring_ptr = EP_Param->Ring_EnqueuePtr;
	if (Ring_ptr->TRB_Type == LINK)
	{
		XHCI_LinkTRB_s *LinkTRB_ptr = (XHCI_LinkTRB_s *)Ring_ptr;
		EP_Param->Ring_EnqueuePtr = (XHCI_TRB_s *)phys_to_virt(LinkTRB_ptr->CmdTRB_ptr);
		EP_Param->Cycle_Bit = 0x1 & !EP_Param->Cycle_Bit;
		EP_Param->Cycle_Toggled = 1;
	}
}

void XHCI_AdvanceRingDeqPtr(EP_Param_s *EP_Param)
{
	EP_Param->Ring_DequeuePtr++;
	XHCI_TRB_s *Ring_ptr = EP_Param->Ring_DequeuePtr;
	if (Ring_ptr->TRB_Type == LINK)
	{
		XHCI_LinkTRB_s *LinkTRB_ptr = (XHCI_LinkTRB_s *)Ring_ptr;
		EP_Param->Ring_EnqueuePtr = (XHCI_TRB_s *)phys_to_virt(LinkTRB_ptr->CmdTRB_ptr);
	}
}

void XHCI_ToggleCycleBit(EP_Param_s *EP_Param)
{
	if (EP_Param->Cycle_Toggled == 1)
	{
		EP_Param->Ring_LinkTRB->Cycle_Bit =
			0x1 & !EP_Param->Ring_LinkTRB->Cycle_Bit;
		EP_Param->Cycle_Toggled = 0;
	}
}

// void XHCI_MakeTRB_EventData(XHCI_TRB_s **Ring_ptr)
// {
// 	memset((*Ring_ptr), 0, sizeof(XHCI_CmdTRB_s));
// 	(*Ring_ptr)->Cycle_Bit = 1;	

// 	XHCI_EvtDataTRB_s *EvtData_TRB = (XHCI_EvtDataTRB_s *)(*Ring_ptr);
// 	EvtData_TRB->Chain_Bit = 0;
// 	EvtData_TRB->IntrOnComp = 1;
// 	EvtData_TRB->CmdTRB_ptr = (u64)&EventData_Test;

// 	XHCI_AdvanceRingEnqPtr(Ring_ptr);
// }

// void XHCI_MakeTRB_SetupStage(XHCI_TRB_s **Ring_ptr, USB_ReqPack_s *request, u8 transtype)
// {
// 	XHCI_SetupStage_s *SetupStageTRB = (XHCI_SetupStage_s *)(*Ring_ptr);
// 	memset(SetupStageTRB, 0, sizeof(XHCI_SetupStage_s));

// 	SetupStageTRB->bmRequestType = request->request_type;
// 	SetupStageTRB->bRequest = request->request;
// 	SetupStageTRB->wValue = request->value;
// 	SetupStageTRB->wIndex = request->index;
// 	SetupStageTRB->wLength = request->length;

// 	SetupStageTRB->TRB_TransLeng = 8;
// 	SetupStageTRB->Trans_Type = transtype;

// 	SetupStageTRB->Cycle_Bit = 1;
// 	SetupStageTRB->Immediate_Data = 1;
// 	SetupStageTRB->TRB_Type = SETUP_STAGE;

// 	XHCI_AdvanceRingEnqPtr(Ring_ptr);
// }

// void XHCI_MakeTRB_DataStage(XHCI_TRB_s **Ring_ptr, u8 *buffer, u32 bufsize, u8 DIR, u16 MaxPackSize)
// {
// 	u8 TRB_type = DATA_STAGE;
// 	u8 *bufptr = buffer;
// 	u32 size = bufsize;
// 	int remaining = (int) (((size + (MaxPackSize - 1)) / MaxPackSize) - 1);
// 	if (remaining < 0)
// 		remaining = 0;

// 	while (size > 0)
// 	{
// 		XHCI_DataStage_s *DataStageTRB = (XHCI_DataStage_s *)(Ring_ptr);
// 		memset(DataStageTRB, 0, sizeof(XHCI_DataStage_s));

// 		DataStageTRB->Data_Buffer = (u64)bufptr;
// 		DataStageTRB->TD_Size = remaining;
// 		DataStageTRB->TRB_TransLeng = ((size < MaxPackSize) ? size : MaxPackSize);

// 		DataStageTRB->Direction = DIR;
// 		DataStageTRB->TRB_Type = TRB_type;
// 		DataStageTRB->Chain_Bit = 1;
// 		DataStageTRB->Eval_Next_TRB = (remaining == 0);
// 		DataStageTRB->Cycle_Bit = 1;

// 		XHCI_AdvanceRingEnqPtr(Ring_ptr);

// 		bufptr += MaxPackSize;
// 		if (size < MaxPackSize)
// 			size = 0;
// 		else
// 			size -= MaxPackSize;
// 		TRB_type = NORMAL;
// 		DIR = 0;
// 	}
// }

// void XHCI_MakeTRB_StatusStage(XHCI_TRB_s **Ring_ptr, u32 *status_addr, u8 DIR)
// {
// 	XHCI_StatusStage_s *StatusStageTRB = (XHCI_StatusStage_s *)(*Ring_ptr);
// 	memset(StatusStageTRB, 0, sizeof(XHCI_StatusStage_s));

// 	StatusStageTRB->TRB_Type = STATUS_STAGE;
// 	StatusStageTRB->Direction = DIR;
// 	StatusStageTRB->IntrOnComp = 1;
// 	StatusStageTRB->Cycle_Bit = 1;

// 	XHCI_AdvanceRingEnqPtr(Ring_ptr);
// }


// void XHCI_MakeTD_CtrlEP(XHCI_TRB_s **Ring_ptr, u8 *buffer, u32 bufsize, u8 DIR)
// {
// 	USB_ReqPack_s ReqPack = { .value = ((DEVICE << 8) | 0), };
// 	if (DIR == USB_TRANSFER_IN)
// 	{
// 		ReqPack.request_type = STDRD_GET_REQUEST;
// 		ReqPack.request = GET_DESCRIPTOR;
// 	}
// 	else
// 	{
// 		ReqPack.request_type = STDRD_SET_REQUEST;
// 		ReqPack.request = SET_DESCRIPTOR;
// 	}

// 	u8 transtype = ((bufsize > 0) << 1) | DIR;
// 	XHCI_MakeTRB_SetupStage(Ring_ptr, &ReqPack, transtype);
// 	XHCI_MakeTRB_DataStage(Ring_ptr, buffer, bufsize, DIR, 8);
// 	XHCI_MakeTRB_StatusStage(Ring_ptr, NULL, DIR ^ 1);
// 	XHCI_MakeTRB_EventData(Ring_ptr);
// }

void XHCI_MakeTRB_NoOp(EP_Param_s *EP_Param)
{
	XHCI_NoOpCmd_TRB_s *Ring_ptr = (XHCI_NoOpCmd_TRB_s *)EP_Param->Ring_EnqueuePtr;
	memset(Ring_ptr, 0, sizeof(XHCI_TRB_s));

	Ring_ptr->TRB_Type = NO_OP;
	Ring_ptr->Cycle_Bit = EP_Param->Cycle_Bit;

	XHCI_AdvanceRingEnqPtr(EP_Param);
}


long XHCI_cmd_out(blkbuf_node_s *node)
{
	XHCI_Params_s *XHCIparam = (XHCI_Params_s *)node->DevSpecParams;
	u8 Slot_ID = XHCIparam->Slot_ID;
	u8 DevCtx_Idx = XHCIparam->DevCtx_Idx;
	while (Slot_ID == 0 && DevCtx_Idx != 0);
	EP_Param_s *EP_Param = (EP_Param_s *)XHCIparam->EP_Param;

	switch(node->cmd)
	{
		// case XHCI_OP_CtrlRead:
		// 	XHCI_MakeTD_CtrlEP(Ring_CurrPtr, node->buffer, node->count, USB_TRANSFER_IN);
		// 	break;

		// case XHCI_OP_CtrlWrite:
		// 	XHCI_MakeTD_CtrlEP(Ring_CurrPtr, node->buffer, node->count, USB_TRANSFER_OUT);
		// 	break;

		case XHCI_OP_IsochRead:
			break;

		case XHCI_OP_IsochWrite:
			break;

		case XHCI_OP_BulkRead:
			break;

		case XHCI_OP_BulkWrite:
			break;

		case XHCI_OP_IntrRead:
			break;

		case XHCI_OP_IntrWrite:
			break;

		case XHCI_OP_NOP:
			XHCI_MakeTRB_NoOp(EP_Param);
			break;

		default:
			break;
	}

	*(XHCI_DoorBell_Regptr + Slot_ID) = (u32)DevCtx_Idx;
	__mb();

	return 1;
}

void XHCI_end_request(blkbuf_node_s *node)
{
	if (node == NULL)
		color_printk(RED, BLACK, "end_request error\n");

	// 设置xhci主控中断位以允许主控发出下一次中断
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
	Main_Intr_RegSet_ptr->ERDP = virt_to_phys(regval_64);
	__mb();


	complete(node->done);
	spin_lock(&req_lock);
	req_in_using = NULL;
	spin_unlock(&req_lock);
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

blkbuf_node_s *XHCI_make_request(unsigned Slot_ID, unsigned DevCtx_Idx,
		long cmd, unsigned long blk_idx, long count, unsigned char *buffer)
{
	blkbuf_node_s *node = kzalloc(sizeof(blkbuf_node_s), GFP_KERNEL);
	XHCI_Params_s *XHCIparam = (XHCI_Params_s *)kzalloc(sizeof(XHCI_Params_s), GFP_KERNEL);
	INIT_LIST_S(&node->req_list);

	node->LBA = blk_idx;
	node->count = count;
	node->buffer = buffer;
	node->DevSpecParams = XHCIparam;
	XHCIparam->Slot_ID = Slot_ID;
	XHCIparam->DevCtx_Idx = DevCtx_Idx;
	XHCIparam->EP_Param = Slot_Params[Slot_ID]->SlotEP_Params[DevCtx_Idx];

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

	return node;
}


long XHCI_transfer(unsigned Slot_ID, unsigned DevCtx_Idx,
		long cmd, unsigned long blk_idx, long count, unsigned char *buffer)
{
	blkbuf_node_s *node = XHCI_make_request(Slot_ID, DevCtx_Idx, cmd, blk_idx, count, buffer);
	DECLARE_COMPLETION_ONSTACK(done);
	node->done = &done;
	node->task = current;

	spin_lock(&req_lock);
	list_header_enqueue(&XHCIreq_lhdr, &node->req_list);
	spin_unlock(&req_lock);

	wake_up_process(thread);
	wait_for_completion(&done);

	XHCI_Params_s *XHCIparam = (XHCI_Params_s *)node->DevSpecParams;
	if (node->DevSpecParams != NULL)
		kfree(node->DevSpecParams);
	if (node != NULL)
		kfree(node);
	return -ENOERR;

	return 1;
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

	XHCI_test_getRegVals(&Reset_Values);
	blkbuf_node_s *node = req_in_using;
	XHCI_Params_s *XHCIparam = (XHCI_Params_s *)node->DevSpecParams;
	EP_Param_s *EP_Param = (EP_Param_s *)XHCIparam->EP_Param;
	// 检查并返回事件TRB
	XHCI_EvtTRB_s *EvtTRB = (XHCI_EvtTRB_s *)phys_to_virt(Main_Intr_RegSet_ptr->ERDP & ~0xF);
	XHCI_TRB_s *CmdTRB_ptr = (XHCI_TRB_s *)phys_to_virt(*(phys_addr_t *)EvtTRB->Fields);
	while (CmdTRB_ptr != EP_Param->Ring_DequeuePtr);
	XHCIparam->EvtTRB = *EvtTRB;

	node->end_handler(parameter);
	XHCI_end_request(node);

	XHCI_AdvanceRingDeqPtr(EP_Param);
	// 处理命令环/传输环翻转
	XHCI_ToggleCycleBit(EP_Param);

	XHCI_test_getRegVals(&Reset_Values);

	color_printk(RED, BLACK, "XHCI Handler: %d\n", Intr_Count);
	Intr_Count++;
	// while (1);
}


void XHCI_ResetHostController()
{
	// /// 遍历XHCI扩展功能
	// xECP_Base = (XHCI_xECP_s *)phys_to_virt(XHCI_BAR0_base + (XHCI_HostCtrl_Cap_Regs_ptr->HCCPARAMS1.xECP << 2));
	// XHCI_xECP_s *xECP_next = xECP_Base;
	// while (1)
	// {
	// 	XHCI_xECP_s xECP_val = *xECP_next;
	// 	switch (xECP_next->Cap_ID)
	// 	{
	// 	case 2:
	// 		XHCI_xECP_SupProt_s SupProt = *(XHCI_xECP_SupProt_s *)xECP_next;
	// 		break;
		
	// 	default:
	// 		break;
	// 	}
	// 	if (xECP_next->Next_xECP == 0)
	// 		break;
	// 	xECP_next = (XHCI_xECP_s *)((u64)xECP_next + (xECP_next->Next_xECP << 2));
	// }
	
	/// 停止主控
	XHCI_HostCtrl_Ops_Regs_ptr->USBCMD &= ~(1 << 0);
	while ((XHCI_HostCtrl_Ops_Regs_ptr->USBSTS & (1 << 0)) == 0);
	XHCI_HostCtrl_Ops_Regs_ptr->USBCMD |= 1 << 1;
	while ((XHCI_HostCtrl_Ops_Regs_ptr->USBCMD & (1 << 1)) != 0);
	memset(Slot_Params, 0, sizeof(Slot_Params));
	
	/// 停止命令环
	XHCI_HostCtrl_Ops_Regs_ptr->CRCR &= 1 << 1;
	while ((XHCI_HostCtrl_Ops_Regs_ptr->CRCR & (1 << 3)) == 1);
	XHCI_test_getRegVals(&Reset_Values);
	/// 重设主控命令环
	u16 Host_CmdRing_Size = RING_SIZE;
	XHCI_CmdTRB_s *Host_CmdRing_Base = kzalloc(sizeof(XHCI_EvtTRB_s) *
		((Host_CmdRing_Size + RingSegMaxSize - 1) / RingSegMaxSize) * RingSegMaxSize, GFP_KERNEL);
	XHCI_HostCtrl_Ops_Regs_ptr->CRCR = virt_to_phys((virt_addr_t)Host_CmdRing_Base) | 0x1;
	// 主控环尾
	XHCI_LinkTRB_s *LinkTRB_ptr = (XHCI_LinkTRB_s *)(Host_CmdRing_Base + RING_SIZE - 1);
	LinkTRB_ptr->CmdTRB_ptr = virt_to_phys((virt_addr_t)Host_CmdRing_Base);
	LinkTRB_ptr->Cycle_Bit = 1;
	LinkTRB_ptr->Toggle_Cycle = 1;
	LinkTRB_ptr->TRB_Type = LINK;
	// 端点参数列表中的主控环
	Slot_Params[0] = (Slot_Param_s *)kzalloc(sizeof(Slot_Param_s), GFP_KERNEL);
	Slot_Params[0]->SlotEP_Params[0] = (EP_Param_s *)kzalloc(sizeof(EP_Param_s), GFP_KERNEL);
	EP_Param_s *HostParam = Slot_Params[0]->SlotEP_Params[0];
	HostParam->Cycle_Bit = LinkTRB_ptr->Cycle_Bit;
	HostParam->Cycle_Toggled = 0;
	HostParam->Ring_EnqueuePtr =
	HostParam->Ring_DequeuePtr = (XHCI_TRB_s *)Host_CmdRing_Base;
	HostParam->Ring_LinkTRB = LinkTRB_ptr;

	XHCI_test_getRegVals(&Reset_Values);
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
	Main_Intr_RegSet_ptr->ERDP = MainEventRing_SegTable_Entp->RingSeg_Base | (1 << 3);		//事件环出队指针


	XHCI_test_getRegVals(&Reset_Values);
	// 重启主控
	XHCI_HostCtrl_Ops_Regs_ptr->USBCMD |= 1 << 0;
	// 测试
	XHCI_test_getRegVals(&Reset_Values);
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
	/// 保存寄存器组初始值
	XHCI_test_getRegVals(&Initial_Values);

	XHCI_ResetHostController();

	// 使能主中断
	Main_Intr_RegSet_ptr->IMAN |= 1 << 0;
	Main_Intr_RegSet_ptr->IMAN |= 1 << 1;

	DCBAAP = (XHCI_DevCtx_s	**)phys_to_virt(XHCI_HostCtrl_Ops_Regs_ptr->DCBAAP);
	MaxSlotEnts = XHCI_HostCtrl_Ops_Regs_ptr->CONFIG.MaxSlotsEn;
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
				List_s *wq_lp = list_header_dequeue(&XHCIreq_lhdr);
				blkbuf_node_s *node = container_of(wq_lp, blkbuf_node_s, req_list);
				req_in_using = node;
				spin_unlock(&req_lock);

				XHCI_cmd_out(node);

				spin_lock(&req_lock);
			}
			spin_unlock(&req_lock);
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
	// for (int Slot_Idx = 1; Slot_Idx <= MaxSlotEnts; Slot_Idx++)
	// {
	// 	if (DCBAAP[Slot_Idx] == NULL)
	// 		continue;
		
	// 	XHCI_DevCtx_s *DCBA = (XHCI_DevCtx_s *)phys_to_virt((phys_addr_t)DCBAAP[Slot_Idx]);
	// 	u8 EP_count = DCBA->Slot_Context.Ctx_Ents;
	// 	while (EP_count < 2);

	// 	XHCI_SlotCtx_s *Slot_Ctx = &DCBA->Slot_Context;
	// 	Slot_Param_s *SlotParam = (Slot_Param_s *)kzalloc(sizeof(Slot_Param_s), GFP_KERNEL);
	// 	Slot_Params[Slot_Idx] = SlotParam;
	// 	for (int EP_Idx = 1; EP_Idx < 32; EP_Idx++)
	// 	{
	// 		XHCI_EPCtx_s *EP_Ctx = &DCBA->EP_Contexts[EP_Idx];
	// 		if (EP_Ctx->EP_State > 0 && EP_Ctx->EP_State < 5)
	// 		{
	// 			EP_Param_s *EP = (EP_Param_s *)kzalloc(sizeof(EP_Param_s), GFP_KERNEL);
	// 			SlotParam->SlotEP_Params[1] = EP;

	// 			EP->Cycle_Bit = 1;
	// 			EP->Ring_EnqueuePtr =
	// 			EP->Ring_DequeuePtr = (XHCI_TRB_s *)phys_to_virt(EP_Ctx->TR_Dequeue_Ptr & ~0xF);
	// 		}
	// 	}
	// }
}

void USB_Keyborad_init()
{
	for (int i = 0; i < 2; i++)
	{
		XHCI_transfer(0, 0, XHCI_OP_NOP, 0, 0, NULL);
		mdelay(100);
	}
}