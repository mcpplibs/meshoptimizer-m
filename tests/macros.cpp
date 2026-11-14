import mcpplibs.mwrap.meshoptimizer.macros;
namespace macros = mcpplibs::mwrap::meshoptimizer::macros;
static_assert(macros::MESHOPTIMIZER_VERSION == 1030);
int main() { return macros::MESHOPTIMIZER_VERSION / 1000 == 1 ? 0 : 1; }
