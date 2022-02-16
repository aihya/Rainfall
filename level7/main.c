#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char	*some_variable = "";

void	m()
{
	int	secs;

	secs = time(NULL);
	printf("%s - %d\n", some_variable, secs);
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

	return (0);
}
