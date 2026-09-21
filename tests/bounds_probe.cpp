#include <grevir/test/register_memory.hpp>
unsigned char bytes[8];
unsigned char* const pointer = bytes;
using Valid = setl::DebugMcuRegister<std::uint16_t, 6, 8, pointer>;
using Negative = setl::DebugMcuRegister<std::uint8_t, -1, 8, pointer>;
using CrossingEnd = setl::DebugMcuRegister<std::uint16_t, 7, 8, pointer>;
using Oversized = setl::DebugMcuRegister<std::uint64_t, 0, 4, pointer>;
template <int Id> struct Case;
template <> struct Case<0> { using Type = Valid; };
template <> struct Case<1> { using Type = Negative; };
template <> struct Case<2> { using Type = CrossingEnd; };
template <> struct Case<3> { using Type = Oversized; };
static_assert(sizeof(Case<CASE_ID>::Type) > 0);
