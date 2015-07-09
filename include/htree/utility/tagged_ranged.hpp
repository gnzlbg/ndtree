#pragma once
/// \file tagged_ranged.hpp
#include <htree/utility/ranges.hpp>

namespace htree {

/// A tagged type with values in range [from, to)
template <class T, T from, T to, class Tag = void> struct tagged_ranged {
  T value;

  constexpr T operator*() const noexcept {
    assert(*this);
    return value;
  }
  constexpr T& operator*() noexcept {
    assert(*this);
    return value;
  }
  constexpr explicit operator bool() const noexcept {
    return value >= from and value < to;
  }

  static constexpr auto rng() noexcept {
    return view::iota(from, to)
           | view::transform([](T const& i) { return tagged_ranged{i}; });
  }
};

}  // namespace htree
