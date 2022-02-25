#include <stdio.h>

void	p(void)
{
	int		some_useless_variable;
	void	*return_addr; // Big chance the type of this variable is uint64_t and the previous variable does not exist
	char	buffer[56];
	
	fflush(stdin);
	gets(buffer);

	// 0 makes the function return the return_address of the current function.
	return_addr = __builtin_return_address(0);
	if (return_addr & 0xb0000000 != 0xb0000000)
	{
		printf("(%p)\n", return_addr);
		exit(1);
	}
	puts(buffer);
	strdup(buffer);
}

int	main()
{
	p();
	return (0);
}

