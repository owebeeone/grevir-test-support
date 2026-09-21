#include <grevir/test/register_memory.hpp>
#include <algorithm>
#include <iterator>
#include <catch2/catch_test_macros.hpp>

namespace {
using Memory = grevir::test::RegisterMemory<8>;
struct OtherIdentity;
using Other = grevir::test::RegisterMemory<8, OtherIdentity>;
unsigned char compatibility_bytes[8]{};
unsigned char* const compatibility_pointer = compatibility_bytes;
using Debug = setl::DebugMcuRegister<std::uint16_t, 1, 8, compatibility_pointer>;
}

TEST_CASE("register fixture resets and isolates stores", "[test-support]") {
  Memory::reset(0xaa);
  Other::reset(0x55);
  Memory::write<std::uint16_t>(1, 0x1234);
  REQUIRE(Memory::read<std::uint16_t>(1) == 0x1234);
  REQUIRE(Memory::bytes[0] == 0xaa);
  REQUIRE(Memory::bytes[3] == 0xaa);
  REQUIRE(Other::bytes[1] == 0x55);
  REQUIRE(Other::events.empty());
  Memory::reset();
  REQUIRE(Memory::events.empty());
  REQUIRE(Memory::bytes == std::array<unsigned char, 8>{});
}

TEST_CASE("register fixture rejects invalid accesses before touching storage", "[test-support]") {
  Memory::reset(0xaa);
  REQUIRE_THROWS_AS(Memory::read<std::uint8_t>(-1), std::out_of_range);
  REQUIRE_THROWS_AS(Memory::write<std::uint16_t>(7, 0), std::out_of_range);
  REQUIRE_THROWS_AS(Memory::modify<std::uint8_t>(8, 0, 1), std::out_of_range);
  REQUIRE(Memory::events.empty());
  REQUIRE(Memory::bytes[7] == 0xaa);
  Memory::write<std::uint8_t>(7, 0xf0);
  Memory::modify<std::uint8_t>(7, 0xff, 0x03);
  REQUIRE(Memory::bytes[7] == 0xf3);
}

TEST_CASE("legacy debug accessor uses safe unaligned copies and masks writes", "[test-support]") {
  std::fill(std::begin(compatibility_bytes), std::end(compatibility_bytes), 0xaa);
  Debug::set(0x1234);
  REQUIRE(Debug::get() == 0x1234);
  Debug::set_mask(0xffff, 0x000f);
  REQUIRE(Debug::get() == 0x123f);
  REQUIRE(compatibility_bytes[0] == 0xaa);
  REQUIRE(compatibility_bytes[3] == 0xaa);
}
