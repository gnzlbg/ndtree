#pragma once
/// \file bit.hpp Bit manipulation utilities
#include <ndtree/types.hpp>
#include <ndtree/utility/assert.hpp>
#include <ndtree/utility/ranges.hpp>
#include <ndtree/utility/math.hpp>

namespace ndtree {
inline namespace v1 {
//

/// Bit manipulation utilities
namespace bit {

/// Bit width of type Int
template <class Int>
constexpr auto width = static_cast<Int>(CHAR_BIT * sizeof(Int{}));

/// Does the type Int have the bit \p b?
/// note: used to assert if bit is within bounds.
template <class Int, CONCEPT_REQUIRES_(Integral<Int>{})>
constexpr bool has_bit(uint_t b) noexcept {
  return b >= 0 and b < width<Int>;
}

/// Gets the value of the \p i-th bit of the integer \p x
template <class Int, CONCEPT_REQUIRES_(Integral<Int>{})>
constexpr bool get(Int x, uint_t b) {
  NDTREE_ASSERT(has_bit<Int>(b), "bit index {} out-of-bounds [0, {})", b,
                width<Int>);
  return bool(x & (Int{1} << b));
}

/// Sets the \p i-th bit of \p x to \p value
template <class Int, CONCEPT_REQUIRES_(Integral<Int>{})>
constexpr void set(Int& x, uint_t b, bool value) {
  NDTREE_ASSERT(has_bit<Int>(b), "bit index {} out-of-bounds [0, {})", b,
                width<Int>);
  if (value) {
    x |= (Int{1} << b);
  } else {
    x &= ~(Int{1} << b);
  }
}

/// Integer representation of the bit range [from, to) of x
template <class Int, Int max = width<Int>, CONCEPT_REQUIRES_(Integral<Int>{})>
constexpr auto to_int(Int x, Int from = 0, Int to = max) -> Int {
  NDTREE_ASSERT(from >= 0 and to >= from and to <= max, "");
  Int value = 0;
  Int count = 0;
  for (auto&& i : view::ints(from, to)) {
    // explanation:
    // auto b = get(x, i);
    // value += b ? math::ipow(Int{2}, count) : Int{0};
    // is equivalent to:
    value += static_cast<Int>(get(x, i)) * math::ipow(Int{2}, count);
    ++count;
  }
  return value;
}

/// Reverse integer representation of the bit range [from, to) of x
/// TODO: clean this up
template <class Int, Int max = width<Int>, CONCEPT_REQUIRES_(Integral<Int>{})>
constexpr auto to_int_r(Int x, Int from = 0, Int to = max) -> Int {
  NDTREE_ASSERT(from >= 0 and to >= from, "");
  if (from == to) { return 0; }
  Int value = 0;
  Int count = 0;
  for (Int i = to; i > from; --i) {
    auto j = i - 1;
    // explanation:
    // auto b = get(x, i);
    // value += b ? math::ipow(Int{2}, count) : Int{0};
    // is equivalent to:
    value += static_cast<Int>(get(x, j)) * math::ipow(Int{2}, count);
    ++count;
  }
  return value;
}

/// Swaps the bits \p a and \p b in \p x
template <class Int, CONCEPT_REQUIRES_(Integral<Int>{})>
constexpr void swap(Int& x, uint_t b0, uint_t b1) {
  bool tmp = get(x, b0);
  set(x, b0, get(x, b1));
  set(x, b1, tmp);
}

/// Range of bit positions for type \tparam Int
/// TODO: make this constexpr
template <class Int, CONCEPT_REQUIRES_(Integral<Int>{})> auto bits() noexcept {
  return view::iota(0_u, width<uint_t>);
}

/// Maximum representable unsigned integer value in \p no_bits
///
/// Formula: 2^{n} - 1
///
/// Since 2^{n} might not fit in the integer type it uses the following
/// relation: 2^{n} - 1 = 2^{n - 1} * 2 - 1 = 2^{n - 1} + (2^{n - 1} - 1)
///
constexpr uint_t max_value(uint_t no_bits) {
  if (no_bits == 0_u) { return 0_u; }
  const auto tmp = math::ipow(2_u, no_bits - 1);
  return tmp + (tmp - 1);
}

/// Does adding \p offset to the first \p no_bits of \p value overflows?
template <class Integer, CONCEPT_REQUIRES_(SignedIntegral<Integer>{})>
constexpr bool overflows_on_add(uint_t value, Integer offset,
                                uint_t no_bits = width<uint_t>) {
  if (offset >= int_t{0}) {
    return max_value(no_bits) - value < static_cast<uint_t>(offset);
  } else {
    return value < static_cast<uint_t>(-offset);
  }
}

/// Does adding \p offset to the first \p no_bits of \p value overflows?
template <class Integer, CONCEPT_REQUIRES_(UnsignedIntegral<Integer>{})>
constexpr bool overflows_on_add(uint_t value, Integer offset,
                                uint_t no_bits = width<uint_t>) {
  return max_value(no_bits) - value < static_cast<uint_t>(offset);
}

}  // namespace bit
}  // namespace v1
}  // namespace ndtree
