#! /usr/bin/python3

import argparse
import re

parser = argparse.ArgumentParser(description='Description of your script.')
parser.add_argument('-i', '--input', type=str, help='input: full filename of asm-offsets.s')
parser.add_argument('-o', '--output', type=str, help='output: full filename of asm-offsets.h')

args = parser.parse_args()

if not args.input:
	print("Error: missing input file!")
	exit(1)

if not args.output:
	print("Error: missing output file!")
	exit(1)



output_content = '''
#ifndef __ASM_OFFSETS_H__
#define __ASM_OFFSETS_H__

'''

pattern = r"[ ]"
with open(args.input, "r") as input_file:
	for input_line in input_file:
		matches = re.findall(r'"->([^"]*)"', input_line)
		if (matches):
			fields = re.split(pattern, matches[0])
			output_line: str = ""
			if (fields[0]):
				output_line += fields[0]
				if (fields[1]):
					output_line += "#define " + fields[1]
			output_content += "\t" + output_line + "\n"
	input_file.close()


output_content += '''
#endif
'''

print(output_content)
with open(args.output, "w") as output_file:
	output_file.write(output_content)
	output_file.flush()
	output_file.close()