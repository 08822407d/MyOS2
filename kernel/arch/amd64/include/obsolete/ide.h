/***************************************************
*		版权声明
*
*	本操作系统名为：MINE
*	该操作系统未经授权不得以盈利或非盈利为目的进行开发，
*	只允许个人学习以及公开交流使用
*
*	代码最终所有权及解释权归田宇所有；
*
*	本模块作者：	田宇
*	EMail:		345538255@qq.com
*
*
***************************************************/
#ifndef _DISK_H_
#define _DISK_H_

	#define MASTER					0x0
	#define SLAVE					0x1
	#define IDE_CMD_BASE(c)			(0x1F0 - (c) * 0x80)

	#define IDE_PIO_DATA(p)			(IDE_CMD_BASE(p) + 0)
	#define IDE_PIO_ERR_STAT(p)		(IDE_CMD_BASE(p) + 1)
	#define IDE_PIO_LBA_COUNT(p)	(IDE_CMD_BASE(p) + 2)
	#define IDE_PIO_LBA_LOW(p)		(IDE_CMD_BASE(p) + 3)
	#define IDE_PIO_LBA_MID(p)		(IDE_CMD_BASE(p) + 4)
	#define IDE_PIO_LBA_HIGH(p)		(IDE_CMD_BASE(p) + 5)
	#define IDE_PIO_DEV_OPT(p)		(IDE_CMD_BASE(p) + 6)
	#define IDE_PIO_CMD_STAT(p)		(IDE_CMD_BASE(p) + 7)

	#define IED_PIO_CTRL_BASE(c)	(IDE_CMD_BASE(c) + 0x206)

	#define	PORT_DISK0_ALT_STA_CTL	0x3f6

	#define	DISK_STATUS_BUSY		(1 << 7)
	#define	DISK_STATUS_READY		(1 << 6)
	#define	DISK_STATUS_SEEK		(1 << 4)
	#define	DISK_STATUS_REQ			(1 << 3)
	#define	DISK_STATUS_ERROR		(1 << 0)

	#define ATA_READ_CMD			0x24
	#define ATA_WRITE_CMD			0x34
	#define ATA_INFO_CMD			0xE0
	#define ATA_DISK_IDENTIFY		0xEC

// the definations should be in ATA-8 spec "Table 45 — IDENTIFY DEVICE data"
	typedef struct Identify_Device_data
	{
		//	0		General configuration (see 7.12.7.2)
		//			   15	0 = ATA device
		//			14:8	Retired
		//			 7:6	Obsolete
		//			 5:3	Retired
		//			   2	Incomplete response
		//			   1	Retired
		//			   0	Reserved
		unsigned short General_Config;

		//	1		Obsolete
		unsigned short Obsolete0;

		//	2		Specific configuration (see 7.12.7.4)
		unsigned short Specific_Coinfig;

		//	3		Obsolete
		unsigned short Obsolete1;

		//	4-5		Retired
		unsigned short Retired0[2];

		//	6		Obsolete
		unsigned short Obsolete2;

		//	7-8		Reserved for the CompactFlash Association
		unsigned short CompactFlash[2];

		//	9		Retired
		unsigned short Retired1;

		//	10-19	Serial number (see 7.12.7.10)
		unsigned short Serial_Number[10];

		//	20-21	Retired
		unsigned short Retired2[2];

		//	22		Obsolete
		unsigned short Obsolete3;

		//	23-26	Firmware revision (see 7.12.7.13)
		unsigned short Firmware_Version[4];

		//	27-46	Model number (see 7.12.7.14)
		unsigned short Model_Number[20];

		//	47		See 7.12.7.15
		//			15:8 	80h 
		//			 7:0  	00h=Reserved 
		//					01h-FFh = Maximumnumber of logical sectors that
		//								shall be transferred per DRQ data block
		unsigned short Max_logical_transferred_per_DRQ;

		//	48		Trusted Computing feature set options (see 7.12.7.16)
		//			   15	Shall be cleared to zero
		//			   14	Shall be set to one
		//			13:1	Reserved for the Trusted Computing Group
		//			   0	Trusted Computing feature set is supported
		unsigned short Trusted_Computing_feature_set_options;

		//	49		Capabilities (see 7.12.7.17)
		//			15:14	Reserved for the IDENTIFY PACKET DEVICE command.
		//			   13	1 = Standby timer values as specified in this standard are supported.
		//					0 = Standby timer values shall be vendor specific.
		//			   12	Reserved for the IDENTIFY PACKET DEVICE command.
		//			   11	1 = IORDY (see ATA8-APT) supported
		//					0 = IORDY (see ATA8-APT) may be supported
		//			   10	IORDY (see ATA8-APT) may be disabled
		//			   9	Shall be set to one (i.e., LBA is supported).
		//			   8	DMA supported
		//			 7:2	Reserved
		//			 1:0	Long Physical Sector Alignment Error reporting
		unsigned short Capabilities0;

		//	50		Capabilities (see 7.12.7.17)
		//			   15	Shall be cleared to zero
		//			   14	Shall be set to one
		//			13:2	Reserved
		//			   1	Obsolete
		//			   0	1 = There is a minimum Standby time value and it is vendor specific.
		//					0 = There is no minimum Standby timer value.
		unsigned short Capabilities1;

		//	51-52	Obsolete
		unsigned short Obsolete4[2];

		//	53		See 7.12.7.19
		//			15:8	Free-fall Control Sensitivity
		//			 7:3 	Reserved
		//			   2 	the fields reported in word 88 are valid
		//			   1 	the fields reported in words (70:64) are valid
		//			   0	Obsolete
		unsigned short Report_88_70to64_valid;

		//	54-58	Obsolete
		unsigned short Obsolete5[5];

		//	59		See 7.12.7.21
		//			   15	The BLOCK ERASE EXT command is supported (see 7.36.2)
		//			   14	The OVERWRITE EXT command is supported (see 7.36.4)
		//			   13	The CRYPTO SCRAMBLE EXT command is supported (see 7.36.3)
		//			   12	The Sanitize feature set is supported (see 4.17)
		//			   11	1 = The commands allowed during a sanitize operation are as specified by
		//						this standard (see 4.17.5)
		//					0 = The commands allowed during a sanitize operation are as specified by
		//						ACS-2
		//			   10	The SANITIZE ANTIFREEZE LOCK EXT command is supported (see 7.36.5)
		//			   9	Reserved
		//			   8	Multiple logical sector setting is valid
		//			 7:0	Current setting for number of logical sectors that shall be transferred per
		//					DRQ data block
		unsigned short Mul_Sec_Setting_Valid;

		//	60-61	Total number of user addressable logical sectors for 28-bit commands (DWord)
		//			(see 7.12.7.22)
		unsigned int Addressable_Logical_Sectors_for_28;

		//	62	Obsolete
		unsigned short Obsolete6;

		//	63		See 7.12.7.24
		//			15:11	Reserved
		//			   10	Multiword DMA mode 2 is selected
		//			   9	Multiword DMA mode 1 is selected
		//			   8	Multiword DMA mode 0 is selected
		//			 7:3	Reserved
		//			   2	Multiword DMA mode 2 and below are supported
		//			   1	Multiword DMA mode 1 and below are supported
		//			   0	Multiword DMA mode 0 is supported
		unsigned short MultWord_DMA_Select;

		//	64		See 7.12.7.25
		//			15:2	Reserved
		//			 1:0	PIO mode 3 and mode 4 supported
		unsigned short PIO_mode_supported;

		//	65		Minimum Multiword DMA transfer cycle time per word (see 7.12.7.26)
		unsigned short Min_MulWord_DMA_cycle_time_per_word;

		//	66		Manufacturer’s recommended Multiword DMA transfer cycle time (see 7.12.7.27)
		unsigned short Manufacture_Recommend_MulWord_DMA_cycle_time;

		//	67		Minimum PIO transfer cycle time without flow control (see 7.12.7.28)
		unsigned short Min_PIO_cycle_time_Flow_Control;

		//	68		Minimum PIO transfer cycle time with IORDY (see ATA8-APT) flow control
		//			(see 7.12.7.29)
		unsigned short Min_PIO_cycle_time_IOREDY_Flow_Control;

		//	69		Additional Supported (see 7.12.7.30)
		//			   15	Reserved for CFA
		//			   14	Deterministic data in trimmed LBA range(s) is supported
		//			   13	Long Physical Sector Alignment Error Reporting Control is supported
		//			   12	Obsolete
		//			   11	READ BUFFER DMA is supported
		//			   10	WRITE BUFFER DMA is supported
		//			   9	Obsolete
		//			   8	DOWNLOAD MICROCODE DMA is supported
		//			   7	Reserved for IEEE 1667
		//			   6	0 = Optional ATA device 28-bit commands supported
		//			   5	Trimmed LBA range(s) returning zeroed data is supported
		//			   4	Device Encrypts All User Data on the device
		//			   3	Extended Number of User Addressable Sectors is supported
		//			   2	All write cache is non-volatile
		//			 1:0	Reserved
		unsigned short Additional_Supported;

		//	70		Reserved
		unsigned short Reserved1;

		//	71-74	Reserved for the IDENTIFY PACKET DEVICE command
		unsigned short Reserved2[4];

		//	75		Queue depth (see 7.12.7.33)
		//			15:5	Reserved
		//			 4:0	Maximum queue depth – 1
		unsigned short Queue_depth;

		//	76		Serial ATA Capabilities (see 7.12.7.34)
		//			   15	Supports READ LOG DMA EXT as equivalent to READ LOG EXT
		//			   14	Supports Device Automatic Partial to Slumber transitions
		//			   13	Supports Host Automatic Partial to Slumber transitions
		//			   12	Supports NCQ priority information
		//			   11	Supports Unload while NCQ commands are outstanding
		//			   10	Supports the SATA Phy Event Counters log
		//			   9	Supports receipt of host initiated power management requests
		//			   8	Supports the NCQ feature set
		//			 7:4	Reserved for Serial ATA
		//			   3	Supports SATA Gen3 Signaling Speed (6.0Gb/s)
		//			   2	Supports SATA Gen2 Signaling Speed (3.0Gb/s)
		//			   1	Supports SATA Gen1 Signaling Speed (1.5Gb/s)
		//			   0	Shall be cleared to zero
		unsigned short SATA_Capabilities;

		//	77		Serial ATA Additional Capabilities (see 7.12.7.35)
		//			15:7	Reserved for Serial ATA
		//			   6	Supports RECEIVE FPDMA QUEUED and SEND FPDMA QUEUED
		//			   5	Supports NCQ Queue Management Command
		//			   4	Supports NCQ Streaming
		//			 3:1	Coded value indicating current negotiated Serial ATA signal speed
		//			   0	Shall be cleared to zero
		unsigned short Reserved3;

		//	78		Serial ATA features supported (see 7.12.7.36)
		//			15:8	Reserved for Serial ATA
		//			   7	Device supports NCQ Autosense
		//			   6	Device supports Software Settings Preservation
		//			   5	Device supports Hardware Feature Control
		//			   4	Device supports in-order data delivery
		//			   3	Device supports initiating power management
		//			   2	Device supports DMA Setup auto-activation
		//			   1	Device supports non-zero buffer offsets
		//			   0	Shall be cleared to zero
		unsigned short SATA_features_Supported;

		//	79		Serial ATA features enabled (see 7.12.7.37)
		//			15:8	Reserved for Serial ATA
		//			   7	Automatic Partial to Slumber transitions enabled
		//			   6	Software Settings Preservation enabled
		//			   5	Hardware Feature Control is enabled
		//			   4	In-order data delivery enabled
		//			   3	Device initiated power management enabled
		//			   2	DMA Setup auto-activation enabled
		//			   1	Non-zero buffer offsets enabled
		//			   0	Shall be cleared to zero
		unsigned short SATA_features_enabled;

		//	80		Major version number (see 7.12.7.38)
		//			15:11	Reserved
		//			   10	supports ACS-3
		//			   9	supports ACS-2
		//			   8	supports ATA8-ACS
		//			   7	supports ATA/ATAPI-7
		//			   6	supports ATA/ATAPI-6
		//			   5	supports ATA/ATAPI-5
		//			   4	Obsolete
		//			   3	Obsolete
		//			   2	Obsolete
		//			   1	Obsolete
		//			   0	Reserved
		unsigned short Major_Version;

		//	81		Minor version number (see 7.12.7.39)
		unsigned short Minor_Version;

		//	82		Commands and feature sets supported (see 7.12.7.40)
		//			   15	Obsolete
		//			   14	The NOP command is supported
		//			   13	The READ BUFFER command is supported
		//			   12	The WRITE BUFFER command is supported
		//			11:10	Obsolete
		//			   9	Shall be cleared to zero (i.e., the DEVICE RESET command is not supported)
		//			 8:7	Obsolete
		//			   6	Read look-ahead is supported
		//			   5	The volatile write cache is supported
		//			   4	Shall be cleared to zero (i.e., the PACKET feature set is not supported)
		//			   3	Shall be set to one (i.e., the Power Management feature set is supported)
		//			   2	Obsolete
		//			   1	The Security feature set is supported
		//			   0	The SMART feature set is supported
		unsigned short Cmd_feature_sets_supported0;

		//	83		Commands and feature sets supported (see 7.12.7.40)
		//			   15	Shall be cleared to zero
		//			   14	Shall be set to one
		//			   13	The FLUSH CACHE EXT command is supported
		//			   12	Shall be set to one (i.e., the FLUSH CACHE command is supported)
		//			   11	Obsolete
		//			   10	The 48-bit Address feature set is supported
		//			 9:8	Obsolete
		//			   7	Obsolete
		//			   6	SET FEATURES subcommand is required to spin-up after power-up
		//			   5	The PUIS feature set is supported
		//			   4	Obsolete
		//			   3	The APM feature set is supported
		//			   2	Reserved for CFA
		//			   1	Obsolete
		//			   0	The DOWNLOAD MICROCODE command is supported
		unsigned short Cmd_feature_sets_supported1;

		//	84		Commands and feature sets supported (see 7.12.7.40)
		//			   15	Shall be cleared to zero
		//			   14	Shall be set to one
		//			   13	The IDLE IMMEDIATE command with UNLOAD feature is supported
		//			12:11	Obsolete
		//			10:9	Obsolete
		//			   8	Shall be set to one (i.e., the World Wide Name is supported)
		//			   7	Obsolete
		//			   6	The WRITE DMA FUA EXT command and WRITE MULTIPLE FUA EXT
		//					command are supported
		//			   5	The GPL feature set is supported
		//			   4	The Streaming feature set is supported
		//			   3	Obsolete
		//			   2	Reserved
		//			   1	The SMART self-test is supported
		//			   0	SMART error logging is supported
		unsigned short Cmd_feature_sets_supported2;

		//	85		Commands and feature sets supported or enabled (see 7.12.7.41)
		//			   15	Obsolete
		//			   14	The NOP command is supported
		//			   13	The READ BUFFER command is supported
		//			   12	The WRITE BUFFER command is supported
		//			   11	Obsolete
		//			   10	Obsolete
		//			   9	Shall be cleared to zero (i.e., the DEVICE RESET command is not
		//					supported)
		//			 8:7	Obsolete
		//			   6	Read look-ahead is enabled
		//			   5	The volatile write cache is enabled
		//			   4	Shall be cleared to zero (i.e., the PACKET feature set is not supported)
		//			   3	Shall be set to one (i.e., the Power Management feature set is supported)
		//			   2	Obsolete
		//			   1	The Security feature set is enabled
		//			   0	The SMART feature set is enabled
		unsigned short Cmd_feature_sets_supported3;

		//	86		Commands and feature sets supported or enabled (see 7.12.7.41)
		//			   15	Words 119..120 are valid
		//			   14	Reserved
		//			   13	FLUSH CACHE EXT command supported
		//			   12	FLUSH CACHE command supported
		//			   11	Obsolete
		//			   10	The 48-bit Address features set is supported
		//			 9:8	Obsolete
		//			   7	Obsolete
		//			   6	SET FEATURES subcommand is required to spin-up after power-up
		//			   5	The PUIS feature set is enabled
		//			   4	Obsolete
		//			   3	The APM feature set is enabled
		//			   2	Reserved for CFA
		//			   1	Obsolete
		//			   0	The DOWNLOAD MICROCODE command is supported
		unsigned short Cmd_feature_sets_supported4;

		//	87		Commands and feature sets supported or enabled(see 7.12.7.41)
		//			   15	Shall be cleared to zero
		//			   14	Shall be set to one
		//			   13	The IDLE IMMEDIATE command with UNLOAD FEATURE is supported
		//			12:11	Obsolete
		//			10:9	Obsolete
		//			   8	Shall be set to one (i.e., the World Wide Name is supported)
		//			   7	Obsolete
		//			   6	The WRITE DMA FUA EXT command and WRITE MULTIPLE FUA EXT
		//					command are supported
		//			   5	The GPL feature set is supported
		//			   4	Obsolete
		//			   3	Obsolete
		//			   2	Media serial number is valid
		//			   1	SMART self-test supported
		//			   0	SMART error logging is supported
		unsigned short Cmd_feature_sets_supported5;

		//	88		Ultra DMA modes (see 7.12.7.42)
		//			   15	Reserved
		//			   14	Ultra DMA mode 6 is selected.
		//			   13	Ultra DMA mode 5 is selected.
		//			   12	Ultra DMA mode 4 is selected.
		//			   11	Ultra DMA mode 3 is selected.
		//			   10	Ultra DMA mode 2 is selected.
		//			   9	Ultra DMA mode 1 is selected.
		//			   8	Ultra DMA mode 0 is selected.
		//			   7	Reserved
		//			   6	Ultra DMA mode 6 and below are supported
		//			   5	Ultra DMA mode 5 and below are supported
		//			   4	Ultra DMA mode 4 and below are supported
		//			   3	Ultra DMA mode 3 and below are supported
		//			   2	Ultra DMA mode 2 and below are supported
		//			   1	Ultra DMA mode 1 and below are supported
		//			   0	Ultra DMA mode 0 is supported
		unsigned short Ultra_DMA_modes;

		//	89		See 7.12.7.43
		//			   15	1=Extended Time is reported in bits 14:0
		//					0=Time is reported in bits 7:0
		//			If bit 15 is set to one
		//			14:0Extended Time required for Normal Erase mode SECURITY ERASE UNIT
		//					command
		//			If bit 15 is set to zero
		//			14:8	Reserved
		//			 7:0	Time required for Normal Erase mode SECURITY ERASE UNIT command	
		unsigned short Time_required_Erase_CMD;

		//	90		See 7.12.7.44
		//			   15	1=Extended Time is reported in bits 14:0
		//					0=Time is reported in bits 7:0
		//			If bit 15 is set to one
		//			14:0	Extended Time required for Enhanced Erase mode SECURITY ERASE UNIT
		//					command
		//			If bit 15 is set to zero
		//			14:8	Reserved
		//			 7:0	Time required for Enhanced Erase mode SECURITY ERASE UNIT command	
		unsigned short Time_required_Enhanced_CMD;

		//	91
		//			15:8	Reserved
		//			 7:0	Current APM level value (see 7.12.7.45)
		unsigned short Current_APM_level_Value;

		//	92		Master Password Identifier (see 7.12.7.46)	
		unsigned short Master_Password_Identifier;

		//	93		Hardware reset results (see 7.12.7.47)
		//			For SATA devices, word 93 shall be set to the value 0000h.
		//			   15	Shall be cleared to zero.
		//			   14	Shall be set to one for PATA devices.
		//			   13	1 = device detected the CBLID- above V iHB (see ATA8-APT)
		//					0 = device detected the CBLID- below V iL (see ATA8-APT)
		//			12:8	Device 1 hardware reset result. Device 0 shall clear these bits to zero.
		//					Device 1 shall set these bits as follows:
		//			   12	Reserved
		//			   11	Device 1 asserted PDIAG-.
		//			10:9	These bits indicate how Device 1 determined the device number:
		//					00 = Reserved
		//					01 = a jumper was used.
		//					10 = the CSEL signal was used.
		//					11 = some other method was used or the method is unknown.
		//			   8	Shall be set to one.
		//			 7:0	Device 0 hardware reset result. Device 1 shall clear these bits to zero.
		//					Device 0 shall set these bits as follows:
		//			   7	Reserved
		//			   6	Device 0 responds when Device 1 is selected.
		//			   5	Device 0 detected the assertion of DASP-.
		//			   4	Device 0 detected the assertion of PDIAG-.
		//			   3	Device 0 passed diagnostics.
		//			 2:1	These bits indicate how Device 0 determined the device number:
		//					00 = Reserved
		//					01 = a jumper was used.
		//					10 = the CSEL signal was used.
		//					11 = some other method was used or the method is unknown.
		//			   0	Shall be set to one for PATA devices.
		unsigned short HardWare_Reset_Result;

		//	94		Obsolete
		unsigned short Obsolete7;

		//	95		Stream Minimum Request Size (see 7.12.7.49)
		unsigned short Stream_Min_Request_Size;

		//	96		Streaming Transfer Time – DMA (see 7.12.7.50)
		unsigned short Streaming_Transger_time_DMA;

		//	97		Streaming Access Latency – DMA and PIO (see 7.12.7.51)
		unsigned short Streaming_Access_Latency_DMA_PIO;

		//	98-99	Streaming Performance Granularity (DWord) (see 7.12.7.52)
		unsigned short Streaming_Performance_Granularity[2];

		//	100-103	Number of User Addressable Logical Sectors (QWord) (see 7.12.7.53)
		unsigned long Total_user_LBA_for_48_Address_Feature_set;

		//	104		Streaming Transfer Time – PIO (see 7.12.7.54)
		unsigned short Streaming_Transfer_Time_PIO;

		//	105		Maximum number of 512-byte blocks per DATA SET MANAGEMENT command (see 7.5)
		unsigned short MaxBlks_per_DSM;

		//	106		Physical sector size / logical sector size (see 7.12.7.56)
		//			   15	Shall be cleared to zero
		//			   14	Shall be set to one
		//			   13	Device has multiple logical sectors per physical sector.
		//			   12	Device Logical Sector longer than 256 words
		//			11:4	Reserved
		//			 3:0	2^X logical sectors per physical sector
		unsigned short Physical_Logical_Sector_Size;

		//	107		Inter-seek delay for ISO/IEC 7779 standard acoustic testing (see 7.12.7.57)
		unsigned short Inter_seek_delay;

		//	108-111	World wide name (see 7.12.7.58)
		unsigned short World_wide_name[4];

		//	112-115	Reserved
		unsigned short Reserved5[4];

		//	116		Obsolete
		unsigned short Obsolete8;

		//	117-118	Logical sector size (DWord) (see 7.12.7.61)
		unsigned short Words_per_Logical_Sector[2];

		//	119		Commands and feature sets supported (Continued from words 82..84) (see 7.12.7.40)
		//			   15	Shall be cleared to zero
		//			   14	Shall be set to one
		//			13:10	Reserved
		//			   9	DSN feature set is supported
		//			   8	Accessible Max Address Configuration feature set is supported
		//			   7	EPC feature set is supported
		//			   6	Sense Data Reporting feature set is supported
		//			   5	The Free-fall Control feature set is supported
		//			   4	Download Microcode mode 3 is supported
		//			   3	The READ LOG DMA EXT command and WRITE LOG DMA EXT
		//					command are supported
		//			   2	The WRITE UNCORRECTABLE EXT command is supported
		//			   1	The Write-Read-Verify feature set is supported
		//			   0	Obsolete
		unsigned short CMD_feature_Supported;

		//	120		Commands and feature sets supported or enabled (Continued from words 85..87)
		//			(see 7.12.7.41)
		//			   15	Shall be cleared to zero
		//			   14	Shall be set to one
		//			13:10	Reserved
		//			   9	DSN feature set is enabled
		//			   8	Reserved
		//			   7	EPC feature set is enabled
		//			   6	Sense Data Reporting feature set is enabled
		//			   5	The Free-fall Control feature set is enabled
		//			   4	Download Microcode mode 3 is supported
		//			   3	The READ LOG DMA EXT command and WRITE LOG DMA EXT
		//					command are supported
		//			   2	The WRITE UNCORRECTABLE EXT command is supported
		//			   1	The Write-Read-Verify feature set is enabled
		//			   0	Obsolete
		unsigned short CMD_feature_Supported_enabled;

		//	121-126	Reserved for expanded supported and enabled settings
		unsigned short Reserved7[6];

		//	127		Obsolete
		unsigned short Obsolete9;

		//	128		Security status (see 7.12.7.66)
		//			15:9	Reserved
		//			   8	Master Password Capability: 0 = High, 1 = Maximum
		//			 7:6	Reserved
		//			   5	Enhanced security erase supported
		//			   4	Security count expired
		//			   3	Security frozen
		//			   2	Security locked
		//			   1	Security enabled
		//			   0	Security supported
		unsigned short Security_Status;

		//	129-159	Vendor specific
		unsigned short Vendor_Specific[31];

		//	160-167	Reserved for CFA (see 7.12.7.68)
		unsigned short Reserved8[8];

		//	168		See 7.12.7.69
		//			15:4	Reserved
		//			 3:0	Device Nominal Form Factor
		unsigned short Dev_from_Factor;

		//	169-175	Reserved
		unsigned short Reserved9[7];

		//	176-205	Current media serial number (ATA string)
		unsigned short Current_Media_Serial_Number[30];

		//	206		SCT Command Transport
		unsigned short SCT_Cmd_Transport;

		//	207-208	Reserved for CE-ATA
		unsigned short Reserved10[2];

		//	209		Alignment of logical blocks within a physical block
		unsigned short Alignment_Logical_blocks_within_a_physical_block;

		//	210-211	Write-Read-Verify Sector Count Mode 3 (DWord)
		unsigned short Write_Read_Verify_Sector_Count_Mode_3[2];

		//	212-213	Write-Read-Verify Sector Count Mode 2 (DWord)
		unsigned short Write_Read_Verify_Sector_Count_Mode_2[2];

		//	214		NV Cache Capabilities
		unsigned short NV_Cache_Capabilities;

		//	215-216	NV Cache Size in Logical Blocks (DWord)
		unsigned short NV_Cache_Size[2];

		//	217		Nominal media rotation rate
		unsigned short Nominal_media_rotation_rate;

		//	218		Reserved
		unsigned short Reserved11;

		//	219		NV Cache Options
		unsigned short NV_Cache_Options;

		//	220		Write-Read-Verify feature set current mode
		unsigned short Write_Read_Verify_feature_set_current_mode;

		//	221		Reserved
		unsigned short Reserved12;

		//	222		Transport major version number. 
		//			0000h or ffffh = device does not report version
		unsigned short Transport_Major_Version_Number;

		//	223		Transport Minor version number
		unsigned short Transport_Minor_Version_Number;

		//	224-233	Reserved for CE-ATA
		unsigned short Reserved13[10];

		//	234		Minimum number of 512-byte data blocks per DOWNLOAD MICROCODE command for mode 03h
		unsigned short Mini_blocks_per_CMD;

		//	235		Maximum number of 512-byte data blocks per DOWNLOAD MICROCODE command for mode 03h
		unsigned short Max_blocks_per_CMD;

		//	236-254	Reserved
		unsigned short Reserved14[19];

		//	255		Integrity word
		//			15:8	Checksum
		//			 7:0	Checksum Validity Indicator
		unsigned short Integrity_word;
	} __attribute__((packed)) IDE_id_dev_data_s;

#endif /* _DISK_H_ */