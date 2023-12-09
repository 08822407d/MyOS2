/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/

#ifndef __NVMe_H__

#define __NVMe_H__

#include <linux/kernel/types.h>
// #include "block.h"

////Opcodes for Admin Commands
#define	ADMIN_CMD_DELETE_IO_SUBMISSION_QUEUE	0x00
#define ADMIN_CMD_CREATE_IO_SUBMISSION_QUEUE	0x01
#define ADMIN_CMD_GET_LOG_PAGE			0x02
#define ADMIN_CMD_DELETE_IO_COMPLETION_QUEUE	0x04
#define ADMIN_CMD_CREATE_IO_COMPLETION_QUEUE	0x05
#define ADMIN_CMD_IDENTIFY			0x06
#define ADMIN_CMD_ABORT				0x08
#define ADMIN_CMD_SET_FEATURES			0x09
#define ADMIN_CMD_GET_FEATURES			0x0A
#define ADMIN_CMD_ASYNCHRONOUS_EVENT_REQUEST	0x0C
#define ADMIN_CMD_NAMESPACE_MANAGEMENT		0x0D
#define ADMIN_CMD_FIRMWARE_COMMIT		0x10
#define ADMIN_CMD_FIRMWARE_IMAGE_DOWNLOAD	0x11
#define ADMIN_CMD_DEVICE_SELF_TEST		0x14
#define ADMIN_CMD_NAMESPACE_ATTACHMENT		0x15
#define ADMIN_CMD_KEEP_ALIVE			0x18
#define ADMIN_CMD_DIRECTIVE_SEND		0x19
#define ADMIN_CMD_DIRECTIVE_RECEIVE		0x1A
#define ADMIN_CMD_VIRTUALIZATION_MANAGEMENT	0x1C
#define ADMIN_CMD_NVMe_MI_SEND			0x1D
#define ADMIN_CMD_NVMe_MI_RECEIVE		0x1E
#define ADMIN_CMD_DOORBELL_BUFFER_CONFIG	0x7C

////Opcodes for NVM Commands
#define NVM_CMD_FLUSH				0x00
#define NVM_CMD_WRITE				0x01
#define NVM_CMD_READ				0x02
#define NVM_CMD_WRITE_UNCORRECTABLE		0x04
#define NVM_CMD_COMPARE				0x05
#define NVM_CMD_WRITE_ZEROES			0x08
#define NVM_CMD_DATASET_MANAGEMENT		0x09
#define NVM_CMD_RESERVATION_REGISTER		0x0D
#define NVM_CMD_RESERVATION_REPORT		0x0E
#define NVM_CMD_RESERVATION_ACQUIRE		0x11
#define NVM_CMD_RESERVATION_RELEASE		0x15


struct NVMe_Controller_Registers
{
	unsigned long	CAP;
	unsigned int	VS;
	unsigned int	INTMS;

	unsigned int	INTMC;
	unsigned int	CC;
	unsigned int	Reserved;
	unsigned int	CSTS;

	unsigned int	NSSR;
	unsigned int	AQA;
	unsigned long	ASQ;

	unsigned long	ACQ;
	unsigned int	CMBLOC;
	unsigned int	CMBSZ;

	unsigned int	BPINFO;
	unsigned int	BPRSEL;
	unsigned long	BPMBL;
}__attribute__((packed));

///Submission Queue Entry – Command Format

struct Submission_Queue_Entry
{
	///Dword 00
	u32		OPC		:8,		///Opcode
			FUSE	:2,		///Fused Operation
			R		:4,		///Reserved
			PSDT	:2,		///PRP or SGL for Data Transfer
			CID		:16;	///Command Identifier
	///Dword 01
	u32 	NSID;			///Namespace Identifier

	///Dword 02~03
	u32		Dword02;
	u32		Dword03;

	///Dword 04~05
	u64		MPTR;			///Metadata Pointer

	///Dword 06~09
	u64		PRP_SGL_Entry1;	///PRP Entry 1 | SGL Entry 1
	u64		PRP_SGL_Entry2;	///PRP Entry 2 | SGL Entry 2

	///Dword 10~15
	u32		Dword10;
	u32		Dword11;
	u32		Dword12;
	u32		Dword13;
	u32		Dword14;
	u32		Dword15;
}__attribute__((packed));

///Physical Region Page Entry and List

struct Physical_Region_Page_Entry
{
	u64		R:2,
			PBAO:62;	///Page Base Address and Offset
}__attribute__((packed));

///Scatter Gather List (SGL)

struct SGL_Data_Block_Descriptor
{
	unsigned long	Address;
	unsigned int	Length;
	unsigned char	Reserved[3];
}__attribute__((packed));

struct SGL_Bit_Bucket_Descriptor
{
	unsigned long	Reserved0;
	unsigned int	Length;
	unsigned char	Reserved1[3];
}__attribute__((packed));

struct SGL_Segment_Descriptor
{
	unsigned long	Address;
	unsigned int	Length;
	unsigned char	Reserved[3];
}__attribute__((packed));

struct SGL_Last_Segment_Descriptor
{
	unsigned long	Address;
	unsigned int	Length;
	unsigned char	Reserved[3];
}__attribute__((packed));

struct Keyed_SGL_Data_Block_Descriptor
{
	unsigned long	Address;
	unsigned int	Length;
	unsigned char	Reserved[3];
}__attribute__((packed));

union SGL_Descriptor
{
	struct SGL_Data_Block_Descriptor	SGL_Data_Block_Desc;
	struct SGL_Bit_Bucket_Descriptor	SGL_Bit_Bucket_Desc;
	struct SGL_Segment_Descriptor		SGL_Segment_Desc;
	struct SGL_Last_Segment_Descriptor	SGL_Last_Segment_Desc;
	struct Keyed_SGL_Data_Block_Descriptor	Keyed_SGL_Data_Block_Desc;
};

struct Scatter_Gather_List_Descriptor
{
	union SGL_Descriptor	Description;	///Descriptor Type Specific
	unsigned char	SGL_Desc_SUB_Type:4,	///SGL Descriptor Sub Type->SGL Identifier
			SGL_Desc_Type:4;	///SGL Descriptor Type
}__attribute__((packed));

///Completion Queue Entry

struct Completion_Queue_Entry
{
	unsigned int	CMD;	///Command Specific
	unsigned int	Reserved;
	unsigned int	SQHD:16,	///SQ Head Pointer
			SQID:16;	///SQ Identifier
	unsigned int	CID:16,	///Command Identifier
			P:1,	///Phase Tag
			SC:8,	///Status Code->Status Field
			SCT:3,	///Status Code Type
			R:2,	///Reserved
			M:1,	///More
			DNR:1;	///Do Not Retry
}__attribute__((packed));

struct NVMe_Identify_Controller_Data_Structure
{
	////////////////Controller Capabilities and Features 0-255
	unsigned short	VID;
	unsigned short	SSVID;
	unsigned char	SN[20];
	unsigned char	MN[40];
	unsigned char	FR[8];
	unsigned char	RAB;
	unsigned char	IEEE[3];
	unsigned char	CMIC;
	unsigned char	MDTS;
	unsigned short	CNTLID;
	unsigned int	VER;
	unsigned int	RTD3R;
	unsigned int	RTD3E;
	unsigned int	OAES;
	unsigned int	CTRATT;
	unsigned char	Reserved0[12];
	unsigned int	FGUID[4];
	unsigned char	Reserved1[112];
	unsigned char	Reserved2[16];
	////////////////Admin Command Set Attributes & Optional Controller Capabilities 256-511
	unsigned short	OACS;
	unsigned char	ACL;
	unsigned char	AERL;
	unsigned char	FRMW;
	unsigned char	LPA;
	unsigned char	ELPE;
	unsigned char	NPSS;
	unsigned char	AVSCC;
	unsigned char	APSTA;
	unsigned short	WCTEMP;
	unsigned short	CCTEMP;
	unsigned short	MTFA;
	unsigned int	HMPRE;
	unsigned int	HMMIN;
	unsigned char	TNVMCAP[16];
	unsigned char	UNVMCAP[16];
	unsigned int	RPMBS;
	unsigned short	EDSTT;
	unsigned char	DSTO;
	unsigned char	FWUG;
	unsigned short	KAS;
	unsigned short	HCTMA;
	unsigned short	MNTMT;
	unsigned short	MXTMT;
	unsigned int	SANICAP;
	unsigned char	Reserved3[180];
	////////////////NVM Command Set Attributes 512-2047
	unsigned char	SQES;
	unsigned char	CQES;
	unsigned short	MAXCMD;
	unsigned int	NN;
	unsigned short	ONCS;
	unsigned short	FUSES;
	unsigned char	FNA;
	unsigned char	VWC;
	unsigned short	AWUN;
	unsigned short	AWUPF;
	unsigned char	NVSCC;
	unsigned char	Reserved4;
	unsigned short	ACWU;
	unsigned char	Reserved5[2];
	unsigned int	SGLS;
	unsigned char	Reserved6[228];
	unsigned short	SUBNQN[128];
	unsigned char	Reserved7[768];
	unsigned char	Reserved8[256];
	////////////////Power State Descriptors 2048-3071
	unsigned char	PSD0[32];
	unsigned char	PSD1[32];
	unsigned char	PSD2[32];
	unsigned char	PSD3[32];
	unsigned char	PSD4[32];
	unsigned char	PSD5[32];
	unsigned char	PSD6[32];
	unsigned char	PSD7[32];
	unsigned char	PSD8[32];
	unsigned char	PSD9[32];

	unsigned char	PSD10[32];
	unsigned char	PSD11[32];
	unsigned char	PSD12[32];
	unsigned char	PSD13[32];
	unsigned char	PSD14[32];
	unsigned char	PSD15[32];
	unsigned char	PSD16[32];
	unsigned char	PSD17[32];
	unsigned char	PSD18[32];
	unsigned char	PSD19[32];

	unsigned char	PSD20[32];
	unsigned char	PSD21[32];
	unsigned char	PSD22[32];
	unsigned char	PSD23[32];
	unsigned char	PSD24[32];
	unsigned char	PSD25[32];
	unsigned char	PSD26[32];
	unsigned char	PSD27[32];
	unsigned char	PSD28[32];
	unsigned char	PSD29[32];

	unsigned char	PSD30[32];
	unsigned char	PSD31[32];
	////////////////Vendor Specific 3072-4095
	unsigned char	Reserved9[1024];
}__attribute__((packed));

///Status Code – Generic Command Status Values

#define Status_Code_Successful_Completion	00h
#define Status_Code_Invalid_Command_Opcode	01h
#define Status_Code_Invalid_Field_in_Command	02h
#define Status_Code_Command_ID_Conflict		03h
#define Status_Code_Data_Transfer_Error		04h
#define Status_Code_Commands_Aborted_due_to_Power_Loss_Notification	05h
#define Status_Code_Internal_Error		06h
#define Status_Code_Command_Abort_Requested	07h
#define Status_Code_Command_Aborted_due_to_SQ_Deletion		08h
#define Status_Code_Command_Aborted_due_to_Failed_Fused_Command		09h
#define Status_Code_Command_Aborted_due_to_Missing_Fused_Command	0Ah
#define Status_Code_Invalid_Namespace_or_Format		0Bh
#define Status_Code_Command_Sequence_Error		0Ch
#define Status_Code_Invalid SGL Segment Descriptor	0Dh
#define Status_Code_Invalid_Number_of_SGL_Descriptors	0Eh
#define Status_Code_Data_SGL_Length_Invalid		0Fh
#define Status_Code_Metadata_SGL_Length_Invalid		10h
#define Status_Code_SGL_Descriptor_Type_Invalid		11h
#define Status_Code_Invalid_Use_of_Controller_Memory_Buffer	12h
#define Status_Code_PRP_Offset_Invalid			13h
#define Status_Code_Atomic_Write_Unit_Exceeded		14h
#define Status_Code_Operation_Denied			15h
#define Status_Code_SGL_Offset_Invalid			16h
#define Status_Code_Host_Identifier_Inconsistent_Format		18h
#define Status_Code_Keep_Alive_Timeout_Expired		19h
#define Status_Code_Keep_Alive_Timeout_Invalid		1Ah
#define Status_Code_Command_Aborted_due_to_Preempt_and_Abort	1Bh
#define Status_Code_Sanitize_Failed			1Ch
#define Status_Code_Sanitize_In_Progress		1Dh
#define Status_Code_SGL_Data_Block_Granularity_Invalid		1Eh
#define Status_Code_Command_Not_Supported_for_Queue_in_CMB	1Fh

///Status Code – Generic Command Status Values, NVM Command Set

#define Status_Code_LBA_Out_of_Range		80h
#define Status_Code_Capacity_Exceeded		81h
#define Status_Code_Namespace_Not_Ready		82h
#define Status_Code_Reservation_Conflict	83h
#define Status_Code_Format_In_Progress		84h

///Status Code – Command Specific Status Values

#define Status_Code_Completion_Queue_Invalid		00h
#define Status_Code_Invalid_Queue_Identifier		01h
#define Status_Code_Invalid_Queue_Size			02h
#define Status_Code_Abort_Command_Limit_Exceeded	03h
#define Status_Code_Asynchronous_Event_Request_Limit_Exceeded		05h
#define Status_Code_Invalid_Firmware_Slot		06h
#define Status_Code_Invalid_Firmware_Image		07h
#define Status_Code_Invalid_Interrupt_Vector		08h
#define Status_Code_Invalid_Log_Page			09h
#define Status_Code_Invalid_Format			0Ah
#define Status_Code_Firmware_Activation_Requires_Conventional_Reset	0Bh
#define Status_Code_Invalid_Queue_Deletion		0Ch
#define Status_Code_Feature_Identifier_Not_Saveable	0Dh
#define Status_Code_Feature_Not_Changeable		0Eh
#define Status_Code_Feature_Not_Namespace_Specific	0Fh
#define Status_Code_Firmware_Activation_Requires_NVM_Subsystem_Reset	10h
#define Status_Code_Firmware_Activation_Requires_Reset			11h
#define Status_Code_Firmware_Activation_Requires_Maximum_Time_Violation		12h
#define Status_Code_Firmware_Activation_Prohibited	13h
#define Status_Code_Overlapping_Range			14h
#define Status_Code_Namespace_Insufficient_Capacity	15h
#define Status_Code_Namespace_Identifier_Unavailable	16h
#define Status_Code_Namespace_Already_Attached		18h
#define Status_Code_Namespace_Is_Private		19h
#define Status_Code_Namespace_Not_Attached		1Ah
#define Status_Code_Thin_Provisioning_Not_Supported	1Bh
#define Status_Code_Controller_List_Invalid		1Ch
#define Status_Code_Device_Self_test_In_Progress	1Dh
#define Status_Code_Boot_Partition_Write_Prohibited	1Eh
#define Status_Code_Invalid_Controller_Identifier	1Fh
#define Status_Code_Invalid_Secondary_Controller_State	20h
#define Status_Code_Invalid_Number_of_Controller_Resources	21h
#define Status_Code_Invalid_Resource_Identifier		22h

///Status Code – Command Specific Status Values, NVM Command Set

#define Status_Code_Conflicting_Attributes		80h
#define Status_Code_Invalid_Protection_Information	81h
#define Status_Code_Attempted_Write_to_Read_Only_Range	82h

///Status Code – Media and Data Integrity Error Values, NVM Command Set

#define Status_Code_Write_Fault			80h
#define Status_Code_Unrecovered_Read_Error	81h
#define Status_Code_End_to_end_Guard_Check_Error	82h
#define Status_Code_End_to_end_Application_Tag_Check_Error	83h
#define Status_Code_End_to_end_Reference_Tag_Check_Error	84h
#define Status_Code_Compare_Failure		85h
#define Status_Code_Access_Denied		86h
#define Status_Code_Deallocated_or_Unwritten_Logical_Block	87h


/*

*/

extern struct block_device_operation NVMe_device_operation;

void NVMe_init();

void NVMe_exit();

#endif

