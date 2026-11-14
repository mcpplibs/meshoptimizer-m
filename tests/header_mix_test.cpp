// The raw header and the import in one translation unit (the header first, the
// canonical order): the module's names are the very same entities. While the
// header's macro is defined it expands even inside a qualified name, so
// `macros::MESHOPTIMIZER_VERSION` is only reachable with the macro set aside.
#include <meshoptimizer.h>
#include <gtest/gtest.h>

import std;
import mcpplibs.mwrap.meshoptimizer;

namespace {
constexpr int raw_version = MESHOPTIMIZER_VERSION;
}

#pragma push_macro("MESHOPTIMIZER_VERSION")
#undef MESHOPTIMIZER_VERSION
static_assert(mcpplibs::mwrap::meshoptimizer::macros::MESHOPTIMIZER_VERSION == raw_version);
#pragma pop_macro("MESHOPTIMIZER_VERSION")

TEST(HeaderMix, SameEntities) {
    static_assert(std::is_same_v<decltype(&::meshopt_quantizeUnorm), int (*)(float, int)>);
    static_assert(sizeof(meshopt_Meshlet) == 16);
    const std::array<unsigned int, 3> triangle{0, 1, 2};
    std::array<unsigned int, 3> out{};
    meshopt_optimizeVertexCache(out.data(), triangle.data(), triangle.size(), 3);
    EXPECT_EQ(out, triangle);
}
