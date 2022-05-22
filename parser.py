import sys
import re

lines = sys.stdin.readlines()

for i in range(0, len(lines) - 1, 2):
    color_name = re.findall(r'[a-zA-Z][^A-Z]*', lines[i].strip())
    r = lines[i + 1].strip()[1:3].lower()
    g = lines[i + 1].strip()[3:5].lower()
    b = lines[i + 1].strip()[5:7].lower()
    print('inline constexpr color {} {{ 0x{}, 0x{}, 0x{} }};'.format('_'.join(color_name).lower(), r, g, b))
