#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <dirent.h>

#include <sys/fcntl.h>

DIR* opendir(const char *path)
{
	int fd = 0;
	DIR* dir = NULL;

	fd = open(path, O_DIRECTORY, 0);

	if(fd >= 0)
	{
		dir = (DIR*)malloc(sizeof(DIR));
		if (dir == NULL)
		{
			close(fd);
			return (DIR *)-ENOMEM;
		}
		memset(dir, 0, sizeof(DIR));
		dir->fd = fd;
		dir->buf_pos = 0;
		dir->buf_end = 256;

		return dir;
	}
	else
		return NULL;
}