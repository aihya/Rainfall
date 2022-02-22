#include <stdio.h>

char	language = 0x0;

void	greetuser(char *main_buffer)
{
	char	buffer[76];	// $esp + 0x48 // 0x48

	if (language == 0x1)
		strcpy(buffer, "Hyvää päivää ");
	else if (language == 0x2)
		strcpy(buffer, "Goedemiddag! ");
	else
		strcpy(buffer, "Hello ");
	strcat(buffer, main_buffer);
	puts(buffer);
}

int		main(int argc, char **argv)
{
	char	buffer[73];	// $esp + 0x50
	char	lang;	// $esp + 0x9c

	if (argc != 0x3)
		return (0x1);
	bzero(buffer, 0x13); // ???

	strncpy(buffer, argv[1], 0x28);
	strncpy(buffer + 0x28, argv[2], 0x20);

	lang = getenv("LANG");
	if (lang)
	{
		if (memcmp(lang, "fi", 0x2))
			language = 0x1;
		else if (memcmp(lang, "nl", 0x2))
			language = 0x2;
	}
	strncpy(lang, buffer, 0x13);
	return (greetuser(buffer));
}
