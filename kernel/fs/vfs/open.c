#include <include/fs/file.h>
#include <include/fs/namei.h>

// Linux function proto:
// long do_sys_open(int dfd, const char __user *filename, int flags, umode_t mode)
long do_sys_open(int dfd, const char * filename, int flags)
{
	// Linux call stack:
	//					||
	//					\/
	// static long do_sys_openat2(int dfd, const char __user *filename,
	//				   struct open_how *how)
	int fd;

	const char * k_filename;
	unsigned long err = getname(&k_filename, filename);
	fd = get_unused_fd_flags(0);
	if (fd >=0)
	{
		file_s * fp = do_filp_open(dfd, k_filename);
		if (fp == NULL)
		{

		}
		else
		{
			fd_install(fd, fp);
		}
	}
}