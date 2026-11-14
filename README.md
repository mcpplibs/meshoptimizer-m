# meshoptimizer

> meshoptimizer, the mesh optimization library, through C++23 modules.

**English** · [简体中文](README.zh.md) · [Upstream](https://github.com/zeux/meshoptimizer) · [Releases](https://github.com/mcpplibs/meshoptimizer-m/releases) · [mcpp](https://github.com/mcpp-community/mcpp) · [Index](https://github.com/mcpplibs/mcpp-index)

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![mcpp-community](https://img.shields.io/badge/mcpp--community-mwrap-blue)](https://github.com/mcpp-community)
[![CI](https://github.com/mcpplibs/meshoptimizer-m/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mcpplibs/meshoptimizer-m/actions/workflows/ci.yml)

The upstream API is unchanged: every `meshopt_*` function, struct, enum and
enumerator of `<meshoptimizer.h>` is the upstream global, the header's C++
overloads over index types included (`unsigned short` indices work as they do
with the header). The whole library — indexing, vertex cache / overdraw /
vertex fetch optimization, simplification, meshlets, spatial ordering, the
vertex, index and meshlet codecs — is vendored byte for byte from the 1.3
release and built with the package; nothing else is linked.

## Modules

- `import mcpplibs.mwrap.meshoptimizer;`: the whole API (118 names: 9 types, 20 enumerators, 89 functions), re-exporting the one below
- `import mcpplibs.mwrap.meshoptimizer.macros;`: `MESHOPTIMIZER_VERSION` in `mcpplibs::mwrap::meshoptimizer::macros`

> More Modules API -> [upstream's README](upstream/meshoptimizer/README.md), which documents every function

## Getting started

```sh
mcpp new lod --template mcpplibs.mwrap.meshoptimizer@1.3
cd lod
mcpp run
```

The default template indexes a grid of quads, orders it for the vertex cache
and simplifies it to a tenth of its triangles.

```toml
[dependencies]
mcpplibs.mwrap.meshoptimizer = "1.3"
```

```cpp
import std;
import mcpplibs.mwrap.meshoptimizer;

int main() {
    std::vector<float>        positions = /* x y z … */ {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0};
    std::vector<unsigned int> indices{0, 1, 2, 0, 2, 3};
    meshopt_optimizeVertexCache(indices.data(), indices.data(), indices.size(), positions.size() / 3);
    std::vector<unsigned int> fewer(indices.size());
    const std::size_t kept = meshopt_simplify(fewer.data(), indices.data(), indices.size(), positions.data(), positions.size() / 3, 3 * sizeof(float), 3, 0.05f);
    std::println("{} of {} indices kept", kept, indices.size());
}
```

## Macros

A named module cannot export a macro. `MESHOPTIMIZER_VERSION` (1030 for 1.3) is
`inline constexpr int` under its original name:

```cpp
namespace macros = mcpplibs::mwrap::meshoptimizer::macros;
static_assert(macros::MESHOPTIMIZER_VERSION >= 1030);
```

The header's other macros (`MESHOPTIMIZER_API`, `MESHOPTIMIZER_EXPERIMENTAL`,
`MESHOPTIMIZER_ALLOC_CALLCONV`) decorate its own declarations; a consumer does
not spell them. A translation unit that also includes `<meshoptimizer.h>` (add
`upstream/meshoptimizer/src` to its include path) sees the same entities; while
the header's macro is defined it expands inside the qualified name too
(`tests/header_mix_test.cpp` shows the order and `push_macro`).

## What is and is not wrapped

- Wrapped: the library (`src/`), every public name. `meshopt_IndexAdapter`, the header's helper for its own templates, is not exported.
- The functions upstream marks experimental (`MESHOPTIMIZER_EXPERIMENTAL`) are exported as they are; their signatures may change with upstream.
- Not here: gltfpack, the JavaScript and Rust bindings, the demo.
- The custom allocator (`meshopt_setAllocator`) is process-wide, as upstream's.

## Verification

- `mcpp test`: indexing, vertex cache order, simplification, meshlets and their bounds, both codecs' round trips, the short-index overloads, header mixing, the macro mapping.
- `python3 tools/check_exports.py`: `src/meshoptimizer.cppm` is exactly what `tools/surface.py` derives from the header; no `meshopt_*` name of the header is left out.
- `python3 tools/test_template.py`: `mcpp new` from an installed copy, a strict build, and a run of the generated project.
- CI: Linux (GCC 16, LLVM 23), macOS and Windows (LLVM 23).

## License

The wrapper is Apache-2.0 (mcpp-community). meshoptimizer is MIT, © Arseny
Kapoulkine; its README and license are kept in `upstream/meshoptimizer/` and
deployed with a program's licenses. Provenance: `upstream/documents.json`.
