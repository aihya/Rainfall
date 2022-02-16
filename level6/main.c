#include <stdio.h>

void	n() // 0x8048454
{
	system("/bin/sh");
}

void	m() // 0x8048468
{
	puts("Nope");
}

void	main(int argc, char *argv[])
{
	void	*buffer1;
	void	*buffer2;

	buffer1 = malloc(0x40); // 64 bytes
	buffer2 = malloc(0x4); // 4 bytes

	*buffer2 = 0x8048468;

	strcpy(buffer1, argv[1]);
	((void(*(*buffer2))(void))();
}
