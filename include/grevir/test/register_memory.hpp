#pragma once

#include <cstddef>
#include <type_traits>
#include <array>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>

namespace grevir::test {
enum class Kind { Read, Write, BarrierEnter, BarrierExit };
struct Event {
  Kind kind;
  std::ptrdiff_t address;
  std::size_t width;
  std::uint64_t value;
  bool operator==(const Event&) const = default;
};
template <std::size_t Capacity, typename Identity = void>
struct RegisterMemory {
  inline static std::array<unsigned char, Capacity> bytes{};
  inline static std::vector<Event> events;
  template <typename T>
  static void check(std::ptrdiff_t address) {
    static_assert(std::is_unsigned_v<T> && sizeof(T) <= sizeof(std::uint64_t));
    static_assert(sizeof(T) <= Capacity);
    if (address < 0 || std::size_t(address) > bytes.size() - sizeof(T)) {
      throw std::out_of_range("mock register address");
    }
  }
  template <typename T>
  static T read(std::ptrdiff_t address) {
    check<T>(address);
    T value{};
    std::memcpy(&value, bytes.data() + address, sizeof(T));
    events.push_back({Kind::Read, address, sizeof(T), value});
    return value;
  }
  template <typename T>
  static void write(std::ptrdiff_t address, T value) {
    check<T>(address);
    std::memcpy(bytes.data() + address, &value, sizeof(T));
    events.push_back({Kind::Write, address, sizeof(T), value});
  }
  template <typename T>
  static void modify(std::ptrdiff_t address, T value, T mask) {
    const T original = read<T>(address);
    write<T>(address, static_cast<T>((original & ~mask) | (value & mask)));
  }
  static void reset(unsigned char fill = 0) {
    bytes.fill(fill);
    events.clear();
  }
};
} // namespace grevir::test

namespace setl {
// Compatibility binding adapted from the legacy DebugMcuRegister. memcpy avoids
// packed-object aliasing and unaligned host loads; storage remains caller-owned.
template <typename T, std::ptrdiff_t Address, unsigned Size,
          unsigned char* const& Memory>
struct DebugMcuRegister {
  static constexpr std::ptrdiff_t addrx = Address;
  using nv_type = std::remove_cv_t<T>;
  static_assert(std::is_trivially_copyable_v<nv_type>);
  static_assert(Address >= 0 && sizeof(nv_type) <= Size
      && static_cast<std::size_t>(Address) <= Size - sizeof(nv_type),
    "GREVIR_DEBUG_REGISTER_OUT_OF_BOUNDS");
  static nv_type get() {
    nv_type value;
    std::memcpy(&value, Memory + Address, sizeof(value));
    return value;
  }
  static void set(nv_type value) {
    std::memcpy(Memory + Address, &value, sizeof(value));
  }
  static void set_mask(nv_type value, nv_type mask) {
    set(static_cast<nv_type>((get() & ~mask) | (value & mask)));
  }
};
} // namespace setl
