#ifndef _MOUNT_H_
#define _MOUNT_H_

#include <include/fs/dcache.h>

	struct mount;
	typedef struct mount mount_s;

	typedef struct mountpoint
	{
		dentry_s *	m_dentry;
	} mountpoint_s;
	

	typedef struct mount
	{

	} mount_s;

#endif /* _MOUNT_H_ */