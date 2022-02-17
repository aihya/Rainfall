#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char	*key = "";

void	m()
{
	int	secs;

	secs = time(NULL);
	printf("%s - %d\n", key, secs);
}

int		main(int argc, char **argv)
{
	char	*ptr_1; // ?
	char	*ptr_2; // ?

	ptr_1 = (char *)malloc(0x8);
	ptr_1[0] = 0x1;
	ptr_1[4] = (char *)malloc(0x8);

	ptr_2 = (char *)malloc(0x8);
	ptr_2[0] = 0x2;
	ptr_2[4] = (char *)malloc(0x8);

	strcpy(ptr_1 + 4, argv[1]);
	strcpy(ptr_2 + 4, argv[2]);

	fgets(buffer, 0x44, fopen("/home/user/level8/.pass", "r"));
	puts("~~");

	return (0);
}
