#include <stdio.h>

void	p(char *buffer, char *header)
{
	char	puffer[0x1008];

	puts(header);
	read(0x0, puffer, 0x1000 /* 4096 bytes */);
	puffer[strchr(puffer, '\n')] = '\0';
	strncpy(buffer, puffer, 0x14);
}

void	pp(char *buffer)
{
	char	buffer1[20];
	char	buffer2[20];

	p(buffer2, " - ");
	p(buffer1, " - ");

	strcpy(buffer, buffer2);

	*(buffer + strlen(buffer)) = " ";

	strcat(buffer, buffer1);
}

int	main()
{
	char	buffer[42];

	pp(buffer);
	puts(buffer);
	return (0);
}
