#pragma once
/// \file tagged_ranged.hpp
#include <ndtree/utility/ranges.hpp>

namespace ndtree {
inline namespace v1 {
/// A tagged type with values in range [from, to)
template <class T, T from, T to, class Tag = void> struct bounded {
  T value;

  constexpr T operator*() const noexcept {
    NDTREE_ASSERT(value >= from and value < to,
                  "value is out-of-bounds [{}, {})", from, to);
    return value;
  }
  constexpr T& operator*() noexcept {
    NDTREE_ASSERT(value >= from and value < to,
                  "value is out-of-bounds [{}, {})", from, to);
    return value;
  }
  constexpr explicit operator bool() const noexcept {
    return value >= from and value < to;
  }

  CONCEPT_REQUIRES(RandomAccessIncrementable<T>{})
  static constexpr auto rng() noexcept {
    return view::iota(from, to)
           | view::transform([](T const& i) { return bounded{i}; });
  }
};
}  // namespace v1
}  // namespace ndtree
