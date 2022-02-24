#include <stdio.h>

int main(int argc, char **argv)
{
	gid_t	egid;		// esp + 0x1c
	gid_t	euid;		// esp + 0x18
	char	*buffer;	// esp + 0x10

	if (atoi(argv[1]) == 0x1a7) // 423
	{
		buffer = strdup("/bin/sh");
		egid = getegid();
		euid = geteuid();
		setresgid(egid, egid, egid);
		setresuid(euid, euid, euid);
		execv("/bin/sh", &buffer);
	}
	else
		fwrite("No !\n", 0x1, 0x5, stderr);
	return (0);
}
