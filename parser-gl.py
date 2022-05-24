from operator import concat
import sys
import re
import argparse

parser = argparse.ArgumentParser(description='GL functions lookup utility.')
parser.add_argument('infile', action='store', help='Specify input file.')
parser.add_argument('source', action='store', help='Specify source file.')
parser.add_argument('outfile', action='store', help='Specify output file.')

args = parser.parse_args()

funcs = ''
with open(args.infile, 'r') as f:
    funcs = f.readlines()

srcs = open('/usr/include/GLES2/gl2.h').read()
srcs += '\n'
srcs += open('/usr/include/GLES2/gl2ext.h').read()

declares = []
defines = []
inits = []

for line in funcs:
    line_striped = line.strip()
    if not line_striped or '//' in line_striped:
        # comments
        declares += [line_striped]
        defines += [line_striped]
        inits += [line_striped]
        continue
    func_name = line_striped
    obj = re.search('GL_APICALL[^\\(]+GL_APIENTRY\\s+{}\\s*\\([^\\)]*\\);'.format(func_name), srcs)
    if obj is None:
        raise RuntimeError('Function not found: {}'.format(func_name))
    raw_func = obj.group(0)
    declare_no_semicolon = re.sub(r'GL_APIENTRY(\s+\w+)(\s*[^;]*);', r'(GL_APIENTRYP \1)\2', raw_func)

    declare = 'extern {};'.format(declare_no_semicolon)
    define = '{} = nullptr;'.format(declare_no_semicolon)

    declares += [declare]
    defines += [define]
    inits += ['SDL_GL_InitializeFunction({});'.format(func_name)]

output_src = ''

with open(args.source, 'r') as f:
    output_src = f.read()

output_src = output_src.replace('@GL_FUNC_DECLARATIONS@', '\n'.join(declares))
output_src = output_src.replace('@GL_FUNC_DEFINITIONS@', '\n'.join(defines))
output_src = output_src.replace('@GL_FUNC_INITIALIZATIONS@', '\n'.join(inits))

with open(args.outfile, 'w') as f:
    f.write(output_src)