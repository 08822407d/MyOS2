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
		u32		Fields[4];
	} XHCI_TRB_s;

	typedef struct XHCI_EventTRB_Common {
		u32		Fields1[3];

		u32		Cycle_Bit		: 1,
				Fields2			: 9,
				TRB_Type		: 6,
				Fields3			: 16;
	} __attribute__((packed)) XHCI_EvtTRB_s;
	
	typedef struct XHCI_CommandTRB_Common {
		u32		Fields1[3];

		u32		Cycle_Bit		: 1,
				Fields2			: 9,
				TRB_Type		: 6,
				Fields3			: 8,
				Slot_ID			: 8;
	} __attribute__((packed)) XHCI_CmdTRB_s;

	typedef struct XHCI_Command_Completion_Event_TRB {
		u64		CmdTRB_ptr;

		u32		CmdComp_Param	: 24,
				CmdCode			: 8;

		u32     Cycle_Bit		: 1,
				RsvdZ			: 9,
				TRB_Type		: 6,
				VF_ID			: 8,
				Slot_ID			: 8;
	} __attribute__((packed)) XHCI_CCE_TRB_s;

	typedef struct XHCI_No_Op_Command_TRB {
		u32		RsvdZ1[3];

		u32		Cycle_Bit		: 1,
				RsvdZ			: 9,
				TRB_Type		: 6,
				VF_ID			: 8,
				RsvdZ2			: 8;
	} __attribute__((packed)) XHCI_NoOpCmd_TRB_s;

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
	} __attribute__((packed)) XHCI_LinkTRB_s;

#endif