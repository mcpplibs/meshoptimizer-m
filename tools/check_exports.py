#!/usr/bin/env python3
"""Audit the module surface against the public header.

src/meshoptimizer.cppm must be exactly what tools/surface.py writes from
upstream/meshoptimizer/src/meshoptimizer.h — every struct, class, enum,
enumerator and function — and src/macros.cppm must map MESHOPTIMIZER_VERSION,
the header's one object-like constant (its other macros are linkage and calling
convention decorations, which a module's importer never spells).
"""
from pathlib import Path
import re
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import surface  # noqa: E402


def main():
    written = (surface.ROOT / 'src/meshoptimizer.cppm').read_text(encoding='utf-8')
    assert written == surface.module_text(), 'src/meshoptimizer.cppm is not what tools/surface.py --write gives'
    header = surface.strip(surface.HEADER.read_text(encoding='utf-8'))
    exported = set(re.findall(r'^export using ::(\w+);', written, flags=re.M))
    every = set(re.findall(r'\b(meshopt_\w+)\b', header))
    # A name of the header that is not exported is a parameter, a member, or the internal adapter.
    types, enumerators, functions = surface.names(surface.HEADER.read_text(encoding='utf-8'))
    declared = set(types) | set(enumerators) | set(functions)
    assert exported == declared, sorted(exported ^ declared)
    left = every - exported - surface.INTERNAL
    assert not left, sorted(left)
    constants = set(re.findall(r'^#define\s+(MESHOPTIMIZER_\w+)\s+\d', surface.HEADER.read_text(encoding='utf-8'), flags=re.M))
    assert constants == {'MESHOPTIMIZER_VERSION'}, constants
    macros = (surface.ROOT / 'src/macros.cppm').read_text(encoding='utf-8')
    assert 'inline constexpr int MESHOPTIMIZER_VERSION' in macros
    print(f'{len(exported)} names exported: {len(types)} types, {len(enumerators)} enumerators, {len(functions)} functions; 1 constant mapped')


if __name__ == '__main__':
    main()
