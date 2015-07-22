#pragma once
/// \file location.hpp
#include <array>
#include <ndtree/utility/bit.hpp>
#include <ndtree/utility/math.hpp>
#include <ndtree/utility/ranges.hpp>

namespace ndtree {
inline namespace v1 {
template <int nd, int max_level> struct location {
  std::array<int, nd> x;
  int level;

 private:
  static constexpr auto dimensions() noexcept { return view::ints(0, nd); }

  location() : x{}, level(0) {}

  location(std::array<float, nd> x_, int l = max_level) : level(l) {
    constexpr auto scale = math::ipow(2, max_level);
    for (auto d : dimensions()) { x[d] = x_[d] * scale; }
  }

  void push(int pos_in_parent) {
    ++level;
    for (auto d : dimensions()) {
      bit::set(x[d], level, bit::get(pos_in_parent, d));
    }
  }

  int operator[](int l) const noexcept {
    int value = 0;
    for (auto d : dimensions()) {
      bit::set(value, d, bit::get(x[d], nd * l + d));
    }
    return value;
  }

  /// Reverses the bits of the location
  constexpr void reverse() noexcept {
    if (level() == 0) { return; }
    auto first = 1;
    auto last = level;
    while (last - first > 1) {
      for (auto d : dimensions()) { bit::swap(x[d], first, last); }
      ++first;
      --last;
    }
  }
};

}  // namespace v1
}  // namespace ndtree
