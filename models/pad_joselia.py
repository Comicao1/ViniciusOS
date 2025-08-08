import sys
import os

def pad_file(filepath, block_size=512):
    with open(filepath, 'rb+') as f:
        data = f.read()
        original_size = len(data)
        padding_needed = (block_size - (original_size % block_size)) % block_size

        if padding_needed > 0:
            f.write(b'\x00' * padding_needed)
            print(f"Padded '{filepath}' from {original_size} to {original_size + padding_needed} bytes.")
        else:
            print(f"No padding needed for '{filepath}'. Already aligned to {block_size} bytes.")

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python pad_joselia.py <file_path> [block_size]")
        sys.exit(1)

    path = sys.argv[1]
    block = int(sys.argv[2]) if len(sys.argv) >= 3 else 512

    if not os.path.isfile(path):
        print(f"Error: File '{path}' not found.")
        sys.exit(1)

    pad_file(path, block)
