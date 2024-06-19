#! /usr/bin/python3

import argparse
import shutil
from pathlib import Path

parser = argparse.ArgumentParser(description='Description of your script.')
parser.add_argument('-i', '--input_dir', type=str, help='input_dir: asm-offsets.*.s generated temporary dir')
parser.add_argument('-o', '--output', type=str, help='output: full filename of asm-offsets.s')

args = parser.parse_args()

if not args.input_dir:
	print("Error: missing input dir!")
	exit(1)

if not args.output:
	print("Error: missing output file!")
	exit(1)

root_dir = Path(args.input_dir)
pattern = f"**/asm-offsets.c.s"
matching_files = list(root_dir.glob(pattern))
# print("Find file in(" + args.input_dir + "): ", matching_files[0])
if (matching_files[0]):
    shutil.copy(matching_files[0], args.output)


# print("Input_dir: " + args.input_dir + "\nOutput: " + args.output + "\n")

# file_path = "/home/cheyh/projs/MyOS2/build/mykernel/CMakeFiles/asm-offset.dir/arch/x86_64/kbuild/asm-offsets.c.s"  # Replace with the actual path
# if os.path.exists(file_path):
#     print(f"The file '{file_path}' exists.")
# else:
#     print(f"The file '{file_path}' does not exist.")