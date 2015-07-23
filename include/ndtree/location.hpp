#pragma once
/// \file location.hpp
#include <array>
#include <ndtree/types.hpp>
#include <ndtree/utility/bit.hpp>
#include <ndtree/utility/math.hpp>
#include <ndtree/utility/ranges.hpp>

namespace ndtree {
inline namespace v1 {
//

/// Location within a tree of dimension nd
///
/// The underlying storage type T determines the maximum level
/// within the tree that can be represented.
///
template <int nd, typename T = uint_t> struct location {
  std::array<T, nd> x;
  uint_t level = 0;

  static constexpr auto dimensions() noexcept {
    return view::ints(0_u, uint_t{nd});
  }

  static constexpr uint_t max_level() noexcept { return 8 * sizeof(T{}); }

  location() = default;

  location(std::array<float, nd> x_, int l) : level(l) {
    NDTREE_ASSERT(l < max_level(), "");
    constexpr uint_t scale = math::ipow(2, l);
    for (auto&& d : dimensions()) { x[d] = x_[d] * scale; }
  }

  location(std::initializer_list<uint_t> ps) {
    for (auto&& p : ps) { push(p); }
  }

  // from root:
  template <class Rng, CONCEPT_REQUIRES_(Range<Rng>())> location(Rng&& ps) {
    for (auto&& p : ps) { push(p); }
  }

  void push(uint_t pos_in_parent) {
    ++level;
    NDTREE_ASSERT(level < max_level(), "");
    for (auto&& d : dimensions()) {
      bit::set(x[d], level, bit::get(pos_in_parent, d));
    }
  }

  uint_t operator[](uint_t l) const noexcept {
    NDTREE_ASSERT(l > 0 and l <= level, "");
    uint_t value = 0;
    for (auto&& d : dimensions()) { bit::set(value, d, bit::get(x[d], l)); }
    return value;
  }

  auto operator()() const noexcept {
    return (level == 0_u ? view::iota(0_u, 0_u) : view::iota(1_u, level + 1_u))
           | view::transform([&](uint_t l) { return (*this)[l]; });
  }

  /// Reverses the bits of the location
  constexpr void reverse() noexcept {
    if (level == 0 || level == 1) { return; }
    int_t first = 1;
    int_t last = level;
    int swap_count = 0;
    while (last - first > 0) {
      for (auto&& d : dimensions()) { bit::swap(x[d], first, last); }
      ++first;
      --last;
      swap_count++;
    }
  }

  uint_t to_int(uint_t d) const noexcept {
    return bit::to_int(x[d], 1, level + 1);
  }

  uint_t to_int() const noexcept {
    auto tmp = shuffle(x, 1, level);
    return bit::to_int(tmp, 0, level * nd);
  }

  // constexpr void shift(std::array<float, nd> x_) {
  //   for (auto&& d : dimensions()) { auto i = bit::to_int(x[d], 1, level + 1);
  //   }
  // }
};

template <int nd> constexpr bool operator==(location<nd> a, location<nd> b) {
  return a.level == 0 ? a.level == b.level
                      : a.level == b.level and equal(a(), b());
}

template <int nd> constexpr bool operator!=(location<nd> a, location<nd> b) {
  return !(a == b);
}

// TODO: add operator < that lexicographically compares the coordinates for each
// spatial dimension

}  // namespace v1
}  // namespace ndtree
