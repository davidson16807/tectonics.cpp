import sys
import re

'''
sample usage:

python3 replace.py beautify.tsv profiled-2A-O0.txt

remember: THIS WILL CHANGE FILES IN-PLACE!
'''

replacements_file = sys.argv[1]
target_files = sys.argv[2:]

with open(replacements_file, 'r') as file:
        text = file.read()
        replacements = [line.split('\t') for line in text.split('\n')]
        replacements = [replacement for replacement in replacements if 3>=len(replacement)>=2]

for target_file in target_files:
        with open(target_file, 'r') as file:
                text = file.read()
        for replacement in replacements:
                if len(replacement)==3 and 'regex' in replacement[2]:
                        replaced = replacement[0]
                        replacement = replacement[1]
                        text = re.sub(replaced, replacement, text)
                else:
                        replaced = replacement[0]
                        replacement = replacement[1]
                        text = text.replace(replaced, replacement)
        with open(target_file, 'w') as file:
                file.write(text)
