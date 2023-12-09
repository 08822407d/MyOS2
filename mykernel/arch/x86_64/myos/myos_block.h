#include <linux/lib/list.h>
#include <linux/kernel/completion.h>

struct blkbuf_node;
typedef struct blkbuf_node blkbuf_node_s;
typedef struct blkbuf_node
{
	List_s			req_list;
	unsigned		ATA_controller;
	unsigned		ATA_disk;

	completion_s	*done;
	task_s			*task;
	unsigned int	count;
	unsigned char	cmd;
	unsigned long	LBA;
	unsigned char	*buffer;
	void			(*end_handler)(unsigned long parameter);
} blkbuf_node_s;