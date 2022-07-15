#define _GNU_SOURCE
#include <dirent.h> /* Defines DT_* constants */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#define handle_error(msg) \ do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct linux_dirent
{
	long d_ino;
	off_t d_off;
	unsigned short d_reclen;
	char d_name[];
};

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	int fd, nread;
	char buf[BUF_SIZE];
	struct linux_dirent *d;
	int bpos;
	char d_type;
	fd = open(argc > 1 ? argv[1] : ".", O_RDONLY | O_DIRECTORY);
	if (fd == -1)
		handle_error("open");
	for ( ; ; )
	{
		nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
		if (nread == -1)
			handle_error("getdents");
		if (nread == 0)
			break;
			
		printf("--------------- nread=%d ---------------", nread);
		printf("inode# file type d_reclen d_off d_name");
		for (bpos = 0; bpos < nread;)
		{
			d = (struct linux_dirent *) (buf + bpos);
			printf("%8ld ", d->d_ino);
			d_type = *(buf + bpos + d->d_reclen - 1);
			printf("%-10s ",
					(d_type == DT_REG)	? "regular" : (d_type == DT_DIR)
										? "directory" : (d_type == DT_FIFO)
										? "FIFO" : (d_type == DT_SOCK)
										? "socket" : (d_type == DT_LNK)
										? "symlink" : (d_type == DT_BLK)
										? "block dev" : (d_type == DT_CHR)
										? "char dev" : "???");
			printf("%4d %10lld %s", d->d_reclen, (long long) d->d_off, d->d_name);
			bpos += d->d_reclen;
		}
	}
	
	exit(EXIT_SUCCESS);
}

/*
 * This is the "filldir" function type, used by readdir() to let
 * the kernel specify what kind of dirent layout it wants to have.
 * This allows the kernel to read directories into kernel space or
 * to have different dirent layouts depending on the binary type.
 */
dir_ctxt_s;
typedef int (*filldir_t)(dir_ctxt_s *, const char *, int, loff_t, u64,
			 unsigned);

dir_ctxt_s {
	filldir_t actor;
	loff_t pos;
};

struct linux_dirent64 {
	u64		d_ino;
	s64		d_off;
	unsigned short	d_reclen;
	unsigned char	d_type;
	char		d_name[];
};

struct getdents_callback64 {
	dir_ctxt_s ctx;
	struct linux_dirent64 __user * current_dir;
	int prev_reclen;
	int count;
	int error;
};