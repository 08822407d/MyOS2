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
	#define IDE_CMD_BASE(c)			(0x1F0U - (c) * 0x80U)

	#define IDE_PIO_DATA(p)			(IDE_CMD_BASE(p) + 0)
	#define IDE_PIO_ERR_STAT(p)		(IDE_CMD_BASE(p) + 1)
	#define IDE_PIO_LBA_COUNT(p)	(IDE_CMD_BASE(p) + 2)
	#define IDE_PIO_LBA_LOW(p)		(IDE_CMD_BASE(p) + 3)
	#define IDE_PIO_LBA_MID(p)		(IDE_CMD_BASE(p) + 4)
	#define IDE_PIO_LBA_HIGH(p)		(IDE_CMD_BASE(p) + 5)
	#define IDE_PIO_DEV_OPT(p)		(IDE_CMD_BASE(p) + 6)
	#define IDE_PIO_CMD_STAT(p)		(IDE_CMD_BASE(p) + 7)

	#define IED_PIO_CTRL_BASE(c)	(IDE_CMD_BASE(c) + 0x206U)

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


#endif /* _DISK_H_ */