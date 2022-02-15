#include <stdio.h>

int		some_variable = 0;

void	v()
{
	char	buffer[520]; // 0x208

	fgets(buffer, 512, stdin); // 0x200
	printf(buffer);
	if (some_variable == 64)
	{
		fwrite("Wait what?!", 1, 12, stdout);
		system("/bin/sh");
	}
}

int		main()
{
	v();
	return (0);
}
