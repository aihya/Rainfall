#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
	char	*buffer1;
	int		*buffer2;

	buffer1 = malloc(0x40); // esp + 0x1c
	buffer2 = malloc(0x4); // esp + 0x18

	*buffer2 = 0x8048468;

	strcpy(buffer1, argv[1]);
	((void (*)(void))((void*)buffer2))();
}
