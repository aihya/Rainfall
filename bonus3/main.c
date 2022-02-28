#include <stdio.h>

int	main(int argc, char **argv)
{
	char	key[132];	// ???	esp + 0x18
	int		fd;			// 		esp + 0x9c


	fd = fopen("/home/user/end/.pass", "r");

	memset(key, 0x0, 0x21);
	if (fd == 0 || argc != 0x2)
		return (-1);

	fread(key, 0x1, 0x42, fd);
	key[0x59] = '\0';
	key[atoi(argv[1])] = '\0';

	fread(key + 0x42, 0x1, 0x41, fd);

	fclose(fd);

	if (!strcmp(key, argv[1]))
		execl("/bin/sh", "sh", NULL);
	else
		puts(key + 0x42);
	return (0);
}
