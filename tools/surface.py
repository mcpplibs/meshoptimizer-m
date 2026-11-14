#!/usr/bin/env python3
"""The public names of upstream/meshoptimizer/src/meshoptimizer.h, as the module exports them.

`names(header)` lists every namespace-scope `meshopt_*` entity — functions (C
functions and the header's C++ overloads share a name, and one using-declaration
exports them all), structs, the class, enums and enumerators — except
meshopt_IndexAdapter, the header's own helper for its templates.
`python3 tools/surface.py --write` writes src/meshoptimizer.cppm from it.
"""
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]
HEADER = ROOT / 'upstream/meshoptimizer/src/meshoptimizer.h'
INTERNAL = {'meshopt_IndexAdapter'}


def strip(text: str) -> str:
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
    return re.sub(r'//[^\n]*', '', text)


def names(header: str):
    text = strip(header)
    types, enumerators, functions = [], [], []
    for kind, name in re.findall(r'^(struct|class|enum)\s+(meshopt_\w+)\s*$', text, flags=re.M):
        if name not in INTERNAL and name not in types:
            types.append(name)
    for body in re.findall(r'^enum(?:\s+meshopt_\w+)?\s*\{(.*?)\};', text, flags=re.S | re.M):
        for name in re.findall(r'\b(meshopt_\w+)\b\s*(?:=[^,}]*)?(?:,|$)', body, flags=re.M):
            if name not in enumerators:
                enumerators.append(name)
    for line in text.splitlines():
        if not line.startswith(('MESHOPTIMIZER_API', 'MESHOPTIMIZER_EXPERIMENTAL', 'inline ')):
            continue
        found = re.search(r'\b(meshopt_\w+)\s*\(', line)
        if found and found.group(1) not in functions and found.group(1) not in INTERNAL:
            functions.append(found.group(1))
    return types, enumerators, functions


def module_text() -> str:
    types, enumerators, functions = names(HEADER.read_text(encoding='utf-8'))
    out = ['// mcpplibs.mwrap.meshoptimizer — meshoptimizer, the mesh optimization library, as a C++',
           '// module. Every exported name is the upstream global from <meshoptimizer.h>, unchanged: the C',
           "// functions and the header's C++ overloads over index types under one name each.",
           '// Written by tools/surface.py from the header (tools/check_exports.py holds the two together).',
           'module;', '#include <meshoptimizer.h>', '', 'export module mcpplibs.mwrap.meshoptimizer;', '',
           'export import mcpplibs.mwrap.meshoptimizer.macros;', '', '// structs, the class, enums']
    out += [f'export using ::{name};' for name in types]
    out += ['', '// enumerators']
    out += [f'export using ::{name};' for name in enumerators]
    out += ['', '// functions']
    out += [f'export using ::{name};' for name in functions]
    return '\n'.join(out) + '\n'


if __name__ == '__main__':
    if '--write' in sys.argv:
        (ROOT / 'src/meshoptimizer.cppm').write_text(module_text(), encoding='utf-8', newline='\n')
    types, enumerators, functions = names(HEADER.read_text(encoding='utf-8'))
    print(f'{len(types)} types, {len(enumerators)} enumerators, {len(functions)} functions')
