#include <stdio.h>

void	run()
{
	fwrite("Good... Wait what?\n", 0x13, stdout);
	system("/bin/sh");
}

int	main()
{
	char	buffer[64];

	gets(buffer);
}
