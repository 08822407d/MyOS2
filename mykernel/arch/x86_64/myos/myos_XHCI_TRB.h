#ifndef __XHCI_TRBDEF_H__
#define __XHCI_TRBDEF_H__

#include <linux/kernel/types.h>

	// Common TRB types
	enum {
		NORMAL					= 1,
		SETUP_STAGE,
		DATA_STAGE,
		STATUS_STAGE,
		ISOCH,
		LINK,
		EVENT_DATA,
		NO_OP,
		ENABLE_SLOT				= 9,
		DISABLE_SLOT,
		ADDRESS_DEVICE,
		CONFIG_EP,
		EVALUATE_CONTEXT,
		RESET_EP,
		STOP_EP					= 15,
		SET_TR_DEQUEUE,
		RESET_DEVICE,
		FORCE_EVENT,
		DEG_BANDWIDTH,
		SET_LAT_TOLERANCE,
		GET_PORT_BAND			= 21,
		FORCE_HEADER,
		NO_OP_CMD,
		// 24 - 31 = reserved
		TRANS_EVENT				=32,
		COMMAND_COMPLETION,
		PORT_STATUS_CHANGE,
		BANDWIDTH_REQUEST,
		DOORBELL_EVENT,
		HOST_CONTROLLER_EVENT	= 37,
		DEVICE_NOTIFICATION,
		MFINDEX_WRAP, 
		// 40 - 47 = reserved
		// 48 - 63 = Vendor Defined
	};

	typedef struct XHCI_TRB_Common {
		u32		Fields[3];

		u32		Cycle_Bit		: 1,
				Fields2			: 9,
				TRB_Type		: 6,
				Fields3			: 16;
	} __packed __aligned(16) XHCI_TRB_s;

	typedef struct XHCI_TransferTRB_Common
	{
		u32		Fields1[2];

		u32		Fields2			: 22,
				Intr_Target		: 10;

		u32		Cycle_Bit		: 1,
				Fields3			: 5,
				Immediate_Data	: 1,
				Fields4			: 3,
				TRB_Type		: 6,
				Fields5			: 16;
	} __packed __aligned(16) XHCI_TransTRB_s;
	
	typedef struct XHCI_EventTRB_Common {
		u32		Fields[2];

		u32		Fields2			: 24,
				Comp_Code		: 8;

		u32		Cycle_Bit		: 1,
				Fields3			: 9,
				TRB_Type		: 6,
				Fields4			: 16;
	} __packed __aligned(16) XHCI_EvtTRB_s;
	
	typedef struct XHCI_CommandTRB_Common {
		u32		Fields1[3];

		u32		Cycle_Bit		: 1,
				Fields2			: 9,
				TRB_Type		: 6,
				Fields3			: 8,
				Slot_ID			: 8;
	} __packed __aligned(16) XHCI_CmdTRB_s;

	typedef struct XHCI_OtherTRB_Common
	{
		u64		Fields1;

		u32		RsvdZ1			: 22,
				Intr_Target		: 10;

		u32		Cycle_Bit		: 1,
				Fields2			: 1,
				RsvdZ2			: 2,
				Chain_Bit		: 1,
				IntrOnComp		: 1,
				Fields3			: 4,
				TRB_Type		: 6,
				RsvdZ3			: 16;
	} __packed __aligned(16) XHCI_OtherTRB_s;


/*==============================================================================================*
 *											transfer class										*
 *==============================================================================================*/
	typedef struct XHCI_Setup_Stage_TRB
	{
		u8		bmRequestType;
		u8		bRequest;
		u16		wValue;
		u16		wIndex;
		u16		wLength;

		u32		TRB_TransLeng	: 17,		
				RsvdZ1			: 5,
				Intr_Target		: 10;

		u32		Cycle_Bit		: 1,
				RsvdZ2			: 4,
				IntrOnComp		: 1,
				Immediate_Data	: 1,
				RsvdZ3			: 3,
				TRB_Type		: 6,
				Trans_Type		: 2,
				RsvdZ4			: 14;
	} __packed __aligned(16) XHCI_SetupStage_s;

	typedef struct XHCI_Data_Stage_TRB
	{
		u64		Data_Buffer;

		u32		TRB_TransLeng	: 17,		
				TD_Size			: 5,
				Intr_Target		: 10;

		u32		Cycle_Bit		: 1,
				Eval_Next_TRB	: 1,
				ISP				: 1,
				No_Snoop		: 1,
				Chain_Bit		: 1,
				IntrOnComp		: 1,
				Immediate_Data	: 1,
				RsvdZ1			: 3,
				TRB_Type		: 6,
				Direction		: 1,
				RsvdZ2			: 15;
	} __packed __aligned(16) XHCI_DataStage_s;

	typedef struct XHCI_Statue_Stage_TRB
	{
		u64		RsvdZ1;

		u32		RsvdZ2			: 22,		
				Intr_Target		: 10;

		u32		Cycle_Bit		: 1,
				Eval_Next_TRB	: 1,
				RsvdZ3			: 2,
				Chain_Bit		: 1,
				IntrOnComp		: 1,
				RsvdZ4			: 4,
				TRB_Type		: 6,
				Direction		: 1,
				RsvdZ5			: 15;
	} __packed __aligned(16) XHCI_StatusStage_s;


/*==============================================================================================*
 *											event class											*
 *==============================================================================================*/
	typedef struct XHCI_Transfer_Event_TRB
	{
		u64		TRB_ptr;

		u32		TRB_TransLeng	: 24,		
				Compl_Code		: 8;

		u32		Cycle_Bit		: 1,
				RsvdZ1			: 1,
				Event_Data		: 1,
				RsvdZ2			: 7,
				TRB_Type		: 6,
				EndPt_ID		: 5,
				RsvdZ3			: 3,
				Slot_ID			: 8;
	} __packed __aligned(16) XHCI_TransEvt_s;

	typedef struct XHCI_Command_Completion_Event_TRB {
		u64		CmdTRB_ptr;

		u32		CmdComp_Param	: 24,
				Compl_Code		: 8;

		u32     Cycle_Bit		: 1,
				RsvdZ			: 9,
				TRB_Type		: 6,
				VF_ID			: 8,
				Slot_ID			: 8;
	} __packed __aligned(16) XHCI_CmdComplEvt_TRB_s;

	typedef struct XHCI_Status_Change_Event_TRB {
		u32		RsvdZ1			: 24,
				Port_ID			: 8;

		u32		RsvdZ2;

		u32		RsvdZ3			: 24,
				Compl_Code		: 8;

		u32     Cycle_Bit		: 1,
				RsvdZ4			: 9,
				TRB_Type		: 6,
				RsvdZ5			: 16;
	} __packed __aligned(16) XHCI_StChEvt_TRB_s;


/*==============================================================================================*
 *											command class										*
 *==============================================================================================*/
	typedef struct XHCI_No_Op_Command_TRB {
		u32		RsvdZ1[3];

		u32		Cycle_Bit		: 1,
				RsvdZ			: 9,
				TRB_Type		: 6,
				VF_ID			: 8,
				RsvdZ2			: 8;
	} __packed __aligned(16) XHCI_NoOpCmd_TRB_s;


/*==============================================================================================*
 *											other class											*
 *==============================================================================================*/
	typedef struct XHCI_Link_TRB {
		u64		CmdTRB_ptr;
		
		u32		RsvdZ1			: 22,
				Intr_Target		: 10;

		u32		Cycle_Bit		: 1,
				Toggle_Cycle	: 1,
				RsvdZ2			: 2,
				Chain_Bit		: 1,
				IntrOnComp		: 1,
				RsvdZ3			: 4,
				TRB_Type		: 6,
				RsvdZ4			: 16;
	} __packed __aligned(16) XHCI_LinkTRB_s;

	typedef struct XHCI_EventData_TRB {
		u64		CmdTRB_ptr;
		
		u32		RsvdZ1			: 22,
				Intr_Target		: 10;

		u32		Cycle_Bit		: 1,
				Eval_Next_TRB	: 1,
				RsvdZ2			: 2,
				Chain_Bit		: 1,
				IntrOnComp		: 1,
				RsvdZ3			: 3,
				Blk_Evt_Intr	: 1,
				TRB_Type		: 6,
				RsvdZ4			: 16;
	} __packed __aligned(16) XHCI_EvtDataTRB_s;

#endif