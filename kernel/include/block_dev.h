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

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "wait_queue.h"

	typedef struct blkdev_ops
	{
		long (* open)();
		long (* close)();
		long (* ioctl)(long cmd,
						long arg);
		long (* transfer)(long cmd,
							unsigned long blk_idx,
							long count,
							unsigned char * buffer);
	} blkdev_ops_s;

	struct blkbuf_node;
	typedef struct blkbuf_node blkbuf_node_s;
	typedef struct blkbuf_node
	{
		wait_queue_T	wq;
		unsigned int	count;
		unsigned char	cmd;
		// unsigned char finished_flag;
		unsigned long	LBA;
		unsigned char *	buffer;
		void			(*end_handler)(unsigned long parameter);
	} blkbuf_node_s;

	typedef struct bdev_req_queue
	{
		wait_queue_hdr_s	bdev_wqhdr;
		blkbuf_node_s *		in_using;
	} bdev_req_queue_T;

#endif /* _BLOCK_H_ */