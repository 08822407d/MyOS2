#ifndef __XHCI_H__
#define __XHCI_H__

#include <linux/kernel/types.h>

#include "myos_XHCI_TRB.h"


	enum {
		XHCI_OP_CtrlRead,
		XHCI_OP_CtrlWrite,
		XHCI_OP_IsochRead,
		XHCI_OP_IsochWrite,
		XHCI_OP_BulkRead,
		XHCI_OP_BulkWrite,
		XHCI_OP_IntrRead,
		XHCI_OP_IntrWrite,
		XHCI_OP_NOP,
	};

	typedef struct XHCI_HostCtrl_Cap_Regs {
		u8		CAPLENGTH;
		u8		Rsvd1;
		u16		HCIVERSION;

		struct __attribute__((packed))
		{
			u32
				MaxSlots	: 8,
				MaxIntrs	: 11,
				Rsvd		: 5,
				MaxPorts	: 8;
		} HCSPARAMS1;

		struct __attribute__((packed))
		{
			u32
				IST			: 4,
				ERST_Max	: 4,
				Rsvd		: 13,
				MaxSB_Hi	: 5,
				SPR			: 1,
				MaxSB_Lo	: 5;
		} HCSPARAMS2;

		struct __attribute__((packed))
		{
			u32
				U1_DEL		: 8,
				MaxIntrs	: 8,
				U2_DEL		: 16;
		} HCSPARAMS3;

		struct __attribute__((packed))
		{
			u32
				AC_64		: 1,
				BNC			: 1,
				CSZ			: 1,
				PPC			: 1,
				PIND		: 1,
				LHRC		: 1,
				LTC			: 1,
				NSS			: 1,
				PAE			: 1,
				SPC			: 1,
				SEC			: 1,
				CFC			: 1,
				MaxPSASize	: 4,
				xECP		: 16;
		} HCCPARAMS1;

		u32		DBOFF;
		u32		RTSOFF;
		
		struct __attribute__((packed))
		{
			u32
				U3C			: 1,
				CMC			: 1,
				FSC			: 1,
				CTC			: 1,
				LEC			: 1,
				CIC			: 1,
				ETC			: 1,
				ETC_TSC		: 1,
				GSC			: 1,
				VTC			: 1,
				xECP		: 22;
		} HCCPARAMS2;

		u8		*Rsvd2;
	} __attribute__((packed)) XHCI_HCCR_s;


	typedef struct USB_Port_Reg_Set {
		struct __attribute__((packed))
		{
			u32
				CCS			: 1,
				PED			: 1,
				TM			: 1,
				OCA			: 1,
				PR			: 1,
				PLS			: 4,
				PP			: 1,
				Port_Speed	: 4,
				PIC			: 2,
				LWS			: 1,
				CSC			: 1,
				PEC			: 1,
				WRC			: 1,
				OCC			: 1,
				PRC			: 1,
				PLC			: 1,
				CEC			: 1,
				CAS			: 1,
				WCE			: 1,
				WDE			: 1,
				WOE			: 1,
				RsvdZ		: 2,
				DR			: 1,
				WPR			: 1;
		} PORTSC;

		struct __attribute__((packed))
		{
			u32
				U1_Timeout	: 8,
				U2_Timeout	: 8,
				FLA			: 1,
				RsvdP3		: 15;
		} PORTPMSC;

		struct __attribute__((packed))
		{
			u32
				LEC			: 16,
				RLC			: 4,
				TLC			: 4,
				RsvdP		: 8;
		} PORTLI;

		u32		PORTHLPMC;
	} USB_PRS_s;

	typedef struct XHCI_HostCtrl_Ops_Regs {
		// struct __attribute__((packed))
		// {
		// 	u32
		// 		RunStop		: 1,
		// 		HCRST		: 1,
		// 		INTE		: 1,
		// 		HSEE		: 1,
		// 		RsvdP1		: 3,
		// 		LHCRST		: 1,
		// 		CSS			: 1,
		// 		CRS			: 1,
		// 		EWE			: 1,
		// 		EU3S		: 1,
		// 		RsvdP2		: 1,
		// 		CME			: 1,
		// 		ETE			: 1,
		// 		TSC_EN		: 1,
		// 		VTIOE		: 1,
		// 		RsvdP3		: 15;
		// } USBCMD;
		u32		USBCMD;

		// struct __attribute__((packed))
		// {
		// 	u32
		// 		HCH			: 1,
		// 		RsvdZ1		: 1,
		// 		HSE			: 1,
		// 		EINT		: 1,
		// 		PCD			: 1,
		// 		RsvdZ2		: 3,
		// 		SSS			: 1,
		// 		RSS			: 1,
		// 		SRE			: 1,
		// 		CNR			: 1,
		// 		HCE			: 1,
		// 		RsvdZ3		: 19;
		// } USBSTS;
		u32		USBSTS;

		u32		PAGESIZE;
		u32		RsvdZ1[2];
		u32		DNCTRL;

		u64		CRCR;

		u32		RsvdZ2[4];
		u64		DCBAAP;

		struct __attribute__((packed))
		{
			u32
				MaxSlotsEn	: 8,
				U3E			: 1,
				CIE			: 1,
				RsvdP		: 22;
		} CONFIG;

		u32		RsvdZ3[241];

		USB_PRS_s	USB_Port_Reg_Sets[256];
	} XHCI_HCOR_s;


	typedef struct XHCI_Event_Ring_SegTable_Ent {
		u64		RingSeg_Base;
		u16		RingSeg_Size;
		u16		RsvdZ[3];
	} XHCI_ERSegTblEnt_s;

	typedef struct Intr_Reg_Set {
		// struct __attribute__((packed))
		// {
		// 	u32
		// 		IP			: 1,
		// 		IE			: 1,
		// 		RsvdP		: 30;
		// } IMAN;
		u32		IMAN;

		struct __attribute__((packed))
		{
			u32
				IMODI		: 16,
				IMODC		: 16;
		} IMOD;

		u64		ERSTSZ;
		XHCI_ERSegTblEnt_s	*ERSTBA;
		u64		ERDP;
	} XHCI_IRS_s;

	typedef struct XHCI_HostCtrl_RunTime_Regs {
		u32		MFINDEX;
		u8		RsvdZ[28];
		XHCI_IRS_s		IRS_Arr[1024];
	} XHCI_HCRTR_s;


	typedef struct XHCI_Slot_Context {
		u32		Route_Str		: 20,
				Speed			: 4,
				RsvdZ1			: 1,
				Multi_TT		: 1,
				Hub				: 1,
				Ctx_Ents		: 5;

		u32		Max_ExitLatency	: 15,
				RHP_Num			: 8,
				Num_Ports		: 8;

		u32		PHS_ID			: 8,
				PP_Num			: 8,
				TTT				: 2,
				RsvdZ2			: 4,
				Intr_Target		: 10;

		u32		USB_Dev_Addr	: 8,
				RsvdZ3			: 19,
				Slot_State		: 5;

		u32		RsvdO[4];
	} __attribute__((packed)) XHCI_SlotCtx_s;

	typedef struct XHCI_Endpoint_Context {
		u32		EP_State		: 3,
				RsvdZ1			: 5,
				Mult			: 2,
				MaxPStreams		: 5,
				LSA				: 1,
				Interval		: 8,
				Max_ESIT_PldHi	: 8;

		u32		RsvdZ2			: 1,
				CErr			: 2,
				EP_Type			: 3,
				RsvdZ3			: 1,
				HID				: 1,
				MaxBurstSize	: 8,
				MaxPacketSize	: 16;
		
		u64		TR_Dequeue_Ptr;

		u32		AvgTRBLen		: 16,
				Max_ESIT_PldLo	: 16;

		u32		RsvdO[3];
	} __attribute__((packed)) XHCI_EPCtx_s;

	typedef struct XHCI_Device_Context {
		XHCI_SlotCtx_s	Slot_Context;
		XHCI_EPCtx_s	Ctrl_EP;
		XHCI_EPCtx_s	IO_EPs[30];
	} XHCI_DevCtx_s;
	
	typedef struct XHCI_Extended_Capabilitie_Entry {
		u8		Cap_ID;
		u8		Next_xECP;
		u16		Cap_Specific;
	} __attribute__((packed)) XHCI_xECP_s;

	void XHCI_init();
	void XHCI_exit();

#endif