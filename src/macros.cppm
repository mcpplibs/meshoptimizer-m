// mcpplibs.mwrap.meshoptimizer.macros — <meshoptimizer.h>'s one object-like
// constant, MESHOPTIMIZER_VERSION, as `inline constexpr int` under its original
// name in mcpplibs::mwrap::meshoptimizer::macros (a named module cannot export
// a macro). The header's other macros (MESHOPTIMIZER_API,
// MESHOPTIMIZER_EXPERIMENTAL, MESHOPTIMIZER_ALLOC_CALLCONV) decorate its own
// declarations and are not spelled by a consumer.
module;
#include <meshoptimizer.h>

namespace mwrap_meshoptimizer_capture {
inline constexpr int value_MESHOPTIMIZER_VERSION = MESHOPTIMIZER_VERSION;
}  // namespace mwrap_meshoptimizer_capture

#undef MESHOPTIMIZER_VERSION

export module mcpplibs.mwrap.meshoptimizer.macros;

export namespace mcpplibs::mwrap::meshoptimizer::macros {
// major * 1000 + minor * 10 + patch
inline constexpr int MESHOPTIMIZER_VERSION = mwrap_meshoptimizer_capture::value_MESHOPTIMIZER_VERSION;
}  // namespace mcpplibs::mwrap::meshoptimizer::macros
