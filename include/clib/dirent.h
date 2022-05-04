#ifndef	_DIRENT_H
#define	_DIRENT_H

#include <linux/kernel/types.h>

#include <uapi/dirent.h>

typedef linux_dirent64_s dirent_s;

#ifdef __cplusplus
extern "C" {
#endif

	#define __NEED_ino_t
	#define __NEED_off_t
	#if defined(_BSD_SOURCE) || defined(_GNU_SOURCE)
		#define __NEED_size_t
	#endif

	struct __dirstream
	{
		off_t tell;
		int fd;
		int buf_pos;
		int buf_end;
		volatile int lock[1];
		/* Any changes to this struct must preserve the property:
		* offsetof(struct __dirent, buf) % sizeof(off_t) == 0 */
		char buf[2048];
	};

	typedef struct __dirstream DIR;

	#define d_fileno d_ino

	int			closedir(DIR *);
	DIR			*fdopendir(int);
	DIR			*opendir(const char *);
	dirent_s 	*readdir(DIR *);
	int			readdir_r(DIR *__restrict, dirent_s *__restrict, dirent_s **__restrict);
	void		rewinddir(DIR *);
	int			dirfd(DIR *);

	int	alphasort(const dirent_s **, const dirent_s **);
	int	scandir(const char *, dirent_s ***, int (*)(const dirent_s *), int (*)(const dirent_s **, const dirent_s **));

	int getdents64(unsigned int fd, dirent_s *dirent, unsigned int count);

	#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
		void	seekdir(DIR *, long);
		long	telldir(DIR *);
	#endif

	#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
		#define DT_UNKNOWN 0
		#define DT_FIFO 1
		#define DT_CHR 2
		#define DT_DIR 4
		#define DT_BLK 6
		#define DT_REG 8
		#define DT_LNK 10
		#define DT_SOCK 12
		#define DT_WHT 14
		#define IFTODT(x) ((x)>>12 & 017)
		#define DTTOIF(x) ((x)<<12)
		int getdents(int, dirent_s *, size_t);
	#endif

	#ifdef _GNU_SOURCE
	int versionsort(const dirent_s **, const dirent_s **);
	#endif

	#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
		#define dirent64 dirent
		#define readdir64 readdir
		#define readdir64_r readdir_r
		#define scandir64 scandir
		#define alphasort64 alphasort
		#define versionsort64 versionsort
		#define off64_t off_t
		#define ino64_t ino_t
		#define getdents64 getdents
	#endif

#ifdef __cplusplus
}
#endif

#endif