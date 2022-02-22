#include <stdio.h>

int	main(int argc, char **argv)
{
	char	buffer[40];
	int		val;

	val = atoi(argv[1]);
	if (val > 0x9)
		return (0x1);

	memcpy(buffer, argv[2], val * 4);

	if (val == 0x574f4c46)
		execl("/bin/sh", "sh", 0x0);

	return (0x0);
}
