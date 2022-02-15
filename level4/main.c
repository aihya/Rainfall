#include <stdio.h>

int	some_variable = 0x0

void	p(char *buffer)
{
	printf(buffer);
}

void	n()
{
	char	buffer[520]; // ebp - 0x208

	fgets(buffer, 512, stdin);
	p(buffer);
	if (some_variable == 0x1025544)
		system("/bin/cat /home/user/level5/.pass");
}

int		main()
{
	n();
	return (0);
}
