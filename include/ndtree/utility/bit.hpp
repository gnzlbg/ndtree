#pragma once
/// \file bit.hpp Bit manipulation
#include <ndtree/types.hpp>
#include <ndtree/utility/assert.hpp>
#include <ndtree/utility/ranges.hpp>
#include <ndtree/utility/math.hpp>

namespace ndtree {
inline namespace v1 {
//

/// Bit manipulation utilities
namespace bit {

/// Gets the value of the \p i-th bit of the integer \p data
template <class Int, CONCEPT_REQUIRES_(Integral<Int>{})>
constexpr bool get(Int data, uint_t i) {
  NDTREE_ASSERT(i >= 0 and i < 8 * sizeof(Int{}),
                "bit index out-of-bounds [0, {})", 8 * sizeof(Int{}));
  return data & (1 << i);
}

/// Sets the \p i-th bit of \p data to \p value
template <class Int, CONCEPT_REQUIRES_(Integral<Int>{})>
constexpr void set(Int& data, uint_t i, bool value) {
  NDTREE_ASSERT(i >= 0 and i < 8 * sizeof(Int{}),
                "bit index out-of-bounds [0, {})", 8 * sizeof(Int{}));

  if (value) {
    data |= 1 << i;
  } else {
    data &= ~(1 << i);
  }
}

template <class Int, Int max = 8 * sizeof(Int{}),
          CONCEPT_REQUIRES_(Integral<Int>{})>
constexpr auto to_int(Int data, Int from = 0, Int to = max) -> Int {
  Int value = 0;
  for (auto&& i : view::ints(from, to)) {
    auto b = get(data, i);
    value += b ? math::ipow(Int{2}, i) : Int{0};
  }
  return value;
}

template <class Int, CONCEPT_REQUIRES_(Integral<Int>{})>
constexpr void swap(Int& data, uint_t a, uint_t b) {
  bool tmp = get(data, a);
  set(data, a, get(data, b));
  set(data, b, tmp);
}

template <class Int, CONCEPT_REQUIRES_(Integral<Int>{})> auto bits() noexcept {
  return view::iota(uint_t{0}, uint_t{8 * sizeof(Int{})});
}

}  // namespace bit
}  // namespace v1
}  // namespace ndtree
