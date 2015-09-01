#pragma once
/// \file location.hpp
#include <array>
#include <ndtree/types.hpp>
#include <ndtree/relations/tree.hpp>
#include <ndtree/utility/bit.hpp>
#include <ndtree/utility/math.hpp>
#include <ndtree/utility/ranges.hpp>
#include <ndtree/utility/compact_optional.hpp>

#include <iostream>

namespace ndtree {
inline namespace v1 {
//

/// Location within a tree of dimension nd
///
/// The underlying storage type T determines the maximum level
/// within the tree that can be represented.
///
/// TODO: reconsider the storage format
/// TODO: make level a member function
/// TODO: make x and level private
/// TODO: rework optional location
/// TODO: check overflow from array of floats
///
/// Right now optimized for insertion but makes it a pain for shifting
/// coordinates and doing useful stuff with the location hashes
///
template <int nd, typename T = uint_t> struct location {
  using this_t = location<nd, T>;
  using opt_this_t = compact_optional<this_t>;
  using value_type = this_t;
  using storage_type = this_t;
  using reference_type = this_t const&;

  std::array<T, nd> x;
  uint_t level = 0;

  static constexpr auto dimensions() noexcept { return ndtree::dimensions(nd); }

  static constexpr uint_t max_level() noexcept { return 8 * sizeof(T{}); }

  constexpr auto levels() const noexcept {
    return level == 0 ? view::iota(0_u, 0_u) : view::iota(1_u, level + 1_u);
  }

  void reset_bits() noexcept {
    for (auto&& d : dimensions()) { x[d] = static_cast<T>(0); }
  }

  location() = default;

  template <typename U, CONCEPT_REQUIRES_(std::is_floating_point<U>{})>
  location(std::array<U, nd> x_, uint_t l = (max_level() - 1))
   : level(l) {
    NDTREE_ASSERT(l < max_level(), "");

    for (auto&& d : dimensions()) {
      NDTREE_ASSERT(x_[d] > 0. and x_[d] < 1., "location from non-normalized "
                                               "float (d: {}, x[d]: {}) "
                                               "out-of-range (0., 1.)",
                    d, x_[d]);
    }

    num_t scale = math::ipow(2_u, l);
    for (auto&& d : dimensions()) { from_int_r(d, x_[d] * scale); }
  }

  location(std::initializer_list<uint_t> ps) {
    reset_bits();
    for (auto&& p : ps) { push(p); }
  }

  // from root:
  template <class Rng, CONCEPT_REQUIRES_(Range<Rng>())> location(Rng&& ps) {
    reset_bits();
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
    return bit::to_int_r(x[d], static_cast<T>(1), static_cast<T>(level + 1));
  }

  void from_int_r(uint_t d, uint_t v) noexcept {
    uint_t i = 1;
    for (auto l : levels() | view::reverse) {
      bit::set(x[d], i, bit::get(v, l - 1));
      ++i;
    }
  }

  friend opt_this_t shift(this_t l, uint_t d, int_t v) noexcept {
    uint_t xi = l.to_int(d);
    if (!bit::overflows_on_add(xi, v, l.level)) {
      xi += v;
      l.x[d]
       = bit::to_int_r(xi, static_cast<T>(0), static_cast<T>(l.level + 1));
      return opt_this_t{l};
    } else {
      return opt_this_t{};
    }
  }

  static constexpr this_t empty_value() noexcept {
    this_t t;
    t.level = std::numeric_limits<uint_t>::max();
    return t;
  }
  static constexpr bool is_empty_value(this_t v) noexcept {
    return v.level == std::numeric_limits<uint_t>::max();
  }

  static constexpr value_type const& access_value(
   storage_type const& v) noexcept {
    return v;
  }
  static constexpr value_type const& store_value(value_type const& v) noexcept {
    return v;
  }
  static constexpr value_type&& store_value(value_type&& v) noexcept {
    return std::move(v);
  }
};

/// Optional location
template <int nd, typename T = uint_t>
using optional_location = typename location<nd, T>::opt_this_t;

template <int nd, typename T>
optional_location<nd, T> parent(location<nd, T> l) noexcept {
  l.level == 0 ? optional_location<nd, T>{} : optional_location<nd, T>{l.pop()};
}

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
