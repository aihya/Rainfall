char	*service = NULL;
char	*auth = NULL; // 0x804a008

int		main()
{
	char	buffer[128];
	while (1)
	{
		printf("%p, %p \n", auth, service);
		if (fgets(buffer, 0x80, stdin))
			break ;
		else if (strncmp(buffer, "auth ", 5) == 0)
		{
			auth = malloc(0x4);
			*auth = 0x0;
		}
		else if (strncmp(buffer, "reset", 5) == 0)
			free(auth);
		else if (strncmp(buffer, "service", 6) == 0)
			service = strdup(buffer + 7);
		else if (strncmp(buffer, "login", 5) == 0)
		{
			if (auth)
				system("/bin/sh");
			else
				fwrite("Password:\n", 0x1, 0xa, stdout);
		}
	}
	return (0);
}
