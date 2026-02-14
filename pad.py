import sys
import os

SECTOR_SIZE = 512
SECTOR_COUNT = 32
BLOCK_SIZE = SECTOR_SIZE * SECTOR_COUNT  # 16384 bytes

filename = sys.argv[1]

with open(filename, "rb") as f:
    data = f.read()

pad_len = (BLOCK_SIZE - (len(data) % BLOCK_SIZE)) % BLOCK_SIZE
data += b"\x00" * pad_len

with open(filename, "wb") as f:
    f.write(data)

print(f"Padded {filename} to {len(data)} bytes (next {SECTOR_COUNT} sectors = {BLOCK_SIZE} bytes).")
