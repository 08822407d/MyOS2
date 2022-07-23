#include <string.h>
#include <stdlib.h>
#include <dirent.h>


dirent_s *readdir(DIR *dir)
{
	int len = 0;
	memset(dir->buf, 0, 256);
	len = getdents64(dir->fd, (dirent_s *)dir->buf, 256);
	if(len > 0)
		return (dirent_s *)dir->buf;
	else
		return NULL;
}