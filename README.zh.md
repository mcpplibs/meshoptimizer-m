# meshoptimizer

> meshoptimizer 网格优化库的 C++23 模块封装。

[English](README.md) · **简体中文** · [Upstream](https://github.com/zeux/meshoptimizer) · [Releases](https://github.com/mcpplibs/meshoptimizer-m/releases) · [mcpp](https://github.com/mcpp-community/mcpp) · [Index](https://github.com/mcpplibs/mcpp-index)

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![mcpp-community](https://img.shields.io/badge/mcpp--community-mwrap-blue)](https://github.com/mcpp-community)
[![CI](https://github.com/mcpplibs/meshoptimizer-m/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mcpplibs/meshoptimizer-m/actions/workflows/ci.yml)

上游 API 保持原样：`<meshoptimizer.h>` 里的每个 `meshopt_*` 函数、结构体、枚举和
枚举值都是上游的全局实体，头文件里按索引类型重载的 C++ 模板也在内（`unsigned short`
索引的用法与直接用头文件相同）。整个库——建索引、顶点缓存／过度绘制／顶点读取优化、
简化、meshlet、空间排序、顶点／索引／meshlet 编解码——逐字节取自 1.3 发布包并随本包
构建，不链接其他东西。

## Modules

- `import mcpplibs.mwrap.meshoptimizer;`：全部 API（118 个名字：9 个类型、20 个枚举值、89 个函数），并再导出下面这个
- `import mcpplibs.mwrap.meshoptimizer.macros;`：`mcpplibs::mwrap::meshoptimizer::macros` 里的 `MESHOPTIMIZER_VERSION`

> More Modules API -> [上游 README](upstream/meshoptimizer/README.md)，其中有每个函数的说明

## 快速开始

```sh
mcpp new lod --template mcpplibs.mwrap.meshoptimizer@1.3
cd lod
mcpp run
```

默认模板给一片四边形网格建索引、按顶点缓存重排，再简化到十分之一的三角形。

```toml
[dependencies]
mcpplibs.mwrap.meshoptimizer = "1.3"
```

```cpp
import std;
import mcpplibs.mwrap.meshoptimizer;

int main() {
    std::vector<float>        positions{0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0};
    std::vector<unsigned int> indices{0, 1, 2, 0, 2, 3};
    meshopt_optimizeVertexCache(indices.data(), indices.data(), indices.size(), positions.size() / 3);
    std::vector<unsigned int> fewer(indices.size());
    const std::size_t kept = meshopt_simplify(fewer.data(), indices.data(), indices.size(), positions.data(), positions.size() / 3, 3 * sizeof(float), 3, 0.05f);
    std::println("{} of {} indices kept", kept, indices.size());
}
```

## 宏

具名模块不能导出宏。`MESHOPTIMIZER_VERSION`（1.3 为 1030）以原名映射为
`inline constexpr int`：

```cpp
namespace macros = mcpplibs::mwrap::meshoptimizer::macros;
static_assert(macros::MESHOPTIMIZER_VERSION >= 1030);
```

头文件的其余宏（`MESHOPTIMIZER_API`、`MESHOPTIMIZER_EXPERIMENTAL`、
`MESHOPTIMIZER_ALLOC_CALLCONV`）只修饰它自己的声明，使用者不会写到。同时包含
`<meshoptimizer.h>` 的翻译单元（把 `upstream/meshoptimizer/src` 加入包含路径）看到的是
同一批实体；宏有定义时在限定名里也会展开（次序与 `push_macro` 的用法见
`tests/header_mix_test.cpp`）。

## 封装范围

- 已封装：库本身（`src/`）的每个公开名字。`meshopt_IndexAdapter` 是头文件自己模板用的辅助类型，不导出。
- 上游标为实验性的函数（`MESHOPTIMIZER_EXPERIMENTAL`）照原样导出，签名可能随上游变化。
- 不包含：gltfpack、JavaScript 与 Rust 绑定、演示程序。
- 自定义分配器（`meshopt_setAllocator`）与上游一样是进程级的。

## 验证

- `mcpp test`：建索引、顶点缓存次序、简化、meshlet 及其包围、两种编解码的往返、短索引重载、头文件混用、宏映射。
- `python3 tools/check_exports.py`：`src/meshoptimizer.cppm` 与 `tools/surface.py` 从头文件推出的内容完全一致，头文件里没有遗漏的 `meshopt_*` 名字。
- `python3 tools/test_template.py`：从已安装的副本执行 `mcpp new`，严格构建并运行生成的工程。
- CI：Linux（GCC 16、LLVM 23）、macOS 与 Windows（LLVM 23）。

## 许可

封装部分为 Apache-2.0（mcpp-community）。meshoptimizer 为 MIT 许可，© Arseny
Kapoulkine；其 README 与许可证保留在 `upstream/meshoptimizer/`，并随程序的许可文件一同部署。
来源与校验和见 `upstream/documents.json`。
