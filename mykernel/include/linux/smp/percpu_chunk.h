// source: linux-6.11

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _PERCPU_CHUNK_H_
#define _PERCPU_CHUNK_H_

#include <linux/compiler/compiler.h>
#include <linux/kernel/cache.h>
#include <linux/kernel/types.h>
#include <linux/kernel/lib_api.h>


	struct simple_pcpu_chunk {
		List_s		list;			/* linked to pcpu_slot lists */
		int			free_bytes;		/* free bytes in the chunk */
		int			free_offset;

		// 保存后续新创建chunk和fisrt-chunk之间的差值
		// 用于确保通过perpcu-API访问动态分配的percpu变量时
		// 可以正确计算offset
		void		*base_addr;
		// 通过memblock或者slub-buddy申请到的内存块的虚拟地址
		void		*data;			/* chunk data */
	};

	struct simple_pcpu_chunk;
	typedef struct simple_pcpu_chunk pcpu_chunk_s;

#endif /* _PERCPU_CHUNK_H_ */
