/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/

#ifndef __UNISTD_H__

#define __UNISTD_H__

int		open(char *filename, int flags);
int		close(int fd);
long	read(int fd, void *buf, long nbyte);
long	write(int fd, const void *buf, long nbyte);
long	lseek(int fd, long offset, int whence);
long	chdir(char * path);

int		fork(void);
int 	vfork(void);
int 	execve(const char *path, char *const argv[], char *const envp[]);


#endif
