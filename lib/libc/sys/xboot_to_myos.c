#include <sys/vfs.h>

#include <unistd.h>

int vfs_open(const char * path, uint32_t flags, uint32_t mode)
{
	return open(path, flags);
}

int vfs_close(int fd)
{
	return close(fd);
}

uint64_t vfs_read(int fd, const void * buf, uint64_t len)
{
	read(fd, buf, len);
}

uint64_t vfs_write(int fd, const void * buf, uint64_t len)
{
	write(fd, buf, len);
}

int64_t vfs_lseek(int fd, int64_t off, int whence)
{
	lseek(fd, off, whence);
}