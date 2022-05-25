import re
import argparse
from os import path

parser = argparse.ArgumentParser(description='GL functions lookup utility.')
parser.add_argument('infile', action='store', help='Specify input file.')
parser.add_argument('source', action='store', help='Specify source file.')
parser.add_argument('outfile', action='store', help='Specify output file.')
parser.add_argument('-I', '--include', action='store', help='SDL2 include paths.')

args = parser.parse_args()

funcs = ''
with open(args.infile, 'r') as f:
    funcs = f.readlines()

include_paths = []
if args.include:
    include_paths = args.include.split()
else:
    include_paths = ['/usr/include/SDL2', '/usr/local/include/SDL2']

srcs = ''

for include_path in include_paths:
    if path.isfile(path.join(include_path, 'SDL_opengl.h')):
        with open(path.join(include_path, 'SDL_opengl.h')) as f:
            srcs += f.read()
            srcs += '\n'
    if path.isfile(path.join(include_path, 'SDL_opengl_glext.h')):
        with open(path.join(include_path, 'SDL_opengl_glext.h')) as f:
            srcs += f.read()
            srcs += '\n'

pre_include = []
post_include = []
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
    obj = re.search('GLAPI[^\\(]+GLAPIENTRY\\s+{}\\s*\\([^\\)]*\\);'.format(func_name), srcs)
    if obj is None:
        obj = re.search('GLAPI[^\\(]+APIENTRY\\s+{}\\s*\\([^\\)]*\\);'.format(func_name), srcs)
    if obj is None:
        raise RuntimeError('Function not found: {}'.format(func_name))
    raw_func = obj.group(0)
    declare_no_semicolon = re.sub(r'\s+(\w*)APIENTRY(\s+\w+)(\s*[^;]*);', r'(\1APIENTRYP \2)\3', raw_func)

    declare = 'extern {};'.format(declare_no_semicolon)
    define = '{} = nullptr;'.format(declare_no_semicolon)

    pre_include += ['#define {} deprecated_{}'.format(func_name, func_name)]
    post_include += ['#undef {}'.format(func_name)]
    declares += [declare]
    defines += [define]
    inits += ['SDL_GL_InitializeFunction({});'.format(func_name)]

output_src = ''

with open(args.source, 'r') as f:
    output_src = f.read()

output_src = output_src.replace('@GL_FUNC_PREINCLUDE@', '\n'.join(pre_include))
output_src = output_src.replace('@GL_FUNC_POSTINCLUDE@', '\n'.join(post_include))
output_src = output_src.replace('@GL_FUNC_DECLARATIONS@', '\n'.join(declares))
output_src = output_src.replace('@GL_FUNC_DEFINITIONS@', '\n'.join(defines))
output_src = output_src.replace('@GL_FUNC_INITIALIZATIONS@', '\n'.join(inits))

with open(args.outfile, 'w') as f:
    f.write(output_src)