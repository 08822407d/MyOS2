#include <stdlib.h>

extern int main(int, const char **);

__attribute__((section(".crt1.text"))) void _start(int argc, const char * argv[])
{
	exit(main(argc, argv));
}