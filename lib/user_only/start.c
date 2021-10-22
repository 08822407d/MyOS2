#include "stdlib.h"

extern int main(int, const char **);

void _start(int argc, const char * argv[])
{
	exit(main(argc, argv));
}